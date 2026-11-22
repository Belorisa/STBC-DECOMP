/*
===============================================================================

	ImageManagerClass.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "ImageManagerClass.h"
#include "renderer/NiImage.h"
#include "renderer/NiRender.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0044f170 _$E231
// FUNCTION: STBC 0x0044f190 _$E230

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0044ef20 _$E32
// FUNCTION: STBC 0x0044ef30 _$E29
// FUNCTION: STBC 0x0044ef40 _$E31
// FUNCTION: STBC 0x0044ef50 _$E30
// FUNCTION: STBC 0x0044ef60 _$E38
// FUNCTION: STBC 0x0044ef70 _$E35
// FUNCTION: STBC 0x0044ef80 _$E37
// FUNCTION: STBC 0x0044ef90 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044f1a0 _$E45
// FUNCTION: STBC 0x0044f1b0 _$E44
// FUNCTION: STBC 0x0044f1c0 _$E80
// FUNCTION: STBC 0x0044f1d0 _$E79
// FUNCTION: STBC 0x0044f1e0 _$E93
// FUNCTION: STBC 0x0044f1f0 _$E92
// FUNCTION: STBC 0x0044f200 _$E56
// FUNCTION: STBC 0x0044f210 _$E55
// FUNCTION: STBC 0x0044f220 _$E60
// FUNCTION: STBC 0x0044f230 _$E59
// FUNCTION: STBC 0x0044f240 _$E64
// FUNCTION: STBC 0x0044f250 _$E63
// FUNCTION: STBC 0x0044f260 _$E68
// FUNCTION: STBC 0x0044f270 _$E67
// FUNCTION: STBC 0x0044f280 _$E171
// FUNCTION: STBC 0x0044f290 _$E170
// FUNCTION: STBC 0x0044f2a0 _$E73
// FUNCTION: STBC 0x0044f2b0 _$E72
// FUNCTION: STBC 0x0044f2c0 _$E97
// FUNCTION: STBC 0x0044f2d0 _$E96
// FUNCTION: STBC 0x0044f2e0 _$E196
// FUNCTION: STBC 0x0044f2f0 _$E195
// FUNCTION: STBC 0x0044f320 _$E206
// FUNCTION: STBC 0x0044f330 _$E205

// The path of the image being looked up, and the path of its version at the
// current detail (the string FindDetailPath returns).
// DATA: STBC 0x00980644
static char imagePath[256];
// DATA: STBC 0x00980504
static char detailPath[256];

// The image manager (the SWIG variable g_kImageManager).
// FUNCTION: STBC 0x0044e2d0 _$E224
// FUNCTION: STBC 0x0044e2e0 _$E221
// FUNCTION: STBC 0x0044e2f0 _$E223
// FUNCTION: STBC 0x0044e300 _$E222
// DATA: STBC 0x00980618
ImageManagerClass	g_kImageManager;

/*
================
ImageManagerClass::ImageManagerClass

High detail and trilinear filtering, no renderer and no size limit.
================
*/
// FUNCTION: STBC 0x0044e3d0
ImageManagerClass::ImageManagerClass( void ) {
	textureFilterQuality = 2;
	textureFilterMode = NiTextureModeProperty::FILTER_TRILERP;
	imageDetail = HIGH_DETAIL;
	renderer = NULL;
	maxImageSize = 0;
	unknown0C = 0;
}

/*
================
ImageManagerClass::SetRenderer
================
*/
// FUNCTION: STBC 0x0044e440
void ImageManagerClass::SetRenderer( NiRender *renderer_ ) {
	if ( renderer_ != NULL ) {
		renderer_->VirtualSlot36( maxImageSize );
	}
	renderer = renderer_;
}

/*
================
ImageManagerClass::SetImageDetail

Low and medium detail limit the images to 128 and 256 pixels, high detail
doesn't limit them.
================
*/
// FUNCTION: STBC 0x0044e470
void ImageManagerClass::SetImageDetail( int detail ) {
	switch ( detail ) {
	case LOW_DETAIL:
		SetMaxImageSize( 128 );
		break;
	case MEDIUM_DETAIL:
		SetMaxImageSize( 256 );
		break;
	case HIGH_DETAIL:
		SetMaxImageSize( 0 );
		break;
	}
	imageDetail = detail;
}

/*
================
ImageManagerClass::FindDetailPath

The path of the image's version at the current detail: its directory plus
the detail's subdirectory plus its file name. The result is kept in a
static buffer.
================
*/
// FUNCTION: STBC 0x0044e4c0 ?FindDetailPath@ImageManagerClass@@QAEPADPAD@Z
// PARKED: register allocation: retail sets the unknown detail's path on the switch's no-case edge after the cases, re-read from the stack (mov ebx, [esp+14h]); the initialised local is set up front from ebp (an explicit default: 97.2, the compare chain falls into it)
char *ImageManagerClass::FindDetailPath( char *path ) {
	strcpy( imagePath, path );
	char *detailDirectory = path;		// BUG: an unknown detail uses the path itself
	switch ( imageDetail ) {
	case LOW_DETAIL:
		detailDirectory = "/Low";
		break;
	case MEDIUM_DETAIL:
		detailDirectory = "/Medium";
		break;
	case HIGH_DETAIL:
		detailDirectory = "/High";
		break;
	}
	char *separator = strrchr( imagePath, '/' );
	if ( separator != NULL ) {
		*separator = '\0';
		strcpy( detailPath, imagePath );
		*separator = '/';
		strcat( detailPath, detailDirectory );
		strcat( detailPath, separator );
	} else {
		strcpy( detailPath, detailDirectory + 1 );
		strcat( detailPath, "/" );
		strcat( detailPath, path );
	}
	return detailPath;
}

