/*
===============================================================================

	BridgeObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "BridgeObjectClass.h"
#include "CharacterClass.h"
#include "Game.h"
#include "ShipClass.h"
#include "ExplodeEmitterProperty.h"
#include "SmokeEmitterProperty.h"
#include "SparkEmitterProperty.h"
#include "EffectEmitterProperty.h"
#include "NiMath.h"
#include "NiNode.h"
#include "PointParticleController.h"
#include "PositionOrientationProperty.h"
#include "TGModelPropertyInstance.h"
#include "TGModelPropertyList.h"
#include "TGModelPropertySet.h"
#include "TGSystemWrapperClass.h"
#include "NiMaterialProperty.h"
#include "SetClass.h"
#include "SetManager.h"
#include "CharacterClass.h"
#include "ModelHelpers.h"
#include "ImageUtils.h"
#include "PyEmbed.h"
#include "TGDimmerController.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGIntEvent.h"
#include "TGModelManager.h"
#include "TGObjPtrEvent.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include "UtopiaApp.h"
#include "WeaponHitEvent.h"
#include "ZoomCameraObjectClass.h"
#include "ImageManagerClass.h"
#include "NiAmbientLight.h"
#include "NiDynamicEffect.h"
#include "NiImageFile.h"
#include "NiMultiTextureProperty.h"
#include "NiRawImageData.h"
#include "NiTriShape.h"
#include "NiVertexColorProperty.h"
#include "renderer/NiImage.h"
#include "NiMain.h"
#include "NiAnimation.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00660870 _$E32
// FUNCTION: STBC 0x00660880 _$E29
// FUNCTION: STBC 0x00660890 _$E31
// FUNCTION: STBC 0x006608a0 _$E30
// FUNCTION: STBC 0x006608b0 _$E38
// FUNCTION: STBC 0x006608c0 _$E35
// FUNCTION: STBC 0x006608d0 _$E37
// FUNCTION: STBC 0x006608e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00660690 _$E302
// FUNCTION: STBC 0x006606a0 _$E301
// FUNCTION: STBC 0x006606b0 _$E339
// FUNCTION: STBC 0x006606c0 _$E338
// FUNCTION: STBC 0x006606d0 _$E343
// FUNCTION: STBC 0x006606e0 _$E342
// FUNCTION: STBC 0x006606f0 _$E351
// FUNCTION: STBC 0x00660700 _$E350
// FUNCTION: STBC 0x00660710 _$E305
// FUNCTION: STBC 0x00660720 _$E304
// FUNCTION: STBC 0x00660730 _$E531
// FUNCTION: STBC 0x00660740 _$E530
// FUNCTION: STBC 0x00660750 _$E554
// FUNCTION: STBC 0x00660760 _$E553
// FUNCTION: STBC 0x00660770 _$E545
// FUNCTION: STBC 0x00660780 _$E544
// FUNCTION: STBC 0x00660790 _$E558
// FUNCTION: STBC 0x006607a0 _$E557
// FUNCTION: STBC 0x006607b0 _$E562
// FUNCTION: STBC 0x006607c0 _$E561
// FUNCTION: STBC 0x006607d0 _$E623
// FUNCTION: STBC 0x006607e0 _$E622
// FUNCTION: STBC 0x006607f0 _$E626
// FUNCTION: STBC 0x00660800 _$E625
// FUNCTION: STBC 0x00660810 _$E583
// FUNCTION: STBC 0x00660820 _$E582
// FUNCTION: STBC 0x00660830 _$E599
// FUNCTION: STBC 0x00660840 _$E598
// FUNCTION: STBC 0x00660850 _$E566
// FUNCTION: STBC 0x00660860 _$E565
// FUNCTION: STBC 0x006608f0 _$E606
// FUNCTION: STBC 0x00660900 _$E605
// FUNCTION: STBC 0x00660910 _$E609
// FUNCTION: STBC 0x00660920 _$E608
// FUNCTION: STBC 0x00660930 _$E645
// FUNCTION: STBC 0x00660940 _$E644
// FUNCTION: STBC 0x00660950 _$E569
// FUNCTION: STBC 0x00660960 _$E568
// FUNCTION: STBC 0x00660970 _$E572
// FUNCTION: STBC 0x00660980 _$E571
// FUNCTION: STBC 0x00660990 _$E575
// FUNCTION: STBC 0x006609a0 _$E574
// FUNCTION: STBC 0x006609b0 _$E290
// FUNCTION: STBC 0x006609c0 _$E289
// FUNCTION: STBC 0x006609d0 _$E579
// FUNCTION: STBC 0x006609e0 _$E578
// FUNCTION: STBC 0x006609f0 _$E612
// FUNCTION: STBC 0x00660a00 _$E611
// FUNCTION: STBC 0x00660a10 _$E587
// FUNCTION: STBC 0x00660a20 _$E586
// FUNCTION: STBC 0x00660a30 _$E591
// FUNCTION: STBC 0x00660a40 _$E590
// FUNCTION: STBC 0x00660a50 _$E66
// FUNCTION: STBC 0x00660a60 _$E65
// FUNCTION: STBC 0x00660a70 _$E595
// FUNCTION: STBC 0x00660a80 _$E594
// FUNCTION: STBC 0x00660a90 _$E360
// FUNCTION: STBC 0x00660aa0 _$E359
// FUNCTION: STBC 0x00660ab0 _$E653
// FUNCTION: STBC 0x00660ac0 _$E652
// FUNCTION: STBC 0x00660ad0 _$E678
// FUNCTION: STBC 0x00660ae0 _$E677
// FUNCTION: STBC 0x00660af0 _$E666
// FUNCTION: STBC 0x00660b00 _$E665
// FUNCTION: STBC 0x00660b10 _$E603
// FUNCTION: STBC 0x00660b20 _$E602
// FUNCTION: STBC 0x00660b30 _$E616
// FUNCTION: STBC 0x00660b40 _$E615
// FUNCTION: STBC 0x00660b50 _$E77
// FUNCTION: STBC 0x00660b60 _$E76
// FUNCTION: STBC 0x00660b70 _$E620
// FUNCTION: STBC 0x00660b80 _$E619
// FUNCTION: STBC 0x00660b90 _$E293
// FUNCTION: STBC 0x00660ba0 _$E292
// FUNCTION: STBC 0x00660bb0 _$E81
// FUNCTION: STBC 0x00660bc0 _$E80
// FUNCTION: STBC 0x00660bd0 _$E630
// FUNCTION: STBC 0x00660be0 _$E629
// FUNCTION: STBC 0x00660bf0 _$E638
// FUNCTION: STBC 0x00660c00 _$E637
// FUNCTION: STBC 0x00660c10 _$E85
// FUNCTION: STBC 0x00660c20 _$E84
// FUNCTION: STBC 0x00660c30 _$E642
// FUNCTION: STBC 0x00660c40 _$E641
// FUNCTION: STBC 0x00660c50 _$E89
// FUNCTION: STBC 0x00660c60 _$E88
// FUNCTION: STBC 0x00660c70 _$E649
// FUNCTION: STBC 0x00660c80 _$E648
// FUNCTION: STBC 0x00660c90 _$E206
// FUNCTION: STBC 0x00660ca0 _$E205
// FUNCTION: STBC 0x00660cb0 _$E634
// FUNCTION: STBC 0x00660cc0 _$E633
// FUNCTION: STBC 0x00660cd0 _$E662
// FUNCTION: STBC 0x00660ce0 _$E661
// FUNCTION: STBC 0x00660cf0 _$E658
// FUNCTION: STBC 0x00660d00 _$E657
// FUNCTION: STBC 0x00660d10 _$E670
// FUNCTION: STBC 0x00660d20 _$E669
// FUNCTION: STBC 0x00660d30 _$E674
// FUNCTION: STBC 0x00660d40 _$E673
// FUNCTION: STBC 0x00660d50 _$E681
// FUNCTION: STBC 0x00660d60 _$E680
// FUNCTION: STBC 0x00660d70 _$E684
// FUNCTION: STBC 0x00660d80 _$E683
// FUNCTION: STBC 0x00660d90 _$E148
// FUNCTION: STBC 0x00660da0 _$E147
// FUNCTION: STBC 0x00660db0 _$E688
// FUNCTION: STBC 0x00660dc0 _$E687
// FUNCTION: STBC 0x00660dd0 _$E692
// FUNCTION: STBC 0x00660de0 _$E691
// FUNCTION: STBC 0x00660df0 _$E161
// FUNCTION: STBC 0x00660e00 _$E160
// FUNCTION: STBC 0x00660e10 _$E211
// FUNCTION: STBC 0x00660e20 _$E210
// FUNCTION: STBC 0x00660e30 _$E365
// FUNCTION: STBC 0x00660e40 _$E364
// FUNCTION: STBC 0x00660e50 _$E215
// FUNCTION: STBC 0x00660e60 _$E214
// FUNCTION: STBC 0x00660e70 _$E369
// FUNCTION: STBC 0x00660e80 _$E368
// FUNCTION: STBC 0x00660e90 _$E376
// FUNCTION: STBC 0x00660ea0 _$E375
// FUNCTION: STBC 0x00660eb0 _$E399
// FUNCTION: STBC 0x00660ec0 _$E398
// FUNCTION: STBC 0x00660ed0 _$E403
// FUNCTION: STBC 0x00660ee0 _$E402
// FUNCTION: STBC 0x00660ef0 _$E106
// FUNCTION: STBC 0x00660f00 _$E105
// FUNCTION: STBC 0x00660f10 _$E252
// FUNCTION: STBC 0x00660f20 _$E251
// FUNCTION: STBC 0x00660f30 _$E259
// FUNCTION: STBC 0x00660f40 _$E258
// FUNCTION: STBC 0x00660f50 _$E263
// FUNCTION: STBC 0x00660f60 _$E262
// FUNCTION: STBC 0x00660f70 _$E267
// FUNCTION: STBC 0x00660f80 _$E266
// FUNCTION: STBC 0x00660f90 _$E271
// FUNCTION: STBC 0x00660fa0 _$E270
// FUNCTION: STBC 0x00660fb0 _$E277
// FUNCTION: STBC 0x00660fc0 _$E276
// FUNCTION: STBC 0x00660fd0 _$E347
// FUNCTION: STBC 0x00660fe0 _$E346
// FUNCTION: STBC 0x00660ff0 _$E389
// FUNCTION: STBC 0x00661000 _$E388
// FUNCTION: STBC 0x00661010 _$E407
// FUNCTION: STBC 0x00661020 _$E406
// FUNCTION: STBC 0x006610b0 _$E411
// FUNCTION: STBC 0x006610c0 _$E410
// FUNCTION: STBC 0x00661290 _$E415
// FUNCTION: STBC 0x006612a0 _$E414
// FUNCTION: STBC 0x006613d0 _$E419
// FUNCTION: STBC 0x006613e0 _$E418
// FUNCTION: STBC 0x00661770 _$E423
// FUNCTION: STBC 0x00661780 _$E422
// FUNCTION: STBC 0x00661790 _$E427
// FUNCTION: STBC 0x006617a0 _$E426
// FUNCTION: STBC 0x006619c0 _$E431
// FUNCTION: STBC 0x006619d0 _$E430
// FUNCTION: STBC 0x00662420 _$E435
// FUNCTION: STBC 0x00662430 _$E434
// FUNCTION: STBC 0x00662440 _$E111
// FUNCTION: STBC 0x00662450 _$E110
// FUNCTION: STBC 0x00662600 _$E439
// FUNCTION: STBC 0x00662610 _$E438
// FUNCTION: STBC 0x00662620 _$E116
// FUNCTION: STBC 0x00662630 _$E115
// FUNCTION: STBC 0x006627e0 _$E443
// FUNCTION: STBC 0x006627f0 _$E442
// FUNCTION: STBC 0x00662800 _$E447
// FUNCTION: STBC 0x00662810 _$E446
// FUNCTION: STBC 0x00662a20 _$E451
// FUNCTION: STBC 0x00662a30 _$E450
// FUNCTION: STBC 0x00662c60 _$E455
// FUNCTION: STBC 0x00662c70 _$E454
// FUNCTION: STBC 0x006631b0 _$E125
// FUNCTION: STBC 0x006631c0 _$E124
// FUNCTION: STBC 0x00663560 _$E459
// FUNCTION: STBC 0x00663570 _$E458
// FUNCTION: STBC 0x006635c0 _$E463
// FUNCTION: STBC 0x006635d0 _$E462
// FUNCTION: STBC 0x00663770 _$E467
// FUNCTION: STBC 0x00663780 _$E466
// FUNCTION: STBC 0x00663a20 _$E131
// FUNCTION: STBC 0x00663a30 _$E130
// FUNCTION: STBC 0x00663d90 _$E136
// FUNCTION: STBC 0x00663da0 _$E135
// FUNCTION: STBC 0x006641a0 _$E166
// FUNCTION: STBC 0x006641b0 _$E165
// FUNCTION: STBC 0x00664d10 _$E471
// FUNCTION: STBC 0x00664d20 _$E470
// FUNCTION: STBC 0x00664d30 _$E171
// FUNCTION: STBC 0x00664d40 _$E170
// FUNCTION: STBC 0x00664d50 _$E475
// FUNCTION: STBC 0x00664d60 _$E474
// FUNCTION: STBC 0x00664d70 _$E479
// FUNCTION: STBC 0x00664d80 _$E478
// FUNCTION: STBC 0x00664d90 _$E176
// FUNCTION: STBC 0x00664da0 _$E175
// FUNCTION: STBC 0x00664db0 _$E483
// FUNCTION: STBC 0x00664dc0 _$E482
// FUNCTION: STBC 0x00664dd0 _$E141
// FUNCTION: STBC 0x00664de0 _$E140
// FUNCTION: STBC 0x00664df0 _$E487
// FUNCTION: STBC 0x00664e00 _$E486
// FUNCTION: STBC 0x00664e10 _$E491
// FUNCTION: STBC 0x00664e20 _$E490
// FUNCTION: STBC 0x00664e30 _$E181
// FUNCTION: STBC 0x00664e40 _$E180
// FUNCTION: STBC 0x00664e50 _$E186
// FUNCTION: STBC 0x00664e60 _$E185
// FUNCTION: STBC 0x00664e70 _$E495
// FUNCTION: STBC 0x00664e80 _$E494
// FUNCTION: STBC 0x00664e90 _$E499
// FUNCTION: STBC 0x00664ea0 _$E498
// FUNCTION: STBC 0x00664eb0 _$E503
// FUNCTION: STBC 0x00664ec0 _$E502
// FUNCTION: STBC 0x00664ed0 _$E507
// FUNCTION: STBC 0x00664ee0 _$E506
// FUNCTION: STBC 0x00664ef0 _$E511
// FUNCTION: STBC 0x00664f00 _$E510
// FUNCTION: STBC 0x00664f10 _$E515
// FUNCTION: STBC 0x00664f20 _$E514
// FUNCTION: STBC 0x00664f30 _$E519
// FUNCTION: STBC 0x00664f40 _$E518
// FUNCTION: STBC 0x00664f50 _$E523
// FUNCTION: STBC 0x00664f60 _$E522
// FUNCTION: STBC 0x00664f70 _$E191
// FUNCTION: STBC 0x00664f80 _$E190
// FUNCTION: STBC 0x00664f90 _$E527
// FUNCTION: STBC 0x00664fa0 _$E526
// FUNCTION: STBC 0x00664fd0 _$E196
// FUNCTION: STBC 0x00664fe0 _$E195
// FUNCTION: STBC 0x00664ff0 _$E201
// FUNCTION: STBC 0x00665000 _$E200
// FUNCTION: STBC 0x00665810 _$E696
// FUNCTION: STBC 0x00665820 _$E695
// FUNCTION: STBC 0x00665830 _$E701
// FUNCTION: STBC 0x00665840 _$E700
// FUNCTION: STBC 0x00665880 _$E705
// FUNCTION: STBC 0x00665890 _$E704

// The class handler table's construction and its destruction at exit.
// FUNCTION: STBC 0x00661030 _$E711
// FUNCTION: STBC 0x00661040 _$E707
// FUNCTION: STBC 0x00661050 _$E710
// DATA: STBC 0x00991928
TGEventHandlerTable BridgeObjectClass::classHandlers;

// A light level target nearer than this to the current level needs no fade
// (Unknown6638C0). A variable, not a constant: retail reads it from the unit's
// .data, before the detail directories (invented name).
// DATA: STBC 0x00956fa4
static float lightLevelTolerance = 0.01f;

// The texture directories of the image details, under the model's own
// (ReadFromStream).
// DATA: STBC 0x00956fa8
static char *detailDirectories[] = { "Low/", "Medium/", "High/" };

/*
================
BridgeObjectClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00661080
void BridgeObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, BRIDGEOBJECTCLASS_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
BridgeObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x006610d0
void BridgeObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, BRIDGEOBJECTCLASS_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
BridgeObjectClass::GetClassObjects

The set's bridge objects and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00661100
BridgeObjectClass **BridgeObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (BridgeObjectClass **)set->objectTable.GetObjects( BRIDGEOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
BridgeObjectClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x00661130
int BridgeObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( BRIDGEOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
BridgeObjectClass::GetObjectByID

The bridge object with the given ID in the set, or among every object when
the set is NULL; NULL when it is none or not a BridgeObjectClass.
================
*/
// FUNCTION: STBC 0x00661150
BridgeObjectClass *BridgeObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( BRIDGEOBJECTCLASS_TYPE ) ) {
			return (BridgeObjectClass *)object;
		}
		return NULL;
	}
	return (BridgeObjectClass *)set->GetClassObject( BRIDGEOBJECTCLASS_TYPE, id );
}

