// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerCameraManager.h"

AMGPPlayerCameraManager::AMGPPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = MGP_CAMERA_DEFAULT_FOV;
	ViewPitchMin = MGP_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = MGP_CAMERA_DEFAULT_PITCH_MAX;
}
