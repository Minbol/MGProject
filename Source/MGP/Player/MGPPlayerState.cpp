// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerState.h"

#include "MGP/GameMode/MGPExperienceManagerComponent.h"
#include "MGP/GameMode/MGPGameModeBase.h"

void AMGPPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if ( GameStateBase == nullptr )
		return;

	const TObjectPtr<UMGPExperienceManagerComponent> ExperienceManagerComponent = GameStateBase->FindComponentByClass<UMGPExperienceManagerComponent>();
	if ( !IsValid( ExperienceManagerComponent ) )
		return;

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded( FOnExperienceLoaded::FDelegate::CreateUObject( this, &AMGPPlayerState::OnExperienceLoaded ) );
}

void AMGPPlayerState::OnExperienceLoaded(const UMGPExperienceDef* InCurrentExperience)
{
	const TObjectPtr<AMGPGameModeBase> GameMode = GetWorld()->GetAuthGameMode<AMGPGameModeBase>();
	if( !IsValid( GameMode ) )
		return;

	const UMGPPawnData* NewPawnData = GameMode->GetPawnDataForController( GetOwningController() );
	check( NewPawnData );
	
	SetPawnData( NewPawnData );
}

void AMGPPlayerState::SetPawnData(const UMGPPawnData* InPawnData)
{
	check( InPawnData );
	check( PawnData == nullptr )

	PawnData = InPawnData;
}