/*
================
BridgeObjectClass::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x006611c0
BridgeObjectClass *BridgeObjectClass::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( BRIDGEOBJECTCLASS_TYPE, name );
		}
		return (BridgeObjectClass *)object;
	}
	return (BridgeObjectClass *)set->GetClassObject( BRIDGEOBJECTCLASS_TYPE, name );
}

/*
================
BridgeObjectClass::Cast

The object as a BridgeObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00661220
BridgeObjectClass *BridgeObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( BRIDGEOBJECTCLASS_TYPE ) ) {
		return (BridgeObjectClass *)object;
	}
	return NULL;
}
/*
================
BridgeObjectClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00661250
void BridgeObjectClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BridgeObjectClass::CreateEffectHandler, "BridgeObjectClass::CreateEffectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BridgeObjectClass::HandleChangeAlertLevel, "BridgeObjectClass::HandleChangeAlertLevel" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BridgeObjectClass::PlayerChanged, "BridgeObjectClass::PlayerChanged" );
}

/*
================
BridgeObjectClass::RegisterHandlers

The class adds no handlers to its table: each object listens to the
player's ship itself.
================
*/
// FUNCTION: STBC 0x006612b0
void BridgeObjectClass::RegisterHandlers( void ) {
}

/*
================
BridgeObjectClass::PlayerChanged

The hit and alert handlers move from the old player's ship to the new
one's.
================
*/
// FUNCTION: STBC 0x006612c0
void BridgeObjectClass::PlayerChanged( TGEvent *event ) {
	TGEventHandlerObject *oldPlayer = (TGEventHandlerObject *)event->GetSource();
	TGEventHandlerObject *newPlayer = (TGEventHandlerObject *)( (TGObjPtrEvent *)event )->GetObjPtr();
	g_kEventManager.RemoveBroadcastHandler( ET_WEAPON_HIT, this, "BridgeObjectClass::CreateEffectHandler", oldPlayer );
	g_kEventManager.RemoveBroadcastHandler( ET_SET_ALERT_LEVEL, this, "BridgeObjectClass::HandleChangeAlertLevel", oldPlayer );
	g_kEventManager.AddBroadcastMethodHandler( ET_WEAPON_HIT, this, "BridgeObjectClass::CreateEffectHandler", newPlayer );
	g_kEventManager.AddBroadcastMethodHandler( ET_SET_ALERT_LEVEL, this, "BridgeObjectClass::HandleChangeAlertLevel", newPlayer );
	playerHandlersAdded = true;
}

