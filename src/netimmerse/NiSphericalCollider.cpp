// NiSphericalCollider.cpp -- a sphere that particles bounce off

#include "NiRTLib.h"
#include <math.h>

#include "NiSphericalCollider.h"
#include "NiMatrix3.h"
#include "NiParticleBomb.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a0520 _$E69
// FUNCTION: STBC 0x007a0540 _$E68
// FUNCTION: STBC 0x007a0550 _$E17
// FUNCTION: STBC 0x007a0560 _$E14
// FUNCTION: STBC 0x007a0570 _$E16
// FUNCTION: STBC 0x007a0580 _$E15
// FUNCTION: STBC 0x007a0590 _$E23
// FUNCTION: STBC 0x007a05a0 _$E20
// FUNCTION: STBC 0x007a05b0 _$E22
// FUNCTION: STBC 0x007a05c0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a05d0 _$E30
// FUNCTION: STBC 0x007a05e0 _$E29
// FUNCTION: STBC 0x007a05f0 _$E34
// FUNCTION: STBC 0x007a0600 _$E33
// FUNCTION: STBC 0x007a0650 _$E50
// FUNCTION: STBC 0x007a0660 _$E49

// FUNCTION: STBC 0x0079fba0 _$E62
// FUNCTION: STBC 0x0079fbb0 _$E58
// FUNCTION: STBC 0x0079fbd0 _$E61
// FUNCTION: STBC 0x0079fbe0 _$E59
// DATA: STBC 0x9A0380 ?rtti@NiSphericalCollider@@2VNiRTTI@@B
const NiRTTI NiSphericalCollider::rtti( "NiSphericalCollider", &NiForce::rtti );

/*
================
NiSphericalCollider::Register
================
*/
// FUNCTION: STBC 0x79FC00 ?Register@NiSphericalCollider@@SAHXZ
NiImplementRegister( NiSphericalCollider )

/*
================
NiSphericalCollider::NiSphericalCollider
================
*/
// FUNCTION: STBC 0x79FD50
NiSphericalCollider::NiSphericalCollider()
	: position( NiPoint3::ZERO ), worldPosition( NiPoint3::ZERO ) {
	radius = 1.0f;
	radiusSquared = 1.0f;
	bounce = 1.0f;
	unknown18 = 0.0f;
	unknown1C = 0.0f;
}

/*
================
NiSphericalCollider::UpdateWorld

Transforms the world centre in place, not the model centre: the result
compounds unless UpdateWorldIdentity reset it first.
================
*/
// FUNCTION: STBC 0x79FDC0
void NiSphericalCollider::UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	worldPosition = rotate * scale * worldPosition + translate;
}

/*
================
NiSphericalCollider::UpdateWorldIdentity
================
*/
// FUNCTION: STBC 0x79FE20
void NiSphericalCollider::UpdateWorldIdentity() {
	worldPosition = position;
}

/*
================
NiSphericalCollider::Apply

A particle on the surface loses the part of its velocity along the
normal. Otherwise the step's path is intersected with the sphere, from
outside or from inside; on a hit the particle moves to the hit point, the
step is cut to what is left of it, and the velocity is reflected about the
normal and scaled by the bounce. The statements are AnimOnly.exe
0x48BFD7's (NiSphericalCollider.cpp): one float reused for the length,
the dot products and the squared distance.
================
*/
// FUNCTION: STBC 0x79FE40
// PARKED: register and x87 order: retail keeps position_ in edi, loaded after the pushes (ours ebx, at entry), and sums the surface dot product y, x, z where this build gives x, z, y
bool NiSphericalCollider::Apply( NiPoint3 &position_, NiPoint3 &velocity, float &deltaTime, float time ) {
	const float epsilon = 0.001f;

	NiPoint3 toCentre = worldPosition - position_;
	float value = toCentre.Length();
	bool onSurface = value < radius + epsilon && value > radius - epsilon;
	if ( onSurface ) {
		toCentre /= value;
		value = toCentre * velocity;
		velocity -= value * toCentre;
		return true;
	}

	NiPoint3 dir = deltaTime * velocity;
	float stepLength = dir.Length();
	if ( stepLength > epsilon ) {
		dir /= stepLength;
	} else {
		return false;
	}

	// the path's distance to the first or (from inside) the second
	// crossing of the sphere
	value = toCentre.SqrLength();
	float along = toCentre * dir;
	if ( along < 0.0f && value >= radiusSquared ) {
		return false;
	}
	float discriminant = radiusSquared - value + along * along;
	if ( discriminant < 0.0f && value >= radiusSquared ) {
		return false;
	}
	discriminant = NiSqrt( discriminant );
	float hitDistance;
	if ( value >= radiusSquared ) {
		hitDistance = along - discriminant;
	} else {
		hitDistance = along + discriminant;
	}
	if ( hitDistance > stepLength ) {
		return false;
	}

	NiPoint3 hitPoint = position_ + hitDistance * dir;
	deltaTime -= hitDistance / stepLength * deltaTime;
	NiPoint3 normal = ( hitPoint - worldPosition ) / radius;
	if ( value < radiusSquared ) {
		normal = -normal;
	}
	value = velocity * normal;
	velocity -= ( 2.0f * value ) * normal;
	velocity *= bounce;
	position_ = hitPoint;
	return true;
}

