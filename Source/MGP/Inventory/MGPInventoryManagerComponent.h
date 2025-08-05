// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MGPInventoryManagerComponent.generated.h"

class UMGPInventoryItemDefinition;
class UMGPInventoryItemInstance;

USTRUCT(BlueprintType)
struct FMGPInventoryEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UMGPInventoryItemInstance> Instance;
};

USTRUCT(BlueprintType)
struct FMGInventoryList
{
	GENERATED_BODY()

public:
	FMGInventoryList(UActorComponent* InOwnerComponent = nullptr)
		:OwnerComponent(InOwnerComponent)
	{}

	UMGPInventoryItemInstance* AddEntry(TSubclassOf<UMGPInventoryItemDefinition> InItemDef);

public:
	UPROPERTY()
	TArray<FMGPInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MGP_API UMGPInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMGPInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UMGPInventoryItemInstance* AddItemDefinition(TSubclassOf<UMGPInventoryItemDefinition> InItemDef);

public:
	UPROPERTY()
	FMGInventoryList InventoryList;
};
