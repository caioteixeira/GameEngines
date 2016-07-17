#pragma once
#include "Object.h"
#include "Delegate.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

// A TimerHandle is used to track a timer once created
class TimerHandle
{
	friend class GameTimerManager;
public:
	TimerHandle()
		:mValue(-1)
	{}

	bool IsValid() const { return mValue != -1; }
	int GetValue() const { return mValue; }
	bool operator==(const TimerHandle& other) const
	{
		return mValue == other.mValue;
	}
private:
	int mValue;
};

// Overload the hash function for TimerHandle so it
// can be used in unordered_map
namespace std
{
	template <>
	struct hash<TimerHandle>
	{
		size_t operator()(const TimerHandle & x) const
		{
			return hash<int>()(x.GetValue());
		}
	};
}

// Define a timer delegate ptr as a delegate with no parameters
typedef DelegateNoParamsPtr TimerDelegatePtr;

class GameTimerManager
{
public:
	GameTimerManager();

	// Tick all the timers -- should only be called from Game::Tick
	void Tick(float deltaTime);

	// Sets a game timer
	// Parameters:
	// - TimerHandle to store the handle to resulting timer
	// - Pointer to the object the timer is mapped to (usually this)
	// - Pointer to the member function that the timer calls
	// - Float duration of the timer
	// - bool on whether or not the timer loops (defaults to false)
	template <typename T>
	void SetTimer(TimerHandle& outHandle, T* obj, void (T::*func)(),
		float duration, bool looping = false)
	{
		// This is some hardcore template magic
		// Don't really worry how this works, other than that it
		// calls SetTimerInternal with a properly constructed delegate
		TimerDelegatePtr delegate = std::make_shared<DelegateNoParams<T>>(obj, func);
		SetTimerInternal(outHandle, obj, delegate, duration, looping);
	}

	// Clears a timer
	void ClearTimer(const TimerHandle& handle);

	// Pause a timer
	void PauseTimer(const TimerHandle& handle);

	// Unpause a timer
	void UnpauseTimer(const TimerHandle& handle);

	// Get the remaining time of a timer
	float GetRemainingTime(const TimerHandle& handle);

	// Add time to a timer
	void AddTime(const TimerHandle& handle, float time);

	// Clear all timers associated with a particular object
	void ClearAllTimers(Object* obj);
private:
	enum TimerStatus
	{
		Pending,
		Active,
		Paused,
		Cleared
	};

	struct TimerInfo
	{
		// (Raw) Pointer to Object this TimerInfo is bound to
		Object* mObj;

		// Delegate for this timer
		TimerDelegatePtr mDelegate;

		// TimerHandle for this timer
		TimerHandle mHandle;

		// Total duration of this timer
		float mDuration;

		// Remaining time on the timer
		float mRemainingTime;

		// Status of the timer
		TimerStatus mStatus;

		// Whether or not the timer is looping
		bool mIsLooping;
	};

	void SetTimerInternal(TimerHandle& outHandle, Object* obj,
		TimerDelegatePtr delegate, float duration, bool looping);

	void AddToObjMap(Object* obj, const TimerHandle& handle);
	void RemoveFromObjMap(Object* obj, const TimerHandle& handle);

	// Map of timer handles to timers
	std::unordered_map<TimerHandle, TimerInfo> mActiveTimers;

	// Map of pending timers
	std::unordered_map<TimerHandle, TimerInfo> mPendingTimers;

	// Map of Object to set of timer handles
	std::unordered_map<Object*, 
		std::unordered_set<TimerHandle>> mObjToHandlesMap;

	// Vector of cleared timers
	std::vector<TimerHandle> mClearedTimers;

	// Tracks the next timer handle ID to assign
	int mNextTimerId;

	// bool to track whether or not timers are ticking
	bool mAreTimersTicking;
};
