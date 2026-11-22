/*
===============================================================================

	NiD3DVertexBufferManager.h

	The renderer's vertex buffer manager: a map of Direct3D vertex buffers
	it created on the device.

	Evidence: NiD3DRender's creation (0x7C3480) allocates 0x2C bytes and
	runs the constructor (0x7CB2C0) with the Direct3D object, the device and
	whether the device is hardware transform and lighting; the constructor
	takes a reference to both, builds a hash map at 0x0C (vtable 0x898AEC,
	37 buckets) and picks the vertex buffer capabilities: D3DVBCAPS_WRITEONLY
	(0x10000), plus D3DVBCAPS_SYSTEMMEMORY (0x800) for a software device. The
	destructor (0x7CB370) releases both interfaces, then each map entry's
	two vertex buffers. The class name is invented from that use.

===============================================================================
*/

#ifndef __NID3DVERTEXBUFFERMANAGER_H__
#define __NID3DVERTEXBUFFERMANAGER_H__

#include <string.h>

#include "NiD3D7.h"
#include "NiTMap.h"

class NiGeometryData;
class NiPoint3;
class NiPoint2;
class NiColorA;

/*
================
NiD3DVertexBufferEntry

The two vertex buffers kept for one vertex format, used in turn, each with
the description it was created from. The class name is invented.

Evidence: the manager's buffer lookup (0x7CB570) allocates 0x2C bytes,
clears both buffers and both descriptions and starts at buffer 0; it
grows the current buffer's description to the vertex count it needs and
toggles the current index after each use. The destructor (0x7CB370)
releases both buffers before freeing the entry.
================
*/
class NiD3DVertexBufferEntry {
public:
							NiD3DVertexBufferEntry() {
								vertexBuffers[1] = NULL;
								vertexBuffers[0] = NULL;
								memset( descs, 0, sizeof( descs ) );
								current = 0;
							}

	IDirect3DVertexBuffer7 *	vertexBuffers[2];
	D3DVERTEXBUFFERDESC		descs[2];		// dwSize is never set
	unsigned char			current;		// the buffer the next lookup hands out
};

class NiD3DVertexBufferManager {
public:
							NiD3DVertexBufferManager( IDirect3D7 *direct3D, IDirect3DDevice7 *device, bool softwareVertices );
							~NiD3DVertexBufferManager();

	// a vertex buffer of the format for at least vertexCount vertices, one
	// of two kept per format and used in turn; NULL when it can't create one
	IDirect3DVertexBuffer7 *	Unknown7CB570( DWORD fvf, unsigned int vertexCount );
	// an index block of at least count indices (contents left to the caller)
	unsigned short *		Unknown7CBBA0( unsigned int count );
	// indices for quadCount quads as two triangles each: 0 1 2, 0 2 3
	unsigned short *		Unknown7CBBE0( unsigned int quadCount );

	// a vertex buffer holding the data's vertices: positions, then
	// normals, colors and up to eight texture coordinate sets as the data
	// has them; reuses the given buffer when it has the same format and
	// size; NULL when the data has no vertices or the buffer can't be
	// created or filled
	IDirect3DVertexBuffer7 *	Unknown7CB8E0( NiGeometryData *data, IDirect3DVertexBuffer7 *reuse );

	// a vertex buffer holding a screen polygon: the positions with their
	// depth set from the z of each (near plus z times range), then the
	// colors packed to 32 bits and the texture coordinates when given; NULL
	// when it can't be created or locked
	IDirect3DVertexBuffer7 *	Unknown7CB770( unsigned short vertexCount, NiPoint3 *vertices, NiPoint2 *textures, NiColorA *colors, float nearDistance, float depthRange );

	IDirect3D7 *			direct3D;		// referenced at construction
	IDirect3DDevice7 *		device;			// referenced at construction
	DWORD					capabilities;	// D3DVBCAPS flags of the buffers it creates
	NiTMap<unsigned int, NiD3DVertexBufferEntry *>	entries;	// by vertex format (vtable 0x898AEC)
	unsigned int			unknown1C;		// the size of unknown20, in indices
	unsigned short *		unknown20;		// an index block, grown on demand
	unsigned int			unknown24;		// the size of unknown28, in indices
	unsigned short *		unknown28;		// quad list indices, grown on demand
};

#endif /* !__NID3DVERTEXBUFFERMANAGER_H__ */
