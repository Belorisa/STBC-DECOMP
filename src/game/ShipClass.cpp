/*
===============================================================================

	ShipClass.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include "ShipClass.h"
#include "CloakingSubsystem.h"
#include "Game.h"
#include "MultiplayerGame.h"
#include "HullClass.h"
#include "ImpulseEngineSubsystem.h"
#include "TGBoolEvent.h"
#include "TGIntEvent.h"
#include "TGObjPtrEvent.h"
#include "ProximityManager.h"
#include "ProximityCheck.h"
#include "PlacementObject.h"
#include "Nebula.h"
#include "PhaserBank.h"
#include "PhaserSystem.h"
#include "PulseWeapon.h"
#include "PulseWeaponSystem.h"
#include "PyEmbed.h"
#include "PowerSubsystem.h"
#include "PoweredSubsystem.h"
#include "RepairSubsystem.h"
#include "SensorSubsystem.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ShieldClass.h"
#include "ShipProperty.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGPoolManager.h"
#include "TGSoundManager.h"
#include "TGSound.h"
#include "TGSoundRegion.h"
#include "TGStream.h"
#include "TorpedoSystem.h"
#include "TorpedoTube.h"
#include "TractorBeamProjector.h"
#include "TractorBeamSystem.h"
#include "UtopiaModule.h"
#include "WarpEngineSubsystem.h"
#include "ArtificialIntelligence.h"
#include "WeaponSystem.h"
#include "NiQuaternion.h"
#include "CollisionEvent.h"
#include "WeaponHitEvent.h"
#include "Force.h"
#include "WeaponPayload.h"
#include "ShieldPatchParticles.h"
#include "ImageManagerClass.h"
#include "EffectController.h"
#include "WarpEvent.h"
#include "WarpSequence.h"
#include "Torpedo.h"
#include "EngineProperty.h"
#include "RepairSubsystemProperty.h"
#include "ImpulseEngineProperty.h"
#include "PowerProperty.h"
#include "WeaponSystemProperty.h"
#include "TorpedoAmmoType.h"
#include "TorpedoSystemProperty.h"
#include "PoweredSubsystemProperty.h"
#include "ObjectExplodingEvent.h"
#include "Weapon.h"
#include "Planet.h"
#include "TGBufferStream.h"
#include "TGMessage.h"
#include "MultiplayerPlayer.h"
#include "UnknownClass6A8C50.h"
#include "NetworkAI.h"
#include "TGPlayerEvent.h"
#include "TopWindow.h"
#include "MainWindow.h"
#include "MultiplayerWindow.h"
#include "NiMain.h"
#include "NiAlphaController.h"
#include "NiAnimBlender.h"
#include "NiBezFloatKey.h"
#include "NiBezPosKey.h"
#include "NiBezRotKey.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiColorData.h"
#include "NiColorKey.h"
#include "NiEulerRotKey.h"
#include "NiFlipController.h"
#include "NiFloatController.h"
#include "NiFloatData.h"
#include "NiForce.h"
#include "NiGravity.h"
#include "NiKeyframeManager.h"
#include "NiKeyType4FloatKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiLightColorController.h"
#include "NiLinColorKey.h"
#include "NiLinFloatKey.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiLookAtController.h"
#include "NiMaterialColorController.h"
#include "NiMorphController.h"
#include "NiMorphData.h"
#include "NiMorpherController.h"
#include "NiMorphKey.h"
#include "NiParticleBomb.h"
#include "NiParticleSystemController.h"
#include "NiPathController.h"
#include "NiPlanarCollider.h"
#include "NiPosData.h"
#include "NiRollController.h"
#include "NiSequenceStreamHelper.h"
#include "NiSphericalCollider.h"
#include "NiTCBFloatKey.h"
#include "NiTCBPosKey.h"
#include "NiTCBRotKey.h"
#include "NiTextKeyExtraData.h"
#include "NiVisController.h"
#include "NiVisData.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005aa5e0 _$E66
// FUNCTION: STBC 0x005aa5f0 _$E65
// FUNCTION: STBC 0x005aa600 _$E379
// FUNCTION: STBC 0x005aa610 _$E378
// FUNCTION: STBC 0x005aa620 _$E77
// FUNCTION: STBC 0x005aa630 _$E76
// FUNCTION: STBC 0x005aa640 _$E81
// FUNCTION: STBC 0x005aa650 _$E80
// FUNCTION: STBC 0x005aa660 _$E85
// FUNCTION: STBC 0x005aa670 _$E84
// FUNCTION: STBC 0x005aa680 _$E89
// FUNCTION: STBC 0x005aa690 _$E88
// FUNCTION: STBC 0x005aa6a0 _$E206
// FUNCTION: STBC 0x005aa6b0 _$E205
// FUNCTION: STBC 0x005aa6c0 _$E148
// FUNCTION: STBC 0x005aa6d0 _$E147
// FUNCTION: STBC 0x005aa6e0 _$E161
// FUNCTION: STBC 0x005aa6f0 _$E160
// FUNCTION: STBC 0x005aa700 _$E211
// FUNCTION: STBC 0x005aa710 _$E210
// FUNCTION: STBC 0x005aa720 _$E384
// FUNCTION: STBC 0x005aa730 _$E383
// FUNCTION: STBC 0x005aa740 _$E215
// FUNCTION: STBC 0x005aa750 _$E214
// FUNCTION: STBC 0x005aa760 _$E388
// FUNCTION: STBC 0x005aa770 _$E387
// FUNCTION: STBC 0x005aa780 _$E395
// FUNCTION: STBC 0x005aa790 _$E394
// FUNCTION: STBC 0x005aa7a0 _$E455
// FUNCTION: STBC 0x005aa7b0 _$E454
// FUNCTION: STBC 0x005aa7c0 _$E459
// FUNCTION: STBC 0x005aa7d0 _$E458
// FUNCTION: STBC 0x005aa7e0 _$E106
// FUNCTION: STBC 0x005aa7f0 _$E105
// FUNCTION: STBC 0x005aa800 _$E430
// FUNCTION: STBC 0x005aa810 _$E429
// FUNCTION: STBC 0x005aa820 _$E234
// FUNCTION: STBC 0x005aa830 _$E233
// FUNCTION: STBC 0x005aa840 _$E583
// FUNCTION: STBC 0x005aa850 _$E582
// FUNCTION: STBC 0x005aa860 _$E587
// FUNCTION: STBC 0x005aa870 _$E586
// FUNCTION: STBC 0x005aa880 _$E241
// FUNCTION: STBC 0x005aa890 _$E240
// FUNCTION: STBC 0x005aa8a0 _$E591
// FUNCTION: STBC 0x005aa8b0 _$E590
// FUNCTION: STBC 0x005aa8c0 _$E614
// FUNCTION: STBC 0x005aa8d0 _$E613
// FUNCTION: STBC 0x005aa8e0 _$E605
// FUNCTION: STBC 0x005aa8f0 _$E604
// FUNCTION: STBC 0x005aa900 _$E245
// FUNCTION: STBC 0x005aa910 _$E244
// FUNCTION: STBC 0x005aa920 _$E249
// FUNCTION: STBC 0x005aa930 _$E248
// FUNCTION: STBC 0x005aa940 _$E618
// FUNCTION: STBC 0x005aa950 _$E617
// FUNCTION: STBC 0x005aa960 _$E253
// FUNCTION: STBC 0x005aa970 _$E252
// FUNCTION: STBC 0x005aa980 _$E259
// FUNCTION: STBC 0x005aa990 _$E258
// FUNCTION: STBC 0x005aa9a0 _$E622
// FUNCTION: STBC 0x005aa9b0 _$E621
// FUNCTION: STBC 0x005aa9c0 _$E691
// FUNCTION: STBC 0x005aa9d0 _$E690
// FUNCTION: STBC 0x005aa9e0 _$E694
// FUNCTION: STBC 0x005aa9f0 _$E693
// FUNCTION: STBC 0x005aaa00 _$E275
// FUNCTION: STBC 0x005aaa10 _$E274
// FUNCTION: STBC 0x005aaa20 _$E651
// FUNCTION: STBC 0x005aaa30 _$E650
// FUNCTION: STBC 0x005aaa40 _$E279
// FUNCTION: STBC 0x005aaa50 _$E278
// FUNCTION: STBC 0x005aaa60 _$E463
// FUNCTION: STBC 0x005aaa70 _$E462
// FUNCTION: STBC 0x005aaa80 _$E667
// FUNCTION: STBC 0x005aaa90 _$E666
// FUNCTION: STBC 0x005aaaa0 _$E467
// FUNCTION: STBC 0x005aaab0 _$E466
// FUNCTION: STBC 0x005aaac0 _$E471
// FUNCTION: STBC 0x005aaad0 _$E470
// FUNCTION: STBC 0x005aaae0 _$E626
// FUNCTION: STBC 0x005aaaf0 _$E625
// FUNCTION: STBC 0x005aab00 _$E475
// FUNCTION: STBC 0x005aab10 _$E474
// FUNCTION: STBC 0x005aab20 _$E674
// FUNCTION: STBC 0x005aab30 _$E673
// FUNCTION: STBC 0x005aab40 _$E307
// FUNCTION: STBC 0x005aab50 _$E306
// FUNCTION: STBC 0x005aab60 _$E677
// FUNCTION: STBC 0x005aab70 _$E676
// FUNCTION: STBC 0x005aab80 _$E479
// FUNCTION: STBC 0x005aab90 _$E478
// FUNCTION: STBC 0x005aaba0 _$E717
// FUNCTION: STBC 0x005aabb0 _$E716
// FUNCTION: STBC 0x005aabc0 _$E629
// FUNCTION: STBC 0x005aabd0 _$E628
// FUNCTION: STBC 0x005aabe0 _$E483
// FUNCTION: STBC 0x005aabf0 _$E482
// FUNCTION: STBC 0x005aac00 _$E632
// FUNCTION: STBC 0x005aac10 _$E631
// FUNCTION: STBC 0x005aac20 _$E635
// FUNCTION: STBC 0x005aac30 _$E634
// FUNCTION: STBC 0x005aac40 _$E418
// FUNCTION: STBC 0x005aac50 _$E417
// FUNCTION: STBC 0x005aac60 _$E487
// FUNCTION: STBC 0x005aac70 _$E486
// FUNCTION: STBC 0x005aac80 _$E111
// FUNCTION: STBC 0x005aac90 _$E110
// FUNCTION: STBC 0x005aaca0 _$E647
// FUNCTION: STBC 0x005aacb0 _$E646
// FUNCTION: STBC 0x005aacc0 _$E680
// FUNCTION: STBC 0x005aacd0 _$E679
// FUNCTION: STBC 0x005aace0 _$E491
// FUNCTION: STBC 0x005aacf0 _$E490
// FUNCTION: STBC 0x005aad00 _$E655
// FUNCTION: STBC 0x005aad10 _$E654
// FUNCTION: STBC 0x005aad20 _$E116
// FUNCTION: STBC 0x005aad30 _$E115
// FUNCTION: STBC 0x005aad40 _$E659
// FUNCTION: STBC 0x005aad50 _$E658
// FUNCTION: STBC 0x005aad60 _$E495
// FUNCTION: STBC 0x005aad70 _$E494
// FUNCTION: STBC 0x005aad80 _$E499
// FUNCTION: STBC 0x005aad90 _$E498
// FUNCTION: STBC 0x005aada0 _$E663
// FUNCTION: STBC 0x005aadb0 _$E662
// FUNCTION: STBC 0x005aadc0 _$E503
// FUNCTION: STBC 0x005aadd0 _$E502
// FUNCTION: STBC 0x005aade0 _$E721
// FUNCTION: STBC 0x005aadf0 _$E720
// FUNCTION: STBC 0x005aae00 _$E507
// FUNCTION: STBC 0x005aae10 _$E506
// FUNCTION: STBC 0x005aae20 _$E125
// FUNCTION: STBC 0x005aae30 _$E124
// FUNCTION: STBC 0x005aae40 _$E746
// FUNCTION: STBC 0x005aae50 _$E745
// FUNCTION: STBC 0x005aae60 _$E734
// FUNCTION: STBC 0x005aae70 _$E733
// FUNCTION: STBC 0x005aae80 _$E511
// FUNCTION: STBC 0x005aae90 _$E510
// FUNCTION: STBC 0x005aaea0 _$E515
// FUNCTION: STBC 0x005aaeb0 _$E514
// FUNCTION: STBC 0x005aaec0 _$E519
// FUNCTION: STBC 0x005aaed0 _$E518
// FUNCTION: STBC 0x005aaee0 _$E671
// FUNCTION: STBC 0x005aaef0 _$E670
// FUNCTION: STBC 0x005aaf00 _$E131
// FUNCTION: STBC 0x005aaf10 _$E130
// FUNCTION: STBC 0x005aaf20 _$E136
// FUNCTION: STBC 0x005aaf30 _$E135
// FUNCTION: STBC 0x005aaf40 _$E684
// FUNCTION: STBC 0x005aaf50 _$E683
// FUNCTION: STBC 0x005aaf60 _$E688
// FUNCTION: STBC 0x005aaf70 _$E687
// FUNCTION: STBC 0x005aaf80 _$E166
// FUNCTION: STBC 0x005aaf90 _$E165
// FUNCTION: STBC 0x005aafa0 _$E421
// FUNCTION: STBC 0x005aafb0 _$E420
// FUNCTION: STBC 0x005aafc0 _$E433
// FUNCTION: STBC 0x005aafd0 _$E432
// FUNCTION: STBC 0x005aafe0 _$E698
// FUNCTION: STBC 0x005aaff0 _$E697
// FUNCTION: STBC 0x005ab000 _$E706
// FUNCTION: STBC 0x005ab010 _$E705
// FUNCTION: STBC 0x005ab020 _$E710
// FUNCTION: STBC 0x005ab030 _$E709
// FUNCTION: STBC 0x005ab040 _$E714
// FUNCTION: STBC 0x005ab050 _$E713
// FUNCTION: STBC 0x005ab060 _$E702
// FUNCTION: STBC 0x005ab070 _$E701
// FUNCTION: STBC 0x005ab080 _$E730
// FUNCTION: STBC 0x005ab090 _$E729
// FUNCTION: STBC 0x005ab0a0 _$E523
// FUNCTION: STBC 0x005ab0b0 _$E522
// FUNCTION: STBC 0x005ab0c0 _$E171
// FUNCTION: STBC 0x005ab0d0 _$E170
// FUNCTION: STBC 0x005ab0e0 _$E726
// FUNCTION: STBC 0x005ab0f0 _$E725
// FUNCTION: STBC 0x005ab100 _$E527
// FUNCTION: STBC 0x005ab110 _$E526
// FUNCTION: STBC 0x005ab120 _$E531
// FUNCTION: STBC 0x005ab130 _$E530
// FUNCTION: STBC 0x005ab140 _$E176
// FUNCTION: STBC 0x005ab150 _$E175
// FUNCTION: STBC 0x005ab160 _$E535
// FUNCTION: STBC 0x005ab170 _$E534
// FUNCTION: STBC 0x005ab180 _$E738
// FUNCTION: STBC 0x005ab190 _$E737
// FUNCTION: STBC 0x005ab1a0 _$E141
// FUNCTION: STBC 0x005ab1b0 _$E140
// FUNCTION: STBC 0x005ab1c0 _$E742
// FUNCTION: STBC 0x005ab1d0 _$E741
// FUNCTION: STBC 0x005ab1e0 _$E749
// FUNCTION: STBC 0x005ab1f0 _$E748
// FUNCTION: STBC 0x005ab200 _$E752
// FUNCTION: STBC 0x005ab210 _$E751
// FUNCTION: STBC 0x005ab220 _$E539
// FUNCTION: STBC 0x005ab230 _$E538
// FUNCTION: STBC 0x005ab240 _$E543
// FUNCTION: STBC 0x005ab250 _$E542
// FUNCTION: STBC 0x005ab260 _$E756
// FUNCTION: STBC 0x005ab270 _$E755
// FUNCTION: STBC 0x005ab280 _$E760
// FUNCTION: STBC 0x005ab290 _$E759
// FUNCTION: STBC 0x005ab2a0 _$E181
// FUNCTION: STBC 0x005ab2b0 _$E180
// FUNCTION: STBC 0x005ab2c0 _$E186
// FUNCTION: STBC 0x005ab2d0 _$E185
// FUNCTION: STBC 0x005ab2e0 _$E547
// FUNCTION: STBC 0x005ab2f0 _$E546
// FUNCTION: STBC 0x005ab300 _$E551
// FUNCTION: STBC 0x005ab310 _$E550
// FUNCTION: STBC 0x005ab320 _$E555
// FUNCTION: STBC 0x005ab330 _$E554
// FUNCTION: STBC 0x005ab340 _$E559
// FUNCTION: STBC 0x005ab350 _$E558
// FUNCTION: STBC 0x005ab360 _$E563
// FUNCTION: STBC 0x005ab370 _$E562
// FUNCTION: STBC 0x005ab380 _$E567
// FUNCTION: STBC 0x005ab390 _$E566
// FUNCTION: STBC 0x005ab3a0 _$E571
// FUNCTION: STBC 0x005ab3b0 _$E570
// FUNCTION: STBC 0x005ab3c0 _$E575
// FUNCTION: STBC 0x005ab3d0 _$E574
// FUNCTION: STBC 0x005ab3e0 _$E191
// FUNCTION: STBC 0x005ab3f0 _$E190
// FUNCTION: STBC 0x005ab400 _$E579
// FUNCTION: STBC 0x005ab410 _$E578
// FUNCTION: STBC 0x005ab420 _$E196
// FUNCTION: STBC 0x005ab430 _$E195
// FUNCTION: STBC 0x005ab440 _$E201
// FUNCTION: STBC 0x005ab450 _$E200
// FUNCTION: STBC 0x005ab460 _$E639
// FUNCTION: STBC 0x005ab470 _$E638
// FUNCTION: STBC 0x005ab480 _$E643
// FUNCTION: STBC 0x005ab490 _$E642
// FUNCTION: STBC 0x005b21a0 _$E764
// FUNCTION: STBC 0x005b21b0 _$E763
// FUNCTION: STBC 0x005b27d0 _$E769
// FUNCTION: STBC 0x005b27e0 _$E768
// FUNCTION: STBC 0x005b27f0 _$E364
// FUNCTION: STBC 0x005b2800 _$E363

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005aa560 _$E32
// FUNCTION: STBC 0x005aa570 _$E29
// FUNCTION: STBC 0x005aa580 _$E31
// FUNCTION: STBC 0x005aa590 _$E30
// FUNCTION: STBC 0x005aa5a0 _$E38
// FUNCTION: STBC 0x005aa5b0 _$E35
// FUNCTION: STBC 0x005aa5c0 _$E37
// FUNCTION: STBC 0x005aa5d0 _$E36

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005b3ba0 _$E784
// FUNCTION: STBC 0x005b3bc0 _$E783

// The class's handler table: its constructor, exit-time destructor and their guard.
// FUNCTION: STBC 0x005ab4a0 _$E775
// FUNCTION: STBC 0x005ab4b0 _$E771
// FUNCTION: STBC 0x005ab4c0 _$E774
// FUNCTION: STBC 0x005ab4d0 _$E772

// DATA: STBC 0x0098e7a8
TGEventHandlerTable ShipClass::classHandlers;

// The signs of the seven corners of a box other than ( 1, 1, 1 ).
// DATA: STBC 0x008942e4
static const float BOX_CORNERS[7][3] = {
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f }
};

// The offsets of the 26 cells around a cell.
// DATA: STBC 0x008e5f5c
static short cellNeighbours[26][3] = {
	{ -1, -1, -1 },
	{ 0, -1, -1 },
	{ 1, -1, -1 },
	{ -1, 0, -1 },
	{ 0, 0, -1 },
	{ 1, 0, -1 },
	{ -1, 1, -1 },
	{ 0, 1, -1 },
	{ 1, 1, -1 },
	{ -1, -1, 0 },
	{ 0, -1, 0 },
	{ 1, -1, 0 },
	{ -1, 0, 0 },
	{ 1, 0, 0 },
	{ -1, 1, 0 },
	{ 0, 1, 0 },
	{ 1, 1, 0 },
	{ -1, -1, 1 },
	{ 0, -1, 1 },
	{ 1, -1, 1 },
	{ -1, 0, 1 },
	{ 0, 0, 1 },
	{ 1, 0, 1 },
	{ -1, 1, 1 },
	{ 0, 1, 1 },
	{ 1, 1, 1 }
};

/*
================
ShipClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x005ab4f0
void ShipClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, SHIPCLASS_TYPE );
		DamageableObject::AddToSet( set );
	}
}

/*
================
ShipClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x005ab520
void ShipClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, SHIPCLASS_TYPE );
		DamageableObject::RemoveFromSet( set );
	}
}

/*
================
ShipClass::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x005ab550
ShipClass **ShipClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (ShipClass **)set->objectTable.GetObjects( SHIPCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
ShipClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x005ab580
int ShipClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( SHIPCLASS_TYPE );
	}
	return 0;
}

/*
================
ShipClass::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a ShipClass.
================
*/
// FUNCTION: STBC 0x005ab5a0
ShipClass *ShipClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( SHIPCLASS_TYPE ) ) {
			return (ShipClass *)object;
		}
		return NULL;
	}
	return (ShipClass *)set->GetClassObject( SHIPCLASS_TYPE, id );
}

