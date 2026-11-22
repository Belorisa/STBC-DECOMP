/*
===============================================================================

	NiMaterialProperty.h

	The material colours of lit surfaces: ambient, diffuse, specular and
	emissive, the shininess and the alpha.

	Evidence: the vtable at 0x899EF8 (NiProperty's 14 slots, none added;
	Type returns 3); CreateClone (0x8009B0) allocates 0x58 bytes; the
	renderer reads a key at 0x18, the colours at 0x1C, 0x28, 0x34 and 0x40,
	the shininess at 0x4C, the alpha at 0x50 and a revision at 0x54, and
	uses the default material at 0x9A21F8. The NiRTTI is 0x9A21E0, named
	"NiMaterialProperty". The constructor (0x800910) is out of line; it
	takes each new material's key from a counter (0x978B3C).

===============================================================================
*/

#ifndef __NIMATERIALPROPERTY_H__
#define __NIMATERIALPROPERTY_H__

#include "NiColor.h"
#include "NiPointer.h"
#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiMaterialPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899F30). The class name is invented.
================
*/
// FUNCTION: STBC 0x00800b70 ??_GNiMaterialPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiMaterialProperty
class NiMaterialPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x00801090 ??_GNiMaterialProperty@@UAEPAXI@Z @netimmerse/NiMaterialProperty
class NiMaterialProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x801080 ?GetRTTI@NiMaterialProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x8010D0 ?Type@NiMaterialProperty@@UAEHXZ
	virtual int				Type() { return MATERIAL; }
	static int				GetType() { return MATERIAL; }

	enum {
		MATERIAL			= 3
	};

							NiMaterialProperty();

	// accessors (Gamebryo lineage names)
	int						GetKey() const { return key; }
	const NiColor &			GetAmbientColor() const { return ambient; }
	const NiColor &			GetDiffuseColor() const { return diffuse; }
	const NiColor &			GetSpecularColor() const { return specular; }
	const NiColor &			GetEmittance() const { return emissive; }
	float					GetShineness() const { return shininess; }
	float					GetAlpha() const { return alpha; }
	unsigned int			GetRevisionID() const { return revision; }
	void					IncRevisionID() { revision++; }
	// set by every setter; NiPropertyState::GetMaterialAndRevise turns it into a new revision
	bool					GetChanged() const { return GetBit( UNKNOWN_FLAG_POS ); }
	void					SetChanged( bool changed ) { SetBit( changed, UNKNOWN_FLAG_POS ); }
	void					SetAlpha( float alpha_ ) { alpha = alpha_; SetChanged( true ); }	// NiAlphaController::Update (0x7959D0)
	// the colour setters of NiMaterialColorController::Update (0x7AC3B0)
	// FUNCTION: STBC 0x004a3dd0 ?SetAmbientColor@NiMaterialProperty@@QAEXABVNiColor@@@Z @game/PlacementObject
	void					SetAmbientColor( const NiColor &color ) { ambient = color; SetChanged( true ); }
	// FUNCTION: STBC 0x004a3df0 ?SetDiffuseColor@NiMaterialProperty@@QAEXABVNiColor@@@Z @game/PlacementObject
	void					SetDiffuseColor( const NiColor &color ) { diffuse = color; SetChanged( true ); }
	void					SetSpecularColor( const NiColor &color ) { specular = color; SetChanged( true ); }
	// FUNCTION: STBC 0x46C6D0 ?SetEmittance@NiMaterialProperty@@QAEXABVNiColor@@@Z
	void					SetEmittance( const NiColor &color ) { emissive = color; SetChanged( true ); }

	// the material of objects without one
	// FUNCTION: STBC 0x7D8490 ?GetDefault@NiMaterialProperty@@SA?AV?$NiPointer@VNiMaterialProperty@@@@XZ
	static NiPointer<NiMaterialProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiMaterialProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiMaterialProperty *dest );
	int						key;			// TODO identity: the renderer's material cache key
	NiColor					ambient;
	NiColor					diffuse;
	NiColor					specular;
	NiColor					emissive;
	float					shininess;
	float					alpha;
	unsigned int			revision;		// TODO identity: the renderer compares it with its cached copy

	static int				nextKey;		// the key of the next material; name invented

	// the fields of the flags
	enum {
		UNKNOWN_FLAG_POS	= 0			// TODO identity: set by the constructor; no reader known
	};
};

NiRegisterStream( NiMaterialProperty );

#endif /* !__NIMATERIALPROPERTY_H__ */
