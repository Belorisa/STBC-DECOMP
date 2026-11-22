// NiD3DLightManager.cpp -- the renderer's light manager

#include "NiRTLib.h"
#include <math.h>
#include <float.h>

#include "NiD3DLightManager.h"
#include "NiAmbientLight.h"
#include "NiDirectionalLight.h"
#include "NiSpotLight.h"
#include "NiMath.h"
#include "NiLight.h"
#include "NiPointer.h"
#include "NiTextureModeProperty.h"
#include "NiDynamicEffectState.h"
#include "NiVertexColorProperty.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d1f10 _$E34
// FUNCTION: STBC 0x007d1f20 _$E45
// FUNCTION: STBC 0x007d1f30 _$E49
// FUNCTION: STBC 0x007d1f40 _$E53
// FUNCTION: STBC 0x007d1f50 _$E57
// FUNCTION: STBC 0x007d1f60 _$E107
// FUNCTION: STBC 0x007d1f70 _$E162
// FUNCTION: STBC 0x007d1f80 _$E177
// FUNCTION: STBC 0x007d1f90 _$E182
// FUNCTION: STBC 0x007d1fa0 _$E186
// FUNCTION: STBC 0x007d1fb0 _$E190
// FUNCTION: STBC 0x007d1fc0 _$E194
// FUNCTION: STBC 0x007d1fd0 _$E198
// FUNCTION: STBC 0x007d1fe0 _$E202

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d1ef0 _$E212
// FUNCTION: STBC 0x007d1df0 _$E17
// FUNCTION: STBC 0x007d1e10 _$E15
// FUNCTION: STBC 0x007d1e20 _$E23
// FUNCTION: STBC 0x007d1e40 _$E21

/*
================
NiD3DLightEntry::NiD3DLightEntry

Copies the light under its Direct3D light index.
================
*/
// FUNCTION: STBC 0x7D0F60
NiD3DLightEntry::NiD3DLightEntry( const NiLight &light ) {
	unknown68 = 0;
	index = light.GetIndex();
	Update( light );
}

/*
================
NiD3DLightEntry::Update

Copies the light's parameters into the Direct3D light when the light
changed since the last copy: its kind, place, direction, attenuation and
cone, and its colors scaled by its dimmer. True when they were copied.
================
*/
// FUNCTION: STBC 0x7D0F90
// PARKED: register: blocks agree (Gamebryo 1.2.2 NiDX8LightManager::LightEntry::Update's attenuation flag); retail reads the light's colors at fixed offsets from the light where this build holds each color's address in a register
// uiRevID, bAttenuation, pkSpot, kPos and kDir are Gamebryo 1.2.2's names, which the /Od frame order
// of the debug NetImmerse 3.1's copy (AnimOnly 0x5212A1) needs.
bool NiD3DLightEntry::Update( const NiLight &source ) {
	unsigned int uiRevID = source.GetRevision();

	bool bAttenuation = false;

	if ( unknown68 != uiRevID ) {
		unknown68 = uiRevID;

		memset( &light, 0, sizeof( light ) );
		int type = source.GetEffectType();
		assert( type != NiDynamicEffect::AMBIENT_LIGHT );
		switch ( type ) {
		case NiDynamicEffect::DIR_LIGHT: {
			light.dltType = D3DLIGHT_DIRECTIONAL;
			const NiPoint3 &kDir = ( (NiDirectionalLight *)&source )->GetWorldDirection();
			light.dvDirection.x = kDir.x;
			light.dvDirection.y = kDir.y;
			light.dvDirection.z = kDir.z;
			break;
		}
		case NiDynamicEffect::POINT_LIGHT: {
			light.dltType = D3DLIGHT_POINT;
			const NiPoint3 &kPos = ( (NiPointLight *)&source )->GetWorldTranslate();
			light.dvPosition.x = kPos.x;
			light.dvPosition.y = kPos.y;
			light.dvPosition.z = kPos.z;
			bAttenuation = true;
			break;
		}
		case NiDynamicEffect::SPOT_LIGHT: {
			light.dltType = D3DLIGHT_SPOT;
			NiSpotLight *pkSpot = (NiSpotLight *)&source;
			const NiPoint3 &kPos = pkSpot->GetWorldTranslate();
			const NiPoint3 &kDir = pkSpot->GetWorldDirection();
			light.dvPosition.x = kPos.x;
			light.dvPosition.y = kPos.y;
			light.dvPosition.z = kPos.z;
			light.dvDirection.x = kDir.x;
			light.dvDirection.y = kDir.y;
			light.dvDirection.z = kDir.z;
			light.dvTheta = light.dvPhi = pkSpot->GetSpotAngle() * NI_PI / 90.0f;
			light.dvFalloff = pkSpot->GetSpotExponent();
			bAttenuation = true;
			break;
		}
		}

		if ( bAttenuation ) {
			// only a point or a spot light sets it
			NiPointLight *point = (NiPointLight *)&source;
			light.dvAttenuation0 = point->GetConstantAttenuation();
			light.dvAttenuation1 = point->GetLinearAttenuation();
			light.dvAttenuation2 = point->GetQuadraticAttenuation();
			light.dvRange = D3DLIGHT_RANGE_MAX;
		}

		float dimmer = source.GetDimmer();
		const NiColor &ambient = source.GetAmbientColor() * dimmer;
		const NiColor &diffuse = source.GetDiffuseColor() * dimmer;
		const NiColor &specular = source.GetSpecularColor() * dimmer;
		light.dcvAmbient.r = ambient.r;
		light.dcvAmbient.g = ambient.g;
		light.dcvAmbient.b = ambient.b;
		light.dcvDiffuse.r = diffuse.r;
		light.dcvDiffuse.g = diffuse.g;
		light.dcvDiffuse.b = diffuse.b;
		light.dcvSpecular.r = specular.r;
		light.dcvSpecular.g = specular.g;
		light.dcvSpecular.b = specular.b;
		return true;
	}
	return false;
}

