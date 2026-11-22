/*
===============================================================================

	TractorBeamGraphic.cpp

===============================================================================
*/

#include <iostream>
#include "TractorBeamGraphic.h"
#include "TractorBeamProperty.h"
#include "ShipClass.h"
#include "NiPointLight.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "Backdrop.h"
#include "GlowPass.h"
#include "NiCamera.h"
#include "SetClass.h"
#include "ImageManagerClass.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiFogProperty.h"
#include <math.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004f9360 _$E471
// FUNCTION: STBC 0x004f9380 _$E470

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004f7b60 _$E45
// FUNCTION: STBC 0x004f7b70 _$E44
// FUNCTION: STBC 0x004f7b80 _$E285
// FUNCTION: STBC 0x004f7b90 _$E284
// FUNCTION: STBC 0x004f8230 _$E56
// FUNCTION: STBC 0x004f8240 _$E55
// FUNCTION: STBC 0x004f8250 _$E60
// FUNCTION: STBC 0x004f8260 _$E59
// FUNCTION: STBC 0x004f8810 _$E64
// FUNCTION: STBC 0x004f8820 _$E63
// FUNCTION: STBC 0x004f8b10 _$E68
// FUNCTION: STBC 0x004f8b20 _$E67
// FUNCTION: STBC 0x004f8b60 _$E192
// FUNCTION: STBC 0x004f8b70 _$E191
// FUNCTION: STBC 0x004f8b80 _$E134
// FUNCTION: STBC 0x004f8b90 _$E133
// FUNCTION: STBC 0x004f8ba0 _$E147
// FUNCTION: STBC 0x004f8bb0 _$E146
// FUNCTION: STBC 0x004f8bc0 _$E197
// FUNCTION: STBC 0x004f8bd0 _$E196
// FUNCTION: STBC 0x004f8be0 _$E290
// FUNCTION: STBC 0x004f8bf0 _$E289
// FUNCTION: STBC 0x004f8c00 _$E201
// FUNCTION: STBC 0x004f8c10 _$E200
// FUNCTION: STBC 0x004f8c20 _$E294
// FUNCTION: STBC 0x004f8c30 _$E293
// FUNCTION: STBC 0x004f8c40 _$E301
// FUNCTION: STBC 0x004f8c50 _$E300
// FUNCTION: STBC 0x004f8c60 _$E316
// FUNCTION: STBC 0x004f8c70 _$E315
// FUNCTION: STBC 0x004f8c80 _$E320
// FUNCTION: STBC 0x004f8c90 _$E319
// FUNCTION: STBC 0x004f8ca0 _$E92
// FUNCTION: STBC 0x004f8cb0 _$E91
// FUNCTION: STBC 0x004f8cc0 _$E209
// FUNCTION: STBC 0x004f8cd0 _$E208
// FUNCTION: STBC 0x004f8ce0 _$E216
// FUNCTION: STBC 0x004f8cf0 _$E215
// FUNCTION: STBC 0x004f8d00 _$E220
// FUNCTION: STBC 0x004f8d10 _$E219
// FUNCTION: STBC 0x004f8d20 _$E224
// FUNCTION: STBC 0x004f8d30 _$E223
// FUNCTION: STBC 0x004f8d40 _$E228
// FUNCTION: STBC 0x004f8d50 _$E227
// FUNCTION: STBC 0x004f8d60 _$E234
// FUNCTION: STBC 0x004f8d70 _$E233
// FUNCTION: STBC 0x004f8d80 _$E273
// FUNCTION: STBC 0x004f8d90 _$E272
// FUNCTION: STBC 0x004f8da0 _$E324
// FUNCTION: STBC 0x004f8db0 _$E323
// FUNCTION: STBC 0x004f8dc0 _$E328
// FUNCTION: STBC 0x004f8dd0 _$E327
// FUNCTION: STBC 0x004f8de0 _$E332
// FUNCTION: STBC 0x004f8df0 _$E331
// FUNCTION: STBC 0x004f8e00 _$E336
// FUNCTION: STBC 0x004f8e10 _$E335
// FUNCTION: STBC 0x004f8e20 _$E340
// FUNCTION: STBC 0x004f8e30 _$E339
// FUNCTION: STBC 0x004f8e40 _$E238
// FUNCTION: STBC 0x004f8e50 _$E237
// FUNCTION: STBC 0x004f8e60 _$E344
// FUNCTION: STBC 0x004f8e70 _$E343
// FUNCTION: STBC 0x004f8e80 _$E348
// FUNCTION: STBC 0x004f8e90 _$E347
// FUNCTION: STBC 0x004f8ea0 _$E352
// FUNCTION: STBC 0x004f8eb0 _$E351
// FUNCTION: STBC 0x004f8ec0 _$E97
// FUNCTION: STBC 0x004f8ed0 _$E96
// FUNCTION: STBC 0x004f8ee0 _$E356
// FUNCTION: STBC 0x004f8ef0 _$E355
// FUNCTION: STBC 0x004f8f00 _$E102
// FUNCTION: STBC 0x004f8f10 _$E101
// FUNCTION: STBC 0x004f8f20 _$E360
// FUNCTION: STBC 0x004f8f30 _$E359
// FUNCTION: STBC 0x004f8f40 _$E364
// FUNCTION: STBC 0x004f8f50 _$E363
// FUNCTION: STBC 0x004f8f60 _$E368
// FUNCTION: STBC 0x004f8f70 _$E367
// FUNCTION: STBC 0x004f8f80 _$E372
// FUNCTION: STBC 0x004f8f90 _$E371
// FUNCTION: STBC 0x004f8fa0 _$E111
// FUNCTION: STBC 0x004f8fb0 _$E110
// FUNCTION: STBC 0x004f8fc0 _$E376
// FUNCTION: STBC 0x004f8fd0 _$E375
// FUNCTION: STBC 0x004f8fe0 _$E380
// FUNCTION: STBC 0x004f8ff0 _$E379
// FUNCTION: STBC 0x004f9000 _$E384
// FUNCTION: STBC 0x004f9010 _$E383
// FUNCTION: STBC 0x004f9020 _$E117
// FUNCTION: STBC 0x004f9030 _$E116
// FUNCTION: STBC 0x004f9040 _$E122
// FUNCTION: STBC 0x004f9050 _$E121
// FUNCTION: STBC 0x004f9060 _$E152
// FUNCTION: STBC 0x004f9070 _$E151
// FUNCTION: STBC 0x004f9080 _$E277
// FUNCTION: STBC 0x004f9090 _$E276
// FUNCTION: STBC 0x004f90a0 _$E157
// FUNCTION: STBC 0x004f90b0 _$E156
// FUNCTION: STBC 0x004f90c0 _$E388
// FUNCTION: STBC 0x004f90d0 _$E387
// FUNCTION: STBC 0x004f90e0 _$E392
// FUNCTION: STBC 0x004f90f0 _$E391
// FUNCTION: STBC 0x004f9100 _$E162
// FUNCTION: STBC 0x004f9110 _$E161
// FUNCTION: STBC 0x004f9120 _$E396
// FUNCTION: STBC 0x004f9130 _$E395
// FUNCTION: STBC 0x004f9140 _$E127
// FUNCTION: STBC 0x004f9150 _$E126
// FUNCTION: STBC 0x004f9160 _$E400
// FUNCTION: STBC 0x004f9170 _$E399
// FUNCTION: STBC 0x004f9180 _$E404
// FUNCTION: STBC 0x004f9190 _$E403
// FUNCTION: STBC 0x004f91a0 _$E167
// FUNCTION: STBC 0x004f91b0 _$E166
// FUNCTION: STBC 0x004f91c0 _$E172
// FUNCTION: STBC 0x004f91d0 _$E171
// FUNCTION: STBC 0x004f91e0 _$E408
// FUNCTION: STBC 0x004f91f0 _$E407
// FUNCTION: STBC 0x004f9200 _$E412
// FUNCTION: STBC 0x004f9210 _$E411
// FUNCTION: STBC 0x004f9220 _$E416
// FUNCTION: STBC 0x004f9230 _$E415
// FUNCTION: STBC 0x004f9240 _$E420
// FUNCTION: STBC 0x004f9250 _$E419
// FUNCTION: STBC 0x004f9260 _$E424
// FUNCTION: STBC 0x004f9270 _$E423
// FUNCTION: STBC 0x004f9280 _$E428
// FUNCTION: STBC 0x004f9290 _$E427
// FUNCTION: STBC 0x004f92a0 _$E432
// FUNCTION: STBC 0x004f92b0 _$E431
// FUNCTION: STBC 0x004f92c0 _$E436
// FUNCTION: STBC 0x004f92d0 _$E435
// FUNCTION: STBC 0x004f92e0 _$E177
// FUNCTION: STBC 0x004f92f0 _$E176
// FUNCTION: STBC 0x004f9300 _$E440
// FUNCTION: STBC 0x004f9310 _$E439
// FUNCTION: STBC 0x004f9320 _$E182
// FUNCTION: STBC 0x004f9330 _$E181
// FUNCTION: STBC 0x004f9340 _$E187
// FUNCTION: STBC 0x004f9350 _$E186
// FUNCTION: STBC 0x004f9390 _$E444
// FUNCTION: STBC 0x004f93a0 _$E443
// FUNCTION: STBC 0x004f93b0 _$E449
// FUNCTION: STBC 0x004f93c0 _$E448
// FUNCTION: STBC 0x004f93d0 _$E453
// FUNCTION: STBC 0x004f93e0 _$E452

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004f7ae0 _$E32
// FUNCTION: STBC 0x004f7af0 _$E29
// FUNCTION: STBC 0x004f7b00 _$E31
// FUNCTION: STBC 0x004f7b10 _$E30
// FUNCTION: STBC 0x004f7b20 _$E38
// FUNCTION: STBC 0x004f7b30 _$E35
// FUNCTION: STBC 0x004f7b40 _$E37
// FUNCTION: STBC 0x004f7b50 _$E36

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004f5dd0 _$E459
// FUNCTION: STBC 0x004f5de0 _$E455
// FUNCTION: STBC 0x004f5e00 _$E458
// FUNCTION: STBC 0x004f5e10 _$E456
// DATA: STBC 0x986A10 ?rtti@TractorBeamGraphic@@2VNiRTTI@@B
const NiRTTI TractorBeamGraphic::rtti( "TractorBeamGraphic", &NiNode::rtti );

