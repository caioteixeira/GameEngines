#include "ITPEnginePCH.h"

IMPL_COMPONENT(CharacterMoveComponent, MoveComponent, 8);

CharacterMoveComponent::CharacterMoveComponent(Actor& owner)
	:Super(owner)
	,mState(CM_Default)
	,mFootCastOffset(10.0f)
	,mStateDuration(0.0f)
	,mAnimLength(0.0f)
{
	SetFaceMoveDirection(true);
}

void CharacterMoveComponent::BindLinearAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&CharacterMoveComponent::OnLinearAxis);
}

void CharacterMoveComponent::BindHorizontalAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&CharacterMoveComponent::OnHorizontalAxis);
}

void CharacterMoveComponent::BindJump(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAction(name, IE_Pressed, this,
		&CharacterMoveComponent::OnJumpAction);
}

void CharacterMoveComponent::OnLinearAxis(float value)
{
	SetLinearAxis(value);
	// Clear out x acceleration and velocity
	mVelocity.x = 0.0f;
	mAcceleration.x = 0.0f;
}

void CharacterMoveComponent::OnHorizontalAxis(float value)
{
	SetHorizontalAxis(value);
	// Clear out y acceleration and velocity
	mVelocity.y = 0.0f;
	mAcceleration.y = 0.0f;
}

void CharacterMoveComponent::OnJumpAction()
{
	// Can only jump if on the ground
	if (mState == CM_Idle || mState == CM_Run)
	{
		SetState(CM_JumpStart);
	}
}

void CharacterMoveComponent::Tick(float deltaTime)
{
	DbgAssert(mOwnerMesh != nullptr, "CharacterMoveComponent can't work w/out owner mesh!");

	mStateDuration += deltaTime;

	switch (mState)
	{
		case CM_Default:
			// TODO: Lab 5
			// Change the initial state to fall instead of idle
			SetState(CM_Idle);
			break;
		case CM_Idle:
			TickIdle(deltaTime);
			break;
		case CM_Run:
			TickRun(deltaTime);
			break;
		case CM_JumpStart:
			TickJumpStart(deltaTime);
			break;
		case CM_Jump:
			TickJump(deltaTime);
			break;
		case CM_Land:
			TickLand(deltaTime);
			break;
		case CM_Fall:
			TickFall(deltaTime);
			break;
		default:
			break;
	}

	// Super tick after state changes, because it may have changed gravity, etc
	Super::Tick(deltaTime);
}

void CharacterMoveComponent::SetAnimation(ECharState state, AnimationPtr anim)
{
	// If we already set it, there may be a new anim
	auto iter = mAnims.find(state);
	if (iter != mAnims.end())
	{
		mAnims[state] = anim;
	}
	else
	{
		mAnims.emplace(state, anim);
	}
}

