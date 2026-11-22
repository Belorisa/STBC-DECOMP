/*
===============================================================================

	FlareController.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "FlareController.h"
#include "TriShapeOrientedParticles.h"
#include "TextureAnimManager.h"
#include "TGSystemWrapperClass.h"
#include "ImageManagerClass.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "SpinController.h"
#include "SunEffect.h"
#include "TGFuzzyTriShape.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004f13d0 _$E32
// FUNCTION: STBC 0x004f13e0 _$E29
// FUNCTION: STBC 0x004f13f0 _$E31
// FUNCTION: STBC 0x004f1400 _$E30
// FUNCTION: STBC 0x004f1410 _$E38
// FUNCTION: STBC 0x004f1420 _$E35
// FUNCTION: STBC 0x004f1430 _$E37
// FUNCTION: STBC 0x004f1440 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004f3d10 _$E638
// FUNCTION: STBC 0x004f3d30 _$E637

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004f1030 _$E80
// FUNCTION: STBC 0x004f1040 _$E79
// FUNCTION: STBC 0x004f1050 _$E453
// FUNCTION: STBC 0x004f1060 _$E452
// FUNCTION: STBC 0x004f1070 _$E457
// FUNCTION: STBC 0x004f1080 _$E456
// FUNCTION: STBC 0x004f1090 _$E461
// FUNCTION: STBC 0x004f10a0 _$E460
// FUNCTION: STBC 0x004f10b0 _$E470
// FUNCTION: STBC 0x004f10c0 _$E469
// FUNCTION: STBC 0x004f10d0 _$E105
// FUNCTION: STBC 0x004f10e0 _$E104
// FUNCTION: STBC 0x004f10f0 _$E474
// FUNCTION: STBC 0x004f1100 _$E473
// FUNCTION: STBC 0x004f1110 _$E478
// FUNCTION: STBC 0x004f1120 _$E477
// FUNCTION: STBC 0x004f1130 _$E108
// FUNCTION: STBC 0x004f1140 _$E107
// FUNCTION: STBC 0x004f1150 _$E539
// FUNCTION: STBC 0x004f1160 _$E538
// FUNCTION: STBC 0x004f1170 _$E499
// FUNCTION: STBC 0x004f1180 _$E498
// FUNCTION: STBC 0x004f1190 _$E515
// FUNCTION: STBC 0x004f11a0 _$E514
// FUNCTION: STBC 0x004f11b0 _$E482
// FUNCTION: STBC 0x004f11c0 _$E481
// FUNCTION: STBC 0x004f11d0 _$E522
// FUNCTION: STBC 0x004f11e0 _$E521
// FUNCTION: STBC 0x004f11f0 _$E525
// FUNCTION: STBC 0x004f1200 _$E524
// FUNCTION: STBC 0x004f1210 _$E558
// FUNCTION: STBC 0x004f1220 _$E557
// FUNCTION: STBC 0x004f1230 _$E485
// FUNCTION: STBC 0x004f1240 _$E484
// FUNCTION: STBC 0x004f1250 _$E488
// FUNCTION: STBC 0x004f1260 _$E487
// FUNCTION: STBC 0x004f1270 _$E491
// FUNCTION: STBC 0x004f1280 _$E490
// FUNCTION: STBC 0x004f1290 _$E68
// FUNCTION: STBC 0x004f12a0 _$E67
// FUNCTION: STBC 0x004f12b0 _$E495
// FUNCTION: STBC 0x004f12c0 _$E494
// FUNCTION: STBC 0x004f12d0 _$E528
// FUNCTION: STBC 0x004f12e0 _$E527
// FUNCTION: STBC 0x004f12f0 _$E503
// FUNCTION: STBC 0x004f1300 _$E502
// FUNCTION: STBC 0x004f1310 _$E507
// FUNCTION: STBC 0x004f1320 _$E506
// FUNCTION: STBC 0x004f1330 _$E511
// FUNCTION: STBC 0x004f1340 _$E510
// FUNCTION: STBC 0x004f1350 _$E566
// FUNCTION: STBC 0x004f1360 _$E565
// FUNCTION: STBC 0x004f1370 _$E591
// FUNCTION: STBC 0x004f1380 _$E590
// FUNCTION: STBC 0x004f1390 _$E579
// FUNCTION: STBC 0x004f13a0 _$E578
// FUNCTION: STBC 0x004f13b0 _$E519
// FUNCTION: STBC 0x004f13c0 _$E518
// FUNCTION: STBC 0x004f1450 _$E532
// FUNCTION: STBC 0x004f1460 _$E531
// FUNCTION: STBC 0x004f1470 _$E536
// FUNCTION: STBC 0x004f1480 _$E535
// FUNCTION: STBC 0x004f1490 _$E71
// FUNCTION: STBC 0x004f14a0 _$E70
// FUNCTION: STBC 0x004f14b0 _$E83
// FUNCTION: STBC 0x004f14c0 _$E82
// FUNCTION: STBC 0x004f14d0 _$E543
// FUNCTION: STBC 0x004f14e0 _$E542
// FUNCTION: STBC 0x004f14f0 _$E551
// FUNCTION: STBC 0x004f1500 _$E550
// FUNCTION: STBC 0x004f1510 _$E555
// FUNCTION: STBC 0x004f1520 _$E554
// FUNCTION: STBC 0x004f1530 _$E45
// FUNCTION: STBC 0x004f1540 _$E44
// FUNCTION: STBC 0x004f1550 _$E282
// FUNCTION: STBC 0x004f1560 _$E281
// FUNCTION: STBC 0x004f1570 _$E562
// FUNCTION: STBC 0x004f1580 _$E561
// FUNCTION: STBC 0x004f1590 _$E547
// FUNCTION: STBC 0x004f15a0 _$E546
// FUNCTION: STBC 0x004f15b0 _$E575
// FUNCTION: STBC 0x004f15c0 _$E574
// FUNCTION: STBC 0x004f15d0 _$E571
// FUNCTION: STBC 0x004f15e0 _$E570
// FUNCTION: STBC 0x004f15f0 _$E116
// FUNCTION: STBC 0x004f1600 _$E115
// FUNCTION: STBC 0x004f1610 _$E56
// FUNCTION: STBC 0x004f1620 _$E55
// FUNCTION: STBC 0x004f1630 _$E583
// FUNCTION: STBC 0x004f1640 _$E582
// FUNCTION: STBC 0x004f1650 _$E587
// FUNCTION: STBC 0x004f1660 _$E586
// FUNCTION: STBC 0x004f1670 _$E120
// FUNCTION: STBC 0x004f1680 _$E119
// FUNCTION: STBC 0x004f1690 _$E594
// FUNCTION: STBC 0x004f16a0 _$E593
// FUNCTION: STBC 0x004f16b0 _$E597
// FUNCTION: STBC 0x004f16c0 _$E596
// FUNCTION: STBC 0x004f16d0 _$E124
// FUNCTION: STBC 0x004f16e0 _$E123
// FUNCTION: STBC 0x004f16f0 _$E601
// FUNCTION: STBC 0x004f1700 _$E600
// FUNCTION: STBC 0x004f1710 _$E239
// FUNCTION: STBC 0x004f1720 _$E238
// FUNCTION: STBC 0x004f1730 _$E605
// FUNCTION: STBC 0x004f1740 _$E604
// FUNCTION: STBC 0x004f28f0 _$E181
// FUNCTION: STBC 0x004f2900 _$E180
// FUNCTION: STBC 0x004f2a60 _$E194
// FUNCTION: STBC 0x004f2a70 _$E193
// FUNCTION: STBC 0x004f3550 _$E287
// FUNCTION: STBC 0x004f3560 _$E286
// FUNCTION: STBC 0x004f3570 _$E292
// FUNCTION: STBC 0x004f3580 _$E291
// FUNCTION: STBC 0x004f3590 _$E296
// FUNCTION: STBC 0x004f35a0 _$E295
// FUNCTION: STBC 0x004f35b0 _$E300
// FUNCTION: STBC 0x004f35c0 _$E299
// FUNCTION: STBC 0x004f35f0 _$E307
// FUNCTION: STBC 0x004f3600 _$E306
// FUNCTION: STBC 0x004f3610 _$E317
// FUNCTION: STBC 0x004f3620 _$E316
// FUNCTION: STBC 0x004f3630 _$E321
// FUNCTION: STBC 0x004f3640 _$E320
// FUNCTION: STBC 0x004f3650 _$E143
// FUNCTION: STBC 0x004f3660 _$E142
// FUNCTION: STBC 0x004f3670 _$E244
// FUNCTION: STBC 0x004f3680 _$E243
// FUNCTION: STBC 0x004f3690 _$E251
// FUNCTION: STBC 0x004f36a0 _$E250
// FUNCTION: STBC 0x004f36b0 _$E255
// FUNCTION: STBC 0x004f36c0 _$E254
// FUNCTION: STBC 0x004f36d0 _$E259
// FUNCTION: STBC 0x004f36e0 _$E258
// FUNCTION: STBC 0x004f36f0 _$E263
// FUNCTION: STBC 0x004f3700 _$E262
// FUNCTION: STBC 0x004f3710 _$E269
// FUNCTION: STBC 0x004f3720 _$E268
// FUNCTION: STBC 0x004f3730 _$E325
// FUNCTION: STBC 0x004f3740 _$E324
// FUNCTION: STBC 0x004f3750 _$E329
// FUNCTION: STBC 0x004f3760 _$E328
// FUNCTION: STBC 0x004f3770 _$E333
// FUNCTION: STBC 0x004f3780 _$E332
// FUNCTION: STBC 0x004f3790 _$E337
// FUNCTION: STBC 0x004f37a0 _$E336
// FUNCTION: STBC 0x004f37b0 _$E341
// FUNCTION: STBC 0x004f37c0 _$E340
// FUNCTION: STBC 0x004f37d0 _$E345
// FUNCTION: STBC 0x004f37e0 _$E344
// FUNCTION: STBC 0x004f37f0 _$E349
// FUNCTION: STBC 0x004f3800 _$E348
// FUNCTION: STBC 0x004f3810 _$E353
// FUNCTION: STBC 0x004f3820 _$E352
// FUNCTION: STBC 0x004f3830 _$E357
// FUNCTION: STBC 0x004f3840 _$E356
// FUNCTION: STBC 0x004f3850 _$E361
// FUNCTION: STBC 0x004f3860 _$E360
// FUNCTION: STBC 0x004f3870 _$E148
// FUNCTION: STBC 0x004f3880 _$E147
// FUNCTION: STBC 0x004f3890 _$E365
// FUNCTION: STBC 0x004f38a0 _$E364
// FUNCTION: STBC 0x004f38b0 _$E153
// FUNCTION: STBC 0x004f38c0 _$E152
// FUNCTION: STBC 0x004f38d0 _$E369
// FUNCTION: STBC 0x004f38e0 _$E368
// FUNCTION: STBC 0x004f38f0 _$E373
// FUNCTION: STBC 0x004f3900 _$E372
// FUNCTION: STBC 0x004f3910 _$E377
// FUNCTION: STBC 0x004f3920 _$E376
// FUNCTION: STBC 0x004f3930 _$E381
// FUNCTION: STBC 0x004f3940 _$E380
// FUNCTION: STBC 0x004f3950 _$E158
// FUNCTION: STBC 0x004f3960 _$E157
// FUNCTION: STBC 0x004f3970 _$E385
// FUNCTION: STBC 0x004f3980 _$E384
// FUNCTION: STBC 0x004f3990 _$E389
// FUNCTION: STBC 0x004f39a0 _$E388
// FUNCTION: STBC 0x004f39b0 _$E393
// FUNCTION: STBC 0x004f39c0 _$E392
// FUNCTION: STBC 0x004f39d0 _$E164
// FUNCTION: STBC 0x004f39e0 _$E163
// FUNCTION: STBC 0x004f39f0 _$E169
// FUNCTION: STBC 0x004f3a00 _$E168
// FUNCTION: STBC 0x004f3a10 _$E199
// FUNCTION: STBC 0x004f3a20 _$E198
// FUNCTION: STBC 0x004f3a30 _$E397
// FUNCTION: STBC 0x004f3a40 _$E396
// FUNCTION: STBC 0x004f3a50 _$E204
// FUNCTION: STBC 0x004f3a60 _$E203
// FUNCTION: STBC 0x004f3a70 _$E401
// FUNCTION: STBC 0x004f3a80 _$E400
// FUNCTION: STBC 0x004f3a90 _$E405
// FUNCTION: STBC 0x004f3aa0 _$E404
// FUNCTION: STBC 0x004f3ab0 _$E209
// FUNCTION: STBC 0x004f3ac0 _$E208
// FUNCTION: STBC 0x004f3ad0 _$E409
// FUNCTION: STBC 0x004f3ae0 _$E408
// FUNCTION: STBC 0x004f3af0 _$E174
// FUNCTION: STBC 0x004f3b00 _$E173
// FUNCTION: STBC 0x004f3b10 _$E413
// FUNCTION: STBC 0x004f3b20 _$E412
// FUNCTION: STBC 0x004f3b30 _$E417
// FUNCTION: STBC 0x004f3b40 _$E416
// FUNCTION: STBC 0x004f3b50 _$E214
// FUNCTION: STBC 0x004f3b60 _$E213
// FUNCTION: STBC 0x004f3b70 _$E219
// FUNCTION: STBC 0x004f3b80 _$E218
// FUNCTION: STBC 0x004f3b90 _$E421
// FUNCTION: STBC 0x004f3ba0 _$E420
// FUNCTION: STBC 0x004f3bb0 _$E425
// FUNCTION: STBC 0x004f3bc0 _$E424
// FUNCTION: STBC 0x004f3bd0 _$E273
// FUNCTION: STBC 0x004f3be0 _$E272
// FUNCTION: STBC 0x004f3bf0 _$E429
// FUNCTION: STBC 0x004f3c00 _$E428
// FUNCTION: STBC 0x004f3c10 _$E433
// FUNCTION: STBC 0x004f3c20 _$E432
// FUNCTION: STBC 0x004f3c30 _$E437
// FUNCTION: STBC 0x004f3c40 _$E436
// FUNCTION: STBC 0x004f3c50 _$E441
// FUNCTION: STBC 0x004f3c60 _$E440
// FUNCTION: STBC 0x004f3c70 _$E445
// FUNCTION: STBC 0x004f3c80 _$E444
// FUNCTION: STBC 0x004f3c90 _$E224
// FUNCTION: STBC 0x004f3ca0 _$E223
// FUNCTION: STBC 0x004f3cb0 _$E449
// FUNCTION: STBC 0x004f3cc0 _$E448
// FUNCTION: STBC 0x004f3cd0 _$E229
// FUNCTION: STBC 0x004f3ce0 _$E228
// FUNCTION: STBC 0x004f3cf0 _$E234
// FUNCTION: STBC 0x004f3d00 _$E233
// FUNCTION: STBC 0x004f3d40 _$E609
// FUNCTION: STBC 0x004f3d50 _$E608
// FUNCTION: STBC 0x004f3d60 _$E614
// FUNCTION: STBC 0x004f3d70 _$E613
// FUNCTION: STBC 0x004f3d80 _$E618
// FUNCTION: STBC 0x004f3d90 _$E617

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004f1750 _$E625
// FUNCTION: STBC 0x004f1760 _$E621
// FUNCTION: STBC 0x004f1780 _$E624
// FUNCTION: STBC 0x004f1790 _$E622
// DATA: STBC 0x9867A8 ?rtti@FlareController@@2VNiRTTI@@B
const NiRTTI FlareController::rtti( "FlareController", &EffectController::rtti );

/*
================
SunEffect::CreateSphere

A unit sphere of rings * 8 latitude rows by rings * 16 longitude
columns (the poles and the seam duplicated), turned by a small random
rotation; additive, depth tested but not written, unsorted, under the
texture tiled uTile by vTile. Fuzzy, it is a TGFuzzyTriShape with white
vertex colours as emissive; otherwise a lit white NiTriShape.
================
*/
// FUNCTION: STBC 0x004f06d0
// DRAFT: inline budget: retail calls the image NiPointer assignment (0x45D9A0) out of line, which needs own cost 24..50 tuples lower (c2trace whatif; -8..-22 also expands the texture property's release); the sphere loop's registers and slots differ
NiAVObject *SunEffect::CreateSphere( int rings, char *texture, bool fuzzy, float uTile, float vTile ) {
	int numRows = rings * 8;
	int numColumns = rings * 16;
	int numVertices = numRows * numColumns;
	int numTriangles = ( numColumns * 2 - 2 ) * ( numRows - 1 );

	NiPoint3 *vertices = new NiPoint3[numVertices];
	NiPoint2 *textures = new NiPoint2[numVertices];
	NiPoint3 *normals = new NiPoint3[numVertices];
	NiColorA *colors;
	if ( fuzzy ) {
		colors = new NiColorA[numVertices];
	} else {
		colors = NULL;
	}
	unsigned short *triangles = new unsigned short[numTriangles * 3];

	float phi = NI_HALF_PI - NI_PI * 0.5f;
	int index = 0;
	float thetaStep = NI_TWO_PI / ( numColumns - 1 );
	float phiStep = NI_PI / ( numRows - 1 );
	for ( int row = 0; row < numRows; row++ ) {
		float theta = NI_TWO_PI * -0.5f;
		float v = row / (float)( numRows - 1 ) * vTile;
		float cosPhi = (float)cos( phi );
		float sinPhi = (float)sin( phi );
		for ( int column = 0; column < numColumns; column++ ) {
			int vertex = row * numColumns + column;
			float sinTheta = (float)sin( theta );
			normals[vertex] = NiPoint3( sinTheta * sinPhi, (float)cos( theta ) * sinPhi, cosPhi );
			vertices[vertex] = normals[vertex];
			textures[vertex].y = v;
			textures[vertex].x = column / (float)( numColumns - 1 ) * uTile;
			if ( row > 0 && column > 0 ) {
				triangles[index++] = vertex;
				triangles[index++] = vertex - 1;
				triangles[index++] = vertex - numColumns;
				triangles[index++] = vertex - numColumns;
				triangles[index++] = vertex - 1;
				triangles[index++] = vertex - numColumns - 1;
			}
			theta += thetaStep;
		}
		phi += phiStep;
	}

	NiTriShape *sphere;
	if ( fuzzy ) {
		for ( int i = 0; i < numVertices; i++ ) {
			colors[i] = NiColorA::WHITE;
		}
		sphere = new TGFuzzyTriShape( numVertices, vertices, normals, colors, textures, numTriangles, triangles );
	} else {
		sphere = new NiTriShape( numVertices, vertices, normals, NULL, textures, numTriangles, triangles );
	}

	NiPoint3 axis( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
	axis.Unitize();
	NiMatrix3 rotation;
	rotation.MakeRotation( rand() * ( 1.0f / RAND_MAX ), axis );
	sphere->SetRotate( rotation );

	if ( fuzzy ) {
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		sphere->AttachProperty( vertexColor );
	} else {
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		sphere->AttachProperty( vertexColor );
		NiMaterialProperty *material = new NiMaterialProperty;
		material->SetEmittance( NiColor::WHITE );
		sphere->AttachProperty( material );
	}

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetNoSorter( true );
	sphere->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	sphere->AttachProperty( zBuffer );

	NiPointer<NiTextureProperty> textureProperty = new NiTextureProperty;
	textureProperty->SetImage( NiImage::Create( texture, 2, 7, 2 ) );
	sphere->AttachProperty( textureProperty );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	sphere->AttachProperty( textureMode );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	sphere->AttachProperty( material );

	sphere->UpdateProperties();
	return sphere;
}

/*
================
FlareController::FindController
================
*/
// FUNCTION: STBC 0x004f17b0
FlareController *FlareController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		FlareController *flareController = NiDynamicCast( FlareController, controller );
		if ( flareController ) {
			return flareController;
		}
	}
	return 0;
}

