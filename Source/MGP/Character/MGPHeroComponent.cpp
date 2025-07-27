// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPHeroComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "MGPPawnExtensionComponent.h"
#include "MGP/MGPGameplayTags.h"
#include "MGP/Camera/MGPCameraComponent.h"
#include "MGP/Player/MGPPlayerController.h"
#include "MGP/Player/MGPPlayerState.h"
#include "MGP/GameFeatures/MGPGameFeatureAction_AddInputContextMapping.h"
#include "MGP/Input/MGPInputComponent.h"

const FName UMGPHeroComponent::NAME_ActorFeatureName( "Hero" );
const FName UMGPHeroComponent::NAME_BindInputsNow( "BindInputsNow" );

UMGPHeroComponent::UMGPHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick		   = false;
}

void UMGPHeroComponent::OnRegister()
{
	Super::OnRegister();

	if ( !GetPawn<APawn>() )
		return;

	RegisterInitStateFeature();
}

void UMGPHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged( UMGPPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false );
	ensure( TryToChangeInitState( FMGPGameplayTags::Get().InitState_Spawned ) );
	CheckDefaultInitialization();
}

void UMGPHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UMGPHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	if ( Params.FeatureName == UMGPPawnExtensionComponent::NAME_ActorFeatureName )
	{
		if ( Params.FeatureState == InitTags.InitState_Spawned )
		{
			CheckDefaultInitialization();
		}
	}
}

bool UMGPHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check( Manager );

	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	AMGPPlayerState* MGPPS = GetPlayerState<AMGPPlayerState>();
	APawn* Pawn = GetPawn<APawn>();

	if ( !CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned )
	{
		if ( Pawn != nullptr )
		{
			return true;
		}
	}
	
	if ( CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable )
	{
		if ( MGPPS == nullptr )
		{
			return false;
		}
		
		return true;
	}

	if ( CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized )
	{
		return MGPPS && Manager->HasFeatureReachedInitState( Pawn, UMGPPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized );
	}

	if ( CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady )
	{
		return true;
	}

	return false;
}

void UMGPHeroComponent::CheckDefaultInitialization()
{
	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	ContinueInitStateChain( StateChain );
}

void UMGPHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FMGPGameplayTags& InitTags = FMGPGameplayTags::Get();
	if ( CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized )
	{
		const APawn* Pawn = GetPawn<APawn>();
		const AMGPPlayerState* MGPPS = GetPlayerState<AMGPPlayerState>();
		if ( !IsValid( Pawn ) || !IsValid( MGPPS ) )
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UMGPPawnData* PawnData = nullptr;
		if ( UMGPPawnExtensionComponent* PawnExtensionComponent = UMGPPawnExtensionComponent::FindPawnExtensionComponent( Pawn ) )
		{
			PawnData = PawnExtensionComponent->GetPawnData<UMGPPawnData>();
		}

		if ( bIsLocallyControlled && IsValid( PawnData ) )
		{
			if ( UMGPCameraComponent* CameraComponent = UMGPCameraComponent::FindCameraComponent( Pawn ) )
			{
				CameraComponent->OnDetermineCameraModeDelegate.BindUObject( this, &ThisClass::DetermineCameraMode );
			}
		}

		if ( AMGPPlayerController* MGPPC = GetController<AMGPPlayerController>() )
		{
			if ( Pawn->InputComponent != nullptr )
			{
				InitializePlayerInput( Pawn->InputComponent );
			}
		}
	}
}

TSubclassOf<UMGPCameraMode> UMGPHeroComponent::DetermineCameraMode()
{
	const APawn* Pawn = GetPawn<APawn>();
	if ( !IsValid( Pawn ) )
	{
		return nullptr;
	}

	if ( UMGPPawnExtensionComponent* PawnExtensionComponent = UMGPPawnExtensionComponent::FindPawnExtensionComponent( Pawn ) )
	{
		if ( const UMGPPawnData* PawnData = PawnExtensionComponent->GetPawnData<UMGPPawnData>() )
		{
			return PawnData->DefaultCameraMode;
		}
	}
	
	return nullptr;
}

void UMGPHeroComponent::InitializePlayerInput(UInputComponent* InPlayerInputComponent)
{
	check( InPlayerInputComponent );
	const APawn* Pawn = GetPawn<APawn>();
	check( Pawn );

	const APlayerController* PC = GetController<APlayerController>();
	check( PC );

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check( LP );

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check( Subsystem );

	Subsystem->ClearAllMappings();

	if ( const UMGPPawnExtensionComponent* PawnExtComp = UMGPPawnExtensionComponent::FindPawnExtensionComponent( Pawn ) )
	{
		if( const UMGPPawnData* PawnData = PawnExtComp->GetPawnData<UMGPPawnData>() )
		{
			if ( const UMGPInputConfig* InputConfig = PawnData->InputConfig )
			{
				for ( const FInputMappingContextAndPriority& Mapping : DefaultInputMappings )
				{
					if ( UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous() )
					{
						if ( Mapping.bRegisterWithSettings )
						{
							if ( UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings() )
							{
								Settings->RegisterInputMappingContext( IMC );
							}
							
							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;				
							Subsystem->AddMappingContext( IMC, Mapping.Priority, Options );
						}
					}
				}

				if ( UMGPInputComponent* MGPIC = Cast<UMGPInputComponent>( InPlayerInputComponent ) )
				{
					const FMGPGameplayTags& GameplayTags = FMGPGameplayTags::Get();
					
					MGPIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false );
					MGPIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false );
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent( const_cast<APlayerController*>( PC ), NAME_BindInputsNow );
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent( const_cast<APawn*>( Pawn ), NAME_BindInputsNow );
}

void UMGPHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UMGPHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}
