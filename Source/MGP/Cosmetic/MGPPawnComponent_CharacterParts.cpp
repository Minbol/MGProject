// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPawnComponent_CharacterParts.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

bool FMGPCharacterPartList::SpawnActorForEntry(FMGPAppliedCharacterPartEntry& InEntry)
{
	bool bCreateAnyActor = false;
	if ( InEntry.Part.PartClass != nullptr )
	{
		if ( USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo() )
		{
			if ( UChildActorComponent* PartComponent = NewObject<UChildActorComponent>( OwnerComponent->GetOwner() ) )
		 	{
		 		PartComponent->SetupAttachment( ComponentToAttachTo, InEntry.Part.SocketName );
		 		PartComponent->SetChildActorClass( InEntry.Part.PartClass );
		 		PartComponent->RegisterComponent();

		 		if ( const AActor* SpawnedActor = PartComponent->GetChildActor() )
		 		{
		 			if ( USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent() )
		 			{
		 				SpawnedRootComponent->AddTickPrerequisiteComponent( ComponentToAttachTo );
		 			}
		 		}

		 		InEntry.SpawnedComponent = PartComponent;
		 		bCreateAnyActor = true;
		 	}
		}
	}
	
	return bCreateAnyActor;
}

void FMGPCharacterPartList::DestroyActorForEntry(FMGPAppliedCharacterPartEntry& InEntry)
{
	if ( InEntry.SpawnedComponent )
	{
		InEntry.SpawnedComponent->DestroyComponent();
		InEntry.SpawnedComponent = nullptr;
	}
}

FMGPCharacterPartHandle FMGPCharacterPartList::AddEntry(const FMGPCharacterPart& InNewPart)
{
	FMGPCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if ( IsValid( OwnerComponent ) && IsValid( OwnerComponent->GetOwner() ) && OwnerComponent->GetOwner()->HasAuthority() )
	{
		FMGPAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part= InNewPart;
		NewEntry.PartHandle = Result.PartHandle;

		if ( SpawnActorForEntry( NewEntry ) )
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FMGPCharacterPartList::RemoveEntry(const FMGPCharacterPartHandle InHandle)
{
	for ( auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt )
	{
		FMGPAppliedCharacterPartEntry Entry = *EntryIt;
		if ( Entry.PartHandle == InHandle.PartHandle )
		{
			DestroyActorForEntry( Entry );
		}
	}
}

FGameplayTagContainer FMGPCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;
	for ( auto& Entry : Entries )
	{
		if ( Entry.SpawnedComponent )
		{
			if ( const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>( Entry.SpawnedComponent->GetChildActor() ) )
			{
				TagInterface->GetOwnedGameplayTags( Result );
			}
		}
	}

	return Result;
}

UMGPPawnComponent_CharacterParts::UMGPPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	CharacterPartList(this)
{
}

FMGPCharacterPartHandle UMGPPawnComponent_CharacterParts::AddCharacterPart(const FMGPCharacterPart& InNewPart)
{
	return CharacterPartList.AddEntry( InNewPart );
}

void UMGPPawnComponent_CharacterParts::RemoveCharacterPart(const FMGPCharacterPartHandle InHandle)
{
	CharacterPartList.RemoveEntry( InHandle );
}

USkeletalMeshComponent* UMGPPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if ( AActor* OwnerActor = GetOwner() )
	{
		if ( ACharacter* OwningCharacter = Cast<ACharacter>( OwnerActor ) )
		{
			if ( USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh() )
			{
				return MeshComponent;
			}
		}
	}
	
	return nullptr;
}

FGameplayTagContainer UMGPPawnComponent_CharacterParts::GetCombinedTags(const FGameplayTag InGameplayTag)
{
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if ( InGameplayTag.IsValid() )
	{
		return Result.Filter( FGameplayTagContainer( InGameplayTag ) );
	}
	else
	{
		return Result;
	}
}

USceneComponent* UMGPPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if ( USkeletalMeshComponent* MeshComponent = GetParentMeshComponent() )
	{
		return MeshComponent;
	}
	else if( AActor* OwnerActor = GetOwner() )
	{
		return OwnerActor->GetRootComponent();
	}
	
	return nullptr;
}

void UMGPPawnComponent_CharacterParts::BroadcastChanged()
{
	if ( USkeletalMeshComponent* MeshComponent = GetParentMeshComponent() )
	{
		constexpr bool bReinitPose = true;
		const FGameplayTagContainer MergedTags = GetCombinedTags( FGameplayTag() );
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle( MergedTags );

		MeshComponent->SetSkeletalMesh( DesiredMesh, bReinitPose );

		if ( UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcePhysicsAsset )
		{
			MeshComponent->SetPhysicsAsset( PhysicsAsset, bReinitPose );
		}
	}
}
