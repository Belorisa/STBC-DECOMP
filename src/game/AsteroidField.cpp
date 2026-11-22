/*
===============================================================================

	AsteroidField.cpp

===============================================================================
*/

#include <iostream>
#include "AsteroidField.h"
#include <string.h>
#include "TGStream.h"
#include "ProximityCheck.h"
#include "ProximityManager.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGStringEvent.h"
#include "NiNode.h"
#include "TGEventManager.h"
#include "ProximityEvent.h"
#include "WarpEngineSubsystem.h"
#include "AsteroidTile.h"
#include "TGModelUtils.h"
#include <stdio.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0041baf0 _$E483
// FUNCTION: STBC 0x0041bb10 _$E482

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00419370 _$E32
// FUNCTION: STBC 0x00419380 _$E29
// FUNCTION: STBC 0x00419390 _$E31
// FUNCTION: STBC 0x004193a0 _$E30
// FUNCTION: STBC 0x004193b0 _$E38
// FUNCTION: STBC 0x004193c0 _$E35
// FUNCTION: STBC 0x004193d0 _$E37
// FUNCTION: STBC 0x004193e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004193f0 _$E66
// FUNCTION: STBC 0x00419400 _$E65
// FUNCTION: STBC 0x00419410 _$E293
// FUNCTION: STBC 0x00419420 _$E292
// FUNCTION: STBC 0x00419430 _$E77
// FUNCTION: STBC 0x00419440 _$E76
// FUNCTION: STBC 0x00419450 _$E81
// FUNCTION: STBC 0x00419460 _$E80
// FUNCTION: STBC 0x00419470 _$E85
// FUNCTION: STBC 0x00419480 _$E84
// FUNCTION: STBC 0x00419490 _$E89
// FUNCTION: STBC 0x004194a0 _$E88
// FUNCTION: STBC 0x004196b0 _$E206
// FUNCTION: STBC 0x004196c0 _$E205
// FUNCTION: STBC 0x0041b270 _$E148
// FUNCTION: STBC 0x0041b280 _$E147
// FUNCTION: STBC 0x0041b290 _$E161
// FUNCTION: STBC 0x0041b2a0 _$E160
// FUNCTION: STBC 0x0041b2b0 _$E211
// FUNCTION: STBC 0x0041b2c0 _$E210
// FUNCTION: STBC 0x0041b2d0 _$E298
// FUNCTION: STBC 0x0041b2e0 _$E297
// FUNCTION: STBC 0x0041b2f0 _$E215
// FUNCTION: STBC 0x0041b300 _$E214
// FUNCTION: STBC 0x0041b310 _$E302
// FUNCTION: STBC 0x0041b320 _$E301
// FUNCTION: STBC 0x0041b330 _$E309
// FUNCTION: STBC 0x0041b340 _$E308
// FUNCTION: STBC 0x0041b350 _$E319
// FUNCTION: STBC 0x0041b360 _$E318
// FUNCTION: STBC 0x0041b370 _$E323
// FUNCTION: STBC 0x0041b380 _$E322
// FUNCTION: STBC 0x0041b390 _$E106
// FUNCTION: STBC 0x0041b3a0 _$E105
// FUNCTION: STBC 0x0041b3b0 _$E245
// FUNCTION: STBC 0x0041b3c0 _$E244
// FUNCTION: STBC 0x0041b3d0 _$E252
// FUNCTION: STBC 0x0041b3e0 _$E251
// FUNCTION: STBC 0x0041b3f0 _$E256
// FUNCTION: STBC 0x0041b400 _$E255
// FUNCTION: STBC 0x0041b410 _$E260
// FUNCTION: STBC 0x0041b420 _$E259
// FUNCTION: STBC 0x0041b430 _$E264
// FUNCTION: STBC 0x0041b440 _$E263
// FUNCTION: STBC 0x0041b450 _$E270
// FUNCTION: STBC 0x0041b460 _$E269
// FUNCTION: STBC 0x0041b470 _$E327
// FUNCTION: STBC 0x0041b480 _$E326
// FUNCTION: STBC 0x0041b490 _$E331
// FUNCTION: STBC 0x0041b4a0 _$E330
// FUNCTION: STBC 0x0041b4b0 _$E335
// FUNCTION: STBC 0x0041b4c0 _$E334
// FUNCTION: STBC 0x0041b4d0 _$E339
// FUNCTION: STBC 0x0041b4e0 _$E338
// FUNCTION: STBC 0x0041b4f0 _$E343
// FUNCTION: STBC 0x0041b500 _$E342
// FUNCTION: STBC 0x0041b510 _$E347
// FUNCTION: STBC 0x0041b520 _$E346
// FUNCTION: STBC 0x0041b530 _$E351
// FUNCTION: STBC 0x0041b540 _$E350
// FUNCTION: STBC 0x0041b550 _$E355
// FUNCTION: STBC 0x0041b560 _$E354
// FUNCTION: STBC 0x0041b570 _$E359
// FUNCTION: STBC 0x0041b580 _$E358
// FUNCTION: STBC 0x0041b590 _$E363
// FUNCTION: STBC 0x0041b5a0 _$E362
// FUNCTION: STBC 0x0041b5b0 _$E111
// FUNCTION: STBC 0x0041b5c0 _$E110
// FUNCTION: STBC 0x0041b5d0 _$E367
// FUNCTION: STBC 0x0041b5e0 _$E366
// FUNCTION: STBC 0x0041b5f0 _$E116
// FUNCTION: STBC 0x0041b600 _$E115
// FUNCTION: STBC 0x0041b610 _$E371
// FUNCTION: STBC 0x0041b620 _$E370
// FUNCTION: STBC 0x0041b630 _$E375
// FUNCTION: STBC 0x0041b640 _$E374
// FUNCTION: STBC 0x0041b650 _$E379
// FUNCTION: STBC 0x0041b660 _$E378
// FUNCTION: STBC 0x0041b670 _$E383
// FUNCTION: STBC 0x0041b680 _$E382
// FUNCTION: STBC 0x0041b690 _$E125
// FUNCTION: STBC 0x0041b6a0 _$E124
// FUNCTION: STBC 0x0041b6b0 _$E387
// FUNCTION: STBC 0x0041b6c0 _$E386
// FUNCTION: STBC 0x0041b6d0 _$E391
// FUNCTION: STBC 0x0041b6e0 _$E390
// FUNCTION: STBC 0x0041b6f0 _$E395
// FUNCTION: STBC 0x0041b700 _$E394
// FUNCTION: STBC 0x0041b710 _$E131
// FUNCTION: STBC 0x0041b720 _$E130
// FUNCTION: STBC 0x0041b730 _$E136
// FUNCTION: STBC 0x0041b740 _$E135
// FUNCTION: STBC 0x0041b750 _$E166
// FUNCTION: STBC 0x0041b760 _$E165
// FUNCTION: STBC 0x0041b770 _$E399
// FUNCTION: STBC 0x0041b780 _$E398
// FUNCTION: STBC 0x0041b790 _$E171
// FUNCTION: STBC 0x0041b7a0 _$E170
// FUNCTION: STBC 0x0041b7b0 _$E403
// FUNCTION: STBC 0x0041b7c0 _$E402
// FUNCTION: STBC 0x0041b7d0 _$E407
// FUNCTION: STBC 0x0041b7e0 _$E406
// FUNCTION: STBC 0x0041b7f0 _$E176
// FUNCTION: STBC 0x0041b800 _$E175
// FUNCTION: STBC 0x0041b810 _$E411
// FUNCTION: STBC 0x0041b820 _$E410
// FUNCTION: STBC 0x0041b830 _$E141
// FUNCTION: STBC 0x0041b840 _$E140
// FUNCTION: STBC 0x0041b850 _$E415
// FUNCTION: STBC 0x0041b860 _$E414
// FUNCTION: STBC 0x0041b870 _$E419
// FUNCTION: STBC 0x0041b880 _$E418
// FUNCTION: STBC 0x0041b890 _$E181
// FUNCTION: STBC 0x0041b8a0 _$E180
// FUNCTION: STBC 0x0041b8b0 _$E186
// FUNCTION: STBC 0x0041b8c0 _$E185
// FUNCTION: STBC 0x0041b8d0 _$E423
// FUNCTION: STBC 0x0041b8e0 _$E422
// FUNCTION: STBC 0x0041b8f0 _$E427
// FUNCTION: STBC 0x0041b900 _$E426
// FUNCTION: STBC 0x0041b910 _$E431
// FUNCTION: STBC 0x0041b920 _$E430
// FUNCTION: STBC 0x0041b930 _$E435
// FUNCTION: STBC 0x0041b940 _$E434
// FUNCTION: STBC 0x0041b950 _$E439
// FUNCTION: STBC 0x0041b960 _$E438
// FUNCTION: STBC 0x0041b970 _$E443
// FUNCTION: STBC 0x0041b980 _$E442
// FUNCTION: STBC 0x0041b9b0 _$E447
// FUNCTION: STBC 0x0041b9c0 _$E446
// FUNCTION: STBC 0x0041b9f0 _$E451
// FUNCTION: STBC 0x0041ba00 _$E450
// FUNCTION: STBC 0x0041ba10 _$E191
// FUNCTION: STBC 0x0041ba20 _$E190
// FUNCTION: STBC 0x0041ba30 _$E455
// FUNCTION: STBC 0x0041ba40 _$E454
// FUNCTION: STBC 0x0041ba50 _$E196
// FUNCTION: STBC 0x0041ba60 _$E195
// FUNCTION: STBC 0x0041ba70 _$E201
// FUNCTION: STBC 0x0041ba80 _$E200
// FUNCTION: STBC 0x0041bce0 _$E459
// FUNCTION: STBC 0x0041bcf0 _$E458
// FUNCTION: STBC 0x0041bd00 _$E464
// FUNCTION: STBC 0x0041bd10 _$E463
// FUNCTION: STBC 0x0041bd20 _$E468
// FUNCTION: STBC 0x0041bd30 _$E467


