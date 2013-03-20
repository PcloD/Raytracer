#ifndef __FAST_POW__
#define __FAST_POW__

/*
 * (c) Ian Stephenson
 *
 * ian@dctsystems.co.uk
 *
 * Fast pow() reference implementation
 */

#include <math.h>

float log2(float i)
	{
	return(log(i)/log(2.0f));
	}
float shift23=(1<<23);
float OOshift23=1.0/(1<<23);

float myLog2(float i)
	{
	float LogBodge=0.346607f;
	float x;
	float y;
	x=(float) *(int *)&i;
	x*= OOshift23; //1/pow(2,23);
	x=x-127;

	y=x-floorf(x);
	y=(y-y*y)*LogBodge;
	return x+y;
	}
float myPow2(float i)
	{
	float PowBodge=0.33971f;
	float x;
	float y=i-floorf(i);
	y=(y-y*y)*PowBodge;

	x=i+127-y;
	x*= shift23; //pow(2,23);
	*(int*)&x=(int)x;
	return x;
	}

float myPow(float a, float b)
	{
	return myPow2(b*myLog2(a));
	}

#endif
