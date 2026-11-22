/*
===============================================================================

	TGGeomUtils.cpp

	The line-sphere test behind TGGeomUtils::LineSphereIntersection. The
	file name is invented: retail keeps the function as a unit of its own
	at 0x4570D0, after TGFuzzyTriShape's code, followed by its locale-id
	pair and <iostream> quads (0x457490-0x45753F); nothing names the unit.

===============================================================================
*/

#include <iostream>
#include <math.h>
#include "TGPoint3.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00457490 _$E54
// FUNCTION: STBC 0x004574b0 _$E53

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004574c0 _$E32
// FUNCTION: STBC 0x004574d0 _$E29
// FUNCTION: STBC 0x004574e0 _$E31
// FUNCTION: STBC 0x004574f0 _$E30
// FUNCTION: STBC 0x00457500 _$E38
// FUNCTION: STBC 0x00457510 _$E35
// FUNCTION: STBC 0x00457520 _$E37
// FUNCTION: STBC 0x00457530 _$E36

/*
================
Unknown4570D0

Where the segment from start to start + direction meets the sphere: the
quadratic in the segment's parameter, whose roots in [0, 1] are the
meeting points, the nearer first.
================
*/
// FUNCTION: STBC 0x004570d0
// DRAFT: x87: retail spills b*b and 4ac to the dead argument slots and loads the centre vector into the FPU for its square, where this build keeps them on the stack; its out-of-line NiPoint3::operator= in the second two-hit case is expanded here
int Unknown4570D0( const NiPoint3 &start, const NiPoint3 &direction, const NiPoint3 &center, float radius, NiPoint3 *hit1, NiPoint3 *hit2 ) {
	float a = direction * direction;
	if ( a == 0.0f ) {
		return 0;
	}
	NiPoint3 toCenter = center - start;
	float b = ( toCenter * -direction ) * -2.0f;
	float bSquared = b * b;
	float fourAC = ( toCenter * toCenter - radius * radius ) * a * -4.0f;
	if ( -fourAC > bSquared ) {
		return 0;
	}

	float root = (float)sqrt( bSquared + fourAC );
	float t1 = ( root + b ) / ( 2.0f * a );
	float t2 = ( b - root ) / ( 2.0f * a );
	if ( t1 <= 1.0f && t1 >= 0.0f ) {
		if ( t1 > t2 ) {
			if ( t2 >= 0.0f ) {
				*hit1 = start + direction * t2;
				*hit2 = start + direction * t1;
				return 2;
			}
		} else if ( t2 <= 1.0f ) {
			*hit1 = start + direction * t1;
			*hit2 = start + direction * t2;
			return 2;
		}
		*hit1 = start + direction * t1;
		return 1;
	}
	if ( t2 <= 1.0f && t2 >= 0.0f ) {
		*hit1 = start + direction * t2;
		return 1;
	}
	return 0;
}
