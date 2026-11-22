/*
===============================================================================

	WarpFlash.cpp

===============================================================================
*/

#include <iostream>
#include "WarpFlash.h"
#include <stdio.h>
#include "TGStream.h"
#include "SetClass.h"
#include "UtopiaModule.h"
#include "TGSoundRegion.h"
#include "TGSoundManager.h"
#include "TGSound.h"
#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "NiAlphaProperty.h"
#include "NiBillboardNode.h"
#include "NiFlipController.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiTriShape.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004d5900 _$E657
// FUNCTION: STBC 0x004d5920 _$E656

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d4450 _$E32
// FUNCTION: STBC 0x004d4460 _$E29
// FUNCTION: STBC 0x004d4470 _$E31
// FUNCTION: STBC 0x004d4480 _$E30
// FUNCTION: STBC 0x004d4490 _$E38
// FUNCTION: STBC 0x004d44a0 _$E35
// FUNCTION: STBC 0x004d44b0 _$E37
// FUNCTION: STBC 0x004d44c0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d4130 _$E456
// FUNCTION: STBC 0x004d4140 _$E455
// FUNCTION: STBC 0x004d4150 _$E460
// FUNCTION: STBC 0x004d4160 _$E459
// FUNCTION: STBC 0x004d4170 _$E464
// FUNCTION: STBC 0x004d4180 _$E463
// FUNCTION: STBC 0x004d4190 _$E468
// FUNCTION: STBC 0x004d41a0 _$E467
// FUNCTION: STBC 0x004d41b0 _$E491
// FUNCTION: STBC 0x004d41c0 _$E490
// FUNCTION: STBC 0x004d41d0 _$E482
// FUNCTION: STBC 0x004d41e0 _$E481
// FUNCTION: STBC 0x004d41f0 _$E495
// FUNCTION: STBC 0x004d4200 _$E494
// FUNCTION: STBC 0x004d4210 _$E499
// FUNCTION: STBC 0x004d4220 _$E498
// FUNCTION: STBC 0x004d4230 _$E570
// FUNCTION: STBC 0x004d4240 _$E569
// FUNCTION: STBC 0x004d4250 _$E573
// FUNCTION: STBC 0x004d4260 _$E572
// FUNCTION: STBC 0x004d4270 _$E528
// FUNCTION: STBC 0x004d4280 _$E527
// FUNCTION: STBC 0x004d4290 _$E540
// FUNCTION: STBC 0x004d42a0 _$E539
// FUNCTION: STBC 0x004d42b0 _$E503
// FUNCTION: STBC 0x004d42c0 _$E502
// FUNCTION: STBC 0x004d42d0 _$E547
// FUNCTION: STBC 0x004d42e0 _$E546
// FUNCTION: STBC 0x004d42f0 _$E550
// FUNCTION: STBC 0x004d4300 _$E549
// FUNCTION: STBC 0x004d4310 _$E592
// FUNCTION: STBC 0x004d4320 _$E591
// FUNCTION: STBC 0x004d4330 _$E506
// FUNCTION: STBC 0x004d4340 _$E505
// FUNCTION: STBC 0x004d4350 _$E509
// FUNCTION: STBC 0x004d4360 _$E508
// FUNCTION: STBC 0x004d4370 _$E512
// FUNCTION: STBC 0x004d4380 _$E511
// FUNCTION: STBC 0x004d4390 _$E520
// FUNCTION: STBC 0x004d43a0 _$E519
// FUNCTION: STBC 0x004d43b0 _$E524
// FUNCTION: STBC 0x004d43c0 _$E523
// FUNCTION: STBC 0x004d43d0 _$E553
// FUNCTION: STBC 0x004d43e0 _$E552
// FUNCTION: STBC 0x004d43f0 _$E275
// FUNCTION: STBC 0x004d4400 _$E274
// FUNCTION: STBC 0x004d4410 _$E532
// FUNCTION: STBC 0x004d4420 _$E531
// FUNCTION: STBC 0x004d4430 _$E536
// FUNCTION: STBC 0x004d4440 _$E535
// FUNCTION: STBC 0x004d44d0 _$E600
// FUNCTION: STBC 0x004d44e0 _$E599
// FUNCTION: STBC 0x004d44f0 _$E625
// FUNCTION: STBC 0x004d4500 _$E624
// FUNCTION: STBC 0x004d4510 _$E613
// FUNCTION: STBC 0x004d4520 _$E612
// FUNCTION: STBC 0x004d4530 _$E544
// FUNCTION: STBC 0x004d4540 _$E543
// FUNCTION: STBC 0x004d4550 _$E66
// FUNCTION: STBC 0x004d4560 _$E65
// FUNCTION: STBC 0x004d4570 _$E557
// FUNCTION: STBC 0x004d4580 _$E556
// FUNCTION: STBC 0x004d4590 _$E234
// FUNCTION: STBC 0x004d45a0 _$E233
// FUNCTION: STBC 0x004d45b0 _$E561
// FUNCTION: STBC 0x004d45c0 _$E560
// FUNCTION: STBC 0x004d45d0 _$E564
// FUNCTION: STBC 0x004d45e0 _$E563
// FUNCTION: STBC 0x004d45f0 _$E567
// FUNCTION: STBC 0x004d4600 _$E566
// FUNCTION: STBC 0x004d4610 _$E577
// FUNCTION: STBC 0x004d4620 _$E576
// FUNCTION: STBC 0x004d4630 _$E585
// FUNCTION: STBC 0x004d4640 _$E584
// FUNCTION: STBC 0x004d4650 _$E589
// FUNCTION: STBC 0x004d4660 _$E588
// FUNCTION: STBC 0x004d4670 _$E596
// FUNCTION: STBC 0x004d4680 _$E595
// FUNCTION: STBC 0x004d4690 _$E581
// FUNCTION: STBC 0x004d46a0 _$E580
// FUNCTION: STBC 0x004d46b0 _$E77
// FUNCTION: STBC 0x004d46c0 _$E76
// FUNCTION: STBC 0x004d46d0 _$E609
// FUNCTION: STBC 0x004d46e0 _$E608
// FUNCTION: STBC 0x004d46f0 _$E81
// FUNCTION: STBC 0x004d4700 _$E80
// FUNCTION: STBC 0x004d4710 _$E85
// FUNCTION: STBC 0x004d4720 _$E84
// FUNCTION: STBC 0x004d4730 _$E605
// FUNCTION: STBC 0x004d4740 _$E604
// FUNCTION: STBC 0x004d4750 _$E89
// FUNCTION: STBC 0x004d4760 _$E88
// FUNCTION: STBC 0x004d4770 _$E206
// FUNCTION: STBC 0x004d4780 _$E205
// FUNCTION: STBC 0x004d4790 _$E617
// FUNCTION: STBC 0x004d47a0 _$E616
// FUNCTION: STBC 0x004d47b0 _$E621
// FUNCTION: STBC 0x004d47c0 _$E620
// FUNCTION: STBC 0x004d47d0 _$E628
// FUNCTION: STBC 0x004d47e0 _$E627
// FUNCTION: STBC 0x004d47f0 _$E631
// FUNCTION: STBC 0x004d4800 _$E630
// FUNCTION: STBC 0x004d4810 _$E635
// FUNCTION: STBC 0x004d4820 _$E634
// FUNCTION: STBC 0x004d4830 _$E639
// FUNCTION: STBC 0x004d4840 _$E638
// FUNCTION: STBC 0x004d50d0 _$E148
// FUNCTION: STBC 0x004d50e0 _$E147
// FUNCTION: STBC 0x004d50f0 _$E161
// FUNCTION: STBC 0x004d5100 _$E160
// FUNCTION: STBC 0x004d5110 _$E211
// FUNCTION: STBC 0x004d5120 _$E210
// FUNCTION: STBC 0x004d5130 _$E239
// FUNCTION: STBC 0x004d5140 _$E238
// FUNCTION: STBC 0x004d5150 _$E215
// FUNCTION: STBC 0x004d5160 _$E214
// FUNCTION: STBC 0x004d5180 _$E243
// FUNCTION: STBC 0x004d5190 _$E242
// FUNCTION: STBC 0x004d51a0 _$E250
// FUNCTION: STBC 0x004d51b0 _$E249
// FUNCTION: STBC 0x004d51c0 _$E315
// FUNCTION: STBC 0x004d51d0 _$E314
// FUNCTION: STBC 0x004d51e0 _$E319
// FUNCTION: STBC 0x004d51f0 _$E318
// FUNCTION: STBC 0x004d5200 _$E106
// FUNCTION: STBC 0x004d5210 _$E105
// FUNCTION: STBC 0x004d5220 _$E280
// FUNCTION: STBC 0x004d5230 _$E279
// FUNCTION: STBC 0x004d5240 _$E287
// FUNCTION: STBC 0x004d5250 _$E286
// FUNCTION: STBC 0x004d5260 _$E291
// FUNCTION: STBC 0x004d5270 _$E290
// FUNCTION: STBC 0x004d5280 _$E295
// FUNCTION: STBC 0x004d5290 _$E294
// FUNCTION: STBC 0x004d52a0 _$E299
// FUNCTION: STBC 0x004d52b0 _$E298
// FUNCTION: STBC 0x004d52c0 _$E305
// FUNCTION: STBC 0x004d52d0 _$E304
// FUNCTION: STBC 0x004d52e0 _$E323
// FUNCTION: STBC 0x004d52f0 _$E322
// FUNCTION: STBC 0x004d5300 _$E327
// FUNCTION: STBC 0x004d5310 _$E326
// FUNCTION: STBC 0x004d5320 _$E331
// FUNCTION: STBC 0x004d5330 _$E330
// FUNCTION: STBC 0x004d5340 _$E335
// FUNCTION: STBC 0x004d5350 _$E334
// FUNCTION: STBC 0x004d5360 _$E339
// FUNCTION: STBC 0x004d5370 _$E338
// FUNCTION: STBC 0x004d5380 _$E343
// FUNCTION: STBC 0x004d5390 _$E342
// FUNCTION: STBC 0x004d53e0 _$E271
// FUNCTION: STBC 0x004d53f0 _$E270
// FUNCTION: STBC 0x004d5400 _$E347
// FUNCTION: STBC 0x004d5410 _$E346
// FUNCTION: STBC 0x004d5420 _$E351
// FUNCTION: STBC 0x004d5430 _$E350
// FUNCTION: STBC 0x004d5440 _$E355
// FUNCTION: STBC 0x004d5450 _$E354
// FUNCTION: STBC 0x004d5460 _$E111
// FUNCTION: STBC 0x004d5470 _$E110
// FUNCTION: STBC 0x004d5480 _$E359
// FUNCTION: STBC 0x004d5490 _$E358
// FUNCTION: STBC 0x004d54a0 _$E116
// FUNCTION: STBC 0x004d54b0 _$E115
// FUNCTION: STBC 0x004d54c0 _$E363
// FUNCTION: STBC 0x004d54d0 _$E362
// FUNCTION: STBC 0x004d54e0 _$E367
// FUNCTION: STBC 0x004d54f0 _$E366
// FUNCTION: STBC 0x004d5500 _$E371
// FUNCTION: STBC 0x004d5510 _$E370
// FUNCTION: STBC 0x004d5520 _$E375
// FUNCTION: STBC 0x004d5530 _$E374
// FUNCTION: STBC 0x004d5540 _$E125
// FUNCTION: STBC 0x004d5550 _$E124
// FUNCTION: STBC 0x004d5560 _$E379
// FUNCTION: STBC 0x004d5570 _$E378
// FUNCTION: STBC 0x004d5580 _$E383
// FUNCTION: STBC 0x004d5590 _$E382
// FUNCTION: STBC 0x004d55a0 _$E387
// FUNCTION: STBC 0x004d55b0 _$E386
// FUNCTION: STBC 0x004d55c0 _$E131
// FUNCTION: STBC 0x004d55d0 _$E130
// FUNCTION: STBC 0x004d55e0 _$E136
// FUNCTION: STBC 0x004d55f0 _$E135
// FUNCTION: STBC 0x004d5600 _$E166
// FUNCTION: STBC 0x004d5610 _$E165
// FUNCTION: STBC 0x004d5620 _$E391
// FUNCTION: STBC 0x004d5630 _$E390
// FUNCTION: STBC 0x004d5640 _$E171
// FUNCTION: STBC 0x004d5650 _$E170
// FUNCTION: STBC 0x004d5660 _$E395
// FUNCTION: STBC 0x004d5670 _$E394
// FUNCTION: STBC 0x004d5680 _$E399
// FUNCTION: STBC 0x004d5690 _$E398
// FUNCTION: STBC 0x004d56a0 _$E176
// FUNCTION: STBC 0x004d56b0 _$E175
// FUNCTION: STBC 0x004d56c0 _$E403
// FUNCTION: STBC 0x004d56d0 _$E402
// FUNCTION: STBC 0x004d56e0 _$E141
// FUNCTION: STBC 0x004d56f0 _$E140
// FUNCTION: STBC 0x004d5700 _$E407
// FUNCTION: STBC 0x004d5710 _$E406
// FUNCTION: STBC 0x004d5720 _$E411
// FUNCTION: STBC 0x004d5730 _$E410
// FUNCTION: STBC 0x004d5740 _$E181
// FUNCTION: STBC 0x004d5750 _$E180
// FUNCTION: STBC 0x004d5760 _$E186
// FUNCTION: STBC 0x004d5770 _$E185
// FUNCTION: STBC 0x004d5780 _$E415
// FUNCTION: STBC 0x004d5790 _$E414
// FUNCTION: STBC 0x004d57a0 _$E419
// FUNCTION: STBC 0x004d57b0 _$E418
// FUNCTION: STBC 0x004d57c0 _$E423
// FUNCTION: STBC 0x004d57d0 _$E422
// FUNCTION: STBC 0x004d57e0 _$E427
// FUNCTION: STBC 0x004d57f0 _$E426
// FUNCTION: STBC 0x004d5800 _$E431
// FUNCTION: STBC 0x004d5810 _$E430
// FUNCTION: STBC 0x004d5820 _$E435
// FUNCTION: STBC 0x004d5830 _$E434
// FUNCTION: STBC 0x004d5840 _$E439
// FUNCTION: STBC 0x004d5850 _$E438
// FUNCTION: STBC 0x004d5860 _$E443
// FUNCTION: STBC 0x004d5870 _$E442
// FUNCTION: STBC 0x004d5880 _$E191
// FUNCTION: STBC 0x004d5890 _$E190
// FUNCTION: STBC 0x004d58a0 _$E447
// FUNCTION: STBC 0x004d58b0 _$E446
// FUNCTION: STBC 0x004d58c0 _$E196
// FUNCTION: STBC 0x004d58d0 _$E195
// FUNCTION: STBC 0x004d58e0 _$E201
// FUNCTION: STBC 0x004d58f0 _$E200
// FUNCTION: STBC 0x004d5930 _$E643
// FUNCTION: STBC 0x004d5940 _$E642
// FUNCTION: STBC 0x004d5950 _$E648
// FUNCTION: STBC 0x004d5960 _$E647
// FUNCTION: STBC 0x004d5970 _$E264
// FUNCTION: STBC 0x004d5980 _$E263

