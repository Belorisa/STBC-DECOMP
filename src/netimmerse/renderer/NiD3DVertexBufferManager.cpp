// NiD3DVertexBufferManager.cpp -- the renderer's vertex buffers, by vertex format

#include "NiRTLib.h"
#include "NiD3DVertexBufferManager.h"
#include "NiGeometryData.h"
#include "NiPoint3.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiTimeController.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007cbe40 _$E41
// FUNCTION: STBC 0x007cbe50 _$E49

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007cbdc0 _$E206
// FUNCTION: STBC 0x007cbde0 _$E17
// FUNCTION: STBC 0x007cbe00 _$E15
// FUNCTION: STBC 0x007cbe10 _$E23
// FUNCTION: STBC 0x007cbe30 _$E21

/*
================
NiD3DVertexBufferManager::NiD3DVertexBufferManager

Keeps a reference to the Direct3D object and the device. A software device
gets its buffers in system memory.
================
*/
// FUNCTION: STBC 0x7CB2C0
NiD3DVertexBufferManager::NiD3DVertexBufferManager( IDirect3D7 *direct3D_, IDirect3DDevice7 *device_, bool softwareVertices ) {
	direct3D = direct3D_;
	direct3D->AddRef();
	device = device_;
	device->AddRef();
	capabilities = D3DVBCAPS_WRITEONLY | ( softwareVertices ? D3DVBCAPS_SYSTEMMEMORY : 0 );
	unknown1C = 0;
	unknown20 = NULL;
	unknown24 = 0;
	unknown28 = NULL;
}

/*
================
NiD3DVertexBufferManager::~NiD3DVertexBufferManager

Releases the interfaces, then every entry's vertex buffers.
================
*/
// FUNCTION: STBC 0x7CB370
NiD3DVertexBufferManager::~NiD3DVertexBufferManager() {
	if ( direct3D ) {
		direct3D->Release();
		direct3D = NULL;
	}
	if ( device ) {
		device->Release();
		device = NULL;
	}

	while ( entries.GetCount() ) {
		NiTMapIterator pos = entries.GetFirstPos();
		unsigned int fvf;
		NiD3DVertexBufferEntry *entry;
		entries.GetNext( pos, fvf, entry );
		if ( entry ) {
			entries.RemoveAt( fvf );
			if ( entry->vertexBuffers[0] ) {
				entry->vertexBuffers[0]->Release();
				entry->vertexBuffers[0] = NULL;
			}
			if ( entry->vertexBuffers[1] ) {
				entry->vertexBuffers[1]->Release();
				entry->vertexBuffers[1] = NULL;
			}
			delete entry;
		}
	}

	delete [] unknown20;
	delete [] unknown28;
}

/*
================
NiD3DVertexBufferManager::Unknown7CB570

The format's current vertex buffer, recreated larger when it holds fewer
than vertexCount vertices; the next call hands out the other one.
================
*/
// FUNCTION: STBC 0x7CB570
IDirect3DVertexBuffer7 *NiD3DVertexBufferManager::Unknown7CB570( DWORD fvf, unsigned int vertexCount ) {
	NiD3DVertexBufferEntry *entry;
	if ( entries.GetAt( fvf, entry ) ) {
		if ( entry->descs[entry->current].dwNumVertices < vertexCount ) {
			if ( entry->vertexBuffers[entry->current] ) {
				entry->vertexBuffers[entry->current]->Release();
				entry->vertexBuffers[entry->current] = NULL;
			}
			entry->descs[entry->current].dwNumVertices = vertexCount;
		}
	} else {
		entry = new NiD3DVertexBufferEntry;
		entry->descs[entry->current].dwNumVertices = vertexCount;
		entry->descs[entry->current].dwFVF = fvf;
		entries.SetAt( fvf, entry );
	}

	entry->descs[entry->current].dwCaps = capabilities;
	entry->descs[entry->current].dwFVF = fvf;
	if ( !entry->vertexBuffers[entry->current] ) {
		if ( direct3D->CreateVertexBuffer( &entry->descs[entry->current], &entry->vertexBuffers[entry->current], 0 ) != D3D_OK
			|| !entry->vertexBuffers[entry->current] ) {
			entry->descs[entry->current].dwNumVertices = 0;
			return NULL;
		}
	}

	entry->current ^= 1;
	return entry->vertexBuffers[entry->current ^ 1];
}

