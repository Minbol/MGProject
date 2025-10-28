// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MGPReticleWidgetBase.generated.h"

class UMGPInventoryItemInstance;
class UMGPWeaponInstance;
/**
 * 
 */
UCLASS()
class MGP_API UMGPReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMGPReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UMGPWeaponInstance* InWeapon);

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMGPWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMGPInventoryItemInstance> InventoryInstance;
};
