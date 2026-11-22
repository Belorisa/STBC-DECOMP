/*
===============================================================================

	NiTriBasedGeomData.h

	Geometry data drawn as triangles: NiGeometryData with a triangle count.

	Evidence: the vtable at 0x89921C (18 slots: NiGeometryData's 16, slot 14
	still _purecall, then 2 new); the constructor (0x7EEC70) passes five
	arguments on to NiGeometryData's and stores the sixth at 0x34; the
	destructor (0x7EECD0) adds nothing; the NiRTTI (0x9A1AD0) is named
	"NiTriBasedGeomData" with NiGeometryData's as its base; the viewer
	strings name the count ("m_usTriangles"). Slot 17 returns the count and
	slot 16 ignores its unsigned short, as NiGeometryData's slots 13 and 12
	do for the vertices. The names follow the Gamebryo lineage
	(m_usTriangles, Set/GetActiveTriangleCount).

===============================================================================
*/

#ifndef __NITRIBASEDGEOMDATA_H__
#define __NITRIBASEDGEOMDATA_H__

#include "NiGeometryData.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x007EF050 ??_GNiTriBasedGeomData@@UAEPAXI@Z
class NiTriBasedGeomData : public NiGeometryData {
public:
	// FUNCTION: STBC 0x007EF040 ?GetRTTI@NiTriBasedGeomData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiTriBasedGeomData();


	// the triangles in use
	// FUNCTION: STBC 0x007EF080 ?SetActiveTriangleCount@NiTriBasedGeomData@@UAEXG@Z
	virtual void			SetActiveTriangleCount( unsigned short count ) {}
	// FUNCTION: STBC 0x007EF090 ?GetActiveTriangleCount@NiTriBasedGeomData@@UBEGXZ
	virtual unsigned short	GetActiveTriangleCount() const { return triangleCount; }

							NiTriBasedGeomData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_ );
							NiTriBasedGeomData();

	unsigned short			GetTriangleCount() const;
	void					SetTriangleCount( unsigned short count );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriBasedGeomData *dest );

	unsigned short			triangleCount;
};

NiRegisterStream( NiTriBasedGeomData );

/*
================
NiTriBasedGeomData::GetTriangleCount
================
*/
inline unsigned short NiTriBasedGeomData::GetTriangleCount() const {
	return triangleCount;
}

/*
================
NiTriBasedGeomData::SetTriangleCount
================
*/
inline void NiTriBasedGeomData::SetTriangleCount( unsigned short count ) {
	triangleCount = count;
}

#endif /* !__NITRIBASEDGEOMDATA_H__ */
