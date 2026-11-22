/*
===============================================================================

	NiDDImage.h

	The Direct3D renderer's image: an NiImage with the DirectDraw surfaces
	made from it, one per renderer that uses it. NiDDBufferImage is the image
	of a frame buffer's own front or back surface.

	Evidence: the static initializers at 0x7C4CC0 and 0x7C4D00 register
	"NiDDImage" (base NiImage) and "NiDDBufferImage" (base NiDDImage); the
	creation functions allocate 0x5C bytes (0x7C6490) and 0x60 bytes
	(0x7C6C40) and store the vtables at 0x898A7C and 0x898AB4 (14 slots each:
	NiImage's 13 and a new slot 13); the destructor (0x7C4F50) empties the
	surface list at 0x30, releasing each surface, and removes the image from
	the static list of images (0x9A11E8) that the constructor adds it to.

===============================================================================
*/

#ifndef __NIDDIMAGE_H__
#define __NIDDIMAGE_H__

#include "NiImage.h"
#include "NiTList.h"
#include "NiD3D7.h"
#include "NiTextureManager.h"

class NiPixelDesc;
class NiDDPixelDesc;
class NiRawImageData;

// a surface made from the image (name invented)
class NiDDImageSurface {
public:
	IDirectDrawSurface7 *	surface;
	unsigned int			format;			// the surface's NiDDPixelDesc index
	IDirectDraw7 *			directDraw;		// the DirectDraw object of the surface
	bool					unknown0C;		// set for every surface by NiDDImage's slot 12
};

// the texture entry of the image in one texture manager table. NiDDImage
// embeds the first one, which also heads the chain of the others (name
// invented)
class NiDDImageBlock {
public:
	NiTextureManager::_DPSetupTexture *		entry;
	unsigned int			entryId;		// the id of the manager table that holds entry
	NiDDImageBlock *		next;
};

// the record at NiDDImage's 0x58: the destructor frees its buffer at 0x04,
// then the record (layout from the destructor; name invented)
class NiDDImageBuffer {
public:
							~NiDDImageBuffer() { delete [] buffer; }

	unsigned int			format;			// the NiDDPixelDesc index of the pixels
	unsigned char *			buffer;
};

class NiDDImage : public NiImage {
public:
	// FUNCTION: STBC 0x7C75C0 ?GetRTTI@NiDDImage@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~NiDDImage();

	virtual bool			VirtualSlot12();
	virtual bool			VirtualSlot13( void *unknown );	// releases the surfaces

							NiDDImage();

	// the image's texture entry in the manager table of an id; NULL when
	// it has none (name invented)
	NiTextureManager::_DPSetupTexture *		GetEntry( unsigned int id ) const;

	// sets the image's texture entry in the manager table of an id (name
	// invented)
	void					SetEntry( unsigned int id, NiTextureManager::_DPSetupTexture *entry_ );

	// drops the texture entries of a manager table from every image (name
	// invented)
	static void				RemoveAllEntries( unsigned int id );

	// a system-memory texture surface of the image's size in a format,
	// with the image's mipmaps when it wants them (name invented)
	bool					CreateSurface( IDirectDraw7 *directDraw, const NiDDPixelDesc &format,
										   IDirectDrawSurface7 *&surface, bool unused );

	// iteration over the surfaces, through the image's own position
	IDirectDrawSurface7 *	Unknown7C6610();
	IDirectDrawSurface7 *	Unknown7C6630();
	// writes the first surface and its mipmaps to a file (name invented)
	bool					Save( const char *name );

	// the surface of the image for a DirectDraw object in one of the
	// manager's pixel formats, made when there is none, and its format;
	// false when there is none (name invented)
	bool					Unknown7C6370( IDirectDraw7 *directDraw, const NiDDPixelDesc **formats, bool unknown3, bool unknown4,
										   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *&format );

	// the surface of the image for a DirectDraw object in a pixel format,
	// made when there is none (name invented)
	bool					Unknown7C6270( IDirectDraw7 *directDraw, const NiDDPixelDesc *format, bool unknown3,
										   IDirectDrawSurface7 *&surface );

