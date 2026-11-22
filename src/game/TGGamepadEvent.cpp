/*
===============================================================================

	TGGamepadEvent.cpp

===============================================================================
*/

#include <iostream>
#include "TGGamepadEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration,
// after the unit's code.
// FUNCTION: STBC 0x006e88e0 _$E418
// FUNCTION: STBC 0x006e8900 _$E417

/*
================
TGGamepadEvent::Copy

Copies the event's state, and the values.
================
*/
// FUNCTION: STBC 0x006e8800
bool TGGamepadEvent::Copy( TGEvent *event ) {
	if ( TGEvent::Copy( event ) == true ) {
		TGGamepadEvent *other = (TGGamepadEvent *)event;
		unknown30 = other->unknown30;
		unknown28 = other->unknown28;
		unknown34 = other->unknown34;
		unknown2C = other->unknown2C;
	}
	return true;
}

/*
================
TGGamepadEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x006e8840
bool TGGamepadEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( unknown28 );
	stream->WriteInt( unknown2C );
	stream->WriteInt( unknown30 );
	stream->WriteLong( unknown34 );
	return true;
}

/*
================
TGGamepadEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x006e8890
bool TGGamepadEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	unknown28 = stream->ReadFloat();
	unknown2C = stream->ReadInt();
	unknown30 = stream->ReadInt();
	unknown34 = stream->ReadLong();
	return true;
}
