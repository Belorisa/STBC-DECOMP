/*
===============================================================================

	ExplodeParticleController.h

	A point particle emitter for explosions, with a track of intensity keys
	and the scene root it lights.

	0x118 bytes. Evidence: the NiRTTI at 0x984DB0 is built with the name
	"ExplodeParticleController" (0x8E0D58) and PointParticleController's
	NiRTTI as its base (0x4CDBA0); Create (0x4CDCB0) allocates 0x118 bytes
	and calls the constructor (0x4CDD30), which calls
	PointParticleController's and stores the vtable 0x88CA08
	(PointParticleController's 40 slots and four of its own, which the SWIG
	wrappers ExplodeParticleController_* call by name). The SWIG wrapper
	ExplodeParticleController_Create ("fff|ffO") gives the defaults of the
	last three.

===============================================================================
*/

#ifndef __EXPLODEPARTICLECONTROLLER_H__
#define __EXPLODEPARTICLECONTROLLER_H__

#include "PointParticleController.h"
#include "NiLight.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004cdda0 ??_GExplodeParticleController@@UAEPAXI@Z
class ExplodeParticleController : public PointParticleController {
public:
	// FUNCTION: STBC 0x004cdd90 ?GetRTTI@ExplodeParticleController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~ExplodeParticleController();

	virtual void			Update( float time );

	virtual void			CreateTarget( char *textureName );
	virtual void			AttachEffect( NiNode *parent );
	virtual void			DetachEffect();

	virtual void			AddDefaultKeys();

	virtual void			AddIntensityKey( float time, float intensity );
	virtual void			RemoveIntensityKeyAt( float time );
	virtual void			RemoveAllIntensityKeys();
	virtual void			SetSceneRoot( char *setName );

							ExplodeParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	static ExplodeParticleController *	Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity = 55.0f, float angleVariance = 150.0f, NiAVObject *emitFrom = NULL );

	// the first ExplodeParticleController of the target's list (name invented)
	static ExplodeParticleController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( ExplodeParticleController *dest );

	// the intensity track's value at the time, over the effect's life (name
	// invented)
	void					GetIntensity( float time, float &intensity );

	// the light the explosions share, dimmed by the intensity track and
	// placed at the emit position (name invented). Nothing in retail ever
	// creates it: the unit only initialises it to NULL and releases it.
	static NiPointer<NiLight>	light;

	char *					sceneRootName;			// 0x100 the name of the set whose scene the explosion lights (invented name)
	KeyArray<NiLinFloatKey>	intensityKeys;			// 0x104
};

#endif /* !__EXPLODEPARTICLECONTROLLER_H__ */
