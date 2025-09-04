// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameplayAbility_RangeWeapon.h"

#include "MGP/Equipment/Weapon/MGPRangedWeaponInstance.h"
#include "MGP/Physics/MGPCollisionChannels.h"

UMGPGameplayAbility_RangeWeapon::UMGPGameplayAbility_RangeWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGPGameplayAbility_RangeWeapon::StartRangeWeaponTargeting()
{
	if ( CurrentActorInfo == nullptr )
	{
		return;
	}

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	if ( AvatarActor == nullptr )
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	if ( AbilitySystemComponent == nullptr )
	{
		return;
	}

	TArray<FHitResult> HitResults;
	PerformLocalTargeting( HitResults );

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.UniqueId = 0;
}

void UMGPGameplayAbility_RangeWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>( GetAvatarActorFromActorInfo() );
	UMGPRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if ( AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData )
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;

		const FTransform TargetTransform = GetTargetingTransform( AvatarPawn, EMGPAbilityTargetingSource::CameraTowardsFocus );
		InputData.AimDir = TargetTransform.GetUnitAxis( EAxis::X );
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;

		TraceBulletsInCartridge( InputData, OutHits );
	}
}

void UMGPGameplayAbility_RangeWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput InInputData, TArray<FHitResult>& OutHits)
{
	UMGPRangedWeaponInstance* WeaponData = InInputData.WeaponData;
	if ( WeaponData == nullptr )
	{
		return;
	}

	const FVector BulletDir = InInputData.AimDir;
	const FVector EndTrace = InInputData.StartTrace + ( BulletDir * WeaponData->MaxDamageRange );

	FVector HitLoc = EndTrace;
	TArray<FHitResult> AllImpacts;
	FHitResult Impact = DoSingleBulletTrace( InInputData.StartTrace, EndTrace, WeaponData->BulletTraceWeaponRadius, false, AllImpacts );
	
	if ( Impact.GetActor() )
	{
		if ( AllImpacts.Num() > 0 )
		{
			OutHits.Append( AllImpacts );
		}

		HitLoc = Impact.ImpactPoint;
	}

	if ( OutHits.IsEmpty() )
	{
		if ( !Impact.bBlockingHit )
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Emplace( Impact );
	}
}

void UMGPGameplayAbility_RangeWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams InParams)
{
	if ( const AActor* Avatar = GetAvatarActorFromActorInfo() )
	{
		TArray<AActor*> AttachedActors;
		Avatar->GetAttachedActors( AttachedActors );
		InParams.AddIgnoredActors( AttachedActors );
	}
}

int32 UMGPGameplayAbility_RangeWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& InHitResults)
{
	for ( int32 Idx = 0; Idx < InHitResults.Num(); ++Idx )
	{
		const FHitResult& CurHitResult = InHitResults[ Idx ];
		if ( CurHitResult.HitObjectHandle.DoesRepresentClass( APawn::StaticClass() ) )
		{
			return  Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();
			if ( HitActor && HitActor->GetAttachParentActor() && Cast<APawn>( HitActor->GetAttachParentActor() ) )
			{
				return Idx;
			}
		}
	}

	return INDEX_NONE;
}

UMGPRangedWeaponInstance* UMGPGameplayAbility_RangeWeapon::GetWeaponInstance()
{
	return Cast<UMGPRangedWeaponInstance>( GetAssociatedEquipment() );
}

FTransform UMGPGameplayAbility_RangeWeapon::GetTargetingTransform(APawn* InSourcePawn, EMGPAbilityTargetingSource InSource)
{
	if ( InSourcePawn == nullptr )
	{
		return FTransform();
	}

	if ( InSource != EMGPAbilityTargetingSource::CameraTowardsFocus )
	{
		return FTransform();
	}

	AController* Controller = InSourcePawn->Controller;
	if ( Controller == nullptr )
	{
		return FTransform();
	}

	constexpr double FocalDistance = 1024.f;
	const APlayerController* PC = Cast<APlayerController>( Controller );
	if ( PC == nullptr )
	{
		return FTransform();
	}

	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint( CamLoc, CamRot );
	const FVector AimDir = CamRot.Vector().GetSafeNormal();
	const FVector FocalLoc = CamLoc + (AimDir * FocalDistance);

	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	const FVector FinalCamLoc = FocalLoc + ( ( ( WeaponLoc - FocalLoc ) | AimDir ) * AimDir );

	return FTransform( CamRot, FinalCamLoc );
}

