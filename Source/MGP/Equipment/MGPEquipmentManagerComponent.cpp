// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPEquipmentManagerComponent.h"

#include "MGPEquipmentDefinition.h"
#include "MGPEquipmentInstance.h"

UMGPEquipmentInstance* FMGPEquipmentList::AddEntry(TSubclassOf<UMGPEquipmentDefinition> InEquipmentDefinition)
{
	UMGPEquipmentInstance* Result = nullptr;
	check( InEquipmentDefinition != nullptr );
	check( OwnerComponent );
	check( OwnerComponent->GetOwner()->HasAuthority() );

	const UMGPEquipmentDefinition* EquipmentCDO = GetDefault<UMGPEquipmentDefinition>( InEquipmentDefinition );
	TSubclassOf<UMGPEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if ( !InstanceType )
	{
		InstanceType = UMGPEquipmentInstance::StaticClass();
	}

	FMGPAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = InEquipmentDefinition;
	NewEntry.Instance = NewObject<UMGPEquipmentInstance>( OwnerComponent->GetOwner(), InstanceType );
	Result = NewEntry.Instance;

	Result->SpawnEquipmentActors( EquipmentCDO->ActorsToSpawn );

	return Result;
}

void FMGPEquipmentList::RemoveEntry(UMGPEquipmentInstance* InInstance)
{
	for ( auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt )
	{
		FMGPAppliedEquipmentEntry& Entry = *EntryIt;
		if ( Entry.Instance == InInstance )
		{
			InInstance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UMGPEquipmentManagerComponent::UMGPEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	EquipmentList(this)
{
}

UMGPEquipmentInstance* UMGPEquipmentManagerComponent::EquipItem(TSubclassOf<UMGPEquipmentDefinition> InEquipmentDefinition)
{
	UMGPEquipmentInstance* Result = nullptr;
	if ( InEquipmentDefinition )
	{
		Result = EquipmentList.AddEntry( InEquipmentDefinition );
		if ( Result )
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void UMGPEquipmentManagerComponent::UnEquipItem(UMGPEquipmentInstance* InItemInstance)
{
	if ( InItemInstance )
	{
		InItemInstance->OnUnEquipped();
		EquipmentList.RemoveEntry( InItemInstance );
	}
}
