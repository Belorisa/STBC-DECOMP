// NiBezierTriangle2Slot43.cpp -- NiBezierTriangle2::VirtualSlot43, in a unit of its own
//
// The file name is invented: retail keeps this function in a unit of its own,
// after the Bezier patch units, holding only it, its <iostream> run and the
// headers' class registrations; nothing names the unit.

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiTriShapeDynamicData.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008544e0 _$E48
// FUNCTION: STBC 0x008544f0 _$E72
// FUNCTION: STBC 0x00854500 _$E76
// FUNCTION: STBC 0x00854510 _$E80
// FUNCTION: STBC 0x00854520 _$E84
// FUNCTION: STBC 0x00854530 _$E197
// FUNCTION: STBC 0x00854540 _$E56
// FUNCTION: STBC 0x00854550 _$E60
// FUNCTION: STBC 0x00854560 _$E64
// FUNCTION: STBC 0x00854570 _$E68
// FUNCTION: STBC 0x00854580 _$E205
// FUNCTION: STBC 0x00854590 _$E211
// FUNCTION: STBC 0x008545a0 _$E215
// FUNCTION: STBC 0x008545b0 _$E219

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00854460 _$E230
// FUNCTION: STBC 0x00854480 _$E17
// FUNCTION: STBC 0x008544a0 _$E15
// FUNCTION: STBC 0x008544b0 _$E23
// FUNCTION: STBC 0x008544d0 _$E21

/*
================
NiBezierTriangle2::VirtualSlot43

Sets the weights of the six control points at barycentric (u, v) from the
quadratic Bernstein basis, and their derivatives with u and v when the
patch has normals.
================
*/
// FUNCTION: STBC 0x854320
void NiBezierTriangle2::VirtualSlot43( float u, float v ) {
	float w = 1.0f - u - v;
	float twoU = 2.0f * u;
	float twoV = 2.0f * v;
	float twoW = 2.0f * w;

	weights[0] = w * w;
	weights[1] = twoU * w;
	weights[2] = u * u;
	weights[3] = twoV * w;
	weights[4] = twoU * v;
	weights[5] = v * v;

	if ( normals ) {
		uDerivatives[0] = -twoW;
		uDerivatives[1] = twoW - twoU;
		uDerivatives[2] = twoU;
		uDerivatives[3] = -twoV;
		uDerivatives[4] = twoV;
		uDerivatives[5] = 0.0f;

		vDerivatives[0] = -twoW;
		vDerivatives[1] = -twoU;
		vDerivatives[2] = 0.0f;
		vDerivatives[3] = twoW - twoV;
		vDerivatives[4] = twoU;
		vDerivatives[5] = twoV;
	}
}
