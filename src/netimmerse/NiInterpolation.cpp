// NiInterpolation.cpp -- the scalar curves of the float keys

#include "NiInterpolation.h"

/*
================
NiLinearInterp
================
*/
// FUNCTION: STBC 0x7B6EB0
float NiLinearInterp( float t, float value0, float value1 ) {
	return ( 1.0f - t ) * value0 + t * value1;
}

/*
================
NiBezierInterp

The cubic in Horner form, in powers of t.
================
*/
// FUNCTION: STBC 0x7B6ED0
float NiBezierInterp( float t, float value0, float outTangent0, float value1, float inTangent1 ) {
	float delta = value1 - value0;
	float a = ( outTangent0 + inTangent1 ) - 2.0f * delta;
	float b = 3.0f * delta - ( 2.0f * outTangent0 + inTangent1 );
	return ( ( a * t + b ) * t + outTangent0 ) * t + value0;
}

/*
================
NiHermiteInterp

The same cubic as NiBezierInterp.
================
*/
// FUNCTION: STBC 0x7B6F20
float NiHermiteInterp( float t, float value0, float outTangent0, float value1, float inTangent1 ) {
	float delta = value1 - value0;
	float a = ( outTangent0 + inTangent1 ) - 2.0f * delta;
	float b = 3.0f * delta - ( 2.0f * outTangent0 + inTangent1 );
	return ( ( a * t + b ) * t + outTangent0 ) * t + value0;
}
