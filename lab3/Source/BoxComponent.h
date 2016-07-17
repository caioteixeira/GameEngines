// SphereCollision.h
// Component that adds sphere collision

#pragma once
#include "CollisionComponent.h"
#include "Mesh.h"
#include "Math.h"

class BoxComponent : public CollisionComponent
{
	DECL_COMPONENT(BoxComponent, CollisionComponent);
public:
	BoxComponent(Actor& owner);
	
	bool Intersects(CollisionComponentPtr other) override;
	bool SegmentCast(const Collision::LineSegment& segment, Vector3& outPoint) override;

	const Collision::AxisAlignedBox& GetModelSpaceBounds() const { return mModelSpaceBounds; }
	const Collision::AxisAlignedBox& GetWorldSpaceBounds() const { return mWorldSpaceBounds; }
	
	void BoxFromMesh(MeshPtr mesh);
	void BoxFromBox(const Collision::AxisAlignedBox& box);

	const Vector3& GetScale() const { return mScale; }
	void SetScale(const Vector3& scale) { mScale = scale; }

	void OnUpdatedTransform() override;

	void SetProperties(const rapidjson::Value& properties) override;
private:
	Collision::AxisAlignedBox mModelSpaceBounds;
	Collision::AxisAlignedBox mWorldSpaceBounds;

	// We might want a non-uniform scale of the box
	Vector3 mScale;
};

DECL_PTR(BoxComponent);
