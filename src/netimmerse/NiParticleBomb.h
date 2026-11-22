/*
===============================================================================

	NiParticleBomb.h

	A force that pushes particles away from a point, an axis or a plane for
	a while, optionally weakening with distance.

	Evidence: the NiRTTI (0x9A0368) is named "NiParticleBomb" with NiForce's
	as its base; the vtable at 0x8983C8 replaces GetRTTI, CreateClone, the
	five streaming slots, the destructor and NiForce's three slots; the
	constructor (0x79F260) clears every member; CreateClone allocates 0x54
	bytes. The stream stores four floats, two types and two points. Apply
	(0x79F3C0) acts over the part of the step that overlaps the time from
	the float at 0x18 for the float at 0x10, skips particles farther than
	the float at 0xC when the type at 0x1C is set (1: linear falloff, 2:
	exponential), and pushes them along the type at 0x20's direction (0:
	from the point, 2: from the plane, else from the axis) by the float at
	0x14. The member and enumerator names follow the Gamebryo lineage
	(m_fDecay, m_fDuration, m_fDeltaV, m_fStart, m_eDecayType, m_eSymmType,
	m_kPosition, m_kDirection; NONE, LINEAR, EXPONENTIAL; SPHERICAL,
	CYLINDRICAL, PLANAR).

===============================================================================
*/

#ifndef __NIPARTICLEBOMB_H__
#define __NIPARTICLEBOMB_H__

#include "NiForce.h"
#include "NiPoint3.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiParticleBombLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898404). The class name is invented.
================
*/
// FUNCTION: STBC 0x79F7F0 ??_GNiParticleBombLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x79F810 ??1NiParticleBombLoadRecord@@UAE@XZ
class NiParticleBombLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x79FB50 ??_GNiParticleBomb@@UAEPAXI@Z
// FUNCTION: STBC 0x79FB70 ??1NiParticleBomb@@UAE@XZ
class NiParticleBomb : public NiForce {
public:
	enum DecayType {
		NONE,
		LINEAR,
		EXPONENTIAL
	};

	enum SymmType {
		SPHERICAL,
		CYLINDRICAL,
		PLANAR
	};

	// FUNCTION: STBC 0x79FB40 ?GetRTTI@NiParticleBomb@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual bool			Apply( NiPoint3 &position_, NiPoint3 &velocity, float &deltaTime, float time );
	virtual void			UpdateWorld( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale );
	virtual void			UpdateWorldIdentity();

							NiParticleBomb();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiParticleBomb *dest );

	float					decay;			// the reach when decayType is set
	float					duration;		// seconds
	float					deltaV;			// the speed added per second
	float					start;			// the time the bomb goes off
	DecayType				decayType;
	SymmType				symmType;
	NiPoint3				position;		// model space
	NiPoint3				direction;		// the axis or the plane's normal
	NiPoint3				worldPosition;	// set by UpdateWorld
	NiPoint3				worldDirection;
};

NiRegisterStream( NiParticleBomb );

#endif /* !__NIPARTICLEBOMB_H__ */