/*
================
NiD3DLightManager::NiD3DLightManager

Takes a reference to the device and turns its lighting off: no ambient
light, and every material color from the material.
================
*/
// FUNCTION: STBC 0x7D11A0
NiD3DLightManager::NiD3DLightManager( IDirect3DDevice7 *device_ ) : lights( 2 ) {
	device = device_;
	device->AddRef();

	ambient = 0;
	device->SetRenderState( D3DRENDERSTATE_AMBIENT, 0 );
	device->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	unknown34 = 0;
	lighting = false;
	unknown39 = false;
	materialSource = 0;
	device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
	device->SetRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
	device->SetRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );

	unknown2C = 0;
	unknown30 = false;
}

/*
================
NiD3DLightManager::~NiD3DLightManager

Releases the device, then frees every light entry and empties the light
list.
================
*/
// FUNCTION: STBC 0x7D12A0
NiD3DLightManager::~NiD3DLightManager() {
	device->Release();

	while ( entries.GetCount() ) {
		NiTMapIterator pos = entries.GetFirstPos();
		unsigned int index;
		NiD3DLightEntry *entry;
		entries.GetNext( pos, index, entry );
		entries.RemoveAt( index );
		delete entry;
	}

	while ( !lights.IsEmpty() ) {
		lights.RemoveHead();
	}
}

/*
================
NiD3DLightManager::DisableLights

Disables every light enabled on the device and empties the list.
================
*/
// FUNCTION: STBC 0x7D14B0
void NiD3DLightManager::DisableLights() {
	while ( lights.GetSize() ) {
		NiPointer<NiLight> light = lights.RemoveHead();
		device->LightEnable( light->GetIndex(), FALSE );
	}
	unknown2C = 0;
}

