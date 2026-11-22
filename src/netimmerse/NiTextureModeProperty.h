/*
===============================================================================

	NiTextureModeProperty.h

	How a texture is applied, filtered and clamped, packed in NiProperty's
	flags, with two 16-bit level-of-detail values.

	Evidence: the vtable at 0x8896CC (NiProperty's 14 slots, none added;
	Type, 0x430260, returns 5); CreateClone (0x8013D0) allocates 0x1C
	bytes, sets the flags to 0xC2 and the shorts at 0x18 and 0x1A to 0 and
	-75, then copies both shorts; IsEqual (0x8016D0) compares the low byte
	of the flags and both shorts; the viewer strings (0x801710) format
	"m_apply" from bits 0-2, "m_filter" from bits 3-5 and "m_clamp" from
	bits 6-7 with the mode names (the helpers 0x8019D0, 0x801AA0 and
	0x801B70, which NiMultiTextureProperty calls too), then "m_sL" from 0x18
	and "m_sK" from 0x1A; LoadBinary (0x801570) renumbers the apply mode of
	files older than "3.02" and reads the shorts from "3.04" on. The NiRTTI
	is 0x9A2228, named "NiTextureModeProperty".

===============================================================================
*/

#ifndef __NITEXTUREMODEPROPERTY_H__
#define __NITEXTUREMODEPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiTextureModePropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899F34). The class name is invented.
================
*/
// FUNCTION: STBC 0x00801540 ??_GNiTextureModePropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiTextureModeProperty
class NiTextureModePropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x430210 ??_GNiTextureModeProperty@@UAEPAXI@Z @game/AnimTSParticleController
// FUNCTION: STBC 0x430230 ??1NiTextureModeProperty@@UAE@XZ @game/AnimTSParticleController
class NiTextureModeProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x430200 ?GetRTTI@NiTextureModeProperty@@UBEPBVNiRTTI@@XZ @game/AnimTSParticleController
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

	// FUNCTION: STBC 0x430260 ?Type@NiTextureModeProperty@@UAEHXZ @game/AnimTSParticleController
	virtual int				Type() { return TEXTURE_MODE; }
	static int				GetType() { return TEXTURE_MODE; }

	// the property's type number (Type, and GetType for the class)
	enum {
		TEXTURE_MODE		= 5
	};

	// how the texture combines with the surface colour, as the viewer
	// strings name the values
	enum ApplyMode {
		APPLY_REPLACE,
		APPLY_DECAL,
		APPLY_MODULATE,
		APPLY_MODULATE_VERTEX,
		APPLY_HILIGHT,
		APPLY_HILIGHT2
	};

	enum FilterMode {
		FILTER_NEAREST,
		FILTER_BILERP,
		FILTER_TRILERP,
		FILTER_NEAREST_MIPNEAREST,
		FILTER_NEAREST_MIPLERP,
		FILTER_BILERP_MIPNEAREST
	};

	enum ClampMode {
		CLAMP_S_CLAMP_T,
		CLAMP_S_WRAP_T,
		WRAP_S_CLAMP_T,
		WRAP_S_WRAP_T
	};

							// FUNCTION: STBC 0x004a3e30 ??0NiTextureModeProperty@@QAE@XZ @game/PlacementObject
							NiTextureModeProperty() {
								SetApplyMode( APPLY_MODULATE );
								SetFilterMode( FILTER_NEAREST );
								SetClampMode( WRAP_S_WRAP_T );
								l = 0;
								k = -75;
							}

	void					SetApplyMode( ApplyMode mode ) { SetField( mode, APPLY_MASK, APPLY_POS ); }
	ApplyMode				GetApplyMode() const { return (ApplyMode)GetField( APPLY_MASK, APPLY_POS ); }
	void					SetFilterMode( FilterMode mode ) { SetField( mode, FILTER_MASK, FILTER_POS ); }
	FilterMode				GetFilterMode() const { return (FilterMode)GetField( FILTER_MASK, FILTER_POS ); }
	void					SetClampMode( ClampMode mode ) { SetField( mode, CLAMP_MASK, CLAMP_POS ); }
	ClampMode				GetClampMode() const { return (ClampMode)GetField( CLAMP_MASK, CLAMP_POS ); }
	short					GetL() const { return l; }
	short					GetK() const { return k; }

	// the modes by name, "prefix = APPLY_MODULATE"
	static char *			GetViewerString( const char *prefix, ApplyMode mode );
	static char *			GetViewerString( const char *prefix, FilterMode mode );
	static char *			GetViewerString( const char *prefix, ClampMode mode );

	// the texture mode property of objects without one
	// FUNCTION: STBC 0x820C30 ?GetDefault@NiTextureModeProperty@@SA?AV?$NiPointer@VNiTextureModeProperty@@@@XZ
	static NiPointer<NiTextureModeProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiTextureModeProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiTextureModeProperty *dest );

	short					l;				// TODO identity: mipmap level-of-detail values ("m_sL", "m_sK")
	short					k;

	// the fields of the flags
	enum {
		APPLY_MASK			= 0x7,
		APPLY_POS			= 0,
		FILTER_MASK			= 0x7,
		FILTER_POS			= 3,
		CLAMP_MASK			= 0x3,
		CLAMP_POS			= 6
	};
};

NiRegisterStream( NiTextureModeProperty );

#endif /* !__NITEXTUREMODEPROPERTY_H__ */
