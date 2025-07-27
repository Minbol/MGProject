// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameFeatureAction_AddInputContextMapping.h"

#include "EnhancedInputSubsystems.h"
#include "GameFeaturesSubsystem.h"
#include "Engine/World.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"
#include "Components/GameFrameworkComponentManager.h"
#include "MGP/Character/MGPHeroComponent.h"
#include "MGP/System/MGPAssetManager.h"

void UMGPGameFeatureAction_AddInputContextMapping::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();

	RegisterInputMappingContexts();
}

void UMGPGameFeatureAction_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	Super::OnGameFeatureActivating( Context );

	FPerContextData& ActiveData = ContextData.FindOrAdd( Context );
	if ( !ActiveData.ExtensionRequestHandles.IsEmpty() ||
		!ActiveData.ControllersAddedTo.IsEmpty() )
	{
		Reset( ActiveData );
	}
	Super::OnGameFeatureActivating( Context );
}

void UMGPGameFeatureAction_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating( Context );

	FPerContextData* ActiveData = ContextData.Find(Context);
	if ( ensure( ActiveData ) )
	{
		Reset( *ActiveData );
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::OnGameFeatureUnregistering()
{
	Super::OnGameFeatureUnregistering();

	UnregisterInputMappingContexts();
}

void UMGPGameFeatureAction_AddInputContextMapping::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ( ( GameInstance != nullptr ) && ( World != nullptr ) && World->IsGameWorld() )
	{
		if ( UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>( GameInstance ) )
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject( this, &ThisClass::HandleControllerExtension, ChangeContext );
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
				ComponentManager->AddExtensionHandler( APlayerController::StaticClass(), AddAbilitiesDelegate );

			ActiveData.ExtensionRequestHandles.Add( ExtensionRequestHandle );
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts()
{
	RegisterInputContextMappingsForGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject( this, &UMGPGameFeatureAction_AddInputContextMapping::RegisterInputContextMappingsForGameInstance );

	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for ( TIndirectArray<FWorldContext>::TConstIterator WorldContextIterator = WorldContexts.CreateConstIterator(); WorldContextIterator; ++WorldContextIterator )
	{
		RegisterInputContextMappingsForGameInstance( WorldContextIterator->OwningGameInstance );
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::RegisterInputContextMappingsForGameInstance(UGameInstance* InGameInstance)
{
	if ( InGameInstance != nullptr && !InGameInstance->OnLocalPlayerAddedEvent.IsBoundToObject( this ) )
	{
		InGameInstance->OnLocalPlayerAddedEvent.AddUObject( this, &UMGPGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextsForLocalPlayer );
		InGameInstance->OnLocalPlayerRemovedEvent.AddUObject( this, &UMGPGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer );
		
		for ( TArray<ULocalPlayer*>::TConstIterator LocalPlayerIterator = InGameInstance->GetLocalPlayerIterator(); LocalPlayerIterator; ++LocalPlayerIterator )
		{
			RegisterInputMappingContextsForLocalPlayer(*LocalPlayerIterator);
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* InLocalPlayer)
{
	if ( ensure( InLocalPlayer ) )
	{
		UMGPAssetManager& AssetManager = UMGPAssetManager::Get();
		
		if ( const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( InLocalPlayer ) )
		{
			if ( UEnhancedInputUserSettings* Settings = EISubsystem->GetUserSettings() )
			{
				for ( const FInputMappingContextAndPriority& Entry : InputMappings )
				{
					if (!Entry.bRegisterWithSettings)
					{
						continue;
					}

					if ( const UInputMappingContext* IMC = AssetManager.GetAsset( Entry.InputMapping ) )
					{
						Settings->RegisterInputMappingContext( IMC );
					}
				}
			}
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts()
{
	FWorldDelegates::OnStartGameInstance.Remove( RegisterInputContextMappingsForGameInstanceHandle );
	RegisterInputContextMappingsForGameInstanceHandle.Reset();

	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for ( TIndirectArray<FWorldContext>::TConstIterator WorldContextIterator = WorldContexts.CreateConstIterator(); WorldContextIterator; ++WorldContextIterator )
	{
		UnregisterInputContextMappingsForGameInstance( WorldContextIterator->OwningGameInstance );
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::UnregisterInputContextMappingsForGameInstance(UGameInstance* InGameInstance)
{
	if ( InGameInstance != nullptr )
	{
		InGameInstance->OnLocalPlayerAddedEvent.RemoveAll( this );
		InGameInstance->OnLocalPlayerRemovedEvent.RemoveAll( this );

		for ( TArray<ULocalPlayer*>::TConstIterator LocalPlayerIterator = InGameInstance->GetLocalPlayerIterator(); LocalPlayerIterator; ++LocalPlayerIterator )
		{
			UnregisterInputMappingContextsForLocalPlayer( *LocalPlayerIterator );
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer)
{
	if ( ensure( LocalPlayer ) )
	{
		if ( const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer ) )
		{
			if ( UEnhancedInputUserSettings* Settings = EISubsystem->GetUserSettings() )
			{
				for ( const FInputMappingContextAndPriority& Entry : InputMappings )
				{
					if ( !Entry.bRegisterWithSettings )
					{
						continue;
					}
					
					if ( const UInputMappingContext* IMC = Entry.InputMapping.Get() )
					{
						Settings->UnregisterInputMappingContext( IMC );
					}
				}
			}
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while ( !ActiveData.ControllersAddedTo.IsEmpty() )
	{
		TWeakObjectPtr<APlayerController> ControllerPtr = ActiveData.ControllersAddedTo.Top();
		if ( ControllerPtr.IsValid() )
		{
			RemoveInputMapping( ControllerPtr.Get(), ActiveData );
		}
		else
		{
			ActiveData.ControllersAddedTo.Pop();
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::HandleControllerExtension(AActor* InActor, FName InEventName, FGameFeatureStateChangeContext InChangeContext)
{
	APlayerController* AsController = CastChecked<APlayerController>( InActor );
	FPerContextData& ActiveData = ContextData.FindOrAdd( InChangeContext );

	if ( ( InEventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ) || ( InEventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved ) )
	{
		RemoveInputMapping( AsController, ActiveData );
	}
	else if ( ( InEventName == UGameFrameworkComponentManager::NAME_ExtensionAdded ) || ( InEventName == UMGPHeroComponent::NAME_BindInputsNow ) )
	{
		AddInputMappingForPlayer( AsController->GetLocalPlayer(), ActiveData );
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::AddInputMappingForPlayer(UPlayer* InPlayer, FPerContextData& InActiveData)
{
	if ( const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( InPlayer ) )
	{
		if ( UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
		{
			for ( const FInputMappingContextAndPriority& Entry : InputMappings )
			{
				if ( const UInputMappingContext* IMC = Entry.InputMapping.Get() )
				{
					InputSystem->AddMappingContext(IMC, Entry.Priority);
				}
			}
		}
	}
}

void UMGPGameFeatureAction_AddInputContextMapping::RemoveInputMapping(APlayerController* InPlayerController, FPerContextData& InActiveData)
{
	if ( ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer() )
	{
		if ( UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
		{
			for ( const FInputMappingContextAndPriority& Entry : InputMappings )
			{
				if ( const UInputMappingContext* IMC = Entry.InputMapping.Get() )
				{
					InputSystem->RemoveMappingContext( IMC );
				}
			}
		}
	}

	InActiveData.ControllersAddedTo.Remove( InPlayerController );
}
