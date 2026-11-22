/*
===============================================================================

	ImageUtils.h

	Game functions on raw images: the screen shot writers and the colour
	lookup tables the bridge's lighting fills (ImageUtils.cpp, 0x4588A0 to
	0x459390; the unit, the file and every name in it are invented).

	Evidence: TGSystemWrapperClass::TakeScreenshot (0x71B150) hands the
	grabbed back buffer and the file name to 0x4588A0 for "%s.tga" and to
	0x458E40 for "%s.bmp"; BridgeObjectClass::ApplyLightLevel and Update
	fill their tables with 0x459130 and 0x459160.

===============================================================================
*/

#ifndef __IMAGEUTILS_H__
#define __IMAGEUTILS_H__

class NiImage;
class NiRawImageData;

// The header of a TGA file, field by field as the writer puts it out
// (naturally aligned: the writer never writes it whole).
struct TGAFileHeader {
	unsigned char	idLength;
	unsigned char	colorMapType;
	unsigned char	imageType;			// 2: uncompressed true colour
	unsigned short	colorMapStart;
	unsigned short	colorMapLength;
	unsigned char	colorMapDepth;
	unsigned short	xOrigin;
	unsigned short	yOrigin;
	unsigned short	width;
	unsigned short	height;
	unsigned char	bitsPerPixel;		// 24 or 32
	unsigned char	descriptor;
};

// Write the image to the file as an uncompressed TGA (24 bits, or 32 with
// alpha) or as a 24-bit BMP; false when the file can't be written.
bool	SaveTGAFile( const char *fileName, NiRawImageData *image );
bool	SaveBMPFile( const char *fileName, NiRawImageData *image );

// Fills a 256-entry table with each index times the scale.
void	BuildScaleTable( unsigned char *table, float scale );
// Fills a 256-entry table with a gamma curve of the exponent's inverse,
// scaled to reach 255.
void	BuildGammaTable( unsigned char *table, float gamma );
// Fills a 256-entry table with the gamma curve between the input levels,
// mapped onto the output levels. Nothing in the game calls it.
void	BuildGammaRangeTable( unsigned char *table, float inLow, float gamma, float inHigh, float outLow,
							  float outHigh );
// Fills a 256-entry table with each index times the contrast plus the
// brightness, clamped to 0 to 255. Nothing in the game calls it.
void	BuildLevelsTable( unsigned char *table, float brightness, float contrast );

// Passes the first three bytes of every pixel through the table.
void	ApplyColorTable( NiRawImageData *image, const unsigned char *table );
// The same on an image's pixels, which it then marks changed.
void	ApplyColorTable( NiImage *image, const unsigned char *table );

#endif /* !__IMAGEUTILS_H__ */