/*
================
NiD3DLightManager::UpdateLights

Disables the lights enabled on the device, then enables the switched-on
lights of an effect state, giving the device each light whose parameters
changed; the ambient lights only add up into the ambient color.
================
*/
// FUNCTION: STBC 0x7D1570
// Gamebryo 1.2.2: NiDX8LightManager::UpdateLights (the name); the statements are the debug NetImmerse 3.1's
// (AnimOnly 0x5218E2)
void NiD3DLightManager::UpdateLights( NiDynamicEffectState *effects ) {
	NiColorA ambientColor( 0.0f, 0.0f, 0.0f, 1.0f );

	while ( lights.GetSize() ) {
		NiPointer<NiLight> light = lights.RemoveHead();
		device->LightEnable( light->GetIndex(), FALSE );
	}

	if ( unknown39 ) {
		NiDynamicEffectState::ListItem *pos = 0;
		if ( effects ) {
			pos = effects->GetLightHeadPos();
		}
		while ( pos ) {
			NiLight *light = (NiLight *)effects->GetNextLight( pos );
			if ( !light->GetSwitch() || light->GetDimmer() < 0.01f ) {
				continue;
			}
			if ( light->GetEffectType() == NiDynamicEffect::AMBIENT_LIGHT ) {
				const NiColor &lightAmbient = light->GetAmbientColor();
				float dimmer = light->GetDimmer();
				ambientColor.r += lightAmbient.r * dimmer;
				ambientColor.g += lightAmbient.g * dimmer;
				ambientColor.b += lightAmbient.b * dimmer;
			} else {
				NiD3DLightEntry *entry = 0;
				bool changed = false;
				if ( entries.GetAt( light->GetIndex(), entry ) && entry ) {
					changed = entry->Update( *light );
				} else {
					entry = new NiD3DLightEntry( *light );
					entries.SetAt( light->GetIndex(), entry );
					changed = true;
				}
				if ( changed ) {
					device->SetLight( entry->GetIndex(), entry->GetD3DLight() );
				}
				device->LightEnable( entry->GetIndex(), TRUE );
				lights.AddHead( light );
			}
		}
	}

	D3DCOLOR color = RGBA_MAKE( (unsigned char)( ambientColor.r * 255.0f ), (unsigned char)( ambientColor.g * 255.0f ), (unsigned char)( ambientColor.b * 255.0f ), (unsigned char)( ambientColor.a * 255.0f ) );
	if ( ambient != color ) {
		device->SetRenderState( D3DRENDERSTATE_AMBIENT, color );
		ambient = color;
	}
}

/*
================
NiD3DLightManager::Unknown7D1950

Lighting is off for a replaced texture when the caller's flag is set, and
when the vertex colors are emissive but light nothing. Otherwise the
material sources follow the vertex color property, and the lights are set
again when the effect state or the vertex color lighting changed.
================
*/
// FUNCTION: STBC 0x7D1950
void NiD3DLightManager::Unknown7D1950( NiDynamicEffectState *effects, bool unknown, NiTextureModeProperty *textureMode, NiVertexColorProperty *vertexColor ) {
	unknown34 = textureMode->GetApplyMode();
	if ( unknown && unknown34 == NiTextureModeProperty::APPLY_REPLACE ) {
		if ( lighting ) {
			device->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
			lighting = false;
		}
		return;
	}

	bool lit = false;
	NiVertexColorProperty::SourceVertexMode source = vertexColor->GetSourceMode();
	if ( vertexColor->GetLightingMode() == NiVertexColorProperty::LIGHTING_E ) {
		switch ( source ) {
		case NiVertexColorProperty::SOURCE_IGNORE:
		case NiVertexColorProperty::SOURCE_AMB_DIFF:
			unknown39 = false;
			if ( materialSource == 1 ) {
				// the original sets the emissive source three times, where the
				// ambient and diffuse sources are meant
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
				materialSource = 0;
			}
			lit = true;
			break;
		case NiVertexColorProperty::SOURCE_EMISSIVE:
			// emissive vertex colors that light only the emissive term
			// leave nothing for the lights to do
			lit = false;
			break;
		}
	} else {
		if ( source != materialSource ) {
			switch ( source ) {
			case NiVertexColorProperty::SOURCE_IGNORE:
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
				break;
			case NiVertexColorProperty::SOURCE_EMISSIVE:
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1 );
				device->SetRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
				break;
			case NiVertexColorProperty::SOURCE_AMB_DIFF:
				device->SetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
				device->SetRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
				device->SetRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
				break;
			}
			materialSource = source;
		}
		unknown39 = true;
		lit = true;
	}

	if ( lit ) {
		if ( !lighting ) {
			device->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			lighting = true;
		}

		if ( effects != unknown2C || unknown39 != unknown30 ) {
			UpdateLights( effects );
			unknown2C = effects;
			unknown30 = unknown39;
		}
	} else if ( lighting ) {
		device->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		lighting = false;
	}
}

/*
================
NiD3DLightManager::RemoveLight

Forgets a light that is being deleted: its entry, its Direct3D light and
its place in the enabled list.
================
*/
// FUNCTION: STBC 0x7D1B00
// PARKED: retail calls DeleteItem (0x7D1ED0) out of line on RemovePos's tail and middle branches; this build inlines the middle one (inline budget, as in 0x6F11B0)
void NiD3DLightManager::RemoveLight( NiLight *light ) {
	NiD3DLightEntry *entry = 0;
	entries.GetAt( light->GetIndex(), entry );
	entries.RemoveAt( light->GetIndex() );
	delete entry;

	device->LightEnable( light->GetIndex(), FALSE );
	lights.Remove( light );
}
