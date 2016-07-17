// Asset.h
// Base class for assets that can be loaded,
// such as images, sounds, meshes, etc.

#pragma once
#include <memory>
#include "ObjectMacros.h"

class Game;

class Asset : public std::enable_shared_from_this<Asset>
{
public:
	Asset(class Game& game);
	virtual ~Asset();
protected:
	// Change the access level to the protected
	using std::enable_shared_from_this<Asset>::shared_from_this;
	virtual bool Load(const char* fileName, class AssetCache* cache) = 0;
	class Game& mGame;
};

DECL_PTR(Asset);
