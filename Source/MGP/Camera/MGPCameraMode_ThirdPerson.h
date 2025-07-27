// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPCameraMode.h"
#include "MGPCameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MGP_API UMGPCameraMode_ThirdPerson : public UMGPCameraMode
{
	GENERATED_BODY()

public:
	UMGPCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person" )
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
