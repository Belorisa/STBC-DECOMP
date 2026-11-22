// NiTopologicalSort.cpp -- ordering items by a pairwise comparison

#include "NiRTLib.h"
#include "NiTopologicalSort.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00825b70 _$E30
// FUNCTION: STBC 0x00825b90 _$E17
// FUNCTION: STBC 0x00825bb0 _$E15
// FUNCTION: STBC 0x00825bc0 _$E23
// FUNCTION: STBC 0x00825be0 _$E21

/*
================
NiTopologicalSort::NiTopologicalSort
================
*/
// FUNCTION: STBC 0x8257F0
NiTopologicalSort::NiTopologicalSort( int maxItems_ ) {
	maxItems = maxItems_;
	maxEdges = maxItems_ * maxItems_;
	edges = new bool[maxEdges];
	inDegree = new int[maxItems];
	outDegree = new int[maxItems];
	nodes = new NiBoxSortableItem *[maxItems];
}

/*
================
NiTopologicalSort::~NiTopologicalSort
================
*/
// FUNCTION: STBC 0x825840
NiTopologicalSort::~NiTopologicalSort() {
	delete [] edges;
	delete [] inDegree;
	delete [] outDegree;
	delete [] nodes;
}

/*
================
NiTopologicalSort::Sort
================
*/
// FUNCTION: STBC 0x825870
bool NiTopologicalSort::Sort( NiBoxSortableItem **items, int count, NiTopologicalCompare compare, void *data ) {
	if ( count > maxItems ) {
		maxItems = count;
		maxEdges = count * count;
		delete [] edges;
		delete [] inDegree;
		delete [] outDegree;
		delete [] nodes;
		edges = new bool[maxEdges];
		inDegree = new int[maxItems];
		outDegree = new int[maxItems];
		nodes = new NiBoxSortableItem *[maxItems];
	}

	BuildGraph( items, count, compare, data );
	return Order( items, count );
}

/*
================
NiTopologicalSort::BuildGraph

Compares every pair once. An edge runs from the item that goes later to
the item that goes first.
================
*/
// FUNCTION: STBC 0x825910
// PARKED: register allocation: retail keeps count in its argument slot and i in EBP across both loops; this build keeps count in EBP and i in EBX
void NiTopologicalSort::BuildGraph( NiBoxSortableItem **items, int count, NiTopologicalCompare compare, void *data ) {
	int *in = inDegree;
	int *out = outDegree;
	NiBoxSortableItem **node = nodes;
	int i;
	for ( i = 0; i < count; i++ ) {
		*in++ = 0;
		*out++ = 0;
		*node++ = *items++;
		edges[i * count + i] = false;
	}

	for ( i = 0; i < count; i++ ) {
		for ( int j = i + 1; j < count; j++ ) {
			int order = compare( nodes[i], nodes[j], data );
			if ( order == 0 ) {
				edges[j * count + i] = false;
				edges[i * count + j] = false;
			} else if ( order < 0 ) {
				edges[j * count + i] = true;
				edges[i * count + j] = false;
				inDegree[i]++;
				outDegree[j]++;
			} else {
				edges[j * count + i] = false;
				edges[i * count + j] = true;
				inDegree[j]++;
				outDegree[i]++;
			}
		}
	}
}

/*
================
NiTopologicalSort::Order

Fills the items from the last to the first, each time with an item no
remaining edge leads to. When every remaining item has incoming edges the
graph has a cycle: the item with the fewest incoming edges goes next, the
one with the most outgoing edges among those.
================
*/
// FUNCTION: STBC 0x825A70
bool NiTopologicalSort::Order( NiBoxSortableItem **items, int count ) {
	bool cycle = false;

	for ( int k = count - 1; k >= 0; k-- ) {
		int i;
		for ( i = 0; i < count; i++ ) {
			if ( inDegree[i] == 0 ) {
				break;
			}
		}

		if ( i == count ) {
			cycle = true;
			int fewestIn = count + 1;
			int mostOut;
			for ( int j = 0; j < count; j++ ) {
				int in = inDegree[j];
				if ( in > 0 && in <= fewestIn ) {
					if ( in == fewestIn ) {
						if ( outDegree[j] > mostOut ) {
							mostOut = outDegree[j];
							i = j;
						}
					} else {
						fewestIn = in;
						mostOut = outDegree[j];
						i = j;
					}
				}
			}
		}

		items[k] = nodes[i];
		for ( int j = 0; j < count; j++ ) {
			if ( edges[i * count + j] ) {
				inDegree[j]--;
			}
		}
		outDegree[i] = 0;
		inDegree[i] = -1;
	}

	return cycle;
}
