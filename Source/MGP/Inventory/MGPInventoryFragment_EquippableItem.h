// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPInventoryItemDefinition.h"
#include "MGPInventoryFragment_EquippableItem.generated.h"

class UMGPEquipmentDefinition;
/**
 * 
 */
UCLASS()
class MGP_API UMGPInventoryFragment_EquippableItem : public UMGPInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = MGP)
	TSubclassOf<UMGPEquipmentDefinition> EquipmentDefinition;
};
