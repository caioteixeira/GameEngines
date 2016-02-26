// MoveComponent.h
// Component that does basic velocity-based movement
// Generally used as a "pre-tick" component

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
	DECL_COMPONENT(MoveComponent, Component);
public:
	MoveComponent(Actor& owner);

	void Tick(float deltaTime) override;

	// Linear speed (in game units/s)
	void SetLinearSpeed(float speed) { mLinearSpeed = speed; }
	float GetLinearSpeed() const { return mLinearSpeed; }

	// Yaw speed (in radians/s)
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
	float GetYawSpeed() const { return mYawSpeed; }

	// Pitch speed (in radians/s)
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	float GetPitchSpeed() const { return mPitchSpeed; }

	// Left/right speed
	void SetHorizontalSpeed(float speed) { mHorizontalSpeed = speed; }
	float GetHorizontalSpeed() const { return mHorizontalSpeed; }

	// AddTo calls should always be +1.0f or -1.0f
	void AddToLinearAxis(float delta);
	void AddToYawAxis(float delta);
	void AddToPitchAxis(float delta);
	void AddToHorizontalAxis(float delta);

	void SetLinearAxis(float axis);
	float GetLinearAxis() const { return mLinearAxis; }

	void SetAngularAxis(float axis);
	float GetYawAxis() const { return mYawAxis; }

	void SetPitchAxis(float axis);
	float GetPitchAxis() const { return mPitchAxis; }

	void SetHorizontalAxis(float axis);
	float GetHorizontalAxis() const { return mHorizontalAxis; }

	void SetFaceMoveDirection(bool value) { mFaceMoveDirection = value; }
	bool GetFaceMoveDirection() const { return mFaceMoveDirection; }

	void SetAffectedByGravity(bool value) { mAffectedByGravity = value; }
	bool GetAffectedByGravity() const { return mAffectedByGravity; }

	// This is an extra force that the user can apply, because the other forces
	// are recomputed every frame in Tick.
	void SetUserForce(const Vector3& force) { mUserForce = force; }
	const Vector3& GetUserForce() const { return mUserForce; }

	void SetMass(float mass) { mMass = mass; }
	float GetMass() const { return mMass; }

	void SetProperties(const rapidjson::Value& properties) override;
protected:
	Vector3 mUserForce;
	Vector3 mForceInternal;
	Vector3 mVelocity;
	Vector3 mAcceleration;

	float mLinearSpeed;
	float mHorizontalSpeed;
	float mYawSpeed;
	float mPitchSpeed;

	float mLinearAxis;
	float mHorizontalAxis;
	float mYawAxis;
	float mPitchAxis;

	float mGravityZ;
	float mMass;

	bool mFaceMoveDirection;
	bool mAffectedByGravity;
};

DECL_PTR(MoveComponent);
