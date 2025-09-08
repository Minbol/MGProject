// Fill out your copyright notice in the Description page of Project Settings.

#include "MGPAbilitySystemComponent.h"

#include "Abilities/MGPGameplayAbility.h"
#include "MGP/Animation/MGPAnimInstance.h"

UMGPAbilitySystemComponent::UMGPAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
}

void UMGPAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	if ( !IsValid( InOwnerActor ) )
	{
		return;
	}

	if ( !IsValid( InAvatarActor ) )
	{
		return;
	}

	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	const bool HasNewPawnAvatar = Cast<APawn>( InAvatarActor ) && InAvatarActor != ActorInfo->AvatarActor.Get();
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if ( HasNewPawnAvatar )
	{
		if ( UMGPAnimInstance* AnimInstance = Cast<UMGPAnimInstance>( ActorInfo->GetAnimInstance() ) )
		{
			AnimInstance->InitializeWithAbilitySystem( this );
		}
	}
}

void UMGPAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InInputTag)
{
	if ( !InInputTag.IsValid() )
		return;

	for( auto& AbilitySpec : ActivatableAbilities.Items )
	{
		if ( AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact( InInputTag ) )
		{
			InputPressedSpecHandles.AddUnique( AbilitySpec.Handle );
			InputHeldSpecHandles.AddUnique( AbilitySpec.Handle );
		}
	}
}

void UMGPAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InInputTag)
{
	if ( !InInputTag.IsValid() )
		return;

	for( auto& AbilitySpec : ActivatableAbilities.Items )
	{
		if ( AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact( InInputTag ) )
		{
			InputReleasedSpecHandles.AddUnique( AbilitySpec.Handle );
			InputHeldSpecHandles.Remove( AbilitySpec.Handle );
		}
	}
}

void UMGPAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	for ( const auto& SpecHandle : InputHeldSpecHandles )
	{
		if ( const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if ( AbilitySpec->Ability && !AbilitySpec->IsActive() )
			{
				const UMGPGameplayAbility* AbilityCDO = CastChecked<UMGPGameplayAbility>( AbilitySpec->Ability );
				if ( AbilityCDO->ActivationPolicy == EMGPAbilityActivationPolicy::WhileInputActive )
				{
					AbilitiesToActivate.AddUnique( AbilitySpec->Handle );
				}
			}
		}
	}

	for ( const auto& SpecHandle : InputPressedSpecHandles )
	{
		if ( FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if ( AbilitySpec->Ability )
			{
				AbilitySpec->InputPressed = true;
				if ( AbilitySpec->IsActive() )
				{
					AbilitySpecInputPressed( *AbilitySpec );
				}
				else
				{
					const UMGPGameplayAbility* AbilityCDO = CastChecked<UMGPGameplayAbility>( AbilitySpec->Ability );
					if ( AbilityCDO->ActivationPolicy == EMGPAbilityActivationPolicy::OnInputTriggered )
					{
						AbilitiesToActivate.AddUnique( AbilitySpec->Handle );
					}
				}
			}
		}
	}

	for ( const auto& AbilitySpecHandle : AbilitiesToActivate )
	{
		TryActivateAbility( AbilitySpecHandle );
	}

	for ( const auto& SpecHandle : InputReleasedSpecHandles )
	{
		if ( FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if ( AbilitySpec->Ability )
			{
				AbilitySpec->InputPressed = false;
				if ( AbilitySpec->IsActive() )
				{
					AbilitySpecInputReleased( *AbilitySpec );
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
