/*
===============================================================================

	NiSphericalCollider.h

	A sphere that particles bounce off, from inside or outside.

	Evidence: the NiRTTI (0x9A0380) is named "NiSphericalCollider" with
	NiForce's as its base; the vtable at 0x898408 replaces GetRTTI,
	CreateClone, the five streaming slots, the destructor and NiForce's three
	slots; the constructor (0x79FD50) sets the floats at 0xC, 0x10 and 0x14
	to 1 and clears those at 0x18 and 0x1C; CreateClone allocates 0x38
	bytes. Apply (0x79FE40) tests the distance from the centre against the
	float at 0xC and the squared distance against the float at 0x10 (the
	radius and its square), divides the hit normal by the float at 0xC and
	scales the reflected velocity by the float at 0x14. The floats at 0x18
	and 0x1C are streamed, cloned and compared but nothing here reads them.
	The names radius and position follow the Gamebryo lineage (m_fRadius,
	m_kPosition); bounce is the Gamebryo collider base's m_fBounce.

===============================================================================
*/

#ifndef __NISPHERICALCOLLIDER_H__
#define __NISPHERICALCOLLIDER_H__

#include "NiForce.h"
#include "NiPoint3.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiSphericalColliderLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898444). The class name is invented.
================
*/
// FUNCTION: STBC 0x7A0370 ??_GNiSphericalColliderLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7A0390 ??1NiSphericalColliderLoadRecord@@UAE@XZ
class NiSphericalColliderLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x7A0620 ??_GNiSphericalCollider@@UAEPAXI@Z
// FUNCTION: STBC 0x7A0640 ??1NiSphericalCollider@@UAE@XZ
class NiSphericalCollider : public NiForce {
public:
	// FUNCTION: STBC 0x7A0610 ?GetRTTI@NiSphericalCollider@@UBEPBVNiRTTI@@XZ
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

							NiSphericalCollider();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiSphericalCollider *dest );

	float					radius;
	float					radiusSquared;
	float					bounce;			// the share of the velocity kept after a hit
	float					unknown18;
	float					unknown1C;
	NiPoint3				position;		// the centre, model space
	NiPoint3				worldPosition;	// set by UpdateWorld
};

NiRegisterStream( NiSphericalCollider );

#endif /* !__NISPHERICALCOLLIDER_H__ */
