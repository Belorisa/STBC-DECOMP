// NiAlphaProperty.cpp -- alpha blending and alpha testing

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiAlphaProperty.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007ec8d0 _$E82
// FUNCTION: STBC 0x007ec8f0 _$E17
// FUNCTION: STBC 0x007ec910 _$E15
// FUNCTION: STBC 0x007ec920 _$E23
// FUNCTION: STBC 0x007ec940 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ec950 _$E30
// FUNCTION: STBC 0x007ec960 _$E41
// FUNCTION: STBC 0x007ec970 _$E45
// FUNCTION: STBC 0x007ec980 _$E49
// FUNCTION: STBC 0x007ec990 _$E53
// FUNCTION: STBC 0x007ec9a0 _$E58

// FUNCTION: STBC 0x007ec040 _$E64
// FUNCTION: STBC 0x007ec060 _$E61
// DATA: STBC 0x9A1A10 ?rtti@NiAlphaProperty@@2VNiRTTI@@B
const NiRTTI NiAlphaProperty::rtti( "NiAlphaProperty", &NiProperty::rtti );

/*
================
NiAlphaProperty::Register
================
*/
// FUNCTION: STBC 0x7EC080 ?Register@NiAlphaProperty@@SAHXZ
NiImplementRegister( NiAlphaProperty )

// FUNCTION: STBC 0x007ec1d0 _$E70
// FUNCTION: STBC 0x007ec1f0 _$E67
// DATA: STBC 0x9A19F8
NiPointer<NiAlphaProperty> NiAlphaProperty::defaultProperty;

/*
===============================================================================

	NiAlphaPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiAlphaPropertyInit {
public:
	// FUNCTION: STBC 0x7EC250 ??0NiAlphaPropertyInit@@QAE@XZ
	NiAlphaPropertyInit() { NiAlphaProperty::defaultProperty = new NiAlphaProperty; }
	~NiAlphaPropertyInit() { NiAlphaProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x007ec230 _$E75
// FUNCTION: STBC 0x007ec2f0 _$E73
static NiAlphaPropertyInit init;

/*
================
NiAlphaProperty::CreateClone
================
*/
// FUNCTION: STBC 0x7EC320
NiObject *NiAlphaProperty::CreateClone() {
	NiAlphaProperty *clone = new NiAlphaProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiAlphaProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x7EC3A0
void NiAlphaProperty::CopyMembers( NiAlphaProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->testRef = testRef;
}

/*
================
NiAlphaProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x7EC3C0
void NiAlphaProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAlphaProperty *alpha = new NiAlphaProperty;
	stream.SetLoadRecord( new NiAlphaPropertyLoadRecord );
	alpha->LoadBinary( stream );
	object = alpha;
}

/*
================
NiAlphaProperty::LoadBinary
================
*/
// FUNCTION: STBC 0x7EC4C0
void NiAlphaProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiStreamLoadBinary( stream, testRef );
}

/*
================
NiAlphaProperty::LinkObject
================
*/
// FUNCTION: STBC 0x7EC4F0
void NiAlphaProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiAlphaProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7EC500
bool NiAlphaProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiAlphaProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x7EC510
void NiAlphaProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, testRef );
}

/*
================
NiAlphaProperty::IsEqual
================
*/
// FUNCTION: STBC 0x7EC540
bool NiAlphaProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiAlphaProperty *alpha = (NiAlphaProperty *)object;
	if ( GetAlphaBlending() != alpha->GetAlphaBlending()
		|| GetSrcBlendMode() != alpha->GetSrcBlendMode()
		|| GetDestBlendMode() != alpha->GetDestBlendMode()
		|| GetAlphaTesting() != alpha->GetAlphaTesting()
		|| GetTestMode() != alpha->GetTestMode()
		|| GetTestRef() != alpha->GetTestRef() ) {
		return false;
	}
	return true;
}

/*
================
NiAlphaProperty::GetViewerStrings

The blending flag and the destination factor; the source factor and the
test are left out.
================
*/
// FUNCTION: STBC 0x7EC5B0
void NiAlphaProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bAlpha", GetAlphaBlending() ) );
	strings->Add( GetViewerString( "m_destBlend", GetDestBlendMode() ) );
}

/*
================
NiAlphaProperty::GetViewerString
================
*/
// FUNCTION: STBC 0x7EC780
char *NiAlphaProperty::GetViewerString( const char *prefix, AlphaFunction mode ) {
	char *string = new char[strlen( prefix ) + 22];
	switch ( mode ) {
	case ALPHA_ONE:
		sprintf( string, "%s = ALPHA_ONE", prefix );
		break;
	case ALPHA_ZERO:
		sprintf( string, "%s = ALPHA_ZERO", prefix );
		break;
	case ALPHA_SRCCOLOR:
		sprintf( string, "%s = ALPHA_SRCCOLOR", prefix );
		break;
	case ALPHA_INVSRCCOLOR:
		sprintf( string, "%s = ALPHA_INVSRCCOLOR", prefix );
		break;
	case ALPHA_DESTCOLOR:
		sprintf( string, "%s = ALPHA_DESTCOLOR", prefix );
		break;
	case ALPHA_INVDESTCOLOR:
		sprintf( string, "%s = ALPHA_INVDESTCOLOR", prefix );
		break;
	case ALPHA_SRCALPHA:
		sprintf( string, "%s = ALPHA_SRCALPHA", prefix );
		break;
	case ALPHA_INVSRCALPHA:
		sprintf( string, "%s = ALPHA_INVSRCALPHA", prefix );
		break;
	case ALPHA_DESTALPHA:
		sprintf( string, "%s = ALPHA_DESTALPHA", prefix );
		break;
	case ALPHA_INVDESTALPHA:
		sprintf( string, "%s = ALPHA_INVDESTALPHA", prefix );
		break;
	case ALPHA_SRCALPHASAT:
		sprintf( string, "%s = ALPHA_SRCALPHASAT", prefix );
		break;
	}
	return string;
}
