// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "MGPInputConfig.h"
#include "MGPInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API UMGPInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UMGPInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UMGPInputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent InTriggerEvent, UserClass* InObject, FuncType InFunc, bool bLogIfNotFound)
	{
		check( InInputConfig );

		if ( const UInputAction* IA =InInputConfig->FindNativeInputActionForTag( InInputTag, bLogIfNotFound ) )
		{
			BindAction( IA, InTriggerEvent, InObject, InFunc );
		}
	}

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMGPInputConfig* InInputConfig, UserClass* InObject, PressedFuncType InPressedFunc, ReleasedFuncType InReleasedFunc, TArray<uint32>& InBindHandles)
	{
		check( InInputConfig );

		for ( const auto& Action : InInputConfig->AbilityInputActions )
		{
			if ( Action.InputAction && Action.InputTag.IsValid() )
			{
				if ( InPressedFunc )
				{
					InBindHandles.Emplace( BindAction( Action.InputAction, ETriggerEvent::Triggered, InObject, InPressedFunc, Action.InputTag ).Gethandle() );
				}

				if ( InReleasedFunc )
				{
					InBindHandles.Emplace( BindAction( Action.InputAction, ETriggerEvent::Completed, InObject, InReleasedFunc, Action.InputTag ).Gethandle() );
				}
			}
		}
	}
};