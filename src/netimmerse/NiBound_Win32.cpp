// NiBound_Win32.cpp -- the platform part of NiBound: copying, comparing, merging and moving spheres
//
// Gamebryo 1.2.2: Win32/NiBound_Win32.cpp holds Merge and Update apart from
// NiBound.cpp; retail keeps them, with the assignment and the comparison, in a
// unit of its own after NiMath's (0x812F30), with its locale pair, <iostream>
// quads and registrations.

#include "NiRTLib.h"
#include <math.h>

#include "NiBound.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008132e0 _$E39
// FUNCTION: STBC 0x008132f0 _$E50
// FUNCTION: STBC 0x00813300 _$E54
// FUNCTION: STBC 0x00813310 _$E58
// FUNCTION: STBC 0x00813320 _$E62
// FUNCTION: STBC 0x00813330 _$E177
// FUNCTION: STBC 0x00813340 _$E182
// FUNCTION: STBC 0x00813350 _$E186

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00813260 _$E193
// FUNCTION: STBC 0x00813280 _$E17
// FUNCTION: STBC 0x008132a0 _$E15
// FUNCTION: STBC 0x008132b0 _$E23
// FUNCTION: STBC 0x008132d0 _$E21


/*
================
NiBound::operator=

Copies the sphere; the copy counts as computed. NiD3DRender's slots 43
and 45 to 48 call it.
================
*/
// FUNCTION: STBC 0x812F30 ??4NiBound@@QAEAAV0@ABV0@@Z
NiBound &NiBound::operator=( const NiBound &bound ) {
	center = bound.center;
	radius = bound.radius;
	unknown10 = false;
	return *this;
}

/*
================
NiBound::operator==
================
*/
// FUNCTION: STBC 0x812F60 ??8NiBound@@QAE_NABV0@@Z
bool NiBound::operator==( const NiBound &bound ) {
	return radius == bound.radius && center == bound.center;
}

/*
================
NiBound::operator<=

Whether this sphere lies within the other one. Gamebryo 1.1.2:
NiBound::operator<=.
================
*/
// FUNCTION: STBC 0x812FB0 ??NNiBound@@QAE_NABV0@@Z
bool NiBound::operator<=( const NiBound &bound ) {
	NiPoint3 diff = bound.center - center;
	return diff.Length() + radius <= bound.radius + fuzzFactor;
}

/*
================
NiBound::operator>=

Whether this sphere holds the other one. Gamebryo 1.1.2:
NiBound::operator>=.
================
*/
// FUNCTION: STBC 0x813010 ??PNiBound@@QAE_NABV0@@Z
bool NiBound::operator>=( const NiBound &bound ) {
	NiPoint3 diff = bound.center - center;
	return diff.Length() + bound.radius <= radius + fuzzFactor;
}

/*
================
NiBound::Merge

The smallest sphere holding both: either sphere when it holds the other,
otherwise the sphere across both along the line of their centers.
================
*/
// FUNCTION: STBC 0x813070
// Gamebryo 1.2.2: NiBound::Merge (NiBound_Win32.cpp)
void NiBound::Merge( const NiBound *bound ) {
	NiPoint3 diff = center - bound->center;
	float lengthSqr = diff.SqrLength();
	float radiusDiff = bound->radius - radius;
	float radiusDiffSqr = radiusDiff * radiusDiff;
	float length, coeff;

	if ( radiusDiff >= 0.0f ) {
		if ( radiusDiffSqr >= lengthSqr ) {
			// bound holds this sphere
			center = bound->center;
			radius = bound->radius;
		} else {
			length = sqrtf( lengthSqr );
			if ( length > tolerance ) {
				coeff = ( length - radiusDiff ) / ( 2.0f * length );
				center = bound->center + coeff * diff;
			}
			radius = 0.5f * ( bound->radius + length + radius );
		}
	} else if ( radiusDiffSqr < lengthSqr ) {
		length = sqrtf( lengthSqr );
		if ( length > tolerance ) {
			coeff = ( length - radiusDiff ) / ( 2.0f * length );
			center = bound->center + coeff * diff;
		}
		radius = 0.5f * ( bound->radius + length + radius );
	}
	// else this sphere holds bound
}

/*
================
NiBound::Update

Becomes bound carried by a scale, a rotation and a translation, the
world bound of a model bound.
================
*/
// FUNCTION: STBC 0x8131E0
void NiBound::Update( const NiBound &bound, const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	center = scale * ( rotate * bound.center ) + translate;
	radius = scale * bound.radius;
	unknown10 = false;
}
