/*
===============================================================================

	NiDynamicEffectState.h

	The dynamic effects that reach a geometry object, gathered on the way
	down the scene graph: the lights, sorted by index, the projected lights
	and shadows, and at most one environment map and one fog map. Shared
	through reference counting.

	Evidence: the vtable at 0x898FE0 (one slot, the scalar deleting
	destructor 0x7E67A0); the destructor (0x820EA0) frees the three lists
	at 0x08, 0x0C and 0x10; the copy (0x820F00) allocates 0x1C bytes,
	clears the count at 0x04 and copies the lists item by item and the two
	pointers at 0x14 and 0x18; AddEffect (0x821020) files a light (effect
	type 0-3) into the list at 0x08 and a texture effect by its texture type
	(0x18C) into 0x0C, 0x10, 0x14 or 0x18. NiGeometry holds one at 0xCC and
	NiAVObject::UpdateEffects (0x7DC950) builds it. The class and member
	names follow the Gamebryo lineage (NiDynamicEffectState, the light,
	projected light and projected shadow lists, the environment and fog
	maps), which keeps this layout.

===============================================================================
*/

#ifndef __NIDYNAMICEFFECTSTATE_H__
#define __NIDYNAMICEFFECTSTATE_H__

#include "NiPointer.h"

class NiDynamicEffect;
class NiTextureEffect;

// FUNCTION: STBC 0x007E67A0 ??_GNiDynamicEffectState@@UAEPAXI@Z @netimmerse/NiNode
class NiDynamicEffectState {
public:
	// an item of an effect list
	struct ListItem {
		ListItem *			next;
		NiDynamicEffect *	effect;
	};

							NiDynamicEffectState() { refCount = 0; lights = 0; projectedLights = 0; projectedShadows = 0; environmentMap = 0; fogMap = 0; }
	virtual					~NiDynamicEffectState();

	void					IncRefCount() { refCount++; }
	int						DecRefCount();

	// files the effect in its list; RemoveEffect takes it out again
	void					AddEffect( NiDynamicEffect *effect );
	void					RemoveEffect( NiDynamicEffect *effect );

	// a new state with the same effects
	NiDynamicEffectState *	Copy() const;

	// the same effects in the same order
	bool					Equal( const NiDynamicEffectState *state ) const;

	// walking the lights in index order (Gamebryo lineage names):
	// GetNextLight returns NULL at the end
	ListItem *				GetLightHeadPos() const { return lights; }
	NiDynamicEffect *		GetNextLight( ListItem *&pos ) const;

	// walking the projected lights and shadows, the same way (Gamebryo
	// lineage names); the list items hold texture effects only, and
	// GetNextProjLight and GetNextProjShadow are defined in
	// NiTextureEffect.h, where that class is complete
	ListItem *				GetProjLightHeadPos() const { return projectedLights; }
	ListItem *				GetProjShadowHeadPos() const { return projectedShadows; }
	NiTextureEffect *		GetNextProjLight( ListItem *&pos ) const;
	NiTextureEffect *		GetNextProjShadow( ListItem *&pos ) const;

	// the environment and fog maps, or NULL (Gamebryo lineage names)
	NiTextureEffect *		GetEnvironmentMap() const { return environmentMap; }
	NiTextureEffect *		GetFogMap() const { return fogMap; }

	// the state the last geometry object took: the next object takes it
	// again when its own state is equal (Gamebryo 1.1.2's ms_spCachedState)
	static NiPointer<NiDynamicEffectState>	cachedState;
	// the cached state and setting it (Gamebryo 1.1.2 names,
	// NiDynamicEffectState.inl)
	static NiDynamicEffectState *	GetCachedState() { return cachedState; }
	static void				SetCachedState( NiDynamicEffectState *state ) { cachedState = state; }

protected:
	// a list sorted by effect index: ListInsertSorted ignores an index already
	// listed, ListRemove takes out the item with the effect's index (names:
	// Gamebryo 1.1.2, NiDynamicEffectState.h)
	static void				ListInsertSorted( ListItem *&list, NiDynamicEffect *effect );
	static void				ListRemove( ListItem *&list, NiDynamicEffect *effect );

	// whole lists (Gamebryo 1.1.2 names)
	static void				ListDelete( ListItem *&list );
	static void				ListCopy( ListItem *&dest, const ListItem *source );
	static bool				ListsEqual( const ListItem *list0, const ListItem *list1 );

	int						refCount;
	ListItem *				lights;				// sorted by effect index
	ListItem *				projectedLights;	// texture effects
	ListItem *				projectedShadows;	// texture effects
	NiTextureEffect *		environmentMap;
	NiTextureEffect *		fogMap;
};

/*
================
NiDynamicEffectState::DecRefCount

Releases a reference; the last one deletes the state.
================
*/
// FUNCTION: STBC 0x004e1580 ?DecRefCount@NiDynamicEffectState@@QAEHXZ @game/Glow
inline int NiDynamicEffectState::DecRefCount() {
	int count = --refCount;
	if ( count == 0 ) {
		delete this;
	}
	return count;
}

/*
================
NiDynamicEffectState::GetNextLight

Returns the light at pos and advances pos.

Evidence: the renderer's light pass (0x7D1570) walks the list with this
null-checked element and next sequence.
================
*/
inline NiDynamicEffect *NiDynamicEffectState::GetNextLight( ListItem *&pos ) const {
	if ( !pos ) {
		return 0;
	}
	NiDynamicEffect *light = pos->effect;
	pos = pos->next;
	return light;
}

/*
================
NiDynamicEffectState::ListDelete
================
*/
inline void NiDynamicEffectState::ListDelete( ListItem *&list ) {
	while ( list ) {
		ListItem *item = list;
		list = item->next;
		delete item;
	}
}

/*
================
NiDynamicEffectState::ListCopy

Builds dest as a copy of the non-empty list source.
================
*/
inline void NiDynamicEffectState::ListCopy( ListItem *&dest, const ListItem *source ) {
	dest = new ListItem;
	dest->effect = source->effect;
	ListItem *item = dest;
	while ( source->next ) {
		item->next = new ListItem;
		source = source->next;
		item = item->next;
		item->effect = source->effect;
	}
	item->next = 0;
}

/*
================
NiDynamicEffectState::ListsEqual

The same effects in the same order.
================
*/
inline bool NiDynamicEffectState::ListsEqual( const ListItem *list0, const ListItem *list1 ) {
	while ( list0 ) {
		if ( !list1 || list0->effect != list1->effect ) {
			return false;
		}
		list0 = list0->next;
		list1 = list1->next;
	}
	if ( list1 ) {
		return false;
	}
	return true;
}

#endif /* !__NIDYNAMICEFFECTSTATE_H__ */
