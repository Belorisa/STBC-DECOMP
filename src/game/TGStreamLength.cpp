/*
===============================================================================

	TGStreamLength.cpp

	The 16-bit length codec of TGStream's compressed vectors. Its owner is
	unknown (the unit after TGStream's), so the unit's name is invented.

===============================================================================
*/

#include "TGStream.h"

/*
================
Unknown6D3A90

Packs a length: a sign bit, a decade (the length's size class, each ten times
the last, from 0.001) and the position inside the decade in 12 bits.
================
*/
// FUNCTION: STBC 0x006d3a90
unsigned short Unknown6D3A90( float length ) {
	bool negative = false;
	if ( length < 0.0f ) {
		length = -length;
		negative = true;
	}
	float high = 0.001f;
	float low = 0.0f;
	unsigned int decade;
	unsigned short position;
	for ( decade = 0; decade < 9; decade++ ) {
		if ( length < high ) {
			position = (unsigned short)( ( length - low ) / ( high - low ) * 4095.0f );
			break;
		}
		low = high;
		high = low * 10.0f;
	}
	// BUG: a length of 100000 or more leaves the loop at decade 9 with no position and no clamp.
	if ( decade == 8 ) {
		position = 0xFFF;
		decade = 7;
	}
	if ( negative ) {
		decade |= 8;
	}
	return (unsigned short)( ( decade << 12 ) + position );
}

/*
================
Unknown6D3B30

Unpacks a length of Unknown6D3A90.
================
*/
// FUNCTION: STBC 0x006d3b30
float Unknown6D3B30( unsigned short packed ) {
	unsigned char decade = packed >> 12;
	unsigned short position = packed & 0xFFF;
	bool negative = false;
	if ( decade & 8 ) {
		decade &= 7;
		negative = true;
	}
	float high = 0.001f;
	float low = 0.0f;
	while ( decade ) {
		low = high;
		high *= 10.0f;
		decade--;
	}
	float length = position * ( 1.0f / 4095.0f );
	length *= high - low;
	length += low;
	return negative ? -length : length;
}
