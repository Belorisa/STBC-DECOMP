// NiFlipController.cpp -- texture image flipping

#include "NiRTLib.h"
#include <assert.h>
#include <float.h>

#include "NiFlipController.h"
#include "NiObjectNET.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00794540 _$E97
// FUNCTION: STBC 0x00794560 _$E96
// FUNCTION: STBC 0x007944c0 _$E17
// FUNCTION: STBC 0x007944d0 _$E14
// FUNCTION: STBC 0x007944e0 _$E16
// FUNCTION: STBC 0x007944f0 _$E15
// FUNCTION: STBC 0x00794500 _$E23
// FUNCTION: STBC 0x00794510 _$E20
// FUNCTION: STBC 0x00794520 _$E22
// FUNCTION: STBC 0x00794530 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00794570 _$E30
// FUNCTION: STBC 0x00794580 _$E29
// FUNCTION: STBC 0x00794590 _$E60
// FUNCTION: STBC 0x007945a0 _$E59
// FUNCTION: STBC 0x007945b0 _$E73
// FUNCTION: STBC 0x007945c0 _$E72
// FUNCTION: STBC 0x007945d0 _$E45
// FUNCTION: STBC 0x007945e0 _$E44
// FUNCTION: STBC 0x007945f0 _$E41
// FUNCTION: STBC 0x00794600 _$E40
// FUNCTION: STBC 0x00794610 _$E49
// FUNCTION: STBC 0x00794620 _$E48
// FUNCTION: STBC 0x00794630 _$E53
// FUNCTION: STBC 0x00794640 _$E52
// FUNCTION: STBC 0x00794650 _$E78
// FUNCTION: STBC 0x00794660 _$E77
// FUNCTION: STBC 0x00794720 _$E82
// FUNCTION: STBC 0x00794730 _$E81

// FUNCTION: STBC 0x00793870 _$E88
// FUNCTION: STBC 0x00793880 _$E84
// FUNCTION: STBC 0x007938a0 _$E87
// FUNCTION: STBC 0x007938b0 _$E85
// DATA: STBC 0x9A00D0 ?rtti@NiFlipController@@2VNiRTTI@@B
const NiRTTI NiFlipController::rtti( "NiFlipController", &NiTimeController::rtti );

/*
================
NiFlipController::Register
================
*/
// FUNCTION: STBC 0x7938D0 ?Register@NiFlipController@@SAHXZ
NiImplementRegister( NiFlipController )

/*
================
NiFlipController::FindController
================
*/
// FUNCTION: STBC 0x793A20
NiFlipController *NiFlipController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiFlipController *flipController = NiDynamicCast( NiFlipController, controller );
		if ( flipController ) {
			return flipController;
		}
	}
	return 0;
}

/*
================
NiFlipController::NiFlipController

No images yet, flipped at 30 frames per second.
================
*/
// FUNCTION: STBC 0x793A70
NiFlipController::NiFlipController()
	: images( 1, 1 ), currentIndex( 0 ), startTime( FLT_MAX ), duration( 0.0f ), secondsPerFrame( 1.0f / 30.0f ) {
}

/*
================
NiFlipController::SetImage

Grows the list to hold index first.
================
*/
// FUNCTION: STBC 0x793B20
void NiFlipController::SetImage( NiImage *image, unsigned int index ) {
	if ( images.GetSize() <= index ) {
		images.SetSize( index + 1 );
	}
	images.SetAtGrow( index, image );
	UpdateDuration();
}

/*
================
NiFlipController::DetachImage

Takes the image at index out of the list and returns it (Gamebryo 1.2.2:
NiFlipController::DetachTexture).
================
*/
// FUNCTION: STBC 0x793C70
NiPointer<NiImage> NiFlipController::DetachImage( unsigned int index ) {
	NiPointer<NiImage> image = images.RemoveAt( index );
	UpdateDuration();
	return image;
}

/*
================
NiFlipController::GetImageAt

The image at index, or NULL past the images (name invented).
================
*/
// FUNCTION: STBC 0x793D10
NiImage *NiFlipController::GetImageAt( unsigned int index ) {
	if ( index < images.GetSize() ) {
		return images.GetAt( index );
	}
	return 0;
}

/*
================
NiFlipController::UpdateDuration

The duration covers every image; an index past the images restarts at
the first.
================
*/
// FUNCTION: STBC 0x793D60
void NiFlipController::UpdateDuration() {
	unsigned int count = images.GetSize();
	duration = count * secondsPerFrame;
	hiKeyTime = startTime + duration;
	if ( currentIndex >= count ) {
		currentIndex = 0;
	}
}

/*
================
NiFlipController::SetStartTime
================
*/
// FUNCTION: STBC 0x793D90
void NiFlipController::SetStartTime( float startTime_ ) {
	startTime = startTime_;
	loKeyTime = startTime_;
	hiKeyTime = startTime_ + duration;
}

