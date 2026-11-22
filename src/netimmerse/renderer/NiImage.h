/*
===============================================================================

	NiImage.h

	A texture image: a file name or raw pixel data, with the mipmap and
	format preferences a renderer uses when it creates the texture. Each
	renderer derives its own image class (NiDDImage for Direct3D).

	Evidence: the static initializer at 0x7E0950 registers "NiImage" with
	NiObject's NiRTTI as its base; the constructor (0x7E12F0) stores the
	vtable at 0x898D78 (13 slots; slot 12, 0x7E1FE0, returns false) and gives
	each image a serial number from a static counter (0x9783D0); the viewer
	strings (0x7E1AD0) name the members ("m_pURL", "m_spRaw", "m_uiWidth",
	"m_uiHeight", "m_uiMipLevels", "m_uiFlags", "m_ePrefTexFormat",
	"m_fImageScale").

	The NiImage unit lives with the renderer, which uses it.

===============================================================================
*/

#ifndef __NIIMAGE_H__
#define __NIIMAGE_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiRawImageData.h"
#include "NiStream.h"
#include "NiTList.h"
#include "NiStreamMacros.h"

class NiRender;

class NiImage;

/*
================
NiImageLoadRecord

What LoadBinary (0x7E16B0) reads for CreateObject (0x7E1630) and
LinkObject (0x7E1790): whether the image names a file, the image already
loaded from that file in the same stream, and otherwise the link ID of
the raw pixels. CreateObject builds it inline (vtable 0x898DAC). The
class and member names are invented.
================
*/
// FUNCTION: STBC 0x007E1FA0 ??_GNiImageLoadRecord@@UAEPAXI@Z
class NiImageLoadRecord : public NiObjectLoadRecord {
public:
							NiImageLoadRecord() { rawLinkID = 0; external = true; sharedImage = 0; }

	bool					external;		// the image names a file rather than linking raw pixels
	NiImage *				sharedImage;	// the stream's image of the same file, NULL for the first
	unsigned int			rawLinkID;
};

