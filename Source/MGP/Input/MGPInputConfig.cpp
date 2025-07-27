// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPInputConfig.h"

UMGPInputConfig::UMGPInputConfig(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UInputAction* UMGPInputConfig::FindNativeInputActionForTag(const FGameplayTag& InInputTag, bool bLogNotFound) const
{
	for ( const auto Action : NativeInputActions )
	{
		if ( Action.InputAction  && Action.InputTag == InInputTag )
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}

const UInputAction* UMGPInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InInputTag, bool bLogNotFound) const
{
	for ( const auto Action : AbilityInputActions )
	{
		if ( Action.InputAction  && Action.InputTag == InInputTag )
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}
