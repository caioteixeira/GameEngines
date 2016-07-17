#pragma once
#include "Component.h"
#include "Sound.h"

class SoundCue
{
public:
	SoundCue()
		:mChannel(-1)
	{}

	SoundCue(int channel)
		:mChannel(channel)
	{}

	void Pause();
	void Resume();
	void Stop();

	bool IsPlaying();
	bool IsPaused();
private:
	int mChannel;
};

class AudioComponent : public Component
{
	DECL_COMPONENT(AudioComponent, Component);
public:
	AudioComponent(Actor& owner);

	SoundCue PlaySoundCue(SoundPtr sound, bool looping = false);
};

DECL_PTR(AudioComponent);