/*
================
NiSphericalCollider::CreateClone
================
*/
// FUNCTION: STBC 0x7A0220
NiObject *NiSphericalCollider::CreateClone() {
	NiSphericalCollider *clone = new NiSphericalCollider;
	CopyMembers( clone );
	return clone;
}

/*
================
NiSphericalCollider::CopyMembers
================
*/
// FUNCTION: STBC 0x7A0280
void NiSphericalCollider::CopyMembers( NiSphericalCollider *dest ) {
	NiForce::CopyMembers( dest );
	dest->radius = radius;
	dest->radiusSquared = radiusSquared;
	dest->bounce = bounce;
	dest->unknown18 = unknown18;
	dest->unknown1C = unknown1C;
	dest->position = position;
	dest->worldPosition = worldPosition;
}

/*
================
NiSphericalCollider::CreateObject
================
*/
// FUNCTION: STBC 0x7A02E0
void NiSphericalCollider::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSphericalCollider *collider = new NiSphericalCollider;
	stream.SetLoadRecord( new NiSphericalColliderLoadRecord );
	collider->LoadBinary( stream );
	object = collider;
}

/*
================
NiSphericalCollider::LoadBinary
================
*/
// FUNCTION: STBC 0x7A03A0
void NiSphericalCollider::LoadBinary( NiStream &stream ) {
	NiForce::LoadBinary( stream );
	NiStreamLoadBinary( stream, radius );
	NiStreamLoadBinary( stream, radiusSquared );
	NiStreamLoadBinary( stream, bounce );
	NiStreamLoadBinary( stream, unknown18 );
	NiStreamLoadBinary( stream, unknown1C );
	position.LoadBinary( stream );
	worldPosition = position;
}

/*
================
NiSphericalCollider::LinkObject
================
*/
// FUNCTION: STBC 0x7A0420
void NiSphericalCollider::LinkObject( NiStream &stream ) {
	NiForce::LinkObject( stream );
}

/*
================
NiSphericalCollider::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7A0430
bool NiSphericalCollider::RegisterStreamables( NiStream &stream ) {
	return NiForce::RegisterStreamables( stream );
}

/*
================
NiSphericalCollider::SaveBinary
================
*/
// FUNCTION: STBC 0x7A0440
void NiSphericalCollider::SaveBinary( NiStream &stream ) {
	NiForce::SaveBinary( stream );
	NiStreamSaveBinary( stream, radius );
	NiStreamSaveBinary( stream, radiusSquared );
	NiStreamSaveBinary( stream, bounce );
	NiStreamSaveBinary( stream, unknown18 );
	NiStreamSaveBinary( stream, unknown1C );
	position.SaveBinary( stream );
}

/*
================
NiSphericalCollider::IsEqual

The centre is not compared.
================
*/
// FUNCTION: STBC 0x7A04B0
bool NiSphericalCollider::IsEqual( NiObject *object ) {
	if ( !NiForce::IsEqual( object ) ) {
		return false;
	}
	NiSphericalCollider *other = (NiSphericalCollider *)object;
	if ( radius != other->radius || radiusSquared != other->radiusSquared || bounce != other->bounce ||
		unknown18 != other->unknown18 || unknown1C != other->unknown1C ) {
		return false;
	}
	return true;
}
