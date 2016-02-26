#include "ITPEnginePCH.h"

GameTimerManager::GameTimerManager()
	:mNextTimerId(0)
	,mAreTimersTicking(false)
{

}

void GameTimerManager::Tick(float deltaTime)
{
	mAreTimersTicking = true;
	
	// Tick the active timers
	for (auto& t : mActiveTimers)
	{
		TimerInfo& timer = t.second;
		if (timer.mStatus == Active)
		{
			timer.mRemainingTime -= deltaTime;
			if (timer.mRemainingTime <= 0.0f)
			{
				// Trigger the timer
				timer.mDelegate->Execute();
				// Is this looping?
				if (timer.mIsLooping)
				{
					timer.mRemainingTime += timer.mDuration;
				}
				else
				{
					timer.mStatus = Cleared;
					mClearedTimers.push_back(timer.mHandle);
				}
			}
		}
	}

	// Remove any ended timers
	for (auto& h : mClearedTimers)
	{
		auto iter = mActiveTimers.find(h);
		if (iter != mActiveTimers.end())
		{
			RemoveFromObjMap(iter->second.mObj, h);
			mActiveTimers.erase(h);
		}
	}
	mClearedTimers.clear();

	// Add any pending timers
	for (auto& t : mPendingTimers)
	{
		TimerInfo& timer = t.second;
		if (timer.mStatus == Pending)
		{
			timer.mStatus = Active;
		}
		mActiveTimers.emplace(timer.mHandle, timer);
	}
	mPendingTimers.clear();

	mAreTimersTicking = false;
}

void GameTimerManager::ClearTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		// We can just remove this from pending timers
		RemoveFromObjMap(iter->second.mObj, handle);
		mPendingTimers.erase(iter);
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Cleared;
			mClearedTimers.push_back(handle);
		}
	}
}

void GameTimerManager::PauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Paused;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Paused;
		}
	}
}

void GameTimerManager::UnpauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Pending;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Active;
		}
	}
}

float GameTimerManager::GetRemainingTime(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		return iter->second.mRemainingTime;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			return iter->second.mRemainingTime;
		}
	}

	// Unknown timer
	return -1.0f;
}

void GameTimerManager::AddTime(const TimerHandle& handle, float time)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mRemainingTime += time;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mRemainingTime += time;
		}
	}
}

void GameTimerManager::ClearAllTimers(Object* obj)
{
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		for (auto& t : iter->second)
		{
			ClearTimer(t);
		}

		mObjToHandlesMap.erase(iter);
	}
}

void GameTimerManager::SetTimerInternal(TimerHandle& outHandle, Object* obj, TimerDelegatePtr delegate, float duration, bool looping)
{
	outHandle.mValue = mNextTimerId;
	
	TimerInfo timer;
	timer.mObj = obj;
	timer.mDelegate = delegate;
	timer.mHandle = outHandle;
	timer.mDuration = duration;
	timer.mRemainingTime = duration;
	timer.mIsLooping = looping;

	if (mAreTimersTicking)
	{
		// Add to pending set
		timer.mStatus = Pending;
		mPendingTimers.emplace(outHandle, timer);
	}
	else
	{
		// Add to active set
		timer.mStatus = Active;
		mActiveTimers.emplace(outHandle, timer);
	}

	// Add to the timers associated with this object
	AddToObjMap(obj, outHandle);

	mNextTimerId++;
}

void GameTimerManager::AddToObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	// Do we already know of this object?
	if (iter != mObjToHandlesMap.end())
	{
		iter->second.emplace(handle);
	}
	else
	{
		std::unordered_set<TimerHandle> temp;
		temp.emplace(handle);
		mObjToHandlesMap.emplace(obj, temp);
	}
}

void GameTimerManager::RemoveFromObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		auto handleIter = iter->second.find(handle);
		if (handleIter != iter->second.end())
		{
			iter->second.erase(handleIter);
		}
	}
}
