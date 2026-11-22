// NiBezierRectangle3Slot43.cpp -- NiBezierRectangle3::VirtualSlot43, in a unit of its own
//
// The file name is invented: retail keeps this function in a unit of its own,
// after the Bezier patch units, holding only it, its <iostream> run and the
// headers' class registrations; nothing names the unit.

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierRectangle2.h"
#include "NiBezierRectangle3.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00855140 _$E219
// FUNCTION: STBC 0x008553f0 _$E48
// FUNCTION: STBC 0x00855400 _$E72
// FUNCTION: STBC 0x00855410 _$E76
// FUNCTION: STBC 0x00855420 _$E80
// FUNCTION: STBC 0x00855430 _$E84
// FUNCTION: STBC 0x00855440 _$E197
// FUNCTION: STBC 0x00855450 _$E56
// FUNCTION: STBC 0x00855460 _$E60
// FUNCTION: STBC 0x00855470 _$E64
// FUNCTION: STBC 0x00855480 _$E68
// FUNCTION: STBC 0x00855490 _$E205
// FUNCTION: STBC 0x008554a0 _$E211
// FUNCTION: STBC 0x008554b0 _$E215

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00855370 _$E226
// FUNCTION: STBC 0x00855390 _$E17
// FUNCTION: STBC 0x008553b0 _$E15
// FUNCTION: STBC 0x008553c0 _$E23
// FUNCTION: STBC 0x008553e0 _$E21

/*
================
NiBezierRectangle3::VirtualSlot43

Sets the weights of the sixteen control points at (u, v) as the products of
the cubic Bernstein basis in each parameter, and their derivatives with u
and v when the patch has normals.
================
*/
// FUNCTION: STBC 0x855150
void NiBezierRectangle3::VirtualSlot43( float u, float v ) {
	float omu = 1.0f - u;
	float omuu = omu * u;
	float omv = 1.0f - v;
	float omvv = omv * v;
	float omu2 = omu * omu;
	float basisU[4];
	float basisV[4];
	basisU[0] = omu2 * omu;
	basisU[1] = 3.0f * omuu * omu;
	basisU[2] = 3.0f * omuu * u;
	float u2 = u * u;
	basisU[3] = u2 * u;
	float omv2 = omv * omv;
	basisV[0] = omv2 * omv;
	basisV[1] = 3.0f * omvv * omv;
	basisV[2] = 3.0f * omvv * v;
	float v2 = v * v;
	basisV[3] = v2 * v;

	int i;
	int j;
	int index = 0;
	for ( j = 0; j < 4; j++ ) {
		for ( i = 0; i < 4; i++ ) {
			weights[index++] = basisU[i] * basisV[j];
		}
	}

	if ( normals ) {
		float threeU = 3.0f * u;
		float threeV = 3.0f * v;
		float slopeU[4];
		float slopeV[4];
		slopeU[0] = omu2 * -3.0f;
		slopeU[1] = 3.0f * ( 1.0f - threeU ) * omu;
		slopeU[2] = 3.0f * ( 2.0f - threeU ) * u;
		slopeU[3] = 3.0f * u2;
		slopeV[0] = omv2 * -3.0f;
		slopeV[1] = 3.0f * ( 1.0f - threeV ) * omv;
		slopeV[2] = 3.0f * ( 2.0f - threeV ) * v;
		slopeV[3] = 3.0f * v2;

		index = 0;
		for ( j = 0; j < 4; j++ ) {
			for ( i = 0; i < 4; i++ ) {
				uDerivatives[index++] = slopeU[i] * basisV[j];
			}
		}
		index = 0;
		for ( j = 0; j < 4; j++ ) {
			for ( i = 0; i < 4; i++ ) {
				vDerivatives[index++] = basisU[i] * slopeV[j];
			}
		}
	}
}
