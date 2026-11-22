// NiRawImageData.cpp -- an image's raw pixels

#include "NiRTLib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "NiRawImageData.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"
#include "renderer/NiD3DPixelFormat.h"
#include "renderer/NiImage.h"
#include "NiNode.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e0930 _$E237
// FUNCTION: STBC 0x007e0880 _$E17
// FUNCTION: STBC 0x007e08a0 _$E15
// FUNCTION: STBC 0x007e08b0 _$E23
// FUNCTION: STBC 0x007e08d0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e08e0 _$E30
// FUNCTION: STBC 0x007e0920 _$E44

// FUNCTION: STBC 0x007de050 _$E213
// FUNCTION: STBC 0x007de070 _$E210
// DATA: STBC 0x9A15D0 ?rtti@NiRawImageData@@2VNiRTTI@@B
const NiRTTI NiRawImageData::rtti( "NiRawImageData", &NiObject::rtti );

/*
================
NiRawImageData::Register
================
*/
// FUNCTION: STBC 0x7DE090 ?Register@NiRawImageData@@SAHXZ
NiImplementRegister( NiRawImageData )

// FUNCTION: STBC 0x007de1e0 _$E219
// FUNCTION: STBC 0x007de260 _$E216
// DATA: STBC 0x9A15D8 ?PALETTIZED_DESC@NiRawImageData@@2VNiPixelDesc@@B
const NiPixelDesc NiRawImageData::PALETTIZED_DESC( 8, true, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
// FUNCTION: STBC 0x007de280 _$E224
// FUNCTION: STBC 0x007de310 _$E222
// DATA: STBC 0x9A15A8 ?RGB_DESC@NiRawImageData@@2VNiPixelDesc@@B
const NiPixelDesc NiRawImageData::RGB_DESC( 24, false, 0xFF, 0xFF00, 0xFF0000, 0, 8, 8, 8, 0, 0, 8, 16, 0 );
// FUNCTION: STBC 0x007de330 _$E229
// FUNCTION: STBC 0x007de3c0 _$E227
// DATA: STBC 0x9A1608 ?RGBA_DESC@NiRawImageData@@2VNiPixelDesc@@B
const NiPixelDesc NiRawImageData::RGBA_DESC( 24, false, 0xFF, 0xFF00, 0xFF0000, 0xFF000000, 8, 8, 8, 8, 0, 8, 16, 24 );

/*
================
NiRawImageData::NiRawImageData
================
*/
// FUNCTION: STBC 0x7DE3E0 ??0NiRawImageData@@QAE@XZ
NiRawImageData::NiRawImageData() {
	format = FORMAT_RGB;
	width = 0;
	height = 0;
	pixelStride = 3;
	rowStride = 0;
	palette = 0;
	data = 0;
}

/*
================
NiRawImageData::NiRawImageData
================
*/
// FUNCTION: STBC 0x7DE420 ??0NiRawImageData@@QAE@W4Format@0@II@Z
NiRawImageData::NiRawImageData( Format format_, unsigned int width_, unsigned int height_ ) {
	format = FORMAT_RGB;
	width = 0;
	height = 0;
	pixelStride = 3;
	rowStride = 0;
	palette = 0;
	data = 0;
	Create( format_, width_, height_ );
}

/*
================
NiRawImageData::Create

As the debug copy has it (AnimOnly.exe 0x40AD98, NiRawImageData.cpp): the
switch on the stored format, the size stored once after it.
================
*/
// FUNCTION: STBC 0x7DE4A0
bool NiRawImageData::Create( Format format_, unsigned int width_, unsigned int height_ ) {
	if ( width_ == width && height_ == height && format_ == format ) {
		return true;
	}

	FreeData();
	data = 0;
	delete [] palette;
	palette = 0;

	format = format_;
	width = 0;
	height = 0;
	pixelStride = 0;
	rowStride = 0;

	switch ( format ) {
	case FORMAT_PALETTIZED:
		palette = new NiPaletteEntry[256];
		if ( !palette ) {
			return false;
		}
		AllocateData( width_ * height_ );
		if ( !data ) {
			delete [] palette;
			palette = 0;
			return false;
		}
		pixelStride = 1;
		rowStride = width_;
		break;
	case FORMAT_RGB:
		palette = 0;
		AllocateData( width_ * 3 * height_ );
		if ( !data ) {
			return false;
		}
		pixelStride = 3;
		rowStride = width_ * 3;
		break;
	case FORMAT_RGBA:
		palette = 0;
		AllocateData( width_ * 4 * height_ );
		if ( !data ) {
			return false;
		}
		pixelStride = 4;
		rowStride = width_ * 4;
		break;
	default:
		format = FORMAT_PALETTIZED;
		width = 0;
		height = 0;
		pixelStride = 0;
		rowStride = 0;
		break;
	}

	width = width_;
	height = height_;
	return true;
}

/*
================
NiRawImageData::Unknown7DE640
================
*/
// FUNCTION: STBC 0x7DE640
bool NiRawImageData::Unknown7DE640( unsigned int width_, unsigned int height_ ) {
	if ( width * height < width_ * height_ ) {
		return false;
	}
	height = height_;
	width = width_;
	rowStride = pixelStride * width_;
	return true;
}

/*
================
NiRawImageData::~NiRawImageData
================
*/
// FUNCTION: STBC 0x7DE680
NiRawImageData::~NiRawImageData() {
	delete [] palette;
	FreeData();
}

/*
================
NiRawImageData::Copy
================
*/
// FUNCTION: STBC 0x7DE6E0
bool NiRawImageData::Copy( NiRawImageData *source ) {
	if ( !source ) {
		return false;
	}
	if ( !Create( source->format, source->width, source->height ) ) {
		return false;
	}

	unsigned int size = source->pixelStride * source->height * source->width;
	const unsigned char *sourceData = source->data;
	unsigned char *destData = data;
	if ( format == FORMAT_PALETTIZED ) {
		const NiPaletteEntry *sourceEntry = source->palette;
		NiPaletteEntry *destEntry = palette;
		for ( int i = 0; i < 256; i++ ) {
			*destEntry++ = *sourceEntry++;
		}
	}
	memcpy( destData, sourceData, size );
	return true;
}

/*
================
NiRawImageData::Unknown7DE760

Makes this image a copy of the source at a new size: the format and the
palette are the source's, and each pixel is the source pixel at the same
fraction of its width and height, chosen without any filtering. False when
there is no source or the size is empty, or the image cannot be made.
Statements as the debug copy has them (AnimOnly.exe 0x40B197,
NiRawImageData.cpp).
================
*/
// FUNCTION: STBC 0x7DE760
bool NiRawImageData::Unknown7DE760( NiRawImageData *source, unsigned int newWidth, unsigned int newHeight ) {
	if ( !source || !newWidth || !newHeight ) {
		return false;
	}
	if ( !Create( source->format, newWidth, newHeight ) ) {
		return false;
	}

	unsigned int srcWidth = source->width;
	unsigned int srcHeight = source->height;
	unsigned char *src = source->data;
	unsigned char *dest = data;
	unsigned int x;
	unsigned int y;
	unsigned int srcX;
	unsigned int srcY;
	unsigned char *srcPixel;

	switch ( format ) {
	case FORMAT_PALETTIZED: {
		NiPaletteEntry *srcEntry = source->palette;
		NiPaletteEntry *destEntry = palette;
		for ( x = 0; x < 256; x++ ) {
			*destEntry++ = *srcEntry++;
		}
		for ( y = 0; y < newHeight; y++ ) {
			srcY = y * srcHeight / newHeight;
			for ( x = 0; x < newWidth; x++ ) {
				srcX = x * srcWidth / newWidth;
				*dest++ = src[srcWidth * srcY + srcX];
			}
		}
		break;
	}
	case FORMAT_RGB:
		for ( y = 0; y < newHeight; y++ ) {
			srcY = y * srcHeight / newHeight;
			for ( x = 0; x < newWidth; x++ ) {
				srcX = x * srcWidth / newWidth;
				srcPixel = src + 3 * ( srcX + srcWidth * srcY );
				unsigned char *destPixel = dest + 3 * ( x + newWidth * y );
				*destPixel++ = *srcPixel++;
				*destPixel++ = *srcPixel++;
				*destPixel++ = *srcPixel++;
			}
		}
		break;
	case FORMAT_RGBA:
		for ( y = 0; y < newHeight; y++ ) {
			srcY = y * srcHeight / newHeight;
			for ( x = 0; x < newWidth; x++ ) {
				srcX = x * srcWidth / newWidth;
				srcPixel = src + 4 * ( srcWidth * srcY + srcX );
				*dest++ = srcPixel[0];
				*dest++ = srcPixel[1];
				*dest++ = srcPixel[2];
				*dest++ = srcPixel[3];
			}
		}
		break;
	}
	return true;
}

/*
================
NiRawImageData::Unknown7DE990

Fills this image with the source at half its width and height, each
pixel the mean of a 2x2 block; a palettized source is reduced through its
palette into an RGB image. False when the formats or sizes do not fit.
================
*/
// FUNCTION: STBC 0x7DE990
bool NiRawImageData::Unknown7DE990( NiRawImageData *source ) {
	unsigned int x;
	unsigned int y;

	if ( !source || ( source->format == FORMAT_PALETTIZED && format != FORMAT_RGB ) ||
		 ( source->format != FORMAT_PALETTIZED && source->format != format ) || source->width != 2 * width ||
		 source->height != 2 * height ) {
		return false;
	}

	// each output channel is the unsigned mean of the block's four
	unsigned char *dest = data;
	unsigned char *src = source->data;
	switch ( source->format ) {
	case FORMAT_RGBA:
		for ( y = 0; y < height; y++ ) {
			for ( x = 0; x < width; x++ ) {
				*dest++ = ( src[0] + src[4] + src[source->rowStride] + src[source->rowStride + 4] ) / 4u;
				*dest++ = ( src[1] + src[5] + src[source->rowStride + 1] + src[source->rowStride + 5] ) / 4u;
				*dest++ = ( src[2] + src[6] + src[source->rowStride + 2] + src[source->rowStride + 6] ) / 4u;
				*dest++ = ( src[3] + src[7] + src[source->rowStride + 3] + src[source->rowStride + 7] ) / 4u;
				src += 8;
			}
			src += source->rowStride;
		}
		break;
	case FORMAT_RGB:
		for ( y = 0; y < height; y++ ) {
			for ( x = 0; x < width; x++ ) {
				*dest++ = ( src[0] + src[3] + src[source->rowStride] + src[source->rowStride + 3] ) / 4u;
				*dest++ = ( src[1] + src[4] + src[source->rowStride + 1] + src[source->rowStride + 4] ) / 4u;
				*dest++ = ( src[2] + src[5] + src[source->rowStride + 2] + src[source->rowStride + 5] ) / 4u;
				src += 6;
			}
			src += source->rowStride;
		}
		break;
	case FORMAT_PALETTIZED:
		for ( y = 0; y < height; y++ ) {
			for ( x = 0; x < width; x++ ) {
				NiPaletteEntry *entry00 = &source->palette[src[0]];
				NiPaletteEntry *entry01 = &source->palette[src[1]];
				NiPaletteEntry *entry10 = &source->palette[src[source->rowStride]];
				NiPaletteEntry *entry11 = &source->palette[src[source->rowStride + 1]];
				src += 2;
				*dest++ = ( entry00->red + entry01->red + entry10->red + entry11->red ) / 4u;
				*dest++ = ( entry00->green + entry01->green + entry10->green + entry11->green ) / 4u;
				*dest++ = ( entry00->blue + entry01->blue + entry10->blue + entry11->blue ) / 4u;
			}
			src += source->rowStride;
		}
		break;
	}
	return true;
}

/*
================
NiRawImageData::Unknown7DECF0

A palettized image gives its palette entries, any other its first three
bytes of each pixel. The alpha comes from the same place in the second
image: a palettized one's red, or the first byte of each pixel. As the
debug copy has it (AnimOnly.exe 0x40B953, NiRawImageData.cpp).
================
*/
// FUNCTION: STBC 0x7DECF0
NiRawImageData *NiRawImageData::Unknown7DECF0( NiRawImageData *color, NiRawImageData *alpha ) {
	if ( !color || !alpha || color->GetWidth() != alpha->GetWidth() || color->GetHeight() != alpha->GetHeight() ) {
		return 0;
	}

	NiRawImageData *image = new NiRawImageData( FORMAT_RGBA, color->GetWidth(), color->GetHeight() );
	if ( !image ) {
		return 0;
	}

	unsigned int width = color->GetWidth();
	unsigned int height = color->GetHeight();
	unsigned char *src = color->GetPixels();
	unsigned char *dest = image->GetPixels();
	unsigned int i;
	if ( color->GetFormat() == FORMAT_PALETTIZED ) {
		for ( i = 0; i < height * width; i++ ) {
			NiPaletteEntry *entry = &color->GetPalette()[*src++];
			*dest++ = entry->red;
			*dest++ = entry->green;
			*dest++ = entry->blue;
			dest++;
		}
	} else {
		for ( i = 0; i < height * width; i++ ) {
			*dest++ = src[0];
			*dest++ = src[1];
			*dest++ = src[2];
			src += color->GetPixelStride();
			dest++;
		}
	}

	src = alpha->GetPixels();
	dest = image->GetPixels() + 3;
	if ( alpha->GetFormat() == FORMAT_PALETTIZED ) {
		for ( i = 0; i < height * width; i++ ) {
			NiPaletteEntry *entry = &alpha->GetPalette()[*src++];
			*dest = entry->red;
			dest += 4;
		}
	} else {
		for ( i = 0; i < height * width; i++ ) {
			*dest = *src;
			dest += 4;
			src += alpha->GetPixelStride();
		}
	}
	return image;
}

/*
================
NiRawImageData::Unknown7DEEA0
================
*/
// FUNCTION: STBC 0x7DEEA0
void NiRawImageData::Unknown7DEEA0( NiRawImageData *source ) {
	format = source->format;
	width = source->width;
	height = source->height;
	pixelStride = source->pixelStride;
	rowStride = source->rowStride;
	palette = source->palette;
	data = source->data;
	source->palette = 0;
	source->data = 0;
}

/*
================
NiRawImageData::Unknown7DEEE0

Writes the pixels, converted to a pixel format, into locked surface memory
of a pitch. A palettized image goes through a table of its 256 entries
already in the surface's form for sixteen and thirty-two bits; any other
packs each pixel by the masks, shifts and losses of the format, with a full
alpha unless the image has one. Twenty-four bit surfaces are RGB or BGR by
their masks and nothing else; eight bit surfaces take only a palettized
image. False when the format does not fit; true, writing nothing, for an
image of an unknown format or a surface of an unknown depth.
================
*/
// FUNCTION: STBC 0x7DEEE0
// DRAFT: structure agrees (format switch, bits-per-pixel switches, one 256-entry table for palettized 16 and 32 bit); retail does the 16 bit packing in unsigned short intermediates (movzx di, bl), reads its arguments late, and orders the four channel terms differently; frame 0x430 against our 0x438
bool NiRawImageData::Unknown7DEEE0( const NiPixelDesc &format_, unsigned int pitch, unsigned char *bits ) {
	unsigned int x;
	unsigned int y;
	unsigned int i;
	unsigned char *src = data;
	unsigned char *row = bits;
	union {
		unsigned short table16[256];
		unsigned int table32[256];
	};

	switch ( format ) {
	case FORMAT_PALETTIZED:
		switch ( format_.bitsPerPixel ) {
		case 8:
			if ( !format_.palettized ) {
				return false;
			}
			for ( y = 0; y < height; y++ ) {
				memcpy( row, src, width );
				row += pitch;
				src += width;
			}
			return true;
		case 16:
			for ( i = 0; i < 256; i++ ) {
				table16[i] = ( ( ( palette[i].red >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] ) |
							 ( ( ( palette[i].green >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							 ( ( ( palette[i].blue >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							 ( ( ( 0xFF >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] );
			}
			for ( y = 0; y < height; y++ ) {
				unsigned short *pixel = (unsigned short *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = table16[*src++];
				}
				row += pitch;
			}
			return true;
		case 24:
			if ( format_.GetMask( 1 ) != 0xFF00 ) {
				return false;
			}
			if ( format_.GetMask( 0 ) == 0xFF ) {
				for ( y = 0; y < height; y++ ) {
					unsigned char *pixel = row;
					for ( x = 0; x < width; x++ ) {
						pixel[0] = palette[*src].red;
						pixel[1] = palette[*src].green;
						pixel[2] = palette[*src++].blue;
						pixel += 3;
					}
					row += pitch;
				}
				return true;
			}
			if ( format_.GetMask( 0 ) != 0xFF0000 ) {
				return false;
			}
			for ( y = 0; y < height; y++ ) {
				unsigned char *pixel = row;
				for ( x = 0; x < width; x++ ) {
					pixel[0] = palette[*src].blue;
					pixel[1] = palette[*src].green;
					pixel[2] = palette[*src++].red;
					pixel += 3;
				}
				row += pitch;
			}
			return true;
		case 32:
			for ( i = 0; i < 256; i++ ) {
				table32[i] = ( ( ( palette[i].blue >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							 ( ( ( palette[i].green >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							 ( ( ( palette[i].red >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] ) |
							 ( ( ( 0xFF >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] );
			}
			for ( y = 0; y < height; y++ ) {
				unsigned int *pixel = (unsigned int *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = table32[*src++];
				}
				row += pitch;
			}
			return true;
		default:
			return true;
		}
	case FORMAT_RGB:
		switch ( format_.bitsPerPixel ) {
		case 8:
			return false;
		case 16:
			for ( y = 0; y < height; y++ ) {
				unsigned short *pixel = (unsigned short *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = ( ( ( src[0] >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] ) |
							   ( ( ( src[1] >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							   ( ( ( src[2] >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							   ( ( ( 0xFF >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] );
					src += 3;
				}
				row += pitch;
			}
			return true;
		case 24:
			if ( format_.GetMask( 1 ) != 0xFF00 ) {
				return false;
			}
			if ( format_.GetMask( 0 ) == 0xFF ) {
				for ( y = 0; y < height; y++ ) {
					memcpy( row, src, rowStride );
					row += pitch;
					src += rowStride;
				}
				return true;
			}
			if ( format_.GetMask( 0 ) != 0xFF0000 ) {
				return false;
			}
			for ( y = 0; y < height; y++ ) {
				unsigned char *pixel = row;
				for ( x = 0; x < width; x++ ) {
					pixel[0] = src[2];
					pixel[1] = src[1];
					pixel[2] = src[0];
					pixel += 3;
					src += 3;
				}
				row += pitch;
			}
			return true;
		case 32:
			for ( y = 0; y < height; y++ ) {
				unsigned int *pixel = (unsigned int *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = ( ( ( src[1] >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							   ( ( ( src[2] >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							   ( ( ( src[0] >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] ) |
							   ( ( ( 0xFF >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] );
					src += 3;
				}
				row += pitch;
			}
			return true;
		}
		return true;
	case FORMAT_RGBA:
		switch ( format_.bitsPerPixel ) {
		case 8:
			return false;
		case 16:
			for ( y = 0; y < height; y++ ) {
				unsigned short *pixel = (unsigned short *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = ( ( ( src[1] >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							   ( ( ( src[3] >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] ) |
							   ( ( ( src[2] >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							   ( ( ( src[0] >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] );
					src += 4;
				}
				row += pitch;
			}
			return true;
		case 24:
			if ( format_.GetMask( 1 ) != 0xFF00 ) {
				return false;
			}
			if ( format_.GetMask( 0 ) == 0xFF ) {
				for ( y = 0; y < height; y++ ) {
					unsigned char *pixel = row;
					for ( x = 0; x < width; x++ ) {
						pixel[0] = src[0];
						pixel[1] = src[1];
						pixel[2] = src[2];
						pixel += 3;
						src += 4;
					}
					row += pitch;
				}
				return true;
			}
			if ( format_.GetMask( 0 ) != 0xFF0000 ) {
				return false;
			}
			for ( y = 0; y < height; y++ ) {
				unsigned char *pixel = row;
				for ( x = 0; x < width; x++ ) {
					pixel[0] = src[2];
					pixel[1] = src[1];
					pixel[2] = src[0];
					pixel += 3;
					src += 4;
				}
				row += pitch;
			}
			return true;
		case 32:
			for ( y = 0; y < height; y++ ) {
				unsigned int *pixel = (unsigned int *)row;
				for ( x = 0; x < width; x++ ) {
					*pixel++ = ( ( ( src[1] >> format_.losses[1] ) << format_.shifts[1] ) & format_.masks[1] ) |
							   ( ( ( src[3] >> format_.losses[3] ) << format_.shifts[3] ) & format_.masks[3] ) |
							   ( ( ( src[2] >> format_.losses[2] ) << format_.shifts[2] ) & format_.masks[2] ) |
							   ( ( ( src[0] >> format_.losses[0] ) << format_.shifts[0] ) & format_.masks[0] );
					src += 4;
				}
				row += pitch;
			}
			return true;
		}
		return true;
		return true;
	}
	return true;
}

/*
================
NiRawImageData::Unknown7DF9A0

Takes the pixels of locked surface memory. A palettized surface of eight
bits keeps its indices; a sixteen or thirty-two bit one is unpacked to RGB
or, with an alpha mask, to RGBA by the masks, shifts and losses of its
pixel format; a twenty-four bit one without alpha is copied as it is if
red comes first, or with red and blue exchanged if blue does. False for
any other format. Statements as the debug copy has them (AnimOnly.exe
0x40CB79, NiRawImageData.cpp), which reads the masks through an accessor
(0x40E550) this tree's NiPixelDesc doesn't declare yet.
================
*/
// FUNCTION: STBC 0x7DF9A0
bool NiRawImageData::Unknown7DF9A0( const NiPixelDesc &format_, unsigned int pitch, unsigned int width_, unsigned int height_, const unsigned char *bits ) {
	switch ( format_.bitsPerPixel ) {
	case 8: {
		if ( !format_.palettized ) {
			return false;
		}
		if ( !Create( FORMAT_PALETTIZED, width_, height_ ) ) {
			return false;
		}
		unsigned char *dest = data;
		for ( unsigned int y = 0; y < height_; y++ ) {
			memcpy( dest, bits, width_ );
			dest += rowStride;
			bits += pitch;
		}
		break;
	}
	case 16:
		if ( format_.palettized ) {
			return false;
		}
		if ( format_.GetMask( 3 ) ) {
			if ( !Create( FORMAT_RGBA, width_, height_ ) ) {
				return false;
			}
			unsigned char *dest = data;
			for ( unsigned int y = 0; y < height_; y++ ) {
				const unsigned short *pixel = (const unsigned short *)( bits + pitch * y );
				for ( unsigned int x = 0; x < width_; x++ ) {
					*dest++ = ( ( *pixel & format_.masks[0] ) >> format_.shifts[0] ) << format_.losses[0];
					*dest++ = ( ( *pixel & format_.masks[1] ) >> format_.shifts[1] ) << format_.losses[1];
					*dest++ = ( ( *pixel & format_.masks[2] ) >> format_.shifts[2] ) << format_.losses[2];
					*dest++ = ( ( *pixel & format_.masks[3] ) >> format_.shifts[3] ) << format_.losses[3];
					pixel++;
				}
			}
		} else {
			if ( !Create( FORMAT_RGB, width_, height_ ) ) {
				return false;
			}
			unsigned char *dest = data;
			for ( unsigned int y = 0; y < height_; y++ ) {
				const unsigned short *pixel = (const unsigned short *)( bits + pitch * y );
				for ( unsigned int x = 0; x < width_; x++ ) {
					*dest++ = ( ( *pixel & format_.masks[0] ) >> format_.shifts[0] ) << format_.losses[0];
					*dest++ = ( ( *pixel & format_.masks[1] ) >> format_.shifts[1] ) << format_.losses[1];
					*dest++ = ( ( *pixel & format_.masks[2] ) >> format_.shifts[2] ) << format_.losses[2];
					pixel++;
				}
			}
		}
		break;
	case 24: {
		if ( format_.palettized || format_.GetMask( 3 ) ) {
			return false;
		}
		if ( !Create( FORMAT_RGB, width_, height_ ) ) {
			return false;
		}
		unsigned char *dest = data;
		if ( format_.GetMask( 1 ) != 0xFF00 ) {
			return false;
		}
		if ( format_.GetMask( 0 ) == 0xFF ) {
			for ( unsigned int y = 0; y < height_; y++ ) {
				memcpy( dest, bits, rowStride );
				bits += pitch;
				dest += rowStride;
			}
		} else if ( format_.GetMask( 0 ) == 0xFF0000 ) {
			for ( unsigned int y = 0; y < height_; y++ ) {
				const unsigned char *pixel = bits + pitch * y;
				for ( unsigned int x = 0; x < width_; x++ ) {
					unsigned char blue = *pixel++;
					unsigned char green = *pixel++;
					unsigned char red = *pixel++;
					*dest++ = red;
					*dest++ = green;
					*dest++ = blue;
				}
			}
		} else {
			return false;
		}
		break;
	}
	case 32:
		if ( format_.palettized ) {
			return false;
		}
		if ( format_.GetMask( 3 ) ) {
			if ( !Create( FORMAT_RGBA, width_, height_ ) ) {
				return false;
			}
			unsigned char *dest = data;
			for ( unsigned int y = 0; y < height_; y++ ) {
				const unsigned int *pixel = (const unsigned int *)( bits + pitch * y );
				for ( unsigned int x = 0; x < width_; x++ ) {
					*dest++ = ( ( *pixel & format_.masks[0] ) >> format_.shifts[0] ) << format_.losses[0];
					*dest++ = ( ( *pixel & format_.masks[1] ) >> format_.shifts[1] ) << format_.losses[1];
					*dest++ = ( ( *pixel & format_.masks[2] ) >> format_.shifts[2] ) << format_.losses[2];
					*dest++ = ( ( *pixel & format_.masks[3] ) >> format_.shifts[3] ) << format_.losses[3];
					pixel++;
				}
			}
		} else {
			if ( !Create( FORMAT_RGB, width_, height_ ) ) {
				return false;
			}
			unsigned char *dest = data;
			for ( unsigned int y = 0; y < height_; y++ ) {
				const unsigned int *pixel = (const unsigned int *)( bits + pitch * y );
				for ( unsigned int x = 0; x < width_; x++ ) {
					*dest++ = ( ( *pixel & format_.masks[0] ) >> format_.shifts[0] ) << format_.losses[0];
					*dest++ = ( ( *pixel & format_.masks[1] ) >> format_.shifts[1] ) << format_.losses[1];
					*dest++ = ( ( *pixel & format_.masks[2] ) >> format_.shifts[2] ) << format_.losses[2];
					pixel++;
				}
			}
		}
		break;
	default:
		return false;
	}
	return true;
}

/*
================
NiRawImageData::Unknown7DFE40

An RGBA image of the source's size and colors; a pixel whose red, green
and blue each lie within their ranges takes keyAlpha, any other alpha.
================
*/
// FUNCTION: STBC 0x7DFE40
// DRAFT: register choice only: the alpha arguments and the palette index take cl/dl the other way round
bool NiRawImageData::Unknown7DFE40( NiRawImageData *source, unsigned char redMin, unsigned char redMax,
									unsigned char greenMin, unsigned char greenMax, unsigned char blueMin,
									unsigned char blueMax, unsigned char keyAlpha, unsigned char alpha ) {
	if ( !source ) {
		return false;
	}
	if ( !Create( FORMAT_RGBA, source->width, source->height ) ) {
		return false;
	}

	unsigned int count = width * height;
	switch ( source->format ) {
	case FORMAT_PALETTIZED: {
		const unsigned char *src = source->data;
		NiPaletteEntry *palette_ = source->palette;
		unsigned char *dst = data;
		for ( unsigned int i = 0; i < count; i++ ) {
			NiPaletteEntry *entry = &palette_[*src++];
			*dst++ = entry->red;
			*dst++ = entry->green;
			*dst++ = entry->blue;
			if ( entry->red >= redMin && entry->red <= redMax && entry->green >= greenMin && entry->green <= greenMax
				 && entry->blue >= blueMin && entry->blue <= blueMax ) {
				*dst = keyAlpha;
			} else {
				*dst = alpha;
			}
			dst++;
		}
		break;
	}
	case FORMAT_RGB: {
		const unsigned char *src = source->data;
		unsigned char *dst = data;
		for ( unsigned int i = 0; i < count; i++ ) {
			unsigned char red = *src++;
			unsigned char green = *src++;
			unsigned char blue = *src++;
			*dst++ = red;
			*dst++ = green;
			*dst++ = blue;
			if ( red >= redMin && red <= redMax && green >= greenMin && green <= greenMax && blue >= blueMin && blue <= blueMax ) {
				*dst = keyAlpha;
			} else {
				*dst = alpha;
			}
			dst++;
		}
		break;
	}
	case FORMAT_RGBA: {
		const unsigned char *src = source->data;
		unsigned char *dst = data;
		for ( unsigned int i = 0; i < count; i++ ) {
			unsigned char red = *src++;
			unsigned char green = *src++;
			unsigned char blue = *src;
			src += 2;
			*dst++ = red;
			*dst++ = green;
			*dst++ = blue;
			if ( red >= redMin && red <= redMax && green >= greenMin && green <= greenMax && blue >= blueMin && blue <= blueMax ) {
				*dst = keyAlpha;
			} else {
				*dst = alpha;
			}
			dst++;
		}
		break;
	}
	}
	return true;
}

/*
================
NiRawImageData::Unknown7DFFF0

Copies the part of another image from a column and row on into this
image's top left corner, clipped to both images, converting the source's
format to this one's; an RGB or palettized source gets the alpha given
(0 to 1) when this image is RGBA. False for a missing source, a
palettized destination or a corner outside the source. No caller in
retail.
================
*/
// FUNCTION: STBC 0x7DFFF0
bool NiRawImageData::Unknown7DFFF0( NiRawImageData *source, unsigned int x, unsigned int y, float alpha ) {
	if ( !source || format == FORMAT_PALETTIZED ) {
		return false;
	}

	unsigned char alphaByte = (unsigned char)( alpha * 255.0f );
	unsigned char *dstRow = data;
	if ( x >= source->width || y >= source->height ) {
		return false;
	}

	const unsigned char *srcRow = ( *source )( x, y );
	unsigned int columns = source->width - x;
	unsigned int rows = source->height - y;
	if ( columns > width ) {
		columns = width;
	}
	if ( rows > height ) {
		rows = height;
	}

	if ( format == FORMAT_RGBA ) {
		switch ( source->format ) {
		case FORMAT_RGBA: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				memcpy( dstRow, srcRow, columns * 4 );
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		case FORMAT_RGB: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				unsigned char *dst = dstRow;
				const unsigned char *src = srcRow;
				for ( unsigned int i = 0; i < columns; i++ ) {
					*dst++ = *src++;
					*dst++ = *src++;
					*dst++ = *src++;
					*dst++ = alphaByte;
				}
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		case FORMAT_PALETTIZED: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				unsigned char *dst = dstRow;
				const unsigned char *src = srcRow;
				for ( unsigned int i = 0; i < columns; i++ ) {
					NiPaletteEntry *entry = &source->palette[*src++];
					*dst++ = entry->red;
					*dst++ = entry->green;
					*dst++ = entry->blue;
					*dst++ = alphaByte;
				}
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		}
	} else {
		switch ( source->format ) {
		case FORMAT_RGBA: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				unsigned char *dst = dstRow;
				const unsigned char *src = srcRow;
				for ( unsigned int i = 0; i < columns; i++ ) {
					*dst++ = *src++;
					*dst++ = *src++;
					*dst++ = *src++;
					src++;
				}
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		case FORMAT_RGB: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				memcpy( dstRow, srcRow, columns * 3 );
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		case FORMAT_PALETTIZED: {
			for ( unsigned int j = 0; j < rows; j++ ) {
				unsigned char *dst = dstRow;
				const unsigned char *src = srcRow;
				for ( unsigned int i = 0; i < columns; i++ ) {
					NiPaletteEntry *entry = &source->palette[*src++];
					*dst++ = entry->red;
					*dst++ = entry->green;
					*dst++ = entry->blue;
				}
				srcRow += source->rowStride;
				dstRow += rowStride;
			}
			break;
		}
		}
	}
	return true;
}

/*
================
NiRawImageData::CreateClone

An image is shared by its clones, not copied.
================
*/
// FUNCTION: STBC 0x7E0300
NiObject *NiRawImageData::CreateClone() {
	return this;
}

/*
================
NiRawImageData::CreateObject
================
*/
// FUNCTION: STBC 0x7E0320
void NiRawImageData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiRawImageData *image = new NiRawImageData;
	stream.SetLoadRecord( new NiRawImageDataLoadRecord );
	image->LoadBinary( stream );
	object = image;
}

/*
================
NiRawImageData::LoadBinary

The width, height and format, then the palette of a palettized image and
the pixels.

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x40DDE1, NiRawImageData.cpp): the early return, both blocks read through
NiStreamLoadBinary, and the locals' original names, which decide their
frame slots there (width_ and height_ swap them).
================
*/
// FUNCTION: STBC 0x7E0400
void NiRawImageData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int uiWidth, uiHeight;
	Format eFormat;
	NiStreamLoadBinary( stream, uiWidth );
	NiStreamLoadBinary( stream, uiHeight );
	NiStreamLoadBinary( stream, eFormat );
	if ( !Create( eFormat, uiWidth, uiHeight ) ) {
		return;
	}

	if ( format == FORMAT_PALETTIZED ) {
		NiStreamLoadBinary( stream, (unsigned char *)palette, 256 * sizeof( NiPaletteEntry ) );
	}
	NiStreamLoadBinary( stream, data, height * rowStride );
}

/*
================
NiRawImageData::LinkObject
================
*/
// FUNCTION: STBC 0x7E0490
void NiRawImageData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiRawImageData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7E04A0
bool NiRawImageData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiRawImageData::SaveBinary
================
*/
// FUNCTION: STBC 0x7E04B0
void NiRawImageData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, width );
	NiStreamSaveBinary( stream, height );
	NiStreamSaveEnum( stream, format );
	if ( format == FORMAT_PALETTIZED ) {
		assert( palette );
		NiStreamSaveBinary( stream, (unsigned char *)palette, 256 * sizeof( NiPaletteEntry ) );
	}
	NiStreamSaveBinary( stream, data, height * rowStride );
}

/*
================
NiRawImageData::IsEqual
================
*/
// FUNCTION: STBC 0x7E0520
bool NiRawImageData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiRawImageData *image = (NiRawImageData *)object;
	if ( width != image->width || height != image->height || format != image->format ) {
		return false;
	}
	if ( format == FORMAT_PALETTIZED ) {
		assert( palette );
		if ( memcmp( palette, image->palette, 256 * sizeof( NiPaletteEntry ) ) != 0 ) {
			return false;
		}
	}
	if ( memcmp( data, image->data, height * rowStride ) != 0 ) {
		return false;
	}
	return true;
}

/*
================
NiRawImageData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7E0590
// PARKED: inline budget: retail expands NiTArray::SetAt for the last two Adds and calls SetSize out of line; this build calls SetAt for the palette string
void NiRawImageData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( GetViewerString( "m_eFormat", format ) );
	strings->Add( NiGetViewerString( "m_uiWidth", width ) );
	strings->Add( NiGetViewerString( "m_uiHeight", height ) );
	strings->Add( NiGetViewerString( "m_uiPixelStride", pixelStride ) );
	strings->Add( NiGetViewerString( "m_uiRowStride", rowStride ) );
	strings->Add( NiGetViewerString( "m_pPal", palette ) );
	strings->Add( NiGetViewerString( "m_pData", data ) );
}

/*
================
NiRawImageData::GetViewerString

Returns "<prefix> = FORMAT_PALETTIZED", "FORMAT_RGB" or "FORMAT_RGBA" in a
new string that the caller deletes.
================
*/
// FUNCTION: STBC 0x7E0810
char *NiRawImageData::GetViewerString( const char *prefix, Format format_ ) {
	char *string = new char[strlen( prefix ) + 21];
	switch ( format_ ) {
	case FORMAT_PALETTIZED:
		sprintf( string, "%s = FORMAT_PALETTIZED", prefix );
		break;
	case FORMAT_RGB:
		sprintf( string, "%s = FORMAT_RGB", prefix );
		break;
	case FORMAT_RGBA:
		sprintf( string, "%s = FORMAT_RGBA", prefix );
		break;
	}
	return string;
}
