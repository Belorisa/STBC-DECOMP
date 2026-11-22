// NiBezierRectangle2Slot43.cpp -- NiBezierRectangle2::VirtualSlot43, in a unit of its own
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
// FUNCTION: STBC 0x00855070 _$E48
// FUNCTION: STBC 0x00855080 _$E72
// FUNCTION: STBC 0x00855090 _$E76
// FUNCTION: STBC 0x008550a0 _$E80
// FUNCTION: STBC 0x008550b0 _$E84
// FUNCTION: STBC 0x008550c0 _$E197
// FUNCTION: STBC 0x008550d0 _$E56
// FUNCTION: STBC 0x008550e0 _$E60
// FUNCTION: STBC 0x008550f0 _$E64
// FUNCTION: STBC 0x00855100 _$E68
// FUNCTION: STBC 0x00855110 _$E205
// FUNCTION: STBC 0x00855120 _$E211
// FUNCTION: STBC 0x00855130 _$E215

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00854ff0 _$E226
// FUNCTION: STBC 0x00855010 _$E17
// FUNCTION: STBC 0x00855030 _$E15
// FUNCTION: STBC 0x00855040 _$E23
// FUNCTION: STBC 0x00855060 _$E21

/*
================
NiBezierRectangle2::VirtualSlot43

Sets the weights of the nine control points at (u, v) as the products of
the quadratic Bernstein basis in each parameter, and their derivatives
with u and v when the patch has normals.
================
*/
// FUNCTION: STBC 0x854E80
void NiBezierRectangle2::VirtualSlot43( float u, float v ) {
	float omu = 1.0f - u;
	float omv = 1.0f - v;
	float basisU[3];
	float basisV[3];
	basisU[0] = omu * omu;
	basisU[1] = 2.0f * ( omu * u );
	basisU[2] = u * u;
	basisV[0] = omv * omv;
	basisV[1] = 2.0f * ( omv * v );
	basisV[2] = v * v;

	int i;
	int j;
	int index = 0;
	for ( j = 0; j < 3; j++ ) {
		for ( i = 0; i < 3; i++ ) {
			weights[index++] = basisU[i] * basisV[j];
		}
	}

	if ( normals ) {
		float twoU = 2.0f * u;
		float twoV = 2.0f * v;
		float slopeU[3];
		float slopeV[3];
		slopeU[0] = omu * -2.0f;
		slopeU[1] = 2.0f * ( 1.0f - twoU );
		slopeU[2] = twoU;
		slopeV[0] = omv * -2.0f;
		slopeV[1] = 2.0f * ( 1.0f - twoV );
		slopeV[2] = twoV;

		index = 0;
		for ( j = 0; j < 3; j++ ) {
			for ( i = 0; i < 3; i++ ) {
				uDerivatives[index++] = slopeU[i] * basisV[j];
			}
		}
		index = 0;
		for ( j = 0; j < 3; j++ ) {
			for ( i = 0; i < 3; i++ ) {
				vDerivatives[index++] = basisU[i] * slopeV[j];
			}
		}
	}
}
