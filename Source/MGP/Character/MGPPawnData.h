// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MGPPawnData.generated.h"

class UMGPInputConfig;
class UMGPCameraMode;
/**
 * 
 */
UCLASS()
class MGP_API UMGPPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMGPPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MGP|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MGP|Camera")
	TSubclassOf<UMGPCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MGP|InputConfig")
	TObjectPtr<UMGPInputConfig> InputConfig;
};
