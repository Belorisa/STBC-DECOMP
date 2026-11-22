// NiPropertyState.cpp -- the rendering properties in effect at a geometry object

#include "NiRTLib.h"
#include "NiPropertyState.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00820ae0 _$E161
// FUNCTION: STBC 0x00820a80 _$E17
// FUNCTION: STBC 0x00820aa0 _$E15
// FUNCTION: STBC 0x00820ab0 _$E23
// FUNCTION: STBC 0x00820ad0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00820b00 _$E30
// FUNCTION: STBC 0x00820b10 _$E41
// FUNCTION: STBC 0x00820b20 _$E45
// FUNCTION: STBC 0x00820b30 _$E49
// FUNCTION: STBC 0x00820b40 _$E53
// FUNCTION: STBC 0x00820b50 _$E58
// FUNCTION: STBC 0x00820b80 _$E63
// FUNCTION: STBC 0x00820bb0 _$E77
// FUNCTION: STBC 0x00820be0 _$E83
// FUNCTION: STBC 0x00820bf0 _$E123
// FUNCTION: STBC 0x00820c20 _$E88
// FUNCTION: STBC 0x00820c50 _$E100
// FUNCTION: STBC 0x00820c60 _$E113
// FUNCTION: STBC 0x00820c70 _$E133
// FUNCTION: STBC 0x00820ca0 _$E138
// FUNCTION: STBC 0x00820cd0 _$E148
// FUNCTION: STBC 0x00820d00 _$E153
// FUNCTION: STBC 0x00820d30 _$E68
// FUNCTION: STBC 0x00820d60 _$E143
// FUNCTION: STBC 0x00820d90 _$E128
// FUNCTION: STBC 0x00820dc0 _$E118
// FUNCTION: STBC 0x00820dd0 _$E93

// DATA: STBC 0x9A2C70 ?states@NiPropertyState@@1IA
unsigned int NiPropertyState::states;

/*
================
NiPropertyState::NiPropertyState

Starts with every property class's default.
================
*/
// FUNCTION: STBC 0x820460 ??0NiPropertyState@@QAE@XZ
NiPropertyState::NiPropertyState() {
	Reset();
	refCount = 0;
	states++;
}

/*
================
NiPropertyState::NiPropertyState

Shares the properties of state; the copy starts unreferenced.
================
*/
// FUNCTION: STBC 0x8204E0 ??0NiPropertyState@@QAE@ABV0@@Z
NiPropertyState::NiPropertyState( const NiPropertyState &state ) {
	for ( int i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		properties[i] = state.properties[i];
	}
	refCount = 0;
	states++;
}

/*
================
NiPropertyState::~NiPropertyState
================
*/
// FUNCTION: STBC 0x820590 ??1NiPropertyState@@QAE@XZ
NiPropertyState::~NiPropertyState() {
	states--;
}

/*
================
NiPropertyState::Reset

Puts each property class's default in its slot, in the order of the class
names.
================
*/
// FUNCTION: STBC 0x8205B0 ?Reset@NiPropertyState@@QAEXXZ
void NiPropertyState::Reset() {
	properties[NiAlphaProperty::GetType()] = NiAlphaProperty::GetDefault();
	properties[NiCorrectionProperty::GetType()] = NiCorrectionProperty::GetDefault();
	properties[NiDitherProperty::GetType()] = NiDitherProperty::GetDefault();
	properties[NiFogProperty::GetType()] = NiFogProperty::GetDefault();
	properties[NiMaterialProperty::GetType()] = NiMaterialProperty::GetDefault();
	properties[NiMultiTextureProperty::GetType()] = NiMultiTextureProperty::GetDefault();
	properties[NiShadeProperty::GetType()] = NiShadeProperty::GetDefault();
	properties[NiSpecularProperty::GetType()] = NiSpecularProperty::GetDefault();
	properties[NiStencilProperty::GetType()] = NiStencilProperty::GetDefault();
	properties[NiTextureModeProperty::GetType()] = NiTextureModeProperty::GetDefault();
	properties[NiTextureProperty::GetType()] = NiTextureProperty::GetDefault();
	properties[NiTransparentProperty::GetType()] = NiTransparentProperty::GetDefault();
	properties[NiVertexColorProperty::GetType()] = NiVertexColorProperty::GetDefault();
	properties[NiWireframeProperty::GetType()] = NiWireframeProperty::GetDefault();
	properties[NiZBufferProperty::GetType()] = NiZBufferProperty::GetDefault();
}

/*
================
NiPropertyState::operator=

Takes the properties of state; the reference count stays.
================
*/
// FUNCTION: STBC 0x820930 ??4NiPropertyState@@QAEAAV0@ABV0@@Z
NiPropertyState &NiPropertyState::operator=( const NiPropertyState &state ) {
	for ( int i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		properties[i] = state.properties[i];
	}
	return *this;
}

/*
================
NiPropertyState::GetProperties

Copies the properties into an array of MAX_TYPES pointers, without
references.
================
*/
// FUNCTION: STBC 0x820980 ?GetProperties@NiPropertyState@@QBEXPAPAVNiProperty@@@Z
void NiPropertyState::GetProperties( NiProperty **properties_ ) const {
	for ( int i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		properties_[i] = properties[i];
	}
}

/*
================
NiPropertyState::GetMaterialAndRevise

Returns the material, first giving it a new revision if it changed since
the last call, so the renderer refreshes its cached copy.
================
*/
// FUNCTION: STBC 0x8209A0 ?GetMaterialAndRevise@NiPropertyState@@QBEPAVNiMaterialProperty@@XZ
NiMaterialProperty *NiPropertyState::GetMaterialAndRevise() const {
	NiMaterialProperty *material = GetMaterial();
	if ( material->GetChanged() ) {
		material->SetChanged( false );
		material->IncRevisionID();
	}
	return material;
}
