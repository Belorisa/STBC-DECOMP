/*
===============================================================================

	WarpTrail.cpp

===============================================================================
*/

#include <iostream>
#include "WarpTrail.h"
#include "ImageManagerClass.h"
#include "PyEmbed.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiTriShape.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004d93b0 _$E441
// FUNCTION: STBC 0x004d93d0 _$E440

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d8d90 _$E32
// FUNCTION: STBC 0x004d8da0 _$E29
// FUNCTION: STBC 0x004d8db0 _$E31
// FUNCTION: STBC 0x004d8dc0 _$E30
// FUNCTION: STBC 0x004d8dd0 _$E38
// FUNCTION: STBC 0x004d8de0 _$E35
// FUNCTION: STBC 0x004d8df0 _$E37
// FUNCTION: STBC 0x004d8e00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d8e10 _$E45
// FUNCTION: STBC 0x004d8e20 _$E44
// FUNCTION: STBC 0x004d8e30 _$E262
// FUNCTION: STBC 0x004d8e40 _$E261
// FUNCTION: STBC 0x004d8e50 _$E56
// FUNCTION: STBC 0x004d8e60 _$E55
// FUNCTION: STBC 0x004d8e70 _$E60
// FUNCTION: STBC 0x004d8e80 _$E59
// FUNCTION: STBC 0x004d8e90 _$E64
// FUNCTION: STBC 0x004d8ea0 _$E63
// FUNCTION: STBC 0x004d8eb0 _$E68
// FUNCTION: STBC 0x004d8ec0 _$E67
// FUNCTION: STBC 0x004d8ed0 _$E192
// FUNCTION: STBC 0x004d8ee0 _$E191
// FUNCTION: STBC 0x004d8ef0 _$E134
// FUNCTION: STBC 0x004d8f00 _$E133
// FUNCTION: STBC 0x004d8f10 _$E147
// FUNCTION: STBC 0x004d8f20 _$E146
// FUNCTION: STBC 0x004d8f30 _$E197
// FUNCTION: STBC 0x004d8f40 _$E196
// FUNCTION: STBC 0x004d8f50 _$E267
// FUNCTION: STBC 0x004d8f60 _$E266
// FUNCTION: STBC 0x004d8f70 _$E201
// FUNCTION: STBC 0x004d8f80 _$E200
// FUNCTION: STBC 0x004d8f90 _$E271
// FUNCTION: STBC 0x004d8fa0 _$E270
// FUNCTION: STBC 0x004d8fb0 _$E278
// FUNCTION: STBC 0x004d8fc0 _$E277
// FUNCTION: STBC 0x004d8fd0 _$E288
// FUNCTION: STBC 0x004d8fe0 _$E287
// FUNCTION: STBC 0x004d8ff0 _$E292
// FUNCTION: STBC 0x004d9000 _$E291
// FUNCTION: STBC 0x004d9010 _$E92
// FUNCTION: STBC 0x004d9020 _$E91
// FUNCTION: STBC 0x004d9030 _$E233
// FUNCTION: STBC 0x004d9040 _$E232
// FUNCTION: STBC 0x004d9070 _$E240
// FUNCTION: STBC 0x004d9080 _$E239
// FUNCTION: STBC 0x004d9090 _$E244
// FUNCTION: STBC 0x004d90a0 _$E243
// FUNCTION: STBC 0x004d90b0 _$E248
// FUNCTION: STBC 0x004d90c0 _$E247
// FUNCTION: STBC 0x004d90d0 _$E252
// FUNCTION: STBC 0x004d90e0 _$E251
// FUNCTION: STBC 0x004d90f0 _$E258
// FUNCTION: STBC 0x004d9100 _$E257
// FUNCTION: STBC 0x004d9110 _$E296
// FUNCTION: STBC 0x004d9120 _$E295
// FUNCTION: STBC 0x004d9130 _$E300
// FUNCTION: STBC 0x004d9140 _$E299
// FUNCTION: STBC 0x004d9150 _$E304
// FUNCTION: STBC 0x004d9160 _$E303
// FUNCTION: STBC 0x004d9170 _$E308
// FUNCTION: STBC 0x004d9180 _$E307
// FUNCTION: STBC 0x004d9190 _$E312
// FUNCTION: STBC 0x004d91a0 _$E311
// FUNCTION: STBC 0x004d91b0 _$E316
// FUNCTION: STBC 0x004d91c0 _$E315
// FUNCTION: STBC 0x004d91d0 _$E205
// FUNCTION: STBC 0x004d91e0 _$E204
// FUNCTION: STBC 0x004d91f0 _$E320
// FUNCTION: STBC 0x004d9200 _$E319
// FUNCTION: STBC 0x004d9210 _$E324
// FUNCTION: STBC 0x004d9220 _$E323
// FUNCTION: STBC 0x004d9230 _$E328
// FUNCTION: STBC 0x004d9240 _$E327
// FUNCTION: STBC 0x004d9250 _$E97
// FUNCTION: STBC 0x004d9260 _$E96
// FUNCTION: STBC 0x004d9270 _$E332
// FUNCTION: STBC 0x004d9280 _$E331
// FUNCTION: STBC 0x004d9290 _$E102
// FUNCTION: STBC 0x004d92a0 _$E101
// FUNCTION: STBC 0x004d92b0 _$E336
// FUNCTION: STBC 0x004d92c0 _$E335
// FUNCTION: STBC 0x004d92d0 _$E340
// FUNCTION: STBC 0x004d92e0 _$E339
// FUNCTION: STBC 0x004d92f0 _$E344
// FUNCTION: STBC 0x004d9300 _$E343
// FUNCTION: STBC 0x004d9310 _$E348
// FUNCTION: STBC 0x004d9320 _$E347
// FUNCTION: STBC 0x004d9330 _$E111
// FUNCTION: STBC 0x004d9340 _$E110
// FUNCTION: STBC 0x004d9350 _$E352
// FUNCTION: STBC 0x004d9360 _$E351
// FUNCTION: STBC 0x004d9370 _$E356
// FUNCTION: STBC 0x004d9380 _$E355
// FUNCTION: STBC 0x004d9390 _$E360
// FUNCTION: STBC 0x004d93a0 _$E359
// FUNCTION: STBC 0x004d93e0 _$E117
// FUNCTION: STBC 0x004d93f0 _$E116
// FUNCTION: STBC 0x004d9400 _$E122
// FUNCTION: STBC 0x004d9410 _$E121
// FUNCTION: STBC 0x004d9420 _$E152
// FUNCTION: STBC 0x004d9430 _$E151
// FUNCTION: STBC 0x004d9440 _$E364
// FUNCTION: STBC 0x004d9450 _$E363
// FUNCTION: STBC 0x004d9460 _$E157
// FUNCTION: STBC 0x004d9470 _$E156
// FUNCTION: STBC 0x004d9480 _$E368
// FUNCTION: STBC 0x004d9490 _$E367
// FUNCTION: STBC 0x004d94a0 _$E372
// FUNCTION: STBC 0x004d94b0 _$E371
// FUNCTION: STBC 0x004d94c0 _$E162
// FUNCTION: STBC 0x004d94d0 _$E161
// FUNCTION: STBC 0x004d94e0 _$E376
// FUNCTION: STBC 0x004d94f0 _$E375
// FUNCTION: STBC 0x004d9500 _$E127
// FUNCTION: STBC 0x004d9510 _$E126
// FUNCTION: STBC 0x004d9520 _$E380
// FUNCTION: STBC 0x004d9530 _$E379
// FUNCTION: STBC 0x004d9540 _$E384
// FUNCTION: STBC 0x004d9550 _$E383
// FUNCTION: STBC 0x004d9560 _$E167
// FUNCTION: STBC 0x004d9570 _$E166
// FUNCTION: STBC 0x004d9580 _$E172
// FUNCTION: STBC 0x004d9590 _$E171
// FUNCTION: STBC 0x004d95a0 _$E388
// FUNCTION: STBC 0x004d95b0 _$E387
// FUNCTION: STBC 0x004d95c0 _$E392
// FUNCTION: STBC 0x004d95d0 _$E391
// FUNCTION: STBC 0x004d95e0 _$E396
// FUNCTION: STBC 0x004d95f0 _$E395
// FUNCTION: STBC 0x004d9600 _$E400
// FUNCTION: STBC 0x004d9610 _$E399
// FUNCTION: STBC 0x004d9620 _$E404
// FUNCTION: STBC 0x004d9630 _$E403
// FUNCTION: STBC 0x004d9640 _$E408
// FUNCTION: STBC 0x004d9650 _$E407
// FUNCTION: STBC 0x004d9660 _$E412
// FUNCTION: STBC 0x004d9670 _$E411
// FUNCTION: STBC 0x004d9680 _$E416
// FUNCTION: STBC 0x004d9690 _$E415
// FUNCTION: STBC 0x004d96a0 _$E177
// FUNCTION: STBC 0x004d96b0 _$E176
// FUNCTION: STBC 0x004d96c0 _$E420
// FUNCTION: STBC 0x004d96d0 _$E419
// FUNCTION: STBC 0x004d96e0 _$E182
// FUNCTION: STBC 0x004d96f0 _$E181
// FUNCTION: STBC 0x004d9700 _$E187
// FUNCTION: STBC 0x004d9710 _$E186
// FUNCTION: STBC 0x004d9720 _$E424
// FUNCTION: STBC 0x004d9730 _$E423
// FUNCTION: STBC 0x004d9740 _$E429
// FUNCTION: STBC 0x004d9750 _$E428
// FUNCTION: STBC 0x004d9760 _$E433
// FUNCTION: STBC 0x004d9770 _$E432