	// makes the image's surface from its buffer (unknown58) or from its
	// pixels; with no single format, in the first of the formats that works
	bool					Unknown7C59E0( IDirectDraw7 *directDraw, const NiDDPixelDesc *&format, bool unknown3,
										   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *single,
										   const NiDDPixelDesc **formats );
	bool					Unknown7C5C60( IDirectDraw7 *directDraw, const NiDDPixelDesc *&format, bool unknown3,
										   IDirectDrawSurface7 *&surface, const NiDDPixelDesc *single,
										   const NiDDPixelDesc **formats, bool unknown7 );

	// copies raw pixels into a surface and its mipmap levels
	bool					Unknown7C5330( NiRawImageData *raw, const NiDDPixelDesc &format, IDirectDrawSurface7 *surface );

	// fills a marked surface again from the image's pixels
	bool					Unknown7C6080( const NiDDPixelDesc *format, IDirectDrawSurface7 *surface, bool unknown );

	// NiImage's creation function (0x7C6490)
	static NiImage *		Create();
	// an image read from a file of raw pixels (name invented)
	static NiDDImage *		CreateFromRawFile( const char *filename );

	// runs slot 13 on every image, when the DirectDraw object goes (name
	// invented)
	static bool				ReleaseAllSurfaces( IDirectDraw7 *directDraw );

	static const NiRTTI		rtti;

protected:
	NiTList<NiDDImageSurface *>	surfaces;
	NiTListIterator			surfacePos;	// the iteration's position
	NiDDImageBlock			entries;		// the entry in the first table to ask (entryId 0 for none), heading the
											// chain of the entries in other tables (name invented)
	NiDDImageBuffer *		unknown58;		// freed by the destructor

	static NiTList<NiDDImage *>	images;	// every live NiDDImage
	static unsigned int		instances;	// live NiDDImages (name invented)
};

class NiDDBufferImage : public NiDDImage {
public:
	// FUNCTION: STBC 0x7C75D0 ?GetRTTI@NiDDBufferImage@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~NiDDBufferImage();

	virtual bool			VirtualSlot13( void *unknown );

							NiDDBufferImage() { unknown5C = NULL; }

	bool					Unknown7C70C0( IDirectDrawSurface7 **surface );

	// reads the frame buffer surface's pixels into raw pixels; false when
	// there is no surface or it cannot be read
	bool					Unknown7C7160( NiRawImageData *raw );

	// the image of a frame buffer's surface; NULL without a surface
	static NiDDBufferImage *	Create( IDirectDrawSurface7 *surface, IDirectDraw7 *directDraw );

	static const NiRTTI		rtti;

protected:
	NiDDImageSurface *		unknown5C;		// the frame buffer surface's record, also in the surface list
};

/*
================
NiDDImage::GetEntry

As the debug copy has it (AnimOnly.exe 0x531AF0).
================
*/
inline NiTextureManager::_DPSetupTexture *NiDDImage::GetEntry( unsigned int id ) const {
	if ( id == entries.entryId ) {
		return entries.entry;
	}
	if ( entries.next ) {
		NiDDImageBlock *block = entries.next;
		do {
			if ( id == block->entryId ) {
				return block->entry;
			}
			block = block->next;
		} while ( block );
	}
	return NULL;
}

/*
================
NiDDImage::SetEntry
================
*/
// FUNCTION: STBC 0x7D7B00
inline void NiDDImage::SetEntry( unsigned int id, NiTextureManager::_DPSetupTexture *entry_ ) {
	if ( id == entries.entryId ) {
		entries.entry = entry_;
		return;
	}
	if ( entries.entryId ) {
		NiDDImageBlock *block;
		for ( block = entries.next; block; block = block->next ) {
			if ( id == block->entryId ) {
				block->entry = entry_;
				return;
			}
		}
		block = new NiDDImageBlock;
		block->next = entries.next;
		block->entryId = id;
		block->entry = entry_;
		entries.next = block;
		return;
	}
	entries.entryId = id;
	entries.entry = entry_;
}

#endif /* !__NIDDIMAGE_H__ */
