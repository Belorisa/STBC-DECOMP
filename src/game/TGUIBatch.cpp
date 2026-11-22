/*
===============================================================================

	TGUIBatch.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGUIBatch.h"
#include "NiTriShapeDynamicData.h"
#include "TGIconPoly.h"
#include "TGIconManager.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00747b40 _$E32
// FUNCTION: STBC 0x00747b50 _$E29
// FUNCTION: STBC 0x00747b60 _$E31
// FUNCTION: STBC 0x00747b70 _$E30
// FUNCTION: STBC 0x00747b80 _$E38
// FUNCTION: STBC 0x00747b90 _$E35
// FUNCTION: STBC 0x00747ba0 _$E37
// FUNCTION: STBC 0x00747bb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00747bc0 _$E45
// FUNCTION: STBC 0x00747bd0 _$E44
// FUNCTION: STBC 0x00747be0 _$E233
// FUNCTION: STBC 0x00747bf0 _$E232
// FUNCTION: STBC 0x00747c00 _$E56
// FUNCTION: STBC 0x00747c10 _$E55
// FUNCTION: STBC 0x00747c20 _$E60
// FUNCTION: STBC 0x00747c30 _$E59
// FUNCTION: STBC 0x00747c40 _$E64
// FUNCTION: STBC 0x00747c50 _$E63
// FUNCTION: STBC 0x00747c60 _$E68
// FUNCTION: STBC 0x00747c70 _$E67
// FUNCTION: STBC 0x00747c80 _$E192
// FUNCTION: STBC 0x00747c90 _$E191
// FUNCTION: STBC 0x00747ca0 _$E134
// FUNCTION: STBC 0x00747cb0 _$E133
// FUNCTION: STBC 0x00747cc0 _$E147
// FUNCTION: STBC 0x00747cd0 _$E146
// FUNCTION: STBC 0x00747ce0 _$E238
// FUNCTION: STBC 0x00747cf0 _$E237
// FUNCTION: STBC 0x00747d00 _$E243
// FUNCTION: STBC 0x00747d10 _$E242
// FUNCTION: STBC 0x00747d20 _$E247
// FUNCTION: STBC 0x00747d30 _$E246
// FUNCTION: STBC 0x00747d40 _$E251
// FUNCTION: STBC 0x00747d50 _$E250
// FUNCTION: STBC 0x00747d60 _$E258
// FUNCTION: STBC 0x00747d70 _$E257
// FUNCTION: STBC 0x00747d80 _$E268
// FUNCTION: STBC 0x00747d90 _$E267
// FUNCTION: STBC 0x00747da0 _$E272
// FUNCTION: STBC 0x00747db0 _$E271
// FUNCTION: STBC 0x00747dc0 _$E92
// FUNCTION: STBC 0x00747dd0 _$E91
// FUNCTION: STBC 0x00747de0 _$E200
// FUNCTION: STBC 0x00747df0 _$E199
// FUNCTION: STBC 0x00747e00 _$E207
// FUNCTION: STBC 0x00747e10 _$E206
// FUNCTION: STBC 0x00747e20 _$E211
// FUNCTION: STBC 0x00747e30 _$E210
// FUNCTION: STBC 0x00747e40 _$E215
// FUNCTION: STBC 0x00747e50 _$E214
// FUNCTION: STBC 0x00747e60 _$E219
// FUNCTION: STBC 0x00747e70 _$E218
// FUNCTION: STBC 0x00747e80 _$E225
// FUNCTION: STBC 0x00747e90 _$E224
// FUNCTION: STBC 0x00747ea0 _$E276
// FUNCTION: STBC 0x00747eb0 _$E275
// FUNCTION: STBC 0x00747ec0 _$E280
// FUNCTION: STBC 0x00747ed0 _$E279
// FUNCTION: STBC 0x00747ee0 _$E284
// FUNCTION: STBC 0x00747ef0 _$E283
// FUNCTION: STBC 0x00747f00 _$E288
// FUNCTION: STBC 0x00747f10 _$E287
// FUNCTION: STBC 0x00747f20 _$E292
// FUNCTION: STBC 0x00747f30 _$E291
// FUNCTION: STBC 0x00747f40 _$E296
// FUNCTION: STBC 0x00747f50 _$E295
// FUNCTION: STBC 0x00747f60 _$E300
// FUNCTION: STBC 0x00747f70 _$E299
// FUNCTION: STBC 0x00747f80 _$E304
// FUNCTION: STBC 0x00747f90 _$E303
// FUNCTION: STBC 0x00747fa0 _$E308
// FUNCTION: STBC 0x00747fb0 _$E307
// FUNCTION: STBC 0x00747fc0 _$E312
// FUNCTION: STBC 0x00747fd0 _$E311
// FUNCTION: STBC 0x00747fe0 _$E97
// FUNCTION: STBC 0x00747ff0 _$E96
// FUNCTION: STBC 0x00748000 _$E316
// FUNCTION: STBC 0x00748010 _$E315
// FUNCTION: STBC 0x00748020 _$E102
// FUNCTION: STBC 0x00748030 _$E101
// FUNCTION: STBC 0x00748040 _$E320
// FUNCTION: STBC 0x00748050 _$E319
// FUNCTION: STBC 0x00748060 _$E324
// FUNCTION: STBC 0x00748070 _$E323
// FUNCTION: STBC 0x00748080 _$E328
// FUNCTION: STBC 0x00748090 _$E327
// FUNCTION: STBC 0x007480a0 _$E332
// FUNCTION: STBC 0x007480b0 _$E331
// FUNCTION: STBC 0x007480c0 _$E111
// FUNCTION: STBC 0x007480d0 _$E110
// FUNCTION: STBC 0x007480e0 _$E336
// FUNCTION: STBC 0x007480f0 _$E335
// FUNCTION: STBC 0x00748100 _$E340
// FUNCTION: STBC 0x00748110 _$E339
// FUNCTION: STBC 0x00748120 _$E344
// FUNCTION: STBC 0x00748130 _$E343
// FUNCTION: STBC 0x00748140 _$E117
// FUNCTION: STBC 0x00748150 _$E116
// FUNCTION: STBC 0x00748160 _$E122
// FUNCTION: STBC 0x00748170 _$E121
// FUNCTION: STBC 0x00748180 _$E152
// FUNCTION: STBC 0x00748190 _$E151
// FUNCTION: STBC 0x007481a0 _$E348
// FUNCTION: STBC 0x007481b0 _$E347
// FUNCTION: STBC 0x007481c0 _$E157
// FUNCTION: STBC 0x007481d0 _$E156
// FUNCTION: STBC 0x007481e0 _$E352
// FUNCTION: STBC 0x007481f0 _$E351
// FUNCTION: STBC 0x00748200 _$E356
// FUNCTION: STBC 0x00748210 _$E355
// FUNCTION: STBC 0x00748220 _$E162
// FUNCTION: STBC 0x00748230 _$E161
// FUNCTION: STBC 0x00748240 _$E360
// FUNCTION: STBC 0x00748250 _$E359
// FUNCTION: STBC 0x00748260 _$E127
// FUNCTION: STBC 0x00748270 _$E126
// FUNCTION: STBC 0x00748280 _$E364
// FUNCTION: STBC 0x00748290 _$E363
// FUNCTION: STBC 0x007482a0 _$E368
// FUNCTION: STBC 0x007482b0 _$E367
// FUNCTION: STBC 0x007482c0 _$E167
// FUNCTION: STBC 0x007482d0 _$E166
// FUNCTION: STBC 0x007482e0 _$E172
// FUNCTION: STBC 0x007482f0 _$E171
// FUNCTION: STBC 0x00748300 _$E372
// FUNCTION: STBC 0x00748310 _$E371
// FUNCTION: STBC 0x00748320 _$E376
// FUNCTION: STBC 0x00748330 _$E375
// FUNCTION: STBC 0x00748340 _$E229
// FUNCTION: STBC 0x00748350 _$E228
// FUNCTION: STBC 0x00748360 _$E380
// FUNCTION: STBC 0x00748370 _$E379
// FUNCTION: STBC 0x00748380 _$E384
// FUNCTION: STBC 0x00748390 _$E383
// FUNCTION: STBC 0x007483a0 _$E388
// FUNCTION: STBC 0x007483b0 _$E387
// FUNCTION: STBC 0x007483c0 _$E392
// FUNCTION: STBC 0x007483d0 _$E391
// FUNCTION: STBC 0x007483e0 _$E396
// FUNCTION: STBC 0x007483f0 _$E395
// FUNCTION: STBC 0x00748400 _$E177
// FUNCTION: STBC 0x00748410 _$E176
// FUNCTION: STBC 0x00748420 _$E400
// FUNCTION: STBC 0x00748430 _$E399
// FUNCTION: STBC 0x00748440 _$E182
// FUNCTION: STBC 0x00748450 _$E181
// FUNCTION: STBC 0x00748460 _$E187
// FUNCTION: STBC 0x00748470 _$E186

/*
================
TGUIBatch::TGUIBatch

An empty batch drawing from the texture: a shape with a unit bound and the
texture, a blending alpha, a vertex colour, a white emissive material and,
for a non-zero clamp mode, a texture mode clamping one way (1), the other
(2) or both.
================
*/
// FUNCTION: STBC 0x00748480
TGUIBatch::TGUIBatch( NiTextureProperty *texture_, int clamp ) {
	next = NULL;
	texture = texture_;
	shape = new NiTriShape( (NiTriShapeData *)NULL );
	shape->GetWorldBound().SetCenter( 0.5f, 0.5f, 1.5f );
	shape->GetWorldBound().SetRadius( 1.0f );
	unknown0C = NULL;
	unknown10 = 0;
	unknown12 = 0;
	shape->AttachProperty( texture_ );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	shape->AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	shape->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	shape->AttachProperty( material );

	if ( clamp != 0 ) {
		NiTextureModeProperty *textureMode = new NiTextureModeProperty;
		if ( clamp == 2 ) {
			textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_WRAP_T );
		} else if ( clamp == 1 ) {
			textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_CLAMP_T );
		} else {
			textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
		}
		shape->AttachProperty( textureMode );
	}
	shape->UpdateProperties();
}

