/*
===============================================================================

	NiCollisionSwitch.h

	A node that switches collision testing of its subtree on and off.

	Evidence: the NiRTTI (0x9A1E98, built at 0x7F8CC0) is named
	"NiCollisionSwitch" with NiNode's as its base; the vtable at 0x899920
	has NiNode's 44 slots and adds none, overriding the collision slots
	32-34, which test the subtree only with flag bit 5 set. The creation
	function (0x7F8F90) allocates 0xE8 bytes, NiNode's size; the
	constructor, inlined there, sets flag bit 5. The viewer strings
	(0x7F9180) name the flag "m_bPropagate".

===============================================================================
*/

#ifndef __NICOLLISIONSWITCH_H__
#define __NICOLLISIONSWITCH_H__

#include "NiNode.h"
#include "NiStreamMacros.h"

/*
================
NiCollisionSwitchLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x7F8F90) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x8999D0 last.
================
*/
// FUNCTION: STBC 0x7F90B0 ??_GNiCollisionSwitchLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F90D0 ??1NiCollisionSwitchLoadRecord@@UAE@XZ
class NiCollisionSwitchLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x7F9430 ??_GNiCollisionSwitch@@UAEPAXI@Z
// FUNCTION: STBC 0x7F9450 ??1NiCollisionSwitch@@UAE@XZ
class NiCollisionSwitch : public NiNode {
public:
	// FUNCTION: STBC 0x7F9420 ?GetRTTI@NiCollisionSwitch@@UBEPBVNiRTTI@@XZ
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

	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );

							NiCollisionSwitch() { SetPropagate( true ); }

	// collision tests reach the subtree
	void					SetPropagate( bool propagate );
	bool					GetPropagate() const { return ( flags >> PROPAGATE_SWITCH_POS ) & 1; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// flags, above NiAVObject's
	enum {
		PROPAGATE_SWITCH_MASK	= 0x0020,
		PROPAGATE_SWITCH_POS	= 5
	};

	void					CopyMembers( NiCollisionSwitch *dest );
};

NiRegisterStream( NiCollisionSwitch );

/*
================
NiCollisionSwitch::SetPropagate
================
*/
inline void NiCollisionSwitch::SetPropagate( bool propagate ) {
	if ( propagate ) {
		flags |= PROPAGATE_SWITCH_MASK;
	} else {
		flags &= ~PROPAGATE_SWITCH_MASK;
	}
}

#endif /* !__NICOLLISIONSWITCH_H__ */
