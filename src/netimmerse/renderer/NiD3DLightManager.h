/*
===============================================================================

	NiD3DLightManager.h

	The renderer's light manager: it maps scene graph lights to Direct3D
	light indices and keeps the device's lighting render states.

	Evidence: the renderer's creation (0x7C3C35) allocates 0x44 bytes and
	runs the constructor (0x7D11A0), which builds a hash map at 0x00 (vtable
	0x898B0C, 37 buckets) and a pooled list at 0x10 (two items per block),
	takes a reference to the device at 0x28 and resets the lighting render
	states; NiD3DRender's slot 40 hands it the objects that are NiLights
	(NiRTTI 0x9A1BF8); the destructor is 0x7D12A0. The class name is
	invented from that use.

	The map is keyed by the Direct3D light index each light carries at 0xCC
	and holds the light as SetLight (IDirect3DDevice7 slot 0x48) takes it:
	0x7D1570 allocates a 0x70-byte entry, fills it through 0x7D0F90 and
	passes it to SetLight with the index at 0x6C. The list holds the lights
	enabled on the device (LightEnable, slot 0xB0): 0x7D14B0 disables and
	unlinks them all at the start of each frame.

	This unit instantiates NiTMap<unsigned int, NiD3DLightEntry *> (vtable
	0x898B0C): KeyToHashIndex (0x7D1D50), IsKeysEqual (0x7D1D60), SetValue
	(0x7D1D80), ClearValue (0x7D1DA0) and the scalar deleting destructor
	(0x7D1E50); their annotations belong above the template definitions in
	NiTMap.h.

===============================================================================
*/

#ifndef __NID3DLIGHTMANAGER_H__
#define __NID3DLIGHTMANAGER_H__

#include "NiTMap.h"
#include "NiTList.h"
#include "NiD3D7.h"
#include "NiLight.h"

class NiDynamicEffectState;
class NiTextureModeProperty;
class NiVertexColorProperty;

// a light as the device knows it (name invented)
class NiD3DLightEntry {
public:
							NiD3DLightEntry( const NiLight &light );

	D3DLIGHT7				light;			// the light passed to SetLight
	unsigned int			unknown68;		// 0 when the entry is created
	unsigned int			index;			// the Direct3D light index

	// copies a light's parameters; true when they changed (Gamebryo 1.1.2:
	// NiDX8LightManager::LightEntry::Update)
	bool					Update( const NiLight &source );

	// Gamebryo 1.2.2 NiDX8LightManager::LightEntry's accessors
	D3DLIGHT7 *				GetD3DLight() { return &light; }
	int						GetIndex() const { return index; }
};

class NiD3DLightManager {
public:
							NiD3DLightManager( IDirect3DDevice7 *device_ );
							~NiD3DLightManager();

	void					DisableLights();				// at the start of each frame (name invented)
	void					RemoveLight( NiLight *light );	// the light is being deleted (name invented)
	void					UpdateLights( NiDynamicEffectState *effects );	// sets the device's lights and ambient color
	// sets the lighting render states for an object's properties, and its
	// lights when they or the material sources changed
	void					Unknown7D1950( NiDynamicEffectState *effects, bool unknown, NiTextureModeProperty *textureMode, NiVertexColorProperty *vertexColor );

	NiTMap<unsigned int, NiD3DLightEntry *>	entries;	// by light index
	NiTList<NiLight *>		lights;			// the lights enabled on the device
	IDirect3DDevice7 *		device;			// referenced at construction
	NiDynamicEffectState *	unknown2C;		// the effect state whose lights are set (0x7D1950)
	bool					unknown30;		// the unknown39 that went with unknown2C
	unsigned int			unknown34;		// a property's flags & 7
	bool					lighting;		// D3DRENDERSTATE_LIGHTING as set on the device
	bool					GetLighting() const { return lighting; }

	bool					unknown39;
	unsigned int			materialSource;	// which vertex color the material source states take (0-2)
	D3DCOLOR				ambient;		// D3DRENDERSTATE_AMBIENT as set on the device
};

#endif /* !__NID3DLIGHTMANAGER_H__ */
