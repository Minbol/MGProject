// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUIExtensions.generated.h"

class UCommonActivatableWidget;
class ULocalPlayer;
/**
 * 
 */
UCLASS()
class COMMONGAME_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UCommonUIExtensions();
	
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(ULocalPlayer* InLocalPlayer, FGameplayTag InLayerName, TSubclassOf<UCommonActivatableWidget> InClass);
};
