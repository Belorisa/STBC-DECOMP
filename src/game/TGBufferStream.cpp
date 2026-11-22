/*
===============================================================================

	TGBufferStream.cpp

===============================================================================
*/

#include <string.h>

#include "TGBufferStream.h"

/*
================
TGBufferStream::TGBufferStream
================
*/
// FUNCTION: STBC 0x006cefe0
TGBufferStream::TGBufferStream( void ) {
	buffer = NULL;
	size = 0;
	position = 0;
	boolPosition = 0;
	boolMask = 0;
}

/*
================
TGBufferStream::GetBuffer
================
*/
// FUNCTION: STBC 0x006cf0c0
char *TGBufferStream::GetBuffer( void ) {
	return buffer;
}

/*
================
TGBufferStream::OpenBuffer

Starts a stream over a new buffer of size bytes.
================
*/
// FUNCTION: STBC 0x006cf0d0
void TGBufferStream::OpenBuffer( int size ) {
	VirtualSlot60( new char[size], size );
}

/*
================
TGBufferStream::CloseBuffer

Closes the stream and frees its buffer.
================
*/
// FUNCTION: STBC 0x006cf100
void TGBufferStream::CloseBuffer( void ) {
	char *data = buffer;
	Close();
	delete data;
}

/*
================
TGBufferStream::~TGBufferStream
================
*/
// FUNCTION: STBC 0x006cf120
TGBufferStream::~TGBufferStream( void ) {
	Close();
}

/*
================
TGBufferStream::VirtualSlot00

A buffer stream can't open a file.
================
*/
// FUNCTION: STBC 0x006cf170
void TGBufferStream::VirtualSlot00( char *fileName, char *mode ) {
	exception->code = TGSTREAM_NOT_OPEN;
}

/*
================
TGBufferStream::VirtualSlot60

Starts a stream over size bytes of data, which the caller keeps.
================
*/
// FUNCTION: STBC 0x006cf180
void TGBufferStream::VirtualSlot60( char *data, int size ) {
	if ( buffer != NULL ) {
		exception->code = TGSTREAM_ALREADY_OPEN;
		return;
	}
	writeMode = 0;
	exception->code = TGSTREAM_OK;
	buffer = data;
	this->size = size;
	position = 0;
	boolPosition = 0;
	boolMask = 0;
}

/*
================
TGBufferStream::Close
================
*/
// FUNCTION: STBC 0x006cf1c0
void TGBufferStream::Close( void ) {
	if ( buffer != NULL ) {
		buffer = NULL;
		size = 0;
		position = 0;
		boolPosition = 0;
		boolMask = 0;
	} else {
		exception->code = TGSTREAM_NOT_OPEN;
	}
}

/*
================
TGBufferStream::GetLength
================
*/
// FUNCTION: STBC 0x006cf1f0
int TGBufferStream::GetLength( void ) {
	if ( buffer != NULL ) {
		return size;
	}
	exception->code = TGSTREAM_NOT_OPEN;
	return -1;
}

/*
================
TGBufferStream::SetWriteMode
================
*/
// FUNCTION: STBC 0x006cf210
void TGBufferStream::SetWriteMode( int writeMode ) {
}

/*
================
TGBufferStream::GetWriteMode
================
*/
// FUNCTION: STBC 0x006cf220
int TGBufferStream::GetWriteMode( void ) {
	return writeMode;
}

/*
================
TGBufferStream::Read

Copies what is left when the buffer runs out.
================
*/
// FUNCTION: STBC 0x006cf230
void TGBufferStream::Read( void *data, int count ) {
	if ( position + count > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		int remaining = size - position;
		if ( remaining > 0 ) {
			memcpy( data, buffer + position, remaining );
			position += remaining;
		}
		return;
	}
	memcpy( data, buffer + position, count );
	position += count;
}

/*
================
TGBufferStream::Write

Copies what fits when the buffer runs out.
================
*/
// FUNCTION: STBC 0x006cf2b0
void TGBufferStream::Write( void *data, int count ) {
	if ( position + count > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		int remaining = size - position;
		if ( remaining > 0 ) {
			memcpy( buffer + position, data, remaining );
			position += remaining;
		}
		return;
	}
	memcpy( buffer + position, data, count );
	position += count;
}

/*
================
TGBufferStream::ReadCLine
================
*/
// FUNCTION: STBC 0x006cf330
// PARKED: retail gives the empty-line case its own exit block; loop shapes tried all merge it
void TGBufferStream::ReadCLine( char *line, int lineSize, char terminator ) {
	int i;
	for ( i = 0; i < lineSize - 1 && position < size; i++ ) {
		line[i] = buffer[position];
		position++;
		if ( line[i] == terminator ) {
			line[i + 1] = '\0';
			return;
		}
	}
	line[i] = '\0';
}

