// NiDDImage.cpp -- the Direct3D renderer's images

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiDDImage.h"
#include "NiD3DPixelFormat.h"
#include "NiD3DRender.h"
#include "NiRawImageData.h"
#include "NiFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007c7a80 _$E239
// FUNCTION: STBC 0x007c72d0 _$E17
// FUNCTION: STBC 0x007c72f0 _$E15
// FUNCTION: STBC 0x007c7300 _$E23
// FUNCTION: STBC 0x007c7320 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007c7510 _$E30
// FUNCTION: STBC 0x007c75a0 _$E44
// FUNCTION: STBC 0x007c75b0 _$E57
// FUNCTION: STBC 0x007c7b40 _$E67
// FUNCTION: STBC 0x007c7b50 _$E71
// FUNCTION: STBC 0x007c7b60 _$E75
// FUNCTION: STBC 0x007c7b70 _$E79
// FUNCTION: STBC 0x007c7b80 _$E83
// FUNCTION: STBC 0x007c7b90 _$E187
// FUNCTION: STBC 0x007c7ba0 _$E192
// FUNCTION: STBC 0x007c7bb0 _$E197

// FUNCTION: STBC 0x007c4cc0 _$E221
// FUNCTION: STBC 0x007c4ce0 _$E218
// DATA: STBC 0x9A1218 ?rtti@NiDDImage@@2VNiRTTI@@B
const NiRTTI NiDDImage::rtti( "NiDDImage", &NiImage::rtti );

// FUNCTION: STBC 0x007c4d00 _$E226
// FUNCTION: STBC 0x007c4d20 _$E224
// DATA: STBC 0x9A1230 ?rtti@NiDDBufferImage@@2VNiRTTI@@B
const NiRTTI NiDDBufferImage::rtti( "NiDDBufferImage", &NiDDImage::rtti );

// DATA: STBC 0x9A123C ?instances@NiDDImage@@1IA
unsigned int NiDDImage::instances;

/*
================
NiDDImage::NiDDImage

Counts the image and adds it to the list of images. Defined here, out of
line: retail keeps the copy (0x7C4DF0) first in the unit, and the unit's
creation functions expand it.
================
*/
// FUNCTION: STBC 0x7C4DF0
NiDDImage::NiDDImage() : surfaces( 2 ) {
	surfacePos = NULL;
	entries.entryId = 0;
	entries.next = NULL;
	unknown58 = NULL;
	instances++;
	images.AddTail( this );
}

/*
================
NiDDImage::~NiDDImage

Releases the surfaces, lets the texture managers drop the image, frees
the image's blocks and leaves the list of images.
================
*/
// FUNCTION: STBC 0x7C4F50
// FUNCTION: STBC 0x7C4F30 ??_GNiDDImage@@UAEPAXI@Z
NiDDImage::~NiDDImage() {
	NiDDImage::VirtualSlot13( NULL );
	NiD3DRender::Unknown7C2EB0( this );
	instances--;

	NiDDImageBlock *block = entries.next;
	while ( block ) {
		NiDDImageBlock *dead = block;
		block = block->next;
		delete dead;
	}
	delete unknown58;

	images.Remove( this );
}

// The list's guard byte is not the NiRTTIs' (0x9A11E5, not 0x9A11E4), so
// a function definition separates them.
// FUNCTION: STBC 0x007c4d40 _$E232
// FUNCTION: STBC 0x007c4d80 _$E229
// DATA: STBC 0x9A11E8 ?images@NiDDImage@@1V?$NiTList@PAVNiDDImage@@@@A
NiTList<NiDDImage *> NiDDImage::images( 2 );

