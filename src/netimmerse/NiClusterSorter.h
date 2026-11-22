/*
===============================================================================

	NiClusterSorter.h

	The sort behind NiClusterAccumulator: the registered objects are boxed
	in the camera's view, gathered into clusters of overlapping boxes along
	each axis, and each cluster put in drawing order. The class, member and
	method names are invented.

	Evidence: the vtable at 0x89A764 holds the scalar deleting destructor
	(0x8225F0), the sort of a list of NiSortedObjects (0x822690), the start
	of the walk (0x822830: the walk position at 0x18 set to the count), the
	next object (0x822840: from the last entry down, NULL at the end) and
	the release of the entries (0x822860). The constructor (0x822580) keeps
	its argument as a referenced object at 4 (NiClusterAccumulator passes
	NULL and sets its camera there while accumulating), builds the
	topological sort at 0x1C for 0x10 items and clears the count, the
	capacity and the two arrays; the destructor (0x822610) frees the
	arrays, destroys the topological sort and releases the camera. The
	sort (0x822690) grows the arrays to the list's count, boxes each object
	with NiCamera's 0x7E9560 into a pooled entry (0x825C30), then sorts
	each cluster by one axis with qsort (the comparisons 0x822AD0, 0x822A90,
	0x822A50, on the box minimum's z, y and x) and splits it where the
	boxes stop overlapping on that axis (0x8229C0, 0x822930, 0x8228A0),
	for depth, then screen y, then screen x. Last it orders each cluster
	(0x822BD0): a pair by the depth comparison (0x822C60), a larger cluster
	by the topological sort with the same comparison and the camera's eye.
	The per-axis sorts also have out-of-line copies (0x822B10, 0x822B50,
	0x822B90).

===============================================================================
*/

#ifndef __NICLUSTERSORTER_H__
#define __NICLUSTERSORTER_H__

#include "NiPointer.h"
#include "NiTList.h"
#include "NiTopologicalSort.h"
#include "NiClusterSortEntry.h"

class NiCamera;
class NiSortedObject;

// FUNCTION: STBC 0x8225F0 ??_GNiClusterSorter@@UAEPAXI@Z
class NiClusterSorter {
public:
							NiClusterSorter( NiCamera *camera_ );
	virtual					~NiClusterSorter();

	virtual void			Sort( NiTList<NiSortedObject *> *objects );

	// the sorted objects, last to first
	virtual void			StartIteration();
	virtual NiSortedObject *	GetNext();

	// returns the entries to their pool
	virtual void			RemoveAllEntries();

	void					SetCamera( NiCamera *camera_ ) { camera = camera_; }

protected:
	// each cluster sorted by the box minimum on one axis
	void					SortX();
	void					SortY();
	void					SortZ();

	// each cluster split where the boxes stop overlapping on one axis
	void					SplitX();
	void					SplitY();
	void					SplitZ();

	// each cluster put in drawing order
	void					OrderClusters();

	NiPointer<NiCamera>		camera;			// the camera the boxes are taken in
	int						entryCount;		// entries in use
	int						entryMax;		// the arrays' size
	NiBoxSortableItem **	entries;
	int *					clusterSizes;	// per entry, the entries in the cluster that starts there
	int						iterator;		// the walk position, counting down
	NiTopologicalSort		clusterOrder;	// orders the clusters of more than two entries
};

#endif /* !__NICLUSTERSORTER_H__ */
