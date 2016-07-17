#include "ITPEnginePCH.h"

IMPL_COMPONENT(CameraComponent, Component, 8);

CameraComponent::CameraComponent(Actor& owner)
	:Super(owner)
	,mTargetOffset(0.0f)
{
	SetSpringConstant(128.0f);
}

void CameraComponent::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	Vector3 ideal = ComputeIdealPosition();
	Vector3 displace = mCameraPos - ideal;
	Vector3 accel = (-mSpringConstant * displace) - (mDampConstant * mCameraVelocity);
	mCameraVelocity += accel * deltaTime;
	mCameraPos += mCameraVelocity * deltaTime;

	Vector3 shipPos = mOwner.GetWorldTransform().GetTranslation() +
		Vector3::UnitX * mTargetOffset;
	Vector3 forward = shipPos - mCameraPos;
	forward.Normalize();
	Vector3 left = Cross(Vector3::UnitZ, forward);
	left.Normalize();
	Vector3 up = Cross(forward, left);
	up.Normalize();

	mCameraMat = Matrix4::CreateLookAt(mCameraPos, shipPos, up);
	
	// Tell the renderer
	mOwner.GetGame().GetRenderer().UpdateViewMatrix(mCameraMat);
}

void CameraComponent::SetHorizontalDist(float min, float max)
{
	mHDist = Vector2(min, max);
}

void CameraComponent::SetVerticalDist(float min, float max)
{
	mVDist = Vector2(min, max);
}

void CameraComponent::SetTargetOffset(float offset)
{
	mTargetOffset = offset;
}

void CameraComponent::SetSpringConstant(float value)
{
	mSpringConstant = value;
	mDampConstant = 2.0f * Math::Sqrt(mSpringConstant);
}

void CameraComponent::SnapToIdealPosition()
{
	mCameraPos = ComputeIdealPosition();
}

void CameraComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	Vector3 hdist;
	if (GetVectorFromJSON(properties, "horizontalDist", hdist))
	{
		SetHorizontalDist(hdist.x, hdist.y);
	}

	Vector3 vdist;
	if (GetVectorFromJSON(properties, "verticalDist", vdist))
	{
		SetVerticalDist(vdist.x, vdist.y);
	}

	GetFloatFromJSON(properties, "targetOffset", mTargetOffset);

	float spring;
	if (GetFloatFromJSON(properties, "springConstant", spring))
	{
		SetSpringConstant(spring);
	}
}

Vector3 CameraComponent::ComputeIdealPosition()
{
	float horizDist = mHDist.x;
	float vertDist = mVDist.x;

	if (mMove)
	{
		horizDist = Math::Lerp(mHDist.x, mHDist.y, mMove->GetLinearAxis());
		vertDist = Math::Lerp(mVDist.x, mVDist.y, mMove->GetLinearAxis());
	}

	Vector3 shipPos = mOwner.GetWorldTransform().GetTranslation();
	Vector3 cameraPos = shipPos - Vector3::UnitX * horizDist +
		Vector3::UnitZ * vertDist;

	return cameraPos;
}