// The number of the next flash's name, "Warp Flash %d" (invented name).
// DATA: STBC 0x009856fc
int WarpFlash::flashCount;

/*
================
WarpFlash::WarpFlash

At the ship: in its set, at its location and rotation, as large as its
node's bound.
================
*/
// FUNCTION: STBC 0x004d4850 ??0WarpFlash@@QAE@PAVBaseObjectClass@@M@Z
WarpFlash::WarpFlash( BaseObjectClass *ship, float time ) {
	SetClass *set = ship->GetContainingSet();
	float radius = ship->GetNode()->GetWorldBound().GetRadius();
	Init( set, &ship->GetWorldLocation(), &ship->GetWorldRotation(), radius, time );
}

/*
================
WarpFlash::WarpFlash
================
*/
// FUNCTION: STBC 0x004d48e0 ??0WarpFlash@@QAE@PAVSetClass@@PAVTGPoint3@@PAVTGMatrix3@@MM@Z
WarpFlash::WarpFlash( SetClass *set, TGPoint3 *location, TGMatrix3 *rotation, float radius, float time ) {
	Init( set, location, rotation, radius, time );
}

/*
================
WarpFlash::Init
================
*/
// FUNCTION: STBC 0x004d4950
void WarpFlash::Init( SetClass *set, const TGPoint3 *location, const TGMatrix3 *rotation, float radius, float time ) {
	startTime = g_kUtopiaModule.GetGameTime();
	endTime = startTime + time;
	unknown70 = radius * 5.0f;
	NiNode *flash = CreateFlash( time );
	SetNiObject( flash );
	SetTranslate( location );

	char name[128];
	sprintf( name, "Warp Flash %d", flashCount++ );
	set->AddObjectToSet( this, name );

	TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
	if ( region != NULL ) {
		TGSound *sound = g_kSoundManager.GetSound( "Warp Flash" );
		if ( sound != NULL ) {
			sound->AttachToNode( node );
			region->AddSound( sound );
			sound->Play( true );
		}
	}
}

