/*
===============================================================================

	WeaponHitEvent.cpp

===============================================================================
*/

#include <iostream>
#include "WeaponHitEvent.h"
#include "ObjectClass.h"
#include "ShipClass.h"
#include "TGStream.h"
#include "NiAVObject.h"
#include "NiMatrix3.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005b90d0 _$E455
// FUNCTION: STBC 0x005b90f0 _$E454

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005b8bd0 _$E32
// FUNCTION: STBC 0x005b8be0 _$E29
// FUNCTION: STBC 0x005b8bf0 _$E31
// FUNCTION: STBC 0x005b8c00 _$E30
// FUNCTION: STBC 0x005b8c10 _$E38
// FUNCTION: STBC 0x005b8c20 _$E35
// FUNCTION: STBC 0x005b8c30 _$E37
// FUNCTION: STBC 0x005b8c40 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005b8c50 _$E69
// FUNCTION: STBC 0x005b8c60 _$E68
// FUNCTION: STBC 0x005b8c70 _$E271
// FUNCTION: STBC 0x005b8c80 _$E270
// FUNCTION: STBC 0x005b8c90 _$E80
// FUNCTION: STBC 0x005b8ca0 _$E79
// FUNCTION: STBC 0x005b8cb0 _$E84
// FUNCTION: STBC 0x005b8cc0 _$E83
// FUNCTION: STBC 0x005b8cd0 _$E88
// FUNCTION: STBC 0x005b8ce0 _$E87
// FUNCTION: STBC 0x005b8cf0 _$E92
// FUNCTION: STBC 0x005b8d00 _$E91
// FUNCTION: STBC 0x005b8d10 _$E209
// FUNCTION: STBC 0x005b8d20 _$E208
// FUNCTION: STBC 0x005b8d30 _$E151
// FUNCTION: STBC 0x005b8d40 _$E150
// FUNCTION: STBC 0x005b8d50 _$E164
// FUNCTION: STBC 0x005b8d60 _$E163
// FUNCTION: STBC 0x005b8d70 _$E214
// FUNCTION: STBC 0x005b8d80 _$E213
// FUNCTION: STBC 0x005b8d90 _$E276
// FUNCTION: STBC 0x005b8da0 _$E275
// FUNCTION: STBC 0x005b8db0 _$E218
// FUNCTION: STBC 0x005b8dc0 _$E217
// FUNCTION: STBC 0x005b8dd0 _$E280
// FUNCTION: STBC 0x005b8de0 _$E279
// FUNCTION: STBC 0x005b8df0 _$E287
// FUNCTION: STBC 0x005b8e00 _$E286
// FUNCTION: STBC 0x005b8e10 _$E297
// FUNCTION: STBC 0x005b8e20 _$E296
// FUNCTION: STBC 0x005b8e30 _$E301
// FUNCTION: STBC 0x005b8e40 _$E300
// FUNCTION: STBC 0x005b8e50 _$E109
// FUNCTION: STBC 0x005b8e60 _$E108
// FUNCTION: STBC 0x005b8e70 _$E237
// FUNCTION: STBC 0x005b8e80 _$E236
// FUNCTION: STBC 0x005b8e90 _$E244
// FUNCTION: STBC 0x005b8ea0 _$E243
// FUNCTION: STBC 0x005b8eb0 _$E248
// FUNCTION: STBC 0x005b8ec0 _$E247
// FUNCTION: STBC 0x005b8ed0 _$E252
// FUNCTION: STBC 0x005b8ee0 _$E251
// FUNCTION: STBC 0x005b8ef0 _$E256
// FUNCTION: STBC 0x005b8f00 _$E255
// FUNCTION: STBC 0x005b8f10 _$E262
// FUNCTION: STBC 0x005b8f20 _$E261
// FUNCTION: STBC 0x005b8f30 _$E305
// FUNCTION: STBC 0x005b8f40 _$E304
// FUNCTION: STBC 0x005b8f50 _$E309
// FUNCTION: STBC 0x005b8f60 _$E308
// FUNCTION: STBC 0x005b8f70 _$E313
// FUNCTION: STBC 0x005b8f80 _$E312
// FUNCTION: STBC 0x005b8f90 _$E317
// FUNCTION: STBC 0x005b8fa0 _$E316
// FUNCTION: STBC 0x005b8fb0 _$E321
// FUNCTION: STBC 0x005b8fc0 _$E320
// FUNCTION: STBC 0x005b8fd0 _$E325
// FUNCTION: STBC 0x005b8fe0 _$E324
// FUNCTION: STBC 0x005b8ff0 _$E329
// FUNCTION: STBC 0x005b9000 _$E328
// FUNCTION: STBC 0x005b9010 _$E333
// FUNCTION: STBC 0x005b9020 _$E332
// FUNCTION: STBC 0x005b9030 _$E337
// FUNCTION: STBC 0x005b9040 _$E336
// FUNCTION: STBC 0x005b9050 _$E341
// FUNCTION: STBC 0x005b9060 _$E340
// FUNCTION: STBC 0x005b9070 _$E114
// FUNCTION: STBC 0x005b9080 _$E113
// FUNCTION: STBC 0x005b9090 _$E345
// FUNCTION: STBC 0x005b90a0 _$E344
// FUNCTION: STBC 0x005b90b0 _$E119
// FUNCTION: STBC 0x005b90c0 _$E118
// FUNCTION: STBC 0x005b9100 _$E349
// FUNCTION: STBC 0x005b9110 _$E348
// FUNCTION: STBC 0x005b9120 _$E353
// FUNCTION: STBC 0x005b9130 _$E352
// FUNCTION: STBC 0x005b9140 _$E357
// FUNCTION: STBC 0x005b9150 _$E356
// FUNCTION: STBC 0x005b9160 _$E361
// FUNCTION: STBC 0x005b9170 _$E360
// FUNCTION: STBC 0x005b9180 _$E128
// FUNCTION: STBC 0x005b9190 _$E127
// FUNCTION: STBC 0x005b91a0 _$E365
// FUNCTION: STBC 0x005b91b0 _$E364
// FUNCTION: STBC 0x005b91c0 _$E369
// FUNCTION: STBC 0x005b91d0 _$E368
// FUNCTION: STBC 0x005b91e0 _$E373
// FUNCTION: STBC 0x005b91f0 _$E372
// FUNCTION: STBC 0x005b9200 _$E134
// FUNCTION: STBC 0x005b9210 _$E133
// FUNCTION: STBC 0x005b9220 _$E139
// FUNCTION: STBC 0x005b9230 _$E138
// FUNCTION: STBC 0x005b9240 _$E169
// FUNCTION: STBC 0x005b9250 _$E168
// FUNCTION: STBC 0x005b9260 _$E377
// FUNCTION: STBC 0x005b9270 _$E376
// FUNCTION: STBC 0x005b9280 _$E174
// FUNCTION: STBC 0x005b9290 _$E173
// FUNCTION: STBC 0x005b92a0 _$E381
// FUNCTION: STBC 0x005b92b0 _$E380
// FUNCTION: STBC 0x005b92c0 _$E385
// FUNCTION: STBC 0x005b92d0 _$E384
// FUNCTION: STBC 0x005b92e0 _$E179
// FUNCTION: STBC 0x005b92f0 _$E178
// FUNCTION: STBC 0x005b9300 _$E389
// FUNCTION: STBC 0x005b9310 _$E388
// FUNCTION: STBC 0x005b9320 _$E144
// FUNCTION: STBC 0x005b9330 _$E143
// FUNCTION: STBC 0x005b9340 _$E393
// FUNCTION: STBC 0x005b9350 _$E392
// FUNCTION: STBC 0x005b9360 _$E397
// FUNCTION: STBC 0x005b9370 _$E396
// FUNCTION: STBC 0x005b9380 _$E184
// FUNCTION: STBC 0x005b9390 _$E183
// FUNCTION: STBC 0x005b93a0 _$E189
// FUNCTION: STBC 0x005b93b0 _$E188
// FUNCTION: STBC 0x005b93c0 _$E401
// FUNCTION: STBC 0x005b93d0 _$E400
// FUNCTION: STBC 0x005b93e0 _$E405
// FUNCTION: STBC 0x005b93f0 _$E404
// FUNCTION: STBC 0x005b9400 _$E409
// FUNCTION: STBC 0x005b9410 _$E408
// FUNCTION: STBC 0x005b9420 _$E413
// FUNCTION: STBC 0x005b9430 _$E412
// FUNCTION: STBC 0x005b9440 _$E417
// FUNCTION: STBC 0x005b9450 _$E416
// FUNCTION: STBC 0x005b9460 _$E421
// FUNCTION: STBC 0x005b9470 _$E420
// FUNCTION: STBC 0x005b9480 _$E425
// FUNCTION: STBC 0x005b9490 _$E424
// FUNCTION: STBC 0x005b94a0 _$E429
// FUNCTION: STBC 0x005b94b0 _$E428
// FUNCTION: STBC 0x005b94c0 _$E194
// FUNCTION: STBC 0x005b94d0 _$E193
// FUNCTION: STBC 0x005b94e0 _$E433
// FUNCTION: STBC 0x005b94f0 _$E432
// FUNCTION: STBC 0x005b9500 _$E199
// FUNCTION: STBC 0x005b9510 _$E198
// FUNCTION: STBC 0x005b9520 _$E204
// FUNCTION: STBC 0x005b9530 _$E203
// FUNCTION: STBC 0x005b9540 _$E437
// FUNCTION: STBC 0x005b9550 _$E436
// FUNCTION: STBC 0x005b9560 _$E442
// FUNCTION: STBC 0x005b9570 _$E441
// FUNCTION: STBC 0x005b9580 _$E446
// FUNCTION: STBC 0x005b9590 _$E445

