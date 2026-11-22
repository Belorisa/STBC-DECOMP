/*
===============================================================================

	ImageManagerClass.h

	The members of ImageManagerClass that the SWIG wrappers ImageManagerClass_* call. Of the data
	members only the image detail at 4 is declared: GetImageDetail's wrapper
	(0x5FA110) reads it, and SetImageDetail (0x44E470) stores its argument
	there. SetDefaultTextureFilterQuality (0x44E990) keeps its argument at
	0x10 and the texture mode filter it stands for at 0x14, which the
	particle controllers give their texture mode properties. The variable
	setter of g_kImageManager (0x65C245) copies at least 0x24 bytes; the
	members past 0x14 are not declared yet.

===============================================================================
*/

#ifndef __IMAGEMANAGERCLASS_H__
#define __IMAGEMANAGERCLASS_H__

#include "NiTextureModeProperty.h"
#include "NiTStringMap.h"
#include "NiPointer.h"
#include "NiObject.h"

class NiImage;
class NiRender;

// The destructor is the compiler's: it only destroys the image map.
// FUNCTION: STBC 0x0044e310 ??1ImageManagerClass@@QAE@XZ @game/ImageManagerClass
class ImageManagerClass {
public:
	// The image details (SetImageDetail's switch takes 0 to 2; the names
	// are invented).
	enum {
		LOW_DETAIL,
		MEDIUM_DETAIL,
		HIGH_DETAIL
	};

	ImageManagerClass( void );

	int		GetImageDetail( void ) { return imageDetail; }
	void	SetImageDetail( int );
	// Limits the images' size and tells the renderer (invented name).
	void	SetMaxImageSize( int size );
	// Keeps the renderer and tells it the size limit (invented name;
	// UtopiaModule's display-mode slots 6 and 7 call it).
	void	SetRenderer( NiRender *renderer_ );
	NiImage *	GetImage( char *, bool, bool );
	void	EnableMipMaps( bool );
	bool	AreMipMapsEnabled( void );
	void	SetDefaultTextureFilterQuality( int );
	// Releases every image loaded; Mission::Terminate calls it (invented name).
	void	PurgeImages( void );
	// Releases the images nothing but the manager holds; nothing in retail
	// calls it (0x44EA30; invented name).
	void	PurgeUnusedImages( void );
	// Keeps the value at 0x0C and hands it to the renderer's slot 34, marking
	// every image for the renderer again; nothing in retail calls it (0x44E840; TODO identity).
	void	Unknown44E840( unsigned int value );
	NiTextureModeProperty::FilterMode	GetTextureFilterMode( void ) { return textureFilterMode; }
	int	GetDefaultTextureFilterQuality( void ) { return textureFilterQuality; }

	NiRender *	renderer;		// 0x00 (identity unproven: SetMaxImageSize calls its virtual slot 36)
	int		imageDetail;		// 0x04 a *_DETAIL value
	int		maxImageSize;		// 0x08 0 for no limit (invented name)
	int		unknown0C;			// 0x0C
	int		textureFilterQuality;	// 0x10 0 to 2, SetDefaultTextureFilterQuality's argument
	NiTextureModeProperty::FilterMode	textureFilterMode;	// 0x14 the filter of that quality (invented name)
	NiTStringMap<NiPointer<NiImage> >	images;	// 0x18 the images loaded, by file name (invented name): GetImage (0x44E610) looks the file up here; the destructor (0x44E310) stores the map's vtables 0x88B6A0 and 0x88B68C

	// Declared for the SWIG wrappers that call them.
public:
	char *	FindDetailPath( char * );	// retail 0x44E4C0: the SWIG wrapper ImageManagerClass_FindDetailPath calls it
};

// The image manager (the SWIG variable g_kImageManager).
extern ImageManagerClass	g_kImageManager;

#endif /* !__IMAGEMANAGERCLASS_H__ */
