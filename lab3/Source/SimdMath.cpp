#include "ITPEnginePCH.h"

void SimdMatrix4::LoadFromQuaternion(const Quaternion& quat)
{
	// Sanjay: Don't have time to implement this properly
	Matrix4 temp = Matrix4::CreateFromQuaternion(quat);
	*this = SimdMatrix4(temp);
}

void SimdMatrix4::Invert()
{
	// Sanjay: Don't have time to implement this properly
	Matrix4 temp = ToMatrix4();
	temp.Invert();
	*this = SimdMatrix4(temp);
}
