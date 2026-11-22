/*
===============================================================================

	DamageMesh.cpp

	The growable triangle meshes of the damage geometry.

===============================================================================
*/

#include <iostream>
#include "DamageMesh.h"

#include <new>
#include "NiAVObject.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "NiFlipController.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "NiTextureProperty.h"
#include "NiTimeController.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004b23f0 _$E420
// FUNCTION: STBC 0x004b2410 _$E419

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004b2070 _$E32
// FUNCTION: STBC 0x004b2080 _$E29
// FUNCTION: STBC 0x004b2090 _$E31
// FUNCTION: STBC 0x004b20a0 _$E30
// FUNCTION: STBC 0x004b20b0 _$E38
// FUNCTION: STBC 0x004b20c0 _$E35
// FUNCTION: STBC 0x004b20d0 _$E37
// FUNCTION: STBC 0x004b20e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004b21d0 _$E57
// FUNCTION: STBC 0x004b21e0 _$E56
// FUNCTION: STBC 0x004b21f0 _$E137
// FUNCTION: STBC 0x004b2200 _$E136
// FUNCTION: STBC 0x004b2210 _$E150
// FUNCTION: STBC 0x004b2220 _$E149
// FUNCTION: STBC 0x004b2230 _$E68
// FUNCTION: STBC 0x004b2240 _$E67
// FUNCTION: STBC 0x004b2250 _$E72
// FUNCTION: STBC 0x004b2260 _$E71
// FUNCTION: STBC 0x004b2420 _$E76
// FUNCTION: STBC 0x004b2430 _$E75
// FUNCTION: STBC 0x004b2440 _$E80
// FUNCTION: STBC 0x004b2450 _$E79
// FUNCTION: STBC 0x004b2460 _$E170
// FUNCTION: STBC 0x004b2470 _$E169
// FUNCTION: STBC 0x004b25d0 _$E195
// FUNCTION: STBC 0x004b25e0 _$E194
// FUNCTION: STBC 0x004b25f0 _$E208
// FUNCTION: STBC 0x004b2600 _$E207
// FUNCTION: STBC 0x004b2610 _$E228
// FUNCTION: STBC 0x004b2620 _$E227
// FUNCTION: STBC 0x004b2630 _$E217
// FUNCTION: STBC 0x004b2640 _$E216
// FUNCTION: STBC 0x004b2650 _$E224
// FUNCTION: STBC 0x004b2660 _$E223
// FUNCTION: STBC 0x004b2670 _$E232
// FUNCTION: STBC 0x004b2680 _$E231
// FUNCTION: STBC 0x004b2690 _$E236
// FUNCTION: STBC 0x004b26a0 _$E235
// FUNCTION: STBC 0x004b26b0 _$E240
// FUNCTION: STBC 0x004b26c0 _$E239
// FUNCTION: STBC 0x004b26d0 _$E246
// FUNCTION: STBC 0x004b26e0 _$E245
// FUNCTION: STBC 0x004b26f0 _$E199
// FUNCTION: STBC 0x004b2700 _$E198
// FUNCTION: STBC 0x004b2710 _$E203
// FUNCTION: STBC 0x004b2720 _$E202
// FUNCTION: STBC 0x004b2730 _$E212
// FUNCTION: STBC 0x004b2740 _$E211

/*
================
DamageVertexBlock::DamageVertexBlock
================
*/
// FUNCTION: STBC 0x004b2750
DamageVertexBlock::DamageVertexBlock( bool hasTextures, bool hasNormals, bool hasColors ) {
	vertices = new NiPoint3[SIZE];
	if ( hasTextures ) {
		textures = new NiPoint2[SIZE];
	} else {
		textures = NULL;
	}
	if ( hasColors ) {
		colors = new NiColorA[SIZE];
	} else {
		colors = NULL;
	}
	if ( hasNormals ) {
		normals = new NiPoint3[SIZE];
	} else {
		normals = NULL;
	}
}

