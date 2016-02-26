#include "ITPEnginePCH.h"
#include "CollisionComponent.h"

IMPL_COMPONENT(CollisionComponent, Component, 1);

CollisionComponent::CollisionComponent(Actor& owner)
	:Component(owner)
	,mIsWalkable(false)
{

}

void CollisionComponent::Register()
{
	Super::Register();
	mOwner.GetGame().GetPhysWorld().AddComponent(ThisPtr());
}

void CollisionComponent::Unregister()
{
	Super::Unregister();
	mOwner.GetGame().GetPhysWorld().RemoveComponent(ThisPtr());
}

bool CollisionComponent::Intersects(std::shared_ptr<CollisionComponent> other)
{
	return false;
}

bool CollisionComponent::SegmentCast(const Collision::LineSegment& segment, Vector3& outPoint)
{
	return false;
}

void CollisionComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	GetBoolFromJSON(properties, "walkable", mIsWalkable);
}
