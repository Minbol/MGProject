// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCosmeticAnimationTypes.h"

TSubclassOf<UAnimInstance> FMGPAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& InCosmeticTags) const
{
	for ( const auto& Rule : LayerRules )
	{
		if ( ( Rule.Layer != nullptr ) && InCosmeticTags.HasAll( Rule.RequireTags ) )
		{
			return Rule.Layer;
		}
	}

	return DefaultLayer;
}

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