/*
================
AsteroidField::Cast

The object as an AsteroidField; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004194b0
AsteroidField *AsteroidField::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ASTEROIDFIELD_TYPE ) ) {
		return (AsteroidField *)object;
	}
	return NULL;
}

/*
================
AsteroidField::AddToSet
================
*/
// FUNCTION: STBC 0x00419530
void AsteroidField::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, ASTEROIDFIELD_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
AsteroidField::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00419560
void AsteroidField::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, ASTEROIDFIELD_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
AsteroidField::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x004195c0
int AsteroidField::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( ASTEROIDFIELD_TYPE );
	}
	return 0;
}

/*
================
AsteroidField::GetObjectByID

The object of the class with the ID in the set, or in the object table when
the set is NULL; NULL when it is none or not an AsteroidField.
================
*/
// FUNCTION: STBC 0x004195e0
AsteroidField *AsteroidField::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( ASTEROIDFIELD_TYPE ) ) {
			return (AsteroidField *)object;
		}
		return NULL;
	}
	return (AsteroidField *)set->GetClassObject( ASTEROIDFIELD_TYPE, id );
}

/*
================
AsteroidField::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x00419650
AsteroidField *AsteroidField::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( ASTEROIDFIELD_TYPE, name );
		}
		return (AsteroidField *)object;
	}
	return (AsteroidField *)set->GetClassObject( ASTEROIDFIELD_TYPE, name );
}

/*
================
AsteroidField::AsteroidField

The field's own node; a field made with no ID (a new one, not one being
loaded) also follows ships into and out of sets.
================
*/
// FUNCTION: STBC 0x004196d0
AsteroidField::AsteroidField( long id, int tilesAcross_ ) : ObjectClass( id ), unknown68( 400.0f ), unknownA8( 2 ) {
	SetNiObject( new NiNode );
	proximityCheck = NULL;
	tileCount = 0;
	asteroidSizeFactor = 10.0f;
	SetOuterFieldRadius( 20.0f );
	SetInnerFieldRadius( 10.0f );
	tilesAcross = tilesAcross_;
	if ( id == 0 ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "AsteroidField::HandleShipEnterSet", ANY_TARGET );
		g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "AsteroidField::HandleShipExitSet", ANY_TARGET );
	}
	unknownC0 = 0;
	SetCollisionFlags( CFB_IN_PROXIMITY_MANAGER | CFB_COLLIDES_WITH_GROUP_3 );
	SetHailable( false );
	SetScannable( false );
}

