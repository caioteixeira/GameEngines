#pragma once
#include "MoveComponent.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"
#include "CollisionComponent.h"
#include <string>
#include <unordered_map>

enum ECharState
{
	CM_Default,		// Default state (does nothing)
	CM_Idle,		// Standing on the ground
	CM_Run,			// Running on the ground
	CM_JumpStart,	// First starting to jump
	CM_Jump,		// Still going up
	CM_Land,		// Landing
	CM_Fall,		// Falling
};

class CharacterMoveComponent : public MoveComponent
{
	DECL_COMPONENT(CharacterMoveComponent, MoveComponent);
public:
	CharacterMoveComponent(Actor& owner);

	void BindLinearAxis(const std::string& name);
	void BindHorizontalAxis(const std::string& name);
	void BindJump(const std::string& name);
	
	void OnLinearAxis(float value);
	void OnHorizontalAxis(float value);
	void OnJumpAction();

	void Tick(float deltaTime) override;

	void SetAnimation(ECharState state, AnimationPtr anim);
	const AnimationPtr GetAnimation(ECharState state) const;

	void SetOwnerMesh(SkeletalMeshComponentPtr mesh) { mOwnerMesh = mesh; }

	void SetJumpForceZ(float jumpZ) { mJumpForceZ = jumpZ; }
	float GetJumpForceZ() const { return mJumpForceZ; }

	void SetFootCastOffset(float offset) { mFootCastOffset = offset; }
	float GetFootCastOffset() const { return mFootCastOffset; }

	void SetProperties(const rapidjson::Value& properties) override;
protected:
	bool CheckFootCast(CollisionComponentPtr& outComp, Vector3& outPos);
	void SetState(ECharState newState);
	void TickIdle(float deltaTime);
	void TickRun(float deltaTime);
	void TickJumpStart(float deltaTime);
	void TickJump(float deltaTime);
	void TickLand(float deltaTime);
	void TickFall(float deltaTime);

	std::unordered_map<ECharState, AnimationPtr> mAnims;

	SkeletalMeshComponentPtr mOwnerMesh;

	ECharState mState;

	// How long the current state has been active
	float mStateDuration;
	// Length of the currently playing animation (only some states use this)
	float mAnimLength;
	// Initial jumping force
	float mJumpForceZ;
	// Offset from feet that's used to see if walking is possible
	float mFootCastOffset;
};

DECL_PTR(CharacterMoveComponent);