/*
================
SimpleFeatureCollector::SimpleFeatureCollector
================
*/
// FUNCTION: STBC 0x004b27f0
SimpleFeatureCollector::SimpleFeatureCollector( bool hasTextures_, bool hasNormals_, bool hasColors_ ) {
	hasTextures = hasTextures_;
	hasNormals = hasNormals_;
	vertexCount = 0;
	triangleCount = 0;
	vertexCapacity = 0;
	vertices = NULL;
	normals = NULL;
	textures = NULL;
	colors = NULL;
	triangleCapacity = 0;
	triangles = NULL;
	hasColors = hasColors_;
}

/*
================
SimpleFeatureCollector::~SimpleFeatureCollector
================
*/
// FUNCTION: STBC 0x004b28a0
SimpleFeatureCollector::~SimpleFeatureCollector() {
	unsigned int i;
	for ( i = 0; i < vertexBlocks.GetSize(); i++ ) {
		delete vertexBlocks.GetAt( i );
	}
	for ( i = 0; i < triangleBlocks.GetSize(); i++ ) {
		delete [] triangleBlocks.GetAt( i );
	}
}

/*
================
SimpleFeatureCollector::Reset
================
*/
// FUNCTION: STBC 0x004b2940
void SimpleFeatureCollector::Reset() {
	vertexCount = 0;
	triangleCount = 0;
	vertexCapacity = 0;
	vertices = NULL;
	normals = NULL;
	textures = NULL;
	colors = NULL;
	triangleCapacity = 0;
	triangles = NULL;
}

/*
================
SimpleFeatureCollector::FreeBlocks
================
*/
// FUNCTION: STBC 0x004b2970
void SimpleFeatureCollector::FreeBlocks() {
	unsigned int i;
	for ( i = 0; i < vertexBlocks.GetSize(); i++ ) {
		delete vertexBlocks.GetAt( i );
	}
	for ( i = 0; i < triangleBlocks.GetSize(); i++ ) {
		delete [] triangleBlocks.GetAt( i );
	}
}

/*
================
SimpleFeatureCollector::SetVertexArrays
================
*/
// FUNCTION: STBC 0x004b29e0
void SimpleFeatureCollector::SetVertexArrays( unsigned short capacity, NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_,
										 NiColorA *colors_ ) {
	vertexCapacity = capacity;
	vertices = vertices_;
	normals = normals_;
	vertexCount = 0;
	textures = textures_;
	colors = colors_;
}

/*
================
SimpleFeatureCollector::SetTriangleArray
================
*/
// FUNCTION: STBC 0x004b2a10
void SimpleFeatureCollector::SetTriangleArray( unsigned short capacity, unsigned short *triangles_ ) {
	triangleCount = 0;
	triangleCapacity = capacity;
	triangles = triangles_;
}

/*
================
SimpleFeatureCollector::GetVertices
================
*/
// FUNCTION: STBC 0x004b2a30
void SimpleFeatureCollector::GetVertices( NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_, NiColorA *colors_ ) {
	NiPoint3 *vertex = vertices;
	NiPoint2 *texture = textures;
	NiPoint3 *normal = normals;
	NiColorA *color = colors;
	unsigned int first = vertexCapacity < vertexCount ? vertexCapacity : vertexCount;
	unsigned int i = 0;
	for ( ; i < first; i++ ) {
		*vertices_ = *vertex;
		vertices_++;
		vertex++;
		if ( textures_ != NULL ) {
			*textures_ = *texture;
			textures_++;
			texture++;
		}
		if ( normals_ != NULL ) {
			*normals_ = *normal;
			normals_++;
			normal++;
		}
		if ( colors_ != NULL ) {
			*colors_ = *color;
			colors_++;
			color++;
		}
	}
	for ( unsigned int block = 0; i < vertexCount; block++ ) {
		if ( block >= vertexBlocks.GetSize() ) {
			return;
		}
		DamageVertexBlock *vertexBlock = vertexBlocks.GetAt( block );
		vertex = vertexBlock->vertices;
		texture = vertexBlock->textures;
		normal = vertexBlock->normals;
		color = vertexBlock->colors;
		for ( unsigned int j = 0; i < vertexCount && j < DamageVertexBlock::SIZE; i++, j++ ) {
			*vertices_ = *vertex;
			vertices_++;
			vertex++;
			if ( textures_ != NULL ) {
				*textures_ = *texture;
				textures_++;
				texture++;
			}
			if ( normals_ != NULL ) {
				*normals_ = *normal;
				normals_++;
				normal++;
			}
			if ( colors_ != NULL ) {
				*colors_ = *color;
				colors_++;
				color++;
			}
		}
	}
}

