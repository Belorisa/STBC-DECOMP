// NiLightColorController.cpp -- light colour animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiLightColorController.h"
#include "NiObjectNET.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a6ed0 _$E221
// FUNCTION: STBC 0x007a6ef0 _$E220
// FUNCTION: STBC 0x007a6e50 _$E17
// FUNCTION: STBC 0x007a6e60 _$E14
// FUNCTION: STBC 0x007a6e70 _$E16
// FUNCTION: STBC 0x007a6e80 _$E15
// FUNCTION: STBC 0x007a6e90 _$E23
// FUNCTION: STBC 0x007a6ea0 _$E20
// FUNCTION: STBC 0x007a6eb0 _$E22
// FUNCTION: STBC 0x007a6ec0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a6f00 _$E30
// FUNCTION: STBC 0x007a6f10 _$E29
// FUNCTION: STBC 0x007a6f20 _$E72
// FUNCTION: STBC 0x007a6f30 _$E71
// FUNCTION: STBC 0x007a6f40 _$E41
// FUNCTION: STBC 0x007a6f50 _$E40
// FUNCTION: STBC 0x007a6f60 _$E76
// FUNCTION: STBC 0x007a6f70 _$E75
// FUNCTION: STBC 0x007a6f80 _$E80
// FUNCTION: STBC 0x007a6f90 _$E79
// FUNCTION: STBC 0x007a6fa0 _$E195
// FUNCTION: STBC 0x007a6fb0 _$E194
// FUNCTION: STBC 0x007a6fc0 _$E200
// FUNCTION: STBC 0x007a6fd0 _$E199
// FUNCTION: STBC 0x007a6fe0 _$E204
// FUNCTION: STBC 0x007a6ff0 _$E203
// FUNCTION: STBC 0x007a7000 _$E60
// FUNCTION: STBC 0x007a7010 _$E59
// FUNCTION: STBC 0x007a7020 _$E64
// FUNCTION: STBC 0x007a7030 _$E63
// FUNCTION: STBC 0x007a70a0 _$E208
// FUNCTION: STBC 0x007a70b0 _$E207

// FUNCTION: STBC 0x007a6490 _$E214
// FUNCTION: STBC 0x007a64a0 _$E210
// FUNCTION: STBC 0x007a64c0 _$E213
// FUNCTION: STBC 0x007a64d0 _$E211
// DATA: STBC 0x9A0478 ?rtti@NiLightColorController@@2VNiRTTI@@B
const NiRTTI NiLightColorController::rtti( "NiLightColorController", &NiTimeController::rtti );

/*
================
NiLightColorController::Register
================
*/
// FUNCTION: STBC 0x7A64F0 ?Register@NiLightColorController@@SAHXZ
NiImplementRegister( NiLightColorController )

/*
================
NiLightColorController::FindController
================
*/
// FUNCTION: STBC 0x7A6640
NiLightColorController *NiLightColorController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiLightColorController *colorController = NiDynamicCast( NiLightColorController, controller );
		if ( colorController ) {
			return colorController;
		}
	}
	return 0;
}

/*
================
NiLightColorController::NiLightColorController

A new controller animates the diffuse and specular colours.
================
*/
// FUNCTION: STBC 0x7A6690
NiLightColorController::NiLightColorController( NiPosData *data_ ) : data( data_ ) {
	lastIndex = 0;
	SetAmbient( false );
}

/*
================
NiLightColorController::~NiLightColorController
================
*/
// FUNCTION: STBC 0x7A66C0
NiLightColorController::~NiLightColorController() {
	data = 0;
}

/*
================
NiLightColorController::ReplaceAnim
================
*/
// FUNCTION: STBC 0x7A6760
void NiLightColorController::ReplaceAnim( unsigned int numKeys, NiPosKey **keys, NiPosKey::KeyType keyType ) {
	if ( !data ) {
		data = new NiPosData;
	}
	data->ReplaceAnim( keys, numKeys, keyType );
	lastIndex = 0;
	loKeyTime = keys[0]->GetTime();
	hiKeyTime = keys[numKeys - 1]->GetTime();
}

