#pragma once
#include <iostream>
#include "Math.h"
#include "SimdMath.h"
#include "MiniCppUnit.hpp"
#include "PoolAlloc.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace ITP485
{

class SimdVector3Test : public TestFixture<SimdVector3Test>
{
public:
	void PrintVector3(const Vector3& vec)
	{
		printf("<%f, %f, %f>\n", vec.x, vec.y, vec.z);
	}

	TEST_FIXTURE_DESCRIBE(SimdVector3Test, "Testing SimdVector3...")
	{
		//TEST_CASE_DESCRIBE(testLoadSave, "Loading and saving");
		TEST_CASE_DESCRIBE(testAdd, "Addition");
		TEST_CASE_DESCRIBE(testSub, "Subtraction");
		TEST_CASE_DESCRIBE(testMul, "Component-wise multiplication");
		TEST_CASE_DESCRIBE(testScalarMult, "Scalar Multiply");
		TEST_CASE_DESCRIBE(testNormalize, "Normalize");
		TEST_CASE_DESCRIBE(testDotProduct, "Dot Product");
		TEST_CASE_DESCRIBE(testLengthSquared, "Length Squared");
		TEST_CASE_DESCRIBE(testLength, "Length");
		TEST_CASE_DESCRIBE(testCross, "Cross Product");
		TEST_CASE_DESCRIBE(testLerp, "Linear Interpolation");
	}
	void testLoadSave()
	{
		// Create local vector3
 		Vector3 v(1.0f, 2.0f, 3.0f);
		ASSERT_EQUALS_EPSILON(1.0f, v.x, 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, v.y, 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, v.z, 0.01f);

		// Load into SimdVector3
		SimdVector3 sv(v);

		// Now we should be able to fetch it back
		Vector3 r = sv.ToVector3();
		ASSERT_EQUALS_EPSILON(1.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(2.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, r.z, 0.01f);
	}
	void testNormalize()
	{
		Vector3 v(0.0f, 3.0f, 4.0f);
		
		SimdVector3 sv(v);
		sv.Normalize();

		Vector3 r = sv.ToVector3();
		ASSERT_EQUALS_EPSILON(0.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(0.6f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(0.8f, r.z, 0.01f);
	}
	void testDotProduct()
	{
		Vector3 v1(2.0f, 4.0f, 6.0f);
		Vector3 v2(3.0f, 5.0f, 2.0f);

		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);

		SimdVector3 sr = sv1.Dot(sv2);
		Vector3 r = sr.ToVector3();
		ASSERT_EQUALS_EPSILON(38.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(38.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(38.0f, r.z, 0.01f);
	}
	void testAdd()
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(2.0f, 4.0f, 6.0f);
		
		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);
		
		sv1.Add(sv2);

		Vector3 r = sv1.ToVector3();

		ASSERT_EQUALS_EPSILON(3.0f, r.x, 0.01f);
 		ASSERT_EQUALS_EPSILON(6.0f, r.y, 0.01f);
 		ASSERT_EQUALS_EPSILON(9.0f, r.z, 0.01f);
	}
	void testSub()
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(2.0f, 4.0f, 6.0f);

		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);

		sv1.Sub(sv2);

		Vector3 r = sv1.ToVector3();

		ASSERT_EQUALS_EPSILON(-1.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(-3.0f, r.z, 0.01f);
	}
	void testMul()
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(2.0f, 4.0f, 6.0f);

		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);

		sv1.Mul(sv2);

		Vector3 r = sv1.ToVector3();

		ASSERT_EQUALS_EPSILON(2.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(8.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(18.0f, r.z, 0.01f);
	}
	void testLengthSquared()
	{
 		Vector3 v(3.0f, 1.0f, 4.0f);

		SimdVector3 sv(v);
		SimdVector3 sr = sv.LengthSq();

		Vector3 r = sr.ToVector3();

 		ASSERT_EQUALS_EPSILON(26.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(26.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(26.0f, r.z, 0.01f);
	}
	void testLength()
	{
		Vector3 v(3.0f, 0.0f, 4.0f);

		SimdVector3 sv(v);
		SimdVector3 sr = sv.Length();

		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(5.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(5.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(5.0f, r.z, 0.01f);
	}
	void testCross()
	{
 		Vector3 v1(3.0f, 2.0f, 5.0f);
 		Vector3 v2(4.0f, 6.0f, 1.0f);

		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);

		SimdVector3 sr = sv1.Cross(sv2);
		Vector3 r = sr.ToVector3();

 		ASSERT_EQUALS_EPSILON(-28.0f, r.x, 0.01f);
 		ASSERT_EQUALS_EPSILON(17.0f, r.y, 0.01f);
 		ASSERT_EQUALS_EPSILON(10.0f, r.z, 0.01f);
	}
	void testLerp()
	{
		Vector3 v1(1.0f, 10.0f, 100.0f);
		Vector3 v2(0.0f, 0.0f, 0.0f);

		SimdVector3 sv1(v1);
		SimdVector3 sv2(v2);
		
		SimdVector3 sr = sv1.Lerp(sv2, 0.75f);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(0.25f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(2.5f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(25.0f, r.z, 0.01f);
	}
	void testScalarMult()
	{
		Vector3 v1(1.0f, 4.0f, 5.0f);
		
		SimdVector3 sv(v1);
		sv.Mul(3.0f);

		Vector3 r = sv.ToVector3();
		ASSERT_EQUALS_EPSILON(3.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(12.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(15.0f, r.z, 0.01f);
	}
};

class SimdMatrix4Test : public TestFixture<SimdMatrix4Test>
{
public:
	void PrintVector3(const Vector3& vec)
	{
		printf("<%f, %f, %f>\n", vec.x, vec.y, vec.z);
	}

	TEST_FIXTURE_DESCRIBE(SimdMatrix4Test, "TestingSimdMatrix4...")
	{
		//TEST_CASE_DESCRIBE(testLoadSave, "Loading and saving");
		TEST_CASE_DESCRIBE(testVectorTransform, "Transform a vector (as a position) by a matrix");
		TEST_CASE_DESCRIBE(testVectorTransformX2, "Transform a vector (as a position) by a matrix and back");
		TEST_CASE_DESCRIBE(testVectorTransformAsVector, "Transform a vector (as a vector) by a matrix");
		TEST_CASE_DESCRIBE(testMatrixMult, "Multiply two matrices together (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixScale, "Creates scale matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateX, "Creates rotate X matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateY, "Creates rotate Y matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixRotateZ, "Creates rotate Z matrix (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixTranslate, "Creates translate matrix (then apply to vector)");
//		TEST_CASE_DESCRIBE(testMatrixQuaternion, "Creates a matrix from a quaternion (then apply to vector)");
// 		TEST_CASE_DESCRIBE(testMatrixInvert, "Inverts a matrix");
	}
	void testLoadSave()
	{
		float data[4][4] =
		{
			0.499999970f, -0.866025448f, 0.000000000f, 10.0000000f,
			0.612372458f, 0.353553355f, -0.707106769f, -14.1421356f,
			0.612372458f, 0.353553355f, 0.707106769f, 42.4264069f,
			0.000000000f, 0.000000000f, 0.000000000f, 1.00000000f
		};

		Matrix4 m(data);

		SimdMatrix4 sm(m);

		Matrix4 r = sm.ToMatrix4();

 		for (int i = 0; i < 4; ++i)
 		{
 			for (int j = 0; j < 4; ++j)
 			{
 				ASSERT_EQUALS_EPSILON(m.mat[i][j], r.mat[i][j], 0.01f);
 			}
 		}
	}
	void testVectorTransform()
	{
		Vector3 v;
		float trans[4][4] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-5.0f, -10.0f, 4.0f, 1.0f
		};
		Matrix4 translate(trans);

		SimdVector3 sv(v);
		SimdMatrix4 sm(translate);

		SimdVector3 sr = Transform(sv, sm);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(-5.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(-10.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(4.0f, r.z, 0.01f);
	}
	void testVectorTransformX2()
	{
		Vector3 v(5.0f, 5.0f, 5.0f);
		float trans[4][4] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-5.0f, -10.0f, 4.0f, 1.0f
		};
		Matrix4 translate(trans);

		float trans2[4][4] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			5.0f, 10.0f, -4.0f, 1.0f
		};
		Matrix4 translate2(trans2);

		SimdVector3 sv(v);
		SimdMatrix4 sm1(translate);
		SimdMatrix4 sm2(translate2);

		SimdVector3 sr = Transform(sv, sm1);
		sr = Transform(sr, sm2);

		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(5.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(5.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(5.0f, r.z, 0.01f);
	}
	void testVectorTransformAsVector()
	{
		Vector3 v(1.0f, 1.0f, 1.0f);
		float trans[4][4] =
		{
			3.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 2.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 8.0f, 0.0f,
			-5.0f, -10.0f, 4.0f, 1.0f
		};
		Matrix4 transform(trans);

		SimdVector3 sv(v);
		SimdMatrix4 sm(transform);

		SimdVector3 sr = Transform(sv, sm, 0.0f);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(3.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(2.5f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(8.0f, r.z, 0.01f);
	}
	void testMatrixMult()
	{
		float data0[4][4] =
		{
			1.00000000f, 0.000000000f, 0.000000000f, 0.000000000f,
			0.000000000f, 0.707106769f, -0.707106769f, 0.000000000f,
			0.000000000f, 0.707106769f, 0.707106769f, 0.000000000f,
			0.000000000f, 0.000000000f, 0.000000000f, 1.00000000f
		};
		Matrix4 mat0(data0);

		float data1[4][4] =
		{
			1.00000000f, 0.000000000f, 0.000000000f, 10.0000000f,
			0.000000000f, 1.00000000f, 0.000000000f, 20.0000000f,
			0.000000000f, 0.000000000f, 1.00000000f, 40.0000000f,
			0.000000000f, 0.000000000f, 0.000000000f, 1.00000000f
		};
		Matrix4 mat1(data1);

		float data2[4][4] =
		{
			0.499999970f, -0.866025448f, 0.000000000f, 0.000000000f,
			0.866025448f, 0.499999970f, 0.000000000f, 0.000000000f,
			0.000000000f, 0.000000000f, 1.00000000f, 0.000000000f,
			0.000000000f, 0.000000000f, 0.000000000f, 1.00000000f
		};
		Matrix4 mat2(data2);


		float result[4][4] =
		{
			0.499999970f, -0.866025448f, 0.000000000f, 10.0000000f,
			0.612372458f, 0.353553355f, -0.707106769f, -14.1421356f,
			0.612372458f, 0.353553355f, 0.707106769f, 42.4264069f,
			0.000000000f, 0.000000000f, 0.000000000f, 1.00000000f
		};
		Matrix4 matR(result);

		SimdMatrix4 smat0(mat0);
		SimdMatrix4 smat1(mat1);
		SimdMatrix4 smat2(mat2);

		smat0.Mul(smat1);
		smat0.Mul(smat2);

		Matrix4 r = smat0.ToMatrix4();

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				ASSERT_EQUALS_EPSILON(r.mat[i][j], matR.mat[i][j], 0.01f);
			}
		}
	}
	void testMatrixScale()
	{
		SimdMatrix4 scale;
		scale.LoadScale(3.0f);
		
		Vector3 v(1.0f, 1.0f, 1.0f);
		SimdVector3 sv(v);

		SimdVector3 sr = Transform(sv, scale);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(3.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, r.z, 0.01f);
	}
	void testMatrixRotateX()
	{
		SimdMatrix4 rot;
		rot.LoadRotationX(Math::PiOver2);
		
		Vector3 v(Vector3::UnitY);
		SimdVector3 sv(v);

		SimdVector3 sr = Transform(sv, rot);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(0.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, r.z, 0.01f);
	}
	void testMatrixRotateY()
	{
		SimdMatrix4 rot;
		rot.LoadRotationY(Math::PiOver2);

		Vector3 v(Vector3::UnitX);
		SimdVector3 sv(v);

		SimdVector3 sr = Transform(sv, rot);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(0.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(-1.0f, r.z, 0.01f);
	}
	void testMatrixRotateZ()
	{
		SimdMatrix4 rot;
		rot.LoadRotationZ(Math::PiOver2);

		Vector3 v(Vector3::UnitX);
		SimdVector3 sv(v);

		SimdVector3 sr = Transform(sv, rot);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(0.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(1.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(0.0f, r.z, 0.01f);
	}
	void testMatrixTranslate()
	{
		SimdMatrix4 trans;
		trans.LoadTranslation(Vector3(5.0f, 10.0f, -4.0f));

		Vector3 v;
		SimdVector3 sv(v);

		SimdVector3 sr = Transform(sv, trans);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(5.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(10.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(-4.0f, r.z, 0.01f);
	}
	void testMatrixQuaternion()
	{
		Vector3 axis(1.0f, 0.0f, 0.0f);
		Quaternion q(axis, Math::Pi / 6.0f);
		
		Vector3 v(Vector3::UnitY);
		SimdVector3 sv(v);
		
		SimdMatrix4 m;
		m.LoadFromQuaternion(q);
		SimdVector3 sr = Transform(sv, m);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(0.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(0.866f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(0.5f, r.z, 0.01f);
	}

	void testMatrixInvert()
	{
		SimdMatrix4 trans;
		trans.LoadTranslation(Vector3(1.0f, 2.0f, -3.0f));

		trans.Invert();
		SimdVector3 sv(Vector3::Zero);

		SimdVector3 sr = Transform(sv, trans);
		Vector3 r = sr.ToVector3();

		ASSERT_EQUALS_EPSILON(-1.0f, r.x, 0.01f);
		ASSERT_EQUALS_EPSILON(-2.0f, r.y, 0.01f);
		ASSERT_EQUALS_EPSILON(3.0f, r.z, 0.01f);
	}
};

typedef PoolAllocator<32, 64> SmallPool;
typedef PoolAllocator<16, 16> TestPool;

class PoolAllocatorTest : public TestFixture<PoolAllocatorTest>
{
public:
	TEST_FIXTURE_DESCRIBE(PoolAllocatorTest, "Testing Pool Allocator...")
	{
		TEST_CASE_DESCRIBE(testAllocFree32, "Allocate/Free, Allocate/Free single 32 byte block");
		TEST_CASE_DESCRIBE(testOverflow, "If small pool is full, does it return 0? (SHOULD ASSERT IN DEBUG!)");
		TEST_CASE_DESCRIBE(testReuse, "Allocate/Free all 32 byte blocks 10 times in a row with no errors");
		TEST_CASE_DESCRIBE(testBoundaryCheck, "Allocate, Overwrite Boundary, Free (SHOULD ASSERT IN DEBUG!)");
		TEST_CASE_DESCRIBE(testReuseRandom, "Allocate all blocks/randomly free some, try to allocate again.");
	}
	void testAllocFree32()
	{
 		SmallPool pool;
 		char* temp = reinterpret_cast<char*>(pool.Allocate(32));
		ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
		ASSERT_TEST_MESSAGE(pool.GetNumBlocksFree() == 63, "Incorrect number of blocks remaining.");
 		pool.Free(temp);
		ASSERT_TEST_MESSAGE(pool.GetNumBlocksFree() == 64, "Incorrect number of blocks remaining.");
#ifdef _DEBUG
 		// Make sure when we call free, the data is overwritten
 		ASSERT_EQUALS((char)0xde, (*temp));
#endif
 		char* temp2 = reinterpret_cast<char*>(pool.Allocate(32));
 		ASSERT_TEST_MESSAGE(temp == temp2, "Block not returned to front of free list on Free");
 		pool.Free(temp2);
	}
	void testOverflow()
	{
		SmallPool pool;
 		void* temp;
 		for (int i = 0; i < 64; i++)
 		{
			ASSERT_TEST_MESSAGE(pool.GetNumBlocksFree() == (64 - i), "Incorrect number of blocks remaining.");
 			temp = pool.Allocate(32);
 			ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
			//printf("%d\n", sizeof(PoolBlock<32>));
			//printf("temp = 0x%x\n", temp);
 		}
 
 		// make sure we return 0 when full
 		temp = pool.Allocate(32);
 		ASSERT_TEST_MESSAGE(temp == 0, "Didn't return 0 when absolutely no blocks left.");
	}
	void testReuse()
	{
		SmallPool pool;
 		void* pointers[64];
 		void* pointers2[64];
 		//void* temp;
 		for (int i = 0; i < 5; i++)
 		{
 			for (int j = 0; j < 64; j++)
 			{
 				pointers[j] = pool.Allocate(32);
 				ASSERT_TEST_MESSAGE(pointers[j] != 0, "Allocate returned 0 when there should be blocks left.");
 			}
 			//temp = pool.Allocate(32);
 			//ASSERT_TEST_MESSAGE(temp == 0, "Allocate did not return 0 when full.");
 
 			for (int j = 0; j < 64; j++)
 			{
 				pool.Free(pointers[64 - j - 1]);
 			}
 
 			for (int j = 0; j < 64; j++)
 			{
 				pointers2[j] = pool.Allocate(32);
 				ASSERT_TEST_MESSAGE(pointers[j] == pointers2[j], "Blocks not returned to free list in expected order.")
 			}
 			//temp = pool.Allocate(32);
 			//ASSERT_TEST_MESSAGE(temp == 0, "Allocate did not return 0 when full.");
 
 			for (int j = 0; j < 64; j++)
 			{
 				pool.Free(pointers[64 - j - 1]);
 			}
 		}
	}
	void testBoundaryCheck()
	{
		SmallPool pool;
 		char* temp = reinterpret_cast<char*>(pool.Allocate(32));
		ASSERT_TEST_MESSAGE(temp != 0, "Allocate returned 0 when there should be blocks left.");
 		temp[32] = 'a';
 		pool.Free(temp);
	}
	void testReuseRandom()
	{
		SmallPool pool;
		std::vector<void*> blocks;
		for (int i = 0; i < 64; i++)
		{
			blocks.push_back(pool.Allocate(32));
		}

		std::srand(static_cast<unsigned>(std::time(0)));
		std::random_shuffle(blocks.begin(), blocks.end());
		for (int i = 0; i < 32; i++)
		{
			void* temp = blocks.back();
			pool.Free(temp);
			blocks.pop_back();
		}

		for (int i = 0; i < 32; i++)
		{
			void* temp = pool.Allocate(32);
			ASSERT_TEST_MESSAGE(std::find(blocks.begin(), blocks.end(), temp) == blocks.end(),
				"Allocate returned a pointer that's already in use.");
			blocks.push_back(temp);
		}
	}
};

REGISTER_FIXTURE(SimdVector3Test);
REGISTER_FIXTURE(SimdMatrix4Test);
REGISTER_FIXTURE(PoolAllocatorTest);
} // namespace ITP485
