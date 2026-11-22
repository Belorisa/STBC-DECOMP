/*
===============================================================================

	PhaserGraphic.cpp

===============================================================================
*/

#include <iostream>
#include "PhaserGraphic.h"
#include "PhaserProperty.h"
#include "ShipClass.h"
#include "UtopiaApp.h"
#include "NiPointLight.h"
#include "NiMath.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "Backdrop.h"
#include "SetClass.h"
#include "GlowPass.h"
#include "ImageManagerClass.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiFogProperty.h"
#include "TGModelUtils.h"
#include <math.h>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004e4000 _$E45
// FUNCTION: STBC 0x004e4010 _$E44
// FUNCTION: STBC 0x004e4020 _$E273
// FUNCTION: STBC 0x004e4030 _$E272
// FUNCTION: STBC 0x004e5fe0 _$E56
// FUNCTION: STBC 0x004e5ff0 _$E55
// FUNCTION: STBC 0x004e6000 _$E60
// FUNCTION: STBC 0x004e6010 _$E59
// FUNCTION: STBC 0x004e6760 _$E64
// FUNCTION: STBC 0x004e6770 _$E63
// FUNCTION: STBC 0x004e6780 _$E68
// FUNCTION: STBC 0x004e6790 _$E67
// FUNCTION: STBC 0x004e6a90 _$E192
// FUNCTION: STBC 0x004e6aa0 _$E191
// FUNCTION: STBC 0x004e7b30 _$E134
// FUNCTION: STBC 0x004e7b40 _$E133
// FUNCTION: STBC 0x004e7b50 _$E147
// FUNCTION: STBC 0x004e7b60 _$E146
// FUNCTION: STBC 0x004e7b70 _$E197
// FUNCTION: STBC 0x004e7b80 _$E196
// FUNCTION: STBC 0x004e7b90 _$E278
// FUNCTION: STBC 0x004e7ba0 _$E277
// FUNCTION: STBC 0x004e7bb0 _$E201
// FUNCTION: STBC 0x004e7bc0 _$E200
// FUNCTION: STBC 0x004e7bd0 _$E282
// FUNCTION: STBC 0x004e7be0 _$E281
// FUNCTION: STBC 0x004e7bf0 _$E289
// FUNCTION: STBC 0x004e7c00 _$E288
// FUNCTION: STBC 0x004e7c10 _$E312
// FUNCTION: STBC 0x004e7c20 _$E311
// FUNCTION: STBC 0x004e7c30 _$E316
// FUNCTION: STBC 0x004e7c40 _$E315
// FUNCTION: STBC 0x004e7c50 _$E92
// FUNCTION: STBC 0x004e7c60 _$E91
// FUNCTION: STBC 0x004e7c70 _$E209
// FUNCTION: STBC 0x004e7c80 _$E208
// FUNCTION: STBC 0x004e7c90 _$E216
// FUNCTION: STBC 0x004e7ca0 _$E215
// FUNCTION: STBC 0x004e7cb0 _$E220
// FUNCTION: STBC 0x004e7cc0 _$E219
// FUNCTION: STBC 0x004e7cd0 _$E224
// FUNCTION: STBC 0x004e7ce0 _$E223
// FUNCTION: STBC 0x004e7cf0 _$E228
// FUNCTION: STBC 0x004e7d00 _$E227
// FUNCTION: STBC 0x004e7d30 _$E234
// FUNCTION: STBC 0x004e7d40 _$E233
// FUNCTION: STBC 0x004e7d50 _$E298
// FUNCTION: STBC 0x004e7d60 _$E297
// FUNCTION: STBC 0x004e7d70 _$E320
// FUNCTION: STBC 0x004e7d80 _$E319
// FUNCTION: STBC 0x004e7d90 _$E324
// FUNCTION: STBC 0x004e7da0 _$E323
// FUNCTION: STBC 0x004e7db0 _$E328
// FUNCTION: STBC 0x004e7dc0 _$E327
// FUNCTION: STBC 0x004e7dd0 _$E332
// FUNCTION: STBC 0x004e7de0 _$E331
// FUNCTION: STBC 0x004e7df0 _$E336
// FUNCTION: STBC 0x004e7e00 _$E335
// FUNCTION: STBC 0x004e7e10 _$E238
// FUNCTION: STBC 0x004e7e20 _$E237
// FUNCTION: STBC 0x004e7e30 _$E340
// FUNCTION: STBC 0x004e7e40 _$E339
// FUNCTION: STBC 0x004e7e50 _$E344
// FUNCTION: STBC 0x004e7e60 _$E343
// FUNCTION: STBC 0x004e7e70 _$E348
// FUNCTION: STBC 0x004e7e80 _$E347
// FUNCTION: STBC 0x004e7e90 _$E97
// FUNCTION: STBC 0x004e7ea0 _$E96
// FUNCTION: STBC 0x004e7eb0 _$E352
// FUNCTION: STBC 0x004e7ec0 _$E351
// FUNCTION: STBC 0x004e7ed0 _$E102
// FUNCTION: STBC 0x004e7ee0 _$E101
// FUNCTION: STBC 0x004e7ef0 _$E356
// FUNCTION: STBC 0x004e7f00 _$E355
// FUNCTION: STBC 0x004e7f10 _$E360
// FUNCTION: STBC 0x004e7f20 _$E359
// FUNCTION: STBC 0x004e7f30 _$E364
// FUNCTION: STBC 0x004e7f40 _$E363
// FUNCTION: STBC 0x004e7f50 _$E368
// FUNCTION: STBC 0x004e7f60 _$E367
// FUNCTION: STBC 0x004e7f70 _$E111
// FUNCTION: STBC 0x004e7f80 _$E110
// FUNCTION: STBC 0x004e7f90 _$E372
// FUNCTION: STBC 0x004e7fa0 _$E371
// FUNCTION: STBC 0x004e7fb0 _$E376
// FUNCTION: STBC 0x004e7fc0 _$E375
// FUNCTION: STBC 0x004e7fd0 _$E380
// FUNCTION: STBC 0x004e7fe0 _$E379
// FUNCTION: STBC 0x004e7ff0 _$E117
// FUNCTION: STBC 0x004e8000 _$E116
// FUNCTION: STBC 0x004e8010 _$E122
// FUNCTION: STBC 0x004e8020 _$E121
// FUNCTION: STBC 0x004e8030 _$E152
// FUNCTION: STBC 0x004e8040 _$E151
// FUNCTION: STBC 0x004e8050 _$E302
// FUNCTION: STBC 0x004e8060 _$E301
// FUNCTION: STBC 0x004e8070 _$E157
// FUNCTION: STBC 0x004e8080 _$E156
// FUNCTION: STBC 0x004e8090 _$E384
// FUNCTION: STBC 0x004e80a0 _$E383
// FUNCTION: STBC 0x004e80b0 _$E388
// FUNCTION: STBC 0x004e80c0 _$E387
// FUNCTION: STBC 0x004e80d0 _$E162
// FUNCTION: STBC 0x004e80e0 _$E161
// FUNCTION: STBC 0x004e80f0 _$E392
// FUNCTION: STBC 0x004e8100 _$E391
// FUNCTION: STBC 0x004e8110 _$E127
// FUNCTION: STBC 0x004e8120 _$E126
// FUNCTION: STBC 0x004e8130 _$E396
// FUNCTION: STBC 0x004e8140 _$E395
// FUNCTION: STBC 0x004e8150 _$E400
// FUNCTION: STBC 0x004e8160 _$E399
// FUNCTION: STBC 0x004e8170 _$E167
// FUNCTION: STBC 0x004e8180 _$E166
// FUNCTION: STBC 0x004e8190 _$E172
// FUNCTION: STBC 0x004e81a0 _$E171
// FUNCTION: STBC 0x004e81b0 _$E404
// FUNCTION: STBC 0x004e81c0 _$E403
// FUNCTION: STBC 0x004e81d0 _$E408
// FUNCTION: STBC 0x004e81e0 _$E407
// FUNCTION: STBC 0x004e81f0 _$E412
// FUNCTION: STBC 0x004e8200 _$E411
// FUNCTION: STBC 0x004e8210 _$E416
// FUNCTION: STBC 0x004e8220 _$E415
// FUNCTION: STBC 0x004e8230 _$E420
// FUNCTION: STBC 0x004e8240 _$E419
// FUNCTION: STBC 0x004e8250 _$E424
// FUNCTION: STBC 0x004e8260 _$E423
// FUNCTION: STBC 0x004e8270 _$E428
// FUNCTION: STBC 0x004e8280 _$E427
// FUNCTION: STBC 0x004e8290 _$E432
// FUNCTION: STBC 0x004e82a0 _$E431
// FUNCTION: STBC 0x004e82b0 _$E177
// FUNCTION: STBC 0x004e82c0 _$E176
// FUNCTION: STBC 0x004e82d0 _$E436
// FUNCTION: STBC 0x004e82e0 _$E435
// FUNCTION: STBC 0x004e82f0 _$E182
// FUNCTION: STBC 0x004e8300 _$E181
// FUNCTION: STBC 0x004e8310 _$E187
// FUNCTION: STBC 0x004e8320 _$E186
// FUNCTION: STBC 0x004e8360 _$E440
// FUNCTION: STBC 0x004e8370 _$E439
// FUNCTION: STBC 0x004e8380 _$E445
// FUNCTION: STBC 0x004e8390 _$E444
// FUNCTION: STBC 0x004e83a0 _$E449
// FUNCTION: STBC 0x004e83b0 _$E448

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004e3f80 _$E32
// FUNCTION: STBC 0x004e3f90 _$E29
// FUNCTION: STBC 0x004e3fa0 _$E31
// FUNCTION: STBC 0x004e3fb0 _$E30
// FUNCTION: STBC 0x004e3fc0 _$E38
// FUNCTION: STBC 0x004e3fd0 _$E35
// FUNCTION: STBC 0x004e3fe0 _$E37
// FUNCTION: STBC 0x004e3ff0 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004e8330 _$E477
// FUNCTION: STBC 0x004e8350 _$E476

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004e4040 _$E464
// FUNCTION: STBC 0x004e4050 _$E460
// FUNCTION: STBC 0x004e4070 _$E463
// FUNCTION: STBC 0x004e4080 _$E461
// DATA: STBC 0x986368 ?rtti@PhaserGraphic@@2VNiRTTI@@B
const NiRTTI PhaserGraphic::rtti( "PhaserGraphic", &NiNode::rtti );