/*
================
AsteroidField::GetObjType
================
*/
// FUNCTION: STBC 0x00419980
int AsteroidField::GetObjType( void ) {
	return ASTEROIDFIELD_TYPE;
}

/*
================
AsteroidField::IsTypeOf
================
*/
// FUNCTION: STBC 0x00419990
bool AsteroidField::IsTypeOf( int type ) {
	if ( type == ASTEROIDFIELD_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
AsteroidField::GetClassName
================
*/
// FUNCTION: STBC 0x004199f0
char *AsteroidField::GetClassName( void ) {
	return "AsteroidField";
}

/*
================
AsteroidField::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00419a00
char *AsteroidField::GetSwigTypeName( void ) {
	return "_p_AsteroidField";
}

/*
================
AsteroidField::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00419a10
char *AsteroidField::GetPythonClassName( void ) {
	return "AsteroidFieldPtr";
}

/*
================
AsteroidField::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00419a20
TGEventHandlerTable *AsteroidField::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
AsteroidField::~AsteroidField
================
*/
// FUNCTION: STBC 0x00419a60
AsteroidField::~AsteroidField( void ) {
	DeleteSlots();
	DeleteTiles();
}

/*
================
AsteroidField::RegisterHandlerNames

The field's handler methods by their class-qualified names.
================
*/
// FUNCTION: STBC 0x00419b30
void AsteroidField::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidField::HandleEnterSet, "AsteroidField::HandleEnterSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidField::HandleExitSet, "AsteroidField::HandleExitSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidField::HandleShipEnterSet, "AsteroidField::HandleShipEnterSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidField::HandleShipExitSet, "AsteroidField::HandleShipExitSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&AsteroidField::HandleProximityEvent, "AsteroidField::HandleProximityEvent" );
}

