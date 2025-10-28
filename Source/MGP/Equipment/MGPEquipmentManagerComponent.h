// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "MGP/AbilitySystem/MGPAbilitySet.h"
#include "MGPEquipmentManagerComponent.generated.h"

class UMGPEquipmentInstance;
class UMGPEquipmentDefinition;
class UMGPAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FMGPAppliedEquipmentEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSubclassOf<UMGPEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UMGPEquipmentInstance> Instance;

	UPROPERTY()
	FMGPAbilitySet_GrantedHandles GrantedHandles;
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

	UMGPAbilitySystemComponent* GetAbilitySystemComponent() const;

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

	UFUNCTION(BlueprintCallable)
	TArray<UMGPEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UMGPEquipmentInstance> InstanceType) const;

	UMGPEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UMGPEquipmentInstance> InInstanceType);

	template<typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType( T::StaticClass() );
	}
	
public:
	UPROPERTY()
	FMGPEquipmentList EquipmentList;
};
