/*
===============================================================================

	NiNode.h

	An interior node of the scene graph: an NiAVObject with an array of
	children and the list of dynamic effects that affect it.

	Evidence: the vtable at 0x898F2C (44 slots, 5 new); the constructor
	(0x7E37C0) builds the children array in place (vtable 0x898FDC, one
	smart pointer growing by 1), clears the effect list at 0xE0 and sets
	flag bit 3, which slot 26 reports; the creation function the stream
	registers for "NiNode" (0x7E5450) allocates 0xE8 bytes. NiAVObject's
	Update calls slot 43 on the parent after the downward pass;
	NiAVObject::AttachParent (0x7DC330) calls slot 40 with the object.
	The destructor (0x7E3880) detaches every effect and every child.
	The header includes NiDynamicEffect.h: every unit's NetImmerse class
	registrations run NiDynamicEffect's right before NiNode's.

===============================================================================
*/

#ifndef __NINODE_H__
#define __NINODE_H__

#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiStreamMacros.h"

// the effects that affect a node (Gamebryo lineage typedef); ~NiNode calls
// the out-of-line Remove (0x7E63D0) through its DetachAllEffects
typedef NiTSimpleList<NiDynamicEffect *>	NiDynamicEffectList;

/*
================
NiNodeLoadRecord

The link IDs of the children and of the effects, read by LoadBinary
(0x7E5630) and resolved by LinkObject (0x7E57D0). The class name is
invented.

Evidence: the creation functions of NiNode's subclasses (NiBone's 0x7F7990,
NiBezierMesh's 0x831510) allocate 0x58 bytes and build NiAVObject's record,
then two arrays at 0x28 (vtable 0x898FE8) and 0x40 (vtable 0x898FF0), each
with the default sizes; both vtables differ from that of
NiTArray<unsigned int> (0x88B914).
================
*/
// FUNCTION: STBC 0x7E55D0 ??_GNiNodeLoadRecord@@UAEPAXI@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7E55F0 ??1NiNodeLoadRecord@@UAE@XZ @netimmerse/NiNode
class NiNodeLoadRecord : public NiAVObjectLoadRecord {
public:
	// TODO identity: the element types; two distinct 4-byte types, taken
	// here as the pointer types the IDs stand for (the old stream format
	// saved the objects' addresses as their link IDs)
	NiTArray<NiAVObject *>	childLinkIDs;
	NiTArray<NiDynamicEffect *>	effectLinkIDs;
};

// FUNCTION: STBC 0x007e67d0 ??_GNiNode@@UAEPAXI@Z @netimmerse/NiNode
class NiNode : public NiAVObject {
public:
	// FUNCTION: STBC 0x4E3640 ?GetRTTI@NiNode@@UBEPBVNiRTTI@@XZ @netimmerse/NiNode
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiNode();

