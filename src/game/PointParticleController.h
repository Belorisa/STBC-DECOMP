/*
===============================================================================

	PointParticleController.h

	An emitter whose particles are drawn as textured points: CreateTarget
	builds the particle geometry with the named texture and alpha, z-buffer
	and vertex-colour properties.

	0x100 bytes, EmitterController's size. Evidence: the NiRTTI at 0x985238
	is built with the name "PointParticleController" (0x8E0D74) and
	EmitterController's NiRTTI as its base (0x4D0E00); the constructor
	(0x4D0F20) calls EmitterController's with its six arguments and stores
	the vtable 0x88CB64 (EmitterController's 40 slots, none added); Create
	(0x4D0EA0) allocates 0x100 bytes and passes an emit velocity of 1, no
	angle variance and no emit object. The destructor is the implicit one
	(the copy kept at 0x4CDEE0 jumps to EmitterController's). The target is
	a TriShapeParticles (TargetIsRequiredType tests the NiRTTI at 0x986B30,
	named "TriShapeParticles"), which CreateTarget builds.

===============================================================================
*/

#ifndef __POINTPARTICLECONTROLLER_H__
#define __POINTPARTICLECONTROLLER_H__

#include "EmitterController.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004d0f70 ??_GPointParticleController@@UAEPAXI@Z
// FUNCTION: STBC 0x004cdee0 ??1PointParticleController@@UAE@XZ @game/ExplodeParticleController
class PointParticleController : public EmitterController {
public:
	// FUNCTION: STBC 0x004d0f60 ?GetRTTI@PointParticleController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

	virtual void			CreateTarget( char *textureName );

	virtual bool			VirtualSlot36( float time );
	virtual void			VirtualSlot37( int index );
	virtual void			VirtualSlot38();

							PointParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	static PointParticleController *	Create( float lifeTime, float emitLife, float emitFrequency );

	// the first PointParticleController of the target's list (name invented)
	static PointParticleController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;
};

#endif /* !__POINTPARTICLECONTROLLER_H__ */
