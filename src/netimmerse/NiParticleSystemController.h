/*
===============================================================================

	NiParticleSystemController.h

	Emits, moves and retires the particles of its target: an NiParticles,
	whose model data holds the positions, or an NiNode, which gets one
	child node per particle carrying a clone of the particle object.

	Evidence: the static initializer at 0x7AE970 builds the NiRTTI at
	0x9A0770 ("NiParticleSystemController", based on NiTimeController's);
	the vtable at 0x898790 has NiTimeController's 20 slots and adds one
	(slot 20, 0x7AF450, which finds whether the target is an NiParticles
	or an NiNode); CreateObject (0x7B2320) allocates 0x150 bytes and a
	0x78-byte load record; the constructor (0x7AEB70) takes the color
	data, the destructor (0x7AED80) frees the per-particle arrays, the
	saved particles, the particle objects and the force and collider
	chains. The member names are invented unless noted: the class has no
	viewer strings beyond its banner, and NetImmerse 3.1's file layout
	differs from the later public descriptions.

===============================================================================
*/

#ifndef __NIPARTICLESYSTEMCONTROLLER_H__
#define __NIPARTICLESYSTEMCONTROLLER_H__

#include "NiColorData.h"
#include "NiForce.h"
#include "NiObjectNET.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiTArray.h"
#include "NiTimeController.h"
#include "NiStreamMacros.h"

class NiAVObject;
class NiNode;
class NiParticles;

/*
================
NiParticleSystemControllerLoadRecord

The link IDs LoadBinary (0x7B2460) reads and LinkObject (0x7B2910)
resolves: the particle object's and the color data's, the particle
objects', the saved particles' objects', the forces' and the colliders'.
The class name is invented; its vtable is 0x8987E8.
================
*/
// FUNCTION: STBC 0x7B3B10 ??_GNiParticleSystemControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7B3B30 ??1NiParticleSystemControllerLoadRecord@@UAE@XZ
class NiParticleSystemControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiParticleSystemControllerLoadRecord() : forceLinkIDs( 1, 1 ), colliderLinkIDs( 1, 1 ) {
								particleObjectLinkID = 0;
								colorDataLinkID = 0;
							}

	unsigned int			particleObjectLinkID;
	unsigned int			colorDataLinkID;
	// TODO identity: the element types. The arrays' vtables differ from
	// those of NiTArray<unsigned int> (0x88B914) and NiTArray<unsigned
	// long> (0x8990D8); the first (0x8987F0) is NiDynamicEffectLoadRecord's
	// NiTArray<int>, the second (0x88BA2C) also serves a game class (built
	// at 0x463FC0), and the force and collider arrays share the third
	// (0x8987EC), taken as the pointer type the IDs stand for, as
	// NiNodeLoadRecord's are
	NiTArray<int>			particleObjectLinkIDs;
	NiTArray<unsigned char *>	savedParticleLinkIDs;
	NiTArray<NiForce *>		forceLinkIDs;
	NiTArray<NiForce *>		colliderLinkIDs;
};

// FUNCTION: STBC 0x7B3AF0 ??_GNiParticleSystemController@@UAEPAXI@Z
class NiParticleSystemController : public NiTimeController {
public:
	// flags, past NiTimeController's
	enum {
		UNKNOWN_10_POS		= 4,		// cleared by the constructor
		UNKNOWN_20_POS		= 5,		// set by the constructor
		UNKNOWN_C0_MASK		= 0x0003,	// a 2-bit field at 0x00C0, 1 after construction
		UNKNOWN_C0_POS		= 6,
		UNKNOWN_700_MASK	= 0x0007,	// a 3-bit field at 0x0700, 0 after construction
		UNKNOWN_700_POS		= 8,
		UNKNOWN_7800_MASK	= 0x000F,	// a 4-bit field at 0x7800
		UNKNOWN_7800_POS	= 11
	};

	/*
	================
	NiParticleSystemController::Rotation

	A particle's rotation: an angle about an axis (name invented).
	================
	*/
	struct Rotation {
		float				angle;
		NiPoint3			axis;
	};

	/*
	================
	NiParticleSystemController::Particle

	A particle saved with the controller and emitted again when the system
	restarts (name invented).
	================
	*/
	// FUNCTION: STBC 0x7AF9B0 ??0Particle@NiParticleSystemController@@QAE@XZ
	// FUNCTION: STBC 0x7AF0B0 ??1Particle@NiParticleSystemController@@QAE@XZ
	// FUNCTION: STBC 0x7AEFE0 ??_EParticle@NiParticleSystemController@@QAEPAXI@Z
	struct Particle {
		float				lifeSpan;
		float				time;			// the time it was born at
		int					generation;		// the spawn generation, 0 for an emitted particle
		NiPoint3			position;
		NiPoint3			direction;		// scaled by the emission speed
		NiPointer<NiAVObject>	object;		// cloned into the particle's node, or NULL
		Rotation			rotation;
	};

