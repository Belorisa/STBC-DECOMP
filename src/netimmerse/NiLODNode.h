/*
===============================================================================

	NiLODNode.h

	A level-of-detail node: a switch node that activates the children
	whose distance range holds the camera's distance, measured from the
	first child's bound center or, with the position-in-range flag, from a
	location kept with each range.

	Evidence: the NiRTTI (0x9A21B0, built at 0x7FF0E0) is named "NiLODNode"
	with NiSwitchNode's as its base; the vtable at 0x899E40 has
	NiSwitchNode's 44 slots and adds none. The constructor (0x7FF270)
	builds the array of ranges at 0x120 (vtable 0x899EF0) and clears flag
	bit 6; the destructor (0x7FF2F0) frees every range. A range is 0x28
	bytes; its viewer string (0x800380) prints
	"N=%f,WN=%f,F=%f,WF=%f, loc=(%f,%f,%f)", and slot 37 (0x7FF5F0) derives
	the world members from the model ones. The node's viewer strings
	(0x800170) name "m_bPositionInRange" (flag bit 6).

===============================================================================
*/

#ifndef __NILODNODE_H__
#define __NILODNODE_H__

#include "NiSwitchNode.h"
#include "NiStreamMacros.h"

/*
================
NiLODNodeLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7FFD00) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x899EF4 last.
================
*/
// FUNCTION: STBC 0x7FFE40 ??_GNiLODNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FFE60 ??1NiLODNodeLoadRecord@@UAE@XZ
class NiLODNodeLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x00800610 ??_GNiLODNode@@UAEPAXI@Z
class NiLODNode : public NiSwitchNode {
public:
	/*
	================
	NiLODNode::Range

	The distances at which a child is active, in the node's model space and
	scaled to the world. The name is invented; the member names follow the
	viewer string's labels.
	================
	*/
	class Range {
	public:
		char *				GetViewerString( const char *prefix ) const;

		float				nearDistance;		// "N"
		float				worldNearDistance;	// "WN": nearDistance at the world scale
		float				farDistance;		// "F"
		float				worldFarDistance;	// "WF": farDistance at the world scale
		NiPoint3			location;			// "loc": the point measured from, with the position-in-range flag
		NiPoint3			worldLocation;		// location in world space
	};

	// FUNCTION: STBC 0x800600 ?GetRTTI@NiLODNode@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiLODNode();

	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual void			VirtualSlot35( NiCamera *camera );
	virtual void			VirtualSlot36( NiCamera *camera );
	virtual void			UpdateWorldData();

							NiLODNode();

	// the range of the child at index_, measured from the origin until set
	// otherwise (name invented)
	void					SetRange( float nearDistance, float farDistance, unsigned int index_ );
	// stores a range at the index, growing the array (name invented;
	// AnimOnly.exe 0x443B60, called by SetRange and LoadBinary)
	void					SetRange( Range *range, unsigned int index_ ) { ranges.SetAtGrow( index_, range ); }

	// measure from each range's location rather than the first child's
	// bound center
	void					SetPositionInRange( bool positionInRange );
	bool					GetPositionInRange() const { return ( flags >> POSITION_IN_RANGE_POS ) & 1; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// flags, above NiSwitchNode's
	enum {
		POSITION_IN_RANGE_MASK	= 0x0040,
		POSITION_IN_RANGE_POS	= 6
	};

	void					CopyMembers( NiLODNode *dest );

	// the world members of the ranges from the model ones (name invented)
	void					UpdateWorldRanges();

	NiTArray<Range *>		ranges;		// owned, one per child
};

NiRegisterStream( NiLODNode );

/*
================
NiLODNode::SetPositionInRange
================
*/
inline void NiLODNode::SetPositionInRange( bool positionInRange ) {
	if ( positionInRange ) {
		flags |= POSITION_IN_RANGE_MASK;
	} else {
		flags &= ~POSITION_IN_RANGE_MASK;
	}
}

#endif /* !__NILODNODE_H__ */