/*
================
AsteroidField::RegisterHandlers

The field's set and proximity handlers in its class table.
================
*/
// FUNCTION: STBC 0x00419ba0
void AsteroidField::RegisterHandlers( void ) {
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_ENTERED_SET, "AsteroidField::HandleEnterSet" );
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_EXITED_SET, "AsteroidField::HandleExitSet" );
	TGEventHandlerObject::AddMethodHandlerForClass( &classHandlers, ET_PROXIMITY_ASTEROID_FIELD, "AsteroidField::HandleProximityEvent" );
}

/*
================
AsteroidField::Update

Updates every slot, then the object.
================
*/
// FUNCTION: STBC 0x00419be0
void AsteroidField::Update( float time ) {
	for ( unsigned int i = 0; i < slots.GetSize(); i++ ) {
		AsteroidTileSlot *slot = slots.GetAt( i );
		if ( slot != NULL ) {
			UpdateSlot( slot, i );
		}
	}
	ObjectClass::Update( time );
}

/*
================
AsteroidField::UpdateSlot

An empty slot takes a tile (a free one, else a new one) when a ship of
the field comes near it, and files the tile in the field's set; a filled
slot shows its tile while a ship is near, and otherwise gives it up (or
only hides it when the tile is kept).
================
*/
// FUNCTION: STBC 0x00419c30
void AsteroidField::UpdateSlot( AsteroidTileSlot *slot, unsigned int index ) {
	NiTListIterator pos = unknownA8.GetHeadPos();
	if ( slot->entry == NULL ) {
		while ( pos != NULL ) {
			if ( IsShipNearSlot( slot, unknownA8.GetNext( pos ) ) == true ) {
				slot->entry = FindUnplacedTile();
				if ( slot->entry == NULL ) {
					slot->entry = CreateTile();
					slot->entry->unknown08 = false;
				}
				slot->unknown04 = true;
				NiPoint3 position = GetWorldLocation() + slot->offset;
				slot->entry->tile->SetTranslate( &position );
				slot->entry->slot = slot;
				slot->entry->tile->SetRandomSeed( index + 0x10ABCDEF );
				GetContainingSet()->AddObjectToSet( slot->entry->tile, slot->entry->tile->GetName() );
				slot->entry->tile->UpdateNodeOnly();
				return;
			}
		}
		return;
	}

	bool shipNear = false;
	while ( pos != NULL && !shipNear ) {
		if ( IsShipNearSlot( slot, unknownA8.GetNext( pos ) ) == true ) {
			shipNear = true;
		}
	}
	if ( !shipNear ) {
		if ( !slot->entry->unknown08 ) {
			GetContainingSet()->RemoveObjectFromSet( slot->entry->tile->GetObjID() );
			slot->entry->slot = NULL;
			slot->entry = NULL;
			slot->unknown04 = false;
		} else {
			if ( !slot->entry->tile->IsHidden() ) {
				slot->entry->tile->SetHidden( true );
			}
			slot->unknown04 = false;
		}
	} else {
		if ( slot->entry->tile->IsHidden() ) {
			slot->entry->tile->SetHidden( false );
		}
		slot->unknown04 = true;
	}
}

