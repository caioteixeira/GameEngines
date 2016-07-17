#pragma once
#include "Asset.h"
#include "BoneTransform.h"
#include <string>
#include <vector>

class Skeleton : public Asset
{
	DECL_ASSET(Skeleton, Asset);
public:
	struct Bone
	{
		BoneTransform mLocalBindPose;
		std::string mName;
		int mParent;
	};
	Skeleton(class Game& game);

	bool Load(const char* fileName, class AssetCache* cache) override;

	size_t GetNumBones() const { return mBones.size(); }
	const Bone& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<Bone>& GetBones() const { return mBones; }

	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }
protected:
	// Automatically called once the skeleton has been loaded
	void ComputeGlobalInvBindPose();
private:
	std::vector<Bone> mBones;
	std::vector<Matrix4> mGlobalInvBindPoses;
};

DECL_PTR(Skeleton);
