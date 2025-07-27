// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPPlayerCharacter.h"

#include "MGPPawnExtensionComponent.h"
#include "MGP/Camera/MGPCameraComponent.h"

// Sets default values
AMGPPlayerCharacter::AMGPPlayerCharacter()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick		   = false;

	PawnExtensionComponent = CreateDefaultSubobject<UMGPPawnExtensionComponent>( TEXT( "PawnExtensionComponent" ) );
	CameraComponent = CreateDefaultSubobject<UMGPCameraComponent>( TEXT( "MainCameraComponent" ) );
	CameraComponent->SetRelativeLocation( FVector( -300.0f, 0.0f, 75.0f ) );
}

// Called when the game starts or when spawned
void AMGPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMGPPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMGPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if ( PawnExtensionComponent != nullptr )
	{
		PawnExtensionComponent->SetupPlayerInputComponent();
	}
}

