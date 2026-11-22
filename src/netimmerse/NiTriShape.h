/*
===============================================================================

	NiTriShape.h

	A triangle list geometry object: an NiTriBasedGeom drawn from
	NiTriShapeData.

	Evidence: the vtable at 0x899374 (51 slots: NiTriBasedGeom's 49, then 2
	new); the constructors (0x7F2040, 0x7F20D0, 0x7F20F0) call
	NiTriBasedGeom's and add no members, the first creating the data from
	seven arguments; the NiRTTI (0x9A1BB8) is named "NiTriShape" with
	NiTriBasedGeom's as its base; CreateClone (0x7F3160) allocates 0xE4
	bytes. Slots 46-48 read the triangle list of the model data and the
	model or world vertices; slot 23 (0x7F30D0) builds the bounding box
	tree from them. The destructor is the implicit one: its copy (0x4E7D20,
	from a game unit, as GetRTTI's at 0x4E7D10) jumps straight to
	NiTriBasedGeom's without storing a vptr.

===============================================================================
*/

#ifndef __NITRISHAPE_H__
#define __NITRISHAPE_H__

#include "NiTriBasedGeom.h"
#include "NiTriShapeData.h"
#include "NiStreamMacros.h"

/*
================
NiTriShapeLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x7F31F0) allocates 0x2C bytes and stores the vptr
0x88B910 after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28; its destructor (0x456A80) frees the array.
The copies the record's vtable (0x88B910) keeps come from TGFuzzyTriShape's
unit, the first in link order to create the record.
================
*/
// FUNCTION: STBC 0x456A60 ??_GNiTriShapeLoadRecord@@UAEPAXI@Z @game/TGFuzzyTriShape
// FUNCTION: STBC 0x456A80 ??1NiTriShapeLoadRecord@@UAE@XZ @game/TGFuzzyTriShape
class NiTriShapeLoadRecord : public NiTriBasedGeomLoadRecord {
};

// FUNCTION: STBC 0x007F35E0 ??_GNiTriShape@@UAEPAXI@Z
class NiTriShape : public NiTriBasedGeom {
public:
	// FUNCTION: STBC 0x4E7D10 ?GetRTTI@NiTriShape@@UBEPBVNiRTTI@@XZ @netimmerse/NiTriShape
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );


	// builds the bounding box tree
	virtual void			VirtualSlot23( int unknown );

	// TODO identity: slot 31 draws the shape through the renderer at the
	// argument's 0x188 and counts the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const;

	// the model data's normals from its triangles (the out-of-line copy the
	// linker kept, 0x456F60, comes from a game unit)
	// FUNCTION: STBC 0x456F60 ?CalculateNormals@NiTriShape@@UAEXXZ @game/TGFuzzyTriShape
	virtual void			CalculateNormals() { GetShapeData()->CalculateNormals(); }

	// the members VertexCompare compares (Gamebryo 1.2.2:
	// NiGeometryData::VERTEX_MASK and the rest)
	enum {
		VERTEX_MASK		= 0x01,
		NORMAL_MASK		= 0x02,
		COLOR_MASK		= 0x04,
		TEXTURE_MASK	= 0x08
	};

	// Gamebryo 1.2.2: NiGeometryData::VertexCompare, there on the data and
	// not virtual: compares two vertices member by member, over the members
	// the mask selects; the first difference as memcmp reports it, else 0
	virtual int				VertexCompare( unsigned short index0, unsigned short index1, unsigned char mask );
	// Gamebryo 1.2.2: NiOptimize::ReallocateVertexArrays, there a tool
	// function: moves the vertices to the positions the map gives, into
	// arrays of newVertexCount; a map entry of -1 drops the vertex
	virtual void			ReallocateVertexArrays( unsigned short newVertexCount, int *map );

	// the vertex weld; Gamebryo 1.2.2 has these as NiOptimize tool functions
	// (CollapseVertices, RemoveUnusedVertices, RemoveDegenerateTriangles,
	// NonDegenerateTriangle) and NiGeometryData members (IndexSort,
	// IndexQSort, ChoosePivot). CollapseVertices merges the vertices
	// VertexCompare finds equal, texture coordinates within the tolerance
	// when there is one; Nebula::CreateSphere and MetaVolume call it after
	// building their unshared vertices.
	bool					NonDegenerateTriangle( unsigned short first );
	bool					RemoveDegenerateTriangles();
	void					CollapseVertices( unsigned short **newToOld );
	void					IndexSort( unsigned short *&index0, unsigned short *&index1, unsigned char mask );
	void					IndexQSort( unsigned short *index, unsigned char mask, int l, int r );
	int						ChoosePivot( unsigned short *index, unsigned char mask, int l, int r );
	void					RemoveUnusedVertices( unsigned short **newToOld );

	// the texture coordinate distance under which CollapseVertices merges
	// two vertices, and its square (names invented; nothing in retail
	// writes them, so the texture coordinates must match exactly)
	static float			textureTolerance;
	static float			textureToleranceSquared;

							NiTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ );
							NiTriShape( NiTriShapeData *data );
							NiTriShape();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the model data as what it is (name invented); game code reads it
	// (0x73C4D0 and 0x740C30)
	NiTriShapeData *		GetShapeData() const { return (NiTriShapeData *)(NiGeometryData *)modelData; }

	// the model data's triangle list (Gamebryo lineage names)
	unsigned int			GetTriListLength() const;
	unsigned short *		GetTriList() const;

protected:
	void					CopyMembers( NiTriShape *dest );
};

NiRegisterStream( NiTriShape );

/*
================
NiTriShape::GetTriListLength
================
*/
inline unsigned int NiTriShape::GetTriListLength() const {
	NiTriShapeData *data = (NiTriShapeData *)(NiGeometryData *)modelData;
	return data->GetTriListLength();
}

/*
================
NiTriShape::GetTriList
================
*/
inline unsigned short *NiTriShape::GetTriList() const {
	NiTriShapeData *data = (NiTriShapeData *)(NiGeometryData *)modelData;
	return data->GetTriList();
}

#endif /* !__NITRISHAPE_H__ */
