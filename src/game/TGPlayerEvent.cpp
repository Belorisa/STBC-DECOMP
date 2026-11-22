/*
===============================================================================

	TGPlayerEvent.cpp

===============================================================================
*/

#include "TGPlayerEvent.h"
#include "TGStream.h"
#include "NiMain.h"

/*
================
TGPlayerEvent::TGPlayerEvent
================
*/
// FUNCTION: STBC 0x006bb840 ??0TGPlayerEvent@@QAE@XZ
TGPlayerEvent::TGPlayerEvent( void ) {
	playerID = 0;
}

/*
================
TGPlayerEvent::~TGPlayerEvent
================
*/
// FUNCTION: STBC 0x006bb860 ??1TGPlayerEvent@@UAE@XZ
TGPlayerEvent::~TGPlayerEvent( void ) {
}

/*
================
TGPlayerEvent::Copy
================
*/
// FUNCTION: STBC 0x006bb870
bool TGPlayerEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		playerID = ( (TGPlayerEvent *)event )->playerID;
	}
	return copied;
}

/*
================
TGPlayerEvent::WriteToNetStream

The ID goes as one byte.
================
*/
// FUNCTION: STBC 0x006bb890
bool TGPlayerEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteChar( (char)playerID );
	return true;
}

/*
================
TGPlayerEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x006bb8b0
bool TGPlayerEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	playerID = stream->ReadChar();
	return true;
}