/*
================
AsteroidField::IsShipNearSlot

Whether the ship (not in warp) is within a tile's diagonal, plus 0x68,
of the slot's place in the world.
================
*/
// FUNCTION: STBC 0x00419de0
// PARKED: schedule: the x87 sum is retail's; the pop of esi (the node) issues right after the tile size's fadd here, after the last stack pops in retail (c2trace --sched: it fills an early free slot)
bool AsteroidField::IsShipNearSlot( AsteroidTileSlot *slot, ShipClass *ship ) {
	if ( ship != NULL ) {
		WarpEngineSubsystem *warpEngines = ship->GetWarpEngineSubsystem();
		if ( warpEngines == NULL || warpEngines->GetWarpState() == WarpEngineSubsystem::WES_NOT_WARPING ) {
			NiPoint3 slotPosition = TGModelUtils::LocalToWorldPoint( GetNode(), slot->offset );
			NiPoint3 shipPosition = ship->GetWorldLocation();
			NiPoint3 offset = shipPosition - slotPosition;
			float tileSize = GetTileSize();
			if ( offset * offset <= tileSize * tileSize + unknown68 * unknown68 * 2.0f ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
AsteroidField::HandleEnterSet

The field entered a set: replace its proximity check with one around the
outer radius that holds the set's ships, put it in the set's proximity
manager and make the slots. A set without a proximity manager gets none.
================
*/
// FUNCTION: STBC 0x00419f00
void AsteroidField::HandleEnterSet( TGEvent *event ) {
	SetClass *set = GetContainingSet();
	if ( proximityCheck != NULL ) {
		DetachObject( proximityCheck );
		proximityCheck->RemoveAndDelete();
		proximityCheck = NULL;
	}
	ProximityManager *manager = set->GetProximityManager();
	if ( manager == NULL ) {
		CallNextHandler( event );
		return;
	}
	proximityCheck = new ProximityCheck( ET_PROXIMITY_ASTEROID_FIELD, this );
	proximityCheck->SetRadius( ( outerFieldRadius + unknown68 ) * 1.42f );
	int count = 0;
	ObjectClass **ships = (ObjectClass **)set->objectTable.GetObjects( SHIPCLASS_TYPE, &count );
	if ( count != 0 ) {
		proximityCheck->AddObjectListToCheckList( ships, count, 0 );
	}
	AttachObject( proximityCheck );
	manager->AddObject( proximityCheck );
	CreateSlots();
	CallNextHandler( event );
}

/*
================
AsteroidField::HandleExitSet

The field left a set: drop its proximity check and clear its slots and tiles.
================
*/
// FUNCTION: STBC 0x00419ff0
void AsteroidField::HandleExitSet( TGEvent *event ) {
	if ( proximityCheck != NULL ) {
		DetachObject( proximityCheck );
		proximityCheck->RemoveAndDelete();
		proximityCheck = NULL;
	}
	DeleteSlots();
	DeleteTiles();
	unknownA8.RemoveAll();
	CallNextHandler( event );
}

/*
================
AsteroidField::HandleShipEnterSet

A ship entered a set: when it is the field's own set, put the ship into the proximity check.
================
*/
// FUNCTION: STBC 0x0041a080
void AsteroidField::HandleShipEnterSet( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship != NULL && ship->GetContainingSet() == GetContainingSet() && proximityCheck != NULL ) {
		proximityCheck->AddObjectToCheckList( ship, 0 );
	}
	CallNextHandler( event );
}

/*
================
AsteroidField::HandleShipExitSet

A ship left a set: when it is the field's own set, take the ship out of the proximity check
and out of the field's list of objects in the field.
================
*/
// FUNCTION: STBC 0x0041a0c0
void AsteroidField::HandleShipExitSet( TGStringEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship == NULL ) {
		CallNextHandler( event );
		return;
	}
	if ( GetContainingSet() == NULL ) {
		// BUG: the next handler is not called when the field has no set.
		return;
	}
	char *setName = event->GetCString();
	int different = strcmp( setName, GetContainingSet()->GetName() );
	delete [] setName;
	if ( different != 0 ) {
		// BUG: the next handler is not called when the ship left another set.
		return;
	}
	if ( proximityCheck != NULL ) {
		proximityCheck->RemoveObjectFromCheckList( ship );
	}
	unknownA8.Remove( ship );
	CallNextHandler( event );
}

/*
================
AsteroidField::HandleProximityEvent

A ship crossed the field's proximity check: entering, it joins the
field's list; leaving, it drops out. Ships without an ID and ships in
warp are left alone.
================
*/
// FUNCTION: STBC 0x0041a240
void AsteroidField::HandleProximityEvent( ProximityEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetObject() );
	if ( ship == NULL ) {
		CallNextHandler( event );
		return;
	}
	if ( ship->GetObjID() == 0 ) {
		CallNextHandler( event );
		return;
	}
	WarpEngineSubsystem *warpEngines = ship->GetWarpEngineSubsystem();
	if ( warpEngines != NULL && warpEngines->GetWarpState() != WarpEngineSubsystem::WES_NOT_WARPING ) {
		CallNextHandler( event );
		return;
	}

	if ( proximityCheck->GetTriggerType( ship ) == 0 ) {
		proximityCheck->SetTriggerType( ship, 1 );
		unknownA8.AddTail( ship );
	} else if ( proximityCheck->GetTriggerType( ship ) == 1 ) {
		proximityCheck->SetTriggerType( ship, 0 );
		unknownA8.Remove( ship );
	}
	CallNextHandler( event );
}

/*
================
AsteroidField::IsShipInside

Whether the ship is within the inner radius of the field.
================
*/
// FUNCTION: STBC 0x0041a440
bool AsteroidField::IsShipInside( ShipClass *ship ) {
	if ( ship != NULL && ( GetWorldLocation() - ship->GetWorldLocation() ).Length() <= innerFieldRadius ) {
		return true;
	}
	return false;
}

/*
================
AsteroidField::CreateSlots

One slot per tile of the grid tilesAcross tiles wide each way, centred
on the field.
================
*/
// FUNCTION: STBC 0x0041a4a0
// PARKED: schedule: retail loads the tile count and stores i = 0 before loading the radius for the tile size; the accessor, an open-coded size in four spellings and declaration order are flat
void AsteroidField::CreateSlots( void ) {
	DeleteSlots();
	float tileSize = GetTileSize();
	NiPoint3 position;
	for ( int i = 0; i < tilesAcross; i++ ) {
		float halfTile = tileSize * 0.5f;
		position.x = ( i - tilesAcross * 0.5f ) * tileSize + halfTile;
		for ( int j = 0; j < tilesAcross; j++ ) {
			position.y = ( j - tilesAcross * 0.5f ) * tileSize + halfTile;
			for ( int k = 0; k < tilesAcross; k++ ) {
				position.z = ( k - tilesAcross * 0.5f ) * tileSize + halfTile;
				AsteroidTileSlot *slot = new AsteroidTileSlot;
				slot->unknown04 = false;
				slot->offset = position;
				slot->entry = NULL;
				slots.Add( slot );
			}
		}
	}
}

/*
================
AsteroidField::DeleteSlots
================
*/
// FUNCTION: STBC 0x0041a6f0
void AsteroidField::DeleteSlots( void ) {
	for ( unsigned int i = 0; slots.GetEffectiveSize() > 0; i++ ) {
		AsteroidTileSlot *slot = slots.GetAt( i );
		if ( slot != NULL ) {
			delete slot;
		}
		slots.RemoveAt( i );
	}
}

/*
================
AsteroidField::CreateTile

A new tile of the field's asteroid settings, named "<field> Tile <n>",
in a new entry in the tile list.
================
*/
// FUNCTION: STBC 0x0041a760
// PARKED: float copy: retail passes the size factor through the FPU (fld/fstp) where this build pushes it with mov; both accessors 92.5, the float accessor alone 89.9 (spilled before the new), a (float) cast no change
AsteroidTileEntry *AsteroidField::CreateTile( void ) {
	AsteroidTileEntry *entry = new AsteroidTileEntry;
	AsteroidTile *tile = new AsteroidTile( numAsteroidsPerTile, asteroidSizeFactor, 0 );
	tile->SetField( this );
	tile->SetTileSize( GetTileSize() );
	char name[100];
	sprintf( name, "%s Tile %d", GetName(), tileCount++ );
	tile->SetName( name );
	entry->unknown08 = false;
	entry->slot = NULL;
	entry->tile = tile;
	tiles.Add( entry );
	return entry;
}

/*
================
AsteroidField::FindUnplacedTile
================
*/
// FUNCTION: STBC 0x0041a910
AsteroidTileEntry *AsteroidField::FindUnplacedTile( void ) {
	for ( unsigned int i = 0; i < tiles.GetSize(); i++ ) {
		AsteroidTileEntry *entry = tiles.GetAt( i );
		if ( entry != NULL && !entry->unknown08 && entry->slot == NULL ) {
			return entry;
		}
	}
	return NULL;
}

/*
================
AsteroidField::DeleteTiles

Takes each tile out of its set and deletes the entries (invented name).
================
*/
// FUNCTION: STBC 0x0041a950
void AsteroidField::DeleteTiles( void ) {
	for ( unsigned int i = 0; tiles.GetEffectiveSize() > 0; i++ ) {
		AsteroidTileEntry *entry = tiles.GetAt( i );
		if ( entry != NULL ) {
			if ( entry->tile->GetContainingSet() != NULL ) {
				entry->tile->GetContainingSet()->DeleteObjectFromSet( entry->tile->GetName() );
			}
			delete entry;
		}
		tiles.RemoveAt( i );
	}
}

/*
================
AsteroidField::SetOuterFieldRadius

The proximity check's radius follows, padded by the margin at 0x68.
================
*/
// FUNCTION: STBC 0x0041a9d0
void AsteroidField::SetOuterFieldRadius( float radius ) {
	outerFieldRadius = radius;
	if ( proximityCheck != NULL ) {
		proximityCheck->SetRadius( ( radius + unknown68 ) * 1.42f );
	}
}

/*
================
AsteroidField::SetInnerFieldRadius
================
*/
// FUNCTION: STBC 0x0041aa00
void AsteroidField::SetInnerFieldRadius( float radius ) {
	innerFieldRadius = radius;
}

/*
================
AsteroidField::SetNumAsteroidsPerTile
================
*/
// FUNCTION: STBC 0x0041aa10
void AsteroidField::SetNumAsteroidsPerTile( int count ) {
	numAsteroidsPerTile = count;
}

/*
================
AsteroidField::SetAsteroidSizeFactor
================
*/
// FUNCTION: STBC 0x0041aa20
void AsteroidField::SetAsteroidSizeFactor( float factor ) {
	asteroidSizeFactor = factor;
}

/*
================
AsteroidField::WriteToStream

The radii, the ID of the proximity check, the tiles that no slot holds, the
slots with their tiles, the list at 0xA8 by IDs and the counts.
================
*/
// FUNCTION: STBC 0x0041aa30
bool AsteroidField::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( outerFieldRadius );
	stream->WriteFloat( innerFieldRadius );
	stream->WriteID( TGObject::GetSavedID( proximityCheck ) );
	for ( unsigned int i = 0; i < tiles.GetSize(); i++ ) {
		AsteroidTileEntry *entry = tiles.GetAt( i );
		if ( entry != NULL && entry->slot == NULL ) {
			stream->WriteBool( true );
			stream->WriteID( TGObject::GetSavedID( entry->tile ) );
			ObjectClass *tile = entry->tile;
			if ( tile != NULL ) {
				if ( tile->GetContainingSet() == NULL ) {
					stream->WriteBool( true );
					tile->WriteToStream( stream );
				} else {
					stream->WriteBool( false );
				}
			} else {
				stream->WriteBool( false );
			}
			stream->WriteBool( entry->unknown08 );
		}
	}
	stream->WriteBool( false );
	stream->WriteInt( slots.GetEffectiveSize() );
	for ( unsigned int j = 0; j < slots.GetSize(); j++ ) {
		AsteroidTileSlot *slot = slots.GetAt( j );
		if ( slot != NULL ) {
			if ( slot->entry != NULL ) {
				stream->WriteBool( true );
				stream->WriteID( TGObject::GetSavedID( slot->entry->tile ) );
				stream->WriteBool( slot->entry->unknown08 );
			} else {
				stream->WriteBool( false );
			}
			stream->WriteBool( slot->unknown04 );
			stream->WriteFloat( slot->offset.x );
			stream->WriteFloat( slot->offset.y );
			stream->WriteFloat( slot->offset.z );
		}
	}
	stream->WriteInt( unknownA8.GetSize() );
	NiTListIterator pos = unknownA8.GetHeadPos();
	while ( pos != NULL ) {
		ShipClass *ship = unknownA8.GetNext( pos );
		stream->WriteID( ship->GetObjID() );
	}
	stream->WriteInt( unknownC0 );
	stream->WriteInt( tilesAcross );
	stream->WriteInt( numAsteroidsPerTile );
	stream->WriteFloat( asteroidSizeFactor );
	stream->WriteInt( tileCount );
	return true;
}