/*
================
BridgeObjectClass::Create

A bridge object showing a copy of the model file's model; NULL when the
model is not loaded.
================
*/
// FUNCTION: STBC 0x00661360
BridgeObjectClass *BridgeObjectClass::Create( char *modelFile ) {
	TGAnimNode *model = g_kModelManager.CopyModel( modelFile );
	if ( model != NULL ) {
		BridgeObjectClass *bridge = new BridgeObjectClass( model );
		bridge->SetModelName( modelFile );
		return bridge;
	}
	return NULL;
}

/*
================
BridgeObjectClass::SetModelName
================
*/
// FUNCTION: STBC 0x006613f0
void BridgeObjectClass::SetModelName( char *name ) {
	if ( modelName != NULL ) {
		delete modelName;
		modelName = NULL;
	}
	if ( name != NULL ) {
		modelName = new char[strlen( name ) + 1];
		strcpy( modelName, name );
	}
}

/*
================
BridgeObjectClass::BridgeObjectClass

A bridge showing the model, at full light, its panels lit; it follows the
player's ship changes and sets its lighting up.
================
*/
// FUNCTION: STBC 0x00661460 ??0BridgeObjectClass@@QAE@PAVNiAVObject@@@Z
BridgeObjectClass::BridgeObjectClass( NiAVObject *model ) : alertLights( 2 ), alertDimmers( 2 ) {
	unknown84 = 1.0f;
	unknown7C = 0.0f;
	lastHullDamageTime = 0.0f;
	lcarsState = LCARS_ON;
	redAlert = false;
	alertGlow = 0.0f;
	playerHandlersAdded = false;
	modelName = NULL;
	propertySet = new TGModelPropertySet;
	SetNiObject( model );
	node->Update( 0.0f );
	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "BridgeObjectClass::PlayerChanged", Game::GetCurrentGame() );
	SetupLighting();
}

/*
================
BridgeObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x006616c0
int BridgeObjectClass::GetObjType( void ) {
	return BRIDGEOBJECTCLASS_TYPE;
}

/*
================
BridgeObjectClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x006616d0
bool BridgeObjectClass::IsTypeOf( int type ) {
	if ( type == BRIDGEOBJECTCLASS_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
BridgeObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x00661730
char *BridgeObjectClass::GetClassName( void ) {
	return "BridgeObjectClass";
}

/*
================
BridgeObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00661740
char *BridgeObjectClass::GetSwigTypeName( void ) {
	return "_p_BridgeObjectClass";
}

/*
================
BridgeObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00661750
char *BridgeObjectClass::GetPythonClassName( void ) {
	return "BridgeObjectClassPtr";
}

/*
================
BridgeObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00661760
TGEventHandlerTable *BridgeObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
BridgeObjectClass::BridgeObjectClass

The loader's bridge: the stream supplies the model and the property set.
A new one (no ID) follows the player's ship changes.
================
*/
// FUNCTION: STBC 0x006617e0 ??0BridgeObjectClass@@QAE@J@Z
BridgeObjectClass::BridgeObjectClass( long id ) : ObjectClass( id ), alertLights( 2 ), alertDimmers( 2 ) {
	unknown84 = 1.0f;
	unknown7C = 0.0f;
	lastHullDamageTime = 0.0f;
	lcarsState = LCARS_ON;
	redAlert = false;
	alertGlow = 0.0f;
	playerHandlersAdded = false;
	modelName = NULL;
	if ( id == 0 ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "BridgeObjectClass::PlayerChanged", Game::GetCurrentGame() );
	}
	propertySet = NULL;
}

