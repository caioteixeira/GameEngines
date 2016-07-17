#pragma once
#include "GraphicsDriver.h"
#include <string>
#include <unordered_map>

class InputLayoutCache
{
public:
	InputLayoutPtr GetLayout(const std::string& inLayoutName);
	void RegisterLayout(const std::string& inLayoutName, InputLayoutPtr inLayout);

private:
	std::unordered_map<std::string, InputLayoutPtr> mNameToLayoutMap;
};
