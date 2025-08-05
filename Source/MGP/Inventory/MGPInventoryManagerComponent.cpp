// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPInventoryManagerComponent.h"

#include "MGPInventoryItemDefinition.h"
#include "MGPInventoryItemInstance.h"

UMGPInventoryItemInstance* FMGInventoryList::AddEntry(TSubclassOf<UMGPInventoryItemDefinition> InItemDef)
{
	UMGPInventoryItemInstance* Result = nullptr;
	check( InItemDef );
	check( OwnerComponent );

	AActor* OwningActor = OwnerComponent->GetOwner();
	check( OwningActor->HasAuthority() );

	FMGPInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UMGPInventoryItemInstance>( OwningActor );
	NewEntry.Instance->ItemDef = InItemDef;

	Result = NewEntry.Instance;
	return Result;
}

UMGPInventoryManagerComponent::UMGPInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	InventoryList(this)
{
}

void UMGPInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

UMGPInventoryItemInstance* UMGPInventoryManagerComponent::AddItemDefinition(TSubclassOf<UMGPInventoryItemDefinition> InItemDef)
{
	UMGPInventoryItemInstance* Result = nullptr;
	if ( InItemDef )
	{
		Result = InventoryList.AddEntry(InItemDef);
	}

	return Result;
}
