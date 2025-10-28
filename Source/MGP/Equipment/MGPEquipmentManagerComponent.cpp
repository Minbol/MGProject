// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPEquipmentManagerComponent.h"

#include "MGPEquipmentDefinition.h"
#include "MGPEquipmentInstance.h"
#include "MGP/AbilitySystem/MGPAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

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

	if ( UMGPAbilitySystemComponent* ASC = GetAbilitySystemComponent() )
	{
		for ( auto AbilitySet : EquipmentCDO->AbilitySetsToGrant )
		{
			AbilitySet->GiveToAbilitySystem( ASC, &NewEntry.GrantedHandles, Result );
		}
	}

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
			if ( UMGPAbilitySystemComponent* ASC = GetAbilitySystemComponent() )
			{
				Entry.GrantedHandles.TakeFromAbilitySystem( ASC );
			}
			
			InInstance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UMGPAbilitySystemComponent* FMGPEquipmentList::GetAbilitySystemComponent() const
{
	if ( !IsValid( OwnerComponent ) )
	{
		return nullptr;
	}

	const AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UMGPAbilitySystemComponent>( UAbilitySystemGlobals::GetAbilitySystemComponentFromActor( OwningActor ) );
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

TArray<UMGPEquipmentInstance*> UMGPEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UMGPEquipmentInstance> InstanceType) const
{
	TArray<UMGPEquipmentInstance*> Result;
	for ( auto Entry : EquipmentList.Entries )
	{
		if ( Entry.Instance->IsA( InstanceType ) )
		{
			Result.Add( Entry.Instance );
		}
	}

	return Result;
}

UMGPEquipmentInstance* UMGPEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UMGPEquipmentInstance> InInstanceType)
{
	for ( auto& Entry : EquipmentList.Entries )
	{
		if ( UMGPEquipmentInstance* Instance = Entry.Instance )
		{
			if ( Instance->IsA( InInstanceType ) )
			{
				return Instance;
			}
		}
	}

	return nullptr;
}
