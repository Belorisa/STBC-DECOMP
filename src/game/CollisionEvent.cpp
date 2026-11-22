/*
===============================================================================

	CollisionEvent.cpp

===============================================================================
*/

#include <iostream>
#include "CollisionEvent.h"
#include "ObjectClass.h"
#include "TGStream.h"
#include "NiMatrix3.h"
#include "TGModelUtils.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00587ac0 _$E442
// FUNCTION: STBC 0x00587ae0 _$E441

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00587570 _$E32
// FUNCTION: STBC 0x00587580 _$E29
// FUNCTION: STBC 0x00587590 _$E31
// FUNCTION: STBC 0x005875a0 _$E30
// FUNCTION: STBC 0x005875b0 _$E38
// FUNCTION: STBC 0x005875c0 _$E35
// FUNCTION: STBC 0x005875d0 _$E37
// FUNCTION: STBC 0x005875e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005875f0 _$E69
// FUNCTION: STBC 0x00587600 _$E68
// FUNCTION: STBC 0x00587610 _$E226
// FUNCTION: STBC 0x00587620 _$E225
// FUNCTION: STBC 0x00587630 _$E80
// FUNCTION: STBC 0x00587640 _$E79
// FUNCTION: STBC 0x00587650 _$E84
// FUNCTION: STBC 0x00587660 _$E83
// FUNCTION: STBC 0x00587670 _$E88
// FUNCTION: STBC 0x00587680 _$E87
// FUNCTION: STBC 0x00587690 _$E92
// FUNCTION: STBC 0x005876a0 _$E91
// FUNCTION: STBC 0x005876b0 _$E209
// FUNCTION: STBC 0x005876c0 _$E208
// FUNCTION: STBC 0x005876d0 _$E151
// FUNCTION: STBC 0x005876e0 _$E150
// FUNCTION: STBC 0x005876f0 _$E164
// FUNCTION: STBC 0x00587700 _$E163
// FUNCTION: STBC 0x00587710 _$E214
// FUNCTION: STBC 0x00587720 _$E213
// FUNCTION: STBC 0x00587730 _$E231
// FUNCTION: STBC 0x00587740 _$E230
// FUNCTION: STBC 0x00587750 _$E218
// FUNCTION: STBC 0x00587760 _$E217
// FUNCTION: STBC 0x00587770 _$E235
// FUNCTION: STBC 0x00587780 _$E234
// FUNCTION: STBC 0x00587790 _$E242
// FUNCTION: STBC 0x005877a0 _$E241
// FUNCTION: STBC 0x005877b0 _$E267
// FUNCTION: STBC 0x005877c0 _$E266
// FUNCTION: STBC 0x005877d0 _$E271
// FUNCTION: STBC 0x005877e0 _$E270
// FUNCTION: STBC 0x005877f0 _$E109
// FUNCTION: STBC 0x00587800 _$E108
// FUNCTION: STBC 0x00587810 _$E253
// FUNCTION: STBC 0x00587820 _$E252
// FUNCTION: STBC 0x00587830 _$E260
// FUNCTION: STBC 0x00587840 _$E259
// FUNCTION: STBC 0x00587880 _$E283
// FUNCTION: STBC 0x00587890 _$E282
// FUNCTION: STBC 0x005878a0 _$E287
// FUNCTION: STBC 0x005878b0 _$E286
// FUNCTION: STBC 0x005878c0 _$E327
// FUNCTION: STBC 0x005878d0 _$E326
// FUNCTION: STBC 0x005878e0 _$E333
// FUNCTION: STBC 0x005878f0 _$E332
// FUNCTION: STBC 0x00587900 _$E275
// FUNCTION: STBC 0x00587910 _$E274
// FUNCTION: STBC 0x00587920 _$E279
// FUNCTION: STBC 0x00587930 _$E278
// FUNCTION: STBC 0x00587940 _$E291
// FUNCTION: STBC 0x00587950 _$E290
// FUNCTION: STBC 0x00587960 _$E295
// FUNCTION: STBC 0x00587970 _$E294
// FUNCTION: STBC 0x00587980 _$E299
// FUNCTION: STBC 0x00587990 _$E298
// FUNCTION: STBC 0x005879a0 _$E303
// FUNCTION: STBC 0x005879b0 _$E302
// FUNCTION: STBC 0x005879c0 _$E307
// FUNCTION: STBC 0x005879d0 _$E306
// FUNCTION: STBC 0x005879e0 _$E311
// FUNCTION: STBC 0x005879f0 _$E310
// FUNCTION: STBC 0x00587a00 _$E315
// FUNCTION: STBC 0x00587a10 _$E314
// FUNCTION: STBC 0x00587a20 _$E319
// FUNCTION: STBC 0x00587a30 _$E318
// FUNCTION: STBC 0x00587a40 _$E114
// FUNCTION: STBC 0x00587a50 _$E113
// FUNCTION: STBC 0x00587a60 _$E323
// FUNCTION: STBC 0x00587a70 _$E322
// FUNCTION: STBC 0x00587a80 _$E119
// FUNCTION: STBC 0x00587a90 _$E118
// FUNCTION: STBC 0x00587aa0 _$E337
// FUNCTION: STBC 0x00587ab0 _$E336
// FUNCTION: STBC 0x00587af0 _$E341
// FUNCTION: STBC 0x00587b00 _$E340
// FUNCTION: STBC 0x00587b10 _$E345
// FUNCTION: STBC 0x00587b20 _$E344
// FUNCTION: STBC 0x00587b30 _$E349
// FUNCTION: STBC 0x00587b40 _$E348
// FUNCTION: STBC 0x00587b50 _$E128
// FUNCTION: STBC 0x00587b60 _$E127
// FUNCTION: STBC 0x00587b70 _$E353
// FUNCTION: STBC 0x00587b80 _$E352
// FUNCTION: STBC 0x00587b90 _$E357
// FUNCTION: STBC 0x00587ba0 _$E356
// FUNCTION: STBC 0x00587bb0 _$E361
// FUNCTION: STBC 0x00587bc0 _$E360
// FUNCTION: STBC 0x00587bd0 _$E134
// FUNCTION: STBC 0x00587be0 _$E133
// FUNCTION: STBC 0x00587bf0 _$E139
// FUNCTION: STBC 0x00587c00 _$E138
// FUNCTION: STBC 0x00587c10 _$E169
// FUNCTION: STBC 0x00587c20 _$E168
// FUNCTION: STBC 0x00587c30 _$E365
// FUNCTION: STBC 0x00587c40 _$E364
// FUNCTION: STBC 0x00587c50 _$E174
// FUNCTION: STBC 0x00587c60 _$E173
// FUNCTION: STBC 0x00587c70 _$E369
// FUNCTION: STBC 0x00587c80 _$E368
// FUNCTION: STBC 0x00587c90 _$E373
// FUNCTION: STBC 0x00587ca0 _$E372
// FUNCTION: STBC 0x00587cb0 _$E179
// FUNCTION: STBC 0x00587cc0 _$E178
// FUNCTION: STBC 0x00587cd0 _$E377
// FUNCTION: STBC 0x00587ce0 _$E376
// FUNCTION: STBC 0x00587cf0 _$E144
// FUNCTION: STBC 0x00587d00 _$E143
// FUNCTION: STBC 0x00587d10 _$E381
// FUNCTION: STBC 0x00587d20 _$E380
// FUNCTION: STBC 0x00587d30 _$E385
// FUNCTION: STBC 0x00587d40 _$E384
// FUNCTION: STBC 0x00587d50 _$E184
// FUNCTION: STBC 0x00587d60 _$E183
// FUNCTION: STBC 0x00587d70 _$E189
// FUNCTION: STBC 0x00587d80 _$E188
// FUNCTION: STBC 0x00587d90 _$E389
// FUNCTION: STBC 0x00587da0 _$E388
// FUNCTION: STBC 0x00587db0 _$E393
// FUNCTION: STBC 0x00587dc0 _$E392
// FUNCTION: STBC 0x00587dd0 _$E397
// FUNCTION: STBC 0x00587de0 _$E396
// FUNCTION: STBC 0x00587df0 _$E401
// FUNCTION: STBC 0x00587e00 _$E400
// FUNCTION: STBC 0x00587e10 _$E405
// FUNCTION: STBC 0x00587e20 _$E404
// FUNCTION: STBC 0x00587e30 _$E409
// FUNCTION: STBC 0x00587e40 _$E408
// FUNCTION: STBC 0x00587e50 _$E413
// FUNCTION: STBC 0x00587e60 _$E412
// FUNCTION: STBC 0x00587e70 _$E417
// FUNCTION: STBC 0x00587e80 _$E416
// FUNCTION: STBC 0x00587e90 _$E194
// FUNCTION: STBC 0x00587ea0 _$E193
// FUNCTION: STBC 0x00587eb0 _$E421
// FUNCTION: STBC 0x00587ec0 _$E420
// FUNCTION: STBC 0x00587ed0 _$E199
// FUNCTION: STBC 0x00587ee0 _$E198
// FUNCTION: STBC 0x00587ef0 _$E204
// FUNCTION: STBC 0x00587f00 _$E203
// FUNCTION: STBC 0x00587f10 _$E425
// FUNCTION: STBC 0x00587f20 _$E424
// FUNCTION: STBC 0x00587f30 _$E430
// FUNCTION: STBC 0x00587f40 _$E429
// FUNCTION: STBC 0x00587f50 _$E434
// FUNCTION: STBC 0x00587f60 _$E433

