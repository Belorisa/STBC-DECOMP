// NiNode.cpp -- interior nodes of the scene graph

#include "NiRTLib.h"
#include <assert.h>
#include <stdio.h>

#include "NiNode.h"
#include "NiCollisionRecord.h"
#include "NiPick.h"
#include "NiDynamicEffect.h"
#include "NiStream.h"
#include "NiCloneExtraData.h"
#include "NiDynamicEffectState.h"
#include "NiClusterAccumulator.h"
#include "NiTriBasedGeom.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e5d50 _$E30
// FUNCTION: STBC 0x007e5d60 _$E41
// FUNCTION: STBC 0x007e5d70 _$E45
// FUNCTION: STBC 0x007e5de0 _$E49
// FUNCTION: STBC 0x007e5f30 _$E53
// FUNCTION: STBC 0x007e60b0 _$E77
// FUNCTION: STBC 0x007e6510 _$E177
// FUNCTION: STBC 0x007e6650 _$E194
// FUNCTION: STBC 0x007e6770 _$E119
// FUNCTION: STBC 0x007e6780 _$E132
// FUNCTION: STBC 0x007e6790 _$E182
// FUNCTION: STBC 0x007e67c0 _$E199
// FUNCTION: STBC 0x007e67f0 _$E186
// FUNCTION: STBC 0x007e6800 _$E203
// FUNCTION: STBC 0x007e6850 _$E210
// FUNCTION: STBC 0x007e6860 _$E232
// FUNCTION: STBC 0x007e6870 _$E218
// FUNCTION: STBC 0x007e6880 _$E225
// FUNCTION: STBC 0x007e6890 _$E236
// FUNCTION: STBC 0x007e68a0 _$E240

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e6660 _$E255
// FUNCTION: STBC 0x007e5cb0 _$E17
// FUNCTION: STBC 0x007e5cd0 _$E15
// FUNCTION: STBC 0x007e5ce0 _$E23
// FUNCTION: STBC 0x007e5d00 _$E21

// FUNCTION: STBC 0x007e3630 _$E246
// FUNCTION: STBC 0x007e3650 _$E243
// DATA: STBC 0x9A1870 ?rtti@NiNode@@2VNiRTTI@@B
const NiRTTI NiNode::rtti( "NiNode", &NiAVObject::rtti );

/*
================
NiNode::Register
================
*/
// FUNCTION: STBC 0x7E3670 ?Register@NiNode@@SAHXZ
NiImplementRegister( NiNode )

/*
================
NiNode::NiNode
================
*/
// FUNCTION: STBC 0x7E37C0
NiNode::NiNode() {
	SetVisual( true );
}

/*
================
NiNode::~NiNode

Detaches the effects, then the children (Gamebryo 1.2.2: NiNode::~NiNode
calls DetachAllEffects; its Remove stays out of line here, expanded in
DetachEffect and DetachAllEffects themselves).
================
*/
// FUNCTION: STBC 0x7E3880
NiNode::~NiNode() {
	DetachAllEffects();

	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiPointer<NiAVObject> child = DetachChildAt( i );
		child = 0;
	}
}

/*
================
NiNode::AttachChild

Takes the child from its old parent; firstAvail reuses an empty slot.
================
*/
// FUNCTION: STBC 0x7E39B0
void NiNode::AttachChild( NiAVObject *child, bool firstAvail ) {
	assert( child );
	if ( !child ) {
		return;
	}

	// keeps the child alive while it leaves its old parent
	child->IncRefCount();
	child->AttachParent( this );
	if ( firstAvail ) {
		children.AddFirstEmpty( child );
	} else {
		children.Add( child );
	}
	assert( child->GetRefCount() >= 2 );
	child->DecRefCount();
}

/*
================
NiNode::DetachChildAt

Removes the child at index and returns it; NULL past the end.
================
*/
// FUNCTION: STBC 0x7E3A30
NiPointer<NiAVObject> NiNode::DetachChildAt( unsigned int index ) {
	if ( index < children.GetSize() ) {
		NiPointer<NiAVObject> child = children.GetAt( index );
		if ( child ) {
			child->DetachParent();
			children.RemoveAt( index );
		}
		return child;
	}
	return 0;
}

