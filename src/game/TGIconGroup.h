/*
===============================================================================

	TGIconGroup.h

	A named set of icons: where each icon lies in the group's textures.
	The icon manager holds the loaded groups; icons and paragraphs keep a
	reference to the group they draw from.

	0x34 bytes, with the vtable 0x897BD8 (5 slots).

	Evidence: the constructor (0x72BB20, with the name) builds a map at
	0x0C (37 buckets, vtable 0x897BF0) and an array at 0x1C (vtable
	0x897BEC) that it sizes to 15 elements growing by 10, clears 0x04 and
	0x08 and sets the name (SetName, 0x72BF70). The destructor (0x72BCE0)
	deletes the name and empties the group (Reset, 0x72BDE0). The SWIG
	wrappers TGIconGroup_GetName and GetCount read 0x04 and 0x10 (the map's
	count). TGParagraph::SetFontGroup counts the references at 0x08 up
	inline and down through 0x72BF60. The slots are the destructor,
	SetIconLocation (TGIconGroup_SetIconLocation, "Oilffff|ii"),
	GetPyWrapper (TGIconGroup_GetPyWrapper: PyEmbed's wrapper with
	"TGIconGroup") and the stream pair TGFontGroup's overrides extend
	(0x72CB70, 0x72CC50).

===============================================================================
*/

#ifndef __TGICONGROUP_H__
#define __TGICONGROUP_H__

#include <string.h>

#include "NiObject.h"
#include "NiTextureProperty.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiTMap.h"

struct _object;
class TGStream;

// Where one icon lies in a texture of its group (invented name). 0x30
// bytes with the vtable 0x897C0C; SetIconLocation (0x72C260) builds one
// from the position and size in pixels it is given: the corners as
// fractions of the texture size, and the size itself.
class TGIconLocation {
public:
					TGIconLocation( void );

	// The stream pair of the group's icons (invented names, as TGObject's).
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	// The copy TGIconGroup::Unknown72C990 gives each icon (invented: written
	// out because retail block-copies the corners, rep movsd of 8 dwords, as
	// the constructor clears them; the implicit copy would loop over them).
	TGIconLocation &	operator=( const TGIconLocation &location ) {
		texture = location.texture;
		memcpy( corners, location.corners, sizeof( corners ) );
		pixelWidth = location.pixelWidth;
		pixelHeight = location.pixelHeight;
		return *this;
	}

	// The group's texture the icon lies in: SpaceCamera::CreateSpaceDust
	// (0x42F380) attaches the one GetIconTexture returns to its particles.
	NiPointer<NiTextureProperty>	texture;	// 0x04 held: Reset's delete releases it (0x72BDE0)
	float			corners[4][2];			// 0x08 the texture coordinates (s, t) of the four corners
	float			pixelWidth;				// 0x28
	float			pixelHeight;			// 0x2C
};

// FUNCTION: STBC 0x0072acb0 ??_GTGIconGroup@@UAEPAXI@Z @game/TGIconManager
class TGIconGroup {
public:
					TGIconGroup( char *name );
					// For a group read from a stream (TGFontFamily::ReadFromStream).
					TGIconGroup( void ) {}
	virtual			~TGIconGroup( void );

	virtual void	SetIconLocation( int iconNum, long textureNum, float x, float y, float width, float height, int rotation = 0, int mirror = 0 );
	virtual _object *	GetPyWrapper( void );
	virtual bool	WriteToStream( TGStream *stream );		// invented name, as TGObject's
	virtual bool	ReadFromStream( TGStream *stream );		// invented name, as TGObject's

	void	Reset( void );
	void	SetName( char * );
	char *	GetName( void ) { return name; }
	int	GetCount( void ) { return icons.GetCount(); }
	NiTArray<NiPointer<NiTextureProperty> > *	GetTextures( void ) { return &unknown1C; }
	int	LoadIconTexture( char *, bool );
	// Turns the icon's corners by a quarter turn per step (1 to 3), swapping
	// its width and height for an odd number (0x72C530).
	void	Unknown72C530( int iconNum, int rotation );
	// Mirrors the icon's corners across the vertical (1) or the horizontal
	// (2) axis (0x72C690).
	void	Unknown72C690( int iconNum, int mirror );
	// Gives group copies of this group's name, icons and textures
	// (0x72C990; the font groups' scaled copy, 0x744430, starts with it).
	void	Unknown72C990( TGIconGroup *group );
	// The texture the icon lies in; NULL for an icon the group lacks (invented name).
	NiTextureProperty *	GetIconTexture( int iconNum );
	float	GetIconPixelWidth( int );
	float	GetIconPixelHeight( int );
	float	GetIconScreenWidth( int );
	float	GetIconScreenHeight( int );
	bool	IsNumValid( int );
	void	GoToFirstPos( void );
	int	GetNext( void );

	// The holders of the group: icons and paragraphs (NetImmerse's names).
	void	IncRefCount( void ) { refCount++; }
	int		GetRefCount( void ) { return refCount; }
	void	DecRefCount( void );

protected:
	// TGIconManager::CreateIconPoly looks the icon up in the icons directly.
	friend class TGIconManager;

	char *			name;					// 0x04 a heap copy, or NULL
	int				refCount;				// 0x08
	NiTMap<int, TGIconLocation *>	icons;	// 0x0C the icons by number (invented name)
	NiTArray<NiPointer<NiTextureProperty> >	unknown1C;	// 0x1C the textures by number: ViewScreenNode::SetStaticTextureIconGroup (0x676940) hands each one's image (0x18) to a flip controller (invented name)

	static NiTMapIterator	iconPos;		// where GoToFirstPos and GetNext are in the icons of the group in use (invented name)
};

/*
================
TGIconLocation::TGIconLocation

An icon with no texture and no extent.
================
*/
inline TGIconLocation::TGIconLocation( void ) : texture( NULL ) {
	memset( corners, 0, sizeof( corners ) );
	pixelWidth = 0.0f;
	pixelHeight = 0.0f;
}

#endif /* !__TGICONGROUP_H__ */
