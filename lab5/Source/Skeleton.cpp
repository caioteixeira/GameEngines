#include "ITPEnginePCH.h"
#include <SDL/SDL_log.h>


Skeleton::Skeleton(class Game& game)
	:Asset(game)
{

}

bool Skeleton::Load(const char* fileName, class AssetCache* cache)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Skeleton %s", fileName);
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
		SDL_Log("Skeleton %s is not valid json", fileName);
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpskel" ||
		ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName);
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName);
		return false;
	}

	size_t count = bonecount.GetUint();

	DbgAssert(count <= MAX_SKELETON_BONES, "Skeleton exceeds maximum allowed bones.");

	mBones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName);
		return false;
	}

	if (bones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName);
		return false;
	}

	Bone temp;

	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName, i);
			return false;
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.mName = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.mParent = parent.GetInt();

		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName, i);
			return false;
		}

		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName, i);
			return false;
		}

		temp.mLocalBindPose.mRotation.x = rot[0].GetDouble();
		temp.mLocalBindPose.mRotation.y = rot[1].GetDouble();
		temp.mLocalBindPose.mRotation.z = rot[2].GetDouble();
		temp.mLocalBindPose.mRotation.w = rot[3].GetDouble();

		temp.mLocalBindPose.mTranslation.x = trans[0].GetDouble();
		temp.mLocalBindPose.mTranslation.y = trans[1].GetDouble();
		temp.mLocalBindPose.mTranslation.z = trans[2].GetDouble();

		mBones.emplace_back(temp);
	}

	// Now that we have the bones
	ComputeGlobalInvBindPose();

	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	//Clear vector
	mGlobalInvBindPoses.erase(mGlobalInvBindPoses.begin(), mGlobalInvBindPoses.end());
	std::vector<Matrix4> matrices;

	//Convert to matrices
	for (int i = 0; i < mBones.size(); i++)
	{
		Bone bone = mBones[i];
		Matrix4 m = bone.mLocalBindPose.ToMatrix();

		Matrix4 parentMatrix = Matrix4::Identity;
		if(bone.mParent >= 0 && bone.mParent < matrices.size())
		{
			parentMatrix = matrices[bone.mParent];
		}

		matrices.push_back(m * parentMatrix);
	}

	//Invert matrices
	for(auto & matrix : matrices)
	{
		matrix.Invert();
		mGlobalInvBindPoses.push_back(matrix);
	}
}