/*
================
ShipClass::GetObject

The ship with the given name in the set, or in the first set that has one,
searching from the last set, when the set is NULL.
================
*/
// FUNCTION: STBC 0x005ab610
ShipClass *ShipClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( SHIPCLASS_TYPE, name_ );
		}
		return (ShipClass *)object;
	}
	return (ShipClass *)set->GetClassObject( SHIPCLASS_TYPE, name_ );
}

/*
================
ShipClass::Cast

The object as a ShipClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005ab670
ShipClass *ShipClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SHIPCLASS_TYPE ) ) {
		return (ShipClass *)object;
	}
	return NULL;
}

/*
================
ShipClass::RegisterHandlerNames

Also registers the handler names of the subsystems a ship owns.
================
*/
// FUNCTION: STBC 0x005ab6a0
void ShipClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::WeaponHitHandler, "ShipClass::WeaponHitHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::HandleChangeAlertLevel, "ShipClass::HandleChangeAlertLevel" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::SetNoAIState, "ShipClass::SetNoAIState" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::HandleTractorHitStart, "ShipClass::HandleTractorHitStart" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::HandleTractorHitStop, "ShipClass::HandleTractorHitStop" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::SetTargetHandler, "ShipClass::SetTargetHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::CollisionEffectHandler, "ShipClass::CollisionEffectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::HostCollisionEffectHandler, "ShipClass::HostCollisionEffectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::PlanetCollisionHandler, "ShipClass::PlanetCollisionHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::StartWarpHandler, "ShipClass::StartWarpHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::EnvironmentDamageHandler, "ShipClass::EnvironmentDamageHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ShipClass::Destroyed, "ShipClass::Destroyed" );

	WeaponSystem::RegisterHandlerNames();
	TorpedoSystem::RegisterHandlerNames();
	RepairSubsystem::RegisterHandlerNames();
	CloakingSubsystem::RegisterHandlerNames();
	ShieldClass::RegisterHandlerNames();
	PoweredSubsystem::RegisterHandlerNames();
	SensorSubsystem::RegisterHandlerNames();
	WarpEngineSubsystem::RegisterHandlerNames();
	Unknown5AB910();
}

/*
================
ShipClass::RegisterHandlers

A collision is handled by CollisionEffectHandler, a collision the host
reports by HostCollisionEffectHandler; the AI's done and dormant events
both clear the AI state.
================
*/
// FUNCTION: STBC 0x005ab7c0
void ShipClass::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_WEAPON_HIT, "ShipClass::WeaponHitHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_SET_ALERT_LEVEL, "ShipClass::HandleChangeAlertLevel" );
	AddMethodHandlerForClass( &classHandlers, ET_AI_DONE, "ShipClass::SetNoAIState" );
	AddMethodHandlerForClass( &classHandlers, ET_AI_DORMANT, "ShipClass::SetNoAIState" );
	AddMethodHandlerForClass( &classHandlers, ET_TRACTOR_BEAM_STARTED_HITTING, "ShipClass::HandleTractorHitStart" );
	AddMethodHandlerForClass( &classHandlers, ET_TRACTOR_BEAM_STOPPED_HITTING, "ShipClass::HandleTractorHitStop" );
	AddMethodHandlerForClass( &classHandlers, ET_SET_TARGET, "ShipClass::SetTargetHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_COLLISION, "ShipClass::CollisionEffectHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_HOST_OBJECT_COLLISION, "ShipClass::HostCollisionEffectHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_PLANET_COLLISION, "ShipClass::PlanetCollisionHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_START_WARP, "ShipClass::StartWarpHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_ENVIRONMENT_DAMAGE, "ShipClass::EnvironmentDamageHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_DESTROYED, "ShipClass::Destroyed" );

	WeaponSystem::RegisterHandlers();
	TorpedoSystem::RegisterHandlers();
	RepairSubsystem::RegisterHandlers();
	CloakingSubsystem::RegisterHandlers();
	ShieldClass::RegisterHandlers();
	PoweredSubsystem::RegisterHandlers();
	SensorSubsystem::RegisterHandlers();
	WarpEngineSubsystem::RegisterHandlers();
	if ( g_kUtopiaModule.IsClient() ) {
		Unknown5AB920();
	}
}

/*
================
ShipClass::Unknown5AB910
================
*/
// FUNCTION: STBC 0x005ab910
void ShipClass::Unknown5AB910( void ) {
}

/*
================
ShipClass::Unknown5AB920
================
*/
// FUNCTION: STBC 0x005ab920
void ShipClass::Unknown5AB920( void ) {
}

/*
================
ShipClass::Create

A new ship showing the named model; NULL when the pool is exhausted.
================
*/
// FUNCTION: STBC 0x005ab930
ShipClass *ShipClass::Create( char *modelName ) {
	return new ShipClass( modelName );
}

/*
================
ShipClass::InitObject

What both constructors share: at rest at red alert 2, targetable and
hurtable, with no target.
================
*/
// FUNCTION: STBC 0x005ab970
void ShipClass::InitObject( void ) {
	unknown1E4 = false;
	usingTargetAngularVelocity = false;
	docked = false;
	targetAngularVelocity = NiPoint3( 0.0f, 0.0f, 0.0f );
	targetAngularDirectionType = 0;
	disabledEngineDeceleration = 0.1f;
	impulse = 0.0f;
	impulseDirection.x = 0.0f;
	impulseDirection.y = 0.0f;
	impulseDirection.z = 0.0f;
	impulseDirectionType = 0;
	doingInSystemWarp = false;
	inSystemWarpTargetID = 0;
	inSystemWarpDistance = 0.0f;
	targetID = 0;
	targetSubsystemID = 0;
	usingTargetOffset = false;
	targetOffset = NiPoint3::ZERO;
	shieldPatches = NULL;
	atmosphereEffect = NULL;
	hitShield = -1;
	affiliation = 0;
	alertLevel = 2;
	unknown23C = unknown896A9C;
	shipProperty = NULL;
	targetable = true;
	hurtable = true;
	destroyBrokenSystems = true;
	invincible = false;
	netPlayerID = 0;
	collisionDamageDisabled = false;
	script = NULL;
	deathScript = NULL;
	aiDoneIgnore = 0;
	mass = 100.0f;
	rotationalInertia = 100.0f;
	unknown2FC = false;
}

/*
================
ShipClass::SetScale
================
*/
// FUNCTION: STBC 0x005abaa0
void ShipClass::SetScale( float scale ) {
	BaseObjectClass::SetScale( scale );
	Unknown5ABAC0();
}

/*
================
ShipClass::Unknown5ABAC0

Takes the ship's box from the first collision model there is, its extent
grown by the square root of three, and scales it by the node's and the
model's scale.
================
*/
// FUNCTION: STBC 0x005abac0
// PARKED: inline budget: retail calls NiPoint3::operator= (0x414560) out of line for the two box copies (as in PhaserProperty::Copy), which this build expands; the rest follows
void ShipClass::Unknown5ABAC0( void ) {
	if ( unknown10C.GetSize() >= 1 ) {
		// BUG: model is left uninitialised when every entry is NULL (retail then reads the stack slot of the last GetAt temporary)
		OBBModel *model;
		for ( unsigned int i = 0; i < unknown10C.GetSize(); i++ ) {
			if ( unknown10C.GetAt( i ) ) {
				model = unknown10C.GetAt( i );
				break;
			}
		}
		if ( model != NULL ) {
			boxCenter = model->GetCenter();
			NiPoint3 extent;
			extent = model->GetHalfExtent();
			double root3 = sqrt( 3.0 );
			boxExtent.x = extent.x * root3;
			boxExtent.y = extent.y * root3;
			boxExtent.z = extent.z * root3;
			boxExtent *= GetNode()->GetScale();
			boxCenter *= GetNode()->GetScale();
			boxExtent *= GetNiObject()->GetScale();
			boxCenter *= GetNiObject()->GetScale();
		}
	}
}

/*
================
ShipClass::VirtualSlot58

The corners of the world-aligned box around the ship's box: each corner of
the ship's box is turned into the world frame, the smallest components give
the first point and their negation the second, both about the box's center in
the world.
================
*/
// FUNCTION: STBC 0x005abc30
void ShipClass::VirtualSlot58( NiPoint3 *point1, NiPoint3 *point2 ) {
	if ( niObject != NULL ) {
		const NiMatrix3 &rotate = niObject->GetWorldRotate();
		*point1 = rotate * boxExtent;
		for ( int i = 0; i < 7; i++ ) {
			NiPoint3 corner( BOX_CORNERS[i][0] * boxExtent.x, BOX_CORNERS[i][1] * boxExtent.y, BOX_CORNERS[i][2] * boxExtent.z );
			NiPoint3 turned = rotate * corner;
			for ( int j = 0; j < 3; j++ ) {
				if ( ( *point1 )[j] > turned[j] ) {
					( *point1 )[j] = turned[j];
				}
			}
		}
		*point2 = -*point1;

		NiPoint3 center = niObject->GetWorldTranslate() + rotate * boxCenter;
		*point1 += center;
		*point2 += center;
	} else {
		ObjectClass::VirtualSlot58( point1, point2 );
	}
}

/*
================
ShipClass::SetupModel
================
*/
// FUNCTION: STBC 0x005abda0
void ShipClass::SetupModel( char *modelName ) {
	DamageableObject::SetupModel( modelName );
	Unknown5ABAC0();
}

/*
================
ShipClass::ShipClass

A ship showing a clone of the named model.
================
*/
// FUNCTION: STBC 0x005abdc0 ??0ShipClass@@QAE@PAD@Z
ShipClass::ShipClass( char *modelName ) : DamageableObject( modelName ), tractorBeams( 2 ) {
	InitObject();
	Unknown5ABAC0();
}

/*
================
ShipClass::GetObjType
================
*/
// FUNCTION: STBC 0x005abe60
int ShipClass::GetObjType( void ) {
	return SHIPCLASS_TYPE;
}

/*
================
ShipClass::GetClassName
================
*/
// FUNCTION: STBC 0x005abee0
char *ShipClass::GetClassName( void ) {
	return "ShipClass";
}

/*
================
ShipClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x005abef0
char *ShipClass::GetSwigTypeName( void ) {
	return "_p_ShipClass";
}

/*
================
ShipClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x005abf00
char *ShipClass::GetPythonClassName( void ) {
	return "ShipClassPtr";
}

/*
================
ShipClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x005abf10
TGEventHandlerTable *ShipClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ShipClass::TurnTowardOrientation
================
*/
// FUNCTION: STBC 0x005abf90
float ShipClass::TurnTowardOrientation( TGPoint3 *forward, TGPoint3 *up ) {
	NiPoint3 upCopy = *up;
	NiPoint3 forwardCopy = *forward;
	return Unknown5AD490( &forwardCopy, &upCopy );
}

/*
================
ShipClass::ShipClass

A ship loaded by ID.
================
*/
// FUNCTION: STBC 0x005ac010 ??0ShipClass@@QAE@J@Z
ShipClass::ShipClass( long id ) : DamageableObject( id ), tractorBeams( 2 ) {
	InitObject();
}

/*
================
ShipClass::~ShipClass

The death script's copy is never freed.
================
*/
// FUNCTION: STBC 0x005ac0a0
// FUNCTION: STBC 0x005abfe0 ??_GShipClass@@UAEPAXI@Z
ShipClass::~ShipClass( void ) {
	StopFiringWeapons();
	ClearAI( true, NULL );
	if ( script != NULL ) {
		delete [] script;
	}
}

/*
================
ShipClass::SetScript

Keeps a copy of the name of the ship's script.
================
*/
// FUNCTION: STBC 0x005ac170
void ShipClass::SetScript( char *script_ ) {
	if ( script != NULL ) {
		delete [] script;
		script = NULL;
	}
	if ( script_ != NULL ) {
		script = new char[strlen( script_ ) + 1];
		strcpy( script, script_ );
	}
}

/*
================
ShipClass::SetDeathScript

Keeps a copy of the "module.function" the ship runs when it dies.
================
*/
// FUNCTION: STBC 0x005ac1e0
void ShipClass::SetDeathScript( char *deathScript_ ) {
	if ( deathScript != NULL ) {
		delete [] deathScript;
		deathScript = NULL;
	}
	if ( deathScript_ != NULL ) {
		deathScript = new char[strlen( deathScript_ ) + 1];
		strcpy( deathScript, deathScript_ );
	}
}

/*
================
ShipClass::RunDeathScript

Calls the death script with the ship; without one, Effects.ObjectExploding.
A death script without a '.' leaves the module name unset.
================
*/
// FUNCTION: STBC 0x005ac250
void ShipClass::RunDeathScript( void ) {
	char module[256];
	char function[256];

	if ( deathScript == NULL ) {
		strcpy( module, "Effects" );
		strcpy( function, "ObjectExploding" );
	} else {
		int length = strlen( deathScript );
		for ( int i = length - 1; i >= 0; i-- ) {
			if ( deathScript[i] == '.' ) {
				strcpy( function, &deathScript[i + 1] );
				strncpy( module, deathScript, i );
				module[i] = '\0';
				break;
			}
		}
	}

	PyObject *pyShip = VirtualSlot08();
	int result = PyEmbed::CallFunction( module, function, NULL, NULL, "(O)", pyShip );
	Py_DECREF( pyShip );
	if ( result == -1 ) {
		PyErr_Print();
	}
}


/*
================
ShipClass::StartGetSubsystemMatch

Starts a walk over the ship's subsystems of the type.
================
*/
// FUNCTION: STBC 0x005ac370
ShipClass::MatchIterator *ShipClass::StartGetSubsystemMatch( int type ) {
	MatchIterator *iterator = new MatchIterator;
	iterator->type = type;
	iterator->pos = subsystems.list.GetHeadPos();
	return iterator;
}

/*
================
ShipClass::GetNextSubsystemMatch

The walk's next subsystem of its type; NULL at the end.
================
*/
// FUNCTION: STBC 0x005ac390
ShipSubsystem *ShipClass::GetNextSubsystemMatch( MatchIterator *iterator ) {
	if ( iterator == NULL || iterator->pos == NULL ) {
		return NULL;
	}
	ShipSubsystem *subsystem;
	do {
		subsystem = subsystems.list.GetNext( iterator->pos );
	} while ( iterator->pos != NULL && subsystem != NULL && !subsystem->IsTypeOf( iterator->type ) );
	if ( subsystem == NULL || !subsystem->IsTypeOf( iterator->type ) ) {
		subsystem = NULL;
	}
	return subsystem;
}

/*
================
ShipClass::GetSubsystemByProperty

The subsystem made from the property, among the ship's subsystems and
their children; NULL when there is none.
================
*/
// FUNCTION: STBC 0x005ac3f0
ShipSubsystem *ShipClass::GetSubsystemByProperty( SubsystemProperty *property ) {
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = subsystems.list.GetNext( pos );
		if ( subsystem->GetProperty() == property ) {
			return subsystem;
		}
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			ShipSubsystem *child = subsystem->GetChildSubsystem( i );
			if ( child->GetProperty() == property ) {
				return child;
			}
		}
	}
	return NULL;
}

