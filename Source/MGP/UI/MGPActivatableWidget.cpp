// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPActivatableWidget.h"

UMGPActivatableWidget::UMGPActivatableWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UMGPActivatableWidget::GetDesiredInputConfig() const
{
	switch ( InputConfig )
	{
	case EMGPWidgetInputMode::GameAndMenu:
		return FUIInputConfig( ECommonInputMode::All, GameMouseCaptureMode );
	case EMGPWidgetInputMode::Game:
		return FUIInputConfig( ECommonInputMode::Game, GameMouseCaptureMode );
	case EMGPWidgetInputMode::Menu:
		return FUIInputConfig( ECommonInputMode::Menu, GameMouseCaptureMode );
	case EMGPWidgetInputMode::Default:
		default:
		return TOptional<FUIInputConfig>();
	}
}
