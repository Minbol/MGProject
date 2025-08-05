// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "MGPEquipmentManagerComponent.generated.h"

class UMGPEquipmentInstance;
class UMGPEquipmentDefinition;

USTRUCT(BlueprintType)
struct FMGPAppliedEquipmentEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSubclassOf<UMGPEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UMGPEquipmentInstance> Instance;
};

USTRUCT(BlueprintType)
struct FMGPEquipmentList
{
	GENERATED_BODY()

public:
	FMGPEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		:OwnerComponent(InOwnerComponent)
	{}

	UMGPEquipmentInstance* AddEntry(TSubclassOf<UMGPEquipmentDefinition> InEquipmentDefinition);
	void RemoveEntry(UMGPEquipmentInstance* InInstance);

public:
	UPROPERTY()
	TArray<FMGPAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * 
 */
UCLASS()
class MGP_API UMGPEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UMGPEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UMGPEquipmentInstance* EquipItem(TSubclassOf<UMGPEquipmentDefinition> InEquipmentDefinition);
	void UnEquipItem(UMGPEquipmentInstance* InItemInstance);

public:
	UPROPERTY()
	FMGPEquipmentList EquipmentList;
};
