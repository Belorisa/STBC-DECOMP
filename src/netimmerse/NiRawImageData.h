/*
===============================================================================

	NiRawImageData.h

	An image's raw pixels: a format, a size, the strides of a pixel and of
	a row, the pixel buffer and, for a palettized image, its palette of 256
	entries.

	Evidence: the vtable at 0x898D44 holds NiObject's 12 slots and adds
	none (it is followed by its load record's vtable, 0x898D74); its
	CreateClone (0x7E0300) returns the image itself. The viewer strings
	(0x7E0590) name the members "m_eFormat" ("FORMAT_PALETTIZED",
	"FORMAT_RGB", "FORMAT_RGBA"), "m_uiWidth", "m_uiHeight",
	"m_uiPixelStride", "m_uiRowStride", "m_pPal" and "m_pData" at 0x08 to
	0x20. The constructors (0x7DE3E0, 0x7DE420) and the stream (0x7E0400)
	size the image through 0x7DE4A0, which allocates the palette (0x400
	bytes) and the pixels (through 0x8221F0, freed by 0x822210). NiImage
	reads the files of its images into one; the D3D renderer converts it
	to and from surface memory. The names of the members are the viewer
	strings' without the prefixes; the method names are invented.

===============================================================================
*/

#ifndef __NIRAWIMAGEDATA_H__
#define __NIRAWIMAGEDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

class NiPixelDesc;
class NiFile;

/*
================
NiPaletteEntry

A palette entry: four bytes, cleared on construction (the palette's
allocation clears each). The name is invented.
================
*/
struct NiPaletteEntry {
							NiPaletteEntry() { red = 0; green = 0; blue = 0; alpha = 0; }

	unsigned char			red;
	unsigned char			green;
	unsigned char			blue;
	unsigned char			alpha;
};

/*
================
NiRawImageDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898D74). The class name is invented.
================
*/
// FUNCTION: STBC 0x7E03D0 ??_GNiRawImageDataLoadRecord@@UAEPAXI@Z
class NiRawImageDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x7E0900 ??_GNiRawImageData@@UAEPAXI@Z
class NiRawImageData : public NiObject {
public:
	// FUNCTION: STBC 0x7E08F0 ?GetRTTI@NiRawImageData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiRawImageData();

	// the pixel formats ("m_eFormat")
	enum Format {
		FORMAT_PALETTIZED,		// one byte per pixel, indexing the palette
		FORMAT_RGB,				// three bytes per pixel
		FORMAT_RGBA				// four bytes per pixel
	};

	// an empty RGB image
							NiRawImageData();
							NiRawImageData( Format format_, unsigned int width_, unsigned int height_ );

	// reallocates the image for a format and size, keeping it when they do
	// not change; false when an allocation fails
	bool					Create( Format format_, unsigned int width_, unsigned int height_ );

	// a copy of another image's format, size, palette and pixels
	bool					Copy( NiRawImageData *source );

	// resizes the pixels to a size that fits the buffer; false when it
	// does not
	bool					Unknown7DE640( unsigned int width_, unsigned int height_ );

	// a copy of another image at a new size, each pixel the nearest to its
	// place in the source (name invented)
	bool					Unknown7DE760( NiRawImageData *source, unsigned int newWidth, unsigned int newHeight );

	// reduces the pixels of another image into these
	bool					Unknown7DE990( NiRawImageData *source );

	// a new RGBA image from two of one size: the color of the first and
	// the first channel of the second as alpha; NULL when they differ or
	// either is missing (name invented)
	static NiRawImageData *	Unknown7DECF0( NiRawImageData *color, NiRawImageData *alpha );

	// takes over another image's format, size and pixels; the other image
	// keeps no buffers
	void					Unknown7DEEA0( NiRawImageData *source );

	// copies the pixels, converted to a pixel format, into locked surface
	// memory of a pitch
	bool					Unknown7DEEE0( const NiPixelDesc &format, unsigned int pitch, unsigned char *bits );
	// takes the pixels of locked surface memory of a pixel format, pitch
	// and size (NiDDBufferImage::Unknown7C7160 reads a frame buffer with it)
	bool					Unknown7DF9A0( const NiPixelDesc &format, unsigned int pitch, unsigned int width, unsigned int height,
										   const unsigned char *bits );

	// an RGBA copy of another image, the alpha of each pixel keyAlpha when
	// its color lies within the ranges given, else alpha (name invented)
	bool					Unknown7DFE40( NiRawImageData *source, unsigned char redMin, unsigned char redMax,
										   unsigned char greenMin, unsigned char greenMax, unsigned char blueMin,
										   unsigned char blueMax, unsigned char keyAlpha, unsigned char alpha );

	// copies another image from a column and row on into this one's top
	// left corner, converting the format; alpha (0 to 1) fills an RGBA
	// image from an RGB or palettized one
	bool					Unknown7DFFF0( NiRawImageData *source, unsigned int x, unsigned int y, float alpha );

	unsigned char *			GetPixels() { return data; }
	// the pixel at a column and row; NiFrameRate's digit textures call the
	// out-of-line copy 0x7BC990 (DamageTool.exe's symbol: operator())
	// FUNCTION: STBC 0x7BC990 ??RNiRawImageData@@QAEPAEII@Z @netimmerse/NiFrameRate
	// PARKED: no out-of-line copy until NiFrameRate::SetupProperties stops inlining it (inline budget wall there)
	unsigned char *			operator()( unsigned int x, unsigned int y ) { return data + x * pixelStride + y * rowStride; }
	Format					GetFormat() const { return format; }
	unsigned int			GetWidth() const { return width; }
	unsigned int			GetHeight() const { return height; }
	unsigned int			GetPixelStride() const { return pixelStride; }	// Gamebryo lineage name
	unsigned int			GetRowStride() const { return rowStride; }	// Gamebryo lineage name
	NiPaletteEntry *		GetPalette() const { return palette; }

	// the "%s = FORMAT_..." viewer string of a format
	static char *			GetViewerString( const char *prefix, Format format_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the formats of the raw formats (DamageTool.exe's names)
	static const NiPixelDesc	PALETTIZED_DESC;
	static const NiPixelDesc	RGB_DESC;
	static const NiPixelDesc	RGBA_DESC;

protected:
	// the image file readers fill the image in place
	friend bool				NiReadBMPFile( NiFile &file, NiRawImageData *image );
	friend bool				NiReadSGIFile( NiFile &file, NiRawImageData *image );
	friend bool				NiReadTGA( NiFile &file, NiRawImageData *image );

	// the pixel buffer of a size in bytes, and its release (defined in
	// another unit, 0x8221F0 and 0x822210)
	void					AllocateData( unsigned int size );
	void					FreeData();

	Format					format;
	unsigned int			width;
	unsigned int			height;
	unsigned int			pixelStride;	// bytes per pixel
	unsigned int			rowStride;		// bytes per row
	NiPaletteEntry *		palette;		// 256 entries for a palettized image, else NULL
	unsigned char *			data;
};

NiRegisterStream( NiRawImageData );

#endif /* !__NIRAWIMAGEDATA_H__ */
