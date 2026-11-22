/*
===============================================================================

	ImageUtils.cpp

	The screen shot writers and the colour lookup tables (the unit and the
	file name are invented).

===============================================================================
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "ImageUtils.h"
#include "NiRawImageData.h"
#include "renderer/NiImage.h"
#include "renderer/NiD3DPixelFormat.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>), and the
// locale id's guard.
// FUNCTION: STBC 0x00459410 _$E80
// FUNCTION: STBC 0x00459430 _$E79

// The TGA file's footer: no extension or developer area, and the signature.
static const char TGA_SIGNATURE[] = "TRUEVISION-XFILE.";

/*
================
SaveTGAFile

The rows go bottom up, in the file's blue, green, red (and alpha) order:
converted in one block through the image's pixel format conversion, or
written pixel by pixel when the buffers can't be had.
================
*/
// FUNCTION: STBC 0x004588a0
// DRAFT: structure and calls follow retail; retail's frame is 0x2C bytes larger (more locals than the header, the format and the three channel arrays), so every slot is offset, and the conversion branch's stores are scheduled differently
bool SaveTGAFile( const char *fileName, NiRawImageData *image ) {
	if ( fileName == NULL || image == NULL ) {
		return false;
	}
	FILE *file = fopen( fileName, "wb" );
	if ( file == NULL ) {
		return false;
	}

	TGAFileHeader header;
	header.idLength = 0;
	fwrite( &header.idLength, 1, sizeof( header.idLength ), file );
	header.colorMapType = 0;
	fwrite( &header.colorMapType, 1, sizeof( header.colorMapType ), file );
	header.imageType = 2;
	fwrite( &header.imageType, 1, sizeof( header.imageType ), file );
	header.colorMapStart = 0;
	header.colorMapLength = 0;
	header.colorMapDepth = 0;
	fwrite( &header.colorMapStart, 1, sizeof( header.colorMapStart ), file );
	fwrite( &header.colorMapLength, 1, sizeof( header.colorMapLength ), file );
	fwrite( &header.colorMapDepth, 1, sizeof( header.colorMapDepth ), file );
	header.xOrigin = 0;
	header.yOrigin = 0;
	fwrite( &header.xOrigin, 1, sizeof( header.xOrigin ), file );
	fwrite( &header.yOrigin, 1, sizeof( header.yOrigin ), file );
	header.width = image->GetWidth();
	header.height = image->GetHeight();
	fwrite( &header.width, 1, sizeof( header.width ), file );
	fwrite( &header.height, 1, sizeof( header.height ), file );
	if ( image->GetFormat() == NiRawImageData::FORMAT_RGB || image->GetFormat() == NiRawImageData::FORMAT_PALETTIZED ) {
		header.bitsPerPixel = 24;
	} else {
		header.bitsPerPixel = 32;
	}
	fwrite( &header.bitsPerPixel, 1, sizeof( header.bitsPerPixel ), file );
	header.descriptor = 0;
	fwrite( &header.descriptor, 1, sizeof( header.descriptor ), file );

	unsigned int pixelStride = image->GetPixelStride();
	unsigned int rowStride = image->GetRowStride();
	unsigned char *pixels = new unsigned char[image->GetHeight() * rowStride];
	unsigned char *row = new unsigned char[rowStride];
	if ( pixels != NULL && row != NULL ) {
		NiPixelDesc format;
		unsigned char bits[4];
		unsigned char losses[4];
		unsigned char shifts[4];
		if ( image->GetFormat() == NiRawImageData::FORMAT_RGB || image->GetFormat() == NiRawImageData::FORMAT_PALETTIZED ) {
			bits[0] = 8;
			bits[1] = 8;
			bits[2] = 8;
			bits[3] = 0;
			losses[0] = 0;
			losses[1] = 0;
			losses[2] = 0;
			losses[3] = 8;
			shifts[0] = 16;
			shifts[1] = 8;
			shifts[2] = 0;
			shifts[3] = 0;
			format.palettized = false;
			format.bitsPerPixel = 24;
			format.masks[0] = 0xFF0000;
			format.masks[1] = 0xFF00;
			format.masks[2] = 0xFF;
			format.masks[3] = 0;
		} else {
			bits[0] = 8;
			bits[1] = 8;
			bits[2] = 8;
			bits[3] = 8;
			losses[0] = 0;
			losses[1] = 0;
			losses[2] = 0;
			losses[3] = 0;
			shifts[0] = 24;
			shifts[1] = 12;	// BUG: the green channel sits at bit 16, not 12
			shifts[2] = 8;
			shifts[3] = 0;
			format.palettized = false;
			format.bitsPerPixel = 32;
			format.masks[0] = 0xFF000000;
			format.masks[1] = 0xFF0000;
			format.masks[2] = 0xFF00;
			format.masks[3] = 0xFF;
		}
		memcpy( format.bits, bits, sizeof( bits ) );
		memcpy( format.losses, losses, sizeof( losses ) );
		memcpy( format.shifts, shifts, sizeof( shifts ) );
		if ( !image->Unknown7DEEE0( format, rowStride, pixels ) ) {
			delete pixels;
			delete row;
			fclose( file );
			return false;
		}
		int bottom = image->GetHeight() - 1;
		for ( int top = 0; top < bottom; top++, bottom-- ) {
			memcpy( row, pixels + bottom * rowStride, rowStride );
			memcpy( pixels + bottom * rowStride, pixels + top * rowStride, rowStride );
			memcpy( pixels + top * rowStride, row, rowStride );
		}
		fwrite( pixels, image->GetHeight() * rowStride, 1, file );
	} else {
		unsigned char *source = image->GetPixels() + ( header.height - 1 ) * rowStride;
		if ( image->GetFormat() == NiRawImageData::FORMAT_RGB ) {
			for ( unsigned int y = 0; y < header.height; y++ ) {
				unsigned char *pixel = source;
				for ( unsigned int x = 0; x < header.width; x++ ) {
					fwrite( &pixel[2], 1, 1, file );
					fwrite( &pixel[1], 1, 1, file );
					fwrite( &pixel[0], 1, 1, file );
					pixel += pixelStride;
				}
				source -= rowStride;
			}
		} else if ( image->GetFormat() == NiRawImageData::FORMAT_RGBA ) {
			for ( unsigned int y = 0; y < header.height; y++ ) {
				unsigned char *pixel = source;
				for ( unsigned int x = 0; x < header.width; x++ ) {
					fwrite( &pixel[2], 1, 1, file );
					fwrite( &pixel[1], 1, 1, file );
					fwrite( &pixel[0], 1, 1, file );
					fwrite( &pixel[3], 1, 1, file );
					pixel += pixelStride;
				}
				source -= rowStride;
			}
		} else {
			delete pixels;
			delete row;
			fclose( file );
			return false;
		}
	}

	unsigned int offset = 0;
	fwrite( &offset, sizeof( offset ), 1, file );
	fwrite( &offset, sizeof( offset ), 1, file );
	fwrite( TGA_SIGNATURE, 1, sizeof( TGA_SIGNATURE ), file );
	delete pixels;
	delete row;
	fclose( file );
	return true;
}

