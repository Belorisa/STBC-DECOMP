/*
===============================================================================

	NiTextureEffect.h

	A dynamic effect that projects a texture onto the objects it affects:
	a projected light or shadow, an environment map or a fog map.

	Evidence: the NiRTTI is 0x9A2498 (GetRTTI, 0x809C90); the vtable at
	0x89A1E4 has NiDynamicEffect's 40 slots and adds none (the load
	record's ??_G follows at slot 40's place); slot 39 (0x809CD0) returns
	TEXTURE_EFFECT. The constructor (0x808BB0) copies the identity into
	both projection matrices, clears both translations and the image,
	builds both planes from NiPoint3::UNIT_X and 0, then sets the clamp
	mode to 3 and the second short to -75; the destructor (0x808CB0) tells
	the renderers the effect is going. UpdateWorldData (0x808F00) follows
	NiAVObject's with 0x808D30, which builds the world projection and
	plane from the model ones. The member names and the order follow the
	Gamebryo lineage (m_kModelProjMat, m_kModelProjTrans, m_kWorldProjMat,
	m_kWorldProjTrans, m_spTexture, m_eFilter, m_eClamp, m_eTextureType,
	m_eCoordMode, m_bPlaneEnable, m_kModelPlane, m_kWorldPlane, m_sPS2L,
	m_sPS2K); the renderer's texture pipeline tests the coordinate mode
	against 1 and reads the plane switch.

===============================================================================
*/

#ifndef __NITEXTUREEFFECT_H__
#define __NITEXTUREEFFECT_H__

#include "NiDynamicEffect.h"
#include "NiMatrix3.h"
#include "NiPlane.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiTextureModeProperty.h"
#include "renderer/NiImage.h"
#include "NiStreamMacros.h"

/*
================
NiTextureEffectLoadRecord

The image's link ID, read by LoadBinary (0x809270) and resolved by
LinkObject (0x809430). CreateObject (0x809120) allocates 0x44 bytes and
stores the vptr 0x89A284 last. The class name is invented.
================
*/
// FUNCTION: STBC 0x809220 ??_GNiTextureEffectLoadRecord@@UAEPAXI@Z @netimmerse/NiTextureEffect
// FUNCTION: STBC 0x809240 ??1NiTextureEffectLoadRecord@@UAE@XZ @netimmerse/NiTextureEffect
class NiTextureEffectLoadRecord : public NiDynamicEffectLoadRecord {
public:
	unsigned int			imageLinkID;
};

class NiTextureEffect : public NiDynamicEffect {
public:
	// FUNCTION: STBC 0x809C90 ?GetRTTI@NiTextureEffect@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiTextureEffect();

	virtual void			UpdateWorldData();

	// FUNCTION: STBC 0x809CD0 ?GetEffectType@NiTextureEffect@@UBEHXZ
	virtual int				GetEffectType() const { return TEXTURE_EFFECT; }

							NiTextureEffect();

	// the kinds of projection, named by GetViewerStrings
	enum TextureType {
		PROJECTED_LIGHT,
		PROJECTED_SHADOW,
		ENVIRONMENT_MAP,
		FOG_MAP
	};

	// coordinate modes, named by GetViewerStrings: UpdateProjection builds
	// a fixed projection from the world rotation for a sphere map; the
	// pipeline tests the mode against WORLD_PERSPECTIVE
	enum CoordGenType {
		WORLD_PARALLEL,
		WORLD_PERSPECTIVE,
		SPHERE_MAP
	};

	NiImage *				GetEffectTexture() const { return image; }
	NiTextureModeProperty::FilterMode	GetTextureFilter() const { return filter; }
	NiTextureModeProperty::ClampMode	GetTextureClamp() const { return clamp; }
	TextureType				GetTextureType() const { return textureType; }
	CoordGenType			GetTextureCoordGen() const { return coordMode; }
	bool					GetClippingPlaneEnable() const { return planeEnabled; }
	const NiMatrix3 &		GetWorldProjectionMatrix() const { return worldProjection; }
	const NiPoint3 &		GetWorldProjectionTranslation() const { return worldProjectionTranslate; }
	const NiPlane &			GetWorldClippingPlane() const { return worldPlane; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the modes by name, for GetViewerStrings
	static char *			GetViewerString( const char *prefix, TextureType type );
	static char *			GetViewerString( const char *prefix, CoordGenType mode );

protected:
	void					CopyMembers( NiTextureEffect *dest );

	// the world projection and plane from the model ones and the world
	// transform (Gamebryo lineage name)
	void					UpdateProjection();

	NiMatrix3				modelProjection;
	NiPoint3				modelProjectionTranslate;
	NiMatrix3				worldProjection;
	NiPoint3				worldProjectionTranslate;
	NiPointer<NiImage>		image;
	NiTextureModeProperty::FilterMode	filter;
	NiTextureModeProperty::ClampMode	clamp;		// WRAP_S_WRAP_T after construction
	TextureType				textureType;
	CoordGenType			coordMode;
	unsigned char			planeEnabled;	// a bool in meaning; the viewer strings format it as a number
	NiPlane					modelPlane;
	NiPlane					worldPlane;
	short					l;			// as NiTextureModeProperty's
	short					k;			// -75 after construction
	bool					unknown1BC;
	bool					unknown1BD;
};

NiRegisterStream( NiTextureEffect );

/*
================
NiDynamicEffectState::GetNextProjLight

Returns the projected light at pos and advances pos; NULL at the end. The
list holds texture effects only: AddEffect files them by texture type.
================
*/
inline NiTextureEffect *NiDynamicEffectState::GetNextProjLight( ListItem *&pos ) const {
	if ( !pos ) {
		return 0;
	}
	NiTextureEffect *light = static_cast<NiTextureEffect *>( pos->effect );
	pos = pos->next;
	return light;
}

/*
================
NiDynamicEffectState::GetNextProjShadow

Returns the projected shadow at pos and advances pos; NULL at the end.
================
*/
inline NiTextureEffect *NiDynamicEffectState::GetNextProjShadow( ListItem *&pos ) const {
	if ( !pos ) {
		return 0;
	}
	NiTextureEffect *shadow = static_cast<NiTextureEffect *>( pos->effect );
	pos = pos->next;
	return shadow;
}

#endif /* !__NITEXTUREEFFECT_H__ */
