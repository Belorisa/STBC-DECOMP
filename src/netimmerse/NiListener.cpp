// NiListener.cpp -- the Miles 3D listeners

#include "NiRTLib.h"
#include "NiListener.h"
#include "NiProvider_Info.h"
#include "NiSoundSystem.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0078d500 _$E200
// FUNCTION: STBC 0x0078d520 _$E199
// FUNCTION: STBC 0x0078d480 _$E17
// FUNCTION: STBC 0x0078d490 _$E14
// FUNCTION: STBC 0x0078d4a0 _$E16
// FUNCTION: STBC 0x0078d4b0 _$E15
// FUNCTION: STBC 0x0078d4c0 _$E23
// FUNCTION: STBC 0x0078d4d0 _$E20
// FUNCTION: STBC 0x0078d4e0 _$E22
// FUNCTION: STBC 0x0078d4f0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0078d530 _$E30
// FUNCTION: STBC 0x0078d540 _$E29
// FUNCTION: STBC 0x0078d550 _$E187
// FUNCTION: STBC 0x0078d560 _$E186
// FUNCTION: STBC 0x0078d570 _$E41
// FUNCTION: STBC 0x0078d580 _$E40
// FUNCTION: STBC 0x0078d590 _$E45
// FUNCTION: STBC 0x0078d5a0 _$E44
// FUNCTION: STBC 0x0078d5b0 _$E49
// FUNCTION: STBC 0x0078d5c0 _$E48
// FUNCTION: STBC 0x0078d5d0 _$E53
// FUNCTION: STBC 0x0078d5e0 _$E52
// FUNCTION: STBC 0x0078d5f0 _$E177
// FUNCTION: STBC 0x0078d600 _$E176
// FUNCTION: STBC 0x0078d620 _$E181
// FUNCTION: STBC 0x0078d630 _$E180

// The rtti's construction and exit destruction.
// FUNCTION: STBC 0x0078cb70 _$E193
// FUNCTION: STBC 0x0078cb80 _$E189
// FUNCTION: STBC 0x0078cba0 _$E192
// FUNCTION: STBC 0x0078cbb0 _$E190
// DATA: STBC 0x99FD60 ?rtti@NiListener@@2VNiRTTI@@B
const NiRTTI NiListener::rtti( "NiListener", &NiAVObject::rtti );

/*
================
NiListener::Register
================
*/
// FUNCTION: STBC 0x78CBD0 ?Register@NiListener@@SAHXZ
NiImplementRegister( NiListener )

/*
================
NiListener::NiListener

Faces down x with z up, at the parent's origin.
================
*/
// FUNCTION: STBC 0x78CD20
NiListener::NiListener() : listeners( 2 ) {
	direction = NiPoint3( 1.0f, 0.0f, 0.0f );
	up = NiPoint3( 0.0f, 1.0f, 0.0f );
	localTranslate = NiPoint3( 0.0f, 0.0f, 0.0f );
	NiMatrix3 rotate;
	rotate.MakeDiagonal( 1.0f, 1.0f, 1.0f );
	localRotate = rotate;
	localVelocity = NiPoint3( 0.0f, 0.0f, 0.0f );
}

/*
================
NiListener::~NiListener
================
*/
// FUNCTION: STBC 0x78CE10
NiListener::~NiListener() {
	CloseListeners();
}

/*
================
NiListener::CloseListeners
================
*/
// FUNCTION: STBC 0x78CEB0
void NiListener::CloseListeners() {
	while ( !listeners.IsEmpty() ) {
		AIL_close_3D_listener( listeners.RemoveHead() );
	}
}

/*
================
NiListener::AddProvider
================
*/
// FUNCTION: STBC 0x78CF10
void NiListener::AddProvider( NiProvider_Info *provider ) {
	if ( !provider->IsOpen() && !provider->Open() ) {
		return;
	}
	listeners.AddTail( AIL_open_3D_listener( provider->GetProvider() ) );
}

