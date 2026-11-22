// NiLODNode.cpp -- level-of-detail nodes

#include "NiRTLib.h"
#include <assert.h>
#include "NiLODNode.h"
#include "NiCollisionRecord.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiLines.h"
#include "NiLinesData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00800500 _$E250
// FUNCTION: STBC 0x008003f0 _$E17
// FUNCTION: STBC 0x00800410 _$E15
// FUNCTION: STBC 0x00800420 _$E23
// FUNCTION: STBC 0x00800440 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008004f0 _$E30
// FUNCTION: STBC 0x00800520 _$E45
// FUNCTION: STBC 0x00800530 _$E49
// FUNCTION: STBC 0x00800540 _$E53
// FUNCTION: STBC 0x00800550 _$E57
// FUNCTION: STBC 0x00800560 _$E181
// FUNCTION: STBC 0x00800570 _$E34
// FUNCTION: STBC 0x00800580 _$E123
// FUNCTION: STBC 0x00800590 _$E136
// FUNCTION: STBC 0x008005b0 _$E191
// FUNCTION: STBC 0x008005c0 _$E195
// FUNCTION: STBC 0x008005d0 _$E199
// FUNCTION: STBC 0x008005e0 _$E206
// FUNCTION: STBC 0x008005f0 _$E210
// FUNCTION: STBC 0x00800630 _$E214
// FUNCTION: STBC 0x008005a0 _$E186

// FUNCTION: STBC 0x007ff0e0 _$E243
// FUNCTION: STBC 0x007ff100 _$E240
// DATA: STBC 0x9A21B0 ?rtti@NiLODNode@@2VNiRTTI@@B
const NiRTTI NiLODNode::rtti( "NiLODNode", &NiSwitchNode::rtti );

/*
================
NiLODNode::Register
================
*/
// FUNCTION: STBC 0x7FF120 ?Register@NiLODNode@@SAHXZ
NiImplementRegister( NiLODNode )

/*
================
NiLODNode::NiLODNode

No ranges; distances are measured from the first child's bound center.
================
*/
// FUNCTION: STBC 0x7FF270
NiLODNode::NiLODNode() {
	SetPositionInRange( false );
}

/*
================
NiLODNode::~NiLODNode
================
*/
// FUNCTION: STBC 0x7FF2F0
NiLODNode::~NiLODNode() {
	for ( unsigned int i = 0; i < ranges.GetSize(); i++ ) {
		delete ranges.GetAt( i );
	}
}

/*
================
NiLODNode::SetRange

A range already at the index is not freed.
================
*/
// FUNCTION: STBC 0x7FF3B0
void NiLODNode::SetRange( float nearDistance, float farDistance, unsigned int index_ ) {
	// AnimOnly.exe 0x442665 (NiLODNode.cpp)
	Range *range = new Range;
	assert( range );
	range->nearDistance = nearDistance;
	range->worldNearDistance = nearDistance;
	range->farDistance = farDistance;
	range->worldFarDistance = farDistance;
	range->location = NiPoint3::ZERO;
	range->worldLocation = NiPoint3::ZERO;
	SetRange( range, index_ );
}

/*
================
NiLODNode::UpdateWorldRanges
================
*/
// FUNCTION: STBC 0x7FF530
void NiLODNode::UpdateWorldRanges() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		Range *range = ranges.GetAt( i );
		if ( range ) {
			range->worldLocation = worldRotate * ( worldScale * range->location ) + worldTranslate;
			range->worldNearDistance = range->nearDistance * worldScale;
			range->worldFarDistance = range->farDistance * worldScale;
		}
	}
}

/*
================
NiLODNode::UpdateWorldData

NiAVObject's, then the ranges follow the world transform.
================
*/
// FUNCTION: STBC 0x7FF5F0
void NiLODNode::UpdateWorldData() {
	NiSwitchNode::UpdateWorldData();
	UpdateWorldRanges();
}