/*
================
PhaserBeam::PhaserBeam

Two tubes, the shell and the core, each six rings of numSides vertices
around half a turn, with the vertices left at the origin for
VirtualSlot35 to place. The colours run from the outer colours at the edges
to the inner ones in the middle; the texture's t coordinate runs across
the sides, perimeterTile times.
================
*/
// FUNCTION: STBC 0x004e40a0
// DRAFT: register picks (this in edi, retail ebx; numSides_ in ebp, retail edi) and frame slots differ from the start (retail's frame 4 bytes larger); calls and statement order agree
PhaserBeam::PhaserBeam( const NiColorA &outerShellColor, const NiColorA &innerShellColor, const NiColorA &outerCoreColor, const NiColorA &innerCoreColor,
						int numSides_, float mainRadius_, float taperRadius_, float coreScale_, float taperRatio_,
						float taperMinLength_, float taperMaxLength_, float lengthTextureTilePerUnit_, float perimeterTile_,
						float textureSpeed_, char *textureName )
	: NiTriShape( numSides_ * PHASER_BEAM_RINGS * 2, new NiPoint3[numSides_ * PHASER_BEAM_RINGS * 2], NULL, new NiColorA[numSides_ * PHASER_BEAM_RINGS * 2],
				  new NiPoint2[numSides_ * PHASER_BEAM_RINGS * 2], ( numSides_ - 1 ) * ( PHASER_BEAM_RINGS - 1 ) * 4,
				  new unsigned short[( numSides_ - 1 ) * ( PHASER_BEAM_RINGS - 1 ) * 12] ),
	  unknownE4( 0.7f ), unknownE8( 1.0f ), mainRadius( mainRadius_ ), taperRadius( taperRadius_ ), coreScale( coreScale_ ),
	  taperMinLength( taperMinLength_ ), taperRatio( taperRatio_ ), taperMaxLength( taperMaxLength_ ),
	  lengthTextureTilePerUnit( lengthTextureTilePerUnit_ ), perimeterTile( perimeterTile_ ), textureSpeed( textureSpeed_ ),
	  numSides( numSides_ ), sideAlphas( NULL ), sideTextureT( NULL ), sideDirections( new NiPoint3[numSides_] ), textureOffset( 0.0f ) {
	int i;
	int ring;

	// the sides' directions around half a turn of the beam's axis
	float step = NI_PI / ( numSides - 1 );
	NiTriShapeData *data = GetShapeData();
	NiPoint3 *vertices = data->GetVertices();
	NiColorA *colors = data->GetColors();
	unsigned short *triangles = data->GetTriList();
	int shellVertices = numSides_ * PHASER_BEAM_RINGS;	// the core's vertices follow the shell's
	sideAlphas = new float[numSides_ * PHASER_BEAM_RINGS * 2];
	for ( i = 0; i < numSides; i++ ) {
		float angle = i * step;
		float cosine = (float)cos( angle );
		float alpha = (float)sin( angle );
		sideDirections[i] = NiPoint3( cosine, 0.0f, alpha );
		for ( ring = 0; ring < PHASER_BEAM_RINGS; ring++ ) {
			vertices[ring * numSides_ + i] = NiPoint3( 0.0f, 0.0f, 0.0f );
			sideAlphas[ring * numSides_ + i] = alpha;
			vertices[shellVertices + ring * numSides_ + i] = NiPoint3( 0.0f, 0.0f, 0.0f );
			sideAlphas[shellVertices + ring * numSides_ + i] = alpha;
		}
	}

	// the colours, from the outer ones at the edges to the inner ones in
	// the middle side
	NiColorA shellStep = ( innerShellColor - outerShellColor ) / (float)( numSides_ / 2 - 1 );
	NiColorA coreStep = ( innerCoreColor - outerCoreColor ) / (float)( numSides_ / 2 - 1 );
	int halfSides = numSides_ / 2;
	for ( i = 0; i < halfSides; i++ ) {
		NiColorA shellColor = outerShellColor + shellStep * (float)i;
		NiColorA coreColor = outerCoreColor + coreStep * (float)i;
		for ( ring = 0; ring < PHASER_BEAM_RINGS; ring++ ) {
			colors[ring * numSides_ + i] = shellColor;
			colors[shellVertices + ring * numSides_ + i] = coreColor;
			colors[( ring + 1 ) * numSides_ - 1 - i] = shellColor;
			colors[shellVertices + ( ring + 1 ) * numSides_ - 1 - i] = coreColor;
		}
	}
	if ( numSides_ % 2 != 0 ) {
		for ( ring = 0; ring < PHASER_BEAM_RINGS; ring++ ) {
			// BUG: one side past the middle one, which the loop above has
			// already coloured: the middle side keeps no colour
			colors[ring * numSides_ + halfSides + 1].r = innerShellColor.r;
			colors[ring * numSides_ + halfSides + 1].g = innerShellColor.g;
			colors[ring * numSides_ + halfSides + 1].b = innerShellColor.b;
			colors[shellVertices + ring * numSides_ + halfSides + 1].r = innerCoreColor.r;
			colors[shellVertices + ring * numSides_ + halfSides + 1].g = innerCoreColor.g;
			colors[shellVertices + ring * numSides_ + halfSides + 1].b = innerCoreColor.b;
		}
	}

	// two triangles between each pair of rings and sides, the shell's then
	// the core's
	int index = 0;
	for ( i = 0; i < numSides_ - 1; i++ ) {
		for ( ring = 0; ring < PHASER_BEAM_RINGS - 1; ring++ ) {
			int a = ring * numSides_ + i;
			int b = ( ring + 1 ) * numSides_ + i;
			triangles[index++] = a;
			triangles[index++] = b + 1;
			triangles[index++] = a + 1;
			triangles[index++] = b;
			triangles[index++] = b + 1;
			triangles[index++] = a;
		}
	}
	for ( i = 0; i < numSides_ - 1; i++ ) {
		for ( ring = 0; ring < PHASER_BEAM_RINGS - 1; ring++ ) {
			int a = ring * numSides_ + shellVertices + i;
			int b = ( ring + 1 ) * numSides_ + shellVertices + i;
			triangles[index++] = a;
			triangles[index++] = b + 1;
			triangles[index++] = a + 1;
			triangles[index++] = b;
			triangles[index++] = b + 1;
			triangles[index++] = a;
		}
	}

	// the texture's t coordinate across the sides
	sideTextureT = new float[numSides_ * PHASER_BEAM_RINGS * 2];
	for ( i = 0; i < numSides_; i++ ) {
		float t = (float)i / ( numSides_ - 1 );
		for ( ring = 0; ring < PHASER_BEAM_RINGS; ring++ ) {
			sideTextureT[ring * numSides_ + i] = t * perimeterTile;
			sideTextureT[shellVertices + ring * numSides_ + i] = t * perimeterTile;
		}
	}

	NiBound &bound = GetShapeData()->GetBound();
	bound.center.x = 0.0f;
	bound.center.y = 0.0f;
	bound.center.z = 0.0f;
	bound.radius = mainRadius;

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
PhaserBeam::~PhaserBeam
================
*/
// FUNCTION: STBC 0x004e4e70
PhaserBeam::~PhaserBeam() {
	delete [] sideAlphas;
	delete [] sideTextureT;
	delete [] sideDirections;
}

/*
================
PhaserBeam::SetDirection

Turns the beam from its rest direction, +y, to direction, and sizes its
model bound for length.
================
*/
// FUNCTION: STBC 0x004e4eb0
void PhaserBeam::SetDirection( const NiPoint3 &direction_, float length_, bool unknown ) {
	direction = direction_;
	length = length_;
	unknown110 = unknown;

	NiPoint3 axis = direction_.Cross( NiPoint3::UNIT_Y );
	axis.Unitize();
	float angle = NiACos( direction_.y );
	NiMatrix3 rotation;
	rotation.MakeRotation( angle, axis );
	SetRotate( rotation );

	NiBound &bound = GetShapeData()->GetBound();
	bound.center = NiPoint3( 0.0f, length * 0.5f, 0.0f );
	bound.radius = length * ( 1.0f / 2.05f );
	bound.unknown10 = false;
	UpdateWorldBound();
}

/*
================
GetDistanceScale

How much wider a beam end is drawn for its distance from the eye: half
width up close, growing a tenth per unit to full width at 10 units, then
a hundredth per unit past 100, at most four times (invented name).
================
*/
static inline float GetDistanceScale( float distance ) {
	float scale = 1.0f;
	if ( distance < 10.0f ) {
		scale = distance / 10.0f;
		if ( scale < 0.5f ) {
			scale = 0.5f;
		}
	} else if ( distance > 100.0f ) {
		scale = distance / 100.0f;
		if ( scale > 4.0f ) {
			scale = 4.0f;
		}
	}
	return scale;
}

/*
================
PhaserBeam::VirtualSlot35

Rebuilds the tube for the camera before the base's pass. The beam is six
rings along its length: the taper at each end and the body between. Each
end's radius grows with its distance from the eye, so the beam keeps its
width on screen. A beam that crosses the plane just past the eye is cut
there: its near end moves to where it leaves the view, widened to cover
the frustum's edge. The tube is then turned about its axis to face the
eye, and its texture rolls with the view across it.
================
*/
// FUNCTION: STBC 0x004e5040
// DRAFT: inline budget: the eye is TGModelUtils::WorldToLocalPoint (retail's out-of-line 0x4695F0/0x41CBD0 pair); retail expands the end distances and the cut's vectors with no constructor calls, and dots planes 4 and 5 inline; this build does the reverse (6 extra NiPoint3 constructor calls); the end distances' temporary is unproven
void PhaserBeam::VirtualSlot35( NiCamera *camera ) {
	// the rings along the beam
	float taper = ( length * 0.5f ) * taperRatio;
	if ( taper < taperMinLength ) {
		taper = taperMinLength;
	} else if ( taper > taperMaxLength ) {
		taper = taperMaxLength;
	}
	float rings[6];
	if ( unknown110 ) {
		if ( taper > length ) {
			taper = length * 0.8f;
		}
		rings[0] = 0.0f;
		rings[1] = taper * 0.5f;
		rings[2] = taper;
		rings[3] = ( length - taper ) + taper;
		rings[4] = rings[3];
		rings[5] = rings[3];
	} else {
		if ( taper + taper > length ) {
			taper = length * 0.35f;
		}
		float body = length - ( taper + taper );
		rings[0] = 0.0f;
		rings[1] = taper * 0.5f;
		rings[2] = taper;
		rings[3] = body + taper;
		rings[4] = taper * 1.5f + body;
		rings[5] = ( taper + taper ) + body;
	}

	// each end's radius for its distance from the eye
	float startRadius = mainRadius;
	float endRadius = mainRadius;
	NiPoint3 eye = TGModelUtils::WorldToLocalPoint( this, camera->GetWorldLocation() );
	float startDistance = NiPoint3( eye.x, eye.y - rings[2], eye.z ).Length();
	float endDistance = NiPoint3( eye.x, eye.y - rings[3], eye.z ).Length();
	startRadius *= GetDistanceScale( startDistance );
	endRadius *= GetDistanceScale( endDistance );

	// cut the beam at the plane just past the eye
	NiNode *parentNode = GetParent();
	NiPlane nearPlane;
	nearPlane.SetConstant( ( camera->GetWorldDirection() + camera->GetWorldLocation() ) * camera->GetWorldDirection() );
	nearPlane.SetNormal( camera->GetWorldDirection() );
	NiPoint3 start = parentNode->GetWorldTranslate();
	NiPoint3 end = start + length * direction;
	float startSide = nearPlane.Distance( start );
	float endSide = nearPlane.Distance( end );
	NiMatrix3 restRotate( localRotate );
	if ( startSide < 0.0f || endSide < 0.0f ) {
		NiPoint3 onPlane = nearPlane.GetNormal() * nearPlane.GetConstant();
		NiPoint3 segment = end - start;
		float t = -( ( nearPlane.GetNormal() * ( start - onPlane ) ) / ( nearPlane.GetNormal() * segment ) );
		NiPoint3 cut = segment * t + start;
		float cutRadius = ( endRadius - startRadius ) * t + startRadius;
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
			NiPoint3 newStart = cut + outward * ( edge + cutRadius );
			NiPoint3 beamDirection;
			if ( startSide < 0.0f ) {
				// the start is behind the eye: it moves to the cut
				beamDirection = end - newStart;
				beamDirection.Unitize();
				SetTranslate( ( end - beamDirection * length ) - parentNode->GetWorldTranslate() );
			} else {
				beamDirection = newStart - start;
				beamDirection.Unitize();
			}
			NiPoint3 axis = beamDirection.Cross( NiPoint3::UNIT_Y );
			axis.Unitize();
			float angle = NiACos( beamDirection.y );
			NiMatrix3 rotation;
			rotation.MakeRotation( angle, axis );
			SetRotate( rotation );
		}
	}

	// the world transform, turned about the axis to face the eye
	NiMatrix3 parentRotate;
	NiPoint3 parentTranslate;
	float parentScale;
	if ( parentNode ) {
		parentRotate = parentNode->GetWorldRotate();
		parentTranslate = parentNode->GetWorldTranslate();
		parentScale = parentNode->GetWorldScale();
	} else {
		parentRotate = NiMatrix3::IDENTITY;
		parentTranslate = NiPoint3::ZERO;
		parentScale = 1.0f;
	}
	NiMatrix3 rotate = parentRotate * localRotate;
	NiPoint3 translate = parentTranslate + parentScale * ( parentRotate * localTranslate );
	float scale = parentScale * localScale;
	NiMatrix3 faceRotate = NiMatrix3::IDENTITY;
	NiPoint3 toCamera = ( 1.0f / scale ) * ( ( camera->GetWorldLocation() - translate ) * rotate );
	float toCameraLength = sqrtf( toCamera.x * toCamera.x + toCamera.z * toCamera.z );
	if ( toCameraLength >= 1e-12f ) {
		float invLength = 1.0f / toCameraLength;
		toCamera.x *= invLength;
		toCamera.z *= invLength;
		faceRotate.SetCol( 0, toCamera.z, 0.0f, -toCamera.x );
		faceRotate.SetCol( 1, 0.0f, 1.0f, 0.0f );
		faceRotate.SetCol( 2, toCamera.x, 0.0f, toCamera.z );
	}
	worldRotate = rotate * faceRotate;
	worldTranslate = translate;
	worldScale = scale;
	UpdateWorldBound();

	// the texture rolls with the view across the beam
	NiPoint3 view = -camera->GetWorldDirection() * rotate;
	float roll;
	if ( view.z >= 0.0f ) {
		roll = (float)asin( view.x ) / NI_PI * perimeterTile;
	} else {
		roll = -( (float)asin( view.x ) / NI_PI * perimeterTile );
	}

	// the tube's vertices: the shell, then the core inside it
	float radii[6];
	radii[0] = taperRadius;
	radii[1] = startRadius - ( startRadius - taperRadius ) * 0.25f;
	radii[2] = startRadius;
	radii[3] = endRadius;
	radii[4] = endRadius - ( endRadius - taperRadius ) * 0.25f;
	radii[5] = taperRadius;
	NiPoint3 *vertices = modelData->GetVertices();
	NiPoint2 *textures = modelData->GetTextures();
	NiColorA *colors = modelData->GetColors();
	int coreOffset = numSides * 6;
	for ( int i = 0; i < numSides; i++ ) {
		for ( int ring = 0; ring < 6; ring++ ) {
			float x = radii[ring] * sideDirections[i].x;
			float z = sideDirections[i].z * radii[ring];
			int index = ring * numSides + i;
			vertices[index].x = x;
			vertices[index].y = rings[ring];
			vertices[index].z = z;
			vertices[index + coreOffset].x = x * coreScale;
			vertices[index + coreOffset].y = rings[ring];
			vertices[index + coreOffset].z = z * coreScale;
		}
	}

	// the edges fade as the beam is seen end on
	int vertexCount = modelData->GetVertexCount();
	float alpha = ( unknownE8 - unknownE4 ) * NiAbs( view.y ) + unknownE4;
	for ( int j = 0; j < vertexCount; j++ ) {
		float sideAlpha = alpha * sideAlphas[j];
		colors[j].a = sideAlpha * sideAlpha;
		textures[j].x = vertices[j].y * lengthTextureTilePerUnit + textureOffset;
		textures[j].y = roll + sideTextureT[j];
	}
	modelData->IncRevisionID();

	NiTriShape::VirtualSlot35( camera );
	SetTranslate( NiPoint3::ZERO );
	SetRotate( restRotate );
}

