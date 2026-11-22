/*
===============================================================================

	StartFiringEvent.cpp

===============================================================================
*/

#include <iostream>
#include "StartFiringEvent.h"
#include "TGStream.h"
#include "ObjectClass.h"
#include "ShipClass.h"
#include "HullClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005b7d80 _$E32
// FUNCTION: STBC 0x005b7d90 _$E29
// FUNCTION: STBC 0x005b7da0 _$E31
// FUNCTION: STBC 0x005b7db0 _$E30
// FUNCTION: STBC 0x005b7dc0 _$E38
// FUNCTION: STBC 0x005b7dd0 _$E35
// FUNCTION: STBC 0x005b7de0 _$E37
// FUNCTION: STBC 0x005b7df0 _$E36

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005b8440 _$E459
// FUNCTION: STBC 0x005b8460 _$E458

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005b7e00 _$E69
// FUNCTION: STBC 0x005b7e10 _$E68
// FUNCTION: STBC 0x005b7e20 _$E275
// FUNCTION: STBC 0x005b7e30 _$E274
// FUNCTION: STBC 0x005b7e40 _$E80
// FUNCTION: STBC 0x005b7e50 _$E79
// FUNCTION: STBC 0x005b7e60 _$E84
// FUNCTION: STBC 0x005b7e70 _$E83
// FUNCTION: STBC 0x005b7e80 _$E88
// FUNCTION: STBC 0x005b7e90 _$E87
// FUNCTION: STBC 0x005b7ea0 _$E92
// FUNCTION: STBC 0x005b7eb0 _$E91
// FUNCTION: STBC 0x005b7ec0 _$E209
// FUNCTION: STBC 0x005b7ed0 _$E208
// FUNCTION: STBC 0x005b7ee0 _$E151
// FUNCTION: STBC 0x005b7ef0 _$E150
// FUNCTION: STBC 0x005b7f00 _$E164
// FUNCTION: STBC 0x005b7f10 _$E163
// FUNCTION: STBC 0x005b7f20 _$E214
// FUNCTION: STBC 0x005b7f30 _$E213
// FUNCTION: STBC 0x005b7f40 _$E280
// FUNCTION: STBC 0x005b7f50 _$E279
// FUNCTION: STBC 0x005b7f60 _$E218
// FUNCTION: STBC 0x005b7f70 _$E217
// FUNCTION: STBC 0x005b7f80 _$E284
// FUNCTION: STBC 0x005b7f90 _$E283
// FUNCTION: STBC 0x005b7fa0 _$E291
// FUNCTION: STBC 0x005b7fb0 _$E290
// FUNCTION: STBC 0x005b7fc0 _$E301
// FUNCTION: STBC 0x005b7fd0 _$E300
// FUNCTION: STBC 0x005b7fe0 _$E305
// FUNCTION: STBC 0x005b7ff0 _$E304
// FUNCTION: STBC 0x005b8000 _$E109
// FUNCTION: STBC 0x005b8010 _$E108
// FUNCTION: STBC 0x005b8020 _$E237
// FUNCTION: STBC 0x005b8030 _$E236
// FUNCTION: STBC 0x005b8040 _$E244
// FUNCTION: STBC 0x005b8050 _$E243
// FUNCTION: STBC 0x005b8060 _$E248
// FUNCTION: STBC 0x005b8070 _$E247
// FUNCTION: STBC 0x005b8080 _$E252
// FUNCTION: STBC 0x005b8090 _$E251
// FUNCTION: STBC 0x005b80a0 _$E256
// FUNCTION: STBC 0x005b80b0 _$E255
// FUNCTION: STBC 0x005b80c0 _$E262
// FUNCTION: STBC 0x005b80d0 _$E261
// FUNCTION: STBC 0x005b80e0 _$E309
// FUNCTION: STBC 0x005b80f0 _$E308
// FUNCTION: STBC 0x005b8100 _$E313
// FUNCTION: STBC 0x005b8110 _$E312
// FUNCTION: STBC 0x005b8120 _$E317
// FUNCTION: STBC 0x005b8130 _$E316
// FUNCTION: STBC 0x005b8140 _$E321
// FUNCTION: STBC 0x005b8150 _$E320
// FUNCTION: STBC 0x005b8160 _$E325
// FUNCTION: STBC 0x005b8170 _$E324
// FUNCTION: STBC 0x005b8180 _$E329
// FUNCTION: STBC 0x005b8190 _$E328
// FUNCTION: STBC 0x005b81a0 _$E333
// FUNCTION: STBC 0x005b81b0 _$E332
// FUNCTION: STBC 0x005b81c0 _$E337
// FUNCTION: STBC 0x005b81d0 _$E336
// FUNCTION: STBC 0x005b81e0 _$E341
// FUNCTION: STBC 0x005b81f0 _$E340
// FUNCTION: STBC 0x005b8200 _$E345
// FUNCTION: STBC 0x005b8210 _$E344
// FUNCTION: STBC 0x005b8220 _$E114
// FUNCTION: STBC 0x005b8230 _$E113
// FUNCTION: STBC 0x005b8240 _$E349
// FUNCTION: STBC 0x005b8250 _$E348
// FUNCTION: STBC 0x005b8260 _$E119
// FUNCTION: STBC 0x005b8270 _$E118
// FUNCTION: STBC 0x005b8280 _$E353
// FUNCTION: STBC 0x005b8290 _$E352
// FUNCTION: STBC 0x005b82a0 _$E357
// FUNCTION: STBC 0x005b82b0 _$E356
// FUNCTION: STBC 0x005b82c0 _$E361
// FUNCTION: STBC 0x005b82d0 _$E360
// FUNCTION: STBC 0x005b82e0 _$E365
// FUNCTION: STBC 0x005b82f0 _$E364
// FUNCTION: STBC 0x005b8300 _$E128
// FUNCTION: STBC 0x005b8310 _$E127
// FUNCTION: STBC 0x005b8320 _$E369
// FUNCTION: STBC 0x005b8330 _$E368
// FUNCTION: STBC 0x005b8340 _$E373
// FUNCTION: STBC 0x005b8350 _$E372
// FUNCTION: STBC 0x005b8360 _$E377
// FUNCTION: STBC 0x005b8370 _$E376
// FUNCTION: STBC 0x005b8380 _$E134
// FUNCTION: STBC 0x005b8390 _$E133
// FUNCTION: STBC 0x005b83a0 _$E139
// FUNCTION: STBC 0x005b83b0 _$E138
// FUNCTION: STBC 0x005b83c0 _$E169
// FUNCTION: STBC 0x005b83d0 _$E168
// FUNCTION: STBC 0x005b83e0 _$E381
// FUNCTION: STBC 0x005b83f0 _$E380
// FUNCTION: STBC 0x005b8400 _$E174
// FUNCTION: STBC 0x005b8410 _$E173
// FUNCTION: STBC 0x005b8420 _$E385
// FUNCTION: STBC 0x005b8430 _$E384
// FUNCTION: STBC 0x005b8470 _$E389
// FUNCTION: STBC 0x005b8480 _$E388
// FUNCTION: STBC 0x005b8490 _$E179
// FUNCTION: STBC 0x005b84a0 _$E178
// FUNCTION: STBC 0x005b84b0 _$E393
// FUNCTION: STBC 0x005b84c0 _$E392
// FUNCTION: STBC 0x005b84d0 _$E144
// FUNCTION: STBC 0x005b84e0 _$E143
// FUNCTION: STBC 0x005b84f0 _$E397
// FUNCTION: STBC 0x005b8500 _$E396
// FUNCTION: STBC 0x005b8510 _$E401
// FUNCTION: STBC 0x005b8520 _$E400
// FUNCTION: STBC 0x005b8530 _$E184
// FUNCTION: STBC 0x005b8540 _$E183
// FUNCTION: STBC 0x005b8550 _$E189
// FUNCTION: STBC 0x005b8560 _$E188
// FUNCTION: STBC 0x005b8570 _$E405
// FUNCTION: STBC 0x005b8580 _$E404
// FUNCTION: STBC 0x005b8590 _$E409
// FUNCTION: STBC 0x005b85a0 _$E408
// FUNCTION: STBC 0x005b85b0 _$E413
// FUNCTION: STBC 0x005b85c0 _$E412
// FUNCTION: STBC 0x005b85d0 _$E417
// FUNCTION: STBC 0x005b85e0 _$E416
// FUNCTION: STBC 0x005b85f0 _$E421
// FUNCTION: STBC 0x005b8600 _$E420
// FUNCTION: STBC 0x005b8610 _$E425
// FUNCTION: STBC 0x005b8620 _$E424
// FUNCTION: STBC 0x005b8630 _$E429
// FUNCTION: STBC 0x005b8640 _$E428
// FUNCTION: STBC 0x005b8650 _$E433
// FUNCTION: STBC 0x005b8660 _$E432
// FUNCTION: STBC 0x005b8670 _$E194
// FUNCTION: STBC 0x005b8680 _$E193
// FUNCTION: STBC 0x005b8690 _$E437
// FUNCTION: STBC 0x005b86a0 _$E436
// FUNCTION: STBC 0x005b86b0 _$E199
// FUNCTION: STBC 0x005b86c0 _$E198
// FUNCTION: STBC 0x005b86d0 _$E204
// FUNCTION: STBC 0x005b86e0 _$E203
// FUNCTION: STBC 0x005b86f0 _$E441
// FUNCTION: STBC 0x005b8700 _$E440
// FUNCTION: STBC 0x005b8710 _$E446
// FUNCTION: STBC 0x005b8720 _$E445
// FUNCTION: STBC 0x005b8730 _$E450
// FUNCTION: STBC 0x005b8740 _$E449

