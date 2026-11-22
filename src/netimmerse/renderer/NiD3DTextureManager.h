/*
===============================================================================

	NiD3DTextureManager.h

	The Direct3D texture manager: NiTextureManager on a DirectDraw object
	and a device.

	Evidence: NiD3DRender's creation (0x7C3480) gets it from 0x7D4950,
	which reuses a manager of the same DirectDraw object found in a static
	map (0x9A1408) or creates one (0x7D5080); the manager is 0xF8 bytes with
	the vtable 0x898B58. The class name is invented, after the renderer's
	other helpers.

===============================================================================
*/

#ifndef __NID3DTEXTUREMANAGER_H__
#define __NID3DTEXTUREMANAGER_H__

#include "NiTextureManager.h"
#include "NiD3D7.h"
#include "NiTMap.h"
#include "NiTList.h"
#include "NiDDImage.h"

// what a texture format callback gives back: whether the device has the
// format, and a copy of it (names invented)
struct NiD3DTextureFormatSearch {
	bool					found;
	NiDDPixelDesc *	format;			// allocated; the manager keeps it
};

class NiD3DTextureManager : public NiTextureManager {
public:
	virtual unsigned int	VirtualSlot01( NiImage *image );
	virtual bool			VirtualSlot02( NiDDImage *image );
	virtual bool			VirtualSlot03( NiDDImage *image, bool unknown1, bool keep, int table );
	virtual IDirectDrawSurface7 *	VirtualSlot04( unsigned int format, int stage );
	virtual bool			VirtualSlot05( NiImage *image, bool kept, int table );
	virtual bool			VirtualSlot06( int table, NiImage *image, NiPointer<NiImage> &current, IDirectDrawSurface7 *&surface,
										  bool &changed, bool &mipmapped );
	virtual void			VirtualSlot07();
	virtual					~NiD3DTextureManager();

	// the entry of a key in the current table; whether it is older than
	// the current frame (name invented)
	bool					Unknown7D5640( unsigned int key, _DPSetupTexture *&entry );
	// the hash bucket of a key (name invented; AnimOnly.exe 0x5319C0)
	unsigned int			GetBucket( unsigned int key ) { return ( key >> 11 ) & 0xFF; }

	// the surface of an image in one of the manager's formats, the
	// description of a texture for it, less the mipmap levels the size
	// limits skip, and the key of the texture: its format, size, aspect
	// and mipmapping (name invented)
	bool					Unknown7D6380( NiDDImage *image, IDirectDrawSurface7 *&surface, DDSURFACEDESC2 &desc, unsigned int &key );
	// that key from the size's logarithms, the format and mipmapping (name
	// invented; AnimOnly.exe 0x531BC0, D3DTextureManager.cpp, whose only
	// caller is Unknown7D6380)
	unsigned int			MakeKey( unsigned int widthLog, unsigned int heightLog, unsigned int formatIndex, bool mipmapped );
	// the key of a texture of the description, sized by its bytes per
	// pixel where Unknown7D6380 uses the format's bias; nothing in retail
	// calls it
	unsigned int			Unknown7D74E0( const DDSURFACEDESC2 &desc );

	// makes or refills the texture of an image from its surface (name
	// invented)
	bool					Unknown7D5B30( NiDDImage *image, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc, unsigned int key,
										   _DPSetupTexture *&entry );

	// whether an image can be a texture: its surface is made when its
	// size is not known yet, and both sides are powers of two (name
	// invented)
	bool					Unknown7D62A0( NiDDImage *image );
	// whether a side is a power of two: a member, as AnimOnly.exe 0x531B70
	// (D3DTextureManager.cpp) calls it (name invented)
	bool					IsPowerOfTwo( unsigned int value );

	// copies a surface into a texture, level by level; cdecl, the device
	// is not used (name invented)
	static void				Unknown7D5310( IDirect3DDevice7 *device, IDirectDrawSurface7 *source, IDirectDrawSurface7 *texture );

	// makes the texture of an image and its entry in the current table
	// (name invented)
	bool					Unknown7D5440( NiDDImage *image, unsigned int key, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc,
										   _DPSetupTexture *&entry );

