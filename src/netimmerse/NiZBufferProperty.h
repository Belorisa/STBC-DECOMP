/*
===============================================================================

	NiZBufferProperty.h

	The depth buffer: the depth test (bit 0 of NiProperty's flags) and depth
	writes (bit 1), both off by default.

	Evidence: the vtable at 0x888A40 (NiProperty's 14 slots, none added;
	Type returns 9); the creation function (0x8111A0) allocates 0x18
	bytes, NiProperty's own size, and clears the flags; IsEqual (0x8112B0)
	compares bits 0 and 1; the viewer strings name "m_bZTest" and
	"m_bZWrite". The NiRTTI is
	0x9A27E8, named "NiZBufferProperty".

===============================================================================
*/

#ifndef __NIZBUFFERPROPERTY_H__
#define __NIZBUFFERPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiZBufferPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x89A728). The class name is invented.
================
*/
// FUNCTION: STBC 0x00811240 ??_GNiZBufferPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiZBufferProperty
class NiZBufferPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x416520 ??_GNiZBufferProperty@@UAEPAXI@Z @game/AnimTSParticleController
// FUNCTION: STBC 0x416540 ??1NiZBufferProperty@@UAE@XZ @game/AnimTSParticleController
class NiZBufferProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x416510 ?GetRTTI@NiZBufferProperty@@UBEPBVNiRTTI@@XZ @game/AnimTSParticleController
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

	// FUNCTION: STBC 0x416570 ?Type@NiZBufferProperty@@UAEHXZ @game/AnimTSParticleController
	virtual int				Type() { return ZBUFFER; }
	static int				GetType() { return ZBUFFER; }

	// the property's type number (Type, and GetType for the class)
	enum {
		ZBUFFER				= 9
	};

							NiZBufferProperty() {
								SetZBufferTest( false );
								SetZBufferWrite( false );
							}

	void					SetZBufferTest( bool test ) { SetBit( test, ZBUFFER_TEST_POS ); }
	bool					GetZBufferTest() const { return GetBit( ZBUFFER_TEST_POS ); }
	void					SetZBufferWrite( bool write ) { SetBit( write, ZBUFFER_WRITE_POS ); }
	bool					GetZBufferWrite() const { return GetBit( ZBUFFER_WRITE_POS ); }

	// the zbuffer property of objects without one
	// FUNCTION: STBC 0x820D10 ?GetDefault@NiZBufferProperty@@SA?AV?$NiPointer@VNiZBufferProperty@@@@XZ
	static NiPointer<NiZBufferProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiZBufferProperty>	defaultProperty;	// name invented

protected:
	// the fields of the flags
	enum {
		ZBUFFER_TEST_POS	= 0,
		ZBUFFER_WRITE_POS	= 1
	};
};

NiRegisterStream( NiZBufferProperty );

#endif /* !__NIZBUFFERPROPERTY_H__ */
