// NiBezierTriangle4Slot43.cpp -- NiBezierTriangle4::VirtualSlot43, in a unit of its own
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
// FUNCTION: STBC 0x00854980 _$E223
// FUNCTION: STBC 0x00854da0 _$E48
// FUNCTION: STBC 0x00854db0 _$E72
// FUNCTION: STBC 0x00854dc0 _$E76
// FUNCTION: STBC 0x00854dd0 _$E80
// FUNCTION: STBC 0x00854de0 _$E84
// FUNCTION: STBC 0x00854df0 _$E197
// FUNCTION: STBC 0x00854e00 _$E56
// FUNCTION: STBC 0x00854e10 _$E60
// FUNCTION: STBC 0x00854e20 _$E64
// FUNCTION: STBC 0x00854e30 _$E68
// FUNCTION: STBC 0x00854e40 _$E205
// FUNCTION: STBC 0x00854e50 _$E211
// FUNCTION: STBC 0x00854e60 _$E215
// FUNCTION: STBC 0x00854e70 _$E219

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00854d20 _$E230
// FUNCTION: STBC 0x00854d40 _$E17
// FUNCTION: STBC 0x00854d60 _$E15
// FUNCTION: STBC 0x00854d70 _$E23
// FUNCTION: STBC 0x00854d90 _$E21

/*
================
NiBezierTriangle4::VirtualSlot43

Sets the weights of the fifteen control points at barycentric (u, v) from
the quartic Bernstein basis, and their derivatives with u and v when the
patch has normals.
================
*/
// FUNCTION: STBC 0x854990
void NiBezierTriangle4::VirtualSlot43( float u, float v ) {
	float w = 1.0f - u - v;
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;
	float fourU3 = 4.0f * u2 * u;
	float fourV3 = 4.0f * v2 * v;
	float fourW3 = 4.0f * w2 * w;
	float twelveW2U = 12.0f * w2 * u;
	float twelveU2W = 12.0f * u2 * w;
	float twelveV2U = 12.0f * v2 * u;

	weights[0] = w2 * w2;
	weights[1] = fourW3 * u;
	weights[2] = 6.0f * w2 * u2;
	weights[3] = fourU3 * w;
	weights[4] = u2 * u2;
	weights[5] = fourW3 * v;
	weights[6] = twelveW2U * v;
	weights[7] = twelveU2W * v;
	weights[8] = fourU3 * v;
	weights[9] = 6.0f * w2 * v2;
	weights[10] = twelveV2U * w;
	weights[11] = 6.0f * v2 * u2;
	weights[12] = fourV3 * w;
	weights[13] = fourV3 * u;
	weights[14] = v2 * v2;

	if ( normals ) {
		float twelveW2V = 12.0f * w2 * v;
		float twelveWV2 = 12.0f * v2 * w;
		float twelveU2V = 12.0f * u2 * v;
		float twentyFourWUV = 24.0f * w * u * v;

		uDerivatives[0] = -fourW3;
		uDerivatives[1] = fourW3 - twelveW2U;
		uDerivatives[2] = twelveW2U - twelveU2W;
		uDerivatives[3] = twelveU2W - fourU3;
		uDerivatives[4] = fourU3;
		uDerivatives[5] = -twelveW2V;
		uDerivatives[6] = twelveW2V - twentyFourWUV;
		uDerivatives[7] = twentyFourWUV - twelveU2V;
		uDerivatives[8] = twelveU2V;
		uDerivatives[9] = -twelveWV2;
		uDerivatives[10] = twelveWV2 - twelveV2U;
		uDerivatives[11] = twelveV2U;
		uDerivatives[12] = -fourV3;
		uDerivatives[13] = fourV3;
		uDerivatives[14] = 0.0f;

		vDerivatives[0] = -fourW3;
		vDerivatives[1] = -twelveW2U;
		vDerivatives[2] = -twelveU2W;
		vDerivatives[3] = -fourU3;
		vDerivatives[4] = 0.0f;
		vDerivatives[5] = fourW3 - twelveW2V;
		vDerivatives[6] = twelveW2U - twentyFourWUV;
		vDerivatives[7] = twelveU2W - twelveU2V;
		vDerivatives[8] = fourU3;
		vDerivatives[9] = twelveW2V - twelveWV2;
		vDerivatives[10] = twentyFourWUV - twelveV2U;
		vDerivatives[11] = twelveU2V;
		vDerivatives[12] = twelveWV2 - fourV3;
		vDerivatives[13] = twelveV2U;
		vDerivatives[14] = fourV3;
	}
}
