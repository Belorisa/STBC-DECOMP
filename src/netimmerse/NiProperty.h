/*
===============================================================================

	NiProperty.h

	The base of the rendering properties (alpha, z-buffer, fog, material,
	texture, ...) that scene graph objects carry.

	Evidence: the vtable at 0x888B0C (14 slots: slot 12 is _purecall, and the
	derived properties return a small type number from it, 0 for
	NiAlphaProperty, 2 for NiFogProperty, 9 for NiZBufferProperty, 10 for
	NiDitherProperty); the inline constructor (0x40DAD0) clears the 16-bit
	flags at 0x14; the streaming functions read and write those two bytes.
	The destructor is the compiler's, inlined at its uses.

	NiPropertyList, the list of properties an NiAVObject (or an extra data
	block such as GlowPass) carries, is typedef'd here as in the Gamebryo
	lineage, so a unit can use it without NiAVObject.h.

===============================================================================
*/

#ifndef __NIPROPERTY_H__
#define __NIPROPERTY_H__

#include "NiObjectNET.h"
#include "NiStreamMacros.h"
#include "NiTSimpleList.h"

// FUNCTION: STBC 0x0040db00 ??_GNiProperty@@UAEPAXI@Z @game/CharacterClass
class NiProperty : public NiObjectNET {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7DBB00)
	static int				Register();
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// FUNCTION: STBC 0x40DAF0 ?GetRTTI@NiProperty@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			VirtualSlot03();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual int				Type() = 0;
	virtual void			Update( float time );

							NiProperty();

	// Type returns 0 .. MAX_TYPES - 1 (NiAVObject's property lookups
	// reject anything else)
	enum {
		MAX_TYPES			= 15
	};

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiProperty *dest );

	// packed fields of the flags (Gamebryo lineage: NiDeclareFlags); the
	// out-of-line copies 0x40DA60 and 0x40DA90 come from a game unit
	// FUNCTION: STBC 0x40DA60 ?SetField@NiProperty@@IAEXGGG@Z @game/AnimTSParticleController
	void					SetField( unsigned short value, unsigned short mask, unsigned short pos ) { flags = ( flags & ~( mask << pos ) ) | ( value << pos ); }
	unsigned short			GetField( unsigned short mask, unsigned short pos ) const { return ( flags >> pos ) & mask; }
	// FUNCTION: STBC 0x40DA90 ?SetBit@NiProperty@@IAEX_NG@Z @game/AnimTSParticleController
	void					SetBit( bool value, unsigned short pos ) { if ( value ) { flags |= 1 << pos; } else { flags &= ~( 1 << pos ); } }
	bool					GetBit( unsigned short pos ) const { return ( flags >> pos ) & 1; }

	unsigned short			flags;		// flags, meaning set by each property
};

NiRegisterStream( NiProperty );

/*
================
NiProperty::NiProperty
================
*/
// FUNCTION: STBC 0x40DAD0 ??0NiProperty@@QAE@XZ @game/FlareController
inline NiProperty::NiProperty() {
	flags = 0;
}

// the properties attached to an NiAVObject (Gamebryo lineage typedef)
typedef NiTSimpleList<NiPointer<NiProperty> >	NiPropertyList;

#endif /* !__NIPROPERTY_H__ */