// The trail's size and texture when the Effects script doesn't give them
// (invented names).
#define DEFAULT_TRAIL_WIDTH		0.125f
#define DEFAULT_TRAIL_HEIGHT	1.25f

/*
================
WarpTrail::WarpTrail

A quad of the script's trail texture, its width either side of the node and
its height up from it, drawn white and blended, tested against the depth
buffer without writing it.
================
*/
// FUNCTION: STBC 0x004d88e0
// PARKED: inline budget: retail expands the texture mode constructor's SetClampMode SetField (or 0xC0; site 24's nested share 49 needs 50) and calls IncRevisionID (0x4D9050) out of line at the end; this build the reverse; retail's later verdicts bound site 24's budget to 1042..1052 (+47..57) and need >= 30 more units used after site 36, which no single construct of the body gives
WarpTrail::WarpTrail() {
	char *textureName = NULL;
	float width = DEFAULT_TRAIL_WIDTH;
	float height = DEFAULT_TRAIL_HEIGHT;
	PyEmbed::GetValue( "Effects", "g_pcWarpTrailTextureName", "s", &textureName );
	PyEmbed::GetValue( "Effects", "g_fWarpTrailWidth", "f", &width );
	PyEmbed::GetValue( "Effects", "g_fWarpTrailHeight", "f", &height );

	NiPoint3 *vertices = new NiPoint3[4];
	vertices[0] = NiPoint3( -width, 0.0f, 0.0f );
	vertices[1] = NiPoint3( width, 0.0f, 0.0f );
	vertices[2] = NiPoint3( width, height, 0.0f );
	vertices[3] = NiPoint3( -width, height, 0.0f );

	NiPoint2 *textures = new NiPoint2[4];
	textures[0] = NiPoint2( 0.0f, 0.0f );
	textures[1] = NiPoint2( 1.0f, 0.0f );
	textures[2] = NiPoint2( 1.0f, 1.0f );
	textures[3] = NiPoint2( 0.0f, 1.0f );

	unsigned short *triList = new unsigned short[6];
	triList[0] = 0;
	triList[1] = 1;
	triList[2] = 2;
	triList[3] = 0;
	triList[4] = 2;
	triList[5] = 3;

	NiColorA *colors = new NiColorA[4];
	for ( int i = 0; i < 4; i++ ) {
		colors[i] = NiColorA::WHITE;
	}

	NiTriShape *shape = new NiTriShape( 4, vertices, NULL, colors, textures, 2, triList );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( textureName, true, true ) );
	shape->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	shape->AttachProperty( textureMode );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	shape->AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	shape->AttachProperty( alpha );

	AttachChild( shape, false );
	shape->UpdateProperties();
	shape->GetShapeData()->IncRevisionID();
}

/*
================
WarpTrail::~WarpTrail
================
*/
// FUNCTION: STBC 0x004d8d70
WarpTrail::~WarpTrail() {
}

/*
================
WarpTrail::VirtualSlot44
================
*/
// FUNCTION: STBC 0x004d8d80
void WarpTrail::VirtualSlot44( float time ) {
	Update( time );
}
