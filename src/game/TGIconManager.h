/*
===============================================================================

	TGIconManager.h

	The manager of the icon groups: the registered groups the scripts can
	create on demand, the loaded groups, and the camera and screen the icons
	are drawn for. There is one, the global g_kIconManager.

	0x7C bytes, not polymorphic.

	Evidence: the constructor (0x729B30) builds an NiTArray at 0x00 (its
	vtable 0x897BD4; one element, growing by one), two NiTLists at 0x18 and
	0x30 (two items per block), an NiPointer at 0x50 and an NiTStringMap at
	0x6C (37 buckets) of NiObject pointers, whose vtables (0x88B68C,
	0x88B6A0) it shares with another map of that type (0x44EFA0), then clears 0x48 to 0x68 and sets 0x60 to -0.5. The
	destructor (0x729A00) tears them down in reverse and stores no vtable of
	its own. RegisterIconGroup appends a 0x10-byte record (the name, a
	script module and a function, 0x747990) to the array; GetIconGroup
	searches the list at 0x18 by name and, failing that, calls the record's
	function (PyEmbed::CallFunction) and searches again; AddIconGroup appends
	to that list and Purge deletes its unused groups. SetDisplayInfo holds
	the camera (0x50), stores its second argument at 0x54 and the renderer's
	screen size as floats at 0x58 and 0x5C, and reapplies 0x60 through
	SetPixelCenterAdj. The SWIG wrappers TGIconManager_GetCamera/SetCamera,
	GetScale/SetScale, GetScreenWidth/Height and GetPixelCenterAdj name
	those members.

===============================================================================
*/

#ifndef __TGICONMANAGER_H__
#define __TGICONMANAGER_H__

#include "NiTArray.h"
#include "NiTList.h"
#include "NiTStringMap.h"
#include "NiPointer.h"
#include "NiObject.h"
#include "TGRect.h"
#include "NiCamera.h"

class NiImage;

class NiCamera;
class NiColorA;
class TGIconGroup;
class TGIconPoly;
class NiTextureProperty;
class TGUIBatch;

class TGStream;

// RegisterIconGroup's record: the name of a group and the script function
// that creates it (invented names). Its copies of the strings are freed by
// the destructor, which is not virtual; the vtable 0x897C04 holds the
// stream functions (0x747AA0 writes the three strings, 0x747AD0 reads
// them). The constructor is 0x747990, the destructor 0x747A60.
class TGIconGroupRegistration {
public:
					TGIconGroupRegistration( char *name, char *module, char *function );
					// For a registration read from a stream (TGFontFamily::ReadFromStream).
					TGIconGroupRegistration( void ) {}
					~TGIconGroupRegistration( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	char *			name;					// 0x04 the group's name
	char *			module;					// 0x08 the script module
	char *			function;				// 0x0C the function that creates the group
};

/*
===============================================================================

	TGFontRegistration

	A size the scripts can create a font group of: the icon group's
	registration and the size. 0x14 bytes, vtable 0x897E64 (its two stream
	functions). The name is invented.

===============================================================================
*/

class TGFontRegistration : public TGIconGroupRegistration {
public:
					TGFontRegistration( void ) { size = 0.0f; }
					TGFontRegistration( char *name, char *module, char *function, float size_ ) :
						TGIconGroupRegistration( name, module, function ) { size = size_; }

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	float			size;					// 0x10
};

// How many polys AllocIconPoly makes at a time.
#define ICONPOLY_BLOCK_SIZE		64

class TGIconManager {
public:
					TGIconManager( void );
					~TGIconManager( void );

	void	RegisterIconGroup( char *, char *, char * );
	TGIconGroup *	CreateIconGroup( char * );
	void	AddIconGroup( TGIconGroup * );
	bool	DeleteIconGroup( char * );
	TGIconGroup *	GetIconGroup( char * );
	void	Purge( void );
	// Drops the camera, the registrations and every group (0x729C40,
	// called by the UI module as the game ends; invented name).
	void	Shutdown( void );
	// Forgets the icon groups without deleting them, before a saved game is loaded
	// (invented name, after TGEventManager::ForgetAll).
	void	ForgetAll( void );
	void	SetPixelCenterAdj( float );
	void	SetDisplayInfo( NiCamera *, float );
	void	Draw( void );
	// The registrations, the loaded groups and the scale and screen size
	// (invented names, as TGObject's; 0x72A960 and 0x72A9F0).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );
	// Empties the registered groups and runs their script functions again
	// (invented name).
	void	ReloadIconGroups( void );

