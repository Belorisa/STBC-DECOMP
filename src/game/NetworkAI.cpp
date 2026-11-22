/*
===============================================================================

	NetworkAI.cpp

===============================================================================
*/

#include <iostream>
#include "NetworkAI.h"
#include "ShipClass.h"
#include "TGStream.h"
#include "Torpedo.h"
#include "UtopiaModule.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0047d0e0 _$E32
// FUNCTION: STBC 0x0047d0f0 _$E29
// FUNCTION: STBC 0x0047d100 _$E31
// FUNCTION: STBC 0x0047d110 _$E30
// FUNCTION: STBC 0x0047d120 _$E38
// FUNCTION: STBC 0x0047d130 _$E35
// FUNCTION: STBC 0x0047d140 _$E37
// FUNCTION: STBC 0x0047d150 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047d160 _$E69
// FUNCTION: STBC 0x0047d170 _$E68
// FUNCTION: STBC 0x0047d180 _$E275
// FUNCTION: STBC 0x0047d190 _$E274
// FUNCTION: STBC 0x0047d1a0 _$E80
// FUNCTION: STBC 0x0047d1b0 _$E79
// FUNCTION: STBC 0x0047d1c0 _$E84
// FUNCTION: STBC 0x0047d1d0 _$E83
// FUNCTION: STBC 0x0047d1e0 _$E88
// FUNCTION: STBC 0x0047d1f0 _$E87
// FUNCTION: STBC 0x0047d200 _$E92
// FUNCTION: STBC 0x0047d210 _$E91
// FUNCTION: STBC 0x0047d220 _$E209
// FUNCTION: STBC 0x0047d230 _$E208
// FUNCTION: STBC 0x0047d240 _$E151
// FUNCTION: STBC 0x0047d250 _$E150
// FUNCTION: STBC 0x0047d260 _$E164
// FUNCTION: STBC 0x0047d270 _$E163
// FUNCTION: STBC 0x0047d2a0 _$E280
// FUNCTION: STBC 0x0047d2b0 _$E279
// FUNCTION: STBC 0x0047d2c0 _$E218
// FUNCTION: STBC 0x0047d2d0 _$E217
// FUNCTION: STBC 0x0047d2e0 _$E284
// FUNCTION: STBC 0x0047d2f0 _$E283
// FUNCTION: STBC 0x0047d300 _$E291
// FUNCTION: STBC 0x0047d310 _$E290
// FUNCTION: STBC 0x0047d360 _$E109
// FUNCTION: STBC 0x0047d370 _$E108
// FUNCTION: STBC 0x0047d380 _$E237
// FUNCTION: STBC 0x0047d390 _$E236
// FUNCTION: STBC 0x0047d3a0 _$E244
// FUNCTION: STBC 0x0047d3b0 _$E243
// FUNCTION: STBC 0x0047d3c0 _$E248
// FUNCTION: STBC 0x0047d3d0 _$E247
// FUNCTION: STBC 0x0047d3e0 _$E252
// FUNCTION: STBC 0x0047d3f0 _$E251
// FUNCTION: STBC 0x0047d400 _$E256
// FUNCTION: STBC 0x0047d410 _$E255
// FUNCTION: STBC 0x0047d420 _$E262
// FUNCTION: STBC 0x0047d430 _$E261
// FUNCTION: STBC 0x0047d5b0 _$E114
// FUNCTION: STBC 0x0047d5c0 _$E113
// FUNCTION: STBC 0x0047d5f0 _$E119
// FUNCTION: STBC 0x0047d600 _$E118
// FUNCTION: STBC 0x0047d690 _$E128
// FUNCTION: STBC 0x0047d6a0 _$E127
// FUNCTION: STBC 0x0047d710 _$E134
// FUNCTION: STBC 0x0047d720 _$E133
// FUNCTION: STBC 0x0047d730 _$E139
// FUNCTION: STBC 0x0047d740 _$E138
// FUNCTION: STBC 0x0047d750 _$E169
// FUNCTION: STBC 0x0047d760 _$E168
// FUNCTION: STBC 0x0047d790 _$E174
// FUNCTION: STBC 0x0047d7a0 _$E173
// FUNCTION: STBC 0x0047d7f0 _$E179
// FUNCTION: STBC 0x0047d800 _$E178
// FUNCTION: STBC 0x0047d830 _$E144
// FUNCTION: STBC 0x0047d840 _$E143
// FUNCTION: STBC 0x0047d890 _$E184
// FUNCTION: STBC 0x0047d8a0 _$E183
// FUNCTION: STBC 0x0047d8b0 _$E189
// FUNCTION: STBC 0x0047d8c0 _$E188
// FUNCTION: STBC 0x0047d9d0 _$E194
// FUNCTION: STBC 0x0047d9e0 _$E193
// FUNCTION: STBC 0x0047da10 _$E199
// FUNCTION: STBC 0x0047da20 _$E198
// FUNCTION: STBC 0x0047da30 _$E204
// FUNCTION: STBC 0x0047da40 _$E203
// FUNCTION: STBC 0x0047da50 _$E303
// FUNCTION: STBC 0x0047da60 _$E302
// FUNCTION: STBC 0x0047da70 _$E308
// FUNCTION: STBC 0x0047da80 _$E307
// FUNCTION: STBC 0x0047da90 _$E312
// FUNCTION: STBC 0x0047daa0 _$E311

