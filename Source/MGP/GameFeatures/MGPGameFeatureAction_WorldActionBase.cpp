// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameFeatureAction_WorldActionBase.h"

#include "GameFeaturesSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UMGPGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	GameInstanceStartHandles.FindOrAdd( Context ) = FWorldDelegates::OnStartGameInstance.AddUObject( this, 
		&UMGPGameFeatureAction_WorldActionBase::HandleGameInstanceStart, FGameFeatureStateChangeContext( Context ) );
	
	for ( const FWorldContext& WorldContext : GEngine->GetWorldContexts() )
	{
		if ( Context.ShouldApplyToWorldContext( WorldContext ) )
		{
			AddToWorld( WorldContext, Context );
		}
	}
}

void UMGPGameFeatureAction_WorldActionBase::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FDelegateHandle* FoundHandle = GameInstanceStartHandles.Find( Context );
	if ( ensure( FoundHandle ) )
	{
		FWorldDelegates::OnStartGameInstance.Remove( *FoundHandle );
	}
}

void UMGPGameFeatureAction_WorldActionBase::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
{
	if ( FWorldContext* WorldContext = GameInstance->GetWorldContext() )
	{
		if ( ChangeContext.ShouldApplyToWorldContext( *WorldContext ) )
		{
			AddToWorld( *WorldContext, ChangeContext );
		}
	}
}
