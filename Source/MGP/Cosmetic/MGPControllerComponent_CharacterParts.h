// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "MGPControllerComponent_CharacterParts.generated.h"

class UMGPPawnComponent_CharacterParts;
/**
 * 
 */
USTRUCT()
struct FMGPControllerCharacterPartEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMGPCharacterPart Part;

	UPROPERTY(EditAnywhere)
	FMGPCharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class MGP_API UMGPControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()

public:
	UMGPControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = Cosmetic)
	void AddCharacterPart(const FMGPCharacterPart& InNewPart);
	void AddCharacterPartInternal(const FMGPCharacterPart& InNewPart);

protected:
	UMGPPawnComponent_CharacterParts* GetPawnCustomizer() const;
	void RemoveAllCharacterParts();
	
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* InOldPawn, APawn* InNewPawn);

public:
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FMGPControllerCharacterPartEntry> CharacterParts;
};
