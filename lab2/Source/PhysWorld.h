// PhysWorld.h
// The physics world keeps track of all active collision
// components and tests them for collisions against each other

#pragma once
#include "CollisionComponent.h"
#include <unordered_set>
#include <unordered_map>

class PhysWorld
{
public:
	PhysWorld();
	void AddComponent(CollisionComponentPtr component);
	void RemoveComponent(CollisionComponentPtr component);

	void Tick(float deltaTime);

	// Cast a line segment against every collision component,
	// and returns true if something is hit, false if not.
	// Guaranteed to return the closet component hit
	bool SegmentCast(const Actor& owner, const Vector3& start, const Vector3& end, 
		CollisionComponentPtr& outComp, Vector3& outPoint);
private:
	void ClearCollisionPairs();
	bool HasAlreadyCollided(const Actor& a, const Actor& b);
	void AddCollisionPair(const Actor& a, const Actor& b);

	std::unordered_set<CollisionComponentPtr> mComponents;
	// This is used to track pairs of collisions on a particular frame
	// This is to avoid detecting the same collision multiple times.
	std::unordered_map<const Actor*, std::unordered_set<const Actor*>> mCollPairs;

	// temporary vector of components used in case components are added while ticking
	std::unordered_set<CollisionComponentPtr> mPendingComponents;

	bool mIsInTick;
};