/*
================
TractorBeamShape::TractorBeamShape

PhaserBeam's two tubes, the shell and the core, each six rings of numSides
vertices around half a turn, but placed here: the rings' radii flare from
unknown6 out to the radius and close to a point over unknown11, the core
unknown7 times the shell's. The last two rings fade out. The colours and
the texture's t coordinate are PhaserBeam's.
================
*/
// FUNCTION: STBC 0x004f5e30
// DRAFT: register picks (this in esi, retail ebp) and frame slots; retail keeps the ring counter and the core index in frame slots and interleaves the radii stores with the member stores; one SetField more out of line in retail (inline budget, as PhaserGlow)
TractorBeamShape::TractorBeamShape( const NiColorA &color0, const NiColorA &color1, const NiColorA &color2, const NiColorA &color3,
									int numSides_, float radius, float unknown6, float unknown7, float unknown8, float unknown9,
									float unknown10, float unknown11, float unknown12, float unknown13, float textureSpeed_,
									char *textureName )
	: NiTriShape( numSides_ * TRACTOR_BEAM_RINGS * 2, new NiPoint3[numSides_ * TRACTOR_BEAM_RINGS * 2], NULL, new NiColorA[numSides_ * TRACTOR_BEAM_RINGS * 2],
				  new NiPoint2[numSides_ * TRACTOR_BEAM_RINGS * 2], ( numSides_ - 1 ) * ( TRACTOR_BEAM_RINGS - 1 ) * 4,
				  new unsigned short[( numSides_ - 1 ) * ( TRACTOR_BEAM_RINGS - 1 ) * 12] ),
	  unknownE4( true ), unknownE8( 0.7f ), unknownEC( 1.0f ), unknownF4( radius ), unknownF8( unknown11 ), unknownFC( unknown9 ),
	  unknown100( unknown8 ), unknown104( unknown10 ), unknown108( unknown12 ), unknown10C( unknown13 ), textureSpeed( textureSpeed_ ),
	  numSides( numSides_ ), sideAlphas( NULL ), sideTextureT( NULL ), textureOffset( 0.0f ) {
	int i;
	int ring;

	NiTriShapeData *data = GetShapeData();
	NiPoint3 *vertices = data->GetVertices();
	NiColorA *colors = data->GetColors();
	unsigned short *triangles = data->GetTriList();
	int shellVertices = numSides_ * TRACTOR_BEAM_RINGS;	// the core's vertices follow the shell's

	// the sides around half a turn of the beam's axis, the rings' radii
	// along it
	float sides = (float)( numSides_ - 1 );
	float step = NI_PI / sides;
	float radii[TRACTOR_BEAM_RINGS];
	radii[0] = unknown6;
	radii[1] = radius - ( radius - unknown6 ) * 0.25f;
	radii[2] = radius;
	radii[3] = unknown11;
	radii[4] = unknown11 * 0.5f;
	radii[5] = 0.0f;
	sideAlphas = new float[numSides_ * TRACTOR_BEAM_RINGS * 2];
	for ( i = 0; i < numSides; i++ ) {
		float angle = i * step;
		float cosine = (float)cos( angle );
		float sine = (float)sin( angle );
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS; ring++ ) {
			int index = ring * numSides_ + i;
			vertices[index] = NiPoint3( cosine * radii[ring], 0.0f, sine * radii[ring] );
			if ( ring == 5 ) {
				sideAlphas[index] = 0.0f;
			} else if ( ring == 4 ) {
				sideAlphas[index] = sine * 0.5f;
			} else {
				sideAlphas[index] = sine;
			}
			index += shellVertices;
			vertices[index] = NiPoint3( cosine * radii[ring] * unknown7, 0.0f, sine * radii[ring] * unknown7 );
			if ( ring == 5 ) {
				sideAlphas[index] = 0.0f;
			} else if ( ring == 4 ) {
				sideAlphas[index] = sine * 0.5f;
			} else {
				sideAlphas[index] = sine;
			}
		}
	}

	// the colours, from color0 and color2 at the edges to color1 and
	// color3 in the middle side
	NiColorA shellStep = ( color1 - color0 ) / (float)( numSides_ / 2 - 1 );
	NiColorA coreStep = ( color3 - color2 ) / (float)( numSides_ / 2 - 1 );
	int halfSides = numSides_ / 2;
	for ( i = 0; i < halfSides; i++ ) {
		NiColorA shellColor = color0 + shellStep * (float)i;
		NiColorA coreColor = color2 + coreStep * (float)i;
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS; ring++ ) {
			colors[ring * numSides_ + i] = shellColor;
			colors[shellVertices + ring * numSides_ + i] = coreColor;
			colors[( ring + 1 ) * numSides_ - 1 - i] = shellColor;
			colors[shellVertices + ( ring + 1 ) * numSides_ - 1 - i] = coreColor;
		}
	}
	if ( numSides_ % 2 != 0 ) {
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS; ring++ ) {
			// BUG: one side past the middle one, which the loop above has
			// already coloured: the middle side keeps no colour (as PhaserBeam)
			colors[ring * numSides_ + halfSides + 1].r = color1.r;
			colors[ring * numSides_ + halfSides + 1].g = color1.g;
			colors[ring * numSides_ + halfSides + 1].b = color1.b;
			colors[shellVertices + ring * numSides_ + halfSides + 1].r = color3.r;
			colors[shellVertices + ring * numSides_ + halfSides + 1].g = color3.g;
			colors[shellVertices + ring * numSides_ + halfSides + 1].b = color3.b;
		}
	}

	// two triangles between each pair of rings and sides, the shell's then
	// the core's
	int count = 0;
	for ( i = 0; i < numSides_ - 1; i++ ) {
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS - 1; ring++ ) {
			int a = ring * numSides_ + i;
			int b = ( ring + 1 ) * numSides_ + i;
			triangles[count++] = a;
			triangles[count++] = b + 1;
			triangles[count++] = a + 1;
			triangles[count++] = b;
			triangles[count++] = b + 1;
			triangles[count++] = a;
		}
	}
	for ( i = 0; i < numSides_ - 1; i++ ) {
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS - 1; ring++ ) {
			int a = ring * numSides_ + shellVertices + i;
			int b = ( ring + 1 ) * numSides_ + shellVertices + i;
			triangles[count++] = a;
			triangles[count++] = b + 1;
			triangles[count++] = a + 1;
			triangles[count++] = b;
			triangles[count++] = b + 1;
			triangles[count++] = a;
		}
	}

	// the texture's t coordinate across the sides
	sideTextureT = new float[numSides_ * TRACTOR_BEAM_RINGS * 2];
	for ( i = 0; i < numSides_; i++ ) {
		float t = (float)i / sides;
		for ( ring = 0; ring < TRACTOR_BEAM_RINGS; ring++ ) {
			sideTextureT[ring * numSides_ + i] = t * unknown10C;
			sideTextureT[shellVertices + ring * numSides_ + i] = t * unknown10C;
		}
	}

	GetShapeData()->GetBound().unknown10 = true;

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( textureName, true, true ) );
	AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
	textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
	AttachProperty( textureMode );

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

	GlowPass *glowPass = new GlowPass;
	glowPass->unknown14 = false;
	AddExtraData( glowPass );
	UpdateProperties();
}

