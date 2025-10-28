// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MGP/Equipment/Weapon/MGPWeaponInstance.h"
#include "MGPWeaponUserInterface.generated.h"

class UMGPWeaponInstance;
/**
 * 
 */
UCLASS()
class MGP_API UMGPWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMGPWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UMGPWeaponInstance* InOldWeapon, UMGPWeaponInstance* InNewWeapon);

public:
	UPROPERTY(Transient)
	TObjectPtr<UMGPWeaponInstance> CurrentInstance;
};
