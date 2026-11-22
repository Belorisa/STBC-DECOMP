/*
===============================================================================

	NiAlphaProperty.h

	Alpha blending and alpha testing: the blend factors, the test function
	and the test reference, packed in NiProperty's flags.

	Evidence: the vtable at 0x888A08 (NiProperty's 14 slots, none added;
	Type returns 0); the creation function (0x7EC3C0) allocates 0x1C
	bytes; the constructor sets source SRCALPHA in bits 1-4, destination
	INVSRCALPHA in bits 5-8 (flags 0xEC) and clears the test reference, a
	byte at 0x18; IsEqual (0x7EC540) compares bit 0, bits 1-4, bits 5-8,
	bit 9, bits 10-12 and the reference; the viewer strings name
	"m_bAlpha", "m_srcBlend", "m_destBlend" and the eleven ALPHA_ modes.
	The renderer reads the test enable (0x200) and the test function
	(bits 10-12). The NiRTTI is 0x9A1A10, named "NiAlphaProperty".

===============================================================================
*/

#ifndef __NIALPHAPROPERTY_H__
#define __NIALPHAPROPERTY_H__

#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiAlphaPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899120). The class name is invented.

Evidence: CreateObject (0x7EC3C0) clears the three link IDs before it
stores the vptr, the order a derived class's inline constructor leaves.
================
*/
// FUNCTION: STBC 0x007ec490 ??_GNiAlphaPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiAlphaProperty
class NiAlphaPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x40DB70 ??_GNiAlphaProperty@@UAEPAXI@Z @game/AnimTSParticleController
// FUNCTION: STBC 0x40DB90 ??1NiAlphaProperty@@UAE@XZ @game/AnimTSParticleController
class NiAlphaProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x40DB60 ?GetRTTI@NiAlphaProperty@@UBEPBVNiRTTI@@XZ @game/AnimTSParticleController
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

	// FUNCTION: STBC 0x40DBA0 ?Type@NiAlphaProperty@@UAEHXZ @game/AnimTSParticleController
	virtual int				Type() { return ALPHA; }
	static int				GetType() { return ALPHA; }

	// the blend factors, as the viewer strings name them
	enum AlphaFunction {
		ALPHA_ONE,
		ALPHA_ZERO,
		ALPHA_SRCCOLOR,
		ALPHA_INVSRCCOLOR,
		ALPHA_DESTCOLOR,
		ALPHA_INVDESTCOLOR,
		ALPHA_SRCALPHA,
		ALPHA_INVSRCALPHA,
		ALPHA_DESTALPHA,
		ALPHA_INVDESTALPHA,
		ALPHA_SRCALPHASAT,
		ALPHA_MAX_MODES
	};

	// the alpha test functions (Gamebryo lineage names), in the order of
	// NiD3DRenderState's table of D3D comparisons
	enum TestFunction {
		TEST_ALWAYS,
		TEST_LESS,
		TEST_EQUAL,
		TEST_LESSEQUAL,
		TEST_GREATER,
		TEST_NOTEQUAL,
		TEST_GREATEREQUAL,
		TEST_NEVER,
		TEST_MAX_MODES
	};

	// the property's type number (Type, and GetType for the class)
	enum {
		ALPHA				= 0
	};

							// FUNCTION: STBC 0x500970 ??0NiAlphaProperty@@QAE@XZ @game/MapWindow
							NiAlphaProperty() {
								SetAlphaBlending( false );
								SetSrcBlendMode( ALPHA_SRCALPHA );
								SetDestBlendMode( ALPHA_INVSRCALPHA );
								SetAlphaTesting( false );
								SetTestMode( TEST_ALWAYS );
								SetTestRef( 0 );
							}

	void					SetAlphaBlending( bool blend ) { SetBit( blend, ALPHA_BLEND_POS ); }
	bool					GetAlphaBlending() const { return GetBit( ALPHA_BLEND_POS ); }
	void					SetSrcBlendMode( AlphaFunction mode ) { SetField( (unsigned short)mode, SRC_BLEND_MASK, SRC_BLEND_POS ); }
	AlphaFunction			GetSrcBlendMode() const { return (AlphaFunction)GetField( SRC_BLEND_MASK, SRC_BLEND_POS ); }
	void					SetDestBlendMode( AlphaFunction mode ) { SetField( (unsigned short)mode, DEST_BLEND_MASK, DEST_BLEND_POS ); }
	AlphaFunction			GetDestBlendMode() const { return (AlphaFunction)GetField( DEST_BLEND_MASK, DEST_BLEND_POS ); }
	void					SetAlphaTesting( bool test ) { SetBit( test, TEST_ENABLE_POS ); }
	bool					GetAlphaTesting() const { return GetBit( TEST_ENABLE_POS ); }
	// the getter returns the enumeration (AnimOnly.exe 0x42C810)
	void					SetTestMode( TestFunction mode ) { SetField( (unsigned short)mode, TEST_FUNC_MASK, TEST_FUNC_POS ); }
	TestFunction			GetTestMode() const { return (TestFunction)GetField( TEST_FUNC_MASK, TEST_FUNC_POS ); }
	// objects drawn with the property skip the alpha sorter (Gamebryo
	// lineage name; NiAlphaAccumulator's RegisterObject tests it)
	void					SetNoSorter( bool noSorter ) { SetBit( noSorter, NO_SORTER_POS ); }
	bool					GetNoSorter() const { return GetBit( NO_SORTER_POS ); }
	void					SetTestRef( unsigned char ref ) { testRef = ref; }
	unsigned char			GetTestRef() const { return testRef; }

	// the alpha property of objects without one
	// FUNCTION: STBC 0x820B60 ?GetDefault@NiAlphaProperty@@SA?AV?$NiPointer@VNiAlphaProperty@@@@XZ
	static NiPointer<NiAlphaProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiAlphaProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiAlphaProperty *dest );

	// "<prefix> = ALPHA_..." in a new string that the caller deletes
	static char *			GetViewerString( const char *prefix, AlphaFunction mode );

	// the fields of the flags
	enum {
		ALPHA_BLEND_POS		= 0,
		SRC_BLEND_MASK		= 0x000F,
		SRC_BLEND_POS		= 1,
		DEST_BLEND_MASK		= 0x000F,
		DEST_BLEND_POS		= 5,
		TEST_ENABLE_POS		= 9,
		TEST_FUNC_MASK		= 0x0007,
		TEST_FUNC_POS		= 10,
		NO_SORTER_POS		= 13
	};

	unsigned char			testRef;	// the alpha test reference
};

NiRegisterStream( NiAlphaProperty );

#endif /* !__NIALPHAPROPERTY_H__ */
