/*
===============================================================================

	TGAnimPosition.cpp

===============================================================================
*/

#include <windows.h>
#include <string.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGAnimPosition.h"
#include "TGAnimationManagerClass.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007045b0 _$E235
// FUNCTION: STBC 0x007045c0 _$E232
// FUNCTION: STBC 0x007045d0 _$E234
// FUNCTION: STBC 0x007045e0 _$E233
// FUNCTION: STBC 0x007045f0 _$E241
// FUNCTION: STBC 0x00704600 _$E238
// FUNCTION: STBC 0x00704610 _$E240
// FUNCTION: STBC 0x00704620 _$E239

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).

// FUNCTION: STBC 0x00704a20 _$E248
// FUNCTION: STBC 0x00704a40 _$E247

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00704630 _$E28
// FUNCTION: STBC 0x00704640 _$E27
// FUNCTION: STBC 0x00704650 _$E39
// FUNCTION: STBC 0x00704660 _$E38
// FUNCTION: STBC 0x00704670 _$E43
// FUNCTION: STBC 0x00704680 _$E42
// FUNCTION: STBC 0x00704690 _$E47
// FUNCTION: STBC 0x007046a0 _$E46
// FUNCTION: STBC 0x007046b0 _$E51
// FUNCTION: STBC 0x007046c0 _$E50
// FUNCTION: STBC 0x007046d0 _$E175
// FUNCTION: STBC 0x007046e0 _$E174
// FUNCTION: STBC 0x00704710 _$E184
// FUNCTION: STBC 0x00704720 _$E183

/*
================
TGAnimPosition::TGAnimPosition
================
*/
// FUNCTION: STBC 0x00704730
TGAnimPosition::TGAnimPosition( TGAnimNode *node_, char *animation_ ) {
	node = node_;
	animation = NULL;
	if ( animation_ != NULL ) {
		animation = new char[strlen( animation_ ) + 1];
		strcpy( animation, animation_ );
	}
}

/*
================
TGAnimPosition::~TGAnimPosition
================
*/
// FUNCTION: STBC 0x007047f0
TGAnimPosition::~TGAnimPosition( void ) {
	if ( animation != NULL ) {
		delete [] animation;
	}
}

/*
================
TGAnimPosition::Play

An animation that is not loaded leaves the node as it is.
================
*/
// FUNCTION: STBC 0x00704870
void TGAnimPosition::Play( void ) {
	if ( g_kAnimationManager.IsLoaded( animation ) ) {
		node->UseAnimationPosition( animation );
	}
	Completed();
}

/*
================
TGAnimPosition::WriteToStream
================
*/
// FUNCTION: STBC 0x007048a0
bool TGAnimPosition::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot17( (long)(TGAnimNode *)node );
	stream->VirtualSlot11( animation );
	return true;
}

/*
================
TGAnimPosition::ReadFromStream
================
*/
// FUNCTION: STBC 0x007048e0
bool TGAnimPosition::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}
	savedNode = stream->VirtualSlot16();
	animation = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
TGAnimPosition::FixUpPointers

Fails while the saved node has not been loaded.
================
*/
// FUNCTION: STBC 0x00704920
bool TGAnimPosition::FixUpPointers( void ) {
	if ( !TGAction::FixUpPointers() ) {
		return false;
	}
	if ( savedNode != 0 && pointerMap.Find( savedNode ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGAnimPosition::PostFixUp
================
*/
// FUNCTION: STBC 0x00704990
void TGAnimPosition::PostFixUp( void ) {
	TGAction::PostFixUp();
	node = (TGAnimNode *)pointerMap.Find( savedNode );
}

/*
================
TGAnimPosition::PlaysOn
================
*/
// FUNCTION: STBC 0x00704a10
bool TGAnimPosition::PlaysOn( TGAnimNode *node_ ) {
	return node_ == node ? true : false;
}
