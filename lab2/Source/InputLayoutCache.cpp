#include "ITPEnginePCH.h"

InputLayoutPtr InputLayoutCache::GetLayout( const std::string& inLayoutName )
{
	auto toRet = mNameToLayoutMap.find( inLayoutName );
	return toRet != mNameToLayoutMap.end() ? toRet->second : nullptr;
}

void InputLayoutCache::RegisterLayout( const std::string& inLayoutName, InputLayoutPtr inLayout )
{
	mNameToLayoutMap[ inLayoutName ] = inLayout;
}