/*
================
BridgeObjectClass::~BridgeObjectClass
================
*/
// FUNCTION: STBC 0x006619e0 ??1BridgeObjectClass@@UAE@XZ
// PARKED: inline budget: retail's DecRefCount carries its assert (cost 68); this source needs 67 or less, so it differs from the original somewhere (the numbers are on the card)
BridgeObjectClass::~BridgeObjectClass( void ) {
	int count = normalTextures.GetSize();
	int i;
	for ( i = 0; i < count; i++ ) {
		delete normalTextures.GetAt( i );
	}
	normalTextures.RemoveAll();

	count = dimMapTextures.GetSize();
	for ( i = 0; i < count; i++ ) {
		delete dimMapTextures.GetAt( i );
	}
	dimMapTextures.RemoveAll();

	count = redOffTextures.GetSize();
	for ( i = 0; i < count; i++ ) {
		delete redOffTextures.GetAt( i );
	}
	redOffTextures.RemoveAll();

	count = blinkingTextures.GetSize();
	for ( i = 0; i < count; i++ ) {
		delete blinkingTextures.GetAt( i );
	}
	blinkingTextures.RemoveAll();

	delete propertySet;
	if ( modelName != NULL ) {
		g_kModelManager.Unrefer( modelName );
		delete modelName;
	}
}

/*
================
BridgeObjectClass::AddPlayerHandlers
================
*/
// FUNCTION: STBC 0x00661de0
void BridgeObjectClass::AddPlayerHandlers( void ) {
	if ( !playerHandlersAdded && Game::GetCurrentGame() != NULL ) {
		ShipClass *player = Game::GetCurrentGame()->GetPlayer();
		g_kEventManager.AddBroadcastMethodHandler( ET_WEAPON_HIT, this, "BridgeObjectClass::CreateEffectHandler", player );
		g_kEventManager.AddBroadcastMethodHandler( ET_SET_ALERT_LEVEL, this, "BridgeObjectClass::HandleChangeAlertLevel", player );
	}
}

/*
================
BridgeObjectClass::SetupLighting

Finds the gamma node and the nodes whose textures the light level and the
red alert rewrite, the dim and LCARS materials, and adds the red alert
light: a dark red ambient light that pulses.
================
*/
// FUNCTION: STBC 0x00661e40
void BridgeObjectClass::SetupLighting( void ) {
	gamma = FindNamedObject( niObject, "Gamma" );

	NiAVObject *object = FindNamedObject( niObject, "Red Off GlowAlpha" );
	if ( object != NULL ) {
		EnableGlowAlphaMaps( object, NULL );
		NiTStringMap<BridgeTexture *> textures;
		CollectTextures( object, textures );
		NiTMapIterator pos = textures.GetFirstPos();
		while ( pos != NULL ) {
			const char *name;
			BridgeTexture *texture;
			textures.GetNext( pos, name, texture );
			redOffTextures.Add( texture );
		}
	}

	object = FindNamedObject( niObject, "Red On GlowAlpha" );
	if ( object != NULL ) {
		EnableGlowAlphaMaps( object, NULL );
	}

	object = FindNamedObject( niObject, "Blinking GlowAlpha" );
	if ( object != NULL ) {
		EnableGlowAlphaMaps( object, NULL );
		NiTStringMap<BridgeTexture *> textures;
		CollectTextures( object, textures );
		NiTMapIterator pos = textures.GetFirstPos();
		while ( pos != NULL ) {
			const char *name;
			BridgeTexture *texture;
			textures.GetNext( pos, name, texture );
			blinkingTextures.Add( texture );
		}
	}

	object = FindNamedObject( niObject, "Dim Maps" );
	if ( object != NULL ) {
		NiTStringMap<BridgeTexture *> textures;
		CollectTextures( object, textures );
		NiTMapIterator pos = textures.GetFirstPos();
		while ( pos != NULL ) {
			const char *name;
			BridgeTexture *texture;
			textures.GetNext( pos, name, texture );
			dimMapTextures.Add( texture );
		}
	}

	dimMaterial = NiDynamicCast( NiMaterialProperty, FindNamedProperty( niObject, "Dim Material", NiMaterialProperty::MATERIAL ) );
	lcarsMaterial = NiDynamicCast( NiMaterialProperty, FindNamedProperty( niObject, "LCARs", NiMaterialProperty::MATERIAL ) );
	if ( gamma ) {
		PrepareGamma( gamma );
	}

	object = FindNamedObject( niObject, "Normal" );
	if ( object != NULL ) {
		NiTStringMap<BridgeTexture *> textures;
		CollectTextures( object, textures );
		NiTMapIterator pos = textures.GetFirstPos();
		while ( pos != NULL ) {
			const char *name;
			BridgeTexture *texture;
			textures.GetNext( pos, name, texture );
			normalTextures.Add( texture );
		}
	}

	NiAmbientLight *light = new NiAmbientLight;
	light->SetAmbientColor( NiColor( 0.3f, 0.0f, 0.0f ) );
	AddAlertLight( light, 1.0f, 1.0f, 0.4f );
	niObject->UpdateProperties();
}

/*
================
BridgeObjectClass::AddAlertLight

The dimmer's keys: off for half the off time, a fade in, on for the on
time, a fade out and off for the other half, looping.
================
*/
// FUNCTION: STBC 0x00662460
void BridgeObjectClass::AddAlertLight( NiDynamicEffect *light, float offTime, float onTime, float fadeTime ) {
	alertLights.AddTail( light );
	NiNode *root = GetNode();
	while ( root->GetParent() != NULL ) {
		root = root->GetParent();
	}
	light->AttachAffectedNode( root );
	root->UpdateEffects();
	GetNode()->AttachChild( light, false );
	light->SetSwitch( false );

	NiFloatKey **keys = new NiFloatKey *[6];
	for ( int i = 0; i < 6; i++ ) {
		keys[i] = new NiFloatKey;
	}
	keys[0]->SetTime( 0.0f );
	keys[0]->SetValue( 0.0f );
	keys[1]->SetTime( offTime * 0.5f );
	keys[1]->SetValue( 0.0f );
	keys[2]->SetTime( keys[1]->GetTime() + fadeTime );
	keys[2]->SetValue( 1.0f );
	keys[3]->SetTime( keys[2]->GetTime() + onTime );
	keys[3]->SetValue( 1.0f );
	keys[4]->SetTime( keys[3]->GetTime() + fadeTime );
	keys[4]->SetValue( 0.0f );
	keys[5]->SetTime( keys[4]->GetTime() + offTime * 0.5f );
	keys[5]->SetValue( 0.0f );

	TGDimmerController *dimmer = new TGDimmerController;
	alertDimmers.AddTail( dimmer );
	dimmer->ReplaceAnim( 6, keys, NiFloatKey::LINKEY );
	dimmer->SetTarget( light );
	dimmer->SetCycleType( NiTimeController::LOOP );
}

