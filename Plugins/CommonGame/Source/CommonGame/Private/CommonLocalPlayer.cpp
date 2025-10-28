// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLocalPlayer.h"

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController( GetWorld() );
	APlayerState* PlayerState = PC->PlayerState;
	if ( PlayerState != nullptr )
	{
		Delegate.Execute( this, PlayerState );
	}

	return OnPlayerStateSet.Add( Delegate );
}
