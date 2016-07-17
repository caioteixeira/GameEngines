#include "ITPEnginePCH.h"
#include <unordered_map>

PhysWorld::PhysWorld()
	:mIsInTick(false)
{

}

void PhysWorld::AddComponent(CollisionComponentPtr component)
{
	if (!mIsInTick)
	{
		mComponents.emplace(component);
	}
	else
	{
		mPendingComponents.emplace(component);
	}
}

void PhysWorld::RemoveComponent(CollisionComponentPtr component)
{
	auto iter = mComponents.find(component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}

	iter = mPendingComponents.find(component);
	if (iter != mPendingComponents.end())
	{
		mPendingComponents.erase(iter);
	}
}

void PhysWorld::Tick(float deltaTime)
{
	mIsInTick = true;

	// Clear the collision pairs since this is a new frame
	ClearCollisionPairs();

	// Just collide every component against ever other for now.
	// This is super inefficient but works for basic worlds
	// A potential feature would be to add real world partitioning
	for (auto& outer : mComponents)
	{
		for (auto& inner : mComponents)
		{
			if (outer != inner &&
				!HasAlreadyCollided(outer->GetOwner(), inner->GetOwner()))
			{
				if (outer->Intersects(inner))
				{
					outer->GetOwner().BeginTouch(inner->GetOwner());
					inner->GetOwner().BeginTouch(outer->GetOwner());
					AddCollisionPair(outer->GetOwner(), inner->GetOwner());
				}
			}
		}
	}

	for (auto& c : mPendingComponents)
	{
		mComponents.emplace(c);
	}
	mPendingComponents.clear();

	mIsInTick = false;
}

bool PhysWorld::SegmentCast(const Actor& owner, const Vector3& start, const Vector3& end, 
	CollisionComponentPtr& outComp, Vector3& outPoint)
{
	// clear this out just in case
	outComp = nullptr;

	Collision::LineSegment segment;
	segment.mStart = start;
	segment.mEnd = end;

	// Make sure we get the closest one
	float bestDistSq = FLT_MAX;

	// Test against every component
	for (auto& c : mComponents)
	{
		if (&c->GetOwner() != &owner)
		{
			Vector3 testOut;
			if (c->SegmentCast(segment, testOut))
			{
				float dist = (start - testOut).LengthSq();
				if (dist < bestDistSq)
				{
					bestDistSq = dist;
					outPoint = testOut;
					outComp = c;
				}
			}
		}
	}

	if (outComp != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysWorld::ClearCollisionPairs()
{
	mCollPairs.clear();
}

bool PhysWorld::HasAlreadyCollided(const Actor& a, const Actor& b)
{
	auto pairIter = mCollPairs.find(&a);
	if (pairIter != mCollPairs.end() &&
		pairIter->second.find(&b) != pairIter->second.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysWorld::AddCollisionPair(const Actor& a, const Actor& b)
{
	// Add it in both directions
	auto pairIter = mCollPairs.find(&a);
	if (pairIter != mCollPairs.end())
	{
		pairIter->second.emplace(&b);
	}
	else
	{
		std::unordered_set<const Actor*> second;
		second.emplace(&b);
		mCollPairs.emplace(&a, second);
	}

	pairIter = mCollPairs.find(&b);
	if (pairIter != mCollPairs.end())
	{
		pairIter->second.emplace(&a);
	}
	else
	{
		std::unordered_set<const Actor*> second;
		second.emplace(&a);
		mCollPairs.emplace(&b, second);
	}
}
