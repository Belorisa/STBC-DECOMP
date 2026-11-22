/*
===============================================================================

	TGKeyboardEvent.cpp

===============================================================================
*/

#include "TGKeyboardEvent.h"
#include "TGStream.h"

/*
================
TGKeyboardEvent::Copy

Copies the event's state, and the character, key data and key state.
================
*/
// FUNCTION: STBC 0x006e8740
bool TGKeyboardEvent::Copy( TGEvent *event ) {
	if ( TGEvent::Copy( event ) == true ) {
		TGKeyboardEvent *other = (TGKeyboardEvent *)event;
		unicode = other->unicode;
		keyData = other->keyData;
		keyState = other->keyState;
	}
	return true;
}

/*
================
TGKeyboardEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006e8770
bool TGKeyboardEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteWChar( unicode );
	stream->WriteLong( keyData );
	stream->WriteLong( keyState );
	return true;
}

/*
================
TGKeyboardEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006e87b0
bool TGKeyboardEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	unicode = (unsigned short)stream->ReadWChar();
	keyData = stream->ReadLong();
	keyState = stream->ReadLong();
	return true;
}