/*
================
CollisionEvent::CollisionEvent
================
*/
// FUNCTION: STBC 0x00586d00
CollisionEvent::CollisionEvent( long id ) :
	TGEvent( id ),
	collisionForce( 0.0f ) {
}

/*
================
CollisionEvent::GetObjType
================
*/
// FUNCTION: STBC 0x00586d80
int CollisionEvent::GetObjType( void ) {
	return COLLISIONEVENT_TYPE;
}

/*
================
CollisionEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00586d90
bool CollisionEvent::IsTypeOf( int type ) {
	if ( type == COLLISIONEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
CollisionEvent::GetClassName
================
*/
// FUNCTION: STBC 0x00586dc0
char *CollisionEvent::GetClassName( void ) {
	return "CollisionEvent";
}

/*
================
CollisionEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00586dd0
char *CollisionEvent::GetSwigTypeName( void ) {
	return "_p_CollisionEvent";
}

/*
================
CollisionEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00586de0
char *CollisionEvent::GetPythonClassName( void ) {
	return "CollisionEventPtr";
}

/*
================
CollisionEvent::~CollisionEvent

Frees the points of contact.
================
*/
// FUNCTION: STBC 0x00586e20
CollisionEvent::~CollisionEvent( void ) {
	for ( unsigned int i = 0; i < points.GetSize(); i++ ) {
		delete points.GetAt( i );
	}
}

