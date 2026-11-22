/*
===============================================================================

	DamageMesh.h

	The growable triangle meshes the damage geometry is built into:
	vertices with optional texture coordinates, normals and colours, and
	triangles of three vertex indices. Each mesh starts from caller-supplied
	arrays and continues in blocks of 32 vertices or 32 triangles kept in
	arrays.

	No string of the game names these classes. The two meshes' names are
	from DamageTool.exe (the BC SDK's damage tool, built with RTTI and a
	COFF symbol table): its ??0SimpleFeatureCollector(bool, bool, bool)
	(0x40C3A0) and ??0FeatureCollector(bool, bool, bool) (0x40C8D0, with
	FeatureCollector's one-slot vtable, which its RTTI names) have the
	instructions of 0x4B27F0 and 0x4B2D40, and so do their destructors,
	Reset, SetBaseTriBuffer, PushTri and PackTris against the methods here.
	There the vertex blocks are ??0VertPool(bool, bool, bool) (0x40C300, the
	instructions of 0x4B2750) held in an NiTArray<VertPool *>, and the
	triangle blocks are TriPool * in an NiTArray<TriPool *>; DamageVertexBlock
	keeps its invented name until the NiTArray annotations in
	src/netimmerse are renamed with it.

	Evidence: DamageVertexBlock is 0x10 bytes, built by 0x4B2750 (32
	vertices, and 32 texture coordinates, normals and colours when asked
	for). SimpleFeatureCollector is 0x54 bytes without a vtable, built by
	0x4B27F0 and destroyed by 0x4B28A0; MetaSurfacePolygonizer's statics
	(0x4B35C0) build three. FeatureCollector is 0x58 bytes with a
	vtable (0x88C61C, its deleting destructor only), built by 0x4B2D40 and
	destroyed by 0x4B2E20; the polygonizer's statics (0x4BCC70) build one.
	The two keep the same members in a different order.

===============================================================================
*/

#ifndef __DAMAGEMESH_H__
#define __DAMAGEMESH_H__

#include "NiColor.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiTArray.h"

// the vertices of a mesh past its first arrays, 32 to a block
struct DamageVertexBlock {
	enum {
		SIZE				= 32
	};

	NiPoint3 *			vertices;			// 0x00 owned
	NiPoint2 *			textures;			// 0x04 owned, or NULL
	NiPoint3 *			normals;			// 0x08 owned, or NULL
	NiColorA *			colors;				// 0x0C owned, or NULL

						DamageVertexBlock( bool hasTextures, bool hasNormals, bool hasColors );
						~DamageVertexBlock() { delete [] vertices; delete [] textures; delete [] normals; delete [] colors; }
};

/*
================
SimpleFeatureCollector

The mesh MetaSurfacePolygonizer builds into. The first arrays start empty.
================
*/
class SimpleFeatureCollector {
public:
						SimpleFeatureCollector( bool hasTextures_, bool hasNormals_, bool hasColors_ );
						~SimpleFeatureCollector();

	// the counts (names invented): inline accessors, as their unsigned short
	// results show where MetaSurfacePolygonizer::CollectFeatures widens each
	// count once from a word register ( mov si, [eax+6]; and esi, 0xffff ),
	// which a member read into an int gives as xor; mov
	unsigned short		GetVertexCount() const { return vertexCount; }
	unsigned short		GetTriangleCount() const { return triangleCount; }
	// forgets the counts and the first arrays
	void				Reset();
	// frees the blocks
	void				FreeBlocks();
	// the first arrays: capacity vertices, empty
	void				SetVertexArrays( unsigned short capacity, NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_,
										 NiColorA *colors_ );
	void				SetTriangleArray( unsigned short capacity, unsigned short *triangles_ );
	// appends a vertex, in the first arrays while they have room and then in
	// the blocks; the optional values may be NULL; returns its index
	unsigned short		AddVertex( const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal, const NiColorA *color );
	// appends a triangle; returns its index
	unsigned short		AddTriangle( unsigned short corner0, unsigned short corner1, unsigned short corner2 );
	// copies every vertex out, the first arrays then the blocks; the
	// optional arrays may be NULL
	void				GetVertices( NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_, NiColorA *colors_ );
	// copies every triangle's three indices out
	void				GetTriangles( unsigned short *triangles_ );

	bool				hasTextures;		// 0x00
	bool				hasNormals;			// 0x01
	bool				hasColors;			// 0x02
	unsigned short		vertexCount;		// 0x04
	unsigned short		triangleCount;		// 0x06
	unsigned short		vertexCapacity;		// 0x08 the first arrays' vertices
	NiPoint3 *			vertices;			// 0x0C
	NiPoint3 *			normals;			// 0x10
	NiPoint2 *			textures;			// 0x14
	NiColorA *			colors;				// 0x18
	unsigned short		triangleCapacity;	// 0x1C the first array's triangles
	unsigned short *	triangles;			// 0x20 three indices each
	NiTArray<DamageVertexBlock *>	vertexBlocks;	// 0x24 owned
	NiTArray<unsigned short *>	triangleBlocks;		// 0x3C owned, 32 triangles each
};

/*
================
FeatureCollector

The mesh the polygonizer builds into. The first arrays come full.
================
*/
// FUNCTION: STBC 0x004b2e00 ??_GFeatureCollector@@UAEPAXI@Z @game/DamageMesh
class FeatureCollector {
public:
						FeatureCollector( bool hasTextures_, bool hasNormals_, bool hasColors_ );
	virtual				~FeatureCollector();

