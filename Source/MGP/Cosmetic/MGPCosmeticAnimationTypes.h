// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MGPCosmeticAnimationTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMGPAnimLayerSelectionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequireTags;
};


USTRUCT(BlueprintType)
struct FMGPAnimLayerSelectionSet
{
	GENERATED_BODY()

public:
	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& InCosmeticTags) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMGPAnimLayerSelectionEntry> LayerRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
};

USTRUCT(BlueprintType)
struct FMGPAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequireTags;
};

USTRUCT(BlueprintType)
struct FMGPAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

public:
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& InCosmeticTags) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMGPAnimBodyStyleSelectionEntry> MeshRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcePhysicsAsset = nullptr;
};
