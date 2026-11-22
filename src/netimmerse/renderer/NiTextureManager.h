/*
===============================================================================

	NiTextureManager.h

	The abstract texture manager: the texture memory budget, the texture
	format preferences and the texture stage bindings. The renderer's
	Direct3D texture manager derives from it.

	Evidence: the constructor (0x7D45E0) stores the vtable at 0x898B34 (9
	slots: slot 0 at 0x7D7B70, slots 1-7 _purecall, slot 8 the scalar
	deleting destructor 0x7D4680), sets the four 0x4000 limits at 0x78-0x84
	and the 1 at 0xA0, and adds the manager to a static list (0x9A1400).
	NiD3DRender's destructor ("(Shutdown): Destroying texture manager")
	releases it through the reference count at 0xB4. The class name is
	invented from the log.

===============================================================================
*/

#ifndef __NITEXTUREMANAGER_H__
#define __NITEXTUREMANAGER_H__

#include <stddef.h>

#include "NiD3D7.h"
#include "NiPointer.h"
#include "NiTSimpleList.h"

class NiDDImage;
class NiDDPixelDesc;
class NiImage;
class NiObject;

class NiTextureManager;

// the static list of every texture manager (DamageTool.exe:
// NiTSimpleList<D3DTextureManager *>::AddHead, 0x4AE200)
typedef NiTSimpleList<NiTextureManager *>	NiTextureManagerList;

// the entries a manager allocates at once
#define ENTRY_BLOCK_SIZE		0x20

// the kinds of texture format a manager looks for in its device, by their
// bits per channel (names invented)
enum textureFormat_t {
	TEXTURE_FORMAT_555,
	TEXTURE_FORMAT_565,
	TEXTURE_FORMAT_888,
	TEXTURE_FORMAT_1555,
	TEXTURE_FORMAT_4444,
	TEXTURE_FORMAT_8888,
	TEXTURE_FORMAT_PALETTE8,
	NUM_TEXTURE_FORMATS
};

// the flags at 0xA8 (names invented)
enum textureManagerFlags_t {
	TEXTURE_MANAGER_VIDEO_MEMORY	= 0x01,		// D3DDEVCAPS_TEXTUREVIDEOMEMORY: textures in video memory
	TEXTURE_MANAGER_SQUARE			= 0x02,		// D3DPTEXTURECAPS_SQUAREONLY
	TEXTURE_MANAGER_TRUE_COLOR		= 0x04		// the renderer's true colour choice, passed on to NiDDImage::Unknown7C6370
};

class NiTextureManager {
public:
	// A texture the manager made for an image: a node of one of the
	// manager's hash tables (by the key at 0x0C), or of its free list, both
	// chained through next. The constructor allocates them 0x20 at a time.
	// The name is DamageTool.exe's (D3DTextureManager::_DPSetupTexture, which
	// FindTextureToEvict returns and GetDestinationTextureSurface fills);
	// the member names are invented.
	struct _DPSetupTexture {
		IDirectDrawSurface7 *	surface;		// released when the entry is freed
		unsigned int		table;			// which of the manager's tables holds it
		unsigned int		unknown08;		// flags: 2 keeps the entry out of the buckets, in the manager's unknownC0 list
		unsigned int		key;
		NiDDImage *			image;
		unsigned int		unknown14;
		int					unknown18;		// -1 after the manager's slot 7
		unsigned int		unknown1C;		// compared with the manager's frame count
		unsigned int		unknown20;
		_DPSetupTexture *	next;
		_DPSetupTexture *	prev;
	};

