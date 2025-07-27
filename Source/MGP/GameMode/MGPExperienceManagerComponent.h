// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "MGPExperienceManagerComponent.generated.h"

class UMGPExperienceDef;

enum class EMGPExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam( FOnExperienceLoaded, const UMGPExperienceDef* )
/**
 * 
 */
UCLASS()
class MGP_API UMGPExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& InDelegate);

	void ServerSetCurrentExperience(const FPrimaryAssetId& InExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& InResult);
	void OnExperienceFullLoadCompleted();
	const UMGPExperienceDef* GetCurrentExperienceChecked() const;
	
	FORCEINLINE bool IsExperienceLoaded() const { return ( LoadState == EMGPExperienceLoadState::Loaded ) && ( CurrentExperience != nullptr ); }

public:
	UPROPERTY()
	const UMGPExperienceDef* CurrentExperience;

private:
	EMGPExperienceLoadState LoadState = EMGPExperienceLoadState::Unloaded;
	FOnExperienceLoaded OnExperienceLoaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
	
};