/*
================
StartFiringEvent::GetTarget

The object to fire at, found by its ID.
================
*/
// FUNCTION: STBC 0x005b7b20
ObjectClass *StartFiringEvent::GetTarget( void ) {
	return (ObjectClass *)TGObject::GetTGObjectPtr( targetID );
}

/*
================
StartFiringEvent::SetTarget

Takes the target by its ID; no target is ID 0 (invented name).
================
*/
// FUNCTION: STBC 0x005b7b30
void StartFiringEvent::SetTarget( ObjectClass *target ) {
	if ( target != NULL ) {
		targetID = target->GetObjID();
	} else {
		targetID = 0;
	}
}

/*
================
StartFiringEvent::WriteToNetStream

The offset goes packed relative to the target's bound radius (its own
length when the target is gone).
================
*/
// FUNCTION: STBC 0x005b7b50
bool StartFiringEvent::WriteToNetStream( TGStream *stream ) {
	// MATCH-PENDING: R17, exact only through the invented GetTargetID (StartFiringEvent.h), the inline level on the ID that Torpedo's GetTargetID gives its twin; no evidence shows it here (user's ruling, 2026-11-18)
	TGEvent::WriteToNetStream( stream );
	stream->WriteID( targetID );
	float scale;
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, GetTargetID() );
	if ( target != NULL ) {
		scale = target->GetRadius();
	} else {
		scale = offset.Length();
	}
	stream->VirtualSlot38( offset.x, offset.y, offset.z, scale );
	return true;
}

