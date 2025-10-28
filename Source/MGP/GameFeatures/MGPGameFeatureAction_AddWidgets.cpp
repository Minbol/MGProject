// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameFeatureAction_AddWidgets.h"

#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "Components/GameFrameworkComponentManager.h"
#include "MGP/UI/MGPHUD.h"


void UMGPGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
}

void UMGPGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd( ChangeContext );

	if ( ( GameInstance != nullptr ) && ( World != nullptr ) && World->IsGameWorld() )
	{
		if ( UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>( GameInstance ) )
		{
			const TSoftClassPtr<AActor> HudActorClass = AMGPHUD::StaticClass();
			const TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HudActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject( this, &ThisClass::HandleActorExtension, ChangeContext ) );
			
			ActiveData.ComponentRequests.Add( ExtensionRequestHandle );
		}
	}
	
}

void UMGPGameFeatureAction_AddWidgets::HandleActorExtension(AActor* InActor, FName InEventName, FGameFeatureStateChangeContext InChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd( InChangeContext );

	if ( InEventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || InEventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved )
	{
		RemoveWidgets( InActor, ActiveData );
	}
	else if ( InEventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || InEventName == UGameFrameworkComponentManager::NAME_GameActorReady )
	{
		AddWidgets( InActor, ActiveData );
	}
}

void UMGPGameFeatureAction_AddWidgets::AddWidgets(AActor* InActor, FPerContextData& InActiveData)
{
	AMGPHUD* HUD = CastChecked<AMGPHUD>( InActor );
	if ( ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( HUD->GetOwningPlayerController()->Player ) )
	{
		for ( const auto& Entry : Layout )
		{
			if ( TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get() )
			{
				InActiveData.LayoutAdded.Add( UCommonUIExtensions::PushContentToLayer_ForPlayer( LocalPlayer, Entry.LayerID, ConcreteWidgetClass ) );
			}
		}

		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for ( const auto& Entry : Widgets )
		{
			InActiveData.ExtensionHandles.Add( ExtensionSubsystem->RegisterExtensionAsWidgetForContext( Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1 ) );
		}
	}
}

void UMGPGameFeatureAction_AddWidgets::RemoveWidgets(AActor* InActor, FPerContextData& InActiveData)
{
	AMGPHUD* HUD = CastChecked<AMGPHUD>( InActor );
	for ( const auto& AddedLayout : InActiveData.LayoutAdded )
	{
		if ( AddedLayout.IsValid() )
		{
			AddedLayout->DeactivateWidget();
		}
	}

	InActiveData.LayoutAdded.Reset();

	for ( auto& Handle : InActiveData.ExtensionHandles )
	{
		Handle.Unregister();
	}

	InActiveData.ExtensionHandles.Reset();
}