/*
================
WarpFlash::~WarpFlash
================
*/
// FUNCTION: STBC 0x004d4a30
WarpFlash::~WarpFlash( void ) {
}

/*
================
WarpFlash::CreateFlash
================
*/
// FUNCTION: STBC 0x004d4a40
// PARKED: complete, call set as retail's; loop strength reduction: ours reduces the reverse loop's count - 1 - i to a decremented stack temporary (frame 4 bytes larger), retail recomputes count - i - 1 each pass
NiNode *WarpFlash::CreateFlash( float time ) {
	NiBillboardNode *node = new NiBillboardNode;

	NiPoint3 *vertices = new NiPoint3[4];
	vertices[0] = NiPoint3( -( unknown70 * 0.5f ), -unknown70, 0.0f );
	vertices[1] = NiPoint3( unknown70 * 0.5f, -unknown70, 0.0f );
	vertices[2] = NiPoint3( unknown70 * 0.5f, unknown70, 0.0f );
	vertices[3] = NiPoint3( -( unknown70 * 0.5f ), unknown70, 0.0f );

	NiPoint2 *textures = new NiPoint2[4];
	textures[0] = NiPoint2( 0.0f, 1.0f );
	textures[1] = NiPoint2( 1.0f, 1.0f );
	textures[2] = NiPoint2( 1.0f, 0.0f );
	textures[3] = NiPoint2( 0.0f, 0.0f );

	unsigned short *triangles = new unsigned short[6];
	triangles[0] = 0;
	triangles[1] = 1;
	triangles[2] = 2;
	triangles[3] = 0;
	triangles[4] = 2;
	triangles[5] = 3;

	NiTriShape *quad = new NiTriShape( 4, vertices, NULL, NULL, textures, 2, triangles );
	quad->CalculateNormals();

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	quad->AttachProperty( alpha );

	node->SetMode( NiBillboardNode::RIGID_FACE_CAMERA );
	node->AttachChild( quad, false );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	quad->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	quad->AttachProperty( zBuffer );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	quad->AttachProperty( textureMode );

	NiTextureProperty *texture = new NiTextureProperty;
	quad->AttachProperty( texture );

	NiFlipController *controller = new NiFlipController;
	TGIconGroup *group = g_kIconManager.GetIconGroup( "WarpFlashTextures" );
	unsigned int count = group->GetTextures()->GetSize();
	unsigned int i;
	for ( i = 0; i < count; i++ ) {
		controller->SetImage( group->GetIconTexture( i )->GetImage(), i );
	}
	for ( i = 0; i < count; i++ ) {
		controller->SetImage( group->GetIconTexture( count - 1 - i )->GetImage(), count + i );
	}
	controller->SetAnimType( NiTimeController::APP_INIT );
	controller->SetSecondsPerFrame( time / (float)( count * 2 ) );
	controller->SetCycleType( NiTimeController::LOOP );
	controller->SetTarget( texture );
	controller->SetStartTime( startTime );
	controller->Start( startTime );
	controller->SetBeginKeyTime( 0.0f );
	controller->hiKeyTime = controller->GetSecondsPerFrame() * controller->GetImageCount();

	node->UpdateProperties();
	quad->GetShapeData()->IncRevisionID();
	return node;
}

