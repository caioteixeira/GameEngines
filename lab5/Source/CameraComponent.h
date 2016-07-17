#pragma once
#include "Component.h"
#include "Math.h"
#include "MoveComponent.h"

class CameraComponent : public Component
{
	DECL_COMPONENT(CameraComponent, Component);
public:
	CameraComponent(Actor& owner);

	void Tick(float deltaTime) override;
	
	const Matrix4& GetCameraMatrix() const { return mCameraMat; }

	void SetHorizontalDist(float min, float max);
	void SetVerticalDist(float min, float max);
	void SetTargetOffset(float offset);
	
	void SetMoveComponent(MoveComponentPtr move) { mMove = move; }

	void SetSpringConstant(float value);
	void SnapToIdealPosition();

	void SetProperties(const rapidjson::Value& properties) override;
private:
	Vector3 ComputeIdealPosition();
	Matrix4 mCameraMat;

	Vector3 mCameraPos;
	Vector3 mCameraVelocity;

	Vector2 mHDist;
	Vector2 mVDist;

	MoveComponentPtr mMove;

	float mTargetOffset;
	float mSpringConstant;
	float mDampConstant;
};

DECL_PTR(CameraComponent);
