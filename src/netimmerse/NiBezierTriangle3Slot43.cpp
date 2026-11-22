// NiBezierTriangle3Slot43.cpp -- NiBezierTriangle3::VirtualSlot43, in a unit of its own
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
// FUNCTION: STBC 0x008545c0 _$E223
// FUNCTION: STBC 0x008548a0 _$E48
// FUNCTION: STBC 0x008548b0 _$E72
// FUNCTION: STBC 0x008548c0 _$E76
// FUNCTION: STBC 0x008548d0 _$E80
// FUNCTION: STBC 0x008548e0 _$E84
// FUNCTION: STBC 0x008548f0 _$E197
// FUNCTION: STBC 0x00854900 _$E56
// FUNCTION: STBC 0x00854910 _$E60
// FUNCTION: STBC 0x00854920 _$E64
// FUNCTION: STBC 0x00854930 _$E68
// FUNCTION: STBC 0x00854940 _$E205
// FUNCTION: STBC 0x00854950 _$E211
// FUNCTION: STBC 0x00854960 _$E215
// FUNCTION: STBC 0x00854970 _$E219

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00854820 _$E230
// FUNCTION: STBC 0x00854840 _$E17
// FUNCTION: STBC 0x00854860 _$E15
// FUNCTION: STBC 0x00854870 _$E23
// FUNCTION: STBC 0x00854890 _$E21

/*
================
NiBezierTriangle3::VirtualSlot43

Sets the weights of the ten control points at barycentric (u, v) from the
cubic Bernstein basis, and their derivatives with u and v when the patch
has normals.
================
*/
// FUNCTION: STBC 0x8545D0
void NiBezierTriangle3::VirtualSlot43( float u, float v ) {
	float w = 1.0f - u - v;
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;
	float threeU2 = 3.0f * u2;
	float threeV2 = 3.0f * v2;
	float threeW2 = 3.0f * w2;
	float sixUV = 6.0f * u * v;

	weights[0] = w2 * w;
	weights[1] = threeW2 * u;
	weights[2] = threeU2 * w;
	weights[3] = u2 * u;
	weights[4] = threeW2 * v;
	weights[5] = sixUV * w;
	weights[6] = threeU2 * v;
	weights[7] = threeV2 * w;
	weights[8] = threeV2 * u;
	weights[9] = v2 * v;

	if ( normals ) {
		float sixWU = 6.0f * w * u;
		float sixWV = 6.0f * w * v;

		uDerivatives[0] = -threeW2;
		uDerivatives[1] = threeW2 - sixWU;
		uDerivatives[2] = sixWU - threeU2;
		uDerivatives[3] = threeU2;
		uDerivatives[4] = -sixWV;
		uDerivatives[5] = sixWV - sixUV;
		uDerivatives[6] = sixUV;
		uDerivatives[7] = -threeV2;
		uDerivatives[8] = threeV2;
		uDerivatives[9] = 0.0f;

		vDerivatives[0] = -threeW2;
		vDerivatives[1] = -sixWU;
		vDerivatives[2] = -threeU2;
		vDerivatives[3] = 0.0f;
		vDerivatives[4] = threeW2 - sixWV;
		vDerivatives[5] = sixWU - sixUV;
		vDerivatives[6] = threeU2;
		vDerivatives[7] = sixWV - threeV2;
		vDerivatives[8] = sixUV;
		vDerivatives[9] = threeV2;
	}
}
