// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCameraMode.h"

#include "MGPCameraComponent.h"
#include "MGPPlayerCameraManager.h"

FMGPCameraModeView::FMGPCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(MGP_CAMERA_DEFAULT_FOV)
{
	
}

void FMGPCameraModeView::Blend(const FMGPCameraModeView& InOther, float InOtherWeight)
{
	if ( InOtherWeight <= 0.0f )
	{
		return;
	}
	else if ( InOtherWeight >= 1.0f )
	{
		*this = InOther;
		return;
	}

	Location = FMath::Lerp( Location, InOther.Location, InOtherWeight );

	const FRotator DeltaRotator = ( InOther.ControlRotation - ControlRotation ).GetNormalized();
	Rotation = Rotation + ( InOtherWeight * DeltaRotator );

	const FRotator DeltaControlRotation = ( InOther.ControlRotation - ControlRotation ).GetNormalized();
	ControlRotation = ControlRotation + ( InOtherWeight * DeltaControlRotation );

	FieldOfView = FMath::Lerp( FieldOfView, InOther.FieldOfView, InOtherWeight );
}

UMGPCameraMode::UMGPCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView  = MGP_CAMERA_DEFAULT_FOV;
	ViewPitchMin = MGP_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = MGP_CAMERA_DEFAULT_PITCH_MAX;
	
	BlendTime   = 0.0f;
	BlendAlpha  = 1.0f;
	BlendWeight = 1.0f;

	BlendFunctionType = EMGPCameraModeBlendFunctionType::EaseOut;
	BlendExponent = 4.0f;
}

void UMGPCameraMode::UpdateCameraMode(float InDeltaTime)
{
	UpdateView( InDeltaTime );
	UpdateBlending( InDeltaTime );
}

void UMGPCameraMode::UpdateView(float InDeltaTime)
{
	const FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle( PivotRotation.Pitch, ViewPitchMin, ViewPitchMax );

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView     = FieldOfView;
}

void UMGPCameraMode::UpdateBlending(float InDeltaTime)
{
	if ( BlendTime > 0.0f )
	{
		BlendTime += InDeltaTime / BlendTime;
	}
	else
	{
	 	BlendAlpha = 1.0f;
	}

	const float Exponent = BlendExponent > 0.0f ? BlendExponent : 1.0f;
	switch ( BlendFunctionType )
	{
	case EMGPCameraModeBlendFunctionType::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EMGPCameraModeBlendFunctionType::EaseIn:
		BlendWeight = FMath::InterpEaseIn( 0.0f, 1.0f, BlendAlpha, Exponent );
		break;
	case EMGPCameraModeBlendFunctionType::EaseOut:
		BlendWeight = FMath::InterpEaseOut( 0.0f, 1.0f, BlendAlpha, Exponent );
		break;
	case EMGPCameraModeBlendFunctionType::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut( 0.0f, 1.0f, BlendAlpha, Exponent );
		break;
	default: break;
	}
}

FVector UMGPCameraMode::GetPivotLocation()
{
	const AActor* TargetActor = GetTargetActor();
	check( TargetActor );

	if ( const APawn* TargetPawn = Cast<APawn>(TargetActor) )
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UMGPCameraMode::GetPivotRotation()
{
	const AActor* TargetActor = GetTargetActor();
	check( TargetActor );

	if ( const APawn* TargetPawn = Cast<APawn>(TargetActor) )
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

AActor* UMGPCameraMode::GetTargetActor()
{
	const UMGPCameraComponent* UMGPCameraComponent = GetMGPCameraComponent();
	return UMGPCameraComponent->GetTargetActor();
}

UMGPCameraComponent* UMGPCameraMode::GetMGPCameraComponent() const
{
	return CastChecked<UMGPCameraComponent>( GetOuter() );
}

UMGPCameraModeStack::UMGPCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMGPCameraMode* UMGPCameraModeStack::GetCameraModeInstance(const TSubclassOf<UMGPCameraMode>& InCameraModeClass) 
{
	check( InCameraModeClass );

	for ( auto CameraMode : CameraModeInstances )
	{
		if ( IsValid( CameraMode ) && CameraMode.GetClass() == InCameraModeClass )
		{
			return CameraMode;
		}
	}

	UMGPCameraMode* NewCameraMode = NewObject<UMGPCameraMode>( GetOuter(), InCameraModeClass, NAME_None, RF_NoFlags );
	check( NewCameraMode );
	
	CameraModeInstances.Emplace( NewCameraMode );
	
	return NewCameraMode;
}

void UMGPCameraModeStack::PushCameraMode(const TSubclassOf<UMGPCameraMode>& InCameraMode)
{
	if ( !IsValid( InCameraMode ) )
	{
		return;
	}

	UMGPCameraMode* CameraMode = GetCameraModeInstance( InCameraMode );
	check( CameraMode );

	int32 StackSize = CameraModeStack.Num();
	if ( StackSize > 0 && CameraModeStack[ 0 ] == CameraMode )
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0;

	for ( int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex )
	{
		if ( CameraModeStack[ StackIndex ] == CameraMode )
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= ( 1.0f - CameraModeStack[ StackIndex ]->BlendWeight );
		}
	}

	if ( ExistingStackIndex != INDEX_NONE )
	{
		CameraModeStack.RemoveAt( ExistingStackIndex );
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ( CameraMode->BlendTime > 0.0f ) && ( StackSize > 0 );
	const float BlendWeight = bShouldBlend ? ExistingStackContribution : 1.0f;
	CameraMode->BlendWeight = BlendWeight;
	
	CameraModeStack.Insert( CameraMode, 0 );
	CameraModeStack.Last()->BlendWeight = 1.f;
}

void UMGPCameraModeStack::EvaluateStack(float InDeltaTime, FMGPCameraModeView& OutModeView)
{
	UpdateStack( InDeltaTime );
	BlendStack( OutModeView );
}

void UMGPCameraModeStack::UpdateStack(float InDeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if ( StackSize <= 0 )
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for ( int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex )
	{
		UMGPCameraMode* CameraMode = CameraModeStack[ StackIndex ];
		check( CameraMode );

		CameraMode->UpdateCameraMode( InDeltaTime );

		if ( CameraMode->BlendWeight >= 1.0f )
		{
			RemoveIndex = StackSize + 1;
			RemoveCount = StackSize - RemoveIndex;
			break;
		}
	}

	if ( RemoveCount > 0 )
	{
		CameraModeStack.RemoveAt( RemoveIndex, RemoveCount );
	}
}

void UMGPCameraModeStack::BlendStack(FMGPCameraModeView& OutModeView)
{
	const int32 StackSize = CameraModeStack.Num();
	if ( StackSize <= 0 )
	{
		return;
	}

	const UMGPCameraMode* CameraMode = CameraModeStack[ StackSize - 1 ];
	check( CameraMode );

	OutModeView = CameraMode->View;

	for ( int32 StackIndex = StackSize - 2; StackIndex >= 0; --StackIndex )
	{
		if ( !CameraModeStack.IsValidIndex( StackIndex ) )
			continue;
		
		CameraMode = CameraModeStack[ StackIndex ];
		check( CameraMode );

		OutModeView.Blend( CameraMode->View, CameraMode->BlendWeight );
	}
}
