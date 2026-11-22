/*
===============================================================================

	NiFlipController.h

	Flips the image of its target, an NiTextureProperty, through a list of
	images at a fixed rate.

	Evidence: the vtable at 0x8981F8 (NiTimeController's 20 slots, none
	added); the constructor (0x793A70) builds an NiTArray of image smart
	pointers at 0x30 (one element, growing by one) and sets the index at
	0x48 to 0, the start time at 0x4C to FLT_MAX, the duration at 0x50 to 0
	and the seconds per frame at 0x54 to 1/30; Update (0x793DC0) stores the
	current image at the texture property's image; slot 19 (0x794780)
	tests the target against NiTextureProperty's NiRTTI. The NiRTTI is
	0x9A00D0, named "NiFlipController".

===============================================================================
*/

#ifndef __NIFLIPCONTROLLER_H__
#define __NIFLIPCONTROLLER_H__

#include "NiTimeController.h"
#include "NiTextureProperty.h"
#include "NiStreamMacros.h"

/*
================
NiFlipControllerLoadRecord

The link IDs of the images, read by LoadBinary (0x794030) and resolved by
LinkObject (0x794170). The class name is invented.

Evidence: the creation function (0x793F20) allocates 0x28 bytes: the
controller record, then an array at 0x10 (vtable 0x89824C) of the default
sizes. That vtable is the array's own; the element type is taken as the
pointer type the IDs stand for, as NiNodeLoadRecord's are.
================
*/
// FUNCTION: STBC 0x793FF0 ??_GNiFlipControllerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x794010 ??1NiFlipControllerLoadRecord@@UAE@XZ
class NiFlipControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
	NiTArray<NiImage *>		imageLinkIDs;
};

// FUNCTION: STBC 0x00794680 ??_GNiFlipController@@UAEPAXI@Z @netimmerse/NiFlipController
// FUNCTION: STBC 0x7946A0 ??1NiFlipController@@UAE@XZ
class NiFlipController : public NiTimeController {
public:
	// FUNCTION: STBC 0x794670 ?GetRTTI@NiFlipController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			Update( float time );
	// FUNCTION: STBC 0x794780 ?TargetIsRequiredType@NiFlipController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiTextureProperty, target );
							}

							NiFlipController();

	// puts an image at an index, growing the list (name invented)
	void					SetImage( NiImage *image, unsigned int index );
	// takes the image at index out of the list
	NiPointer<NiImage>		DetachImage( unsigned int index );

	NiImage *				GetImageAt( unsigned int index );
	unsigned int			GetImageCount() const { return images.GetSize(); }

	// the image at the current index (name invented; not inlined, for the
	// smart pointer temporary's unwind state)
	// FUNCTION: STBC 0x794740 ?GetCurrentImage@NiFlipController@@QBEPAVNiImage@@XZ
	NiImage *				GetCurrentImage() const { return images.GetAt( currentIndex ); }

	// the duration and the high key time follow the image count and rate
	// (names invented)
	void					UpdateDuration();
	void					SetStartTime( float startTime_ );
	float					GetStartTime() const { return startTime; }
	void					SetSecondsPerFrame( float secondsPerFrame_ );
	float					GetSecondsPerFrame() const { return secondsPerFrame; }

	// the first NiFlipController of the target's list (name invented)
	static NiFlipController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiFlipController *dest );

	NiTArray<NiPointer<NiImage> >	images;
	unsigned int			currentIndex;
	float					startTime;		// FLT_MAX after construction
	float					duration;		// the image count times secondsPerFrame
	float					secondsPerFrame;	// 1/30 after construction
};

NiRegisterStream( NiFlipController );

#endif /* !__NIFLIPCONTROLLER_H__ */
