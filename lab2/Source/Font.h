#pragma once
#include "Asset.h"
#include <unordered_map>
#include <SDL/SDL_ttf.h>

class Font : public Asset
{
	DECL_ASSET(Font, Asset);
public:
	Font(class Game& game);
	virtual ~Font();

	bool Load(const char* fileName, class AssetCache* cache) override;

	TTF_Font* GetFontData(int pointSize);
private:
	std::unordered_map<int, TTF_Font*> mFontData;
};

DECL_PTR(Font);
