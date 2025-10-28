// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MGPGameFeatureAction_WorldActionBase.h"
#include "UIExtensionSystem.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "MGPGameFeatureAction_AddWidgets.generated.h"

struct FComponentRequestHandle;

USTRUCT()
struct FMGPHUDLayoutRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag LayerID;
};

USTRUCT()
struct FMGPHUDElementEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};
/**
 * 
 */
UCLASS()
class MGP_API UMGPGameFeatureAction_AddWidgets : public UMGPGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutAdded;

		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void HandleActorExtension(AActor* InActor, FName InEventName, FGameFeatureStateChangeContext InChangeContext);

private:
	void AddWidgets(AActor* InActor, FPerContextData& InActiveData);
	void RemoveWidgets(AActor* InActor, FPerContextData& InActiveData);

public:
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FMGPHUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FMGPHUDElementEntry> Widgets;

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
};