/*
================
NiLODNode::VirtualSlot35

Activates, in turn, every child whose range holds the camera's distance,
scaled by the camera's LOD adjustment, and hands each NiSwitchNode's
slot 35; the last one stays active. Without the position-in-range flag
the distance is the first child's, brought up to date first.
================
*/
// FUNCTION: STBC 0x7FF6B0
void NiLODNode::VirtualSlot35( NiCamera *camera ) {
	if ( children.GetSize() == 0 ) {
		return;
	}

	NiPoint3 offset;
	NiPoint3 camPos = camera->GetWorldLocation();	// (name invented; fits the /Od frame order)
	float lodAdjust = camera->GetLODAdjust();	// (name invented; fits the /Od frame order)
	lodAdjust *= lodAdjust;
	float dist;	// (name invented; fits the /Od frame order)

	if ( !GetPositionInRange() ) {
		NiAVObject *child = children.GetAt( 0 );
		assert( child );
		if ( GetUpdateOnlyActive() && childRevisionIDs.GetAt( 0 ) != revisionID ) {
			childRevisionIDs.SetAt( 0, revisionID );
			child->UpdateDownwardPass( savedTime );
		}
		offset = child->GetWorldBound().GetCenter() - camPos;
		dist = offset.SqrLength() * lodAdjust;
	}

	index = -1;
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		Range *range = ranges.GetAt( i );
		assert( range );
		float nearDist = range->worldNearDistance;	// (name invented; fits the /Od frame order)
		nearDist *= nearDist;
		float farDist = range->worldFarDistance;	// (name invented; fits the /Od frame order)
		farDist *= farDist;
		if ( GetPositionInRange() ) {
			offset = range->worldLocation - camPos;
			dist = offset.SqrLength() * lodAdjust;
		}
		if ( nearDist <= dist && dist < farDist ) {
			index = i;
			NiSwitchNode::VirtualSlot35( camera );
		}
	}
}

/*
================
NiLODNode::VirtualSlot36
================
*/
// FUNCTION: STBC 0x7FF8B0
void NiLODNode::VirtualSlot36( NiCamera *camera ) {
	if ( children.GetSize() > 0 && index >= 0 ) {
		NiSwitchNode::VirtualSlot36( camera );
	}
}

/*
================
NiLODNode::TestCollisions

The active child alone when only it is updated, else each child that has
a range.
================
*/
// FUNCTION: STBC 0x7FF8E0
bool NiLODNode::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	NiCollisionRecord childRecord( record0.GetRoot(), 0, record0.GetUnknown08(), record0.GetUnknown0C(), 0 );	// (name invented; fits the /Od frame order)
	NiAVObject *child;
	if ( GetUpdateOnlyActive() ) {
		if ( index != -1 ) {
			child = GetAt( index );
			if ( child ) {
				childRecord.SetObject( child );
				if ( record1.GetAVObject()->TestCollisions( deltaTime, record1, childRecord ) ) {
					return true;
				}
			}
		}
	} else {
		for ( unsigned int i = 0; i < ranges.GetSize(); i++ ) {
			child = GetAt( i );
			if ( child ) {
				childRecord.SetObject( child );
				if ( record1.GetAVObject()->TestCollisions( deltaTime, record1, childRecord ) ) {
					return true;
				}
			}
		}
	}
	return false;
}

