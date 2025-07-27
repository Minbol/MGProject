// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MGPGameModeBase.generated.h"

class UMGPPawnData;
class UMGPExperienceDef;
/**
 * 
 */
UCLASS()
class MGP_API AMGPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMGPGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	const UMGPPawnData* GetPawnDataForController(const AController* InController) const;

private:
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(const FPrimaryAssetId& InExperienceId);
	void OnExperienceLoaded(const UMGPExperienceDef* InCurrentExperience);
	bool IsExperienceLoaded();
};
