// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MGPEquipmentDefinition.generated.h"

class UMGPEquipmentInstance;

USTRUCT()
struct FMGPEquipmentActorToSpawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(Blueprintable)
class MGP_API UMGPEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UMGPEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UMGPEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FMGPEquipmentActorToSpawn> ActorsToSpawn;
};
