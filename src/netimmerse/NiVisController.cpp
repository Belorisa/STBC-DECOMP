// NiVisController.cpp -- visibility animation

#include "NiRTLib.h"
#include <assert.h>
#include <float.h>

#include "NiVisController.h"
#include "NiAVObject.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b6b00 _$E198
// FUNCTION: STBC 0x007b6b20 _$E197
// FUNCTION: STBC 0x007b6a80 _$E17
// FUNCTION: STBC 0x007b6a90 _$E14
// FUNCTION: STBC 0x007b6aa0 _$E16
// FUNCTION: STBC 0x007b6ab0 _$E15
// FUNCTION: STBC 0x007b6ac0 _$E23
// FUNCTION: STBC 0x007b6ad0 _$E20
// FUNCTION: STBC 0x007b6ae0 _$E22
// FUNCTION: STBC 0x007b6af0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b6b30 _$E30
// FUNCTION: STBC 0x007b6b40 _$E29
// FUNCTION: STBC 0x007b6b50 _$E41
// FUNCTION: STBC 0x007b6b60 _$E40
// FUNCTION: STBC 0x007b6b70 _$E53
// FUNCTION: STBC 0x007b6b80 _$E52
// FUNCTION: STBC 0x007b6b90 _$E57
// FUNCTION: STBC 0x007b6ba0 _$E56
// FUNCTION: STBC 0x007b6bb0 _$E61
// FUNCTION: STBC 0x007b6bc0 _$E60
// FUNCTION: STBC 0x007b6bd0 _$E185
// FUNCTION: STBC 0x007b6be0 _$E184
// FUNCTION: STBC 0x007b6bf0 _$E45
// FUNCTION: STBC 0x007b6c00 _$E44
// FUNCTION: STBC 0x007b6c70 _$E49
// FUNCTION: STBC 0x007b6c80 _$E48

// FUNCTION: STBC 0x007b62a0 _$E191
// FUNCTION: STBC 0x007b62b0 _$E187
// FUNCTION: STBC 0x007b62d0 _$E190
// FUNCTION: STBC 0x007b62e0 _$E188
// DATA: STBC 0x9A0890 ?rtti@NiVisController@@2VNiRTTI@@B
const NiRTTI NiVisController::rtti( "NiVisController", &NiTimeController::rtti );

/*
================
NiVisController::Register
================
*/
// FUNCTION: STBC 0x7B6300 ?Register@NiVisController@@SAHXZ
NiImplementRegister( NiVisController )

/*
================
NiVisController::FindController

The first NiVisController of the target's controllers.
================
*/
// FUNCTION: STBC 0x7B6450
NiVisController *NiVisController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiVisController *visController = NiDynamicCast( NiVisController, controller );
		if ( visController ) {
			return visController;
		}
	}
	return 0;
}

/*
================
NiVisController::NiVisController
================
*/
// FUNCTION: STBC 0x7B64A0
NiVisController::NiVisController( NiVisData *data_ ) : data( data_ ) {
	lastIndex = 0;
}

/*
================
NiVisController::~NiVisController
================
*/
// FUNCTION: STBC 0x7B64D0
NiVisController::~NiVisController() {
	data = 0;
}

/*
================
NiVisController::ResetTimeExtrema

The controller's key times become those of the first and last keys.
================
*/
// FUNCTION: STBC 0x7B6570
void NiVisController::ResetTimeExtrema() {
	loKeyTime = FLT_MAX;
	hiKeyTime = -FLT_MAX;
	unsigned int numKeys;
	NiVisKey **anim = GetVisData( numKeys );	// (name invented; fits the /Od frame order)
	if ( anim ) {
		if ( anim[0]->GetTime() < loKeyTime ) {
			loKeyTime = anim[0]->GetTime();
		}
		unsigned int last = numKeys - 1;
		if ( anim[last]->GetTime() > hiKeyTime ) {
			hiKeyTime = anim[last]->GetTime();
		}
	}
	lastIndex = 0;
}

/*
================
NiVisController::ReplaceKeys
================
*/
// FUNCTION: STBC 0x7B65D0
void NiVisController::ReplaceKeys( NiVisKey **keys, unsigned int numKeys, bool resetTimes ) {
	if ( !data ) {
		data = new NiVisData;
	}
	data->ReplaceAnim( keys, numKeys );
	if ( resetTimes ) {
		ResetTimeExtrema();
	}
	lastIndex = 0;
}

/*
================
NiVisController::Update

Hides the target while the key at the scaled time says so.
================
*/
// FUNCTION: STBC 0x7B6690
void NiVisController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	unsigned int numKeys;
	NiVisKey **anim = GetVisData( numKeys );	// (name invented; fits the /Od frame order)
	if ( anim ) {
		float scaledTime = ComputeScaledTime( time );
		bool visible = NiVisKey::GenInterp( scaledTime, anim, numKeys, lastIndex );
		( (NiAVObject *)target )->SetAppCulled( !visible );
	}
}

/*
================
NiVisController::CreateClone
================
*/
// FUNCTION: STBC 0x7B6700
NiObject *NiVisController::CreateClone() {
	NiVisController *clone = new NiVisController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiVisController::CopyMembers

The clone gets a clone of the data.
================
*/
// FUNCTION: STBC 0x7B6760
void NiVisController::CopyMembers( NiVisController *dest ) {
	NiTimeController::CopyMembers( dest );
	if ( data != 0 ) {
		dest->SetVisData( (NiVisData *)data->CreateClone() );
	} else {
		dest->SetVisData( 0 );
	}
}

/*
================
NiVisController::CreateObject
================
*/
// FUNCTION: STBC 0x7B67E0
void NiVisController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiVisController *controller = new NiVisController;
	stream.SetLoadRecord( new NiVisControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiVisController::LoadBinary

The data's link ID.
================
*/
// FUNCTION: STBC 0x7B6870
void NiVisController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiVisControllerLoadRecord *record = (NiVisControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->dataLinkID );
	assert( record->dataLinkID );
	lastIndex = 0;
}

/*
================
NiVisController::LinkObject
================
*/
// FUNCTION: STBC 0x7B68A0
void NiVisController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiVisControllerLoadRecord *record = (NiVisControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	data = (NiVisData *)stream.GetObjectFromLinkID( record->dataLinkID );
}

/*
================
NiVisController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7B68F0
bool NiVisController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	data->RegisterStreamables( stream );
	return true;
}

/*
================
NiVisController::SaveBinary
================
*/
// FUNCTION: STBC 0x7B6920
void NiVisController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiVisController::IsEqual
================
*/
// FUNCTION: STBC 0x7B6950
bool NiVisController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiVisController *controller = (NiVisController *)object;
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}

/*
================
NiVisController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7B6980
void NiVisController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiVisController::TargetIsRequiredType

The target must be an NiAVObject.
================
*/
// FUNCTION: STBC 0x7B6C90
bool NiVisController::TargetIsRequiredType() const {
	return NiIsKindOf( NiAVObject, target );
}
