// NiSwitchNode.cpp -- nodes that show one child

#include "NiRTLib.h"
#include <assert.h>
#include "NiSwitchNode.h"
#include "NiCollisionRecord.h"
#include "NiPick.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "NiEnvMappedTriShape.h"
#include "NiEnvMappedTriShapeData.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiScreenPolygon.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fcd60 _$E30
// FUNCTION: STBC 0x007fcd70 _$E45
// FUNCTION: STBC 0x007fcd80 _$E49
// FUNCTION: STBC 0x007fcd90 _$E53
// FUNCTION: STBC 0x007fcda0 _$E57
// FUNCTION: STBC 0x007fcdb0 _$E181
// FUNCTION: STBC 0x007fcdc0 _$E34
// FUNCTION: STBC 0x007fcdd0 _$E123
// FUNCTION: STBC 0x007fcde0 _$E136
// FUNCTION: STBC 0x007fcdf0 _$E186
// FUNCTION: STBC 0x007fce00 _$E191
// FUNCTION: STBC 0x007fce10 _$E195
// FUNCTION: STBC 0x007fce20 _$E199
// FUNCTION: STBC 0x007fce30 _$E206
// FUNCTION: STBC 0x007fcee0 _$E210

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fcd40 _$E264
// FUNCTION: STBC 0x007fcce0 _$E17
// FUNCTION: STBC 0x007fcd00 _$E15
// FUNCTION: STBC 0x007fcd10 _$E23
// FUNCTION: STBC 0x007fcd30 _$E21

// FUNCTION: STBC 0x007fbaa0 _$E257
// FUNCTION: STBC 0x007fbac0 _$E254
// DATA: STBC 0x9A2030 ?rtti@NiSwitchNode@@2VNiRTTI@@B
const NiRTTI NiSwitchNode::rtti( "NiSwitchNode", &NiNode::rtti );

/*
================
NiSwitchNode::Register
================
*/
// FUNCTION: STBC 0x7FBAE0 ?Register@NiSwitchNode@@SAHXZ
NiImplementRegister( NiSwitchNode )

/*
================
NiSwitchNode::NiSwitchNode

No child is active, and every child is updated.
================
*/
// FUNCTION: STBC 0x7FBC30
NiSwitchNode::NiSwitchNode() {
	SetUpdateOnlyActive( false );
	index = -1;
	revisionID = 1;
	savedTime = 0.0f;
}

/*
================
NiSwitchNode::AttachChild

The new child starts at no revision.
================
*/
// FUNCTION: STBC 0x7FBCE0
void NiSwitchNode::AttachChild( NiAVObject *child, bool firstAvail ) {
	revisionID = 1;
	childRevisionIDs.Add( 0 );
	NiNode::AttachChild( child, firstAvail );
}

/*
================
NiSwitchNode::DetachChild
================
*/
// FUNCTION: STBC 0x7FBDF0
NiPointer<NiAVObject> NiSwitchNode::DetachChild( NiAVObject *child ) {
	revisionID = 1;
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		if ( child == children.GetAt( i ) ) {
			childRevisionIDs.RemoveAt( i );
			break;
		}
	}
	return NiNode::DetachChild( child );
}

/*
================
NiSwitchNode::DetachChildAt
================
*/
// FUNCTION: STBC 0x7FBEC0
NiPointer<NiAVObject> NiSwitchNode::DetachChildAt( unsigned int index_ ) {
	revisionID = 1;
	if ( index_ < children.GetSize() ) {
		childRevisionIDs.RemoveAt( index_ );
	}
	return NiNode::DetachChildAt( index_ );
}

/*
================
NiSwitchNode::SetAt

The child put at the index starts at no revision.
================
*/
// FUNCTION: STBC 0x7FBF30
NiPointer<NiAVObject> NiSwitchNode::SetAt( unsigned int index_, NiAVObject *child ) {
	revisionID = 1;
	childRevisionIDs.SetAtGrow( index_, 0 );
	return NiNode::SetAt( index_, child );
}

