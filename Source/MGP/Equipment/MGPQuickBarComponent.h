// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "MGPQuickBarComponent.generated.h"

class UMGPEquipmentManagerComponent;
class UMGPEquipmentInstance;
class UMGPInventoryItemInstance;
/**
 * 
 */
UCLASS()
class MGP_API UMGPQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UMGPQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(const int32 InSlotIndex, UMGPInventoryItemInstance* InItem);

	UFUNCTION(BlueprintCallable, Category = MGP)
	void SetActiveSlotIndex(const int32 InNewIndex);

private:
	UMGPEquipmentManagerComponent* FindEquipmentManager()const;
	void EquipItemInSlot();
	void UnEquipItemInSlot();

public:
	int32 NumSlots = 3;
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TArray<TObjectPtr<UMGPInventoryItemInstance>> Slots;

	UPROPERTY()
	TObjectPtr<UMGPEquipmentInstance> EquippedItem;
};
