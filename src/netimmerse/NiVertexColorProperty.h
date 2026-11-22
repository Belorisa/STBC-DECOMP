/*
===============================================================================

	NiVertexColorProperty.h

	How vertex colours feed the lighting: the source mode and the lighting
	mode.

	Evidence: the vtable at 0x889694 (NiProperty's 14 slots, none added;
	Type returns 11); CreateClone (0x80F980) allocates 0x20 bytes and
	the constructor sets the source mode at 0x18 to 0 and the lighting mode
	at 0x1C to 1; the renderer reads both. The NiRTTI is 0x9A2758, named
	"NiVertexColorProperty". The enum names follow the Gamebryo lineage,
	whose defaults (SOURCE_IGNORE, LIGHTING_E_A_D) are these; the viewer
	strings name "m_eSource", "m_eLighting" and every mode.

===============================================================================
*/

#ifndef __NIVERTEXCOLORPROPERTY_H__
#define __NIVERTEXCOLORPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiVertexColorPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x89A6EC). The class name is invented.
================
*/
// FUNCTION: STBC 0x0080fad0 ??_GNiVertexColorPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiVertexColorProperty
class NiVertexColorPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x430530 ??_GNiVertexColorProperty@@UAEPAXI@Z @game/AnimTSParticleController
// FUNCTION: STBC 0x430550 ??1NiVertexColorProperty@@UAE@XZ @game/AnimTSParticleController
class NiVertexColorProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x430520 ?GetRTTI@NiVertexColorProperty@@UBEPBVNiRTTI@@XZ @game/AnimTSParticleController
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

	// FUNCTION: STBC 0x430580 ?Type@NiVertexColorProperty@@UAEHXZ @game/AnimTSParticleController
	virtual int				Type() { return VERTEX_COLOR; }
	static int				GetType() { return VERTEX_COLOR; }

	enum {
		VERTEX_COLOR		= 11
	};

	enum SourceVertexMode {
		SOURCE_IGNORE,
		SOURCE_EMISSIVE,
		SOURCE_AMB_DIFF,
		SOURCE_NUM_MODES
	};

	enum LightingMode {
		LIGHTING_E,
		LIGHTING_E_A_D,
		LIGHTING_NUM_MODES
	};

							// FUNCTION: STBC 0x46C680 ??0NiVertexColorProperty@@QAE@XZ @game/OBBRoot
							NiVertexColorProperty() {
								source = SOURCE_IGNORE;
								lighting = LIGHTING_E_A_D;
							}

	// accessors (Gamebryo lineage names)
	SourceVertexMode		GetSourceMode() const { return source; }
	LightingMode			GetLightingMode() const { return lighting; }
	void					SetSourceMode( SourceVertexMode mode ) { source = mode; }
	void					SetLightingMode( LightingMode mode ) { lighting = mode; }

	// the vertexcolor property of objects without one
	// FUNCTION: STBC 0x820D70 ?GetDefault@NiVertexColorProperty@@SA?AV?$NiPointer@VNiVertexColorProperty@@@@XZ
	static NiPointer<NiVertexColorProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiVertexColorProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiVertexColorProperty *dest );

	// "prefix = MODE" in a new block
	static char *			GetViewerString( const char *prefix, SourceVertexMode mode );
	static char *			GetViewerString( const char *prefix, LightingMode mode );

	SourceVertexMode		source;
	LightingMode			lighting;
};

NiRegisterStream( NiVertexColorProperty );

#endif /* !__NIVERTEXCOLORPROPERTY_H__ */
