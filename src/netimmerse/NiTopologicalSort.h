/*
===============================================================================

	NiTopologicalSort.h

	Puts a set of items in an order that respects a pairwise comparison:
	each pair the comparison orders becomes an edge, and the items are
	taken off the graph one without incoming edges at a time. A cycle is
	broken at the item with the fewest incoming and most outgoing edges.
	The class, member and method names are invented. The item type and
	the comparison's are DamageTool.exe's (NiVisibilityMatrix::SortItems,
	ComputeMatrix and TraverseMatrix take NiBoxSortableItem **, the
	comparison NiBoxSortableItem *, NiBoxSortableItem *, void *;
	ComputeMatrix 0x475B50).

	Evidence: NiClusterSorter builds one at 0x1C for 0x10 items (0x8257F0:
	the size, its square, and four heap blocks: the square edge matrix and
	three arrays of the size) and destroys it (0x825840). Its cluster order
	(0x822BD0) calls 0x825870 with a cluster's entries, their count, the
	depth comparison (0x822C60) and the camera's eye. 0x825870 grows the
	blocks, then builds the graph (0x825910) and orders the items
	(0x825A70), which returns whether it had to break a cycle.

===============================================================================
*/

#ifndef __NITOPOLOGICALSORT_H__
#define __NITOPOLOGICALSORT_H__

class NiBoxSortableItem;

// orders a and b: negative when a goes first, positive when b does, zero
// when either order will do
typedef int ( *NiTopologicalCompare )( NiBoxSortableItem *a, NiBoxSortableItem *b, void *data );

class NiTopologicalSort {
public:
							NiTopologicalSort( int maxItems_ );
							~NiTopologicalSort();

	// reorders the items in place; true when a cycle had to be broken
	bool					Sort( NiBoxSortableItem **items, int count, NiTopologicalCompare compare, void *data );

protected:
	void					BuildGraph( NiBoxSortableItem **items, int count, NiTopologicalCompare compare, void *data );
	bool					Order( NiBoxSortableItem **items, int count );

	int						maxItems;		// the arrays' size
	int						maxEdges;		// maxItems squared, the matrix's size
	bool *					edges;			// count by count: edges[from * count + to]
	int *					inDegree;		// per item, the edges into it; -1 once taken
	int *					outDegree;		// per item, the edges out of it
	NiBoxSortableItem **	nodes;			// the items while they are ordered
};

#endif /* !__NITOPOLOGICALSORT_H__ */