/*
================
NiNode::DetachChild

Removes the child and returns it; NULL when it is not a child.
================
*/
// FUNCTION: STBC 0x7E3B30
NiPointer<NiAVObject> NiNode::DetachChild( NiAVObject *child ) {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiPointer<NiAVObject> current = children.GetAt( i );
		if ( current && current == child ) {
			current->DetachParent();
			children.RemoveAt( i );
			return current;
		}
	}
	return 0;
}

/*
================
NiNode::SetAt

Puts the child at index and returns the child it replaces.
================
*/
// FUNCTION: STBC 0x7E3C50
NiPointer<NiAVObject> NiNode::SetAt( unsigned int index, NiAVObject *child ) {
	if ( children.GetSize() <= index ) {
		if ( child ) {
			child->AttachParent( this );
		}
		children.SetAtGrow( index, child );
		return 0;
	}

	NiPointer<NiAVObject> previous = children.GetAt( index );
	if ( previous ) {
		previous->DetachParent();
	}
	if ( child ) {
		child->AttachParent( this );
	}
	children.SetAt( index, child );
	return previous;
}

/*
================
NiNode::IsSameShape

Whether two trees have the same classes and child counts at every node.
================
*/
// FUNCTION: STBC 0x7E3D70 ?IsSameShape@NiNode@@SA_NPAVNiAVObject@@0@Z
bool NiNode::IsSameShape( NiAVObject *object0, NiAVObject *object1 ) {
	if ( object0 == object1 ) {
		return true;
	}
	if ( !object0->IsExactKindOf( object1->GetRTTI() ) ) {
		return false;
	}
	NiNode *pNode0 = NiDynamicCast( NiNode, object0 );
	if ( pNode0 ) {
		NiNode *pNode1 = (NiNode *)object1;
		unsigned int uiCount0 = pNode0->GetArrayCount();
		unsigned int uiCount1 = pNode1->GetArrayCount();
		if ( uiCount0 != uiCount1 ) {
			return false;
		}
		for ( unsigned int i = 0; i < uiCount0; i++ ) {
			NiAVObject *pChild0 = pNode0->GetAt( i );
			NiAVObject *pChild1 = pNode1->GetAt( i );
			if ( ( pChild0 && !pChild1 ) || ( !pChild0 && pChild1 ) ) {
				return false;
			}
			if ( pChild0 && !IsSameShape( pChild0, pChild1 ) ) {
				return false;
			}
		}
	}
	return true;
}
/*
================
NiNode::ApplyTransform

Passes the transform on to the children, in their parent's space: with
onLeft it applies in this node's parent space and is carried into its model
space through the local transform first.
================
*/
// FUNCTION: STBC 0x7E3E30
void NiNode::ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft ) {
	unsigned int i;
	NiAVObject *pChild;

	if ( onLeft ) {
		NiMatrix3 m1 = localRotate.TransposeTimes( rotate * localRotate );	// (name invented; fits the /Od frame order)
		NiPoint3 t1 = ( ( rotate * localTranslate + translate - localTranslate ) * localRotate ) / localScale;	// (name invented; fits the /Od frame order)
		for ( i = 0; i < children.GetSize(); i++ ) {
			pChild = children.GetAt( i );
			if ( pChild ) {
				pChild->ApplyTransform( m1, t1, true );
			}
		}
	} else {
		for ( i = 0; i < children.GetSize(); i++ ) {
			pChild = children.GetAt( i );
			if ( pChild ) {
				pChild->ApplyTransform( rotate, translate, true );
			}
		}
	}
}

