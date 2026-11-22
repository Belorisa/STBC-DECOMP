// NiVisData.cpp -- the keys of a visibility animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiVisData.h"
#include "NiRotKey.h"
#include "NiTCBRotKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b61b0 _$E73
// FUNCTION: STBC 0x007b61d0 _$E72
// FUNCTION: STBC 0x007b61e0 _$E17
// FUNCTION: STBC 0x007b61f0 _$E14
// FUNCTION: STBC 0x007b6200 _$E16
// FUNCTION: STBC 0x007b6210 _$E15
// FUNCTION: STBC 0x007b6220 _$E23
// FUNCTION: STBC 0x007b6230 _$E20
// FUNCTION: STBC 0x007b6240 _$E22
// FUNCTION: STBC 0x007b6250 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b6260 _$E30
// FUNCTION: STBC 0x007b6270 _$E29
// FUNCTION: STBC 0x007b6280 _$E41
// FUNCTION: STBC 0x007b6290 _$E40

// FUNCTION: STBC 0x007b5b40 _$E66
// FUNCTION: STBC 0x007b5b50 _$E62
// FUNCTION: STBC 0x007b5b70 _$E65
// FUNCTION: STBC 0x007b5b80 _$E63
// DATA: STBC 0x9A0858 ?rtti@NiVisData@@2VNiRTTI@@B
const NiRTTI NiVisData::rtti( "NiVisData", &NiObject::rtti );

/*
================
NiVisData::Register
================
*/
// FUNCTION: STBC 0x7B5BA0 ?Register@NiVisData@@SAHXZ
NiImplementRegister( NiVisData )

/*
================
NiVisData::~NiVisData
================
*/
// FUNCTION: STBC 0x7B5CF0
NiVisData::~NiVisData() {
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;
}

/*
================
NiVisData::ReplaceAnim

Without keys the data is left empty; the old keys are not deleted then.
================
*/
// FUNCTION: STBC 0x7B5D30
void NiVisData::ReplaceAnim( NiVisKey **keys_, unsigned int numKeys_ ) {
	if ( !keys_ || !numKeys_ ) {
		numKeys = 0;
		keys = 0;
		return;
	}

	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;

	numKeys = numKeys_;
	keys = keys_;
}

/*
================
NiVisData::CreateClone
================
*/
// FUNCTION: STBC 0x7B5D90
NiObject *NiVisData::CreateClone() {
	return this;
}

/*
================
NiVisData::CreateObject
================
*/
// FUNCTION: STBC 0x7B5DB0
void NiVisData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiVisData *data = new NiVisData;
	stream.SetLoadRecord( new NiVisDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiVisData::LoadBinary

The key count, then the keys.
================
*/
// FUNCTION: STBC 0x7B5E80
void NiVisData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int count;
	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiVisKey **ppVisKeys = new NiVisKey *[count];
		assert( ppVisKeys );
		for ( unsigned int i = 0; i < count; i++ ) {
			ppVisKeys[i] = new NiVisKey;
			ppVisKeys[i]->LoadBinary( stream );
		}
		ReplaceAnim( ppVisKeys, count );
	}
}

/*
================
NiVisData::LinkObject
================
*/
// FUNCTION: STBC 0x7B5F00
void NiVisData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiVisData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7B5F10
bool NiVisData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiVisData::SaveBinary
================
*/
// FUNCTION: STBC 0x7B5F20
void NiVisData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, numKeys );
	if ( numKeys > 0 ) {
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			keys[i]->SaveBinary( stream );
		}
	}
}

/*
================
NiVisData::IsEqual
================
*/
// FUNCTION: STBC 0x7B5F70
bool NiVisData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiVisData *data = (NiVisData *)object;
	if ( numKeys != data->numKeys ) {
		return false;
	}
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		if ( *keys[i] != *data->keys[i] ) {
			return false;
		}
	}
	return true;
}

/*
================
NiVisData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7B5FE0
void NiVisData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumVisKeys", numKeys ) );
}
