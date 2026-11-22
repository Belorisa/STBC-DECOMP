/*
===============================================================================

	TGFuzzyTriShape.h

	A triangle shape with a fuzziness value per vertex. 0xE8 bytes.

	Evidence: the NiRTTI at 0x980AE8 is built with the name
	"TGFuzzyTriShape" (0x8DAEE8) and NiTriShape's NiRTTI as its base
	(0x4566F0); the constructor (0x4568E0) calls NiTriShape's over the data,
	clears the pointer at 0xE4 and stores the vtable 0x88B844: NiTriShape's
	51 slots with GetRTTI (0x456F70), CreateClone (0x456910), the five stream
	slots (0x456AB0-0x456AF0, each forwarding to NiTriShape's), the
	destructor (0x456F80) and slot 35 (0x456B70) replaced. Its unit lies
	between 0x4566E0 and 0x456F90 and has no source yet: only what the nebula
	classes use is declared, and the member and function names other than
	the class's are invented.

===============================================================================
*/

#ifndef __TGFUZZYTRISHAPE_H__
#define __TGFUZZYTRISHAPE_H__

#include "NiTriShape.h"

class NiCamera;

// FUNCTION: STBC 0x00456f80 ??_GTGFuzzyTriShape@@UAEPAXI@Z
class TGFuzzyTriShape : public NiTriShape {
public:
	virtual const NiRTTI *	GetRTTI() const;

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~TGFuzzyTriShape();

	virtual void			VirtualSlot35( NiCamera *camera );

							TGFuzzyTriShape( NiTriShapeData *data );
	// NiTriShape's over new data made of the arrays (0x456890; the
	// DisruptorGraphic constructor calls it)
							TGFuzzyTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_,
											 NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ );

	// frees the values and allocates one per vertex, 1 each (0x456B00)
	void					AllocateFuzziness();

	static const NiRTTI		rtti;

	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

protected:
	void					CopyMembers( TGFuzzyTriShape *dest );

protected:
	float *					fuzziness;			// 0xE4 one per vertex, or NULL; owned
};

/*
================
TGFuzzyTriShape::~TGFuzzyTriShape

Inline: TGFuzzyClusterGeom's destructor (0x599350) expands it; the
out-of-line copy comes from the class's own unit.
================
*/
// FUNCTION: STBC 0x00456fa0 ??1TGFuzzyTriShape@@UAE@XZ @game/TGFuzzyTriShape
inline TGFuzzyTriShape::~TGFuzzyTriShape() {
	delete [] fuzziness;
}

#endif /* !__TGFUZZYTRISHAPE_H__ */