/*
================
ShipClass::IsCloaked
================
*/
// FUNCTION: STBC 0x005ac450
bool ShipClass::IsCloaked( void ) {
	CloakingSubsystem *cloak = GetCloakingSubsystem();
	if ( cloak == NULL ) {
		return false;
	}
	if ( cloak->IsCloaked() == true ) {
		return true;
	}
	return false;
}

/*
================
ShipClass::SetImpulse

The impulse is a fraction of the engines' maximum, -1 (full reverse) to 1.
================
*/
// FUNCTION: STBC 0x005ac470 ?SetImpulse@ShipClass@@QAEXMABVNiPoint3@@H@Z
void ShipClass::SetImpulse( float impulse_, const NiPoint3 &direction, int directionType ) {
	if ( GetImpulseEngineSubsystem() == NULL ) {
		return;
	}

	if ( impulse_ < -1.0f ) {
		impulse_ = -1.0f;
	} else if ( impulse_ > 1.0f ) {
		impulse_ = 1.0f;
	}
	impulse = impulse_;
	impulseDirection = direction;
	impulseDirectionType = directionType;
	unknown1E4 = true;
}

/*
================
ShipClass::IsReverse

Whether the impulse direction points backward.
================
*/
// FUNCTION: STBC 0x005ac4f0
bool ShipClass::IsReverse( void ) {
	if ( impulseDirectionType == 0 ) {
		if ( MODEL_FORWARD.Dot( impulseDirection ) < 0.0f ) {
			return true;
		}
		return false;
	}
	if ( GetWorldForward().Dot( impulseDirection ) < 0.0f ) {
		return true;
	}
	return false;
}

/*
================
ShipClass::SetSpeed

The speed as a fraction of the impulse engines' current maximum; none
without engines that can move the ship.
================
*/
// FUNCTION: STBC 0x005ac590 ?SetSpeed@ShipClass@@QAEXMABVNiPoint3@@H@Z
void ShipClass::SetSpeed( float speed, const NiPoint3 &direction, int directionType ) {
	ImpulseEngineSubsystem *impulseEngines = subsystems.impulseEngineSubsystem;
	if ( impulseEngines != NULL ) {
		float maxSpeed = impulseEngines->GetCurMaxSpeed();
		if ( maxSpeed > 0.0f ) {
			speed /= maxSpeed;
		} else {
			speed = 0.0f;
		}
		SetImpulse( speed, direction, directionType );
	}
}

/*
================
ShipClass::VirtualSlot90

Sets the acceleration the ship is driven at, no longer than the impulse
engines can give. A ship without engines keeps the acceleration it has.
================
*/
// FUNCTION: STBC 0x005ac5e0
// PARKED: x87 operand order inside the inlined Unitize and the final scale (retail multiplies the y component as inverse * y and passes the maximum through the FPU)
void ShipClass::VirtualSlot90( NiPoint3 requested ) {
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines != NULL ) {
		unknown1E4 = false;
		float maxAccel = engines->GetCurMaxAccel();
		float lengthSquared = requested.SqrLength();
		if ( lengthSquared > maxAccel * maxAccel ) {
			requested.Unitize();
			requested *= engines->GetCurMaxAccel();
		}
		acceleration = requested;
	}
}

/*
================
ShipClass::InSystemWarp

Warps the ship across its set toward the destination, stopping distance
short of the destination's bound. The path is checked for other objects
(placements, nebulae and two other kinds are ignored): an object in the
way moves the stopping point to where the path meets its bound, and an
object the ship is already inside cancels the warp. A ship facing away
from the stopping point first turns at full impulse. False when the warp
does not start (no impulse engines, too close, or blocked).
================
*/
// FUNCTION: STBC 0x005ac6e0
// PARKED: x87 residue as in StopInSystemWarp (retail spills the unitized length and reloads it, and orders the scale operands differently), and block layout: retail lays the StopInSystemWarp failure tail (with the Unitize zero branch) between the object loop and EndObjectIteration, where early returns give that place but lose the nested scopes' 0x60 frame; calls, constants and referents match
bool ShipClass::InSystemWarp( ObjectClass *destination, float distance ) {
	inSystemWarpTargetID = destination->GetObjID();
	inSystemWarpDistance = distance;

	if ( GetImpulseEngineSubsystem() != NULL && GetImpulseEngineSubsystem()->GetMaxAccel() > 0.0f ) {
		NiPoint3 target = destination->GetWorldLocation() - GetWorldLocation();
		float range = destination->GetRadius() + distance;
		float length = target.Unitize();
		if ( length > IN_SYSTEM_WARP_CLEARANCE && length > range ) {
			// The stopping point.
			float travel = length - range;
			target.x *= travel;
			target.y *= travel;
			target.z *= travel;
			target += GetWorldLocation();

			SetClass *set = GetContainingSet();
			if ( set != NULL && set->GetProximityManager() != NULL ) {
				ProximityManager *proximityManager = set->GetProximityManager();
				bool clear = true;
				int ignoredTypes[4] = { 0x80E2, PLACEMENTOBJECT_TYPE, PROXIMITYCHECK_TYPE, NEBULA_TYPE };
				NiPoint3 start = GetWorldLocation();
				ProximityManager::ObjectIterator *iterator = proximityManager->GetLineIntersectObjects( start, target, IN_SYSTEM_WARP_CLEARANCE, false );
				for ( ObjectClass *object = proximityManager->GetNextObject( iterator ); object != NULL; object = proximityManager->GetNextObject( iterator ) ) {
					if ( object->GetObjID() == GetObjID() ) {
						continue;
					}
					bool ignored = false;
					for ( unsigned int i = 0; i < sizeof( ignoredTypes ) / sizeof( ignoredTypes[0] ); i++ ) {
						if ( object->IsTypeOf( ignoredTypes[i] ) ) {
							ignored = true;
							break;
						}
					}
					if ( ignored ) {
						continue;
					}

					float radius = object->GetRadius() + IN_SYSTEM_WARP_CLEARANCE;
					if ( ( object->GetWorldLocation() - GetWorldLocation() ).SqrLength() < radius * radius ) {
						clear = false;
						break;
					}

					NiPoint3 path = target - GetWorldLocation();
					NiPoint3 hit1;
					NiPoint3 hit2;
					if ( Unknown4570D0( GetWorldLocation(), path, object->GetWorldLocation(), radius, &hit1, &hit2 ) >= 1
							&& ( hit1 - target ).SqrLength() > 1.0f ) {
						NiPoint3 toHit = hit1 - GetWorldLocation();
						if ( toHit.SqrLength() < ( target - GetWorldLocation() ).SqrLength() ) {
							target = hit1;
						}
					}
				}
				proximityManager->EndObjectIteration( iterator );

				if ( clear ) {
					if ( !doingInSystemWarp ) {
						TGBoolEvent *event = new TGBoolEvent;
						event->SetSource( this );
						event->SetDestination( this );
						event->SetBool( true );
						event->SetEventType( ET_IN_SYSTEM_WARP );
						g_kEventManager.AddEvent( event );
					}
					doingInSystemWarp = true;

					if ( Game::GetCurrentPlayer() == this && g_kSoundManager.GetPlayingSound( "InSystemWarp" ) == NULL ) {
						TGSound *sound = g_kSoundManager.GetSound( "InSystemWarp" );
						if ( sound != NULL ) {
							sound->SetLooping( true );
							sound->Play( true );
						}
					}

					TurnTowardLocation( target );
					NiPoint3 direction = target - GetWorldLocation();
					direction.Unitize();
					if ( GetWorldForward().Dot( direction ) < IN_SYSTEM_WARP_FACING ) {
						// Turn toward the stopping point first.
						StopInSystemWarp();
						NiPoint3 forward = TGPoint3::GetModelForward();
						SetImpulse( 1.0f, forward, 0 );
						return true;
					}

					NiPoint3 velocity = target - GetWorldLocation();
					velocity.Unitize();
					velocity *= IN_SYSTEM_WARP_SPEED;
					SetVelocity( &velocity );
					return true;
				}
			}
		}
	}

	StopInSystemWarp();
	return false;
}

/*
================
ShipClass::StopInSystemWarp

Ends an in-system warp: the ship slows to the impulse engines' top speed
and stops its engines, hears ET_IN_SYSTEM_WARP (false), and the player's
ship stops the warp sound.
================
*/
// FUNCTION: STBC 0x005acdb0
void ShipClass::StopInSystemWarp( void ) {
	inSystemWarpTargetID = 0;
	inSystemWarpDistance = 0.0f;
	if ( !doingInSystemWarp ) {
		return;
	}

	float maxSpeed = 0.0f;
	if ( GetImpulseEngineSubsystem() != NULL ) {
		maxSpeed = GetImpulseEngineSubsystem()->GetMaxSpeed();
	}
	NiPoint3 velocity = GetVelocity();
	float speed = velocity.Unitize();
	if ( speed < maxSpeed ) {
		maxSpeed = speed;
	}
	velocity *= maxSpeed;
	SetVelocity( &velocity );
	SetImpulse( 0.0f, TGPoint3::GetModelForward(), 0 );

	TGBoolEvent *event = new TGBoolEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetBool( false );
	event->SetEventType( ET_IN_SYSTEM_WARP );
	g_kEventManager.AddEvent( event );

	if ( Game::GetCurrentPlayer() == this ) {
		TGSound *sound = g_kSoundManager.GetPlayingSound( "InSystemWarp" );
		if ( sound != NULL ) {
			sound->Stop();
		}
	}
	doingInSystemWarp = false;
}

/*
================
ShipClass::SetLimitedAngularAcceleration

Sets the angular acceleration, shortened to the impulse engines' current
maximum angular acceleration, and stops working toward a target angular
velocity. Without impulse engines nothing changes.
================
*/
// FUNCTION: STBC 0x005acfa0
// PARKED: x87 order: retail loads the maximum before the squared length, keeps the squared length in the dead directionType slot and takes Unitize's square root from it; this build computes the length again
void ShipClass::SetLimitedAngularAcceleration( NiPoint3 acceleration_, int directionType ) {
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines == NULL ) {
		return;
	}

	usingTargetAngularVelocity = false;
	SetAngularDirectionType( directionType );
	if ( acceleration_.SqrLength() > engines->GetCurMaxAngularAccel() * engines->GetCurMaxAngularAccel() ) {
		acceleration_.Unitize();
		acceleration_ *= engines->GetCurMaxAngularAccel();
	}
	SetAngularAcceleration( &acceleration_ );
}

/*
================
ShipClass::SetLimitedAngularAccelerationLinear

Sets the angular acceleration and its linear change over a time, each
shortened to the impulse engines' current maximum angular acceleration,
and stops working toward a target angular velocity. Without impulse
engines nothing changes.
================
*/
// FUNCTION: STBC 0x005ad090
void ShipClass::SetLimitedAngularAccelerationLinear( const NiPoint3 &acceleration_, const NiPoint3 &change, float time, int directionType ) {
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines == NULL ) {
		return;
	}

	usingTargetAngularVelocity = false;
	SetAngularDirectionType( directionType );
	NiPoint3 limited = acceleration_;
	NiPoint3 limitedChange = change;
	float maxSquared = engines->GetCurMaxAngularAccel() * engines->GetCurMaxAngularAccel();
	if ( limited.SqrLength() > maxSquared ) {
		limited.Unitize();
		limited *= engines->GetCurMaxAngularAccel();
	}
	if ( limitedChange.SqrLength() > maxSquared ) {
		limitedChange.Unitize();
		limitedChange *= engines->GetCurMaxAngularAccel();
	}
	SetAngularAccelerationLinear( &limited, &limitedChange, time );
}

/*
================
ShipClass::SetTargetAngularVelocityDirect

Turns the ship at the given velocity, in the current angular direction
type's frame.
================
*/
// FUNCTION: STBC 0x005ad290 ?SetTargetAngularVelocityDirect@ShipClass@@QAEXABVNiPoint3@@@Z
void ShipClass::SetTargetAngularVelocityDirect( const NiPoint3 &velocity ) {
	targetAngularVelocity = velocity;
	usingTargetAngularVelocity = true;
	targetAngularDirectionType = angularDirectionType;
}

/*
================
ShipClass::SetTargetAngularVelocityFraction

Turns the ship at a fraction of the impulse engines' current maximum
angular velocity, one fraction for each axis.
================
*/
// FUNCTION: STBC 0x005ad2d0
void ShipClass::SetTargetAngularVelocityFraction( NiPoint3 *fraction ) {
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines != NULL ) {
		SetTargetAngularVelocityDirect( engines->GetCurMaxAngularVelocity() * *fraction );
	}
}

/*
================
ShipClass::GetTargetAngularVelocity

The target angular velocity as a fraction of the impulse engines' current
maximum, per axis; zero when the ship has no engines or they cannot turn.
================
*/
// FUNCTION: STBC 0x005ad310
NiPoint3 ShipClass::GetTargetAngularVelocity( void ) {
	if ( GetImpulseEngineSubsystem() == NULL ) {
		return NiPoint3::ZERO;
	}
	if ( GetImpulseEngineSubsystem()->GetMaxAngularVelocity() == 0.0f ) {
		return NiPoint3::ZERO;
	}
	return targetAngularVelocity * ( 1.0f / GetImpulseEngineSubsystem()->GetCurMaxAngularVelocity() );
}

/*
================
ShipClass::TurnTowardLocation
================
*/
// FUNCTION: STBC 0x005ad3a0 ?TurnTowardLocation@ShipClass@@QAEMABVNiPoint3@@@Z
float ShipClass::TurnTowardLocation( const NiPoint3 &location ) {
	NiPoint3 direction = location - GetWorldLocation();
	direction.Unitize();
	return TurnTowardDirection( direction );
}

/*
================
ShipClass::TurnTowardDirection
================
*/
// FUNCTION: STBC 0x005ad450 ?TurnTowardDirection@ShipClass@@QAEMABVNiPoint3@@@Z
float ShipClass::TurnTowardDirection( const NiPoint3 &direction ) {
	return TurnDirectionsToDirections( GetWorldForward(), direction );
}

/*
================
ShipClass::Unknown5AD490
================
*/
// FUNCTION: STBC 0x005ad490
float ShipClass::Unknown5AD490( NiPoint3 *forward, NiPoint3 *up ) {
	return TurnDirectionsToDirections( GetWorldForward(), *forward, GetWorldUp(), *up );
}

/*
================
ShipClass::TurnTowardDifference

Searches for the shortest time, between 0 and 30 seconds, in which the
ship can turn through the difference (an axis whose length is the angle)
with an angular acceleration that changes linearly, neither end of it
beyond the impulse engines' maximum angular acceleration; then sets that
acceleration and returns the time.
================
*/
// FUNCTION: STBC 0x005ad4d0
float ShipClass::TurnTowardDifference( NiPoint3 *difference ) {
	float time = 5.0f;
	float low = 0.0f;
	float high = 30.0f;
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines == NULL ) {
		return 0.0f;
	}

	float maxSquared = engines->GetCurMaxAngularAccel() * engines->GetCurMaxAngularAccel();
	NiPoint3 accel;
	NiPoint3 change;
	do {
		accel = ( *difference * 6.0f / time - GetAngularVelocity() * 4.0f ) / time;
		change = GetAngularVelocity() * -2.0f / time - accel;
		float accelSquared = accel.SqrLength();
		float changeSquared = change.SqrLength();
		if ( accelSquared > maxSquared || changeSquared > maxSquared ) {
			// too fast: try a longer time
			low = time;
			time = ( high + time ) * 0.5f;
		} else if ( accelSquared < maxSquared || changeSquared < maxSquared ) {
			// slower than it can: try a shorter time
			high = time;
			time = ( low + time ) * 0.5f;
		} else {
			break;
		}
	} while ( high - low > 0.0625f );

	accel = ( *difference * 6.0f / high - GetAngularVelocity() * 4.0f ) / high;
	change = -2.0f * GetAngularVelocity() / high - accel;
	SetLimitedAngularAccelerationLinear( accel, change, high, 1 );
	return high;
}

/*
================
ShipClass::TurnDirectionsToDirections

The turn as an axis whose length is the angle, which TurnTowardDifference
works toward. A turn without an axis (the directions already lined up or
opposite) is about the ship's up direction.
================
*/
// FUNCTION: STBC 0x005ad910
float ShipClass::TurnDirectionsToDirections( const NiPoint3 &curForward, const NiPoint3 &newForward, const NiPoint3 &curUp, const NiPoint3 &newUp ) {
	float angle;
	NiPoint3 axis;
	if ( curUp == NiPoint3::ZERO || newUp == NiPoint3::ZERO ) {
		GetTurnBetween( angle, axis, curForward, newForward );
		if ( axis == NiPoint3::ZERO ) {
			axis = GetWorldUp();
		}
		axis *= angle;
	} else {
		NiQuaternion turn = GetTurnBetweenFrames( curForward, curUp, newForward, newUp );
		turn.ToAngleAxis( angle, axis );
		if ( axis == NiPoint3::ZERO ) {
			axis = GetWorldUp();
		}
		axis *= -angle;
	}
	return TurnTowardDifference( &axis );
}

/*
================
ShipClass::Update

Updates the motion over the time since the last update (at least a
millisecond), keeps an in-system warp heading for its target, then
updates as a DamageableObject.
================
*/
// FUNCTION: STBC 0x005adae0
void ShipClass::Update( float time ) {
	float elapsed;
	if ( lastUpdateTime < 0.0f ) {
		elapsed = MIN_UPDATE_INTERVAL;
	} else {
		float delta = time - lastUpdateTime;
		if ( delta < MIN_UPDATE_INTERVAL ) {
			delta = MIN_UPDATE_INTERVAL;
		}
		elapsed = delta;
	}
	Unknown5B0900( elapsed );

	if ( IsDoingInSystemWarp() ) {
		ObjectClass *destination = ObjectClass::GetObjectByID( GetContainingSet(), inSystemWarpTargetID );
		if ( destination != NULL ) {
			InSystemWarp( destination, inSystemWarpDistance );
		}
	}
	Unknown5ADBD0();
	DamageableObject::Update( time );
}

