// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

class UCommonLocalPlayer;
struct FUIExtensionHandle;
/**
 * 
 */
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	void ResetExtensionPoint();
	void OnAddOrRemoveExtension(EUIExtensionAction InAction, const FUIExtensionRequest& InRequest);
	void RegisterExtensionPoint();
	void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* InLocalPlayer, APlayerState* InPlayerState);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIExtension")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIExtension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIExtension")
	TArray<TObjectPtr<UClass>> DataClass;

	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

	TArray<FUIExtensionPointHandle> ExtensionPointHandles;
};
