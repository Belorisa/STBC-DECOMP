/*
===============================================================================

	NiCorrectionProperty.h

	Gamma correction on or off: bit 0 of NiProperty's flags, set by
	default.

	Evidence: the vtable at 0x8999D4 (NiProperty's 14 slots, none added;
	Type, 0x7F9BD0, returns 1); CreateClone (0x7F9750) allocates 0x18
	bytes, NiProperty's own size, and sets the flags to 1; the viewer
	strings (0x7F9910) name "m_bCorrect", formatted from bit 0. The NiRTTI
	is 0x9A1ED0, named "NiCorrectionProperty".

===============================================================================
*/

#ifndef __NICORRECTIONPROPERTY_H__
#define __NICORRECTIONPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiCorrectionPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899A0C). The class name is invented.
================
*/
// FUNCTION: STBC 0x007f9870 ??_GNiCorrectionPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiCorrectionProperty
class NiCorrectionPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x007f9b90 ??_GNiCorrectionProperty@@UAEPAXI@Z @netimmerse/NiCorrectionProperty
class NiCorrectionProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x7F9B80 ?GetRTTI@NiCorrectionProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x7F9BD0 ?Type@NiCorrectionProperty@@UAEHXZ
	virtual int				Type() { return CORRECTION; }
	static int				GetType() { return CORRECTION; }

	// the property's type number (Type, and GetType for the class)
	enum {
		CORRECTION			= 1
	};

							NiCorrectionProperty() { SetCorrect( true ); }

	void					SetCorrect( bool correct ) { SetBit( correct, CORRECT_POS ); }
	bool					GetCorrect() const { return GetBit( CORRECT_POS ); }

	// the correction property of objects without one
	// FUNCTION: STBC 0x820B90 ?GetDefault@NiCorrectionProperty@@SA?AV?$NiPointer@VNiCorrectionProperty@@@@XZ
	static NiPointer<NiCorrectionProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiCorrectionProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		CORRECT_POS			= 0
	};
};

NiRegisterStream( NiCorrectionProperty );

#endif /* !__NICORRECTIONPROPERTY_H__ */
