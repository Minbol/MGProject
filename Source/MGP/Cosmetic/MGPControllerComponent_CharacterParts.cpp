// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPControllerComponent_CharacterParts.h"

#include "MGPPawnComponent_CharacterParts.h"

UMGPControllerComponent_CharacterParts::UMGPControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() )
	{
		if ( AController* OwingController = GetController<AController>() )
		{
			OwingController->OnPossessedPawnChanged.AddDynamic( this, &ThisClass::OnPossessedPawnChanged );
		}
	}
}

void UMGPControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

void UMGPControllerComponent_CharacterParts::AddCharacterPart(const FMGPCharacterPart& InNewPart)
{
	AddCharacterPartInternal( InNewPart );
}

void UMGPControllerComponent_CharacterParts::AddCharacterPartInternal(const FMGPCharacterPart& InNewPart)
{
	FMGPControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = InNewPart;

	if ( UMGPPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer() )
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart( InNewPart );
	}
}

UMGPPawnComponent_CharacterParts* UMGPControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if ( APawn* ControllerPawn = GetPawn<APawn>() )
	{
		return ControllerPawn->FindComponentByClass<UMGPPawnComponent_CharacterParts>();
	}
	
	return nullptr;
}

void UMGPControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if ( UMGPPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer() )
	{
		for ( auto& Entry : CharacterParts )
		{
			PawnCustomizer->RemoveCharacterPart( Entry.Handle );
		}
	}

	CharacterParts.Reset();
}

void UMGPControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* InOldPawn, APawn* InNewPawn)
{
	if ( UMGPPawnComponent_CharacterParts* OldCustomizer = InOldPawn ? InOldPawn->FindComponentByClass<UMGPPawnComponent_CharacterParts>() : nullptr )
	{
		for ( FMGPControllerCharacterPartEntry& Entry : CharacterParts )
		{
			OldCustomizer->RemoveCharacterPart( Entry.Handle );
			Entry.Handle.Reset();
		}
	}

	if ( UMGPPawnComponent_CharacterParts* NewCustomizer = InNewPawn ? InNewPawn->FindComponentByClass<UMGPPawnComponent_CharacterParts>() : nullptr )
	{
		for ( FMGPControllerCharacterPartEntry& Entry : CharacterParts )
		{
			check( !Entry.Handle.IsValid() )
			Entry.Handle = NewCustomizer->AddCharacterPart( Entry.Part );
		}
	}
}