	// the first arrays: capacity vertices, all in use
	void				SetVertexArrays( unsigned short capacity, NiPoint3 *vertices_, NiPoint3 *normals_, NiPoint2 *textures_,
										 NiColorA *colors_ );
	void				SetTriangleArray( unsigned short capacity, unsigned short *triangles_ );
	// overwrites a vertex; the optional values may be NULL
	void				SetVertex( unsigned short index, const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal,
								   const NiColorA *color );
	// appends a vertex or a triangle, into the blocks once the first arrays
	// are full; returns its index
	unsigned short		AddVertex( const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal, const NiColorA *color );
	unsigned short		AddTriangle( unsigned short corner0, unsigned short corner1, unsigned short corner2 );
	// frees the blocks and empties their arrays
	void				FreeBlocks();
	// copies each vertex out to the slot remap gives it, skipping 0xFFFF
	void				GetVertices( const unsigned short *remap, NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_,
									 NiColorA *colors_ );
	// copies each triangle out to the slot remap gives it, skipping 0xFFFF
	void				GetTriangles( unsigned short *triangles_, const unsigned short *remap );

	bool				hasNormals;			// 0x04
	bool				hasColors;			// 0x05
	bool				hasTextures;		// 0x06
	unsigned short		triangleCount;		// 0x08
	unsigned short		vertexCount;		// 0x0A
	unsigned short		vertexCapacity;		// 0x0C
	NiPoint3 *			vertices;			// 0x10
	NiPoint3 *			normals;			// 0x14
	NiPoint2 *			textures;			// 0x18
	NiColorA *			colors;				// 0x1C
	unsigned short		triangleCapacity;	// 0x20
	unsigned short *	triangles;			// 0x24
	NiTArray<DamageVertexBlock *>	vertexBlocks;	// 0x28 owned
	NiTArray<unsigned short *>	triangleBlocks;		// 0x40 owned, 32 triangles each
};

/*
================
FeatureCollector::AddVertex

AddVertex and AddTriangle are inline: retail expands both in
MetaVolumePolygonizer::Polygonize( Voxel &, VoxelBSPTree * ) and keeps
their out-of-line copies (0x4C4E10, 0x4C5000) among the polygonizer's
template copies, where DamageTool.exe has its PushVert and PushTri
(0x4170B0, 0x4172A0).
================
*/
// FUNCTION: STBC 0x004c4e10 ?AddVertex@FeatureCollector@@QAEGPBVNiPoint3@@PBVNiPoint2@@0PBVNiColorA@@@Z @game/DamagePolygonizer
inline unsigned short FeatureCollector::AddVertex( const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal,
												 const NiColorA *color ) {
	if ( vertexCount < vertexCapacity ) {
		vertices[vertexCount] = *vertex;
		if ( texture != NULL ) {
			textures[vertexCount] = *texture;
		}
		if ( normal != NULL ) {
			normals[vertexCount] = *normal;
		}
		if ( color != NULL ) {
			colors[vertexCount] = *color;
		}
	} else {
		unsigned int extra = vertexCount - vertexCapacity;
		unsigned int blockIndex = extra / DamageVertexBlock::SIZE;
		unsigned int slot = extra % DamageVertexBlock::SIZE;
		DamageVertexBlock *block;
		if ( blockIndex + 1 > vertexBlocks.GetSize() ) {
			block = new DamageVertexBlock( hasTextures, hasNormals, hasColors );
			vertexBlocks.Add( block );
		} else {
			block = vertexBlocks.GetAt( blockIndex );
		}
		block->vertices[slot] = *vertex;
		if ( texture != NULL ) {
			block->textures[slot] = *texture;
		}
		if ( normal != NULL ) {
			block->normals[slot] = *normal;
		}
		if ( color != NULL ) {
			block->colors[slot] = *color;
		}
	}
	vertexCount++;
	return vertexCount - 1;
}

/*
================
FeatureCollector::AddTriangle
================
*/
// FUNCTION: STBC 0x004c5000 ?AddTriangle@FeatureCollector@@QAEGGGG@Z @game/DamagePolygonizer
// DRAFT: inline budget: retail calls NiTArray<unsigned short *>::SetSize and SetAt out of line (0x4BBAE0, 0x4BBB60), this build expands SetSize; the in-capacity branch agrees
inline unsigned short FeatureCollector::AddTriangle( unsigned short corner0, unsigned short corner1, unsigned short corner2 ) {
	if ( triangleCount < triangleCapacity ) {
		int index = triangleCount * 3;
		triangles[index++] = corner0;
		triangles[index++] = corner1;
		triangles[index] = corner2;
	} else {
		unsigned int extra = triangleCount - triangleCapacity;
		unsigned int blockIndex = extra / DamageVertexBlock::SIZE;
		unsigned int slot = extra % DamageVertexBlock::SIZE;
		unsigned short *block;
		if ( blockIndex + 1 > triangleBlocks.GetSize() ) {
			block = new unsigned short[DamageVertexBlock::SIZE * 3];
			triangleBlocks.Add( block );
		} else {
			block = triangleBlocks.GetAt( blockIndex );
		}
		int index = slot * 3;
		block[index++] = corner0;
		block[index++] = corner1;
		block[index] = corner2;
	}
	triangleCount++;
	return triangleCount - 1;
}

#endif /* !__DAMAGEMESH_H__ */
