/*
===============================================================================

	NiD3DMaterialManager.h

	The render state manager's material cache: the Direct3D material made
	from each material property, and the material last set on the device.

	Evidence: NiD3DRenderState's constructor (0x7D1FF0) builds it at 0x1CC
	through the out-of-line constructor 0x7D7DC0, which builds a hash map at
	0x00 (vtable 0x898B80, 37 buckets), then adds an entry for the default
	material property (the static at 0x9A21F8) and makes it current at 0x10,
	setting the flag at 0x14. An entry (0x48 bytes) is a D3DMATERIAL7
	followed by the revision of the property it was made from; 0x7D7D40
	refills it when the property's revision (at 0x54) changed: the ambient
	(0x1C), diffuse (0x28), specular (0x34) and emissive (0x40) colors, the
	shininess (0x4C) as the power and the alpha (0x50) in each color. The
	destructor is 0x7D7F80. The class names are invented from that use.

	This unit instantiates NiTMap<unsigned int, NiD3DMaterialEntry *> (vtable
	0x898B80): ~NiTMap (0x7D8260), the scalar deleting destructor (0x7D8330),
	KeyToHashIndex (0x7D82D0), IsKeysEqual (0x7D82E0), SetValue (0x7D8300) and
	ClearValue (0x7D8320); their annotations belong above the template
	definitions in NiTMap.h. Apply (0x7D8240) is the unit's only function
	without a material property.

===============================================================================
*/

#ifndef __NID3DMATERIALMANAGER_H__
#define __NID3DMATERIALMANAGER_H__

#include "NiTMap.h"
#include "NiD3D7.h"

class NiMaterialProperty;

// a material as the device takes it (name invented)
class NiD3DMaterialEntry {
public:
							NiD3DMaterialEntry( NiMaterialProperty *property ) { revision = 0; Update( property ); }

	// copies the property's colors when its revision changed; true when
	// they were copied (name invented)
	bool					Update( NiMaterialProperty *property );

	D3DMATERIAL7			material;		// passed to SetMaterial
	unsigned int			revision;		// the property's revision it was made from
};

class NiD3DMaterialManager {
public:
							NiD3DMaterialManager();
							~NiD3DMaterialManager();

	// sets the current material on the device when it changed (name
	// invented)
	void					Apply( IDirect3DDevice7 *device );
	// makes the property's material the current one (name invented)
	void					SetCurrent( NiMaterialProperty *property );

	NiTMap<unsigned int, NiD3DMaterialEntry *>	entries;
	NiD3DMaterialEntry *	current;		// the material set on the device
	bool					changed;		// current changed since it was last set
};

#endif /* !__NID3DMATERIALMANAGER_H__ */