class NiImage : public NiObject {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7E0990)
	static int				Register();

	// FUNCTION: STBC 0x7E1F70 ?GetRTTI@NiImage@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiImage();

	// FUNCTION: STBC 0x7E1FE0 ?VirtualSlot12@NiImage@@UAE_NXZ
	virtual bool			VirtualSlot12() { return false; }

	// the preferred texture formats; the viewer strings (0x7E1D10) give
	// the names by value (type name invented)
	enum texFormat_t {
		TEX_RGB555,
		TEX_RGB565,
		TEX_RGB888,
		TEX_RGBA5551,
		TEX_RGBA4444,
		TEX_RGBA8888,
		TEX_PAL8,
		TEX_DEFAULT
	};

							NiImage();

	// streaming: an image of a file already loaded by the stream is that
	// image
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// the file the image was loaded from, or NULL (Gamebryo lineage: m_pURL)
	char *					GetURL() const { return url; }
	// the pixels, when not loaded from a file (name invented)
	NiRawImageData *		GetRawImageData() const { return raw; }
	unsigned int			GetWidth() const { return width; }
	unsigned int			GetHeight() const { return height; }
	// the serial number (Gamebryo lineage name, as NiPixelData's;
	// AnimOnly.exe 0x5319E0, which D3DTextureManager.cpp calls)
	unsigned int			GetRevisionID() const { return unknown1C; }
	// whether any of the flag bits are set: bit 1 asks for mipmaps (name
	// invented; AnimOnly.exe 0x518490, which CreateSurface and
	// NiDDImage::Unknown7C5C60 call with 2)
	bool					TestFlags( unsigned int mask ) const { return ( flags & mask ) != 0; }
	// sets flag bits (name invented; AnimOnly.exe 0x410B10)
	void					AddFlags( unsigned int flags_ ) { flags |= flags_; }

	static const NiRTTI		rtti;

	// makes the renderer's kind of image; NiD3DRender::Create (0x7C09C0)
	// installs NiDDImage::Create, and two functions of the NiImage unit
	// (0x7E0C50, 0x7E1630) call it (name invented)
	static NiImage *		(*createFunction)();

	// makes an image of raw pixels through createFunction (0x7E0D60; name
	// and argument meanings invented)
	static NiImage *		Create( NiPointer<NiRawImageData> raw, int mipmap, int format, int unknown3 );
	// the same for an image file (0x7E0C50)
	static NiImage *		Create( const char *file, int mipmap, int format, int unknown3 );

	// what Create does for a mipmap (or unknown3) argument of 2; the
	// application sets them (names invented)
	static bool				defaultMipmap;
	static bool				defaultUnknown3;

	// why the last image or surface creation failed: the NiImage unit's
	// creation functions set 4 to 6, NiDDImage's surface creation 11 and
	// 12 (name invented)
	static unsigned int		error;
	static unsigned int		GetError() { return error; }	// (name invented; AnimOnly.exe 0x531B60)

	// the pixels of an image file, NULL when it cannot be read; the
	// object is fileImage, shared by every call (name invented)
	static NiRawImageData *	Unknown7E15A0( const char *file );
	// the pixels Unknown7E15A0 reads image files into, shared by every call
	// (name invented)
	static NiPointer<NiRawImageData>	fileImage;

	// the mipmap levels below the top one for a size: the smaller of the
	// two powers of two, 0 when a side is not a power of two
	// (Gamebryo 1.1.2: NiPixelData::ComputeMipmapLevels, which by then counts
	// the top level and takes the larger side)
	static unsigned int		ComputeMipmapLevels( unsigned int width, unsigned int height );

	// the renderers told of each new image; the application adds its
	// renderer (name invented)
	static NiTList<NiRender *>	renderers;
	// a position in renderers that RemoveRenderer moves past the renderer
	// it removes; nothing else in the binary reads it (name invented)
	static NiTListIterator	rendererPos;

	// the next image's serial number (unknown1C), from 1 (name invented)
	static unsigned int		nextSerial;

	// SaveBinary saves the file name of an image that has one rather than
	// its pixels; true unless the application clears it (name invented)
	static bool				saveFileNames;
	static void				AddRenderer( NiRender *renderer );
	// (Gamebryo lineage name; UtopiaModule's VirtualSlot06 calls it)
	static void				RemoveRenderer( NiRender *renderer );
	// TODO identity: drops every renderer (0x7E1260), and walks them through
	// rendererPos: from the first (0x7E12C0), the next or NULL (0x7E12D0)
	static void				Unknown7E1260();
	static void				Unknown7E12C0();
	static NiRender *		Unknown7E12D0();
	// the default creation function: a plain image (0x7E1500)
	static NiImage *		Unknown7E1500();

	// the image file, as a standardized copy (name invented after GetURL)
	void					SetURL( const char *file );
	// the pixels (name invented)
	void					SetRawImageData( NiRawImageData *raw_ );
	// a copy of the pixels, NULL when there are none or the copy fails (name
	// invented)
	NiRawImageData *		CopyRawImageData();

	// tells every renderer of the image (their slot 53); false as soon as
	// one refuses it (name invented)
	bool					Unknown7E0EF0();
	// turns the image's file into a NIF file of its pixels beside it and
	// takes that file instead; false when the file cannot be read (name
	// invented; 0x7E0F30)
	bool					Unknown7E0F30();

	// the viewer string of a texture format: the prefix and the format's name
	static char *			GetViewerString( const char *prefix, texFormat_t format );

protected:
	char *					url;			// the image file
	NiPointer<NiRawImageData>	raw;		// the pixels, when not loaded from a file
	unsigned int			width;
	unsigned int			height;
	unsigned int			mipLevels;

public:
	unsigned int			unknown1C;		// a serial number, unique per image
	unsigned int			unknown20;		// counts the renderer's presentations of a frame buffer image

protected:
	unsigned int			flags;
	texFormat_t				prefTexFormat;	// TEX_DEFAULT, TEX_PAL8, TEX_RGBA8888, ...
	float					imageScale;		// 128.5 at construction
};

NiRegisterStream( NiImage );

#endif /* !__NIIMAGE_H__ */
