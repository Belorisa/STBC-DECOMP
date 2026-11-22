/*
===============================================================================

	NiClusterSortEntry.h

	One registered object as the cluster sorter sees it: the object and
	its box in the camera's view, screen x and y and depth z. The entries
	come from a pool of their own. The class name is DamageTool.exe's
	(NiBoxSortableItem: its operator new and delete, and the visibility
	matrix's item type); the member names are invented, and the file keeps
	its invented name.

	Evidence: NiClusterSorter::Sort (0x822690) allocates each entry (0x28
	bytes) through the pooled operator new (0x825C30, "UNKNOWN", 0), clears
	the byte at 0x20, stores the object at 0 and has NiCamera's 0x7E9560
	fill the box corners at 4 and 0x10. The per-axis comparisons (0x822A50,
	0x822A90, 0x822AD0) read the minimum corner, the cluster splits and the
	depth comparison both corners. RemoveAllEntries (0x822860) returns them
	through the pool's operator delete (0x825CC0). The pool (0x825C30)
	hands out 0x28-byte entries from blocks of blockSize (0x9798A4, 4),
	chained through their last word (free list 0x9B2E6C, blocks listed at
	0x9B2E60); other sizes go to the heap.

===============================================================================
*/

#ifndef __NICLUSTERSORTENTRY_H__
#define __NICLUSTERSORTENTRY_H__

#include <stddef.h>
#include "NiPoint3.h"
#include "NiTSimpleList.h"

class NiSortedObject;

class NiBoxSortableItem {
public:
	// the pool of 0x28-byte entries
	static void *			operator new( size_t size, const char *file, int line );
	static void				operator delete( void *block );

	NiSortedObject *		object;
	NiPoint3				min;			// the box in the camera's view: screen x and y, depth z
	NiPoint3				max;
	unsigned int			unknown1C;		// TODO identity: never read or written by the sorter
	bool					unknown20;		// TODO identity: cleared by Sort, never read
	NiBoxSortableItem *	nextFree;		// the pool's free list

	static NiBoxSortableItem *	freeList;
	static NiTSimpleList<char *>	blocks;	// every heap block, to be freed at exit
	static int				blockSize;		// entries per heap block
};

#endif /* !__NICLUSTERSORTENTRY_H__ */