/*
================
NiListener::Update
================
*/
// FUNCTION: STBC 0x78CFA0
// PARKED: x87 scheduling: retail computes 1/scale as fld 1.0; fdiv st(1) and pops the scale after; this build folds it to fdivr [1.0] (operator/, operator*=, a named scale and operator/= tried)
void NiListener::Update() {
	if ( listeners.IsEmpty() ) {
		return;
	}

	NiPoint3 position = worldTranslate;
	NiPoint3 dir = worldRotate * direction;
	NiPoint3 upDir = worldRotate * up;
	NiPoint3 velocity = worldVelocity;
	velocity *= 1.0f / ( NiSoundSystem::GetSoundSystem()->GetUnitsPerMeter() * 1000.0f );
	velocity.z = -velocity.z;
	dir.z *= -1.0;
	upDir.z *= -1.0;
	position.z *= -1.0;

	NiTListIterator pos;
	for ( pos = listeners.GetHeadPos(); pos; pos = listeners.GetNextPos( pos ) ) {
		H3DPOBJECT listener = listeners.Get( pos );
		AIL_set_3D_position( listener, position.x, position.y, position.z );
		AIL_set_3D_orientation( listener, dir.x, dir.y, dir.z, upDir.x, upDir.y, upDir.z );
		AIL_set_3D_velocity_vector( listener, velocity.x, velocity.y, velocity.z );
	}
}

/*
================
NiListener::GetListenerPosition
================
*/
// FUNCTION: STBC 0x78D0F0
NiPoint3 NiListener::GetListenerPosition() {
	if ( listeners.IsEmpty() ) {
		return NiPoint3::ZERO;
	}

	NiPoint3 position;
	AIL_3D_position( listeners.GetHead(), &position.x, &position.y, &position.z );
	position.z *= -1.0;
	return position;
}

/*
================
NiListener::GetListenerVelocity
================
*/
// FUNCTION: STBC 0x78D170
NiPoint3 NiListener::GetListenerVelocity() {
	if ( listeners.IsEmpty() ) {
		return NiPoint3::ZERO;
	}

	NiPoint3 velocity;
	AIL_3D_velocity( listeners.GetHead(), &velocity.x, &velocity.y, &velocity.z );
	float scale = NiSoundSystem::GetSoundSystem()->GetUnitsPerMeter() * 1000.0f;
	velocity = velocity * scale;
	velocity.z = -velocity.z;
	return velocity;
}

/*
================
NiListener::GetListenerOrientation
================
*/
// FUNCTION: STBC 0x78D200
void NiListener::GetListenerOrientation( NiPoint3 &dir, NiPoint3 &upDir ) {
	if ( listeners.IsEmpty() ) {
		return;
	}

	AIL_3D_orientation( listeners.GetHead(), &dir.x, &dir.y, &dir.z, &upDir.x, &upDir.y, &upDir.z );
	dir.z *= -1.0;
	upDir.z *= -1.0;
}

/*
================
NiListener::CreateObject

Loads into the sound system's listener rather than a new object.
================
*/
// FUNCTION: STBC 0x78D250
void NiListener::CreateObject( NiStream &stream, NiObject *&object ) {
	NiListener *listener = NiSoundSystem::GetSoundSystem()->GetListener();
	stream.SetLoadRecord( new NiListenerLoadRecord );
	listener->LoadBinary( stream );
	object = listener;
}

/*
================
NiListener::LoadBinary
================
*/
// FUNCTION: STBC 0x78D340
void NiListener::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );
	direction.LoadBinary( stream );
	up.LoadBinary( stream );
}

/*
================
NiListener::LinkObject
================
*/
// FUNCTION: STBC 0x78D370
void NiListener::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );
}

/*
================
NiListener::RegisterStreamables

Registers the sound system after the listener.
================
*/
// FUNCTION: STBC 0x78D380
bool NiListener::RegisterStreamables( NiStream &stream ) {
	if ( !NiAVObject::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( !NiSoundSystem::GetSoundSystem()->RegisterStreamables( stream ) ) {
		return false;
	}
	return true;
}

/*
================
NiListener::SaveBinary
================
*/
// FUNCTION: STBC 0x78D3B0
void NiListener::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );
	direction.SaveBinary( stream );
	up.SaveBinary( stream );
}

/*
================
NiListener::IsEqual
================
*/
// FUNCTION: STBC 0x78D3E0
bool NiListener::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiListener *listener = (NiListener *)object;
	if ( direction != listener->direction ) {
		return false;
	}
	if ( up != listener->up ) {
		return false;
	}
	return true;
}