/*
================
WarpFlash::Update

Runs the base object's update, then ends the flash once the time runs past
its end time.
================
*/
// FUNCTION: STBC 0x004d4fc0
void WarpFlash::Update( float time ) {
	ObjectClass::Update( time );
	if ( time > endTime ) {
		SetDeleteMe( true );
	}
}

/*
================
WarpFlash::WriteToStream

Writes the base object's data, then the flash's start and end times and
unknown70.
================
*/
// FUNCTION: STBC 0x004d4ff0
bool WarpFlash::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( startTime );
	stream->WriteFloat( endTime );
	stream->WriteFloat( unknown70 );
	return true;
}

/*
================
WarpFlash::ReadFromStream

Reads the base object's data and the flash's times and unknown70, then
builds the flash again for the time that was left.
================
*/
// FUNCTION: STBC 0x004d5030
bool WarpFlash::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	startTime = stream->ReadFloat();
	endTime = stream->ReadFloat();
	unknown70 = stream->ReadFloat();
	float timeLeft = endTime - startTime;
	NiNode *flash = CreateFlash( timeLeft );
	SetNiObject( flash );
	return true;
}

/*
================
WarpFlash::SaveStatics

Writes the flash count for the saved game. The name is invented, after
LensFlare::SaveStatics.
================
*/
// FUNCTION: STBC 0x004d5090
bool WarpFlash::SaveStatics( TGStream *stream ) {
	stream->WriteInt( flashCount );
	return true;
}

/*
================
WarpFlash::LoadStatics

Reads the flash count back. The name is invented, after
LensFlare::LoadStatics.
================
*/
// FUNCTION: STBC 0x004d50b0
bool WarpFlash::LoadStatics( TGStream *stream ) {
	flashCount = stream->ReadInt();
	return true;
}
