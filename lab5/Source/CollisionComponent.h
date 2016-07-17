// CollisionComponent.h
// Base class for all collision components
// supported by the game

#pragma  once
#include "Component.h"
#include "CollisionHelpers.h"

class CollisionComponent : public Component
{
	DECL_COMPONENT(CollisionComponent, Component);
public:
	CollisionComponent(Actor& owner);

	void Register() override;
	void Unregister() override;

	virtual bool Intersects(std::shared_ptr<CollisionComponent> other);
	virtual bool SegmentCast(const Collision::LineSegment& segment, Vector3& outPoint);

	void SetIsWalkable(bool walkable) { mIsWalkable = true; }
	bool GetIsWalkable() const { return mIsWalkable; }

	void SetProperties(const rapidjson::Value& properties) override;
private:
	// Determines whether or not this collision component can be walked on
	bool mIsWalkable;
};

DECL_PTR(CollisionComponent);
