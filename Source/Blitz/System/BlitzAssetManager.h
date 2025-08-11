// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BlitzAssetManager.generated.h"

class UBlitzPawnData;
/**
 * Game implementation of the asset manager that overrides functionality and stores game-specific types.
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 * todo: This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS(Config = Game)
class BLITZ_API UBlitzAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UBlitzAssetManager();
};

	/*
	// Returns the AssetManager singleton object.
	static UBlitzAssetManager& Get();

	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	// TSoftClassPtr can be used in UProperties for blueprint subclasses(BP), but TSoftObjectPtr can only be used in UProperties(CPP)
	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetClassPointer, bool bKeepInMemory = true);

	// Logs all assets currently loaded and tracked by the asset manager.
	static void DumpLoadedAssets();

	// todo: const UBlitzGameData& GetGameData();
	
	//const UBlitzPawnData* GetDefaultPawnData() const;

protected:
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);

	//~UAssetManager interface
	virtual void StartInitialLoading() override;
#if WITH_EDITOR
	virtual void PreBeginPIE(bool bStartSimulate) override;
#endif
	//~End of UAssetManager interface

	// Pawn data used when spawning player pawns if there isn't one set on the player state.
	/*UPROPERTY(Config)
	TSoftObjectPtr<UBlitzPawnData> DefaultPawnData;#1#

private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UBlitzAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		// if LoadedAsset is nullptr, Synchronously loaded asset from asset path
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			// 和checkf相比，ensureAlwaysMsgf针对非关键资源，即使条件为false仍然不会crash，只会输出警告，且在所有版本生效
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		// if LoadedAsset is valid now and needed to keep in memory, add it to LoadedAssets list.
		if (LoadedAsset && bKeepInMemory)
		{
			Get().AddLoadedAsset(LoadedAsset);
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UBlitzAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetClassPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetClassPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetClassPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetClassPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}*/
