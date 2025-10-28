// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPReticleWidgetBase.h"

#include "MGP/Equipment/Weapon/MGPWeaponInstance.h"
#include "MGP/Inventory/MGPInventoryItemInstance.h"

UMGPReticleWidgetBase::UMGPReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMGPReticleWidgetBase::InitializeFromWeapon(UMGPWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;

	if ( WeaponInstance )
	{
		InventoryInstance = Cast<UMGPInventoryItemInstance>( WeaponInstance->GetInstigator() );
	}
}
