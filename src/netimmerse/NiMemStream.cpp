// NiMemStream.cpp -- a byte stream in memory

#include <new>
#include <string.h>
#include "NiRTLib.h"
#include <assert.h>
#include "NiMemStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086ef60 _$E30
// FUNCTION: STBC 0x0086ef80 _$E17
// FUNCTION: STBC 0x0086efa0 _$E15
// FUNCTION: STBC 0x0086efb0 _$E23
// FUNCTION: STBC 0x0086efd0 _$E21

/*
================
NiMemStream::NiMemStream

Gamebryo 1.1.2: NiMemStream::NiMemStream( const void *, unsigned int ).
================
*/
// FUNCTION: STBC 0x86ED70 ??0NiMemStream@@QAE@PBXI@Z
NiMemStream::NiMemStream( const void *buffer_, unsigned int size ) {
	buffer = (char *)buffer_;
	read = 0;
	write = size;
	allocSize = 0;
	freeze = false;
}

/*
================
NiMemStream::NiMemStream

Gamebryo 1.1.2: NiMemStream::NiMemStream().
================
*/
// FUNCTION: STBC 0x86EDC0 ??0NiMemStream@@QAE@XZ
NiMemStream::NiMemStream() {
	buffer = new char[allocSize = 1024];
	assert( buffer != NULL );
	read = write = 0;
	freeze = false;
}

/*
================
NiMemStream::~NiMemStream

Frees a buffer of its own unless the caller has taken it.
================
*/
// FUNCTION: STBC 0x86EE20
// FUNCTION: STBC 0x86EDA0 ??_GNiMemStream@@UAEPAXI@Z
NiMemStream::~NiMemStream() {
	if ( allocSize > 0 && !freeze ) {
		delete [] buffer;
	}
}

/*
================
NiMemStream::operator bool
================
*/
// FUNCTION: STBC 0x86EE50 ??BNiMemStream@@UBE_NXZ
NiMemStream::operator bool() const {
	return true;
}

/*
================
NiMemStream::Read

Reads what is left, at most bytes.
================
*/
// FUNCTION: STBC 0x86EE60
unsigned int NiMemStream::Read( void *buffer_, unsigned int bytes ) {
	unsigned int unreadBytes = write - read;
	if ( bytes > unreadBytes ) {
		bytes = unreadBytes;
	}
	memcpy( buffer_, &buffer[read], bytes );
	read += bytes;
	return bytes;
}

/*
================
NiMemStream::Write

A stream over a caller's buffer writes nothing. A full buffer doubles, or
grows by exactly what the write needs when doubling is not enough.
================
*/
// FUNCTION: STBC 0x86EEA0
unsigned int NiMemStream::Write( const void *buffer_, unsigned int bytes ) {
	if ( allocSize == 0 ) {
		return 0;
	}

	unsigned int unwrittenBytes = allocSize - write;
	if ( bytes > unwrittenBytes ) {
		unsigned int newSize = ( bytes <= unwrittenBytes + allocSize ) ? 2 * allocSize : allocSize + bytes - unwrittenBytes;
		char *newBuffer = new char[newSize];
		memcpy( newBuffer, buffer, write );
		delete [] buffer;
		buffer = newBuffer;
		allocSize = newSize;
	}

	memcpy( &buffer[write], buffer_, bytes );
	write += bytes;
	return bytes;
}

/*
================
NiMemStream::Str
================
*/
// FUNCTION: STBC 0x86EF40
void *NiMemStream::Str() {
	freeze = true;
	return buffer;
}

/*
================
NiMemStream::Freeze
================
*/
// FUNCTION: STBC 0x86EF50
void NiMemStream::Freeze( bool freeze_ ) {
	freeze = freeze_;
}
