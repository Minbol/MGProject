// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "MGPPlayerController.generated.h"

class UMGPAbilitySystemComponent;
class AMGPPlayerState;
/**
 * 
 */
UCLASS()
class MGP_API AMGPPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	AMGPPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

protected:
	AMGPPlayerState* GetPlayerState() const;
	UMGPAbilitySystemComponent* GetAbilitySystemComponent() const;
};
