// NiClusterSorter.cpp -- the cluster sort behind NiClusterAccumulator

#include "NiRTLib.h"
#include <stdlib.h>

#include "NiClusterSorter.h"
#include "NiCamera.h"
#include "NiSortedObject.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00822e30 _$E234
// FUNCTION: STBC 0x00822dd0 _$E17
// FUNCTION: STBC 0x00822df0 _$E15
// FUNCTION: STBC 0x00822e00 _$E23
// FUNCTION: STBC 0x00822e20 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00822e50 _$E38
// FUNCTION: STBC 0x00822e60 _$E53
// FUNCTION: STBC 0x00822e70 _$E57
// FUNCTION: STBC 0x00822e80 _$E61
// FUNCTION: STBC 0x00822e90 _$E65
// FUNCTION: STBC 0x00822ea0 _$E184
// FUNCTION: STBC 0x00822eb0 _$E42
// FUNCTION: STBC 0x00822ec0 _$E126
// FUNCTION: STBC 0x00822ed0 _$E139
// FUNCTION: STBC 0x00822ee0 _$E189
// FUNCTION: STBC 0x00822ef0 _$E194
// FUNCTION: STBC 0x00822f00 _$E198
// FUNCTION: STBC 0x00822f10 _$E202
// FUNCTION: STBC 0x00822f20 _$E209

/*
================
NiClusterSorter::NiClusterSorter
================
*/
// FUNCTION: STBC 0x822580
NiClusterSorter::NiClusterSorter( NiCamera *camera_ ) : camera( camera_ ), clusterOrder( 16 ) {
	entryCount = 0;
	entryMax = 0;
	entries = 0;
	clusterSizes = 0;
}

/*
================
NiClusterSorter::~NiClusterSorter
================
*/
// FUNCTION: STBC 0x822610
NiClusterSorter::~NiClusterSorter() {
	delete [] entries;
	delete [] clusterSizes;
}

/*
================
NiClusterSorter::Sort

Boxes the objects in the camera's view and orders them. The whole list
starts as one cluster; each axis in turn sorts the clusters and splits them
where the boxes stop overlapping, and each final cluster is put in drawing
order.
================
*/
// FUNCTION: STBC 0x822690
void NiClusterSorter::Sort( NiTList<NiSortedObject *> *objects ) {
	entryCount = objects->GetSize();
	if ( !entryCount ) {
		return;
	}

	if ( entryCount > entryMax ) {
		delete [] entries;
		delete [] clusterSizes;
		entryMax = entryCount;
		entries = new NiBoxSortableItem *[entryMax];
		clusterSizes = new int[entryMax];
	}

	NiTListIterator pos = objects->GetHeadPos();
	for ( int i = 0; i < entryCount; i++ ) {
		NiSortedObject *object = objects->GetNext( pos );
		NiBoxSortableItem *entry = new( "UNKNOWN", 0 ) NiBoxSortableItem;
		entry->unknown20 = false;
		entry->object = object;
		camera->Unknown7E9560( object, entry->min, entry->max );
		entries[i] = entry;
		clusterSizes[i] = 0;
	}
	clusterSizes[0] = entryCount;

	SortZ();
	SplitZ();
	SortY();
	SplitY();
	SortX();
	SplitX();
	OrderClusters();
}

/*
================
NiClusterSorter::StartIteration
================
*/
// FUNCTION: STBC 0x822830
void NiClusterSorter::StartIteration() {
	iterator = entryCount;
}

/*
================
NiClusterSorter::GetNext

The objects from the last entry to the first; NULL after the first.
================
*/
// FUNCTION: STBC 0x822840
NiSortedObject *NiClusterSorter::GetNext() {
	if ( iterator ) {
		return entries[--iterator]->object;
	}
	return 0;
}

/*
================
NiClusterSorter::RemoveAllEntries
================
*/
// FUNCTION: STBC 0x822860
void NiClusterSorter::RemoveAllEntries() {
	NiBoxSortableItem **entry = entries;
	for ( int i = 0; i < entryCount; i++ ) {
		delete *entry;
		*entry++ = 0;
	}
}

/*
================
NiClusterSorter::SplitX

Within each cluster, sorted by the box minimum's x, a new cluster starts at
the first box that lies wholly to the right of the boxes before it.
================
*/
// FUNCTION: STBC 0x8228A0
void NiClusterSorter::SplitX() {
	int clusterEnd = clusterSizes[0];
	int start = 0;
	while ( start < entryCount ) {
		float maxX = entries[start]->max.x;
		int i;
		for ( i = start + 1; i < clusterEnd && entries[i]->min.x <= maxX; i++ ) {
			float boxMax = entries[i]->max.x;
			if ( boxMax > maxX ) {
				maxX = boxMax;
			}
		}
		clusterSizes[start] = i - start;
		start = i;
		if ( entryCount == start ) {
			break;
		}
		if ( clusterEnd == start ) {
			clusterEnd = start + clusterSizes[start];
		}
	}
}

/*
================
NiClusterSorter::SplitY
================
*/
// FUNCTION: STBC 0x822930
void NiClusterSorter::SplitY() {
	int clusterEnd = clusterSizes[0];
	int start = 0;
	while ( start < entryCount ) {
		float maxY = entries[start]->max.y;
		int i;
		for ( i = start + 1; i < clusterEnd && entries[i]->min.y <= maxY; i++ ) {
			float boxMax = entries[i]->max.y;
			if ( boxMax > maxY ) {
				maxY = boxMax;
			}
		}
		clusterSizes[start] = i - start;
		start = i;
		if ( entryCount == start ) {
			break;
		}
		if ( clusterEnd == start ) {
			clusterEnd = start + clusterSizes[start];
		}
	}
}

