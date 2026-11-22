// NiTimeController.cpp -- the base of the animation controllers

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "NiTimeController.h"
#include "NiCloneExtraData.h"
#include "NiMath.h"
#include "NiObjectNET.h"
#include "NiNode.h"
#include "NiProperty.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007daa30 _$E200
// FUNCTION: STBC 0x007da9d0 _$E17
// FUNCTION: STBC 0x007da9f0 _$E15
// FUNCTION: STBC 0x007daa00 _$E23
// FUNCTION: STBC 0x007daa20 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007daa50 _$E30
// FUNCTION: STBC 0x007daa80 _$E41
// FUNCTION: STBC 0x007daa90 _$E45
// FUNCTION: STBC 0x007daaa0 _$E50
// FUNCTION: STBC 0x007daab0 _$E54
// FUNCTION: STBC 0x007daac0 _$E178
// FUNCTION: STBC 0x007daae0 _$E187
// FUNCTION: STBC 0x007daad0 _$E183

// FUNCTION: STBC 0x007d99d0 _$E193
// FUNCTION: STBC 0x007d99f0 _$E190
// DATA: STBC 0x9A14E0 ?rtti@NiTimeController@@2VNiRTTI@@B
const NiRTTI NiTimeController::rtti( "NiTimeController", &NiObject::rtti );

/*
================
NiTimeController::Register
================
*/
// FUNCTION: STBC 0x7D9A10 ?Register@NiTimeController@@SAHXZ
NiImplementRegister( NiTimeController )

/*
================
NiTimeController::NiTimeController

A new controller is active and clamps, in application time.
================
*/
// FUNCTION: STBC 0x7D9B60
NiTimeController::NiTimeController() {
	flags = ACTIVE_MASK | ( CLAMP << CYCLETYPE_POS );
	frequency = 1.0f;
	phase = 0.0f;
	loKeyTime = NI_INFINITY;
	hiKeyTime = -NI_INFINITY;
	startTime = -NI_INFINITY;
	lastTime = -NI_INFINITY;
	scaledTime = 0.0f;
	target = 0;
	next = 0;
}

/*
================
NiTimeController::~NiTimeController
================
*/
// FUNCTION: STBC 0x7D9C10
NiTimeController::~NiTimeController() {
	next = 0;
}

/*
================
NiTimeController::ItemsInList

Counts the controllers from this one to the end of the list.
================
*/
// FUNCTION: STBC 0x7D9CB0
unsigned int NiTimeController::ItemsInList() const {
	unsigned int count = 0;
	for ( const NiTimeController *controller = this; controller; controller = controller->next ) {
		count++;
	}
	return count;
}

/*
================
NiTimeController::Start

Activates the controller; an APP_INIT controller counts time from now.
================
*/
// FUNCTION: STBC 0x7D9CC0
void NiTimeController::Start( float time ) {
	SetActive( true );
	lastTime = -NI_INFINITY;
	if ( GetAnimType() == APP_INIT ) {
		startTime = time;
	}
}

/*
================
NiTimeController::Stop
================
*/
// FUNCTION: STBC 0x7D9CF0
void NiTimeController::Stop() {
	SetActive( false );
	lastTime = -NI_INFINITY;
	if ( GetAnimType() == APP_INIT ) {
		startTime = -NI_INFINITY;
	}
}