/*
================
NiD3DVertexBufferManager::Unknown7CB770

Fills one of the format's vertex buffers with a screen polygon: each
position with its depth made from its z, then the colors and the texture
coordinates as the polygon has them. As the debug copy has it
(AnimOnly.exe 0x52C5D6, NiDirect3D), the buffer tested again with the lock.
================
*/
// FUNCTION: STBC 0x7CB770
IDirect3DVertexBuffer7 *NiD3DVertexBufferManager::Unknown7CB770( unsigned short vertexCount, NiPoint3 *vertices, NiPoint2 *textures, NiColorA *colors, float nearDistance, float depthRange ) {
	DWORD fvf = 0;
	unsigned int stride = 0;
	fvf |= D3DFVF_XYZ;
	unsigned int positionOffset = stride;
	stride += sizeof( NiPoint3 );
	unsigned int colorOffset;
	if ( colors ) {
		fvf |= D3DFVF_DIFFUSE;
		colorOffset = stride;
		stride += sizeof( D3DCOLOR );
	}
	unsigned int textureOffset;
	if ( textures ) {
		fvf |= D3DFVF_TEX1;
		textureOffset = stride;
		stride += sizeof( NiPoint2 );
	}

	IDirect3DVertexBuffer7 *vertexBuffer = Unknown7CB570( fvf, vertexCount );
	if ( !vertexBuffer ) {
		return NULL;
	}
	unsigned char *bits = NULL;
	if ( !vertexBuffer || vertexBuffer->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void **)&bits, NULL ) != D3D_OK ) {
		return NULL;
	}

	unsigned int i;
	unsigned char *vertex = bits + positionOffset;
	for ( i = 0; i < vertexCount; i++, vertex += stride, vertices++ ) {
		float depth = depthRange * vertices->z + nearDistance;
		NiPoint3 *position = (NiPoint3 *)vertex;
		position->x = depth * vertices->x;
		position->y = depth * vertices->y;
		position->z = depth;
	}
	if ( colors ) {
		vertex = bits + colorOffset;
		for ( i = 0; i < vertexCount; i++, vertex += stride, colors++ ) {
			*(D3DCOLOR *)vertex = ( FloatToInt( colors->a * 255.0f ) << 24 ) | ( FloatToInt( colors->r * 255.0f ) << 16 )
				| ( FloatToInt( colors->g * 255.0f ) << 8 ) | FloatToInt( colors->b * 255.0f );
		}
	}
	if ( textures ) {
		vertex = bits + textureOffset;
		for ( i = 0; i < vertexCount; i++, vertex += stride, textures++ ) {
			memcpy( vertex, textures, sizeof( NiPoint2 ) );
		}
	}

	if ( vertexBuffer->Unlock() != D3D_OK ) {
		return NULL;
	}
	return vertexBuffer;
}

