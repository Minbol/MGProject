// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPEquipmentInstance.h"

#include "MGPEquipmentDefinition.h"
#include "GameFramework/Character.h"

UMGPEquipmentInstance::UMGPEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UMGPEquipmentInstance::OnUnEquipped()
{
	K2_OnUnEquipped();
}

void UMGPEquipmentInstance::SpawnEquipmentActors(const TArray<FMGPEquipmentActorToSpawn>& InActorsToSpawn)
{
	if ( APawn* OwningPawn = GetPawn() )
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if ( const ACharacter* Character = Cast<ACharacter>( OwningPawn ) )
		{
			AttachTarget = Character->GetMesh();
		}

		for ( auto& SpawnInfo : InActorsToSpawn )
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>( SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn );
			NewActor->FinishSpawning( FTransform::Identity, true );
			NewActor->SetActorRelativeTransform( SpawnInfo.AttachTransform );
			NewActor->AttachToComponent( AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket );

			SpawnedActors.Emplace( NewActor );
		}
	}
}

void UMGPEquipmentInstance::DestroyEquipmentActors()
{
	for ( auto Actor : SpawnedActors )
	{
		if ( Actor )
		{
			Actor->Destroy();
		}
	}
}

APawn* UMGPEquipmentInstance::GetPawn() const
{
	return Cast<APawn>( GetOuter() );
}

APawn* UMGPEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> InPawnType)
{
	APawn* Result = nullptr;
	if ( UClass* ActualPawnType = InPawnType )
	{
		if ( GetOuter()->IsA( ActualPawnType ) )
		{
			Result = Cast<APawn>( GetOuter() );
		}
	}

	return Result;
}
