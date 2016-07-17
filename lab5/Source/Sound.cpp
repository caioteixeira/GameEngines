#include "ITPEnginePCH.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

Sound::Sound(class Game& game)
	:Asset(game)
	,mData(nullptr)
{

}

Sound::~Sound()
{
	if (mData)
	{
		Mix_FreeChunk(mData);
	}
}

bool Sound::Load(const char* fileName, class AssetCache* cache)
{
	mData = Mix_LoadWAV(fileName);
	
	if (!mData)
	{
		SDL_Log("Failed to load sound %s", fileName);
		return false;
	}

	return true;
}
