// NiParticleBomb.cpp -- a timed particle force pushing away from a point, axis or plane

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>

#include "NiParticleBomb.h"
#include "NiMatrix3.h"
#include "NiGravity.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079fa50 _$E69
// FUNCTION: STBC 0x0079fa70 _$E68
// FUNCTION: STBC 0x0079fa80 _$E17
// FUNCTION: STBC 0x0079fa90 _$E14
// FUNCTION: STBC 0x0079faa0 _$E16
// FUNCTION: STBC 0x0079fab0 _$E15
// FUNCTION: STBC 0x0079fac0 _$E23
// FUNCTION: STBC 0x0079fad0 _$E20
// FUNCTION: STBC 0x0079fae0 _$E22
// FUNCTION: STBC 0x0079faf0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079fb00 _$E30
// FUNCTION: STBC 0x0079fb10 _$E29
// FUNCTION: STBC 0x0079fb20 _$E34
// FUNCTION: STBC 0x0079fb30 _$E33
// FUNCTION: STBC 0x0079fb80 _$E50
// FUNCTION: STBC 0x0079fb90 _$E49

// FUNCTION: STBC 0x0079f0b0 _$E62
// FUNCTION: STBC 0x0079f0c0 _$E58
// FUNCTION: STBC 0x0079f0e0 _$E61
// FUNCTION: STBC 0x0079f0f0 _$E59
// DATA: STBC 0x9A0368 ?rtti@NiParticleBomb@@2VNiRTTI@@B
const NiRTTI NiParticleBomb::rtti( "NiParticleBomb", &NiForce::rtti );

/*
================
NiParticleBomb::Register
================
*/
// FUNCTION: STBC 0x79F110 ?Register@NiParticleBomb@@SAHXZ
NiImplementRegister( NiParticleBomb )

/*
================
NiParticleBomb::NiParticleBomb
================
*/
// FUNCTION: STBC 0x79F260
NiParticleBomb::NiParticleBomb()
	: position( NiPoint3::ZERO ), direction( NiPoint3::ZERO ),
	worldPosition( NiPoint3::ZERO ), worldDirection( NiPoint3::ZERO ) {
	deltaV = 0.0f;
	start = 0.0f;
	decayType = NONE;
	symmType = SPHERICAL;
	decay = 0.0f;
	duration = 0.0f;
}

/*
================
NiParticleBomb::UpdateWorld

Transforms the world frame in place, not the model frame: the result
compounds unless UpdateWorldIdentity reset it first.
================
*/
// FUNCTION: STBC 0x79F300
void NiParticleBomb::UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	worldPosition = rotate * scale * worldPosition + translate;
	worldDirection = rotate * worldDirection;
}

/*
================
NiParticleBomb::UpdateWorldIdentity
================
*/
// FUNCTION: STBC 0x79F390
void NiParticleBomb::UpdateWorldIdentity() {
	worldPosition = position;
	worldDirection = direction;
}

/*
================
NiParticleBomb::Apply

Pushes the particle for the part of the step that overlaps the bomb's
duration, away from the point, the axis or the plane, by deltaV per
second weakened by the decay.
================
*/
// FUNCTION: STBC 0x79F3C0
bool NiParticleBomb::Apply( NiPoint3 &position_, NiPoint3 &velocity, float &deltaTime, float time ) {
	float stepStart = time - deltaTime;
	float begin = start;
	float end = start + duration;
	if ( begin < stepStart ) {
		begin = stepStart;
	}
	if ( end > time ) {
		end = time;
	}
	float overlap = end - begin;
	if ( overlap <= 0.0f ) {
		return false;
	}

	NiPoint3 diff = position_ - worldPosition;
	float distance = diff.Length();
	if ( decayType != NONE && distance > decay ) {
		return false;
	}

	NiPoint3 dir;
	if ( symmType == SPHERICAL ) {
		dir = diff / distance;
	} else if ( symmType == PLANAR ) {
		distance = worldDirection * diff;
		if ( distance < 0.0f ) {
			distance = -distance;
			dir = -worldDirection;
		} else {
			dir = worldDirection;
		}
	} else {
		distance = worldDirection * diff;
		dir = diff - distance * worldDirection;
		distance = dir.Length();
		if ( distance != 0.0f ) {
			dir /= distance;
		}
	}

	float falloff = 1.0f;
	if ( decayType == LINEAR ) {
		falloff = ( decay - distance ) / decay;
	} else if ( decayType == EXPONENTIAL ) {
		falloff = 1.0f / NiExp( distance / decay );
	}
	velocity += dir * ( falloff * deltaV * overlap );
	return false;
}

