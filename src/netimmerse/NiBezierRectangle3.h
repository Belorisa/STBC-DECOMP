/*
===============================================================================

	NiBezierRectangle3.h

	A rectangular Bezier patch of degree 3: sixteen control points.

	Evidence: the NiRTTI (0x9B3268, built at 0x849310) is named
	"NiBezierRectangle3" with NiBezierRectangle's as its base; the vtable at
	0x89AECC has NiBezierRectangle's 51 slots and overrides 0, 1, 4-8, 10, 43
	and 46. The constructors (0x8494A0, 0x849510) pass degree 3 to
	NiBezierRectangle's; the creation function (0x84C740) allocates 0x178
	bytes and a 0x28-byte load record of its own (vtable 0x89AF98).
	LoadBinary and the clone tessellate at the patch's level unless the
	patch owns unknownE4.

===============================================================================
*/

#ifndef __NIBEZIERRECTANGLE3_H__
#define __NIBEZIERRECTANGLE3_H__

#include "NiBezierRectangle.h"
#include "NiStreamMacros.h"

class NiBezierRectangle2;

/*
================
NiBezierRectangle3LoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x84C740) allocates 0x28 bytes and stores the vptr
0x89AF98 after the property link ID array at 0x10; its destructor
(0x84C840) frees the array.
================
*/
// FUNCTION: STBC 0x84C820 ??_GNiBezierRectangle3LoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x84C840 ??1NiBezierRectangle3LoadRecord@@UAE@XZ
class NiBezierRectangle3LoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x8494E0 ??_GNiBezierRectangle3@@UAEPAXI@Z
// FUNCTION: STBC 0x849500 ??1NiBezierRectangle3@@UAE@XZ
class NiBezierRectangle3 : public NiBezierRectangle {
public:
	// FUNCTION: STBC 0x8494D0 ?GetRTTI@NiBezierRectangle3@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			VirtualSlot43( float, float );
	virtual bool			VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short );

							NiBezierRectangle3( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierRectangle3( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierRectangle3() {}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// TODO identity: a new biquadratic patch with the same surface (the
	// edges' midpoints kept); the texture coordinates and colours are taken
	// from the patch evaluated at the new control parameters (nothing in
	// retail calls it)
	NiBezierRectangle2 *	Unknown84ACF0();

	static const NiRTTI		rtti;

protected:
	// the clone is tessellated at its level unless it owns unknownE4
	void					CopyMembers( NiBezierRectangle3 *dest );
};

NiRegisterStream( NiBezierRectangle3 );

#endif /* !__NIBEZIERRECTANGLE3_H__ */
