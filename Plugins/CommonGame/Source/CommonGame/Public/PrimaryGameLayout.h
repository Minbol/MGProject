// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PrimaryGameLayout.generated.h"

class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag InLayerName);
	
	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer(FGameplayTag InLayerTag, UCommonActivatableWidgetContainerBase* InLayerWidget);

	template<typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag InLayerName, UClass* InClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>( InLayerName, InClass, [](ActivatableWidgetT&) {} );
	}

	template<typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag InLayerName, UClass* InClass, TFunctionRef<void(ActivatableWidgetT&)> InPostInitFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "InClass must be a subclass of UCommonActivatableWidget");
		if ( UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget( InLayerName ) )
		{
			return Layer->AddWidget<ActivatableWidgetT>( InClass, InPostInitFunc );
		}

		return nullptr;
	}

public:
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
