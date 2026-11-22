// NiMath.cpp -- NetImmerse math constants and fast approximations

#include "NiRTLib.h"
#include <float.h>

#include "NiMath.h"

// NI_TWO_PI's dynamic initialiser.
// FUNCTION: STBC 0x00812d70 _$E32

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00812eb0 _$E39
// FUNCTION: STBC 0x00812ed0 _$E17
// FUNCTION: STBC 0x00812ef0 _$E15
// FUNCTION: STBC 0x00812f00 _$E23
// FUNCTION: STBC 0x00812f20 _$E21

// DATA: STBC 0x89A730 ?NI_INFINITY@@3MB
const float NI_INFINITY = FLT_MAX;

// DATA: STBC 0x9A28D4 ?NI_PI@@3MB
const float NI_PI = (float)( 4.0 * atan( 1.0 ) );
// DATA: STBC 0x9A28CC ?NI_HALF_PI@@3MB
const float NI_HALF_PI = NI_PI * 0.5f;
// DATA: STBC 0x9A28D0 ?NI_TWO_PI@@3MB
const float NI_TWO_PI = 2.0f * NI_PI;

/*
================
NiFastATan2

atan2 from a fifth-order polynomial in the smaller ratio (Abramowitz and
Stegun 4.4.49), folded into the right quadrant.
================
*/
// FUNCTION: STBC 0x812D80
float NiFastATan2( float y, float x ) {
	if ( x == 0.0f && y == 0.0f ) {
		return 0.0f;
	}

	float offset = 0.0f;
	float ratio;
	float absY = (float)fabs( y );
	float absX = (float)fabs( x );
	if ( absY > absX ) {
		ratio = x / y;
		if ( ratio > 0.0f ) {
			offset = NI_HALF_PI;
		} else if ( ratio < 0.0f ) {
			offset = -NI_HALF_PI;
		}
	} else {
		ratio = y / x;
	}

	float ratioSqr = ratio * ratio;
	float angle = ratio * ( ( ( ( 0.0208351f * ratioSqr - 0.0851330f ) * ratioSqr + 0.1801410f ) * ratioSqr - 0.3302995f ) * ratioSqr + 0.9998660f );
	if ( offset != 0.0f ) {
		angle = offset - angle;
	}

	if ( y < 0.0f && x < 0.0f ) {
		angle -= NI_PI;
	}
	if ( y > 0.0f && x < 0.0f ) {
		angle += NI_PI;
	}
	return angle;
}
