// NiColorData.cpp -- the keys of a colour animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiColorData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079de90 _$E66
// FUNCTION: STBC 0x0079deb0 _$E65
// FUNCTION: STBC 0x0079dec0 _$E17
// FUNCTION: STBC 0x0079ded0 _$E14
// FUNCTION: STBC 0x0079dee0 _$E16
// FUNCTION: STBC 0x0079def0 _$E15
// FUNCTION: STBC 0x0079df00 _$E23
// FUNCTION: STBC 0x0079df10 _$E20
// FUNCTION: STBC 0x0079df20 _$E22
// FUNCTION: STBC 0x0079df30 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079df40 _$E30
// FUNCTION: STBC 0x0079df50 _$E29
// FUNCTION: STBC 0x0079df60 _$E49
// FUNCTION: STBC 0x0079df70 _$E48
// FUNCTION: STBC 0x0079dfb0 _$E53
// FUNCTION: STBC 0x0079dfc0 _$E52

// FUNCTION: STBC 0x0079d800 _$E59
// FUNCTION: STBC 0x0079d810 _$E55
// FUNCTION: STBC 0x0079d830 _$E58
// FUNCTION: STBC 0x0079d840 _$E56
// DATA: STBC 0x9A02E0 ?rtti@NiColorData@@2VNiRTTI@@B
const NiRTTI NiColorData::rtti( "NiColorData", &NiObject::rtti );

/*
================
NiColorData::Register
================
*/
// FUNCTION: STBC 0x79D860 ?Register@NiColorData@@SAHXZ
NiImplementRegister( NiColorData )

/*
================
NiColorData::~NiColorData
================
*/
// FUNCTION: STBC 0x79D9B0
NiColorData::~NiColorData() {
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;
}

/*
================
NiColorData::CreateClone
================
*/
// FUNCTION: STBC 0x79DA00
NiObject *NiColorData::CreateClone() {
	return this;
}

/*
================
NiColorData::CreateObject
================
*/
// FUNCTION: STBC 0x79DA20
void NiColorData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiColorData *data = new NiColorData;
	stream.SetLoadRecord( new NiColorDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiColorData::LoadBinary

The key count, then, when there are keys, their type and the keys, each
read by the function of that type.
================
*/
// FUNCTION: STBC 0x79DAF0
void NiColorData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int count;
	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiColorKey::KeyType type;
		NiStreamLoadBinary( stream, type );

		NiColorKey **ppColorKeys = new NiColorKey *[count];
		assert( ppColorKeys );
		NiColorKey::CreateFunction cf = NiColorKey::GetCreateFunction( type );
		assert( cf );
		for ( unsigned int index = 0; index < count; index++ ) {	// (name invented; fits the /Od frame order)
			ppColorKeys[index] = cf( stream );
		}

		SetAnim( ppColorKeys, count, type );
	}
}

/*
================
NiColorData::LinkObject
================
*/
// FUNCTION: STBC 0x79DBC0
void NiColorData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiColorData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x79DBD0
bool NiColorData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiColorData::SaveBinary
================
*/
// FUNCTION: STBC 0x79DBE0
void NiColorData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, numKeys );
	if ( numKeys > 0 ) {
		NiStreamSaveEnum( stream, keyType );
		NiColorKey::SaveFunction saveFunc = NiColorKey::GetSaveFunction( keyType );	// (name invented; fits the /Od frame order)
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			saveFunc( stream, keys[i] );
		}
	}
}

/*
================
NiColorData::IsEqual
================
*/
// FUNCTION: STBC 0x79DC50
bool NiColorData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiColorData *data = (NiColorData *)object;
	if ( numKeys != data->numKeys || keyType != data->keyType ) {
		return false;
	}

	NiColorKey::EqualFunction equal = NiColorKey::GetEqualFunction( keyType );
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
NiColorData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x79DCC0
void NiColorData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumKeys", numKeys ) );
}
