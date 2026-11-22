// NiSGIReader.cpp -- the SGI image reader of NiReadImageFile (unit name invented)

#include "NiRTLib.h"
#include "NiFile.h"
#include "NiRawImageData.h"
#include "NiImageFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00824670 _$E54
// FUNCTION: STBC 0x00824690 _$E17
// FUNCTION: STBC 0x008246b0 _$E15
// FUNCTION: STBC 0x008246c0 _$E23
// FUNCTION: STBC 0x008246e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008246f0 _$E33
// FUNCTION: STBC 0x00824700 _$E47

// the SGI image file's magic number
#define SGI_MAGIC				474
// the header's storage formats
#define SGI_STORAGE_VERBATIM	0
#define SGI_STORAGE_RLE			1
// the size of the header block; the pixels or the RLE tables follow it
#define SGI_HEADER_SIZE			512

// the leading fields of the 512 byte SGI header, in the file's order
struct NiSGIHeader {
	unsigned short			magic;
	unsigned char			storage;		// SGI_STORAGE_VERBATIM or SGI_STORAGE_RLE
	unsigned char			bpc;			// bytes per channel: 1 or 2
	unsigned short			dimension;
	unsigned short			xsize;
	unsigned short			ysize;
	unsigned short			zsize;			// channels: 1 grey, 2 grey and alpha, 3 RGB, 4 RGBA
	unsigned int			pixmin;
	unsigned int			pixmax;
	char					dummy[4];
	char					name[80];
	unsigned int			colormap;
};

// reads one channel value from the pixel data, advancing the cursor
typedef unsigned char ( *NiSGIGetChannel )( unsigned char *&cursor );

// reads one row of one channel into dest, a channel value every stride bytes
typedef void ( *NiSGIReadRow )( unsigned char *dest, int row, int channel, NiSGIHeader *header,
	unsigned long *starts, NiSGIGetChannel getChannel, unsigned char *&cursor, int stride );

/*
================
GetByte

The file's numbers are big-endian.
================
*/
static unsigned char GetByte( NiFile &file ) {
	unsigned char value;
	file.Read( &value, sizeof( value ) );
	return value;
}

/*
================
GetShort
================
*/
static unsigned short GetShort( NiFile &file ) {
	unsigned char bytes[2];
	file.Read( bytes, sizeof( bytes ) );
	return ( bytes[0] << 8 ) + bytes[1];
}

/*
================
GetLong
================
*/
static unsigned int GetLong( NiFile &file ) {
	unsigned char bytes[4];
	file.Read( bytes, sizeof( bytes ) );
	return ( ( ( ( ( bytes[0] << 8 ) + bytes[1] ) << 8 ) + bytes[2] ) << 8 ) + bytes[3];
}

/*
================
GetByteChannel
================
*/
// FUNCTION: STBC 0x824500
static unsigned char GetByteChannel( unsigned char *&cursor ) {
	return *cursor++;
}

/*
================
GetShortChannel

A 16 bit value, scaled down to a byte (as 255 to 1).
================
*/
// FUNCTION: STBC 0x824510
static unsigned char GetShortChannel( unsigned char *&cursor ) {
	unsigned short value;
	unsigned char *bytes = (unsigned char *)&value;
	bytes[1] = *cursor++;
	bytes[0] = *cursor++;
	return value / 255;
}

/*
================
ReadVerbatimRow

The values follow one another in the data.
================
*/
// FUNCTION: STBC 0x824550
static void ReadVerbatimRow( unsigned char *dest, int row, int channel, NiSGIHeader *header,
	unsigned long *starts, NiSGIGetChannel getChannel, unsigned char *&cursor, int stride ) {
	unsigned short width = header->xsize;
	for ( unsigned short x = 0; x < width; x++ ) {
		*dest = getChannel( cursor );
		dest += stride;
	}
}

/*
================
ReadRLERow

The row starts where the start table says, as an offset into the file;
the data holds the file from the end of the two tables on. A count byte
with the high bit set is followed by that many values; without it, by one
value repeated that many times. A zero byte ends the row.
================
*/
// FUNCTION: STBC 0x824590
// PARKED: register: the loop layout matches (while ( true ) with break); retail makes the stride parameter a register candidate (EBP, loaded on entry) and puts offset in EDI, where this build leaves stride in memory (no candidate in c2trace --regs) and offset in EBP; unsigned offset and function-scope declarations didn't move it
static void ReadRLERow( unsigned char *dest, int row, int channel, NiSGIHeader *header,
	unsigned long *starts, NiSGIGetChannel getChannel, unsigned char *&cursor, int stride ) {
	int offset = starts[header->ysize * channel + row]
		- ( header->ysize * header->zsize * 2 * sizeof( unsigned int ) + SGI_HEADER_SIZE );
	while ( true ) {
		unsigned char pixel = cursor[offset];
		if ( !pixel ) {
			break;
		}
		unsigned char *run = cursor + offset + 1;
		unsigned char count = pixel & 0x7F;
		if ( pixel & 0x80 ) {
			offset += count + 1;
			while ( count-- ) {
				*dest = getChannel( run );
				dest += stride;
			}
		} else {
			offset += 2;
			unsigned char value = getChannel( run );
			while ( count-- ) {
				*dest = value;
				dest += stride;
			}
		}
	}
}

