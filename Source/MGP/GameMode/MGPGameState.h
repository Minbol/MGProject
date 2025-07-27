// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MGPGameState.generated.h"

class UMGPExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class MGP_API AMGPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMGPGameState();

public:
	UPROPERTY()
	TObjectPtr<UMGPExperienceManagerComponent> ExperienceManagerComponent;
};