FVector UMGPGameplayAbility_RangeWeapon::GetWeaponTargetingSourceLocation() const
{
	const APawn* AvatarPawn = Cast<APawn>( GetAvatarActorFromActorInfo() );
	if ( AvatarPawn == nullptr )
	{
		return FVector();
	}

	return AvatarPawn->GetActorLocation();
}

FHitResult UMGPGameplayAbility_RangeWeapon::DoSingleBulletTrace(const FVector& InStartTrace, const FVector& EndTrace, float InSweepRadius, bool bInSimulated, TArray<FHitResult>& OutHits)
{
	FHitResult ResultImpact;

	if ( FindFirstPawnHitResult( OutHits ) == INDEX_NONE )
	{
		ResultImpact = WeaponTrace( InStartTrace, EndTrace, 0.f, bInSimulated, OutHits );
	}

	if ( FindFirstPawnHitResult( OutHits ) == INDEX_NONE )
	{
		if ( InSweepRadius > 0.f )
		{
			TArray<FHitResult> SweepHits;
			ResultImpact = WeaponTrace( InStartTrace, EndTrace, InSweepRadius, bInSimulated, OutHits );

			const int32 FirstPawnIdx = FindFirstPawnHitResult( SweepHits );
			if ( SweepHits.IsValidIndex( FirstPawnIdx ) )
			{
				bool bUseSweepHits = true;
				for ( int32 Idx = 0; Idx < FirstPawnIdx; ++Idx )
				{
					const FHitResult& CurHitResult = SweepHits[ Idx ];
					auto Pred = [ &CurHitResult ]( const FHitResult& Other )
					{
						return Other.HitObjectHandle == CurHitResult.HitObjectHandle;	
					};

					if ( CurHitResult.bBlockingHit && OutHits.ContainsByPredicate( Pred ) )
					{
						bUseSweepHits = false;
						break;
					}
				}

				if ( bUseSweepHits )
				{
					OutHits = SweepHits;
				}
			}
		}
	}

	return ResultImpact;
}

FHitResult UMGPGameplayAbility_RangeWeapon::WeaponTrace(const FVector& InStartTrace, const FVector& InEndTrace, float InSweepRadius, bool bInSimulated, TArray<FHitResult>& OutHits)
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams TraceParams( SCENE_QUERY_STAT( WeaponTrace ), true, GetAvatarActorFromActorInfo() );
	TraceParams.bReturnPhysicalMaterial = true;

	AddAdditionalTraceIgnoreActors( TraceParams );

	const ECollisionChannel TraceChannel = DetermineTraceChannel( TraceParams, bInSimulated );
	if ( InSweepRadius > 0.f )
	{
		GetWorld()->SweepMultiByChannel( HitResults, InStartTrace, InEndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere( InSweepRadius ), TraceParams );
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel( HitResults, InStartTrace, InEndTrace, TraceChannel, TraceParams );
	}

	FHitResult Hit( ForceInit );
	if ( HitResults.Num() > 0 )
	{
		for ( FHitResult& CurHitResult : HitResults )
		{
			auto Pred = [ &CurHitResult ]( const FHitResult& Other )
			{
				return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
			};

			if ( !OutHits.ContainsByPredicate( Pred ) )
			{
				OutHits.Emplace( CurHitResult );
			}
		}

		Hit = OutHits.Last();
	}
	else
	{
		Hit.TraceStart = InStartTrace;
		Hit.TraceEnd = InEndTrace;
	}

	return Hit;
}

ECollisionChannel UMGPGameplayAbility_RangeWeapon::DetermineTraceChannel(FCollisionQueryParams& InParams, bool bInSimulated) const
{
	return MGP_TRACECHANNEL_WEAPON;
}
