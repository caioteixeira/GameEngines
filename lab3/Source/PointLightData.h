#pragma once
#include "Math.h"

const size_t MAX_POINT_LIGHTS = 8;

struct PointLightData
{
	PointLightData();

	Vector3 mDiffuse;
	float p; //padding
	Vector3 mSpecular;
	float p2; //padding
	Vector3 mPosition;
	float mSpecularPower;
	float mInnerRadius;
	float mOuterRadius;
	unsigned int mEnabled;
	float p4; //padding
};
