/*
===============================================================================

	NiSpotLight.h

	A point light limited to a cone around its local x axis.

	Evidence: the vtable at 0x89A0D4 (NiLight's 40 slots, slot 39 returning
	3); the constructor (0x806C70) runs NiPointLight's inline one, sets the
	world direction at 0x154 to (1, 0, 0) and clears the floats at 0x160
	and 0x164; the world update (0x806D20) calls NiPointLight's, rotates
	NiPoint3::UNIT_X by the world rotation into the direction and bumps the
	revision again; the destructor (0x806CC0) first tells every renderer to
	drop the light, then runs NiPointLight's; the NiRTTI (0x9A23F8) is
	named "NiSpotLight" with NiPointLight's as its base; the viewer strings
	name the members ("m_wDir", "m_fSpotAngle", "m_fSpotExponent").

===============================================================================
*/

#ifndef __NISPOTLIGHT_H__
#define __NISPOTLIGHT_H__

#include "NiPointLight.h"
#include "NiStreamMacros.h"

/*
================
NiSpotLightLoadRecord

CreateObject (0x806F10) allocates 0x40 bytes and stores the effect's record
layout under its own vptr (0x89A174); it adds nothing to it. The class name
is invented, after NiPointLightLoadRecord.
================
*/
// FUNCTION: STBC 0x807020 ??_GNiSpotLightLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x807040 ??1NiSpotLightLoadRecord@@UAE@XZ
class NiSpotLightLoadRecord : public NiPointLightLoadRecord {
};

// FUNCTION: STBC 0x008074B0 ??_GNiSpotLight@@UAEPAXI@Z
class NiSpotLight : public NiPointLight {
public:
	// FUNCTION: STBC 0x008074A0 ?GetRTTI@NiSpotLight@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiSpotLight();

	virtual void			UpdateWorldData();

	// FUNCTION: STBC 0x008074E0 ?GetEffectType@NiSpotLight@@UBEHXZ
	virtual int				GetEffectType() const { return SPOT_LIGHT; }

							NiSpotLight();

	const NiPoint3 &		GetWorldDirection() const { return worldDirection; }
	float					GetSpotAngle() const { return spotAngle; }
	float					GetSpotExponent() const { return spotExponent; }
	// each marks the light changed (Gamebryo lineage names)
	void					SetSpotAngle( float angle ) { spotAngle = angle; IncRevisionID(); }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiSpotLight *dest );

	NiPoint3				worldDirection;		// the local x axis in world space ("m_wDir")
	float					spotAngle;
	float					spotExponent;
};

NiRegisterStream( NiSpotLight );

#endif /* !__NISPOTLIGHT_H__ */
