// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MGPActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EMGPWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

/**
 *
 */
UCLASS()
class MGP_API UMGPActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UMGPActivatableWidget(const FObjectInitializer& ObjectInitializer);

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	EMGPWidgetInputMode InputConfig = EMGPWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
