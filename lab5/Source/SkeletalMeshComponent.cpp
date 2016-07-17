#include "ITPEnginePCH.h"

IMPL_COMPONENT(SkeletalMeshComponent, MeshComponent, 32);

SkeletalMeshComponent::SkeletalMeshComponent(Actor& owner)
	:MeshComponent(owner)
{

}

void SkeletalMeshComponent::Draw(class Renderer& render)
{
	if (mMesh)
	{
		render.DrawSkeletalMesh(mMesh->GetVertexArray(), mMesh->GetTexture(mTextureIndex),
			mOwner.GetWorldTransform(), mPalette);
	}
}

void SkeletalMeshComponent::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (mAnimation != nullptr)
	{
		mAnimationTime += deltaTime * mAnimationPlayRate;

		if (mAnimationTime > mAnimation->GetLength())
		{
			mAnimationTime -= mAnimation->GetLength();
		}
	}

	ComputeMatrixPalette();
}

float SkeletalMeshComponent::PlayAnimation(AnimationPtr anim, float playRate /*= 1.0f*/, float blendTime /*= 0.0f*/)
{
	DbgAssert(mSkeleton != nullptr, "Can't play an animation without a skeleton!");
	DbgAssert(mSkeleton->GetNumBones() == anim->GetNumBones(), 
		"Skeleton and animation have a different number of bones!");

	mAnimation = anim;
	mAnimationPlayRate = playRate;
	mAnimationTime = 0.0f;

	ComputeMatrixPalette();

	return anim->GetLength();
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

void SkeletalMeshComponent::Extract(std::queue<std::shared_ptr<RenderCommand>>& updateQueue)
{
	auto command = std::make_shared<DrawSkinnedMeshCommand>(mMesh, mOwner.GetWorldTransform(), mTextureIndex, mPalette);
	updateQueue.push(command);
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	auto globalInvMatrices = mSkeleton->GetGlobalInvBindPoses();
	std::vector<Matrix4> pose;
	mAnimation->GetGlobalPoseAtTime(pose, mSkeleton, mAnimationTime);

	for (int i = 0; i < globalInvMatrices.size(); i++)
	{
		mPalette.mSkeletonBones[i] = globalInvMatrices[i] * pose[i];
	}
}
