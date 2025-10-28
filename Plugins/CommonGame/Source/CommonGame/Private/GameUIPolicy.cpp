// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIPolicy.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* InLocalPlayer)
{
	InLocalPlayer->OnPlayerControllerSet.AddWeakLambda( this, [this](UCommonLocalPlayer* InLocalPlayer, APlayerController* InPlayerController)
	{
		NotifyPlayerRemoved( InLocalPlayer );
		if ( FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey( InLocalPlayer ) )
		{
			AddLayoutFromViewport( InLocalPlayer, LayoutInfo->RootLayout );
			LayoutInfo->bAddedToViewport = true;
		}
		else
		{
			CreateLayoutWidget( InLocalPlayer );
		}
	});

	if ( FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey( InLocalPlayer ) )
	{
		AddLayoutFromViewport( InLocalPlayer, LayoutInfo->RootLayout );
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget( InLocalPlayer );
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* InLocalPlayer)
{
	if ( FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey( InLocalPlayer ) )
	{
		RemoveLayoutFromViewport( InLocalPlayer, LayoutInfo->RootLayout );
		LayoutInfo->bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* InLocalPlayer)
{
	NotifyPlayerRemoved( InLocalPlayer );
	InLocalPlayer->OnPlayerControllerSet.RemoveAll( this );

	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey( InLocalPlayer );
	if ( LayoutInfoIdx != INDEX_NONE )
	{
		UPrimaryGameLayout* Layout = RootViewportLayouts[ LayoutInfoIdx ].RootLayout;
		RootViewportLayouts.RemoveAt( LayoutInfoIdx );
		RemoveLayoutFromViewport( InLocalPlayer, Layout );
	}
}

void UGameUIPolicy::AddLayoutFromViewport(UCommonLocalPlayer* InLocalPlayer, UPrimaryGameLayout* InLayout)
{
	InLayout->SetPlayerContext(FLocalPlayerContext( InLocalPlayer ) );
	InLayout->AddToPlayerScreen( 1000 );
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* InLocalPlayer, UPrimaryGameLayout* InLayout)
{
	const TWeakPtr<SWidget> LayoutSlateWidget = InLayout->GetCachedWidget();
	if ( LayoutSlateWidget.IsValid() )
	{
		InLayout->RemoveFromParent();
	}
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* InLocalPlayer)
{
	if ( APlayerController* PlayerController = InLocalPlayer->GetPlayerController( GetWorld() ) )
	{
		const TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass( InLocalPlayer );
		if ( LayoutWidgetClass != nullptr && !LayoutWidgetClass->HasAnyClassFlags( CLASS_Abstract ) )
		{
			UPrimaryGameLayout* NewLayout = CreateWidget<UPrimaryGameLayout>( PlayerController, LayoutWidgetClass );
			RootViewportLayouts.Emplace( InLocalPlayer, NewLayout, true );

			AddLayoutFromViewport( InLocalPlayer, NewLayout );
		}
	}
}

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(UCommonLocalPlayer* InLocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey( InLocalPlayer );
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* InLocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}
