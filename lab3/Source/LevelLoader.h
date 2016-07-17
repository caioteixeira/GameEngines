#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include "Component.h"

class Game;

typedef std::function<std::shared_ptr<class Actor>(Game&, const rapidjson::Value&)> ActorSpawnFunc;
typedef std::function<std::shared_ptr<class Component>(class Actor&, Component::UpdateType,
	const rapidjson::Value&)> ComponentSpawnFunc;

class LevelLoader
{
public:
	LevelLoader(Game& game);
	void Load(const char* fileName);
private:
	void SetupSpawnMaps();

	Game& mGame;

	struct ComponentInfo
	{
		const TypeInfo* mType;
		ComponentSpawnFunc mFunc;
		ComponentInfo(const TypeInfo* type, ComponentSpawnFunc func)
			:mType(type)
			,mFunc(func)
		{ }
	};

	std::unordered_map<std::string, ActorSpawnFunc> mActorSpawnMap;
	std::unordered_map<std::string, ComponentInfo> mCompSpawnMap;
};

// Helpers - Return true if successful, and also sets out parameter to parsed value
// For each function, the first parameter is the containing JSON object, the second is the
// name of the property in the containing object, and the third is the value you acquire.
// Furthermore, if the property is not found, each function is guaranteed not to modify the
// return value.
bool GetFloatFromJSON(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
bool GetIntFromJSON(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
bool GetStringFromJSON(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
bool GetBoolFromJSON(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
bool GetVectorFromJSON(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
bool GetQuaternionFromJSON(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
