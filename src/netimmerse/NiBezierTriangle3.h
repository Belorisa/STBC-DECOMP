/*
===============================================================================

	NiBezierTriangle3.h

	A triangular Bezier patch of degree 3: ten control points.

	Evidence: the NiRTTI (0x9B3120, built at 0x83A780) is named
	"NiBezierTriangle3" with NiBezierTriangle's as its base; the vtable at
	0x89AB64 has NiBezierTriangle's 50 slots and overrides 0, 1, 4-8, 10, 43
	and 46. The constructors (0x83A910, 0x83A940) pass degree 3 to
	NiBezierTriangle's; the creation function (0x83D4D0) allocates 0x17C
	bytes and a 0x28-byte load record of its own (vtable 0x89AC2C).
	LoadBinary and the clone tessellate at the patch's level unless the
	patch owns unknownE4.

===============================================================================
*/

#ifndef __NIBEZIERTRIANGLE3_H__
#define __NIBEZIERTRIANGLE3_H__

#include "NiBezierTriangle.h"
#include "NiStreamMacros.h"

class NiBezierTriangle2;
class NiBezierTriangle4;

/*
================
NiBezierTriangle3LoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x83D4D0) allocates 0x28 bytes and stores the vptr
0x89AC2C after the property link ID array at 0x10; its destructor
(0x83D5D0) frees the array.
================
*/
// FUNCTION: STBC 0x83D5B0 ??_GNiBezierTriangle3LoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x83D5D0 ??1NiBezierTriangle3LoadRecord@@UAE@XZ
class NiBezierTriangle3LoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x83D7C0 ??_GNiBezierTriangle3@@UAEPAXI@Z
// FUNCTION: STBC 0x83D7E0 ??1NiBezierTriangle3@@UAE@XZ
class NiBezierTriangle3 : public NiBezierTriangle {
public:
	// FUNCTION: STBC 0x83D7B0 ?GetRTTI@NiBezierTriangle3@@UBEPBVNiRTTI@@XZ
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

							NiBezierTriangle3( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierTriangle3( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierTriangle3() {}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// TODO identity: new patches with the same surface, of degree 2 (the
	// edges' midpoints kept) and of degree 4; the texture coordinates and
	// colours are taken from the patch evaluated at the new control
	// parameters (nothing in retail calls them)
	NiBezierTriangle2 *		Unknown83B370();
	NiBezierTriangle4 *		Unknown83BFB0();

	static const NiRTTI		rtti;

protected:
	// the clone is tessellated at its level unless it owns unknownE4
	void					CopyMembers( NiBezierTriangle3 *dest );
};

NiRegisterStream( NiBezierTriangle3 );

#endif /* !__NIBEZIERTRIANGLE3_H__ */
