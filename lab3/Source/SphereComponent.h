// SphereCollision.h
// Component that adds sphere collision

#pragma once
#include "CollisionComponent.h"
#include "Mesh.h"
#include "Math.h"

class SphereComponent : public CollisionComponent
{
	DECL_COMPONENT(SphereComponent, CollisionComponent);
public:
	SphereComponent(Actor& owner);
	
	bool Intersects(CollisionComponentPtr other) override;
	bool SegmentCast(const Collision::LineSegment& segment, Vector3& outPoint) override;

	const Collision::Sphere& GetModelSpaceBounds() const { return mModelSpaceBounds; }
	const Collision::Sphere& GetWorldSpaceBounds() const { return mWorldSpaceBounds; }
	
	void SphereFromMesh(MeshPtr mesh);

	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }

	void OnUpdatedTransform() override;
private:
	Collision::Sphere mModelSpaceBounds;
	Collision::Sphere mWorldSpaceBounds;

	float mScale;
};

DECL_PTR(SphereComponent);