/*
================
PhaserBeam::ResetTextureOffset
================
*/
// FUNCTION: STBC 0x004e6020
void PhaserBeam::ResetTextureOffset() {
	textureOffset = 0.0f;
}

/*
================
PhaserBeam::ScrollTexture
================
*/
// FUNCTION: STBC 0x004e6030
void PhaserBeam::ScrollTexture( float deltaTime ) {
	textureOffset += deltaTime * textureSpeed;
}

/*
================
PhaserGlow::UpdateDownwardPass
================
*/
// FUNCTION: STBC 0x004e6060
void PhaserGlow::UpdateDownwardPass( float time ) {
	NiBillboardNode::UpdateDownwardPass( time );
	if ( reset ) {
		elapsedTime = 0.0f;
		lastTime = time;
	} else {
		elapsedTime += time - lastTime;
		lastTime = time;
	}
}

/*
================
PhaserGlow::PhaserGlow

A camera-facing quad of the phaser lights texture: an outer square in
innerColor around an inner one in outerColor, drawn additively over
everything (no depth test) and glowing; VirtualSlot35 hides it when
something is between the camera and the glow.
================
*/
// FUNCTION: STBC 0x004e60c0
// DRAFT: inline budget: the call set is retail's except the outer corners, whose NiPoint3 constructor (0x414540) retail calls into a temporary copied inline: one inline level below this body (a by-value wrapper, unidentified), where the nested share is under its cost 47; registers differ
PhaserGlow::PhaserGlow( SetClass *set_, ShipClass *ship_, float scale, const NiColorA &innerColor, const NiColorA &outerColor ) {
	reset = true;
	elapsedTime = 0.0f;
	lastTime = 0.0f;
	SetMode( RIGID_FACE_CAMERA );
	set = set_;
	ship = ship_;
	unknown130 = 1.0f;

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
	triangles[6] = 4;
	triangles[7] = 5;
	triangles[8] = 6;
	triangles[9] = 4;
	triangles[10] = 6;
	triangles[11] = 7;

	NiTriShape *glowShape = new NiTriShape( 8, vertices, NULL, cornerColors, textures, 4, triangles );
	shape = glowShape;

	NiImage *image = g_kImageManager.GetImage( "Data/Textures/Tactical/PhaserLights.tga", true, true );
	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	glowShape->AttachProperty( texture );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferWrite( false );
	zBuffer->SetZBufferTest( false );
	glowShape->AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetAlphaTesting( false );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	glowShape->AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	glowShape->AttachProperty( vertexColor );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	glowShape->AttachProperty( textureMode );

	pick.SetFrontOnly( false );
	pick.SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	pick.SetCoordinateType( NiPick::WORLD_COORDINATES );
	pick.SetReturnTexture( false );
	pick.SetReturnNormal( false );
	pick.SetReturnColor( false );
	pick.SetObserveAppCullFlag( true );
	pick.SetSortType( NiPick::SORT );
	pick.SetPickType( NiPick::FIND_FIRST );

	GlowPass *glowPass = new GlowPass;
	glowPass->unknown14 = false;
	glowShape->AddExtraData( glowPass );
	AttachChild( glowShape, false );
}

