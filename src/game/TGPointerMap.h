/*
===============================================================================

	TGPointerMap.h

	The objects of a game being loaded, by the address they had when the
	game was saved. An object's ReadFromStream files itself under the
	address it reads, and FixUpPointers and PostFixUp turn the saved
	addresses of the objects it points to back into pointers. The objects
	module clears it once the load is over.

	The class and its names are invented: no string names them.

	Evidence: the one instance (0x99A418) is a NiTMap of 37 buckets
	(vtable 0x896208, hooks 0x6EFCD0 to 0x6EFD20) held as the first and
	only member: its constructor (0x6EF3A0) builds the map, then clears it
	under an EH state, and its destructor (0x6EF480) clears it before the
	map's own destructor runs (the map's vtable stored again between the
	two). Two hundred and fifty functions inline SetAt and GetAt on it; the
	objects module's slot 1 (0x6EF7C0) inlines RemoveAll. The keys are read
	with the stream's integer read; the values are of every type.

===============================================================================
*/

#ifndef __TGPOINTERMAP_H__
#define __TGPOINTERMAP_H__

#include "NiTMap.h"

class TGPointerMap {
public:
	// FUNCTION: STBC 0x006ef3a0 ??0TGPointerMap@@QAE@XZ
					TGPointerMap( void ) { RemoveAll(); }
	// FUNCTION: STBC 0x006ef480 ??1TGPointerMap@@QAE@XZ
					~TGPointerMap( void ) { RemoveAll(); }

	// Files the object under its saved address.
	void			Add( long savedAddress, void *object ) { map.SetAt( savedAddress, object ); }
	// The object of a saved address; NULL when none was filed.
	void *			Find( long savedAddress );
	void			RemoveAll( void ) { map.RemoveAll(); }

	NiTMap<long, void *>	map;
};

inline void *TGPointerMap::Find( long savedAddress ) {
	void *object;
	if ( map.GetAt( savedAddress, object ) ) {
		return object;
	}
	return NULL;
}

// The map of the load in progress.
extern TGPointerMap pointerMap;

#endif /* !__TGPOINTERMAP_H__ */
