#pragma once
#include "Math.h"
#include <xmmintrin.h>
#include <smmintrin.h>

// SHUFFLER is like shuffle, but has easier to understand indices
#define _MM_SHUFFLER( xi, yi, zi, wi ) _MM_SHUFFLE( wi, zi, yi, xi )

class alignas(16) SimdVector3
{
	// Underlying vector
	__m128 mVec;
public:
	// Empty default constructor
	SimdVector3() { }

	// Constructor from __m128
	explicit SimdVector3(__m128 vec)
	{
		mVec = vec;
	}

	// Constructor if converting from Vector3
	explicit SimdVector3(const Vector3& vec)
	{
		FromVector3(vec);
	}

	// Load from a Vector3 into this SimdVector3
	void FromVector3(const Vector3& vec)
	{
		// We can't assume this is aligned
		mVec = _mm_setr_ps(vec.x, vec.y, vec.z, 0.0f);
	}

	// Convert this SimdVector3 to a Vector3
	Vector3 ToVector3() const
	{
		return Vector3(mVec);
	}

	// this = this + other
	void Add(const SimdVector3& other)
	{
		this->mVec = _mm_add_ps(this->mVec, other.mVec);
	}

	// this = this - other
	void Sub(const SimdVector3& other)
	{
		this->mVec = _mm_sub_ps(this->mVec, other.mVec);
	}

	// this = this * other (componentwise)
	void Mul(const SimdVector3& other)
	{
		this->mVec = _mm_mul_ps(this->mVec, other.mVec);
	}

	// this = this * scalar
	void Mul(float scalar)
	{
		__m128 scalar_vector = _mm_set_ps1(scalar);
		this->mVec = _mm_mul_ps(this->mVec, scalar_vector);
	}

	// Normalize this vector
	void Normalize()
	{
		__m128 temp = _mm_dp_ps(this->mVec, this->mVec, 0x7F); //Dot with itself
		__m128 sqr = _mm_rsqrt_ps(temp); //Take square root
		this->mVec = _mm_mul_ps(this->mVec, sqr); //multiply
	}

	// (this dot other), storing the dot product
	// in EVERY COMPONENT of returned SimdVector3
	SimdVector3 Dot(const SimdVector3& other) const
	{
		SimdVector3 r;
		r.mVec = _mm_dp_ps(this->mVec, other.mVec, 0x7F);

		return r;
	}

	// Length Squared of this, storing the result in
	// EVERY COMPONENT of returned SimdVector3
	SimdVector3 LengthSq() const
	{
		return Dot(*this);
	}

	// Length of this, storing the result in
	// EVERY COMPONENT of returned SimdVector3
	SimdVector3 Length() const
	{
		SimdVector3 r;
		__m128 temp = _mm_dp_ps(this->mVec, this->mVec, 0x7F); //Dot with itself
		r.mVec = _mm_sqrt_ps(temp); //Take square root
		return r;
	}

	// result = this (cross) other
	SimdVector3 Cross(const SimdVector3& other) const
	{
		SimdVector3 r;

		__m128 tempA = _mm_shuffle_ps(this->mVec, this->mVec, _MM_SHUFFLER(1, 2, 0, 3));
		__m128 tempB = _mm_shuffle_ps(other.mVec, other.mVec, _MM_SHUFFLER(2, 0, 1, 3));
		r.mVec = _mm_mul_ps(tempA, tempB);

		tempA = _mm_shuffle_ps(this->mVec, this->mVec, _MM_SHUFFLER(2, 0, 1, 3));
		tempB = _mm_shuffle_ps(other.mVec, other.mVec, _MM_SHUFFLER(1, 2, 0, 3));

		tempA = _mm_mul_ps(tempA, tempB);

		r.mVec = _mm_sub_ps(r.mVec, tempA);

		return r;
	}