/*
================
NiSwitchNode::VirtualSlot35

NiNode's, then an active child that missed updates while inactive is
updated at the last update time.
================
*/
// FUNCTION: STBC 0x7FC050
void NiSwitchNode::VirtualSlot35( NiCamera *camera ) {
	NiNode::VirtualSlot35( camera );
	if ( index >= 0 && GetUpdateOnlyActive() ) {
		NiAVObject *child = children.GetAt( index );
		if ( childRevisionIDs.GetAt( index ) != revisionID ) {
			childRevisionIDs.SetAt( index, revisionID );
			child->UpdateDownwardPass( savedTime );
		}
	}
}

/*
================
NiSwitchNode::Display

Only the active child is drawn.
================
*/
// FUNCTION: STBC 0x7FC140
void NiSwitchNode::Display( NiCamera *camera ) {
	if ( index >= 0 ) {
		NiAVObject *child = children.GetAt( index );
		if ( child ) {
			child->CullShow( camera );
		}
	}
}

/*
================
NiSwitchNode::UpdateDownwardPass

With the update-only-active flag set, the update only starts a new
revision and the active child catches up when drawn; the world bound
follows the model bound either way.
================
*/
// FUNCTION: STBC 0x7FC1A0
void NiSwitchNode::UpdateDownwardPass( float time ) {
	UpdateWorldData();
	savedTime = time;

	if ( GetUpdateOnlyActive() ) {
		if ( index < 0 ) {
			return;
		}
		revisionID++;
	} else {
		for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
			NiAVObject *child = children.GetAt( i );
			if ( child ) {
				child->UpdateDownwardPass( time );
			}
		}
		UpdateModelBound();
	}

	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}

/*
================
NiSwitchNode::UpdateUpwardPass
================
*/
// FUNCTION: STBC 0x7FC270
void NiSwitchNode::UpdateUpwardPass() {
	UpdateModelBound();
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
	if ( GetParent() ) {
		GetParent()->UpdateUpwardPass();
	}
}

/*
================
NiSwitchNode::UpdateModelBound

Merges the world bounds of the children that have one, carried back into
the node's model space.
================
*/
// FUNCTION: STBC 0x7FC2B0
void NiSwitchNode::UpdateModelBound() {
	bool visible = false;	// (name invented; fits the /Od frame order)
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *avObject = children.GetAt( i );	// (name invented; fits the /Od frame order)
		if ( avObject && avObject->VirtualSlot26() ) {
			const NiBound &childWorldBound = avObject->GetWorldBound();	// (name invented; fits the /Od frame order)
			NiPoint3 delta = childWorldBound.GetCenter() - worldTranslate;	// (name invented; fits the /Od frame order)
			float oneOverScale = 1.0f / worldScale;	// (name invented; fits the /Od frame order)
			NiPoint3 localPos = ( delta * worldRotate ) * oneOverScale;	// (name invented; fits the /Od frame order)
			float childRadius = childWorldBound.GetRadius() * oneOverScale;	// (name invented; fits the /Od frame order)
			if ( !visible ) {
				visible = true;
				modelBound.SetCenterAndRadius( localPos, childRadius );
			} else {
				NiBound bound;
				bound.SetCenterAndRadius( localPos, childRadius );
				modelBound.Merge( &bound );
			}
		}
	}

	SetVisual( visible );
}

/*
================
NiSwitchNode::TestCollisions

With only the active child updated, only the active child is tested;
otherwise NiNode's test of every child.
================
*/
// FUNCTION: STBC 0x7FC480
bool NiSwitchNode::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	if ( GetUpdateOnlyActive() ) {
		if ( index >= 0 ) {
			NiAVObject *child = GetAt( index );
			if ( child ) {
				NiCollisionRecord record( record0.GetRoot(), child, record0.GetUnknown08(), record0.GetUnknown0C(), -1 );
				if ( record1.GetAVObject()->TestCollisions( deltaTime, record1, record ) ) {
					return true;
				}
			}
		}
	} else {
		return NiNode::TestCollisions( deltaTime, record0, record1 );
	}
	return false;
}

