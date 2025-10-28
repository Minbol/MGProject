// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryGameLayout.h"

#include "Widgets/CommonActivatableWidgetContainer.h"

UPrimaryGameLayout::UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

UCommonActivatableWidgetContainerBase* UPrimaryGameLayout::GetLayerWidget(FGameplayTag InLayerName)
{
	return Layers.FindRef( InLayerName );
}

void UPrimaryGameLayout::RegisterLayer(FGameplayTag InLayerTag, UCommonActivatableWidgetContainerBase* InLayerWidget)
{
	if ( !IsDesignTime() )
	{
		InLayerWidget->SetTransitionDuration( 0.f );
		Layers.Add( InLayerTag, InLayerWidget );
	}
}
