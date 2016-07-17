#include "ITPEnginePCH.h"
#include <SDL/SDL_log.h>

Animation::Animation(class Game& game)
	:Asset(game)
{

}

bool Animation::Load(const char* fileName, class AssetCache* cache)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Animation %s", fileName);
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Animation %s is not valid json", fileName);
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpanim" ||
		ver != 2)
	{
		SDL_Log("Animation %s unknown format", fileName);
		return false;
	}

	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", fileName);
		return false;
	}

	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", fileName);
		return false;
	}

	mNumFrames = frames.GetUint();
	mLength = length.GetDouble();
	mNumBones = bonecount.GetUint();

	mTracks.resize(mNumBones);

	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray())
	{
		SDL_Log("Sequence %s missing a tracks array.", fileName);
		return false;
	}

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		if (!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", fileName, i);
			return false;
		}

		size_t boneIndex = tracks[i]["bone"].GetUint();

		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is missing transforms.", fileName, i);
			return false;
		}

		BoneTransform temp;

		if (transforms.Size() != mNumFrames)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", fileName, i);
			return false;
		}

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", fileName, i);
				return false;
			}

			temp.mRotation.x = rot[0].GetDouble();
			temp.mRotation.y = rot[1].GetDouble();
			temp.mRotation.z = rot[2].GetDouble();
			temp.mRotation.w = rot[3].GetDouble();

			temp.mTranslation.x = trans[0].GetDouble();
			temp.mTranslation.y = trans[1].GetDouble();
			temp.mTranslation.z = trans[2].GetDouble();

			mTracks[boneIndex].emplace_back(temp);
		}
	}

	return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, SkeletonPtr inSkeleton, float inTime)
{
	float frameLength = mLength / (mNumFrames - 1);
	int frameA = static_cast<int>(floor(inTime/frameLength));
	int frameB = frameA + 1;

	float f = inTime/mLength;

	for (int i = 0; i < inSkeleton->GetNumBones(); i++)
	{
		auto & bone = inSkeleton->GetBone(i);

		Matrix4 m = Matrix4::Identity;

		if (mTracks[i].size() > 0)
		{
			BoneTransform b = Interpolate(mTracks[i][frameA], mTracks[i][frameB], f);
			m = b.ToMatrix();
		}

		Matrix4 parentMatrix = Matrix4::Identity;
		if (bone.mParent >= 0 && bone.mParent < outPoses.size())
		{
			parentMatrix = outPoses[bone.mParent];
		}
		
		outPoses.push_back(m * parentMatrix);
	}

	outPoses.size();
}
