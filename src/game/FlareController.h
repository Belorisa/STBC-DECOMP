/*
===============================================================================

	FlareController.h

	An effect controller that scatters flares over a sphere: at its emit
	frequency it places a new flare at a random point of the sphere, facing
	out along the radius, gives it a random texture animation track of its
	texture animation and a random size, and over the flare's life plays
	the track, sizes it by the dimension keys and colours it by the
	effect's colour and alpha keys. Its target is a TriShapeOrientedParticles.

	FlareControllerData: 0x58 bytes. Evidence: the constructor of
	FlareController (0x4F1800) builds an EffectControllerData and a fourth
	key array of 0x10-byte keys at 0x44 and stores the vtable 0x88DBC0
	(EffectControllerData's 12 slots with the destructor, 0x4F19B0,
	replaced; no NiRTTI of its own); the destructor (0x4F19D0) frees the
	fourth array, then EffectControllerData's. The keys are NiLinPosKeys:
	AddDimensionKey (0x4F2240) stores a width and a length as the key's x
	and y, and the update interpolates them through the LINKEY entry of
	NiPosKey's interpolation table.

	FlareController: 0x88 bytes. Evidence: the NiRTTI at 0x9867A8 is built
	with the name "FlareController" (0x8E1050) and EffectController's NiRTTI
	as its base (0x4F1760); CreateClone (0x4F1F40) allocates 0x88 bytes and
	calls the constructor (0x4F1800), which calls EffectController's with
	the new data and stores the vtable 0x88DB34 (EffectController's 34 slots
	and one of its own). TargetIsRequiredType tests the NiRTTI at 0x986B08,
	named "TriShapeOrientedParticles".

	The member and function names are invented, after their use.

===============================================================================
*/

#ifndef __FLARECONTROLLER_H__
#define __FLARECONTROLLER_H__

#include "EffectController.h"
#include "NiLinPosKey.h"
#include "NiPoint2.h"

class TextureAnimContainer;
class TextureAnimTrack;

/*
================
FlareControllerData

EffectControllerData with the flares' dimension keys.
================
*/
// FUNCTION: STBC 0x004f19b0 ??_GFlareControllerData@@UAEPAXI@Z
// FUNCTION: STBC 0x004f19d0 ??1FlareControllerData@@UAE@XZ
class FlareControllerData : public EffectControllerData {
public:
	KeyArray<NiLinPosKey>	dimensionKeys;		// 0x44 x the width, y the length, over the flare's life
};

// FUNCTION: STBC 0x004f1990 ??_GFlareController@@UAEPAXI@Z
class FlareController : public EffectController {
public:
	// FUNCTION: STBC 0x004f1950 ?GetRTTI@FlareController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	virtual					~FlareController();

	virtual void			Stop();
	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	virtual bool			TargetIsRequiredType() const;

	virtual void			CreateTarget( char *textureAnimName );

	// keys outside the flare's life, 0 to 1, are ignored
	virtual void			AddColorKey( float time, float r, float g, float b );
	virtual void			AddAlphaKey( float time, float alpha );

	// sizes the particle arrays for the most flares alive at once
	virtual void			VirtualSlot34();

							FlareController( float flareLife_, float emitFrequency_, float radius_, NiAVObject *emitFrom_ );

	void					AddDimensionKey( float time, float width, float length );

	// the emitter's setters, named as EmitterController's (invented)
	void					SetEmitFromObject( NiAVObject *object ) { emitFrom = object; }
	void					SetEmitFrequency( float emitFrequency_ ) { emitFrequency = emitFrequency_; }
	void					SetFlareLife( float flareLife_ ) { flareLife = flareLife_; }
	void					SetFlareLifeVariance( float flareLifeVariance_ ) { flareLifeVariance = flareLifeVariance_; }
	void					SetRadius( float radius_ ) { radius = radius_; }

	// the first FlareController of the target's list
	static FlareController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( FlareController *dest );

	// the data as what it is
	FlareControllerData *	GetFlareData() { return (FlareControllerData *)(EffectControllerData *)data; }

	// the target's arrays, cached
	void					GetTargetArrays( NiObjectNET *particles );

	// plays the flare's track and sizes it for the fraction of its life
	// it has lived
	void					UpdateFlare( int index, float time, float deltaTime, float lifeFraction );
	// the last live flare takes the place of the one at the index
	void					RemoveFlare( int index );
	// no live flares
	void					RemoveAllFlares();
	// a new flare, if its life is longer than the time owed to it
	bool					EmitFlare( float time );

	NiPointer<NiAVObject>	emitFrom;				// 0x38 not read by the flare code
	float					radius;					// 0x3C of the sphere the flares are placed on
	float *					birthTimes;				// 0x40 one per flare
	float *					lifeSpans;				// 0x44 one per flare
	float *					sizes;					// 0x48 one per flare: the dimension keys' scale
	TextureAnimContainer *	textureAnim;			// 0x4C the tracks the flares play, from g_kTextureAnimManager
	TextureAnimTrack **		tracks;					// 0x50 one per flare
	NiPoint2 *				dimensions;				// 0x54 the target's arrays
	NiPoint3 *				positions;				// 0x58
	NiPoint3 *				directions;				// 0x5C
	NiColorA *				colors;					// 0x60
	NiPoint2 *				textures;				// 0x64
	float					flareLife;				// 0x68 seconds
	float					flareLifeVariance;		// 0x6C
	float					emitTimer;				// 0x70 the time owed to the next flare
	float					unknown74;				// 0x74
	float					emitFrequency;			// 0x78 seconds between flares
	float					emitFrequencyVariance;	// 0x7C
	int						numActive;				// 0x80 the live flares
	int						numParticles;			// 0x84 the flares allocated
};

#endif /* !__FLARECONTROLLER_H__ */