/*
================
NiD3DVertexBufferManager::Unknown7CB8E0

Each vertex is the position, then the normal and the diffuse color when
the data has them, then the texture coordinates of each set. The offsets
and copies as the debug copy has them (AnimOnly.exe 0x52C88E,
NiDirect3D); BC's build also releases a new buffer that fails to lock or
unlock, as retail's bytes show.
================
*/
// FUNCTION: STBC 0x7CB8E0
IDirect3DVertexBuffer7 *NiD3DVertexBufferManager::Unknown7CB8E0( NiGeometryData *data, IDirect3DVertexBuffer7 *reuse ) {
	unsigned short vertexCount = data->GetActiveVertexCount();
	if ( !vertexCount ) {
		return NULL;
	}

	NiPoint3 *vertices = data->GetVertices();
	NiPoint3 *normals = data->GetNormals();
	unsigned int textureSets = data->GetTextureSets();
	NiColorA *colors = data->GetColors();

	// Direct3D takes at most eight texture coordinate sets
	textureSets = ( textureSets > 8 ) ? 8 : textureSets;

	DWORD fvf = 0;
	unsigned int stride = 0;
	fvf |= D3DFVF_XYZ;
	unsigned int positionOffset = stride;
	stride += sizeof( NiPoint3 );
	unsigned int normalOffset;
	if ( normals ) {
		fvf |= D3DFVF_NORMAL;
		normalOffset = stride;
		stride += sizeof( NiPoint3 );
	}
	unsigned int colorOffset;
	if ( colors ) {
		fvf |= D3DFVF_DIFFUSE;
		colorOffset = stride;
		stride += sizeof( D3DCOLOR );
	}
	fvf |= textureSets << D3DFVF_TEXCOUNT_SHIFT;
	unsigned int textureOffset = stride;
	stride += textureSets * sizeof( NiPoint2 );

	IDirect3DVertexBuffer7 *vertexBuffer = NULL;
	D3DVERTEXBUFFERDESC desc;
	if ( reuse ) {
		desc.dwSize = sizeof( desc );
		reuse->GetVertexBufferDesc( &desc );
		if ( !( desc.dwCaps & D3DVBCAPS_OPTIMIZED ) && desc.dwFVF == fvf && desc.dwNumVertices == vertexCount ) {
			vertexBuffer = reuse;
		}
	}
	if ( !vertexBuffer ) {
		memset( &desc, 0, sizeof( desc ) );
		desc.dwSize = sizeof( desc );
		desc.dwCaps = capabilities;
		desc.dwFVF = fvf;
		desc.dwNumVertices = vertexCount;
		if ( direct3D->CreateVertexBuffer( &desc, &vertexBuffer, 0 ) != D3D_OK ) {
			return NULL;
		}
	}

	unsigned char *bits = NULL;
	if ( !vertexBuffer ) {
		return NULL;
	}
	if ( vertexBuffer->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void **)&bits, NULL ) == D3D_OK ) {
		unsigned int i;
		unsigned char *vertex = bits + positionOffset;
		for ( i = 0; i < vertexCount; i++, vertex += stride, vertices++ ) {
			memcpy( vertex, vertices, sizeof( NiPoint3 ) );
		}
		if ( normals ) {
			vertex = bits + normalOffset;
			for ( i = 0; i < vertexCount; i++, vertex += stride, normals++ ) {
				memcpy( vertex, normals, sizeof( NiPoint3 ) );
			}
		}
		if ( colors ) {
			vertex = bits + colorOffset;
			for ( i = 0; i < vertexCount; i++, vertex += stride, colors++ ) {
				*(D3DCOLOR *)vertex = ( FloatToInt( colors->a * 255.0f ) << 24 ) | ( FloatToInt( colors->r * 255.0f ) << 16 )
					| ( FloatToInt( colors->g * 255.0f ) << 8 ) | FloatToInt( colors->b * 255.0f );
			}
		}
		for ( unsigned int set = 0; set < textureSets; set++ ) {
			vertex = bits + textureOffset + set * 2 * sizeof( float );
			NiPoint2 *textures = data->GetTextureSet( set );
			for ( i = 0; i < vertexCount; i++, vertex += stride, textures++ ) {
				memcpy( vertex, textures, sizeof( NiPoint2 ) );
			}
		}

		if ( vertexBuffer->Unlock() == D3D_OK ) {
			return vertexBuffer;
		}
		if ( vertexBuffer != reuse ) {
			vertexBuffer->Release();
		}
		return NULL;
	}

	if ( vertexBuffer && vertexBuffer != reuse ) {
		vertexBuffer->Release();
	}
	return NULL;
}

/*
================
NiD3DVertexBufferManager::Unknown7CBBA0
================
*/
// FUNCTION: STBC 0x7CBBA0
unsigned short *NiD3DVertexBufferManager::Unknown7CBBA0( unsigned int count ) {
	if ( count >= unknown1C ) {
		delete [] unknown20;
		unknown20 = new unsigned short[count];
		unknown1C = unknown20 ? count : 0;
	}
	return unknown20;
}

/*
================
NiD3DVertexBufferManager::Unknown7CBBE0
================
*/
// FUNCTION: STBC 0x7CBBE0
unsigned short *NiD3DVertexBufferManager::Unknown7CBBE0( unsigned int quadCount ) {
	unsigned int count = quadCount * 6;
	if ( count >= unknown24 ) {
		delete [] unknown28;
		unknown28 = new unsigned short[count];
		if ( unknown28 ) {
			unknown24 = count;
			unsigned short *index = unknown28;
			unsigned int vertex = 0;
			for ( unsigned int i = 0; i < quadCount; i++ ) {
				*index++ = vertex;
				*index++ = vertex + 1;
				*index++ = vertex + 2;
				*index++ = vertex;
				*index++ = vertex + 2;
				*index++ = vertex + 3;
				vertex += 4;
			}
		} else {
			unknown24 = 0;
		}
	}
	return unknown28;
}
