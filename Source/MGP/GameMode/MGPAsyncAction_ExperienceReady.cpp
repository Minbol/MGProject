// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPAsyncAction_ExperienceReady.h"

#include "MGPExperienceManagerComponent.h"

UMGPAsyncAction_ExperienceReady::UMGPAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMGPAsyncAction_ExperienceReady* UMGPAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* InWorldContextObject)
{
	UMGPAsyncAction_ExperienceReady* Action = nullptr;
	if ( UWorld* World = GEngine->GetWorldFromContextObject( InWorldContextObject, EGetWorldErrorMode::LogAndReturnNull ) )
	{
		Action = NewObject<UMGPAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance( World );
	}

	return Action;
}

void UMGPAsyncAction_ExperienceReady::Activate()
{
	if ( UWorld* World = WorldPtr.Get() )
	{
		if ( AGameStateBase* GameState = World->GetGameState() )
		{
			ListenToExperienceLoading( GameState );
		}
		else
		{
			World->GameStateSetEvent.AddUObject( this, &ThisClass::HandleGameStateSet );
		}
	}
	else
	{
		SetReadyToDestroy();
	}
}

void UMGPAsyncAction_ExperienceReady::HandleGameStateSet(AGameStateBase* InGameState)
{
	if ( UWorld* World = WorldPtr.Get() )
	{
		World->GameStateSetEvent.RemoveAll( this );
	}

	ListenToExperienceLoading( InGameState );
}

void UMGPAsyncAction_ExperienceReady::ListenToExperienceLoading(AGameStateBase* InGameState)
{
	check( InGameState );
	UMGPExperienceManagerComponent* ExperienceManagerComponent = InGameState->FindComponentByClass<UMGPExperienceManagerComponent>();
	check( ExperienceManagerComponent );

	if ( ExperienceManagerComponent->IsExperienceLoaded() )
	{
		UWorld* World = InGameState->GetWorld();
		check( World );

		World->GetTimerManager().SetTimerForNextTick( FTimerDelegate::CreateUObject( this, &ThisClass::BroadcastReady ) );
	}
	else
	{
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded( FOnExperienceLoaded::FDelegate::CreateUObject( this, &ThisClass::HandleExperienceLoaded ) );
	}
}

void UMGPAsyncAction_ExperienceReady::HandleExperienceLoaded(const UMGPExperienceDef* InCurrentExperience)
{
	BroadcastReady();
}

void UMGPAsyncAction_ExperienceReady::BroadcastReady()
{
	OnReady.Broadcast();
	SetReadyToDestroy();
}
