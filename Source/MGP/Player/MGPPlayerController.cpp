// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerController.h"

#include "MGP/Camera/MGPPlayerCameraManager.h"

AMGPPlayerController::AMGPPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AMGPPlayerCameraManager::StaticClass();
}
