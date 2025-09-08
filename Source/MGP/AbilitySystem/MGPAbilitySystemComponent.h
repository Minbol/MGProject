// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MGPAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API UMGPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMGPAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	void AbilityInputTagPressed(const FGameplayTag& InInputTag);
	void AbilityInputTagReleased(const FGameplayTag& InInputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
