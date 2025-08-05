// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGP/Cosmetic/MGPCosmeticAnimationTypes.h"
#include "MGP/Equipment/MGPEquipmentInstance.h"
#include "MGPWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API UMGPWeaponInstance : public UMGPEquipmentInstance
{
	GENERATED_BODY()

public:
	UMGPWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bInEquipped, const FGameplayTagContainer& InCosmeticTags) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FMGPAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FMGPAnimLayerSelectionSet UnEquippedAnimSet;
};
