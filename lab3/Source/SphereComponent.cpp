#include "ITPEnginePCH.h"

IMPL_COMPONENT(SphereComponent, CollisionComponent, 100);

SphereComponent::SphereComponent(Actor& owner)
	:CollisionComponent(owner)
	,mScale(1.0f)
{

}

bool SphereComponent::Intersects(CollisionComponentPtr other)
{
	if (IsA<SphereComponent>(other))
	{
		SphereComponentPtr sphereOther = Cast<SphereComponent>(other);
		return Collision::Intersects(mWorldSpaceBounds, sphereOther->mWorldSpaceBounds);
	}

	// Ignore any other components for now
	return false;
}

bool SphereComponent::SegmentCast(const Collision::LineSegment& segment, Vector3& outPoint)
{
	// TODO: Implement
	return false;
}

void SphereComponent::SphereFromMesh(MeshPtr mesh)
{
	mModelSpaceBounds = mesh->GetBoundingSphere();
	OnUpdatedTransform();
}

void SphereComponent::OnUpdatedTransform()
{
	// Update the radius/center from our owning component

	// Scale the radius by the world transform scale
	// (We can assume the scale is uniform, because
	// actors don't allow non-uniform scaling)
	Vector3 scaleVec = mOwner.GetWorldTransform().GetScale();
	mWorldSpaceBounds.mRadius = mModelSpaceBounds.mRadius * mScale * scaleVec.x;

	mWorldSpaceBounds.mCenter = mModelSpaceBounds.mCenter +
		mOwner.GetWorldTransform().GetTranslation();
}
