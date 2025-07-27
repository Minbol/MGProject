// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameModeBase.h"

#include "MGPExperienceManagerComponent.h"
#include "MGPGameState.h"
#include "Kismet/GameplayStatics.h"
#include "MGP/Character/MGPPawnExtensionComponent.h"
#include "MGP/Character/MGPPlayerCharacter.h"
#include "MGP/Player/MGPPlayerState.h"
#include "MGP/Player/MGPPlayerController.h"
#include "MGP/Player/MGPPlayerState.h"

AMGPGameModeBase::AMGPGameModeBase()
{
	GameStateClass		  = AMGPGameState::StaticClass();
	PlayerControllerClass = AMGPPlayerController::StaticClass();
	PlayerStateClass      = AMGPPlayerState::StaticClass();
	DefaultPawnClass	  = AMGPPlayerCharacter::StaticClass();
}

void AMGPGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick( this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne );
}

void AMGPGameModeBase::InitGameState()
{
	Super::InitGameState();

	if ( const TObjectPtr<UMGPExperienceManagerComponent> ExperienceManagerComponent = GameState->FindComponentByClass<UMGPExperienceManagerComponent>() )
	{
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded( FOnExperienceLoaded::FDelegate::CreateUObject( this, &ThisClass::OnExperienceLoaded ) );
	}
}

void AMGPGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if ( IsExperienceLoaded() )
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

UClass* AMGPGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if ( const UMGPPawnData* PawnData = GetPawnDataForController( InController ) )
	{
		if ( PawnData->PawnClass )
		{
			return PawnData->PawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AMGPGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.ObjectFlags |= RF_Transient;
	SpawnParameters.bDeferConstruction = true;

	if ( UClass* PawnClass = GetDefaultPawnClassForController( NewPlayer ) )
	{
		if( APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>( PawnClass, SpawnTransform, SpawnParameters ) )
		{
			if ( UMGPPawnExtensionComponent* PawnExtensionComponent = UMGPPawnExtensionComponent::FindPawnExtensionComponent( SpawnedPawn ) )
			{
				if ( const UMGPPawnData* PawnData = GetPawnDataForController( NewPlayer ) )
				{
					PawnExtensionComponent->SetPawnData( PawnData );
				}
			}

			SpawnedPawn->FinishSpawning( SpawnTransform );
			return SpawnedPawn;
		}
	}

	return nullptr;
}

const UMGPPawnData* AMGPGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if ( InController )
	{
		if( const AMGPPlayerState* MPGPS = InController->GetPlayerState<AMGPPlayerState>() )
		{
			if ( const UMGPPawnData* PawnData = MPGPS->GetPawnData<UMGPPawnData>() )
			{
				return PawnData;
			}
		}
	}

	if ( GameState == nullptr )
		return nullptr;
	
	const UMGPExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UMGPExperienceManagerComponent>();
	if ( ExperienceManagerComponent == nullptr )
		return nullptr;

	if ( ExperienceManagerComponent->IsExperienceLoaded() )
	{
		const UMGPExperienceDef* ExperienceDef = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if ( ExperienceDef->GetPawnData() )
		{
			return ExperienceDef->GetPawnData();
		}
	}
	
	return nullptr;
}

void AMGPGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperiencedId;

	if ( !ExperiencedId.IsValid() && UGameplayStatics::HasOption( OptionsString, TEXT( "Experience" ) ) )
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption( OptionsString, TEXT( "Experience" ) );
		ExperiencedId = FPrimaryAssetId( FPrimaryAssetType( UMGPExperienceDef::StaticClass()->GetFName() ), FName( *ExperienceFromOptions ) );
	}

	if ( !ExperiencedId.IsValid() )
	{
		ExperiencedId = FPrimaryAssetId( FPrimaryAssetType( "MGPExperienceDef" ), FName( "BP_DefaultExperience" ) );
	}
	
	OnMatchAssignmentGiven( ExperiencedId );
}

void AMGPGameModeBase::OnMatchAssignmentGiven(const FPrimaryAssetId& InExperienceId)
{
	check( InExperienceId.IsValid() )
	check( GameState != nullptr )
	
	UMGPExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UMGPExperienceManagerComponent>();
	check( ExperienceManagerComponent )

	ExperienceManagerComponent->ServerSetCurrentExperience( InExperienceId );
}

void AMGPGameModeBase::OnExperienceLoaded(const UMGPExperienceDef* InCurrentExperience)
{
	for ( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		APlayerController* PC = Cast<APlayerController>( *Iterator );
		if ( PC != nullptr && PC->GetPawn() == nullptr )
		{
			if( PlayerCanRestart( PC ) )
			{
				RestartPlayer( PC );
			}
		}
	}
}

bool AMGPGameModeBase::IsExperienceLoaded()
{
	if ( const TObjectPtr<UMGPExperienceManagerComponent> ExperienceManagerComponent = GameState->FindComponentByClass<UMGPExperienceManagerComponent>() )
	{
		return ExperienceManagerComponent->IsExperienceLoaded();
	}

	return false;
}
