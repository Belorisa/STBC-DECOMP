/*
===============================================================================

	NiPointLight.h

	A light from one point, weaker with distance: the attenuation is
	1 / ( constant + linear * d + quadratic * d * d ).

	Evidence: the vtable at 0x8898B0 (NiLight's 40 slots, slot 39 returning
	2; a game unit emitted it first, with the inline constructor and
	GetRTTI); the constructor, inline in CreateClone (0x803970), calls
	NiLight's and sets the attenuations at 0x148, 0x14C and 0x150 to 0, 1
	and 0; the world update (0x803950) only bumps the revision; the
	destructor (0x8038F0) first tells every renderer to drop the light
	(NiRender::RemoveEffect); the NiRTTI (0x9A22A0) is named
	"NiPointLight" with NiLight's as its base; the viewer strings name the
	members ("m_fAtten0", "m_fAtten1", "m_fAtten2").

===============================================================================
*/

#ifndef __NIPOINTLIGHT_H__
#define __NIPOINTLIGHT_H__

#include "NiLight.h"
#include "NiStreamMacros.h"

/*
================
NiPointLightLoadRecord

CreateObject (0x803AD0) allocates 0x40 bytes and stores the effect's record
layout under its own vptr; it adds nothing to it. The class name is
invented.
================
*/
// FUNCTION: STBC 0x803BF0 ??_GNiPointLightLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x803C10 ??1NiPointLightLoadRecord@@UAE@XZ
class NiPointLightLoadRecord : public NiLightLoadRecord {
};

// FUNCTION: STBC 0x004348E0 ??_GNiPointLight@@UAEPAXI@Z @game/LightObjectClass
class NiPointLight : public NiLight {
public:
	// FUNCTION: STBC 0x004348D0 ?GetRTTI@NiPointLight@@UBEPBVNiRTTI@@XZ @game/LightObjectClass
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

	virtual					~NiPointLight();

	virtual void			UpdateWorldData();

	// FUNCTION: STBC 0x00434920 ?GetEffectType@NiPointLight@@UBEHXZ @game/LightObjectClass
	virtual int				GetEffectType() const { return POINT_LIGHT; }

							NiPointLight() { constantAttenuation = 0.0f; linearAttenuation = 1.0f; quadraticAttenuation = 0.0f; }

	float					GetConstantAttenuation() const { return constantAttenuation; }
	float					GetLinearAttenuation() const { return linearAttenuation; }
	float					GetQuadraticAttenuation() const { return quadraticAttenuation; }
	// each marks the light changed (Gamebryo lineage names)
	void					SetConstantAttenuation( float c ) { constantAttenuation = c; IncRevisionID(); }
	void					SetLinearAttenuation( float l ) { linearAttenuation = l; IncRevisionID(); }
	void					SetQuadraticAttenuation( float q ) { quadraticAttenuation = q; IncRevisionID(); }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiPointLight *dest );

	float					constantAttenuation;	// "m_fAtten0"
	float					linearAttenuation;		// "m_fAtten1"
	float					quadraticAttenuation;	// "m_fAtten2"
};

NiRegisterStream( NiPointLight );

#endif /* !__NIPOINTLIGHT_H__ */
