// NiTGAReader.cpp -- the Targa reader of NiReadImageFile (unit name invented)
//
// The reader follows netpbm's tgatoppm (Jef Poskanzer's get_pixel,
// get_map_entry and readtga, with its static decoder state), changed to
// fill an NiRawImageData and to keep a 32 bit image's alpha.

#include "NiRTLib.h"
#include "NiFile.h"
#include "NiRawImageData.h"
#include "NiImageFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00825750 _$E54
// FUNCTION: STBC 0x00825770 _$E17
// FUNCTION: STBC 0x00825790 _$E15
// FUNCTION: STBC 0x008257a0 _$E23
// FUNCTION: STBC 0x008257c0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008257d0 _$E33
// FUNCTION: STBC 0x008257e0 _$E47

// the image types
#define TGA_MAP				1
#define TGA_RGB				2
#define TGA_MONO			3
#define TGA_RLEMAP			9
#define TGA_RLERGB			10
#define TGA_RLEMONO			11
#define TGA_COMPMAP			32
#define TGA_COMPMAP4		33

// the row interleaving
#define TGA_IL_TWO			1
#define TGA_IL_FOUR			2

// the largest color map
#define TGA_MAX_COLORS		16384

// the 18 byte file header; the last byte is split into the four fields
// after it
struct NiTGAHeader {
	unsigned char			idLength;
	unsigned char			colorMapType;
	unsigned char			imageType;
	unsigned char			mapIndexLo, mapIndexHi;
	unsigned char			mapLengthLo, mapLengthHi;
	unsigned char			mapEntrySize;
	unsigned char			xOriginLo, xOriginHi;
	unsigned char			yOriginLo, yOriginHi;
	unsigned char			widthLo, widthHi;
	unsigned char			heightLo, heightHi;
	unsigned char			pixelSize;
	unsigned char			attributeBits;
	unsigned char			reserved;
	unsigned char			originTop;		// the first row is the top one
	unsigned char			interleave;		// TGA_IL_TWO, TGA_IL_FOUR or none
};

// the bytes of the header the file stores before the flags byte
#define TGA_HEADER_FIELDS_SIZE	17

// a color map entry packed red, green, blue, alpha from the low byte up;
// the readers take its bytes in memory order
#define TGA_PACK( r, g, b, a )	( ( ( ( ( ( a ) << 8 ) | ( b ) ) << 8 | ( g ) ) << 8 ) | ( r ) )
#define TGA_RED( p )			( ( (unsigned char *)&( p ) )[0] )
#define TGA_GREEN( p )			( ( (unsigned char *)&( p ) )[1] )
#define TGA_BLUE( p )			( ( (unsigned char *)&( p ) )[2] )
#define TGA_ALPHA( p )			( ( (unsigned char *)&( p ) )[3] )

// the decoder state
static bool mapped;
static bool rlEncoded;
static unsigned int colorMap[TGA_MAX_COLORS];
static int rleCount;
static int rleFlag;

static void GetPixel24( NiFile &file, unsigned char *dest, int size );

/*
================
GetByte

One byte of the file, through NiStream.h's NiBinaryStreamLoad: retail
calls its unsigned char copy (0x803650) where VC6 stops expanding it.
================
*/
static unsigned char GetByte( NiFile &file ) {
	unsigned char value;
	NiBinaryStreamLoad( file, &value, 1 );
	return value;
}

/*
================
GetMapEntry

Reads a color map entry of size bits into value.
================
*/
static void GetMapEntry( NiFile &file, unsigned int *value, int size ) {
	unsigned char j, k, r, g, b, a;

	switch ( size ) {
	case 8:
		// grey: read and triplicate
		r = g = b = GetByte( file );
		a = 255;
		break;
	case 16:
	case 15:
		// five bits each of red, green and blue
		j = GetByte( file );
		k = GetByte( file );
		r = ( k & 0x7C ) << 1;
		g = ( ( k & 0x03 ) << 6 ) + ( ( j & 0xE0 ) >> 2 );
		b = j << 3;
		a = 255;
		break;
	case 32:
	case 24:
		// eight bits each of blue, green, red and alpha
		b = GetByte( file );
		g = GetByte( file );
		r = GetByte( file );
		if ( size == 32 ) {
			a = GetByte( file );
		} else {
			a = 255;
		}
		break;
	}
	*value = TGA_PACK( r, g, b, a );
}

