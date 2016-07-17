#include "ITPEnginePCH.h"

namespace Collision
{

	bool Intersects(const Sphere& a, const Sphere& b)
	{
		Vector3 diff = a.mCenter - b.mCenter;
		float DistSq = diff.LengthSq();
		float sumRadiiSq = (a.mRadius + b.mRadius) * (a.mRadius + b.mRadius);
		if (DistSq <= sumRadiiSq)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(const AxisAlignedBox & a, const AxisAlignedBox & b)
	{
		if(a.mMin.x > b.mMax.x)
		{
			return false;
		}

		if(a.mMax.x < b.mMin.x)
		{
			return false;
		}

		if (a.mMin.y > b.mMax.y)
		{
			return false;
		}

		if (a.mMax.y < b.mMin.y)
		{
			return false;
		}

		if (a.mMin.z > b.mMax.z)
		{
			return false;
		}

		if (a.mMax.z < b.mMin.z)
		{
			return false;
		}

		return true;
	}

	bool SegmentCast(const LineSegment& segment, const AxisAlignedBox& box, Vector3& outPoint)
	{
		Vector3 d = segment.mEnd - segment.mStart;
		
		float tMin = 0.0f;
		float tMax = d.Length();

		d.Normalize();

		//x slab
		if (Math::Abs(d.x) < 0.0001f)
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (segment.mStart.x < box.mMin.x || segment.mStart.x > box.mMax.x)
			{
				return false;
			}
		} else {
			//Compute intersection
			float ood = 1.0f / d.x;
			float t1 = (box.mMin.x - segment.mStart.x) * ood;
			float t2 = (box.mMax.x - segment.mStart.x) * ood;
			//Make t1 be intersection with near plane
			if (t1 > t2)
			{
				float t = t1;
				t1 = t2;
				t2 = t;
			}
			//Compute the intersection
			tMin = Math::Max(t1, tMin);
			tMax = Math::Min(t2, tMax);

			//Exit with no collision
			if (tMin > tMax)
			{
				return false;
			}
		}

		//y slab
		if (Math::Abs(d.y) < 0.0001f)
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (segment.mStart.y < box.mMin.y || segment.mStart.y > box.mMax.y)
			{
				return false;
			}
		} else {
			//Compute intersection
			float ood = 1.0f / d.y;
			float t1 = (box.mMin.y - segment.mStart.y) * ood;
			float t2 = (box.mMax.y - segment.mStart.y) * ood;
			//Make t1 be intersection with near plane
			if (t1 > t2)
			{
				float t = t1;
				t1 = t2;
				t2 = t;
			}
			//Compute the intersection
			tMin = Math::Max(t1, tMin);
			tMax = Math::Min(t2, tMax);

			//Exit with no collision
			if (tMin > tMax)
			{
				return false;
			}
		}

		//z slab
		if (Math::Abs(d.z) < 0.0001f)
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (segment.mStart.z < box.mMin.z || segment.mStart.z > box.mMax.z)
			{
				return false;
			}
		} else {
			//Compute intersection
			float ood = 1.0f / d.z;
			float t1 = (box.mMin.z - segment.mStart.z) * ood;
			float t2 = (box.mMax.z - segment.mStart.z) * ood;
			//Make t1 be intersection with near plane
			if (t1 > t2)
			{
				float t = t1;
				t1 = t2;
				t2 = t;
			}
			//Compute the intersection
			tMin = Math::Max(t1, tMin);
			tMax = Math::Min(t2, tMax);

			//Exit with no collision
			if (tMin > tMax)
			{
				return false;
			}
		}

		//If it's outside the segment lenght
		/*if(tMin > endT)
		{
			return false;
		}*/

		outPoint = segment.mStart + (d * tMin);
		return true;
	}

} // namespace
