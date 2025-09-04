// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGP/Equipment/Weapon/MGPWeaponInstance.h"
#include "MGPRangedWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API UMGPRangedWeaponInstance : public UMGPWeaponInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.f;
};
