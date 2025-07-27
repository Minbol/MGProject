// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPExperienceManagerComponent.h"

#include "MGPExperienceDef.h"
#include "MGP/System/MGPAssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "MGPExperienceActionSet.h"

void UMGPExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& InDelegate)
{
	if ( IsExperienceLoaded() )
	{
		InDelegate.Execute( CurrentExperience );
	}
	else
	{
		OnExperienceLoaded.Add( MoveTemp( InDelegate ) );
	}
}

void UMGPExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& InExperienceId)
{
	const UMGPAssetManager& AssetManager = UMGPAssetManager::Get();

	TSubclassOf<UMGPExperienceDef> AssetClass;
	{
		const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath( InExperienceId );
		AssetClass = Cast<UClass>( AssetPath.TryLoad( ) );
	}

	const UMGPExperienceDef* Experience = GetDefault<UMGPExperienceDef>( AssetClass );
	check( Experience != nullptr );
	check( CurrentExperience == nullptr );

	CurrentExperience = Experience;

	StartExperienceLoad();
}

void UMGPExperienceManagerComponent::StartExperienceLoad()
{
	check( CurrentExperience );
	check( LoadState == EMGPExperienceLoadState::Unloaded );

	LoadState = EMGPExperienceLoadState::Loading;
	UMGPAssetManager& AssetManager = UMGPAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Emplace( CurrentExperience->GetPrimaryAssetId() );

	TArray<FName> BundleToLoad;
	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = OwnerNetMode != NM_DedicatedServer;
	const bool bLoadServer = OwnerNetMode != NM_Client;

	if ( bLoadClient )
	{
		BundleToLoad.Emplace( UGameFeaturesSubsystemSettings::LoadStateClient );
	}
	if ( bLoadServer )
	{
		BundleToLoad.Emplace( UGameFeaturesSubsystemSettings::LoadStateServer );
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject( this, &ThisClass::OnExperienceLoadComplete );
	TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
	if ( !BundleAssetList.IsEmpty() )
	{
		BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundleToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	}

	if ( BundleLoadHandle.IsValid() || BundleLoadHandle->HasLoadCompleted() )
	{
		FStreamableHandle::ExecuteDelegate( OnAssetsLoadedDelegate );
	}
	else
	{
		BundleLoadHandle->BindCompleteDelegate( OnAssetsLoadedDelegate );
		BundleLoadHandle->BindCancelDelegate( FStreamableDelegate::CreateLambda( [ OnAssetsLoadedDelegate ]()
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		} ) );
	}
}

void UMGPExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check( LoadState == EMGPExperienceLoadState::Loading );
	check( CurrentExperience );

	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [ This = this ]( const UPrimaryDataAsset* InContext, const TArray<FString>& InFeaturePluginList )
	{
		for ( const FString& PluginName : InFeaturePluginList )
		{
			FString PluginURL;
			if( UGameFeaturesSubsystem::Get().GetPluginURLByName( PluginName, PluginURL ) )
			{
				This->GameFeaturePluginURLs.AddUnique( PluginURL );
			}
		}
	};

	CollectGameFeaturePluginURLs( CurrentExperience, CurrentExperience->GetGameFeaturesToEnable() );
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if ( 0 < NumGameFeaturePluginsLoading )
	{
		LoadState = EMGPExperienceLoadState::LoadingGameFeatures;
		for ( const FString& PluginURL : GameFeaturePluginURLs )
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin( PluginURL, FGameFeaturePluginLoadComplete::CreateUObject( this, &ThisClass :: OnGameFeaturePluginLoadComplete) );
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void UMGPExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& InResult)
{
	NumGameFeaturePluginsLoading--;
	if ( NumGameFeaturePluginsLoading == 0 )
	{
		OnExperienceFullLoadCompleted();
	}
}

void UMGPExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check( LoadState != EMGPExperienceLoadState::Loaded )

	LoadState = EMGPExperienceLoadState::ExecutingActions;
	FGameFeatureActivatingContext Context;
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld( GetWorld() );
	if ( ExistingWorldContext != nullptr )
	{
		Context.SetRequiredWorldContextHandle( ExistingWorldContext->ContextHandle );
	}

	auto ActivateListOfActions = [ &Context ]( const TArray<UGameFeatureAction*>& InActionList )
	{
		for ( UGameFeatureAction* Action : InActionList )
		{
			if ( Action != nullptr )
			{
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating( Context );
			}
		}
	};

	ActivateListOfActions( CurrentExperience->Actions );
	for ( const TObjectPtr<UMGPExperienceActionSet>& ActionSet : CurrentExperience->ActionsSets )
	{
		ActivateListOfActions( ActionSet->Actions );
	}

	LoadState = EMGPExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast( CurrentExperience );
	OnExperienceLoaded.Clear();
}

const UMGPExperienceDef* UMGPExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check( LoadState == EMGPExperienceLoadState::Loaded );
	check( CurrentExperience != nullptr );

	return CurrentExperience;
}