/*
================
NiReadSGIFile

Reads an SGI image of 1 to 4 channels, verbatim or run-length encoded,
with 8 or 16 bits a channel (RLE only with 8). Grey images are spread to
RGB; a second or fourth channel is the alpha. The rows are stored bottom
up.
================
*/
// FUNCTION: STBC 0x823F60
bool NiReadSGIFile( NiFile &file, NiRawImageData *image ) {
	unsigned long *starts = NULL;
	unsigned char *data = NULL;
	bool result = false;
	unsigned char *buffer = NULL;
	int dataSize = 0;
	NiSGIHeader header;
	NiSGIReadRow readRow;

	header.magic = GetShort( file );
	if ( header.magic != SGI_MAGIC ) {
		return false;
	}
	if ( !file ) {
		return false;
	}
	header.storage = GetByte( file );
	if ( header.storage != SGI_STORAGE_RLE && header.storage != SGI_STORAGE_VERBATIM ) {
		return false;
	}
	if ( !file ) {
		return false;
	}
	header.bpc = GetByte( file );
	if ( header.bpc > 2 || header.bpc < 1 ) {
		return false;
	}
	header.dimension = GetShort( file );
	header.xsize = GetShort( file );
	header.ysize = GetShort( file );
	header.zsize = GetShort( file );
	header.pixmin = GetLong( file );
	header.pixmax = GetLong( file );
	file.Seek( sizeof( header.dummy ) + sizeof( header.name ), NiFile::seekCur );
	header.colormap = GetLong( file );
	file.Seek( SGI_HEADER_SIZE, NiFile::seekSet );

	if ( header.storage == SGI_STORAGE_RLE ) {
		// the start table, then the length table, a row of a channel each
		int tableSize = header.ysize * header.zsize;
		if ( header.bpc != 1 ) {
			return false;
		}
		starts = new unsigned long[tableSize];
		if ( !starts ) {
			return false;
		}
		unsigned long *start = starts;
		int i;
		for ( i = 0; i < tableSize; i++ ) {
			*start++ = GetLong( file );
		}
		for ( i = 0; i < tableSize; i++ ) {
			dataSize += GetLong( file );
		}
		readRow = ReadRLERow;
	} else {
		readRow = ReadVerbatimRow;
		dataSize = header.xsize * header.ysize * header.zsize * header.bpc;
	}

	bool alpha = header.zsize > 3 || header.zsize == 2;
	if ( image->Create( alpha ? NiRawImageData::FORMAT_RGBA : NiRawImageData::FORMAT_RGB, header.xsize, header.ysize ) ) {
		int channels = header.zsize < 3 ? 1 : 3;
		NiSGIGetChannel getChannel = header.bpc == 2 ? GetShortChannel : GetByteChannel;
		data = new unsigned char[dataSize];
		if ( data ) {
			if ( header.bpc == 1 ) {
				file.Read( data, dataSize );
			} else {
				unsigned short *sample = (unsigned short *)data;
				for ( int count = dataSize >> 1; count > 0; count-- ) {
					*sample++ = GetShort( file );
				}
			}
			buffer = data;

			unsigned int stride = image->rowStride;
			unsigned char *pixels = image->data;
			int pixelSize = alpha ? 4 : 3;
			int channel, row;
			for ( channel = 0; channel < 3; channel++ ) {
				// a grey image is read again for each color
				if ( channels == 1 ) {
					data = buffer;
				}
				for ( row = 0; row < header.ysize; row++ ) {
					readRow( pixels + ( header.ysize - row - 1 ) * stride + channel, row,
						channels == 3 ? channel : 0, &header, starts, getChannel, data, pixelSize );
				}
			}
			if ( alpha ) {
				for ( row = 0; row < header.ysize; row++ ) {
					readRow( pixels + ( header.ysize - row - 1 ) * stride + 3, row,
						channels, &header, starts, getChannel, data, pixelSize );
				}
			}
			result = true;
		}
	}

	if ( buffer ) {
		delete [] buffer;
	}
	if ( starts ) {
		delete [] starts;
	}
	return result;
}
