// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPWeaponUserInterface.h"

#include "MGP/Equipment/MGPEquipmentManagerComponent.h"

UMGPWeaponUserInterface::UMGPWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMGPWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if ( APawn* Pawn = GetOwningPlayerPawn() )
	{
		if ( UMGPEquipmentManagerComponent* EquipmentManagerComponent = Pawn->FindComponentByClass<UMGPEquipmentManagerComponent>() )
		{
			if ( UMGPWeaponInstance* NewInstance = EquipmentManagerComponent->GetFirstInstanceOfType<UMGPWeaponInstance>() )
			{
				if ( NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr )
				{
					UMGPWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged( OldWeapon, CurrentInstance );
				}
			}
		}
	}
}
