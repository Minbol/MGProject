// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerController.h"

#include "MGPPlayerState.h"
#include "MGP/Camera/MGPPlayerCameraManager.h"
#include "MGP/AbilitySystem/MGPAbilitySystemComponent.h"

AMGPPlayerController::AMGPPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AMGPPlayerCameraManager::StaticClass();
}

void AMGPPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if ( UMGPAbilitySystemComponent* ASC = GetAbilitySystemComponent() )
	{
		ASC->ProcessAbilityInput( DeltaTime, bGamePaused );
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

AMGPPlayerState* AMGPPlayerController::GetPlayerState() const
{
	return CastChecked<AMGPPlayerState>( PlayerState, ECastCheckedType::NullAllowed );
}

UMGPAbilitySystemComponent* AMGPPlayerController::GetAbilitySystemComponent() const
{
	const AMGPPlayerState* PS = GetPlayerState();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}
