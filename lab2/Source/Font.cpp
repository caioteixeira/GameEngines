#include "ITPEnginePCH.h"
#include <vector>

Font::Font(class Game& game)
	:Asset(game)
{

}

Font::~Font()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

bool Font::Load(const char* fileName, class AssetCache* cache)
{
	std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName, size);
		if (!font)
		{
			SDL_Log("Failed to load font %s in size %d", fileName, size);
			return false;
		}
		mFontData.emplace(size, font);
	}

	return true;
}

TTF_Font* Font::GetFontData(int pointSize)
{
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		return iter->second;
	}

	return nullptr;
}
