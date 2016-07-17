#include "ITPEnginePCH.h"

AssetCache::AssetCache(Game& game, const char* rootDirectory)
	:mGame(game)
	,mRoot(rootDirectory)
{

}

void AssetCache::Clear()
{
	mAssetMap.clear();
}
