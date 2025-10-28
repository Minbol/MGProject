// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIMangerSubsystem.generated.h"

class UCommonLocalPlayer;
class UGameUIPolicy;
/**
 * 
 */
UCLASS(Abstract, Config = Game)
class COMMONGAME_API UGameUIMangerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameUIMangerSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void NotifyPlayerAdded(UCommonLocalPlayer* InLocalPlayer);
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* InLocalPlayer);
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* InLocalPlayer);

	void SwitchPolicy(UGameUIPolicy* InNewPolicy);
	
	UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; };

public:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy;
	
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
