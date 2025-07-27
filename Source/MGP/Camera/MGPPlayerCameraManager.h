// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MGPPlayerCameraManager.generated.h"

#define MGP_CAMERA_DEFAULT_FOV ( 80.0f )
#define MGP_CAMERA_DEFAULT_PITCH_MIN ( -89.0f )
#define MGP_CAMERA_DEFAULT_PITCH_MAX ( 89.0f )

/**
 * 
 */
UCLASS()
class MGP_API AMGPPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMGPPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
