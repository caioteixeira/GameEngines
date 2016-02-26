#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include "Skeleton.h"
#include "Animation.h"

class SkeletalMeshComponent : public MeshComponent
{
	DECL_COMPONENT(SkeletalMeshComponent, MeshComponent);
public:
	SkeletalMeshComponent(Actor& owner);

	void Draw(class Renderer& render) override;

	void SetSkeleton(SkeletonPtr skeleton) { mSkeleton = skeleton; }
	void Tick(float deltaTime) override;

	// Play an animation. Returns the length of the animation
	float PlayAnimation(AnimationPtr anim, float playRate = 1.0f, float blendTime = 0.0f);

	void SetProperties(const rapidjson::Value& properties) override;
protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;
	SkeletonPtr mSkeleton;
	AnimationPtr mAnimation;
	float mAnimationPlayRate;
	float mAnimationTime;
};

DECL_PTR(SkeletalMeshComponent);
