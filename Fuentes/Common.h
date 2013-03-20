// -----------------------------------------------------------
// common.h
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#ifndef I_COMMON_H
#define I_COMMON_H

#include <math.h>
#include <stdlib.h>
#include "Types.h"

enum ResultadoRayo
{
	ResultadoRayoImpacto = 1, 
	ResultadoRayoFallo = 0, 
	ResultadoRayoImpactoInterior = -1 
};

inline float Rand( float a_Range ) { return ((float)rand() / RAND_MAX) * a_Range; }

#define DOT(A,B)		(DotProduct(A, B))
#define NORMALIZE(A)	{A.normalize();/*  CoordType l=(CoordType) (1/A.length());A.x*=l;A.y*=l;A.z*=l;*/}
#define LENGTH(A)		(A.length())
#define SQRLENGTH(A)	(A.lengthSqr())
#define SQRDISTANCE(A,B) ((A-B).lengthSqr())

#define EPSILON			((CoordType) 0.0001f)

#define PI				3.141592653589793238462f

class Plane
{
public:
	Plane() : N( (CoordType) 0, (CoordType) 0, (CoordType) 0 ), D( (CoordType) 0 ) {};
	Plane( Vector3 a_Normal, CoordType a_D ) : N( a_Normal ), D( a_D ) {};

	Vector3 N;
	CoordType D;
};

typedef Vector3 Color;

#endif