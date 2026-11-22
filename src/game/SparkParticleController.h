/*
===============================================================================

	SparkParticleController.h

	A point particle emitter whose particles are drawn as sparks: lines
	trailing behind each particle, of the tail length, fading by the tail
	fade.

	0x10C bytes. Evidence: the NiRTTI at 0x985450 is built with the name
	"SparkParticleController" (0x8E0D8C) and PointParticleController's
	NiRTTI as its base (0x4D2390); Create (0x4D2430) allocates 0x10C bytes
	and passes its six arguments to the constructor (0x4D24B0), which calls
	PointParticleController's and stores the vtable 0x88CC04. The SWIG
	wrapper SparkParticleController_Create ("fff|ffO") gives the defaults of
	the last three; SparkParticleController_SetTailLength and _SetTailFade
	write 0x104 and 0x108.

===============================================================================
*/

#ifndef __SPARKPARTICLECONTROLLER_H__
#define __SPARKPARTICLECONTROLLER_H__

#include "PointParticleController.h"
#include "NiLines.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004d2520 ??_GSparkParticleController@@UAEPAXI@Z
// FUNCTION: STBC 0x004d2540 ??1SparkParticleController@@UAE@XZ
class SparkParticleController : public PointParticleController {
public:
	// FUNCTION: STBC 0x004d2510 ?GetRTTI@SparkParticleController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual void			CreateTarget( char *textureName );
	virtual void			AttachEffect( NiNode *parent );
	virtual void			DetachEffect();

	// also stretches the particle's tail back along its velocity
	virtual void			VirtualSlot35( int index, float time, float deltaTime, float lifeFraction );
	virtual void			VirtualSlot37( int index );

							SparkParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	void					SetTailLength( float tailLength_ ) { tailLength = tailLength_; }
	void					SetTailFade( float tailFade_ ) { tailFade = tailFade_; }

	static SparkParticleController *	Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity = 35.0f, float angleVariance = 90.0f, NiAVObject *emitFrom = NULL );

	// the first SparkParticleController of the target's list (name invented)
	static SparkParticleController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( SparkParticleController *dest );

	// two vertices per particle: the head at the particle and the tail
	// behind it (name invented)
	NiPointer<NiLines>		tails;					// 0x100
	float					tailLength;				// 0x104 the tail's length, back along the velocity
	float					tailFade;				// 0x108 the tail's alpha, as a fraction of the head's
};

#endif /* !__SPARKPARTICLECONTROLLER_H__ */
