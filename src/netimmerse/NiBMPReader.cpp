// NiBMPReader.cpp -- the Windows bitmap reader of NiReadImageFile (unit name invented)

#include "NiRTLib.h"
#include "NiFile.h"
#include "NiRawImageData.h"
#include "NiImageFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00824bd0 _$E54
// FUNCTION: STBC 0x00824bf0 _$E17
// FUNCTION: STBC 0x00824c10 _$E15
// FUNCTION: STBC 0x00824c20 _$E23
// FUNCTION: STBC 0x00824c40 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00824c50 _$E33
// FUNCTION: STBC 0x00824c60 _$E47

// the bitmap file's signature, "BM"
#define BMP_SIGNATURE			0x4D42
// the bytes of the file and info headers before the palette
#define BMP_HEADERS_SIZE		54

// the info header's compression
#define BMP_COMPRESSION_NONE		0
#define BMP_COMPRESSION_BITFIELDS	3

/*
================
NiReadBMPFile

Reads an uncompressed 4, 8, 24 or 32 bit bitmap, bottom-up or top-down,
turning the palette and the pixels from BGR to RGB. A 4 bit image is
widened to a byte per pixel. Statements as the debug copy has them
(AnimOnly.exe 0x4652D0, NiBMPImageFile.cpp: the reads through the binary
stream loaders, the image size kept apart from the header's, asserts on the
palette and the skip), which reads through NiBinaryStream wrappers of its
own (0x465C40, 0x4652B0) this tree writes as the loader they call.
================
*/
// FUNCTION: STBC 0x824710
bool NiReadBMPFile( NiFile &file, NiRawImageData *image ) {
	unsigned short signature;
	unsigned short planes;
	unsigned int imageSize;
	unsigned int colorsUsed;
	NiRawImageData::Format format;
	unsigned short bitCount;
	unsigned int size;
	unsigned int compression;
	unsigned int width;
	int height;
	unsigned int dataOffset;
	int xPelsPerMeter, yPelsPerMeter;
	unsigned int colorsImportant;
	unsigned int imageWidth = 0;
	unsigned int imageHeight = 0;
	bool bottomUp = false;

	NiBinaryStreamLoad( file, &signature, 1 );
	if ( signature != BMP_SIGNATURE ) {
		return false;
	}

	// the file header: the size and the reserved words are skipped
	NiBinaryStreamLoad( file, &size, 1 );
	file.Seek( 4, NiFile::seekCur );
	NiBinaryStreamLoad( file, &dataOffset, 1 );

	// the info header
	NiBinaryStreamLoad( file, &size, 1 );
	NiBinaryStreamLoad( file, &width, 1 );
	NiBinaryStreamLoad( file, &height, 1 );
	NiBinaryStreamLoad( file, &planes, 1 );
	NiBinaryStreamLoad( file, &bitCount, 1 );
	NiBinaryStreamLoad( file, &compression, 1 );
	NiBinaryStreamLoad( file, &imageSize, 1 );
	NiBinaryStreamLoad( file, &xPelsPerMeter, 1 );
	NiBinaryStreamLoad( file, &yPelsPerMeter, 1 );
	NiBinaryStreamLoad( file, &colorsUsed, 1 );
	NiBinaryStreamLoad( file, &colorsImportant, 1 );

	if ( size == 4 ) {
		return false;
	}
	if ( compression != BMP_COMPRESSION_NONE && bitCount == 32 && compression != BMP_COMPRESSION_BITFIELDS ) {
		return false;
	}

	switch ( bitCount ) {
	case 4:
	case 8:
		format = NiRawImageData::FORMAT_PALETTIZED;
		break;
	case 24:
		format = NiRawImageData::FORMAT_RGB;
		break;
	case 32:
		format = NiRawImageData::FORMAT_RGBA;
		break;
	default:
		return false;
	}

	// a positive height is a bottom-up image
	imageWidth = width;
	if ( height > 0 ) {
		bottomUp = true;
		imageHeight = height;
	} else {
		bottomUp = false;
		imageHeight = -height;
	}
	if ( !image->Create( format, imageWidth, imageHeight ) ) {
		return false;
	}

	if ( colorsUsed == 0 && bitCount != 24 ) {
		colorsUsed = 1 << bitCount;
	}
	if ( imageSize == 0 ) {
		imageSize = ( ( imageWidth * bitCount + 31 ) & ~31 ) / 8 * imageHeight;
	}

	if ( format == NiRawImageData::FORMAT_PALETTIZED ) {
		assert( image->GetPalette() );
		if ( colorsUsed ) {
			NiBinaryStreamLoad( file, (unsigned char *)image->GetPalette(), colorsUsed * 4 );
		}
		NiPaletteEntry *entry = image->GetPalette();
		for ( unsigned int i = 0; i < colorsUsed; i++, entry++ ) {
			unsigned char blue = entry->red;
			entry->red = entry->blue;
			entry->blue = blue;
		}
	} else {
		file.Seek( colorsUsed * 4, NiFile::seekCur );
	}

	int skip = dataOffset - BMP_HEADERS_SIZE - colorsUsed * 4;
	assert( skip >= 0 );
	if ( skip > 0 ) {
		file.Seek( skip, NiFile::seekCur );
	}

	if ( bitCount == 4 ) {
		// two pixels a byte, the high nibble first
		unsigned char *buffer = new unsigned char[imageWidth / 2 + 5];
		unsigned int rowBytes = imageWidth / 2 + ( imageWidth & 1 );
		unsigned int padding = 0;
		if ( rowBytes & 3 ) {
			padding = 4 - ( rowBytes & 3 );
		}
		if ( bottomUp ) {
			unsigned int stride = image->GetRowStride();
			unsigned int rows = image->GetHeight();
			unsigned char *row = image->GetPixels() + ( image->GetHeight() - 1 ) * stride;
			for ( unsigned int y = 0; y < rows; y++, row -= stride ) {
				NiBinaryStreamLoad( file, buffer, rowBytes + padding );
				unsigned char *pixel = row;
				unsigned char *source = buffer;
				for ( unsigned int x = 0; x < rowBytes; x++ ) {
					*pixel++ = *source >> 4;
					*pixel++ = *source & 0xF;
					source++;
				}
			}
		} else {
			unsigned int stride = image->GetRowStride();
			unsigned int rows = image->GetHeight();
			unsigned char *row = image->GetPixels();
			for ( unsigned int y = 0; y < rows; y++, row += stride ) {
				NiBinaryStreamLoad( file, buffer, rowBytes + padding );
				unsigned char *pixel = row;
				unsigned char *source = buffer;
				for ( unsigned int x = 0; x < rowBytes; x++ ) {
					*pixel++ = *source >> 4;
					*pixel++ = *source & 0xF;
					source++;
				}
			}
		}
		delete [] buffer;
	} else if ( bottomUp ) {
		unsigned int stride = image->GetRowStride();
		unsigned int rows = image->GetHeight();
		unsigned int padding = 0;
		if ( stride & 3 ) {
			padding = 4 - ( stride & 3 );
		}
		unsigned char *row = image->GetPixels() + ( image->GetHeight() - 1 ) * stride;
		for ( unsigned int y = 0; y < rows; y++, row -= stride ) {
			NiBinaryStreamLoad( file, row, stride );
			if ( padding ) {
				unsigned char pad[4];
				NiBinaryStreamLoad( file, pad, padding );
			}
		}
	} else {
		unsigned int stride = image->GetRowStride();
		unsigned int rows = image->GetHeight();
		if ( stride & 3 ) {
			// set twice, as the debug copy does (AnimOnly.exe 0x4659B2)
			unsigned int padding = 0;
			padding = 4 - ( stride & 3 );
			// BUG: a top-down image with padded rows is read bottom-up
			unsigned char *row = image->GetPixels() + ( image->GetHeight() - 1 ) * stride;
			for ( unsigned int y = 0; y < rows; y++, row -= stride ) {
				NiBinaryStreamLoad( file, row, stride );
				if ( padding ) {
					unsigned char pad[4];
					NiBinaryStreamLoad( file, pad, padding );
				}
			}
		} else {
			NiBinaryStreamLoad( file, image->GetPixels(), imageSize );
		}
	}

	if ( format != NiRawImageData::FORMAT_PALETTIZED ) {
		unsigned int pixelStride = image->GetPixelStride();
		unsigned int count = image->GetWidth() * image->GetHeight();
		unsigned char *pixel = image->GetPixels();
		for ( unsigned int i = 0; i < count; i++, pixel += pixelStride ) {
			unsigned char blue = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = blue;
		}
	}
	return true;
}