/*
================
TGBufferStream::WriteCLine

Writes the line and a newline; a line that doesn't fit fills the buffer.
================
*/
// FUNCTION: STBC 0x006cf3a0
void TGBufferStream::WriteCLine( char *string ) {
	int length = strlen( string );
	if ( position + length + 1 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	memcpy( buffer + position, string, length );
	position += length;
	buffer[position] = '\n';
	position++;
}

/*
================
TGBufferStream::ReadCString
================
*/
// FUNCTION: STBC 0x006cf410
void TGBufferStream::ReadCString( char *string, int stringSize ) {
	long length = ReadLong();
	if ( length > stringSize - 1 ) {
		length = stringSize - 1;
	}
	if ( length > 0 ) {
		Read( string, length );
	}
	string[length] = '\0';
}

/*
================
TGBufferStream::WriteCString
================
*/
// FUNCTION: STBC 0x006cf460
void TGBufferStream::WriteCString( char *string ) {
	int length;
	for ( length = 0; string[length] != '\0'; length++ ) {
	}
	WriteLong( length );
	Write( string, length );
}

/*
================
TGBufferStream::ReadCWLine
================
*/
// FUNCTION: STBC 0x006cf4a0
void TGBufferStream::ReadCWLine( unsigned short *line, int lineSize, unsigned short terminator ) {
	for ( int i = 0; i < lineSize - 1 && position + 1 < size; i++ ) {
		unsigned short c = ReadShort();
		line[i] = c;
		if ( c == 0 ) {
			return;
		}
		if ( c == terminator ) {
			line[i + 1] = 0;
			return;
		}
	}
}

/*
================
TGBufferStream::WriteCWLine

Writes the wide characters and the terminator.
================
*/
// FUNCTION: STBC 0x006cf500
void TGBufferStream::WriteCWLine( unsigned short *string ) {
	while ( *string != 0 ) {
		WriteShort( *string );
		string++;
	}
	WriteShort( 0 );
}

/*
================
TGBufferStream::ReadChar
================
*/
// FUNCTION: STBC 0x006cf540
char TGBufferStream::ReadChar( void ) {
	if ( position + 1 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		return 0;
	}
	return buffer[position++];
}

/*
================
TGBufferStream::ReadWChar
================
*/
// FUNCTION: STBC 0x006cf570
short TGBufferStream::ReadWChar( void ) {
	return ReadShort();
}

/*
================
TGBufferStream::ReadBool

Takes the next bit of the current packed byte.
================
*/
// FUNCTION: STBC 0x006cf580
// PARKED: VC6 keeps the packed byte, not its count, across the bit test; no source form found yet
bool TGBufferStream::ReadBool( void ) {
	if ( boolMask == 0 ) {
		if ( position + 1 > size ) {
			// BUG: a read failure recorded as a write failure.
			exception->code = TGSTREAM_WRITE_FAILED;
			return false;
		}
		boolPosition = position;
		boolMask = 1;
		position++;
	}
	unsigned char packed = buffer[boolPosition];
	unsigned char count = packed >> MAX_PACKED_BOOLS;
	bool value = ( boolMask & packed & 0x1F ) != 0;
	unsigned char lastMask = 1 << count;
	boolMask <<= 1;
	if ( boolMask >= lastMask ) {
		boolMask = 0;
	}
	return value;
}

/*
================
TGBufferStream::ReadShort
================
*/
// FUNCTION: STBC 0x006cf600
short TGBufferStream::ReadShort( void ) {
	if ( position + 2 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		position += 2;
		return 0;
	}
	short value = *(short *)( buffer + position );
	position += 2;
	return value;
}

/*
================
TGBufferStream::ReadInt
================
*/
// FUNCTION: STBC 0x006cf640
int TGBufferStream::ReadInt( void ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		position += 4;
		return 0;
	}
	int value = *(int *)( buffer + position );
	position += 4;
	return value;
}

/*
================
TGBufferStream::ReadLong
================
*/
// FUNCTION: STBC 0x006cf670
long TGBufferStream::ReadLong( void ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		position += 4;
		return 0;
	}
	long value = *(long *)( buffer + position );
	position += 4;
	return value;
}

/*
================
TGBufferStream::ReadID
================
*/
// FUNCTION: STBC 0x006cf6a0
long TGBufferStream::ReadID( void ) {
	return ReadLong();
}

/*
================
TGBufferStream::ReadFloat
================
*/
// FUNCTION: STBC 0x006cf6b0
float TGBufferStream::ReadFloat( void ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		position += 4;
		return 0.0f;
	}
	float value = *(float *)( buffer + position );
	position += 4;
	return value;
}