/*
================
BridgeObjectClass::PrepareGamma

Keeps a byte copy of each vertex colour of the geometry at and under the
object, in the colours' own block after them, for ApplyGamma to scale.
================
*/
// FUNCTION: STBC 0x00662640
void BridgeObjectClass::PrepareGamma( NiAVObject *object ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	NiColorA *oldColors;
	if ( shape != NULL && ( oldColors = shape->modelData->GetColors() ) != NULL ) {
		int count = shape->GetVertexCount();
		// the colours, then three bytes per vertex
		NiColorA *colors = (NiColorA *)new char[count * ( sizeof( NiColorA ) + 3 )];
		memcpy( colors, oldColors, count * sizeof( NiColorA ) );
		unsigned char *bytes = (unsigned char *)( colors + count );
		for ( int i = 0; i < count; i++ ) {
			bytes[i * 3] = (unsigned char)( oldColors[i].r * 255.0f );
			bytes[i * 3 + 1] = (unsigned char)( oldColors[i].g * 255.0f );
			bytes[i * 3 + 2] = (unsigned char)( oldColors[i].b * 255.0f );
		}
		delete oldColors;
		NiGeometryData *data = shape->modelData;
		unsigned short textureSets = data->GetTextureSets();
		data->SetData( count, data->GetVertices(), data->GetNormals(), colors, data->GetTextures() );
		shape->modelData->SetTextureSets( textureSets );
	}

	NiNode *parent = NiDynamicCast( NiNode, object );
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			if ( parent->GetAt( i ) != NULL ) {
				PrepareGamma( parent->GetAt( i ) );
			}
		}
	}
}

/*
================
BridgeObjectClass::CreateTexture
================
*/
// FUNCTION: STBC 0x00662820
void BridgeObjectClass::CreateTexture( BridgeTexture *&texture, NiImage *image ) {
	NiPointer<NiRawImageData> raw;
	if ( image->GetRawImageData() != NULL ) {
		raw = image->GetRawImageData();
	} else {
		raw = new NiRawImageData;
		if ( !NiReadImageFile( image->GetURL(), raw ) ) {
			texture = NULL;
			return;
		}
		image = NiImage::Create( raw, 0, NiImage::TEX_DEFAULT, 2 );
	}
	unsigned int height = raw->GetHeight();
	unsigned int width = raw->GetWidth();
	NiRawImageData *original = new NiRawImageData( NiRawImageData::FORMAT_RGB, width, height );
	memcpy( original->GetPixels(), raw->GetPixels(), original->GetRowStride() * height );
	texture = new BridgeTexture;
	texture->image = image;
	texture->original = original;
}

/*
================
BridgeObjectClass::CreateAlphaTexture
================
*/
// FUNCTION: STBC 0x00662a40
void BridgeObjectClass::CreateAlphaTexture( BridgeTexture *&texture, NiImage *image ) {
	texture = NULL;
	NiPointer<NiRawImageData> raw;
	if ( image->GetRawImageData() != NULL ) {
		raw = image->GetRawImageData();
	} else {
		raw = new NiRawImageData;
		if ( !NiReadImageFile( image->GetURL(), raw ) ) {
			return;
		}
		image = NiImage::Create( raw, 0, NiImage::TEX_DEFAULT, 2 );
	}
	if ( raw->GetFormat() != NiRawImageData::FORMAT_RGBA ) {
		return;
	}
	unsigned int height = raw->GetHeight();
	unsigned int width = raw->GetWidth();
	NiRawImageData *original = new NiRawImageData( NiRawImageData::FORMAT_RGBA, width, height );
	memcpy( original->GetPixels(), raw->GetPixels(), original->GetRowStride() * height );
	texture = new BridgeTexture;
	texture->image = image;
	texture->original = original;
}

/*
================
BridgeObjectClass::CollectTextures

The texture of a multi-texture property is its dark map's, else its glow
alpha map's, else its glow map's. The property takes the image's file
name, under which the map keeps one texture for every property that shows
the file, and the property's map then shows the texture's image.
================
*/
// FUNCTION: STBC 0x00662c80
// DRAFT: call set as retail (single-statement bodies unbraced); retail calls the string map's hooks virtually in the lookups; the three map branches are otherwise in place
void BridgeObjectClass::CollectTextures( NiAVObject *object, NiTStringMap<BridgeTexture *> &textures ) {
	BridgeTexture *texture;
	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		NiMultiTextureProperty::Map *map = property->GetMap( NiMultiTextureProperty::DARK_INDEX );
		if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL ) {
			property->SetName( map->image->GetURL() );
			if ( !textures.GetAt( property->GetName(), texture ) ) {
				CreateTexture( texture, map->image );
				if ( texture == NULL )
					goto children;
				textures.SetAt( property->GetName(), texture );
			}
		} else {
			map = property->GetMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX );
			if ( map != NULL && map->image != NULL && map->image->GetURL() != NULL ) {
				property->SetName( map->image->GetURL() );
				if ( !textures.GetAt( property->GetName(), texture ) ) {
					CreateAlphaTexture( texture, map->image );
					if ( texture == NULL )
						goto children;
					textures.SetAt( property->GetName(), texture );
				}
			} else {
				map = property->GetMap( NiMultiTextureProperty::GLOW_INDEX );
				if ( map == NULL || map->image == NULL || map->image->GetURL() == NULL )
					goto children;
				property->SetName( map->image->GetURL() );
				if ( !textures.GetAt( property->GetName(), texture ) ) {
					CreateTexture( texture, map->image );
					if ( texture == NULL )
						goto children;
					textures.SetAt( property->GetName(), texture );
				}
			}
		}
		if ( texture != NULL )
			map->SetImage( texture->image );
	}

children:
	if ( object != NULL ) {
		NiNode *parent = NiDynamicCast( NiNode, object );
		if ( parent != NULL ) {
			for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
				if ( parent->GetAt( i ) != NULL )
					CollectTextures( parent->GetAt( i ), textures );
			}
		}
	}
}

/*
================
BridgeObjectClass::Update

Flickers the LCARS panels, pulses the blinking textures with the red alert
dimmer (and fades them out after it), and fades the light level.
================
*/
// FUNCTION: STBC 0x006631d0
void BridgeObjectClass::Update( float time ) {
	float delta;
	if ( lastUpdateTime < 0.0f ) {
		AddPlayerHandlers();
		delta = 0.001f;
	} else
		delta = time - lastUpdateTime;

	if ( lcarsState == LCARS_FLICKERING && lcarsMaterial != NULL ) {
		flickerTime -= delta;
		if ( flickerTime > 0.0f ) {
			float level = rand() * ( 1.0f / RAND_MAX );
			lcarsMaterial->SetEmittance( NiColor( level, level, level ) );
		} else {
			lcarsState = flickerReturnState;
			if ( lcarsState == LCARS_ON )
				lcarsMaterial->SetEmittance( NiColor::WHITE );
			else
				lcarsMaterial->SetEmittance( NiColor::BLACK );
		}
	}

	if ( redAlert ) {
		TGDimmerController *dimmer = NiDynamicCast( TGDimmerController, alertDimmers.GetHead() );
		if ( dimmer != NULL ) {
			float glow = dimmer->GetLevel();
			if ( glow > 1.0f )
				glow = 1.0f;
			else if ( glow < 0.0f )
				glow = 0.0f;
			if ( glow != alertGlow ) {
				unsigned char table[256];
				BuildScaleTable( table, glow );
				ApplyAlphaTable( &blinkingTextures, table );
				alertGlow = glow;
			}
		}
	} else if ( alertGlow > 0.0f ) {
		float glow = alertGlow - 0.2f;
		if ( glow > 1.0f )
			glow = 1.0f;
		else if ( glow < 0.0f )
			glow = 0.0f;
		unsigned char table[256];
		BuildScaleTable( table, glow );
		ApplyAlphaTable( &blinkingTextures, table );
		alertGlow = glow;
	}

	if ( unknown7C > 0.0f ) {
		unknown7C -= delta;
		if ( unknown7C < 0.0f )
			unknown7C = 0.0f;
		float fraction = unknown7C / unknown80;
		unknown84 = ( 1.0f - fraction ) * unknown8C + fraction * unknown88;
		ApplyLightLevel();
		if ( unknown7C == 0.0f ) {
			unknown7C = -1.0f;
			unknown84 = unknown88 = unknown8C;
		}
	}

	ObjectClass::Update( time );
}

