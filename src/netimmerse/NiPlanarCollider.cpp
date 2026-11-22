// NiPlanarCollider.cpp -- a rectangle that particles bounce off

#include "NiRTLib.h"
#include "NiPlanarCollider.h"
#include "NiMatrix3.h"
#include "NiSphericalCollider.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a12f0 _$E69
// FUNCTION: STBC 0x007a1310 _$E68
// FUNCTION: STBC 0x007a1320 _$E17
// FUNCTION: STBC 0x007a1330 _$E14
// FUNCTION: STBC 0x007a1340 _$E16
// FUNCTION: STBC 0x007a1350 _$E15
// FUNCTION: STBC 0x007a1360 _$E23
// FUNCTION: STBC 0x007a1370 _$E20
// FUNCTION: STBC 0x007a1380 _$E22
// FUNCTION: STBC 0x007a1390 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a13a0 _$E30
// FUNCTION: STBC 0x007a13b0 _$E29
// FUNCTION: STBC 0x007a13c0 _$E34
// FUNCTION: STBC 0x007a13d0 _$E33
// FUNCTION: STBC 0x007a1420 _$E50
// FUNCTION: STBC 0x007a1430 _$E49

// FUNCTION: STBC 0x007a0670 _$E62
// FUNCTION: STBC 0x007a0680 _$E58
// FUNCTION: STBC 0x007a06a0 _$E61
// FUNCTION: STBC 0x007a06b0 _$E59
// DATA: STBC 0x9A03B0 ?rtti@NiPlanarCollider@@2VNiRTTI@@B
const NiRTTI NiPlanarCollider::rtti( "NiPlanarCollider", &NiForce::rtti );

/*
================
NiPlanarCollider::Register
================
*/
// FUNCTION: STBC 0x7A06D0 ?Register@NiPlanarCollider@@SAHXZ
NiImplementRegister( NiPlanarCollider )

/*
================
NiPlanarCollider::NiPlanarCollider
================
*/
// FUNCTION: STBC 0x7A0820
NiPlanarCollider::NiPlanarCollider()
	: position( NiPoint3::ZERO ), xAxis( NiPoint3::ZERO ), yAxis( NiPoint3::ZERO ), normal( NiPoint3::ZERO ),
	worldPosition( NiPoint3::ZERO ), worldXAxis( NiPoint3::ZERO ), worldYAxis( NiPoint3::ZERO ),
	worldNormal( NiPoint3::ZERO ) {
	height = 1.0f;
	width = 1.0f;
	bounce = 1.0f;
	worldConstant = 0.0f;
	constant = 0.0f;
}

/*
================
NiPlanarCollider::UpdateWorld

Transforms the world frame in place, not the model frame: the result
compounds unless UpdateWorldIdentity reset it first.
================
*/
// FUNCTION: STBC 0x7A0920
void NiPlanarCollider::UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	worldPosition = rotate * scale * worldPosition + translate;
	worldNormal = rotate * worldNormal;
	worldXAxis = rotate * worldXAxis;
	worldYAxis = rotate * worldYAxis;
	worldConstant = ( -worldNormal ).Dot( worldPosition );
}

/*
================
NiPlanarCollider::UpdateWorldIdentity

The world plane keeps its constant.
================
*/
// FUNCTION: STBC 0x7A0A10
void NiPlanarCollider::UpdateWorldIdentity() {
	worldPosition = position;
	worldXAxis = xAxis;
	worldYAxis = yAxis;
	worldNormal = normal;
}

/*
================
NiPlanarCollider::Apply

Intersects the step's path with the plane. A path moving away from the
plane's normal flips the world plane to face it, for good. A hit point that
rounding put on the wrong side is nudged back to the particle's side; a hit
inside the rectangle moves the particle there, cuts the step to what is
left of it, and reflects the velocity about the normal, scaled by the
bounce. The statements are AnimOnly.exe 0x48CB8D's (NiPlanarCollider.cpp),
whose side checks after the nudge and the bounce are debug-only (retail
has none of their code). There the first side test assigns the distance
inside the comparison, as the later ones do; retail's fcom keeps it a
statement of its own.
================
*/
// FUNCTION: STBC 0x7A0A60
bool NiPlanarCollider::Apply( NiPoint3 &position_, NiPoint3 &velocity, float &deltaTime, float time ) {
	const float epsilon = 0.001f;
	NiPoint3 offset;
	NiPoint3 hitPoint;
	NiPoint3 step;

	float distance = worldNormal * position_ + worldConstant;
	bool front = distance > 0.0f;
	float toPlane = -distance;

	step = deltaTime * velocity;
	float approach = worldNormal * step;
	if ( approach < epsilon && approach > -epsilon ) {
		return false;
	}
	if ( approach > 0.0f ) {
		worldNormal = -worldNormal;
		approach = -approach;
		worldConstant = -worldConstant;
		front = !front;
		toPlane = -toPlane;
	}
	float hitFraction = toPlane / approach;
	if ( hitFraction < 0.0f ) {
		return false;
	}
	if ( hitFraction > 1.0f ) {
		return false;
	}

	hitPoint = position_ + hitFraction * step;
	bool hitFront = ( distance = worldNormal * hitPoint + worldConstant ) > 0.0f;
	if ( front != hitFront ) {
		if ( front ) {
			hitPoint = hitPoint + epsilon * worldNormal;
		} else {
			hitPoint = hitPoint - epsilon * worldNormal;
		}
#ifdef _DEBUG
		hitFront = ( distance = worldNormal * hitPoint + worldConstant ) > 0.0f;
		assert( front == hitFront );
#endif
	}

	offset = hitPoint - worldPosition;
	float alongX = offset * worldXAxis;
	offset -= alongX * worldXAxis;
	if ( alongX < 0.0f ) {
		alongX = -alongX;
	}
	float offXSquared = offset.SqrLength();
	if ( 0.5f * width < alongX || offXSquared > 0.25f * height * height ) {
		return false;
	}

	deltaTime -= hitFraction * deltaTime;
	approach = velocity * worldNormal;
	velocity -= ( 2.0f * approach ) * worldNormal;
	velocity *= bounce;
	position_ = hitPoint;

#ifdef _DEBUG
	hitFront = ( distance = worldNormal * position_ + worldConstant ) > 0.0f;
	assert( front == hitFront );
	hitPoint = position_ + deltaTime * velocity;
	hitFront = ( distance = worldNormal * hitPoint + worldConstant ) > 0.0f;
	assert( front == hitFront );
#endif
	return true;
}

