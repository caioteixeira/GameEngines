// AssetCache.h
// The asset cache is used to ensure that each particular asset
// file is only loaded once at most.
// Generally, you will call the Load function on the AssetCache
// member in Game

#pragma once
#include "Asset.h"
#include <string>
#include <unordered_map>

class Game;

class AssetCache
{
public:
	AssetCache(Game& game, const char* rootDirectory);

	// Template magics to load an arbitrary Asset into the cache
	// Generally will be used like:
	// assetCache.Load<AssetClass>("path/file.name")
	template <typename T>
	std::shared_ptr<T> Load(const char* fileName)
	{
		std::string path(mRoot);
		path += fileName;
		auto iter = mAssetMap.find(path);

		if (iter != mAssetMap.end())
		{
			return std::static_pointer_cast<T>(iter->second);
		}
		else
		{
			std::shared_ptr<T> asset = T::StaticLoad(path.c_str(), this, mGame);
			if (asset)
			{
				mAssetMap.emplace(path, asset);
			}
			return asset;
		}
	}

	template <typename T>
	std::shared_ptr<T> Load(const std::string& fileName)
	{
		return Load<T>(fileName.c_str());
	}
	
	void Clear();
private:
	std::unordered_map<std::string, AssetPtr> mAssetMap;
	Game& mGame;
	const char* mRoot;
};
