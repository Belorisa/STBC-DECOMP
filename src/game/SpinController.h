/*
===============================================================================

	SpinController.h

	Turns its target about a random axis at a constant rate; the sun's
	sphere and glow shells each get one (SunEffect::Create). 0x40 bytes:
	NiTimeController's, the axis at 0x30 and the rate at 0x3C. The vtable
	0x88DBF4 keeps NiTimeController's 20 slots and its RTTI, with
	CreateClone (0x4F2D30), the stream functions (0x4F2E80-0x4F2EB0, each
	only calling NiTimeController's), IsEqual (0x4F2D20), the destructor
	(0x4F2CF0), Update (0x4F2C20) and TargetIsRequiredType (0x4F2CC0)
	replaced. Nothing names the class or its members: the names are
	invented.

===============================================================================
*/

#ifndef __SPINCONTROLLER_H__
#define __SPINCONTROLLER_H__

#include "NiTimeController.h"
#include "NiMath.h"
#include "NiPoint3.h"

// FUNCTION: STBC 0x004f2cf0 ??_GSpinController@@UAEPAXI@Z
// FUNCTION: STBC 0x004f2d10 ??1SpinController@@UAE@XZ
class SpinController : public NiTimeController {
public:
	// the stream's creation function: a new controller, read with a plain
	// controller's record
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

							// a random axis, 0.015 radians a second
							SpinController() {
								axis = NiPoint3( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
								axis.Unitize();
								lastTime = -NI_INFINITY;
								rate = 0.015f;
							}

protected:
	void					CopyMembers( SpinController *dest );

	NiPoint3				axis;		// 0x30 a unit vector
	float					rate;		// 0x3C radians a second
};

#endif /* !__SPINCONTROLLER_H__ */
