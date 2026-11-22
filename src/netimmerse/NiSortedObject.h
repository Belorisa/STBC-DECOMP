/*
===============================================================================

	NiSortedObject.h

	A geometry object registered with the cluster sorter during a camera's
	click, with the camera's disabled culling planes at the time. The
	objects come from a pool of their own. The class and member names are
	invented.

	Evidence: NiCamera's 0x7E8B10 allocates one (0x1C bytes) through the
	pooled operator new (0x822300, "UNKNOWN", 0), builds it (0x7EB290: the
	pointer at 4 and the mask at 8 cleared), stores the geometry at 4 and
	copies the camera's plane mask (0x208) into 8; 0x7E8B90 puts the mask
	back on the camera while the renderer draws the object. The renderer's
	draw (0x7E2FD0) has the object display its geometry (0x8223C0).
	NiClusterAccumulator keeps them in a list and deletes them (0x8223A0,
	the pool's operator delete) after the sort, releasing the geometry
	first. The pool (0x822300) hands out 0x1C-byte objects from blocks of
	blockSize (0x9797B8, 4), chained through their last word (free list
	0x9A2D60, blocks listed at 0x9A2D40); other sizes go to the heap.

===============================================================================
*/

#ifndef __NISORTEDOBJECT_H__
#define __NISORTEDOBJECT_H__

#include <stddef.h>
#include "NiPointer.h"
#include "NiGeometry.h"
#include "NiCamera.h"
#include "NiTSimpleList.h"

// FUNCTION: STBC 0x7EB290 ??0NiSortedObject@@QAE@XZ @netimmerse/NiCamera
// FUNCTION: STBC 0x7EB6F0 ??1NiSortedObject@@QAE@XZ @netimmerse/NiClusterAccumulator
class NiSortedObject {
public:
	// the pool of 0x1C-byte objects
	static void *			operator new( size_t size, const char *file, int line );
	static void				operator delete( void *block );

	// runs the geometry's first controller's slot 15 and displays the
	// geometry
	void					Display( NiCamera *camera );

	unsigned int			unknown00;		// TODO identity: never written by the camera or the accumulator
	NiPointer<NiAVObject>	geometry;
	NiBitVector				disabledPlanes;	// the camera's when the object was registered
	NiSortedObject *		nextFree;		// the pool's free list

	static NiSortedObject *	freeList;
	static NiTSimpleList<char *>	blocks;	// every heap block, to be freed at exit
	static int				blockSize;		// objects per heap block
};

#endif /* !__NISORTEDOBJECT_H__ */
