/*
===============================================================================

	NiBezierTriangle2.h

	A triangular Bezier patch of degree 2: six control points.

	Evidence: the NiRTTI (0x9B30D8, built at 0x838E60) is named
	"NiBezierTriangle2" with NiBezierTriangle's as its base; the vtable at
	0x89AA94 has NiBezierTriangle's 50 slots and overrides 0, 1, 4-8, 10, 43
	and 46. The constructors (0x838FF0, 0x839020) pass degree 2 to
	NiBezierTriangle's; the creation function (0x83A330) allocates 0x17C
	bytes and a 0x28-byte load record of its own (vtable 0x89AB60).
	LoadBinary and the clone tessellate at the patch's level unless the
	patch owns unknownE4.

===============================================================================
*/

#ifndef __NIBEZIERTRIANGLE2_H__
#define __NIBEZIERTRIANGLE2_H__

#include "NiBezierTriangle.h"
#include "NiStreamMacros.h"

class NiBezierTriangle3;

/*
================
NiBezierTriangle2LoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x83A330) allocates 0x28 bytes and stores the vptr
0x89AB60 after the property link ID array at 0x10; its destructor
(0x83A430) frees the array.
================
*/
// FUNCTION: STBC 0x83A410 ??_GNiBezierTriangle2LoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x83A430 ??1NiBezierTriangle2LoadRecord@@UAE@XZ
class NiBezierTriangle2LoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x83A720 ??_GNiBezierTriangle2@@UAEPAXI@Z
// FUNCTION: STBC 0x83A740 ??1NiBezierTriangle2@@UAE@XZ
class NiBezierTriangle2 : public NiBezierTriangle {
public:
	// FUNCTION: STBC 0x83A710 ?GetRTTI@NiBezierTriangle2@@UBEPBVNiRTTI@@XZ
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

							NiBezierTriangle2( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierTriangle2( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierTriangle2() {}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// TODO identity: a new cubic patch with the same surface: the control
	// points raised to degree 3, the texture coordinates and colours taken
	// from the patch evaluated at the cubic's control parameters (nothing
	// in retail calls it)
	NiBezierTriangle3 *		Unknown8395C0();

	static const NiRTTI		rtti;

protected:
	// the clone is tessellated at its level unless it owns unknownE4
	void					CopyMembers( NiBezierTriangle2 *dest );
};

NiRegisterStream( NiBezierTriangle2 );

#endif /* !__NIBEZIERTRIANGLE2_H__ */
