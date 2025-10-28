// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonUIExtensions.h"

#include "CommonLocalPlayer.h"
#include "GameUIMangerSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"

UCommonUIExtensions::UCommonUIExtensions()
{
}

UCommonActivatableWidget* UCommonUIExtensions::PushContentToLayer_ForPlayer(ULocalPlayer* InLocalPlayer, FGameplayTag InLayerName, TSubclassOf<UCommonActivatableWidget> InClass)
{
	if ( UGameUIMangerSubsystem* UIManager = InLocalPlayer->GetGameInstance()->GetSubsystem<UGameUIMangerSubsystem>() )
	{
		if ( UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy() )
		{
			if ( UPrimaryGameLayout* RootLayout = Policy->GetRootLayout( CastChecked<UCommonLocalPlayer>( InLocalPlayer ) ) )
			{
				return RootLayout->PushWidgetToLayerStack( InLayerName, InClass );
			}
		}
	}
	
	return nullptr;
}
