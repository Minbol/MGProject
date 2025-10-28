// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameUIPolicy.generated.h"

class UCommonLocalPlayer;
class UPrimaryGameLayout;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout;

	bool bAddedToViewport = false;

	bool operator==(const ULocalPlayer* InOtherLocalPlayer) const { return LocalPlayer == InOtherLocalPlayer; }

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		:LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bAddedToViewport(bIsInViewport)
	{}
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	void NotifyPlayerAdded(UCommonLocalPlayer* InLocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* InLocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* InLocalPlayer);

	UPrimaryGameLayout* GetRootLayout(UCommonLocalPlayer* InLocalPlayer) const;
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* InLocalPlayer);

private:
	void AddLayoutFromViewport(UCommonLocalPlayer* InLocalPlayer, UPrimaryGameLayout* InLayout);
	void RemoveLayoutFromViewport(UCommonLocalPlayer* InLocalPlayer, UPrimaryGameLayout* InLayout);
	void CreateLayoutWidget(UCommonLocalPlayer* InLocalPlayer);

public:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
