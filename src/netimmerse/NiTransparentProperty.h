/*
===============================================================================

	NiTransparentProperty.h

	Colour-key transparency: bit 0 of NiProperty's flags turns it on, and
	the key colour is a packed 32-bit value.

	Evidence: the vtable at 0x89A2C4 (NiProperty's 14 slots, none added;
	Type, 0x80B120, returns 7); CreateClone (0x80ABC0) allocates 0x1C
	bytes, clears the flags and the colour at 0x18 and copies the colour;
	IsEqual (0x80ADB0) compares bit 0 and the colour; the viewer strings
	name "m_bTransparent" and "m_uiColor". The NiRTTI is 0x9A2508, named
	"NiTransparentProperty".

===============================================================================
*/

#ifndef __NITRANSPARENTPROPERTY_H__
#define __NITRANSPARENTPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiTransparentPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x89A2FC). The class name is invented.
================
*/
// FUNCTION: STBC 0x0080ad00 ??_GNiTransparentPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiTransparentProperty
class NiTransparentPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x0080b0e0 ??_GNiTransparentProperty@@UAEPAXI@Z @netimmerse/NiTransparentProperty
class NiTransparentProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x80B0D0 ?GetRTTI@NiTransparentProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x80B120 ?Type@NiTransparentProperty@@UAEHXZ
	virtual int				Type() { return TRANSPARENCY; }
	static int				GetType() { return TRANSPARENCY; }

	// the property's type number (Type, and GetType for the class); not TRANSPARENT, which
	// windows.h defines
	enum {
		TRANSPARENCY		= 7
	};

							NiTransparentProperty() { SetTransparent( false ); color = 0; }

	void					SetTransparent( bool transparent ) { SetBit( transparent, TRANSPARENT_POS ); }
	bool					GetTransparent() const { return GetBit( TRANSPARENT_POS ); }
	unsigned int			GetColor() const { return color; }

	// the transparent property of objects without one
	// FUNCTION: STBC 0x820CB0 ?GetDefault@NiTransparentProperty@@SA?AV?$NiPointer@VNiTransparentProperty@@@@XZ
	static NiPointer<NiTransparentProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiTransparentProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiTransparentProperty *dest );

	unsigned int			color;			// the key colour, packed

	// the fields of the flags
	enum {
		TRANSPARENT_POS		= 0
	};
};

NiRegisterStream( NiTransparentProperty );

#endif /* !__NITRANSPARENTPROPERTY_H__ */
