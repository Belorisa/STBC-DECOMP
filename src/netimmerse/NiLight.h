/*
===============================================================================

	NiLight.h

	The base of the light effects: a dimmer and the ambient, diffuse and
	specular colours, all white and full strength by default.

	Evidence: the vtable at 0x899448 (40 slots, slot 39 still _purecall);
	the constructor (0x7F37A0) calls NiDynamicEffect's, sets the float at
	0x120 and the three colours at 0x124, 0x130 and 0x13C to 1.0 and clears
	the world bound's radius; the NiRTTI (0x9A1BF8) is named "NiLight" with
	NiDynamicEffect's as its base; NiAmbientLight's creation function
	(0x7F4130) allocates 0x148 bytes and adds nothing. The member names
	come from the viewer strings "m_fDimmer", "m_amb", "m_diff" and
	"m_spec".

===============================================================================
*/

#ifndef __NILIGHT_H__
#define __NILIGHT_H__

#include "NiColor.h"
#include "NiDynamicEffect.h"
#include "NiStreamMacros.h"

/*
================
NiLightLoadRecord

The record level of the light classes: NiDynamicEffect's, adding nothing.
The class name is invented.

Evidence: the load records mirror the object classes (NiAVObject's derives
from NiObjectNET's), and the light creation functions expand their records'
constructors one level less deep than an NiDynamicEffect record would allow
(NiSpotLight::CreateObject, 0x806F10, calls the effect array's constructor
out of line); the vtable stores of this level are dead in every inlined
constructor.
================
*/
class NiLightLoadRecord : public NiDynamicEffectLoadRecord {
};

// FUNCTION: STBC 0x007f3db0 ??_GNiLight@@UAEPAXI@Z @netimmerse/NiLight
class NiLight : public NiDynamicEffect {
public:
	// FUNCTION: STBC 0x7F3DA0 ?GetRTTI@NiLight@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiLight();

							NiLight();

	float					GetDimmer() const { return dimmer; }
	void					SetDimmer( float dimmer_ ) { dimmer = dimmer_; IncRevisionID(); }
	const NiColor &			GetAmbientColor() const { return ambient; }
	void					SetAmbientColor( const NiColor &color ) { ambient = color; IncRevisionID(); }
	const NiColor &			GetDiffuseColor() const { return diffuse; }
	void					SetDiffuseColor( const NiColor &color ) { diffuse = color; IncRevisionID(); }
	const NiColor &			GetSpecularColor() const { return specular; }
	void					SetSpecularColor( const NiColor &color ) { specular = color; IncRevisionID(); }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiLight *dest );

	float					dimmer;		// scales the colours
	NiColor					ambient;
	NiColor					diffuse;
	NiColor					specular;
};

NiRegisterStream( NiLight );

#endif /* !__NILIGHT_H__ */