/*
================
SaveBMPFile

The rows go bottom up, each padded to an even pixel count, through the
image's pixel format conversion.
================
*/
// FUNCTION: STBC 0x00458e40
// DRAFT: retail keeps the file pointer in the name parameter's slot and drops the format constructor's zeroing stores (every field is set after it); this build keeps both
bool SaveBMPFile( const char *fileName, NiRawImageData *image ) {
	if ( fileName != NULL && image != NULL ) {
		FILE *file = fopen( fileName, "wb" );
		if ( file != NULL ) {
			unsigned int width = image->GetWidth();
			int padding = 0;
			if ( width & 1 ) {
				padding = 3;
			}
			unsigned int rowSize = width * 3 + padding;

			BITMAPFILEHEADER fileHeader = { 0 };
			fileHeader.bfType = 0x4D42;	// "BM"
			fileHeader.bfSize = image->GetHeight() * rowSize + sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
			fileHeader.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
			fwrite( &fileHeader, sizeof( fileHeader ), 1, file );

			BITMAPINFOHEADER infoHeader;
			memset( &infoHeader, 0, sizeof( infoHeader ) );
			infoHeader.biSize = sizeof( infoHeader );
			infoHeader.biWidth = width;
			if ( image->GetWidth() & 1 ) {
				infoHeader.biWidth++;
			}
			infoHeader.biHeight = image->GetHeight();
			infoHeader.biPlanes = 1;
			infoHeader.biBitCount = 24;
			infoHeader.biSizeImage = image->GetHeight() * rowSize;
			fwrite( &infoHeader, sizeof( infoHeader ), 1, file );

			unsigned char *pixels = new unsigned char[image->GetHeight() * rowSize];
			unsigned char *row = new unsigned char[rowSize];
			if ( pixels != NULL && row != NULL ) {
				NiPixelDesc format;
				format.palettized = false;
				format.bitsPerPixel = 24;
				format.masks[0] = 0xFF0000;
				format.masks[1] = 0xFF00;
				format.masks[2] = 0xFF;
				format.masks[3] = 0;
				format.bits[0] = 8;
				format.bits[1] = 8;
				format.bits[2] = 8;
				format.bits[3] = 0;
				format.losses[0] = 0;
				format.losses[1] = 0;
				format.losses[2] = 0;
				format.losses[3] = 8;
				format.shifts[0] = 16;
				format.shifts[1] = 8;
				format.shifts[2] = 0;
				format.shifts[3] = 0;
				format.unknown20 = 0;
				if ( !image->Unknown7DEEE0( format, rowSize, pixels ) ) {
					delete pixels;
					delete row;
					fclose( file );
					return false;
				}
				int bottom = image->GetHeight() - 1;
				for ( int top = 0; top < bottom; top++, bottom-- ) {
					memcpy( row, pixels + bottom * rowSize, rowSize );
					memcpy( pixels + bottom * rowSize, pixels + top * rowSize, rowSize );
					memcpy( pixels + top * rowSize, row, rowSize );
				}
				fwrite( pixels, image->GetHeight() * rowSize, 1, file );
				delete pixels;
				delete row;
				fclose( file );
				return true;
			}
			delete pixels;
			delete row;
			fclose( file );
		}
	}
	return false;
}

