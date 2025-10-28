// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGP/Inventory/MyMGPInventoryItemFragment.h"
#include "MGPInventoryFragment_ReticleConfig.generated.h"

class UMGPReticleWidgetBase;
/**
 * 
 */
UCLASS()
class MGP_API UMGPInventoryFragment_ReticleConfig : public UMyMGPInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<UMGPReticleWidgetBase>> ReticleWidgets;
};