// the shortest time between two visibility tests of a glow, in seconds
#define GLOW_UPDATE_INTERVAL	( 1.0f / 15.0f )
// how fast a glow fades in or out, in full brightness a second
#define GLOW_FADE_RATE			5.0f

/*
================
PhaserGlow::VirtualSlot35

Fades the glow in while the beam's end is in front of the camera and
nothing hides it, out otherwise, at most GLOW_UPDATE_INTERVAL apart: a
ray from the eye to the glow is picked against the ship's model, then
against the set's other objects (backdrops aside); a hit closer than the
glow hides it. A reset glow starts fully lit or dark.
================
*/
// FUNCTION: STBC 0x004e6810
void PhaserGlow::VirtualSlot35( NiCamera *camera ) {
	if ( elapsedTime > GLOW_UPDATE_INTERVAL || reset ) {
		const NiPoint3 &eye = camera->GetWorldLocation();
		NiPoint3 toGlow = GetWorldTranslate() - eye;
		if ( toGlow * camera->GetWorldDirection() > 0.0f ) {
			float visibility = -1.0f;
			NiPick::Record *record;
			if ( ship->VirtualSlot63( &eye, toGlow, &pick ) ) {
				record = pick.GetRecord( 0 );
				if ( record->GetDistance() < 1.0f ) {
					visibility = 1.0f;
				}
			} else {
				set->StartPick( (NiPoint3 *)&eye, &toGlow, true );
				ObjectClass *object = set->GetPickedObject( &record );
				if ( object != NULL ) {
					while ( true ) {
						if ( object != ship && !object->IsTypeOf( BACKDROP_TYPE ) && object->VirtualSlot63( &eye, toGlow, &pick ) ) {
							record = pick.GetRecord( 0 );
							if ( record->GetDistance() < 1.0f ) {
								visibility = 1.0f;
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

			if ( reset ) {
				if ( visibility > 0.0f ) {
					unknown130 = 1.0f;
				} else {
					unknown130 = 0.0f;
				}
				reset = false;
			} else {
				unknown130 += ( visibility * GLOW_FADE_RATE ) * elapsedTime;
				if ( unknown130 > 1.0f ) {
					unknown130 = 1.0f;
				} else if ( unknown130 < 0.0f ) {
					unknown130 = 0.0f;
				}
			}

			for ( int i = 0; i < 8; i++ ) {
				colors[i].a = 1.0f - unknown130;
			}
			shape->GetShapeData()->IncRevisionID();
			elapsedTime = 0.0f;
			NiBillboardNode::VirtualSlot35( camera );
			return;
		}
	}
	NiBillboardNode::VirtualSlot35( camera );
}

/*
================
PhaserGraphic::PhaserGraphic

Builds the beam from the property, a light and a glow for each end.
================
*/
// FUNCTION: STBC 0x004e6ab0
// PARKED: retail pushes the property's float getters through fld/fstp in the beam's argument list; this build first copies them to frame temporaries with integer moves (getters, direct members and pointer arguments tried)
PhaserGraphic::PhaserGraphic( PhaserProperty *property_, ShipClass *ship_ ) : property( property_ ), ship( ship_ ) {
	AttachChild( new PhaserBeam( property->GetOuterShellColor(), property->GetInnerShellColor(), property->GetOuterCoreColor(),
		property->GetInnerCoreColor(), property->GetNumSides(), property->GetMainRadius(), property->GetTaperRadius(),
		property->GetCoreScale(), property->GetTaperRatio(), property->GetTaperMinLength(), property->GetTaperMaxLength(),
		property->GetLengthTextureTilePerUnit(), property->GetPerimeterTile(), property->GetTextureSpeed(),
		property->GetTextureName() ), false );
	AttachChild( new NiPointLight, false );
	AttachChild( new NiPointLight, false );
	AttachChild( new PhaserGlow( ship_->GetContainingSet(), ship_, 1.0f, property->GetInnerShellColor(), property->GetInnerCoreColor() ), false );
	AttachChild( new PhaserGlow( ship_->GetContainingSet(), ship_, 1.0f, property->GetInnerShellColor(), property->GetInnerCoreColor() ), false );
}

/*
================
PhaserGraphic::Unknown4E6E90

Starts the charge-up at time for duration seconds: the graphic moves to
the bank's position, both lights are lit in the inner shell colour and
put on the ship, the beam is hidden and the glows are shown, reset onto
the ship's set at the beam's radius.
================
*/
// FUNCTION: STBC 0x004e6e90
// DRAFT: call set as retail (the scale if/else arms without braces: own 671, the end light's NiColor::operator= out of line); left: retail copies the radius to the scale local through fld/fstp, ours through eax, and one lea scheduled apart
void PhaserGraphic::Unknown4E6E90( float time, float duration_ ) {
	SetTranslate( property->GetPosition() );

	NiPointLight *light = GetStartLightNode();
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( property->GetInnerShellColor().r, property->GetInnerShellColor().g, property->GetInnerShellColor().b ) );
	light->SetDimmer( 0.4f );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( property->GetMainRadius() * 2.0f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetSwitch( true );
	ship->AddLight( light );

	light = GetEndLightNode();
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( property->GetInnerShellColor().r, property->GetInnerShellColor().g, property->GetInnerShellColor().b ) );
	light->SetDimmer( 0.4f );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( property->GetMainRadius() * 2.0f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetSwitch( true );
	ship->AddLight( light );

	GetBeam()->SetAppCulled( true );
	GetStartGlow()->SetAppCulled( false );
	GetEndGlow()->SetAppCulled( false );
	GetStartGlow()->SetSet( ship->GetContainingSet() );
	GetEndGlow()->SetSet( ship->GetContainingSet() );

	float scale = property->GetMainRadius();
	PhaserGlow *glow = GetStartGlow();
	if ( scale > 0.0f )
		glow->SetScale( scale );
	else
		glow->SetScale( -scale );
	scale = property->GetMainRadius();
	glow = GetEndGlow();
	if ( scale > 0.0f )
		glow->SetScale( scale );
	else
		glow->SetScale( -scale );

	GetStartGlow()->Reset();
	GetEndGlow()->Reset();

	startTime = time;
	duration = duration_;
}

/*
================
PhaserGraphic::Unknown4E7200

Moves the two lights and glows of a charging beam along the arc, from
either side of the width angle towards it as time passes; true once the
charge-up is over and they meet.
================
*/
// FUNCTION: STBC 0x004e7200
// DRAFT: retail registers no EH state for the forward temporaries of the two sums, expands the end point's scaled right vector and copies inline; the start half and the clamp match
bool PhaserGraphic::Unknown4E7200( float time, float widthAngle ) {
	float fraction = ( time - startTime ) / duration;
	if ( fraction < 0.0f ) {
		fraction = 0.0f;
	} else if ( fraction > 1.0f ) {
		fraction = 1.0f;
	}

	float halfArc = ( ( property->GetArcWidthAngleMax() - property->GetArcWidthAngleMin() ) * 0.5f ) * ( 1.0f - fraction );
	float angle = halfArc + widthAngle;
	NiPoint3 right = property->GetOrientationForward().Cross( property->GetOrientationUp() );

	NiPoint3 position = property->GetOrientationForward() * ( cos( angle ) * property->GetLength() ) + right * ( sin( angle ) * property->GetWidth() );
	GetStartLightNode()->SetTranslate( position );
	GetStartGlow()->SetTranslate( position );

	angle = widthAngle - halfArc;
	position = property->GetOrientationForward() * ( cos( angle ) * property->GetLength() ) + right * ( sin( angle ) * property->GetWidth() );
	GetEndLightNode()->SetTranslate( position );
	GetEndGlow()->SetTranslate( position );

	return fraction == 1.0f;
}

/*
================
PhaserGraphic::RemoveLights
================
*/
// FUNCTION: STBC 0x004e7540
void PhaserGraphic::RemoveLights() {
	ship->RemoveLight( GetStartLightNode() );
	ship->RemoveLight( GetEndLightNode() );
}

/*
================
PhaserGraphic::IsFinished
================
*/
// FUNCTION: STBC 0x004e7580
bool PhaserGraphic::IsFinished() {
	if ( startTime + duration <= UtopiaApp::GetApp()->GetGameTime() ) {
		return true;
	}
	return false;
}

/*
================
PhaserGraphic::Start

Starts the beam at time for duration seconds: the glows are reset onto
the ship's set, the start light is lit in the beam's inner shell colour
and put on the ship, and the end light and glow wait for the beam to
reach its end.
================
*/
// FUNCTION: STBC 0x004e75b0
// PARKED: schedule (region cut): the scheduler's third region ends at its 81-tuple cap on the end light's first `mov edx, [esi+0xe8]`, so its `add` runs before the `lea ecx`; retail's cut falls at least one tuple earlier and pairs the lea into the load's latency slot
void PhaserGraphic::Start( float time, float duration_ ) {
	GetStartGlow()->SetSet( ship->GetContainingSet() );
	GetStartGlow()->Reset();
	GetEndGlow()->SetSet( ship->GetContainingSet() );

	GetStartLightNode()->SetTranslate( NiPoint3::ZERO );
	GetStartGlow()->SetTranslate( NiPoint3::ZERO );

	NiPointLight *light = GetStartLightNode();
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( property->GetInnerShellColor().r, property->GetInnerShellColor().g, property->GetInnerShellColor().b ) );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( property->GetMainRadius() * 2.0f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetDimmer( 0.6f );

	light = GetEndLightNode();
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( property->GetInnerShellColor().r, property->GetInnerShellColor().g, property->GetInnerShellColor().b ) );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( property->GetMainRadius() * 2.0f );
	light->SetQuadraticAttenuation( 0.0f );
	light->SetDimmer( 1.0f );

	GetBeam()->ResetTextureOffset();
	ship->AddLight( GetStartLightNode() );
	startTime = time;
	duration = duration_;

	GetEndLightNode()->SetSwitch( false );
	GetEndGlow()->SetAppCulled( true );

	float scale = property->GetMainRadius() * 4.5f;
	PhaserGlow *glow = GetEndGlow();
	if ( scale > 0.0f ) {
		glow->SetScale( scale );
	} else {
		glow->SetScale( -scale );
	}
}

/*
================
PhaserGraphic::Update

Places the beam at position along direction: it grows to its full length
over the duration, and the end light and glow appear when it gets there.
================
*/
// FUNCTION: STBC 0x004e78b0
void PhaserGraphic::Update( const NiPoint3 &position, const NiPoint3 &direction, float length, float time ) {
	SetTranslate( position );

	float fraction;
	bool full;
	if ( duration > 0.0f ) {
		fraction = ( time - startTime ) / duration;
		if ( fraction < 0.0f ) {
			fraction = 0.0f;
			full = false;
		} else if ( fraction > 1.0f ) {
			fraction = 1.0f;
			full = true;
		} else if ( fraction == 1.0f ) {
			full = true;
		} else {
			full = false;
		}
	} else {
		fraction = 1.0f;
		full = true;
	}

	PhaserBeam *beam = GetBeam();
	GetBeam()->SetAppCulled( false );
	beam->SetDirection( direction, fraction * length, full );

	if ( full ) {
		NiPointLight *light = GetEndLightNode();
		light->SetTranslate( direction * length );
		light->SetSwitch( true );

		if ( GetEndGlow()->GetAppCulled() ) {
			GetEndGlow()->SetAppCulled( false );
			GetEndGlow()->Reset();
		}
		GetEndGlow()->SetTranslate( direction * length );
	}

	beam->ScrollTexture( time );
}

/*
================
PhaserGraphic::RemoveStartLight
================
*/
// FUNCTION: STBC 0x004e7a80
void PhaserGraphic::RemoveStartLight() {
	ship->RemoveLight( (NiPointLight *)GetAt( 1 ) );
}

/*
================
PhaserGraphic::GetEndLight
================
*/
// FUNCTION: STBC 0x004e7ab0
NiPointLight *PhaserGraphic::GetEndLight() {
	return (NiPointLight *)GetAt( 2 );
}

/*
================
PhaserGraphic::GetStartLightNode
================
*/
// FUNCTION: STBC 0x004e7ac0
NiPointLight *PhaserGraphic::GetStartLightNode() {
	return (NiPointLight *)GetAt( 1 );
}

/*
================
PhaserGraphic::GetEndLightNode
================
*/
// FUNCTION: STBC 0x004e7ad0
NiPointLight *PhaserGraphic::GetEndLightNode() {
	return (NiPointLight *)GetAt( 2 );
}

/*
================
PhaserGraphic::GetStartGlow
================
*/
// FUNCTION: STBC 0x004e7ae0
PhaserGlow *PhaserGraphic::GetStartGlow() {
	return (PhaserGlow *)GetAt( 3 );
}

/*
================
PhaserGraphic::GetEndGlow
================
*/
// FUNCTION: STBC 0x004e7af0
PhaserGlow *PhaserGraphic::GetEndGlow() {
	return (PhaserGlow *)GetAt( 4 );
}

/*
================
PhaserGraphic::GetBeam
================
*/
// FUNCTION: STBC 0x004e7b00
PhaserBeam *PhaserGraphic::GetBeam() {
	return (PhaserBeam *)GetAt( 0 );
}