/*
================
TractorBeamShape::~TractorBeamShape
================
*/
// FUNCTION: STBC 0x004f6be0
TractorBeamShape::~TractorBeamShape() {
	delete [] sideAlphas;
	delete [] sideTextureT;
}

/*
================
TractorBeamShape::SetDirection

Turns the beam from its rest direction, +y, to direction, and places the six
rings of the tube along length: the ends and four in between, with the
flared ends taking a part of the length set by the shape's limits. Every side
of the tube gets the same height on each ring. The model bound is recomputed.
================
*/
// FUNCTION: STBC 0x004f6c10
// PARKED: schedule: retail tests the side count after the second ring store (fld; test; fstp), this build before it; declaration order of the vertex pointer, the core offset and the rings doesn't move it
void TractorBeamShape::SetDirection( const NiPoint3 &direction_, float length_ ) {
	direction = direction_;
	length = length_;

	NiPoint3 axis = direction_.Cross( NiPoint3::UNIT_Y );
	axis.Unitize();
	float angle = NiACos( direction_.y );
	NiMatrix3 rotation;
	rotation.MakeRotation( angle, axis );
	SetRotate( rotation );

	float flare = unknown100 * ( length_ * 0.5f );
	if ( flare < unknownFC ) {
		flare = unknownFC;
	} else if ( flare > unknown104 ) {
		flare = unknown104;
	}
	if ( flare + unknownF8 > length_ ) {
		flare = length_ * 0.35f;
	}
	float middle = length_ - flare - unknownF8;

	float rings[TRACTOR_BEAM_RINGS];
	rings[0] = 0.0f;
	rings[1] = flare * 0.5f;
	rings[2] = flare;
	rings[3] = flare + middle;
	rings[4] = unknownF8 * 0.75f + middle + flare;
	rings[5] = unknownF8 + middle + flare;

	NiPoint3 *vertices = GetShapeData()->GetVertices();
	int shellVertices = numSides * TRACTOR_BEAM_RINGS;	// the core's vertices follow the shell's
	for ( int side = 0; side < numSides; side++ ) {
		for ( int ring = 0; ring < TRACTOR_BEAM_RINGS; ring++ ) {
			int index = numSides * ring + side;
			vertices[index].y = rings[ring];
			vertices[shellVertices + index].y = rings[ring];
		}
	}
	GetShapeData()->GetBound().unknown10 = true;
}

