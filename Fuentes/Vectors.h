/* Copyright (C) Dante Treglia II and Mark A. DeLoura, 2000. 
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright (C) Dante Treglia II and Mark A. DeLoura, 2000"
*/
//==========================================================
// C++ Matrix Library
// Version: 2.6
// Date: May 19, 2000
// Authors: Dante Treglia II and Mark A. DeLoura
// Thanks to: Miguel Gomez, Stan Melax, Pete Isensee, 
//   Gabor Nagy, Scott Bilas, James Boer, Eric Lengyel
//==========================================================

#ifndef _MTXLIB_H
#define _MTXLIB_H
#include "Types.h"
#include <cstdio>
#include <cmath>
#include <cassert>
#include <xmmintrin.h>


static inline CoordType DegToRad(CoordType a) { return a*0.01745329252f;};
static inline CoordType RadToDeg(CoordType a) { return a*57.29577951f;};

class Vector2;
class Vector3;
class Vector4;
class Matrix33;
class Matrix44;

////////////////////////////////////////////////////////////
// Vector2 class
//

class Vector2 
{
public:
	// Members
	CoordType x, y;

public:
	// Constructors
	Vector2() {};
	// Constructor with initializing CoordType values
	Vector2(CoordType inX, CoordType inY): x(inX), y(inY) {}
	// Constructor with initializing Vector2
	Vector2(const Vector2 &v): x(v.x), y(v.y) {}

public:
	// Array indexing
	CoordType                 &operator [] (unsigned int i) {
		assert (i<2);
		return *(&x+i);
	}
	// Array indexing
	const CoordType           &operator [] (unsigned int i) const {
		assert (i<2);
		return *(&x+i);
	}
	// Add a Vector2 to this one
	Vector2               &operator += (const Vector2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	// Subtract a Vector2 from this one
	Vector2               &operator -= (const Vector2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	// Multiply the Vector2 by a CoordType
	Vector2               &operator *= (CoordType f) {
		x *= f;
		y *= f;
		return *this;
	}
	// Divide the Vector2 by a CoordType
	Vector2               &operator /= (CoordType f) {
		x /= f;
		y /= f;
		return *this;
	}
	// Are these two Vector2's equal?
	friend Boolean           operator == (const Vector2 &a, const Vector2 &b) {
		return((a.x == b.x) && (a.y == b.y));
	}
	// Are these two Vector2's not equal?
	friend Boolean           operator != (const Vector2 &a, const Vector2 &b) {
		return((a.x != b.x) || (a.y != b.y));
	}
	// Negate this vector
	friend Vector2        operator - (const Vector2 &a) {
		return Vector2(-a.x, -a.y);
	}
	// Add two Vector2's
	friend Vector2        operator + (const Vector2 &a, const Vector2 &b) {
		Vector2 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one Vector2 from another
	friend Vector2        operator - (const Vector2 &a, const Vector2 &b) {
		Vector2 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply Vector2 by a CoordType
	friend Vector2        operator * (const Vector2 &v, CoordType f) {
		return Vector2(f * v.x, f * v.y);
	}
	// Multiply Vector2 by a CoordType
	friend Vector2        operator * (CoordType f, const Vector2 &v) {
		return Vector2(f * v.x, f * v.y);
	}
	// Divide Vector2 by a CoordType
	friend Vector2        operator / (const Vector2 &v, CoordType f) {
		return Vector2(v.x / f, v.y / f);
	}

public:
	// Methods
	// Set Values
	void                  set(CoordType xIn, CoordType yIn) {
		x = xIn;
		y = yIn;
	}
	// Get length of a Vector2
	CoordType                 length() const {
		return(CoordType) sqrt(x*x + y*y);
	}
	// Get squared length of a Vector2
	CoordType                 lengthSqr() const {
		return(x*x + y*y);
	}
	// Does Vector2 equal (0, 0)?
	Boolean                  isZero() const {
		return((x == 0.0F) && (y == 0.0F));
	}
	// Normalize a Vector2
	Vector2               &normalize() {
		CoordType m = length();

		//if (m > 0.0F)
			m = 1.0F / m;
		//else
		//	m = 0.0F;
		x *= m;
		y *= m;

		return *this;
	}
};


////////////////////////////////////////////////////////////
// Vector3 class
//

class Vector3 
{
public:
	// Members
	CoordType x, y, z, w;

public:
	// Constructors
	Vector3() : x(0), y(0), z(0), w(0) {};
	// Constructor with initializing CoordType values
	Vector3(CoordType inX, CoordType inY, CoordType inZ): x(inX), y(inY), z(inZ), w(0) {}
	// Constructor with initializing Vector3
//	Vector3(const Vector3 &v): x(v.x), y(v.y), z(v.z), w(v.w) {}
	// Constructor with initializing Vector2
	explicit Vector3(const Vector2 &v): x(v.x), y(v.y), z(0.0F), w(0) {}
	// Constructor with initializing Vector4
	//explicit Vector3(const Vector4 &v): x(v.x), y(v.y), z(v.z) {}

public:
	// Operators
	// Array indexing
	CoordType                 &operator [] (unsigned int i) {
		assert (i<3);
		return *(&x+i);
	}
	// Array indexing
	const CoordType           &operator [] (unsigned int i) const {
		assert (i<3);
		return *(&x+i);
	}
	// Assign from a Vector2
	Vector3               &operator =  (const Vector2 &v) {
		x = v.x;
		y = v.y;
		z = 0.0F;
		return *this;
	}
	// Add a Vector3 to this one
	Vector3               &operator += (const Vector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	// Subtract a Vector3 from this one
	Vector3               &operator -= (const Vector3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	// Multiply the Vector3 by a CoordType
	Vector3               &operator *= (CoordType f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	// Divide the Vector3 by a CoordType
	Vector3               &operator /= (CoordType f) {
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	inline Vector3 operator * ( const Vector3& rhs) const	{
		Vector3 kProd;

		kProd.x = rhs.x * x;
		kProd.y = rhs.y * y;
		kProd.z = rhs.z * z;

		return kProd;
	}

	// Are these two Vector3's equal?
	friend Boolean           operator == (const Vector3 &a, const Vector3 &b) {
		return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
	}
	// Are these two Vector3's not equal?
	friend Boolean           operator != (const Vector3 &a, const Vector3 &b) {
		return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
	}
	// Negate a Vector3
	friend Vector3        operator - (const Vector3 &a) {
		return Vector3(-a.x, -a.y, -a.z);
	}
	// Add two Vector3's
	friend Vector3        operator + (const Vector3 &a, const Vector3 &b) {
		Vector3 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one Vector3 from another
	friend Vector3        operator - (const Vector3 &a, const Vector3 &b) {
		Vector3 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply Vector3 by a CoordType
	friend Vector3        operator * (const Vector3 &v, CoordType f) {
		return Vector3(f * v.x, f * v.y, f * v.z);
	}
	// Multiply Vector3 by a CoordType
	friend Vector3        operator * (CoordType f, const Vector3 &v) {
		return Vector3(f * v.x, f * v.y, f * v.z);
	}
	// Divide Vector3 by a CoordType
	friend Vector3        operator / (const Vector3 &v, CoordType f) {
		return Vector3(v.x / f, v.y / f, v.z / f);
	}

public:
	// Methods
	// Set Values
	void                  set(CoordType xIn, CoordType yIn, CoordType zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	// Get length of a Vector3
	CoordType                 length() const {
		return(CoordType) sqrt(x*x + y*y + z*z + w*w);
	}
	// Get squared length of a Vector3
	CoordType                 lengthSqr() const {
		return(x*x + y*y + z*z + w*w);
	}
	// Does Vector3 equal (0, 0, 0)?
	Boolean                  isZero() const {
		return((x == 0.0F) && (y == 0.0F) && (z == 0.0F));
	}
	// Normalize a Vector3
	Vector3               &normalize() {

		CoordType m = length();
		//fdangelo: Si me aseguro de nunca llamar con longitudes iguales a cero, puedo evitarme este costoso IF !
//		if (m > 0.0F)
			m = 1.0F / m;
//		else
//			m = 0.0F;
		x *= m;
		y *= m;
		z *= m;

		return *this;
		
	}
};


////////////////////////////////////////////////////////////
// Vector4 class
//

class Vector4 
{
public:
	// Members
	CoordType x, y, z, w;

public:
	// Constructors
	// Vector4(): x(0), y(0), z(0), w(0) {};
	Vector4() {};
	// Constructor with initializing CoordType values
	Vector4(CoordType inX, CoordType inY, CoordType inZ, CoordType inW): x(inX), y(inY), z(inZ), w(inW) {};
	// Constructor with initializing Vector4
//	Vector4(const Vector4 &v): x(v.x), y(v.y), z(v.z), w(v.w) {};
	// Constructor with initializing Vector3
	explicit Vector4(const Vector3 &v): x(v.x), y(v.y), z(v.z), w(0.0F) {};
	// Constructor with initializing Vector2
	explicit Vector4(const Vector2 &v): x(v.x), y(v.y), z(0.0F), w(0.0F) {};

public:
	// Operators
	// Array indexing
	CoordType                 &operator [] (unsigned int i) {
		assert (i<4);
		//return *(&x+i);
		return(i == 0) ? x : (i == 1) ? y : (i == 2) ? z : w;
	}
	// Array indexing
	const CoordType           &operator [] (unsigned int i) const {
		assert (i<4);
		//return *(&x+i);
		return(i == 0) ? x : (i == 1) ? y : (i == 2) ? z : w;
	}
	// Assign from a Vector3
	Vector4               &operator =  (const Vector3 &v) { 
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.0F;
		return *this;
	}
	// Assign from a Vector2
	Vector4               &operator =  (const Vector2 &v) {
		x = v.x;
		y = v.y;
		z = 0.0F;
		w = 0.0F;
		return *this;
	}
	// Add a Vector4 to this one
	Vector4               &operator += (const Vector4 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	// Subtract a Vector4 from this one
	Vector4               &operator -= (const Vector4 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	// Multiply the Vector4 by a CoordType
	Vector4               &operator *= (CoordType f) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	// Divide the Vector4 by a CoordType
	Vector4               &operator /= (CoordType f) {
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
	// Are these two Vector4's equal?
	friend Boolean           operator == (const Vector4 &a, const Vector4 &b) {
		return((a.x == b.x) && (a.y == b.y) &&
			(a.z == b.z) && (a.w == b.w));
	}
	// Are these two Vector4's not equal?
	friend Boolean           operator != (const Vector4 &a, const Vector4 &b) {
		return((a.x != b.x) || (a.y != b.y) ||
			(a.z != b.z) || (a.w != b.w));
	}
	// Negate a Vector4
	friend Vector4        operator - (const Vector4 &a) {
		return Vector4(-a.x, -a.y, -a.z, -a.w);
	}
	// Add two Vector4's
	friend Vector4        operator + (const Vector4 &a, const Vector4 &b) {
		Vector4 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one Vector4 from another
	friend Vector4        operator - (const Vector4 &a, const Vector4 &b) {
		Vector4 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply Vector4 by a CoordType
	friend Vector4        operator * (const Vector4 &v, CoordType f) {
		return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
	}
	// Multiply Vector4 by a CoordType
	friend Vector4        operator * (CoordType f, const Vector4 &v) {
		return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
	}
	// Divide Vector4 by a CoordType
	friend Vector4        operator / (const Vector4 &v, CoordType f) {
		return Vector4(v.x / f, v.y / f, v.z / f, v.w / f);
	}

public:
	// Methods
	// Set Values
	void                  set(CoordType xIn, CoordType yIn, CoordType zIn, CoordType wIn) {
		x = xIn;
		y = yIn;
		z = zIn;
		w = wIn;
	}
	// Get length of a Vector4
	CoordType                 length() const {
		return(CoordType) sqrt(x*x + y*y + z*z + w*w);
	}
	// Get squared length of a Vector4
	CoordType                 lengthSqr() const {
		return(x*x + y*y + z*z + w*w);
	}
	// Does Vector4 equal (0, 0, 0, 0)?
	Boolean                  isZero() const {
		return((x == 0.0F) && (y == 0.0F) && (z == 0.0F) && (w == 0.0F));
	}
	// Normalize a Vector4
	Vector4               &normalize() {
		CoordType m = length();
		//if (m > 0.0F)
			m = 1.0F / m;
		//else
		//	m = 0.0F;
		x *= m;
		y *= m;
		z *= m;
		w *= m;
		return *this;
	}
};


////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

Vector2 Normalized(const Vector2 &a);
Vector3 Normalized(const Vector3 &a);
Vector4 Normalized(const Vector4 &a);
CoordType DotProduct(const Vector2 &a, const Vector2 &b);
CoordType DotProduct(const Vector3 &a, const Vector3 &b);
CoordType DotProduct(const Vector4 &a, const Vector4 &b);
void SwapVec(Vector2 &a, Vector2 &b);
void SwapVec(Vector3 &a, Vector3 &b);
void SwapVec(Vector4 &a, Vector4 &b);
Vector3 CrossProduct(const Vector3 &a, const Vector3 &b);
Boolean NearlyEquals(const Vector2 &a, const Vector2 &b, CoordType r);
Boolean NearlyEquals(const Vector3 &a, const Vector3 &b, CoordType r);
Boolean NearlyEquals(const Vector4 &a, const Vector4 &b, CoordType r);


////////////////////////////////////////////////////////////
// Matrix33 class
//

class Matrix33 
{
public:
	// Members
	Vector3       col[3];

public:
	// Constructors
	Matrix33() {};
	// Constructor with initializing value
	Matrix33(CoordType v) {
		col[0].set(v, v, v);
		col[1].set(v, v, v);
		col[2].set(v, v, v);
	}
	// Constructor with initializing Matrix33
	Matrix33(const Matrix33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
	}
	// Constructor with initializing Vector3's
	Matrix33(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
		col[0] = v0;
		col[1] = v1;
		col[2] = v2;
	}

public:
	// Operators
	// Array indexing
	Vector3       &operator [] (unsigned int i) {
		assert (i<3);
		return(Vector3&)col[i];
	}
	// Array indexing
	const Vector3     &operator [] (unsigned int i) const {
		assert (i<3);
		return(Vector3&)col[i];
	}
	// Assign
	Matrix33      &operator =  (const Matrix33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		return *this;
	}
	// Add a Matrix33 to this one
	Matrix33      &operator += (const Matrix33 &m) {
		col[0] += m[0];
		col[1] += m[1];
		col[2] += m[2];
		return *this;
	}
	// Subtract a Matrix33 from this one
	Matrix33      &operator -= (const Matrix33 &m) {
		col[0] -= m[0];
		col[1] -= m[1];
		col[2] -= m[2];
		return *this;
	}
	// Multiply the Matrix33 by another Matrix33
	Matrix33      &operator *= (const Matrix33 &m);
	// Multiply the Matrix33 by a CoordType
	Matrix33      &operator *= (CoordType f) {
		col[0] *= f;
		col[1] *= f;
		col[2] *= f;
		return *this;
	}
	// Are these two Matrix33's equal?
	friend Boolean       operator == (const Matrix33 &a, const Matrix33 &b) {
		return((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
	}
	// Are these two Matrix33's not equal?
	friend Boolean       operator != (const Matrix33 &a, const Matrix33 &b) {
		return((a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]));
	}
	// Add two Matrix33's
	friend Matrix33   operator + (const Matrix33 &a, const Matrix33 &b) {
		Matrix33 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one Matrix33 from another
	friend Matrix33   operator - (const Matrix33 &a, const Matrix33 &b) {
		Matrix33 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply Matrix33 by another Matrix33
	friend Matrix33   operator * (const Matrix33 &a, const Matrix33 &b) {
		Matrix33 ret(a);
		ret *= b;
		return ret;
	}
	// Multiply a Vector3 by this Matrix33
	friend Vector3    operator * (const Matrix33 &m, const Vector3 &v) {
		Vector3 ret;
		ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
		ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
		ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
		return ret;
	}
	// Multiply a Vector3 by this Matrix33
	friend Vector3    operator * (const Vector3 &v, const Matrix33 &m) {
		Vector3 ret;
		ret.x = DotProduct(m[0], v);
		ret.y = DotProduct(m[1], v);
		ret.z = DotProduct(m[2], v);
		return ret;
	}
	// Multiply Matrix33 by a CoordType
	friend Matrix33   operator * (const Matrix33 &m, CoordType f) {
		Matrix33 ret(m);
		ret *= f;
		return ret;
	}
	// Multiply Matrix33 by a CoordType
	friend Matrix33   operator * (CoordType f, const Matrix33 &m) {
		Matrix33 ret(m);
		ret *= f;
		return ret;
	}

public:
	// Methods
	// Set Matrix33 to the identity matrix
	Matrix33      &identity() {
		col[0].set(1.0, 0.0, 0.0);
		col[1].set(0.0, 1.0, 0.0);
		col[2].set(0.0, 0.0, 1.0);
		return *this;
	}
	// Transpose the Matrix33
	Matrix33      &transpose();
	// Invert the Matrix33
	Matrix33      &invert();
};

Matrix33    IdentityMatrix33();
Matrix33    TransposeMatrix33(const Matrix33 &m);
Matrix33    InvertMatrix33(const Matrix33 &m);
Matrix33    RotateRadMatrix33(CoordType rad);
Matrix33    TranslateMatrix33(CoordType x, CoordType y);
Matrix33    ScaleMatrix33(CoordType x, CoordType y, CoordType z = 1.0);


////////////////////////////////////////////////////////////
// Matrix44 class
//

class Matrix44 
{
public:
	// Members
	Vector4   col[4];

public:
	// Constructors
	Matrix44() {};
	// Constructor with initializing value
	Matrix44(CoordType v) {
		col[0].set(v, v, v, v);
		col[1].set(v, v, v, v);
		col[2].set(v, v, v, v);
		col[3].set(v, v, v, v);
	}
	// Constructor with initializing Matrix44
	Matrix44(const Matrix44 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		col[3] = m[3];
	}
	// Constructor with initializing Vector4's
	Matrix44(const Vector4 &v0, const Vector4 &v1, 
		const Vector4 &v2, const Vector4 &v3) {
			col[0] = v0;
			col[1] = v1;
			col[2] = v2;
			col[3] = v3;
	}
	// Constructor with initializing Matrix33
	explicit Matrix44(const Matrix33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		col[3].set(0.0, 0.0, 0.0, 1.0);
	}

public:
	// Operators
	// Array indexing
	Vector4       &operator [] (unsigned int i) {
		assert (i<4);
		return col[i];
	}
	// Array indexing
	const Vector4     &operator [] (unsigned int i) const {
		assert (i<4);
		return col[i];
	}
	// Assign
	Matrix44      &operator =  (const Matrix44 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		col[3] = m[3];
		return *this;
	}
	// Assign a Matrix33 to the Matrix44
	Matrix44      &operator =  (const Matrix33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		col[3].set(0.0, 0.0, 0.0, 1.0);
		return *this;
	}
	// Add a Matrix44 to this one
	Matrix44      &operator += (const Matrix44 &m) {
		col[0] += m[0];
		col[1] += m[1];
		col[2] += m[2];
		col[3] += m[3];
		return *this;
	}
	// Subtract a Matrix44 from this one
	Matrix44      &operator -= (const Matrix44 &m) {
		col[0] -= m[0];
		col[1] -= m[1];
		col[2] -= m[2];
		col[3] -= m[3];
		return *this;
	}
	// Multiply the Matrix44 by another Matrix44
	Matrix44      &operator *= (const Matrix44 &m);
	// Multiply the Matrix44 by a CoordType
	Matrix44      &operator *= (CoordType f) {
		col[0] *= f;
		col[1] *= f;
		col[2] *= f;
		col[3] *= f;
		return *this;
	}
	//Matrix44      &operator /= (CoordType f) {}
	// Are these two Matrix44's equal?
	friend Boolean       operator == (const Matrix44 &a, const Matrix44 &b) {
		return((a[0] == b[0]) && (a[1] == b[1]) &&
			(a[2] == b[2]) && (a[3] == b[3]));
	}
	// Are these two Matrix44's not equal?
	friend Boolean       operator != (const Matrix44 &a, const Matrix44 &b) {
		return((a[0] != b[0]) || (a[1] != b[1]) ||
			(a[2] != b[2]) || (a[3] != b[3]));
	}
	// Add two Matrix44's
	friend Matrix44   operator + (const Matrix44 &a, const Matrix44 &b) {
		Matrix44 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one Matrix44 from another
	friend Matrix44   operator - (const Matrix44 &a, const Matrix44 &b) {
		Matrix44 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply Matrix44 by another Matrix44
	friend Matrix44   operator * (const Matrix44 &a, const Matrix44 &b) {
		Matrix44 ret(a);
		ret *= b;
		return ret;
	}
	// Multiply a Vector3 by this Matrix44
	friend Vector3    operator * (const Matrix44 &m, const Vector3 &v) {
		Vector4 ret(v);
		ret = m * ret;
		return Vector3(ret.x, ret.y, ret.z);
	}
	// Multiply a Vector3 by this Matrix44
	friend Vector3    operator * (const Vector3 &v, const Matrix44 &m) {
		Vector4 ret(v);
		ret = ret * m;
		return Vector3(ret.x, ret.y, ret.z);
	}
	// Multiply a Vector4 by this Matrix44
	friend Vector4    operator * (const Matrix44 &m, const Vector4 &v) {
		Vector4 ret;

		ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
		ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
		ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
		ret.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
		return ret;
	}
	// Multiply a Vector4 by this Matrix44
	friend Vector4    operator * (const Vector4 &v, const Matrix44 &m) {
		Vector4 ret;
		ret.x = DotProduct(m[0], v);
		ret.y = DotProduct(m[1], v);
		ret.z = DotProduct(m[2], v);
		ret.w = DotProduct(m[3], v);
		return ret;
	}
	// Multiply Matrix44 by a CoordType
	friend Matrix44   operator * (const Matrix44 &m, CoordType f) {
		Matrix44 ret(m);
		ret *= f;
		return ret;
	}
	// Set Matrix44 to the identity matrix
	friend Matrix44   operator * (CoordType f, const Matrix44 &m) {
		Matrix44 ret(m);
		ret *= f;
		return ret;
	}

public:
	// Methods
	// Set Matrix44 to the identity matrix
	Matrix44      &identity() {
		col[0].set(1.0, 0.0, 0.0, 0.0);
		col[1].set(0.0, 1.0, 0.0, 0.0);
		col[2].set(0.0, 0.0, 1.0, 0.0);
		col[3].set(0.0, 0.0, 0.0, 1.0);
		return *this;
	}
	// Transpose the Matrix44
	Matrix44      &transpose();
	// Invert the Matrix44
	Matrix44      &invert();
};

Matrix44    IdentityMatrix44();
Matrix44    TransposeMatrix44(const Matrix44 &m);
Matrix44    InvertMatrix44(const Matrix44 &m);
Matrix44    RotateRadMatrix44(char axis, CoordType rad);
Matrix44    RotateRadMatrix44(const Vector3 &axis, CoordType rad);
Matrix44    TranslateMatrix44(CoordType x, CoordType y, CoordType z);
Matrix44    ScaleMatrix44(CoordType x, CoordType y, CoordType z, CoordType w = 1.0);
Matrix44    LookAtMatrix44(const Vector3 &camPos, const Vector3 &target, 
							const Vector3 &camUp);
Matrix44    FrustumMatrix44(CoordType l, CoordType r, CoordType b, CoordType t, 
							CoordType n, CoordType f);
Matrix44    PerspectiveMatrix44(CoordType fovY, CoordType aspect, 
								CoordType n, CoordType f);
Matrix44    OrthoMatrix44(CoordType l, CoordType r, CoordType b, CoordType t, 
						  CoordType n, CoordType f);
Matrix44    OrthoNormalMatrix44(const Vector3 &xdir, 
								const Vector3 &ydir, const Vector3 &zdir);

#endif