/*
================
AsteroidField::ReadFromStream

Reads what WriteToStream wrote: the saved IDs stay in the pointer members
until PostFixUp turns them into the objects.
================
*/
// FUNCTION: STBC 0x0041ac30
bool AsteroidField::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	outerFieldRadius = stream->ReadFloat();
	innerFieldRadius = stream->ReadFloat();
	proximityCheck = (ProximityCheck *)stream->ReadID();
	while ( stream->ReadBool() == true ) {
		AsteroidTileEntry *entry = new AsteroidTileEntry;
		entry->tile = (AsteroidTile *)stream->ReadID();
		if ( stream->ReadBool() == true ) {
			entry->tile = (AsteroidTile *)TGObject::LoadObject( stream, 0 );
			entry->tile = (AsteroidTile *)entry->tile->GetObjID();
		}
		entry->unknown08 = stream->ReadBool();
		entry->slot = NULL;
		tiles.AddFirstEmpty( entry );
	}
	for ( unsigned int numSlots = stream->ReadInt(); numSlots > 0; numSlots-- ) {
		AsteroidTileSlot *slot = new AsteroidTileSlot;
		if ( stream->ReadBool() == true ) {
			AsteroidTileEntry *entry = new AsteroidTileEntry;
			entry->tile = (AsteroidTile *)stream->ReadID();
			entry->unknown08 = stream->ReadBool();
			entry->slot = slot;
			slot->entry = entry;
			tiles.AddFirstEmpty( entry );
		} else {
			slot->entry = NULL;
		}
		slot->unknown04 = stream->ReadBool();
		slot->offset.x = stream->ReadFloat();
		slot->offset.y = stream->ReadFloat();
		slot->offset.z = stream->ReadFloat();
		slots.AddFirstEmpty( slot );
	}
	for ( unsigned int numObjects = stream->ReadInt(); numObjects > 0; numObjects-- ) {
		ShipClass *ship = (ShipClass *)stream->ReadID();
		unknownA8.AddTail( ship );
	}
	unknownC0 = stream->ReadInt();
	tilesAcross = stream->ReadInt();
	numAsteroidsPerTile = stream->ReadInt();
	asteroidSizeFactor = stream->ReadFloat();
	tileCount = stream->ReadInt();
	return true;
}

