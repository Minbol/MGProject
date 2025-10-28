// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionPointWidget.h"

#include "CommonLocalPlayer.h"
#include "GameFramework/PlayerState.h"

#define LOCTEXT_NAMESPACE "UIExtension"

UUIExtensionPointWidget::UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UUIExtensionPointWidget::RebuildWidget()
{
	if ( !IsDesignTime() && ExtensionPointTag.IsValid() )
	{
		ResetExtensionPoint();
		RegisterExtensionPoint();

		FDelegateHandle Handle = GetOwningLocalPlayer<UCommonLocalPlayer>()->CallAndRegister_OnPlayerStateSet(
			UCommonLocalPlayer::FPlayerStateSetDelegate::FDelegate::CreateUObject( this, &ThisClass::RegisterExtensionPointForPlayerState ) );
	}

	if ( IsDesignTime() )
	{
		auto GetExtensionPointText = [this]()
		{
			return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "ExtensionPoint\n{0}"), FText::FromName(ExtensionPointTag.GetTagName()));
		};

		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);
		MessageBox->AddSlot()
			.Padding(0.5f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text_Lambda(GetExtensionPointText)
			];
		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}

void UUIExtensionPointWidget::ResetExtensionPoint()
{
	ResetInternal();
	ExtensionMapping.Reset();
	
	for ( auto& Handle : ExtensionPointHandles )
	{
		Handle.Unregister();
	}

	ExtensionPointHandles.Reset();
}

void UUIExtensionPointWidget::OnAddOrRemoveExtension(EUIExtensionAction InAction, const FUIExtensionRequest& InRequest)
{
	switch ( InAction )
	{
	case EUIExtensionAction::Added:
		{
			UObject* Data = InRequest.Data;
			TSubclassOf<UUserWidget> WidgetClass(Cast<UClass>( Data ) );
			if ( WidgetClass )
			{
				UUserWidget* Widget = CreateEntryInternal( WidgetClass );
				ExtensionMapping.Add( InRequest.ExtensionHandle, Widget );
			}
		}
	break;
	case EUIExtensionAction::Removed:
		{
			if ( UUserWidget* Extension = ExtensionMapping.FindRef( InRequest.ExtensionHandle ) )
			{
				RemoveEntryInternal( Extension );
				ExtensionMapping.Remove( InRequest.ExtensionHandle );
			}
		}
	break;
		default: break;
	}
}

void UUIExtensionPointWidget::RegisterExtensionPoint()
{
	if ( UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>() )
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add( UUserWidget::StaticClass() );
		AllowedDataClasses.Append( DataClass );
		
		ExtensionPointHandles.Add( ExtensionSubsystem->RegisterExtensionPoint(
			ExtensionPointTag,
			ExtensionPointTagMatch,
			AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject( this, &ThisClass::OnAddOrRemoveExtension ) ) );
		
		ExtensionPointHandles.Add( ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag,
			GetOwningLocalPlayer(),
			ExtensionPointTagMatch,
			AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject( this, &ThisClass::OnAddOrRemoveExtension ) ) );
	}
}

void UUIExtensionPointWidget::RegisterExtensionPointForPlayerState(UCommonLocalPlayer* InLocalPlayer, APlayerState* InPlayerState)
{
	if ( UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>() )
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add( UUserWidget::StaticClass() );
		AllowedDataClasses.Append( DataClass );
		
		ExtensionPointHandles.Add( ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag,
			InPlayerState,
			ExtensionPointTagMatch,
			AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject( this, &ThisClass::OnAddOrRemoveExtension )
		) );
	}
}
