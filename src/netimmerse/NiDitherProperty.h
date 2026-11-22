/*
===============================================================================

	NiDitherProperty.h

	Dithering on or off: bit 0 of NiProperty's flags.

	Evidence: the vtable at 0x8889D0 (NiProperty's 14 slots, none added;
	Type returns 10); the creation function (0x7FA760) allocates 0x18
	bytes, NiProperty's own size, and clears the flags; IsEqual (0x7FA870)
	compares bit 0; the viewer strings name "m_bDither". The NiRTTI is
	0x9A1F70, named "NiDitherProperty".

===============================================================================
*/

#ifndef __NIDITHERPROPERTY_H__
#define __NIDITHERPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiDitherPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899AB4). The class name is invented.
================
*/
// FUNCTION: STBC 0x007fa800 ??_GNiDitherPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiDitherProperty
class NiDitherPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x004126d0 ??_GNiDitherProperty@@UAEPAXI@Z @netimmerse/NiDitherProperty
class NiDitherProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x4126C0 ?GetRTTI@NiDitherProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x412720 ?Type@NiDitherProperty@@UAEHXZ
	virtual int				Type() { return DITHER; }
	static int				GetType() { return DITHER; }

	// the property's type number (Type, and GetType for the class)
	enum {
		DITHER				= 10
	};

							NiDitherProperty() { SetDithering( false ); }

	void					SetDithering( bool dither ) { SetBit( dither, DITHER_POS ); }
	bool					GetDithering() const { return GetBit( DITHER_POS ); }

	// the dither property of objects without one
	// FUNCTION: STBC 0x820D40 ?GetDefault@NiDitherProperty@@SA?AV?$NiPointer@VNiDitherProperty@@@@XZ
	static NiPointer<NiDitherProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiDitherProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		DITHER_POS			= 0
	};
};

NiRegisterStream( NiDitherProperty );

#endif /* !__NIDITHERPROPERTY_H__ */
