/*
===============================================================================

	NiFltAnimationNode.h

	A switch node that flips through its children over time, as the
	animation nodes of OpenFlight models do: each draw shows the child for
	the last update time within the period, running forward and, with the
	bounce flag, back again.

	Evidence: the NiRTTI (0x9A20B0, built at 0x7FCEF0) is named
	"NiFltAnimationNode" with NiSwitchNode's as its base; the vtable at
	0x899C88 has NiSwitchNode's 44 slots and adds none. The creation
	function (0x7FD230) allocates 0x128 bytes; the constructor, inlined
	there, clears flag bit 6 and sets the double at 0x120 to 1.0. The viewer
	strings (0x7FD480) name "m_dPeriod" and "m_bBounce" (flag bit 6).

===============================================================================
*/

#ifndef __NIFLTANIMATIONNODE_H__
#define __NIFLTANIMATIONNODE_H__

#include "NiSwitchNode.h"
#include "NiStreamMacros.h"

/*
================
NiFltAnimationNodeLoadRecord

The record CreateObject hands the stream: NiSwitchNode's, under its own
vtable, as the node derives from NiSwitchNode. The class name is invented.

Evidence: CreateObject (0x7FD230) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x899D38 last; the extra level of constructor
nesting shows in what CreateObject expands (it calls NiObjectNETLoadRecord's
constructor, 0x7F7C50, out of line).
================
*/
// FUNCTION: STBC 0x7FD350 ??_GNiFltAnimationNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FD370 ??1NiFltAnimationNodeLoadRecord@@UAE@XZ
class NiFltAnimationNodeLoadRecord : public NiSwitchNodeLoadRecord {
};

// FUNCTION: STBC 0x7FD7F0 ??_GNiFltAnimationNode@@UAEPAXI@Z
// FUNCTION: STBC 0x7FD810 ??1NiFltAnimationNode@@UAE@XZ
class NiFltAnimationNode : public NiSwitchNode {
public:
	// FUNCTION: STBC 0x7FD7E0 ?GetRTTI@NiFltAnimationNode@@UBEPBVNiRTTI@@XZ
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

	virtual void			Display( NiCamera *camera );

	// a one-second period, without bouncing
							NiFltAnimationNode() { SetBounce( false ); period = 1.0; }

	// the time to run through every child once
	void					SetPeriod( double period_ ) { period = period_; }
	double					GetPeriod() const { return period; }

	// run back through the children after each forward run
	void					SetBounce( bool bounce );
	bool					GetBounce() const { return ( flags >> BOUNCE_POS ) & 1; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// flags, above NiSwitchNode's
	enum {
		BOUNCE_MASK				= 0x0040,
		BOUNCE_POS				= 6
	};

	void					CopyMembers( NiFltAnimationNode *dest );

	// makes the child for the time active (name invented)
	void					SetIndexFromTime( double time );

	double					period;
};

NiRegisterStream( NiFltAnimationNode );

/*
================
NiFltAnimationNode::SetBounce
================
*/
inline void NiFltAnimationNode::SetBounce( bool bounce ) {
	if ( bounce ) {
		flags |= BOUNCE_MASK;
	} else {
		flags &= ~BOUNCE_MASK;
	}
}

#endif /* !__NIFLTANIMATIONNODE_H__ */
