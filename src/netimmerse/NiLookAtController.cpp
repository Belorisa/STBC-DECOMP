// NiLookAtController.cpp -- turns an object to face another

#include "NiRTLib.h"
#include "NiLookAtController.h"
#include "NiNode.h"
#include "NiCloneExtraData.h"
#include "NiObjectNET.h"
#include "NiFloatKey.h"
#include "NiLinFloatKey.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a8130 _$E30
// FUNCTION: STBC 0x007a8140 _$E29
// FUNCTION: STBC 0x007a8150 _$E45
// FUNCTION: STBC 0x007a8160 _$E44
// FUNCTION: STBC 0x007a8170 _$E41
// FUNCTION: STBC 0x007a8180 _$E40
// FUNCTION: STBC 0x007a8190 _$E49
// FUNCTION: STBC 0x007a81a0 _$E48
// FUNCTION: STBC 0x007a81b0 _$E53
// FUNCTION: STBC 0x007a81c0 _$E52
// FUNCTION: STBC 0x007a81d0 _$E177
// FUNCTION: STBC 0x007a81e0 _$E176
// FUNCTION: STBC 0x007a8260 _$E181
// FUNCTION: STBC 0x007a8270 _$E180
// FUNCTION: STBC 0x007a82b0 _$E186
// FUNCTION: STBC 0x007a82c0 _$E185
// FUNCTION: STBC 0x007a82d0 _$E190
// FUNCTION: STBC 0x007a82e0 _$E189

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a80e0 _$E216
// FUNCTION: STBC 0x007a8100 _$E215
// FUNCTION: STBC 0x007a8060 _$E17
// FUNCTION: STBC 0x007a8070 _$E14
// FUNCTION: STBC 0x007a8080 _$E16
// FUNCTION: STBC 0x007a8090 _$E15
// FUNCTION: STBC 0x007a80a0 _$E23
// FUNCTION: STBC 0x007a80b0 _$E20
// FUNCTION: STBC 0x007a80c0 _$E22
// FUNCTION: STBC 0x007a80d0 _$E21

// FUNCTION: STBC 0x007a7610 _$E209
// FUNCTION: STBC 0x007a7620 _$E205
// FUNCTION: STBC 0x007a7640 _$E208
// FUNCTION: STBC 0x007a7650 _$E206
// DATA: STBC 0x9A04B8 ?rtti@NiLookAtController@@2VNiRTTI@@B
const NiRTTI NiLookAtController::rtti( "NiLookAtController", &NiTimeController::rtti );

/*
================
NiLookAtController::Register
================
*/
// FUNCTION: STBC 0x7A7670 ?Register@NiLookAtController@@SAHXZ
NiImplementRegister( NiLookAtController )

/*
================
NiLookAtController::FindController
================
*/
// FUNCTION: STBC 0x7A77C0
NiLookAtController *NiLookAtController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiLookAtController *lookAtController = NiDynamicCast( NiLookAtController, controller );
		if ( lookAtController ) {
			return lookAtController;
		}
	}
	return 0;
}

/*
================
NiLookAtController::Update

Turns the target so that the chosen axis faces the looked-at object. The
world rotation wanted is built from the direction to the object and an up
vector, then taken into the parent's frame. Gamebryo 1.2.2:
NiLookAtController::Update.
================
*/
// FUNCTION: STBC 0x7A7810
void NiLookAtController::Update( float time ) {
	if ( !lookAt || DontDoUpdate( time ) ) {
		return;
	}

	NiPoint3 targetWorldPos = lookAt->GetWorldTranslate();

	// the target's world data is not updated yet: compute where it will be
	NiMatrix3 parentWorldRot;
	NiPoint3 myWorldPos;
	NiAVObject *avTarget = (NiAVObject *)target;
	NiNode *node = avTarget->GetParent();	// (name invented; fits the /Od frame order)
	if ( node ) {
		parentWorldRot = node->GetWorldRotate();
		myWorldPos = node->GetWorldTranslate() + node->GetWorldScale() * ( parentWorldRot * avTarget->GetTranslate() );
	} else {
		parentWorldRot = NiMatrix3::IDENTITY;
		myWorldPos = avTarget->GetTranslate();
	}

	NiMatrix3 rot;
	NiPoint3 at = targetWorldPos - myWorldPos;
	if ( at.SqrLength() < 0.001f ) {
		rot.MakeIdentity();
	} else {
		NiPoint3 up;
		NiPoint3 right;
		float dot;

		at.Unitize();

		if ( at.z < 1.001f && at.z > 0.999f ) {
			// up and at are too close: the Y axis is the other up
			up = NiPoint3::UNIT_Y;
			dot = at.y;
		} else {
			up = NiPoint3::UNIT_Z;
			dot = at.z;
		}

		// take the component of up along at out of it
		up -= dot * at;
		up.Unitize();

		if ( !GetFlip() ) {
			at = -at;
		}

		right = up.Cross( at );

		switch ( GetAxis() ) {
		case X:
			rot.SetCol( 0, at );
			rot.SetCol( 1, up );
			rot.SetCol( 2, -right );
			break;
		case Y:
			rot.SetCol( 0, right );
			rot.SetCol( 1, at );
			rot.SetCol( 2, -up );
			break;
		case Z:
			rot.SetCol( 0, right );
			rot.SetCol( 1, up );
			rot.SetCol( 2, at );
			break;
		}

		rot = parentWorldRot.TransposeTimes( rot );
	}

	avTarget->SetRotate( rot );
}

