// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MGPInventoryItemInstance.generated.h"

class UMGPInventoryItemFragment;
class UMGPInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class MGP_API UMGPInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UMGPInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	template<typename T>
	T* FindFragmentByClass() const
	{
		return (T*)(FindFragmentByClass(T::StaticClass()));
	}

private:
	const UMGPInventoryItemFragment* FindFragmentByClass(TSubclassOf<UMGPInventoryItemFragment> InFragmentClass) const;

public:
	UPROPERTY()
	TSubclassOf<UMGPInventoryItemDefinition> ItemDef;
};
