/*
===============================================================================

	DisruptorGraphic.cpp

===============================================================================
*/

#include <iostream>
#include "DisruptorGraphic.h"
#include "TGColorA.h"
#include "NiMath.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiAlphaProperty.h"
#include "NiFogProperty.h"
#include <math.h>
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004ddc30 _$E418
// FUNCTION: STBC 0x004ddc50 _$E417

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004ddbb0 _$E32
// FUNCTION: STBC 0x004ddbc0 _$E29
// FUNCTION: STBC 0x004ddbd0 _$E31
// FUNCTION: STBC 0x004ddbe0 _$E30
// FUNCTION: STBC 0x004ddbf0 _$E38
// FUNCTION: STBC 0x004ddc00 _$E35
// FUNCTION: STBC 0x004ddc10 _$E37
// FUNCTION: STBC 0x004ddc20 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004ddc60 _$E45
// FUNCTION: STBC 0x004ddc70 _$E44
// FUNCTION: STBC 0x004ddc80 _$E56
// FUNCTION: STBC 0x004ddc90 _$E55
// FUNCTION: STBC 0x004ddca0 _$E60
// FUNCTION: STBC 0x004ddcb0 _$E59
// FUNCTION: STBC 0x004ddcc0 _$E64
// FUNCTION: STBC 0x004ddcd0 _$E63
// FUNCTION: STBC 0x004ddce0 _$E68
// FUNCTION: STBC 0x004ddcf0 _$E67
// FUNCTION: STBC 0x004ddd00 _$E192
// FUNCTION: STBC 0x004ddd10 _$E191
// FUNCTION: STBC 0x004ddd20 _$E200
// FUNCTION: STBC 0x004ddd30 _$E199
// FUNCTION: STBC 0x004ddd40 _$E207
// FUNCTION: STBC 0x004ddd50 _$E206
// FUNCTION: STBC 0x004ddd60 _$E211
// FUNCTION: STBC 0x004ddd70 _$E210
// FUNCTION: STBC 0x004ddd80 _$E215
// FUNCTION: STBC 0x004ddd90 _$E214
// FUNCTION: STBC 0x004ddda0 _$E219
// FUNCTION: STBC 0x004dddb0 _$E218
// FUNCTION: STBC 0x004dddc0 _$E225
// FUNCTION: STBC 0x004dddd0 _$E224
// FUNCTION: STBC 0x004dddf0 _$E134
// FUNCTION: STBC 0x004dde00 _$E133
// FUNCTION: STBC 0x004dde10 _$E147
// FUNCTION: STBC 0x004dde20 _$E146
// FUNCTION: STBC 0x004dde30 _$E167
// FUNCTION: STBC 0x004dde40 _$E166
// FUNCTION: STBC 0x004dde50 _$E122
// FUNCTION: STBC 0x004dde60 _$E121
// FUNCTION: STBC 0x004dde70 _$E177
// FUNCTION: STBC 0x004dde80 _$E176
// FUNCTION: STBC 0x004dde90 _$E187
// FUNCTION: STBC 0x004ddea0 _$E186
// FUNCTION: STBC 0x004ddeb0 _$E92
// FUNCTION: STBC 0x004ddec0 _$E91
// FUNCTION: STBC 0x004dded0 _$E117
// FUNCTION: STBC 0x004ddee0 _$E116
// FUNCTION: STBC 0x004ddef0 _$E111
// FUNCTION: STBC 0x004ddf00 _$E110

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004dd1f0 _$E408
// FUNCTION: STBC 0x004dd200 _$E404
// FUNCTION: STBC 0x004dd220 _$E407
// FUNCTION: STBC 0x004dd230 _$E405
// DATA: STBC 0x985F18 ?rtti@DisruptorGraphic@@2VNiRTTI@@B
const NiRTTI DisruptorGraphic::rtti( "DisruptorGraphic", &TGFuzzyTriShape::rtti );

// The bolt's profile: the tip, the rings' positions along the axis, in 55ths
// of the length from the tail, and their radii, in 13ths of the radius, and
// the tail (invented names).
const int		RING_COUNT = 4;
const int		PROFILE_COUNT = RING_COUNT + 2;

