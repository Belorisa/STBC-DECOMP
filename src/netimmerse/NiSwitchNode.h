/*
===============================================================================

	NiSwitchNode.h

	A node that shows one of its children: the child at the index is drawn
	and the others are skipped. With the update-only-active flag set, only
	the active child is updated, and each child catches up with the last
	update time when it becomes active.

	Evidence: the NiRTTI (0x9A2030, built at 0x7FBA90) is named
	"NiSwitchNode" with NiNode's as its base; the vtable at 0x899BD4 has
	NiNode's 44 slots and adds none. The constructor (0x7FBC30) sets the
	index at 0xE8 to -1, the time at 0xEC to 0, the revision at 0xF0 to 1,
	builds the per-child revisions at 0xF4 and the model bound at 0x10C, and
	clears flag bit 5. The viewer strings (0x7FCAE0) name "m_iIndex",
	"m_bUpdateOnlyActive" (flag bit 5) and "m_modelBound"; the other names
	follow the Gamebryo lineage, which keeps the class.

===============================================================================
*/

#ifndef __NISWITCHNODE_H__
#define __NISWITCHNODE_H__

#include "NiCamera.h"
#include "NiNode.h"
#include "NiStreamMacros.h"

/*
================
NiSwitchNodeLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7FC850) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x899C84 last.
================
*/
// FUNCTION: STBC 0x7FC9B0 ??_GNiSwitchNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FC9D0 ??1NiSwitchNodeLoadRecord@@UAE@XZ
class NiSwitchNodeLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x7FCE50 ??_GNiSwitchNode@@UAEPAXI@Z @netimmerse/NiSwitchNode
// FUNCTION: STBC 0x7FCE70 ??1NiSwitchNode@@UAE@XZ @netimmerse/NiSwitchNode
class NiSwitchNode : public NiNode {
public:
	// FUNCTION: STBC 0x7FCE40 ?GetRTTI@NiSwitchNode@@UBEPBVNiRTTI@@XZ @netimmerse/NiSwitchNode
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual void			UpdateDownwardPass( float time );
	virtual void			Display( NiCamera *camera );
	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );
	virtual void			VirtualSlot35( NiCamera *camera );

	// children: each change starts a new revision
	virtual void			AttachChild( NiAVObject *child, bool firstAvail );
	virtual NiPointer<NiAVObject>	DetachChild( NiAVObject *child );
	virtual NiPointer<NiAVObject>	DetachChildAt( unsigned int index_ );
	virtual NiPointer<NiAVObject>	SetAt( unsigned int index_, NiAVObject *child );

	virtual void			UpdateUpwardPass();

							NiSwitchNode();

	// the active child; -1 for none
	void					SetIndex( int index_ ) { index = index_; }
	int						GetIndex() const { return index; }
	// the child at the index, or NULL when the index is out of range
	// (Gamebryo lineage name). Retail compares the size against the index
	// unsigned (cmp size, index; jbe: DamageableObject::Unknown593270),
	// where Gamebryo 1.1.2 tests index < int( size ).
	NiAVObject *			GetActiveChild() const {
								if ( index >= 0 && children.GetSize() > (unsigned int)index ) {
									return children.GetAt( index );
								}
								return 0;
							}

	void					SetUpdateOnlyActive( bool updateOnlyActive );
	bool					GetUpdateOnlyActive() const { return ( flags >> UPDATE_ONLY_ACTIVE_POS ) & 1; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// flags, above NiAVObject's
	enum {
		UPDATE_ONLY_ACTIVE_MASK	= 0x0020,
		UPDATE_ONLY_ACTIVE_POS	= 5
	};

	void					CopyMembers( NiSwitchNode *dest );

	// the children's bound in the node's model space (name invented)
	void					UpdateModelBound();

	int						index;				// the active child, -1 for none
	float					savedTime;			// the time of the last update
	unsigned int			revisionID;			// advanced by each update of the active child alone
	// TODO identity: the element type; a 4-byte integer whose array vtable
	// (0x898518) differs from those of NiTArray<unsigned int> (0x88B914),
	// NiTArray<int> (0x8987F0) and NiTArray<unsigned long> (0x8990D8)
	NiTArray<long>			childRevisionIDs;	// the revision each child was last updated at
	NiBound					modelBound;
};

NiRegisterStream( NiSwitchNode );

/*
================
NiSwitchNode::SetUpdateOnlyActive
================
*/
inline void NiSwitchNode::SetUpdateOnlyActive( bool updateOnlyActive ) {
	if ( updateOnlyActive ) {
		flags |= UPDATE_ONLY_ACTIVE_MASK;
	} else {
		flags &= ~UPDATE_ONLY_ACTIVE_MASK;
	}
}

#endif /* !__NISWITCHNODE_H__ */
