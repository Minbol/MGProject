// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPawnExtensionComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "MGP/MGPGameplayTags.h"

const FName UMGPPawnExtensionComponent::NAME_ActorFeatureName( "PawnExtension" );

UMGPPawnExtensionComponent::UMGPPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick		   = false;
}

void UMGPPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	if ( !GetPawn<APawn>() )
		return;

	RegisterInitStateFeature();
	UGameFrameworkComponentManager* GameFrameworkComponentManager = UGameFrameworkComponentManager::GetForActor( GetOwningActor() );
}

void UMGPPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged( NAME_None, FGameplayTag(), false );
	ensure( TryToChangeInitState( FMGPGameplayTags::Get().InitState_Spawned ) );
	CheckDefaultInitialization();
}

void UMGPPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UMGPPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if ( Params.FeatureName != NAME_ActorFeatureName )
	{
		const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
		if ( Params.FeatureState == InitTags.InitState_DataAvailable )
		{
			CheckDefaultInitialization();
		}
	}
}

bool UMGPPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check( Manager );

	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();

	if ( !CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned )
	{
		if ( Pawn != nullptr )
		{
			return true;
		}
	}
	
	if ( CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable )
	{
		if ( !PawnData )
		{
			return false;
		}
			

		if ( Pawn->IsPlayerControlled() )
		{
			if ( !GetController<AController>() )
			{
				return false;
			}
		}
		
		return true;
	}

	if ( CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized )
	{
		return Manager->HaveAllFeaturesReachedInitState( Pawn, InitTags.InitState_DataAvailable );
	}

	if ( CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady )
	{
		return true;
	}

	return false;
}

void UMGPPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();
	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	ContinueInitStateChain( StateChain );
}

void UMGPPawnExtensionComponent::SetPawnData(const UMGPPawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if ( Pawn->GetLocalRole() != ROLE_Authority )
	{
		return;
	}

	if ( PawnData != nullptr )
	{
		return;
	}

	PawnData = InPawnData;
}

void UMGPPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}