/*
================
ImageManagerClass::GetImage

The image of the file, loaded once and kept by name. Unless it is to be
mipmapped, the image comes from the detail directory, at lower details
until it fits the renderer's largest texture size, and from the file
itself when there is none. NULL when it isn't loaded yet and load is
false, or when the file can't be read.
================
*/
// FUNCTION: STBC 0x0044e610
NiImage *ImageManagerClass::GetImage( char *file, bool load, bool mipMap ) {
	NiPointer<NiImage> image;
	if ( images.GetAt( file, image ) ) {
		return image;
	}
	if ( mipMap ) {
		mipMap = NiImage::defaultMipmap;
	}
	if ( load ) {
		int savedDetail = imageDetail;
		unsigned int maxWidth;
		unsigned int maxHeight;
		if ( !mipMap ) {
			renderer->VirtualSlot37( &maxWidth, &maxHeight );
		}
		for ( ;; ) {
			char *detailPath = FindDetailPath( file );
			image = NiImage::Create( detailPath, mipMap, 7, 2 );
			if ( image == NULL || mipMap || imageDetail <= 0 ) {
				break;
			}
			if ( image->GetWidth() <= maxWidth && image->GetHeight() <= maxHeight ) {
				break;
			}
			imageDetail--;
		}
		imageDetail = savedDetail;
		if ( image == NULL ) {
			image = NiImage::Create( file, mipMap, 7, 2 );
		}
		if ( image != NULL ) {
			images.SetAt( file, image );
		}
		return image;
	}
	return NULL;
}

/*
================
ImageManagerClass::Unknown44E840

Keeps the value and hands it to the renderer (its slot 34), then bumps
every loaded image's unknown20 so the renderer takes it up again. Nothing
in retail calls it (TODO identity).
================
*/
// FUNCTION: STBC 0x0044e840
void ImageManagerClass::Unknown44E840( unsigned int value ) {
	unknown0C = value;
	if ( renderer != NULL ) {
		renderer->VirtualSlot34( value );
		NiTMapIterator pos = images.GetFirstPos();
		while ( pos != NULL ) {
			const char *name;
			NiPointer<NiImage> image;
			images.GetNext( pos, name, image );
			image->unknown20++;
		}
	}
}

/*
================
ImageManagerClass::EnableMipMaps

Whether images get mipmaps unless they say otherwise.
================
*/
// FUNCTION: STBC 0x0044e950
void ImageManagerClass::EnableMipMaps( bool enable ) {
	NiImage::defaultMipmap = enable;
}

/*
================
ImageManagerClass::AreMipMapsEnabled
================
*/
// FUNCTION: STBC 0x0044e960
bool ImageManagerClass::AreMipMapsEnabled( void ) {
	return NiImage::defaultMipmap;
}

/*
================
ImageManagerClass::SetMaxImageSize

Keeps the size, and tells the renderer.
================
*/
// FUNCTION: STBC 0x0044e970
void ImageManagerClass::SetMaxImageSize( int size ) {
	maxImageSize = size;
	if ( renderer != NULL ) {
		renderer->VirtualSlot36( size );
	}
}

/*
================
ImageManagerClass::SetDefaultTextureFilterQuality

Point, bilinear or trilinear filtering for the textures made from now on.
================
*/
// FUNCTION: STBC 0x0044e990
void ImageManagerClass::SetDefaultTextureFilterQuality( int quality ) {
	textureFilterQuality = quality;
	switch ( quality ) {
	case 0:
		textureFilterMode = NiTextureModeProperty::FILTER_NEAREST;
		break;
	case 1:
		textureFilterMode = NiTextureModeProperty::FILTER_BILERP;
		break;
	case 2:
		textureFilterMode = NiTextureModeProperty::FILTER_TRILERP;
		break;
	}
}

/*
================
ImageManagerClass::PurgeImages

Releases every image loaded.
================
*/
// FUNCTION: STBC 0x0044e9c0 ?PurgeImages@ImageManagerClass@@QAEXXZ
void ImageManagerClass::PurgeImages( void ) {
	images.RemoveAll();
}

/*
================
ImageManagerClass::PurgeUnusedImages

Releases every image that only the map and this walk hold.
================
*/
// FUNCTION: STBC 0x0044ea30
void ImageManagerClass::PurgeUnusedImages( void ) {
	const char *name;
	NiPointer<NiImage> image;
	NiTMapIterator pos = images.GetFirstPos();
	while ( pos != NULL ) {
		images.GetNext( pos, name, image );
		if ( image->GetRefCount() == 2 ) {
			images.RemoveAt( name );
		}
	}
}