/*
================
NiTimeController::ComputeScaledTime

The controller's own time for an application time: the elapsed time scaled
by the frequency and shifted by the phase, then mapped into the key range by
the cycle type. The first call starts the clock.
================
*/
// FUNCTION: STBC 0x7D9D20
float NiTimeController::ComputeScaledTime( float time ) {
	float cycleTime = 0.0f;

	if ( startTime == -NI_INFINITY ) {
		startTime = time;
	}

	float deltaTime;
	if ( lastTime == -NI_INFINITY ) {
		scaledTime = 0.0f;
		if ( GetAnimType() == APP_INIT ) {
			deltaTime = 0.0f;
		} else {
			deltaTime = time;
		}
	} else {
		deltaTime = time - lastTime;
	}

	scaledTime += deltaTime * frequency;
	cycleTime = scaledTime + phase;
	lastTime = time;

	switch ( GetCycleType() ) {
		case LOOP: {
			float span = hiKeyTime - loKeyTime;
			if ( span != 0.0f ) {
				cycleTime = fmodf( cycleTime, span ) + loKeyTime;
			} else {
				cycleTime = loKeyTime;
			}
			break;
		}
		case REVERSE: {
			float span = hiKeyTime - loKeyTime;
			if ( span != 0.0f ) {
				float twoSpan = 2.0f * span;
				float cycle = fmodf( cycleTime, twoSpan );
				if ( cycle > span ) {
					cycleTime = twoSpan - cycle + loKeyTime;
				} else {
					cycleTime = cycle + loKeyTime;
				}
			} else {
				cycleTime = loKeyTime;
			}
			break;
		}
	}

	if ( cycleTime > hiKeyTime ) {
		return hiKeyTime;
	}
	if ( cycleTime < loKeyTime ) {
		return loKeyTime;
	}
	return cycleTime;
}

/*
================
NiTimeController::StartAnimations

Starts the object's controllers from the beginning, then those of its
properties and, for a node, of its children, recursively.
================
*/
// FUNCTION: STBC 0x7D9E40 ?StartAnimations@NiTimeController@@SAXPAVNiObjectNET@@@Z
void NiTimeController::StartAnimations( NiObjectNET *object ) {
	NiTimeController *controller;
	for ( controller = object->GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Start( -NI_INFINITY );
	}

	if ( NiIsKindOf( NiAVObject, object ) ) {
		NiAVObject *avObject = NiDynamicCast( NiAVObject, object );
		NiPropertyList *list = &avObject->GetPropertyList();
		while ( list ) {
			NiProperty *property = list->GetItem();
			if ( property && property->GetControllers() ) {
				StartAnimations( property );
			}
			list = list->GetNext();
		}
	}

	if ( NiIsKindOf( NiNode, object ) ) {
		NiNode *node = NiDynamicCast( NiNode, object );
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child ) {
				StartAnimations( child );
			}
		}
	}
}

/*
================
NiTimeController::StartAnimations

Starts the object's controllers at the time, then those of its properties
and, for a node, of its children, recursively.
================
*/
// FUNCTION: STBC 0x7D9FC0 ?StartAnimations@NiTimeController@@SAXPAVNiObjectNET@@M@Z
void NiTimeController::StartAnimations( NiObjectNET *object, float time ) {
	NiTimeController *controller;
	for ( controller = object->GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Start( time );
	}

	if ( NiIsKindOf( NiAVObject, object ) ) {
		NiAVObject *avObject = NiDynamicCast( NiAVObject, object );
		NiPropertyList *list = &avObject->GetPropertyList();
		while ( list ) {
			NiProperty *property = list->GetItem();
			if ( property && property->GetControllers() ) {
				StartAnimations( property, time );
			}
			list = list->GetNext();
		}
	}

	if ( NiIsKindOf( NiNode, object ) ) {
		NiNode *node = NiDynamicCast( NiNode, object );
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child ) {
				StartAnimations( child, time );
			}
		}
	}
}

/*
================
NiTimeController::StopAnimations

Stops the object's controllers, then those of its properties and, for a
node, of its children, recursively.
================
*/
// FUNCTION: STBC 0x7DA140
void NiTimeController::StopAnimations( NiObjectNET *object ) {
	NiTimeController *controller;
	for ( controller = object->GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Stop();
	}

	if ( NiIsKindOf( NiAVObject, object ) ) {
		NiAVObject *avObject = NiDynamicCast( NiAVObject, object );
		NiPropertyList *list = &avObject->GetPropertyList();
		while ( list ) {
			NiProperty *property = list->GetItem();
			if ( property && property->GetControllers() ) {
				StopAnimations( property );
			}
			list = list->GetNext();
		}
	}

	if ( NiIsKindOf( NiNode, object ) ) {
		NiNode *node = NiDynamicCast( NiNode, object );
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child ) {
				StopAnimations( child );
			}
		}
	}
}