/*
================
DisruptorGraphic::DisruptorGraphic

The shell is a tip, four rings of segments vertices and a tail along the
bolt's axis (y), in the shell colour; the core is the same shape, scaled
around the axis by coreRadiusScale and along it by coreLengthScale about
the shell's tip, in the core colour. Each is closed by a fan at the tip, the
quads between the rings, and a fan at the tail. The shape draws its vertex
colours unlit and blended additively, without writing depth or fog.
================
*/
// FUNCTION: STBC 0x004dd250
// DRAFT: call set agrees (the tip and tail vertices as NiPoint3 temporaries put NiAlphaProperty's SetField and SetBit out of line as retail); the ring loop's registers and frame slots differ (frame 0x5C against 0x64)
DisruptorGraphic::DisruptorGraphic( TGColorA *shellColor, TGColorA *coreColor, int segments, float length, float radius,
									float coreRadiusScale, float coreLengthScale ) :
	TGFuzzyTriShape( segments * 8 + 4, new NiPoint3[segments * 8 + 4], NULL, new NiColorA[segments * 8 + 4], NULL, segments * 16,
					 new unsigned short[segments * 48] ) {
	NiTriShapeData *data = GetShapeData();
	NiPoint3 *vertices = data->GetVertices();
	NiColorA *colors = data->GetColors();
	unsigned short *triangles = data->GetTriList();

	float halfLength = length * 0.5f;
	// tip, rings, tail
	float profileY[PROFILE_COUNT];
	profileY[0] = length - halfLength;
	profileY[1] = length * ( 54.6f / 55.0f ) - halfLength;
	profileY[2] = length * ( 53.5f / 55.0f ) - halfLength;
	profileY[3] = length * ( 51.0f / 55.0f ) - halfLength;
	profileY[4] = length * ( 40.0f / 55.0f ) - halfLength;
	profileY[5] = -halfLength;
	float profileRadius[PROFILE_COUNT];
	profileRadius[0] = 0.0f;
	profileRadius[1] = radius * ( 5.0f / 13.0f );
	profileRadius[2] = radius * ( 10.0f / 13.0f );
	profileRadius[3] = radius;
	profileRadius[4] = radius * ( 8.5f / 13.0f );
	profileRadius[5] = 0.0f;
	float coreOffset = ( 1.0f - coreLengthScale ) * halfLength;

	int shellTail = segments * 4 + 1;
	int coreTip = segments * 4 + 2;
	int coreTail = segments * 8 + 3;

	vertices[0] = NiPoint3( 0.0f, profileY[0], 0.0f );
	colors[0] = *shellColor;
	vertices[coreTip] = NiPoint3( 0.0f, profileY[0] * coreLengthScale + coreOffset, 0.0f );
	colors[coreTip] = *coreColor;
	vertices[shellTail] = NiPoint3( 0.0f, profileY[PROFILE_COUNT - 1], 0.0f );
	colors[shellTail] = *shellColor;
	vertices[coreTail] = NiPoint3( 0.0f, profileY[PROFILE_COUNT - 1] * coreLengthScale + coreOffset, 0.0f );
	colors[coreTail] = *coreColor;

	int i;
	int ring;
	float step = NI_TWO_PI / segments;
	for ( i = 0; i < segments; i++ ) {
		float angle = i * step;
		float c = cos( angle );
		float s = sin( angle );
		for ( ring = 0; ring < RING_COUNT; ring++ ) {
			int shellIndex = 1 + ring * segments + i;
			int coreIndex = coreTip + 1 + ring * segments + i;
			float x = c * profileRadius[ring + 1];
			float z = s * profileRadius[ring + 1];
			vertices[shellIndex].x = x;
			vertices[shellIndex].y = profileY[ring + 1];
			vertices[shellIndex].z = z;
			colors[shellIndex] = *shellColor;
			vertices[coreIndex].x = x * coreRadiusScale;
			vertices[coreIndex].y = coreLengthScale * profileY[ring + 1] + coreOffset;
			vertices[coreIndex].z = z * coreRadiusScale;
			colors[coreIndex] = *coreColor;
		}
	}

	int k = 0;
	for ( i = 0; i < segments; i++ ) {
		int next = ( i + 1 ) % segments;
		triangles[k++] = 0;
		triangles[k++] = next + 1;
		triangles[k++] = i + 1;
		for ( ring = 1; ring < RING_COUNT; ring++ ) {
			triangles[k++] = i + 1 + ( ring - 1 ) * segments;
			triangles[k++] = next + 1 + ( ring - 1 ) * segments;
			triangles[k++] = next + 1 + ring * segments;
			triangles[k++] = i + 1 + ring * segments;
			triangles[k++] = i + 1 + ( ring - 1 ) * segments;
			triangles[k++] = next + 1 + ring * segments;
		}
		triangles[k++] = shellTail;
		triangles[k++] = shellTail - next - 1;
		triangles[k++] = shellTail - i - 1;
	}
	for ( i = 0; i < segments; i++ ) {
		int next = ( i + 1 ) % segments;
		triangles[k++] = coreTip;
		triangles[k++] = next + 1 + coreTip;
		triangles[k++] = i + 1 + coreTip;
		for ( ring = 1; ring < RING_COUNT; ring++ ) {
			triangles[k++] = i + ( ring - 1 ) * segments + 1 + coreTip;
			triangles[k++] = segments * 4 + 3 + ( ring - 1 ) * segments + next;
			triangles[k++] = next + ring * segments + 1 + coreTip;
			triangles[k++] = segments * 4 + 3 + ring * segments + i;
			triangles[k++] = i + ( ring - 1 ) * segments + 1 + coreTip;
			triangles[k++] = next + ring * segments + 1 + coreTip;
		}
		triangles[k++] = coreTail;
		triangles[k++] = coreTip - next + segments * 4;
		triangles[k++] = coreTip - i + segments * 4;
	}

	data->CalculateNormals();
	data->bound.unknown10 = true;

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	AttachProperty( alpha );

	NiFogProperty *fog = new NiFogProperty;
	fog->SetFog( false );
	AttachProperty( fog );

	UpdateProperties();
}

/*
================
DisruptorGraphic::~DisruptorGraphic
================
*/
// FUNCTION: STBC 0x004ddb80
DisruptorGraphic::~DisruptorGraphic() {
}
