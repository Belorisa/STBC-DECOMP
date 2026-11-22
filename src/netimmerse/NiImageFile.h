/*
===============================================================================

	NiImageFile.h

	Reads image files into raw pixel data: a NIF file that holds one
	NiRawImageData, or a Targa, Windows bitmap or SGI image read by its
	format's reader.

	Evidence: the reader (0x819000) strips a "file://" prefix, picks the
	format by the extension (".nif", ".tga", ".bmp", ".rgb", ".rgba",
	".int", ".inta") and opens the other formats through NiFile for the
	format readers (0x824C70, 0x824710, 0x823F60), each in a unit of its
	own. NiImage reads the files of its images through it. The names are
	invented: no strings name these functions.

===============================================================================
*/

#ifndef __NIIMAGEFILE_H__
#define __NIIMAGEFILE_H__

class NiFile;
class NiRawImageData;

// reads the named image file into image; false when the file cannot be read
// or holds no image
bool						NiReadImageFile( const char *file, NiRawImageData *image );

// the format readers: read an open file into image; false on failure
bool						NiReadTGA( NiFile &file, NiRawImageData *image );
bool						NiReadBMPFile( NiFile &file, NiRawImageData *image );
bool						NiReadSGIFile( NiFile &file, NiRawImageData *image );

#endif /* !__NIIMAGEFILE_H__ */