/*
================
NiClusterSorter::SplitZ
================
*/
// FUNCTION: STBC 0x8229C0
void NiClusterSorter::SplitZ() {
	int clusterEnd = clusterSizes[0];
	int start = 0;
	while ( start < entryCount ) {
		float maxZ = entries[start]->max.z;
		int i;
		for ( i = start + 1; i < clusterEnd && entries[i]->min.z <= maxZ; i++ ) {
			float boxMax = entries[i]->max.z;
			if ( boxMax > maxZ ) {
				maxZ = boxMax;
			}
		}
		clusterSizes[start] = i - start;
		start = i;
		if ( entryCount == start ) {
			break;
		}
		if ( clusterEnd == start ) {
			clusterEnd = start + clusterSizes[start];
		}
	}
}

/*
================
CompareX

The qsort comparison of two entries by their box minimum's x.
================
*/
// FUNCTION: STBC 0x822A50
static int CompareX( const void *a, const void *b ) {
	float xA = ( *(NiBoxSortableItem **)a )->min.x;
	float xB = ( *(NiBoxSortableItem **)b )->min.x;
	if ( xA < xB ) {
		return -1;
	}
	if ( xA > xB ) {
		return 1;
	}
	return 0;
}

/*
================
CompareY
================
*/
// FUNCTION: STBC 0x822A90
static int CompareY( const void *a, const void *b ) {
	float yA = ( *(NiBoxSortableItem **)a )->min.y;
	float yB = ( *(NiBoxSortableItem **)b )->min.y;
	if ( yA < yB ) {
		return -1;
	}
	if ( yA > yB ) {
		return 1;
	}
	return 0;
}

/*
================
CompareZ
================
*/
// FUNCTION: STBC 0x822AD0
static int CompareZ( const void *a, const void *b ) {
	float zA = ( *(NiBoxSortableItem **)a )->min.z;
	float zB = ( *(NiBoxSortableItem **)b )->min.z;
	if ( zA < zB ) {
		return -1;
	}
	if ( zA > zB ) {
		return 1;
	}
	return 0;
}

/*
================
NiClusterSorter::SortX
================
*/
// FUNCTION: STBC 0x822B10
void NiClusterSorter::SortX() {
	NiBoxSortableItem **cluster = entries;
	int *end = clusterSizes + entryCount;
	for ( int *size = clusterSizes; size < end; ) {
		int count = *size;
		qsort( cluster, count, sizeof( NiBoxSortableItem * ), CompareX );
		cluster += count;
		size += count;
	}
}

/*
================
NiClusterSorter::SortY
================
*/
// FUNCTION: STBC 0x822B50
void NiClusterSorter::SortY() {
	NiBoxSortableItem **cluster = entries;
	int *end = clusterSizes + entryCount;
	for ( int *size = clusterSizes; size < end; ) {
		int count = *size;
		qsort( cluster, count, sizeof( NiBoxSortableItem * ), CompareY );
		cluster += count;
		size += count;
	}
}

/*
================
NiClusterSorter::SortZ
================
*/
// FUNCTION: STBC 0x822B90
void NiClusterSorter::SortZ() {
	NiBoxSortableItem **cluster = entries;
	int *end = clusterSizes + entryCount;
	for ( int *size = clusterSizes; size < end; ) {
		int count = *size;
		qsort( cluster, count, sizeof( NiBoxSortableItem * ), CompareZ );
		cluster += count;
		size += count;
	}
}

/*
================
CompareDepth

Orders two entries for drawing, for the topological sort: zero when their
screen boxes do not overlap, negative when a is drawn after b. Boxes apart
in depth go by depth; otherwise the eye's side of the plane between the two
world bounds decides.
================
*/
// FUNCTION: STBC 0x822C60
static int CompareDepth( NiBoxSortableItem *entryA, NiBoxSortableItem *entryB, void *data ) {
	const NiPoint3 &eye = *(const NiPoint3 *)data;

	if ( entryA->max.x < entryB->min.x || entryB->max.x < entryA->min.x
		|| entryA->max.y < entryB->min.y || entryB->max.y < entryA->min.y ) {
		return 0;
	}
	if ( entryA->max.z < entryB->min.z ) {
		return -1;
	}
	if ( entryB->max.z < entryA->min.z ) {
		return 1;
	}

	const NiBound &boundA = entryA->object->geometry->GetWorldBound();
	const NiBound &boundB = entryB->object->geometry->GetWorldBound();
	float t;
	if ( boundA.radius == 0.0f && boundB.radius == 0.0f ) {
		t = 1.0f;
	} else {
		float radiusA = boundA.radius;
		float radiusB = boundB.radius;
		t = radiusB / ( radiusA + radiusB );
	}
	NiPoint3 diff = boundA.center - boundB.center;
	if ( ( eye - diff * t - boundB.center ).Dot( diff ) > 0.0f ) {
		return -1;
	}
	return 1;
}

/*
================
NiClusterSorter::OrderClusters
================
*/
// FUNCTION: STBC 0x822BD0
void NiClusterSorter::OrderClusters() {
	const NiPoint3 *eye = &camera->GetWorldLocation();
	for ( int start = 0; start < entryCount; ) {
		int size = clusterSizes[start];
		if ( size > 1 ) {
			if ( size == 2 ) {
				if ( CompareDepth( entries[start], entries[start + 1], (void *)eye ) == 1 ) {
					NiBoxSortableItem *entry = entries[start];
					entries[start] = entries[start + 1];
					entries[start + 1] = entry;
				}
			} else {
				clusterOrder.Sort( &entries[start], size, CompareDepth, (void *)eye );
			}
		}
		start += size;
	}
}
