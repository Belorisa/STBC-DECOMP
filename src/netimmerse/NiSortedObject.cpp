// NiSortedObject.cpp -- the cluster sorter's objects and their pool

#include "NiRTLib.h"
#include "NiSortedObject.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00822440 _$E239
// FUNCTION: STBC 0x00822460 _$E17
// FUNCTION: STBC 0x00822480 _$E15
// FUNCTION: STBC 0x00822490 _$E23
// FUNCTION: STBC 0x008224b0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008224c0 _$E30
// FUNCTION: STBC 0x008224d0 _$E196
// FUNCTION: STBC 0x008224e0 _$E119
// FUNCTION: STBC 0x008224f0 _$E132
// FUNCTION: STBC 0x00822500 _$E41
// FUNCTION: STBC 0x00822510 _$E45
// FUNCTION: STBC 0x00822520 _$E49
// FUNCTION: STBC 0x00822530 _$E53
// FUNCTION: STBC 0x00822540 _$E177
// FUNCTION: STBC 0x00822560 _$E206
// FUNCTION: STBC 0x00822550 _$E201
// FUNCTION: STBC 0x00822570 _$E210

// DATA: STBC 0x9A2D60 ?freeList@NiSortedObject@@2PAV1@A
NiSortedObject *NiSortedObject::freeList;

// FUNCTION: STBC 0x008222c0 _$E227
// DATA: STBC 0x9A2D40 ?blocks@NiSortedObject@@2V?$NiTSimpleList@PAD@@A
NiTSimpleList<char *> NiSortedObject::blocks;

/*
================
NiSortedObjectShutdown

Frees the pool's heap blocks at exit; the list's own heap nodes are
not freed. The class has only its destructor: retail registers it
with atexit from a static initialiser of its own (0x8222D0), apart from
the list's constructor (0x8222C0), and expands it in 0x8222E0. DamageTool.exe
has the same three initialisers for its pooled NiBoxSortableItem,
whose list ms_blocks is a NiTSimpleList<char *> (as NiDrawableItem's). The class and object names
are invented.
================
*/
class NiSortedObjectShutdown {
public:
	~NiSortedObjectShutdown() {
		for ( NiTSimpleList<char *> *list = &NiSortedObject::blocks; list; list = list->GetNext() ) {
			delete [] list->GetItem();
		}
	}
};

// FUNCTION: STBC 0x008222d0 _$E232
// FUNCTION: STBC 0x008222e0 _$E230
static NiSortedObjectShutdown shutdown;

// DATA: STBC 0x9797B8 ?blockSize@NiSortedObject@@2HA
int NiSortedObject::blockSize = 4;

/*
================
NiSortedObject::operator new

Takes an object from the free list, refilling the list with a new block
when it is empty. Other sizes go to the heap.
================
*/
// FUNCTION: STBC 0x822300 ??2NiSortedObject@@SAPAXIPBDH@Z
void *NiSortedObject::operator new( size_t size, const char *file, int line ) {
	if ( size != sizeof( NiSortedObject ) ) {
		return ::operator new( size );
	}

	NiSortedObject *object = freeList;
	if ( object ) {
		freeList = object->nextFree;
	} else {
		// a new block: the first object is returned, the others chained into
		// the free list
		size_t blockBytes = sizeof( NiSortedObject ) * blockSize;
		object = (NiSortedObject *)::operator new( blockBytes );
		if ( !object ) {
			return 0;
		}
		blocks.AddHead( (char *)object );
		for ( int i = 1; i < blockSize; i++ ) {
			object[i - 1].nextFree = &object[i];
		}
		object[blockSize - 1].nextFree = 0;
		freeList = &object[1];
	}
	return object;
}

/*
================
NiSortedObject::operator delete

Returns the object to the free list.
================
*/
// FUNCTION: STBC 0x8223A0 ??3NiSortedObject@@SAXPAX@Z
void NiSortedObject::operator delete( void *block ) {
	NiSortedObject *object = (NiSortedObject *)block;
	object->nextFree = freeList;
	freeList = object;
}

/*
================
NiSortedObject::Display
================
*/
// FUNCTION: STBC 0x8223C0
void NiSortedObject::Display( NiCamera *camera ) {
	NiTimeController *controllers = geometry->GetControllers();
	if ( controllers ) {
		controllers->VirtualSlot15();
	}
	geometry->Display( camera );
}
