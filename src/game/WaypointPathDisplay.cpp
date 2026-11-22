/*
===============================================================================

	WaypointPathDisplay.cpp

	The line along a waypoint's path (see WaypointPathDisplay.h).

===============================================================================
*/

#include <iostream>
#include "WaypointPathDisplay.h"
#include "ImageManagerClass.h"
#include "NiLines.h"
#include "NiNode.h"
#include "NiColor.h"
#include "NiPoint2.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00590060 _$E421
// FUNCTION: STBC 0x00590080 _$E420

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0058fc40 _$E32
// FUNCTION: STBC 0x0058fc50 _$E29
// FUNCTION: STBC 0x0058fc60 _$E31
// FUNCTION: STBC 0x0058fc70 _$E30
// FUNCTION: STBC 0x0058fc80 _$E38
// FUNCTION: STBC 0x0058fc90 _$E35
// FUNCTION: STBC 0x0058fca0 _$E37
// FUNCTION: STBC 0x0058fcb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0058fcc0 _$E50
// FUNCTION: STBC 0x0058fcd0 _$E49
// FUNCTION: STBC 0x0058fce0 _$E228
// FUNCTION: STBC 0x0058fcf0 _$E227
// FUNCTION: STBC 0x0058fd00 _$E61
// FUNCTION: STBC 0x0058fd10 _$E60
// FUNCTION: STBC 0x0058fd20 _$E65
// FUNCTION: STBC 0x0058fd30 _$E64
// FUNCTION: STBC 0x0058fd40 _$E69
// FUNCTION: STBC 0x0058fd50 _$E68
// FUNCTION: STBC 0x0058fd60 _$E73
// FUNCTION: STBC 0x0058fd70 _$E72
// FUNCTION: STBC 0x0058fd80 _$E192
// FUNCTION: STBC 0x0058fd90 _$E191
// FUNCTION: STBC 0x0058fda0 _$E134
// FUNCTION: STBC 0x0058fdb0 _$E133
// FUNCTION: STBC 0x0058fdc0 _$E147
// FUNCTION: STBC 0x0058fdd0 _$E146
// FUNCTION: STBC 0x0058fde0 _$E220
// FUNCTION: STBC 0x0058fdf0 _$E219
// FUNCTION: STBC 0x0058fe00 _$E233
// FUNCTION: STBC 0x0058fe10 _$E232
// FUNCTION: STBC 0x0058fe40 _$E224
// FUNCTION: STBC 0x0058fe50 _$E223
// FUNCTION: STBC 0x0058fe60 _$E237
// FUNCTION: STBC 0x0058fe70 _$E236
// FUNCTION: STBC 0x0058fe80 _$E244
// FUNCTION: STBC 0x0058fe90 _$E243
// FUNCTION: STBC 0x0058fea0 _$E254
// FUNCTION: STBC 0x0058feb0 _$E253
// FUNCTION: STBC 0x0058fec0 _$E258
// FUNCTION: STBC 0x0058fed0 _$E257
// FUNCTION: STBC 0x0058fee0 _$E97
// FUNCTION: STBC 0x0058fef0 _$E96
// FUNCTION: STBC 0x0058ff00 _$E200
// FUNCTION: STBC 0x0058ff10 _$E199
// FUNCTION: STBC 0x0058ff20 _$E207
// FUNCTION: STBC 0x0058ff30 _$E206
// FUNCTION: STBC 0x0058ff40 _$E270
// FUNCTION: STBC 0x0058ff50 _$E269
// FUNCTION: STBC 0x0058ff60 _$E274
// FUNCTION: STBC 0x0058ff70 _$E273
// FUNCTION: STBC 0x0058ff80 _$E314
// FUNCTION: STBC 0x0058ff90 _$E313
// FUNCTION: STBC 0x0058ffa0 _$E320
// FUNCTION: STBC 0x0058ffb0 _$E319
// FUNCTION: STBC 0x0058ffc0 _$E262
// FUNCTION: STBC 0x0058ffd0 _$E261
// FUNCTION: STBC 0x0058ffe0 _$E266
// FUNCTION: STBC 0x0058fff0 _$E265
// FUNCTION: STBC 0x00590000 _$E278
// FUNCTION: STBC 0x00590010 _$E277
// FUNCTION: STBC 0x00590020 _$E282
// FUNCTION: STBC 0x00590030 _$E281
// FUNCTION: STBC 0x00590040 _$E286
// FUNCTION: STBC 0x00590050 _$E285
// FUNCTION: STBC 0x00590090 _$E290
// FUNCTION: STBC 0x005900a0 _$E289
// FUNCTION: STBC 0x005900b0 _$E294
// FUNCTION: STBC 0x005900c0 _$E293
// FUNCTION: STBC 0x005900d0 _$E298
// FUNCTION: STBC 0x005900e0 _$E297
// FUNCTION: STBC 0x005900f0 _$E302
// FUNCTION: STBC 0x00590100 _$E301
// FUNCTION: STBC 0x00590110 _$E306
// FUNCTION: STBC 0x00590120 _$E305
// FUNCTION: STBC 0x00590130 _$E102
// FUNCTION: STBC 0x00590140 _$E101
// FUNCTION: STBC 0x00590150 _$E310
// FUNCTION: STBC 0x00590160 _$E309
// FUNCTION: STBC 0x00590170 _$E107
// FUNCTION: STBC 0x00590180 _$E106
// FUNCTION: STBC 0x00590190 _$E324
// FUNCTION: STBC 0x005901a0 _$E323
// FUNCTION: STBC 0x005901b0 _$E328
// FUNCTION: STBC 0x005901c0 _$E327
// FUNCTION: STBC 0x005901d0 _$E332
// FUNCTION: STBC 0x005901e0 _$E331
// FUNCTION: STBC 0x005901f0 _$E336
// FUNCTION: STBC 0x00590200 _$E335
// FUNCTION: STBC 0x00590210 _$E116
// FUNCTION: STBC 0x00590220 _$E115
// FUNCTION: STBC 0x00590230 _$E211
// FUNCTION: STBC 0x00590240 _$E210
// FUNCTION: STBC 0x00590250 _$E215
// FUNCTION: STBC 0x00590260 _$E214
// FUNCTION: STBC 0x00590270 _$E340
// FUNCTION: STBC 0x00590280 _$E339
// FUNCTION: STBC 0x00590290 _$E122
// FUNCTION: STBC 0x005902a0 _$E121
// FUNCTION: STBC 0x005902b0 _$E78
// FUNCTION: STBC 0x005902c0 _$E77
// FUNCTION: STBC 0x005902d0 _$E152
// FUNCTION: STBC 0x005902e0 _$E151
// FUNCTION: STBC 0x005902f0 _$E344
// FUNCTION: STBC 0x00590300 _$E343
// FUNCTION: STBC 0x00590310 _$E157
// FUNCTION: STBC 0x00590320 _$E156
// FUNCTION: STBC 0x00590330 _$E348
// FUNCTION: STBC 0x00590340 _$E347
// FUNCTION: STBC 0x00590350 _$E352
// FUNCTION: STBC 0x00590360 _$E351
// FUNCTION: STBC 0x00590370 _$E162
// FUNCTION: STBC 0x00590380 _$E161
// FUNCTION: STBC 0x00590390 _$E356
// FUNCTION: STBC 0x005903a0 _$E355
// FUNCTION: STBC 0x005903b0 _$E127
// FUNCTION: STBC 0x005903c0 _$E126
// FUNCTION: STBC 0x005903d0 _$E360
// FUNCTION: STBC 0x005903e0 _$E359
// FUNCTION: STBC 0x005903f0 _$E364
// FUNCTION: STBC 0x00590400 _$E363
// FUNCTION: STBC 0x00590410 _$E167
// FUNCTION: STBC 0x00590420 _$E166
// FUNCTION: STBC 0x00590430 _$E172
// FUNCTION: STBC 0x00590440 _$E171
// FUNCTION: STBC 0x00590450 _$E368
// FUNCTION: STBC 0x00590460 _$E367
// FUNCTION: STBC 0x00590470 _$E372
// FUNCTION: STBC 0x00590480 _$E371
// FUNCTION: STBC 0x00590490 _$E376
// FUNCTION: STBC 0x005904a0 _$E375
// FUNCTION: STBC 0x005904b0 _$E380
// FUNCTION: STBC 0x005904c0 _$E379
// FUNCTION: STBC 0x005904d0 _$E384
// FUNCTION: STBC 0x005904e0 _$E383
// FUNCTION: STBC 0x005904f0 _$E388
// FUNCTION: STBC 0x00590500 _$E387
// FUNCTION: STBC 0x00590510 _$E392
// FUNCTION: STBC 0x00590520 _$E391
// FUNCTION: STBC 0x00590530 _$E396
// FUNCTION: STBC 0x00590540 _$E395
// FUNCTION: STBC 0x00590550 _$E177
// FUNCTION: STBC 0x00590560 _$E176
// FUNCTION: STBC 0x00590570 _$E400
// FUNCTION: STBC 0x00590580 _$E399
// FUNCTION: STBC 0x00590590 _$E182
// FUNCTION: STBC 0x005905a0 _$E181
// FUNCTION: STBC 0x005905b0 _$E187
// FUNCTION: STBC 0x005905c0 _$E186
// FUNCTION: STBC 0x005905d0 _$E404
// FUNCTION: STBC 0x005905e0 _$E403
// FUNCTION: STBC 0x005905f0 _$E409
// FUNCTION: STBC 0x00590600 _$E408
// FUNCTION: STBC 0x00590610 _$E413
// FUNCTION: STBC 0x00590620 _$E412

