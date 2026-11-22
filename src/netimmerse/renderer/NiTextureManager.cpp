// NiTextureManager.cpp -- the abstract texture manager

#include "NiRTLib.h"
#include "NiTextureManager.h"
#include "NiD3DTextureManager.h"
#include "NiD3DPixelFormat.h"
#include "NiDDImage.h"
#include "NiD3DRender.h"

#include <string.h>

// Static objects' construction and exit-time destruction.
// FUNCTION: STBC 0x007d3290 _$E236
// FUNCTION: STBC 0x007d32e0 _$E233
// FUNCTION: STBC 0x007d3370 _$E241
// FUNCTION: STBC 0x007d33c0 _$E239
// FUNCTION: STBC 0x007d3450 _$E230

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d7a70 _$E248
// FUNCTION: STBC 0x007d7790 _$E32
// FUNCTION: STBC 0x007d77b0 _$E30
// FUNCTION: STBC 0x007d77c0 _$E38
// FUNCTION: STBC 0x007d77e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d7840 _$E46
// FUNCTION: STBC 0x007d7850 _$E85
// FUNCTION: STBC 0x007d7860 _$E60
// FUNCTION: STBC 0x007d7870 _$E73
// FUNCTION: STBC 0x007d7a90 _$E89
// FUNCTION: STBC 0x007d7aa0 _$E93
// FUNCTION: STBC 0x007d7ab0 _$E97
// FUNCTION: STBC 0x007d7ac0 _$E101
// FUNCTION: STBC 0x007d7ad0 _$E205
// FUNCTION: STBC 0x007d7ae0 _$E210
// FUNCTION: STBC 0x007d7af0 _$E215

// DATA: STBC 0x9A1400 ?managers@NiTextureManager@@2V?$NiTSimpleList@PAVNiTextureManager@@@@A
NiTextureManagerList NiTextureManager::managers;

/*
================
QUERY_TEXTURE_FORMAT

Asks the device for its format of one kind; when it has one, keeps and
registers it, records log2 of its bytes per pixel and sets found. Jumps to
failed when the query fails. (name invented)
================
*/
#define QUERY_TEXTURE_FORMAT( callback, type )								\
	search.found = false;													\
	search.format = NULL;													\
	if ( device->EnumTextureFormats( callback, &search ) != D3D_OK ) {		\
		goto failed;														\
	}																		\
	if ( search.found ) {													\
		unknown08[type] = search.format;									\
		NiDDPixelDescTable::Register( *search.format );					\
		search.format->unknown20 = type;									\
		unsigned int sizeShift;												\
		switch ( search.format->bitsPerPixel ) {							\
		case 8:																\
			sizeShift = 0;													\
			break;															\
		case 16:															\
			sizeShift = 1;													\
			break;															\
		case 24:															\
		case 32:															\
			sizeShift = 2;													\
			break;															\
		default:															\
			sizeShift = 2;													\
			break;															\
		}																	\
		unknown24[type] = sizeShift;										\
		unknown40[type] = true;												\
		found = true;														\
	}

/*
================
NiD3DTextureManager::QueryTextureFormats

Asks the device for one format of each kind, alpha formats first.
================
*/
// FUNCTION: STBC 0x7D3460
bool NiD3DTextureManager::QueryTextureFormats( IDirect3DDevice7 *device ) {
	bool found = false;
	NiD3DTextureFormatSearch search;

	QUERY_TEXTURE_FORMAT( EnumTextureFormat4444, TEXTURE_FORMAT_4444 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormat8888, TEXTURE_FORMAT_8888 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormat1555, TEXTURE_FORMAT_1555 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormat555, TEXTURE_FORMAT_555 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormat565, TEXTURE_FORMAT_565 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormat888, TEXTURE_FORMAT_888 )
	QUERY_TEXTURE_FORMAT( EnumTextureFormatPalette8, TEXTURE_FORMAT_PALETTE8 )

	if ( !found ) {
		return false;
	}
	return true;

failed:
	NiD3DRender::Error( "Creation failed: Texture format query failed" );
	return false;
}

/*
================
NiD3DTextureManager::EnumTextureFormatPalette8
================
*/
// FUNCTION: STBC 0x7D3960
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormatPalette8( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_PALETTEINDEXED8 ) || ( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( pixelFormat );
	return D3DENUMRET_CANCEL;
}

#undef QUERY_TEXTURE_FORMAT

