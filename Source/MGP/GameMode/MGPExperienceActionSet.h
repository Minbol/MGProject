// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MGPExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class MGP_API UMGPExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMGPExperienceActionSet();

public:
	UPROPERTY(EditAnywhere, Category = "Actions to Perform" )
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
