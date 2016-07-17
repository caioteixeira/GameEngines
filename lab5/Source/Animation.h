#pragma once
#include "Asset.h"
#include "BoneTransform.h"
#include <vector>
#include "Skeleton.h"

class Animation : public Asset
{
	DECL_ASSET(Animation, Asset);
public:
	Animation(class Game& game);

	bool Load(const char* fileName, class AssetCache* cache) override;

	size_t GetNumBones() const { return mNumBones; }
	size_t GetNumFrames() const { return mNumFrames; }
	float GetLength() const { return mLength; }

	// Fills the provided matrix with the global (current) pose matrices for each
	// bone at the specified time in the animation. It is expected that the time
	// is >= 0.0f and <= mLength
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, SkeletonPtr inSkeleton, float inTime);
private:
	// Number of bones for the animation
	size_t mNumBones;

	// Number of frames in the animation
	size_t mNumFrames;

	// Length of the animation in seconds
	float mLength;

	// Transform information for each frame on the track
	// Each index in the outer vector is a bone, inner vector
	// is a frame
	std::vector<std::vector<BoneTransform>> mTracks;
};

DECL_PTR(Animation);