/*
================
TGUIBatch::~TGUIBatch

Releases the two NetImmerse objects and frees the block.
================
*/
// FUNCTION: STBC 0x007487b0
TGUIBatch::~TGUIBatch( void ) {
	texture = NULL;
	shape = NULL;
	if ( unknown0C != NULL ) {
		delete unknown0C;
	}
}

/*
================
TGUIBatch::AddIconPoly
================
*/
// FUNCTION: STBC 0x00748890
TGIconPoly *TGUIBatch::AddIconPoly( TGRect *rect, NiPoint2 *textureCoords, NiColorA *color ) {
	if ( unknown10 == unknown12 ) {
		Grow();
	}
	TGIconPoly *poly = g_kIconManager.AllocIconPoly();
	NiTriShapeData *data = shape->GetShapeData();
	poly->index = unknown10;
	poly->batch = this;
	unknown0C[unknown10] = poly;
	unknown10++;
	data->SetActiveTriangleCount( unknown10 * 2 );
	data->SetActiveVertexCount( unknown10 * 4 );
	poly->rect = *rect;
	poly->UpdateVertices();
	poly->SetTextureCoords( textureCoords );
	poly->SetColor( color );
	data->IncRevisionID();
	return poly;
}

/*
================
TGUIBatch::RemoveIconPoly

The batch's last quad moves into the removed poly's place (its vertices,
texture coordinates and colours, and its poly), the active counts drop by
one quad, and the poly goes back to the icon manager.
================
*/
// FUNCTION: STBC 0x00748950 ?RemoveIconPoly@TGUIBatch@@QAEXPAVTGIconPoly@@@Z
void TGUIBatch::RemoveIconPoly( TGIconPoly *poly ) {
	unsigned short index = poly->index;
	NiTriShapeData *data = shape->GetShapeData();
	NiPoint3 *vertices = data->GetVertices();
	NiPoint2 *textures = data->GetTextures();
	NiColorA *colors = data->GetColors();

	if ( unknown10 > 1 && index < unknown10 - 1 ) {
		unsigned short to = index * 4;
		unsigned short from = unknown10 * 4 - 4;
		for ( int i = 0; i < 4; i++ ) {
			vertices[to + i] = vertices[from + i];
			textures[to + i] = textures[from + i];
			colors[to + i] = colors[from + i];
		}
		unknown0C[index] = unknown0C[unknown10 - 1];
		unknown0C[index]->index = index;
	}

	unknown10--;
	data->SetActiveVertexCount( unknown10 * 4 );
	data->SetActiveTriangleCount( unknown10 * 2 );
	data->IncRevisionID();
	g_kIconManager.FreeIconPoly( poly );
}

