// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPExperienceActionSet.h"

#include "GameFeatureAction.h"

UMGPExperienceActionSet::UMGPExperienceActionSet()
{
}

#if WITH_EDITORONLY_DATA
void UMGPExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for ( UGameFeatureAction* Action : Actions )
	{
		if ( Action )
		{
			Action->AddAdditionalAssetBundleData( AssetBundleData );
		}
	}
}
#endif