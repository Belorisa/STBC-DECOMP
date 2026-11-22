/*
===============================================================================

	TGOverlayController.cpp

===============================================================================
*/

#include "TGOverlayController.h"
#include "renderer/NiImage.h"
#include "NiImageFile.h"
#include "NiRawImageData.h"
#include "NiTList.h"
#include "NiTextureProperty.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>), and the
// locale id's guard.
// FUNCTION: STBC 0x004585d0 _$E88
// FUNCTION: STBC 0x004585e0 _$E85
// FUNCTION: STBC 0x004585f0 _$E87
// FUNCTION: STBC 0x00458600 _$E86
// FUNCTION: STBC 0x00458610 _$E94
// FUNCTION: STBC 0x00458620 _$E91
// FUNCTION: STBC 0x00458630 _$E93
// FUNCTION: STBC 0x00458640 _$E92
// FUNCTION: STBC 0x004586c0 _$E107
// FUNCTION: STBC 0x004586e0 _$E106

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00458790 _$E21
// FUNCTION: STBC 0x004587a0 _$E20
// FUNCTION: STBC 0x004587b0 _$E36
// FUNCTION: STBC 0x004587c0 _$E35
// FUNCTION: STBC 0x004587d0 _$E49
// FUNCTION: STBC 0x004587e0 _$E48
// FUNCTION: STBC 0x004587f0 _$E53
// FUNCTION: STBC 0x00458800 _$E52
// FUNCTION: STBC 0x00458810 _$E57
// FUNCTION: STBC 0x00458820 _$E56
// FUNCTION: STBC 0x00458830 _$E61
// FUNCTION: STBC 0x00458840 _$E60
// FUNCTION: STBC 0x00458850 _$E65
// FUNCTION: STBC 0x00458860 _$E64
// FUNCTION: STBC 0x00458870 _$E70
// FUNCTION: STBC 0x00458880 _$E69

// FUNCTION: STBC 0x00457540 _$E100
// FUNCTION: STBC 0x00457550 _$E96
// FUNCTION: STBC 0x00457570 _$E99
// FUNCTION: STBC 0x00457580 _$E97
// DATA: STBC 0x980B20 ?rtti@TGOverlayController@@2VNiRTTI@@B
const NiRTTI TGOverlayController::rtti( "TGOverlayController", &NiObject::rtti );

/*
================
TGOverlayImage::TGOverlayImage
================
*/
// FUNCTION: STBC 0x004575a0
TGOverlayImage::TGOverlayImage() {
	pixelStride = 0;
	runCount = 0;
	runs = NULL;
	pixels = NULL;
}

/*
================
TGOverlayImage::~TGOverlayImage
================
*/
// FUNCTION: STBC 0x004575e0
TGOverlayImage::~TGOverlayImage() {
	delete [] runs;
	delete [] pixels;
}

/*
================
TGOverlayImage::Apply

Copies each run's packed pixels to its place in the image.
================
*/
// FUNCTION: STBC 0x00457610
void TGOverlayImage::Apply( NiRawImageData *dest ) {
	for ( unsigned int i = 0; i < runCount; i++ ) {
		memcpy( dest->GetPixels() + runs[i].destStart * pixelStride, pixels + runs[i].sourceStart * pixelStride,
			runs[i].length * pixelStride );
	}
}