/*
================
NiLODNode::FindCollisions

As TestCollisions, stopping at a result of 1.
================
*/
// FUNCTION: STBC 0x7FF9B0
int NiLODNode::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	NiCollisionRecord childRecord( record0.GetRoot(), 0, record0.GetUnknown08(), record0.GetUnknown0C(), 0 );	// (name invented; fits the /Od frame order)
	NiAVObject *child;
	if ( GetUpdateOnlyActive() ) {
		if ( index != -1 ) {
			child = GetAt( index );
			if ( child ) {
				childRecord.SetObject( child );
				if ( record1.GetAVObject()->FindCollisions( deltaTime, record1, childRecord ) == 1 ) {
					return 1;
				}
			}
		}
	} else {
		for ( unsigned int i = 0; i < ranges.GetSize(); i++ ) {
			child = GetAt( i );
			if ( child ) {
				childRecord.SetObject( child );
				if ( record1.GetAVObject()->FindCollisions( deltaTime, record1, childRecord ) == 1 ) {
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
================
NiLODNode::CreateClone
================
*/
// FUNCTION: STBC 0x7FFA90
NiObject *NiLODNode::CreateClone() {
	NiLODNode *clone = new NiLODNode;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiLODNode::CopyMembers

The clone gets copies of the ranges.
================
*/
// FUNCTION: STBC 0x7FFB50
void NiLODNode::CopyMembers( NiLODNode *dest ) {
	NiSwitchNode::CopyMembers( dest );
	for ( unsigned int i = 0; i < ranges.GetSize(); i++ ) {
		Range *range = ranges.GetAt( i );
		if ( range ) {
			Range *copy = new Range;
			*copy = *range;
			dest->ranges.SetAtGrow( i, copy );
		} else {
			dest->ranges.SetAtGrow( i, 0 );
		}
	}
}

/*
================
NiLODNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7FFD00
void NiLODNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiLODNode *node = new NiLODNode;
	stream.SetLoadRecord( new NiLODNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiLODNode::LoadBinary

The count of ranges, then each range's distances and location; the world
members follow at the next update.
================
*/
// FUNCTION: STBC 0x7FFEA0
void NiLODNode::LoadBinary( NiStream &stream ) {
	NiSwitchNode::LoadBinary( stream );

	int numRanges;	// (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, numRanges );
	for ( int i = 0; i < numRanges; i++ ) {
		Range *range = new Range;
		assert( range );
		NiStreamLoadBinary( stream, range->nearDistance );
		NiStreamLoadBinary( stream, range->farDistance );
		range->location.LoadBinary( stream );
		SetRange( range, i );
	}
}

/*
================
NiLODNode::LinkObject
================
*/
// FUNCTION: STBC 0x800020
void NiLODNode::LinkObject( NiStream &stream ) {
	NiSwitchNode::LinkObject( stream );
}

/*
================
NiLODNode::RegisterStreamables
================
*/
// FUNCTION: STBC 0x800030
bool NiLODNode::RegisterStreamables( NiStream &stream ) {
	return NiSwitchNode::RegisterStreamables( stream );
}

/*
================
NiLODNode::SaveBinary
================
*/
// FUNCTION: STBC 0x800040
void NiLODNode::SaveBinary( NiStream &stream ) {
	NiSwitchNode::SaveBinary( stream );

	int numRanges = ranges.GetSize();	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, numRanges );
	for ( int i = 0; i < numRanges; i++ ) {
		Range *range = ranges.GetAt( i );
		assert( range );
		NiStreamSaveBinary( stream, range->nearDistance );
		NiStreamSaveBinary( stream, range->farDistance );
		range->location.SaveBinary( stream );
	}
}

/*
================
NiLODNode::IsEqual

Compares the model members of the ranges.
================
*/
// FUNCTION: STBC 0x8000B0
bool NiLODNode::IsEqual( NiObject *object ) {
	if ( !NiSwitchNode::IsEqual( object ) ) {
		return false;
	}

	NiLODNode *otherNode = (NiLODNode *)object;	// (name invented; fits the /Od frame order)
	int count = ranges.GetSize();
	int otherCount = otherNode->ranges.GetSize();	// (name invented; fits the /Od frame order)
	if ( count != otherCount ) {
		return false;
	}
	for ( int i = 0; i < count; i++ ) {
		Range *range = ranges.GetAt( i );
		Range *otherRange = otherNode->ranges.GetAt( i );	// (name invented; fits the /Od frame order)
		if ( range->nearDistance != otherRange->nearDistance ||
			range->farDistance != otherRange->farDistance ||
			range->location != otherRange->location ) {
			return false;
		}
	}
	if ( GetPositionInRange() != otherNode->GetPositionInRange() ) {
		return false;
	}
	return true;
}

/*
================
NiLODNode::GetViewerStrings
================
*/
// FUNCTION: STBC 0x800170
void NiLODNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiSwitchNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bPositionInRange", GetPositionInRange() ) );

	for ( unsigned int i = 0; i < ranges.GetSize(); i++ ) {
		Range *range = ranges.GetAt( i );
		assert( range );
		char text[64];
		sprintf( text, "range[%d]", i );
		strings->Add( range->GetViewerString( text ) );
	}
}

/*
================
NiLODNode::Range::GetViewerString

The distances and the model location in a new string that the caller
deletes. The prefix is measured for the string's size but not printed.
================
*/
// FUNCTION: STBC 0x800380
char *NiLODNode::Range::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 124];
	sprintf( string, "N=%f,WN=%f,F=%f,WF=%f, loc=(%f,%f,%f)", nearDistance, worldNearDistance,
		farDistance, worldFarDistance, location.x, location.y, location.z );
	return string;
}