/*
================
BridgeObjectClass::FlickerLCARs

The panels flicker for the time given; a flicker under way only takes
the new time.
================
*/
// FUNCTION: STBC 0x00663580
void BridgeObjectClass::FlickerLCARs( float time ) {
	if ( lcarsState != LCARS_FLICKERING ) {
		flickerReturnState = lcarsState;
		lcarsState = LCARS_FLICKERING;
	}
	flickerTime = time;
}

/*
================
BridgeObjectClass::TurnLCARsOff

The panels go dark: the material of the panels stops emitting.
================
*/
// FUNCTION: STBC 0x006635e0
void BridgeObjectClass::TurnLCARsOff( void ) {
	if ( lcarsState != LCARS_OFF ) {
		lcarsState = LCARS_OFF;
		if ( lcarsMaterial != NULL ) {
			lcarsMaterial->SetEmittance( NiColor::BLACK );
		}
	}
}

/*
================
BridgeObjectClass::TurnLCARsOn

The panels light up again: the material of the panels emits white.
================
*/
// FUNCTION: STBC 0x00663620
void BridgeObjectClass::TurnLCARsOn( void ) {
	if ( lcarsState != LCARS_ON ) {
		lcarsState = LCARS_ON;
		if ( lcarsMaterial != NULL ) {
			lcarsMaterial->SetEmittance( NiColor::WHITE );
		}
	}
}

/*
================
BridgeObjectClass::AlertLightsOn

Switches the red alert lights on and starts their dimmers.
================
*/
// FUNCTION: STBC 0x00663660
void BridgeObjectClass::AlertLightsOn( void ) {
	if ( unknownA8 != NULL ) {
		unknownA8->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	}
	NiTListIterator pos = alertLights.GetHeadPos();
	while ( pos != NULL ) {
		NiPointer<NiDynamicEffect> light = alertLights.GetNext( pos );
		light->SetSwitch( true );
	}
	pos = alertDimmers.GetHeadPos();
	while ( pos != NULL ) {
		NiPointer<NiTimeController> dimmer = alertDimmers.GetNext( pos );
		dimmer->Start( -NI_INFINITY );
	}
}

/*
================
BridgeObjectClass::AlertLightsOff

Switches the red alert lights off and stops their dimmers.
================
*/
// FUNCTION: STBC 0x00663790
void BridgeObjectClass::AlertLightsOff( void ) {
	if ( unknownA8 != NULL ) {
		unknownA8->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		if ( unknownAC != NULL ) {
			unknownAC->SetEmittance( NiColor::WHITE );
		}
	}
	NiTListIterator pos = alertLights.GetHeadPos();
	while ( pos != NULL ) {
		NiPointer<NiDynamicEffect> light = alertLights.GetNext( pos );
		light->SetSwitch( false );
	}
	pos = alertDimmers.GetHeadPos();
	while ( pos != NULL ) {
		NiPointer<NiTimeController> dimmer = alertDimmers.GetNext( pos );
		dimmer->Stop();
	}
}

/*
================
BridgeObjectClass::Unknown6638C0
================
*/
// FUNCTION: STBC 0x006638c0
void BridgeObjectClass::Unknown6638C0( float duration, float target ) {
	if ( fabsf( target - unknown84 ) > lightLevelTolerance ) {
		unknown7C = duration;
	} else {
		unknown7C = 0.0f;
	}
	unknown88 = unknown84;
	unknown8C = target;
	unknown80 = duration;
}

/*
================
BridgeObjectClass::ApplyLightLevel

The dim material emits the light level (at most 1); the glow of the red-off
and dim map textures comes up from a level of 0.4 to full at 1, and the
normal textures and the gamma node follow a gamma curve of the level.
================
*/
// FUNCTION: STBC 0x00663910
void BridgeObjectClass::ApplyLightLevel( void ) {
	unsigned char glowTable[256];
	unsigned char gammaTable[256];

	if ( dimMaterial != NULL ) {
		float level = unknown84;
		if ( level > 1.0f ) {
			level = 1.0f;
		}
		dimMaterial->SetEmittance( NiColor( level, level, level ) );
	}

	float glow = ( unknown84 - 0.4f ) * ( 1.0f / 0.6f );
	if ( glow > 1.0f ) {
		glow = 1.0f;
	} else if ( glow < 0.0f ) {
		glow = 0.0f;
	}
	BuildScaleTable( glowTable, glow );
	BuildGammaTable( gammaTable, unknown84 );
	ApplyAlphaTable( &redOffTextures, glowTable );
	ApplyColorTable( &normalTextures, gammaTable );
	ApplyColorTable( &dimMapTextures, glowTable );
	ApplyGamma( gamma, gammaTable );
}

/*
================
BridgeObjectClass::ApplyGamma

The vertex colours of the geometry at and under the object become their
byte copies (PrepareGamma) through the table.
================
*/
// FUNCTION: STBC 0x00663a40
void BridgeObjectClass::ApplyGamma( NiAVObject *object, unsigned char *table ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL && shape->modelData->GetColors() != NULL ) {
		NiColorA *colors = shape->modelData->GetColors();
		int count = shape->GetVertexCount();
		unsigned char *bytes = (unsigned char *)( colors + count );
		for ( int i = 0; i < count; i++ ) {
			colors[i].r = table[bytes[i * 3]] / 255.0f;
			colors[i].g = table[bytes[i * 3 + 1]] / 255.0f;
			colors[i].b = table[bytes[i * 3 + 2]] / 255.0f;
		}
		shape->modelData->IncRevisionID();
	}

	NiNode *parent = NiDynamicCast( NiNode, object );
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			if ( parent->GetAt( i ) != NULL ) {
				ApplyGamma( parent->GetAt( i ), table );
			}
		}
	}
}

/*
================
BridgeObjectClass::ApplyColorTable

Each texture's image takes its original pixels' colours through the table.
================
*/
// FUNCTION: STBC 0x00663ba0
// DRAFT: the rows' start pointers (y times the row stride, as ApplyAlphaTable) give retail's inner loop; left: retail loads width and height before the destination's pixel stride and updates the row pointers in another order (register allocation)
void BridgeObjectClass::ApplyColorTable( NiTArray<BridgeTexture *> *textures, unsigned char *table ) {
	for ( unsigned int i = 0; i < textures->GetSize(); i++ ) {
		BridgeTexture *texture = textures->GetAt( i );
		NiRawImageData *original = texture->original;
		NiRawImageData *pixels = texture->image->GetRawImageData();
		int sourceStride = original->GetPixelStride();
		int width = pixels->GetWidth();
		int height = pixels->GetHeight();
		int destStride = pixels->GetPixelStride();
		int sourceRowStride = original->GetRowStride();
		unsigned char *sourcePixels = original->GetPixels();
		int destRowStride = pixels->GetRowStride();
		unsigned char *destPixels = pixels->GetPixels();
		for ( int y = 0; y < height; y++ ) {
			// BUG: each row starts at the other image's row stride
			unsigned char *dest = destPixels + y * sourceRowStride;
			unsigned char *source = sourcePixels + y * destRowStride;
			for ( int x = 0; x < width; x++ ) {
				dest[0] = table[source[0]];
				dest[1] = table[source[1]];
				dest[2] = table[source[2]];
				source += sourceStride;
				dest += destStride;
			}
		}
		texture->image->unknown20++;
		texture->image->VirtualSlot12();
	}
}

