// NiPick.cpp -- ray picks through the scene graph

#include "NiRTLib.h"
#include "NiPick.h"
#include "NiAVObject.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00812240 _$E42
// FUNCTION: STBC 0x00812250 _$E53
// FUNCTION: STBC 0x00812260 _$E57
// FUNCTION: STBC 0x00812270 _$E61
// FUNCTION: STBC 0x00812280 _$E65
// FUNCTION: STBC 0x00812290 _$E180
// FUNCTION: STBC 0x008122a0 _$E185
// FUNCTION: STBC 0x008122b0 _$E189

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00812220 _$E196
// FUNCTION: STBC 0x008121c0 _$E17
// FUNCTION: STBC 0x008121e0 _$E15
// FUNCTION: STBC 0x008121f0 _$E23
// FUNCTION: STBC 0x00812210 _$E21

/*
================
NiPick::NiPick

A pick that finds every triangle the ray hits, sorted, in world
coordinates, front faces only.
================
*/
// FUNCTION: STBC 0x811CE0
NiPick::NiPick() {
	pickType = FIND_ALL;
	sortType = SORT;
	intersectType = TRIANGLE_INTERSECT;
	coordinateType = WORLD_COORDINATES;
	frontOnly = true;
	observeAppCullFlag = false;
	unknown12 = false;
	root = 0;
	returnTexture = false;
	returnNormal = false;
	returnSmoothNormal = false;
	returnColor = false;
	lastAdded = 0;
}

/*
================
NiPick::~NiPick
================
*/
// FUNCTION: STBC 0x811D90
NiPick::~NiPick() {
	ClearResultsArray();
}

/*
================
NiPick::ClearResultsArray
================
*/
// FUNCTION: STBC 0x811E70
void NiPick::ClearResultsArray() {
	if ( results.GetSize() ) {
		for ( unsigned int i = 0; i < results.GetSize(); i++ ) {
			Record *record = results.RemoveAt( i );
			delete record;
		}
	}
}

/*
================
NiPick::PickObjects

The root's FindIntersections files the hits; a bubble sort orders them,
since there are few. Gamebryo 1.2.2: NiPick::PickObjects, which reaches the
root through NiCollisionTraversals instead.
================
*/
// FUNCTION: STBC 0x811ED0
bool NiPick::PickObjects( const NiPoint3 &origin, const NiPoint3 &dir, bool append ) {
	if ( !append ) {
		ClearResultsArray();
	}

	if ( !root->FindIntersections( origin, dir, *this ) ) {
		return false;
	}
	if ( !results.GetEffectiveSize() ) {
		return false;
	}

	// a bound pick ignores the sort type
	if ( sortType == NO_SORT || intersectType == BOUND_INTERSECT ) {
		return true;
	}

	if ( results.GetEffectiveSize() > 1 ) {
		unsigned int size = results.GetSize();
		unsigned int i;
		Record *record0;
		Record *record1;
		Record *save;

		if ( pickType == FIND_ALL ) {
			// nearest first
			for ( i = 0; i < size; i++ ) {
				record0 = results.GetAt( i );
				if ( !record0 ) {
					continue;
				}

				for ( unsigned int j = i + 1; j < size; j++ ) {
					record1 = results.GetAt( j );
					if ( !record1 ) {
						continue;
					}

					if ( record1->GetDistance() < record0->GetDistance() ) {
						save = record0;
						record0 = record1;
						record1 = save;
						results.SetAt( i, record0 );
						results.SetAt( j, record1 );
					}
				}
			}
		} else {
			// keep only the nearest
			record0 = results.GetAt( 0 );
			for ( i = 1; i < size; i++ ) {
				record1 = results.RemoveAt( i );
				if ( record1 ) {
					if ( record1->GetDistance() < record0->GetDistance() ) {
						save = record0;
						record0 = record1;
						delete save;
					} else {
						delete record1;
					}
				}
			}
			results.SetAt( 0, record0 );
		}
	}

	return true;
}