/*
================
CollisionEvent::Copy

Copies the event's state and adds copies of the other event's points of
contact; the other event's array is taken to have no holes.
================
*/
// FUNCTION: STBC 0x00586e70
bool CollisionEvent::Copy( TGEvent *event ) {
	bool result = TGEvent::Copy( event );
	CollisionEvent *other = (CollisionEvent *)event;
	for ( unsigned int i = 0; i < other->points.GetSize(); i++ ) {
		points.Add( new NiPoint3( *other->points.GetAt( i ) ) );
	}
	return result;
}

/*
================
CollisionEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x00586fb0
bool CollisionEvent::WriteToStream( TGStream *stream ) {
	bool result = TGEvent::WriteToStream( stream );
	if ( result ) {
		stream->WriteInt( points.GetEffectiveSize() );
		for ( unsigned int i = 0; i < points.GetSize(); i++ ) {
			NiPoint3 *point = points.GetAt( i );
			if ( point != NULL ) {
				stream->WriteFloat( point->x );
				stream->WriteFloat( point->y );
				stream->WriteFloat( point->z );
			}
		}
		stream->WriteFloat( collisionForce );
	}
	return result;
}

/*
================
CollisionEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x00587030
bool CollisionEvent::ReadFromStream( TGStream *stream ) {
	bool result = TGEvent::ReadFromStream( stream );
	if ( result ) {
		int count = stream->ReadInt();
		for ( int i = count - 1; i >= 0; i-- ) {
			float x = stream->ReadFloat();
			float y = stream->ReadFloat();
			float z = stream->ReadFloat();
			points.Add( new NiPoint3( x, y, z ) );
		}
		collisionForce = stream->ReadFloat();
	}
	return result;
}

/*
================
CollisionEvent::WriteToNetStream

The points of contact go out in the destination's model space, packed
against its radius; with no destination they are packed against their own
length.
================
*/
// FUNCTION: STBC 0x005871a0
bool CollisionEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteChar( (char)points.GetEffectiveSize() );
	for ( unsigned int i = 0; i < points.GetSize(); i++ ) {
		NiPoint3 *point = points.GetAt( i );
		if ( point != NULL ) {
			NiPoint3 position = *point;
			float radius;
			ObjectClass *object = (ObjectClass *)destination;
			if ( object != NULL ) {
				NiNode *node = object->GetNode();
				position = TGModelUtils::WorldToLocalPoint( node, position );
				radius = object->GetRadius();
			} else {
				radius = position.Length();
			}
			stream->VirtualSlot38( position.x, position.y, position.z, radius );
		}
	}
	stream->WriteFloat( collisionForce );
	return true;
}

/*
================
CollisionEvent::ReadFromNetStream

Reads the points back in the destination's model space; PointsToWorldSpace
moves them into world space once the destination is known.
================
*/
// FUNCTION: STBC 0x00587300
bool CollisionEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	float radius;
	ObjectClass *object = ObjectClass::Cast( TGObject::GetTGObjectPtr( (long)destination ) );
	if ( object != NULL ) {
		radius = object->GetRadius();
		if ( radius <= 0.0f ) {
			radius = 0.01f;
		}
	} else {
		radius = 1.0f;
	}
	int count = stream->ReadChar();
	for ( int i = count - 1; i >= 0; i-- ) {
		float x, y, z;
		stream->VirtualSlot39( &x, &y, &z, radius );
		points.Add( new NiPoint3( x, y, z ) );
	}
	collisionForce = stream->ReadFloat();
	return true;
}

/*
================
CollisionEvent::PointsToWorldSpace
================
*/
// FUNCTION: STBC 0x005874a0
bool CollisionEvent::PointsToWorldSpace( void ) {
	if ( destination != INVALID_DESTINATION ) {
		ObjectClass *object = ObjectClass::Cast( destination );
		if ( object != NULL ) {
			for ( unsigned int i = 0; i < points.GetSize(); i++ ) {
				NiPoint3 *point = points.GetAt( i );
				NiNode *node = object->GetNode();
				*point = node->GetWorldRotate() * ( node->GetWorldScale() * *point ) + node->GetWorldTranslate();
			}
		}
	}
	return true;
}
