// NiPoint3Unitize.cpp -- normalising arrays of vectors with the fast square root
//
// UnitizeVectors is the only function of its object file in retail; the file
// name is invented.

#include "NiRTLib.h"
#include "NiPoint3.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008218e0 _$E35
// FUNCTION: STBC 0x00821900 _$E17
// FUNCTION: STBC 0x00821920 _$E15
// FUNCTION: STBC 0x00821930 _$E23
// FUNCTION: STBC 0x00821950 _$E21

/*
================
NiPoint3::UnitizeVectors

Scales each vector to unit length, taking the square root from the table:
half the exponent, and the root of the top seven mantissa bits (plus the
low exponent bit). A zero vector stays zero.

Gamebryo 1.2.2: NiPoint3::UnitizeVectors works on the float's bits in
place through one pointer, the Graphics Gems square root (Lalonde and
Dawson).
================
*/
// FUNCTION: STBC 0x821830
void NiPoint3::UnitizeVectors( NiPoint3 *vectors, unsigned int count ) {
	float length;
	short exponent;

	// the length's bits, worked on in place
	unsigned int *rep = (unsigned int *)&length;

	for ( unsigned int i = 0; i < count; i++ ) {
		length = vectors->x * vectors->x + vectors->y * vectors->y + vectors->z * vectors->z;

		if ( !( *rep ) ) {
			length = 0.0f;
		} else {
			exponent = ( *rep >> 23 ) - 127;
			*rep &= 0x7FFFFF;
			if ( exponent & 1 ) {
				*rep |= 0x800000;
			}
			exponent >>= 1;
			*rep = sqrtTable[*rep >> 16] | ( ( exponent + 127 ) << 23 );
			length = 1.0f / length;
		}

		vectors->x *= length;
		vectors->y *= length;
		vectors->z *= length;
		vectors++;
	}
}
