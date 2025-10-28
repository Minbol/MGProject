// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSystem.generated.h"

struct FUIExtension;
class UUIExtensionSubsystem;

/**
 * 
 */

UENUM(BlueprintType)
enum class EUIExtensionPointMatch : uint8
{
	ExactMatch,
	PartialMatch,
};

UENUM(BlueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed,
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()

public:
	FUIExtensionHandle() {}
	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtension>& InDataPtr)
		:ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{}
	
	friend class UUIExtensionSubsystem;

	void Unregister();
	
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

	friend FORCEINLINE uint32 GetTypeHash(const FUIExtensionHandle InHandle)
	{
		return PointerHash(InHandle.DataPtr.Get());
	}

public:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtension> DataPtr;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;
};

DECLARE_DELEGATE_TwoParams( FExtendExtensionPointDelegate, EUIExtensionAction Action, const FUIExtensionRequest& Request );
struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
public:
	bool DoesExtensionPassContract(const FUIExtension* InExtension);
	
public:
	FGameplayTag ExtensionPointTag;
	TWeakObjectPtr<UObject> ContextObject;
	TArray<UClass*> AllowedDataClasses;
	EUIExtensionPointMatch ExtensionPointTagMatchType = EUIExtensionPointMatch::ExactMatch;
	FExtendExtensionPointDelegate Callback;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()

public:
	FUIExtensionPointHandle() {}
	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr)
		: ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return false; }
	bool operator==(const FUIExtensionPointHandle& Other) const { return false; }
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }

public:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtensionPoint> DataPtr;
};

struct FUIExtension : TSharedFromThis<FUIExtension>
{
	FGameplayTag ExtensionPointTag;

	UObject* Data; // Widget
	TWeakObjectPtr<UObject> ContextObject;
	int32 Priority = INDEX_NONE;
};

template<>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void UnRegisterExtension(const FUIExtensionHandle& InHandle);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& InExtensionPointTag, UObject* InContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& InExtensionPointTag, UObject* InContextObject, UObject* InData, int32 Priority);

	void UnRegisterExtensionPoint(const FUIExtensionPointHandle& InHandle);
	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& InExtensionPointTag, UObject* InContextObject,
															EUIExtensionPointMatch InExtensionPointTagMatchType,
															const TArray<UClass*>& InAllowedDataClasses,
															FExtendExtensionPointDelegate InCallback);
	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& InExtensionPointTag, EUIExtensionPointMatch InExtensionPointTagMatchType,
													const TArray<UClass*>& InAllowedDataClasses,
													FExtendExtensionPointDelegate InCallback);

	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& InExtensionPoint);
	void NotifyExtensionPointsOfExtension(EUIExtensionAction InAction, TSharedPtr<FUIExtension>& InExtension);

private:
	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& InExtension);
	
public:
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;

	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;
};
