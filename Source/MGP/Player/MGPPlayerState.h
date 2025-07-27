// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MGP/Character/MGPPawnData.h"
#include "MGP/GameMode/MGPExperienceDef.h"
#include "MGPPlayerState.generated.h"

class UMGPExperienceDef;
class UMGPPawnData;
/**
 * 
 */
UCLASS()
class MGP_API AMGPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() final;
	
	void OnExperienceLoaded(const UMGPExperienceDef* InCurrentExperience);
	void SetPawnData(const UMGPPawnData* InPawnData);

	template<typename T>
	const T* GetPawnData() const { return Cast<T>( PawnData ); }
	
private:
	UPROPERTY()
	const UMGPPawnData* PawnData;
};