const AnimationPtr CharacterMoveComponent::GetAnimation(ECharState state) const
{
	auto iter = mAnims.find(state);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void CharacterMoveComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	GetFloatFromJSON(properties, "jumpForceZ", mJumpForceZ);

	if (properties.HasMember("animations"))
	{
		const rapidjson::Value& anims = properties["animations"];
		
		std::string animName;
		if (GetStringFromJSON(anims, "idle", animName))
		{
			SetAnimation(CM_Idle, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}

		if (GetStringFromJSON(anims, "run", animName))
		{
			SetAnimation(CM_Run, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}

		if (GetStringFromJSON(anims, "jumpStart", animName))
		{
			SetAnimation(CM_JumpStart, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}

		if (GetStringFromJSON(anims, "jump", animName))
		{
			SetAnimation(CM_Jump, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}

		if (GetStringFromJSON(anims, "land", animName))
		{
			SetAnimation(CM_Land, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}

		if (GetStringFromJSON(anims, "fall", animName))
		{
			SetAnimation(CM_Fall, mOwner.GetGame().GetAssetCache().Load<Animation>(animName));
		}
	}
}

bool CharacterMoveComponent::CheckFootCast(CollisionComponentPtr& outComp, Vector3& outPos)
{
	// TODO: Lab 5
	// (For now, the foot cast always succeeds)

	outPos = mOwner.GetWorldTransform().GetTranslation();

	return true;
}

void CharacterMoveComponent::SetState(ECharState newState)
{
	AnimationPtr anim = GetAnimation(newState);

	switch (newState)
	{
	// Jump/fall play animations slower, and enable gravity
	case CM_Jump:
	case CM_Fall:
		SetAffectedByGravity(true);
		mUserForce = Vector3(0.0f, 0.0f, 0.0f);
		if (anim)
		{
			// Play the fall animation slower
			mAnimLength = mOwnerMesh->PlayAnimation(anim, 0.6f);
		}
		break;
	// The other non-default states just play their animations
	case CM_Idle:
	case CM_Run:
		if (anim)
		{
			mAnimLength = mOwnerMesh->PlayAnimation(anim);
		}
		break;
	case CM_JumpStart:
		SetAffectedByGravity(true);
		mUserForce = Vector3(0.0f, 0.0f, mJumpForceZ);
		if (anim)
		{
			mAnimLength = mOwnerMesh->PlayAnimation(anim);
		}
		break;
	case CM_Land:
		SetAffectedByGravity(false);
		// Clear out my z velocity/acceleration since I landed
		mVelocity.z = 0.0f;
		mAcceleration.z = 0.0f;
		if (anim)
		{
			mAnimLength = mOwnerMesh->PlayAnimation(anim);
		}
		break;
	// Default states don't do anything
	case CM_Default:
	default:
		break;
	}

	mStateDuration = 0.0f;
	mState = newState;
}

void CharacterMoveComponent::TickIdle(float deltaTime)
{
	// You might fall off if a platform is moving under you or something
	CollisionComponentPtr collComp;
	Vector3 collPoint;
	if (!CheckFootCast(collComp, collPoint))
	{
		SetState(CM_Fall);
	}

	if (!Math::IsZero(GetLinearAxis()) || !Math::IsZero(GetHorizontalAxis()))
	{
		SetState(CM_Run);
	}
}

void CharacterMoveComponent::TickRun(float deltaTime)
{
	// I fell off!
	CollisionComponentPtr collComp;
	Vector3 collPoint;
	if (!CheckFootCast(collComp, collPoint))
	{
		SetState(CM_Fall);
	}

	if (Math::IsZero(GetLinearAxis()) && Math::IsZero(GetHorizontalAxis()))
	{
		SetState(CM_Idle);
	}
}

void CharacterMoveComponent::TickJumpStart(float deltaTime)
{
	// Decrease the force applied
	mUserForce.z -= mJumpForceZ * deltaTime;
	// If the jump start is over, go to the jump state
	if (mStateDuration > (mAnimLength - 0.05f))
	{
		// Sanjay: I changed this to just go into the fall
		// state because it jumps pretty far as is, anyway
		SetState(CM_Fall);
	}
}

void CharacterMoveComponent::TickJump(float deltaTime)
{

}

void CharacterMoveComponent::TickLand(float deltaTime)
{
	// Check that I'm still on something
	CollisionComponentPtr collComp;
	Vector3 collPoint;
	if (!CheckFootCast(collComp, collPoint))
	{
		SetState(CM_Fall);
	}

	// If the jump end is over, go to idle or run state
	if (mStateDuration > (mAnimLength - 0.05f))
	{
		if (Math::IsZero(GetLinearAxis()) && Math::IsZero(GetHorizontalAxis()))
		{
			SetState(CM_Idle);
		}
		else
		{
			SetState(CM_Run);
		}
	}
}

void CharacterMoveComponent::TickFall(float deltaTime)
{
	// Check if I landed on something
	CollisionComponentPtr collComp;
	Vector3 collPoint;
	if (CheckFootCast(collComp, collPoint))
	{
		// Set the z value to the collision height
		Vector3 pos = mOwner.GetPosition();
		pos.z = collPoint.z;
		mOwner.SetPosition(pos);
		SetState(CM_Land);
	}
}
