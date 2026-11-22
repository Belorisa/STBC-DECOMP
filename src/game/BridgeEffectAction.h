/*
===============================================================================

	BridgeEffectAction.h

	An effect action that plays a particle effect on the bridge: the
	constructor builds the controller of the effect type and attaches it at
	the named emitter hardpoint of a bridge object, keeping the hardpoint's
	name.

	0x34 bytes, allocated from the pools. Evidence: the Create functions
	(0x4DC4D0-0x4DC610) allocate 0x34 bytes and call the constructor
	(0x4DC660) with the effect type 0 to 4, which calls EffectAction's,
	stores the vtable 0x88D0BC (EffectAction's 24 slots, overriding the
	destructor) and builds a point, spark, explosion or debris particle
	controller; the destructor (0x4DC9B0) deletes the name at 0x30, which
	the SWIG inline accessor BridgeEffectAction_GetHardpointName reads. The
	property types passed with the controller are the GetObjType values of
	EffectEmitterProperty, SmokeEmitterProperty, SparkEmitterProperty and
	ExplodeEmitterProperty.

===============================================================================
*/

#ifndef __BRIDGEEFFECTACTION_H__
#define __BRIDGEEFFECTACTION_H__

#include "EffectAction.h"

class BridgeObjectClass;
class EmitterController;
class ExplodeParticleController;
class SparkParticleController;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class BridgeEffectAction : public EffectAction {
public:
	// the effect types (invented names, after the Create functions)
	enum EffectType {
		EFFECT_POINT,
		EFFECT_SMOKE,
		EFFECT_SPARKS,
		EFFECT_EXPLOSION,
		EFFECT_DEBRIS
	};

					BridgeEffectAction( int type, float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName_, char *textureName );
	virtual			~BridgeEffectAction( void );

	static BridgeEffectAction *	Create( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName );
	static BridgeEffectAction *	CreateSparks( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName );
	static BridgeEffectAction *	CreateSmoke( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName );
	static BridgeEffectAction *	CreateExplosion( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName );
	static BridgeEffectAction *	CreateDebris( float lifeTime, float emitLife, float emitFrequency, BridgeObjectClass *bridgeObject, char *hardpointName, char *textureName );
	EmitterController *	GetController( void );
	SparkParticleController *	GetSparkController( void );
	ExplodeParticleController *	GetExplosionController( void );

	char *			GetHardpointName( void ) { return hardpointName; }

protected:
	char *			hardpointName;			// 0x30 owned
};

#endif /* !__BRIDGEEFFECTACTION_H__ */