// How far ahead the object looks while it turns: the target is where the
// reported motion takes it in this many seconds (the names are invented).
#define LOOK_AHEAD_TIME		5.0f
#define LOOK_BEHIND_TIME	-5.0f

/*
================
NetworkAI::NetworkAI

A torpedo has no orientation worth reporting: its direction is its
velocity's, and any vector perpendicular to that serves as its up.
================
*/
// FUNCTION: STBC 0x0047dab0 ??0NetworkAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
NetworkAI::NetworkAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ) {
	lastUpdateTime = 0.0f;
	stateTime = 0.0f;
	position = NiPoint3::ZERO;
	direction = NiPoint3::ZERO;
	up = NiPoint3::ZERO;
	speed = 0.0f;
	if ( object != NULL ) {
		if ( object->IsTypeOf( TORPEDO_TYPE ) ) {
			position = object->GetWorldLocation();
			direction = object->GetVelocity();
			speed = direction.Unitize();
			up = direction.Perpendicular();
		} else {
			position = object->GetWorldLocation();
			direction = object->GetWorldForward();
			up = object->GetWorldUp();
			const NiPoint3 &velocity = object->GetVelocity();
			speed = (float)sqrt( velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z );
		}
		stateTime = g_kUtopiaModule.GetGameTime();
	}
}

/*
================
NetworkAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x0047dd20
void NetworkAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais ) {
	ais.AddTail( this );
}

/*
================
NetworkAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x0047dd90
void NetworkAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais ) {
	if ( focus ) {
		ais.AddTail( this );
	}
}

/*
================
NetworkAI::GetAIType
================
*/
// FUNCTION: STBC 0x0047de00
int NetworkAI::GetAIType( void ) {
	return AI_TYPE_NETWORK;
}

/*
================
NetworkAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x0047de10
bool NetworkAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_NETWORK ? true : false;
}

/*
================
NetworkAI::~NetworkAI
================
*/
// FUNCTION: STBC 0x0047de40
// FUNCTION: STBC 0x0047de20 ??_GNetworkAI@@UAEPAXI@Z
NetworkAI::~NetworkAI( void ) {
}

/*
================
NetworkAI::Cast
================
*/
// FUNCTION: STBC 0x0047de50
NetworkAI *NetworkAI::Cast( ArtificialIntelligence *ai ) {
	if ( ai != NULL && ai->GetAIType() == AI_TYPE_NETWORK ) {
		return static_cast<NetworkAI *>( ai );
	}
	return NULL;
}

