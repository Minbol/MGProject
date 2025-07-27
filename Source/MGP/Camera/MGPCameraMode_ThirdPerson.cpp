// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

UMGPCameraMode_ThirdPerson::UMGPCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPCameraMode_ThirdPerson::UpdateView(float InDeltaTime)
{
	const FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle( PivotRotation.Pitch, ViewPitchMin, ViewPitchMax );

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView     = FieldOfView;

	if ( TargetOffsetCurve )
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue( PivotRotation.Pitch );
		View.Location = PivotLocation + PivotRotation.RotateVector( TargetOffset );
	}
}
