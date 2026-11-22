/*
===============================================================================

	NiSpecularProperty.h

	Specular lighting on or off: bit 0 of NiProperty's flags.

	Evidence: the vtable at 0x8949E4 (NiProperty's 14 slots, none added;
	Type returns 12); the creation function (0x806720) allocates 0x18
	bytes, NiProperty's own size, and clears the flags; IsEqual (0x806830)
	compares bit 0; the viewer strings name "m_bSpec". The NiRTTI is
	0x9A23B0, named "NiSpecularProperty".

===============================================================================
*/

#ifndef __NISPECULARPROPERTY_H__
#define __NISPECULARPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiSpecularPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x89A0D0). The class name is invented.
================
*/
// FUNCTION: STBC 0x008067c0 ??_GNiSpecularPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiSpecularProperty
class NiSpecularPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x0066ef60 ??_GNiSpecularProperty@@UAEPAXI@Z @game/CharacterClass
class NiSpecularProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x66EF50 ?GetRTTI@NiSpecularProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x66EFB0 ?Type@NiSpecularProperty@@UAEHXZ
	virtual int				Type() { return SPECULAR; }
	static int				GetType() { return SPECULAR; }

	// the property's type number (Type, and GetType for the class)
	enum {
		SPECULAR			= 12
	};

							NiSpecularProperty() { SetSpecular( false ); }

	void					SetSpecular( bool specular ) { SetBit( specular, SPECULAR_POS ); }
	bool					GetSpecular() const { return GetBit( SPECULAR_POS ); }

	// the specular property of objects without one
	// FUNCTION: STBC 0x820DA0 ?GetDefault@NiSpecularProperty@@SA?AV?$NiPointer@VNiSpecularProperty@@@@XZ
	static NiPointer<NiSpecularProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiSpecularProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		SPECULAR_POS		= 0
	};
};

NiRegisterStream( NiSpecularProperty );

#endif /* !__NISPECULARPROPERTY_H__ */
