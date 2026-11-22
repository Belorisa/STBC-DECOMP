/*
===============================================================================

	BoneStateController.h

	The controller a bridge character's RotateBonesController puts on each
	bone it turns: it plays a set of rotation tracks (BoneState) on the
	bone, composing the interpolated rotation with whatever the bone's own
	animation left there.

	BoneStateController, 0x3C bytes. Evidence: the NiRTTI at 0x9916E0 is
	built with the name "BoneStateController" (0x956F90) and
	NiTimeController's NiRTTI as its base (0x65F900); CreateClone (0x65FAD0)
	allocates 0x3C bytes and calls the constructor (0x65F960), which runs
	NiTimeController's, stores the vtable 0x894694 (23 slots) and clears the
	started flag through slot 21 (0x65FBF0). The vtable overrides GetRTTI
	(0x65FCF0), CreateClone, the destructor (0x65FD00), Start (0x65FB90),
	Stop (0x65FBE0), Update (0x65FA20), SetTarget (0x65FA60) and
	TargetIsRequiredType (0x65FC00: an NiAVObject); slot 20 (0x65FBC0)
	starts it at the game time and slot 22 (0x65F950) returns NiAVObject's
	NiRTTI. RotateBonesController::SetTarget (0x673820) creates them. The
	member names are invented.

===============================================================================
*/

#ifndef __BONESTATECONTROLLER_H__
#define __BONESTATECONTROLLER_H__

#include "NiTimeController.h"
#include "BoneState.h"

// FUNCTION: STBC 0x65FD00 ??_GBoneStateController@@UAEPAXI@Z
class BoneStateController : public NiTimeController {
public:
	// FUNCTION: STBC 0x65FCF0 ?GetRTTI@BoneStateController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~BoneStateController();

	virtual void			Start( float time );
	virtual void			Stop();
	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	virtual bool			TargetIsRequiredType() const;

	// Starts the controller at the game time.
	virtual void			VirtualSlot20();
	// Clears the started flag (the constructor calls it).
	virtual void			VirtualSlot21();
	// NiAVObject's NiRTTI: the type the target must have.
	virtual const NiRTTI *	VirtualSlot22();

							BoneStateController();

	static const NiRTTI		rtti;

protected:
	friend class RotateBonesController;

	void					CopyMembers( BoneStateController *dest );
	void					CopyFrom( BoneStateController *source );

	BoneState *				state;			// 0x30 owned; RotateBonesController sets it
	bool					started;		// 0x34 set by Start, cleared by Stop
	float					lastUpdate;		// 0x38 the time of the last update
};

#endif /* !__BONESTATECONTROLLER_H__ */
