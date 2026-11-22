/*
===============================================================================

	NiBezierRectangle2.h

	A rectangular Bezier patch of degree 2: nine control points.

	Evidence: the NiRTTI (0x9B3218, built at 0x847FA0) is named
	"NiBezierRectangle2" with NiBezierRectangle's as its base; the vtable at
	0x89ADFC has NiBezierRectangle's 51 slots and overrides 0, 1, 4-8, 10, 43
	and 46. The constructors (0x848130, 0x848160) pass degree 2 to
	NiBezierRectangle's; the creation function (0x848FE0) allocates 0x178
	bytes and a 0x28-byte load record of its own (vtable 0x89AEC8).
	LoadBinary and the clone tessellate at the patch's level unless the
	patch owns unknownE4.

===============================================================================
*/

#ifndef __NIBEZIERRECTANGLE2_H__
#define __NIBEZIERRECTANGLE2_H__

#include "NiBezierRectangle.h"
#include "NiStreamMacros.h"

/*
================
NiBezierRectangle2LoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x848FE0) allocates 0x28 bytes and stores the vptr
0x89AEC8 after the property link ID array at 0x10; its destructor
(0x8490E0) frees the array.
================
*/
// FUNCTION: STBC 0x8490C0 ??_GNiBezierRectangle2LoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x8490E0 ??1NiBezierRectangle2LoadRecord@@UAE@XZ
class NiBezierRectangle2LoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x8492C0 ??_GNiBezierRectangle2@@UAEPAXI@Z
// FUNCTION: STBC 0x8492E0 ??1NiBezierRectangle2@@UAE@XZ
class NiBezierRectangle2 : public NiBezierRectangle {
public:
	// FUNCTION: STBC 0x8492B0 ?GetRTTI@NiBezierRectangle2@@UBEPBVNiRTTI@@XZ
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

							NiBezierRectangle2( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierRectangle2( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierRectangle2() {}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// the clone is tessellated at its level unless it owns unknownE4
	void					CopyMembers( NiBezierRectangle2 *dest );
};

NiRegisterStream( NiBezierRectangle2 );

#endif /* !__NIBEZIERRECTANGLE2_H__ */