/*
================
NiD3DTextureManager::EnumTextureFormat555
================
*/
// FUNCTION: STBC 0x7D3B60
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat555( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || ( pixelFormat->dwFlags & DDPF_PALETTEINDEXED8 )
		|| pixelFormat->dwRGBBitCount != 16 || ( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 5 || format.bits[1] != 5 || format.bits[2] != 5 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiD3DTextureManager::EnumTextureFormat565
================
*/
// FUNCTION: STBC 0x7D3D20
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat565( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || ( pixelFormat->dwFlags & DDPF_PALETTEINDEXED8 )
		|| pixelFormat->dwRGBBitCount != 16 || ( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 5 || format.bits[1] != 6 || format.bits[2] != 5 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiD3DTextureManager::EnumTextureFormat888
================
*/
// FUNCTION: STBC 0x7D3EE0
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat888( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || ( pixelFormat->dwFlags & DDPF_PALETTEINDEXED8 )
		|| pixelFormat->dwRGBBitCount != 32 || ( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 8 || format.bits[1] != 8 || format.bits[2] != 8 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiD3DTextureManager::EnumTextureFormat4444
================
*/
// FUNCTION: STBC 0x7D40A0
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat4444( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || !( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) || pixelFormat->dwRGBBitCount != 16 ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 4 || format.bits[1] != 4 || format.bits[2] != 4 || format.bits[3] != 4 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiD3DTextureManager::EnumTextureFormat1555
================
*/
// FUNCTION: STBC 0x7D4260
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat1555( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || !( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) || pixelFormat->dwRGBBitCount != 16 ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 5 || format.bits[1] != 5 || format.bits[2] != 5 || format.bits[3] != 1 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiD3DTextureManager::EnumTextureFormat8888
================
*/
// FUNCTION: STBC 0x7D4420
HRESULT CALLBACK NiD3DTextureManager::EnumTextureFormat8888( LPDDPIXELFORMAT pixelFormat, LPVOID context ) {
	if ( !( pixelFormat->dwFlags & DDPF_RGB ) || !( pixelFormat->dwFlags & DDPF_ALPHAPIXELS ) || pixelFormat->dwRGBBitCount != 32 ) {
		return D3DENUMRET_OK;
	}
	NiDDPixelDesc format( pixelFormat );
	if ( format.bits[0] != 8 || format.bits[1] != 8 || format.bits[2] != 8 || format.bits[3] != 8 ) {
		return D3DENUMRET_OK;
	}
	NiD3DTextureFormatSearch *search = (NiD3DTextureFormatSearch *)context;
	search->found = true;
	search->format = new NiDDPixelDesc( format );
	return D3DENUMRET_CANCEL;
}

/*
================
NiTextureManager::NiTextureManager

Sets the default texture memory limits and joins the list of managers.
================
*/
// FUNCTION: STBC 0x7D45E0
NiTextureManager::NiTextureManager() {
	unknown8C = 0;
	unknown90 = 0;
	unknown98 = 0;
	unknown9C = 0;
	memset( &unknown48, 0, sizeof( unknown48 ) );
	unknown74 = &unknown48;
	unknown78 = 0x4000;
	unknown80 = 0x4000;
	unknown7C = 0x4000;
	unknown84 = 0x4000;
	unknown88 = 0;
	unknownA0 = 1;
	directDraw = NULL;
	for ( unsigned int i = 0; i < 7; i++ ) {
		unknown40[i] = false;
		unknown08[i] = NULL;
	}
	unknownA8 = 0;
	unknownA4 = 0;
	refCount = 0;
	managers.AddHead( this );
}

/*
================
NiTextureManager::~NiTextureManager

Leaves the list of managers and the maps of shared managers, frees the
pixel formats and releases the DirectDraw object and the device.
================
*/
// FUNCTION: STBC 0x7D46A0
// FUNCTION: STBC 0x7D4680 ??_GNiTextureManager@@UAEPAXI@Z
NiTextureManager::~NiTextureManager() {
	managers.Remove( this );

	NiD3DTextureManager *manager;
	if ( NiD3DTextureManager::hardwareManagers.GetAt( directDraw, manager ) && this == manager ) {
		NiD3DTextureManager::hardwareManagers.RemoveAt( directDraw );
	}
	if ( NiD3DTextureManager::softwareManagers.GetAt( directDraw, manager ) && this == manager ) {
		NiD3DTextureManager::softwareManagers.RemoveAt( directDraw );
	}

	for ( unsigned int i = 0; i < 7; i++ ) {
		if ( unknown08[i] ) {
			delete unknown08[i];
			unknown08[i] = NULL;
		}
	}

	if ( directDraw ) {
		directDraw->Release();
	}
	if ( device ) {
		device->Release();
	}
}

/*
================
NiD3DTextureManager::Create

Shares the manager of a DirectDraw object, making it on first use, and
takes a reference to it.
================
*/
// FUNCTION: STBC 0x7D4950
NiD3DTextureManager *NiD3DTextureManager::Create( bool multitexture, bool trueColor, IDirectDraw7 *directDraw,
												  IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc, bool hardware ) {
	NiD3DTextureManager *manager;
	if ( hardware ) {
		if ( !hardwareManagers.GetAt( directDraw, manager ) ) {
			manager = CreateManager( multitexture, trueColor, directDraw, device, deviceDesc );
			if ( !manager ) {
				return NULL;
			}
			hardwareManagers.SetAt( directDraw, manager );
		}
	} else {
		if ( !softwareManagers.GetAt( directDraw, manager ) ) {
			manager = CreateManager( multitexture, trueColor, directDraw, device, deviceDesc );
			if ( !manager ) {
				return NULL;
			}
			softwareManagers.SetAt( directDraw, manager );
		}
	}
	manager->AddReference();
	return manager;
}

// The hardware map is built first: its initialiser (0x7D3290) and its
// destructor's guard bit come before the software map's.
// DATA: STBC 0x9A1408 ?hardwareManagers@NiD3DTextureManager@@2V?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@A
NiTMap<IDirectDraw7 *, NiD3DTextureManager *> NiD3DTextureManager::hardwareManagers;

// DATA: STBC 0x9A13F0 ?softwareManagers@NiD3DTextureManager@@2V?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@A
NiTMap<IDirectDraw7 *, NiD3DTextureManager *> NiD3DTextureManager::softwareManagers;

// DATA: STBC 0x977EC8 ?nextId@NiD3DTextureManager@@2IA
unsigned int NiD3DTextureManager::nextId = 1;

/*
================
NiD3DTextureManager::NiD3DTextureManager

Allocates the first block of free entries and takes the ids of its two
tables; the tables themselves are made later.
================
*/
// FUNCTION: STBC 0x7D4B90
NiD3DTextureManager::NiD3DTextureManager() {
	entryBlocks = new NiTList<_DPSetupTexture *>( 2 );
	freeEntries = (_DPSetupTexture *)calloc( ENTRY_BLOCK_SIZE, sizeof( _DPSetupTexture ) );
	entryBlocks->AddHead( freeEntries );
	for ( unsigned int i = 1; i < ENTRY_BLOCK_SIZE; i++ ) {
		freeEntries[i - 1].next = &freeEntries[i];
	}
	freeEntries[ENTRY_BLOCK_SIZE - 1].next = NULL;

	tables[0] = NULL;
	tables[1] = NULL;
	unknownC0[0] = NULL;
	unknownC0[1] = NULL;
	unknownC8[0] = NULL;
	unknownC8[1] = NULL;
	unknownD8[0] = 0;
	unknownD8[1] = 0;
	unknownD0[0] = NULL;
	unknownD0[1] = NULL;
	unknownE0 = false;
	unknownE1 = false;
	currentTable = 0;
	ids[0] = nextId++;
	ids[1] = nextId++;
}

/*
================
NiD3DTextureManager::~NiD3DTextureManager

Drops the images' entries of both tables, releases every texture and
frees the entry blocks and the tables.
================
*/
// FUNCTION: STBC 0x7D4DB0
// FUNCTION: STBC 0x7D4D90 ??_GNiD3DTextureManager@@UAEPAXI@Z
NiD3DTextureManager::~NiD3DTextureManager() {
	NiDDImage::RemoveAllEntries( ids[0] );
	NiDDImage::RemoveAllEntries( ids[1] );

	for ( unsigned int t = 0; t < 2; t++ ) {
		if ( tables[t] ) {
			for ( unsigned int i = 0; i < 256; i++ ) {
				while ( tables[t][i] ) {
					Unknown7D6650( tables[t][i] );
				}
			}
		}
		while ( unknownC0[t] ) {
			Unknown7D6650( unknownC0[t] );
		}
	}

	_DPSetupTexture *entry = freeEntries;
	while ( entry ) {
		_DPSetupTexture *save = entry;
		entry = entry->next;
		save->image = NULL;
	}

	if ( entryBlocks ) {
		while ( !entryBlocks->IsEmpty() ) {
			free( entryBlocks->RemoveHead() );
		}
		delete entryBlocks;
	}

	delete [] tables[0];
	delete [] unknownC8[0];
	delete [] unknownD0[0];
	if ( unknownE1 ) {
		delete [] tables[1];
		delete [] unknownC8[1];
		delete [] unknownD0[1];
	}
}

/*
================
NiD3DTextureManager::CreateManager

Makes a manager on a device: takes references to the DirectDraw object and
the device, the texture size limits and the flags from the device's caps,
finds its texture formats, and makes the bucket tables, a second set only
for multitexturing on a device with separate texture memories.
================
*/
// FUNCTION: STBC 0x7D5080
NiD3DTextureManager *NiD3DTextureManager::CreateManager( bool multitexture, bool trueColor, IDirectDraw7 *directDraw,
														 IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc ) {
	NiD3DRender::Message( "(Creation): Attempting to create texture manager" );
	NiD3DTextureManager *manager = new NiD3DTextureManager;

	manager->directDraw = directDraw;
	directDraw->AddRef();
	manager->device = device;
	device->AddRef();
	manager->unknown8C = 0;
	manager->unknown78 = manager->unknown80 = deviceDesc->dwMaxTextureWidth;
	manager->unknown7C = manager->unknown84 = deviceDesc->dwMaxTextureHeight;
	if ( deviceDesc->dwDevCaps & D3DDEVCAPS_TEXTUREVIDEOMEMORY ) {
		manager->unknownA8 |= TEXTURE_MANAGER_VIDEO_MEMORY;
	} else {
		manager->unknownA8 &= ~TEXTURE_MANAGER_VIDEO_MEMORY;
	}
	if ( deviceDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY ) {
		manager->unknownA8 |= TEXTURE_MANAGER_SQUARE;
	} else {
		manager->unknownA8 &= ~TEXTURE_MANAGER_SQUARE;
	}
	if ( trueColor ) {
		manager->unknownA8 |= TEXTURE_MANAGER_TRUE_COLOR;
	} else {
		manager->unknownA8 &= ~TEXTURE_MANAGER_TRUE_COLOR;
	}

	if ( !manager->QueryTextureFormats( device ) ) {
		delete manager;
		return NULL;
	}

	unsigned int i;
	manager->tables[0] = new _DPSetupTexture *[256];
	manager->unknownC8[0] = new _DPSetupTexture *[256];
	for ( i = 0; i < 256; i++ ) {
		manager->unknownC8[0][i] = NULL;
		manager->tables[0][i] = NULL;
	}
	manager->unknownD0[0] = new unsigned int[16];
	for ( i = 0; i < 16; i++ ) {
		manager->unknownD0[0][i] = 0;
	}
	if ( ( deviceDesc->dwDevCaps & D3DDEVCAPS_SEPARATETEXTUREMEMORIES ) && multitexture ) {
		manager->unknownE1 = true;
		manager->tables[1] = new _DPSetupTexture *[256];
		manager->unknownC8[1] = new _DPSetupTexture *[256];
		for ( i = 0; i < 256; i++ ) {
			manager->unknownC8[1][i] = NULL;
			manager->tables[1][i] = NULL;
		}
		manager->unknownD0[1] = new unsigned int[16];
		for ( i = 0; i < 16; i++ ) {
			manager->unknownD0[1][i] = 0;
		}
	} else {
		manager->unknownE1 = false;
		manager->currentTable = 0;
	}

	NiD3DRender::Message( "(Creation): Texture manager creation succeeded" );
	return manager;
}

/*
================
NiD3DTextureManager::VirtualSlot01

The pixel format value (unknown20) of the image's surface, 7 without one.
================
*/
// FUNCTION: STBC 0x7D52B0
unsigned int NiD3DTextureManager::VirtualSlot01( NiImage *image ) {
	IDirectDrawSurface7 *ddSurface;	// (name invented; fits the /Od frame order)
	NiDDPixelDesc *format;
	if ( !image || !static_cast<NiDDImage *>( image )->Unknown7C6370( directDraw, unknown08, TestFlags( TEXTURE_MANAGER_TRUE_COLOR ), true, ddSurface, format )
		 || !ddSurface || !format ) {
		return 7;
	}
	return format->unknown20;
}

/*
================
NiD3DTextureManager::Unknown7D5310

Copies a surface's pixels into a texture, level by level: source levels
larger than the texture's top level are skipped. Takes a reference to both
chains and releases each level when done. The device is not used.
================
*/
// FUNCTION: STBC 0x7D5310
void NiD3DTextureManager::Unknown7D5310( IDirect3DDevice7 *device, IDirectDrawSurface7 *source, IDirectDrawSurface7 *texture ) {
	DDSCAPS2 caps;
	memset( &caps, 0, sizeof( caps ) );
	caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

	source->AddRef();
	texture->AddRef();

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	source->GetSurfaceDesc( &desc );
	unsigned int sourceWidth = desc.dwWidth;
	texture->GetSurfaceDesc( &desc );
	unsigned int textureWidth = desc.dwWidth;

	while ( sourceWidth > textureWidth ) {
		IDirectDrawSurface7 *level;
		if ( source->GetAttachedSurface( &caps, &level ) != DD_OK ) {
			level = NULL;
		}
		source->Release();
		source = level;
		sourceWidth >>= 1;
	}

	do {
		HRESULT result;
		do {
			result = texture->Blt( NULL, source, NULL, 0, NULL );
		} while ( result == DDERR_WASSTILLDRAWING );
		if ( result != DD_OK ) {
			OutputDebugString( "Texture Upload Failed\n" );
		}

		IDirectDrawSurface7 *nextSource;
		if ( source->GetAttachedSurface( &caps, &nextSource ) != DD_OK ) {
			nextSource = NULL;
		}
		IDirectDrawSurface7 *nextTexture;
		if ( texture->GetAttachedSurface( &caps, &nextTexture ) != DD_OK ) {
			nextTexture = NULL;
		}
		source->Release();
		texture->Release();
		source = nextSource;
		texture = nextTexture;
	} while ( source && texture );
}

/*
================
NiD3DTextureManager::Unknown7D5440

Makes a texture for an image in the current table: the texture surface,
its entry, linked at the end of its bucket, its palette and an empty
source color key, then copies the image's surface into it and counts its
bytes against the table.
================
*/
// FUNCTION: STBC 0x7D5440
bool NiD3DTextureManager::Unknown7D5440( NiDDImage *image, unsigned int key, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc,
										 _DPSetupTexture *&entry ) {
	if ( unknownE1 ) {
		desc.dwFlags |= DDSD_TEXTURESTAGE;
		desc.dwTextureStage = currentTable;
	}
	IDirectDrawSurface7 *texture = NULL;
	if ( directDraw->CreateSurface( &desc, &texture, NULL ) != DD_OK ) {
		return false;
	}

	entry = Unknown7D7240( image, key, currentTable );
	LinkEntry( entry );
	// BUG: a NULL entry (no free entry and no memory for a block) is used here
	entry->surface = texture;

	if ( desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP ) {
		entry->unknown08 |= 1;
	}
	if ( desc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS ) {
		entry->unknown08 |= 4;
	}
	if ( desc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 ) {
		IDirectDrawPalette *palette;
		if ( surface->GetPalette( &palette ) != DD_OK ) {
			return false;
		}
		if ( entry->surface->SetPalette( palette ) != DD_OK ) {
			return false;
		}
		palette->Release();
	}

	DDCOLORKEY colorKey;
	memset( &colorKey, 0, sizeof( colorKey ) );
	colorKey.dwColorSpaceLowValue = 0;
	colorKey.dwColorSpaceHighValue = 0;
	if ( entry->surface->SetColorKey( DDCKEY_SRCBLT, &colorKey ) != DD_OK ) {
		entry->surface->Release();
		return false;
	}

	Unknown7D5310( device, surface, entry->surface );
	entry->unknown20 = ( desc.ddpfPixelFormat.dwRGBBitCount >> 3 ) * desc.dwWidth * desc.dwHeight;
	unknownD8[currentTable] += entry->unknown20;
	return true;
}

/*
================
NiD3DTextureManager::Unknown7D5640
================
*/
// FUNCTION: STBC 0x7D5640
// The debug NetImmerse 3.1's statements (AnimOnly 0x52FADA).
bool NiD3DTextureManager::Unknown7D5640( unsigned int key, _DPSetupTexture *&entry ) {
	unsigned int bucket = GetBucket( key );
	_DPSetupTexture *found = tables[currentTable][bucket];
	while ( found ) {
		if ( found->key == key ) {
			break;
		}
		found = found->next;
	}
	if ( found ) {
		entry = found;
		return found->unknown1C < unknownA4 ? true : false;
	}
	entry = NULL;
	return false;
}

/*
================
NiD3DTextureManager::Unknown7D56A0

Gives an image an entry whose texture it reuses: the entry takes the
image's serial and presentation numbers, the image's surface is copied
into the texture with its palette, and the entry moves to the end of its
bucket.
================
*/
// FUNCTION: STBC 0x7D56A0
bool NiD3DTextureManager::Unknown7D56A0( NiDDImage *image, IDirectDrawSurface7 *surface, _DPSetupTexture *entry ) {
	entry->unknown14 = image->GetRevisionID();
	entry->unknown18 = image->unknown20;
	entry->image = image;
	image->SetEntry( ids[entry->table], entry );
	Unknown7D5310( device, surface, entry->surface );

	IDirectDrawPalette *palette;
	if ( surface->GetPalette( &palette ) == DD_OK && palette ) {
		if ( entry->surface->SetPalette( palette ) != DD_OK ) {
			return false;
		}
		palette->Release();
	}

	MoveEntryToEnd( entry );
	unknown9C++;
	return true;
}

/*
================
NiD3DTextureManager::Unknown7D57F0

The least recently used entry of the current table: the oldest head of
the buckets in the groups of 16 from group down to lowest. Without one,
lowest moves down to the next group that holds entries and the search
starts again there. Without any, only an entry older than the current
frame qualifies as the first candidate.
================
*/
// FUNCTION: STBC 0x7D57F0
// PARKED: loop optimisation: the debug NetImmerse 3.1's statements (AnimOnly 0x52FCA9); retail keeps the group live (its bucket base shifted per group, the group's count read through a pointer stepped down) where this build derives a trip count
NiTextureManager::_DPSetupTexture *NiD3DTextureManager::Unknown7D57F0( int &low, int &high, bool any ) {
	_DPSetupTexture *oldest = NULL;
	while ( !unknownD0[currentTable][high] && high > low ) {
		high--;
	}

	while ( 1 ) {
		for ( int group = high; group >= low; group-- ) {
			if ( unknownD0[currentTable][group] ) {
				int first = group * 16;
				int end = first + 16;
				for ( int bucket = first; bucket < end; bucket++ ) {
					_DPSetupTexture *entry = tables[currentTable][bucket];
					if ( entry ) {
						if ( oldest ) {
							if ( oldest->unknown1C > entry->unknown1C ) {
								oldest = entry;
							}
						} else if ( any || entry->unknown1C < unknownA4 ) {
							oldest = entry;
						}
					}
				}
			}
		}
		if ( !oldest ) {
			low--;
			while ( low >= 0 && !unknownD0[currentTable][low] ) {
				low--;
			}
			if ( low >= 0 ) {
				high = low;
				continue;
			}
			return NULL;
		}
		return oldest;
	}
	return oldest;
}

/*
================
NiD3DTextureManager::Unknown7D5910

Makes room for a texture: frees the least recently used entry, starting
from the groups of the key's buckets, until the texture can be made.
================
*/
// FUNCTION: STBC 0x7D5910
bool NiD3DTextureManager::Unknown7D5910( NiDDImage *image, unsigned int key, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc,
										 _DPSetupTexture *&entry, bool any ) {
	int low = ( key >> 15 ) & 15;
	int high = 15;
	while ( 1 ) {
		_DPSetupTexture *oldest = Unknown7D57F0( low, high, any );
		if ( !oldest ) {
			break;
		}
		// the debug copy's assert (AnimOnly 0x52FE5F, D3DTextureManager.cpp line 1057):
		// !(pMatch->uiFlags & DPSetupTexture::TEXT_LOCKED)
		assert( !( oldest->unknown08 & 2 ) );
		Unknown7D6650( oldest );
		if ( Unknown7D5440( image, key, surface, desc, entry ) ) {
			return true;
		}
	}
	return false;
}

/*
================
NiD3DTextureManager::Unknown7D5B30

Makes the texture of an image, making room when the texture memory is
full: a texture of the same key older than the current frame is reused,
else the least recently used textures older than the frame are freed,
else the found texture of the same key is reused, else any texture is
freed. unknownA0 records which way was taken.
================
*/
// FUNCTION: STBC 0x7D5B30
// The debug NetImmerse 3.1's statements (AnimOnly 0x52FF24): the first eviction is Unknown7D5910's,
// which retail expands here and calls for the second.
bool NiD3DTextureManager::Unknown7D5B30( NiDDImage *image, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc, unsigned int key,
										 _DPSetupTexture *&entry ) {
	entry = NULL;
	if ( !Unknown7D5440( image, key, surface, desc, entry ) ) {
		if ( unknownE0 ) {
			return false;
		}

		if ( Unknown7D5640( key, entry ) ) {
			Unknown7D56A0( image, surface, entry );
			unknownA0 |= 2;
			return true;
		}

		_DPSetupTexture *found = entry;
		if ( !Unknown7D5910( image, key, surface, desc, entry, false ) ) {
			if ( found ) {
				Unknown7D56A0( image, surface, entry );
				unknownA0 |= 8;
				return true;
			}

			unknownA0 |= 0x10;
			if ( !Unknown7D5910( image, key, surface, desc, entry, true ) ) {
				unknownA0 |= 0x20;
				return false;
			}
		} else {
			unknownA0 |= 4;
			return true;
		}
	}
	return true;
}

/*
================
NiD3DTextureManager::VirtualSlot05

Purges textures: an image's textures that are still current, or with no
image, every texture of a table, or of both tables for -1. The textures
of the lists of kept entries (unknownC0) go only when asked.
================
*/
// FUNCTION: STBC 0x7D5E50
bool NiD3DTextureManager::VirtualSlot05( NiImage *image, bool kept, int table ) {
	if ( !unknownE1 ) {
		table = 0;
	}

	if ( image ) {
		NiDDImage *ddImage = static_cast<NiDDImage *>( image );
		_DPSetupTexture *entry = ddImage->GetEntry( ids[0] );
		if ( entry && entry->unknown14 == image->GetRevisionID() ) {
			Unknown7D6650( entry );
		}
		if ( unknownE1 ) {
			entry = ddImage->GetEntry( ids[1] );
			if ( entry && entry->unknown14 == image->GetRevisionID() ) {
				Unknown7D6650( entry );
			}
		}
		return true;
	}

	unsigned int i;
	if ( table == -1 ) {
		currentTable = 0;
		for ( i = 0; i < 256; i++ ) {
			while ( tables[0][i] ) {
				Unknown7D6650( tables[0][i] );
			}
		}
		currentTable = 1;
		for ( i = 0; i < 256; i++ ) {
			while ( tables[1][i] ) {
				Unknown7D6650( tables[1][i] );
			}
		}
		if ( kept ) {
			currentTable = 0;
			while ( unknownC0[0] ) {
				Unknown7D6650( unknownC0[0] );
			}
			currentTable = 1;
			while ( unknownC0[1] ) {
				Unknown7D6650( unknownC0[1] );
			}
		}
		return true;
	}

	currentTable = table;
	for ( i = 0; i < 256; i++ ) {
		while ( tables[currentTable][i] ) {
			Unknown7D6650( tables[currentTable][i] );
		}
	}
	if ( kept ) {
		while ( unknownC0[currentTable] ) {
			Unknown7D6650( unknownC0[currentTable] );
		}
	}
	return true;
}

/*
================
NiD3DTextureManager::VirtualSlot07

Marks every entry of the tables in use.
================
*/
// FUNCTION: STBC 0x7D6240
void NiD3DTextureManager::VirtualSlot07() {
	unsigned int i;
	_DPSetupTexture *entry;
	for ( i = 0; i < 256; i++ ) {
		for ( entry = tables[0][i]; entry; entry = entry->next ) {
			entry->unknown18 = -1;
		}
	}
	if ( unknownE1 ) {
		for ( i = 0; i < 256; i++ ) {
			for ( entry = tables[1][i]; entry; entry = entry->next ) {
				entry->unknown18 = -1;
			}
		}
	}
}

/*
================
NiD3DTextureManager::IsPowerOfTwo
================
*/
inline bool NiD3DTextureManager::IsPowerOfTwo( unsigned int value ) {
	if ( !value ) {
		return false;
	}
	while ( !( value & 1 ) ) {
		value >>= 1;
	}
	value >>= 1;
	return value == 0;
}

/*
================
NiD3DTextureManager::Unknown7D62A0
================
*/
// FUNCTION: STBC 0x7D62A0
bool NiD3DTextureManager::Unknown7D62A0( NiDDImage *image ) {
	if ( !image ) {
		return true;
	}

	if ( !image->GetWidth() ) {
		NiDDPixelDesc *format = NULL;
		IDirectDrawSurface7 *ddSurface = NULL;	// (name invented; fits the /Od frame order)
		if ( !image->Unknown7C6370( directDraw, unknown08, TestFlags( TEXTURE_MANAGER_TRUE_COLOR ), true, ddSurface, format ) || !ddSurface ) {
			unsigned int error = NiImage::GetError();
			if ( error == 5 || error == 7 ) {
				NiD3DRender::Error( "(TexDebug): Create Texture failed: image file not found or of invalid format" );
			} else {
				NiD3DRender::Error( "(TexDebug): Create Texture failed: image get failed (possible DirectDraw error)" );
			}
			return false;
		}
	}

	if ( !IsPowerOfTwo( image->GetWidth() ) || !IsPowerOfTwo( image->GetHeight() ) ) {
		return false;
	}
	return true;
}

/*
================
NiD3DTextureManager::MakeKey

The texture key: the mipmap bias of the format's bytes per pixel and the
size, the aspect, the format, mipmapping and the size's logarithms.
================
*/
inline unsigned int NiD3DTextureManager::MakeKey( unsigned int widthLog, unsigned int heightLog, unsigned int formatIndex,
												  bool mipmapped ) {
	int aspect = widthLog - heightLog;
	unsigned int aspectBits;
	if ( aspect >= 3 ) {
		aspectBits = 7 << 12;
	} else if ( aspect <= -4 ) {
		aspectBits = 0;
	} else {
		aspectBits = ( aspect + 4 ) << 12;
	}

	int bias = unknown24[formatIndex] + widthLog + heightLog - 8;
	unsigned int biasBits;
	if ( bias < 0 ) {
		biasBits = 0;
	} else if ( bias >= 15 ) {
		biasBits = 15 << 15;
	} else {
		biasBits = ( bias & 15 ) << 15;
	}

	return biasBits | aspectBits | ( ( formatIndex + 1 ) << 9 ) | ( mipmapped ? 0x100 : 0 ) | ( widthLog << 4 ) | heightLog;
}

/*
================
NiD3DTextureManager::Unknown7D6380

The size limits (unknown78, unknown7C) skip the image's largest mipmap
levels, from unknown88 on; the last level always stays. Hardware that
wants square textures (bit 1 of unknownA8) refuses others. Statements as
the debug copy has them (AnimOnly.exe 0x530510, D3DTextureManager.cpp),
which computes the key in MakeKey.
================
*/
// FUNCTION: STBC 0x7D6380
bool NiD3DTextureManager::Unknown7D6380( NiDDImage *image, IDirectDrawSurface7 *&surface, DDSURFACEDESC2 &desc, unsigned int &key ) {
	assert( image );
	surface = NULL;
	NiDDPixelDesc *format = NULL;
	if ( !image->Unknown7C6370( directDraw, unknown08, TestFlags( TEXTURE_MANAGER_TRUE_COLOR ), true, surface, format ) || !surface ) {
		return false;
	}

	unsigned int skip = unknown88;
	if ( unknown78 ) {
		unsigned int width = image->GetWidth();
		while ( ( width >> skip ) > unknown78 ) {
			skip++;
		}
	}
	if ( unknown7C ) {
		unsigned int height = image->GetHeight();
		while ( ( height >> skip ) > unknown7C ) {
			skip++;
		}
	}

	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	HRESULT result = surface->GetSurfaceDesc( &desc );
	if ( result != DD_OK ) {
		NiD3DRender::Error( "(TexDebug): Texture surface get description failed" );
		surface->Release();
		return false;
	}

	if ( !desc.dwMipMapCount ) {
		desc.dwMipMapCount = 1;
	}
	if ( skip >= desc.dwMipMapCount ) {
		skip = desc.dwMipMapCount - 1;
	}
	desc.dwWidth >>= skip;
	desc.dwHeight >>= skip;
	desc.dwMipMapCount -= skip;
	assert( desc.dwMipMapCount );

	unsigned int widthLog = 0;
	unsigned int heightLog = 0;
	unsigned int value = desc.dwWidth >> 1;
	while ( value ) {
		value >>= 1;
		widthLog++;
	}
	value = desc.dwHeight >> 1;
	while ( value ) {
		value >>= 1;
		heightLog++;
	}

	if ( TestFlags( TEXTURE_MANAGER_SQUARE ) && widthLog != heightLog ) {
		NiD3DRender::Error( "(TexDebug): Non-square texture cannot be loaded to this hardware" );
		surface->Release();
		return false;
	}

	key = MakeKey( widthLog, heightLog, format->unknown20, ( desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP ) != 0 );
	return true;
}

/*
================
NiD3DTextureManager::Unknown7D6550

Makes the texture of an image in the current table: a mipmapped texture
when the image's surface has mipmaps, in video memory when the manager
asks for it.
================
*/
// FUNCTION: STBC 0x7D6550
bool NiD3DTextureManager::Unknown7D6550( NiDDImage *image ) {
	unsigned int key;
	IDirectDrawSurface7 *surface;
	_DPSetupTexture *entry = image->GetEntry( ids[currentTable] );
	DDSURFACEDESC2 desc;
	if ( !Unknown7D6380( image, surface, desc, key ) ) {
		return false;
	}

	if ( desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP ) {
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
	} else {
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	}
	desc.ddsCaps.dwCaps |= TestFlags( TEXTURE_MANAGER_VIDEO_MEMORY ) ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY;
	if ( !Unknown7D5B30( image, surface, desc, key, entry ) ) {
		return false;
	}
	unknown98++;
	return true;
}
/*
================
NiD3DTextureManager::Unknown7D6800

Fills an entry's texture again from its image: in place when the image's
texture key is unchanged, else the entry is freed and the image gets a
new texture, which stays in the list of kept entries when the old one
was kept.
================
*/
// FUNCTION: STBC 0x7D6800
bool NiD3DTextureManager::Unknown7D6800( _DPSetupTexture *entry, NiDDImage *image ) {
	IDirectDrawSurface7 *surface;
	DDSURFACEDESC2 desc;
	unsigned int key;
	if ( !Unknown7D6380( image, surface, desc, key ) ) {
		return false;
	}

	if ( entry->key == key ) {
		Unknown7D5310( device, surface, entry->surface );
		entry->unknown18 = image->unknown20;
		return true;
	}

	bool kept = ( entry->unknown08 >> 1 ) & 1;
	Unknown7D6650( entry );

	if ( desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP ) {
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
	} else {
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	}
	desc.ddsCaps.dwCaps |= TestFlags( TEXTURE_MANAGER_VIDEO_MEMORY ) ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY;
	if ( !Unknown7D5B30( image, surface, desc, key, entry ) ) {
		return false;
	}

	if ( kept ) {
		Unknown7D73D0( entry );
		if ( unknownC0[currentTable] ) {
			unknownC0[currentTable]->prev = entry;
		}
		unknownC0[currentTable] = entry;
		entry->prev = NULL;
	}
	unknown98++;
	return true;
}

/*
================
NiD3DTextureManager::VirtualSlot06

The texture of an image for a stage whose texture was current: whether
the stage needs it set, and whether it has mipmaps. A frame buffer image
gives its own surface. The texture comes from the given table, or for -1
from the table that holds it, else the one with fewer texture bytes; it
is made when missing or out of date, refilled when the image changed,
and moved to the end of its bucket as the most recently used.
================
*/
// FUNCTION: STBC 0x7D6B50
bool NiD3DTextureManager::VirtualSlot06( int table, NiImage *image, NiPointer<NiImage> &current, IDirectDrawSurface7 *&surface,
										 bool &changed, bool &mipmapped ) {
	// AnimOnly.exe 0x530C40 (D3DTextureManager.cpp)
	NiImage *previous = current;
	_DPSetupTexture *entry = NULL;
	changed = false;
	mipmapped = false;
	if ( previous ) {
		if ( image ) {
			if ( image->GetRevisionID() != previous->GetRevisionID() ) {
				changed = true;
			}
		} else {
			changed = true;
		}
	} else if ( image ) {
		changed = true;
	}
	current = image;

	if ( !image ) {
		surface = NULL;
	} else {
		if ( NiIsExactKindOf( NiDDBufferImage, image ) ) {
			static_cast<NiDDBufferImage *>( image )->Unknown7C70C0( &surface );
			return true;
		}

		NiDDImage *ddImage = static_cast<NiDDImage *>( image );
		if ( !unknownE1 ) {
			entry = ddImage->GetEntry( ids[0] );
			currentTable = 0;
		} else if ( table == -1 ) {
			entry = ddImage->GetEntry( ids[0] );
			currentTable = 0;
			if ( !entry ) {
				entry = ddImage->GetEntry( ids[1] );
				currentTable = 1;
				if ( !entry ) {
					currentTable = unknownD8[0] >= unknownD8[1];
				}
			}
		} else {
			entry = ddImage->GetEntry( ids[table] );
			currentTable = table;
		}

		if ( !entry ) {
			if ( !Unknown7D62A0( ddImage ) ) {
				ddImage->SetEntry( ids[currentTable], unknown74 );
				surface = NULL;
				return false;
			}
		} else if ( entry == unknown74 ) {
			surface = NULL;
			return false;
		}

		if ( !entry || entry->unknown14 != image->GetRevisionID() ) {
			if ( !Unknown7D6550( ddImage ) ) {
				surface = NULL;
				return false;
			}
			changed = true;
		} else {
			if ( entry->unknown18 != image->unknown20 && !Unknown7D6800( entry, ddImage ) ) {
				surface = NULL;
				return false;
			}
			MoveEntryToEnd( entry );
		}

		entry = ddImage->GetEntry( ids[currentTable] );
		if ( entry && entry->unknown1C < unknownA4 ) {
			entry->unknown1C = unknownA4;
			unknown90++;
		}
	}

	if ( entry ) {
		mipmapped = ( entry->unknown08 & 1 ) != 0;
		surface = entry->surface;
	} else {
		surface = NULL;
	}
	return true;
}

/*
================
NiD3DTextureManager::VirtualSlot02

Makes the image's surface for the manager's DirectDraw object.
================
*/
// FUNCTION: STBC 0x7D6F30
bool NiD3DTextureManager::VirtualSlot02( NiDDImage *image ) {
	IDirectDrawSurface7 *surface = NULL;
	NiDDPixelDesc *format = NULL;
	return image->Unknown7C6370( directDraw, unknown08, TestFlags( TEXTURE_MANAGER_TRUE_COLOR ), true, surface, format );
}

/*
================
NiD3DTextureManager::VirtualSlot03

Makes sure an image has a texture in one of the tables: the given table,
or with the second table in use and no table given, the table that
already holds it, else the one with fewer texture bytes. An image that
cannot be a texture gets the manager's empty entry. An image whose serial
number changed is made again. A kept texture moves to the table's list of
entries flagged 2.
================
*/
// FUNCTION: STBC 0x7D6F70
bool NiD3DTextureManager::VirtualSlot03( NiDDImage *image, bool unknown1, bool keep, int table ) {
	_DPSetupTexture *entry = NULL;
	if ( !unknownE1 ) {
		entry = image->GetEntry( ids[0] );
		currentTable = 0;
	} else if ( table != 0 && table != 1 ) {
		entry = image->GetEntry( ids[0] );
		currentTable = 0;
		if ( !entry ) {
			entry = image->GetEntry( ids[1] );
			currentTable = 1;
		}
		if ( !entry ) {
			currentTable = unknownD8[0] >= unknownD8[1];
		}
	} else {
		entry = image->GetEntry( ids[table] );
		currentTable = table;
	}

	if ( !unknown1 ) {
		unknownE0 = true;
	}

	if ( !entry ) {
		if ( !Unknown7D62A0( image ) ) {
			image->SetEntry( ids[currentTable], unknown74 );
			unknownE0 = false;
			return false;
		}
	} else if ( entry == unknown74 ) {
		unknownE0 = false;
		return false;
	}

	if ( !entry || entry->unknown14 != image->GetRevisionID() ) {
		if ( !Unknown7D6550( image ) ) {
			unknownE0 = false;
			return false;
		}
		entry = image->GetEntry( ids[currentTable] );
	}

	if ( keep && !( entry->unknown08 & 2 ) ) {
		Unknown7D73D0( entry );
		entry->unknown08 |= 2;
		if ( unknownC0[currentTable] ) {
			unknownC0[currentTable]->prev = entry;
		}
		unknownC0[currentTable] = entry;
		entry->prev = NULL;
	}
	unknownE0 = false;
	return true;
}

/*
================
NiD3DTextureManager::Unknown7D7240

The block allocated here chains only its first 31 entries: the loop stops
one entry short of the constructor's, so the last entry of every later
block is never handed out (an original bug, kept).
================
*/
// FUNCTION: STBC 0x7D7240
NiTextureManager::_DPSetupTexture *NiD3DTextureManager::Unknown7D7240( NiDDImage *image, unsigned int key, unsigned int table ) {
	if ( !freeEntries ) {
		freeEntries = (_DPSetupTexture *)calloc( ENTRY_BLOCK_SIZE, sizeof( _DPSetupTexture ) );
		if ( !freeEntries ) {
			return NULL;
		}
		entryBlocks->AddHead( freeEntries );
		memset( freeEntries, 0, ENTRY_BLOCK_SIZE * sizeof( _DPSetupTexture ) );
		for ( unsigned int i = 1; i < ENTRY_BLOCK_SIZE - 1; i++ ) {
			freeEntries[i - 1].next = &freeEntries[i];
		}
		freeEntries[ENTRY_BLOCK_SIZE - 1].next = NULL;
	}

	_DPSetupTexture *entry = freeEntries;
	freeEntries = entry->next;
	entry->next = NULL;
	entry->image = image;
	image->SetEntry( ids[table], entry );
	entry->table = table;
	entry->unknown14 = image->GetRevisionID();
	entry->unknown18 = image->unknown20;
	entry->unknown1C = 0;
	entry->unknown08 = 0;
	entry->surface = NULL;
	entry->key = key;
	return entry;
}

/*
================
NiD3DTextureManager::Unknown7D74E0

The texture key of a surface description, as Unknown7D6380 builds it,
with the pixel size in bytes in place of the format's bias and no format
index.
================
*/
// FUNCTION: STBC 0x7D74E0
unsigned int NiD3DTextureManager::Unknown7D74E0( const DDSURFACEDESC2 &desc ) {
	unsigned int widthLog = 0;
	unsigned int heightLog = 0;
	unsigned int value = desc.dwWidth >> 1;
	while ( value ) {
		value >>= 1;
		widthLog++;
	}
	value = desc.dwHeight >> 1;
	while ( value ) {
		value >>= 1;
		heightLog++;
	}

	int bytes = 1;
	switch ( desc.ddpfPixelFormat.dwRGBBitCount ) {
		case 8:
			bytes = 1;
			break;
		case 16:
			bytes = 2;
			break;
		case 24:
			bytes = 3;
			break;
		case 32:
			bytes = 4;
			break;
	}

	bool mipmapped = ( desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP ) != 0;

	int aspect = widthLog - heightLog;
	unsigned int aspectBits;
	if ( aspect >= 3 ) {
		aspectBits = 7 << 12;
	} else if ( aspect <= -4 ) {
		aspectBits = 0;
	} else {
		aspectBits = ( aspect + 4 ) << 12;
	}

	int bias = bytes + heightLog + widthLog - 8;
	unsigned int biasBits;
	if ( bias < 0 ) {
		biasBits = 0;
	} else if ( bias >= 15 ) {
		biasBits = 15 << 15;
	} else {
		biasBits = ( bias & 15 ) << 15;
	}

	return ( mipmapped ? 0x100 : 0 ) | widthLog << 4 | biasBits | aspectBits | heightLog;
}

/*
================
NiD3DTextureManager::VirtualSlot04

A 16 by 16 texture in the pixel format of a texture stage, for the
device stage given unless it is -1; a palettized one gets a black
palette.
================
*/
// FUNCTION: STBC 0x7D75E0
IDirectDrawSurface7 *NiD3DTextureManager::VirtualSlot04( unsigned int format, int stage ) {
	const NiDDPixelDesc *entry = unknown08[format];
	if ( !entry ) {
		return NULL;
	}

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	entry->GetDDPixelFormat( &desc.ddpfPixelFormat );
	desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	desc.ddsCaps.dwCaps = ( TestFlags( TEXTURE_MANAGER_VIDEO_MEMORY ) ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY ) | DDSCAPS_TEXTURE;
	if ( !entry->palettized ) {
		desc.ddsCaps.dwCaps |= DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
	}
	desc.dwHeight = 16;
	desc.dwWidth = 16;
	if ( stage != -1 ) {
		desc.dwFlags |= DDSD_TEXTURESTAGE;
		desc.dwTextureStage = stage;
	}

	IDirectDrawSurface7 *surface;
	if ( directDraw->CreateSurface( &desc, &surface, NULL ) != DD_OK ) {
		return NULL;
	}

	if ( entry->palettized ) {
		PALETTEENTRY colors[256];
		memset( colors, 0, sizeof( colors ) );
		IDirectDrawPalette *palette = NULL;
		if ( directDraw->CreatePalette( DDPCAPS_8BIT | DDPCAPS_ALLOW256, colors, &palette, NULL ) != DD_OK ) {
			surface->Release();
			return NULL;
		}
		surface->SetPalette( palette );
		palette->Release();
	}
	return surface;
}


