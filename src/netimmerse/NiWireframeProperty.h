/*
===============================================================================

	NiWireframeProperty.h

	Wireframe or filled rendering: bit 0 of NiProperty's flags.

	Evidence: the vtable at 0x888960 (NiProperty's 14 slots, none added;
	Type returns 8); the creation function (0x810A80) allocates 0x18
	bytes, NiProperty's own size, and clears the flags; IsEqual (0x810B90)
	compares bit 0; the viewer strings name "m_bWireframe". The NiRTTI is
	0x9A27A8, named "NiWireframeProperty".

===============================================================================
*/

#ifndef __NIWIREFRAMEPROPERTY_H__
#define __NIWIREFRAMEPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiWireframePropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x89A724). The class name is invented.
================
*/
// FUNCTION: STBC 0x00810b20 ??_GNiWireframePropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiWireframeProperty
class NiWireframePropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x004164b0 ??_GNiWireframeProperty@@UAEPAXI@Z @netimmerse/NiWireframeProperty
class NiWireframeProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x4164A0 ?GetRTTI@NiWireframeProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x416500 ?Type@NiWireframeProperty@@UAEHXZ
	virtual int				Type() { return WIREFRAME; }
	static int				GetType() { return WIREFRAME; }

	// the property's type number (Type, and GetType for the class)
	enum {
		WIREFRAME			= 8
	};

							// FUNCTION: STBC 0x46C610 ??0NiWireframeProperty@@QAE@XZ @game/OBBRoot
							NiWireframeProperty() { SetWireframe( false ); }

	void					SetWireframe( bool wireframe ) { SetBit( wireframe, WIREFRAME_POS ); }
	bool					GetWireframe() const { return GetBit( WIREFRAME_POS ); }

	// the wireframe property of objects without one
	// FUNCTION: STBC 0x820CE0 ?GetDefault@NiWireframeProperty@@SA?AV?$NiPointer@VNiWireframeProperty@@@@XZ
	static NiPointer<NiWireframeProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiWireframeProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		WIREFRAME_POS		= 0
	};
};

NiRegisterStream( NiWireframeProperty );

#endif /* !__NIWIREFRAMEPROPERTY_H__ */
