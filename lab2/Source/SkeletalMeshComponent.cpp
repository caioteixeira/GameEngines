#include "ITPEnginePCH.h"

IMPL_COMPONENT(SkeletalMeshComponent, MeshComponent, 32);

SkeletalMeshComponent::SkeletalMeshComponent(Actor& owner)
	:MeshComponent(owner)
{

}

void SkeletalMeshComponent::Draw(class Renderer& render)
{
	// TODO: Lab 4
}

void SkeletalMeshComponent::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// TODO: Lab 4
}

float SkeletalMeshComponent::PlayAnimation(AnimationPtr anim, float playRate /*= 1.0f*/, float blendTime /*= 0.0f*/)
{
	DbgAssert(mSkeleton != nullptr, "Can't play an animation without a skeleton!");
	DbgAssert(mSkeleton->GetNumBones() == anim->GetNumBones(), 
		"Skeleton and animation have a different number of bones!");

	// TODO: Lab 4

	return 0.0f;
}

void SkeletalMeshComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	std::string skeleton;
	if (GetStringFromJSON(properties, "skeleton", skeleton))
	{
		mSkeleton = mOwner.GetGame().GetAssetCache().Load<Skeleton>(skeleton);
	}
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// TODO: Lab 4
}
