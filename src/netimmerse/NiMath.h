/*
===============================================================================

	NiMath.h

	NetImmerse's math constants and fast math helpers.

	Evidence: NiTimeController reads its "infinite" key times from the float
	at 0x89A730 (FLT_MAX) at run time, so it is a constant object defined
	in another unit rather than a literal. The lineage name is Gamebryo's.

===============================================================================
*/

#ifndef __NIMATH_H__
#define __NIMATH_H__

#include <math.h>
#include <stdlib.h>

extern const float NI_INFINITY;
extern const float NI_PI;
extern const float NI_HALF_PI;
extern const float NI_TWO_PI;

float			NiAbs( float value );
float			NiACos( float value );
float			NiASin( float value );
float			NiATan( float value );
float			NiATan2( float y, float x );
float			NiCos( float radians );
float			NiExp( float value );
float			NiFmod( float x, float y );
float			NiLog( float value );
float			NiPow( float base, float exponent );
float			NiSign( float value );
float			NiSin( float radians );
void			NiSinCos( float radians, float &sinValue, float &cosValue );
float			NiSqr( float value );
float			NiSqrt( float value );

float			NiMin( float a, float b );
float			NiMax( float a, float b );
int				NiMin( int a, int b );
int				NiMax( int a, int b );

bool			NiIsPowerOf2( unsigned int value );

void			NiSrand( unsigned int seed );
int				NiRand();
float			NiSymmetricRandom();

unsigned int	NiFastLog( unsigned int value );
float			NiFastATan2( float y, float x );
float			NiCapelliATan2( float y, float x );

/*
================
NiAbs
================
*/
inline float NiAbs( float value ) {
	return float( fabs( value ) );
}

/*
================
NiASin

asin clamped to its domain: the ends answer plus or minus a half turn.
Gamebryo 1.1.2's NiMath.inl text.
================
*/
inline float NiASin( float value ) {
	if ( -1.0f < value ) {
		if ( value < 1.0f ) {
			return float( asin( value ) );
		} else {
			return NI_HALF_PI;
		}
	} else {
		return -NI_HALF_PI;
	}
}

/*
================
NiACos

acos clamped to its domain: past the ends it answers zero or a half turn.
Gamebryo 1.1.2's NiMath.inl text has an else on both tests; here the
inner one only. The else is an inline-budget cost: with none,
NiQuaternion::Squad (0x815E60) expands its outer Slerp's operator+ where
retail calls it; with both, PlaceByDirectionMode::Update (0x427BD0) calls
its eased movement's operator* where retail expands it.
================
*/
// FUNCTION: STBC 0x00428120 ?NiACos@@YAMM@Z @game/CameraMode
inline float NiACos( float value ) {
	if ( -1.0f < value ) {
		if ( value < 1.0f ) {
			return float( acos( value ) );
		} else {
			return 0.0f;
		}
	}
	return NI_PI;
}

/*
================
NiATan
================
*/
inline float NiATan( float value ) {
	return float( atan( value ) );
}

/*
================
NiATan2
================
*/
inline float NiATan2( float y, float x ) {
	return float( atan2( y, x ) );
}

/*
================
NiCos
================
*/
inline float NiCos( float radians ) {
	return float( cos( radians ) );
}

/*
================
NiExp
================
*/
inline float NiExp( float value ) {
	return float( exp( value ) );
}

/*
================
NiFmod
================
*/
inline float NiFmod( float x, float y ) {
	return float( fmod( x, y ) );
}

/*
================
NiLog
================
*/
inline float NiLog( float value ) {
	return float( log( value ) );
}

/*
================
NiPow
================
*/
inline float NiPow( float base, float exponent ) {
	return float( pow( base, exponent ) );
}

/*
================
NiSign

One, minus one or zero, by the sign of the value.
================
*/
inline float NiSign( float value ) {
	return ( value > 0.0f ? +1.0f : ( value < 0.0f ? -1.0f : 0.0f ) );
}

/*
================
NiSin
================
*/
inline float NiSin( float radians ) {
	return float( sin( radians ) );
}

/*
================
NiSinCos
================
*/
inline void NiSinCos( float radians, float &sinValue, float &cosValue ) {
	sinValue = NiSin( radians );
	cosValue = NiCos( radians );
}

/*
================
NiSqr
================
*/
inline float NiSqr( float value ) {
	return value * value;
}

/*
================
NiSqrt

The square root in double precision, rounded back to float. Gamebryo 1.1.2
keeps it in NiMath.inl, and NiPoint3::Length goes through it: its float
parameter rounds the sum before the fsqrt (_wrap_NiPoint3_Perpendicular,
0x5BE680; SetClass::GetDistance, 0x410570).
================
*/
inline float NiSqrt( float value ) {
	return float( sqrt( value ) );
}

/*
================
NiMin / NiMax
================
*/
inline float NiMin( float a, float b ) {
	return ( a < b ) ? a : b;
}

inline float NiMax( float a, float b ) {
	return ( a > b ) ? a : b;
}

inline int NiMin( int a, int b ) {
	return ( a < b ) ? a : b;
}

inline int NiMax( int a, int b ) {
	return ( a > b ) ? a : b;
}

/*
================
NiIsPowerOf2
================
*/
inline bool NiIsPowerOf2( unsigned int value ) {
	if ( !value ) {
		return false;
	}
	return ( value & ( value - 1 ) ) == 0;
}

/*
================
NiSrand / NiRand
================
*/
inline void NiSrand( unsigned int seed ) {
	srand( seed );
}

inline int NiRand() {
	return rand();
}

/*
================
NiSymmetricRandom

A random number in [-1,1]. Gamebryo 1.1.2 keeps it in NiMath.inl; retail
multiplies by 2 / RAND_MAX (0x888DB4) and subtracts 1, and calls in an
argument list evaluate in the order an inline gives (SpinController's
random axis, 0x4F2D78).
================
*/
inline float NiSymmetricRandom() {
	return rand() * ( 2.0f / RAND_MAX ) - 1.0f;
}

/*
================
NiFastLog

The index of the highest set bit.
================
*/
inline unsigned int NiFastLog( unsigned int value ) {
	unsigned int log = 0;
	unsigned int temp = value >> 1;
	while ( temp & 0xffffffff ) {
		temp >>= 1;
		log++;
	}
	return log;
}

#endif /* !__NIMATH_H__ */
