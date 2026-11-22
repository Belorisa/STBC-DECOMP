/*
===============================================================================

	TGBufferedFileStream.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGBufferedFileStream.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d3250 _$E32
// FUNCTION: STBC 0x006d3260 _$E29
// FUNCTION: STBC 0x006d3270 _$E31
// FUNCTION: STBC 0x006d3280 _$E30
// FUNCTION: STBC 0x006d3290 _$E38
// FUNCTION: STBC 0x006d32a0 _$E35
// FUNCTION: STBC 0x006d32b0 _$E37
// FUNCTION: STBC 0x006d32c0 _$E36

/*
================
TGBufferedFileStream::TGBufferedFileStream
================
*/
// FUNCTION: STBC 0x006d32d0
TGBufferedFileStream::TGBufferedFileStream( int bufferSize_ ) : buffer( NULL ), filePos( 0 ), bufferPos( 0 ) {
	bufferSize = bufferSize_;
	writing = false;
}

/*
================
TGBufferedFileStream::VirtualSlot00

Opens the file, and sizes the buffer when that worked.
================
*/
// FUNCTION: STBC 0x006d3350
void TGBufferedFileStream::VirtualSlot00( char *fileName, char *mode ) {
	TGStream::VirtualSlot00( fileName, mode );
	filePos = 0;
	bufferPos = 0;
	writing = false;
	if ( strchr( mode, 'w' ) || strchr( mode, 'W' ) ) {
		writing = true;
	}
	if ( VirtualSlot58() == 0 ) {
		SetBufferSize( bufferSize );
	}
}

/*
================
TGBufferedFileStream::Close
================
*/
// FUNCTION: STBC 0x006d33c0
void TGBufferedFileStream::Close( void ) {
	if ( writing ) {
		Flush();
	}
	if ( buffer ) {
		delete buffer;
	}
	buffer = NULL;
	bufferSize = 0;
	filePos = 0;
	bufferPos = 0;
	writing = false;
	TGStream::Close();
}

/*
================
TGBufferedFileStream::SetBufferSize

Replaces the buffer, writing out what the old one held; a reading stream
fills the new one from where it was.
================
*/
// FUNCTION: STBC 0x006d3400
void TGBufferedFileStream::SetBufferSize( int size ) {
	if ( writing && buffer && bufferPos > 0 ) {
		Flush();
	}
	if ( buffer ) {
		delete buffer;
	}
	bufferSize = size;
	buffer = new char[size + 1];
	if ( !writing ) {
		TGStream::VirtualSlot55( bufferPos + filePos, SEEK_SET );
		Refill();
	}
}

/*
================
TGBufferedFileStream::Flush

Writes the buffered bytes to the file.
================
*/
// FUNCTION: STBC 0x006d3470
void TGBufferedFileStream::Flush( void ) {
	if ( bufferPos > 0 ) {
		TGStream::Write( buffer, bufferPos );
		bufferPos = 0;
		filePos = TGStream::GetPos();
	}
}

/*
================
TGBufferedFileStream::Refill

Reads the next buffer's worth from where the buffer left off.
================
*/
// FUNCTION: STBC 0x006d34a0
void TGBufferedFileStream::Refill( void ) {
	if ( bufferPos > 0 ) {
		TGStream::VirtualSlot55( filePos + bufferPos, SEEK_SET );
	}
	filePos = TGStream::GetPos();
	bufferPos = 0;
	TGStream::Read( buffer, bufferSize );
}

/*
================
TGBufferedFileStream::Read

A read as big as the buffer skips it.
================
*/
// FUNCTION: STBC 0x006d34e0
void TGBufferedFileStream::Read( void *data, int size ) {
	if ( size >= bufferSize ) {
		TGStream::VirtualSlot55( bufferPos + filePos, SEEK_SET );
		TGStream::Read( data, size );
		filePos = TGStream::GetPos();
		Refill();
		return;
	}
	if ( bufferPos + size >= bufferSize ) {
		Refill();
	}
	memcpy( data, buffer + bufferPos, size );
	bufferPos += size;
}