	virtual void			ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft );
	virtual void			CreateWorldVertices();
	virtual void			UpdateWorldVertices();
	virtual void			DestroyWorldVertices();
	virtual void			CreateWorldNormals();
	virtual void			UpdateWorldNormals();
	virtual void			DestroyWorldNormals();
	virtual NiAVObject *	GetObjectByName( const char *name_ );
	virtual void			VirtualSlot23( int );
	virtual void			VirtualSlot24();
	virtual void			UpdateDownwardPass( float time );
	// FUNCTION: STBC 0x4D5170 ?VirtualSlot26@NiNode@@UAE_NXZ @netimmerse/NiNode
	// the node's visual flag, read and set through the flag bits as the
	// debug copies do (AnimOnly.exe 0x420820, 0x420840: the setter is called
	// out of line by NiNode's constructor, UpdateDownwardPass, UpdateWorldBound
	// and NiSwitchNode::UpdateModelBound); GetViewerStrings shows the flag as
	// "m_bVisual", which names the setter (Gamebryo 1.1.2 keeps the reader as
	// NiAVObject::IsVisualObject)
	virtual bool			VirtualSlot26() { return GetBit( UNKNOWN_08_POS ); }
	void					SetVisual( bool visual ) { SetBit( visual, UNKNOWN_08_POS ); }
	virtual void			UpdatePropertiesDownward( NiPropertyState *parentState );
	virtual void			UpdateEffectsDownward( NiDynamicEffectState *parentState );
	virtual void			Display( NiCamera *camera );
	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );
	virtual void			UpdateWorldBound();

	// children (names from the Gamebryo lineage, which keeps these slots)
	virtual void			AttachChild( NiAVObject *child, bool firstAvail );
	virtual NiPointer<NiAVObject>	DetachChild( NiAVObject *child );
	virtual NiPointer<NiAVObject>	DetachChildAt( unsigned int index );
	virtual NiPointer<NiAVObject>	SetAt( unsigned int index, NiAVObject *child );

	// recomputes the bounds of this node and its ancestors
	virtual void			UpdateUpwardPass();

							NiNode();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the effects that affect this node and its subtree
	void					AttachEffect( NiDynamicEffect *effect );

	// the property and effect states at this node: the parent's with this
	// node's own applied (Gamebryo lineage names)
	NiPointer<NiPropertyState>	UpdatePropertiesUpward();
	NiPointer<NiDynamicEffectState>	UpdateEffectsUpward();
	void					DetachEffect( NiDynamicEffect *effect );
	// detaches every effect (Gamebryo lineage name; nothing in retail calls it)
	void					DetachAllEffects();
	// the effects attached to this node, the first in the embedded list
	// node (empty when its effect and next link are both NULL)
	const NiDynamicEffectList &	GetEffectList() const;

	unsigned int			GetArrayCount() const;
	unsigned int			GetChildCount() const;	// the non-null children (Gamebryo lineage name)
	// releases every child without detaching it (invented name; the damage
	// polygonizer clears its debug node with it, 0x4BEFA0)
	void					RemoveAllChildren();

	// whether two trees have the same shape: the same class at each place
	// and the same child count at each node (name invented; 0x7E3D70)
	static bool				IsSameShape( NiAVObject *object0, NiAVObject *object1 );
	NiAVObject *			GetAt( unsigned int index ) const;

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiNode *dest );

	// attaches the clones of the effects in list (the original's), last first
	void					AttachEffectClones( const NiDynamicEffectList *list );

	// parentState with this node's effects added: a new state when there
	// is no parent state, a copy when copyOnChange is set, else
	// parentState itself (Gamebryo lineage name)
	NiPointer<NiDynamicEffectState>	PushLocalEffects( NiDynamicEffectState *parentState, bool copyOnChange );

	NiTArray< NiPointer<NiAVObject> >	children;
	NiDynamicEffectList		effects;
};

NiRegisterStream( NiNode );

/*
================
NiNode::RemoveAllChildren
================
*/
inline void NiNode::RemoveAllChildren() {
	children.RemoveAll();
}

/*
================
NiNode::GetArrayCount
================
*/
inline unsigned int NiNode::GetArrayCount() const {
	return children.GetSize();
}

/*
================
NiNode::GetChildCount
================
*/
inline unsigned int NiNode::GetChildCount() const {
	return children.GetEffectiveSize();
}

/*
================
NiNode::GetAt

The child at index, or NULL past the end. The out-of-line copy (0x4321A0)
comes from a game unit.
================
*/
// FUNCTION: STBC 0x4321A0
inline NiAVObject *NiNode::GetAt( unsigned int index ) const {
	if ( children.GetSize() <= index ) {
		return 0;
	} else {
		return children.GetAt( index );
	}
}

/*
================
NiNode::GetEffectList
================
*/
inline const NiDynamicEffectList &NiNode::GetEffectList() const {
	return effects;
}

#endif /* !__NINODE_H__ */
