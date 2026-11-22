// NiPosData.cpp -- the keys of a position animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiPosData.h"
#include "NiFloatKey.h"
#include "NiKeyframeController.h"
#include "NiKeyframeData.h"
#include "NiKeyframeManager.h"
#include "NiSequenceStreamHelper.h"
#include "NiStringExtraData.h"
#include "NiTextKeyExtraData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a6350 _$E250
// FUNCTION: STBC 0x007a6370 _$E249
// FUNCTION: STBC 0x007a6380 _$E17
// FUNCTION: STBC 0x007a6390 _$E14
// FUNCTION: STBC 0x007a63a0 _$E16
// FUNCTION: STBC 0x007a63b0 _$E15
// FUNCTION: STBC 0x007a63c0 _$E23
// FUNCTION: STBC 0x007a63d0 _$E20
// FUNCTION: STBC 0x007a63e0 _$E22
// FUNCTION: STBC 0x007a63f0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a6400 _$E30
// FUNCTION: STBC 0x007a6410 _$E29
// FUNCTION: STBC 0x007a6420 _$E56
// FUNCTION: STBC 0x007a6430 _$E55
// FUNCTION: STBC 0x007a6470 _$E60
// FUNCTION: STBC 0x007a6480 _$E59

// FUNCTION: STBC 0x007a5c80 _$E243
// FUNCTION: STBC 0x007a5c90 _$E239
// FUNCTION: STBC 0x007a5cb0 _$E242
// FUNCTION: STBC 0x007a5cc0 _$E240
// DATA: STBC 0x9A0438 ?rtti@NiPosData@@2VNiRTTI@@B
const NiRTTI NiPosData::rtti( "NiPosData", &NiObject::rtti );

/*
================
NiPosData::Register
================
*/
// FUNCTION: STBC 0x7A5CE0 ?Register@NiPosData@@SAHXZ
NiImplementRegister( NiPosData )

/*
================
NiPosData::~NiPosData
================
*/
// FUNCTION: STBC 0x7A5E30
NiPosData::~NiPosData() {
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;
}

/*
================
NiPosData::CreateClone
================
*/
// FUNCTION: STBC 0x7A5E80
NiObject *NiPosData::CreateClone() {
	return this;
}

/*
================
NiPosData::CreateObject
================
*/
// FUNCTION: STBC 0x7A5EA0
void NiPosData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiPosData *data = new NiPosData;
	stream.SetLoadRecord( new NiPosDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiPosData::LoadBinary

The key count, then, when there are keys, their type and the keys, each
read by the function of that type.
================
*/
// FUNCTION: STBC 0x7A5F70
void NiPosData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int count;
	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiPosKey::KeyType type;
		NiStreamLoadEnum( stream, type );

		NiPosKey **ppPosKeys = new NiPosKey *[count];
		assert( ppPosKeys );
		NiPosKey::CreateFunction cf = NiPosKey::GetCreateFunction( type );	// NiColorData.cpp's assert names it cf
		for ( unsigned int index = 0; index < count; index++ ) {	// (name invented; fits the /Od frame order)
			ppPosKeys[index] = cf( stream );
		}
		NiPosKey::FillDerivedValuesFunction derived = NiPosKey::GetFillDerivedFunction( type );
		assert( derived );
		derived( ppPosKeys, count );

		ReplaceAnim( ppPosKeys, count, type );
	}
}

/*
================
NiPosData::LinkObject
================
*/
// FUNCTION: STBC 0x7A6080
void NiPosData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiPosData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7A6090
bool NiPosData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiPosData::SaveBinary
================
*/
// FUNCTION: STBC 0x7A60A0
void NiPosData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, numKeys );
	if ( numKeys > 0 ) {
		NiStreamSaveEnum( stream, keyType );
		NiPosKey::SaveFunction saveFunc = NiPosKey::GetSaveFunction( keyType );	// (name invented; fits the /Od frame order)
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			saveFunc( stream, keys[i] );
		}
	}
}

/*
================
NiPosData::IsEqual
================
*/
// FUNCTION: STBC 0x7A6110
bool NiPosData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiPosData *data = (NiPosData *)object;
	if ( numKeys != data->numKeys || keyType != data->keyType ) {
		return false;
	}

	NiPosKey::EqualFunction equal = NiPosKey::GetEqualFunction( keyType );
	assert( equal );
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		if ( !equal( keys[i], data->keys[i] ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiPosData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7A6180
void NiPosData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumKeys", numKeys ) );
}