/*
================
WeaponHitEvent::WeaponHitEvent
================
*/
// FUNCTION: STBC 0x005b8750
WeaponHitEvent::WeaponHitEvent( long id ) : TGEvent( id ) {
	firingObjectID = 0;
	objectHitNormal = NiPoint3::UNIT_X;
	objectHitPoint = NiPoint3::ZERO;
	weaponInstanceID = 0;
	radius = 1.0f;
	condition = 1.0f;
	damage = 0.0f;
	weaponType = 0;
	isHullHit = false;
	firingPlayerID = 0;
	SetEventType( ET_WEAPON_HIT );
}

/*
================
WeaponHitEvent::GetObjType
================
*/
// FUNCTION: STBC 0x005b87d0
int WeaponHitEvent::GetObjType( void ) {
	return WEAPONHITEVENT_TYPE;
}

/*
================
WeaponHitEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005b87e0
bool WeaponHitEvent::IsTypeOf( int type ) {
	if ( type == WEAPONHITEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
WeaponHitEvent::GetClassName
================
*/
// FUNCTION: STBC 0x005b8810
char *WeaponHitEvent::GetClassName( void ) {
	return "WeaponHitEvent";
}

/*
================
WeaponHitEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x005b8820
char *WeaponHitEvent::GetSwigTypeName( void ) {
	return "_p_WeaponHitEvent";
}

/*
================
WeaponHitEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x005b8830
char *WeaponHitEvent::GetPythonClassName( void ) {
	return "WeaponHitEventPtr";
}

/*
================
WeaponHitEvent::GetFiringObject

The object that fired, found among every object by its ID.
================
*/
// FUNCTION: STBC 0x005b8880
ObjectClass *WeaponHitEvent::GetFiringObject( void ) {
	return ObjectClass::GetObjectByID( NULL, firingObjectID );
}