	virtual void			VirtualSlot00();
	// the texture of an image, made on first use (0x7D52B0 in the Direct3D
	// manager)
	virtual unsigned int	VirtualSlot01( NiImage *image ) = 0;
	virtual bool			VirtualSlot02( NiDDImage *image ) = 0;
	virtual bool			VirtualSlot03( NiDDImage *image, bool unknown1, bool keep, int table ) = 0;
	// a small texture of one of the pixel formats (unknown08), NULL when
	// there is none; the pipeline tests multitexturing with them
	virtual IDirectDrawSurface7 *	VirtualSlot04( unsigned int format, int unknown ) = 0;
	// purges what the manager holds for an image, or for every image when
	// it is NULL (see Release, NiD3DRender::Unknown7C2EB0)
	virtual bool			VirtualSlot05( NiImage *image, bool kept, int table ) = 0;
	// the texture of an image for a device stage, given the stage's current
	// texture: whether the stage needs it set, and whether it has mipmaps;
	// false when there is none (see NiD3DTexturePipeline::SetStageTextures)
	virtual bool			VirtualSlot06( int table, NiImage *image, NiPointer<NiImage> &current, IDirectDrawSurface7 *&surface,
										  bool &changed, bool &mipmapped ) = 0;
	virtual void			VirtualSlot07() = 0;
	virtual					~NiTextureManager();

							NiTextureManager();

	// releases an owner's reference: the last one deletes the manager, any
	// other purges every texture the manager holds (name invented)
	void					Release();

	// takes an owner's reference and purges every texture the manager
	// holds, as Release does for any reference but the last (name invented)
	void					AddReference() { refCount++; VirtualSlot05( 0, true, -1 ); }

	// whether any of the flags at 0xA8 are set (name invented; AnimOnly.exe
	// 0x513F50, which D3DTextureManager.cpp and NiD3DRender::Initialize
	// call with one TEXTURE_MANAGER_ bit)
	bool					TestFlags( unsigned int mask ) const { return ( unknownA8 & mask ) != 0; }

	// the renderer's reads and writes of the manager (names invented;
	// AnimOnly.exe 0x513B80, 0x513DD0, 0x513D80): the device's pixel format
	// of each kind, unknown88, and unknown78 and unknown7C set together
	// where the value does not exceed their limits
	const NiDDPixelDesc **	GetUnknown08() { return unknown08; }
	unsigned int			GetUnknown88() const { return unknown88; }
	// (names invented; AnimOnly.exe 0x513D40, 0x513D60, 0x513CF0)
	unsigned int			GetUnknownA0() const { return unknownA0; }
	void					SetUnknown88( unsigned int value ) { unknown88 = value; }
	bool					GetUnknown8C( unsigned int *value8C, unsigned int *value90, unsigned int *value98, unsigned int *value9C ) const {
								*value8C = unknown8C;
								*value90 = unknown90;
								*value98 = unknown98;
								*value9C = unknown9C;
								return true;
							}
	void					SetUnknown78And7C( unsigned int value ) {
								if ( value <= unknown80 ) {
									unknown78 = value;
								}
								if ( value <= unknown84 ) {
									unknown7C = value;
								}
							}

	unsigned int			unknown04;
	const NiDDPixelDesc *	unknown08[NUM_TEXTURE_FORMATS];	// the device's format of each kind; NULL when it has none
	unsigned int			unknown24[NUM_TEXTURE_FORMATS];	// log2 of its bytes per pixel
	bool					unknown40[NUM_TEXTURE_FORMATS];	// the device has the format
	_DPSetupTexture		unknown48;		// zeroed: the entry of an image that cannot be a texture
	_DPSetupTexture *		unknown74;		// &unknown48 at construction
	unsigned int			unknown78;		// 0x4000 at construction
	unsigned int			unknown7C;		// 0x4000 at construction
	unsigned int			unknown80;		// 0x4000 at construction: the limit of unknown78
	unsigned int			unknown84;		// 0x4000 at construction: the limit of unknown7C
	unsigned int			unknown88;
	unsigned int			unknown8C;
	unsigned int			unknown90;
	unsigned int			unknown94;
	unsigned int			unknown98;
	unsigned int			unknown9C;
	unsigned int			unknownA0;		// 1 at construction
	unsigned int			unknownA4;
	unsigned int			unknownA8;
	IDirectDraw7 *			directDraw;		// the key of the manager maps; released by the destructor
	IDirect3DDevice7 *		device;			// released by the destructor
	int						refCount;

	static NiTextureManagerList	managers;	// every texture manager
};

/*
================
NiTextureManager::Release
================
*/
inline void NiTextureManager::Release() {
	if ( --refCount == 0 ) {
		delete this;
	} else {
		VirtualSlot05( 0, true, -1 );
	}
}

#endif /* !__NITEXTUREMANAGER_H__ */
