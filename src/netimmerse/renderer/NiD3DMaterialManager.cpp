// NiD3DMaterialManager.cpp -- the render state manager's material cache

#include "NiRTLib.h"
#include "NiD3DMaterialManager.h"
#include "NiMaterialProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d83b0 _$E70
// FUNCTION: STBC 0x007d83d0 _$E17
// FUNCTION: STBC 0x007d83f0 _$E15
// FUNCTION: STBC 0x007d8400 _$E23
// FUNCTION: STBC 0x007d8420 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d8440 _$E45
// FUNCTION: STBC 0x007d8450 _$E49
// FUNCTION: STBC 0x007d8460 _$E53
// FUNCTION: STBC 0x007d8470 _$E57
// FUNCTION: STBC 0x007d8480 _$E63
// FUNCTION: STBC 0x007d84b0 _$E34

/*
================
NiD3DMaterialEntry::Update

Every color takes the property's alpha.
================
*/
// FUNCTION: STBC 0x7D7D40
bool NiD3DMaterialEntry::Update( NiMaterialProperty *property ) {
	if ( revision != property->GetRevisionID() ) {
		revision = property->GetRevisionID();

		float alpha = property->GetAlpha();
		const NiColor &emissive = property->GetEmittance();
		material.dcvEmissive.r = emissive.r;
		material.dcvEmissive.g = emissive.g;
		material.dcvEmissive.b = emissive.b;
		material.dcvEmissive.a = alpha;
		const NiColor &ambient = property->GetAmbientColor();
		material.dcvAmbient.r = ambient.r;
		material.dcvAmbient.g = ambient.g;
		material.dcvAmbient.b = ambient.b;
		material.dcvAmbient.a = alpha;
		const NiColor &diffuse = property->GetDiffuseColor();
		material.dcvDiffuse.r = diffuse.r;
		material.dcvDiffuse.g = diffuse.g;
		material.dcvDiffuse.b = diffuse.b;
		material.dcvDiffuse.a = alpha;
		const NiColor &specular = property->GetSpecularColor();
		material.dcvSpecular.r = specular.r;
		material.dcvSpecular.g = specular.g;
		material.dcvSpecular.b = specular.b;
		material.dcvSpecular.a = alpha;
		material.dvPower = property->GetShineness();
		return true;
	}
	return false;
}

/*
================
NiD3DMaterialManager::NiD3DMaterialManager

Starts with the default material property's entry current.
================
*/
// FUNCTION: STBC 0x7D7DC0
NiD3DMaterialManager::NiD3DMaterialManager() {
	current = 0;
	SetCurrent( NiMaterialProperty::GetDefault() );
	changed = true;
}

/*
================
NiD3DMaterialManager::~NiD3DMaterialManager

Frees every material entry.
================
*/
// FUNCTION: STBC 0x7D7F80
NiD3DMaterialManager::~NiD3DMaterialManager() {
	while ( entries.GetCount() ) {
		NiTMapIterator pos = entries.GetFirstPos();
		unsigned int key;
		NiD3DMaterialEntry *entry;
		entries.GetNext( pos, key, entry );
		entries.RemoveAt( key );
		delete entry;
	}
}

/*
================
NiD3DMaterialManager::SetCurrent

Finds or makes the property's entry, by its key, and brings it up to date.
================
*/
// FUNCTION: STBC 0x7D8130
void NiD3DMaterialManager::SetCurrent( NiMaterialProperty *property ) {
	NiD3DMaterialEntry *entry;
	if ( !entries.GetAt( property->GetKey(), entry ) ) {
		entry = new NiD3DMaterialEntry( property );
		entries.SetAt( property->GetKey(), entry );
	}
	if ( entry->Update( property ) ) {
		changed = true;
	}
	if ( entry != current ) {
		changed = true;
		current = entry;
	}
}

/*
================
NiD3DMaterialManager::Apply
================
*/
// FUNCTION: STBC 0x7D8240
void NiD3DMaterialManager::Apply( IDirect3DDevice7 *device ) {
	if ( changed ) {
		changed = false;
		device->SetMaterial( &current->material );
	}
}
