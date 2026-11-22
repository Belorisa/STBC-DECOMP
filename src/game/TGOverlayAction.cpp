/*
===============================================================================

	TGOverlayAction.cpp

===============================================================================
*/

#include <windows.h>
#include <string.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGOverlayAction.h"
#include "TGPointerMap.h"
#include "TGStream.h"

/*
================
TGOverlayAction::TGOverlayAction
================
*/
// FUNCTION: STBC 0x00709140
TGOverlayAction::TGOverlayAction( TGOverlayManager *manager_, char *controller_, char *overlay_, float duration_ ) {
	manager = manager_;
	overlay = new char[strlen( overlay_ ) + 1];
	strcpy( overlay, overlay_ );
	controller = new char[strlen( controller_ ) + 1];
	strcpy( controller, controller_ );
	duration = duration_;
}

/*
================
TGOverlayAction::~TGOverlayAction
================
*/
// FUNCTION: STBC 0x00709240
TGOverlayAction::~TGOverlayAction( void ) {
	if ( manager != NULL && controller != NULL && overlay != NULL ) {
		manager->RemoveOverlay( controller, overlay );
	}
	delete [] overlay;
	delete [] controller;
	manager = NULL;
}

/*
================
TGOverlayAction::Play
================
*/
// FUNCTION: STBC 0x00709310
void TGOverlayAction::Play( void ) {
	manager->AddOverlay( controller, overlay );
	TGTimedAction::Play();
}

/*
================
TGOverlayAction::WriteToStream
================
*/
// FUNCTION: STBC 0x00709330
bool TGOverlayAction::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot17( (long)(TGOverlayManager *)manager );
	stream->VirtualSlot11( controller );
	stream->VirtualSlot11( overlay );
	return true;
}

/*
================
TGOverlayAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x00709370
bool TGOverlayAction::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}
	savedManager = stream->VirtualSlot16();
	controller = stream->VirtualSlot10( -1 );
	overlay = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
TGOverlayAction::FixUpPointers

Fails while the saved manager has not been loaded.
================
*/
// FUNCTION: STBC 0x007093b0
bool TGOverlayAction::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}
	if ( savedManager != 0 && pointerMap.Find( savedManager ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGOverlayAction::PostFixUp
================
*/
// FUNCTION: STBC 0x00709420
void TGOverlayAction::PostFixUp( void ) {
	TGAction::PostFixUp();
	if ( savedManager != 0 ) {
		manager = (TGOverlayManager *)pointerMap.Find( savedManager );
	}
}
