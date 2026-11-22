/*
===============================================================================

	TGOverlayController.h

	A controller that draws named overlays into a character's face texture.
	It keeps the face image, a pristine copy of its pixels and the overlays
	by name; each overlay holds only the pixels in which its image file
	differs from the face. Showing overlays restores the pristine pixels,
	copies each shown overlay's pixels over them, then tells the image it
	changed (TGOverlayManager::ApplyOverlays, 0x707120).

	A NetImmerse object (reference counted, held through NiPointer). Its
	NiRTTI is at 0x980B20 ("TGOverlayController"; the vtable 0x88B954
	returns it from 0x458890). The vtable is NiObject's twelve slots with
	only GetRTTI and the destructor replaced. Evidence:
	CharacterClass::ReplaceBodyAndHead (0x668430) constructs one (0x457930,
	0x20 bytes), keeps it at 0x180 and hands it the head image file
	(0x457B90); CharacterClass::AddFacialImage passes it each image's name
	and file (0x457E10).

===============================================================================
*/

#ifndef __TGOVERLAYCONTROLLER_H__
#define __TGOVERLAYCONTROLLER_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiTStringMap.h"

class NiImage;
class NiRawImageData;

/*
================
TGOverlayRun

One run of an overlay's pixels: where the run starts in the face image
and in the overlay's packed pixels, and how many pixels it covers, all in
pixels. Six bytes (the overlay's run array is 6 bytes per run). The
struct and member names are invented.
================
*/
struct TGOverlayRun {
	unsigned short			destStart;		// the first pixel of the face image
	unsigned short			sourceStart;	// the first pixel of the packed pixels
	unsigned short			length;			// pixels
};

/*
================
TGOverlayImage

An overlay: the runs of pixels in which its image file differs from the
face, with those pixels packed one run after another. A NetImmerse object
without an NiRTTI of its own (its vtable 0x88B924 returns NiObject's).
0x18 bytes (TGOverlayController::AddImage allocates it). The class and
member names are invented.
================
*/
// FUNCTION: STBC 0x4575C0 ??_GTGOverlayImage@@UAEPAXI@Z @game/TGOverlayController
class TGOverlayImage : public NiObject {
public:
							TGOverlayImage();
	virtual					~TGOverlayImage();

	// copies the overlay's pixels into an image of the face's size
	void					Apply( NiRawImageData *dest );

	// takes the pixels in which overlay differs from base
	void					Build( NiRawImageData *base, NiRawImageData *overlay );

protected:
	unsigned int			pixelStride;	// 0x08 bytes per pixel
	unsigned int			runCount;		// 0x0C
	TGOverlayRun *			runs;			// 0x10 heap block of runCount runs
	unsigned char *			pixels;			// 0x14 heap block of the runs' pixels
};

// FUNCTION: STBC 0x4579C0 ??_GTGOverlayController@@UAEPAXI@Z @game/TGOverlayController
class TGOverlayController : public NiObject {
public:
	// FUNCTION: STBC 0x458890 ?GetRTTI@TGOverlayController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

							TGOverlayController();
	virtual					~TGOverlayController();

	// Takes an image (or, without one, the image file) as the face image and
	// keeps a copy of its pixels; returns the face image, NULL when neither
	// is given or the file can't be read (invented name).
	NiImage *				SetImage( NiImage *newImage, char *file );

	// Reads the image file and files it under the name as an overlay of the
	// face image; returns the overlay, NULL when the file can't be read or
	// there is no face image yet (invented name).
	TGOverlayImage *		AddImage( char *name, char *file );

	// restores the face image's pristine pixels (invented name)
	void					ResetImage();

	// copies the named overlay's pixels into the face image (invented name)
	void					ShowOverlay( const char *name );

	// tells the face image its pixels changed (invented name)
	bool					UpdateImage();

	static const NiRTTI		rtti;

protected:
	NiPointer<NiImage>		image;			// 0x08 the face image
	NiPointer<NiRawImageData>	baseImage;	// 0x0C the face image's pixels before any overlay
	NiTStringMap<NiPointer<TGOverlayImage> >	overlays;	// 0x10 by name
};

#endif /* !__TGOVERLAYCONTROLLER_H__ */
