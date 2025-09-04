// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "MGPGameplayAbilityTargetData_SingleTargetHit.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MGP_API FMGPGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

public:
	FMGPGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FMGPGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

public:
	int32 CartridgeID;
};
