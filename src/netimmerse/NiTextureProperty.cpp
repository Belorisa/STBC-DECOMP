// NiTextureProperty.cpp -- the texture image of a surface

#include "NiRTLib.h"
#include "NiTextureProperty.h"
#include "NiStream.h"
#include <stdio.h>
#include <string.h>
#include "NiTextureEffect.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0080a7a0 _$E210
// FUNCTION: STBC 0x0080a7c0 _$E17
// FUNCTION: STBC 0x0080a7e0 _$E15
// FUNCTION: STBC 0x0080a7f0 _$E23
// FUNCTION: STBC 0x0080a810 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0080a820 _$E30
// FUNCTION: STBC 0x0080a830 _$E60
// FUNCTION: STBC 0x0080a840 _$E73
// FUNCTION: STBC 0x0080a850 _$E41
// FUNCTION: STBC 0x0080a860 _$E45
// FUNCTION: STBC 0x0080a870 _$E49
// FUNCTION: STBC 0x0080a880 _$E53
// FUNCTION: STBC 0x0080a8c0 _$E78

// FUNCTION: STBC 0x00809ce0 _$E192
// FUNCTION: STBC 0x00809d00 _$E189
// DATA: STBC 0x9A24D8 ?rtti@NiTextureProperty@@2VNiRTTI@@B
const NiRTTI NiTextureProperty::rtti( "NiTextureProperty", &NiProperty::rtti );

/*
================
NiTextureProperty::Register
================
*/
// FUNCTION: STBC 0x809D20 ?Register@NiTextureProperty@@SAHXZ
NiImplementRegister( NiTextureProperty )

// FUNCTION: STBC 0x00809e70 _$E198
// FUNCTION: STBC 0x00809e90 _$E195
// DATA: STBC 0x9A24E8
NiPointer<NiTextureProperty> NiTextureProperty::defaultProperty;

/*
===============================================================================

	NiTexturePropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiTexturePropertyInit {
public:
	// FUNCTION: STBC 0x809EF0 ??0NiTexturePropertyInit@@QAE@XZ
	NiTexturePropertyInit() { NiTextureProperty::defaultProperty = new NiTextureProperty; }
	~NiTexturePropertyInit() { NiTextureProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00809ed0 _$E203
// FUNCTION: STBC 0x00809fb0 _$E201
static NiTexturePropertyInit init;

/*
================
NiTextureProperty::NiTextureProperty

Defined here, not in the header: the game units call it out of line
(0x809FE0), while this unit's CreateClone and CreateObject expand it.
================
*/
// FUNCTION: STBC 0x809FE0 ??0NiTextureProperty@@QAE@XZ
NiTextureProperty::NiTextureProperty() {
	image = 0;
}

/*
================
NiTextureProperty::NiTextureProperty

The property of the file's image. Called by MetaNebula::CreateExterior
(0x59A2C0).
================
*/
// FUNCTION: STBC 0x80A050 ??0NiTextureProperty@@QAE@PBD@Z
NiTextureProperty::NiTextureProperty( const char *fileName ) {
	image = NiImage::Create( fileName, 2, NiImage::TEX_DEFAULT, 2 );
}

/*
================
NiTextureProperty::NiTextureProperty

The property of an image made from raw pixels.
================
*/
// FUNCTION: STBC 0x80A0F0 ??0NiTextureProperty@@QAE@V?$NiPointer@VNiRawImageData@@@@@Z
NiTextureProperty::NiTextureProperty( NiPointer<NiRawImageData> rawImage ) {
	image = NiImage::Create( rawImage, 2, NiImage::TEX_DEFAULT, 2 );
}

/*
================
NiTextureProperty::~NiTextureProperty
================
*/
// FUNCTION: STBC 0x80A1D0
NiTextureProperty::~NiTextureProperty() {
	image = 0;
}

/*
================
NiTextureProperty::CreateClone

The clone shares the image.
================
*/
// FUNCTION: STBC 0x80A270
NiObject *NiTextureProperty::CreateClone() {
	NiTextureProperty *clone = new NiTextureProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTextureProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x80A340
void NiTextureProperty::CopyMembers( NiTextureProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->image = image;
}

/*
================
NiTextureProperty::CreateObject

Creates the property and loads it, with a record for the link IDs.
================
*/
// FUNCTION: STBC 0x80A390
void NiTextureProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTextureProperty *texture = new NiTextureProperty;
	stream.SetLoadRecord( new NiTexturePropertyLoadRecord );
	texture->LoadBinary( stream );
	object = texture;
}

/*
================
NiTextureProperty::LoadBinary

NiProperty's members, then the image's link ID. Files older than "3.04"
store two more values, which are skipped.
================
*/
// FUNCTION: STBC 0x80A480
void NiTextureProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiTexturePropertyLoadRecord *record = (NiTexturePropertyLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->imageLinkID );

	if ( strcmp( stream.GetFileVersion(), "3.04" ) < 0 ) {
		unsigned int unused;
		NiStreamLoadBinary( stream, unused );
		NiStreamLoadBinary( stream, unused );
	}
}

/*
================
NiTextureProperty::LinkObject
================
*/
// FUNCTION: STBC 0x80A500
void NiTextureProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
	NiTexturePropertyLoadRecord *record = (NiTexturePropertyLoadRecord *)stream.GetLoadRecord();
	image = (NiImage *)stream.GetObjectFromLinkID( record->imageLinkID );
}

/*
================
NiTextureProperty::RegisterStreamables

Registers the image after the property.
================
*/
// FUNCTION: STBC 0x80A550
bool NiTextureProperty::RegisterStreamables( NiStream &stream ) {
	if ( !NiProperty::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( image ) {
		image->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiTextureProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x80A580
void NiTextureProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, (NiImage *)image );
}

/*
================
NiTextureProperty::IsEqual

Equal when both have an image or neither has; the images themselves are
not compared.
================
*/
// FUNCTION: STBC 0x80A5B0
bool NiTextureProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiTextureProperty *texture = (NiTextureProperty *)object;
	if ( ( image && !texture->image ) || ( !image && texture->image ) ) {
		return false;
	}
	return true;
}

/*
================
NiTextureProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x80A5F0
void NiTextureProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );

	char prefix[64];
	sprintf( prefix, "m_spImage" );
	strings->Add( NiGetViewerString( prefix, image ) );
}