/*
================
NiFlipController::SetSecondsPerFrame
================
*/
// FUNCTION: STBC 0x793DB0
void NiFlipController::SetSecondsPerFrame( float secondsPerFrame_ ) {
	secondsPerFrame = secondsPerFrame_;
	UpdateDuration();
}

/*
================
NiFlipController::Update

Shows the image of the frame the scaled time falls in.
================
*/
// FUNCTION: STBC 0x793DC0
void NiFlipController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	float elapsedTime = ComputeScaledTime( time ) - loKeyTime;	// (name invented; fits the /Od frame order)
	unsigned int uiMaxIndex = images.GetSize();	// the name of AnimOnly.exe's assert (NiFlipController.cpp(89)); it fits the /Od frame order
	assert( uiMaxIndex != 0 );
	currentIndex = (unsigned int)( uiMaxIndex * elapsedTime / duration );
	if ( currentIndex >= uiMaxIndex ) {
		currentIndex = uiMaxIndex - 1;
	}

	( (NiTextureProperty *)target )->SetImage( GetCurrentImage() );
}

/*
================
NiFlipController::CreateClone
================
*/
// FUNCTION: STBC 0x793E60
NiObject *NiFlipController::CreateClone() {
	NiFlipController *clone = new NiFlipController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiFlipController::CopyMembers

The clone shares the images.
================
*/
// FUNCTION: STBC 0x793EC0
void NiFlipController::CopyMembers( NiFlipController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->SetStartTime( GetStartTime() );
	dest->SetSecondsPerFrame( GetSecondsPerFrame() );
	for ( unsigned int i = 0; i < GetImageCount(); i++ ) {
		NiImage *image = GetImageAt( i );
		dest->SetImage( image, i );
	}
	dest->UpdateDuration();
}

/*
================
NiFlipController::CreateObject
================
*/
// FUNCTION: STBC 0x793F20
void NiFlipController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiFlipController *controller = new NiFlipController;
	stream.SetLoadRecord( new NiFlipControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiFlipController::LoadBinary

The start time and rate, then the image count and the images' link IDs.
================
*/
// FUNCTION: STBC 0x794030
void NiFlipController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiStreamLoadBinary( stream, startTime );
	NiStreamLoadBinary( stream, secondsPerFrame );

	NiFlipControllerLoadRecord *record = (NiFlipControllerLoadRecord *)stream.GetLoadRecord();
	unsigned int count;
	NiStreamLoadBinary( stream, count );
	for ( unsigned int i = 0; i < count; i++ ) {
		NiImage *linkID;
		NiStreamLoadBinary( stream, linkID );
		record->imageLinkIDs.SetAtGrow( i, linkID );
	}
}

/*
================
NiFlipController::LinkObject

Resolves the images once the controller has a target.
================
*/
// FUNCTION: STBC 0x794170
void NiFlipController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );

	if ( target ) {
		NiFlipControllerLoadRecord *record = (NiFlipControllerLoadRecord *)stream.GetLoadRecord();
		unsigned int count = record->imageLinkIDs.GetSize();
		for ( unsigned int i = 0; i < count; i++ ) {
			unsigned int linkID = (unsigned int)record->imageLinkIDs.GetAt( i );
			if ( linkID ) {
				images.SetAtGrow( i, (NiImage *)stream.GetObjectFromLinkID( linkID ) );
			} else {
				images.SetAtGrow( i, 0 );
			}
		}
	}
	UpdateDuration();
}

/*
================
NiFlipController::RegisterStreamables

Registers the images after the controller.
================
*/
// FUNCTION: STBC 0x794200
bool NiFlipController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}

	unsigned int count = images.GetSize();
	for ( unsigned int i = 0; i < count; i++ ) {
		NiImage *image = images.GetAt( i );
		if ( image ) {
			image->RegisterStreamables( stream );
		}
	}
	return true;
}

/*
================
NiFlipController::SaveBinary

The start time and rate, then the image count and the images' link IDs.
================
*/
// FUNCTION: STBC 0x794280
void NiFlipController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, startTime );
	NiStreamSaveBinary( stream, secondsPerFrame );

	unsigned int count = images.GetSize();
	NiStreamSaveBinary( stream, count );
	for ( unsigned int i = 0; i < count; i++ ) {
		NiImage *image = images.GetAt( i );
		NiStreamSaveBinary( stream, image );
	}
}

/*
================
NiFlipController::IsEqual

The images themselves are not compared.
================
*/
// FUNCTION: STBC 0x794330
bool NiFlipController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiFlipController *controller = (NiFlipController *)object;
	if ( startTime != controller->startTime
		|| duration != controller->duration
		|| secondsPerFrame != controller->secondsPerFrame ) {
		return false;
	}
	return true;
}
