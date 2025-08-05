// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPInventoryItemInstance.h"

#include "MGPInventoryItemDefinition.h"

UMGPInventoryItemInstance::UMGPInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UMGPInventoryItemFragment* UMGPInventoryItemInstance::FindFragmentByClass(TSubclassOf<UMGPInventoryItemFragment> InFragmentClass) const
{
	if ( ( ItemDef != nullptr ) && ( InFragmentClass != nullptr ) )
	{
		return GetDefault<UMGPInventoryItemDefinition>(ItemDef)->FindFragmentByClass( InFragmentClass );
	}

	return nullptr;
}
