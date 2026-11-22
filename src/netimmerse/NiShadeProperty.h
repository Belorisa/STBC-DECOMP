/*
===============================================================================

	NiShadeProperty.h

	Smooth (Gouraud) or flat shading: bit 0 of NiProperty's flags, set by
	default.

	Evidence: the vtable at 0x899F80 (NiProperty's 14 slots, none added;
	Type returns 4); the creation function (0x804400) allocates 0x18
	bytes, NiProperty's own size, and sets the flags to 1; IsEqual (0x804510)
	compares bit 0; the viewer strings name "m_bSmooth". The NiRTTI is
	0x9A22F0, named "NiShadeProperty".

===============================================================================
*/

#ifndef __NISHADEPROPERTY_H__
#define __NISHADEPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiShadePropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899FB8). The class name is invented.
================
*/
// FUNCTION: STBC 0x008044a0 ??_GNiShadePropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiShadeProperty
class NiShadePropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x008047c0 ??_GNiShadeProperty@@UAEPAXI@Z @netimmerse/NiShadeProperty
class NiShadeProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x8047B0 ?GetRTTI@NiShadeProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x804800 ?Type@NiShadeProperty@@UAEHXZ
	virtual int				Type() { return SHADE; }
	static int				GetType() { return SHADE; }

	// the property's type number (Type, and GetType for the class)
	enum {
		SHADE				= 4
	};

							NiShadeProperty() { SetSmooth( true ); }

	void					SetSmooth( bool smooth ) { SetBit( smooth, SMOOTH_POS ); }
	bool					GetSmooth() const { return GetBit( SMOOTH_POS ); }

	// the shade property of objects without one
	// FUNCTION: STBC 0x820C00 ?GetDefault@NiShadeProperty@@SA?AV?$NiPointer@VNiShadeProperty@@@@XZ
	static NiPointer<NiShadeProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiShadeProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		SMOOTH_POS			= 0
	};
};

NiRegisterStream( NiShadeProperty );

#endif /* !__NISHADEPROPERTY_H__ */
