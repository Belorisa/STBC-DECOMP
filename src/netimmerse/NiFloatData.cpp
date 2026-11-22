// NiFloatData.cpp -- the keys of a float animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiFloatData.h"
#include "NiExtraData.h"
#include "NiFloatController.h"
#include "NiObjectNET.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007956c0 _$E79
// FUNCTION: STBC 0x007956e0 _$E78
// FUNCTION: STBC 0x007956f0 _$E17
// FUNCTION: STBC 0x00795700 _$E14
// FUNCTION: STBC 0x00795710 _$E16
// FUNCTION: STBC 0x00795720 _$E15
// FUNCTION: STBC 0x00795730 _$E23
// FUNCTION: STBC 0x00795740 _$E20
// FUNCTION: STBC 0x00795750 _$E22
// FUNCTION: STBC 0x00795760 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00795770 _$E30
// FUNCTION: STBC 0x00795780 _$E29
// FUNCTION: STBC 0x00795790 _$E46
// FUNCTION: STBC 0x007957a0 _$E45
// FUNCTION: STBC 0x007957b0 _$E50
// FUNCTION: STBC 0x007957c0 _$E49

// FUNCTION: STBC 0x00794fb0 _$E72
// FUNCTION: STBC 0x00794fc0 _$E68
// FUNCTION: STBC 0x00794fe0 _$E71
// FUNCTION: STBC 0x00794ff0 _$E69
// DATA: STBC 0x9A0110 ?rtti@NiFloatData@@2VNiRTTI@@B
const NiRTTI NiFloatData::rtti( "NiFloatData", &NiObject::rtti );

/*
================
NiFloatData::Register
================
*/
// FUNCTION: STBC 0x795010 ?Register@NiFloatData@@SAHXZ
NiImplementRegister( NiFloatData )

/*
================
NiFloatData::~NiFloatData
================
*/
// FUNCTION: STBC 0x795160
NiFloatData::~NiFloatData() {
	if ( numKeys > 0 ) {
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			delete keys[i];
		}
		delete [] keys;
	}
}

/*
================
NiFloatData::ReplaceAnim

Without keys the data is left empty; the old keys are not deleted then.
================
*/
// FUNCTION: STBC 0x7951B0
void NiFloatData::ReplaceAnim( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ ) {
	if ( !keys_ || !numKeys_ ) {
		numKeys = 0;
		keys = 0;
		keyType = NiFloatKey::NOINTERP;
		return;
	}

	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;

	numKeys = numKeys_;
	keys = keys_;
	keyType = keyType_;
}

/*
================
NiFloatData::CreateClone
================
*/
// FUNCTION: STBC 0x795230
NiObject *NiFloatData::CreateClone() {
	return this;
}

/*
================
NiFloatData::CreateObject
================
*/
// FUNCTION: STBC 0x795250
void NiFloatData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiFloatData *data = new NiFloatData;
	stream.SetLoadRecord( new NiFloatDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiFloatData::LoadBinary

The key count, then, when there are keys, their type and the keys, each
read by the function of that type.
================
*/
// FUNCTION: STBC 0x795320
void NiFloatData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int count;
	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiFloatKey::KeyType eType;	// (name invented; fits the /Od frame order)
		NiStreamLoadBinary( stream, eType );

		NiFloatKey **ppKeys = new NiFloatKey *[count];
		assert( ppKeys );
		NiFloatKey::CreateFunction cf = NiFloatKey::GetCreateFunction( eType );	// NiColorData.cpp's assert names it cf
		for ( unsigned int index = 0; index < count; index++ ) {	// (name invented; fits the /Od frame order)
			ppKeys[index] = cf( stream, 0 );
		}
		NiFloatKey::FillDerivedValuesFunction derived = NiFloatKey::GetFillDerivedFunction( eType );
		assert( derived );
		derived( ppKeys, count, 0 );

		ReplaceAnim( ppKeys, count, eType );
	}
}

/*
================
NiFloatData::LinkObject
================
*/
// FUNCTION: STBC 0x7953F0
void NiFloatData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiFloatData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x795400
bool NiFloatData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiFloatData::SaveBinary
================
*/
// FUNCTION: STBC 0x795410
void NiFloatData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, numKeys );
	if ( numKeys > 0 ) {
		NiStreamSaveEnum( stream, keyType );
		NiFloatKey::SaveFunction saveFunc = NiFloatKey::GetSaveFunction( keyType );	// (name invented; fits the /Od frame order)
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			saveFunc( stream, keys[i], 0 );
		}
	}
}

/*
================
NiFloatData::IsEqual
================
*/
// FUNCTION: STBC 0x795480
bool NiFloatData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiFloatData *data = (NiFloatData *)object;
	if ( numKeys != data->numKeys || keyType != data->keyType ) {
		return false;
	}

	NiFloatKey::EqualFunction pEqual = NiFloatKey::GetEqualFunction( keyType );
	assert( pEqual );
	for ( unsigned int index = 0; index < numKeys; index++ ) {	// (name invented; fits the /Od frame order)
		if ( !pEqual( keys[index], data->keys[index], 0 ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiFloatData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7954F0
void NiFloatData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumKeys", numKeys ) );
}
