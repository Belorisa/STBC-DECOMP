/*
===============================================================================

	NiForce.h

	The base of the particle forces and colliders: NiGravity, NiParticleBomb,
	NiSphericalCollider and NiPlanarCollider. A particle system controller
	keeps its forces as a chain linked through next, transforms them to world
	space with the system each frame and applies each one to every particle.

	Evidence: the NiRTTI (0x9A0318) is named "NiForce" with NiObject's as its
	base, and the four classes above name it as theirs; the vtable at
	0x898384 has NiObject's 12 slots (CreateClone, the five streaming slots
	and the destructor replaced) and three pure slots (12-14); the derived
	constructors clear the word at 8 under this vtable, CopyMembers
	(0x79E4E0) clones it, and NiParticleSystemController walks the chains at
	its 0xAC and 0xB0 through it, calling slot 13 with its world rotation,
	translation and scale and slot 14 with none. The CreateObject is empty
	(the class is abstract). The slot and member names are invented: the
	Gamebryo successor of this class (NiParticleModifier) has a different
	interface.

===============================================================================
*/

#ifndef __NIFORCE_H__
#define __NIFORCE_H__

#include "NiObject.h"
#include "NiStreamMacros.h"

class NiMatrix3;
class NiPoint3;

// FUNCTION: STBC 0x79F000 ??_GNiForce@@UAEPAXI@Z @netimmerse/NiGravity
// FUNCTION: STBC 0x79F020 ??1NiForce@@UAE@XZ @netimmerse/NiGravity
class NiForce : public NiObject {
public:
	// FUNCTION: STBC 0x79EFF0 ?GetRTTI@NiForce@@UBEPBVNiRTTI@@XZ @netimmerse/NiGravity
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	// applies the force to one particle over the step that ends at time:
	// its position and velocity, and the step's length; a collider that
	// the particle hits moves it to the hit point, shortens the step to
	// what is left of it and returns true, the forces return false (name
	// invented)
	virtual bool			Apply( NiPoint3 &position, NiPoint3 &velocity, float &deltaTime, float time ) = 0;
	// moves the force's world-space frame with its particle system's
	// world transform, or (UpdateWorldIdentity) makes it the model frame
	// (names invented)
	virtual void			UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) = 0;
	virtual void			UpdateWorldIdentity() = 0;

							NiForce() { next = 0; }

	NiForce *				GetNext() const { return next; }
	void					SetNext( NiForce *next_ ) { next = next_; }
	// the number of forces in the chain from this one (name invented)
	unsigned int			GetChainLength();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiForce *dest );

	NiForce *				next;		// the next force of the chain, NULL at its end
};

NiRegisterStream( NiForce );

#endif /* !__NIFORCE_H__ */
