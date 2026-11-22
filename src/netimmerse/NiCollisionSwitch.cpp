// NiCollisionSwitch.cpp -- nodes that switch collision testing

#include "NiRTLib.h"
#include "NiCollisionSwitch.h"
#include "NiPick.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f9380 _$E226
// FUNCTION: STBC 0x007f9320 _$E17
// FUNCTION: STBC 0x007f9340 _$E15
// FUNCTION: STBC 0x007f9350 _$E23
// FUNCTION: STBC 0x007f9370 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f93a0 _$E30
// FUNCTION: STBC 0x007f93b0 _$E41
// FUNCTION: STBC 0x007f93c0 _$E45
// FUNCTION: STBC 0x007f93d0 _$E49
// FUNCTION: STBC 0x007f93e0 _$E53
// FUNCTION: STBC 0x007f93f0 _$E177
// FUNCTION: STBC 0x007f9410 _$E186
// FUNCTION: STBC 0x007f9460 _$E190
// FUNCTION: STBC 0x007f9400 _$E182

// FUNCTION: STBC 0x007f8cc0 _$E219
// FUNCTION: STBC 0x007f8ce0 _$E216
// DATA: STBC 0x9A1E98 ?rtti@NiCollisionSwitch@@2VNiRTTI@@B
const NiRTTI NiCollisionSwitch::rtti( "NiCollisionSwitch", &NiNode::rtti );

/*
================
NiCollisionSwitch::Register
================
*/
// FUNCTION: STBC 0x7F8D00 ?Register@NiCollisionSwitch@@SAHXZ
NiImplementRegister( NiCollisionSwitch )

/*
================
NiCollisionSwitch::TestCollisions

NiNode's test while propagating, else no collision.
================
*/
// FUNCTION: STBC 0x7F8E50
bool NiCollisionSwitch::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	if ( GetPropagate() ) {
		return NiNode::TestCollisions( deltaTime, record0, record1 );
	}
	return false;
}

/*
================
NiCollisionSwitch::FindCollisions

NiNode's test while propagating, else none.
================
*/
// FUNCTION: STBC 0x7F8E80
int NiCollisionSwitch::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	if ( GetPropagate() ) {
		return NiNode::FindCollisions( deltaTime, record0, record1 );
	}
	return 0;
}

/*
================
NiCollisionSwitch::FindIntersections

A culled switch is not picked when the pick observes culling; without
propagation nothing below it is picked.
================
*/
// FUNCTION: STBC 0x7F8EB0
bool NiCollisionSwitch::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	if ( pick.GetObserveAppCullFlag() && GetAppCulled() ) {
		return false;
	}
	if ( GetPropagate() ) {
		return NiNode::FindIntersections( origin, dir, pick );
	}
	return false;
}

/*
================
NiCollisionSwitch::CreateClone
================
*/
// FUNCTION: STBC 0x7F8F00
NiObject *NiCollisionSwitch::CreateClone() {
	NiCollisionSwitch *clone = new NiCollisionSwitch;
	CopyMembers( clone );
	return clone;
}

/*
================
NiCollisionSwitch::CopyMembers

The flag comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7F8F80
void NiCollisionSwitch::CopyMembers( NiCollisionSwitch *dest ) {
	NiNode::CopyMembers( dest );
}

/*
================
NiCollisionSwitch::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7F8F90
void NiCollisionSwitch::CreateObject( NiStream &stream, NiObject *&object ) {
	NiCollisionSwitch *node = new NiCollisionSwitch;
	stream.SetLoadRecord( new NiCollisionSwitchLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiCollisionSwitch::LoadBinary

The flag comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7F9110
void NiCollisionSwitch::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
}

/*
================
NiCollisionSwitch::LinkObject
================
*/
// FUNCTION: STBC 0x7F9120
void NiCollisionSwitch::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
}

/*
================
NiCollisionSwitch::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F9130
bool NiCollisionSwitch::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiCollisionSwitch::SaveBinary
================
*/
// FUNCTION: STBC 0x7F9140
void NiCollisionSwitch::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
}

/*
================
NiCollisionSwitch::IsEqual
================
*/
// FUNCTION: STBC 0x7F9150
bool NiCollisionSwitch::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiCollisionSwitch *node = (NiCollisionSwitch *)object;
	if ( GetPropagate() != node->GetPropagate() ) {
		return false;
	}
	return true;
}

/*
================
NiCollisionSwitch::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F9180
void NiCollisionSwitch::GetViewerStrings( NiTArray<char *> *strings ) {
	NiNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bPropagate", GetPropagate() ) );
}
