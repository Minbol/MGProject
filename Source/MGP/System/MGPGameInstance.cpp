// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameInstance.h"

#include "Components/GameFrameworkComponentManager.h"
#include "MGP/MGPGameplayTags.h"

void UMGPGameInstance::Init()
{
	Super::Init();
	
	if ( UGameFrameworkComponentManager* FrameworkComponent = GetSubsystem<UGameFrameworkComponentManager>( this ) )
	{
		const FMGPGameplayTags& GameplayTags = FMGPGameplayTags::Get();
		
		FrameworkComponent->RegisterInitState( GameplayTags.InitState_Spawned,         false, FGameplayTag() );
		FrameworkComponent->RegisterInitState( GameplayTags.InitState_DataAvailable,   false, GameplayTags.InitState_Spawned );
		FrameworkComponent->RegisterInitState( GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable );
		FrameworkComponent->RegisterInitState( GameplayTags.InitState_GameplayReady,   false, GameplayTags.InitState_DataInitialized );
	}
}

void UMGPGameInstance::Shutdown()
{
	Super::Shutdown();
}
