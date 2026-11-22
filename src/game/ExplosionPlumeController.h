/*
===============================================================================

	ExplosionPlumeController.h

	An AnimTSParticleController for explosion plumes: the plume's emitter
	moves at a velocity of its own, set at random within a cone by
	SetUpRandomVelocity, and a pick can keep the plume off the scene.

	0x124 bytes. Evidence: the constructor (0x4CF210) calls
	AnimTSParticleController's, copies NiPoint3::ZERO to 0x110, clears the
	pointer at 0x120 and stores the vtable 0x88CAB8 (AnimTSParticleController's
	40 slots with the destructor, Update and slot 39 replaced; no NiRTTI of
	its own). The SWIG wrapper ExplosionPlumeController_SetEmitterVelocity
	writes the NiPoint3 at 0x110; the destructor (0x4CF270) deletes the
	NiPick at 0x120 (NiPick's destructor, 0x811D90).

	The member names other than the emitter velocity are invented, after
	their use.

===============================================================================
*/

#ifndef __EXPLOSIONPLUMECONTROLLER_H__
#define __EXPLOSIONPLUMECONTROLLER_H__

#include "AnimTSParticleController.h"

class NiPick;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004cf250 ??_GExplosionPlumeController@@UAEPAXI@Z
class ExplosionPlumeController : public AnimTSParticleController {
public:
	virtual					~ExplosionPlumeController();

	virtual void			Update( float time );

	virtual void			VirtualSlot39();

							ExplosionPlumeController();
	// The script-side constructor (the SWIG wrapper ExplosionPlumeController_Create).
	// FUNCTION: STBC 0x006200f0 ?Create@ExplosionPlumeController@@SAPAV1@XZ @game/AppWrap
	static ExplosionPlumeController *	Create() { return new ExplosionPlumeController; }

	void					SetEmitterVelocity( const NiPoint3 &velocity ) { emitterVelocity = velocity; }
	// a velocity off the emit direction by up to half the angle variance, of
	// a speed between the speed and the speed plus its variance
	void					SetUpRandomVelocity( float angleVariance, float speed, float speedVariance );
	// picks against the subtree from now on, or stops picking (name
	// invented; nothing in retail calls it)
	void					SetPickRoot( NiNode *root );

protected:
	NiPoint3				emitterVelocity;		// 0x110 in the emit object's space
	float					elapsed;				// 0x11C seconds since the plume started
	NiPick *				pick;					// 0x120 owned
};

#endif /* !__EXPLOSIONPLUMECONTROLLER_H__ */
