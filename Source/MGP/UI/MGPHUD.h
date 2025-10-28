// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MGPHUD.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API AMGPHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMGPHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
