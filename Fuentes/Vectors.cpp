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
#include "Vectors.h"
#include <cmath>
#include <cassert>

////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

// Return Normal of Vector2's
Vector2 Normalized(const Vector2 &a)
{
  Vector2 ret(a);
  return ret.normalize();
}
// Return Normal of Vector3's
Vector3 Normalized(const Vector3 &a)
{
  Vector3 ret(a);
  return ret.normalize();
}
// Return Normal of Vector4's
Vector4 Normalized(const Vector4 &a)
{
  Vector4 ret(a);
  return ret.normalize();
}

// Dot product of two Vector2's
CoordType DotProduct(const Vector2 &a, const Vector2 &b) 
{
  return a.x*b.x + a.y*b.y;
}

// Dot product of two Vector3's
CoordType DotProduct(const Vector3 &a, const Vector3 &b) 
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Dot product of two Vector4's
CoordType DotProduct(const Vector4 &a, const Vector4 &b) 
{
  return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

// Swap two Vector2's
void SwapVec(Vector2 &a, Vector2 &b) 
{
  Vector2 tmp(a);

  a = b;
  b = tmp;
}

// Swap two Vector3's
void SwapVec(Vector3 &a, Vector3 &b) 
{
  Vector3 tmp(a);

  a = b;
  b = tmp;
}

// Swap two Vector4's
void SwapVec(Vector4 &a, Vector4 &b) 
{
  Vector4 tmp(a);

  a = b;
  b = tmp;
}

// Cross product of two Vector3's
Vector3 CrossProduct(const Vector3 &a, const Vector3 &b) 
{
  return Vector3(a.y*b.z - a.z*b.y,
      a.z*b.x - a.x*b.z,
      a.x*b.y - a.y*b.x);
}

// Are these two Vector2's nearly equal?
Boolean NearlyEquals( const Vector2& a, const Vector2& b, CoordType r ) 
{
  Vector2 diff = a - b;  // difference

  return(DotProduct(diff, diff) < r*r);  // radius
}

// Are these two Vector3's nearly equal?
Boolean NearlyEquals( const Vector3& a, const Vector3& b, CoordType r ) 
{
  Vector3 diff = a - b;  // difference

  return(DotProduct(diff, diff) < r*r);  // radius
}

// Are these two Vector4's nearly equal?
Boolean NearlyEquals( const Vector4& a, const Vector4& b, CoordType r ) 
{
  Vector4 diff = a - b;  // difference

  return(DotProduct(diff, diff) < r*r);  // radius
}


////////////////////////////////////////////////////////////
// Matrix33 class
//

// Multiply the Matrix33 by another Matrix33
Matrix33 &Matrix33::operator *= (const Matrix33 &m) 
{
  Matrix33 t;

  for (unsigned int r = 0; r < 3; r++)
  {
  for (unsigned int c = 0; c < 3; c++)
  {
    CoordType f = 0;

    f += col[0][r] * m[c][0];
    f += col[1][r] * m[c][1];
    f += col[2][r] * m[c][2];

    t[c][r] = f;
  }
  }

  *this = t;

  return *this;
}

// Transpose the Matrix33
Matrix33 &Matrix33::transpose() 
{
  CoordType t;

  for (unsigned int c = 0; c < 3; c++)
  {
  for (unsigned int r = c + 1; r < 3; r++)
  {
    t = col[c][r];
    col[c][r] = col[r][c];
    col[r][c] = t;
  } 
  }

  return *this;
}

// Invert the Matrix33
Matrix33 &Matrix33::invert() 
{
  Matrix33 a(*this);
  Matrix33 b(IdentityMatrix33());

  unsigned int c, r;
  unsigned int cc;
  unsigned int rowMax; // Points to max abs value row in this column
  unsigned int row;
  CoordType tmp;

  // Go through columns
  for (c=0; c<3; c++)
  {
  // Find the row with max value in this column
  rowMax = c;
  for (r=c+1; r<3; r++)
  {
    if (fabs(a[c][r]) > fabs(a[c][rowMax]))
    {
    rowMax = r;
    }
  }

  // If the max value here is 0, we can't invert.  Return identity.
  if (a[rowMax][c] == 0.0F)
    return(identity());

  // Swap row "rowMax" with row "c"
  for (cc=0; cc<3; cc++)
  {
    tmp = a[cc][c];
    a[cc][c] = a[cc][rowMax];
    a[cc][rowMax] = tmp;
    tmp = b[cc][c];
    b[cc][c] = b[cc][rowMax];
    b[cc][rowMax] = tmp;
  }

  // Now everything we do is on row "c".
  // Set the max cell to 1 by dividing the entire row by that value
  tmp = a[c][c];
  for (cc=0; cc<3; cc++)
  {
    a[cc][c] /= tmp;
    b[cc][c] /= tmp;
  }

  // Now do the other rows, so that this column only has a 1 and 0's
  for (row = 0; row < 3; row++)
  {
    if (row != c)
    {
    tmp = a[c][row];
    for (cc=0; cc<3; cc++)
    {
      a[cc][row] -= a[cc][c] * tmp;
      b[cc][row] -= b[cc][c] * tmp;
    }
    }
  }

  }

  *this = b;

  return *this;
}

// Return a Matrix33 set to the identity matrix
Matrix33 IdentityMatrix33() 
{
  Matrix33 ret;

  return ret.identity();
}

// Return the transpose of the Matrix33
Matrix33 TransposeMatrix33(const Matrix33 &m) 
{
  Matrix33 ret(m);

  return ret.transpose();
}

// Return the inverted Matrix33
Matrix33 InvertMatrix33(const Matrix33 &m) 
{
  Matrix33 ret(m);

  return ret.invert();
}

// Return a 2D rotation Matrix33
Matrix33 RotateRadMatrix33(CoordType rad) 
{
  Matrix33 ret;
  CoordType sinA, cosA;

  sinA = sin(rad);
  cosA = cos(rad);

  ret[0][0] = cosA; ret[1][0] = -sinA; ret[2][0] = 0.0F;
  ret[0][1] = sinA; ret[1][1] =  cosA; ret[2][1] = 0.0F;
  ret[0][2] = 0.0F; ret[1][2] =  0.0F; ret[2][2] = 1.0F;

  return ret;
}

// Return a 2D translation Matrix33
Matrix33 TranslateMatrix33(CoordType x, CoordType y) 
{
  Matrix33 ret;

  ret.identity();
  ret[2][0] = x;
  ret[2][1] = y;

  return ret;
}

// Return a 2D/3D scale Matrix33
Matrix33 ScaleMatrix33(CoordType x, CoordType y, CoordType z) 
{
  Matrix33 ret;

  ret.identity();
  ret[0][0] = x;
  ret[1][1] = y;
  ret[2][2] = z;

  return ret;
}


////////////////////////////////////////////////////////////
// Matrix44 class
//

// Multiply the Matrix44 by another Matrix44
Matrix44 &Matrix44::operator *= (const Matrix44 &m) 
{
  Matrix44 t;
  for (unsigned int r = 0; r < 4; r++)
  {
  for (unsigned int c = 0; c < 4; c++)
  {
    CoordType f = 0;

    f += (col[0][r] * m[c][0]);
    f += (col[1][r] * m[c][1]);
    f += (col[2][r] * m[c][2]);
    f += (col[3][r] * m[c][3]);

    t[c][r] = f;
  }
  }
  *this = t;
  return *this;
}


// Transpose the Matrix44
Matrix44 &Matrix44::transpose() 
{
  CoordType t;

  for (unsigned int c = 0; c < 4; c++)
  {
  for (unsigned int r = c + 1; r < 4; r++)
  {
    t = col[c][r];
    col[c][r] = col[r][c];
    col[r][c] = t;
  } 
  } 

  return *this;
}

// Invert the Matrix44
Matrix44 &Matrix44::invert() 
{
  Matrix44 a(*this);
  Matrix44 b(IdentityMatrix44());

  unsigned int r, c;
  unsigned int cc;
  unsigned int rowMax; // Points to max abs value row in this column
  unsigned int row;
  CoordType tmp;

  // Go through columns
  for (c=0; c<4; c++)
  {

  // Find the row with max value in this column
  rowMax = c;
  for (r=c+1; r<4; r++)
  {
    if (fabs(a[c][r]) > fabs(a[c][rowMax]))
    {
    rowMax = r;
    }
  }

  // If the max value here is 0, we can't invert.  Return identity.
  if (a[rowMax][c] == 0.0F)
    return(identity());

  // Swap row "rowMax" with row "c"
  for (cc=0; cc<4; cc++)
  {
    tmp = a[cc][c];
    a[cc][c] = a[cc][rowMax];
    a[cc][rowMax] = tmp;
    tmp = b[cc][c];
    b[cc][c] = b[cc][rowMax];
    b[cc][rowMax] = tmp;
  }

  // Now everything we do is on row "c".
  // Set the max cell to 1 by dividing the entire row by that value
  tmp = a[c][c];
  for (cc=0; cc<4; cc++)
  {
    a[cc][c] /= tmp;
    b[cc][c] /= tmp;
  }

  // Now do the other rows, so that this column only has a 1 and 0's
  for (row = 0; row < 4; row++)
  {
    if (row != c)
    {
    tmp = a[c][row];
    for (cc=0; cc<4; cc++)
    {
      a[cc][row] -= a[cc][c] * tmp;
      b[cc][row] -= b[cc][c] * tmp;
    }
    }
  }

  }

  *this = b;

  return *this;
}

// Return a Matrix44 set to the identity matrix
Matrix44 IdentityMatrix44() 
{
  Matrix44 ret;

  return ret.identity();
}

// Return the transpose of the Matrix44
Matrix44 TransposeMatrix44(const Matrix44 &m) 
{
  Matrix44 ret(m);

  return ret.transpose();
}

// Return the inverted Matrix44
Matrix44 InvertMatrix44(const Matrix44 &m) 
{
  Matrix44 ret(m);

  return ret.invert();
}

// Return a 3D axis-rotation Matrix44
// Pass in 'x', 'y', or 'z' for the axis.
Matrix44 RotateRadMatrix44(char axis, CoordType rad) 
{
  Matrix44 ret;
  CoordType sinA, cosA;

  sinA = sin(rad);
  cosA = cos(rad);

  switch (axis)
  {
  case 'x':
  case 'X':
    ret[0][0] =  1.0F; ret[1][0] =  0.0F; ret[2][0] =  0.0F;
    ret[0][1] =  0.0F; ret[1][1] =  cosA; ret[2][1] = -sinA;
    ret[0][2] =  0.0F; ret[1][2] =  sinA; ret[2][2] =  cosA;
    break;

  case 'y':
  case 'Y':
    ret[0][0] =  cosA; ret[1][0] =  0.0F; ret[2][0] =  sinA;
    ret[0][1] =  0.0F; ret[1][1] =  1.0F; ret[2][1] =  0.0F;
    ret[0][2] = -sinA; ret[1][2] =  0.0F; ret[2][2] =  cosA;
    break;

  case 'z':
  case 'Z':
    ret[0][0] =  cosA; ret[1][0] = -sinA; ret[2][0] =  0.0F;
    ret[0][1] =  sinA; ret[1][1] =  cosA; ret[2][1] =  0.0F;
    ret[0][2] =  0.0F; ret[1][2] =  0.0F; ret[2][2] =  1.0F;
    break;
  }

  ret[0][3] = 0.0F; ret[1][3] = 0.0F; ret[2][3] = 0.0F;
  ret[3][0] = 0.0F;
  ret[3][1] = 0.0F;
  ret[3][2] = 0.0F;
  ret[3][3] = 1.0F;

  return ret;
}

// Return a 3D axis-rotation Matrix44
// Pass in an arbitrary Vector3 axis.
Matrix44 RotateRadMatrix44(const Vector3 &axis, CoordType rad) 
{
  Matrix44 ret;
  CoordType sinA, cosA;
  CoordType invCosA;
  Vector3 nrm = axis;
  CoordType x, y, z;
  CoordType xSq, ySq, zSq;

  nrm.normalize();
  sinA = sin(rad);
  cosA = cos(rad);
  invCosA = 1.0F - cosA;

  x = nrm.x;
  y = nrm.y;
  z = nrm.z;

  xSq = x * x;
  ySq = y * y;
  zSq = z * z;

  ret[0][0] = (invCosA * xSq) + (cosA);
  ret[1][0] = (invCosA * x * y) - (sinA * z );
  ret[2][0] = (invCosA * x * z) + (sinA * y );
  ret[3][0] = 0.0F;

  ret[0][1] = (invCosA * x * y) + (sinA * z);
  ret[1][1] = (invCosA * ySq) + (cosA);
  ret[2][1] = (invCosA * y * z) - (sinA * x);
  ret[3][1] = 0.0F;

  ret[0][2] = (invCosA * x * z) - (sinA * y);
  ret[1][2] = (invCosA * y * z) + (sinA * x);
  ret[2][2] = (invCosA * zSq) + (cosA);
  ret[3][2] = 0.0F;

  ret[0][3] = 0.0F;
  ret[1][3] = 0.0F;
  ret[2][3] = 0.0F;
  ret[3][3] = 1.0F;

  return ret;
}

// Return a 3D translation Matrix44
Matrix44 TranslateMatrix44(CoordType x, CoordType y, CoordType z) 
{
  Matrix44 ret;

  ret.identity();
  ret[3][0] = x;
  ret[3][1] = y;
  ret[3][2] = z;

  return ret;
}

// Return a 3D/4D scale Matrix44
Matrix44 ScaleMatrix44(CoordType x, CoordType y, CoordType z, CoordType w) 
{
  Matrix44 ret;

  ret.identity();
  ret[0][0] = x;
  ret[1][1] = y;
  ret[2][2] = z;
  ret[3][3] = w;

  return ret;
}

// Return a "lookat" Matrix44 given the current camera position (Vector3),
//   camera-up Vector3, and camera-target Vector3.
Matrix44 LookAtMatrix44(const Vector3 &camPos, const Vector3 &target, 
    const Vector3 &camUp ) 
{
  Matrix44 ret;

  Vector3 F = target - camPos;
  F.normalize();

  Vector3 S = CrossProduct(F, Normalized(camUp));
  S.normalize();

  Vector3 U = CrossProduct(S, F);
  U.normalize();

  ret[0][0] = S.x;
  ret[1][0] = S.y;
  ret[2][0] = S.z;
  ret[3][0] = 0.0;

  ret[0][1] = U.x;
  ret[1][1] = U.y;
  ret[2][1] = U.z;
  ret[3][1] = 0.0;

  ret[0][2] = -F.x;
  ret[1][2] = -F.y;
  ret[2][2] = -F.z;
  ret[3][2] = 0.0;

  ret[0][3] = 0.0F;
  ret[1][3] = 0.0F;
  ret[2][3] = 0.0F;
  ret[3][3] = 1.0F;

  ret *= TranslateMatrix44(-camPos.x, -camPos.y, -camPos.z);

  return ret;
}

// Return a frustum Matrix44 given the left, right, bottom, top,
//   near, and far values for the frustum boundaries.
Matrix44 FrustumMatrix44(CoordType l, CoordType r, 
    CoordType b, CoordType t, CoordType n, CoordType f) 
{
  Matrix44 ret;
  CoordType width = r-l;
  CoordType height = t-b;
  CoordType depth = f-n;

  ret[0][0] = (2*n) / width;
  ret[0][1] = 0.0F;
  ret[0][2] = 0.0F;
  ret[0][3] = 0.0F;

  ret[1][0] = 0.0F;
  ret[1][1] = (2*n) / height;
  ret[1][2] = 0.0F;
  ret[1][3] = 0.0F;

  ret[2][0] = (r + l) / width;
  ret[2][1] = (t + b) / height;
  ret[2][2] = -(f + n) / depth;
  ret[2][3] = -1.0F;

  ret[3][0] = 0.0F;
  ret[3][1] = 0.0F;
  ret[3][2] = -(2*f*n) / depth;
  ret[3][3] = 0.0F;

  return ret;
}

// Return a perspective Matrix44 given the field-of-view in the Y
//   direction in degrees, the aspect ratio of Y/X, and near and
//   far plane distances.
Matrix44 PerspectiveMatrix44(CoordType fovY, CoordType aspect, CoordType n, CoordType f) 
{
  Matrix44 ret;
  CoordType angle;
  CoordType cot;

  angle = fovY / 2.0F;
  angle = DegToRad( angle );

  cot = cos(angle) / sin(angle);

  ret[0][0] = cot / aspect;
  ret[0][1] = 0.0F;
  ret[0][2] = 0.0F;
  ret[0][3] = 0.0F;

  ret[1][0] = 0.0F;
  ret[1][1] = cot;
  ret[1][2] = 0.0F;
  ret[1][3] = 0.0F;

  ret[2][0] = 0.0F;
  ret[2][1] = 0.0F;
  ret[2][2] = -(f + n) / (f - n);
  ret[2][3] = -1.0F;


  ret[3][0] = 0.0F;
  ret[3][1] = 0.0F;
  ret[3][2] = -(2*f*n) / (f - n);
  ret[3][3] = 0.0F;

  return ret;
}

// Return an orthographic Matrix44 given the left, right, bottom, top,
//   near, and far values for the frustum boundaries.
Matrix44 OrthoMatrix44(CoordType l, CoordType r, 
    CoordType b, CoordType t, CoordType n, CoordType f) 
{
  Matrix44 ret;
  CoordType width = r-l;
  CoordType height = t-b;
  CoordType depth = f-n;

  ret[0][0] = 2.0F / width;
  ret[0][1] = 0.0F;
  ret[0][2] = 0.0F;
  ret[0][3] = 0.0F;

  ret[1][0] = 0.0F;
  ret[1][1] = 2.0F / height;
  ret[1][2] = 0.0F;
  ret[1][3] = 0.0F;

  ret[2][0] = 0.0F;
  ret[2][1] = 0.0F;
  ret[2][2] = -(2.0F) / depth;
  ret[2][3] = 0.0F;

  ret[3][0] = -(r + l) / width;
  ret[1][3] = -(t + b) / height;
  ret[3][2] = -(f + n) / depth;
  ret[3][3] = 1.0F;

  return ret;
}

// Return an orientation matrix using 3 basis normalized vectors
Matrix44    OrthoNormalMatrix44(const Vector3 &xdir, 
    const Vector3 &ydir, const Vector3 &zdir)
{
  Matrix44 ret;

  ret[0] = (Vector4)xdir;
  ret[1] = (Vector4)ydir;
  ret[2] = (Vector4)zdir;
  ret[3].set(0.0, 0.0, 0.0, 1.0);

  return ret;
}
