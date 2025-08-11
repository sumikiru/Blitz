// Copyright sumikiru. All Rights Reserved,


#include "BlitzAssetManager.h"

#include "Blitz/BlitzLogChannels.h"

UBlitzAssetManager::UBlitzAssetManager()
{
}

/*
UBlitzAssetManager& UBlitzAssetManager::Get()
{
	check(GEngine);

	if (UBlitzAssetManager* Singleton = Cast<UBlitzAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogBlitz, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to BlitzAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UBlitzAssetManager>();
}

void UBlitzAssetManager::DumpLoadedAssets()
{
	UE_LOG(LogBlitz, Log, TEXT("========== Start Dumping Loaded Assets =========="));

	for (const UObject* LoadedAsset : Get().LoadedAssets)
	{
		UE_LOG(LogBlitz, Log, TEXT("	%s"), *GetNameSafe(LoadedAsset));
	}

	UE_LOG(LogBlitz, Log, TEXT("... %d assets in loaded pool"), Get().LoadedAssets.Num());
	UE_LOG(LogBlitz, Log, TEXT("========== Finish Dumping Loaded Assets =========="));
}

/*const UBlitzPawnData* UBlitzAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}#1#

UObject* UBlitzAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()),
			                                       nullptr,
			                                       FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UBlitzAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UBlitzAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

void UBlitzAssetManager::StartInitialLoading()
{
}

#if WITH_EDITOR
void UBlitzAssetManager::PreBeginPIE(bool bStartSimulate)
{
	Super::PreBeginPIE(bStartSimulate);
}
#endif
*/
