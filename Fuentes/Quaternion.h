/* Copyright (C) Dante Treglia II, 2000. 
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright (C) Dante Treglia II, 2000"
*/
#ifndef _QUATERNION_H
#define _QUATERNION_H
#include "Vectors.h"

#define ERROR_TOLERANCE 1e-6

#define RADIANSDEGREE 0.01745329252f // The number of radians in ONE degree rotation
#define M(x, y) matrix[ x + y * 4 ] // Accessing a matrix element

class Quaternion {
public:
	CoordType x, y, z, w;
	
public:
	Quaternion() : x(0), y(0), z(0), w(1) {};
	Quaternion(const Quaternion &q) : x(q.x), y(q.y), z(q.z), w(q.w) {};
	
	Quaternion(CoordType xI, CoordType yI,  CoordType zI, CoordType wI) : x(xI), y(yI), 
		z(zI), w(wI) {}

	Quaternion(Vector3 axis, CoordType angle) {
		angle = angle * RADIANSDEGREE;
		
		CoordType d, s;
		d = axis.length();
		assert(d != 0 && "Quaternion Axis is zero!");
		s = sin(angle * 0.5f) / d;
		SetValues(axis.x * s, axis.y * s, axis.z * s, cos(angle * 0.5f));
	}
	Quaternion(CoordType yaw, CoordType pitch, CoordType roll) {SetEuler(yaw, pitch, 
		roll);}

public:
	// Multiply Equal
	Quaternion &operator *= (const Quaternion &q) {
		Quaternion temp = *this * q;
		*this = temp;
		return *this;
	}
	// Factor Equal
	Quaternion &operator *= (CoordType s) {
		SetValues(x * s, y * s, z * s, w * s);
		return *this;
	}
	// Add Equal
	Quaternion &operator += (const Quaternion &q) {
		SetValues(x + q.x, y + q.y, z + q.z, w + q.w);
		return *this;
	}
	// Subtract Equal
	Quaternion &operator -= (const Quaternion &q) {
		SetValues(x - q.x, y - q.y, z - q.z, w - q.w);
		return *this;
	}
	// Negate
	friend Quaternion operator - (const Quaternion &a) {
		return Quaternion(-a.x, -a.y, -a.z, -a.w);
	}
	// Add
	friend Quaternion operator + (const Quaternion &a, const Quaternion 
		&b) {
			return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
	// Factor
	friend Quaternion operator * (const Quaternion &a, CoordType s) {
		return Quaternion(a.x * s, a.y * s, a.z * s, a.w * s);
	}
	friend Quaternion operator * (CoordType s, const Quaternion &a) {
		return Quaternion(a.x * s, a.y * s, a.z * s, a.w * s);
	}
	// Multiply
	friend Quaternion operator * (const Quaternion &a, const Quaternion 
		&b) {
			return Quaternion(
				a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
				a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
				); 
	}
	/*
	// Multiply a vector by a Quaternion
	friend Quaternion operator * (const Vector3 &v, const Quaternion &q) {
	Quaternion t(v, 0);
	return t*q;
	}
	*/

public:
	// Values
	Quaternion &SetValues(CoordType xI, CoordType yI, CoordType zI, CoordType wI) {
		x = xI; y = yI; z = zI; w = wI;
		return *this;
	}
	// Euler
	Quaternion &SetEuler(CoordType yaw, CoordType pitch, CoordType roll) {
		yaw = yaw * RADIANSDEGREE;
		pitch = pitch * RADIANSDEGREE;
		roll = roll * RADIANSDEGREE;

		CoordType cosY = cos(yaw / 2.0f);
		CoordType sinY = sin(yaw / 2.0f);
		CoordType cosP = cos(pitch / 2.0f);
		CoordType sinP = sin(pitch / 2.0f);
		CoordType cosR = cos(roll / 2.0f);
		CoordType sinR = sin(roll / 2.0f);
		SetValues(
			cosR * sinP * cosY + sinR * cosP * sinY,
			cosR * cosP * sinY - sinR * sinP * cosY,
			sinR * cosP * cosY - cosR * sinP * sinY,
			cosR * cosP * cosY + sinR * sinP * sinY
			);
		return *this;
	}
	// Normalize
	Quaternion Normalize() {
		return *this * (1.0F / GetLength());
	}
	CoordType GetLength () {
		return sqrt(w * w + x * x + y * y + z * z);
	}
	Matrix33 GetMatrix33() {
		return Matrix33(
			Vector3(1 - 2*(y*y) - 2*(z*z), 2*(x*y+w*z), 2*(x*z-w*y)),
			Vector3(2*(x*y-w*z), 1 - 2*(x*x) - 2*(z*z), 2*(y*z+w*x)),
			Vector3(2*(x*z+w*y), 2*(y*z-w*x), 1 - 2*(x*x) - 2*(y*y))
			);
	}

	CoordType* GetMatrixOpenGL(CoordType* matrix) {
		*this = Normalize();

		CoordType xx = x * x;
		CoordType yy = y * y;
		CoordType zz = z * z;
		
		// M(x,y) to access a matrix element (just makes reading a little easier)
		
		M( 0, 0 ) = 1.0f - 2.0f * ( yy + zz );
		M( 1, 0 ) = 2.0f * ( x * y + w * z );
		M( 2, 0 ) = 2.0f * ( x * z - w * y );
		M( 3, 0 ) = 0.0f;
		
		M( 0, 1 ) = 2.0f * ( x * y - w * z );
		M( 1, 1 ) = 1.0f - 2.0f * ( xx + zz );
		M( 2, 1 ) = 2.0f * ( y * z + w * x );
		M( 3, 1 ) = 0.0f;
		
		M( 0, 2 ) = 2.0f * ( x * z + w * y );
		M( 1, 2 ) = 2.0f * ( y * z - w * x );
		M( 2, 2 ) = 1.0f - 2.0f * ( xx + yy );
		M( 3, 2 ) = 0.0f;
		
		M( 0, 3 ) = 0.0f;
		M( 1, 3 ) = 0.0f;
		M( 2, 3 ) = 0.0f;
		M( 3, 3 ) = 1.0f;
		
		return matrix;
	}
};

Quaternion GetRotationTo(Vector3& from, Vector3& dest);

// Dot Product
inline CoordType DotProduct(const Quaternion &a, const Quaternion &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Smooth Linear Interpolate
inline Quaternion Slerp(const Quaternion &from, const Quaternion &to, 
						CoordType interp) {
	Quaternion temp;
	CoordType omega, cosO, sinO;
	CoordType scale0, scale1;

	cosO = DotProduct(from, to);

	if (cosO < 0.0) {
		cosO = -cosO;
		temp = -to;
	} else  {
		temp = -to;
	}

	if ((1.0 - cosO) > ERROR_TOLERANCE) {
		omega = (CoordType)acos(cosO);
		sinO = sin(omega);
		scale0 = sin((1.0F - interp) * omega) / sinO;
		scale1 = sin(interp * omega) / sinO;
	} else {
		scale0 = 1.0F - interp;
		scale1 = interp;
	}

	return scale0 * from + scale1 * temp;
}

#undef M

#endif