/*
================
NiDDImage::ReleaseAllSurfaces

Releases the surfaces of every image; false when an image fails.
================
*/
// FUNCTION: STBC 0x7C50E0
bool NiDDImage::ReleaseAllSurfaces( IDirectDraw7 *directDraw ) {
	NiTListIterator pos = images.GetHeadPos();
	while ( pos ) {
		if ( !images.GetNext( pos )->VirtualSlot13( directDraw ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiDDImage::RemoveAllEntries

Drops the texture entry of a manager table from every image. An image whose
first entry goes takes the next one of the chain in its place.
================
*/
// FUNCTION: STBC 0x7C5120
void NiDDImage::RemoveAllEntries( unsigned int id ) {
	NiTListIterator pos = images.GetHeadPos();
	while ( pos ) {
		NiDDImage *image = images.GetNext( pos );
		if ( image->entries.entryId == id ) {
			NiDDImageBlock *block = image->entries.next;
			if ( block ) {
				image->entries.entryId = block->entryId;
				image->entries.entry = block->entry;
				image->entries.next = block->next;
				delete block;
			} else {
				image->entries.entryId = 0;
			}
		} else if ( image->entries.next ) {
			NiDDImageBlock *prev = &image->entries;
			NiDDImageBlock *block = prev->next;
			while ( block && block->entryId != id ) {
				prev = block;
				block = block->next;
			}
			if ( block ) {
				prev->next = block->next;
				delete block;
			}
		}
	}
}

/*
================
NiDDImage::VirtualSlot13

Releases every surface made from the image.
================
*/
// FUNCTION: STBC 0x7C51A0
bool NiDDImage::VirtualSlot13( void *unknown ) {
	while ( surfaces.GetSize() ) {
		NiDDImageSurface *entry = surfaces.RemoveHead();
		if ( entry->surface ) {
			entry->surface->Release();
		}
		delete entry;
	}
	return true;
}

/*
================
NiDDImage::CreateSurface

Makes a texture surface in system memory for the image's size and a
format; a palettized format gets no mipmaps. The last argument is unused.
================
*/
// FUNCTION: STBC 0x7C5200
bool NiDDImage::CreateSurface( IDirectDraw7 *directDraw, const NiDDPixelDesc &format,
							   IDirectDrawSurface7 *&surface, bool unused ) {
	if ( !directDraw ) {
		error = 12;
		return false;
	}

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	format.GetDDPixelFormat( &desc.ddpfPixelFormat );
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	desc.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
	desc.ddsCaps.dwCaps |= DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	if ( !format.palettized && TestFlags( 2 ) ) {
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
		desc.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
		desc.dwMipMapCount = mipLevels;
	}
	desc.dwHeight = height;
	desc.dwWidth = width;

	if ( directDraw->CreateSurface( &desc, &surface, NULL ) != DD_OK ) {
		error = 11;
		surface = NULL;
		return false;
	}
	return true;
}

/*
================
NextMipmapLevel

Steps a surface reference to the surface's next mipmap level, NULL after
the last, and releases the surface it held (name invented).
================
*/
// FUNCTION: STBC 0x7C5960
static void NextMipmapLevel( IDirectDrawSurface7 *&surface ) {
	DDSCAPS2 caps;
	memset( &caps, 0, sizeof( caps ) );
	caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
	IDirectDrawSurface7 *next;
	HRESULT result = surface->GetAttachedSurface( &caps, &next );
	if ( result != DD_OK ) {
		next = NULL;
	}
	if ( surface ) {
		surface->Release();
	}
	surface = next;
}

/*
================
CopyToSurface

Copies raw pixels, converted to a pixel format, into a surface; false when
the surface cannot be locked or unlocked (name invented).
================
*/
static bool CopyToSurface( NiRawImageData *raw, const NiPixelDesc *format, IDirectDrawSurface7 *surface ) {
	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	HRESULT result;
	while ( ( result = surface->Lock( NULL, &desc, DDLOCK_WAIT, NULL ) ) == DDERR_SURFACEBUSY ) {
	}
	if ( result != DD_OK ) {
		return false;
	}
	if ( !raw->Unknown7DEEE0( *format, desc.lPitch, (unsigned char *)desc.lpSurface ) ) {
		return false;
	}
	result = surface->Unlock( NULL );
	if ( result != DD_OK && result != DDERR_NOTLOCKED ) {
		return false;
	}
	return true;
}

/*
================
NiDDImage::Unknown7C5330

Copies raw pixels of the image's size into a surface in a format, and
into each of the surface's mipmap levels a copy of the pixels reduced to
the level's size. Each reduction is made from the one before: two
reduced images take turns as the source and the result.
================
*/
// FUNCTION: STBC 0x7C5330
bool NiDDImage::Unknown7C5330( NiRawImageData *raw, const NiDDPixelDesc &format, IDirectDrawSurface7 *surface ) {
	if ( !surface ) {
		error = 17;
		return false;
	}
	if ( raw->GetWidth() != width || raw->GetHeight() != height ) {
		error = 15;
		return false;
	}

	IDirectDrawSurface7 *level = surface;
	level->AddRef();
	if ( !CopyToSurface( raw, &format, level ) ) {
		return false;
	}
	NextMipmapLevel( level );
	if ( !level ) {
		return true;
	}

	NiRawImageData::Format reducedFormat = raw->GetFormat();
	if ( reducedFormat == NiRawImageData::FORMAT_PALETTIZED ) {
		reducedFormat = NiRawImageData::FORMAT_RGB;
	}
	NiPointer<NiRawImageData> reduced[2];

	reduced[0] = new NiRawImageData( reducedFormat, raw->GetWidth() >> 1, raw->GetHeight() >> 1 );
	reduced[0]->Unknown7DE990( raw );
	if ( !CopyToSurface( reduced[0], &format, level ) ) {
		return false;
	}
	NextMipmapLevel( level );
	if ( !level ) {
		return true;
	}

	reduced[1] = new NiRawImageData( reducedFormat, raw->GetWidth() >> 2, raw->GetHeight() >> 2 );
	reduced[1]->Unknown7DE990( reduced[0] );
	if ( !CopyToSurface( reduced[1], &format, level ) ) {
		return false;
	}
	NextMipmapLevel( level );

	while ( level ) {
		reduced[0]->Unknown7DE640( reduced[1]->GetWidth() >> 1, reduced[1]->GetHeight() >> 1 );
		reduced[0]->Unknown7DE990( reduced[1] );
		if ( !CopyToSurface( reduced[0], &format, level ) ) {
			return false;
		}

		NiPointer<NiRawImageData> swap = reduced[0];
		reduced[0] = reduced[1];
		reduced[1] = swap;
		NextMipmapLevel( level );
	}
	return true;
}

/*
================
NiDDImage::Unknown7C59E0

Makes the image's surface from its buffer (unknown58), in the buffer's own
format: the single format when no list of formats is given, else the
first format of the list that is the buffer's. The surface and each of its
mipmap levels take the buffer's pixels in place. Statements as the debug
copy has them (AnimOnly.exe 0x516532, NiDDImage.cpp).
================
*/
// FUNCTION: STBC 0x7C59E0
bool NiDDImage::Unknown7C59E0( IDirectDraw7 *directDraw, const NiDDPixelDesc *&format, bool unknown3,
							   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *single,
							   const NiDDPixelDesc **formats ) {
	assert( width && height );
	assert( unknown58 );
	if ( !formats ) {
		assert( single );
		if ( single->GetIndex() != unknown58->format ) {
			return false;
		}
		format = single;
	} else {
		assert( !single );
		format = NULL;
		for ( unsigned int i = 0; i < 7; i++ ) {
			// the original reads every entry, though the list may hold NULLs
			if ( formats[i]->GetIndex() == unknown58->format ) {
				format = formats[i];
				break;
			}
		}
		if ( !format ) {
			return false;
		}
	}

	if ( !format || !CreateSurface( directDraw, *format, surface, unknown3 ) ) {
		return false;
	}

	NiDDImageSurface *entry = new NiDDImageSurface;
	entry->format = format->GetIndex();
	entry->unknown0C = false;
	entry->surface = surface;
	entry->directDraw = directDraw;

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	surface->AddRef();

	unsigned char *pixels = unknown58->buffer;
	unsigned int levelWidth = width;
	unsigned int levelHeight = height;
	unsigned int bytesPerPixel = format->bitsPerPixel >> 3;
	for ( unsigned int level = 0; level < mipLevels; level++ ) {
		surface->GetSurfaceDesc( &desc );
		unsigned int pitch = levelWidth * bytesPerPixel;
		if ( pitch & 7 ) {
			// the original takes the padding for the pitch
			pitch = 8 - ( pitch & 7 );
		}
		desc.lpSurface = pixels;
		desc.lPitch = pitch;
		desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_LPSURFACE | DDSD_PIXELFORMAT;
		surface->SetSurfaceDesc( &desc, 0 );
		pixels += levelWidth * levelHeight * pitch;
		levelWidth >>= 1;
		levelHeight >>= 1;
		NextMipmapLevel( surface );
	}

	surfaces.AddTail( entry );
	surface = entry->surface;
	AddFlags( 1 );
	return true;
}

/*
================
ChooseFormat

The format of a list for pixels of a kind (0 palettized, 1 true color,
2 with alpha): the preferred format when the list has it and it suits the
pixels, else the first the list has of the formats for the kind, deepest
first or shallowest first (name invented).
================
*/
static const NiDDPixelDesc *ChooseFormat( int kind, bool mipmapped, bool deep, const NiDDPixelDesc **formats,
											 int preferred ) {
	assert( formats );

	// palettized pixels with mipmaps take the true-color formats
	if ( mipmapped && !kind ) {
		kind = 1;
	}

	if ( preferred == NiImage::TEX_DEFAULT || ( preferred == NiImage::TEX_PAL8 && kind ) || !formats[preferred]
		 || preferred > NiImage::TEX_DEFAULT ) {
		if ( deep ) {
			switch ( kind ) {
			case 0:
				if ( formats[NiImage::TEX_PAL8] ) {
					return formats[NiImage::TEX_PAL8];
				}
				// fall through
			case 1:
				if ( formats[NiImage::TEX_RGB888] ) {
					return formats[NiImage::TEX_RGB888];
				}
				if ( formats[NiImage::TEX_RGB565] ) {
					return formats[NiImage::TEX_RGB565];
				}
				if ( formats[NiImage::TEX_RGBA8888] ) {
					return formats[NiImage::TEX_RGBA8888];
				}
				if ( formats[NiImage::TEX_RGBA5551] ) {
					return formats[NiImage::TEX_RGBA5551];
				}
				if ( formats[NiImage::TEX_RGBA4444] ) {
					return formats[NiImage::TEX_RGBA4444];
				}
				break;
			case 2:
				if ( formats[NiImage::TEX_RGBA8888] ) {
					return formats[NiImage::TEX_RGBA8888];
				}
				if ( formats[NiImage::TEX_RGBA5551] ) {
					return formats[NiImage::TEX_RGBA5551];
				}
				if ( formats[NiImage::TEX_RGBA4444] ) {
					return formats[NiImage::TEX_RGBA4444];
				}
				if ( formats[NiImage::TEX_RGB888] ) {
					return formats[NiImage::TEX_RGB888];
				}
				if ( formats[NiImage::TEX_RGB565] ) {
					return formats[NiImage::TEX_RGB565];
				}
				break;
			}
		} else {
			switch ( kind ) {
			case 0:
				if ( formats[NiImage::TEX_PAL8] ) {
					return formats[NiImage::TEX_PAL8];
				}
				// fall through
			case 1:
				if ( formats[NiImage::TEX_RGB565] ) {
					return formats[NiImage::TEX_RGB565];
				}
				if ( formats[NiImage::TEX_RGB888] ) {
					return formats[NiImage::TEX_RGB888];
				}
				if ( formats[NiImage::TEX_RGBA4444] ) {
					return formats[NiImage::TEX_RGBA4444];
				}
				if ( formats[NiImage::TEX_RGBA5551] ) {
					return formats[NiImage::TEX_RGBA5551];
				}
				if ( formats[NiImage::TEX_RGBA8888] ) {
					return formats[NiImage::TEX_RGBA8888];
				}
				break;
			case 2:
				if ( formats[NiImage::TEX_RGBA4444] ) {
					return formats[NiImage::TEX_RGBA4444];
				}
				if ( formats[NiImage::TEX_RGBA5551] ) {
					return formats[NiImage::TEX_RGBA5551];
				}
				if ( formats[NiImage::TEX_RGBA8888] ) {
					return formats[NiImage::TEX_RGBA8888];
				}
				if ( formats[NiImage::TEX_RGB565] ) {
					return formats[NiImage::TEX_RGB565];
				}
				if ( formats[NiImage::TEX_RGB888] ) {
					return formats[NiImage::TEX_RGB888];
				}
				break;
			}
		}
		return NULL;
	}
	return formats[preferred];
}

/*
================
NiDDImage::Unknown7C5C60

Makes the image's surface from the pixels of its file, else its raw
pixels, which give the image its size. With no list of formats the single
format is the one; else the image's preferred format when the list has
it, else the first of the list's formats for the pixels' kind that it
has: palettized pixels take TEX_PAL8 first, then the true-color order.
The last argument prefers the deeper formats. A palettized surface also
gets a palette of the pixels' entries.
================
*/
// FUNCTION: STBC 0x7C5C60
bool NiDDImage::Unknown7C5C60( IDirectDraw7 *directDraw, const NiDDPixelDesc *&format, bool unknown3,
							   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *single,
							   const NiDDPixelDesc **formats, bool unknown7 ) {
	NiPointer<NiRawImageData> pixels = raw;
	if ( url ) {
		if ( !( pixels = Unknown7E15A0( url ) ) ) {
			error = 7;
			return false;
		}
	}

	width = pixels->GetWidth();
	height = pixels->GetHeight();
	assert( width && height );
	mipLevels = ComputeMipmapLevels( width, height );
	mipLevels = mipLevels ? mipLevels : 1;

	NiRawImageData::Format pixelsFormat = pixels->GetFormat();
	if ( !formats ) {
		assert( single );
		format = single;
	} else {
		assert( !single );
		format = ChooseFormat( pixelsFormat, TestFlags( 2 ), unknown7, formats, prefTexFormat );
	}
	if ( !format || !CreateSurface( directDraw, *format, surface, unknown3 ) ) {
		return false;
	}

	NiDDImageSurface *entry = new NiDDImageSurface;
	entry->format = format->GetIndex();
	entry->unknown0C = false;
	entry->surface = surface;
	entry->directDraw = directDraw;

	if ( format->palettized ) {
		if ( pixelsFormat == NiRawImageData::FORMAT_PALETTIZED ) {
			if ( !Unknown7C5330( pixels, *format, entry->surface ) ) {
				goto fail;
			}
			if ( !directDraw ) {
				error = 12;
				goto fail;
			}
			IDirectDrawPalette *palette = NULL;
			if ( directDraw->CreatePalette( DDPCAPS_8BIT | DDPCAPS_ALLOW256, (LPPALETTEENTRY)pixels->GetPalette(),
											&palette, NULL ) != DD_OK ) {
				error = 11;
				goto fail;
			}
			// the original keeps its reference to the palette
			if ( entry->surface->SetPalette( palette ) != DD_OK ) {
				error = 11;
				goto fail;
			}
		} else {
			error = 3;
			goto fail;
		}
	} else if ( !Unknown7C5330( pixels, *format, entry->surface ) ) {
		goto fail;
	}

	surfaces.AddTail( entry );
	surface = entry->surface;
	AddFlags( 1 );
	return true;

fail:
	if ( entry ) {
		if ( entry->surface ) {
			entry->surface->Release();
		}
		delete entry;
	}
	surface = NULL;
	return false;
}

/*
================
SetSurfacePalette

Loads 256 palette entries into the palette a surface already has; false
when the surface has none or the entries are refused (name invented: the
debug copy's own function, AnimOnly.exe 0x517260, NiDDImage.cpp).
================
*/
static bool SetSurfacePalette( void *entries, IDirectDrawSurface7 *surface ) {
	assert( entries && surface );
	IDirectDrawPalette *palette = NULL;
	if ( surface->GetPalette( &palette ) != DD_OK ) {
		return false;
	}
	if ( palette->SetEntries( 0, 0, 256, (LPPALETTEENTRY)entries ) != DD_OK ) {
		return false;
	}
	palette->Release();
	return true;
}

/*
================
NiDDImage::Unknown7C6080

Fills a surface again from the image's pixels: the raw pixels in memory
when asked to prefer them, else those of the image file. The image takes
the pixels' size, which must be its own; a palettized surface also takes
the pixels' palette. As the debug copy has it (AnimOnly.exe 0x516F46,
NiDDImage.cpp).
================
*/
// FUNCTION: STBC 0x7C6080
bool NiDDImage::Unknown7C6080( const NiDDPixelDesc *format, IDirectDrawSurface7 *surface, bool unknown ) {
	if ( !format || !surface ) {
		return false;
	}

	NiPointer<NiRawImageData> pixels( 0 );
	if ( unknown ) {
		pixels = raw;
		if ( !pixels ) {
			if ( !( pixels = Unknown7E15A0( url ) ) ) {
				error = 7;
				return false;
			}
		}
	} else if ( url ) {
		if ( !( pixels = Unknown7E15A0( url ) ) ) {
			error = 7;
			return false;
		}
	} else {
		assert( raw );
		pixels = raw;
	}

	unsigned int pixelsWidth = pixels->GetWidth();
	unsigned int pixelsHeight = pixels->GetHeight();
	assert( pixelsWidth && pixelsHeight );
	if ( pixelsWidth != width || pixelsHeight != height ) {
		error = 15;
		return false;
	}
	width = pixelsWidth;
	height = pixelsHeight;
	mipLevels = ComputeMipmapLevels( width, height );
	mipLevels = mipLevels ? mipLevels : 1;

	if ( format->palettized && pixels->GetFormat() != NiRawImageData::FORMAT_PALETTIZED ) {
		return false;
	}
	if ( !Unknown7C5330( pixels, *format, surface ) ) {
		return false;
	}
	if ( format->palettized && !SetSurfacePalette( pixels->GetPalette(), surface ) ) {
		error = 11;
		return false;
	}
	return true;
}

/*
================
NiDDImage::Unknown7C6270

The image's surface for a DirectDraw object in a pixel format, made when
there is none; false when there is none.
================
*/
// FUNCTION: STBC 0x7C6270
bool NiDDImage::Unknown7C6270( IDirectDraw7 *directDraw, const NiDDPixelDesc *format, bool unknown3,
							   IDirectDrawSurface7 *&surface ) {
	surface = NULL;
	if ( !format ) {
		return false;
	}

	if ( flags & 1 ) {
		NiTListIterator pos = surfaces.GetHeadPos();
		while ( pos ) {
			NiDDImageSurface *entry = surfaces.GetNext( pos );
			if ( entry && entry->format == format->GetIndex() && entry->directDraw == directDraw ) {
				surface = entry->surface;
				if ( entry->unknown0C ) {
					if ( !Unknown7C6080( format, surface, false ) ) {
						return false;
					}
					entry->unknown0C = false;
				}
				break;
			}
		}
	}

	if ( !surface ) {
		if ( unknown58 ) {
			const NiDDPixelDesc *made = NULL;
			if ( !Unknown7C59E0( directDraw, made, unknown3, surface, format, NULL ) ) {
				return false;
			}
		} else {
			if ( !url && !raw ) {
				error = 8;
				return false;
			}
			const NiDDPixelDesc *made = NULL;
			if ( !Unknown7C5C60( directDraw, made, unknown3, surface, format, NULL, false ) ) {
				return false;
			}
		}
	}
	return surface ? true : false;
}

/*
================
NiDDImage::Unknown7C6370

The image's surface for a DirectDraw object in the first of the manager's
pixel formats that has one, made when there is none, and its format.
================
*/
// FUNCTION: STBC 0x7C6370
bool NiDDImage::Unknown7C6370( IDirectDraw7 *directDraw, const NiDDPixelDesc **formats, bool unknown3, bool unknown4,
							   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *&format ) {
	surface = NULL;
	if ( !formats ) {
		return false;
	}

	if ( flags & 1 ) {
		for ( unsigned int i = 0; i < 7; i++ ) {
			format = formats[i];
			if ( !format ) {
				continue;
			}
			NiTListIterator pos = surfaces.GetHeadPos();
			while ( pos ) {
				NiDDImageSurface *entry = surfaces.GetNext( pos );
				if ( entry && entry->format == format->GetIndex() && entry->directDraw == directDraw ) {
					surface = entry->surface;
					if ( entry->unknown0C ) {
						if ( !Unknown7C6080( format, surface, false ) ) {
							return false;
						}
						entry->unknown0C = false;
					}
					i = 7;	// ends the search
					break;
				}
			}
		}
	}

	if ( !surface ) {
		if ( unknown58 ) {
			if ( !Unknown7C59E0( directDraw, format, unknown4, surface, NULL, formats ) ) {
				return false;
			}
		} else if ( url || raw ) {
			if ( !Unknown7C5C60( directDraw, format, unknown4, surface, NULL, formats, unknown3 ) ) {
				return false;
			}
		} else {
			error = 8;
			return false;
		}
	}
	return true;
}

/*
================
NiDDImage::Create

NiImage's creation function for this renderer.
================
*/
// FUNCTION: STBC 0x7C6490
NiImage *NiDDImage::Create() {
	return new NiDDImage;
}

/*
================
NiDDImage::VirtualSlot12

Marks every surface.
================
*/
// FUNCTION: STBC 0x7C65F0
bool NiDDImage::VirtualSlot12() {
	NiTListIterator pos = surfaces.GetHeadPos();
	while ( pos ) {
		surfaces.GetNext( pos )->unknown0C = true;
	}
	return true;
}

/*
================
NiDDImage::Unknown7C6610

The first surface.
================
*/
// FUNCTION: STBC 0x7C6610
IDirectDrawSurface7 *NiDDImage::Unknown7C6610() {
	surfacePos = surfaces.GetHeadPos();
	if ( surfacePos ) {
		NiDDImageSurface *entry = surfaces.GetNext( surfacePos );
		if ( entry ) {
			return entry->surface;
		}
	}
	return 0;
}

/*
================
NiDDImage::Unknown7C6630

The next surface.
================
*/
// FUNCTION: STBC 0x7C6630
IDirectDrawSurface7 *NiDDImage::Unknown7C6630() {
	if ( surfacePos ) {
		NiDDImageSurface *entry = surfaces.GetNext( surfacePos );
		if ( entry ) {
			return entry->surface;
		}
	}
	return 0;
}

/*
================
NiDDImage::Save

Writes the first surface and its mipmap chain to a file: the size, the
level count, the DirectDraw pixel format and the byte count of the
levels, then each level's rows, each padded to a multiple of 8 bytes.
False when there is no surface, the file can't be opened or the surface's
format is not registered.
================
*/
// FUNCTION: STBC 0x7C6650
bool NiDDImage::Save( const char *name ) {
	NiDDImageSurface *entry = surfaces.GetHead();
	if ( !entry ) {
		return false;
	}
	IDirectDrawSurface7 *surface = entry->surface;
	if ( !surface ) {
		return false;
	}

	NiFile file( name, NiFile::WRITE_ONLY, 0x2800 );
	if ( !file ) {
		return false;
	}
	const NiDDPixelDesc *format = NiDDPixelDescTable::descs.GetAt( entry->format );
	if ( !format ) {
		return false;
	}
	DDPIXELFORMAT ddFormat;
	format->GetDDPixelFormat( &ddFormat );

	file.Write( &width, sizeof( width ) );
	file.Write( &height, sizeof( height ) );
	file.Write( &mipLevels, sizeof( mipLevels ) );
	file.Write( &ddFormat, sizeof( ddFormat ) );

	unsigned int rowBytes = ( ddFormat.dwRGBBitCount >> 3 ) * width;
	unsigned int rows = height;
	unsigned int size = 0;
	unsigned int level;
	for ( level = 0; level < mipLevels; level++ ) {
		size += rows * rowBytes;
		if ( rowBytes & 7 ) {
			size += ( 8 - ( rowBytes & 7 ) ) * rows;
		}
		rows >>= 1;
		rowBytes >>= 1;
	}
	file.Write( &size, sizeof( size ) );

	rowBytes = ( ddFormat.dwRGBBitCount >> 3 ) * width;
	rows = height;
	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	surface->AddRef();
	unsigned char padding[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for ( level = 0; level < mipLevels; level++ ) {
		surface->Lock( NULL, &desc, DDLOCK_READONLY, NULL );
		for ( unsigned int row = 0; row < rows; row++ ) {
			file.Write( (unsigned char *)desc.lpSurface + desc.lPitch * row, rowBytes );
			if ( rowBytes & 7 ) {
				file.Write( padding, 8 - ( rowBytes & 7 ) );
			}
		}
		surface->Unlock( NULL );

		DDSCAPS2 caps;
		memset( &caps, 0, sizeof( caps ) );
		caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		rows >>= 1;
		rowBytes >>= 1;
		IDirectDrawSurface7 *next;
		HRESULT result = surface->GetAttachedSurface( &caps, &next );
		if ( result != DD_OK ) {
			next = NULL;
		}
		if ( surface ) {
			surface->Release();
		}
		surface = next;
	}
	return true;
}

/*
================
NiDDImage::CreateFromRawFile

An image read from a file of raw pixels: the width, height and mipmap
level count, the DirectDraw pixel format, the byte count and the bytes.
Nothing in the game calls it (name invented).
================
*/
// FUNCTION: STBC 0x7C6980
NiDDImage *NiDDImage::CreateFromRawFile( const char *filename ) {
	NiDDImage *image = new NiDDImage;

	FILE *file = fopen( filename, "rb" );
	fread( &image->width, sizeof( unsigned int ), 1, file );
	fread( &image->height, sizeof( unsigned int ), 1, file );
	fread( &image->mipLevels, sizeof( unsigned int ), 1, file );
	if ( image->mipLevels > 1 ) {
		image->AddFlags( 2 );
	}

	DDPIXELFORMAT pixelFormat;
	fread( &pixelFormat, sizeof( pixelFormat ), 1, file );
	unsigned int size = 0;
	fread( &size, sizeof( size ), 1, file );

	image->unknown58 = new NiDDImageBuffer;
	NiDDPixelDesc format( &pixelFormat );
	image->unknown58->format = NiDDPixelDescTable::Register( format );
	image->unknown58->buffer = new unsigned char[size];
	fread( image->unknown58->buffer, size, 1, file );
	fclose( file );

	if ( !image->Unknown7E0EF0() ) {
		delete image;
		return NULL;
	}
	return image;
}

/*
================
NiDDBufferImage::Create

The image of a frame buffer's surface, with the surface as its only
texture; NULL without a surface.
================
*/
// FUNCTION: STBC 0x7C6C40
NiDDBufferImage *NiDDBufferImage::Create( IDirectDrawSurface7 *surface, IDirectDraw7 *directDraw ) {
	if ( !surface ) {
		return NULL;
	}

	NiDDBufferImage *image = new NiDDBufferImage;

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	if ( surface->GetSurfaceDesc( &desc ) != DD_OK ) {
		// the original leaks the image here
		return NULL;
	}

	image->unknown5C = new NiDDImageSurface;
	NiDDPixelDesc format( &desc.ddpfPixelFormat );
	image->unknown5C->surface = surface;
	image->unknown5C->format = NiDDPixelDescTable::Register( format );
	image->unknown5C->unknown0C = false;
	image->unknown5C->directDraw = directDraw;
	surface->AddRef();

	image->width = desc.dwWidth;
	image->height = desc.dwHeight;
	image->mipLevels = 1;
	image->surfaces.AddHead( image->unknown5C );
	image->AddFlags( 1 );
	return image;
}

/*
================
NiDDBufferImage::~NiDDBufferImage

Releases the surfaces, the frame buffer's among them.
================
*/
// FUNCTION: STBC 0x7C6F00
// FUNCTION: STBC 0x7C75E0 ??_GNiDDBufferImage@@UAEPAXI@Z
NiDDBufferImage::~NiDDBufferImage() {
	NiDDImage::VirtualSlot13( NULL );
	unknown5C = NULL;
}

/*
================
NiDDBufferImage::Unknown7C70C0
================
*/
// FUNCTION: STBC 0x7C70C0
bool NiDDBufferImage::Unknown7C70C0( IDirectDrawSurface7 **surface ) {
	if ( unknown5C ) {
		*surface = unknown5C->surface;
	} else {
		*surface = 0;
	}
	return true;
}

/*
================
NiDDBufferImage::VirtualSlot13

Releases the surfaces and forgets the buffer.
================
*/
// FUNCTION: STBC 0x7C70F0
bool NiDDBufferImage::VirtualSlot13( void *unknown ) {
	if ( !NiDDImage::VirtualSlot13( unknown ) ) {
		return false;
	}
	unknown5C = 0;
	return true;
}

/*
================
NiDDBufferImage::Unknown7C7160

Locks the frame buffer surface and hands its pixels, in the surface's own
pixel format, to the raw image.
================
*/
// FUNCTION: STBC 0x7C7160
bool NiDDBufferImage::Unknown7C7160( NiRawImageData *raw ) {
	if ( !raw || !unknown5C || !unknown5C->surface ) {
		return false;
	}

	NiDDPixelDesc format( unknown5C->surface );

	DDSURFACEDESC2 desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize = sizeof( desc );
	if ( unknown5C->surface->Lock( NULL, &desc, DDLOCK_WAIT, NULL ) != DD_OK ) {
		return false;
	}
	if ( !raw->Unknown7DF9A0( format, desc.lPitch, desc.dwWidth, desc.dwHeight, (const unsigned char *)desc.lpSurface ) ) {
		unknown5C->surface->Unlock( NULL );
		return false;
	}
	unknown5C->surface->Unlock( NULL );
	return true;
}