/*
================
AsteroidField::FixUpPointers

Every saved ID must name a live object.
================
*/
// FUNCTION: STBC 0x0041b050
bool AsteroidField::FixUpPointers( void ) {
	if ( !BaseObjectClass::FixUpPointers() ) {
		return false;
	}
	if ( proximityCheck != NULL && TGObject::GetTGObjectPtr( (long)proximityCheck ) == NULL ) {
		return false;
	}
	for ( unsigned int i = 0; i < tiles.GetSize(); i++ ) {
		AsteroidTileEntry *entry = tiles.GetAt( i );
		if ( entry != NULL && entry->tile != NULL ) {
			if ( TGObject::GetTGObjectPtr( (long)entry->tile ) == NULL ) {
				return false;
			}
		}
	}
	NiTListIterator pos = unknownA8.GetHeadPos();
	while ( pos != NULL ) {
		ShipClass *saved = unknownA8.GetNext( pos );
		if ( saved != NULL && TGObject::GetTGObjectPtr( (long)saved ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
AsteroidField::PostFixUp

The saved IDs become the objects they name; the list at 0xA8 is rebuilt
element by element.
================
*/
// FUNCTION: STBC 0x0041b0e0
void AsteroidField::PostFixUp( void ) {
	ObjectClass::PostFixUp();
	proximityCheck = (ProximityCheck *)TGObject::GetTGObjectPtr( (long)proximityCheck );
	for ( unsigned int i = 0; i < tiles.GetSize(); i++ ) {
		AsteroidTileEntry *entry = tiles.GetAt( i );
		if ( entry != NULL ) {
			entry->tile = (AsteroidTile *)TGObject::GetTGObjectPtr( (long)entry->tile );
		}
	}
	NiTListIterator pos = unknownA8.GetHeadPos();
	while ( pos != NULL ) {
		ShipClass *saved = unknownA8.GetNext( pos );
		unknownA8.AddHead( (ShipClass *)TGObject::GetTGObjectPtr( (long)saved ) );
		unknownA8.Remove( saved );
	}
}

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004194e0 _$E474
// FUNCTION: STBC 0x004194f0 _$E470
// FUNCTION: STBC 0x00419500 _$E473
// FUNCTION: STBC 0x00419510 _$E471
// DATA: STBC 0x0097eb58
TGEventHandlerTable AsteroidField::classHandlers;
