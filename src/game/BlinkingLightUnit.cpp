/*
===============================================================================

	BlinkingLightUnit.cpp

	The light sprites of the ships' blinking lights. The unit's name is
	invented: retail keeps its <iostream> quad and NetImmerse registrations
	at 0x4DA130 and its code at 0x4DABA0-0x4DB260, with the inline copies of
	NiFloatController's and NiAlphaController's members it instantiates.

===============================================================================
*/

#include <iostream>
#include "BlinkingLightUnit.h"
#include "ImageManagerClass.h"
#include "NiAlphaController.h"
#include "NiAlphaProperty.h"
#include "NiBillboardNode.h"
#include "NiLinFloatKey.h"
#include "NiMath.h"
#include "NiMaterialProperty.h"
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
// FUNCTION: STBC 0x004db6e0 _$E445
// FUNCTION: STBC 0x004db700 _$E444

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004db260 _$E32
// FUNCTION: STBC 0x004db270 _$E29
// FUNCTION: STBC 0x004db280 _$E31
// FUNCTION: STBC 0x004db290 _$E30
// FUNCTION: STBC 0x004db2a0 _$E38
// FUNCTION: STBC 0x004db2b0 _$E35
// FUNCTION: STBC 0x004db2c0 _$E37
// FUNCTION: STBC 0x004db2d0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004dab00 _$E82
// FUNCTION: STBC 0x004dab10 _$E81
// FUNCTION: STBC 0x004dab20 _$E86
// FUNCTION: STBC 0x004dab30 _$E85
// FUNCTION: STBC 0x004dab40 _$E90
// FUNCTION: STBC 0x004dab50 _$E89
// FUNCTION: STBC 0x004dab60 _$E104
// FUNCTION: STBC 0x004dab70 _$E103
// FUNCTION: STBC 0x004dab80 _$E229
// FUNCTION: STBC 0x004dab90 _$E228
// FUNCTION: STBC 0x004db2e0 _$E45
// FUNCTION: STBC 0x004db2f0 _$E44
// FUNCTION: STBC 0x004db300 _$E266
// FUNCTION: STBC 0x004db310 _$E265
// FUNCTION: STBC 0x004db320 _$E56
// FUNCTION: STBC 0x004db330 _$E55
// FUNCTION: STBC 0x004db340 _$E60
// FUNCTION: STBC 0x004db350 _$E59
// FUNCTION: STBC 0x004db360 _$E64
// FUNCTION: STBC 0x004db370 _$E63
// FUNCTION: STBC 0x004db380 _$E68
// FUNCTION: STBC 0x004db390 _$E67
// FUNCTION: STBC 0x004db3a0 _$E213
// FUNCTION: STBC 0x004db3b0 _$E212
// FUNCTION: STBC 0x004db3c0 _$E155
// FUNCTION: STBC 0x004db3d0 _$E154
// FUNCTION: STBC 0x004db3e0 _$E168
// FUNCTION: STBC 0x004db3f0 _$E167
// FUNCTION: STBC 0x004db400 _$E218
// FUNCTION: STBC 0x004db410 _$E217
// FUNCTION: STBC 0x004db420 _$E271
// FUNCTION: STBC 0x004db430 _$E270
// FUNCTION: STBC 0x004db440 _$E222
// FUNCTION: STBC 0x004db450 _$E221
// FUNCTION: STBC 0x004db460 _$E275
// FUNCTION: STBC 0x004db470 _$E274
// FUNCTION: STBC 0x004db480 _$E282
// FUNCTION: STBC 0x004db490 _$E281
// FUNCTION: STBC 0x004db4a0 _$E292
// FUNCTION: STBC 0x004db4b0 _$E291
// FUNCTION: STBC 0x004db4c0 _$E296
// FUNCTION: STBC 0x004db4d0 _$E295
// FUNCTION: STBC 0x004db4e0 _$E109
// FUNCTION: STBC 0x004db4f0 _$E108
// FUNCTION: STBC 0x004db500 _$E237
// FUNCTION: STBC 0x004db510 _$E236
// FUNCTION: STBC 0x004db520 _$E244
// FUNCTION: STBC 0x004db530 _$E243
// FUNCTION: STBC 0x004db540 _$E248
// FUNCTION: STBC 0x004db550 _$E247
// FUNCTION: STBC 0x004db560 _$E252
// FUNCTION: STBC 0x004db570 _$E251
// FUNCTION: STBC 0x004db580 _$E256
// FUNCTION: STBC 0x004db590 _$E255
// FUNCTION: STBC 0x004db5a0 _$E262
// FUNCTION: STBC 0x004db5b0 _$E261
// FUNCTION: STBC 0x004db5c0 _$E300
// FUNCTION: STBC 0x004db5d0 _$E299
// FUNCTION: STBC 0x004db5e0 _$E304
// FUNCTION: STBC 0x004db5f0 _$E303
// FUNCTION: STBC 0x004db600 _$E308
// FUNCTION: STBC 0x004db610 _$E307
// FUNCTION: STBC 0x004db620 _$E312
// FUNCTION: STBC 0x004db630 _$E311
// FUNCTION: STBC 0x004db640 _$E316
// FUNCTION: STBC 0x004db650 _$E315
// FUNCTION: STBC 0x004db660 _$E320
// FUNCTION: STBC 0x004db670 _$E319
// FUNCTION: STBC 0x004db680 _$E226
// FUNCTION: STBC 0x004db690 _$E225
// FUNCTION: STBC 0x004db6a0 _$E324
// FUNCTION: STBC 0x004db6b0 _$E323
// FUNCTION: STBC 0x004db6c0 _$E328
// FUNCTION: STBC 0x004db6d0 _$E327
// FUNCTION: STBC 0x004db710 _$E332
// FUNCTION: STBC 0x004db720 _$E331
// FUNCTION: STBC 0x004db730 _$E133
// FUNCTION: STBC 0x004db740 _$E132
// FUNCTION: STBC 0x004db750 _$E336
// FUNCTION: STBC 0x004db760 _$E335
// FUNCTION: STBC 0x004db770 _$E138
// FUNCTION: STBC 0x004db780 _$E137
// FUNCTION: STBC 0x004db790 _$E340
// FUNCTION: STBC 0x004db7a0 _$E339
// FUNCTION: STBC 0x004db7b0 _$E344
// FUNCTION: STBC 0x004db7c0 _$E343
// FUNCTION: STBC 0x004db7d0 _$E348
// FUNCTION: STBC 0x004db7e0 _$E347
// FUNCTION: STBC 0x004db7f0 _$E352
// FUNCTION: STBC 0x004db800 _$E351
// FUNCTION: STBC 0x004db810 _$E143
// FUNCTION: STBC 0x004db820 _$E142
// FUNCTION: STBC 0x004db830 _$E356
// FUNCTION: STBC 0x004db840 _$E355
// FUNCTION: STBC 0x004db850 _$E360
// FUNCTION: STBC 0x004db860 _$E359
// FUNCTION: STBC 0x004db870 _$E364
// FUNCTION: STBC 0x004db880 _$E363
// FUNCTION: STBC 0x004db890 _$E100
// FUNCTION: STBC 0x004db8a0 _$E99
// FUNCTION: STBC 0x004db8b0 _$E73
// FUNCTION: STBC 0x004db8c0 _$E72
// FUNCTION: STBC 0x004db8d0 _$E173
// FUNCTION: STBC 0x004db8e0 _$E172
// FUNCTION: STBC 0x004db8f0 _$E368
// FUNCTION: STBC 0x004db900 _$E367
// FUNCTION: STBC 0x004db910 _$E178
// FUNCTION: STBC 0x004db920 _$E177
// FUNCTION: STBC 0x004db930 _$E372
// FUNCTION: STBC 0x004db940 _$E371
// FUNCTION: STBC 0x004db950 _$E376
// FUNCTION: STBC 0x004db960 _$E375
// FUNCTION: STBC 0x004db970 _$E183
// FUNCTION: STBC 0x004db980 _$E182
// FUNCTION: STBC 0x004db990 _$E380
// FUNCTION: STBC 0x004db9a0 _$E379
// FUNCTION: STBC 0x004db9b0 _$E148
// FUNCTION: STBC 0x004db9c0 _$E147
// FUNCTION: STBC 0x004db9d0 _$E384
// FUNCTION: STBC 0x004db9e0 _$E383
// FUNCTION: STBC 0x004db9f0 _$E388
// FUNCTION: STBC 0x004dba00 _$E387
// FUNCTION: STBC 0x004dba10 _$E188
// FUNCTION: STBC 0x004dba20 _$E187
// FUNCTION: STBC 0x004dba30 _$E193
// FUNCTION: STBC 0x004dba40 _$E192
// FUNCTION: STBC 0x004dba50 _$E392
// FUNCTION: STBC 0x004dba60 _$E391
// FUNCTION: STBC 0x004dba70 _$E396
// FUNCTION: STBC 0x004dba80 _$E395
// FUNCTION: STBC 0x004dba90 _$E400
// FUNCTION: STBC 0x004dbaa0 _$E399
// FUNCTION: STBC 0x004dbab0 _$E404
// FUNCTION: STBC 0x004dbac0 _$E403
// FUNCTION: STBC 0x004dbad0 _$E408
// FUNCTION: STBC 0x004dbae0 _$E407
// FUNCTION: STBC 0x004dbaf0 _$E412
// FUNCTION: STBC 0x004dbb00 _$E411
// FUNCTION: STBC 0x004dbb10 _$E416
// FUNCTION: STBC 0x004dbb20 _$E415
// FUNCTION: STBC 0x004dbb30 _$E420
// FUNCTION: STBC 0x004dbb40 _$E419
// FUNCTION: STBC 0x004dbb50 _$E198
// FUNCTION: STBC 0x004dbb60 _$E197
// FUNCTION: STBC 0x004dbb70 _$E424
// FUNCTION: STBC 0x004dbb80 _$E423
// FUNCTION: STBC 0x004dbb90 _$E203
// FUNCTION: STBC 0x004dbba0 _$E202
// FUNCTION: STBC 0x004dbbb0 _$E208
// FUNCTION: STBC 0x004dbbc0 _$E207
// FUNCTION: STBC 0x004dbbd0 _$E428
// FUNCTION: STBC 0x004dbbe0 _$E427
// FUNCTION: STBC 0x004dbbf0 _$E433
// FUNCTION: STBC 0x004dbc00 _$E432
// FUNCTION: STBC 0x004dbc10 _$E437
// FUNCTION: STBC 0x004dbc20 _$E436

