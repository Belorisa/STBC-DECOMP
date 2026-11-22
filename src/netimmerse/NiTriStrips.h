/*
===============================================================================

	NiTriStrips.h

	A triangle strip geometry object: an NiTriBasedGeom drawn from
	NiTriStripsData.

	Evidence: the static initializer at 0x80EBF0 builds the NiRTTI at
	0x9A2708 ("NiTriStrips", based on NiTriBasedGeom's); the vtable at
	0x89A624 has NiTriBasedGeom's 49 slots and adds none (the load record's
	follows at 0x89A6E8); the constructors (0x80ED80, 0x80EE20, 0x80EE40)
	call NiTriBasedGeom's and add no members, the first creating the data
	from nine arguments; CreateClone (0x80F1A0) allocates 0xE4 bytes. Slots
	46 and 47 take a triangle's vertex indices from slot 48 (0x80EF80),
	which walks the strips.

===============================================================================
*/

#ifndef __NITRISTRIPS_H__
#define __NITRISTRIPS_H__

#include "NiTriBasedGeom.h"
#include "NiTriStripsData.h"
#include "NiStreamMacros.h"

/*
================
NiTriStripsLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x80F220) allocates 0x2C bytes and stores the vptr
0x89A6E8 after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28.
================
*/
// FUNCTION: STBC 0x80F300 ??_GNiTriStripsLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80F320 ??1NiTriStripsLoadRecord@@UAE@XZ
class NiTriStripsLoadRecord : public NiTriBasedGeomLoadRecord {
};

// FUNCTION: STBC 0x0080F5D0 ??_GNiTriStrips@@UAEPAXI@Z
class NiTriStrips : public NiTriBasedGeom {
public:
	// FUNCTION: STBC 0x80F5C0 ?GetRTTI@NiTriStrips@@UBEPBVNiRTTI@@XZ
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

	// strips build no bounding box tree
	virtual void			VirtualSlot23( int unknown );

	// TODO identity: slot 31 draws the strips through the renderer at the
	// argument's 0x188 and counts the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

	// the model data's normals from its strips
	virtual void			CalculateNormals();

	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const;

							NiTriStrips( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short stripCount_, unsigned short *stripLengths_, unsigned short *stripLists_ );
							NiTriStrips( NiTriStripsData *data );
							NiTriStrips();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriStrips *dest );
};

NiRegisterStream( NiTriStrips );

#endif /* !__NITRISTRIPS_H__ */