/*
================
TGBufferedFileStream::Write

A write as big as the buffer skips it.
================
*/
// FUNCTION: STBC 0x006d3570
void TGBufferedFileStream::Write( void *data, int size ) {
	if ( size >= bufferSize ) {
		Flush();
		TGStream::VirtualSlot55( bufferPos + filePos, SEEK_SET );
		TGStream::Write( data, size );
		filePos = TGStream::GetPos();
		return;
	}
	if ( bufferPos + size >= bufferSize ) {
		Flush();
	}
	memcpy( buffer + bufferPos, data, size );
	bufferPos += size;
}

/*
================
TGBufferedFileStream::ReadCLine
================
*/
// FUNCTION: STBC 0x006d3600
void TGBufferedFileStream::ReadCLine( char *string, int size, char terminator ) {
	TGStream::ReadCLine( string, size, terminator );
	Refill();
}

/*
================
TGBufferedFileStream::WriteCLine
================
*/
// FUNCTION: STBC 0x006d3630
void TGBufferedFileStream::WriteCLine( char *string ) {
	Flush();
	TGStream::WriteCLine( string );
	filePos = TGStream::GetPos();
}

/*
================
TGBufferedFileStream::ReadCString
================
*/
// FUNCTION: STBC 0x006d3660
void TGBufferedFileStream::ReadCString( char *string, int size ) {
	TGStream::ReadCString( string, size );
	Refill();
}

/*
================
TGBufferedFileStream::WriteCString
================
*/
// FUNCTION: STBC 0x006d3680
void TGBufferedFileStream::WriteCString( char *string ) {
	Flush();
	TGStream::WriteCString( string );
	filePos = TGStream::GetPos();
}

/*
================
TGBufferedFileStream::ReadCWLine
================
*/
// FUNCTION: STBC 0x006d36b0
void TGBufferedFileStream::ReadCWLine( unsigned short *string, int size, unsigned short terminator ) {
	TGStream::ReadCWLine( string, size, terminator );
	Refill();
}

/*
================
TGBufferedFileStream::WriteCWLine
================
*/
// FUNCTION: STBC 0x006d36e0
void TGBufferedFileStream::WriteCWLine( unsigned short *string ) {
	Flush();
	TGStream::WriteCWLine( string );
	filePos = TGStream::GetPos();
}

/*
================
TGBufferedFileStream::ReadChar
================
*/
// FUNCTION: STBC 0x006d3710
char TGBufferedFileStream::ReadChar( void ) {
	if ( bufferPos + 1 >= bufferSize ) {
		Refill();
	}
	char value = buffer[bufferPos];
	bufferPos++;
	return value;
}

/*
================
TGBufferedFileStream::ReadShort
================
*/
// FUNCTION: STBC 0x006d3750
short TGBufferedFileStream::ReadShort( void ) {
	if ( bufferPos + 2 >= bufferSize ) {
		Refill();
	}
	short value = *(short *)( buffer + bufferPos );
	bufferPos += 2;
	return value;
}

/*
================
TGBufferedFileStream::ReadInt
================
*/
// FUNCTION: STBC 0x006d3780
int TGBufferedFileStream::ReadInt( void ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Refill();
	}
	int value = *(int *)( buffer + bufferPos );
	bufferPos += 4;
	return value;
}

/*
================
TGBufferedFileStream::ReadLong
================
*/
// FUNCTION: STBC 0x006d37b0
long TGBufferedFileStream::ReadLong( void ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Refill();
	}
	long value = *(long *)( buffer + bufferPos );
	bufferPos += 4;
	return value;
}

/*
================
TGBufferedFileStream::ReadID

An object's ID, as a long. Out of line: UtopiaModule::ProcessInternalLoad
calls it (0x445110) on its stream object.
================
*/
// FUNCTION: STBC 0x006d37e0
long TGBufferedFileStream::ReadID( void ) {
	return ReadLong();
}

