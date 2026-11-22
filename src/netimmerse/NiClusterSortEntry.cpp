// NiClusterSortEntry.cpp -- the cluster sorter's entries and their pool

#include "NiRTLib.h"
#include "NiClusterSortEntry.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00825ce0 _$E46
// FUNCTION: STBC 0x00825d00 _$E17
// FUNCTION: STBC 0x00825d20 _$E15
// FUNCTION: STBC 0x00825d30 _$E23
// FUNCTION: STBC 0x00825d50 _$E21

// DATA: STBC 0x9B2E6C ?freeList@NiBoxSortableItem@@2PAV1@A
NiBoxSortableItem *NiBoxSortableItem::freeList;

// FUNCTION: STBC 0x00825bf0 _$E34
// DATA: STBC 0x9B2E60 ?blocks@NiBoxSortableItem@@2V?$NiTSimpleList@PAD@@A
NiTSimpleList<char *> NiBoxSortableItem::blocks;

/*
================
NiBoxSortableItemShutdown

Frees the pool's heap blocks at exit; the list's own heap nodes are
not freed. The class has only its destructor: retail registers it
with atexit from a static initialiser of its own (0x825C00), apart from
the list's constructor (0x825BF0), and expands it in 0x825C10. DamageTool.exe
has the same three initialisers for its pooled NiBoxSortableItem,
whose list ms_blocks is a NiTSimpleList<char *> (as NiDrawableItem's). The class and object names
are invented.
================
*/
class NiBoxSortableItemShutdown {
public:
	~NiBoxSortableItemShutdown() {
		for ( NiTSimpleList<char *> *list = &NiBoxSortableItem::blocks; list; list = list->GetNext() ) {
			delete [] list->GetItem();
		}
	}
};

// FUNCTION: STBC 0x00825c00 _$E39
// FUNCTION: STBC 0x00825c10 _$E37
static NiBoxSortableItemShutdown shutdown;

// DATA: STBC 0x9798A4 ?blockSize@NiBoxSortableItem@@2HA
int NiBoxSortableItem::blockSize = 4;

/*
================
NiBoxSortableItem::operator new

Takes an entry from the free list, refilling the list with a new block
when it is empty. Other sizes go to the heap.
================
*/
// FUNCTION: STBC 0x825C30 ??2NiBoxSortableItem@@SAPAXIPBDH@Z
void *NiBoxSortableItem::operator new( size_t size, const char *file, int line ) {
	if ( size != sizeof( NiBoxSortableItem ) ) {
		return ::operator new( size );
	}

	NiBoxSortableItem *entry = freeList;
	if ( entry ) {
		freeList = entry->nextFree;
	} else {
		// a new block: the first entry is returned, the others chained into
		// the free list
		size_t blockBytes = sizeof( NiBoxSortableItem ) * blockSize;
		entry = (NiBoxSortableItem *)::operator new( blockBytes );
		if ( !entry ) {
			return 0;
		}
		blocks.AddHead( (char *)entry );
		for ( int i = 1; i < blockSize; i++ ) {
			entry[i - 1].nextFree = &entry[i];
		}
		entry[blockSize - 1].nextFree = 0;
		freeList = &entry[1];
	}
	return entry;
}

/*
================
NiBoxSortableItem::operator delete

Returns the entry to the free list.
================
*/
// FUNCTION: STBC 0x825CC0 ??3NiBoxSortableItem@@SAXPAX@Z
void NiBoxSortableItem::operator delete( void *block ) {
	NiBoxSortableItem *entry = (NiBoxSortableItem *)block;
	entry->nextFree = freeList;
	freeList = entry;
}
