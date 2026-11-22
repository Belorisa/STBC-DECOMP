/*
===============================================================================

	NiGravity.h

	A force that accelerates particles along a direction (planar) or toward
	a point (spherical), optionally weakening with distance.

	Evidence: the NiRTTI (0x9A0340) is named "NiGravity" with NiForce's as
	its base; the vtable at 0x898348 replaces GetRTTI, CreateClone, the five
	streaming slots, the destructor and NiForce's three slots; the
	constructor (0x79E810) sets the float at 0x10 to 1 and clears the rest;
	CreateClone allocates 0x48 bytes. The stream stores two floats, a type
	and two points; type 1 measures from the point, the others along the
	direction. The member names follow the Gamebryo lineage (m_fDecay,
	m_fStrength, m_eType, m_kPosition, m_kDirection; FORCE_PLANAR,
	FORCE_SPHERICAL).

===============================================================================
*/

#ifndef __NIGRAVITY_H__
#define __NIGRAVITY_H__

#include "NiForce.h"
#include "NiPoint3.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiGravityLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x8983C4). The class name is invented.
================
*/
// FUNCTION: STBC 0x79ED60 ??_GNiGravityLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x79ED80 ??1NiGravityLoadRecord@@UAE@XZ
class NiGravityLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x79F060 ??_GNiGravity@@UAEPAXI@Z
// FUNCTION: STBC 0x79F080 ??1NiGravity@@UAE@XZ
class NiGravity : public NiForce {
public:
	enum ForceType {
		FORCE_PLANAR,
		FORCE_SPHERICAL
	};

	// FUNCTION: STBC 0x79F050 ?GetRTTI@NiGravity@@UBEPBVNiRTTI@@XZ
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

							NiGravity();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiGravity *dest );

	float					decay;			// 0: no falloff with distance
	float					strength;
	ForceType				type;
	NiPoint3				position;		// model space
	NiPoint3				direction;
	NiPoint3				worldPosition;	// set by UpdateWorld
	NiPoint3				worldDirection;
};

NiRegisterStream( NiGravity );

#endif /* !__NIGRAVITY_H__ */
