/*
===============================================================================

	NiAmbientLight.h

	A light without direction or position: it adds its ambient colour
	everywhere.

	Evidence: the vtable at 0x8994E8 (NiLight's 40 slots, slot 39 returning
	0); the constructor (0x7F3FC0) calls NiLight's and adds no members
	(CreateClone, 0x7F4040, allocates 0x148 bytes, NiLight's size); the
	destructor (0x7F3FE0) first tells every renderer to drop the light
	(NiRender::RemoveEffect); the NiRTTI (0x9A1C30) is named
	"NiAmbientLight" with NiLight's as its base.

===============================================================================
*/

#ifndef __NIAMBIENTLIGHT_H__
#define __NIAMBIENTLIGHT_H__

#include "NiLight.h"
#include "NiStreamMacros.h"

/*
================
NiAmbientLightLoadRecord

CreateObject (0x7F4130) allocates 0x40 bytes and stores the vptr 0x899588
last (destructor 0x7F4250); it adds nothing to the effect's record. The
class name is invented.
================
*/
// FUNCTION: STBC 0x7F4230 ??_GNiAmbientLightLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F4250 ??1NiAmbientLightLoadRecord@@UAE@XZ
class NiAmbientLightLoadRecord : public NiLightLoadRecord {
};

// FUNCTION: STBC 0x007F44E0 ??_GNiAmbientLight@@UAEPAXI@Z
class NiAmbientLight : public NiLight {
public:
	// FUNCTION: STBC 0x007F44D0 ?GetRTTI@NiAmbientLight@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiAmbientLight();

	// FUNCTION: STBC 0x007F4510 ?GetEffectType@NiAmbientLight@@UBEHXZ
	virtual int				GetEffectType() const { return AMBIENT_LIGHT; }

							NiAmbientLight();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAmbientLight *dest );
};

NiRegisterStream( NiAmbientLight );

#endif /* !__NIAMBIENTLIGHT_H__ */
