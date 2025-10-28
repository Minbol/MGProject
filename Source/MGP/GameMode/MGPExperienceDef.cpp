// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPExperienceDef.h"

#include "GameFeatureAction.h"

UMGPExperienceDef::UMGPExperienceDef(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITORONLY_DATA
void UMGPExperienceDef::UpdateAssetBundleData()
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