/*
================
TractorBeamShape::VirtualSlot35

Rebuilds the tube for the camera before the base's pass. A beam that
crosses the plane just past the eye is cut there: its near end moves to
where it leaves the view, widened to cover the frustum's edge. The tube
is then turned about its axis to face the eye, its texture rolls with the
view across it, and its sides fade as the beam (or, with unknownE4, each
vertex) is seen end on.
================
*/
// FUNCTION: STBC 0x004f6ea0
// DRAFT: calls and branches as retail's; the frame is 8 bytes larger (retail shares toEye's y with the direction's z and lays nearPlane apart), so the slots differ throughout, and the near plane's dot product runs in another x87 order
void TractorBeamShape::VirtualSlot35( NiCamera *camera ) {
	// cut the beam at the plane just past the eye
	NiPlane nearPlane;
	NiPoint3 ahead = camera->GetWorldDirection() + camera->GetWorldLocation();
	nearPlane.SetConstant( camera->GetWorldDirection() * ahead );
	nearPlane.SetNormal( camera->GetWorldDirection() );
	NiPoint3 start = worldTranslate;
	NiPoint3 end = start + direction * length;
	float startSide = nearPlane.Distance( start );
	float endSide = nearPlane.Distance( end );
	if ( startSide < 0.0f || endSide < 0.0f ) {
		NiPoint3 onPlane = nearPlane.GetNormal() * nearPlane.GetConstant();
		NiPoint3 segment = end - start;
		float t = -( ( nearPlane.GetNormal() * ( start - onPlane ) ) / ( nearPlane.GetNormal() * segment ) );
		NiPoint3 cut = segment * t + start;
		float cutRadius = unknownF8 / camera->GetViewFrustum().nearDistance;
		if ( camera->GetPlane( 2 ).Distance( cut ) > -cutRadius && camera->GetPlane( 3 ).Distance( cut ) > -cutRadius
			 && camera->GetPlane( 4 ).Distance( cut ) > -cutRadius && camera->GetPlane( 5 ).Distance( cut ) > -cutRadius ) {
			const NiFrustum &frustum = camera->GetViewFrustum();
			float side = frustum.bottom;
			if ( side < frustum.right ) {
				side = frustum.right;
			}
			if ( side < frustum.left ) {
				side = frustum.left;
			}
			if ( side < frustum.top ) {
				side = frustum.top;
			}
			float edge = side * frustum.nearDistance;
			NiPoint3 outward = cut - ( camera->GetWorldLocation() + frustum.nearDistance * camera->GetWorldDirection() );
			outward.Unitize();
			NiPoint3 newStart = outward * ( edge + cutRadius );
			newStart += cut;
			NiPoint3 beamDirection;
			if ( startSide < 0.0f ) {
				// the start is behind the eye: it moves out past the cut
				beamDirection = end - newStart;
				SetTranslate( ( newStart - end ) * ( 1.0f / ( 1.0f - t ) ) + end );
			} else {
				beamDirection = newStart - start;
			}
			beamDirection.Unitize();
			NiPoint3 axis = beamDirection.Cross( NiPoint3::UNIT_Y );
			float axisLength = axis.Length();
			if ( axisLength > 0.001f ) {
				axis *= 1.0f / axisLength;
				float angle = NiACos( beamDirection.y );
				NiMatrix3 rotation;
				rotation.MakeRotation( angle, axis );
				SetRotate( rotation );
			} else {
				SetRotate( NiMatrix3::IDENTITY );
			}
		}
	}

	// the world transform, turned about the axis to face the eye
	float scale = localScale;
	NiMatrix3 faceRotate = NiMatrix3::IDENTITY;
	NiPoint3 toCamera = ( 1.0f / scale ) * ( ( camera->GetWorldLocation() - localTranslate ) * localRotate );
	float toCameraLength = sqrtf( toCamera.x * toCamera.x + toCamera.z * toCamera.z );
	if ( toCameraLength >= 1e-12f ) {
		float invLength = 1.0f / toCameraLength;
		toCamera.x *= invLength;
		toCamera.z *= invLength;
		faceRotate.SetCol( 0, toCamera.z, 0.0f, -toCamera.x );
		faceRotate.SetCol( 1, 0.0f, 1.0f, 0.0f );
		faceRotate.SetCol( 2, toCamera.x, 0.0f, toCamera.z );
	}
	worldRotate = localRotate * faceRotate;
	worldTranslate = localTranslate;
	worldScale = scale;

	// the texture rolls with the view across the beam
	NiPoint3 *vertices = modelData->GetVertices();
	NiPoint2 *textures = modelData->GetTextures();
	NiColorA *colors = modelData->GetColors();
	NiPoint3 view = -camera->GetWorldDirection() * localRotate;
	float roll;
	if ( view.z >= 0.0f ) {
		roll = (float)asin( view.x ) / NI_PI * unknown10C;
	} else {
		roll = -( (float)asin( view.x ) / NI_PI * unknown10C );
	}

	// the sides fade as they are seen end on
	if ( unknownE4 == true ) {
		NiPoint3 eye = ( 1.0f / worldScale ) * ( ( camera->GetWorldLocation() - worldTranslate ) * worldRotate );
		int vertexCount = modelData->GetVertexCount();
		for ( int i = 0; i < vertexCount; i++ ) {
			NiPoint3 toEye = eye - vertices[i];
			float distance = toEye.Length();
			float cosine;
			if ( distance > 1e-06f ) {
				cosine = ( 1.0f / distance ) * toEye.y;
			} else {
				cosine = 0.0f;
			}
			float alpha = ( ( unknownEC - unknownE8 ) * (float)fabs( cosine ) + unknownE8 ) * sideAlphas[i];
			colors[i].a = alpha * alpha;
			textures[i].x = vertices[i].y * unknown108 + textureOffset;
			textures[i].y = roll + sideTextureT[i];
		}
	} else {
		int vertexCount = modelData->GetVertexCount();
		for ( int i = 0; i < vertexCount; i++ ) {
			float alpha = ( ( unknownEC - unknownE8 ) * (float)fabs( view.y ) + unknownE8 ) * sideAlphas[i];
			colors[i].a = alpha * alpha;
			textures[i].x = vertices[i].y * unknown108 + textureOffset;
			textures[i].y = roll + sideTextureT[i];
		}
	}
	modelData->IncRevisionID();

	NiTriShape::VirtualSlot35( camera );
}

