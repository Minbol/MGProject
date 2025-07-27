// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPCameraMode.generated.h"

class UMGPCameraComponent;

UENUM(BlueprintType)
enum class EMGPCameraModeBlendFunctionType : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

struct FMGPCameraModeView
{
	FMGPCameraModeView();

	void Blend(const FMGPCameraModeView& InOther, float InOtherWeight);
	
	FVector  Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float	 FieldOfView;
};

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class MGP_API UMGPCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UMGPCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateCameraMode(float InDeltaTime);

protected:
	virtual void UpdateView(float InDeltaTime);
	void UpdateBlending(float InDeltaTime);

	FVector GetPivotLocation();
	FRotator GetPivotRotation();
	AActor* GetTargetActor();
	UMGPCameraComponent* GetMGPCameraComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMIN = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMIN = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMIN = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	FMGPCameraModeView View;
	float BlendAlpha;
	float BlendWeight;

	EMGPCameraModeBlendFunctionType BlendFunctionType;
};

UCLASS()
class MGP_API UMGPCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UMGPCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UMGPCameraMode* GetCameraModeInstance(const TSubclassOf<UMGPCameraMode>& InCameraModeClass);
	void PushCameraMode(const TSubclassOf<UMGPCameraMode>& InCameraMode);
	void EvaluateStack(float InDeltaTime, FMGPCameraModeView& OutModeView);

private:
	void UpdateStack(float InDeltaTime);
	void BlendStack(FMGPCameraModeView& OutModeView);

private:
	UPROPERTY()
	TArray<TObjectPtr<UMGPCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UMGPCameraMode>> CameraModeStack;
};
