// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameInstance.h"

#include "CommonLocalPlayer.h"
#include "GameUIMangerSubsystem.h"

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	const int32 Value = Super::AddLocalPlayer(NewPlayer, UserId);
	if ( Value != INDEX_NONE )
	{
		if ( !PrimaryPlayer.IsValid() )
		{
			PrimaryPlayer = NewPlayer;
		}

		GetSubsystem<UGameUIMangerSubsystem>()->NotifyPlayerAdded( Cast<UCommonLocalPlayer>( NewPlayer ) );
	}
	
	return Value;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if ( PrimaryPlayer == ExistingPlayer )
	{
		PrimaryPlayer.Reset();
	}
	
	GetSubsystem<UGameUIMangerSubsystem>()->NotifyPlayerDestroyed( Cast<UCommonLocalPlayer>( ExistingPlayer ) );
	return Super::RemoveLocalPlayer(ExistingPlayer);
}
