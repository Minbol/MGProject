// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPCharacterPartTypes.h"
#include "MGPCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "MGPPawnComponent_CharacterParts.generated.h"

struct FGameplayTagContainer;
struct FMGPCharacterPartHandle;
class UMGPPawnComponent_CharacterParts;
/**
 * 
 */
USTRUCT()
struct FMGPAppliedCharacterPartEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FMGPCharacterPart Part;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;

	int32 PartHandle = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FMGPCharacterPartList
{
	GENERATED_BODY()

public:
	FMGPCharacterPartList()
		: OwnerComponent( nullptr )
	{}

	FMGPCharacterPartList(UMGPPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent( InOwnerComponent )
	{}

public:
	FMGPCharacterPartHandle AddEntry(const FMGPCharacterPart& InNewPart);
	void RemoveEntry(FMGPCharacterPartHandle InHandle);

	FGameplayTagContainer CollectCombinedTags() const;

private:
	bool SpawnActorForEntry(FMGPAppliedCharacterPartEntry& InEntry);
	void DestroyActorForEntry(FMGPAppliedCharacterPartEntry& InEntry);
	
	// FGameplayTagContainer CollectCombinedTags() const;

public:
	UPROPERTY()
	TArray<FMGPAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UMGPPawnComponent_CharacterParts> OwnerComponent ;

	int32 PartHandleCounter = 0;
};

UCLASS()
class MGP_API UMGPPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	UMGPPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FMGPCharacterPartHandle AddCharacterPart(const FMGPCharacterPart& InNewPart);
	void RemoveCharacterPart(FMGPCharacterPartHandle InHandle);
	USceneComponent* GetSceneComponentToAttachTo() const;
	void BroadcastChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag InGameplayTag);

private:
	USkeletalMeshComponent* GetParentMeshComponent() const;

public:
	UPROPERTY()
	FMGPCharacterPartList CharacterPartList;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FMGPAnimBodyStyleSelectionSet BodyMeshes;
};
