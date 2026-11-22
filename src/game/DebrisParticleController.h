/*
===============================================================================

	DebrisParticleController.h

	A point particle emitter for debris.

	0x100 bytes. Evidence: the NiRTTI at 0x984938 is built with the name
	"DebrisParticleController" (0x8E0CF8) and PointParticleController's
	NiRTTI as its base (0x4C88B0); Create (0x4C8900) allocates 0x100 bytes
	and calls the constructor (0x4C8980), which calls
	PointParticleController's and stores the vtable 0x88C804. The SWIG
	wrapper DebrisParticleController_Create ("fff|ffO") gives the defaults
	of the last three. The destructor is the implicit one (0x4C89F0 jumps
	to EmitterController's).

===============================================================================
*/

#ifndef __DEBRISPARTICLECONTROLLER_H__
#define __DEBRISPARTICLECONTROLLER_H__

#include "PointParticleController.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004c89d0 ??_GDebrisParticleController@@UAEPAXI@Z
// FUNCTION: STBC 0x004c89f0 ??1DebrisParticleController@@UAE@XZ
class DebrisParticleController : public PointParticleController {
public:
	// FUNCTION: STBC 0x004c89c0 ?GetRTTI@DebrisParticleController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual void			VirtualSlot35( int index, float time, float deltaTime, float lifeFraction );
	virtual bool			VirtualSlot36( float time );

							DebrisParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	static DebrisParticleController *	Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity = 55.0f, float angleVariance = 20.0f, NiAVObject *emitFrom = NULL );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( DebrisParticleController *dest );
};

#endif /* !__DEBRISPARTICLECONTROLLER_H__ */
