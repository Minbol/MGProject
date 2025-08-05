// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MGPEquipmentInstance.generated.h"

struct FMGPEquipmentActorToSpawn;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MGP_API UMGPEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UMGPEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnEquipped();
	virtual void OnUnEquipped();

	void SpawnEquipmentActors(const TArray<FMGPEquipmentActorToSpawn>& InActorsToSpawn);
	void DestroyEquipmentActors();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnEquipped"))
	void K2_OnUnEquipped();

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = InPawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> InPawnType);

public:
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
