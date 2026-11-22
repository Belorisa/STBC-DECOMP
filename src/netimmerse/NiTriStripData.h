/*
===============================================================================

	NiTriStripData.h

	Triangle data drawn as one triangle strip, of which only the first
	active triangles are drawn.

	Evidence: the NiRTTI (0x9A2638, initialised at 0x80CFC0) is named
	"NiTriStripData" with NiTriBasedGeomData's as its base; CreateObject
	(0x80D590) allocates 0x3C bytes, runs NiTriBasedGeomData's constructor,
	stores the vtable 0x89A4C4 (18 slots; the load record's follows) and
	clears the word at 0x38. Slot 16 (0x80D9D0) clamps its count to the
	triangle count into 0x38 and slot 17 returns it; slot 12 (0x80D980)
	stores two less than its vertex count (the triangle count from the
	whole strip, 0 under three vertices) and slot 13 returns two more than
	0x38 (0 without triangles). The renderer's slot 47 draws it. Member
	names follow the Gamebryo lineage (m_usActiveTriangles).

===============================================================================
*/

#ifndef __NITRISTRIPDATA_H__
#define __NITRISTRIPDATA_H__

#include "NiTriBasedGeomData.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiTriStripDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x89A50C). The class name is invented.
================
*/
// FUNCTION: STBC 0x80D630 ??_GNiTriStripDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80D650 ??1NiTriStripDataLoadRecord@@UAE@XZ
class NiTriStripDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x0080D940 ??_GNiTriStripData@@UAEPAXI@Z
class NiTriStripData : public NiTriBasedGeomData {
public:
	// FUNCTION: STBC 0x80D930 ?GetRTTI@NiTriStripData@@UBEPBVNiRTTI@@XZ
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

	// the vertices in use: two more than the active triangles
	virtual void			SetActiveVertexCount( unsigned short count );
	virtual unsigned short	GetActiveVertexCount() const;
	virtual void			CalculateNormals();

	// the triangles in use, at most the triangle count
	virtual void			SetActiveTriangleCount( unsigned short count );
	// FUNCTION: STBC 0x80D9F0 ?GetActiveTriangleCount@NiTriStripData@@UBEGXZ
	virtual unsigned short	GetActiveTriangleCount() const { return activeTriangleCount; }

	// the strip's vertices: two more than its triangles
							NiTriStripData( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ );
							NiTriStripData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriStripData *dest );

	unsigned short			activeTriangleCount;
};

NiRegisterStream( NiTriStripData );

#endif /* !__NITRISTRIPDATA_H__ */