	// result = this * (1.0f - f) + other * f
	SimdVector3 Lerp(const SimdVector3& other, float f) const
	{
		__m128 scalar = _mm_set_ps1(1.0f - f);
		__m128 r = _mm_mul_ps(this->mVec, scalar); //multiply
		
		scalar = _mm_set_ps1(f);
		__m128 t = _mm_mul_ps(other.mVec, scalar);
		
		r = _mm_add_ps(r, t);

		SimdVector3 out(r);

		return out;
	}

	friend SimdVector3 Transform(const SimdVector3& vec, const class SimdMatrix4& mat, float w);
};

class alignas(16) SimdMatrix4
{
	// Four vectors, one for each row
	__m128 mRows[4];
public:
	// Empty default constructor
	SimdMatrix4() { }

	// Constructor from array of four __m128s
	explicit SimdMatrix4(__m128 rows[4])
	{
		memcpy(mRows, rows, sizeof(__m128) * 4);
	}

	// Constructor if converting from Matrix4
	explicit SimdMatrix4(const Matrix4& mat)
	{
		FromMatrix4(mat);
	}

	// Load from a Matrix4 into this SimdMatrix4
	void FromMatrix4(const Matrix4& mat)
	{
		// We can't assume that mat is aligned, so
		// we can't use mm_set_ps
		memcpy(mRows, mat.mat, sizeof(float) * 16);
	}

	// Convert this SimdMatrix4 to a Matrix4
	Matrix4 ToMatrix4()
	{
		return Matrix4(mRows);
	}

	// this = this * other
	void Mul(const SimdMatrix4& other)
	{
		//Transpose B
		SimdMatrix4 otherT(other);
		otherT.Transpose();

		//Dot products
		for (int i = 0; i < 4; i++) {
			__m128 d0 = _mm_dp_ps(this->mRows[i], otherT.mRows[0], 0xF1);
			__m128 d1 = _mm_dp_ps(this->mRows[i], otherT.mRows[1], 0xF2);
			__m128 d2 = _mm_dp_ps(this->mRows[i], otherT.mRows[2], 0xF4);
			__m128 d3 = _mm_dp_ps(this->mRows[i], otherT.mRows[3], 0xF8);

			__m128 r_vector = _mm_add_ps(d0, d1);
			r_vector = _mm_add_ps(r_vector, d2);
			r_vector = _mm_add_ps(r_vector, d3);

			this->mRows[i] = r_vector;
		}

	}

	// Transpose this matrix
	void Transpose()
	{
		_MM_TRANSPOSE4_PS(mRows[0], mRows[1], mRows[2], mRows[3]);
	}