/*
================
TGUIBatch::Grow

Makes new shape data with room for more quads (as many again as there is
room for now, at least 8 and at most 64), copies the quads in, and gives
each new quad its two triangles.
================
*/
// FUNCTION: STBC 0x00748ac0
// PARKED: register: retail reads the growth into ax and adds the clamped value from its frame slot; this build keeps it in cx on the unclamped path and reloads it at the join
void TGUIBatch::Grow( void ) {
	unsigned short growth = unknown12;
	if ( growth == 0 ) {
		growth = 8;
	} else if ( growth > 64 ) {
		growth = 64;
	}

	NiTriShapeData *oldData = shape->GetShapeData();
	unsigned short quads = unknown10 + growth;
	unsigned short numVertices = quads * 4;
	unsigned short numTriangles = quads * 2;
	NiPoint3 *vertices = new NiPoint3[numVertices];
	NiColorA *colors = new NiColorA[numVertices];
	NiPoint2 *textures = new NiPoint2[numVertices];
	unsigned short *triangles = new unsigned short[numTriangles * 3];
	unknown12 += growth;
	TGIconPoly **polys = new TGIconPoly *[unknown12];
	NiTriShapeDynamicData *data = new NiTriShapeDynamicData( numVertices, vertices, NULL, colors, textures, numTriangles, triangles, 0, 0 );

	unsigned short vertex = unknown10 * 4;
	unsigned short triangle = unknown10 * 6;
	if ( oldData != NULL ) {
		NiPoint3 *oldVertices = oldData->GetVertices();
		NiPoint2 *oldTextures = oldData->GetTextures();
		NiColorA *oldColors = oldData->GetColors();
		unsigned short *oldTriangles = oldData->GetTriList();
		memcpy( vertices, oldVertices, vertex * sizeof( NiPoint3 ) );
		memcpy( textures, oldTextures, vertex * sizeof( NiPoint2 ) );
		memcpy( colors, oldColors, vertex * sizeof( NiColorA ) );
		memcpy( triangles, oldTriangles, triangle * sizeof( unsigned short ) );
		memcpy( polys, unknown0C, unknown10 * sizeof( TGIconPoly * ) );
		delete unknown0C;
	}
	shape->SetModelData( data );
	unknown0C = polys;

	while ( growth-- ) {
		triangles[triangle] = vertex;
		triangles[triangle + 1] = vertex + 1;
		triangles[triangle + 2] = vertex + 3;
		triangles[triangle + 3] = vertex + 1;
		triangles[triangle + 4] = vertex + 2;
		triangles[triangle + 5] = vertex + 3;
		triangle += 6;
		vertex += 4;
	}

	data->SetActiveVertexCount( unknown10 * 4 );
	data->SetActiveTriangleCount( unknown10 * 2 );
}