/*
================
TGOverlayImage::Build

Walks both images pixel by pixel and collects the runs of pixels that
differ (at most 0xFFFF pixels a run), then packs those pixels of overlay
one run after another.
================
*/
// FUNCTION: STBC 0x00457680
// DRAFT: the call set agrees (NewItem expanded, NiTListBlock's constructor out of line); the first loop's run stores, the GetNext walk's register picks and the list's destructor tail differ
void TGOverlayImage::Build( NiRawImageData *base, NiRawImageData *overlay ) {
	unsigned int stride = base->GetPixelStride();
	pixelStride = stride;
	NiTList<TGOverlayRun> runList( 2 );
	unsigned int pixelCount = base->GetWidth() * base->GetHeight();
	unsigned char *basePixel = base->GetPixels();
	unsigned char *overlayPixel = overlay->GetPixels();
	unsigned int packedCount = 0;

	unsigned int i = 0;
	do {
		if ( memcmp( basePixel, overlayPixel, stride ) != 0 ) {
			TGOverlayRun run;
			run.destStart = (unsigned short)i;
			unsigned int length = 0;
			do {
				i++;
				length++;
				basePixel += stride;
				overlayPixel += stride;
			} while ( i < pixelCount && length < 0xFFFF && memcmp( basePixel, overlayPixel, stride ) != 0 );
			run.sourceStart = (unsigned short)packedCount;
			run.length = (unsigned short)length;
			packedCount += length;
			runList.AddTail( run );
		} else {
			i++;
			basePixel += stride;
			overlayPixel += stride;
		}
	} while ( i < pixelCount );

	unsigned char *overlayPixels = overlay->GetPixels();
	if ( packedCount > 0 ) {
		pixels = new unsigned char[packedCount * stride];
		runCount = runList.GetSize();
		runs = new TGOverlayRun[runCount];
		NiTListIterator pos = runList.GetHeadPos();
		unsigned int index = 0;
		while ( pos != NULL ) {
			TGOverlayRun run = runList.GetNext( pos );
			runs[index] = run;
			index++;
			memcpy( pixels + run.sourceStart * stride, overlayPixels + run.destStart * stride, run.length * stride );
		}
	}
}

/*
================
TGOverlayController::TGOverlayController
================
*/
// FUNCTION: STBC 0x00457930
TGOverlayController::TGOverlayController() {
}

/*
================
TGOverlayController::~TGOverlayController

Releases each overlay before the map goes.
================
*/
// FUNCTION: STBC 0x004579e0
TGOverlayController::~TGOverlayController() {
	NiTMapIterator pos = overlays.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		NiPointer<TGOverlayImage> overlay;
		overlays.GetNext( pos, name, overlay );
		overlay = NULL;
	}
}

/*
================
TGOverlayController::SetImage

An image without pixels of its own is read again from its file into a new
image.
================
*/
// FUNCTION: STBC 0x00457b90
// DRAFT: same blocks; retail lays the Create call and the image assignment out after the image path and jumps back from the file path, and releases raw out of line on the image path's read failure only
NiImage *TGOverlayController::SetImage( NiImage *newImage, char *file ) {
	if ( newImage == NULL && file == NULL ) {
		return NULL;
	}

	NiPointer<NiRawImageData> raw;
	if ( newImage != NULL ) {
		raw = newImage->GetRawImageData();
		if ( raw == NULL ) {
			if ( newImage->GetURL() == NULL ) {
				return NULL;
			}
			raw = new NiRawImageData;
			if ( NiReadImageFile( newImage->GetURL(), raw ) ) {
				newImage = NiImage::Create( raw, 0, NiImage::TEX_DEFAULT, 2 );
			} else {
				return NULL;
			}
		}
	} else {
		raw = new NiRawImageData;
		if ( !NiReadImageFile( file, raw ) ) {
			return NULL;
		}
		newImage = NiImage::Create( raw, 0, NiImage::TEX_DEFAULT, 2 );
	}

	image = newImage;
	baseImage = new NiRawImageData;
	baseImage->Copy( raw );
	return image;
}

/*
================
TGOverlayController::AddImage
================
*/
// FUNCTION: STBC 0x00457e10
TGOverlayImage *TGOverlayController::AddImage( char *name, char *file ) {
	if ( baseImage == NULL ) {
		return NULL;
	}

	NiPointer<NiRawImageData> raw = new NiRawImageData;
	if ( !NiReadImageFile( file, raw ) ) {
		return NULL;
	}

	NiPointer<TGOverlayImage> overlay = new TGOverlayImage;
	overlay->Build( baseImage, raw );
	overlays.SetAt( name, overlay );
	return overlay;
}

/*
================
TGOverlayController::ResetImage
================
*/
// FUNCTION: STBC 0x00457f80
void TGOverlayController::ResetImage() {
	image->GetRawImageData()->Copy( baseImage );
	image->unknown20++;
}

/*
================
TGOverlayController::ShowOverlay
================
*/
// FUNCTION: STBC 0x00457fa0
void TGOverlayController::ShowOverlay( const char *name ) {
	NiPointer<TGOverlayImage> overlay;
	if ( overlays.GetAt( name, overlay ) ) {
		overlay->Apply( image->GetRawImageData() );
	}
}

/*
================
TGOverlayController::UpdateImage
================
*/
// FUNCTION: STBC 0x00458060
bool TGOverlayController::UpdateImage() {
	return image->VirtualSlot12();
}