/*
================
BuildScaleTable
================
*/
// FUNCTION: STBC 0x00459130
void BuildScaleTable( unsigned char *table, float scale ) {
	for ( int i = 0; i < 256; i++ ) {
		table[i] = (unsigned char)( i * scale );
	}
}

/*
================
BuildGammaTable
================
*/
// FUNCTION: STBC 0x00459160
void BuildGammaTable( unsigned char *table, float gamma ) {
	float exponent = 1.0f / gamma;
	float scale = 255.0f / (float)pow( 255.0, exponent );
	for ( int i = 0; i < 256; i++ ) {
		table[i] = (unsigned char)( pow( i, exponent ) * scale );
	}
}

/*
================
BuildGammaRangeTable
================
*/
// FUNCTION: STBC 0x004591d0
void BuildGammaRangeTable( unsigned char *table, float inLow, float gamma, float inHigh, float outLow,
						   float outHigh ) {
	gamma = 1.0f / gamma;
	float top = pow( 1.0, gamma );
	float scale = ( inHigh - inLow ) / top;
	outHigh -= outLow;
	outLow *= 255.0f;
	for ( int i = 0; i < 256; i++ ) {
		table[i] = (unsigned char)( ( pow( i * ( 1.0f / 255.0f ), gamma ) * scale + inLow ) * outHigh + outLow );
	}
}

/*
================
BuildLevelsTable
================
*/
// FUNCTION: STBC 0x00459270
void BuildLevelsTable( unsigned char *table, float brightness, float contrast ) {
	float offset = brightness * 255.0f;
	for ( int i = 0; i < 256; i++ ) {
		float value = i * contrast + offset;
		if ( value > 255.0f ) {
			table[i] = 255;
		} else if ( value < 0.0f ) {
			table[i] = 0;
		} else {
			table[i] = (unsigned char)value;
		}
	}
}

/*
================
ApplyColorTable
================
*/
// FUNCTION: STBC 0x004592e0 ?ApplyColorTable@@YAXPAVNiRawImageData@@PBE@Z
void ApplyColorTable( NiRawImageData *image, const unsigned char *table ) {
	if ( image != NULL ) {
		int width = image->GetWidth();
		int pixelStride = image->GetPixelStride();
		int rowStride = image->GetRowStride();
		int height = image->GetHeight();
		unsigned char *pixel = image->GetPixels();
		int rowPadding = rowStride - pixelStride * width;
		for ( int y = 0; y < height; y++ ) {
			for ( int x = 0; x < width; x++ ) {
				*pixel = table[*pixel];
				pixel++;
				*pixel = table[*pixel];
				pixel++;
				*pixel = table[*pixel];
				pixel += pixelStride - 2;
			}
			pixel += rowPadding;
		}
	}
}

/*
================
ApplyColorTable
================
*/
// FUNCTION: STBC 0x00459360 ?ApplyColorTable@@YAXPAVNiImage@@PBE@Z
void ApplyColorTable( NiImage *image, const unsigned char *table ) {
	if ( image != NULL ) {
		ApplyColorTable( image->GetRawImageData(), table );
		image->unknown20++;
		image->VirtualSlot12();
	}
}
