#include "Quaternion.h"

Quaternion GetRotationTo(Vector3& from, Vector3& dest)
{
	Vector3 fallbackAxis(0, 1, 0);

	// Based on Stan Melax's article in Game Programming Gems
	Quaternion q;
	// Copy, since cannot modify local
	Vector3 v0 = from;
	Vector3 v1 = dest;
	v0.normalize();
	v1.normalize();

	Vector3 c = CrossProduct(v0, v1);

	CoordType d = DotProduct(v0, v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		return Quaternion();
	}
	CoordType s = sqrt( (1+d)*2 );
	if (s < 1e-6f)
	{
		if (fallbackAxis != Vector3(0, 0, 0))
		{
			// rotate 180 degrees about the fallback axis
			q = Quaternion(fallbackAxis, 180.0f);
		}
		else
		{
			// Generate an axis
			Vector3 axis = CrossProduct(Vector3(1, 0, 0), from);
			if (axis.length() == 0.0f) // pick another if colinear
				axis = CrossProduct(Vector3(0,1,0), from);
			axis.normalize();
			q = Quaternion(axis, 180.0f);
		}
	}
	else
	{
		CoordType invs = 1 / s;

		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5f;
	}
	return q;
}
