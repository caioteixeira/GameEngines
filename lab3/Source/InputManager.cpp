#include "ITPEnginePCH.h"

void InputManager::HandleKeyPressed(int key)
{
	// Is this an action?
	auto actIter = mKeyToActionMap.find(key);
	if (actIter != mKeyToActionMap.end())
	{
		// Trigger the "pressed" delegate if it exists
		if (actIter->second->mPressedDelegate)
		{
			actIter->second->mPressedDelegate->Execute();
		}
	}

	// Is this an axis?
	auto axIter = mKeyToAxisMap.find(key);
	if (axIter != mKeyToAxisMap.end())
	{
		if (key == axIter->second->mPositiveKey)
		{
			axIter->second->mValue += 1.0f;
		}
		else
		{
			axIter->second->mValue -= 1.0f;
		}

		if (axIter->second->mDelegate)
		{
			axIter->second->mDelegate->Execute(axIter->second->mValue);
		}
	}
}

void InputManager::HandleKeyReleased(int key)
{
	// Is this an action?
	auto actIter = mKeyToActionMap.find(key);
	if (actIter != mKeyToActionMap.end())
	{
		// Trigger the "pressed" delegate if it exists
		if (actIter->second->mReleasedDelegate)
		{
			actIter->second->mReleasedDelegate->Execute();
		}
	}

	// Is this an axis?
	auto axIter = mKeyToAxisMap.find(key);
	if (axIter != mKeyToAxisMap.end())
	{
		if (key == axIter->second->mPositiveKey)
		{
			axIter->second->mValue -= 1.0f;
		}
		else
		{
			axIter->second->mValue += 1.0f;
		}

		if (axIter->second->mDelegate)
		{
			axIter->second->mDelegate->Execute(axIter->second->mValue);
		}
	}
}

void InputManager::AddActionMapping(const std::string& name, int key)
{
	ActionMappingPtr ptr = std::make_shared<ActionMapping>();
	ptr->mName = name;
	ptr->mKey = key;

	mNameToActionMap.emplace(name, ptr);
	mKeyToActionMap.emplace(key, ptr);
}

void InputManager::AddAxisMapping(const std::string& name, int positiveKey, int negativeKey)
{
	AxisMappingPtr ptr = std::make_shared<AxisMapping>();
	ptr->mName = name;
	ptr->mPositiveKey = positiveKey;
	ptr->mNegativeKey = negativeKey;
	ptr->mValue = 0.0f;

	mNameToAxisMap.emplace(name, ptr);
	mKeyToAxisMap.emplace(positiveKey, ptr);
	mKeyToAxisMap.emplace(negativeKey, ptr);
}

void InputManager::ParseMappings(const char* fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		SDL_Log("Input mapping file %s not found", fileName);
		return;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Input mapping file %s is not valid JSON", fileName);
		return;
	}

	// Parse action mappings
	const rapidjson::Value& actions = doc["ActionMappings"];
	if (actions.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < actions.Size(); i++)
		{
			const rapidjson::Value& name = actions[i]["name"];
			const rapidjson::Value& key = actions[i]["key"];

			if (name.IsString() && key.IsInt())
			{
				AddActionMapping(name.GetString(), key.GetInt());
			}
		}
	}

	// Parse axis mappings
	const rapidjson::Value& axii = doc["AxisMappings"];
	if (axii.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < axii.Size(); i++)
		{
			const rapidjson::Value& name = axii[i]["name"];
			const rapidjson::Value& posKey = axii[i]["posKey"];
			const rapidjson::Value& negKey = axii[i]["negKey"];

			if (name.IsString() && posKey.IsInt() && negKey.IsInt())
			{
				AddAxisMapping(name.GetString(), posKey.GetInt(), negKey.GetInt());
			}
		}
	}
}

void InputManager::BindActionInternal(const std::string& name, InputEvent event, ActionDelegatePtr delegate)
{
	auto iter = mNameToActionMap.find(name);
	if (iter != mNameToActionMap.end())
	{
		if (event == IE_Pressed)
		{
			iter->second->mPressedDelegate = delegate;
		}
		else
		{
			iter->second->mReleasedDelegate = delegate;
		}
	}
}

void InputManager::BindAxisInternal(const std::string& name, AxisDelegatePtr delegate)
{
	auto iter = mNameToAxisMap.find(name);
	if (iter != mNameToAxisMap.end())
	{
		iter->second->mDelegate = delegate;
	}
}