/*
================
NiTimeController::SetTarget

Moves the controller from its old target's list to the new target's.
================
*/
// FUNCTION: STBC 0x7DA2C0
void NiTimeController::SetTarget( NiObjectNET *target_ ) {
	if ( target == target_ ) {
		return;
	}

	// keep this controller alive while the old target lets go of it
	IncRefCount();

	if ( target && target->GetControllers() ) {
		target->RemoveController( this );
	}

	target = target_;
	if ( target ) {
		assert( TargetIsRequiredType() );
		NiTimeController *controller = target->GetControllers();
		while ( controller ) {
			if ( controller == this ) {
				// BUG: returns without the DecRefCount below, so a controller
				// already in the new target's list keeps an extra reference
				return;
			}
			controller = controller->GetNext();
		}
		target->PrependController( this );
	}

	DecRefCount();
}

/*
================
NiTimeController::CreateClone

NiTimeController itself is never cloned; the derived classes create the
clone.
================
*/
// FUNCTION: STBC 0x7DA330
NiObject *NiTimeController::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiTimeController::CopyMembers

The clone controls the clone of the target: the one cloning has recorded
on the target, or a new clone of it. The rest of the list is cloned too.
================
*/
// FUNCTION: STBC 0x7DA340
void NiTimeController::CopyMembers( NiTimeController *dest ) {
	NiObject::CopyMembers( dest );
	dest->flags = flags;
	dest->frequency = frequency;
	dest->phase = phase;
	dest->loKeyTime = loKeyTime;
	dest->hiKeyTime = hiKeyTime;
	dest->startTime = startTime;
	dest->lastTime = lastTime;
	dest->scaledTime = scaledTime;

	if ( target ) {
		NiExtraData *extraData = target->GetExtraData();
		if ( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
			NiCloneExtraData *cloneData = (NiCloneExtraData *)extraData;
			dest->target = (NiObjectNET *)cloneData->GetClone();
		} else {
			dest->target = (NiObjectNET *)target->CreateClone();
		}
	}

	if ( next ) {
		dest->next = (NiTimeController *)next->CreateClone();
	} else {
		dest->next = 0;
	}
}

/*
================
NiTimeController::ProcessClone

Passes the call on along the list, to the clone's matching controller.
================
*/
// FUNCTION: STBC 0x7DA420
void NiTimeController::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiTimeController, clone ) );
	NiObject::ProcessClone( clone );
	if ( next ) {
		NiTimeController *controllerClone = (NiTimeController *)clone;
		assert( controllerClone->GetNext() );
		next->ProcessClone( controllerClone->GetNext() );
	}
}

/*
================
NiTimeController::VirtualSlot03

Passes the call on along the list.
================
*/
// FUNCTION: STBC 0x7DA440
void NiTimeController::VirtualSlot03() {
	if ( next ) {
		next->VirtualSlot03();
	}
}

/*
================
NiTimeController::CreateObject

NiTimeController is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x7DA450
void NiTimeController::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiTimeController::LoadBinary

The next controller's link ID, the flags and times, then the target's
link ID: the order SaveBinary writes them in.
================
*/
// FUNCTION: STBC 0x7DA460
void NiTimeController::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
	NiTimeControllerLoadRecord *record = (NiTimeControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->nextLinkID );
	NiStreamLoadBinary( stream, flags );
	NiStreamLoadBinary( stream, frequency );
	NiStreamLoadBinary( stream, phase );
	NiStreamLoadBinary( stream, loKeyTime );
	NiStreamLoadBinary( stream, hiKeyTime );
	NiStreamLoadBinary( stream, record->targetLinkID );
}

