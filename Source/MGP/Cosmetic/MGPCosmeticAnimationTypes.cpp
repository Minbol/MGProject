// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCosmeticAnimationTypes.h"

USkeletalMesh* FMGPAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& InCosmeticTags) const
{
	for ( const auto& Rule : MeshRules )
	{
		if ( ( IsValid( Rule.Mesh ) && InCosmeticTags.HasAll( Rule.RequireTags ) ) )
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh;
}
