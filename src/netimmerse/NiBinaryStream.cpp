// NiBinaryStream.cpp -- the abstract byte stream

#include "NiRTLib.h"
#include "NiBinaryStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086ecf0 _$E30
// FUNCTION: STBC 0x0086ed10 _$E17
// FUNCTION: STBC 0x0086ed30 _$E15
// FUNCTION: STBC 0x0086ed40 _$E23
// FUNCTION: STBC 0x0086ed60 _$E21

/*
================
NiBinaryStream::~NiBinaryStream
================
*/
// FUNCTION: STBC 0x86EC10
NiBinaryStream::~NiBinaryStream() {
}

/*
================
NiBinaryStream::GetLine

Reads a byte at a time up to and including a '\n', and puts the terminator
over the last byte read (Gamebryo 1.1.2: NiBinaryStream::GetLine, which
keeps the '\n' out of the buffer instead).

Quirk: a line cut at maxBytes - 1 loses its last byte to the terminator.
================
*/
// FUNCTION: STBC 0x86EC40
unsigned int NiBinaryStream::GetLine( char *buffer, unsigned int maxBytes ) {
	unsigned int i = 0;
	while ( i + 1 < maxBytes ) {
		if ( Read( &buffer[i], 1 ) != 1 ) {
			break;
		}
		if ( buffer[i++] == '\n' ) {
			break;
		}
	}
	// BUG: a stream that is empty from the start writes the terminator before the buffer
	buffer[i - 1] = 0;
	return i;
}

/*
================
NiBinaryStream::PutS
================
*/
// FUNCTION: STBC 0x86ECB0
unsigned int NiBinaryStream::PutS( const char *buffer ) {
	unsigned int i = 0;
	while ( *buffer != 0 ) {
		if ( Write( buffer++, 1 ) == 1 ) {
			i++;
		} else {
			break;
		}
	}
	return i;
}
