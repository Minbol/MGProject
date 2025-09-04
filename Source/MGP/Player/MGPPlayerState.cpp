// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerState.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "MGP/AbilitySystem/MGPAbilitySet.h"
#include "MGP/AbilitySystem/MGPAbilitySystemComponent.h"
#include "MGP/GameMode/MGPExperienceManagerComponent.h"
#include "MGP/GameMode/MGPGameModeBase.h"

AMGPPlayerState::AMGPPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMGPAbilitySystemComponent>( this, TEXT( "AbilitySystemComponent" ) );
}

void AMGPPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if ( IsValid( AbilitySystemComponent ) )
	{
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();

		if ( ActorInfo->OwnerActor != this && ActorInfo->OwnerActor != ActorInfo->AvatarActor )
		{
			AbilitySystemComponent->InitAbilityActorInfo( this, GetPawn() );
		}
	}

	const AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if ( !IsValid( GameStateBase ) )
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

	for ( UMGPAbilitySet* AbilitySet : PawnData->AbilitySets )
	{
		if ( IsValid( AbilitySet ) )
		{
			AbilitySet->GiveToAbilitySystem( AbilitySystemComponent, nullptr );
		}
	}
}
