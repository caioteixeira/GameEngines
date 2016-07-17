#include "ITPEnginePCH.h"
#include <SDL/SDL_mixer.h>

IMPL_COMPONENT(AudioComponent, Component, 100);

AudioComponent::AudioComponent(Actor& owner)
	:Component(owner)
{

}

SoundCue AudioComponent::PlaySoundCue(SoundPtr sound, bool looping)
{
	int loopval = looping ? -1 : 0;
	return SoundCue(Mix_PlayChannel(-1, sound->GetData(), loopval));
}

void SoundCue::Pause()
{
	if (mChannel != -1)
	{
		Mix_Pause(mChannel);
	}
}

void SoundCue::Resume()
{
	if (mChannel != -1)
	{
		Mix_Resume(mChannel);
	}
}

void SoundCue::Stop()
{
	if (mChannel != -1)
	{
		Mix_HaltChannel(mChannel);
	}
}

bool SoundCue::IsPlaying()
{
	if (mChannel != -1)
	{
		return Mix_Playing(mChannel) == 1;
	}
	else
	{
		return false;
	}
}

bool SoundCue::IsPaused()
{
	if (mChannel != -1)
	{
		return Mix_Paused(mChannel) == 1;
	}
	else
	{
		return false;
	}
}
