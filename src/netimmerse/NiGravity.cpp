// NiGravity.cpp -- a planar or spherical particle force

#include "NiRTLib.h"
#include <math.h>

#include "NiGravity.h"
#include "NiMatrix3.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079ef20 _$E65
// FUNCTION: STBC 0x0079ef40 _$E64
// FUNCTION: STBC 0x0079ef50 _$E17
// FUNCTION: STBC 0x0079ef60 _$E14
// FUNCTION: STBC 0x0079ef70 _$E16
// FUNCTION: STBC 0x0079ef80 _$E15
// FUNCTION: STBC 0x0079ef90 _$E23
// FUNCTION: STBC 0x0079efa0 _$E20
// FUNCTION: STBC 0x0079efb0 _$E22
// FUNCTION: STBC 0x0079efc0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079efd0 _$E30
// FUNCTION: STBC 0x0079efe0 _$E29
// FUNCTION: STBC 0x0079f030 _$E34
// FUNCTION: STBC 0x0079f040 _$E33
// FUNCTION: STBC 0x0079f090 _$E50
// FUNCTION: STBC 0x0079f0a0 _$E49

// FUNCTION: STBC 0x0079e660 _$E58
// FUNCTION: STBC 0x0079e670 _$E54
// FUNCTION: STBC 0x0079e690 _$E57
// FUNCTION: STBC 0x0079e6a0 _$E55
// DATA: STBC 0x9A0340 ?rtti@NiGravity@@2VNiRTTI@@B
const NiRTTI NiGravity::rtti( "NiGravity", &NiForce::rtti );

/*
================
NiGravity::Register
================
*/
// FUNCTION: STBC 0x79E6C0 ?Register@NiGravity@@SAHXZ
NiImplementRegister( NiGravity )

/*
================
NiGravity::NiGravity
================
*/
// FUNCTION: STBC 0x79E810
NiGravity::NiGravity()
	: position( NiPoint3::ZERO ), direction( NiPoint3::ZERO ),
	worldPosition( NiPoint3::ZERO ), worldDirection( NiPoint3::ZERO ) {
	type = FORCE_PLANAR;
	decay = 0.0f;
	strength = 1.0f;
}

/*
================
NiGravity::UpdateWorld

Transforms the world frame in place, not the model frame: the result
compounds unless UpdateWorldIdentity reset it first.
================
*/
// FUNCTION: STBC 0x79E8B0
void NiGravity::UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	worldPosition = rotate * scale * worldPosition + translate;
	worldDirection = rotate * worldDirection;
}

/*
================
NiGravity::UpdateWorldIdentity
================
*/
// FUNCTION: STBC 0x79E940
void NiGravity::UpdateWorldIdentity() {
	worldPosition = position;
	worldDirection = direction;
}

/*
================
NiGravity::Apply

Slows the particle along the direction, or away from the point, scaled by
the strength and the step; a decay weakens the force with the distance
along the direction or from the point.
================
*/
// FUNCTION: STBC 0x79E970
bool NiGravity::Apply( NiPoint3 &position_, NiPoint3 &velocity, float &deltaTime, float time ) {
	NiPoint3 deltaV;	// (name invented; fits the /Od frame order)
	float distance;
	float falloff;
	if ( type == FORCE_SPHERICAL ) {
		NiPoint3 dir = position_ - worldPosition;
		distance = dir.Length();
		if ( distance != 0.0f ) {
			dir /= distance;
		}
		falloff = 1.0f;
		if ( decay != 0.0f ) {
			falloff = NiExp( -decay * distance );
		}
		deltaV = falloff * dir * strength * deltaTime * 0.16f;
	} else {
		falloff = 1.0f;
		if ( decay != 0.0f ) {
			distance = worldDirection * position_;
			falloff = NiExp( -decay * distance );
		}
		deltaV = falloff * worldDirection * strength * deltaTime * 0.16f;
	}
	velocity -= deltaV;
	return false;
}

/*
================
NiGravity::CreateClone
================
*/
// FUNCTION: STBC 0x79EC00
NiObject *NiGravity::CreateClone() {
	NiGravity *clone = new NiGravity;
	CopyMembers( clone );
	return clone;
}

/*
================
NiGravity::CopyMembers
================
*/
// FUNCTION: STBC 0x79EC60
void NiGravity::CopyMembers( NiGravity *dest ) {
	NiForce::CopyMembers( dest );
	dest->decay = decay;
	dest->strength = strength;
	dest->type = type;
	dest->position = position;
	dest->direction = direction;
	dest->worldPosition = worldPosition;
	dest->worldDirection = worldDirection;
}

/*
================
NiGravity::CreateObject
================
*/
// FUNCTION: STBC 0x79ECD0
void NiGravity::CreateObject( NiStream &stream, NiObject *&object ) {
	NiGravity *gravity = new NiGravity;
	stream.SetLoadRecord( new NiGravityLoadRecord );
	gravity->LoadBinary( stream );
	object = gravity;
}

/*
================
NiGravity::LoadBinary

The world direction is assigned to itself where the model direction was
meant; it stays as constructed until the first UpdateWorld.
================
*/
// FUNCTION: STBC 0x79ED90
void NiGravity::LoadBinary( NiStream &stream ) {
	NiForce::LoadBinary( stream );
	NiStreamLoadBinary( stream, decay );
	NiStreamLoadBinary( stream, strength );
	NiStreamLoadEnum( stream, type );
	position.LoadBinary( stream );
	direction.LoadBinary( stream );
	worldPosition = position;
	// BUG: assigns the world direction to itself where the model direction was meant (retail reads and writes 0x3C).
	worldDirection = worldDirection;
}

/*
================
NiGravity::LinkObject
================
*/
// FUNCTION: STBC 0x79EE10
void NiGravity::LinkObject( NiStream &stream ) {
	NiForce::LinkObject( stream );
}

/*
================
NiGravity::RegisterStreamables
================
*/
// FUNCTION: STBC 0x79EE20
bool NiGravity::RegisterStreamables( NiStream &stream ) {
	return NiForce::RegisterStreamables( stream );
}

/*
================
NiGravity::SaveBinary
================
*/
// FUNCTION: STBC 0x79EE30
void NiGravity::SaveBinary( NiStream &stream ) {
	NiForce::SaveBinary( stream );
	NiStreamSaveBinary( stream, decay );
	NiStreamSaveBinary( stream, strength );
	NiStreamSaveEnum( stream, type );
	position.SaveBinary( stream );
	direction.SaveBinary( stream );
}

/*
================
NiGravity::IsEqual
================
*/
// FUNCTION: STBC 0x79EE80
bool NiGravity::IsEqual( NiObject *object ) {
	if ( !NiForce::IsEqual( object ) ) {
		return false;
	}
	NiGravity *other = (NiGravity *)object;
	if ( decay != other->decay || strength != other->strength || type != other->type
		|| position != other->position || direction != other->direction ) {
		return false;
	}
	return true;
}