/*
================
WeaponHitEvent::SetFiringObject
================
*/
// FUNCTION: STBC 0x005b8890
void WeaponHitEvent::SetFiringObject( ObjectClass *object ) {
	if ( object != NULL ) {
		firingObjectID = object->GetObjID();
	} else {
		firingObjectID = 0;
	}
}

/*
================
WeaponHitEvent::GetTargetObject

The event's destination when it is an object; the object hit.
================
*/
// FUNCTION: STBC 0x005b88b0
ObjectClass *WeaponHitEvent::GetTargetObject( void ) {
	if ( destination != NULL && destination->IsTypeOf( OBJECTCLASS_TYPE ) ) {
		return (ObjectClass *)destination;
	}
	return NULL;
}

/*
================
WeaponHitEvent::GetWorldHitPoint
================
*/
// FUNCTION: STBC 0x005b88e0
// PARKED: x87 operand order: retail adds the translate y and z first and x second (t.z + r.z, t.y + r.y, r.x + t.x); `R * p + t` gives all three as retail but x, `t + R * p` all but y and z; a named result copies
NiPoint3 WeaponHitEvent::GetWorldHitPoint( void ) {
	if ( destination != NULL && destination->IsTypeOf( OBJECTCLASS_TYPE ) ) {
		return TGModelUtils::LocalToWorldPoint( ( (ObjectClass *)destination )->GetNode(), objectHitPoint );
	}
	return objectHitPoint;
}

