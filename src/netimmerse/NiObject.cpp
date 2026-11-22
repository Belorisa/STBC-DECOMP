// NiObject.cpp -- the root of the NetImmerse class hierarchy

#include "NiRTLib.h"
#include <string.h>

#include "NiObject.h"
#include "NiStream.h"

// Two dynamic initialisers of the unit's statics.
// FUNCTION: STBC 0x007d84c0 _$E43
// FUNCTION: STBC 0x007d84e0 _$E40

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d8d10 _$E50
// FUNCTION: STBC 0x007d8cb0 _$E17
// FUNCTION: STBC 0x007d8cd0 _$E15
// FUNCTION: STBC 0x007d8ce0 _$E23
// FUNCTION: STBC 0x007d8d00 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d8430 _$E30

// DATA: STBC 0x9A1468 ?rtti@NiObject@@2VNiRTTI@@B
const NiRTTI NiObject::rtti( "NiObject", 0 );

// DATA: STBC 0x9A1478 ?objects@NiObject@@1IA
unsigned int NiObject::objects;
// DATA: STBC 0x9A147C ?defaultCopyType@NiObject@@1W4CopyType@1@A
NiObject::CopyType NiObject::defaultCopyType;
// DATA: STBC 0x9780D6 ?defaultAppendCharacter@NiObject@@1DA
char NiObject::defaultAppendCharacter = '$';

/*
================
NiObject::SetDefaultCopyType
================
*/
// FUNCTION: STBC 0x7D8500
void NiObject::SetDefaultCopyType( CopyType copyType ) {
	defaultCopyType = copyType;
}

/*
================
NiObject::GetDefaultCopyType
================
*/
// FUNCTION: STBC 0x7D8510
NiObject::CopyType NiObject::GetDefaultCopyType() {
	return defaultCopyType;
}

/*
================
NiObject::SetDefaultAppendCharacter
================
*/
// FUNCTION: STBC 0x7D8520
void NiObject::SetDefaultAppendCharacter( char appendCharacter ) {
	defaultAppendCharacter = appendCharacter;
}

/*
================
NiObject::GetDefaultAppendCharacter
================
*/
// FUNCTION: STBC 0x7D8530
char NiObject::GetDefaultAppendCharacter() {
	return defaultAppendCharacter;
}

/*
================
NiObject::Clone

Returns a copy of the object (of the whole subtree for scene graph nodes).
================
*/
// FUNCTION: STBC 0x7D8540
NiObject *NiObject::Clone() {
	NiObject *clone = CreateClone();
	ProcessClone( clone );
	VirtualSlot03();
	return clone;
}

/*
================
NiObject::CreateDeepCopy

Saves the object to a memory block and loads it back as a new object
(Gamebryo lineage: NiObject::CreateDeepCopy).
================
*/
// FUNCTION: STBC 0x7D8560
NiPointer<NiObject> NiObject::CreateDeepCopy() {
	NiStream stream;
	stream.InsertObject( this );

	char *buffer = 0;
	int size = 0;
	stream.Save( buffer, size );

	stream.Load( buffer, size );
	NiPointer<NiObject> copy = stream.GetObjectAt( 0 );
	delete [] buffer;

	return copy;
}

/*
================
NiObject::Register
================
*/
// FUNCTION: STBC 0x7D8650 ?Register@NiObject@@SAHXZ
NiImplementRegister( NiObject )

/*
================
NiObject::NiObject
================
*/
// FUNCTION: STBC 0x7D87A0
NiObject::NiObject() {
	refCount = 0;
	objects++;
}

/*
================
NiObject::~NiObject
================
*/
// FUNCTION: STBC 0x7D87F0
NiObject::~NiObject() {
	objects--;
}

/*
================
NiObject::CreateObject

NiObject is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x7D8810
void NiObject::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiObject::LoadBinary

Reads the object's link ID (its address when it was saved), enters the
object in the stream's link ID map and load list, and keeps the link ID in
the object's load record.
================
*/
// FUNCTION: STBC 0x7D8820
void NiObject::LoadBinary( NiStream &stream ) {
	unsigned int linkID;
	NiStreamLoadBinary( stream, linkID );
	stream.InsertObject( linkID, this );
	stream.AddLinkedObject( (NiObject *)linkID );
	NiLoadRecord *record = stream.GetLoadRecord();
	record->linkID = linkID;
}

/*
================
NiObject::LinkObject

Nothing to resolve: NiObject has no links.
================
*/
// FUNCTION: STBC 0x7D8930
void NiObject::LinkObject( NiStream &stream ) {
}

/*
================
NiObject::RegisterStreamables

Enters the object in the stream's link ID map and save list; false when it
is already there, so that shared objects are saved once.
================
*/
// FUNCTION: STBC 0x7D8940
bool NiObject::RegisterStreamables( NiStream &stream ) {
	if ( !stream.InsertObject( (unsigned int)this, this ) ) {
		return false;
	}
	stream.AddLinkedObject( this );
	return true;
}

/*
================
NiObject::SaveBinary

Writes the class name, which the loader looks up, then the object's
address, which serves as its link ID.
================
*/
// FUNCTION: STBC 0x7D8A40
void NiObject::SaveBinary( NiStream &stream ) {
	stream.SaveCString( GetRTTI()->GetName() );
	NiStreamSaveBinary( stream, this );
}

/*
================
NiObject::IsEqual

Objects of the same class are equal at this level.
================
*/
// FUNCTION: STBC 0x7D8A70
bool NiObject::IsEqual( NiObject *object ) {
	if ( !object ) {
		return false;
	}
	if ( strcmp( GetRTTI()->GetName(), object->GetRTTI()->GetName() ) != 0 ) {
		return false;
	}
	return true;
}

/*
================
NiObject::GetViewerStrings

The class banner, the object's address and its reference count.
================
*/
// FUNCTION: STBC 0x7D8AE0
void NiObject::GetViewerStrings( NiTArray<char *> *strings ) {
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "this", this ) );
	strings->Add( NiGetViewerString( "m_iRefCount", refCount ) );
}