/*
================
TractorBeamShape::SetTextureTime
================
*/
// FUNCTION: STBC 0x004f7ab0
void TractorBeamShape::SetTextureTime( float time ) {
	textureOffset = time * textureSpeed;
}

/*
================
TractorBeamGlow::TractorBeamGlow

PhaserGlow's quad: the phaser lights texture, an outer square in
innerColor around an inner one in outerColor, drawn additively over
everything and glowing.
================
*/
// FUNCTION: STBC 0x004f7ba0
// DRAFT: inline budget, as PhaserGlow's constructor: the outer corners' NiPoint3 constructor sits one inline level down in retail (unidentified wrapper); one more AttachProperty NiPointer constructor goes out of line there
TractorBeamGlow::TractorBeamGlow( SetClass *set_, ShipClass *ship_, const NiColorA &innerColor, const NiColorA &outerColor ) {
	SetMode( RIGID_FACE_CAMERA );
	set = set_;
	ship = ship_;
	unknown12C = 1.0f;

	NiPoint3 *vertices = new NiPoint3[8];
	vertices[0] = NiPoint3( -0.75f, -0.75f, 0.0f );
	vertices[1] = NiPoint3( 0.75f, -0.75f, 0.0f );
	vertices[2] = NiPoint3( 0.75f, 0.75f, 0.0f );
	vertices[3] = NiPoint3( -0.75f, 0.75f, 0.0f );
	vertices[4] = NiPoint3( -0.5f, -0.5f, 0.0f );
	vertices[5] = NiPoint3( 0.5f, -0.5f, 0.0f );
	vertices[6] = NiPoint3( 0.5f, 0.5f, 0.0f );
	vertices[7] = NiPoint3( -0.5f, 0.5f, 0.0f );

	NiPoint2 *textures = new NiPoint2[8];
	textures[0] = NiPoint2( 0.0f, 0.0f );
	textures[1] = NiPoint2( 0.0f, 1.0f );
	textures[2] = NiPoint2( 1.0f, 1.0f );
	textures[3] = NiPoint2( 1.0f, 0.0f );
	textures[4] = NiPoint2( 0.0f, 0.0f );
	textures[5] = NiPoint2( 0.0f, 1.0f );
	textures[6] = NiPoint2( 1.0f, 1.0f );
	textures[7] = NiPoint2( 1.0f, 0.0f );

	NiColorA *cornerColors = new NiColorA[8];
	cornerColors[0] = innerColor;
	cornerColors[1] = innerColor;
	cornerColors[2] = innerColor;
	cornerColors[3] = innerColor;
	cornerColors[4] = outerColor;
	cornerColors[5] = outerColor;
	cornerColors[6] = outerColor;
	cornerColors[7] = outerColor;
	colors = cornerColors;

	unsigned short *triangles = new unsigned short[12];
	triangles[0] = 0;
	triangles[1] = 1;
	triangles[2] = 2;
	triangles[3] = 0;
	triangles[4] = 2;
	triangles[5] = 3;
	// BUG: the second pair repeats the outer square's triangles, so the
	// inner square (vertices 4-7) is never drawn
	triangles[6] = 0;
	triangles[7] = 1;
	triangles[8] = 2;
	triangles[9] = 0;
	triangles[10] = 2;
	triangles[11] = 3;

	shape = new NiTriShape( 8, vertices, NULL, colors, textures, 4, triangles );

	NiImage *image = g_kImageManager.GetImage( "Data/Textures/Tactical/PhaserLights.tga", true, true );
	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	shape->AttachProperty( texture );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferWrite( false );
	zBuffer->SetZBufferTest( false );
	shape->AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetAlphaTesting( false );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	shape->AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	shape->AttachProperty( textureMode );

	pick.SetFrontOnly( false );
	pick.SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	pick.SetReturnTexture( false );
	pick.SetReturnNormal( false );
	pick.SetReturnSmoothNormal( false );
	pick.SetReturnColor( false );
	pick.SetObserveAppCullFlag( true );
	pick.SetSortType( NiPick::NO_SORT );
	pick.SetPickType( NiPick::FIND_FIRST );

	GlowPass *glowPass = new GlowPass;
	glowPass->unknown14 = false;
	shape->AddExtraData( glowPass );
	AttachChild( shape, false );
}

