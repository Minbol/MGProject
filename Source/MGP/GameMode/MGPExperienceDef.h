// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MGPExperienceDef.generated.h"

class UGameFeatureAction;
class UMGPExperienceActionSet;
class UMGPPawnData;
/**
 * 
 */
UCLASS()
class MGP_API UMGPExperienceDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMGPExperienceDef(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE UMGPPawnData*   GetPawnData()             const { return DefaultPawnData; }
	FORCEINLINE TArray<FString> GetGameFeaturesToEnable() const { return GameFeaturesToEnable; }

public:
	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TObjectPtr<UMGPPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TArray<TObjectPtr<UMGPExperienceActionSet>> ActionsSets;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
