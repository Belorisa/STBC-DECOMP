/*
===============================================================================

	RotateBonesController.h

	The controller of a bridge character's model that turns its bones; it
	starts and stops a set of child controllers with itself. Only the class
	and what CharacterClass calls are recovered; its unit (0x673590 to
	0x674B40) has no source yet.

	0x4C bytes. Evidence: the NiRTTI at 0x992040 is built with the name
	"RotateBonesController" (0x958148) and NiTimeController's NiRTTI as its
	base (0x673540); CharacterClass's builder (0x667D60) allocates 0x4C bytes
	and calls the constructor (0x6735D0), which runs NiTimeController's,
	builds the array at 0x30 (NiTArray vtable 0x894AB8, one element), stores
	the vtable 0x894A5C (23 slots) and clears the flag at 0x48 through slot
	21 (0x674A20). The vtable overrides GetRTTI (0x674B10), CreateClone
	(0x6737A0), the destructor (0x674B20), Start (0x674990, which sets the
	flag and calls slot 20 of each child), Stop (0x6749F0, which clears it
	and stops each child), Update (0x673680), SetTarget (0x673820) and
	TargetIsRequiredType (0x673590: an NiAVObject); slot 20 (0x6749D0)
	starts it at the clock's current time and slot 22 (0x6735C0) returns
	NiAVObject's NiRTTI.

===============================================================================
*/

#ifndef __ROTATEBONESCONTROLLER_H__
#define __ROTATEBONESCONTROLLER_H__

#include "NiTimeController.h"
#include "NiTArray.h"
#include "BoneStateController.h"

// FUNCTION: STBC 0x674B20 ??_GRotateBonesController@@UAEPAXI@Z
class RotateBonesController : public NiTimeController {
public:
	// FUNCTION: STBC 0x674B10 ?GetRTTI@RotateBonesController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~RotateBonesController();

	virtual void			Start( float time );
	virtual void			Stop();
	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	virtual bool			TargetIsRequiredType() const;

	// Starts the controller at the clock's current time.
	virtual void			VirtualSlot20();
	// Clears the started flag (the constructor calls it).
	virtual void			VirtualSlot21();
	// NiAVObject's NiRTTI: the type the target must have.
	virtual const NiRTTI *	VirtualSlot22();

							RotateBonesController();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( RotateBonesController *dest );
	void					CopyFrom( RotateBonesController *source );

	// the child controllers Start and Stop pass on (slot 20 and Stop are
	// called on each): the BoneStateControllers SetTarget creates (0x65F960)
	NiTArray<BoneStateController *>	boneControllers;	// 0x30 (name invented)
	bool					started;				// 0x48 set by Start, cleared by Stop
};

#endif /* !__ROTATEBONESCONTROLLER_H__ */