/*
================
WaypointPathDisplay::WaypointPathDisplay
================
*/
// FUNCTION: STBC 0x0058f760
WaypointPathDisplay::WaypointPathDisplay( NiNode *parent_, const BezierCurve &curve_ ) : parent( parent_ ), curve( curve_ ) {
	Build();
}

/*
================
WaypointPathDisplay::~WaypointPathDisplay

Takes the lines off the parent.
================
*/
// FUNCTION: STBC 0x0058f7c0
WaypointPathDisplay::~WaypointPathDisplay() {
	parent->DetachChild( lines );
	lines = NULL;
}

/*
================
WaypointPathDisplay::Build

Nine points from the curve's start to its end, joined in order: every
point but the last is joined to the next.
================
*/
// FUNCTION: STBC 0x0058f890
// DRAFT: inline budget: retail expands NiColorA's constructor in the colour array's loop (no EH state) and calls NiProperty's constructor (0x40DAD0) for the vertex colour property; this build the reverse
void WaypointPathDisplay::Build() {
	if ( lines != NULL ) {
		parent->DetachChild( lines );
		lines = NULL;
	}

	NiPoint3 *vertices = new NiPoint3[NUM_POINTS];
	NiColorA *colors = new NiColorA[NUM_POINTS];
	bool *connections = new bool[NUM_POINTS];
	int i;
	for ( i = 0; i < NUM_POINTS; i++ ) {
		vertices[i] = curve.Evaluate( i * ( 1.0f / ( NUM_POINTS - 1 ) ) );
		colors[i] = NiColorA::WHITE;
		connections[i] = true;
	}
	connections[NUM_POINTS - 1] = false;

	NiPoint2 *textures = new NiPoint2[NUM_POINTS];
	for ( i = 0; i < NUM_POINTS; i++ ) {
		textures[i] = NiPoint2::ZERO;
	}

	lines = new NiLines( NUM_POINTS, vertices, colors, textures, connections );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	lines->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( true );
	lines->AttachProperty( zBuffer );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( "data/Textures/White.tga", true, true ) );
	lines->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	lines->AttachProperty( textureMode );

	lines->UpdateProperties();
	parent->AttachChild( lines, false );
	lines->Update( 0.0f );
}