/*
================
StartFiringEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x005b7be0
bool StartFiringEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	targetID = stream->ReadID();
	float scale;
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, targetID );
	if ( target != NULL ) {
		scale = target->GetRadius();
		if ( scale <= 0.0f ) {
			scale = 0.01f;
		}
	} else {
		scale = 1.0f;
	}
	stream->VirtualSlot39( &offset.x, &offset.y, &offset.z, scale );
	return true;
}

/*
================
StartFiringEvent::WriteToStream

The base's result is not checked.
================
*/
// FUNCTION: STBC 0x005b7c60
bool StartFiringEvent::WriteToStream( TGStream *stream ) {
	TGEvent::WriteToStream( stream );
	stream->WriteID( targetID );
	stream->WriteFloat( offset.x );
	stream->WriteFloat( offset.y );
	stream->WriteFloat( offset.z );
	return true;
}

/*
================
StartFiringEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x005b7cb0
bool StartFiringEvent::ReadFromStream( TGStream *stream ) {
	TGEvent::ReadFromStream( stream );
	targetID = stream->ReadID();
	offset.x = stream->ReadFloat();
	offset.y = stream->ReadFloat();
	offset.z = stream->ReadFloat();
	return true;
}

/*
================
StartFiringEvent::SetOffset

Aims at the offset; with none, at the target's hull's damage point, else at
its center (invented name).
================
*/
// FUNCTION: STBC 0x005b7cf0
void StartFiringEvent::SetOffset( NiPoint3 *offset_ ) {
	if ( offset_ != NULL ) {
		offset = *offset_;
		return;
	}
	offset = NiPoint3::ZERO;
	ShipClass *ship = ShipClass::GetObjectByID( NULL, targetID );
	if ( ship != NULL ) {
		HullClass *hull = ship->GetHull();
		if ( hull != NULL ) {
			offset = hull->GetDamagePoint();
		}
	}
}
