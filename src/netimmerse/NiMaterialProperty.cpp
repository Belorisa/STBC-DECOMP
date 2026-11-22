// NiMaterialProperty.cpp -- the material colours of lit surfaces

#include "NiRTLib.h"
#include "NiMaterialProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"
#include "renderer/NiImage.h"
#include "NiLODNode.h"
#include "NiNode.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"
#include "NiSwitchNode.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00800fb0 _$E238
// FUNCTION: STBC 0x00800fd0 _$E17
// FUNCTION: STBC 0x00800ff0 _$E15
// FUNCTION: STBC 0x00801000 _$E23
// FUNCTION: STBC 0x00801020 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00801030 _$E34
// FUNCTION: STBC 0x00801040 _$E45
// FUNCTION: STBC 0x00801050 _$E49
// FUNCTION: STBC 0x00801060 _$E53
// FUNCTION: STBC 0x00801070 _$E57
// FUNCTION: STBC 0x008010c0 _$E63

// FUNCTION: STBC 0x00800640 _$E220
// FUNCTION: STBC 0x00800660 _$E217
// DATA: STBC 0x9A21E0 ?rtti@NiMaterialProperty@@2VNiRTTI@@B
const NiRTTI NiMaterialProperty::rtti( "NiMaterialProperty", &NiProperty::rtti );

// DATA: STBC 0x978B3C
int NiMaterialProperty::nextKey = 1;

/*
================
NiMaterialProperty::Register
================
*/
// FUNCTION: STBC 0x800680 ?Register@NiMaterialProperty@@SAHXZ
NiImplementRegister( NiMaterialProperty )

// FUNCTION: STBC 0x008007d0 _$E226
// FUNCTION: STBC 0x008007f0 _$E223
// DATA: STBC 0x9A21F8
NiPointer<NiMaterialProperty> NiMaterialProperty::defaultProperty;

/*
===============================================================================

	NiMaterialPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiMaterialPropertyInit {
public:
	// FUNCTION: STBC 0x800850 ??0NiMaterialPropertyInit@@QAE@XZ
	NiMaterialPropertyInit() { NiMaterialProperty::defaultProperty = new NiMaterialProperty; }
	~NiMaterialPropertyInit() { NiMaterialProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00800830 _$E231
// FUNCTION: STBC 0x008008e0 _$E229
static NiMaterialPropertyInit init;

/*
================
NiMaterialProperty::NiMaterialProperty

Grey ambient and diffuse, black specular and emissive, shininess 4, opaque.
Every material gets a key of its own and starts at revision 1.
================
*/
// FUNCTION: STBC 0x800910
NiMaterialProperty::NiMaterialProperty() {
	ambient.r = ambient.g = ambient.b = 0.5f;
	diffuse.r = diffuse.g = diffuse.b = 0.5f;
	specular.r = specular.g = specular.b = 0.0f;
	emissive.r = emissive.g = emissive.b = 0.0f;
	shininess = 4.0f;
	alpha = 1.0f;
	key = nextKey++;
	SetChanged( true );
	revision = 1;
}

/*
================
NiMaterialProperty::CreateClone
================
*/
// FUNCTION: STBC 0x8009B0
NiObject *NiMaterialProperty::CreateClone() {
	NiMaterialProperty *clone = new NiMaterialProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiMaterialProperty::CopyMembers

The clone keeps its own key and revision.
================
*/
// FUNCTION: STBC 0x800A70
void NiMaterialProperty::CopyMembers( NiMaterialProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->ambient = ambient;
	dest->diffuse = diffuse;
	dest->specular = specular;
	dest->emissive = emissive;
	dest->shininess = shininess;
	dest->alpha = alpha;
}

/*
================
NiMaterialProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x800AE0
void NiMaterialProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMaterialProperty *material = new NiMaterialProperty;
	stream.SetLoadRecord( new NiMaterialPropertyLoadRecord );
	material->LoadBinary( stream );
	object = material;
}

/*
================
NiMaterialProperty::LoadBinary

A shininess of 1 or less is replaced by the default, 4.
================
*/
// FUNCTION: STBC 0x800BA0
void NiMaterialProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	ambient.LoadBinary( stream );
	diffuse.LoadBinary( stream );
	specular.LoadBinary( stream );
	emissive.LoadBinary( stream );
	NiStreamLoadBinary( stream, shininess );
	NiStreamLoadBinary( stream, alpha );
	if ( shininess <= 1.0f ) {
		shininess = 4.0f;
	}
}

/*
================
NiMaterialProperty::LinkObject
================
*/
// FUNCTION: STBC 0x800C10
void NiMaterialProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiMaterialProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x800C20
bool NiMaterialProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiMaterialProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x800C30
void NiMaterialProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	ambient.SaveBinary( stream );
	diffuse.SaveBinary( stream );
	specular.SaveBinary( stream );
	emissive.SaveBinary( stream );
	NiStreamSaveBinary( stream, shininess );
	NiStreamSaveBinary( stream, alpha );
}

/*
================
NiMaterialProperty::IsEqual
================
*/
// FUNCTION: STBC 0x800C90
bool NiMaterialProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiMaterialProperty *material = (NiMaterialProperty *)object;
	if ( ambient != material->ambient
		|| diffuse != material->diffuse
		|| specular != material->specular
		|| emissive != material->emissive
		|| shininess != material->shininess
		|| alpha != material->alpha ) {
		return false;
	}
	return true;
}

/*
================
NiMaterialProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x800D80
void NiMaterialProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( ambient.GetViewerString( "m_amb" ) );
	strings->Add( diffuse.GetViewerString( "m_diff" ) );
	strings->Add( specular.GetViewerString( "m_spec" ) );
	strings->Add( emissive.GetViewerString( "m_emit" ) );
	strings->Add( NiGetViewerString( "m_fShine", shininess ) );
	strings->Add( NiGetViewerString( "m_fAlpha", alpha ) );
}