	// gives an image an entry and copies the image into the entry's
	// texture (name invented)
	bool					Unknown7D56A0( NiDDImage *image, IDirectDrawSurface7 *surface, _DPSetupTexture *entry );

	// the least recently used entry of the current table in a range of
	// bucket groups (name invented)
	_DPSetupTexture *		Unknown7D57F0( int &low, int &high, bool any );

	// frees the least recently used textures until the texture of an
	// image can be made (name invented)
	bool					Unknown7D5910( NiDDImage *image, unsigned int key, IDirectDrawSurface7 *surface, DDSURFACEDESC2 &desc,
										   _DPSetupTexture *&entry, bool any );

	// fills an entry's texture again from its image (name invented)
	bool					Unknown7D6800( _DPSetupTexture *entry, NiDDImage *image );

	// makes the texture of an image (name invented)
	bool					Unknown7D6550( NiDDImage *image );

	// returns an entry to the free list (name invented)
	void					Unknown7D73A0( _DPSetupTexture *entry );

	// takes an entry from the free list, allocating a block when it is
	// empty, for an image in a table (name invented)
	_DPSetupTexture *		Unknown7D7240( NiDDImage *image, unsigned int key, unsigned int table );

	// links an entry at the end of its bucket (name invented)
	void					LinkEntry( _DPSetupTexture *entry );

	// moves a linked entry to the end of its bucket, the most recently
	// used place (name invented)
	void					MoveEntryToEnd( _DPSetupTexture *entry );

	// unlinks an entry from its table (name invented)
	void					Unknown7D73D0( _DPSetupTexture *entry );

	// releases an entry's texture and returns the entry to the free list
	// (name invented)
	void					Unknown7D6650( _DPSetupTexture *entry );

							NiD3DTextureManager();

	// the manager of a DirectDraw object, shared when the renderer asks for
	// multitexturing (name invented)
	static NiD3DTextureManager *	Create( bool multitexture, bool trueColor, IDirectDraw7 *directDraw,
											IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc, bool hardware );

	// finds the device's format of each kind, keeping and registering it;
	// false when the device has none or a query fails (name invented, after
	// its error message)
	bool					QueryTextureFormats( IDirect3DDevice7 *device );


