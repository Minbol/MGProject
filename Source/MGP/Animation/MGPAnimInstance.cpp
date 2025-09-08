// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UMGPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if ( AActor* OwningActor = GetOwningActor() )
	{
		if ( UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor( OwningActor ) )
		{
			InitializeWithAbilitySystem( ASC );
		}
	}
}

void UMGPAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent)
{
	GameplayTagPropertyMap.Initialize( this, InAbilitySystemComponent );
}
