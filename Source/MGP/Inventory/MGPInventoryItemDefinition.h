// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MGPInventoryItemDefinition.generated.h"

/**
 * 
 */

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class MGP_API UMGPInventoryItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	
};

UCLASS(Blueprintable)
class MGP_API UMGPInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UMGPInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UMGPInventoryItemFragment* FindFragmentByClass(TSubclassOf<UMGPInventoryItemFragment> InFragmentClass) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UMGPInventoryItemFragment>> Fragments;
};
