/*
===============================================================================

	NiPlanarCollider.h

	A rectangle that particles bounce off.

	Evidence: the NiRTTI (0x9A03B0) is named "NiPlanarCollider" with
	NiForce's as its base; the vtable at 0x898448 replaces GetRTTI,
	CreateClone, the five streaming slots, the destructor and NiForce's three
	slots; the constructor (0x7A0820) sets the floats at 0xC, 0x10 and 0x14
	to 1 and clears the rest; CreateClone allocates 0x80 bytes. The model
	frame (a point, two axes and a plane) sits at 0x18 and its world copy at
	0x4C; UpdateWorld rebuilds the world plane's constant from its normal
	and the world point. Apply (0x7A0A60) intersects the step with the plane
	and keeps hits within half the float at 0x10 along the first axis and
	half the float at 0xC off it, then scales the reflected velocity by the
	float at 0x14. The names follow the Gamebryo lineage (m_fHeight,
	m_fWidth, m_kPosition, m_kXVector, m_kYVector, m_kPlane; the collider
	base's m_fBounce).

===============================================================================
*/

#ifndef __NIPLANARCOLLIDER_H__
#define __NIPLANARCOLLIDER_H__

#include "NiForce.h"
#include "NiPoint3.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiPlanarColliderLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898488). The class name is invented.
================
*/
// FUNCTION: STBC 0x7A1050 ??_GNiPlanarColliderLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7A1070 ??1NiPlanarColliderLoadRecord@@UAE@XZ
class NiPlanarColliderLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x7A13F0 ??_GNiPlanarCollider@@UAEPAXI@Z
// FUNCTION: STBC 0x7A1410 ??1NiPlanarCollider@@UAE@XZ
class NiPlanarCollider : public NiForce {
public:
	// FUNCTION: STBC 0x7A13E0 ?GetRTTI@NiPlanarCollider@@UBEPBVNiRTTI@@XZ
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

							NiPlanarCollider();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiPlanarCollider *dest );

	float					height;			// the extent along yAxis
	float					width;			// the extent along xAxis
	float					bounce;			// the share of the velocity kept after a hit
	NiPoint3				position;		// the centre, model space
	NiPoint3				xAxis;
	NiPoint3				yAxis;
	NiPoint3				normal;			// the plane: normal . point + constant = 0
	float					constant;
	NiPoint3				worldPosition;	// set by UpdateWorld
	NiPoint3				worldXAxis;
	NiPoint3				worldYAxis;
	NiPoint3				worldNormal;	// Apply turns the plane to face the particle
	float					worldConstant;
};

NiRegisterStream( NiPlanarCollider );

#endif /* !__NIPLANARCOLLIDER_H__ */
