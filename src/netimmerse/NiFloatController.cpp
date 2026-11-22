// NiFloatController.cpp -- the base of the float animation controllers

#include "NiRTLib.h"
#include <assert.h>
#include "NiFloatController.h"
#include "NiObjectNET.h"
#include "NiFlipController.h"
#include "NiProperty.h"
#include "NiTextureProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00794df0 _$E112
// FUNCTION: STBC 0x00794e10 _$E111
// FUNCTION: STBC 0x00794e20 _$E17
// FUNCTION: STBC 0x00794e30 _$E14
// FUNCTION: STBC 0x00794e40 _$E16
// FUNCTION: STBC 0x00794e50 _$E15
// FUNCTION: STBC 0x00794e60 _$E23
// FUNCTION: STBC 0x00794e70 _$E20
// FUNCTION: STBC 0x00794e80 _$E22
// FUNCTION: STBC 0x00794e90 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00794ea0 _$E30
// FUNCTION: STBC 0x00794eb0 _$E29
// FUNCTION: STBC 0x00794ec0 _$E41
// FUNCTION: STBC 0x00794ed0 _$E40
// FUNCTION: STBC 0x00794ee0 _$E50
// FUNCTION: STBC 0x00794ef0 _$E49
// FUNCTION: STBC 0x00794f30 _$E54
// FUNCTION: STBC 0x00794f40 _$E53
// FUNCTION: STBC 0x00794f50 _$E58
// FUNCTION: STBC 0x00794f60 _$E57
// FUNCTION: STBC 0x00794f70 _$E62
// FUNCTION: STBC 0x00794f80 _$E61
// FUNCTION: STBC 0x00794f90 _$E66
// FUNCTION: STBC 0x00794fa0 _$E65

// FUNCTION: STBC 0x007947b0 _$E105
// FUNCTION: STBC 0x007947c0 _$E101
// FUNCTION: STBC 0x007947e0 _$E104
// FUNCTION: STBC 0x007947f0 _$E102
// DATA: STBC 0x9A00F0 ?rtti@NiFloatController@@2VNiRTTI@@B
const NiRTTI NiFloatController::rtti( "NiFloatController", &NiTimeController::rtti );

/*
================
NiFloatController::Register
================
*/
// FUNCTION: STBC 0x794810 ?Register@NiFloatController@@SAHXZ
NiImplementRegister( NiFloatController )

/*
================
NiFloatController::FindController
================
*/
// FUNCTION: STBC 0x794960
NiFloatController *NiFloatController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiFloatController *floatController = NiDynamicCast( NiFloatController, controller );
		if ( floatController ) {
			return floatController;
		}
	}
	return 0;
}

/*
================
NiFloatController::~NiFloatController
================
*/
// FUNCTION: STBC 0x7949B0
NiFloatController::~NiFloatController() {
	data = 0;
}

/*
================
NiFloatController::GetAnim
================
*/
// FUNCTION: STBC 0x794A50
NiFloatKey **NiFloatController::GetAnim( unsigned int &numKeys, NiFloatKey::KeyType &keyType ) const {
	if ( data ) {
		return data->GetAnim( numKeys, keyType );
	}
	numKeys = 0;
	keyType = NiFloatKey::NOINTERP;
	return 0;
}

/*
================
NiFloatController::ReplaceAnim
================
*/
// FUNCTION: STBC 0x794A90
void NiFloatController::ReplaceAnim( unsigned int numKeys, NiFloatKey **keys, NiFloatKey::KeyType keyType ) {
	if ( !data ) {
		data = new NiFloatData;
	}
	data->ReplaceAnim( keys, numKeys, keyType );
	lastIndex = 0;
	loKeyTime = keys[0]->GetTime();
	hiKeyTime = keys[numKeys - 1]->GetTime();
}

/*
================
NiFloatController::CreateClone

NiFloatController itself is never cloned; the derived classes create the
clone.
================
*/
// FUNCTION: STBC 0x794B60
NiObject *NiFloatController::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiFloatController::CopyMembers

The clone gets a clone of the data.
================
*/
// FUNCTION: STBC 0x794B70
void NiFloatController::CopyMembers( NiFloatController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->SetFloatData( (NiFloatData *)data->CreateClone() );
}

/*
================
NiFloatController::CreateObject

NiFloatController is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x794BC0
void NiFloatController::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiFloatController::LoadBinary

The data's link ID.
================
*/
// FUNCTION: STBC 0x794BD0
void NiFloatController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiFloatControllerLoadRecord *record = (NiFloatControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->dataLinkID );
	assert( record->dataLinkID );
}

/*
================
NiFloatController::LinkObject
================
*/
// FUNCTION: STBC 0x794BF0
void NiFloatController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiFloatControllerLoadRecord *record = (NiFloatControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	data = (NiFloatData *)stream.GetObjectFromLinkID( record->dataLinkID );
}

/*
================
NiFloatController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x794C40
bool NiFloatController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	data->RegisterStreamables( stream );
	return true;
}

/*
================
NiFloatController::SaveBinary
================
*/
// FUNCTION: STBC 0x794C70
void NiFloatController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiFloatController::IsEqual
================
*/
// FUNCTION: STBC 0x794CA0
bool NiFloatController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiFloatController *controller = (NiFloatController *)object;
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}

/*
================
NiFloatController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x794CD0
void NiFloatController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
