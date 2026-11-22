/*
===============================================================================

	NiTrianglesData.h

	Triangle data drawn as a plain triangle list, three vertices to a
	triangle, of which only the first active ones are drawn.

	Evidence: the NiRTTI (0x9A2540, initialised at 0x80B130) is named
	"NiTrianglesData" with NiTriBasedGeomData's as its base; CreateObject
	(0x80B4B0) allocates 0x3C bytes, runs NiTriBasedGeomData's constructor,
	stores the vtable 0x89A300 (18 slots; the load record's follows) and
	clears the word at 0x38. Slot 16 (0x80B810) clamps its count to the
	triangle count into 0x38 and slot 17 returns it; slot 12 passes a third
	of its vertex count to slot 16 and slot 13 returns three times 0x38.
	The renderer's slot 46 draws it. Member names follow the Gamebryo
	lineage (m_usActiveTriangles); the class has no counterpart there.

===============================================================================
*/

#ifndef __NITRIANGLESDATA_H__
#define __NITRIANGLESDATA_H__

#include "NiTriBasedGeomData.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiTrianglesDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x89A348). The class name is invented.
================
*/
// FUNCTION: STBC 0x80B550 ??_GNiTrianglesDataLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x80B570 ??1NiTrianglesDataLoadRecord@@UAE@XZ
class NiTrianglesDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x0080B7D0 ??_GNiTrianglesData@@UAEPAXI@Z
class NiTrianglesData : public NiTriBasedGeomData {
public:
	// FUNCTION: STBC 0x80B7C0 ?GetRTTI@NiTrianglesData@@UBEPBVNiRTTI@@XZ
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

	// the vertices in use: three to an active triangle
	virtual void			SetActiveVertexCount( unsigned short count );
	virtual unsigned short	GetActiveVertexCount() const;
	virtual void			CalculateNormals();

	// the triangles in use, at most the triangle count
	virtual void			SetActiveTriangleCount( unsigned short count );
	// FUNCTION: STBC 0x80B830 ?GetActiveTriangleCount@NiTrianglesData@@UBEGXZ
	virtual unsigned short	GetActiveTriangleCount() const { return activeTriangleCount; }

	// three vertices to each triangle
							NiTrianglesData( NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ );
							NiTrianglesData();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTrianglesData *dest );

	unsigned short			activeTriangleCount;
};

NiRegisterStream( NiTrianglesData );

#endif /* !__NITRIANGLESDATA_H__ */