/*
================
ShipClass::Unknown5ADB80

The tractor beam systems of the projectors that hold the ship, their
unknownFC added up. Nothing calls it.
================
*/
// FUNCTION: STBC 0x005adb80
float ShipClass::Unknown5ADB80( void ) {
	float total = 0.0f;
	NiTListIterator pos = tractorBeams.GetHeadPos();
	while ( pos != NULL ) {
		TractorBeamProjector *beam = tractorBeams.GetNext( pos );
		total += TractorBeamSystem::Cast( beam->GetParentSubsystem() )->GetUnknownFC();
	}
	return total;
}

/*
================
ShipClass::Unknown5ADBD0

Sets the accelerations from the impulse engines: toward the velocity the
impulse asks for and the target angular velocity, each change limited by
the engines' current maximum acceleration. A dying ship stops
accelerating, and one whose engines are disabled or off slows down.

The acceleration is stored through PhysicsObjectClass's SetAcceleration,
named with its class because ShipClass's own SetAcceleration (slot 91)
hides it; retail copies the point out of line inside that inline.
================
*/
// FUNCTION: STBC 0x005adbd0
// PARKED: inline budget: retail calls NiPoint3's constructor (0x414540), operator* (0x41CBD0) and SetAngularAcceleration out of line at more sites and expands the clamped-acceleration products; this build expands a different subset. Branches, the operator= and length calls match
void ShipClass::Unknown5ADBD0( void ) {
	ImpulseEngineSubsystem *engines = GetImpulseEngineSubsystem();
	if ( engines == NULL || IsDead() ) {
		return;
	}

	if ( lifeTime < INFINITE_LIFE ) {
		PhysicsObjectClass::SetAcceleration( NiPoint3::ZERO );
		SetAngularAcceleration( &NiPoint3::ZERO );
		return;
	}

	if ( !engines->IsDisabled() && engines->IsOn() ) {
		if ( unknown1E4 ) {
			NiPoint3 velocity;
			if ( impulseDirectionType == 0 ) {
				velocity = TGModelUtils::LocalToWorldVector( GetNode(), impulseDirection ) * impulse * engines->GetCurMaxSpeed();
			} else {
				velocity = impulse * impulseDirection * engines->GetCurMaxSpeed();
			}
			NiPoint3 change = velocity - GetVelocity();
			float length = sqrt( change.SqrLength() );
			NiPoint3 accel = change;
			float maxAccel = engines->GetCurMaxAccel();
			if ( length > maxAccel ) {
				accel = change * ( maxAccel / length );
			}
			PhysicsObjectClass::SetAcceleration( accel );
		}

		if ( usingTargetAngularVelocity ) {
			SetAngularDirectionType( targetAngularDirectionType );
			NiPoint3 change = targetAngularVelocity - angularAxis * angularSpeed;
			float length = sqrt( change.SqrLength() );
			NiPoint3 accel = change;
			float maxAccel = engines->GetCurMaxAngularAccel();
			if ( length > maxAccel ) {
				if ( length > 0.0f ) {
					accel = change * ( maxAccel / length );
					SetAngularAcceleration( &accel );
					return;
				}
				accel = NiPoint3::ZERO;
			}
			SetAngularAcceleration( &accel );
		}
	} else {
		NiPoint3 decel = -GetVelocity();
		float speed = decel.Unitize();
		if ( speed < disabledEngineDeceleration ) {
			decel *= speed;
		} else {
			decel *= disabledEngineDeceleration;
		}
		PhysicsObjectClass::SetAcceleration( decel );
	}
}

/*
================
ShipClass::SetTargetable
================
*/
// FUNCTION: STBC 0x005ae070
void ShipClass::SetTargetable( bool targetable_ ) {
	targetable = targetable_;
	SensorSubsystem::TargetableChanged( this );
}

/*
================
ShipClass::SetInvincible

Only the host decides in multiplayer. While the ship warps, its warp
engines keep the flag too.
================
*/
// FUNCTION: STBC 0x005ae090
void ShipClass::SetInvincible( bool invincible_ ) {
	// MATCH-PENDING: R17, the warp engines are read through ShipSubsystems::GetWarpEngineSubsystem (invented, ShipSubsystems.h): only the schedule shows a pointer other than `this` there (the flag's store stays ahead of the load); no other evidence of the accessor
	if ( !g_kUtopiaModule.IsMultiplayer() || g_kUtopiaModule.IsHost() ) {
		invincible = invincible_;
		WarpEngineSubsystem *warpEngines = GetWarpEngineSubsystem();
		if ( warpEngines != NULL && warpEngines->GetWarpState() != 0 ) {
			warpEngines->SetShipInvincible( invincible_ );
		}
	}
}

/*
================
ShipClass::SetHurtable

In multiplayer, only the host decides.
================
*/
// FUNCTION: STBC 0x005ae0d0
void ShipClass::SetHurtable( bool hurtable_ ) {
	if ( !g_kUtopiaModule.IsMultiplayer() || g_kUtopiaModule.IsHost() ) {
		hurtable = hurtable_;
	}
}

/*
================
ShipClass::CanTargetObject

Whether the object is another targetable object the ship's sensors see.
================
*/
// FUNCTION: STBC 0x005ae0f0
bool ShipClass::CanTargetObject( ObjectClass *object ) {
	if ( object == this ) {
		return false;
	}
	if ( !object->IsTargetable() ) {
		return false;
	}
	if ( subsystems.sensorSubsystem == NULL ) {
		return false;
	}
	if ( subsystems.sensorSubsystem->IsObjectVisible( object ) ) {
		return true;
	}
	return false;
}

/*
================
ShipClass::IsPlayerShip

In multiplayer, whether a player flies the ship; otherwise, whether it
is the player's.
================
*/
// FUNCTION: STBC 0x005ae140
bool ShipClass::IsPlayerShip( void ) {
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		return netPlayerID != 0;
	}
	return Game::GetCurrentPlayer() == this;
}

/*
================
ShipClass::GetTarget

The target, while it is in the ship's set and has a name; otherwise the
ship forgets it.
================
*/
// FUNCTION: STBC 0x005ae170
ObjectClass *ShipClass::GetTarget( void ) {
	ObjectClass *target = NULL;
	SetClass *set = GetContainingSet();
	if ( targetID != 0 && set != NULL ) {
		target = ObjectClass::GetObjectByID( set, targetID );
		if ( target == NULL || target->GetName() == NULL ) {
			target = NULL;
			targetID = 0;
		}
	}
	return target;
}

/*
================
ShipClass::SetTargetByID
================
*/
// FUNCTION: STBC 0x005ae1b0
void ShipClass::SetTargetByID( long id ) {
	if ( containingSet != NULL ) {
		ObjectClass *target = ObjectClass::GetObjectByID( containingSet, id );
		SetTarget( target, NULL );
	}
}

/*
================
ShipClass::SetTarget

Targets the object of the ship's set with the given name.
================
*/
// FUNCTION: STBC 0x005ae1e0 ?SetTarget@ShipClass@@QAEXPAD@Z
void ShipClass::SetTarget( char *name ) {
	if ( containingSet != NULL ) {
		ObjectClass *target = ObjectClass::GetObject( containingSet, name );
		SetTarget( target, NULL );
	}
}

/*
================
ShipClass::SetTarget

A new target stops the weapons and posts ET_TARGET_WAS_CHANGED with the
old target; an object without a name clears the target.
================
*/
// FUNCTION: STBC 0x005ae210 ?SetTarget@ShipClass@@QAEXPAVObjectClass@@PAVShipSubsystem@@@Z
void ShipClass::SetTarget( ObjectClass *target, ShipSubsystem *subsystem ) {
	ObjectClass *oldTarget = GetTarget();
	if ( oldTarget == NULL || oldTarget != target ) {
		if ( target != NULL && target->GetName() != NULL ) {
			targetID = target->GetObjID();
		} else {
			targetID = 0;
		}
		StopFiringWeapons();

		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetEventType( ET_TARGET_WAS_CHANGED );
		event->SetDestination( this );
		event->SetSource( subsystem );
		event->SetObjPtr( oldTarget );
		g_kEventManager.AddEvent( event );
	}
	SetTargetSubsystem( subsystem );
}

/*
================
ShipClass::SetTargetSubsystem

Without a subsystem, the target ship's hull. The player's weapon systems
aim at the subsystem's damage point, and a change posts
ET_TARGETED_SUBSYSTEM_CHANGED.
================
*/
// FUNCTION: STBC 0x005ae2c0
void ShipClass::SetTargetSubsystem( ShipSubsystem *subsystem ) {
	bool changed = false;
	if ( subsystem == NULL ) {
		ShipClass *ship = ShipClass::Cast( GetTarget() );
		if ( ship != NULL ) {
			subsystem = ship->GetHull();
		} else if ( targetSubsystemID != 0 ) {
			changed = true;
			targetSubsystemID = 0;
		}
	}
	// BUG: compares the subsystem's ID with the target object's ID, not with targetSubsystemID, so re-targeting the same subsystem still counts as a change
	if ( subsystem != NULL && targetID != subsystem->GetObjID() ) {
		changed = true;
		targetSubsystemID = subsystem->GetObjID();
		UseTargetOffsetTG( false, NULL );
	}

	if ( this == Game::GetCurrentPlayer() && subsystem != NULL ) {
		PhaserSystem *phasers = GetPhaserSystem();
		TorpedoSystem *torpedoes = GetTorpedoSystem();
		PulseWeaponSystem *pulseWeapons = GetPulseWeaponSystem();
		TractorBeamSystem *tractorBeams = GetTractorBeamSystem();
		ShipClass *ship = subsystem->GetParentShip();
		NiPoint3 offset = subsystem->GetDamagePoint();
		if ( phasers != NULL ) {
			phasers->SetTargetOffset( ship, offset );
		}
		if ( torpedoes != NULL ) {
			torpedoes->SetTargetOffset( ship, offset );
		}
		if ( pulseWeapons != NULL ) {
			pulseWeapons->SetTargetOffset( ship, offset );
		}
		if ( tractorBeams != NULL ) {
			tractorBeams->SetTargetOffset( ship, offset );
		}
	}

	if ( changed ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_TARGETED_SUBSYSTEM_CHANGED );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
ShipClass::UseTargetOffsetTG

The weapon systems' target entries and the weapons aiming at the target
take the new offset. Turning the offset off when it was already off
changes nothing.
================
*/
// FUNCTION: STBC 0x005ae430
void ShipClass::UseTargetOffsetTG( bool use, TGPoint3 *offset ) {
	bool wasUsing = usingTargetOffset;
	usingTargetOffset = use;
	NiPoint3 newOffset;
	if ( use && offset != NULL ) {
		newOffset = targetOffset = *offset;
	} else {
		if ( wasUsing == use ) {
			return;
		}
		newOffset = GetTargetOffset();
	}

	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		WeaponSystem *system = WeaponSystem::Cast( subsystems.list.GetNext( pos ) );
		if ( system != NULL ) {
			WeaponSystemTarget *target = system->FindTarget( targetID );
			if ( target != NULL ) {
				target->offset = newOffset;
			}
			for ( int i = 0; i < system->GetNumChildSubsystems(); i++ ) {
				Weapon *weapon = system->GetWeapon( i );
				if ( weapon != NULL && weapon->GetTargetID() == targetID ) {
					weapon->SetTarget( targetID, newOffset );
				}
			}
		}
	}

	TGEvent *event = new TGEvent;
	event->SetEventType( ET_TARGET_OFFSET_CHANGED );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}

/*
================
ShipClass::SetAI

While the ship warps, the warp engines keep the AI to set afterwards.
================
*/
// FUNCTION: STBC 0x005ae5a0
int ShipClass::SetAI( ArtificialIntelligence *ai, bool unknown1, bool unknown2 ) {
	WarpEngineSubsystem *warpEngines = subsystems.warpEngineSubsystem;
	if ( warpEngines != NULL && warpEngines->GetWarpState() != 0 ) {
		ArtificialIntelligence *engineAI = ai->VirtualSlot13();
		warpEngines->SetAI( engineAI, unknown1, unknown2 );
		return engineAI->GetID();
	}
	return PhysicsObjectClass::SetAI( ai, unknown1, unknown2 );
}

/*
================
ShipClass::ClearAI

The warp engines forget the AI too.
================
*/
// FUNCTION: STBC 0x005ae600
void ShipClass::ClearAI( bool unknown, ArtificialIntelligence *ai ) {
	if ( subsystems.warpEngineSubsystem != NULL ) {
		subsystems.warpEngineSubsystem->ClearAI( unknown, ai );
	}
	PhysicsObjectClass::ClearAI( unknown, ai );
}

/*
================
ShipClass::GetTargetSubsystem
================
*/
// FUNCTION: STBC 0x005ae630
ShipSubsystem *ShipClass::GetTargetSubsystem( void ) {
	if ( targetSubsystemID == 0 ) {
		return NULL;
	}
	return (ShipSubsystem *)GetTGObjectPtr( targetSubsystemID );
}

/*
================
ShipClass::GetTargetOffset
================
*/
// FUNCTION: STBC 0x005ae650
const NiPoint3 &ShipClass::GetTargetOffset( void ) {
	if ( usingTargetOffset ) {
		return targetOffset;
	}
	ShipSubsystem *subsystem = GetTargetSubsystem();
	if ( subsystem != NULL ) {
		targetOffset = subsystem->GetDamagePoint();
	} else {
		targetOffset = NiPoint3::ZERO;
	}
	return targetOffset;
}

/*
================
ShipClass::GetNextTarget

Targets the next object of the set, in ID order after the current target,
that the ship can target.
================
*/
// FUNCTION: STBC 0x005ae6d0
void ShipClass::GetNextTarget( void ) {
	SetClass *set = containingSet;
	if ( set == NULL ) {
		return;
	}

	int count = set->objects.GetCount();
	long id = targetID;
	while ( count-- ) {
		ObjectClass *object = set->GetNextObject( id );
		if ( CanTargetObject( object ) ) {
			SetTarget( object, NULL );
			return;
		}
		id = object->GetObjID();
	}
}

// The shield percentages between which a shield lets part of a hit
// through: all of SHIELD_BLEED_THROUGH below the low one, none above the
// high one (invented names).
#define SHIELD_BLEED_LOW		0.1f
#define SHIELD_BLEED_HIGH		0.6f
#define SHIELD_BLEED_THROUGH	0.6f

/*
================
ShipClass::VirtualSlot80

Tests the segment from start to end against the shield bubble (the unit
sphere in the frame of the ship's box) and the hull. A start outside the
bubble hits a facing that is up first; a start inside it hits the hull
first, then the bubble from inside. A shield hit names its facing in
hitShield and gives the world point and normal. With ignoreShields only
the hull is tested. A hidden ship is shown for the test.
================
*/
// FUNCTION: STBC 0x005ae730
// PARKED: inline budget and block layout: the first modelEnd - modelStart's NiPoint3 constructor stays out of line (share 46 against 47: 18 more units or one fewer later site), and retail lays the outside path's hull fallback before the shield body
bool ShipClass::VirtualSlot80( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, bool ignoreShields ) {
	bool hidden = false;
	if ( IsHidden() == true ) {
		SetHidden( false );
		hidden = true;
	}
	hitShield = -1;

	bool result;
	if ( ignoreShields ) {
		result = DamageableObject::VirtualSlot80( start, end, hitPoint, hitNormal, ignoreShields );
	} else {
		NiPoint3 location = GetWorldLocation();
		// the two points in the frame of the shield bubble, where it is the unit sphere
		NiPoint3 modelStart = ( *start - location ) * GetNode()->GetWorldRotate() - boxCenter;
		modelStart.x /= boxExtent.x;
		modelStart.y /= boxExtent.y;
		modelStart.z /= boxExtent.z;
		NiPoint3 modelEnd = ( *end - location ) * GetNode()->GetWorldRotate() - boxCenter;
		modelEnd.x /= boxExtent.x;
		modelEnd.y /= boxExtent.y;
		modelEnd.z /= boxExtent.z;
		NiPoint3 exitPoint;
		if ( modelStart.SqrLength() > 1.0f ) {
			// outside the bubble: the shields first
			result = false;
			if ( Unknown4570D0( modelStart, modelEnd - modelStart, NiPoint3::ZERO, 1.0f, hitPoint, &exitPoint ) ) {
				ShieldClass *shields = GetShields();
				int shield = shields->Unknown56A8D0( hitPoint );
				if ( shields->GetSingleShieldPercentage( shield ) >= 0.1f ) {
					hitShield = shield;
					*hitNormal = *hitPoint;
					hitPoint->x = boxExtent.x * hitPoint->x;
					hitPoint->y = boxExtent.y * hitPoint->y;
					hitPoint->z = boxExtent.z * hitPoint->z;
					// BUG: the box center is added in the world frame, not rotated with the point
					*hitPoint = location + GetNode()->GetWorldRotate() * *hitPoint + boxCenter;
					*hitNormal = GetNode()->GetWorldRotate() * *hitNormal;
					result = true;
				}
			}
			if ( !result ) {
				result = DamageableObject::VirtualSlot80( start, end, hitPoint, hitNormal, false );
			}
		} else {
			result = DamageableObject::VirtualSlot80( start, end, hitPoint, hitNormal, false );
			if ( !result && Unknown4570D0( modelStart, modelEnd - modelStart, NiPoint3::ZERO, 1.0f, hitPoint, &exitPoint ) ) {
				ShieldClass *shields = GetShields();
				int shield = shields->Unknown56A8D0( hitPoint );
				if ( shields->GetSingleShieldPercentage( shield ) > 0.1f ) {
					hitShield = shield;
					*hitNormal = *hitPoint;
					hitPoint->x = boxExtent.x * hitPoint->x;
					hitPoint->y = boxExtent.y * hitPoint->y;
					hitPoint->z = boxExtent.z * hitPoint->z;
					// BUG: the box center is added in the world frame, not rotated with the point
					*hitPoint = location + GetNode()->GetWorldRotate() * *hitPoint + boxCenter;
					*hitNormal = GetNode()->GetWorldRotate() * *hitNormal;
					result = true;
				}
			}
		}
	}

	if ( hidden == true ) {
		SetHidden( true );
	}
	return result;
}