/*
================
SimpleFeatureCollector::GetTriangles
================
*/
// FUNCTION: STBC 0x004b2c80
void SimpleFeatureCollector::GetTriangles( unsigned short *triangles_ ) {
	unsigned short *triangle = triangles;
	unsigned int first = triangleCapacity < triangleCount ? triangleCapacity : triangleCount;
	unsigned int i;
	for ( i = 0; i < first; i++ ) {
		*triangles_++ = *triangle++;
		*triangles_++ = *triangle++;
		*triangles_++ = *triangle++;
	}
	for ( unsigned int block = 0; i < triangleCount; block++ ) {
		if ( block >= triangleBlocks.GetSize() ) {
			return;
		}
		triangle = triangleBlocks.GetAt( block );
		for ( unsigned int j = 0; i < triangleCount && j < DamageVertexBlock::SIZE; i++, j++ ) {
			*triangles_++ = *triangle++;
			*triangles_++ = *triangle++;
			*triangles_++ = *triangle++;
		}
	}
}

/*
================
FeatureCollector::FeatureCollector
================
*/
// FUNCTION: STBC 0x004b2d40
FeatureCollector::FeatureCollector( bool hasTextures_, bool hasNormals_, bool hasColors_ ) {
	hasNormals = hasNormals_;
	hasColors = hasColors_;
	triangleCount = 0;
	vertexCount = 0;
	vertexCapacity = 0;
	vertices = NULL;
	normals = NULL;
	textures = NULL;
	colors = NULL;
	triangleCapacity = 0;
	triangles = NULL;
	hasTextures = hasTextures_;
}

/*
================
FeatureCollector::~FeatureCollector
================
*/
// FUNCTION: STBC 0x004b2e20
FeatureCollector::~FeatureCollector() {
	unsigned int i;
	for ( i = 0; i < vertexBlocks.GetSize(); i++ ) {
		delete vertexBlocks.GetAt( i );
	}
	vertexBlocks.RemoveAll();
	for ( i = 0; i < triangleBlocks.GetSize(); i++ ) {
		delete [] triangleBlocks.GetAt( i );
	}
	triangleBlocks.RemoveAll();
}

/*
================
FeatureCollector::SetVertexArrays
================
*/
// FUNCTION: STBC 0x004b2f00
void FeatureCollector::SetVertexArrays( unsigned short capacity, NiPoint3 *vertices_, NiPoint3 *normals_, NiPoint2 *textures_,
											 NiColorA *colors_ ) {
	vertexCapacity = capacity;
	vertices = vertices_;
	// BUG: the normals take the vertex array; normals_ is never used
	normals = vertices_;
	textures = textures_;
	colors = colors_;
	vertexCount = capacity;
}

/*
================
FeatureCollector::SetTriangleArray
================
*/
// FUNCTION: STBC 0x004b2f30
void FeatureCollector::SetTriangleArray( unsigned short capacity, unsigned short *triangles_ ) {
	triangleCapacity = capacity;
	triangles = triangles_;
	triangleCount = capacity;
}

/*
================
FeatureCollector::SetVertex
================
*/
// FUNCTION: STBC 0x004b2f50
// PARKED: register allocation: retail keeps the index in EAX and the capacity and block slot in EDX, this build the reverse; same instructions otherwise
void FeatureCollector::SetVertex( unsigned short index, const NiPoint3 *vertex, const NiPoint2 *texture, const NiPoint3 *normal,
									   const NiColorA *color ) {
	if ( index < vertexCapacity ) {
		vertices[index] = *vertex;
		if ( normal != NULL ) {
			normals[index] = *normal;
		}
		if ( texture != NULL ) {
			textures[index] = *texture;
		}
		if ( color != NULL ) {
			colors[index] = *color;
		}
	} else {
		unsigned int slot = index - vertexCapacity;
		unsigned int j = slot & ( DamageVertexBlock::SIZE - 1 );
		DamageVertexBlock *block = vertexBlocks.GetAt( slot >> 5 );
		block->vertices[j] = *vertex;
		if ( normal != NULL ) {
			block->normals[j] = *normal;
		}
		if ( texture != NULL ) {
			block->textures[j] = *texture;
		}
		if ( color != NULL ) {
			block->colors[j] = *color;
		}
	}
}

