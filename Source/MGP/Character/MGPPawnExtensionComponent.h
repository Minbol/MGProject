// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGPPawnData.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "MGPPawnExtensionComponent.generated.h"

class UMGPPawnData;
/**
 * 
 */
UCLASS()
class MGP_API UMGPPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UMGPPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	static UMGPPawnExtensionComponent* FindPawnExtensionComponent(const AActor* InActor) { return InActor != nullptr ? InActor->FindComponentByClass<UMGPPawnExtensionComponent>() : nullptr; }
	void SetPawnData(const UMGPPawnData* InPawnData);
	void SetupPlayerInputComponent();
	
	template<typename T>
	const T* GetPawnData() const { return Cast<T>( PawnData ); }

	UPROPERTY(EditInstanceOnly, Category = "MGP|Pawn")
	TObjectPtr<const UMGPPawnData> PawnData;
};