	// The named locals are (invented): retail's callers copy the screen size
	// through fld/fstp, which needs a second copy level here; an inner inline, a
	// named local or a value wrapper give the same code, so its spelling is unknown.
	float	GetScreenWidth( void ) { float width = screenWidth; return width; }
	float	GetScreenHeight( void ) { float height = screenHeight; return height; }
	void	SetScreenWidth( float width ) { screenWidth = width; }
	void	SetScreenHeight( float height ) { screenHeight = height; }
	// Both at once (invented name: no SWIG wrapper).
	void	SetScreenSize( float width, float height ) { screenWidth = width; screenHeight = height; }
	float	GetScale( void ) { return scale; }
	void	SetScale( float scale_ ) { scale = scale_; }
	float	GetPixelCenterAdj( void ) { return pixelCenterAdj; }
	// The images loaded for the groups' textures, by path (invented name).
	// FUNCTION: STBC 0x0072c250 ?GetImages@TGIconManager@@QAEPAV?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@XZ @game/TGIconGroup
	NiTStringMap<NiPointer<NiImage> > *	GetImages( void ) { return &unknown6C; }
	// FUNCTION: STBC 0x42E6C0 ?GetCamera@TGIconManager@@QAE?AV?$NiPointer@VNiCamera@@@@XZ
	NiPointer<NiCamera>	GetCamera( void ) { return camera; }
	void	SetCamera( NiCamera *camera_ ) { if ( camera != camera_ ) { camera = camera_; } }

	// A poly for an icon of a group, made in one of the batches, or NULL
	// when the group is missing (0x72A470, called by TGIcon's
	// Layout).
	// The loaded group of the name, or NULL (invented name).
	TGIconGroup *	FindIconGroup( char *name );

	TGIconPoly *	CreateIconPoly( TGUIBatch **batches, char *groupName, int iconNum, TGRect *rect, NiColorA *color, int clamp );	// invented name
	// Puts a poly taken out of its batch on the free list (0x72A850; invented
	// name): TGUIBatch::RemoveIconPoly passes it each poly it removes.
	void			FreeIconPoly( TGIconPoly *poly );
	// The batch of the list drawing from the texture's image, or NULL (0x72A940).
	TGUIBatch *		Unknown72A940( TGUIBatch *batch, NiTextureProperty *texture );
	// A new batch for the texture, appended to the list, or NULL without a
	// list (0x72A880).
	TGUIBatch *		Unknown72A880( TGUIBatch **batches, NiTextureProperty *texture, int clamp );
	// A poly from the free list, which grows by 64 when it is empty (0x72A6E0;
	// invented name).
	TGIconPoly *	AllocIconPoly( void );

protected:
	// TGIconPoly (0x740A30) reads the pixel centre adjustments directly
	friend class TGIconPoly;
	// TGFontGroup's GetScreen* inlines divide by the screen size straight
	// from memory at every inlined site (TGParagraph::Unknown732420,
	// Unknown732710), which GetScreenWidth/Height's copy level doesn't give
	friend class TGFontGroup;

	NiTArray<TGIconGroupRegistration *>	registrations;	// 0x00 invented name; the groups the scripts can create
	NiTList<TGIconGroup *>	iconGroups;		// 0x18 the loaded groups (invented name)
	NiTList<TGIconPoly *>	polyBlocks;		// 0x30 the arrays of 64 polys AllocIconPoly makes, deleted with delete [] (invented name)
	TGIconPoly *	freeHead;				// 0x48 the free polys, linked through nextFree (invented name)
	TGIconPoly *	freeTail;				// 0x4C the last free poly (invented name)
	NiPointer<NiCamera>	camera;				// 0x50
	float			scale;					// 0x54 SetDisplayInfo's second argument
	float			screenWidth;			// 0x58 in pixels, from the renderer
	float			screenHeight;			// 0x5C in pixels, from the renderer
	float			pixelCenterAdj;			// 0x60 -0.5 at first
	float			screenCenterAdjX;		// 0x64 pixelCenterAdj in screen widths (invented name); cleared by the constructor
	float			screenCenterAdjY;		// 0x68 pixelCenterAdj in screen heights (invented name); cleared by the constructor
	NiTStringMap<NiPointer<NiImage> >	unknown6C;	// 0x6C the icon images loaded, by path (TGIconGroup::LoadIconTexture)
};

// The icon manager (the SWIG variable g_kIconManager).
extern TGIconManager	g_kIconManager;

#endif /* !__TGICONMANAGER_H__ */
