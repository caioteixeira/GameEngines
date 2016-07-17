#pragma once
#include "Math.h"

// Put this in a separate namespace to avoid conflicts
namespace Collision
{
	struct LineSegment
	{
		Vector3 mStart;
		Vector3 mEnd;
	};

	struct AxisAlignedBox
	{
		Vector3 mMin;
		Vector3 mMax;

		// Updates the min/max values based on a new
		// point -- this is used by Mesh when loading
		// to determine the model space bounding box
		void UpdateMinMax(const Vector3& point)
		{
			// TODO: Lab 5
		}
	};

	struct Sphere
	{
		Vector3 mCenter;
		float mRadius;

		void ComputeFromBox(const AxisAlignedBox& box)
		{
			// The midpoint between the min/max should be the center
			// of the sphere
			mCenter = box.mMin + box.mMax;
			mCenter *= 0.5f;

			// The radius is the distance between these two
			mRadius = (mCenter - box.mMin).Length();
		}
	};

	// Helper functions for a variety of intersections
	bool Intersects(const Sphere& a, const Sphere& b);
	bool Intersects(const AxisAlignedBox& a, const AxisAlignedBox& b);

	bool SegmentCast(const LineSegment& segment, const AxisAlignedBox& box, Vector3& outPoint);
}