/*
================
NiNode::UpdateDownwardPass

Runs the controllers of the properties and of the node, updates the world
data and passes the call on to the children, merging the world bounds of
those that are displayed.
================
*/
// FUNCTION: STBC 0x7E3FF0
void NiNode::UpdateDownwardPass( float time ) {
	NiPropertyList *propertyList = &properties;	// (name invented; fits the /Od frame order)
	while ( propertyList ) {
		NiProperty *property = propertyList->GetItem();
		if ( property && property->GetControllers() ) {
			property->Update( time );
		}
		propertyList = propertyList->GetNext();
	}

	for ( NiTimeController *control = GetControllers(); control; control = control->GetNext() ) {	// (name invented; fits the /Od frame order)
		control->Update( time );
	}

	UpdateWorldData();

	bool visible = false;	// (name invented; fits the /Od frame order)
	NiBound *nodeBound = &worldBound;	// (name invented; fits the /Od frame order)
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->UpdateDownwardPass( time );
			if ( child->VirtualSlot26() ) {
				if ( !visible ) {
					visible = true;
					*nodeBound = child->GetWorldBound();
				} else {
					nodeBound->Merge( &child->GetWorldBound() );
				}
			}
		}
	}

	SetVisual( visible );
}

/*
================
NiNode::UpdateUpwardPass
================
*/
// FUNCTION: STBC 0x7E4150
void NiNode::UpdateUpwardPass() {
	NiNode *parentNode = GetParent();
	UpdateWorldBound();
	if ( parentNode ) {
		parentNode->UpdateUpwardPass();
	}
}

/*
================
NiNode::UpdateWorldBound

The world bound of the children that are displayed, merged; the node is
marked when there is one.
================
*/
// FUNCTION: STBC 0x7E4170
void NiNode::UpdateWorldBound() {
	bool visible = false;	// (name invented; fits the /Od frame order)
	NiBound *nodeBound = &worldBound;	// (name invented; fits the /Od frame order)
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child && child->VirtualSlot26() ) {
			if ( !visible ) {
				visible = true;
				*nodeBound = child->GetWorldBound();
			} else {
				nodeBound->Merge( &child->GetWorldBound() );
			}
		}
	}
	SetVisual( visible );
}

/*
================
NiNode::UpdatePropertiesUpward

The property state at this node: the ancestors' properties, then this
node's.
================
*/
// FUNCTION: STBC 0x7E4220
NiPointer<NiPropertyState> NiNode::UpdatePropertiesUpward() {
	NiPointer<NiPropertyState> parentState;
	if ( parent ) {
		parentState = parent->UpdatePropertiesUpward();
	} else {
		parentState = new NiPropertyState;
	}
	return PushLocalProperties( parentState, false );
}

/*
================
NiNode::UpdateEffectsUpward

The effect state at this node: the ancestors' effects, then this node's.
================
*/
// FUNCTION: STBC 0x7E4330
NiPointer<NiDynamicEffectState> NiNode::UpdateEffectsUpward() {
	NiPointer<NiDynamicEffectState> parentState;
	if ( parent ) {
		parentState = parent->UpdateEffectsUpward();
	}
	return PushLocalEffects( parentState, false );
}

/*
================
NiNode::PushLocalEffects
================
*/
// FUNCTION: STBC 0x7E4400
NiPointer<NiDynamicEffectState> NiNode::PushLocalEffects( NiDynamicEffectState *parentState, bool copyOnChange ) {
	if ( GetEffectList().IsEmpty() ) {
		return parentState;
	}

	NiPointer<NiDynamicEffectState> state;
	if ( !parentState ) {
		state = new NiDynamicEffectState;
	} else if ( copyOnChange ) {
		state = parentState->Copy();
	} else {
		state = parentState;
	}

	NiDynamicEffectList *effectList = &effects;	// (name invented; fits the /Od frame order)
	while ( effectList && effectList->GetItem() ) {
		state->AddEffect( effectList->GetItem() );
		effectList = effectList->GetNext();
	}
	return state;
}

/*
================
NiNode::UpdatePropertiesDownward

Passes a copy of the parent's state with this node's properties applied to
every child.
================
*/
// FUNCTION: STBC 0x7E4530
void NiNode::UpdatePropertiesDownward( NiPropertyState *parentState ) {
	NiPointer<NiPropertyState> spState = PushLocalProperties( parentState, true );
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->UpdatePropertiesDownward( spState );
		}
	}
}