/*
================
NiSwitchNode::FindCollisions

As TestCollisions.
================
*/
// FUNCTION: STBC 0x7FC510
int NiSwitchNode::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	if ( GetUpdateOnlyActive() ) {
		if ( index >= 0 ) {
			NiAVObject *child = GetAt( index );
			if ( child ) {
				NiCollisionRecord record( record0.GetRoot(), child, record0.GetUnknown08(), record0.GetUnknown0C(), -1 );
				return record1.GetAVObject()->FindCollisions( deltaTime, record1, record );
			}
		}
	} else {
		return NiNode::FindCollisions( deltaTime, record0, record1 );
	}
	return 0;
}

/*
================
NiSwitchNode::FindIntersections

As NiNode's, with the active child alone.
The record goes through a local and the pick's done test is written out
with NiPick::GetSize, as the SDK's debug NetImmerse 3.1 compiles it
(AnimOnly.exe 0x43F3F8).
================
*/
// FUNCTION: STBC 0x7FC5A0
bool NiSwitchNode::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	if ( pick.GetObserveAppCullFlag() && GetAppCulled() ) {
		return false;
	}
	if ( !TestRayBound( origin, dir ) ) {
		return false;
	}
	if ( pick.GetIntersectType() == NiPick::BOUND_INTERSECT && pick.GetPickType() == NiPick::FIND_FIRST ) {
		NiPick::Record *newRecord = new NiPick::Record( this );
		assert( newRecord );
		pick.Add( newRecord );
		return true;
	}

	bool bFound = false;	// (name invented; fits the /Od frame order)
	if ( index >= 0 ) {
		if ( pick.GetPickType() == NiPick::FIND_FIRST && pick.GetSortType() == NiPick::NO_SORT && pick.GetSize() ) {
			return true;
		}
		NiAVObject *child = GetAt( index );
		if ( child && child->FindIntersections( origin, dir, pick ) ) {
			bFound = true;
		}
	}
	return bFound;
}

/*
================
NiSwitchNode::CreateClone
================
*/
// FUNCTION: STBC 0x7FC740
NiObject *NiSwitchNode::CreateClone() {
	NiSwitchNode *clone = new NiSwitchNode;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiSwitchNode::CopyMembers

The time and the revisions are not copied.
================
*/
// FUNCTION: STBC 0x7FC830
void NiSwitchNode::CopyMembers( NiSwitchNode *dest ) {
	NiNode::CopyMembers( dest );
	dest->index = index;
}

/*
================
NiSwitchNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7FC850
void NiSwitchNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSwitchNode *node = new NiSwitchNode;
	stream.SetLoadRecord( new NiSwitchNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiSwitchNode::LoadBinary

The index; the update-only-active flag comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7FCA10
void NiSwitchNode::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
	NiStreamLoadBinary( stream, index );
}

/*
================
NiSwitchNode::LinkObject
================
*/
// FUNCTION: STBC 0x7FCA40
void NiSwitchNode::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
}

/*
================
NiSwitchNode::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FCA50
bool NiSwitchNode::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiSwitchNode::SaveBinary
================
*/
// FUNCTION: STBC 0x7FCA60
void NiSwitchNode::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
	NiStreamSaveBinary( stream, index );
}

/*
================
NiSwitchNode::IsEqual
================
*/
// FUNCTION: STBC 0x7FCA90
bool NiSwitchNode::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiSwitchNode *node = (NiSwitchNode *)object;
	if ( index != node->index || GetUpdateOnlyActive() != node->GetUpdateOnlyActive() ) {
		return false;
	}
	return true;
}

/*
================
NiSwitchNode::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FCAE0
void NiSwitchNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_iIndex", index ) );
	strings->Add( NiGetViewerString( "m_bUpdateOnlyActive", GetUpdateOnlyActive() ) );
	strings->Add( modelBound.GetViewerString( "m_modelBound" ) );
}