/*
================
Unknown4DABA0

A billboard facing the camera rigidly, holding a unit quad at position
scaled by radius. The quad is textured with textureName, blended additively
over the scene without writing depth, and lit by color through its
material, whose alpha an NiAlphaController animates by numKeys linear keys,
looping at frequency.
================
*/
// FUNCTION: STBC 0x004daba0
// PARKED: inline budget: retail calls the alpha and material properties' NiPointer constructor (0x42FD00) and the z-buffer's second SetBit out of line (site 35's share 56 must fall under 53, site 38's 58 under 58): the function's own cost must be 48..71 tuples lower with the same sites, and no statement of identical bytes is worth that; the quad and EH state's EBP/EDI swap follows
NiNode *Unknown4DABA0( const NiPoint3 &position, const NiColor &color, float radius, float frequency, unsigned int numKeys, float *times, float *values, char *textureName ) {
	NiBillboardNode *node = new NiBillboardNode;
	node->SetMode( NiBillboardNode::RIGID_FACE_CAMERA );

	NiPoint3 *vertices = new NiPoint3[4];
	vertices[0] = NiPoint3( -0.5f, -0.5f, 0.0f );
	vertices[1] = NiPoint3( 0.5f, -0.5f, 0.0f );
	vertices[2] = NiPoint3( 0.5f, 0.5f, 0.0f );
	vertices[3] = NiPoint3( -0.5f, 0.5f, 0.0f );

	NiPoint2 *textures = new NiPoint2[4];
	textures[0] = NiPoint2( 0.0f, 0.0f );
	textures[1] = NiPoint2( 0.0f, 1.0f );
	textures[2] = NiPoint2( 1.0f, 1.0f );
	textures[3] = NiPoint2( 1.0f, 0.0f );

	unsigned short *triangles = new unsigned short[6];
	triangles[0] = 0;
	triangles[1] = 1;
	triangles[2] = 2;
	triangles[3] = 0;
	triangles[4] = 2;
	triangles[5] = 3;

	NiTriShape *quad = new NiTriShape( 4, vertices, NULL, NULL, textures, 2, triangles );
	quad->SetTranslate( position );
	if ( radius > 0.0f ) {
		quad->SetScale( radius );
	} else {
		quad->SetScale( -radius );
	}

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( textureName, true, true ) );
	quad->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
	quad->AttachProperty( textureMode );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetAlphaBlending( true );
	quad->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	quad->AttachProperty( zBuffer );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	quad->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( color );
	material->SetDiffuseColor( color );
	material->SetAlpha( 1.0f );
	quad->AttachProperty( material );

	NiFloatKey **keys = new NiFloatKey *[numKeys];
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		keys[i] = new NiLinFloatKey( times[i], values[i] );
	}

	NiAlphaController *controller = new NiAlphaController;
	controller->SetTarget( material );
	controller->ReplaceAnim( numKeys, keys, NiFloatKey::LINKEY );
	controller->Start( -NI_INFINITY );
	controller->SetCycleType( NiTimeController::LOOP );
	controller->SetFrequency( frequency );

	node->AttachChild( quad, false );
	node->UpdateProperties();
	return node;
}

/*
================
Unknown4DB1D0

Four alpha keys over one cycle, in cycles: dark at 0, lit from a tenth of
the lit time until a tenth before its end, dark again at its end.
================
*/
// FUNCTION: STBC 0x004db1d0
NiNode *Unknown4DB1D0( const NiPoint3 &position, const NiColor &color, float radius, float period, float duration, char *textureName ) {
	float *times = new float[4];
	float *values = new float[4];
	float litTime = duration / period;
	float fadeTime = litTime / 10.0f;
	times[0] = 0.0f;
	values[0] = 0.0f;
	times[1] = fadeTime;
	values[1] = 1.0f;
	times[2] = litTime - fadeTime;
	values[2] = 1.0f;
	times[3] = litTime;
	values[3] = 0.0f;
	NiNode *node = Unknown4DABA0( position, color, radius, period, 4, times, values, textureName );
	delete[] times;
	delete[] values;
	return node;
}
