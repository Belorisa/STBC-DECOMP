/*
===============================================================================

	NiBezierTriangle4.h

	A triangular Bezier patch of degree 4: fifteen control points.

	Evidence: the NiRTTI (0x9B3160, built at 0x83D810) is named
	"NiBezierTriangle4" with NiBezierTriangle's as its base; the vtable at
	0x89AC30 has NiBezierTriangle's 50 slots and overrides 0, 1, 4-8, 10, 43
	and 46. The constructors (0x83D9A0, 0x83DA10) pass degree 4 to
	NiBezierTriangle's; the creation function (0x841F90) allocates 0x17C
	bytes and a 0x28-byte load record of its own (vtable 0x89AD24).
	LoadBinary and the clone tessellate at the patch's level unless the
	patch owns unknownE4.

===============================================================================
*/

#ifndef __NIBEZIERTRIANGLE4_H__
#define __NIBEZIERTRIANGLE4_H__

#include "NiBezierTriangle.h"
#include "NiStreamMacros.h"

class NiBezierTriangle2;
class NiBezierTriangle3;

/*
================
NiBezierTriangle4LoadRecord

The record CreateObject hands the stream: NiAVObject's link IDs only. The
class name is invented.

Evidence: CreateObject (0x841F90) allocates 0x28 bytes and stores the vptr
0x89AD24 after the property link ID array at 0x10; its destructor
(0x842090) frees the array.
================
*/
// FUNCTION: STBC 0x842070 ??_GNiBezierTriangle4LoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x842090 ??1NiBezierTriangle4LoadRecord@@UAE@XZ
class NiBezierTriangle4LoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x83D9E0 ??_GNiBezierTriangle4@@UAEPAXI@Z
// FUNCTION: STBC 0x83DA00 ??1NiBezierTriangle4@@UAE@XZ
class NiBezierTriangle4 : public NiBezierTriangle {
public:
	// FUNCTION: STBC 0x83D9D0 ?GetRTTI@NiBezierTriangle4@@UBEPBVNiRTTI@@XZ
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

							NiBezierTriangle4( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierTriangle4( unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierTriangle4() {}

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// TODO identity: new patches with the same surface, of degree 3 and of
	// degree 2 (least-squares fits of the edges); the texture coordinates
	// and colours are taken from the patch evaluated at the new control
	// parameters (nothing in retail calls them)
	NiBezierTriangle3 *		Unknown83EA40();
	NiBezierTriangle2 *		Unknown840F10();

	static const NiRTTI		rtti;

protected:
	// the clone is tessellated at its level unless it owns unknownE4
	void					CopyMembers( NiBezierTriangle4 *dest );
};

NiRegisterStream( NiBezierTriangle4 );

#endif /* !__NIBEZIERTRIANGLE4_H__ */
