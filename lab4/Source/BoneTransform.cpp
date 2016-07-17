#include "ITPEnginePCH.h"

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mTranslation);

	Matrix4 rotationMatrix = Matrix4::CreateFromQuaternion(mRotation);

	return rotationMatrix * translationMatrix;

}

BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform out;

	out.mRotation = Slerp(a.mRotation, b.mRotation, f);
	out.mTranslation = Lerp(a.mTranslation, b.mTranslation, f);
	
	return out;
}