/*
================
TGBufferedFileStream::ReadFloat
================
*/
// FUNCTION: STBC 0x006d37f0
float TGBufferedFileStream::ReadFloat( void ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Refill();
	}
	float value = *(float *)( buffer + bufferPos );
	bufferPos += 4;
	return value;
}

/*
================
TGBufferedFileStream::ReadDouble
================
*/
// FUNCTION: STBC 0x006d3820
double TGBufferedFileStream::ReadDouble( void ) {
	// BUG: makes room for four bytes, not eight.
	if ( bufferPos + 4 >= bufferSize ) {
		Refill();
	}
	double value = *(double *)( buffer + bufferPos );
	bufferPos += 8;
	return value;
}

/*
================
TGBufferedFileStream::WriteChar
================
*/
// FUNCTION: STBC 0x006d3850
void TGBufferedFileStream::WriteChar( char value ) {
	if ( bufferPos + 1 >= bufferSize ) {
		Flush();
	}
	buffer[bufferPos] = value;
	bufferPos++;
}

/*
================
TGBufferedFileStream::WriteShort
================
*/
// FUNCTION: STBC 0x006d3890
void TGBufferedFileStream::WriteShort( short value ) {
	if ( bufferPos + 2 >= bufferSize ) {
		Flush();
	}
	*(short *)( buffer + bufferPos ) = value;
	bufferPos += 2;
}

/*
================
TGBufferedFileStream::WriteInt
================
*/
// FUNCTION: STBC 0x006d38d0
void TGBufferedFileStream::WriteInt( int value ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Flush();
	}
	*(int *)( buffer + bufferPos ) = value;
	bufferPos += 4;
}

/*
================
TGBufferedFileStream::WriteLong
================
*/
// FUNCTION: STBC 0x006d3910
void TGBufferedFileStream::WriteLong( long value ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Flush();
	}
	*(long *)( buffer + bufferPos ) = value;
	bufferPos += 4;
}

/*
================
TGBufferedFileStream::WriteFloat
================
*/
// FUNCTION: STBC 0x006d3950
void TGBufferedFileStream::WriteFloat( float value ) {
	if ( bufferPos + 4 >= bufferSize ) {
		Flush();
	}
	*(float *)( buffer + bufferPos ) = value;
	bufferPos += 4;
}

/*
================
TGBufferedFileStream::WriteDouble
================
*/
// FUNCTION: STBC 0x006d3990
void TGBufferedFileStream::WriteDouble( double value ) {
	if ( bufferPos + 8 >= bufferSize ) {
		Flush();
	}
	*(double *)( buffer + bufferPos ) = value;
	bufferPos += 8;
}

/*
================
TGBufferedFileStream::WriteID

An object's ID, as a long. Out of line: UtopiaModule::ProcessSave calls it
(0x44428F) on its stream object.
================
*/
// FUNCTION: STBC 0x006d39d0
void TGBufferedFileStream::WriteID( long id ) {
	WriteLong( id );
}

/*
================
TGBufferedFileStream::GetPos
================
*/
// FUNCTION: STBC 0x006d39e0
long TGBufferedFileStream::GetPos( void ) {
	return bufferPos + filePos;
}

/*
================
TGBufferedFileStream::VirtualSlot55

Moves to offset, dropping the buffer.
================
*/
// FUNCTION: STBC 0x006d39f0
void TGBufferedFileStream::VirtualSlot55( long offset, int origin ) {
	if ( writing ) {
		Flush();
	}
	TGStream::VirtualSlot55( offset, origin );
	filePos = TGStream::GetPos();
	bufferPos = 0;
	if ( !writing ) {
		Refill();
	}
}

/*
================
TGBufferedFileStream::Eof
================
*/
// FUNCTION: STBC 0x006d3a40
bool TGBufferedFileStream::Eof( void ) {
	return bufferPos + filePos >= GetLength() ? true : false;
}
