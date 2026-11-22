/*
===============================================================================

	NiPropertyState.h

	The rendering properties in effect at a geometry object, one per
	property type, gathered on the way down the scene graph. Shared through
	reference counting; there is no vtable.

	Evidence: the constructor (0x820460) builds 15 smart pointers at 0x04
	(vector constructor, NiPointer<NiProperty>'s destructor 0x416EC0), calls
	Reset (0x8205B0), clears the count at 0x00 and counts the new state in
	the static at 0x9A2C70; NiAVObject::UpdateProperties (0x7DC690)
	allocates 0x40 bytes for one; the releases call the destructor
	(0x820590) and free the block when the count at 0x00 reaches zero.
	Reset stores each property class's default in its type's slot. The
	class name follows the Gamebryo lineage (NiPropertyState), which keeps
	this layout.

===============================================================================
*/

#ifndef __NIPROPERTYSTATE_H__
#define __NIPROPERTYSTATE_H__

#include "NiPointer.h"
#include "NiProperty.h"
#include "NiAlphaProperty.h"
#include "NiCorrectionProperty.h"
#include "NiDitherProperty.h"
#include "NiFogProperty.h"
#include "NiMaterialProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiShadeProperty.h"
#include "NiSpecularProperty.h"
#include "NiStencilProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiTransparentProperty.h"
#include "NiVertexColorProperty.h"
#include "NiWireframeProperty.h"
#include "NiZBufferProperty.h"

// FUNCTION: STBC 0x004e15d0 ??_GNiPropertyState@@QAEPAXI@Z @game/Glow
class NiPropertyState {
public:
							NiPropertyState();
							NiPropertyState( const NiPropertyState &state );
							~NiPropertyState();

	NiPropertyState &		operator=( const NiPropertyState &state );

	void					IncRefCount() { refCount++; }
	int						DecRefCount();

	// every slot to its property class's default
	void					Reset();

	// the property of each type, indexed by NiProperty::Type
	NiPointer<NiProperty> &	operator[]( int type ) { return properties[type]; }
	// puts the property into the slot of its type (Gamebryo lineage name)
	void					SetProperty( NiProperty *property ) {
		// RULE-EXCEPTION: R35, Gamebryo 1.1.2's NIPROPERTYSTATE.INL text: the
		// early return has no braces; braced, it costs two more tuples and
		// NiFrameRate::Update keeps the texture mode's release out of line
		if ( !property )
			return;

		properties[property->Type()] = property;
	}
	void					GetProperties( NiProperty **properties_ ) const;

	// the property of each type (Gamebryo lineage names)
	NiAlphaProperty *	GetAlpha() const { return (NiAlphaProperty *)(NiProperty *)properties[NiAlphaProperty::ALPHA]; }
	NiCorrectionProperty *	GetCorrection() const { return (NiCorrectionProperty *)(NiProperty *)properties[NiCorrectionProperty::CORRECTION]; }
	NiDitherProperty *	GetDither() const { return (NiDitherProperty *)(NiProperty *)properties[NiDitherProperty::DITHER]; }
	NiFogProperty *	GetFog() const { return (NiFogProperty *)(NiProperty *)properties[NiFogProperty::FOG]; }
	NiMaterialProperty *	GetMaterial() const { return (NiMaterialProperty *)(NiProperty *)properties[NiMaterialProperty::MATERIAL]; }
	NiMultiTextureProperty *	GetMultiTexture() const { return (NiMultiTextureProperty *)(NiProperty *)properties[NiMultiTextureProperty::MULTI_TEXTURE]; }
	NiShadeProperty *	GetShade() const { return (NiShadeProperty *)(NiProperty *)properties[NiShadeProperty::SHADE]; }
	NiSpecularProperty *	GetSpecular() const { return (NiSpecularProperty *)(NiProperty *)properties[NiSpecularProperty::SPECULAR]; }
	NiStencilProperty *	GetStencil() const { return (NiStencilProperty *)(NiProperty *)properties[NiStencilProperty::STENCIL]; }
	NiTextureModeProperty *	GetTextureMode() const { return (NiTextureModeProperty *)(NiProperty *)properties[NiTextureModeProperty::TEXTURE_MODE]; }
	NiTextureProperty *	GetTexture() const { return (NiTextureProperty *)(NiProperty *)properties[NiTextureProperty::TEXTURE]; }
	NiTransparentProperty *	GetTransparent() const { return (NiTransparentProperty *)(NiProperty *)properties[NiTransparentProperty::TRANSPARENCY]; }
	NiVertexColorProperty *	GetVertexColor() const { return (NiVertexColorProperty *)(NiProperty *)properties[NiVertexColorProperty::VERTEX_COLOR]; }
	NiWireframeProperty *	GetWireframe() const { return (NiWireframeProperty *)(NiProperty *)properties[NiWireframeProperty::WIREFRAME]; }
	NiZBufferProperty *	GetZBuffer() const { return (NiZBufferProperty *)(NiProperty *)properties[NiZBufferProperty::ZBUFFER]; }

	// the material; a change to it since the last call (flag bit 0) is
	// consumed and counted in its revision (name invented)
	NiMaterialProperty *	GetMaterialAndRevise() const;

	// exchanges each property of the list with the slot of its type
	// (Gamebryo lineage: NIPROPERTYSTATE.INL)
	void					SwapProperties( NiPropertyList &list );


protected:
	int						refCount;
	NiPointer<NiProperty>	properties[NiProperty::MAX_TYPES];

	static unsigned int		states;			// live NiPropertyStates
};

/*
================
NiPropertyState::DecRefCount

Releases a reference; the last one deletes the state.
================
*/
// FUNCTION: STBC 0x004e15a0 ?DecRefCount@NiPropertyState@@QAEHXZ @game/Glow
inline int NiPropertyState::DecRefCount() {
	int count = --refCount;
	if ( count == 0 ) {
		delete this;
	}
	return count;
}

/*
================
NiPropertyState::SwapProperties

Puts each property of the list into the slot of its type and the slot's
old property into the list in its place. Gamebryo 1.1.2 writes it in
NIPROPERTYSTATE.INL over its NiTList; BC's list is the older chain of
nodes, each holding one property.
================
*/
inline void NiPropertyState::SwapProperties( NiPropertyList &list ) {
	if ( list.IsEmpty() ) {
		return;
	}

	for ( NiPropertyList *node = &list; node != NULL; node = node->next ) {
		NiPointer<NiProperty> property = node->GetItem();
		int type = property->Type();

		// swap in the new property and keep the old one
		node->SetItem( properties[type] );
		properties[type] = property;
	}
}

#endif /* !__NIPROPERTYSTATE_H__ */
