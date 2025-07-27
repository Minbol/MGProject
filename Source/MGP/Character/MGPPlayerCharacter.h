// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "MGPPlayerCharacter.generated.h"

class UMGPCameraComponent;
class UMGPPawnExtensionComponent;

UCLASS()
class MGP_API AMGPPlayerCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMGPPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;\


public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "MGP|Character" )
	TObjectPtr<UMGPPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "MGP|Character" )
	TObjectPtr<UMGPCameraComponent> CameraComponent;
};
