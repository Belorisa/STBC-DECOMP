/*
===============================================================================

	NiBezierMesh.h

	A node whose children are Bezier patches sharing one set of control
	vertices: the mesh owns the vertex, texture coordinate and color arrays
	and an array of edge control-point indices.

	Evidence: the NiRTTI (0x9B2F28, built at 0x82E080) is named
	"NiBezierMesh" with NiNode's as its base; the vtable at 0x89A7DC has
	NiNode's 44 slots and adds 1 (0x89A890, which follows, is the edge
	array's); the creation function the stream registers (0x831510)
	allocates 0x100 bytes. The constructors (0x82E210, 0x82E3B0) store the
	vertex count at 0xE8 and the arrays at 0xEC (12-byte positions), 0xF0
	(8-byte texture coordinates) and 0xF4 (16-byte colors), clear the edge
	array (0xF8) and its count (0xFC), and set the two bytes at 0xFE and
	0xFF; the byte at 0xEA is 1 exactly when there are texture coordinates.
	The destructor (0x82E530) deletes the edge array and frees the three
	arrays. LoadBinary (0x831730) reads the edge indices into an
	NiTSet<unsigned short> (vptr 0x89A890) four per edge.
	NiMorphController's Update morphs the vertices of a target that is not
	an NiTriBasedGeom through 0xE8 and 0xEC, then calls 0x830B80, 0x82FA40
	and slot 38 (UpdateWorldBound).

===============================================================================
*/

#ifndef __NIBEZIERMESH_H__
#define __NIBEZIERMESH_H__

#include "NiNode.h"
#include "NiBezierPatch.h"
#include "NiTSet.h"
#include "NiStreamMacros.h"

class NiBezierPatch;
class NiCamera;
class NiColorA;
class NiPoint2;

/*
================
NiBezierMeshLoadRecord

The record CreateObject hands the stream: NiNode's, under its own vtable.
The class name is invented.

Evidence: CreateObject (0x831510) allocates 0x58 bytes, builds NiNode's
record and stores the vptr 0x89A894 last.
================
*/
// FUNCTION: STBC 0x8316D0 ??_GNiBezierMeshLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x8316F0 ??1NiBezierMeshLoadRecord@@UAE@XZ
class NiBezierMeshLoadRecord : public NiNodeLoadRecord {
};

// FUNCTION: STBC 0x8320A0 ??_GNiBezierMesh@@UAEPAXI@Z
class NiBezierMesh : public NiNode {
public:
	// flags, past NiAVObject's
	enum {
		UNKNOWN_20_POS		= 5,
		UNKNOWN_20_MASK		= 0x0020,	// the first constructor's last argument
		UNKNOWN_40_POS		= 6,
		UNKNOWN_40_BITS		= 0x0001,	// SetField's width mask
		UNKNOWN_40_MASK		= 0x0040	// cleared by the constructors after visiting the patches
	};

	// FUNCTION: STBC 0x832090 ?GetRTTI@NiBezierMesh@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiBezierMesh();

	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );

	// TODO identity: retessellates every patch whose level differs from
	// the argument, within the limit at 0xFF
	virtual void			VirtualSlot44( unsigned char );

							NiBezierMesh( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown );
							NiBezierMesh();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	bool					GetUnknown20() const { return ( flags >> UNKNOWN_20_POS ) & 1; }
	unsigned char			GetUnknownFE() const { return unknownFE; }
	unsigned short			GetVertexCount() const { return vertexCount; }
	NiPoint3 *				GetVertices() const { return vertices; }

	// TODO identity: set from the first constructor's argument
	void					SetUnknown20( bool unknown );
	// TODO identity: handed to the patch children as their flag 0x10
	void					SetUnknown40( bool unknown );
	// TODO identity: SetUnknown40 as a flag field; nothing in retail calls it
	void					Unknown82F730( int unknown );

	// TODO identity: hands the texture set count to every patch child
	// when it changes; nothing in retail calls it
	void					Unknown82F400( unsigned short textureSets_ );
	// TODO identity: hand unknownF8, unknownFC and the camera at 0x128 to
	// every patch child; nothing in retail calls them
	void					Unknown82F880( float unknownF8 );
	void					Unknown82F960( float unknownFC );
	void					Unknown82FB60( NiCamera *camera );
	// TODO identity: grows the control arrays by three points per edge,
	// places them (Unknown830B80) and hands the arrays to every patch
	// child, retessellated at unknownFE; nothing in retail calls it
	void					Unknown8308D0();
	// TODO identity: adds a quartic triangle over the twelve points (the
	// corners, two per side and three inside), sharing or adding the three
	// edges' interior control points; takes and frees the points; the first
	// argument is unused; nothing in retail calls it
	void					Unknown82E770( unsigned short degree, unsigned short *points );
	// TODO identity: compacts the children and links every pair of patches
	// that share a side (equal corner points, in either direction) as
	// neighbours, the edge number negative when the side runs the other
	// way; nothing in retail calls it
	void					Unknown82FDA0();
	// TODO identity: add a triangular (rectangular) patch of the degree, 2
	// to 4 (2 or 3), over the control points the indices name, lowering
	// unknownFF to the patch's highest level; nothing in retail calls them
	void					Unknown82E5C0( unsigned short degree, unsigned short *indices );
	void					Unknown82F2B0( unsigned short degree, unsigned short *indices );
	// TODO identity: places the three interior control points of each
	// edge between its corner points
	void					Unknown830B80();
	// TODO identity: retessellates every patch at its own level
	void					Unknown82FA40();

	static const NiRTTI		rtti;

protected:
	// the clone gets copies of the arrays and the edges
	void					CopyMembers( NiBezierMesh *dest );

	unsigned short			vertexCount;
	unsigned short			textureSets;	// 1 with texture coordinates, else 0
	NiPoint3 *				vertices;		// heap arrays of vertexCount elements, owned
	NiPoint2 *				textures;
	NiColorA *				colors;
	NiTSet<unsigned short> *	edges;		// owned: four control-point indices per edge
	unsigned short			edgeCount;
	unsigned char			unknownFE;		// the patches' tessellation level (TODO identity)
	unsigned char			unknownFF;		// the highest level VirtualSlot44 accepts (TODO identity)
};

NiRegisterStream( NiBezierMesh );

#endif /* !__NIBEZIERMESH_H__ */
