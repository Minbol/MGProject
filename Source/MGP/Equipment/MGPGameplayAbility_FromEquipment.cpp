// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameplayAbility_FromEquipment.h"

#include "Weapon/MGPRangedWeaponInstance.h"

UMGPRangedWeaponInstance* UMGPGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if ( FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec() )
	{
		return Cast<UMGPRangedWeaponInstance>( Spec->SourceObject.Get() );
	}

	return nullptr;
}
