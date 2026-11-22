/*
===============================================================================

	EmitterController.h

	An effect controller that emits particles: each frame it releases new
	particles at the emit frequency, from a point and direction that can
	follow another scene graph object, and moves the live ones by their
	velocity, the gravity and the damping, colouring and sizing them by the
	effect's key tracks over their life.

	0x100 bytes. Evidence: the NiRTTI at 0x984CE8 is built with the name
	"EmitterController" (0x8E0D44) and EffectController's NiRTTI as its base
	(0x4CBB10); the constructor (0x4CBBB0) calls EffectController's with
	the lifetime, stores the vtable 0x88C964 (EffectController's 34 slots
	and six of its own) and takes the emit life, frequency, velocity, angle
	variance and the object to emit from; the destructor 0x4CBD60. The SWIG
	wrappers EmitterController_* name the members they write: the emit
	object 0x38, the radius 0xA8, the life 0xC4 and its variance 0xC8, the
	velocity 0xCC and its variance 0xD0, the angle variance 0xD4, the
	frequency 0xD8 and its variance 0xDC, the gravity 0xF0 and the damping
	0xFC; SetDetachEmitObject and SetInheritsVelocity set flag bits.

===============================================================================
*/

#ifndef __EMITTERCONTROLLER_H__
#define __EMITTERCONTROLLER_H__

#include "EffectController.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiAVObject.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004cbd40 ??_GEmitterController@@UAEPAXI@Z
class EmitterController : public EffectController {
public:
	// FUNCTION: STBC 0x004cbd30 ?GetRTTI@EmitterController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~EmitterController();

	virtual void			Stop();
	virtual void			Update( float time );

	virtual void			DetachEffect();
	virtual void			AddColorKey( float time, float r, float g, float b );
	virtual void			AddAlphaKey( float time, float alpha );
	virtual void			AddSizeKey( float time, float size );

	// the emitter's own slots (placeholder names)
	virtual void			VirtualSlot34();
	// moves the particle at the index on by the time step and colours and
	// sizes it for the fraction of its life it has lived
	virtual void			VirtualSlot35( int index, float time, float deltaTime, float lifeFraction );
	virtual bool			VirtualSlot36( float time );
	virtual void			VirtualSlot37( int index );
	virtual void			VirtualSlot38();
	virtual void			VirtualSlot39();

							EmitterController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ );

	void					SetEmitPositionAndDirection( const NiPoint3 &position, const NiPoint3 &direction );

	// the first EmitterController of the target's list (name invented)
	static EmitterController *	FindController( NiObjectNET *target );

	// flags beyond NiTimeController's and EffectController's: the emit
	// object leaves the scene graph with the effect, the particles inherit
	// the emit object's velocity
	enum {
		DETACH_EMIT_OBJECT_POS	= 6,
		INHERITS_VELOCITY_POS	= 7
	};

	void					SetDetachEmitObject( bool detach ) { SetBit( detach, DETACH_EMIT_OBJECT_POS ); }
	void					SetInheritsVelocity( bool inherits ) { SetBit( inherits, INHERITS_VELOCITY_POS ); }

	NiAVObject *			GetEmitFromObject() { return emitFrom; }
	void					SetEmitFromObject( NiAVObject *object ) { emitFrom = object; }
	void					SetGravity( float x, float y, float z ) { gravity = NiPoint3( x, y, z ); }
	void					SetEmitRadius( float emitRadius_ ) { emitRadius = emitRadius_; }
	void					SetEmitLife( float emitLife_ ) { emitLife = emitLife_; }
	void					SetEmitLifeVariance( float emitLifeVariance_ ) { emitLifeVariance = emitLifeVariance_; }
	void					SetEmitVelocity( float emitVelocity_ ) { emitVelocity = emitVelocity_; }
	void					SetEmitVelocityVariance( float emitVelocityVariance_ ) { emitVelocityVariance = emitVelocityVariance_; }
	void					SetAngleVariance( float angleVariance_ ) { angleVariance = angleVariance_; }
	void					SetEmitFrequency( float emitFrequency_ ) { emitFrequency = emitFrequency_; }
	void					SetEmitFrequencyVariance( float emitFrequencyVariance_ ) { emitFrequencyVariance = emitFrequencyVariance_; }
	void					SetDamping( float damping_ ) { damping = damping_; }

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( EmitterController *dest );

public:
	NiPointer<NiAVObject>	emitFrom;				// 0x38 the object the particles leave from, if any
	NiPoint3				emitPosition;			// 0x3C in the emit object's space, or in world space without one
	NiMatrix3				emitDirection;			// 0x48 rows: the direction and two axes across it
	NiPoint3				modelEmitPosition;		// 0x6C emitPosition in the target's model space
	NiMatrix3				modelEmitDirection;		// 0x78 emitDirection in the target's model space
	NiPoint3				inheritedVelocity;		// 0x9C the emit object's velocity in the target's model space
	float					emitRadius;				// 0xA8
	NiPoint3 *				velocities;				// 0xAC one per particle
	float *					birthTimes;				// 0xB0 one per particle
	float *					lifeSpans;				// 0xB4 one per particle
	float *					sizes;					// 0xB8 one per particle, filled by GetSize; the target's array
	NiPoint3 *				positions;				// 0xBC one per particle; the target's array
	NiColorA *				colors;					// 0xC0 one per particle, filled by GetColor and GetAlpha; the target's array
	float					emitLife;				// 0xC4 seconds
	float					emitLifeVariance;		// 0xC8
	float					emitVelocity;			// 0xCC
	float					emitVelocityVariance;	// 0xD0
	float					angleVariance;			// 0xD4 degrees
	float					emitFrequency;			// 0xD8 seconds between particles
	float					emitFrequencyVariance;	// 0xDC
	float					emitTimer;				// 0xE0 the time owed to the next emission
	int						numToEmit;				// 0xE4 the particles still to emit over the effect's life
	int						numActive;				// 0xE8 the live particles
	int						numParticles;			// 0xEC the particles allocated
	NiPoint3				gravity;				// 0xF0
	float					damping;				// 0xFC
};

#endif /* !__EMITTERCONTROLLER_H__ */