	// FUNCTION: STBC 0x7B3AE0 ?GetRTTI@NiParticleSystemController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiParticleSystemController();

	virtual void			VirtualSlot11();

	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	// FUNCTION: STBC 0x7B3BA0 ?TargetIsRequiredType@NiParticleSystemController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiObjectNET, target );
							}

	// finds whether the target is an NiParticles or an NiNode, and sizes
	// the arrays for an NiParticles' vertices
	virtual void			VirtualSlot20();

							NiParticleSystemController( NiColorData *colorData_ = 0 );

	// the particles' colour over their lives (Gamebryo lineage name)
	void					SetColorData( NiColorData *colorData_ ) { colorData = colorData_; }

	// the first NiParticleSystemController of the target's list (name
	// invented)
	static NiParticleSystemController *	FindController( NiObjectNET *target );

	// the normal the particles are emitted about, and two unit vectors
	// perpendicular to it and to each other
	NiPoint3				GetInitialNormal() const { return initialNormal; }	// Gamebryo lineage name
	void					SetInitialNormal( const NiPoint3 &normal );

	// each member, as the debug copy's CopyMembers reads and writes them
	// (AnimOnly.exe 0x4A1A53; names invented after the members, speed and
	// the grow and fade times after the Gamebryo lineage)
	bool					GetUnknown10() const { return GetBit( UNKNOWN_10_POS ); }
	void					SetUnknown10( bool value ) { SetBit( value, UNKNOWN_10_POS ); }
	float					GetUnknown30() const { return unknown30; }
	void					SetUnknown30( float value ) { unknown30 = value; }
	float					GetSpeed() const { return speed; }
	void					SetSpeed( float value ) { speed = value; }
	float					GetSpeedRandom() const { return speedRandom; }
	void					SetSpeedRandom( float value ) { speedRandom = value; }
	float					GetUnknown3C() const { return unknown3C; }
	void					SetUnknown3C( float value ) { unknown3C = value; }
	float					GetUnknown50() const { return unknown50; }
	void					SetUnknown50( float value ) { unknown50 = value; }
	float					GetUnknown40() const { return unknown40; }
	void					SetUnknown40( float value ) { unknown40 = value; }
	float					GetUnknown48() const { return unknown48; }
	void					SetUnknown48( float value ) { unknown48 = value; }
	float					GetUnknown44() const { return unknown44; }
	void					SetUnknown44( float value ) { unknown44 = value; }
	float					GetUnknown4C() const { return unknown4C; }
	void					SetUnknown4C( float value ) { unknown4C = value; }
	float					GetUnknown94() const { return unknown94; }
	void					SetUnknown94( float value ) { unknown94 = value; }
	float					GetUnknown120() const { return unknown120; }
	void					SetUnknown120( float value ) { unknown120 = value; }
	float					GetUnknown124() const { return unknown124; }
	void					SetUnknown124( float value ) { unknown124 = value; }
	float					GetUnknown128() const { return unknown128; }
	void					SetUnknown128( float value ) { unknown128 = value; }
	float					GetUnknown12C() const { return unknown12C; }
	void					SetUnknown12C( float value ) { unknown12C = value; }
	float					GetUnknown108() const { return unknown108; }
	void					SetUnknown108( float value ) { unknown108 = value; }
	float					GetUnknownFC() const { return unknownFC; }
	void					SetUnknownFC( float value ) { unknownFC = value; }
	float					GetUnknown100() const { return unknown100; }
	void					SetUnknown100( float value ) { unknown100 = value; }
	float					GetUnknown118() const { return unknown118; }
	void					SetUnknown118( float value ) { unknown118 = value; }
	float					GetUnknown11C() const { return unknown11C; }
	void					SetUnknown11C( float value ) { unknown11C = value; }
	int						GetUnknown54() const { return unknown54; }
	void					SetUnknown54( int value ) { unknown54 = value; }
	int						GetUnknown58() const { return unknown58; }
	void					SetUnknown58( int value ) { unknown58 = value; }
	int						GetNumGenerations() const { return numGenerations; }
	void					SetNumGenerations( int value ) { numGenerations = value; }
	int						GetUnknown104() const { return unknown104; }
	void					SetUnknown104( int value ) { unknown104 = value; }
	unsigned int			GetNumParticleObjects() const { return numParticleObjects; }
	NiPoint3				GetUnknown10C() const { return unknown10C; }
	void					SetUnknown10C( const NiPoint3 &value ) { unknown10C = value; }
	int						GetUnknownC0() const { return GetField( UNKNOWN_C0_MASK, UNKNOWN_C0_POS ); }
	void					SetUnknownC0( int value ) { SetField( (unsigned short)value, UNKNOWN_C0_MASK, UNKNOWN_C0_POS ); }
	int						GetUnknown700() const { return GetField( UNKNOWN_700_MASK, UNKNOWN_700_POS ); }
	void					SetUnknown700( int value ) { SetField( (unsigned short)value, UNKNOWN_700_MASK, UNKNOWN_700_POS ); }
	int						GetUnknown7800() const { return GetField( UNKNOWN_7800_MASK, UNKNOWN_7800_POS ); }
	void					SetUnknown7800( int value ) { SetField( (unsigned short)value, UNKNOWN_7800_MASK, UNKNOWN_7800_POS ); }
	float					GetGrowTime() const { return growTime; }
	void					SetGrowTime( float value ) { growTime = value; }
	float					GetFadeTime() const { return fadeTime; }
	void					SetFadeTime( float value ) { fadeTime = value; }
	bool					GetUnknown20() const { return GetBit( UNKNOWN_20_POS ); }
	void					SetUnknown20( bool value ) { SetBit( value, UNKNOWN_20_POS ); }
	unsigned int			GetMaxNumParticles() const { return maxNumParticles; }
	NiAVObject *			GetParticleObject() const { return particleObject; }

	NiAVObject *			GetParticleObjectAt( unsigned int index );
	void					SetParticleObjectAt( NiAVObject *object, unsigned int index );
	void					SetNumParticleObjects( unsigned int count );
	void					SetParticleObject( NiAVObject *object );

	// reallocates the per-particle arrays, dropping every particle and
	// every saved particle
	void					SetMaxNumParticles( unsigned int count );

	void					AddSavedParticle( const Particle &particle );
	void					AddParticle( const Particle &particle );
	void					SpawnParticle( const Particle &parent );
	// replaces a particle with the last one
	void					RemoveParticle( unsigned int index );

	void					AddForce( NiForce *force );
	void					AddCollider( NiForce *collider );

	// the color keys
	void					ReplaceAnim( NiColorKey **keys, unsigned int numKeys, NiColorKey::KeyType keyType );
	NiColorKey **			GetAnim( unsigned int &numKeys, NiColorKey::KeyType &keyType ) const;
	unsigned int			GetNumKeys() const;

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiParticleSystemController *dest );

	// the particle's size factor at its age: growing after birth, fading
	// before its end
	float					GetGrowFade( float age, int generation );
	// the color keys' color at the particle's age, clamped to [0,1]
	NiColorA				GetColor( float age );
	// TODO identity: emits and moves the particles up to time (0x7B0330)
	void					Unknown7B0330( float time );
	// restarts the system: the saved particles again, the forces reset
	void					ResetParticles();
	// the world transform of an object's local frame under its parent
	static void				GetWorldTransform( NiAVObject *object, float &scale, NiPoint3 &translate, NiMatrix3 &rotate );

	float					unknown30;			// 1 after construction
	float					speed;				// the emission speed
	float					speedRandom;		// the spread of the emission speed about speed
	float					unknown3C;			// the lifetime the color keys and the fade measure against
	float					unknown40;			// 1 after construction
	float					unknown44;			// 1 after construction
	float					unknown48;			// 1 after construction
	float					unknown4C;
	float					unknown50;
	int						unknown54;
	int						unknown58;
	unsigned int			numParticleObjects;
	unsigned int			maxNumParticles;
	unsigned int			numParticles;
	NiPointer<NiAVObject> *	particleObjects;	// numParticleObjects
	NiPointer<NiAVObject>	particleObject;		// cloned into each particle's node
	NiPoint3				initialNormal;
	NiPoint3				initialNormalU;		// perpendicular to initialNormal
	NiPoint3				initialNormalV;		// perpendicular to both
	float					unknown94;
	float					lastTime;			// -1 after construction
	float					unknown9C;			// the fraction of a particle left over from the last emission
	float *					lifeSpans;			// maxNumParticles each
	float *					ages;
	int *					generations;
	NiForce *				forces;				// owned chains
	NiForce *				colliders;
	NiPointer<NiNode> *		particleNodes;		// the nodes under an NiNode target
	NiPoint3 *				positions;			// used without an NiParticles target
	NiPoint3 *				velocities;
	Rotation *				rotations;
	int						numGenerations;		// the spawn generations
	NiMatrix3				worldRotate;		// the emitter's world transform
	NiPoint3				worldTranslate;
	float					worldScale;
	float					unknownFC;
	float					unknown100;
	int						unknown104;
	float					unknown108;
	NiPoint3				unknown10C;			// NiPoint3::UNIT_X after construction
	float					unknown118;
	float					unknown11C;			// -1 after construction
	float					unknown120;
	float					unknown124;
	float					unknown128;
	float					unknown12C;
	unsigned int			numSavedParticles;
	Particle *				savedParticles;		// maxNumParticles
	NiPointer<NiColorData>	colorData;
	unsigned int			lastColorIndex;		// the key the last color interpolation found
	float					growTime;
	float					fadeTime;
	NiParticles *			particles;			// the target, when an NiParticles
	NiNode *				node;				// the target, when an NiNode

	// the cutoff for a period near zero (Gamebryo 1.1.2: ms_fEpsilon)
	static const float		epsilon;
};

NiRegisterStream( NiParticleSystemController );

#endif /* !__NIPARTICLESYSTEMCONTROLLER_H__ */
