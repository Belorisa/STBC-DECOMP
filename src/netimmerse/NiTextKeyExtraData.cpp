// NiTextKeyExtraData.cpp -- an animation's text keys

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiTextKeyExtraData.h"
#include "NiBezFloatKey.h"
#include "NiEulerRotKey.h"
#include "NiFloatKey.h"
#include "NiLinFloatKey.h"
#include "NiRotKey.h"
#include "NiTCBFloatKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007973e0 _$E98
// FUNCTION: STBC 0x00797400 _$E97
// FUNCTION: STBC 0x00797410 _$E17
// FUNCTION: STBC 0x00797420 _$E14
// FUNCTION: STBC 0x00797430 _$E16
// FUNCTION: STBC 0x00797440 _$E15
// FUNCTION: STBC 0x00797450 _$E23
// FUNCTION: STBC 0x00797460 _$E20
// FUNCTION: STBC 0x00797470 _$E22
// FUNCTION: STBC 0x00797480 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00796b90 _$E80
// FUNCTION: STBC 0x00796ba0 _$E79
// FUNCTION: STBC 0x00796bb0 _$E57
// FUNCTION: STBC 0x00796bc0 _$E56
// FUNCTION: STBC 0x00796bd0 _$E83
// FUNCTION: STBC 0x00796be0 _$E82
// FUNCTION: STBC 0x00797490 _$E30
// FUNCTION: STBC 0x007974a0 _$E29
// FUNCTION: STBC 0x007974b0 _$E41
// FUNCTION: STBC 0x007974c0 _$E40
// FUNCTION: STBC 0x00797510 _$E45
// FUNCTION: STBC 0x00797520 _$E44

// FUNCTION: STBC 0x00796bf0 _$E89
// FUNCTION: STBC 0x00796c00 _$E85
// FUNCTION: STBC 0x00796c20 _$E88
// FUNCTION: STBC 0x00796c30 _$E86
// DATA: STBC 0x9A01C0 ?rtti@NiTextKeyExtraData@@2VNiRTTI@@B
const NiRTTI NiTextKeyExtraData::rtti( "NiTextKeyExtraData", &NiExtraData::rtti );

/*
================
NiTextKeyExtraData::Register
================
*/
// FUNCTION: STBC 0x796C50 ?Register@NiTextKeyExtraData@@SAHXZ
NiImplementRegister( NiTextKeyExtraData )

/*
================
NiTextKeyExtraData::CreateClone
================
*/
// FUNCTION: STBC 0x796DA0
NiObject *NiTextKeyExtraData::CreateClone() {
	NiTextKeyExtraData *clone = new NiTextKeyExtraData;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTextKeyExtraData::CopyMembers

The keys are copied only when this object is exactly a
NiTextKeyExtraData.
================
*/
// FUNCTION: STBC 0x796E20
void NiTextKeyExtraData::CopyMembers( NiTextKeyExtraData *dest ) {
	NiExtraData::CopyMembers( dest );
	dest->numKeys = numKeys;
	if ( numKeys > 0 && NiIsExactKindOf( NiTextKeyExtraData, this ) ) {
		dest->keys = new NiTextKey[numKeys];
		for ( unsigned int index = 0; index < numKeys; index++ ) {	// (name invented; fits the /Od frame order)
			dest->keys[index].SetTime( keys[index].GetTime() );
			dest->keys[index].SetText( keys[index].GetText() );
		}
	} else {
		dest->keys = 0;
	}
}

/*
================
NiTextKeyExtraData::CreateObject
================
*/
// FUNCTION: STBC 0x796F10
void NiTextKeyExtraData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTextKeyExtraData *data = new NiTextKeyExtraData;
	stream.SetLoadRecord( new NiTextKeyExtraDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiTextKeyExtraData::LoadBinary
================
*/
// FUNCTION: STBC 0x796FE0
void NiTextKeyExtraData::LoadBinary( NiStream &stream ) {
	NiExtraData::LoadBinary( stream );
	unsigned int count;
	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiTextKey *loaded = new NiTextKey[count];
		assert( loaded );
		for ( unsigned int index = 0; index < count; index++ ) {	// (name invented; fits the /Od frame order)
			loaded[index].LoadBinary( stream );
		}
		SetKeys( loaded, count );
	}
}

/*
================
NiTextKeyExtraData::LinkObject
================
*/
// FUNCTION: STBC 0x7970A0
void NiTextKeyExtraData::LinkObject( NiStream &stream ) {
	NiExtraData::LinkObject( stream );
}

/*
================
NiTextKeyExtraData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7970B0
bool NiTextKeyExtraData::RegisterStreamables( NiStream &stream ) {
	return NiExtraData::RegisterStreamables( stream );
}

/*
================
NiTextKeyExtraData::SaveBinary
================
*/
// FUNCTION: STBC 0x7970C0
void NiTextKeyExtraData::SaveBinary( NiStream &stream ) {
	NiExtraData::SaveBinary( stream );
	NiStreamSaveBinary( stream, numKeys );
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		keys[i].SaveBinary( stream );
	}
}

/*
================
NiTextKeyExtraData::IsEqual

Does not ask NiExtraData's IsEqual: text keys carry no raw block, so both
blocks must be empty; the rest of the list must match, then every key's
time and text.
================
*/
// FUNCTION: STBC 0x797100
bool NiTextKeyExtraData::IsEqual( NiObject *object ) {
	// AnimOnly.exe 0x47E6A2 (NiTextKeyExtraData.cpp)
	if ( !object ) {
		return false;
	}
	NiTextKeyExtraData *other = (NiTextKeyExtraData *)object;
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}
	if ( size || data || other->GetSize() || other->GetChunk() ) {
		return false;
	}
	if ( ( next && !other->GetNext() ) || ( !next && other->GetNext() ) ) {
		return false;
	}
	if ( next && !next->IsEqual( other->GetNext() ) ) {
		return false;
	}
	if ( numKeys != other->numKeys ) {
		return false;
	}
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		if ( keys[i] != other->keys[i] ) {
			return false;
		}
	}
	return true;
}

/*
================
NiTextKeyExtraData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x797210
void NiTextKeyExtraData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiExtraData::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumKeys", numKeys ) );
}
