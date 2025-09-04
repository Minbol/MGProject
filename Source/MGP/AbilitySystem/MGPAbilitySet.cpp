// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPAbilitySet.h"

#include "MGPAbilitySystemComponent.h"
#include "Abilities/MGPGameplayAbility.h"

void FMGPAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& InHandle)
{
	if ( InHandle.IsValid() )
	{
		AbilitySpecHandles.Emplace( InHandle );
	}
}

void FMGPAbilitySet_GrantedHandles::TakeFromAbilitySystem(UMGPAbilitySystemComponent* InASC)
{
	if ( !InASC->IsOwnerActorAuthoritative() )
	{
		return;
	}

	for ( auto& Handle : AbilitySpecHandles )
	{
		if ( !Handle.IsValid() )
			continue;

		InASC->ClearAbility( Handle );
	}
}

UMGPAbilitySet::UMGPAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPAbilitySet::GiveToAbilitySystem(UMGPAbilitySystemComponent* InASC, FMGPAbilitySet_GrantedHandles* OutGrantedHandles, UObject* InSourceObject)
{
	if ( InASC == nullptr )
	{
		return;
	}

	if ( !InASC->IsOwnerActorAuthoritative() )
	{
		return;
	}

	for ( int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex )
	{
		const FMGPAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[ AbilityIndex ];
		if ( !IsValid( AbilityToGrant.Ability ) )
		{
			continue;
		}

		UMGPGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMGPGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec( AbilityCDO, AbilityToGrant.AbilityLevel );
		AbilitySpec.SourceObject = InSourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag( AbilityToGrant.InputTag );

		const FGameplayAbilitySpecHandle AbilitySpecHandle = InASC->GiveAbility( AbilitySpec );
		if ( OutGrantedHandles )
		{
			OutGrantedHandles->AddAbilitySpecHandle( AbilitySpecHandle );
		}
	}
}
