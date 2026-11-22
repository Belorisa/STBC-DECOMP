/*
===============================================================================

	NiTriangles.h

	A triangle list geometry object without an index list: an
	NiTriBasedGeom drawn from NiTrianglesData, three consecutive vertices to
	each triangle.

	Evidence: the static initializer at 0x80B880 builds the NiRTTI at
	0x9A2598 ("NiTriangles", based on NiTriBasedGeom's); the vtable at
	0x89A34C has NiTriBasedGeom's 49 slots and adds none (the load record's
	follows at 0x89A410); the constructors (0x80BA10, 0x80BAA0, 0x80BAC0)
	call NiTriBasedGeom's and add no members, the first creating the data
	from five arguments; CreateClone (0x80BD60) allocates 0xE4 bytes. Slot
	48 (0x80BAE0) gives triangle i the vertices 3i to 3i+2, and slot 23
	builds the bounding box tree over that implicit list.

===============================================================================
*/

#ifndef __NITRIANGLES_H__
#define __NITRIANGLES_H__

#include "NiTriBasedGeom.h"
#include "NiTrianglesData.h"
#include "NiStreamMacros.h"

/*
================
NiTrianglesLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x80BDE0) allocates 0x2C bytes and stores the vptr
0x89A410 after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28.
================
*/
// FUNCTION: STBC 0x80BEC0 ??_GNiTrianglesLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80BEE0 ??1NiTrianglesLoadRecord@@UAE@XZ
class NiTrianglesLoadRecord : public NiTriBasedGeomLoadRecord {
};

// FUNCTION: STBC 0x0080C210 ??_GNiTriangles@@UAEPAXI@Z
class NiTriangles : public NiTriBasedGeom {
public:
	// FUNCTION: STBC 0x80C200 ?GetRTTI@NiTriangles@@UBEPBVNiRTTI@@XZ
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

	// builds the bounding box tree
	virtual void			VirtualSlot23( int unknown );

	// TODO identity: slot 31 draws the triangles through the renderer at the
	// argument's 0x188 and counts the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const;

							NiTriangles( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ );
							NiTriangles( NiTrianglesData *data );
							NiTriangles();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriangles *dest );

	// the model data as what it is (name invented)
	NiTrianglesData *		GetTrianglesData() const { return (NiTrianglesData *)(NiGeometryData *)modelData; }
};

NiRegisterStream( NiTriangles );

#endif /* !__NITRIANGLES_H__ */