/*
================
WeaponHitEvent::GetWorldHitNormal
================
*/
// FUNCTION: STBC 0x005b8980
NiPoint3 WeaponHitEvent::GetWorldHitNormal( void ) {
	if ( destination != NULL && destination->IsTypeOf( OBJECTCLASS_TYPE ) ) {
		return ( (ObjectClass *)destination )->GetNode()->GetWorldRotate() * objectHitNormal;
	}
	return objectHitNormal;
}

/*
================
WeaponHitEvent::Copy

Copies the hit; the firing object and the weapon instance are not copied.
================
*/
// FUNCTION: STBC 0x005b89d0
bool WeaponHitEvent::Copy( TGEvent *event ) {
	if ( event->IsTypeOf( WEAPONHITEVENT_TYPE ) ) {
		WeaponHitEvent *other = (WeaponHitEvent *)event;
		objectHitNormal = other->objectHitNormal;
		objectHitPoint = other->objectHitPoint;
		radius = other->radius;
		condition = other->condition;
		damage = other->damage;
		weaponType = other->weaponType;
		isHullHit = other->isHullHit;
		firingPlayerID = other->firingPlayerID;
		return TGEvent::Copy( event );
	}
	return false;
}

/*
================
WeaponHitEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x005b8a50
bool WeaponHitEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( firingObjectID );
	stream->WriteFloat( objectHitNormal.x );
	stream->WriteFloat( objectHitNormal.y );
	stream->WriteFloat( objectHitNormal.z );
	stream->WriteFloat( objectHitPoint.x );
	stream->WriteFloat( objectHitPoint.y );
	stream->WriteFloat( objectHitPoint.z );
	stream->WriteID( weaponInstanceID );
	stream->WriteFloat( radius );
	stream->WriteFloat( condition );
	stream->WriteFloat( damage );
	stream->WriteInt( weaponType );
	stream->WriteBool( isHullHit );
	stream->WriteLong( firingPlayerID );
	return true;
}

/*
================
WeaponHitEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x005b8b10
bool WeaponHitEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	firingObjectID = stream->ReadID();
	objectHitNormal.x = stream->ReadFloat();
	objectHitNormal.y = stream->ReadFloat();
	objectHitNormal.z = stream->ReadFloat();
	objectHitPoint.x = stream->ReadFloat();
	objectHitPoint.y = stream->ReadFloat();
	objectHitPoint.z = stream->ReadFloat();
	weaponInstanceID = stream->ReadID();
	radius = stream->ReadFloat();
	condition = stream->ReadFloat();
	damage = stream->ReadFloat();
	weaponType = stream->ReadInt();
	isHullHit = stream->ReadBool();
	// written with WriteLong, read back with ReadID
	firingPlayerID = stream->ReadID();
	return true;
}
