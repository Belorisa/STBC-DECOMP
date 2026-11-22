/*
===============================================================================

	NiTriStrip.h

	A single triangle strip geometry object: an NiTriBasedGeom drawn from
	NiTriStripData, of which the triangle at each index takes the next
	three strip vertices, every other one wound the other way.

	Evidence: the static initializer at 0x80DA00 builds the NiRTTI at
	0x9A2688 ("NiTriStrip", based on NiTriBasedGeom's); the vtable at
	0x89A510 has NiTriBasedGeom's 49 slots and adds none (the load record's
	follows at 0x89A5D4); the constructors (0x80DB90, 0x80DC20, 0x80DC40)
	call NiTriBasedGeom's and add no members, the first creating the data
	from five arguments; CreateClone (0x80DF10) allocates 0xE4 bytes. The
	destructor is the implicit one (0x80E3E0 jumps to NiTriBasedGeom's).

===============================================================================
*/

#ifndef __NITRISTRIP_H__
#define __NITRISTRIP_H__

#include "NiTriBasedGeom.h"
#include "NiTriStripData.h"
#include "NiStreamMacros.h"

/*
================
NiTriStripLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x80DF90) allocates 0x2C bytes and stores the vptr
0x89A5D4 after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28; its destructor (0x80E090) frees the array.
================
*/
// FUNCTION: STBC 0x80E070 ??_GNiTriStripLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80E090 ??1NiTriStripLoadRecord@@UAE@XZ
class NiTriStripLoadRecord : public NiTriBasedGeomLoadRecord {
};

// FUNCTION: STBC 0x80E3C0 ??_GNiTriStrip@@UAEPAXI@Z
// FUNCTION: STBC 0x80E3E0 ??1NiTriStrip@@UAE@XZ
class NiTriStrip : public NiTriBasedGeom {
public:
	// FUNCTION: STBC 0x80E3B0 ?GetRTTI@NiTriStrip@@UBEPBVNiRTTI@@XZ
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

	// builds the bounding box tree from the strip's triangle list
	virtual void			VirtualSlot23( int unknown );

	// TODO identity: slot 31 draws the strip through the renderer at the
	// argument's 0x188 and counts the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const;

	// the strip's vertices: two more than its triangles
							NiTriStrip( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ );
							NiTriStrip( NiTriStripData *data );
							NiTriStrip();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriStrip *dest );

	// the model data as what it is (name invented)
	NiTriStripData *		GetStripData() const { return (NiTriStripData *)(NiGeometryData *)modelData; }
};

NiRegisterStream( NiTriStrip );

#endif /* !__NITRISTRIP_H__ */