/*
================
TGBufferStream::ReadDouble
================
*/
// FUNCTION: STBC 0x006cf6f0
double TGBufferStream::ReadDouble( void ) {
	if ( position + 8 > size ) {
		exception->code = TGSTREAM_READ_FAILED;
		position += 8;
		return 0.0;
	}
	double value = *(double *)( buffer + position );
	position += 8;
	return value;
}

/*
================
TGBufferStream::WriteChar
================
*/
// FUNCTION: STBC 0x006cf730
void TGBufferStream::WriteChar( char value ) {
	if ( position + 1 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		return;
	}
	buffer[position] = value;
	position++;
}

/*
================
TGBufferStream::WriteWChar
================
*/
// FUNCTION: STBC 0x006cf760
void TGBufferStream::WriteWChar( unsigned short c ) {
	WriteShort( c );
}

/*
================
TGBufferStream::WriteBool

Sets the next bit of the current packed byte, starting a new byte after
MAX_PACKED_BOOLS.
================
*/
// FUNCTION: STBC 0x006cf770
void TGBufferStream::WriteBool( bool value ) {
	unsigned char packed;
	if ( boolMask == 0 ) {
		if ( position + 1 > size ) {
			exception->code = TGSTREAM_WRITE_FAILED;
			position = size;
			return;
		}
		boolPosition = position;
		packed = 0;
		boolMask = 1;
		position++;
	} else {
		packed = buffer[boolPosition];
	}
	unsigned char count = packed >> MAX_PACKED_BOOLS;
	packed &= 0x1F;
	if ( value ) {
		packed |= boolMask;
	}
	boolMask <<= 1;
	count++;
	packed |= count << MAX_PACKED_BOOLS;
	if ( count >= MAX_PACKED_BOOLS ) {
		boolMask = 0;
	}
	buffer[boolPosition] = packed;
}

/*
================
TGBufferStream::WriteShort
================
*/
// FUNCTION: STBC 0x006cf7f0
void TGBufferStream::WriteShort( short value ) {
	if ( position + 2 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	*(short *)( buffer + position ) = value;
	position += 2;
}

/*
================
TGBufferStream::WriteInt
================
*/
// FUNCTION: STBC 0x006cf830
void TGBufferStream::WriteInt( int value ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	*(int *)( buffer + position ) = value;
	position += 4;
}

/*
================
TGBufferStream::WriteLong
================
*/
// FUNCTION: STBC 0x006cf870
void TGBufferStream::WriteLong( long value ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	*(long *)( buffer + position ) = value;
	position += 4;
}

/*
================
TGBufferStream::WriteFloat
================
*/
// FUNCTION: STBC 0x006cf8b0
void TGBufferStream::WriteFloat( float value ) {
	if ( position + 4 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	*(float *)( buffer + position ) = value;
	position += 4;
}

/*
================
TGBufferStream::WriteDouble
================
*/
// FUNCTION: STBC 0x006cf8f0
void TGBufferStream::WriteDouble( double value ) {
	if ( position + 8 > size ) {
		exception->code = TGSTREAM_WRITE_FAILED;
		position = size;
		return;
	}
	*(double *)( buffer + position ) = value;
	position += 8;
}

/*
================
TGBufferStream::WriteID
================
*/
// FUNCTION: STBC 0x006cf930
void TGBufferStream::WriteID( long id ) {
	WriteLong( id );
}

/*
================
TGBufferStream::VirtualSlot55

Moves to offset, fseek style.
================
*/
// FUNCTION: STBC 0x006cf940
void TGBufferStream::VirtualSlot55( long offset, int origin ) {
	if ( origin == SEEK_SET ) {
		if ( offset >= size ) {
			exception->code = TGSTREAM_SEEK_FAILED;
			return;
		}
		position = offset;
	} else if ( origin == SEEK_CUR ) {
		if ( position + offset > size ) {
			exception->code = TGSTREAM_SEEK_FAILED;
			return;
		}
		position += offset;
	} else {
		if ( offset >= size ) {
			exception->code = TGSTREAM_SEEK_FAILED;
			position = 0;
			return;
		}
		position = size - offset;
	}
}

/*
================
TGBufferStream::GetPos
================
*/
// FUNCTION: STBC 0x006cf9b0
long TGBufferStream::GetPos( void ) {
	return position;
}

/*
================
TGBufferStream::Eof
================
*/
// FUNCTION: STBC 0x006cf9c0
bool TGBufferStream::Eof( void ) {
	return position >= size ? true : false;
}
