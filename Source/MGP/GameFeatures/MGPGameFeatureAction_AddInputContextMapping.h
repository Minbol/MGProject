// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPGameFeatureAction_WorldActionBase.h"
#include "MGPGameFeatureAction_AddInputContextMapping.generated.h"

struct FComponentRequestHandle;
class UInputMappingContext;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bRegisterWithSettings = true;
};

/**
 * 
 */
UCLASS()
class MGP_API UMGPGameFeatureAction_AddInputContextMapping : public UMGPGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureUnregistering() override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	UPROPERTY(EditAnywhere, Category="Input")
	TArray<FInputMappingContextAndPriority> InputMappings;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;

	void RegisterInputMappingContexts();
	void RegisterInputContextMappingsForGameInstance(UGameInstance* InGameInstance);
	void RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* InLocalPlayer);
	void UnregisterInputMappingContexts();
	void UnregisterInputContextMappingsForGameInstance(UGameInstance* InGameInstance);
	void UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);

	void Reset(FPerContextData& ActiveData);
	void HandleControllerExtension(AActor* InActor, FName InEventName, FGameFeatureStateChangeContext InChangeContext);
	void AddInputMappingForPlayer(UPlayer* InPlayer, FPerContextData& InActiveData);
	void RemoveInputMapping(APlayerController* InPlayerController, FPerContextData& InActiveData);
};