	// Loads a Scale matrix into this
	void LoadScale(float scale)
	{
		// scale 0 0 0
		mRows[0] = _mm_set_ss(scale);
		mRows[0] = _mm_shuffle_ps(mRows[0], mRows[0], _MM_SHUFFLE(1, 1, 1, 0));

		// 0 scale 0 0
		mRows[1] = _mm_set_ss(scale);
		mRows[1] = _mm_shuffle_ps(mRows[1], mRows[1], _MM_SHUFFLE(1, 1, 0, 1));

		// 0 0 scale 0
		mRows[2] = _mm_set_ss(scale);
		mRows[2] = _mm_shuffle_ps(mRows[2], mRows[2], _MM_SHUFFLE(1, 0, 1, 1));

		// 0 0 0 1
		mRows[3] = _mm_set_ss(1.0f);
		mRows[3] = _mm_shuffle_ps(mRows[3], mRows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Loads a rotation about the X axis into this
	void LoadRotationX(float angle)
	{
		// 1 0 0 0
		mRows[0] = _mm_set_ss(1.0f);
		mRows[0] = _mm_shuffle_ps(mRows[0], mRows[0], _MM_SHUFFLE(1, 1, 1, 0));

		float cosTheta = Math::Cos(angle);
		float sinTheta = Math::Sin(angle);

		// 0 cos sin 0
		mRows[1] = _mm_setr_ps(0.0f, cosTheta, sinTheta, 0.0f);

		// 0 -sin cos 0
		mRows[2] = _mm_setr_ps(0.0f, -sinTheta, cosTheta, 0.0f);

		// 0 0 0 1
		mRows[3] = _mm_set_ss(1.0f);
		mRows[3] = _mm_shuffle_ps(mRows[3], mRows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Loads a rotation about the Y axis into this
	void LoadRotationY(float angle)
	{
		float cosTheta = Math::Cos(angle);
		float sinTheta = Math::Sin(angle);

		// cos 0 -sin 0
		mRows[0] = _mm_setr_ps(cosTheta, 0.0f, -sinTheta, 0.0f);

		// 0 1 0 0
		mRows[1] = _mm_set_ss(1.0f);
		mRows[1] = _mm_shuffle_ps(mRows[1], mRows[1], _MM_SHUFFLER(1, 0, 1, 1));

		// sin 0 cos 0
		mRows[2] = _mm_setr_ps(sinTheta, 0.0f, cosTheta, 0.0f);

		// 0 0 0 1
		mRows[3] = _mm_set_ss(1.0f);
		mRows[3] = _mm_shuffle_ps(mRows[3], mRows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Loads a rotation about the Z axis into this
	void LoadRotationZ(float angle)
	{
		float cosTheta = Math::Cos(angle);
		float sinTheta = Math::Sin(angle);

		// cos sin 0 0
		mRows[0] = _mm_setr_ps(cosTheta, sinTheta, 0.0f, 0.0f);

		// -sin cos 0 0
		mRows[1] = _mm_setr_ps(-sinTheta, cosTheta, 0.0f, 0.0f);

		// 0 0 1 0
		mRows[2] = _mm_set_ss(1.0f);
		mRows[2] = _mm_shuffle_ps(mRows[2], mRows[2], _MM_SHUFFLER(1, 1, 0, 1));

		// 0 0 0 1
		mRows[3] = _mm_set_ss(1.0f);
		mRows[3] = _mm_shuffle_ps(mRows[3], mRows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Loads a translation matrix into this
	void LoadTranslation(const Vector3& trans)
	{
		// 1 0 0 0
		mRows[0] = _mm_set_ss(1.0f);
		mRows[0] = _mm_shuffle_ps(mRows[0], mRows[0], _MM_SHUFFLER(0, 1, 1, 1));

		// 0 1 0 0
		mRows[1] = _mm_set_ss(1.0f);
		mRows[1] = _mm_shuffle_ps(mRows[1], mRows[1], _MM_SHUFFLER(1, 0, 1, 1));

		// 0 0 1 0
		mRows[2] = _mm_set_ss(1.0f);
		mRows[2] = _mm_shuffle_ps(mRows[2], mRows[2], _MM_SHUFFLER(1, 1, 0, 1));

		mRows[3] = _mm_setr_ps(trans.x, trans.y, trans.z, 1.0f);
	}

	// Loads a matrix from a quaternion into this
	void LoadFromQuaternion(const Quaternion& quat);

	// Inverts this matrix
	void Invert();

	friend SimdVector3 Transform(const SimdVector3& vec, const class SimdMatrix4& mat, float w);
};

inline SimdVector3 Transform(const SimdVector3& vec, const SimdMatrix4& mat, float w = 1.0f)
{
	//Transposed matrix
	SimdMatrix4 tMat(mat); 
	tMat.Transpose(); 

	//set w component
	__m128 w_vector = _mm_set_ss(w);
	
	__m128 v = _mm_insert_ps(vec.mVec, w_vector, 0x30);

	//Dot product
	__m128 d0 = _mm_dp_ps(v, tMat.mRows[0], 0xF1);
	__m128 d1 = _mm_dp_ps(v, tMat.mRows[1], 0xF2);
	__m128 d2 = _mm_dp_ps(v, tMat.mRows[2], 0xF4);
	__m128 d3 = _mm_dp_ps(v, tMat.mRows[3], 0xF8);

	__m128 r_vector = _mm_add_ps(d0, d1);
	r_vector = _mm_add_ps(r_vector, d2);
	r_vector = _mm_add_ps(r_vector, d3);

	SimdVector3 r(r_vector);
	return r; // Fix return value
}