/*
================
NetworkAI::Update

Carries the reported position along the reported motion, drives the
object at the reported speed, pulls it toward the position and turns it
toward a point ahead of it.
================
*/
// FUNCTION: STBC 0x0047de70
int NetworkAI::Update( double time ) {
	float now = g_kUtopiaModule.GetGameTime();
	if ( now == lastUpdateTime ) {
		return US_ACTIVE;
	}
	float elapsed = now - lastUpdateTime;

	PhysicsObjectClass *object = GetObject();
	if ( object == NULL ) {
		return US_DONE;
	}

	position += direction * ( elapsed * speed );
	if ( speed <= 0.5f ) {
		object->AlignToVectors( direction, up );
		object->SetAngularVelocity( &NiPoint3::ZERO, 0 );
		object->SetAngularAcceleration( &NiPoint3::ZERO );
	}

	ShipClass *ship = ShipClass::Cast( object );
	if ( ship != NULL ) {
		if ( speed < 0.0f ) {
			ship->SetSpeed( -speed, -TGPoint3::GetModelForward(), 0 );
		} else {
			ship->SetSpeed( speed, TGPoint3::GetModelForward(), 0 );
		}
	} else {
		NiPoint3 velocity = direction * speed;
		object->SetVelocity( &velocity );
	}

	PullTowardPosition( object, elapsed );

	NiPoint3 target;
	if ( speed < 0.0f ) {
		target = position + direction * speed * LOOK_BEHIND_TIME;
	} else {
		target = position + direction * speed * LOOK_AHEAD_TIME;
	}
	TurnTowardTarget( object, target );

	lastUpdateTime = now;
	return US_ACTIVE;
}

/*
================
NetworkAI::PullTowardPosition

Within half a unit of a stopped object's reported position the object
is put on it.
================
*/
// FUNCTION: STBC 0x0047e180
void NetworkAI::PullTowardPosition( PhysicsObjectClass *object, float elapsed ) {
	NiPoint3 offset = position - object->GetWorldLocation();
	float distance = offset.Unitize();
	if ( distance < 0.5f ) {
		if ( speed != 0.0f ) {
			return;
		}
		object->SetTranslate( &position );
	}
	if ( elapsed > 1.0f ) {
		elapsed = 1.0f;
	}
	offset *= distance * elapsed;
	NiPoint3 location = object->GetWorldLocation() + offset;
	object->SetTranslate( &location );
}

/*
================
NetworkAI::TurnTowardTarget
================
*/
// FUNCTION: STBC 0x0047e2e0
void NetworkAI::TurnTowardTarget( PhysicsObjectClass *object, NiPoint3 target ) {
	NiPoint3 forward = target - object->GetWorldLocation();
	forward.Unitize();
	NiPoint3 right = forward.Cross( up );
	NiPoint3 newUp = right.UnitCross( forward );
	object->TurnTowardOrientation( &TGPoint3( forward ), &TGPoint3( newUp ) );
}

/*
================
NetworkAI::WriteToStream
================
*/
// FUNCTION: STBC 0x0047e4b0
bool NetworkAI::WriteToStream( TGStream *stream ) {
	if ( !ArtificialIntelligence::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( lastUpdateTime );
	stream->WriteFloat( stateTime );
	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	stream->WriteFloat( direction.x );
	stream->WriteFloat( direction.y );
	stream->WriteFloat( direction.z );
	stream->WriteFloat( up.x );
	stream->WriteFloat( up.y );
	stream->WriteFloat( up.z );
	stream->WriteFloat( speed );
	return true;
}

/*
================
NetworkAI::NetworkAI

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x0047e560 ??0NetworkAI@@QAE@PAVTGStream@@PA_N@Z
NetworkAI::NetworkAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ) {
	if ( *ok ) {
		lastUpdateTime = stream->ReadFloat();
		stateTime = stream->ReadFloat();
		position.x = stream->ReadFloat();
		position.y = stream->ReadFloat();
		position.z = stream->ReadFloat();
		direction.x = stream->ReadFloat();
		direction.y = stream->ReadFloat();
		direction.z = stream->ReadFloat();
		up.x = stream->ReadFloat();
		up.y = stream->ReadFloat();
		up.z = stream->ReadFloat();
		speed = stream->ReadFloat();
	}
}

/*
================
NetworkAI::Create
================
*/
// FUNCTION: STBC 0x0047e630
ArtificialIntelligence *NetworkAI::Create( TGStream *stream ) {
	bool ok = true;
	NetworkAI *ai = new NetworkAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}