// how much a glow brightens (or, hidden, dims) at each draw
#define GLOW_FADE_STEP		0.15f

/*
================
TractorBeamGlow::VirtualSlot35

While the glow is in front of the camera, brightens it a step when a ray
from the eye reaches it (picked against the ship's model, then against
the set's other objects, backdrops aside) and dims it a step when
something hides it.
================
*/
// FUNCTION: STBC 0x004f82e0
void TractorBeamGlow::VirtualSlot35( NiCamera *camera ) {
	const NiPoint3 &eye = camera->GetWorldLocation();
	NiPoint3 toGlow = GetWorldTranslate() - eye;
	if ( camera->GetWorldDirection() * toGlow > 0.0f ) {
		float step = -GLOW_FADE_STEP;
		NiPick::Record *record;
		if ( ship->VirtualSlot63( &eye, toGlow, &pick ) && ( record = pick.GetRecord( 0 ) )->GetDistance() < 1.0f ) {
			step = GLOW_FADE_STEP;
		} else {
			set->StartPick( (NiPoint3 *)&eye, &toGlow, true );
			ObjectClass *object = set->GetPickedObject( &record );
			if ( object != NULL ) {
				while ( true ) {
					if ( object != ship && !object->IsTypeOf( BACKDROP_TYPE ) && object->VirtualSlot63( &eye, toGlow, &pick ) ) {
						record = pick.GetRecord( 0 );
						if ( record->GetDistance() < 1.0f ) {
							step = GLOW_FADE_STEP;
							break;
						}
					}
					object = set->GetPickedObject();
					if ( object == NULL ) {
						break;
					}
				}
			}
		}

		unknown12C += step;
		if ( unknown12C > 1.0f ) {
			unknown12C = 1.0f;
		} else if ( unknown12C < 0.0f ) {
			unknown12C = 0.0f;
		}

		for ( int i = 0; i < 8; i++ ) {
			colors[i].a = 1.0f - unknown12C;
		}
		shape->GetShapeData()->IncRevisionID();
	}
	NiBillboardNode::VirtualSlot35( camera );
}