/*
================
FlareController::FlareController
================
*/
// FUNCTION: STBC 0x004f1800
FlareController::FlareController( float flareLife_, float emitFrequency_, float radius_, NiAVObject *emitFrom_ ) : EffectController( new FlareControllerData ) {
	textureAnim = NULL;
	tracks = NULL;
	radius = radius_;
	lifeSpans = NULL;
	birthTimes = NULL;
	sizes = NULL;
	dimensions = NULL;
	directions = NULL;
	colors = NULL;
	positions = NULL;
	textures = NULL;
	emitTimer = 0.0f;
	numActive = 0;
	numParticles = 0;
	flareLife = flareLife_;
	flareLifeVariance = 0.0f;
	emitFrequency = emitFrequency_;
	emitFrequencyVariance = 0.0f;
	emitFrom = emitFrom_;
}

/*
================
FlareController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x004f1960
bool FlareController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &TriShapeOrientedParticles::rtti );
}

/*
================
FlareController::~FlareController
================
*/
// FUNCTION: STBC 0x004f1ae0
FlareController::~FlareController() {
	delete [] lifeSpans;
	delete [] birthTimes;
	delete [] sizes;
	delete [] tracks;
}

/*
================
FlareController::CreateTarget

Builds the flares, textured with the texture animation's image when one is
named, lit by their own colours and blended additively over what is
behind them.
================
*/
// FUNCTION: STBC 0x004f1b70
// PARKED: inline budget: with the particle-count test unbraced the last AttachProperty is expanded and the z-buffer SetBit called as retail; retail also calls NiPointer's constructor (0x42FD00) out of line at the first of the last three properties (c2trace [11.1]), which needs one more free site after the last AttachProperty
void FlareController::CreateTarget( char *textureAnimName ) {
	if ( numParticles <= 0 ) {
		VirtualSlot34();
		if ( numParticles <= 0 )
			return;
	}

	TextureAnimContainer *container = NULL;
	g_kTextureAnimManager.GetContainer( textureAnimName, container );
	textureAnim = container;

	TriShapeOrientedParticles *particles = new TriShapeOrientedParticles( numParticles, false, true, GetFlareData()->dimensionKeys.GetCount() > 1, true );

	if ( textureAnimName ) {
		NiTextureProperty *texture = new NiTextureProperty;
		texture->SetImage( textureAnim->GetImage() );
		particles->AttachProperty( texture );

		NiTextureModeProperty *textureMode = new NiTextureModeProperty;
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
		textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
		particles->AttachProperty( textureMode );
	}

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	particles->AttachProperty( vertexColor );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetAlphaBlending( true );
	particles->AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	particles->AttachProperty( zBuffer );

	particles->UpdateProperties();
	particles->SetActiveParticleCount( 0 );
	SetTarget( particles );
}

