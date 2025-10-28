// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIMangerSubsystem.h"

#include "GameUIPolicy.h"

UGameUIMangerSubsystem::UGameUIMangerSubsystem()
{
}

void UGameUIMangerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if ( CurrentPolicy == nullptr && !DefaultUIPolicyClass.IsNull() )
	{
		const TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		SwitchPolicy( NewObject<UGameUIPolicy>( this, PolicyClass ) );
	}
}

void UGameUIMangerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SwitchPolicy( nullptr );
}

bool UGameUIMangerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if ( !CastChecked<UGameInstance>( Outer )->IsDedicatedServerInstance() )
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses( GetClass(), ChildClasses, false );

		return ChildClasses.Num() == 0;
	}
	
	return false;
}

void UGameUIMangerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* InLocalPlayer)
{
	if ( InLocalPlayer != nullptr && CurrentPolicy )
	{
		CurrentPolicy->NotifyPlayerAdded( InLocalPlayer );
	}
}

void UGameUIMangerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* InLocalPlayer)
{
	if ( InLocalPlayer != nullptr && CurrentPolicy )
	{
		CurrentPolicy->NotifyPlayerRemoved( InLocalPlayer );
	}
}

void UGameUIMangerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* InLocalPlayer)
{
	if ( InLocalPlayer != nullptr && CurrentPolicy )
	{
		CurrentPolicy->NotifyPlayerDestroyed( InLocalPlayer );
	}
}

void UGameUIMangerSubsystem::SwitchPolicy(UGameUIPolicy* InNewPolicy)
{
	if ( CurrentPolicy != InNewPolicy )
	{
		CurrentPolicy = InNewPolicy;
	}
}