/*
================
TractorBeamGraphic::TractorBeamGraphic

Builds the beam, a light for each end and the glow, all in the tractor
beam's blue.
================
*/
// FUNCTION: STBC 0x004f84c0
// PARKED: as PhaserGraphic's constructor: retail pushes the property's beam width through fld/fstp in the beam's argument list, this build copies it with integer moves; the colour locals' frame slots follow from it
TractorBeamGraphic::TractorBeamGraphic( TractorBeamProperty *property, ShipClass *ship_ ) : ship( ship_ ) {
	NiColorA color( 0.4f, 0.4f, 1.0f, 1.0f );
	AttachChild( new TractorBeamShape( color, color, color, color, 12, property->GetTractorBeamWidth() * 0.25f, 0.0f, 0.45f,
		0.2f, 1.0f, 5.0f, property->GetTractorBeamWidth(), 0.25f, 1.0f, 0.2f, "data/Textures/Tactical/TractorBeam.tga" ), false );

	NiColor lightColor( color.r, color.g, color.b );
	width = property->GetTractorBeamWidth();

	NiPointLight *light = new NiPointLight;
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( lightColor );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( 0.9f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetDimmer( 1.0f );
	AttachChild( light, false );

	light = new NiPointLight;
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( lightColor );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( 0.9f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetDimmer( 1.0f );
	AttachChild( light, false );

	AttachChild( new TractorBeamGlow( ship_->GetContainingSet(), ship_, color * 0.06f, color ), false );
}

/*
================
TractorBeamGraphic::GetEndLight
================
*/
// FUNCTION: STBC 0x004f8860
NiPointLight *TractorBeamGraphic::GetEndLight() {
	return (NiPointLight *)GetAt( 2 );
}

/*
================
TractorBeamGraphic::GetStartLight
================
*/
// FUNCTION: STBC 0x004f8870
NiPointLight *TractorBeamGraphic::GetStartLight() {
	return (NiPointLight *)GetAt( 1 );
}

/*
================
TractorBeamGraphic::GetGlow
================
*/
// FUNCTION: STBC 0x004f8880
TractorBeamGlow *TractorBeamGraphic::GetGlow() {
	return (TractorBeamGlow *)GetAt( 3 );
}

/*
================
TractorBeamGraphic::GetBeam
================
*/
// FUNCTION: STBC 0x004f8890
TractorBeamShape *TractorBeamGraphic::GetBeam() {
	return (TractorBeamShape *)GetAt( 0 );
}

/*
================
TractorBeamGraphic::Start

Shows the beam with both lights off, puts the start light on the ship and
the glow on the ship's set; a negative duration is taken as none.
================
*/
// FUNCTION: STBC 0x004f88a0
void TractorBeamGraphic::Start( float time, float duration_ ) {
	GetEndLight()->SetSwitch( false );
	GetStartLight()->SetSwitch( false );
	SetAppCulled( false );
	GetStartLight()->SetTranslate( NiPoint3::ZERO );
	GetGlow()->SetTranslate( NiPoint3::ZERO );
	GetGlow()->SetUnknown12C( 1.0f );
	GetGlow()->SetSet( ship->GetContainingSet() );
	ship->AddLight( GetStartLight() );
	startTime = time;
	duration = duration_;
	if ( duration < 0.0f ) {
		duration = 0.0f;
	}
}

/*
================
TractorBeamGraphic::Update

Places the beam at position along direction: it grows to its full length
over the duration, with the end light at its tip.
================
*/
// FUNCTION: STBC 0x004f8990
void TractorBeamGraphic::Update( const NiPoint3 &position, const NiPoint3 &direction, float length, float time ) {
	float fraction = ( time - startTime ) / duration;
	if ( fraction < 0.0f ) {
		fraction = 0.0f;
	} else if ( fraction > 1.0f ) {
		fraction = 1.0f;
	}

	TractorBeamShape *beam = GetBeam();
	beam->SetDirection( direction, fraction * length );
	beam->SetTranslate( position );
	beam->SetTextureTime( time - startTime );

	GetStartLight()->SetTranslate( position );
	if ( !GetStartLight()->GetSwitch() ) {
		GetStartLight()->SetSwitch( true );
	}

	GetEndLight()->SetTranslate( fraction * direction * ( length - width ) + position );
	if ( !GetEndLight()->GetSwitch() ) {
		GetEndLight()->SetSwitch( true );
	}
}

/*
================
TractorBeamGraphic::Stop
================
*/
// FUNCTION: STBC 0x004f8b30
void TractorBeamGraphic::Stop() {
	ship->RemoveLight( GetStartLight() );
	SetAppCulled( true );
}