/*
================
ShipClass::FindSubsystemsInRange
================
*/
// FUNCTION: STBC 0x005aeb90
// PARKED: register allocation: retail keeps the subsystem in EDI and zero (the loop counter) in ESI, and loads the child count before comparing it; this build swaps EDI and ESI. Calls, pushes and branches match
void ShipClass::FindSubsystemsInRange( const NiPoint3 &position, float radius, ShipSubsystem *subsystem, NiTList<ShipSubsystem *> &found ) {
	if ( subsystem->IsTargetable() == true && subsystem->GetCondition() > 0.0f ) {
		NiPoint3 offset = position - subsystem->GetDamagePoint();
		float range = subsystem->GetRadius() + radius;
		if ( offset.SqrLength() < range * range ) {
			found.AddTail( subsystem );
		}
	}
	for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
		ShipSubsystem *child = subsystem->GetChildSubsystem( i );
		FindSubsystemsInRange( position, radius, child, found );
	}
}

/*
================
ShipClass::FindSubsystemsNear
================
*/
// FUNCTION: STBC 0x005aecc0
void ShipClass::FindSubsystemsNear( const NiPoint3 &position, float radius, NiTList<ShipSubsystem *> &found ) {
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = subsystems.list.GetNext( pos );
		if ( GetHull() != subsystem ) {
			FindSubsystemsInRange( position, radius, subsystem, found );
		}
	}
	HullClass *hull = GetHull();
	if ( !found.FindPos( hull ) ) {
		found.AddTail( hull );
	}
}

/*
================
ShipClass::VirtualSlot81

A weapon hit at the world point and normal: the hit becomes a
WeaponHitEvent in the model frame. A hit on a shield (the one
hitShield names) drains the shield, which lets part of the damage
through as it weakens; a hull hit pushes the ship and damages the
subsystems near it. Returns whether the damage is used up. The player's
ship in god mode and a ship that can't be hurt take nothing.
================
*/
// FUNCTION: STBC 0x005aed90 ?VirtualSlot81@ShipClass@@UAE_NPBVNiPoint3@@0PAV2@1PAMPAVWeaponPayload@@PAVTGObject@@PAVObjectClass@@@Z
bool ShipClass::VirtualSlot81( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, float *damage, WeaponPayload *payload, TGObject *source, ObjectClass *firingObject ) {
	if ( this == Game::GetCurrentPlayer() && Game::GetCurrentGame()->InGodMode() ) {
		return true;
	}
	if ( !IsHurtable() ) {
		return true;
	}

	int shield = hitShield;
	hitShield = -1;
	NiPoint3 modelHitPoint = TGModelUtils::WorldToLocalPoint( GetNode(), *hitPoint );
	NiPoint3 modelHitNormal = TGModelUtils::WorldToLocalVector( GetNode(), *hitNormal );
	WeaponHitEvent *event = CreateWeaponHitEvent( &modelHitPoint, &modelHitNormal, *damage, 0.0f, payload, source, firingObject );
	switch ( payload->GetPayloadType() ) {
		case WeaponPayload::WP_PHASER:
			event->SetWeaponType( WeaponHitEvent::PHASER );
			break;
		case WeaponPayload::WP_TRACTOR:
			event->SetWeaponType( WeaponHitEvent::TRACTOR_BEAM );
			break;
		default:
			event->SetWeaponType( WeaponHitEvent::TORPEDO );
			break;
	}

	if ( shield != -1 ) {
		event->SetHullHit( false );
		ShieldClass *shields = GetShields();
		float level = shields->GetCurShields( shield );
		float percentage = shields->GetSingleShieldPercentage( shield );
		float bleedThrough = 0.0f;
		if ( percentage > SHIELD_BLEED_LOW ) {
			if ( percentage < SHIELD_BLEED_HIGH ) {
				bleedThrough = ( 1.0f - ( percentage - SHIELD_BLEED_LOW ) * 2.0f ) * SHIELD_BLEED_THROUGH;
			}
		} else {
			bleedThrough = SHIELD_BLEED_THROUGH;
		}
		level -= ( 1.0f - bleedThrough ) * *damage;
		if ( level < 0.0f ) {
			// what the drained shield couldn't hold, its shortfall below
			// zero, goes through too
			*damage = bleedThrough * *damage + ( 0.0f - level );
			level = 0.0f;
		} else {
			*damage = bleedThrough * *damage;
		}
		if ( g_kUtopiaModule.IsHost() || !g_kUtopiaModule.IsMultiplayer() ) {
			shields->SetCurShields( shield, level );
		}
		event->SetCondition( level / shields->GetMaxShields( shield ) );
	} else {
		event->SetHullHit( true );
		event->SetCondition( GetHull()->GetConditionPercentage() );
		if ( g_kUtopiaModule.IsHost() || !g_kUtopiaModule.IsMultiplayer() ) {
			ApplyWeaponHit( &modelHitPoint, damage, payload, source );
		}
	}

	g_kEventManager.ProcessEvent( event );
	delete event;
	return *damage <= 0.0f;
}

/*
================
ShipClass::WeaponHitHandler

A hit on the shields flashes them, and a torpedo whose full damage got
through plays Effects.TorpedoShieldHit; a hull hit plays
Effects.PhaserHullHit or TorpedoHullHit and adds a damage volume.
================
*/
// FUNCTION: STBC 0x005af010
void ShipClass::WeaponHitHandler( TGEvent *event ) {
	WeaponHitEvent *hit = (WeaponHitEvent *)event;
	int weaponType = hit->GetWeaponType();
	if ( !hit->IsHullHit() ) {
		ShowShieldHit( hit );
		if ( weaponType == WeaponHitEvent::TORPEDO ) {
			Torpedo *torpedo = Torpedo::Cast( hit->GetSource() );
			if ( torpedo != NULL && torpedo->GetDamage() == hit->GetDamage() ) {
				PyObject *pyShip = VirtualSlot08();
				PyObject *pyEvent = hit->VirtualSlot08();
				PyEmbed::CallFunction( "Effects", "TorpedoShieldHit", NULL, NULL, "OO", pyShip, pyEvent );
				Py_DECREF( pyShip );
				Py_DECREF( pyEvent );
			}
		}
	} else {
		char *function = NULL;
		if ( weaponType == WeaponHitEvent::PHASER ) {
			function = "PhaserHullHit";
		} else if ( weaponType == WeaponHitEvent::TORPEDO ) {
			function = "TorpedoHullHit";
		}
		if ( function != NULL ) {
			PyObject *pyShip = VirtualSlot08();
			PyObject *pyEvent = hit->VirtualSlot08();
			PyEmbed::CallFunction( "Effects", function, NULL, NULL, "OO", pyShip, pyEvent );
			Py_DECREF( pyShip );
			Py_DECREF( pyEvent );
		}
		AddWeaponHitDamage( hit );
	}
	CallNextHandler( event );
}

// The weakest hit that lights a shield patch (invented name).
#define MIN_SHIELD_HIT_CONDITION	0.1f
// The scale of a lit shield patch (invented name).
#define SHIELD_HIT_SCALE			0.8f

/*
================
ShipClass::ShowShieldHit
================
*/
// FUNCTION: STBC 0x005af160
void ShipClass::ShowShieldHit( WeaponHitEvent *event ) {
	if ( event->GetCondition() <= 0.0f ) {
		if ( shieldPatches != NULL ) {
			shieldPatches->Unknown4EA170( event->GetObjectHitNormal() );
		}
		return;
	}
	float condition = event->GetCondition();
	if ( condition <= MIN_SHIELD_HIT_CONDITION ) {
		return;
	}

	TGColorA color = subsystems.shields->GetShieldGlowColor();
	color = color * condition;
	if ( shieldPatches == NULL ) {
		shieldPatches = new ShieldPatchParticles( color, boxExtent, boxCenter, g_kImageManager.GetImage( "data/sphere.tga", true, true ) );
		GetNode()->AttachChild( shieldPatches, true );
		shieldPatches->Update( 0.0f );
	}

	// BUG: no default: an unknown effect level leaves the size unset
	int size;
	switch ( EffectController::GetEffectLevel() ) {
		case 0:
			size = 64;
			break;
		case 1:
			size = 96;
			break;
		case 2:
			size = 128;
			break;
		case 3:
			size = 256;
			break;
		case 4:
			size = 512;
			break;
	}
	shieldPatches->Unknown4E9FB0( UtopiaApp::GetApp()->GetGameTime(), event->GetObjectHitNormal(), size, SHIELD_HIT_SCALE );
}

/*
================
ShipClass::AddWeaponHitDamage
================
*/
// FUNCTION: STBC 0x005af420
void ShipClass::AddWeaponHitDamage( WeaponHitEvent *event ) {
	int weaponType = event->GetWeaponType();
	if ( weaponType == WeaponHitEvent::PHASER || weaponType == WeaponHitEvent::TORPEDO ) {
		AddDamageVolume( event->GetObjectHitPoint(), event->GetRadius() * 2.0f, event->GetDamage() * 0.5f );
	}
}
/*
================
ShipClass::Unknown5AF460
================
*/
// FUNCTION: STBC 0x005af460
void ShipClass::Unknown5AF460( void ) {
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = subsystems.list.GetNext( pos );
		if ( subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
			subsystem->SetCondition( 0.0f );
		}
	}
}

/*
================
ShipClass::DamageSystem
================
*/
// FUNCTION: STBC 0x005af4a0
float ShipClass::DamageSystem( ShipSubsystem *system, float damage, TGObject *source, bool destroy ) {
	if ( this == Game::GetCurrentPlayer() && Game::GetCurrentGame()->InGodMode() ) {
		return 0.0f;
	}
	if ( !hurtable ) {
		return 0.0f;
	}
	float condition = system->GetCondition();
	float maxCondition = system->GetMaxCondition();
	float overflow = 0.0f;
	condition -= damage;
	if ( condition <= 0.0f || destroy ) {
		if ( condition <= 0.0f ) {
			overflow = -condition;
		}
		condition = 0.0f;
		if ( invincible == true && system->IsCritical() == true ) {
			condition = system->GetMaxCondition() * 0.01f;
			overflow = 0.0f;
			destroy = false;
		}
	}
	if ( system->IsInvincible() == true ) {
		if ( condition / maxCondition < system->GetDisabledPercentage() ) {
			condition = ( system->GetDisabledPercentage() + 0.01f ) * maxCondition;
			overflow = 0.0f;
		}
	}
	system->SetCondition( condition );
	if ( ( condition <= 0.0f || destroy ) && system->IsCritical() ) {
		Unknown5AFEA0( source );
	}
	return overflow;
}

/*
================
ShipClass::DestroySystem

Damages the subsystem by its whole maximum condition; returns what the
damage call returns, 0 without a subsystem.
================
*/
// FUNCTION: STBC 0x005af5f0
float ShipClass::DestroySystem( ShipSubsystem *system ) {
	if ( system == NULL ) {
		return 0.0f;
	}
	return DamageSystem( system, system->GetMaxCondition(), NULL, true );
}

/*
================
ShipClass::ApplyWeaponHit

The hit pushes the ship with the source's momentum at the hit point and
damages the subsystems around it; a phaser at its lowest power setting
spares the hull (invented name).
================
*/
// FUNCTION: STBC 0x005af630
void ShipClass::ApplyWeaponHit( const NiPoint3 *point, float *strength, WeaponPayload *payload, TGObject *source ) {
	if ( source != NULL && source->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
		PhysicsObjectClass *object = (PhysicsObjectClass *)source;
		ApplyForce( &Force( TGModelUtils::LocalToWorldPoint( GetNode(), *point ), object->GetMass() * object->GetVelocity(), NiPoint3::ZERO ) );
	}

	bool includeHull = true;
	if ( payload->GetPayloadType() == WeaponPayload::WP_PHASER ) {
		PhaserBank *bank = PhaserBank::Cast( source );
		if ( bank != NULL && bank->GetPowerSetting() == EnergyWeapon::EW_LOW ) {
			includeHull = false;
		}
	}
	Unknown5AFD70( *point, strength, payload->GetDamageRadiusFactor(), source, includeHull );
}

/*
================
ShipClass::VirtualSlot82

The base's work, with the ship shown while it runs.
================
*/
// FUNCTION: STBC 0x005af7d0
bool ShipClass::VirtualSlot82( DamageableObject *other, CollisionRecorder *recorder ) {
	bool wasHidden = false;
	if ( IsHidden() == true ) {
		SetHidden( false );
		wasHidden = true;
	}
	bool result = DamageableObject::VirtualSlot82( other, recorder );
	if ( wasHidden == true ) {
		SetHidden( true );
	}
	return result;
}

/*
================
ShipClass::VirtualSlot83

The base's work, with the ship shown while it runs.
================
*/
// FUNCTION: STBC 0x005af830
bool ShipClass::VirtualSlot83( CollisionTest *other, CollisionRecorder *recorder ) {
	bool wasHidden = false;
	if ( IsHidden() == true ) {
		SetHidden( false );
		wasHidden = true;
	}
	bool result = DamageableObject::VirtualSlot83( other, recorder );
	if ( wasHidden == true ) {
		SetHidden( true );
	}
	return result;
}

/*
================
ShipClass::VirtualSlot84

In multiplayer, when this is the current game's player ship and the
object is a ship a player flies, first records where this ship is, how it
faces and its speed. Then the base's work.
================
*/
// FUNCTION: STBC 0x005af890
void ShipClass::VirtualSlot84( CollisionRecorder *recorder, DamageableObject *other, int index ) {
	if ( g_kUtopiaModule.IsMultiplayer() && Game::GetCurrentGame() != NULL && this == Game::GetCurrentGame()->GetPlayer() ) {
		ShipClass *ship = ShipClass::Cast( other );
		if ( ship != NULL && ship->IsPlayerShip() ) {
			unknown2FC = true;
			unknown300 = GetWorldLocation();
			unknown30C = GetWorldForward();
			unknown318 = GetWorldUp();
			unknown324 = (float)sqrt( GetVelocity().SqrLength() );
			if ( IsReverse() ) {
				unknown324 = -unknown324;
			}
		}
	}
	DamageableObject::VirtualSlot84( recorder, other, index );
}

// Whose ship a collision party is, in a multiplayer game.
enum {
	NOT_PLAYER_SHIP,
	REMOTE_PLAYER_SHIP,
	LOCAL_PLAYER_SHIP
};

/*
================
ShipClass::CollisionEffectHandler

In a multiplayer game a collision with a player's ship is handled on the
host. On a client the effect happens only when the local player's ship is
one of the two; the host then tells the others.
================
*/
// FUNCTION: STBC 0x005af9c0
// PARKED: register allocation: retail spills the current game to a stack slot right after the two casts and keeps the two player kinds in ebx and eax; this build holds the game in ebx
void ShipClass::CollisionEffectHandler( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsMultiplayer() ) {
		Unknown5AFAD0( event );
	} else {
		ShipClass *destinationShip = ShipClass::Cast( event->GetDestination() );
		ShipClass *sourceShip = ShipClass::Cast( event->GetSource() );
		Game *game = Game::GetCurrentGame();
		int destinationKind = ( destinationShip != NULL && destinationShip->IsPlayerShip() ) ? ( destinationShip == game->GetPlayer() ? LOCAL_PLAYER_SHIP : REMOTE_PLAYER_SHIP ) : NOT_PLAYER_SHIP;
		int sourceKind = ( sourceShip != NULL && sourceShip->IsPlayerShip() ) ? ( sourceShip == game->GetPlayer() ? LOCAL_PLAYER_SHIP : REMOTE_PLAYER_SHIP ) : NOT_PLAYER_SHIP;
		if ( destinationKind == NOT_PLAYER_SHIP && sourceKind == NOT_PLAYER_SHIP ) {
			Unknown5AFAD0( event );
		} else if ( destinationKind == LOCAL_PLAYER_SHIP || sourceKind == LOCAL_PLAYER_SHIP ) {
			if ( !( destinationKind == REMOTE_PLAYER_SHIP && sourceKind == LOCAL_PLAYER_SHIP ) ) {
				if ( destinationShip == NULL ) {
					Unknown5AFAD0( event );
				} else {
					if ( sourceShip == NULL ) {
						event->SetSource( sourceShip );
					}
					event->SetSource( NULL );
					static_cast<MultiplayerGame *>( game )->SendEventMessage( event, CLIENT_COLLISION_MESSAGE );
					if ( g_kUtopiaModule.IsHost() ) {
						Unknown5AFAD0( event );
					}
				}
			}
		}
	}
	CallNextHandler( event );
}

/*
================
ShipClass::HostCollisionEffectHandler
================
*/
// FUNCTION: STBC 0x005afab0
void ShipClass::HostCollisionEffectHandler( TGEvent *event ) {
	Unknown5AFAD0( event );
	CallNextHandler( event );
}

/*
================
ShipClass::Unknown5AFAD0

The collision's damage: a cloaked ship tells what it hit, then each point
of contact, taken into the model frame, damages the ship with the force's
share over the mass. The player's ship in god mode and an unhurtable ship
take none outside multiplayer, and nothing is done on a multiplayer client.
================
*/
// FUNCTION: STBC 0x005afad0
void ShipClass::Unknown5AFAD0( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsHost() && g_kUtopiaModule.IsMultiplayer() ) {
		return;
	}

	CollisionEvent *collision = (CollisionEvent *)event;
	CloakingSubsystem *cloak = GetCloakingSubsystem();
	if ( cloak != NULL && cloak->IsCloaked() && event->GetSource() != NULL ) {
		TGEvent *cloaked = new TGEvent;
		cloaked->SetEventType( ET_CLOAKED_COLLISION );
		cloaked->SetSource( this );
		cloaked->SetDestination( (TGEventHandlerObject *)event->GetSource() );
		g_kEventManager.AddEvent( cloaked );
	}

	if ( this == Game::GetCurrentPlayer() && Game::GetCurrentGame()->InGodMode() && !g_kUtopiaModule.IsMultiplayer() ) {
		return;
	}
	if ( !hurtable && !g_kUtopiaModule.IsMultiplayer() ) {
		return;
	}
	if ( collisionDamageDisabled ) {
		return;
	}

	float strength = collision->GetCollisionForce() / GetMass() / collision->GetNumPoints();
	if ( strength > 0.01f ) {
		strength = strength * 900.0f + 500.0f;
	}
	for ( int i = 0; i < collision->GetNumPoints(); i++ ) {
		NiPoint3 point = collision->GetPoint( i );
		NiPoint3 local = TGModelUtils::WorldToLocalPoint( GetNode(), point );
		Unknown5AFD70( local, &strength, 1.5f, event->GetSource(), true );
	}
}

