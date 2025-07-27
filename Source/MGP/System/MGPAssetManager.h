// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MGPAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MGP_API UMGPAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMGPAssetManager();

	static UMGPAssetManager& Get();
	static bool ShouldLogAssetLoad();

	virtual void StartInitialLoading();

	void AddLoadedAsset(const TObjectPtr<UObject>& InAsset);
	TObjectPtr<UObject> SynchronousLoadAsset(const FSoftObjectPath& InAssetPath);

private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection SyncObject;

public:
	template <typename T>
	T* GetAsset(const TSoftObjectPtr<T>& InAsset, bool bInKeepsInMemory = true)
	{
		T* LoadedAsset = nullptr;
		const FSoftObjectPath& AssetPath = InAsset.ToSoftObjectPath();

		if ( !AssetPath.IsValid() )
			return nullptr;

		LoadedAsset = InAsset.Get();
		if ( LoadedAsset == nullptr )
		{
			LoadedAsset = Cast<T>(SynchronousLoadAsset( AssetPath ) );
		}

		if ( LoadedAsset && bInKeepsInMemory )
		{
			Get().AddLoadedAsset( Cast<UObject>( LoadedAsset ) );
		}

		return LoadedAsset;
	}

	template <typename T>
	TSubclassOf<T> GetSubClass(const TSoftObjectPtr<T>& InAsset, bool bInKeepsInMemory = true)
	{
		TSubclassOf<T> LoadedSubClass;
		const FSoftObjectPath& AssetPath = InAsset.ToSoftObjectPath();

		if ( !AssetPath.IsValid() )
			return nullptr;

		LoadedSubClass = InAsset.Get();
		if ( LoadedSubClass == nullptr )
		{
			LoadedSubClass = Cast<UClass>(SynchronousLoadAsset( AssetPath ) );
		}

		if ( LoadedSubClass && bInKeepsInMemory )
		{
			Get().AddLoadedAsset( Cast<UObject>( LoadedSubClass ) );
		}

		return LoadedSubClass;
	}
};
