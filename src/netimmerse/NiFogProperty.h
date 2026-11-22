/*
===============================================================================

	NiFogProperty.h

	The fog that covers objects with distance: on/off and the fog function
	packed in NiProperty's flags, the depth, and the colour.

	Evidence: the vtable at 0x888998 (NiProperty's 14 slots, none added;
	Type returns 2); CreateClone (0x7FDB90) allocates 0x28 bytes, sets
	the float at 0x18 to 1.0 and clears the colour at 0x1C; the renderer
	reads the fog bit (0x2) and the function (bits 3-4). The NiRTTI is
	0x9A20C8, named "NiFogProperty".

===============================================================================
*/

#ifndef __NIFOGPROPERTY_H__
#define __NIFOGPROPERTY_H__

#include "NiColor.h"
#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiFogPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899D3C). The class name is invented.
================
*/
// FUNCTION: STBC 0x007fdd40 ??_GNiFogPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiFogProperty
class NiFogPropertyLoadRecord : public NiObjectNETLoadRecord {
};

// FUNCTION: STBC 0x004127c0 ??_GNiFogProperty@@UAEPAXI@Z @netimmerse/NiFogProperty
class NiFogProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x4127B0 ?GetRTTI@NiFogProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x412810 ?Type@NiFogProperty@@UAEHXZ
	virtual int				Type() { return FOG; }
	static int				GetType() { return FOG; }

	enum {
		FOG					= 2
	};

	// the fields of the flags
	enum {
		FOG_ENABLE_MASK		= 0x0002,
		FOG_ENABLE_POS		= 1,
		FOG_FUNCTION_MASK	= 0x0018,
		FOG_FUNCTION_POS	= 3
	};

	// the fog functions, stored shifted into the flags (Gamebryo lineage
	// names). Evidence: the viewer strings (0x7FDE70) switch on the
	// shifted-down field; the renderer's test of the masked bits against
	// 8 is the compiler's folding of the shift.
	enum FogFunction {
		FOG_Z_LINEAR,
		FOG_RANGE_SQ,
		FOG_VERTEX_ALPHA
	};

							NiFogProperty() {
								SetFog( false );
								depth = 1.0f;
								SetFogFunction( FOG_Z_LINEAR );
								color = NiColor( 0.0f, 0.0f, 0.0f );
							}

	// accessors (Gamebryo lineage names)
	void					SetFog( bool fog ) { SetBit( fog, FOG_ENABLE_POS ); }
	void					SetFogFunction( FogFunction function ) { SetField( (unsigned short)function, FOG_FUNCTION_MASK >> FOG_FUNCTION_POS, FOG_FUNCTION_POS ); }
	bool					GetFog() const { return ( flags & FOG_ENABLE_MASK ) != 0; }
	FogFunction				GetFogFunction() const { return (FogFunction)( ( flags & FOG_FUNCTION_MASK ) >> FOG_FUNCTION_POS ); }
	void					SetDepth( float depth_ ) { depth = depth_; }
	float					GetDepth() const { return depth; }
	const NiColor &			GetFogColor() const { return color; }
	void					SetFogColor( const NiColor &color_ ) { color = color_; }

	// the fog property of objects without one
	// FUNCTION: STBC 0x820BC0 ?GetDefault@NiFogProperty@@SA?AV?$NiPointer@VNiFogProperty@@@@XZ
	static NiPointer<NiFogProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiFogProperty>	defaultProperty;	// name invented

protected:
	// "<prefix> = <function>" in a new string that the caller deletes
	static char *			GetViewerString( const char *prefix, FogFunction function );

	void					CopyMembers( NiFogProperty *dest );
	float					depth;		// 1.0 after construction
	NiColor					color;
};

NiRegisterStream( NiFogProperty );

#endif /* !__NIFOGPROPERTY_H__ */