/*
================
FlareController::GetTargetArrays
================
*/
inline void FlareController::GetTargetArrays( NiObjectNET *particles ) {
	TriShapeOrientedParticles *orientedParticles = NiDynamicCast( TriShapeOrientedParticles, particles );
	dimensions = orientedParticles->GetDimensions();
	directions = orientedParticles->GetDirections();
	positions = orientedParticles->GetPositions();
	colors = orientedParticles->GetParticleColors();
	textures = orientedParticles->NiGeometry::GetPackedTextures();
}

/*
================
FlareController::SetTarget
================
*/
// FUNCTION: STBC 0x004f1ec0
void FlareController::SetTarget( NiObjectNET *target_ ) {
	NiTimeController::SetTarget( target_ );
	GetTargetArrays( target_ );
}

/*
================
FlareController::CreateClone
================
*/
// FUNCTION: STBC 0x004f1f40
NiObject *FlareController::CreateClone() {
	FlareController *clone = new FlareController( 1.0f, 1.0f, 1.0f, NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
FlareController::CopyMembers

The copy gets particle arrays of its own and no live flares.
================
*/
// FUNCTION: STBC 0x004f1fc0
void FlareController::CopyMembers( FlareController *dest ) {
	EffectController::CopyMembers( dest );
	dest->textureAnim = textureAnim;
	dest->tracks = new TextureAnimTrack *[numParticles];
	dest->lifeSpans = new float[numParticles];
	dest->birthTimes = new float[numParticles];
	dest->sizes = new float[numParticles];
	dest->radius = radius;
	dest->emitFrequency = emitFrequency;
	// BUG: copies the emit timer, and the frequency variance not at all
	dest->emitTimer = emitTimer;
	dest->flareLife = flareLife;
	dest->flareLifeVariance = flareLifeVariance;
	dest->numActive = 0;
	dest->numParticles = numParticles;
}

/*
================
FlareController::ProcessClone

The clone caches its own target's arrays.
================
*/
// FUNCTION: STBC 0x004f2060
void FlareController::ProcessClone( NiObject *clone ) {
	FlareController *dest = (FlareController *)clone;
	dest->GetTargetArrays( dest->target );
}

/*
================
FlareController::VirtualSlot34

The most flares alive at once is the highest emit rate times the longest
life; the effect lasts at least that long.
================
*/
// FUNCTION: STBC 0x004f20e0
void FlareController::VirtualSlot34() {
	RemoveAllFlares();
	int oldCount = numParticles;
	float maxLife = flareLife + flareLifeVariance * 0.5f;
	emitTimer = 0.0f;
	float maxRate = 1.0f / ( emitFrequency - emitFrequencyVariance * 0.5f );
	if ( lifeTime < maxLife ) {
		lifeTime = maxLife;
	}
	numParticles = (int)( maxRate * maxLife ) + 1;
	// BUG: tests the tracks and sizes the wrong way round, so the arrays are
	// reallocated on every call once they exist
	if ( numParticles != oldCount || !lifeSpans || !birthTimes || tracks || sizes ) {
		delete [] lifeSpans;
		delete [] birthTimes;
		// BUG: the sizes are reallocated below without being freed
		delete [] tracks;
		lifeSpans = new float[numParticles];
		birthTimes = new float[numParticles];
		tracks = new TextureAnimTrack *[numParticles];
		sizes = new float[numParticles];
	}
}

/*
================
FlareController::AddColorKey
================
*/
// FUNCTION: STBC 0x004f21c0
void FlareController::AddColorKey( float time, float r, float g, float b ) {
	if ( time >= 0.0f && time <= 1.0f ) {
		EffectController::AddColorKey( time, r, g, b );
	}
}

/*
================
FlareController::AddAlphaKey
================
*/
// FUNCTION: STBC 0x004f2200
void FlareController::AddAlphaKey( float time, float alpha ) {
	if ( time >= 0.0f && time <= 1.0f ) {
		EffectController::AddAlphaKey( time, alpha );
	}
}

/*
================
FlareController::AddDimensionKey
================
*/
// FUNCTION: STBC 0x004f2240
void FlareController::AddDimensionKey( float time, float width, float length ) {
	GetFlareData()->dimensionKeys.Insert( NiLinPosKey( time, NiPoint3( width, length, 0.0f ) ) );
}

/*
================
FlareController::Stop
================
*/
// FUNCTION: STBC 0x004f23a0
void FlareController::Stop() {
	RemoveAllFlares();
	EffectController::Stop();
}

/*
================
FlareController::UpdateFlare

Colours the flare, shows its track's frame for its age and sizes it from
the dimension keys, scaled by its own size.
================
*/
// FUNCTION: STBC 0x004f23c0
void FlareController::UpdateFlare( int index, float time, float deltaTime, float lifeFraction ) {
	if ( colors ) {
		GetColor( lifeFraction, colors[index].r, colors[index].g, colors[index].b );
		GetAlpha( lifeFraction, colors[index].a );
	}
	int frame = (int)( ( tracks[index]->GetNumFrames() - 1 ) * lifeFraction );
	tracks[index]->GetFrameCorners( frame, &textures[index * 4] );
	if ( dimensions ) {
		int count = GetFlareData()->dimensionKeys.GetCount();
		NiLinPosKey *key = GetFlareData()->dimensionKeys.keys;
		int i;
		for ( i = 0; i < count; i++ ) {
			if ( key[i].GetTime() > lifeFraction ) {
				if ( i == 0 ) {
					dimensions[index].x = key[0].GetPos().x;
					dimensions[index].y = key[0].GetPos().y;
				} else {
					NiLinPosKey *key0 = &key[i - 1];
					NiLinPosKey *key1 = &key[i];
					float time0 = key0->GetTime();
					float time1 = key1->GetTime();
					NiPosKey::InterpFunction interp = NiPosKey::interpFunctions[NiPosKey::LINKEY];
					NiPoint3 dimension = interp( ( lifeFraction - time0 ) / ( time1 - time0 ), key0, key1 );
					dimensions[index].x = dimension.x;
					dimensions[index].y = dimension.y;
				}
				break;
			}
		}
		if ( i == count ) {
			dimensions[index].x = key[count - 1].GetPos().x;
			dimensions[index].y = key[count - 1].GetPos().y;
		}
	}
	// BUG: scales the dimension even when the target has no dimensions
	dimensions[index] *= sizes[index];
}

/*
================
FlareController::RemoveFlare
================
*/
// FUNCTION: STBC 0x004f2560
void FlareController::RemoveFlare( int index ) {
	if ( index != numActive - 1 ) {
		directions[index] = directions[numActive - 1];
		positions[index] = positions[numActive - 1];
		if ( colors ) {
			colors[index] = colors[numActive - 1];
		}
		if ( dimensions ) {
			dimensions[index] = dimensions[numActive - 1];
		}
		// BUG: the moved flare keeps the removed one's track and size
		lifeSpans[index] = lifeSpans[numActive - 1];
		birthTimes[index] = birthTimes[numActive - 1];
	}
	numActive--;
	( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
}

/*
================
FlareController::RemoveAllFlares
================
*/
// FUNCTION: STBC 0x004f2660
void FlareController::RemoveAllFlares() {
	numActive = 0;
	if ( target ) {
		( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
	}
}

/*
================
FlareController::EmitFlare

The new flare gets a random life, track, direction and size; it is placed
on the sphere along its direction.
================
*/
// FUNCTION: STBC 0x004f2690
bool FlareController::EmitFlare( float time ) {
	float life;
	if ( flareLifeVariance > 0.0f ) {
		life = flareLife + ( rand() * ( 1.0f / RAND_MAX ) - 0.5f ) * flareLifeVariance;
	} else {
		life = flareLife;
	}
	if ( life > emitTimer ) {
		lifeSpans[numActive] = life;
		int track = 0;
		if ( textureAnim->GetNumTracks() > 1 ) {
			track = g_kSystemWrapper.GetRandomNumber( textureAnim->GetNumTracks() - 1 );
		}
		tracks[numActive] = textureAnim->GetTrack( track );

		// a random point inside the unit sphere, not too near its centre
		float x, y, z, lengthSq;
		do {
			y = NiSymmetricRandom();
			x = NiSymmetricRandom();
			z = NiSymmetricRandom();
			lengthSq = x * x + ( y * y + z * z );
		} while ( lengthSq > 1.0f || lengthSq < 0.01 );
		directions[numActive] = NiPoint3( x, y, z );
		directions[numActive].Unitize();
		positions[numActive] = radius * directions[numActive];

		birthTimes[numActive] = time - emitTimer;
		float fraction = rand() * ( 1.0f / RAND_MAX );
		GetSize( fraction, sizes[numActive] );
		numActive++;
		( (TriShapeParticles *)target )->SetActiveParticleCount( numActive );
		return true;
	}
	return false;
}

/*
================
FlareController::Update

Ages the live flares, dropping the expired ones, then emits the flares
owed since the last update.
================
*/
// FUNCTION: STBC 0x004f2910
void FlareController::Update( float time ) {
	if ( GetField( STATE_MASK, STATE_POS ) != STATE_RUNNING ) {
		return;
	}
	if ( lastTime < 0.0f ) {
		lastTime = time;
	}
	float deltaTime = time - lastTime;
	if ( deltaTime < 0.0f ) {
		return;
	}
	for ( int i = 0; i < numActive; i++ ) {
		float age = time - birthTimes[i];
		if ( age > lifeSpans[i] ) {
			RemoveFlare( i );
			i--;
		} else {
			UpdateFlare( i, time, deltaTime, age / lifeSpans[i] );
		}
	}
	emitTimer += deltaTime;
	while ( emitTimer >= emitFrequency && numActive < numParticles ) {
		emitTimer -= emitFrequency + ( rand() * ( 1.0f / RAND_MAX ) - 0.5f ) * emitFrequencyVariance;
		if ( EmitFlare( time ) ) {
			UpdateFlare( numActive - 1, time, emitTimer, ( time - birthTimes[numActive - 1] ) / lifeSpans[numActive - 1] );
		}
	}
	lastTime = time;
	( (TriShapeParticles *)target )->InvalidateBound();
}

/*
================
SpinController::CreateObject
================
*/
// FUNCTION: STBC 0x004f2a80
void SpinController::CreateObject( NiStream &stream, NiObject *&object ) {
	SpinController *controller = new SpinController;
	stream.SetLoadRecord( new NiTimeControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
SpinController::Update

Turns the target by the rate times the time since the last update.
================
*/
// FUNCTION: STBC 0x004f2c20
void SpinController::Update( float time ) {
	if ( target ) {
		if ( lastTime < 0.0f ) {
			lastTime = time;
		}
		float elapsed = time - lastTime;
		if ( elapsed > 0.0f ) {
			lastTime = time;
			NiMatrix3 rotation;
			rotation.MakeRotation( elapsed * rate, axis );
			rotation = ( (NiAVObject *)target )->GetRotate() * rotation;
			( (NiAVObject *)target )->SetRotate( rotation );
		}
	}
}

/*
================
SpinController::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x004f2cc0
bool SpinController::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &NiAVObject::rtti );
}

/*
================
SpinController::IsEqual
================
*/
// FUNCTION: STBC 0x004f2d20
bool SpinController::IsEqual( NiObject *object ) {
	return NiTimeController::IsEqual( object );
}

/*
================
SpinController::CreateClone

A new controller (about a new random axis) with this one's base members.
================
*/
// FUNCTION: STBC 0x004f2d30
NiObject *SpinController::CreateClone() {
	SpinController *clone = new SpinController;
	CopyMembers( clone );
	return clone;
}

/*
================
SpinController::CopyMembers
================
*/
// FUNCTION: STBC 0x004f2e70
void SpinController::CopyMembers( SpinController *dest ) {
	NiTimeController::CopyMembers( dest );
}

/*
================
SpinController::LoadBinary
================
*/
// FUNCTION: STBC 0x004f2e80
void SpinController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
}

/*
================
SpinController::LinkObject
================
*/
// FUNCTION: STBC 0x004f2e90
void SpinController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
}

/*
================
SpinController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x004f2ea0
bool SpinController::RegisterStreamables( NiStream &stream ) {
	return NiTimeController::RegisterStreamables( stream );
}

/*
================
SpinController::SaveBinary
================
*/
// FUNCTION: STBC 0x004f2eb0
void SpinController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
}

/*
================
SunEffect::Create

A sun of radius inside a node scaled to outerRadius: the base sphere and
the node the flares leave from turn on their own, two fuzzy glow shells
grow from the sphere out to the node's scale, each turning too, and a
FlareController sends the flares out from the sphere.
================
*/
// FUNCTION: STBC 0x004f2ec0
// DRAFT: inline budget: retail calls IsKindOf and the two ~NiPointer DecRefCounts out of line for the alpha cast; c2trace --whatif: 5 or 6 more top-level inline sites after the casts flip exactly those three
NiAVObject *SunEffect::Create( char *baseTexture, char *flareTexture, float outerRadius, float radius ) {
	char *texture = "data/Textures/SunBase.tga";
	if ( baseTexture != NULL ) {
		texture = baseTexture;
	}

	NiNode *root = new NiNode;
	NiPointer<NiAVObject> sphere;
	NiNode *flareNode = new NiNode;
	root->AttachChild( flareNode, false );

	float scale = radius / outerRadius;
	sphere = CreateSphere( 2, texture, false, 8.0f, 4.0f );
	if ( scale > 0.0f ) {
		sphere->SetScale( scale );
	} else {
		sphere->SetScale( -scale );
	}
	flareNode->AttachChild( sphere, false );

	NiAlphaProperty *alpha = NiDynamicCast( NiAlphaProperty, sphere->GetProperty( NiAlphaProperty::ALPHA ) );
	alpha->SetAlphaBlending( false );
	NiZBufferProperty *zBuffer = NiDynamicCast( NiZBufferProperty, sphere->GetProperty( NiZBufferProperty::ZBUFFER ) );
	zBuffer->SetZBufferWrite( true );

	SpinController *spin = new SpinController;
	spin->SetTarget( flareNode );

	FlareController *flares = new FlareController( 1.0f, 1.0f, 1.0f, NULL );
	flares->SetEmitFromObject( sphere );
	flares->SetEmitFrequency( 0.015f );
	flares->SetFlareLife( 4.0f );
	flares->SetFlareLifeVariance( 0.5f );
	flares->AddSizeKey( 0.0f, 0.5f );
	flares->AddSizeKey( 0.3f, 1.0f );
	flares->AddSizeKey( 0.8f, 1.0f );
	flares->AddSizeKey( 1.0f, 1.5f );
	flares->AddDimensionKey( 0.0f, 0.02f, 0.0f );
	flares->AddDimensionKey( 0.3f, 0.08f, 0.08f );
	flares->AddDimensionKey( 0.5f, 0.08f, 0.1f );
	flares->AddDimensionKey( 0.8f, 0.08f, 0.16f );
	flares->AddDimensionKey( 1.0f, 0.1f, 0.0f );
	flares->AddColorKey( 1.0f, 1.0f, 1.0f, 1.0f );
	flares->AddAlphaKey( 0.0f, 0.0f );
	flares->AddAlphaKey( 0.1f, 1.0f );
	flares->AddAlphaKey( 0.4f, 1.0f );
	flares->AddAlphaKey( 1.0f, 0.0f );
	flares->SetRadius( scale * 0.95f );
	if ( flareTexture == NULL ) {
		flareTexture = "data/Textures/Effects/SunFlaresOrange.tga";
	}
	flares->CreateTarget( flareTexture );
	flares->AttachEffect( flareNode );
	flares->Start( -NI_INFINITY );

	float step = ( 1.0f - scale ) * 0.5f;
	for ( int i = 0; i < 2; ) {
		sphere = CreateSphere( 1, texture, true, 8.0f, 4.0f );
		root->AttachChild( sphere, false );
		i++;
		float shellScale = i * step + scale;
		if ( shellScale <= 0.0f ) {
			shellScale = -shellScale;
		}
		sphere->SetScale( shellScale );
		spin = new SpinController;
		spin->SetTarget( sphere );
		// BUG: redundant: attaches the shell to the root a second time
		root->AttachChild( sphere, false );
	}

	if ( outerRadius > 0.0f ) {
		root->SetScale( outerRadius );
	} else {
		root->SetScale( -outerRadius );
	}
	return root;
}