/*
================
NiTimeController::LinkObject

Resolves the next controller and the target. NiObject has nothing to
link, so its LinkObject is not called.
================
*/
// FUNCTION: STBC 0x7DA4E0
void NiTimeController::LinkObject( NiStream &stream ) {
	NiTimeControllerLoadRecord *record = (NiTimeControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	if ( record->nextLinkID ) {
		next = (NiTimeController *)stream.GetObjectFromLinkID( record->nextLinkID );
	}
	if ( record->targetLinkID ) {
		target = (NiObjectNET *)stream.GetObjectFromLinkID( record->targetLinkID );
	}
}

/*
================
NiTimeController::RegisterStreamables

Registers the rest of the target's list after this controller.
================
*/
// FUNCTION: STBC 0x7DA540
bool NiTimeController::RegisterStreamables( NiStream &stream ) {
	if ( !NiObject::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( next ) {
		next->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiTimeController::SaveBinary

The next controller's link ID, the flags and times, then the target's
link ID.
================
*/
// FUNCTION: STBC 0x7DA570
void NiTimeController::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, next );
	NiStreamSaveBinary( stream, flags );
	NiStreamSaveBinary( stream, frequency );
	NiStreamSaveBinary( stream, phase );
	NiStreamSaveBinary( stream, loKeyTime );
	NiStreamSaveBinary( stream, hiKeyTime );
	NiStreamSaveBinary( stream, target );
}

/*
================
NiTimeController::IsEqual

The animation and cycle types, the frequency, phase and key times, and
whether both controllers have a target.
================
*/
// FUNCTION: STBC 0x7DA5F0
bool NiTimeController::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiTimeController *controller = (NiTimeController *)object;
	if ( GetAnimType() != controller->GetAnimType()
		|| GetCycleType() != controller->GetCycleType()
		|| frequency != controller->frequency
		|| phase != controller->phase
		|| loKeyTime != controller->loKeyTime
		|| hiKeyTime != controller->hiKeyTime ) {
		return false;
	}
	if ( ( target && !controller->target ) || ( !target && controller->target ) ) {
		return false;
	}
	return true;
}

/*
================
NiTimeController::GetViewerStrings

The class banner, the timing members, the target and the two types. Like
NiExtraData's it leaves out NiObject's strings.
================
*/
// FUNCTION: STBC 0x7DA670
void NiTimeController::GetViewerStrings( NiTArray<char *> *strings ) {
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_fFrequency", frequency ) );
	strings->Add( NiGetViewerString( "m_fPhase", phase ) );
	strings->Add( NiGetViewerString( "m_fLoKeyTime", loKeyTime ) );
	strings->Add( NiGetViewerString( "m_fHiKeyTime", hiKeyTime ) );
	strings->Add( NiGetViewerString( "m_fStartTime", startTime ) );
	strings->Add( NiGetViewerString( "m_fLastTime", lastTime ) );
	strings->Add( NiGetViewerString( "m_pTarget", target ) );
	strings->Add( GetViewerString( "anim type", GetAnimType() ) );
	strings->Add( GetViewerString( "cycle type", GetCycleType() ) );
}

/*
================
NiTimeController::GetViewerString

Returns "<prefix> = APP_TIME" or "<prefix> = APP_INIT" in a new string
that the caller deletes.
================
*/
// FUNCTION: STBC 0x7DA900 ?GetViewerString@NiTimeController@@SAPADPBDW4AnimType@1@@Z
char *NiTimeController::GetViewerString( const char *prefix, AnimType type ) {
	char *string = new char[strlen( prefix ) + 12];
	switch ( type ) {
	case APP_TIME:
		sprintf( string, "%s = APP_TIME", prefix );
		break;
	case APP_INIT:
		sprintf( string, "%s = APP_INIT", prefix );
		break;
	}
	return string;
}

/*
================
NiTimeController::GetViewerString

Returns "<prefix> = LOOP", "REVERSE" or "CLAMP" in a new string that the
caller deletes.
================
*/
// FUNCTION: STBC 0x7DA960 ?GetViewerString@NiTimeController@@SAPADPBDW4CycleType@1@@Z
char *NiTimeController::GetViewerString( const char *prefix, CycleType type ) {
	char *string = new char[strlen( prefix ) + 11];
	switch ( type ) {
	case LOOP:
		sprintf( string, "%s = LOOP", prefix );
		break;
	case REVERSE:
		sprintf( string, "%s = REVERSE", prefix );
		break;
	case CLAMP:
		sprintf( string, "%s = CLAMP", prefix );
		break;
	}
	return string;
}