/*
================
NiNode::UpdateEffectsDownward

Passes a copy of the parent's state with this node's effects added to
every child.
================
*/
// FUNCTION: STBC 0x7E4610
void NiNode::UpdateEffectsDownward( NiDynamicEffectState *parentState ) {
	NiPointer<NiDynamicEffectState> spState = PushLocalEffects( parentState, true );
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->UpdateEffectsDownward( spState );
		}
	}
}

/*
================
NiNode::Display

Passes the call on to every child, when the node is displayed.
================
*/
// FUNCTION: STBC 0x7E46F0
void NiNode::Display( NiCamera *camera ) {
	if ( VirtualSlot26() ) {
		for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
			NiAVObject *child = children.GetAt( i );
			if ( child ) {
				child->CullShow( camera );
			}
		}
	}
}

/*
================
NiNode::AttachEffect

The effect attaches the node in turn; each side ignores a partner it
already lists.
================
*/
// FUNCTION: STBC 0x7E4770
void NiNode::AttachEffect( NiDynamicEffect *effect ) {
	if ( !effects.IsInList( effect ) ) {
		effects.AddHead( effect );
		effect->AttachAffectedNode( this );
	}
}

/*
================
NiNode::DetachEffect
================
*/
// FUNCTION: STBC 0x7E47C0
void NiNode::DetachEffect( NiDynamicEffect *effect ) {
	if ( effects.IsInList( effect ) ) {
		effects.Remove( effect );
		effect->DetachAffectedNode( this );
	}
}

/*
================
NiNode::DetachAllEffects

Takes the effects off the list one by one, each forgetting this node.
================
*/
// FUNCTION: STBC 0x7E4870
void NiNode::DetachAllEffects() {
	while ( !effects.IsEmpty() ) {
		NiDynamicEffect *effect = effects.item;
		effects.Remove( effect );
		effect->DetachAffectedNode( this );
	}
}

/*
================
NiNode::CreateWorldVertices

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4900
void NiNode::CreateWorldVertices() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->CreateWorldVertices();
		}
	}
}

/*
================
NiNode::UpdateWorldVertices

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4940
void NiNode::UpdateWorldVertices() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->UpdateWorldVertices();
		}
	}
}

/*
================
NiNode::DestroyWorldVertices

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4980
void NiNode::DestroyWorldVertices() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->DestroyWorldVertices();
		}
	}
}

/*
================
NiNode::CreateWorldNormals

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E49C0
void NiNode::CreateWorldNormals() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->CreateWorldNormals();
		}
	}
}

/*
================
NiNode::UpdateWorldNormals

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4A00
void NiNode::UpdateWorldNormals() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->UpdateWorldNormals();
		}
	}
}

/*
================
NiNode::DestroyWorldNormals

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4A40
void NiNode::DestroyWorldNormals() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->DestroyWorldNormals();
		}
	}
}

/*
================
NiNode::VirtualSlot23

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4A80
void NiNode::VirtualSlot23( int value ) {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->VirtualSlot23( value );
		}
	}
}

/*
================
NiNode::VirtualSlot24

Passes the call on to the children.
================
*/
// FUNCTION: STBC 0x7E4AC0
void NiNode::VirtualSlot24() {
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->VirtualSlot24();
		}
	}
}

/*
================
NiNode::TestCollisions

Whether this subtree meets the object of the other record within
deltaTime. A node that propagates passes the test on to its children;
otherwise the other object's tree is descended, or the bounds decide.
================
*/
// FUNCTION: STBC 0x7E4B00
bool NiNode::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	NiAVObject *other = record1.GetAVObject();
	NiCollisionRecord childRecord( record0.GetRoot(), 0, record0.GetUnknown08(), record0.GetUnknown0C(), 0 );

	if ( !TestBoundIntersect( deltaTime, other ) ) {
		return false;
	}

	if ( GetPropagationMode() == PROPAGATE_NONE ) {
		if ( other->GetPropagationMode() == PROPAGATE_NONE ) {
			return true;
		}
		if ( other->TestCollisions( deltaTime, record1, record0 ) ) {
			return true;
		}
	} else {
		for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
			NiAVObject *child = GetAt( i );
			if ( child ) {
				childRecord.SetObject( child );
				if ( child->TestCollisions( deltaTime, childRecord, record1 ) ) {
					return true;
				}
			}
		}
	}
	return false;
}

