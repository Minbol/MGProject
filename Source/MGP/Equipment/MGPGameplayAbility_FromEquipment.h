// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGP/AbilitySystem/Abilities/MGPGameplayAbility.h"
#include "MGPGameplayAbility_FromEquipment.generated.h"

class UMGPRangedWeaponInstance;
/**
 * 
 */
UCLASS()
class MGP_API UMGPGameplayAbility_FromEquipment : public UMGPGameplayAbility
{
	GENERATED_BODY()

protected:
	UMGPRangedWeaponInstance* GetAssociatedEquipment() const;
};