/*
================
BridgeObjectClass::ApplyAlphaTable

Each texture's image takes its original pixels' alpha through the table.
================
*/
// FUNCTION: STBC 0x00663cb0
void BridgeObjectClass::ApplyAlphaTable( NiTArray<BridgeTexture *> *textures, unsigned char *table ) {
	for ( unsigned int i = 0; i < textures->GetSize(); i++ ) {
		BridgeTexture *texture = textures->GetAt( i );
		NiRawImageData *pixels = texture->image->GetRawImageData();
		int rowStride = pixels->GetRowStride();
		int height = pixels->GetHeight();
		int width = pixels->GetWidth();
		int pixelStride = pixels->GetPixelStride();
		unsigned char *data = pixels->GetPixels();
		unsigned char *original = texture->original->GetPixels();
		for ( int y = 0; y < height; y++ ) {
			int offset = y * rowStride + 3;		// the row's first alpha byte
			for ( int x = 0; x < width; x++ ) {
				data[offset] = table[original[offset]];
				offset += pixelStride;
			}
		}
		texture->image->unknown20++;
		texture->image->VirtualSlot12();
	}
}

/*
================
BridgeObjectClass::GetHardpointProperty
================
*/
// FUNCTION: STBC 0x00663db0
// PARKED: register allocation: retail keeps the result in a frame slot across the name compare's EH region and returns it from there; this build keeps it in EBX and folds the not-found return to NULL
TGModelProperty *BridgeObjectClass::GetHardpointProperty( int propertyType, char *name ) {
	TGModelProperty *property = NULL;
	TGModelPropertyList *list;
	if ( name != NULL ) {
		list = propertySet->GetPropertiesByType( EFFECTEMITTERPROPERTY_TYPE );
	} else {
		list = propertySet->GetPropertiesByType( propertyType );
	}
	if ( list != NULL ) {
		if ( list->GetSize() > 0 ) {
			if ( name != NULL ) {
				list->TGBeginIteration();
				for ( TGModelPropertyInstance *instance = list->TGGetNext(); instance != NULL; instance = list->TGGetNext() ) {
					if ( instance->GetProperty() != NULL && instance->GetProperty()->GetName().CompareC( name, true ) == 0 ) {
						property = instance->GetProperty();
						delete list;
						return property;
					}
				}
				delete list;
				return property;
			}
			int index = g_kSystemWrapper.GetRandomNumber( list->GetSize() );
			list->TGBeginIteration();
			property = list->TGGetNext()->GetProperty();
			while ( index != 0 ) {
				property = list->TGGetNext()->GetProperty();
				index--;
			}
		}
		delete list;
	}
	return property;
}

/*
================
BridgeObjectClass::Unknown663F30
================
*/
// FUNCTION: STBC 0x00663f30
// PARKED: inline budget: retail calls NiPoint3::operator= (0x414560) out of line for SetTranslate and reuses a getter temporary's slot for the reference axis; this build inlines the copy and allocates a larger frame
NiNode *BridgeObjectClass::Unknown663F30( PositionOrientationProperty *property ) {
	if ( property == NULL ) {
		return NULL;
	}
	NiPoint3 position = property->GetPosition();
	NiPoint3 forward = property->GetForward();
	NiPoint3 right = property->GetRight();
	NiPoint3 reference = NiPoint3::UNIT_X;
	NiPoint3 axis = reference.Cross( forward );
	axis.Unitize();
	if ( axis == NiPoint3::ZERO ) {
		axis = right;
	}
	float angle = NiACos( reference.Dot( forward ) );
	NiMatrix3 rotation;
	rotation.MakeRotation( -angle, axis );
	NiNode *node = new NiNode;
	node->SetTranslate( position );
	node->SetRotate( rotation );
	node->Update( 0.0f );
	return node;
}

/*
================
BridgeObjectClass::CreateEffect
================
*/
// FUNCTION: STBC 0x006641c0
char *BridgeObjectClass::CreateEffect( PointParticleController *controller, int propertyType, char *textureName, char *propertyName ) {
	TGModelProperty *property = GetHardpointProperty( propertyType, propertyName );
	if ( property != NULL ) {
		controller->emitFrom = Unknown663F30( (PositionOrientationProperty *)property );
		controller->CreateTarget( textureName );
		controller->AttachEffect( GetNode() );
		return property->GetName().Unknown6F56F0();
	}
	return NULL;
}

/*
================
BridgeObjectClass::DoCrewReactions

Makes the bridge crew react to a hit of the player's ship while it is alive.
With no emitter property name the reaction is "Hit" for everyone. With one,
the property's type picks it: a spark emitter makes the crew near it (inside
36 units) react, turning left or right by the side the emitter is on, and an
explosion throws the crew inside 60 units across the bridge ("Fly") and
makes the others "HitHard"; any other type, or an unknown name, does
nothing. A crew member standing plays the standing form of the animation.
================
*/
// FUNCTION: STBC 0x006642b0 ?DoCrewReactions@BridgeObjectClass@@QAEXPAD@Z
// DRAFT: call set as retail (single-statement bodies unbraced: the emitter's release at the unknown-type return calls DecRefCount); frame slots of the flags, origin and offset differ
void BridgeObjectClass::DoCrewReactions( char *propertyName ) {
	char reaction[64];
	char animation[256];

	if ( Game::GetCurrentGame() == NULL )
		return;
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->IsDead() || player->IsDying() )
		return;

	bool hitHard = false;
	bool sideways = false;
	NiPointer<NiNode> emitter;
	float range;
	float flyRange;
	if ( propertyName != NULL ) {
		TGModelProperty *property = GetHardpointProperty( EFFECTEMITTERPROPERTY_TYPE, propertyName );
		if ( property == NULL )
			return;
		emitter = Unknown663F30( (PositionOrientationProperty *)property );
		switch ( property->GetObjType() ) {
			case SPARKEMITTERPROPERTY_TYPE:
				strcpy( reaction, "React" );
				range = 36.0f;
				sideways = true;
				break;
			case EXPLODEEMITTERPROPERTY_TYPE:
				strcpy( reaction, "HitHard" );
				range = 100000000.0f;
				hitHard = true;
				flyRange = 60.0f;
				break;
			case SMOKEEMITTERPROPERTY_TYPE:
			default:
				return;
		}
	} else {
		strcpy( reaction, "Hit" );
		range = 100000000.0f;
	}

	int count;
	CharacterClass **crew = CharacterClass::GetClassObjects( GetContainingSet(), &count );
	if ( crew != NULL ) {
		NiPoint3 origin;
		if ( emitter != NULL )
			origin = emitter->GetTranslate();
		else
			origin = NiPoint3( 0.0f, 0.0f, 0.0f );
		for ( int i = 0; i < count; i++ ) {
			CharacterClass *character = crew[i];
			NiPoint3 position = character->GetWorldLocation();
			NiPoint3 offset = position - origin;
			float distance = offset.Length();
			if ( hitHard && distance < flyRange ) {
				char *location = character->GetLocation();
				character->IsStateSet( CharacterClass::CS_STANDING );	// BUG: redundant: the result is never used
				sprintf( animation, "%sFly", location );
			} else if ( distance < range ) {
				char *location = character->GetLocation();
				bool standing = character->IsStateSet( CharacterClass::CS_STANDING ) != 0;
				if ( sideways ) {
					if ( offset.x > 0.0f )
						sprintf( animation, "%s%sLeft", location, reaction );
					else
						sprintf( animation, "%s%sRight", location, reaction );
				} else
					sprintf( animation, "%s%s", location, reaction );
				if ( standing )
					strcat( animation, "Standing" );
			} else
				continue;
			character->ClearExtraAnimations();
			if ( !character->IsGoingToAnimate() )
				character->PlayAnimation( animation, -1, NULL );
		}
	}
}