/*
================
GetPixelRGBA

Reads one pixel of size bits, through the run-length decoder of an encoded
image, into four bytes at dest.
================
*/
static void GetPixelRGBA( NiFile &file, unsigned char *dest, int size ) {
	static unsigned char red, green, blue, alpha;
	static unsigned int index;
	unsigned char j, k;

	if ( rlEncoded ) {
		if ( rleCount == 0 ) {
			// a new run
			unsigned char i = GetByte( file );
			rleFlag = i & 0x80;
			if ( rleFlag == 0 ) {
				// a stream of raw pixels
				rleCount = i + 1;
			} else {
				// one pixel repeated
				rleCount = i - 127;
			}
			rleCount--;
		} else {
			rleCount--;
			if ( rleFlag != 0 ) {
				goto encode;
			}
		}
	}

	switch ( size ) {
	case 8:
		red = green = blue = index = GetByte( file );
		alpha = 255;
		break;
	case 16:
	case 15:
		j = GetByte( file );
		k = GetByte( file );
		index = ( (unsigned int)k << 8 ) + j;
		red = ( k & 0x7C ) << 1;
		green = ( ( k & 0x03 ) << 6 ) + ( ( j & 0xE0 ) >> 2 );
		blue = j << 3;
		alpha = 255;
		break;
	// 32 before 24, as in GetPixel24: retail expands the 24 bit reads and
	// the last 32 bit one and calls the first three, which the reader's
	// nested budget gives only in this order
	case 32:
		blue = GetByte( file );
		green = GetByte( file );
		red = GetByte( file );
		alpha = GetByte( file );
		index = 0;
		break;
	case 24:
		blue = GetByte( file );
		green = GetByte( file );
		red = GetByte( file );
		alpha = 255;
		index = 0;
		break;
	}

encode:
	if ( mapped ) {
		// BUG: the pixel's index is not used: every mapped pixel takes the first entry
		dest[0] = TGA_RED( colorMap[0] );
		dest[1] = TGA_GREEN( colorMap[0] );
		dest[2] = TGA_BLUE( colorMap[0] );
		dest[3] = TGA_ALPHA( colorMap[0] );
	} else {
		dest[0] = red;
		dest[1] = green;
		dest[2] = blue;
		dest[3] = alpha;
	}
}

/*
================
ReadHeader

Reads the file header and splits its flags byte; skips the image id.
netpbm's tgatoppm, which the reader follows, reads it in its own static
readtga( ifp, tgaP ); retail's call set needs this level (it moves 106
tuples out of the reader's own cost). The name is invented.
================
*/
static void ReadHeader( NiFile &file, NiTGAHeader *header ) {
	unsigned char flags;

	file.Read( header, TGA_HEADER_FIELDS_SIZE );
	file.Read( &flags, sizeof( flags ) );
	header->attributeBits = flags & 0x0F;
	header->reserved = ( flags & 0x10 ) >> 4;
	header->originTop = ( flags & 0x20 ) >> 5;
	header->interleave = ( flags & 0xC0 ) >> 6;
	if ( header->idLength != 0 ) {
		file.Seek( header->idLength, NiFile::seekCur );
	}
}