/*
================
NiNode::FindCollisions

As TestCollisions, but two leaves of the descent are met by their bounds
and the collision goes to the roots' callbacks.
================
*/
// FUNCTION: STBC 0x7E4BD0
int NiNode::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( record0.GetAVObject() == this );
	NiAVObject *other = record1.GetAVObject();
	NiCollisionRecord childRecord( record0.GetRoot(), 0, record0.GetUnknown08(), record0.GetUnknown0C(), 0 );

	if ( !TestBoundIntersect( deltaTime, other ) ) {
		return CONTINUE_COLLISIONS;
	}

	if ( GetPropagationMode() == PROPAGATE_NONE ) {
		if ( other->GetPropagationMode() == PROPAGATE_NONE ) {
			NiCollisionIntersect intersect;
			if ( FindBoundIntersect( deltaTime, other, intersect.time, intersect.point, true, intersect.normal0,
									 intersect.normal1 ) ) {
				intersect.root0 = record0.GetRoot();
				intersect.object0 = record0.GetAVObject();
				intersect.root1 = record1.GetRoot();
				intersect.object1 = record1.GetAVObject();
				return ReportCollision( intersect );
			}
		} else {
			return other->FindCollisions( deltaTime, record1, record0 );
		}
	} else {
		for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
			NiAVObject *child = children.GetAt( i );
			if ( child ) {
				childRecord.SetObject( child );
				if ( child->FindCollisions( deltaTime, childRecord, record1 ) == TERMINATE_COLLISIONS ) {
					return TERMINATE_COLLISIONS;
				}
			}
		}
	}
	return CONTINUE_COLLISIONS;
}

/*
================
NiNode::FindIntersections

A bound pick that keeps the first hit stops at the node; otherwise the
children are searched until the pick is done.
The record goes through a local and the pick's done test is written out
with NiPick::GetSize, as the SDK's debug NetImmerse 3.1 compiles it
(AnimOnly.exe 0x41DE11).
================
*/
// FUNCTION: STBC 0x7E4D30
bool NiNode::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
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
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		if ( pick.GetPickType() == NiPick::FIND_FIRST && pick.GetSortType() == NiPick::NO_SORT && pick.GetSize() ) {
			return true;
		}
		NiAVObject *child = GetAt( i );
		if ( child && child->FindIntersections( origin, dir, pick ) ) {
			bFound = true;
		}
	}
	return bFound;
}

/*
================
NiNode::GetObjectByName

The node itself when its name matches, else the first match in the
subtree.
================
*/
// FUNCTION: STBC 0x7E4EE0
NiAVObject *NiNode::GetObjectByName( const char *name_ ) {
	NiAVObject *object = NiAVObject::GetObjectByName( name_ );
	if ( object ) {
		return object;
	}

	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			object = child->GetObjectByName( name_ );
			if ( object ) {
				return object;
			}
		}
	}
	return 0;
}

/*
================
NiNode::CreateClone
================
*/
// FUNCTION: STBC 0x7E4F30
NiObject *NiNode::CreateClone() {
	NiNode *clone = new NiNode;
	CopyMembers( clone );
	return clone;
}

/*
================
NiNode::CopyMembers

NiAVObject's members, then a clone of each child at the same index.
================
*/
// FUNCTION: STBC 0x7E50C0
void NiNode::CopyMembers( NiNode *dest ) {
	NiAVObject::CopyMembers( dest );
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			NiAVObject *clone = (NiAVObject *)child->CreateClone();
			assert( clone );
			dest->SetAt( i, clone );
		}
	}
}

