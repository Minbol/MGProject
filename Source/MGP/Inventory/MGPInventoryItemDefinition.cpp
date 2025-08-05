// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPInventoryItemDefinition.h"

UMGPInventoryItemDefinition::UMGPInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UMGPInventoryItemFragment* UMGPInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UMGPInventoryItemFragment> InFragmentClass) const
{
	if ( InFragmentClass )
	{
		for ( auto Fragment : Fragments )
		{
			if ( Fragment && ( Fragment->IsA( InFragmentClass ) ) )
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
