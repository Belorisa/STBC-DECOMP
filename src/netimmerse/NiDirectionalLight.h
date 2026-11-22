/*
===============================================================================

	NiDirectionalLight.h

	A light from one direction everywhere, like the sun: it shines down
	its local x axis.

	Evidence: the vtable at 0x899A10 (NiLight's 40 slots, slot 39 returning
	1); the constructor (0x7F9D70) calls NiLight's and sets the world
	direction at 0x148 to (1, 0, 0); the world update (0x7F9E00) rotates
	NiPoint3::UNIT_X by the world rotation into it and bumps the revision;
	the destructor (0x7F9DA0) first tells every renderer to drop the light
	(NiRender::RemoveEffect); the NiRTTI (0x9A1F18) is named
	"NiDirectionalLight" with NiLight's as its base. The member name follows
	the Gamebryo lineage (m_kWorldDir).

===============================================================================
*/

#ifndef __NIDIRECTIONALLIGHT_H__
#define __NIDIRECTIONALLIGHT_H__

#include "NiLight.h"
#include "NiStreamMacros.h"

/*
================
NiDirectionalLightLoadRecord

CreateObject (0x7F9FB0) allocates 0x40 bytes and stores the effect's record
layout under its own vptr; it adds nothing to it. The class name is
invented.
================
*/
// FUNCTION: STBC 0x7FA0D0 ??_GNiDirectionalLightLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FA0F0 ??1NiDirectionalLightLoadRecord@@UAE@XZ
class NiDirectionalLightLoadRecord : public NiLightLoadRecord {
};

// FUNCTION: STBC 0x007FA380 ??_GNiDirectionalLight@@UAEPAXI@Z
class NiDirectionalLight : public NiLight {
public:
	// FUNCTION: STBC 0x007FA370 ?GetRTTI@NiDirectionalLight@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiDirectionalLight();

	virtual void			UpdateWorldData();

	// FUNCTION: STBC 0x007FA3B0 ?GetEffectType@NiDirectionalLight@@UBEHXZ
	virtual int				GetEffectType() const { return DIR_LIGHT; }

							NiDirectionalLight();

	const NiPoint3 &		GetWorldDirection() const { return worldDirection; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiDirectionalLight *dest );

	NiPoint3				worldDirection;		// the local x axis in world space
};

NiRegisterStream( NiDirectionalLight );

#endif /* !__NIDIRECTIONALLIGHT_H__ */
