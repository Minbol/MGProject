// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "MGPCameraComponent.generated.h"

class UMGPCameraMode;
class UMGPCameraModeStack;

template<class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal( TSubclassOf<UMGPCameraMode>, FMGPCameraModeDelegate );
/**
 * 
 */
UCLASS()
class MGP_API UMGPCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UMGPCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	static UMGPCameraComponent* FindCameraComponent(const AActor* InActor) { return ( IsValid( InActor ) ? InActor->FindComponentByClass<UMGPCameraComponent>() : nullptr ); }

	FORCEINLINE AActor* GetTargetActor() const { return GetOwner(); }
	
	void UpdateCameraModes();

	FORCEINLINE FMGPCameraModeDelegate GetDetermineCameraModeDelegate() const { return OnDetermineCameraModeDelegate; }

public:
	UPROPERTY()
	TObjectPtr<UMGPCameraModeStack> CameraModeStack;

	FMGPCameraModeDelegate OnDetermineCameraModeDelegate;
};
