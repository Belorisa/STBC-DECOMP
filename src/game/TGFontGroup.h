/*
===============================================================================

	TGFontGroup.h

	An icon group whose icons are the characters of a font in one size,
	with the font's spacing.

	0x50 bytes (TGFontManager::CreateFontGroup allocates 0x50), with the
	vtable 0x897E6C, which overrides the five slots of TGIconGroup.

	Evidence: the constructor (0x744270, with the font name and size)
	builds the icon group without a name, copies the font name (0x34),
	stores the size (0x38), names the group "<name><size>" ("%s%.2f") and
	clears 0x3C to 0x4C. Its SetIconLocation (0x7443D0) keeps the tallest
	icon's height in 0x4C. The destructor (0x744360) removes the group
	from g_kFontManager (RemoveGroup) and deletes the name. The SWIG
	wrappers TGFontGroup_GetFontName, GetFontSize, GetPixelTabWidth,
	GetPixelSpaceWidth, GetPixelHorizontalSpacing,
	GetPixelVerticalSpacing and GetPixelMaxHeight read 0x34 to 0x4C in
	that order; their GetScreen* twins scale by g_kIconManager's scale and
	divide by its screen width, or by its screen height for the maximum
	height.

===============================================================================
*/

#ifndef __TGFONTGROUP_H__
#define __TGFONTGROUP_H__

#include "TGIconGroup.h"
#include "TGIconManager.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x007358c0 ??_GTGFontGroup@@UAEPAXI@Z
class TGFontGroup : public TGIconGroup {
public:
					TGFontGroup( char *fontName, float fontSize );
					// For a group read from a stream (TGFontFamily::ReadFromStream).
					TGFontGroup( void ) {}
	virtual			~TGFontGroup( void );

	// TGIconGroup
	virtual void	SetIconLocation( int iconNum, long textureNum, float x, float y, float width, float height, int rotation = 0, int mirror = 0 );
	virtual _object *	GetPyWrapper( void );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	int		MapChar( int );
	// Makes group a copy of this font at another size: the icons, textures
	// and metrics, scaled by size / fontSize (TGFontFamily::GetFontGroup).
	void	Unknown744430( TGFontGroup *group, float size );

	char *	GetFontName( void ) { return fontName; }
	float	GetFontSize( void ) { return fontSize; }
	float	GetPixelMaxHeight( void ) { return maxHeight; }
	float	GetScreenMaxHeight( void ) { return g_kIconManager.GetScale() * maxHeight / g_kIconManager.screenHeight; }
	float	GetScreenHorizontalSpacing( void ) { return g_kIconManager.GetScale() * horizontalSpacing / g_kIconManager.screenWidth; }
	float	GetScreenVerticalSpacing( void ) { return g_kIconManager.GetScale() * verticalSpacing / g_kIconManager.screenWidth; }
	float	GetScreenTabWidth( void ) { return g_kIconManager.GetScale() * tabWidth / g_kIconManager.screenWidth; }
	float	GetScreenSpaceWidth( void ) { return g_kIconManager.GetScale() * spaceWidth / g_kIconManager.screenWidth; }
	float	GetPixelVerticalSpacing( void ) { return verticalSpacing; }
	float	GetPixelHorizontalSpacing( void ) { return horizontalSpacing; }
	float	GetPixelTabWidth( void ) { return tabWidth; }
	float	GetPixelSpaceWidth( void ) { return spaceWidth; }
	void	SetVerticalSpacing( float verticalSpacing_ ) { verticalSpacing = verticalSpacing_; }
	void	SetHorizontalSpacing( float horizontalSpacing_ ) { horizontalSpacing = horizontalSpacing_; }
	void	SetTabWidth( float tabWidth_ ) { tabWidth = tabWidth_; }
	void	SetSpaceWidth( float spaceWidth_ ) { spaceWidth = spaceWidth_; }

protected:
	char *			fontName;				// 0x34 a heap copy, or NULL
	float			fontSize;				// 0x38
	float			tabWidth;				// 0x3C in pixels
	float			spaceWidth;				// 0x40 in pixels
	float			horizontalSpacing;		// 0x44 in pixels
	float			verticalSpacing;		// 0x48 in pixels
	float			maxHeight;				// 0x4C in pixels: the tallest character
};

#endif /* !__TGFONTGROUP_H__ */