/*
================
ShipClass::PlanetCollisionHandler

A hurtable ship that hits a planet stops dead and loses its hull, unless
it or its hull is invincible or it is the player's ship in god mode. Only
the host decides in multiplayer.
================
*/
// FUNCTION: STBC 0x005afca0
void ShipClass::PlanetCollisionHandler( TGEvent *event ) {
	if ( g_kUtopiaModule.IsHost() || !g_kUtopiaModule.IsMultiplayer() ) {
		Game *game = Game::GetCurrentGame();
		if ( this == game->GetPlayer() && game->InGodMode() ) {
			return;
		}
		if ( !IsHurtable() ) {
			return;
		}
		HullClass *hull = GetHull();
		if ( IsInvincible() || ( hull != NULL && hull->IsInvincible() ) ) {
			return;
		}

		SetVelocity( &NiPoint3::ZERO );
		VirtualSlot90( NiPoint3::ZERO );
		SetAngularVelocity( &NiPoint3::ZERO, 1 );
		SetLimitedAngularAcceleration( NiPoint3::ZERO, 1 );
		DestroySystem( hull );
	}
	CallNextHandler( event );
}

/*
================
ShipClass::Unknown5AFD70

Damages the subsystems within the radius of the position (in the model
frame), each by the strength; without includeHull the hull is spared when
anything else was hit. The strength becomes the damage left over.
================
*/
// FUNCTION: STBC 0x005afd70
void ShipClass::Unknown5AFD70( const NiPoint3 &position, float *strength, float radius, TGObject *source, bool includeHull ) {
	NiTList<ShipSubsystem *> hit( 2 );
	FindSubsystemsNear( position, radius, hit );
	if ( !includeHull && hit.GetSize() > 1 ) {
		hit.Remove( GetHull() );
	}

	float leftover = 0.0f;
	NiTListIterator pos = hit.GetHeadPos();
	while ( pos ) {
		leftover += DamageSystem( hit.GetNext( pos ), *strength, source, false );
	}
	*strength = leftover;
}

/*
================
ShipClass::Unknown5AFEA0
================
*/
// FUNCTION: STBC 0x005afea0
// PARKED: block layout: retail lays the firing-player body after both arms of the multiplayer test (the multiplayer arm jumps to it) and the no-killer store last; this build puts the store after the multiplayer arm. Calls, tests and stores match (if/else, ||, ternary and negated forms tried)
void ShipClass::Unknown5AFEA0( TGObject *source ) {
	if ( lifeTime < INFINITE_LIFE || dead ) {
		return;
	}
	if ( shieldPatches != NULL ) {
		shieldPatches = NULL;
	}
	SetTargetByID( 0 );
	StopFiringWeapons();
	Unknown5AF460();
	RunDeathScript();

	ObjectExplodingEvent *event = new ObjectExplodingEvent;
	event->SetDestination( this );
	event->SetLifetime( lifeTime );
	event->SetEventType( ET_OBJECT_EXPLODING );
	if ( g_kUtopiaModule.IsMultiplayer() ? g_kUtopiaModule.IsHost() && source != NULL
		: Game::GetCurrentGame() != NULL && Game::GetCurrentPlayer() != NULL && source != NULL ) {
		if ( source->IsTypeOf( TORPEDO_TYPE ) ) {
			event->SetFiringPlayerID( Torpedo::Cast( source )->GetPlayerID() );
		} else if ( source->IsTypeOf( WEAPON_TYPE ) || source->IsTypeOf( SHIPCLASS_TYPE ) ) {
			ShipClass *ship;
			if ( source->IsTypeOf( WEAPON_TYPE ) ) {
				ship = Weapon::Cast( source )->GetParentShip();
			} else {
				ship = ShipClass::Cast( source );
			}
			if ( g_kUtopiaModule.IsMultiplayer() ) {
				event->SetFiringPlayerID( ship->GetNetPlayerID() );
			} else {
				event->SetFiringPlayerID( ship->GetObjID() );
			}
		}
	} else {
		event->SetFiringPlayerID( 0 );
	}
	g_kEventManager.AddEvent( event );
}

/*
================
ShipClass::AddDamage

The hit first passes through the ship's own handling, which may reduce
its strength.
================
*/
// FUNCTION: STBC 0x005b0060
void ShipClass::AddDamage( NiAVObject *object, float radius, float strength ) {
	Unknown5AFD70( object->GetWorldTranslate(), &strength, radius, NULL, true );
	DamageableObject::AddDamage( object, radius, strength );
}

/*
================
ShipClass::Destroyed

The ship is destroyed: the power plant and its batteries are emptied and
the shields go down, every facing. Then the next handler runs.
================
*/
// FUNCTION: STBC 0x005b00a0
void ShipClass::Destroyed( TGEvent *event ) {
	PowerSubsystem *power = GetPowerSubsystem();
	power->SetCondition( 0.0f );
	power->SetAvailablePower( 0.0f );
	power->SetMainBatteryPower( 0.0f );
	power->SetBackupBatteryPower( 0.0f );

	ShieldClass *shields = GetShields();
	shields->SetCondition( 0.0f );
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
		shields->SetCurShields( i, 0.0f );
	}
	CallNextHandler( event );
}

/*
================
ShipClass::SetAlertLevel

Powers the ship's systems for the alert level: red alert turns every
system on; yellow alert keeps the weapons off but the shields and the
other systems on; green alert turns the shields and the weapons off.
A system turned on runs at its full power percentage.
================
*/
// FUNCTION: STBC 0x005b0110
// PARKED: inline budget: retail expands AddTail at 13 sites and calls GetNext out of line; every local/loop spelling tried expands 11-12 (getters at the call sites give 13 but no hoisted loads)
void ShipClass::SetAlertLevel( int level ) {
	PoweredSubsystem *torpedoes = GetTorpedoSystem();
	PoweredSubsystem *pulseWeapons = GetPulseWeaponSystem();
	PoweredSubsystem *tractorBeams = GetTractorBeamSystem();
	PoweredSubsystem *shields = GetShields();
	PoweredSubsystem *phasers = GetPhaserSystem();
	PoweredSubsystem *impulseEngines = GetImpulseEngineSubsystem();
	PoweredSubsystem *warpEngines = GetWarpEngineSubsystem();
	PoweredSubsystem *sensors = GetSensorSubsystem();

	NiTList<PoweredSubsystem *> turnOn( 2 );
	NiTList<PoweredSubsystem *> turnOff( 2 );

	switch ( level ) {
	case GREEN_ALERT:
		turnOff.AddTail( shields );
		turnOff.AddTail( phasers );
		turnOff.AddTail( torpedoes );
		turnOff.AddTail( pulseWeapons );
		turnOn.AddTail( tractorBeams );
		turnOn.AddTail( impulseEngines );
		turnOn.AddTail( warpEngines );
		turnOn.AddTail( sensors );
		alertLevel = GREEN_ALERT;
		break;
	case YELLOW_ALERT:
		turnOn.AddTail( shields );
		turnOff.AddTail( phasers );
		turnOff.AddTail( torpedoes );
		turnOff.AddTail( pulseWeapons );
		turnOn.AddTail( tractorBeams );
		turnOn.AddTail( impulseEngines );
		turnOn.AddTail( warpEngines );
		turnOn.AddTail( sensors );
		alertLevel = YELLOW_ALERT;
		break;
	case RED_ALERT:
		turnOn.AddTail( shields );
		turnOn.AddTail( phasers );
		turnOn.AddTail( torpedoes );
		turnOn.AddTail( pulseWeapons );
		turnOn.AddTail( tractorBeams );
		turnOn.AddTail( impulseEngines );
		turnOn.AddTail( warpEngines );
		turnOn.AddTail( sensors );
		alertLevel = RED_ALERT;
		break;
	}

	NiTListIterator pos = turnOn.GetHeadPos();
	while ( pos ) {
		PoweredSubsystem *system = turnOn.GetNext( pos );
		if ( system != NULL && !system->IsOn() ) {
			system->TurnOn();
			system->SetPowerPercentageWanted( 1.0f );
		}
	}

	pos = turnOff.GetHeadPos();
	while ( pos ) {
		PoweredSubsystem *system = turnOff.GetNext( pos );
		if ( system != NULL ) {
			system->TurnOff();
		}
	}
}

/*
================
ShipClass::HandleChangeAlertLevel

The event's int is the new alert level.
================
*/
// FUNCTION: STBC 0x005b0760
void ShipClass::HandleChangeAlertLevel( TGEvent *event ) {
	SetAlertLevel( ( (TGIntEvent *)event )->GetInt() );
	CallNextHandler( event );
}

/*
================
ShipClass::CompleteStop

Stops the ship dead: no speed, no turning, every weapon system stops
firing, and a cloak that is up or going up is dropped.
================
*/
// FUNCTION: STBC 0x005b0780
void ShipClass::CompleteStop( void ) {
	SetSpeed( 0.0f, NiPoint3::ZERO, 1 );
	SetTargetAngularVelocityDirect( NiPoint3::ZERO );

	MatchIterator *iterator = StartGetSubsystemMatch( WEAPONSYSTEM_TYPE );
	ShipSubsystem *subsystem;
	while ( ( subsystem = GetNextSubsystemMatch( iterator ) ) != NULL ) {
		WeaponSystem *system = WeaponSystem::Cast( subsystem );
		if ( system != NULL ) {
			system->StopFiring();
		}
	}
	EndGetSubsystemMatch( iterator );

	CloakingSubsystem *cloak = GetCloakingSubsystem();
	if ( cloak != NULL && ( cloak->IsCloaked() || cloak->IsCloaking() ) ) {
		cloak->StopCloaking();
	}
}

/*
================
ShipClass::SetNoAIState

The ship's AI is done: unless the stop is to be ignored, the ship comes to
a stop. Its weapon systems stop firing, except the player's tractor
beams, and a ship other than the player's drops a cloak that is up or
going up. Then the next handler runs.
================
*/
// FUNCTION: STBC 0x005b0810
void ShipClass::SetNoAIState( TGEvent *event ) {
	bool isPlayer = ( this == Game::GetCurrentPlayer() );
	if ( aiDoneIgnore > 0 ) {
		aiDoneIgnore--;
	} else {
		SetSpeed( 0.0f, NiPoint3::ZERO, 1 );
		SetTargetAngularVelocityDirect( NiPoint3::ZERO );
	}

	MatchIterator *iterator = StartGetSubsystemMatch( WEAPONSYSTEM_TYPE );
	ShipSubsystem *subsystem;
	while ( ( subsystem = GetNextSubsystemMatch( iterator ) ) != NULL ) {
		WeaponSystem *system = WeaponSystem::Cast( subsystem );
		if ( system != NULL ) {
			if ( !isPlayer || !subsystem->IsTypeOf( TRACTORBEAMSYSTEM_TYPE ) ) {
				system->StopFiring();
			}
		}
	}
	EndGetSubsystemMatch( iterator );

	if ( !isPlayer ) {
		CloakingSubsystem *cloak = GetCloakingSubsystem();
		if ( cloak != NULL && ( cloak->IsCloaked() || cloak->IsCloaking() ) ) {
			cloak->StopCloaking();
		}
	}
	CallNextHandler( event );
}

/*
================
ShipClass::Unknown5B0900

Steps the subsystems by the elapsed time. Warp engines that are warping
and a cloak that is cloaking or decloaking update every frame for the
time they have gathered; the other subsystems update once
ShipSubsystem::UPDATE_INTERVAL has gathered, or at once when a weapon
system asks for it.
================
*/
// FUNCTION: STBC 0x005b0900
// PARKED: x87 operand order: retail loads each subsystem's unknown48 before adding the elapsed time (fld member; fadd arg) where this build loads the argument first, here and in the inlined TimeSlicedUpdate (as in WeaponSystem::Update)
void ShipClass::Unknown5B0900( float elapsed ) {
	bool warpUpdated = false;
	bool cloakUpdated = false;
	WarpEngineSubsystem *warp = GetWarpEngineSubsystem();
	CloakingSubsystem *cloak = GetCloakingSubsystem();

	if ( warp != NULL && warp->GetWarpState() != 0 ) {
		warp->unknown48 += elapsed;
		warp->Update( warp->unknown48 );
		warp->unknown48 = 0.0f;
		warpUpdated = true;
	}
	if ( cloak != NULL && ( cloak->IsCloaking() || cloak->IsDecloaking() ) ) {
		cloak->unknown48 += elapsed;
		cloak->Update( cloak->unknown48 );
		cloak->unknown48 = 0.0f;
		cloakUpdated = true;
	}

	NiTListIterator pos = subsystems.unknown1C.GetHeadPos();
	while ( pos != NULL ) {
		ShipSubsystem *system = subsystems.unknown1C.GetNext( pos );
		system->unknown48 += elapsed;
		WeaponSystem *weapons = WeaponSystem::Cast( system );
		if ( system->unknown48 > ShipSubsystem::UPDATE_INTERVAL || ( weapons != NULL && weapons->IsForceUpdate() == true ) ) {
			float remainder = (float)fmod( system->unknown48, ShipSubsystem::UPDATE_INTERVAL );
			system->Update( system->unknown48 - remainder );
			system->unknown48 = remainder;
		}
	}

	if ( warp != NULL && !warpUpdated ) {
		warp->TimeSlicedUpdate( elapsed );
	}
	if ( cloak != NULL && !cloakUpdated ) {
		cloak->TimeSlicedUpdate( elapsed );
	}
}

/*
================
ShipClass::GetWeaponSystemGroup

The weapon system that fires a group: 1 the phasers (the pulse weapons on
a ship without phasers), 2 the torpedoes, 3 the pulse weapons, 4 the
tractor beams.
================
*/
// FUNCTION: STBC 0x005b0ad0
WeaponSystem *ShipClass::GetWeaponSystemGroup( int group ) {
	WeaponSystem *system = NULL;
	switch ( group ) {
		case 1:
			system = subsystems.phaserSystem;
			if ( system == NULL ) {
				system = subsystems.pulseWeaponSystem;
			}
			break;
		case 2:
			system = subsystems.torpedoSystem;
			break;
		case 3:
			system = subsystems.pulseWeaponSystem;
			break;
		case 4:
			system = subsystems.tractorBeamSystem;
			break;
	}
	return system;
}

/*
================
ShipClass::GetWeaponGroupForSystem

The group GetWeaponSystemGroup returns the system for; 0 for none.
================
*/
// FUNCTION: STBC 0x005b0b20
int ShipClass::GetWeaponGroupForSystem( WeaponSystem *system ) {
	if ( system == NULL ) {
		return 0;
	}
	if ( system->IsTypeOf( TORPEDOSYSTEM_TYPE ) ) {
		return 2;
	}
	if ( system->IsTypeOf( PHASERSYSTEM_TYPE ) ) {
		return 1;
	}
	if ( system->IsTypeOf( PULSEWEAPONSYSTEM_TYPE ) ) {
		return ( subsystems.phaserSystem != NULL ) ? 3 : 1;
	}
	return system->IsTypeOf( TRACTORBEAMSYSTEM_TYPE ) ? 4 : 0;
}

/*
================
ShipClass::StopFiringWeapons

Every weapon system of the ship stops firing.
================
*/
// FUNCTION: STBC 0x005b0bb0
void ShipClass::StopFiringWeapons( void ) {
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		WeaponSystem *system = WeaponSystem::Cast( subsystems.list.GetNext( pos ) );
		if ( system != NULL ) {
			system->StopFiring();
		}
	}
}

/*
================
ShipClass::HandleTractorHitStart

A tractor beam started hitting the ship: its projector joins the list of
the beams holding the ship.
================
*/
// FUNCTION: STBC 0x005b0bf0
void ShipClass::HandleTractorHitStart( TGEvent *event ) {
	tractorBeams.AddTail( TractorBeamProjector::Cast( event->GetSource() ) );
}

/*
================
ShipClass::HandleTractorHitStop

A tractor beam stopped hitting the ship: its projector leaves the list.
================
*/
// FUNCTION: STBC 0x005b0c70
void ShipClass::HandleTractorHitStop( TGEvent *event ) {
	tractorBeams.Remove( TractorBeamProjector::Cast( event->GetSource() ) );
}

/*
================
ShipClass::VirtualSlot69

PhysicsObjectClass's state, then each subsystem's (its slot 26). Returns
the stream position.
================
*/
// FUNCTION: STBC 0x005b0d80
int ShipClass::VirtualSlot69( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot69( stream );
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		subsystems.list.GetNext( pos )->VirtualSlot26( stream );
	}
	return stream->GetPos();
}

/*
================
ShipClass::VirtualSlot71

PhysicsObjectClass's state, then each subsystem's (its slot 27). Returns
the stream position.
================
*/
// FUNCTION: STBC 0x005b0dc0
int ShipClass::VirtualSlot71( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot71( stream );
	NiTListIterator pos = subsystems.list.GetHeadPos();
	while ( pos ) {
		subsystems.list.GetNext( pos )->VirtualSlot27( stream );
	}
	return stream->GetPos();
}

/*
================
ShipClass::SetTargetHandler

The event's object is the new target; its source, when there is one, is
the targeted subsystem, else a ship target's hull. Then the next handler
runs.
================
*/
// FUNCTION: STBC 0x005b0e00
void ShipClass::SetTargetHandler( TGEvent *event ) {
	TGObject *target = ( (TGObjPtrEvent *)event )->GetObjPtr();
	TGObject *subsystem = event->GetSource();
	targetID = 0;
	targetSubsystemID = 0;
	if ( target != NULL ) {
		targetID = target->GetObjID();
		if ( subsystem == NULL ) {
			ShipClass *ship = ShipClass::Cast( target );
			if ( ship != NULL ) {
				targetSubsystemID = ship->GetHull()->GetObjID();
			}
		} else {
			targetSubsystemID = subsystem->GetObjID();
		}
	}
	CallNextHandler( event );
}