/*
================
NiNode::ProcessClone

NiAVObject's, then the clone takes the clones of the effects and each
child processes its clone.
================
*/
// FUNCTION: STBC 0x7E5180
void NiNode::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiNode, clone ) );
	NiAVObject::ProcessClone( clone );
	assert( GetRTTI() == clone->GetRTTI() );

	NiNode *node = NiDynamicCast( NiNode, clone );
	const NiDynamicEffectList *pList = &GetEffectList();
	if ( !pList->IsEmpty() ) {
		node->AttachEffectClones( pList );
	}

	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child[2];
		child[0] = children.GetAt( i );
		if ( child[0] ) {
			child[1] = node->children.GetAt( i );
			assert( child[1] != NULL );
			child[0]->ProcessClone( child[1] );
		}
	}
}

/*
================
NiNode::AttachEffectClones

Attaches, last first, the clone of each effect in list, or the effect
itself when it was not cloned. The name is invented.
================
*/
// FUNCTION: STBC 0x7E5300
void NiNode::AttachEffectClones( const NiDynamicEffectList *list ) {
	if ( list ) {
		AttachEffectClones( list->GetNext() );
		NiDynamicEffect *pEffect = list->GetItem();
		NiExtraData *extraData = pEffect->GetExtraData();
		NiCloneExtraData *pCloneData = NiDynamicCast( NiCloneExtraData, extraData );	// (name invented; fits the /Od frame order)
		if ( pCloneData ) {
			NiObject *clone = pCloneData->GetClone();
			pEffect = NiDynamicCast( NiDynamicEffect, clone );
			assert( pEffect );
		}
		AttachEffect( pEffect );
	}
}

/*
================
NiNode::VirtualSlot03

NiAVObject's, then each child's.
================
*/
// FUNCTION: STBC 0x7E53E0
void NiNode::VirtualSlot03() {
	NiAVObject::VirtualSlot03();
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->VirtualSlot03();
		}
	}
}

/*
================
NiNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7E5450
void NiNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiNode *node = new NiNode;
	stream.SetLoadRecord( new NiNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiNode::LoadBinary

NiAVObject's members, then the link IDs of the children and of the
effects, kept in the load record for LinkObject.
================
*/
// FUNCTION: STBC 0x7E5630
void NiNode::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );

	NiNodeLoadRecord *record = (NiNodeLoadRecord *)stream.GetLoadRecord();
	unsigned int linkCount;	// (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, linkCount );
	unsigned int i;
	for ( i = 0; i < linkCount; i++ ) {
		NiAVObject *childLinkID;
		NiStreamLoadBinary( stream, childLinkID );
		record->childLinkIDs.Add( childLinkID );
	}

	NiStreamLoadBinary( stream, linkCount );
	for ( i = 0; i < linkCount; i++ ) {
		NiDynamicEffect *effectLinkID;
		NiStreamLoadBinary( stream, effectLinkID );
		record->effectLinkIDs.Add( effectLinkID );
	}
}

/*
================
NiNode::LinkObject

Places each child at its saved index, then attaches the effects, last
first.
================
*/
// FUNCTION: STBC 0x7E57D0
void NiNode::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );

	NiNodeLoadRecord *record = (NiNodeLoadRecord *)stream.GetLoadRecord();
	unsigned int i;
	for ( i = 0; i < record->childLinkIDs.GetSize(); i++ ) {
		unsigned int childLinkID = (unsigned int)record->childLinkIDs.GetAt( i );	// (name invented; fits the /Od frame order)
		NiAVObject *child = (NiAVObject *)stream.GetObjectFromLinkID( childLinkID );
		SetAt( i, child );
	}
	int linkIDCount = record->effectLinkIDs.GetSize();	// (name invented; fits the /Od frame order)
	for ( int j = linkIDCount - 1; j >= 0; j-- ) {
		NiDynamicEffect *effect = (NiDynamicEffect *)stream.GetObjectFromLinkID( (unsigned int)record->effectLinkIDs.GetAt( j ) );
		AttachEffect( effect );
	}
}

