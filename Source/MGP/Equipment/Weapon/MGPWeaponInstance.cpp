// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPWeaponInstance.h"

UMGPWeaponInstance::UMGPWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> UMGPWeaponInstance::PickBestAnimLayer(bool bInEquipped, const FGameplayTagContainer& InCosmeticTags) const
{
	const FMGPAnimLayerSelectionSet& SetToQuery = bInEquipped ? EquippedAnimSet : UnEquippedAnimSet;
	return SetToQuery.SelectBestLayer( InCosmeticTags );
}