/*
================
NiLightColorController::Update

Sets the light's colours to the keys' value at the scaled time, each
channel clamped to [0, 1]: the ambient colour alone, or the diffuse and
specular colours.
================
*/
// FUNCTION: STBC 0x7A6880
void NiLightColorController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	unsigned int num;	// (name invented; fits the /Od frame order)
	NiPosKey::KeyType type;	// (name invented; fits the /Od frame order)
	NiPosKey **anim = GetAnim( num, type );	// (name invented; fits the /Od frame order)
	float sTime = ComputeScaledTime( time );	// (name invented; fits the /Od frame order)
	NiPoint3 keyValue = NiPosKey::GenInterp( sTime, anim, type, num, lastIndex );	// (name invented; fits the /Od frame order)

	NiColor col( keyValue.x, keyValue.y, keyValue.z );	// (name invented; fits the /Od frame order)
	if ( col.r < 0.0f ) {
		col.r = 0.0f;
	} else if ( col.r > 1.0f ) {
		col.r = 1.0f;
	}
	if ( col.g < 0.0f ) {
		col.g = 0.0f;
	} else if ( col.g > 1.0f ) {
		col.g = 1.0f;
	}
	if ( col.b < 0.0f ) {
		col.b = 0.0f;
	} else if ( col.b > 1.0f ) {
		col.b = 1.0f;
	}

	if ( target ) {
		NiLight *light = (NiLight *)target;
		if ( GetAmbient() ) {
			light->SetAmbientColor( col );
			light->SetDiffuseColor( NiColor::BLACK );
			light->SetSpecularColor( NiColor::BLACK );
		} else {
			light->SetAmbientColor( NiColor::BLACK );
			light->SetDiffuseColor( col );
			light->SetSpecularColor( col );
		}
	}
}

/*
================
NiLightColorController::CreateClone
================
*/
// FUNCTION: STBC 0x7A6AD0
NiObject *NiLightColorController::CreateClone() {
	NiLightColorController *clone = new NiLightColorController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiLightColorController::CopyMembers

The clone gets a clone of the data; the ambient flag is not copied.
================
*/
// FUNCTION: STBC 0x7A6B30
void NiLightColorController::CopyMembers( NiLightColorController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->SetPosData( (NiPosData *)data->CreateClone() );
}

/*
================
NiLightColorController::CreateObject
================
*/
// FUNCTION: STBC 0x7A6B80
void NiLightColorController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiLightColorController *controller = new NiLightColorController;
	stream.SetLoadRecord( new NiLightColorControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiLightColorController::LoadBinary

The data's link ID.
================
*/
// FUNCTION: STBC 0x7A6C10
void NiLightColorController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiLightColorControllerLoadRecord *record = (NiLightColorControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->dataLinkID );
	assert( record->dataLinkID );
}

/*
================
NiLightColorController::LinkObject
================
*/
// FUNCTION: STBC 0x7A6C30
void NiLightColorController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiLightColorControllerLoadRecord *record = (NiLightColorControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	data = (NiPosData *)stream.GetObjectFromLinkID( record->dataLinkID );
}

/*
================
NiLightColorController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7A6C80
bool NiLightColorController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	data->RegisterStreamables( stream );
	return true;
}

/*
================
NiLightColorController::SaveBinary
================
*/
// FUNCTION: STBC 0x7A6CB0
void NiLightColorController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiLightColorController::IsEqual
================
*/
// FUNCTION: STBC 0x7A6CE0
bool NiLightColorController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiLightColorController *controller = (NiLightColorController *)object;
	if ( GetAmbient() != controller->GetAmbient() ) {
		return false;
	}
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}

/*
================
NiLightColorController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7A6D30
void NiLightColorController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
