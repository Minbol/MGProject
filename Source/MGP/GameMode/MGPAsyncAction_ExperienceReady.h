// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MGPAsyncAction_ExperienceReady.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadysyncDelegate);

class UMGPExperienceDef;
/**
 * 
 */
UCLASS()
class MGP_API UMGPAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UMGPAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "InWorldContextObject"))
	static UMGPAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* InWorldContextObject);

	virtual void Activate() override;

	void HandleGameStateSet(AGameStateBase* InGameState);
	void ListenToExperienceLoading(AGameStateBase* InGameState);
	void HandleExperienceLoaded(const UMGPExperienceDef* InCurrentExperience);
	void BroadcastReady();

public:
	UPROPERTY(BlueprintAssignable)
	FExperienceReadysyncDelegate OnReady;

	TWeakObjectPtr<UWorld> WorldPtr;
};