/*
================
NiLookAtController::CreateClone
================
*/
// FUNCTION: STBC 0x7A7CA0
NiObject *NiLookAtController::CreateClone() {
	NiLookAtController *clone = new NiLookAtController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiLookAtController::CopyMembers

The clone looks at the same object until ProcessClone finds that object's
clone.
================
*/
// FUNCTION: STBC 0x7A7D20
void NiLookAtController::CopyMembers( NiLookAtController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->SetFlip( GetFlip() );
	dest->SetAxis( GetAxis() );
	dest->SetLookAt( GetLookAt() );
}

/*
================
NiLookAtController::ProcessClone

Points the clone at the clone of the looked-at object, when that object
was cloned along; otherwise at a new clone of it.
================
*/
// FUNCTION: STBC 0x7A7D70
void NiLookAtController::ProcessClone( NiObject *clone ) {
	NiTimeController::ProcessClone( clone );
	if ( lookAt ) {
		NiExtraData *extra = lookAt->GetExtraData();
		NiLookAtController *controller = (NiLookAtController *)clone;	// (name invented; fits the /Od frame order)
		if ( extra && NiIsExactKindOf( NiCloneExtraData, extra ) ) {
			NiCloneExtraData *cloneData = (NiCloneExtraData *)extra;
			controller->lookAt = (NiAVObject *)cloneData->GetClone();
		} else {
			controller->lookAt = (NiAVObject *)lookAt->CreateClone();
		}
	}
}

/*
================
NiLookAtController::CreateObject
================
*/
// FUNCTION: STBC 0x7A7DC0
void NiLookAtController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiLookAtController *controller = new NiLookAtController;
	stream.SetLoadRecord( new NiLookAtControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiLookAtController::LoadBinary

The looked-at object's link ID.
================
*/
// FUNCTION: STBC 0x7A7E60
void NiLookAtController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiLookAtControllerLoadRecord *record = (NiLookAtControllerLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->lookAtLinkID );
}

/*
================
NiLookAtController::LinkObject
================
*/
// FUNCTION: STBC 0x7A7E80
void NiLookAtController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiLookAtControllerLoadRecord *record = (NiLookAtControllerLoadRecord *)stream.GetLoadRecord();
	if ( record && record->lookAtLinkID ) {
		lookAt = (NiAVObject *)stream.GetObjectFromLinkID( record->lookAtLinkID );
	} else {
		lookAt = 0;
	}
}

/*
================
NiLookAtController::RegisterStreamables

The looked-at object is not registered: it is saved with its own scene.
================
*/
// FUNCTION: STBC 0x7A7EC0
bool NiLookAtController::RegisterStreamables( NiStream &stream ) {
	return NiTimeController::RegisterStreamables( stream );
}

/*
================
NiLookAtController::SaveBinary
================
*/
// FUNCTION: STBC 0x7A7ED0
void NiLookAtController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, lookAt );
}

/*
================
NiLookAtController::IsEqual
================
*/
// FUNCTION: STBC 0x7A7F00
bool NiLookAtController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiLookAtController *controller = (NiLookAtController *)object;
	if ( ( lookAt && !controller->lookAt ) || ( !lookAt && controller->lookAt ) ) {
		return false;
	}
	if ( lookAt && !lookAt->IsEqual( controller->lookAt ) ) {
		return false;
	}
	return true;
}

/*
================
NiLookAtController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7A7F60
void NiLookAtController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