/*
================
ShipClass::VirtualSlot70

Reads the net type, then has the script Multiplayer.SpeciesToShip.InitObject
set the ship up for its species. Returns the stream position, 0 when the
script fails.
================
*/
// FUNCTION: STBC 0x005b0e80
int ShipClass::VirtualSlot70( TGStream *stream ) {
	PhysicsObjectClass::VirtualSlot70( stream );
	PyObject *self = VirtualSlot08();
	int result;
	int status = PyEmbed::CallFunction( "Multiplayer.SpeciesToShip", "InitObject", "i", &result, "(Oi)", self, netType );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
		return 0;
	}
	return stream->GetPos();
}

/*
================
ShipClass::WriteToStream

DamageableObject's state, then the ship's: its motion and warp values,
target, affiliation and alert level, the IDs of the tractor projectors
whose beams hit it, the subsystems, the ship property (after whether
there is one), the flags and the scripts.
================
*/
// FUNCTION: STBC 0x005b0f00
bool ShipClass::WriteToStream( TGStream *stream ) {
	char text[256];
	char *className = GetClassName();
	sprintf( text, "Saving %s %s\n", className, name );
	Log( text );

	if ( !DamageableObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( unknown1E4 );
	stream->WriteBool( usingTargetAngularVelocity );
	stream->WriteBool( docked );
	stream->WriteFloat( targetAngularVelocity.x );
	stream->WriteFloat( targetAngularVelocity.y );
	stream->WriteFloat( targetAngularVelocity.z );
	stream->WriteInt( targetAngularDirectionType );
	stream->WriteFloat( impulse );
	stream->WriteFloat( disabledEngineDeceleration );
	stream->WriteFloat( impulseDirection.x );
	stream->WriteFloat( impulseDirection.y );
	stream->WriteFloat( impulseDirection.z );
	stream->WriteLong( impulseDirectionType );
	stream->WriteBool( doingInSystemWarp );
	stream->WriteID( inSystemWarpTargetID );
	stream->WriteFloat( inSystemWarpDistance );
	stream->WriteID( targetID );
	stream->WriteID( targetSubsystemID );
	stream->WriteBool( usingTargetOffset );
	stream->WriteFloat( targetOffset.x );
	stream->WriteFloat( targetOffset.y );
	stream->WriteFloat( targetOffset.z );
	stream->WriteLong( affiliation );
	stream->WriteLong( alertLevel );
	stream->WriteLong( unknown23C );
	stream->WriteLong( hitShield );
	stream->WriteFloat( boxExtent.x );
	stream->WriteFloat( boxExtent.y );
	stream->WriteFloat( boxExtent.z );
	stream->WriteFloat( boxCenter.x );
	stream->WriteFloat( boxCenter.y );
	stream->WriteFloat( boxCenter.z );

	stream->WriteLong( tractorBeams.GetSize() );
	NiTListIterator pos = tractorBeams.GetHeadPos();
	while ( pos ) {
		TractorBeamProjector *projector = tractorBeams.GetNext( pos );
		stream->WriteID( ( projector == NULL ) ? 0 : projector->GetObjID() );
	}

	subsystems.WriteToStream( stream );

	if ( shipProperty != NULL ) {
		stream->WriteBool( true );
		shipProperty->WriteToStream( stream );
	} else {
		char message[256];
		sprintf( message, "Ship %s had no ship property\n", name );
		Log( message );
		stream->WriteBool( false );
	}

	stream->WriteBool( targetable );
	stream->WriteBool( invincible );
	stream->WriteBool( hurtable );
	stream->WriteBool( destroyBrokenSystems );
	stream->VirtualSlot11( script );
	stream->VirtualSlot11( deathScript );
	stream->WriteInt( aiDoneIgnore );
	return true;
}

/*
================
ShipClass::ReadFromStream

Reads what WriteToStream wrote. The tractor list holds the projectors'
IDs until the fix-up pass turns them into pointers.
================
*/
// FUNCTION: STBC 0x005b1220
bool ShipClass::ReadFromStream( TGStream *stream ) {
	if ( !DamageableObject::ReadFromStream( stream ) ) {
		return false;
	}

	unknown1E4 = stream->ReadBool();
	usingTargetAngularVelocity = stream->ReadBool();
	docked = stream->ReadBool();
	targetAngularVelocity.x = stream->ReadFloat();
	targetAngularVelocity.y = stream->ReadFloat();
	targetAngularVelocity.z = stream->ReadFloat();
	targetAngularDirectionType = stream->ReadInt();
	impulse = stream->ReadFloat();
	disabledEngineDeceleration = stream->ReadFloat();
	impulseDirection.x = stream->ReadFloat();
	impulseDirection.y = stream->ReadFloat();
	impulseDirection.z = stream->ReadFloat();
	impulseDirectionType = stream->ReadLong();
	doingInSystemWarp = stream->ReadBool();
	inSystemWarpTargetID = stream->ReadID();
	inSystemWarpDistance = stream->ReadFloat();
	targetID = stream->ReadID();
	targetSubsystemID = stream->ReadID();
	usingTargetOffset = stream->ReadBool();
	targetOffset.x = stream->ReadFloat();
	targetOffset.y = stream->ReadFloat();
	targetOffset.z = stream->ReadFloat();
	affiliation = stream->ReadLong();
	alertLevel = stream->ReadLong();
	unknown23C = stream->ReadLong();
	hitShield = stream->ReadLong();
	boxExtent.x = stream->ReadFloat();
	boxExtent.y = stream->ReadFloat();
	boxExtent.z = stream->ReadFloat();
	boxCenter.x = stream->ReadFloat();
	boxCenter.y = stream->ReadFloat();
	boxCenter.z = stream->ReadFloat();

	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		long projectorID = stream->ReadLong();
		if ( projectorID != 0 ) {
			tractorBeams.AddTail( (TractorBeamProjector *)projectorID );
		}
	}

	subsystems.ReadFromStream( stream );

	if ( stream->ReadBool() == true ) {
		shipProperty = (ShipProperty *)TGObject::LoadObject( stream, 0 );
	} else {
		shipProperty = NULL;
	}

	targetable = stream->ReadBool();
	invincible = stream->ReadBool();
	hurtable = stream->ReadBool();
	destroyBrokenSystems = stream->ReadBool();
	script = stream->VirtualSlot10( -1 );
	deathScript = stream->VirtualSlot10( -1 );
	aiDoneIgnore = stream->ReadInt();
	return true;
}

