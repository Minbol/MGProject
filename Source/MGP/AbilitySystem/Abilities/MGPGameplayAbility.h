// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MGPGameplayAbility.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMGPAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn,
};

UCLASS()
class MGP_API UMGPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMGPGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MGP|AbilityActivation")
	EMGPAbilityActivationPolicy ActivationPolicy;
};
