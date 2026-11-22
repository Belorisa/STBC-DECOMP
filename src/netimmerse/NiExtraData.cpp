// NiExtraData.cpp -- application data blocks attached to objects

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiExtraData.h"
#include "NiStream.h"
#include "NiAccumulator.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d9950 _$E30
// FUNCTION: STBC 0x007d99c0 _$E41

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d98d0 _$E58
// FUNCTION: STBC 0x007d98f0 _$E17
// FUNCTION: STBC 0x007d9910 _$E15
// FUNCTION: STBC 0x007d9920 _$E23
// FUNCTION: STBC 0x007d9940 _$E21

// FUNCTION: STBC 0x007d9030 _$E51
// FUNCTION: STBC 0x007d9050 _$E48
// DATA: STBC 0x9A14A8 ?rtti@NiExtraData@@2VNiRTTI@@B
const NiRTTI NiExtraData::rtti( "NiExtraData", &NiObject::rtti );

/*
================
NiExtraData::Register
================
*/
// FUNCTION: STBC 0x7D9070 ?Register@NiExtraData@@SAHXZ
NiImplementRegister( NiExtraData )

/*
================
NiExtraData::NiExtraData
================
*/
// FUNCTION: STBC 0x7D91C0
NiExtraData::NiExtraData() {
	size = 0;
	data = 0;
	next = 0;
}

/*
================
NiExtraData::~NiExtraData
================
*/
// FUNCTION: STBC 0x7D9230
NiExtraData::~NiExtraData() {
	delete [] data;
	next = 0;
}

/*
================
NiExtraData::Count
================
*/
// FUNCTION: STBC 0x7D92D0
unsigned int NiExtraData::Count() const {
	unsigned int count = 0;
	for ( const NiExtraData *extraData = this; extraData; extraData = extraData->next ) {
		count++;
	}
	return count;
}

/*
================
NiExtraData::CreateClone
================
*/
// FUNCTION: STBC 0x7D92E0
NiObject *NiExtraData::CreateClone() {
	NiExtraData *clone = new NiExtraData;
	CopyMembers( clone );
	return clone;
}

/*
================
NiExtraData::CopyMembers

Copies the data block only for an object of exactly this class (a derived
class copies its own members), then clones the rest of the list.
================
*/
// FUNCTION: STBC 0x7D9380
void NiExtraData::CopyMembers( NiExtraData *dest ) {
	NiObject::CopyMembers( dest );
	dest->size = size;
	if ( size > 0 && NiIsExactKindOf( NiExtraData, this ) ) {
		dest->data = new char[size];
		memcpy( dest->data, data, size );
	} else {
		dest->data = 0;
	}

	if ( next ) {
		dest->next = (NiExtraData *)next->CreateClone();
	} else {
		dest->next = 0;
	}
}

/*
================
NiExtraData::CreateObject
================
*/
// FUNCTION: STBC 0x7D9450
void NiExtraData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiExtraData *extraData = new NiExtraData;
	stream.SetLoadRecord( new NiExtraDataLoadRecord );
	extraData->LoadBinary( stream );
	object = extraData;
}

/*
================
NiExtraData::LoadBinary

The next block's link ID goes into the record in place of this block's
own; the bytes are read only for an object of exactly this class.
================
*/
// FUNCTION: STBC 0x7D9510
void NiExtraData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
	NiLoadRecord *record = stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->linkID );
	NiStreamLoadBinary( stream, size );
	if ( size > 0 && NiIsExactKindOf( NiExtraData, this ) ) {
		data = new char[size];
		assert( data );
		NiStreamLoadBinary( stream, data, size );
	}
}

/*
================
NiExtraData::LinkObject

Resolves the next block. NiObject has nothing to link, so its LinkObject
is not called.
================
*/
// FUNCTION: STBC 0x7D9580
void NiExtraData::LinkObject( NiStream &stream ) {
	NiLoadRecord *record = stream.GetLoadRecord();
	assert( record );
	if ( record->linkID ) {
		next = (NiExtraData *)stream.GetObjectFromLinkID( record->linkID );
	}
}

/*
================
NiExtraData::RegisterStreamables

Registers the rest of the list after this block.
================
*/
// FUNCTION: STBC 0x7D95D0
bool NiExtraData::RegisterStreamables( NiStream &stream ) {
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
NiExtraData::SaveBinary

The next block's link ID, the size, then the bytes; derived classes save
their own data in place of the bytes.
================
*/
// FUNCTION: STBC 0x7D9600
void NiExtraData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, next );
	NiStreamSaveBinary( stream, size );
	if ( size > 0 && NiIsExactKindOf( NiExtraData, this ) ) {
		NiStreamSaveBinary( stream, data, size );
	}
}

/*
================
NiExtraData::IsEqual

The sizes, the bytes and the rest of the lists.
================
*/
// FUNCTION: STBC 0x7D9660
bool NiExtraData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiExtraData *extra = (NiExtraData *)object;
	if ( size != extra->size ) {
		return false;
	}
	if ( memcmp( data, extra->data, size ) != 0 ) {
		return false;
	}
	if ( ( next && !extra->next ) || ( !next && extra->next ) ) {
		return false;
	}
	if ( next ) {
		return next->IsEqual( extra->next );
	}
	return true;
}

/*
================
NiExtraData::GetViewerStrings

The class banner and the size. Unlike the other classes it leaves out
NiObject's strings.
================
*/
// FUNCTION: STBC 0x7D96E0
void NiExtraData::GetViewerStrings( NiTArray<char *> *strings ) {
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiSize", size ) );
}
