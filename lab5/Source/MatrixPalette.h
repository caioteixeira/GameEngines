#pragma once
#include "Math.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette
{
	Matrix4 mSkeletonBones[MAX_SKELETON_BONES];
};
