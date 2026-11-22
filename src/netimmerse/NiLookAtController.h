/*
===============================================================================

	NiLookAtController.h

	Turns its target, an NiAVObject, to face another object.

	Evidence: the vtable at 0x8985B0 (NiTimeController's 20 slots, none
	added); the constructor inlined in CreateClone (0x7A7CA0) allocates
	0x34 bytes, clears bits 4-6 of the flags and the object to look at,
	at 0x30; CopyMembers (0x7A7D20) copies bit 4 and bits 5-6 separately;
	ProcessClone (0x7A7D70) points the clone at the looked-at object's
	clone. The NiRTTI is 0x9A04B8, named "NiLookAtController".

===============================================================================
*/

#ifndef __NILOOKATCONTROLLER_H__
#define __NILOOKATCONTROLLER_H__

#include "NiTimeController.h"
#include "NiAVObject.h"
#include "NiStreamMacros.h"

/*
================
NiLookAtControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the looked-at object's, under a vtable of its own (0x898600). The class
name is invented.
================
*/
// FUNCTION: STBC 0x007a8220 ??_GNiLookAtControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiLookAtController
class NiLookAtControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiLookAtControllerLoadRecord() { lookAtLinkID = 0; }

	unsigned int			lookAtLinkID;
};

// FUNCTION: STBC 0x007a8200 ??_GNiLookAtController@@UAEPAXI@Z @netimmerse/NiLookAtController
class NiLookAtController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7A81F0 ?GetRTTI@NiLookAtController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual void			Update( float time );
	// FUNCTION: STBC 0x7A8280 ?TargetIsRequiredType@NiLookAtController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiAVObject, target );
							}

							NiLookAtController() {
								SetFlip( false );
								SetAxis( X );
								lookAt = 0;
							}

	// the axis that is turned to the object (Gamebryo lineage names)
	enum Axis {
		X,
		Y,
		Z
	};

	// the fields in NiTimeController's flags
	enum {
		FLIP_MASK			= 0x0010,
		FLIP_POS			= 4,
		AXIS_MASK			= 0x0060,
		AXIS_BITS			= 0x0003,
		AXIS_POS			= 5
	};

	// Gamebryo lineage names; the bit and field helpers as the debug copies
	// call them (AnimOnly.exe 0x4959A0, 0x4959C0, 0x495960, 0x495980)
	void					SetFlip( bool flip ) { SetBit( flip, FLIP_POS ); }
	bool					GetFlip() const { return GetBit( FLIP_POS ); }
	void					SetAxis( Axis axis ) { SetField( (unsigned short)axis, AXIS_BITS, AXIS_POS ); }
	Axis					GetAxis() const { return (Axis)GetField( AXIS_BITS, AXIS_POS ); }
	// the object the target faces (Gamebryo lineage names; AnimOnly.exe
	// 0x495920, 0x495940)
	void					SetLookAt( NiAVObject *lookAt_ ) { lookAt = lookAt_; }
	NiAVObject *			GetLookAt() const { return lookAt; }

	// the first NiLookAtController of the target's list (name invented)
	static NiLookAtController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiLookAtController *dest );

	NiAVObject *			lookAt;		// the object the target faces; not referenced
};

NiRegisterStream( NiLookAtController );

#endif /* !__NILOOKATCONTROLLER_H__ */
