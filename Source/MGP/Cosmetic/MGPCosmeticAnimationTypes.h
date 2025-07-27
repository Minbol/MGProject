// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MGPCosmeticAnimationTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMGPAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

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
