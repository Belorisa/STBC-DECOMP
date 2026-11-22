/*
===============================================================================

	TGGameSpyEvent.cpp

===============================================================================
*/

#include <string.h>
#include <winsock.h>

#include "TGGameSpyEvent.h"
#include "TGStream.h"

/*
================
TGGameSpyEvent::TGGameSpyEvent
================
*/
// FUNCTION: STBC 0x006c1760 ??0TGGameSpyEvent@@QAE@XZ
TGGameSpyEvent::TGGameSpyEvent( void ) {
	address = NULL;
	text = NULL;
}

/*
================
TGGameSpyEvent::~TGGameSpyEvent
================
*/
// FUNCTION: STBC 0x006c1790 ??1TGGameSpyEvent@@UAE@XZ
TGGameSpyEvent::~TGGameSpyEvent( void ) {
	if ( text != NULL ) {
		delete [] text;
	}
	if ( address != NULL ) {
		delete address;
	}
}

/*
================
TGGameSpyEvent::Copy

The copy gets its own address and text.
================
*/
// FUNCTION: STBC 0x006c17d0
bool TGGameSpyEvent::Copy( TGEvent *event ) {
	bool copied = TGEvent::Copy( event );
	if ( copied == true ) {
		TGGameSpyEvent *other = (TGGameSpyEvent *)event;
		address = new sockaddr_in;
		*address = *other->address;
		SetText( other->text );
	}
	return copied;
}

/*
================
TGGameSpyEvent::SetAddress
================
*/
// FUNCTION: STBC 0x006c1820
void TGGameSpyEvent::SetAddress( sockaddr_in *address_ ) {
	if ( address != NULL ) {
		delete address;
		address = NULL;
	}
	address = address_;
}

/*
================
TGGameSpyEvent::SetText
================
*/
// FUNCTION: STBC 0x006c1850
void TGGameSpyEvent::SetText( char *text_ ) {
	if ( text != NULL ) {
		delete [] text;
		text = NULL;
	}
	if ( text_ != NULL ) {
		text = new char[strlen( text_ ) + 1];
		strcpy( text, text_ );
	}
}

/*
================
TGGameSpyEvent::WriteToNetStream

Only the text goes: the address stays behind.
================
*/
// FUNCTION: STBC 0x006c18c0
bool TGGameSpyEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->VirtualSlot11( text );
	return true;
}

/*
================
TGGameSpyEvent::ReadFromNetStream

The old text and address are not deleted.
================
*/
// FUNCTION: STBC 0x006c18e0
bool TGGameSpyEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	text = stream->VirtualSlot10( -1 );
	address = NULL;
	return true;
}