/*
================
NiPlanarCollider::CreateClone
================
*/
// FUNCTION: STBC 0x7A0E80
NiObject *NiPlanarCollider::CreateClone() {
	NiPlanarCollider *force = new NiPlanarCollider;
	assert( force );
	CopyMembers( force );
	return force;
}

/*
================
NiPlanarCollider::CopyMembers
================
*/
// FUNCTION: STBC 0x7A0EF0
void NiPlanarCollider::CopyMembers( NiPlanarCollider *dest ) {
	NiForce::CopyMembers( dest );
	dest->height = height;
	dest->width = width;
	dest->bounce = bounce;
	dest->position = position;
	dest->xAxis = xAxis;
	dest->yAxis = yAxis;
	dest->normal = normal;
	dest->constant = constant;
	dest->worldPosition = worldPosition;
	dest->worldXAxis = worldXAxis;
	dest->worldYAxis = worldYAxis;
	dest->worldNormal = worldNormal;
	dest->worldConstant = worldConstant;
}

/*
================
NiPlanarCollider::CreateObject
================
*/
// FUNCTION: STBC 0x7A0FC0
void NiPlanarCollider::CreateObject( NiStream &stream, NiObject *&object ) {
	NiPlanarCollider *collider = new NiPlanarCollider;
	stream.SetLoadRecord( new NiPlanarColliderLoadRecord );
	collider->LoadBinary( stream );
	object = collider;
}

/*
================
NiPlanarCollider::LoadBinary
================
*/
// FUNCTION: STBC 0x7A1080
void NiPlanarCollider::LoadBinary( NiStream &stream ) {
	NiForce::LoadBinary( stream );
	NiStreamLoadBinary( stream, constant );
	worldConstant = constant;
	NiStreamLoadBinary( stream, height );
	NiStreamLoadBinary( stream, width );
	NiStreamLoadBinary( stream, bounce );
	position.LoadBinary( stream );
	xAxis.LoadBinary( stream );
	yAxis.LoadBinary( stream );
	normal.LoadBinary( stream );
	worldPosition = position;
	worldXAxis = xAxis;
	worldYAxis = yAxis;
	worldNormal = normal;
}

/*
================
NiPlanarCollider::LinkObject
================
*/
// FUNCTION: STBC 0x7A1150
void NiPlanarCollider::LinkObject( NiStream &stream ) {
	NiForce::LinkObject( stream );
}

/*
================
NiPlanarCollider::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7A1160
bool NiPlanarCollider::RegisterStreamables( NiStream &stream ) {
	return NiForce::RegisterStreamables( stream );
}

/*
================
NiPlanarCollider::SaveBinary
================
*/
// FUNCTION: STBC 0x7A1170
void NiPlanarCollider::SaveBinary( NiStream &stream ) {
	NiForce::SaveBinary( stream );
	NiStreamSaveBinary( stream, constant );
	NiStreamSaveBinary( stream, height );
	NiStreamSaveBinary( stream, width );
	NiStreamSaveBinary( stream, bounce );
	position.SaveBinary( stream );
	xAxis.SaveBinary( stream );
	yAxis.SaveBinary( stream );
	normal.SaveBinary( stream );
}

/*
================
NiPlanarCollider::IsEqual
================
*/
// FUNCTION: STBC 0x7A11E0
bool NiPlanarCollider::IsEqual( NiObject *object ) {
	if ( !NiForce::IsEqual( object ) ) {
		return false;
	}
	NiPlanarCollider *other = (NiPlanarCollider *)object;
	if ( constant != other->constant || height != other->height || width != other->width ||
		bounce != other->bounce || position != other->position || xAxis != other->xAxis || yAxis != other->yAxis ||
		normal != other->normal ) {
		return false;
	}
	return true;
}