/*
================
ShipClass::FixUpPointers

Fails until the subsystems and every object on tractorBeams have been
loaded.
================
*/
// FUNCTION: STBC 0x005b1500
bool ShipClass::FixUpPointers( void ) {
	if ( !DamageableObject::FixUpPointers() ) {
		return false;
	}
	if ( !subsystems.VirtualSlot02() ) {
		return false;
	}

	NiTListIterator pos = tractorBeams.GetHeadPos();
	while ( pos ) {
		TractorBeamProjector *object = tractorBeams.GetNext( pos );
		if ( object != NULL && GetTGObjectPtr( (long)object ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
ShipClass::PostFixUp

Turns the IDs ReadFromStream put on tractorBeams back into projectors,
keeping their order.
================
*/
// FUNCTION: STBC 0x005b1550
void ShipClass::PostFixUp( void ) {
	DamageableObject::PostFixUp();
	subsystems.VirtualSlot03();

	NiTList<TractorBeamProjector *> projectors( 2 );
	NiTListIterator pos = tractorBeams.GetHeadPos();
	while ( pos ) {
		TractorBeamProjector *object = tractorBeams.GetNext( pos );
		if ( object != NULL ) {
			projectors.AddTail( (TractorBeamProjector *)GetTGObjectPtr( (long)object ) );
		}
	}
	tractorBeams.RemoveAll();
	pos = projectors.GetHeadPos();
	while ( pos ) {
		tractorBeams.AddTail( projectors.GetNext( pos ) );
	}
}

/*
================
ShipClass::VirtualSlot66

Updates the node, then gives the animation node under each child of the
node its plain properties again, and lets the cloak know.
================
*/
// FUNCTION: STBC 0x005b1750
void ShipClass::VirtualSlot66( void ) {
	UpdateNodeOnly();
	NiNode *shipNode = GetNode();
	if ( shipNode != NULL ) {
		for ( unsigned int i = 0; i < shipNode->GetArrayCount(); i++ ) {
			NiNode *child = (NiNode *)shipNode->GetAt( i );
			if ( child != NULL ) {
				TGAnimNode *animNode = NiDynamicCast( TGAnimNode, child->GetAt( 0 ) );
				if ( animNode != NULL ) {
					animNode->Unknown6C5520();
				}
			}
		}
	}

	CloakingSubsystem *cloak = GetCloakingSubsystem();
	if ( cloak != NULL ) {
		cloak->VirtualSlot34();
	}
}

// The power percentages go out scaled to 0-255 from a range of 0 to 1.25.
static const float POWER_PERCENTAGE_SCALE = 0.8f;
static const float POWER_PERCENTAGE_UNSCALE = 1.25f / 255.0f;

/*
================
ShipClass::VirtualSlot72

PhysicsObjectClass's state update, with the cloak's state and, for the
host's players, the subsystems' states (about ten bytes a message, taking
turns); a client sends its subsystems' power settings instead (about six
bytes). The player's own ship is not described to the host's player.
================
*/
// FUNCTION: STBC 0x005b17f0
// DRAFT: frame slots and byte-flag spills differ throughout; retail copies the game time through the FPU, loads the record's position onto the FPU before the offset's subtractions and stores each changed axis before testing the next (63.4)
TGMessage *ShipClass::VirtualSlot72( MultiplayerPlayer *player ) {
	if ( !VirtualSlot79( player ) ) {
		return NULL;
	}
	UnknownClass6A8C50 *record;
	if ( !unknown68.GetAt( player->playerID, record ) ) {
		return NULL;
	}

	float now = g_kUtopiaModule.GetGameTime();
	TGBufferStream stream;
	bool playersShip = false;
	unsigned char flags = 0;
	bool hasCloak = false;
	bool everything = false;
	bool positionDue = false;
	if ( now - record->unknown04 > 1.0f ) {
		everything = true;
	}
	if ( now - record->unknown24 > 1.0f ) {
		positionDue = true;
		record->unknown24 = now;
	}
	bool notHost = false;
	if ( !g_kUtopiaModule.IsHost() ) {
		notHost = true;
	}
	if ( id == player->shipID && g_kUtopiaModule.IsHost() ) {
		playersShip = true;
	}

	NiPoint3 position;
	NiPoint3 forward;
	NiPoint3 up;
	float speed;
	if ( unknown2FC ) {
		position = unknown300;
		forward = unknown30C;
		up = unknown318;
		speed = unknown324;
		unknown2FC = false;
	} else {
		position = GetWorldLocation();
		forward = GetWorldForward();
		up = GetWorldUp();
		speed = GetVelocity().Length();
		if ( IsReverse() ) {
			speed = -speed;
		}
	}

	NiPoint3 offset( position.x - record->unknown10, position.y - record->unknown14, position.z - record->unknown18 );
	if ( !playersShip && doNetUpdate ) {
		char offsetX, offsetY, offsetZ;
		unsigned short offsetLength;
		stream.VirtualSlot41( offset.x, offset.y, offset.z, &offsetX, &offsetY, &offsetZ, &offsetLength );
		char forwardX, forwardY, forwardZ;
		stream.VirtualSlot42( forward.x, forward.y, forward.z, &forwardX, &forwardY, &forwardZ );
		char upX, upY, upZ;
		stream.VirtualSlot42( up.x, up.y, up.z, &upX, &upY, &upZ );

		if ( positionDue ) {
			flags = 1;
			record->unknown10 = position.x;
			record->unknown14 = position.y;
			record->unknown18 = position.z;
			record->unknown20 = 0;
			record->unknown21 = 0;
			record->unknown22 = 0;
			record->unknown1C = 0.0f;
		} else if ( offsetX != record->unknown20 || offsetY != record->unknown21 || offsetZ != record->unknown22 ||
					offsetLength != record->unknown1C || everything ) {
			record->unknown20 = offsetX;
			record->unknown21 = offsetY;
			record->unknown22 = offsetZ;
			flags = 2;
			record->unknown1C = offsetLength;
		}
		if ( forwardX != record->unknown28 || forwardY != record->unknown29 || forwardZ != record->unknown2A || everything ) {
			record->unknown28 = forwardX;
			flags |= 4;
			record->unknown29 = forwardY;
			record->unknown2A = forwardZ;
		}
		if ( upX != record->unknown2B || upY != record->unknown2C || upZ != record->unknown2D || everything ) {
			record->unknown2B = upX;
			flags |= 8;
			record->unknown2C = upY;
			record->unknown2D = upZ;
		}
		if ( speed != record->unknown0C || everything ) {
			flags |= 0x10;
			record->unknown0C = speed;
		}
		CloakingSubsystem *cloak = GetCloakingSubsystem();
		if ( cloak != NULL ) {
			unsigned char on = cloak->IsOn();
			hasCloak = true;
			if ( record->unknown2E != on || everything ) {
				record->unknown2E = on;
				flags |= 0x40;
			}
		}
	}

	if ( notHost ) {
		flags |= 0x80;
	} else if ( !g_kUtopiaModule.IsModemConnection() || everything ||
				( !g_kUtopiaModule.IsClient() ? ( (MultiplayerGame *)Game::GetCurrentGame() )->GetNumberPlayersInGame() <= 1
											 : ( (MultiplayerGame *)Game::GetCurrentGame() )->GetNumberPlayersInGame() <= 2 ) ) {
		flags |= 0x20;
	}
	if ( flags == 0 ) {
		return NULL;
	}
	if ( everything || ( hasCloak && ( flags & 0x40 ) && ( flags & 4 ) && ( flags & 8 ) && ( flags & 0x10 ) && ( flags & 3 ) ) ) {
		record->unknown04 = now;
	}

	char buffer[1024];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( SHIP_UPDATE_MESSAGE );
	stream.WriteLong( id );
	stream.WriteFloat( now );
	stream.WriteChar( flags );
	if ( flags & 1 ) {
		stream.WriteFloat( position.x );
		stream.WriteFloat( position.y );
		stream.WriteFloat( position.z );
		if ( notHost ) {
			stream.WriteBool( true );
			int checksum = subsystems.Unknown5B5EB0();
			stream.WriteShort( (unsigned short)( checksum ^ ( checksum >> 16 ) ) );
		} else {
			stream.WriteBool( false );
		}
	}
	if ( flags & 2 ) {
		stream.VirtualSlot36( offset.x, offset.y, offset.z, true );
	}
	if ( flags & 4 ) {
		stream.VirtualSlot34( forward.x, forward.y, forward.z );
	}
	if ( flags & 8 ) {
		stream.VirtualSlot34( up.x, up.y, up.z );
	}
	if ( flags & 0x10 ) {
		stream.WriteShort( Unknown6D3A90( speed ) );
	}
	if ( ( flags & 0x40 ) && GetCloakingSubsystem() != NULL ) {
		stream.WriteBool( GetCloakingSubsystem()->IsOn() );
	}

	if ( flags & 0x20 ) {
		int start = stream.GetPos();
		if ( record->unknown30 == NULL ) {
			record->unknown30 = subsystems.list.GetHeadPos();
			record->unknown34 = 0;
		}
		NiTListIterator first = record->unknown30;
		stream.WriteChar( record->unknown34 );
		while ( stream.GetPos() - start < 10 ) {
			ShipSubsystem *subsystem = subsystems.list.GetNext( record->unknown30 );
			subsystem->VirtualSlot28( &stream, playersShip );
			record->unknown34++;
			if ( record->unknown30 == NULL ) {
				record->unknown30 = subsystems.list.GetHeadPos();
				record->unknown34 = 0;
			}
			if ( record->unknown30 == first ) {
				break;
			}
		}
	}

	if ( flags & 0x80 ) {
		int start = stream.GetPos();
		if ( record->unknown38 == NULL ) {
			record->unknown38 = subsystems.list.GetHeadPos();
			record->unknown3C = 0;
		}
		NiTListIterator first = record->unknown38;
		while ( stream.GetPos() - start < 6 ) {
			ShipSubsystem *subsystem = subsystems.list.GetNext( record->unknown38 );
			if ( subsystem->IsTypeOf( POWEREDSUBSYSTEM_TYPE ) ) {
				bool added = false;
				UnknownClass6A8C50Item *sent;
				if ( !record->unknown40.GetAt( subsystem->GetObjID(), sent ) ) {
					sent = new UnknownClass6A8C50Item;
					record->unknown40.SetAt( subsystem->GetObjID(), sent );
					added = true;
				}
				unsigned char percentage = (unsigned char)( ( (PoweredSubsystem *)subsystem )->GetPowerPercentageWanted() * POWER_PERCENTAGE_SCALE * 255.0f );
				if ( percentage != sent->unknown00 || added ) {
					sent->unknown00 = percentage;
					stream.WriteChar( record->unknown3C );
					stream.WriteChar( percentage );
				}
			}
			record->unknown3C++;
			if ( record->unknown38 == NULL ) {
				record->unknown38 = subsystems.list.GetHeadPos();
				record->unknown3C = 0;
			}
			if ( record->unknown38 == first ) {
				break;
			}
		}
	}

	TGMessage *message = new TGMessage;
	message->SetData( buffer, stream.GetPos() );
	message->SetGuaranteed( false );
	return message;
}

/*
================
ShipClass::VirtualSlot73

Reads a state update VirtualSlot72 made into the ship's network AI. A
whole position from a client carries its subsystems' checksum: on the host
a wrong one boots the ship's player. The subsystem states and power
settings that follow are handed to the subsystems they name.
================
*/
// FUNCTION: STBC 0x005b21c0
// PARKED: register and schedule: the checksum's low half sign-extends into edx (retail eax); the first-update block's scratch registers rotate eax/ecx/edx against retail's ecx/edx/eax; the PredictPosition time argument's fstp [esp] scheduled one place early (PhysicsObjectClass::VirtualSlot73's wall)
void ShipClass::VirtualSlot73( TGMessage *message ) {
	NetworkAI *ai = NetworkAI::Cast( GetAI() );
	int length;
	char *data = message->GetData( &length );
	TGBufferStream stream;
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	stream.ReadLong();
	float time = stream.ReadFloat();
	unsigned char flags = stream.ReadChar();

	float x, y, z;
	if ( flags & 1 ) {
		x = stream.ReadFloat();
		y = stream.ReadFloat();
		z = stream.ReadFloat();
		if ( stream.ReadBool() ) {
			int checksum = stream.ReadShort();
			if ( g_kUtopiaModule.IsHost() ) {
				int ours = subsystems.Unknown5B5EB0();
				if ( ( (short)( ours >> 16 ) ^ (short)ours ) != checksum ) {
					MultiplayerWindow *window = MultiplayerWindow::Cast( TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER ) );
					TGPlayerEvent *event = new TGPlayerEvent;
					event->SetEventType( ET_PLAYER_BOOT_EVENT );
					event->SetDestination( window );
					event->SetPlayerID( netPlayerID );
					g_kEventManager.AddEvent( event );
				}
			}
		}
		if ( time > unknown88 ) {
			unknown90.x = x;
			unknown90.y = y;
			unknown90.z = z;
			if ( ai != NULL ) {
				ai->position = unknown90;
				ai->stateTime = time;
			}
			unknown88 = time;
			unknown78 = time;
			unknown8C = 0.0f;
			unknown9C.x = x;
			unknown9C.y = y;
			unknown9C.z = z;
		}
	}
	if ( time - unknown88 > 1.0f && time - unknown8C > 1.0f ) {
		NiPoint3 velocity = GetVelocity();
		NiPoint3 accel = acceleration;
		unknown90 = PredictPosition( unknown9C, velocity, accel, time - unknown78 );
		unknown9C = unknown90;
		unknown8C = time;
	}
	if ( flags & 2 ) {
		stream.VirtualSlot37( &x, &y, &z, true );
		if ( time > unknown78 ) {
			unknown9C.x = x + unknown9C.x;
			unknown9C.y = y + unknown9C.y;
			unknown9C.z = z + unknown9C.z;
			NiPoint3 estimate = unknown90;
			estimate.x += x;
			estimate.y += y;
			estimate.z += z;
			if ( ai != NULL ) {
				ai->position = estimate;
				ai->stateTime = time;
			}
			unknown78 = time;
		}
	}
	if ( flags & 4 ) {
		stream.VirtualSlot35( &x, &y, &z );
		if ( time > unknown7C ) {
			if ( ai != NULL ) {
				ai->direction = NiPoint3( x, y, z );
			}
			unknown7C = time;
		}
	}
	if ( flags & 8 ) {
		stream.VirtualSlot35( &x, &y, &z );
		if ( time > unknown80 ) {
			if ( ai != NULL ) {
				ai->up = NiPoint3( x, y, z );
			}
			unknown80 = time;
		}
	}
	if ( flags & 0x10 ) {
		float speed = Unknown6D3B30( stream.ReadShort() );
		if ( time > unknown84 ) {
			if ( ai != NULL ) {
				ai->speed = speed;
			}
			unknown84 = time;
		}
	}
	if ( flags & 0x40 ) {
		bool cloak = stream.ReadBool();
		CloakingSubsystem *cloaking = GetCloakingSubsystem();
		if ( cloaking != NULL ) {
			if ( cloak ) {
				cloaking->StartCloaking();
			} else {
				cloaking->StopCloaking();
			}
		}
	}
	if ( flags & 0x20 ) {
		char index = stream.ReadChar();
		NiTListIterator pos = subsystems.list.GetHeadPos();
		for ( int i = index; i != 0; i-- ) {
			subsystems.list.GetNext( pos );
		}
		while ( stream.GetPos() < length && pos != NULL ) {
			ShipSubsystem *subsystem = subsystems.list.GetNext( pos );
			if ( subsystem == NULL ) {
				break;
			}
			subsystem->VirtualSlot29( &stream, time );
			if ( pos == NULL ) {
				pos = subsystems.list.GetHeadPos();
			}
		}
	}
	if ( flags & 0x80 ) {
		while ( stream.GetPos() < length ) {
			int index = stream.ReadChar();
			unsigned char percentage = stream.ReadChar();
			NiTListIterator pos = subsystems.list.GetHeadPos();
			ShipSubsystem *subsystem;
			for ( int i = 0; i <= index; i++ ) {
				subsystem = subsystems.list.GetNext( pos );
			}
			if ( subsystem == NULL ) {
				break;
			}
			if ( subsystem->IsTypeOf( POWEREDSUBSYSTEM_TYPE ) ) {
				float power = percentage * POWER_PERCENTAGE_UNSCALE;
				( (PoweredSubsystem *)subsystem )->VirtualSlot33( power, time );
			}
		}
	}
}

/*
================
ShipClass::StartWarpHandler

Plays a warp sequence to the event's destination.
================
*/
// FUNCTION: STBC 0x005b2810
void ShipClass::StartWarpHandler( TGEvent *event ) {
	WarpEvent *warp = (WarpEvent *)event;
	WarpSequence *sequence = new WarpSequence( this, warp->GetDestinationSetName(), warp->GetDuration(), warp->GetPlacementName() );
	if ( sequence != NULL ) {
		sequence->Play();
	}
}



/*
================
ShipClass::EnvironmentDamageHandler

A nebula or a planet's atmosphere damages the ship for the frame: the
shields first, while they are up, and the hull once any facing is down.
Nothing happens to the player's ship in god mode or to a ship that is not
hurtable.
================
*/
// FUNCTION: STBC 0x005b2870
void ShipClass::EnvironmentDamageHandler( TGEvent *event ) {
	if ( this == Game::GetCurrentGame()->GetPlayer() && Game::GetCurrentGame()->InGodMode() ) {
		CallNextHandler( event );
		return;
	}
	if ( !hurtable ) {
		CallNextHandler( event );
		return;
	}
	if ( GetContainingSet() != NULL && GetNiObject() != NULL ) {
		float shieldDamage = 0.0f;
		float hullDamage = 0.0f;
		float depth;
		NiPoint3 direction;
		Nebula *nebula = Nebula::Cast( event->GetSource() );
		if ( nebula != NULL ) {
			shieldDamage = nebula->GetHullDamage() * UtopiaApp::GetApp()->GetGameFrameStep();
			hullDamage = nebula->GetDamage() * UtopiaApp::GetApp()->GetGameFrameStep();
		} else {
			Planet *planet = Planet::Cast( event->GetSource() );
			if ( planet != NULL ) {
				shieldDamage = planet->VirtualSlot68( this, &depth, &direction ) * UtopiaApp::GetApp()->GetGameFrameStep();
				hullDamage = planet->VirtualSlot67( this, &depth, &direction ) * UtopiaApp::GetApp()->GetGameFrameStep();
				if ( g_kUtopiaModule.IsClient() || !g_kUtopiaModule.IsMultiplayer() ) {
					Unknown5B2BF0( depth, &direction );
				}
			}
		}

		ShieldClass *shields = GetShields();
		if ( shieldDamage > 0.0f && shields->IsOn() && !shields->IsDisabled() ) {
			shields->SetCurShields( ShieldClass::FRONT_SHIELDS, shields->GetCurShields( ShieldClass::FRONT_SHIELDS ) - shieldDamage );
			shields->SetCurShields( ShieldClass::REAR_SHIELDS, shields->GetCurShields( ShieldClass::REAR_SHIELDS ) - shieldDamage );
			shields->SetCurShields( ShieldClass::TOP_SHIELDS, shields->GetCurShields( ShieldClass::TOP_SHIELDS ) - shieldDamage );
			shields->SetCurShields( ShieldClass::BOTTOM_SHIELDS, shields->GetCurShields( ShieldClass::BOTTOM_SHIELDS ) - shieldDamage );
			shields->SetCurShields( ShieldClass::LEFT_SHIELDS, shields->GetCurShields( ShieldClass::LEFT_SHIELDS ) - shieldDamage );
			shields->SetCurShields( ShieldClass::RIGHT_SHIELDS, shields->GetCurShields( ShieldClass::RIGHT_SHIELDS ) - shieldDamage );
		}
		if ( hullDamage > 0.0f ) {
			HullClass *hull = GetHull();
			if ( ( !shields->IsOn() || shields->IsDisabled()
				|| shields->GetCurShields( ShieldClass::FRONT_SHIELDS ) == 0.0f || shields->GetCurShields( ShieldClass::REAR_SHIELDS ) == 0.0f
				|| shields->GetCurShields( ShieldClass::TOP_SHIELDS ) == 0.0f || shields->GetCurShields( ShieldClass::BOTTOM_SHIELDS ) == 0.0f
				|| shields->GetCurShields( ShieldClass::LEFT_SHIELDS ) == 0.0f || shields->GetCurShields( ShieldClass::RIGHT_SHIELDS ) == 0.0f )
				&& ( g_kUtopiaModule.IsHost() || !g_kUtopiaModule.IsMultiplayer() ) ) {
				DamageSystem( hull, hullDamage, NULL, false );
			}
		}
		CallNextHandler( event );
	} else {
		CallNextHandler( event );
	}
}

/*
================
ShipClass::Unknown5B2BF0

The ship's flight through a planet's atmosphere, as deep as the depth: the
rumble plays louder the deeper the ship is (full from half depth), and the
atmosphere effect sits on the ship's box, turned to face the way it moves
into the atmosphere and sized to the box's reach along those axes. At no
depth both go.
================
*/
// FUNCTION: STBC 0x005b2bf0
// DRAFT: call set now retail's (Perpendicular calls UnitCross out of line); frame 0x9C against 0xA4: retail keeps the first corner product's result apart from reach and reuses the negated half components across the corner temporaries, so the slots differ
void ShipClass::Unknown5B2BF0( float depth, NiPoint3 *direction ) {
	TGSound *sound = g_kSoundManager.GetPlayingSound( unknown23C );
	float volume = depth * 2.0f;
	if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	if ( sound != NULL ) {
		sound->SetVolume( volume );
	} else if ( GetContainingSet() != NULL && GetContainingSet()->GetName() != NULL ) {
		TGSoundRegion *region = TGSoundRegion::GetRegion( GetContainingSet()->GetName() );
		if ( region != NULL ) {
			sound = g_kSoundManager.GetSound( "AtmosphereRumble" );
			if ( sound != NULL ) {
				region->AddSound( sound );
				sound->AttachToNode( node );
				sound->SetLooping( true );
				sound->SetVolume( volume );
				unknown23C = sound->Play( true );
			}
		}
	}

	if ( depth > 0.0f ) {
		// the effect's frame: its x axis into the atmosphere, in the model
		// frame, and two axes across it
		NiPoint3 forward = -*direction * node->GetWorldRotate();
		NiPoint3 side = forward.Perpendicular();
		NiMatrix3 rotation;
		rotation.SetCol( 0, forward );
		rotation.SetCol( 1, side );
		rotation.SetCol( 2, side.Cross( forward ) );

		// the reach of the box's corners along the effect's axes
		float root3 = (float)sqrt( 3.0 );
		NiPoint3 half;
		half.x = boxExtent.x / root3;
		half.y = boxExtent.y / root3;
		half.z = boxExtent.z / root3;
		NiPoint3 reach = NiPoint3( half.x, half.y, half.z ) * rotation;
		NiPoint3 corner;
		corner = NiPoint3( half.x, half.y, -half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( half.x, -half.y, -half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( half.x, -half.y, half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( -half.x, half.y, half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( -half.x, -half.y, -half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( -half.x, -half.y, half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		corner = NiPoint3( -half.x, half.y, -half.z ) * rotation;
		if ( corner.x > reach.x ) reach.x = corner.x;
		if ( corner.y > reach.y ) reach.y = corner.y;
		if ( corner.z > reach.z ) reach.z = corner.z;
		NiPoint3 extent = reach * root3;

		if ( atmosphereEffect == NULL ) {
			unsigned int count = 200;
			switch ( EffectController::GetEffectLevel() ) {
			case 0:
			case 1:
				count = 200;
				break;
			case 2:
				count = 300;
				break;
			case 3:
			case 4:
				count = 400;
				break;
			}
			atmosphereEffect = new AtmosphereEffect( UtopiaApp::GetApp()->GetRealTime(), depth, extent,
				g_kImageManager.GetImage( "data/Textures/AtmosphereEffectShockWave.tga", true, true ),
				g_kImageManager.GetImage( "data/Textures/AtmosphereEffect.tga", true, true ), count );
			node->AttachChild( atmosphereEffect, true );
			atmosphereEffect->SetTranslate( boxCenter );
			atmosphereEffect->UpdateProperties();
		} else {
			atmosphereEffect->Set( UtopiaApp::GetApp()->GetRealTime(), depth, extent );
		}
		atmosphereEffect->SetRotate( rotation );
	} else {
		if ( atmosphereEffect != NULL && atmosphereEffect->GetParent() != NULL ) {
			atmosphereEffect->GetParent()->DetachChild( atmosphereEffect );
		}
		atmosphereEffect = NULL;
		g_kSoundManager.StopSound( unknown23C );
		unknown23C = unknown896A9C;
	}
}

/*
================
ShipClass::VirtualSlot87

Refreshes the subsystems against the first visible damage: their damage
cells are recorded on the first call, and when broken systems are
destroyed, every subsystem but the hull whose cell has been carved away
is destroyed.
================
*/
// FUNCTION: STBC 0x005b3480
void ShipClass::VirtualSlot87( void ) {
	if ( visibleDamage.GetSize() != 0 ) {
		VisibleDamage *damage = visibleDamage.GetAt( 0 );
		if ( damage != NULL ) {
			NiTListIterator pos = subsystems.list.GetHeadPos();
			if ( subsystems.list.GetHead()->unknown28 == 0xFFFF ) {
				while ( pos ) {
					Unknown5B3740( damage, subsystems.list.GetNext( pos ) );
				}
			}
			if ( destroyBrokenSystems ) {
				pos = subsystems.list.GetHeadPos();
				NiTList<ShipSubsystem *> broken( 2 );
				while ( pos ) {
					ShipSubsystem *subsystem = subsystems.list.GetNext( pos );
					if ( subsystem != GetHull() ) {
						Unknown5B3670( damage, subsystem, &broken );
					}
				}
				pos = broken.GetHeadPos();
				while ( pos ) {
					DestroySystem( broken.GetNext( pos ) );
				}
			}
		}
	}
}

/*
================
ShipClass::SetContainingSet

Detaches the two scene objects from their parents and stops the ship's
sound before the move to the set.
================
*/
// FUNCTION: STBC 0x005b35a0
void ShipClass::SetContainingSet( SetClass *set ) {
	if ( atmosphereEffect != NULL && atmosphereEffect->GetParent() != NULL ) {
		atmosphereEffect->GetParent()->DetachChild( atmosphereEffect );
	}
	atmosphereEffect = NULL;
	if ( shieldPatches != NULL && shieldPatches->GetParent() != NULL ) {
		shieldPatches->GetParent()->DetachChild( shieldPatches );
	}
	shieldPatches = NULL;
	g_kSoundManager.StopSound( unknown23C );
	unknown23C = unknown896A9C;
	PhysicsObjectClass::SetContainingSet( set );
}

/*
================
ShipClass::Unknown5B3670
================
*/
// FUNCTION: STBC 0x005b3670
void ShipClass::Unknown5B3670( VisibleDamage *damage, ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *found ) {
	if ( !subsystem->IsInvincible() && subsystem->IsTargetable() == true && subsystem->GetCondition() > 0.0f
		&& !damage->IsCellSet( subsystem->unknown28, subsystem->unknown2A, subsystem->unknown2C ) ) {
		found->AddTail( subsystem );
	}
	if ( subsystem->GetNumChildSubsystems() > 0 ) {
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			ShipSubsystem *child = subsystem->GetChildSubsystem( i );
			Unknown5B3670( damage, child, found );
		}
	}
}

/*
================
ShipClass::Unknown5B3740
================
*/
// FUNCTION: STBC 0x005b3740
// PARKED: inline budget: retail calls NiPoint3's constructor (0x414540) out of line for the scaled damage point and the translated sum (the ShipSubsystem damage-point transform wall); this build expands both. The cell search, frame and calls match
void ShipClass::Unknown5B3740( VisibleDamage *damage, ShipSubsystem *subsystem ) {
	NiPoint3 point = TGModelUtils::LocalToWorldPoint( GetNode(), subsystem->GetDamagePoint() );
	unsigned short x;
	unsigned short y;
	unsigned short z;
	damage->GetCell( &point, &x, &y, &z );
	if ( !damage->IsCellFilled( x, y, z ) ) {
		int cellX = x;
		int cellY = y;
		int cellZ = z;
		for ( int i = 0; i < 26; i++ ) {
			if ( cellX + cellNeighbours[i][0] >= 0 && cellY + cellNeighbours[i][1] >= 0 && cellZ + cellNeighbours[i][2] >= 0
				&& damage->IsCellFilled( x + cellNeighbours[i][0], y + cellNeighbours[i][1], z + cellNeighbours[i][2] ) ) {
				x += cellNeighbours[i][0];
				y += cellNeighbours[i][1];
				z += cellNeighbours[i][2];
				i = 27;
			}
		}
	}
	subsystem->unknown28 = x;
	subsystem->unknown2A = y;
	subsystem->unknown2C = z;
	if ( subsystem->GetNumChildSubsystems() > 0 ) {
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			Unknown5B3740( damage, subsystem->GetChildSubsystem( i ) );
		}
	}
}

/*
================
ShipClass_GetSubsystems

The Python method ShipClass.GetSubsystems: a new tuple of the Python
objects of the ship's subsystems, in the order they were added.
================
*/
// FUNCTION: STBC 0x005b3930
PyObject *ShipClass_GetSubsystems( PyObject *self, PyObject *args ) {
	PyObject *tuple = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		ShipClass *ship;
		PyEmbed::Unknown6F98E0( instance, (void **)&ship, "ShipClass" );
		if ( ship != NULL ) {
			NiTList<ShipSubsystem *> *list = &ship->GetSubsystemList();
			// BUG: redundant: the address of a member is never NULL
			if ( list == NULL ) {
				PyErr_SetString( PyExc_AttributeError, "NULL subsystem list.  Can't return subsystem tuple." );
			} else {
				tuple = PyTuple_New( list->GetSize() );
				int index = 0;
				NiTListIterator pos = list->GetHeadPos();
				while ( pos != NULL ) {
					PyTuple_SetItem( tuple, index++, list->GetNext( pos )->VirtualSlot08() );
				}
			}
		}
	}
	return tuple;
}