/*
================
NiNode::RegisterStreamables

Registers the children and the effects after the node.
================
*/
// FUNCTION: STBC 0x7E58D0
bool NiNode::RegisterStreamables( NiStream &stream ) {
	if ( !NiAVObject::RegisterStreamables( stream ) ) {
		return false;
	}
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = GetAt( i );
		if ( child ) {
			child->RegisterStreamables( stream );
		}
	}
	NiDynamicEffectList *list = &effects;
	while ( list ) {
		NiDynamicEffect *effect = list->GetItem();
		if ( effect ) {
			effect->RegisterStreamables( stream );
		}
		list = list->GetNext();
	}
	return true;
}

/*
================
NiNode::SaveBinary

The order LoadBinary reads: the link IDs (addresses) of the children, NULL
children included, then those of the effects.
================
*/
// FUNCTION: STBC 0x7E5940
void NiNode::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );

	unsigned int size = children.GetSize();	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, size );
	for ( unsigned int i = 0; i < size; i++ ) {
		NiAVObject *child = GetAt( i );
		NiStreamSaveBinary( stream, child );
	}

	unsigned int numEffects = effects.ItemsInList();	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, numEffects );
	if ( numEffects > 0 ) {
		NiDynamicEffectList *list = &effects;
		while ( list ) {
			NiDynamicEffect *effect = list->GetItem();
			NiStreamSaveBinary( stream, effect );
			list = list->GetNext();
		}
	}
}

/*
================
NiNode::IsEqual

Compares the children by index and the effects in list order.

Gamebryo 1.2.2: NiNode::IsEqual tests a one-sided null pair first, then
compares the non-null pair.
================
*/
// FUNCTION: STBC 0x7E5A00
bool NiNode::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiNode *pNode = (NiNode *)object;
	if ( VirtualSlot26() != pNode->VirtualSlot26() ) {
		return false;
	}

	unsigned int uiCount0 = children.GetSize();
	unsigned int uiCount1 = pNode->children.GetSize();
	if ( uiCount0 != uiCount1 ) {
		return false;
	}
	unsigned int i;
	for ( i = 0; i < uiCount0; i++ ) {
		NiAVObject *pChild0 = GetAt( i );
		NiAVObject *pChild1 = pNode->GetAt( i );
		if ( ( pChild0 && !pChild1 ) || ( !pChild0 && pChild1 ) ) {
			return false;
		}
		if ( pChild0 && !pChild0->IsEqual( pChild1 ) ) {
			return false;
		}
	}

	uiCount0 = effects.ItemsInList();
	uiCount1 = pNode->effects.ItemsInList();
	if ( uiCount0 != uiCount1 ) {
		return false;
	}
	if ( uiCount0 > 0 ) {
		NiDynamicEffectList *pPos0 = &effects;
		NiDynamicEffectList *pPos1 = &pNode->effects;
		while ( pPos0 ) {
			NiDynamicEffect *pEffect0 = pPos0->GetItem();
			NiDynamicEffect *pEffect1 = pPos1->GetItem();
			if ( ( pEffect0 && !pEffect1 ) || ( !pEffect0 && pEffect1 ) ) {
				return false;
			}
			if ( pEffect0 && !pEffect0->IsEqual( pEffect1 ) ) {
				return false;
			}
			pPos0 = pPos0->GetNext();
			pPos1 = pPos1->GetNext();
		}
	}
	return true;
}

/*
================
NiNode::GetViewerStrings

NiAVObject's strings, the visibility flag, then one string per effect. The
list is walked through its accessors, as the SDK's debug NetImmerse 3.1
does (AnimOnly.exe 0x41EAC5).
================
*/
// FUNCTION: STBC 0x7E5B30
void NiNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiAVObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bVisual", VirtualSlot26() ) );

	if ( effects.ItemsInList() > 0 ) {
		char text[64];	// (name invented; fits the /Od frame order)
		unsigned int i = 0;
		NiDynamicEffectList *pList = &effects;
		while ( pList ) {
			NiDynamicEffect *effect = pList->GetItem();
			sprintf( text, "effect[%d]", i );
			strings->Add( NiGetViewerString( text, effect ) );
			pList = pList->GetNext();
			i++;
		}
	}
}



