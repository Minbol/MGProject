// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MGPAbilitySet.generated.h"

class UMGPGameplayAbility;
class UMGPAbilitySystemComponent;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FMGPAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMGPGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FMGPAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& InHandle);
	void TakeFromAbilitySystem(UMGPAbilitySystemComponent* InASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

UCLASS()
class MGP_API UMGPAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMGPAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UMGPAbilitySystemComponent* InASC, FMGPAbilitySet_GrantedHandles* OutGrantedHandles, UObject* InSourceObject = nullptr);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FMGPAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