	// the EnumTextureFormats callbacks for each kind of format: each stops
	// at the first format of its kind, giving a copy of it to the
	// NiD3DTextureFormatSearch (names invented)
	static HRESULT CALLBACK	EnumTextureFormat555( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormat565( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormat888( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormat1555( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormat4444( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormat8888( LPDDPIXELFORMAT pixelFormat, LPVOID context );
	static HRESULT CALLBACK	EnumTextureFormatPalette8( LPDDPIXELFORMAT pixelFormat, LPVOID context );

	// makes a manager; cdecl (name invented, after its "Attempting to create
	// texture manager" message)
	static NiD3DTextureManager *	CreateManager( bool multitexture, bool trueColor, IDirectDraw7 *directDraw,
												   IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc );

	// the managers of each DirectDraw object, by Create's last argument;
	// NiTextureManager's destructor (0x7D46A0) removes a manager from both
	// (names invented)
	static NiTMap<IDirectDraw7 *, NiD3DTextureManager *>	hardwareManagers;
	static NiTMap<IDirectDraw7 *, NiD3DTextureManager *>	softwareManagers;

	_DPSetupTexture **	tables[2];		// 256 buckets each, by bits 11-18 of the key: the first entry of each
	_DPSetupTexture *		unknownC0[2];	// the entries flagged 2 (unknown08), outside the buckets
	_DPSetupTexture **	unknownC8[2];	// the last entry of each bucket of tables
	unsigned int *			unknownD0[2];	// 16 counters each: the entries of 16 buckets
	int						unknownD8[2];	// texture bytes of each table (unknown20 of its entries)
	bool					unknownE0;
	bool					unknownE1;		// the second table is in use
	unsigned int			currentTable;	// index of tables, unknownC0-unknownD8 and ids
	_DPSetupTexture *		freeEntries;
	NiTList<_DPSetupTexture *> *	entryBlocks;	// the blocks of 0x20 entries
	unsigned int			ids[2];			// from nextId; an image keeps the id of its entry's table

	static unsigned int		nextId;			// the next table id (name invented)
};

/*
================
NiD3DTextureManager::Unknown7D73A0
================
*/
// FUNCTION: STBC 0x7D73A0
inline void NiD3DTextureManager::Unknown7D73A0( _DPSetupTexture *entry ) {
	if ( entry ) {
		entry->unknown14 = 0;
		entry->next = freeEntries;
		entry->prev = NULL;
		freeEntries = entry;
		entry->image = NULL;
	}
}

/*
================
NiD3DTextureManager::Unknown7D73D0

Unlinks an entry from its bucket, or from the list of flagged entries.
================
*/
// FUNCTION: STBC 0x7D73D0
inline void NiD3DTextureManager::Unknown7D73D0( _DPSetupTexture *entry ) {
	if ( entry ) {
		unsigned int table = entry->table;
		if ( entry->unknown08 & 2 ) {
			if ( entry == unknownC0[table] ) {
				unknownC0[table] = entry->next;
				if ( entry->next ) {
					entry->next->prev = NULL;
				}
			} else {
				entry->prev->next = entry->next;
				if ( entry->next ) {
					entry->next->prev = entry->prev;
				}
			}
		} else {
			unknown8C--;
			unsigned int bucket = ( entry->key >> 11 ) & 0xFF;
			if ( entry == unknownC8[table][bucket] ) {
				if ( entry == tables[table][bucket] ) {
					unknownC8[table][bucket] = NULL;
					tables[table][bucket] = NULL;
				} else {
					unknownC8[table][bucket] = entry->prev;
					unknownC8[table][bucket]->next = NULL;
				}
			} else if ( entry == tables[table][bucket] ) {
				tables[table][bucket] = entry->next;
				if ( entry->next ) {
					entry->next->prev = NULL;
				}
			} else {
				entry->next->prev = entry->prev;
				entry->prev->next = entry->next;
			}
			unknownD0[table][bucket >> 4]--;
		}
	}
}

/*
================
NiD3DTextureManager::Unknown7D6650

Retail keeps an out-of-line copy at 0x7D6650 for the callers that do
not inline it.
================
*/
// FUNCTION: STBC 0x7D6650
inline void NiD3DTextureManager::Unknown7D6650( _DPSetupTexture *entry ) {
	if ( entry ) {
		currentTable = entry->table;
		entry->image->SetEntry( ids[entry->table], NULL );
		unknownD8[currentTable] -= entry->unknown20;
		entry->surface->Release();
		entry->surface = NULL;
		Unknown7D73D0( entry );
		Unknown7D73A0( entry );
		unknown9C++;
	}
}

/*
================
NiD3DTextureManager::LinkEntry

The counterpart of Unknown7D73D0 for an entry outside the flagged list.
================
*/
inline void NiD3DTextureManager::LinkEntry( _DPSetupTexture *entry ) {
	if ( entry ) {
		unsigned int table = entry->table;
		unknown8C++;
		unsigned int bucket = ( entry->key >> 11 ) & 0xFF;
		if ( tables[table][bucket] ) {
			entry->next = NULL;
			entry->prev = unknownC8[table][bucket];
			unknownC8[table][bucket]->next = entry;
			unknownC8[table][bucket] = entry;
		} else {
			unknownC8[table][bucket] = entry;
			tables[table][bucket] = entry;
			entry->prev = NULL;
			entry->next = NULL;
		}
		unknownD0[table][bucket >> 4]++;
	}
}

/*
================
NiD3DTextureManager::MoveEntryToEnd
================
*/
inline void NiD3DTextureManager::MoveEntryToEnd( _DPSetupTexture *entry ) {
	if ( entry && !( entry->unknown08 & 2 ) && entry->next ) {
		unsigned int table = entry->table;
		unsigned int bucket = ( entry->key >> 11 ) & 0xFF;
		if ( entry == tables[table][bucket] ) {
			tables[table][bucket] = entry->next;
			if ( entry->next ) {
				entry->next->prev = NULL;
			}
		} else {
			entry->next->prev = entry->prev;
			entry->prev->next = entry->next;
		}
		entry->next = NULL;
		entry->prev = unknownC8[table][bucket];
		unknownC8[table][bucket]->next = entry;
		unknownC8[table][bucket] = entry;
	}
}

#endif /* !__NID3DTEXTUREMANAGER_H__ */