/*
================
NiReadTGA

Reads a color-mapped, true-color or grey Targa image, plain or run-length
encoded, of 8, 15, 16, 24 or 32 bits a pixel; a 32 bit image keeps its
alpha. The rows may be stored bottom up and interleaved.
================
*/
// FUNCTION: STBC 0x824C70
// PARKED: frame and registers: the call set is retail's (ReadHeader, GetMapEntry and GetPixelRGBA expanded, 9 byte reads and GetPixel24 out of line); retail's frame has one more 4-byte slot and stores trueRow and baseRow in each branch, but initialising them per branch costs 8 tuples and the budget at GetPixel24 allows 2 more
bool NiReadTGA( NiFile &file, NiRawImageData *image ) {
	NiTGAHeader header;
	bool alpha;
	int row, col;

	ReadHeader( file, &header );

	int rows = header.heightLo + header.heightHi * 256;
	int cols = header.widthLo + header.widthHi * 256;

	switch ( header.imageType ) {
	case TGA_MAP:
	case TGA_RGB:
	case TGA_MONO:
	case TGA_RLEMAP:
	case TGA_RLERGB:
	case TGA_RLEMONO:
		break;
	default:
		return false;
	}

	if ( header.imageType == TGA_MAP || header.imageType == TGA_RLEMAP
		|| header.imageType == TGA_COMPMAP || header.imageType == TGA_COMPMAP4 ) {
		// a color-mapped image
		if ( header.colorMapType != 1 ) {
			return false;
		}
		mapped = true;
		alpha = header.mapEntrySize == 32;
		switch ( header.mapEntrySize ) {
		case 8:
		case 24:
		case 32:
		case 15:
		case 16:
			break;
		default:
			return false;
		}
	} else {
		mapped = false;
		alpha = header.pixelSize == 32;
		switch ( header.pixelSize ) {
		case 8:
		case 24:
		case 32:
		case 15:
		case 16:
			break;
		default:
			return false;
		}
	}

	if ( header.colorMapType != 0 ) {
		unsigned int first = header.mapIndexLo + header.mapIndexHi * 256;
		unsigned int length = header.mapLengthLo + header.mapLengthHi * 256;
		if ( first + length + 1 >= TGA_MAX_COLORS ) {
			return false;
		}
		for ( unsigned int i = first; i < first + length; i++ ) {
			GetMapEntry( file, &colorMap[i], header.mapEntrySize );
		}
	}

	if ( header.imageType == TGA_RLEMAP || header.imageType == TGA_RLERGB || header.imageType == TGA_RLEMONO ) {
		rlEncoded = true;
	} else {
		rlEncoded = false;
	}

	if ( !image->Create( alpha ? NiRawImageData::FORMAT_RGBA : NiRawImageData::FORMAT_RGB, cols, rows ) ) {
		return false;
	}

	int trueRow = 0;
	int baseRow = 0;
	if ( alpha ) {
		for ( row = 0; row < rows; row++ ) {
			int realRow = trueRow;
			if ( header.originTop == 0 ) {
				realRow = rows - realRow - 1;
			}
			for ( col = 0; col < cols; col++ ) {
				GetPixelRGBA( file, ( *image )( col, realRow ), header.pixelSize );
			}
			if ( header.interleave == TGA_IL_FOUR ) {
				trueRow += 4;
			} else if ( header.interleave == TGA_IL_TWO ) {
				trueRow += 2;
			} else {
				trueRow++;
			}
			if ( trueRow >= rows ) {
				trueRow = ++baseRow;
			}
		}
	} else {
		for ( row = 0; row < rows; row++ ) {
			int realRow = trueRow;
			if ( header.originTop == 0 ) {
				realRow = rows - realRow - 1;
			}
			for ( col = 0; col < cols; col++ ) {
				GetPixel24( file, ( *image )( col, realRow ), header.pixelSize );
			}
			if ( header.interleave == TGA_IL_FOUR ) {
				trueRow += 4;
			} else if ( header.interleave == TGA_IL_TWO ) {
				trueRow += 2;
			} else {
				trueRow++;
			}
			if ( trueRow >= rows ) {
				trueRow = ++baseRow;
			}
		}
	}
	return true;
}

/*
================
GetPixel24

As GetPixelRGBA, into three bytes; a mapped pixel takes its color map entry
and a 32 bit pixel's alpha is skipped. DamageTool.exe's symbols give the
name and show it static (storage class 3), with netpbm's static locals Red,
Grn, Blu and l. Retail keeps it out of line at 0x8254E0: the reader's
inline budget runs out at it. A mapped pixel's bytes are read through a
pointer into the entry (retail steps it once).
================
*/
// FUNCTION: STBC 0x008254e0
static void GetPixel24( NiFile &file, unsigned char *dest, int size ) {
	static unsigned char red, green, blue;
	static unsigned int index;
	unsigned char j, k;

	if ( rlEncoded ) {
		if ( rleCount == 0 ) {
			// a new run
			unsigned char i = GetByte( file );
			rleFlag = i & 0x80;
			if ( rleFlag == 0 ) {
				// a stream of raw pixels
				rleCount = i + 1;
			} else {
				// one pixel repeated
				rleCount = i - 127;
			}
			rleCount--;
		} else {
			rleCount--;
			if ( rleFlag != 0 ) {
				goto encode;
			}
		}
	}

	switch ( size ) {
	case 8:
		red = green = blue = index = GetByte( file );
		break;
	case 16:
	case 15:
		j = GetByte( file );
		k = GetByte( file );
		index = ( (unsigned int)k << 8 ) + j;
		red = ( k & 0x7C ) << 1;
		green = ( ( k & 0x03 ) << 6 ) + ( ( j & 0xE0 ) >> 2 );
		blue = j << 3;
		break;
	case 32:
		blue = GetByte( file );
		green = GetByte( file );
		red = GetByte( file );
		// the alpha byte is read and dropped
		GetByte( file );
		index = 0;
		break;
	case 24:
		blue = GetByte( file );
		green = GetByte( file );
		red = GetByte( file );
		index = 0;
		break;
	}

encode:
	if ( mapped ) {
		unsigned char *entry = (unsigned char *)&colorMap[index];
		dest[0] = *entry++;
		dest[1] = *entry++;
		dest[2] = *entry;
	} else {
		dest[0] = red;
		dest[1] = green;
		dest[2] = blue;
	}
}
