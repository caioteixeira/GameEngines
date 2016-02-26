// FrameTimer.h
// Used to grab the elapsed frame time each frame

#pragma once

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <chrono>
#endif

class FrameTimer
{
public:
	FrameTimer();

	void Start();

	// Get the elapsed frame time since the last
	// time GetFrameTime was called.
	// Use lockTime to lock to a specific frame time
	float GetFrameTime(float lockTime = -1.0f);

private:
	float GetElapsed() const;
#ifdef _WIN32
	LARGE_INTEGER mFreq;
	LARGE_INTEGER mStart;
#else
	std::chrono::high_resolution_clock::time_point mStart;
#endif
};
