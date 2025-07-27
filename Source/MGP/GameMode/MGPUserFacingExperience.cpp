// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPUserFacingExperience.h"

#include "CommonSessionSubsystem.h"

UMGPUserFacingExperience::UMGPUserFacingExperience(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UCommonSession_HostSessionRequest* UMGPUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	if ( IsValid( Result ) )
	{
		Result->MapID = MapID;
		Result->ExtraArgs.Emplace( TEXT( "Experience" ), ExperienceName );
	}

	return Result;
}
