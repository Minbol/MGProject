// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameplayAbility.h"

UMGPGameplayAbility::UMGPGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EMGPAbilityActivationPolicy::OnInputTriggered;
}
