// NiDynamicEffectState.cpp -- the lights and texture effects in effect at a geometry object
//
// Gamebryo 1.2.2's file name. Retail keeps it in a unit of its own after
// NiPropertyState's (0x820E20-0x821440): cachedState's initialiser, the
// functions, the locale pair, the <iostream> quads and the registrations.

#include "NiRTLib.h"
#include "NiDynamicEffectState.h"
#include "NiDynamicEffect.h"
#include "NiTextureEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00821390 _$E31
// FUNCTION: STBC 0x008213a0 _$E42
// FUNCTION: STBC 0x008213b0 _$E46
// FUNCTION: STBC 0x008213c0 _$E50
// FUNCTION: STBC 0x008213d0 _$E54
// FUNCTION: STBC 0x008213e0 _$E177
// FUNCTION: STBC 0x008213f0 _$E182
// FUNCTION: STBC 0x00821400 _$E119
// FUNCTION: STBC 0x00821410 _$E132
// FUNCTION: STBC 0x00821420 _$E107
// FUNCTION: STBC 0x00821430 _$E186

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00821310 _$E204
// FUNCTION: STBC 0x00821330 _$E17
// FUNCTION: STBC 0x00821350 _$E15
// FUNCTION: STBC 0x00821360 _$E23
// FUNCTION: STBC 0x00821380 _$E21

// cachedState's exit-time release
// FUNCTION: STBC 0x00820e20 _$E189
// DATA: STBC 0x9A2C98
NiPointer<NiDynamicEffectState> NiDynamicEffectState::cachedState;

// A second exit-time release of the cached state: a file-scope object with
// only a destructor (retail registers it at 0x820E60, unguarded, and its exit
// 0x820E70 releases the state and clears the pointer). The class and the
// object are invented names (invented).
class NiDynamicEffectStateShutdown {
public:
	~NiDynamicEffectStateShutdown() { NiDynamicEffectState::cachedState = 0; }
};

// FUNCTION: STBC 0x00820e60 _$E197
// FUNCTION: STBC 0x00820e70 _$E195
static NiDynamicEffectStateShutdown effectStateShutdown;

/*
================
NiDynamicEffectState::~NiDynamicEffectState
================
*/
// FUNCTION: STBC 0x820EA0
NiDynamicEffectState::~NiDynamicEffectState() {
	ListDelete( lights );
	ListDelete( projectedLights );
	ListDelete( projectedShadows );
}

/*
================
NiDynamicEffectState::Copy
================
*/
// FUNCTION: STBC 0x820F00
NiDynamicEffectState *NiDynamicEffectState::Copy() const {
	NiDynamicEffectState *state = new NiDynamicEffectState;
	if ( lights ) {
		ListCopy( state->lights, lights );
	}
	if ( projectedLights ) {
		ListCopy( state->projectedLights, projectedLights );
	}
	if ( projectedShadows ) {
		ListCopy( state->projectedShadows, projectedShadows );
	}
	state->environmentMap = environmentMap;
	state->fogMap = fogMap;
	return state;
}

/*
================
NiDynamicEffectState::AddEffect

Files a light in the light list, and a texture effect in the list or
slot of its texture type.
================
*/
// FUNCTION: STBC 0x821020
void NiDynamicEffectState::AddEffect( NiDynamicEffect *effect ) {
	if ( effect ) {
		if ( effect->GetEffectType() <= NiDynamicEffect::SPOT_LIGHT ) {
			ListInsertSorted( lights, effect );
			return;
		}
		switch ( static_cast<NiTextureEffect *>( effect )->GetTextureType() ) {
			case NiTextureEffect::PROJECTED_LIGHT:
				ListInsertSorted( projectedLights, effect );
				break;
			case NiTextureEffect::PROJECTED_SHADOW:
				ListInsertSorted( projectedShadows, effect );
				break;
			case NiTextureEffect::ENVIRONMENT_MAP:
				environmentMap = static_cast<NiTextureEffect *>( effect );
				break;
			case NiTextureEffect::FOG_MAP:
				fogMap = static_cast<NiTextureEffect *>( effect );
				break;
		}
	}
}

/*
================
NiDynamicEffectState::RemoveEffect

Takes the effect out of the list or slot AddEffect filed it in.
================
*/
// FUNCTION: STBC 0x8210B0
void NiDynamicEffectState::RemoveEffect( NiDynamicEffect *effect ) {
	if ( effect ) {
		if ( effect->GetEffectType() <= NiDynamicEffect::SPOT_LIGHT ) {
			ListRemove( lights, effect );
			return;
		}
		switch ( static_cast<NiTextureEffect *>( effect )->GetTextureType() ) {
			case NiTextureEffect::PROJECTED_LIGHT:
				ListRemove( projectedLights, effect );
				break;
			case NiTextureEffect::PROJECTED_SHADOW:
				ListRemove( projectedShadows, effect );
				break;
			case NiTextureEffect::ENVIRONMENT_MAP:
				if ( effect == environmentMap ) {
					environmentMap = NULL;
				}
				break;
			case NiTextureEffect::FOG_MAP:
				if ( effect == fogMap ) {
					fogMap = NULL;
				}
				break;
		}
	}
}

/*
================
NiDynamicEffectState::Equal

False for NULL.
================
*/
// FUNCTION: STBC 0x821150 ?Equal@NiDynamicEffectState@@QBE_NPBV1@@Z
bool NiDynamicEffectState::Equal( const NiDynamicEffectState *state ) const {
	if ( !state ) {
		return false;
	}
	if ( environmentMap != state->environmentMap ) {
		return false;
	}
	if ( fogMap != state->fogMap ) {
		return false;
	}
	if ( !ListsEqual( lights, state->lights ) ) {
		return false;
	}
	if ( !ListsEqual( projectedLights, state->projectedLights ) ) {
		return false;
	}
	if ( !ListsEqual( projectedShadows, state->projectedShadows ) ) {
		return false;
	}
	return true;
}

/*
================
NiDynamicEffectState::ListInsertSorted
================
*/
// FUNCTION: STBC 0x821220
void NiDynamicEffectState::ListInsertSorted( ListItem *&list, NiDynamicEffect *effect ) {
	int index = effect->GetIndex();
	if ( !list || index < list->effect->GetIndex() ) {
		ListItem *item = new ListItem;
		item->effect = effect;
		item->next = list;
		list = item;
		return;
	}
	if ( index == list->effect->GetIndex() ) {
		return;
	}

	ListItem *prev = list;
	ListItem *item = list->next;
	while ( item && index > item->effect->GetIndex() ) {
		prev = item;
		item = item->next;
	}
	if ( item && index == item->effect->GetIndex() ) {
		return;
	}

	ListItem *newItem = new ListItem;
	newItem->effect = effect;
	newItem->next = item;
	prev->next = newItem;
}

/*
================
NiDynamicEffectState::ListRemove
================
*/
// FUNCTION: STBC 0x8212A0
void NiDynamicEffectState::ListRemove( ListItem *&list, NiDynamicEffect *effect ) {
	int index = effect->GetIndex();
	ListItem *item = list;
	if ( !item || index < item->effect->GetIndex() ) {
		return;
	}
	if ( index == item->effect->GetIndex() ) {
		list = item->next;
		delete item;
		return;
	}

	ListItem *prev = item;
	item = item->next;
	while ( item && index > item->effect->GetIndex() ) {
		prev = item;
		item = item->next;
	}
	if ( item && index == item->effect->GetIndex() ) {
		prev->next = item->next;
		delete item;
	}
}
