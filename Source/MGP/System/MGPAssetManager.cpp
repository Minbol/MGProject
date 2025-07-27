// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPAssetManager.h"

#include "MGP/MGPGameplayTags.h"

UMGPAssetManager::UMGPAssetManager()
{
}

UMGPAssetManager& UMGPAssetManager::Get()
{
	check( GEngine );

	if ( const TObjectPtr<UMGPAssetManager> Manager = Cast<UMGPAssetManager>( GEngine->AssetManager ) )
	{
		return *Manager;
	}

	return *NewObject<UMGPAssetManager>();
}

bool UMGPAssetManager::ShouldLogAssetLoad()
{
	const TCHAR* CommandLine = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param( CommandLine, TEXT( "LogAssetLoads" ) );
	return bLogAssetLoads;
}

void UMGPAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FMGPGameplayTags::InitializeNativeTags();
}

void UMGPAssetManager::AddLoadedAsset(const TObjectPtr<UObject>& InAsset)
{
	if ( ensureAlways( InAsset ) )
	{
		FScopeLock Lock( &SyncObject );
		LoadedAssets.Emplace( InAsset );
	}
}

TObjectPtr<UObject> UMGPAssetManager::SynchronousLoadAsset(const FSoftObjectPath& InAssetPath)
{
	if ( !InAssetPath.IsValid() )
		return nullptr;

	if ( ShouldLogAssetLoad() )
	{
		TUniquePtr<FScopeLogTime> LogTimePtr = MakeUnique<FScopeLogTime>( *FString::Printf( TEXT( "SynchronousLoadAsset [%s]" ), *InAssetPath.ToString(), nullptr, FScopeLogTime::ScopeLog_Seconds ) );
	}

	if ( UAssetManager::IsValid() )
	{
		return UAssetManager::GetStreamableManager().LoadSynchronous( InAssetPath );
	}

	return InAssetPath.TryLoad();
}
