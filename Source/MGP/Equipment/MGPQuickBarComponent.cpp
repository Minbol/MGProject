// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPQuickBarComponent.h"

#include "MGPEquipmentDefinition.h"
#include "MGPEquipmentInstance.h"
#include "MGPEquipmentManagerComponent.h"
#include "MGP/Inventory/MGPInventoryFragment_EquippableItem.h"
#include "MGP/Inventory/MGPInventoryItemInstance.h"

UMGPQuickBarComponent::UMGPQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPQuickBarComponent::BeginPlay()
{
	if ( Slots.Num() < NumSlots )
	{
		Slots.AddDefaulted( NumSlots - Slots.Num() );
	}
	
	Super::BeginPlay();
}

void UMGPQuickBarComponent::AddItemToSlot(const int32 InSlotIndex, UMGPInventoryItemInstance* InItem)
{
	if ( Slots.IsValidIndex( InSlotIndex ) && ( InItem != nullptr ) )
	{
		if ( Slots[ InSlotIndex ] == nullptr )
		{
			Slots[ InSlotIndex ] = InItem;
		}
	}
}

void UMGPQuickBarComponent::SetActiveSlotIndex(const int32 InNewIndex)
{
	if ( Slots.IsValidIndex( InNewIndex ) && ActiveSlotIndex != InNewIndex )
	{
		UnEquipItemInSlot();
		ActiveSlotIndex = InNewIndex;
		EquipItemInSlot();
	}
}

UMGPEquipmentManagerComponent* UMGPQuickBarComponent::FindEquipmentManager() const
{
	if ( const AController* OwnerController = Cast<AController>( GetOwner() ) )
	{
		if ( const APawn* Pawn = OwnerController->GetPawn() )
		{
			return Pawn->FindComponentByClass<UMGPEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void UMGPQuickBarComponent::EquipItemInSlot()
{
	check( Slots.IsValidIndex( ActiveSlotIndex ) );
	check( EquippedItem == nullptr )

	if ( UMGPInventoryItemInstance* SlotItem = Slots[ ActiveSlotIndex ] )
	{
		if ( const UMGPInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UMGPInventoryFragment_EquippableItem>() )
		{
			if ( TSubclassOf<UMGPEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition )
			{
				if ( UMGPEquipmentManagerComponent* EquipmentManagerComponent = FindEquipmentManager() )
				{
					EquippedItem = EquipmentManagerComponent->EquipItem( EquipDef );
					if ( EquippedItem )
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void UMGPQuickBarComponent::UnEquipItemInSlot()
{
	if ( UMGPEquipmentManagerComponent* EquipmentManagerComponent = FindEquipmentManager() )
	{
		if ( EquippedItem )
		{
			EquipmentManagerComponent->UnEquipItem( EquippedItem );
			EquippedItem = nullptr;
		}
	}
}
