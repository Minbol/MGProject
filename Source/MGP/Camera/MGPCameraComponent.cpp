// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCameraComponent.h"

#include "MGPCameraMode.h"

UMGPCameraComponent::UMGPCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), CameraModeStack(nullptr)
{
}

void UMGPCameraComponent::OnRegister()
{
	Super::OnRegister();

	if ( !IsValid( CameraModeStack ) )
	{
		CameraModeStack = NewObject<UMGPCameraModeStack>( this );
	}
}

void UMGPCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	UpdateCameraModes();

	FMGPCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack( DeltaTime, CameraModeView );

	if ( APawn* Targetpawn = Cast<APawn>(GetTargetActor() ) )
	{
		if ( APlayerController* PC = Targetpawn->GetController<APlayerController>() )
		{
			PC->SetControlRotation( CameraModeView.ControlRotation );
		}
	}

	SetWorldLocationAndRotation( CameraModeView.Location, CameraModeView.Rotation );
	FieldOfView = CameraModeView.FieldOfView;
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UMGPCameraComponent::UpdateCameraModes()
{
	check( CameraModeStack );

	if ( OnDetermineCameraModeDelegate.IsBound() )
	{
		if ( TSubclassOf<UMGPCameraMode> CameraMode = OnDetermineCameraModeDelegate.Execute() )
		{
			CameraModeStack->PushCameraMode( CameraMode );
		}
	}
}
