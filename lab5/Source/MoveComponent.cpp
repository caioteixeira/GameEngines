#include "ITPEnginePCH.h"

const float DEFAULT_GRAVITY_Z = -980.0f;

IMPL_COMPONENT(MoveComponent, Component, 100);

MoveComponent::MoveComponent(Actor& owner)
	:Component(owner)
	,mLinearSpeed(0.0f)
	,mYawSpeed(0.0f)
	,mPitchSpeed(0.0f)
	,mHorizontalSpeed(0.0f)
	,mLinearAxis(0.0f)
	,mYawAxis(0.0f)
	,mPitchAxis(0.0f)
	,mHorizontalAxis(0.0f)
	,mGravityZ(DEFAULT_GRAVITY_Z)
	,mMass(1.0f)
	,mFaceMoveDirection(false)
	,mAffectedByGravity(false)
{

}

void MoveComponent::Tick(float deltaTime)
{
	mForceInternal = mUserForce;

	if (!Math::IsZero(mYawAxis) || !Math::IsZero(mPitchAxis))
	{
		Quaternion rot = mOwner.GetRotation();
		
		Quaternion yaw(mOwner.GetWorldTransform().GetZAxis(), mYawSpeed * mYawAxis * deltaTime);
		Quaternion pitch(mOwner.GetWorldTransform().GetYAxis(), mPitchSpeed * mPitchAxis * deltaTime);

		rot = Concatenate(rot, yaw);
		rot = Concatenate(rot, pitch);

		mOwner.SetRotation(rot);
	}

	Vector3 inputVelocity;
	if (!Math::IsZero(mLinearAxis) || !Math::IsZero(mHorizontalAxis))
	{
		Vector3 forward;
		Vector3 left;

		// If we're facing move direction, then move along global axis
		// otherwise, move along local axis
		if (mFaceMoveDirection)
		{
			forward = Vector3::UnitX;
			left = Vector3::NegUnitY;
		}
		else
		{
			forward = mOwner.GetForward();
			left = mOwner.GetWorldTransform().GetYAxis() * -1.0f;
		}

		// Update velocity based on rotation
		Vector3 linearVelocity = forward * mLinearSpeed * mLinearAxis;
		Vector3 horizontalVelocity = left * mHorizontalSpeed * mHorizontalAxis;
		
		// Average them -- to be honest, this is kind of weird, we should
		// probably just compute a direction and just have a set run/walk speed?
		inputVelocity = linearVelocity + horizontalVelocity;
		if (!Math::IsZero(mLinearAxis) && !Math::IsZero(mHorizontalAxis))
		{
			inputVelocity *= 0.5f;
		}

		mForceInternal += inputVelocity;
	}

	// TODO: Lab 5
	// Change this to include force
	if(mAffectedByGravity)
	{
		mForceInternal += Vector3::UnitZ * mGravityZ * mMass;
	}

	/*Modified Verlet integration*/
	Vector3 pos = mOwner.GetPosition();
	Vector3 avgVelocity = mVelocity + mAcceleration * deltaTime / 2;
	pos += avgVelocity * deltaTime;
	pos += inputVelocity * deltaTime;
	mAcceleration = mForceInternal / mMass;
	mVelocity = avgVelocity + mAcceleration * deltaTime / 2;

	/* Old Euler integration code
	Vector3 pos = mOwner.GetPosition();
	mVelocity = inputVelocity;
	pos += mVelocity * deltaTime;*/

	mOwner.SetPosition(pos);

	if (mFaceMoveDirection &&
		(!Math::IsZero(mLinearAxis) || !Math::IsZero(mHorizontalAxis)))
	{
		Vector3 normVel = mVelocity;
		// clear out the z so we don't face the direction we're falling
		normVel.z = 0.0f;
		normVel.Normalize();
		float dot = Dot(Vector3::UnitX, normVel);
		// We have special cases if we're collinear, otherwise compute
		// the cross product and use that angle
		if (dot >= 0.99f)
		{
			mOwner.SetRotation(Quaternion::Identity);
		}
		else if (dot <= -0.99f)
		{
			mOwner.SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
		}
		else
		{
			float angle = Math::Acos(Dot(Vector3::UnitX, normVel));
			Vector3 axis = Cross(Vector3::UnitX, normVel);
			axis.Normalize();

			Quaternion yaw(axis, angle);
			mOwner.SetRotation(yaw);
		}
	}
}

void MoveComponent::AddToLinearAxis(float delta)
{
	mLinearAxis += delta;
	mLinearAxis = Math::Clamp(mLinearAxis, -1.0f, 1.0f);
}

void MoveComponent::AddToYawAxis(float delta)
{
	mYawAxis += delta;
	mYawAxis = Math::Clamp(mYawAxis, -1.0f, 1.0f);
}

void MoveComponent::AddToPitchAxis(float delta)
{
	mPitchAxis += delta;
	mPitchAxis = Math::Clamp(mPitchAxis, -1.0f, 1.0f);
}

void MoveComponent::AddToHorizontalAxis(float delta)
{
	mHorizontalAxis += delta;
	mHorizontalAxis = Math::Clamp(mHorizontalAxis, -1.0f, 1.0f);
}

void MoveComponent::SetLinearAxis(float axis)
{
	mLinearAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetAngularAxis(float axis)
{
	mYawAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetPitchAxis(float axis)
{
	mPitchAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetHorizontalAxis(float axis)
{
	mHorizontalAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	float temp;

	if(GetFloatFromJSON(properties, "linearSpeed", temp))
	{
		SetLinearSpeed(temp);
	}

	if(GetFloatFromJSON(properties, "horizontalSpeed", temp))
	{
		SetHorizontalSpeed(temp);
	}

	if (GetFloatFromJSON(properties, "yawSpeed", temp))
	{
		SetYawSpeed(temp);
	}

	if (GetFloatFromJSON(properties, "pitchSpeed", temp))
	{
		SetPitchSpeed(temp);
	}
}