/*
================
NiParticleBomb::CreateClone
================
*/
// FUNCTION: STBC 0x79F680
NiObject *NiParticleBomb::CreateClone() {
	NiParticleBomb *clone = new NiParticleBomb;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiParticleBomb::CopyMembers
================
*/
// FUNCTION: STBC 0x79F6E0
void NiParticleBomb::CopyMembers( NiParticleBomb *dest ) {
	NiForce::CopyMembers( dest );
	dest->decay = decay;
	dest->duration = duration;
	dest->deltaV = deltaV;
	dest->start = start;
	dest->decayType = decayType;
	dest->symmType = symmType;
	dest->position = position;
	dest->worldPosition = worldPosition;
	dest->direction = direction;
	dest->worldDirection = worldDirection;
}

/*
================
NiParticleBomb::CreateObject
================
*/
// FUNCTION: STBC 0x79F760
void NiParticleBomb::CreateObject( NiStream &stream, NiObject *&object ) {
	NiParticleBomb *bomb = new NiParticleBomb;
	stream.SetLoadRecord( new NiParticleBombLoadRecord );
	bomb->LoadBinary( stream );
	object = bomb;
}

/*
================
NiParticleBomb::LoadBinary
================
*/
// FUNCTION: STBC 0x79F820
void NiParticleBomb::LoadBinary( NiStream &stream ) {
	NiForce::LoadBinary( stream );
	NiStreamLoadBinary( stream, decay );
	NiStreamLoadBinary( stream, duration );
	NiStreamLoadBinary( stream, deltaV );
	NiStreamLoadBinary( stream, start );
	NiStreamLoadEnum( stream, decayType );
	NiStreamLoadEnum( stream, symmType );
	position.LoadBinary( stream );
	worldPosition = position;
	direction.LoadBinary( stream );
	worldDirection = direction;
}

/*
================
NiParticleBomb::LinkObject
================
*/
// FUNCTION: STBC 0x79F8E0
void NiParticleBomb::LinkObject( NiStream &stream ) {
	NiForce::LinkObject( stream );
}

/*
================
NiParticleBomb::RegisterStreamables
================
*/
// FUNCTION: STBC 0x79F8F0
bool NiParticleBomb::RegisterStreamables( NiStream &stream ) {
	return NiForce::RegisterStreamables( stream );
}

/*
================
NiParticleBomb::SaveBinary
================
*/
// FUNCTION: STBC 0x79F900
void NiParticleBomb::SaveBinary( NiStream &stream ) {
	NiForce::SaveBinary( stream );
	NiStreamSaveBinary( stream, decay );
	NiStreamSaveBinary( stream, duration );
	NiStreamSaveBinary( stream, deltaV );
	NiStreamSaveBinary( stream, start );
	NiStreamSaveEnum( stream, decayType );
	NiStreamSaveEnum( stream, symmType );
	position.SaveBinary( stream );
	direction.SaveBinary( stream );
}

/*
================
NiParticleBomb::IsEqual
================
*/
// FUNCTION: STBC 0x79F980
bool NiParticleBomb::IsEqual( NiObject *object ) {
	if ( !NiForce::IsEqual( object ) ) {
		return false;
	}
	NiParticleBomb *other = (NiParticleBomb *)object;
	if ( decay != other->decay || duration != other->duration || deltaV != other->deltaV ||
		start != other->start || decayType != other->decayType || symmType != other->symmType ||
		position != other->position || direction != other->direction ) {
		return false;
	}
	return true;
}
