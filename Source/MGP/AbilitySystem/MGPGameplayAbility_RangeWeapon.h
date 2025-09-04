// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGP/Equipment/MGPGameplayAbility_FromEquipment.h"
#include "MGPGameplayAbility_RangeWeapon.generated.h"

class UMGPRangedWeaponInstance;
/**
 * 
 */

UENUM(BlueprintType)
enum EMGPAbilityTargetingSource : uint8
{
	CameraTowardsFocus,
};

UCLASS()
class MGP_API UMGPGameplayAbility_RangeWeapon : public UMGPGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	UMGPGameplayAbility_RangeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		UMGPRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim    (ForceInitToZero)
			, AimDir    (ForceInitToZero)
		{}
	};

public:
	UFUNCTION(BlueprintCallable)
	void StartRangeWeaponTargeting();

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);

protected:
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput InInputData, TArray<FHitResult>& OutHits );
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams InParams);
	
	int32 FindFirstPawnHitResult(const TArray<FHitResult>& InHitResults);
	
	UMGPRangedWeaponInstance* GetWeaponInstance();
	FTransform GetTargetingTransform(APawn* InSourcePawn, EMGPAbilityTargetingSource InSource);
	FVector GetWeaponTargetingSourceLocation() const;
	FHitResult DoSingleBulletTrace(const FVector& InStartTrace, const FVector& EndTrace, float InSweepRadius, bool bInSimulated, TArray<FHitResult>& OutHits);
	FHitResult WeaponTrace(const FVector& InStartTrace, const FVector& InEndTrace, float InSweepRadius, bool bInSimulated, TArray<FHitResult>& OutHits);
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& InParams, bool bInSimulated) const;
};