/*
================
FeatureCollector::FreeBlocks
================
*/
// FUNCTION: STBC 0x004b3090
void FeatureCollector::FreeBlocks() {
	unsigned int i;
	for ( i = 0; i < vertexBlocks.GetSize(); i++ ) {
		delete vertexBlocks.GetAt( i );
	}
	vertexBlocks.RemoveAll();
	for ( i = 0; i < triangleBlocks.GetSize(); i++ ) {
		delete [] triangleBlocks.GetAt( i );
	}
	triangleBlocks.RemoveAll();
}


/*
================
FeatureCollector::GetVertices
================
*/
// FUNCTION: STBC 0x004b3140
void FeatureCollector::GetVertices( const unsigned short *remap, NiPoint3 *vertices_, NiPoint2 *textures_, NiPoint3 *normals_,
										 NiColorA *colors_ ) {
	unsigned short i;
	for ( i = 0; i < vertexCapacity; i++ ) {
		if ( remap[i] != 0xFFFF ) {
			vertices_[remap[i]] = vertices[i];
			if ( normals_ != NULL ) {
				normals_[remap[i]] = normals[i];
			}
			if ( textures_ != NULL ) {
				textures_[remap[i]] = textures[i];
			}
			if ( colors_ != NULL ) {
				colors_[remap[i]] = colors[i];
			}
		}
	}
	for ( unsigned short b = 0; b < vertexBlocks.GetSize(); b++ ) {
		if ( i >= vertexCount ) {
			return;
		}
		DamageVertexBlock *block = vertexBlocks.GetAt( b );
		for ( unsigned short k = 0; k < DamageVertexBlock::SIZE; k++, i++ ) {
			if ( i >= vertexCount ) {
				break;
			}
			if ( remap[i] != 0xFFFF ) {
				vertices_[remap[i]] = block->vertices[k];
				if ( normals_ != NULL ) {
					normals_[remap[i]] = block->normals[k];
				}
				if ( textures_ != NULL ) {
					textures_[remap[i]] = block->textures[k];
				}
				if ( colors_ != NULL ) {
					colors_[remap[i]] = block->colors[k];
				}
			}
		}
	}
}

/*
================
FeatureCollector::GetTriangles
================
*/
// FUNCTION: STBC 0x004b3390
void FeatureCollector::GetTriangles( unsigned short *triangles_, const unsigned short *remap ) {
	unsigned short first = triangleCapacity < triangleCount ? triangleCapacity : triangleCount;
	unsigned short t;
	for ( t = 0; t < first; t++ ) {
		unsigned short j = remap[t];
		if ( j != 0xFFFF ) {
			int index = j * 3;
			int source = t * 3;
			triangles_[index++] = triangles[source++];
			triangles_[index++] = triangles[source++];
			triangles_[index] = triangles[source];
		}
	}
	for ( unsigned short b = 0; b < triangleBlocks.GetSize(); b++ ) {
		unsigned short *triangle = triangleBlocks.GetAt( b );
		for ( unsigned short k = 0; k < DamageVertexBlock::SIZE; t++, k++ ) {
			if ( t >= triangleCount ) {
				break;
			}
			unsigned short j = remap[t];
			if ( j != 0xFFFF ) {
				int index = j * 3;
				// BUG: the block's cursor only moves past triangles that are
				// kept, so a skipped one shifts every later triangle of the block
				triangles_[index++] = *triangle++;
				triangles_[index++] = *triangle++;
				triangles_[index] = *triangle++;
			}
		}
	}
}