/*
================
BridgeObjectClass::ShakeCrew

Makes the whole crew play its "Hit" (or "HitHard") animation while the
player's ship is alive; a crew member standing plays the standing form.
================
*/
// FUNCTION: STBC 0x00664660
void BridgeObjectClass::ShakeCrew( bool hard ) {
	char animation[256];
	char reaction[64];

	if ( Game::GetCurrentGame() == NULL ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->IsDead() || player->IsDying() ) {
		return;
	}

	if ( hard ) {
		strcpy( reaction, "HitHard" );
	} else {
		strcpy( reaction, "Hit" );
	}
	int count;
	CharacterClass **crew = CharacterClass::GetClassObjects( GetContainingSet(), &count );
	if ( crew == NULL ) {
		return;
	}
	for ( int i = 0; i < count; i++ ) {
		CharacterClass *character = crew[i];
		char *location = character->GetLocation();
		bool standing = character->IsStateSet( CharacterClass::CS_STANDING ) != 0;
		sprintf( animation, "%s%s", location, reaction );
		if ( standing ) {
			strcat( animation, "Standing" );
		}
		character->ClearExtraAnimations();
		if ( !character->IsGoingToAnimate() ) {
			character->PlayAnimation( animation, -1, NULL );
		}
	}
}

/*
================
BridgeObjectClass::HandleChangeAlertLevel

The player's ship's alert level sets the bridge's.
================
*/
// FUNCTION: STBC 0x006647d0
void BridgeObjectClass::HandleChangeAlertLevel( TGEvent *event ) {
	if ( Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() != NULL &&
		 Game::GetCurrentGame()->GetPlayer() == event->GetDestination() ) {
		switch ( ( (TGIntEvent *)event )->GetInt() ) {
			case ShipClass::GREEN_ALERT:
				GoToGreenAllert();
				break;
			case ShipClass::YELLOW_ALERT:
				GoToYellowAllert();
				break;
			case ShipClass::RED_ALERT:
				GoToRedAllert();
				break;
		}
	}
	CallNextHandler( event );
}

/*
================
BridgeObjectClass::GoToGreenAllert

The red alert lights go off and the light level comes up to full.
================
*/
// FUNCTION: STBC 0x00664840
void BridgeObjectClass::GoToGreenAllert( void ) {
	redAlert = false;
	AlertLightsOff();
	Unknown6638C0( 0.5f, 1.0f );
}

/*
================
BridgeObjectClass::GoToYellowAllert

The red alert lights pulse at full light level.
================
*/
// FUNCTION: STBC 0x00664860
void BridgeObjectClass::GoToYellowAllert( void ) {
	redAlert = false;
	AlertLightsOn();
	Unknown6638C0( 0.5f, 1.0f );
}

/*
================
BridgeObjectClass::GoToRedAllert

The red alert lights pulse, the blinking textures with them, and the
light level dims to 0.4.
================
*/
// FUNCTION: STBC 0x00664880
void BridgeObjectClass::GoToRedAllert( void ) {
	redAlert = true;
	AlertLightsOn();
	Unknown6638C0( 0.5f, 0.4f );
}

/*
================
BridgeObjectClass::CreateEffectHandler

A torpedo hit on the player's shields shakes the main camera and the crew,
harder at a quarter of the shields or less, and below a quarter damages
the bridge (the bridge effects script's DoHullDamage). A hull hit damages
the bridge, at most twice a second.
================
*/
// FUNCTION: STBC 0x006648a0
// DRAFT: register allocation: retail hoists both arguments of the two ZoomCameraObjectClass::GetObject calls above the condition test (this build only the name), and loads the application in EAX
void BridgeObjectClass::CreateEffectHandler( TGEvent *event ) {
	if ( Game::GetCurrentPlayer() == NULL ) {
		return;
	}
	WeaponHitEvent *hit = (WeaponHitEvent *)event;
	if ( hit->GetWeaponType() == WeaponHitEvent::TORPEDO && !hit->IsHullHit() ) {
		float condition = hit->GetCondition();
		if ( condition > 0.25f ) {
			ZoomCameraObjectClass *camera = ZoomCameraObjectClass::GetObject( containingSet, "maincamera" );
			if ( camera != NULL ) {
				camera->SetShake( 2.0f, 1.0f );
			}
			ShakeCrew( false );
		} else {
			ZoomCameraObjectClass *camera = ZoomCameraObjectClass::GetObject( containingSet, "maincamera" );
			if ( camera != NULL ) {
				camera->SetShake( 4.0f, 2.0f );
			}
			ShakeCrew( true );
			if ( condition < 0.25f ) {
				PyEmbed::CallFunction( "Bridge.bridgeeffects", "DoHullDamage", "", NULL, "(f)", 1.0 );
				PyErr_Print();
			}
		}
	}
	if ( hit->IsHullHit() == true ) {
		float condition = hit->GetCondition();
		float now = UtopiaApp::GetApp()->GetGameTime();
		if ( now - lastHullDamageTime >= 0.5f ) {
			lastHullDamageTime = now;
			PyEmbed::CallFunction( "Bridge.bridgeeffects", "DoHullDamage", "", NULL, "(f)", (double)condition );
			PyErr_Print();
		}
	}
}

/*
================
BridgeObjectClass::WriteToStream
================
*/
// FUNCTION: STBC 0x006649d0
bool BridgeObjectClass::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( modelName );
	if ( propertySet != NULL ) {
		stream->WriteBool( true );
		propertySet->WriteToStream( stream );
	} else {
		stream->WriteBool( false );
	}
	stream->WriteFloat( alertGlow );
	stream->WriteFloat( lastHullDamageTime );
	stream->WriteFloat( unknown7C );
	stream->WriteFloat( unknown80 );
	stream->WriteFloat( unknown88 );
	stream->WriteFloat( unknown8C );
	stream->WriteLong( lcarsState );
	stream->WriteLong( flickerReturnState );
	stream->WriteFloat( flickerTime );
	stream->VirtualSlot17( (long)GetAnimNode() );
	return true;
}

/*
================
BridgeObjectClass::ReadFromStream

Loads the model again, with the textures of the current image detail, and
sets the lighting up again; the light level starts at full.
================
*/
// FUNCTION: STBC 0x00664ab0
bool BridgeObjectClass::ReadFromStream( TGStream *stream ) {
	char texturePath[256];

	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	modelName = stream->VirtualSlot10( -1 );
	strcpy( texturePath, modelName );
	char *directory = texturePath + strlen( texturePath );
	while ( *directory != '/' && *directory != '\\' && directory != texturePath ) {
		directory--;
	}
	strcpy( directory + 1, detailDirectories[g_kImageManager.GetImageDetail()] );
	g_kModelManager.LoadModel( modelName, NULL, texturePath );
	TGAnimNode *model = g_kModelManager.CloneModel( modelName );
	SetNiObject( model );

	if ( stream->ReadBool() == true ) {
		propertySet = (TGModelPropertySet *)TGObject::LoadObject( stream, 0 );
	} else {
		propertySet = NULL;
	}
	redAlert = false;
	alertGlow = stream->ReadFloat();
	lastHullDamageTime = stream->ReadFloat();
	unknown7C = stream->ReadFloat();
	unknown80 = stream->ReadFloat();
	unknown84 = 1.0f;
	unknown88 = stream->ReadFloat();
	unknown8C = stream->ReadFloat();
	lcarsState = stream->ReadLong();
	flickerReturnState = stream->ReadLong();
	flickerTime = stream->ReadFloat();
	long savedAnimNode = stream->VirtualSlot16();
	pointerMap.Add( savedAnimNode, GetAnimNode() );
	SetupLighting();
	return true;
}
