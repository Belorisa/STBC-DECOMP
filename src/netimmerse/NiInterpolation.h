/*
===============================================================================

	NiInterpolation.h

	The scalar curves the float keys interpolate along: a straight line
	between two values and the cubic between two values with their
	tangents.

	Evidence: three functions between NiTextKey and NiVisKey (0x7B6EB0,
	0x7B6ED0, 0x7B6F20), cdecl, returning floats. The linear float keys'
	interpolation (0x7A7440) passes t and the two keys' values to the first;
	the Bezier float keys (0x79C760) pass t, each key's value and a tangent
	to the second, the TCB float keys (0x79BA1D) and the morph keys
	(0x79C44B) to the third, whose body is the second's. The file and
	function names are invented.

===============================================================================
*/

#ifndef __NIINTERPOLATION_H__
#define __NIINTERPOLATION_H__

// value0 at t = 0 to value1 at t = 1
float						NiLinearInterp( float t, float value0, float value1 );

// the cubic from value0, leaving it along outTangent0, to value1, arriving
// along inTangent1
float						NiBezierInterp( float t, float value0, float outTangent0, float value1, float inTangent1 );
float						NiHermiteInterp( float t, float value0, float outTangent0, float value1, float inTangent1 );

#endif /* !__NIINTERPOLATION_H__ */
