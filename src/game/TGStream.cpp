/*
===============================================================================

	TGStream.cpp

===============================================================================
*/

#include <io.h>
#include <math.h>
#include <string.h>

#include "TGStream.h"

// The smallest vector length the compressed vectors normalize.
#define VECTOR_EPSILON		1.0e-6f

// One axis of a compressed vector: a signed byte, 127 = 1.0.
#define AXIS_SCALE			127.0f

// A compressed length relative to its maximum: an unsigned byte, 255 = 1.0.
#define LENGTH_SCALE		255.0f

/*
================
TGStream::TGStream
================
*/
// FUNCTION: STBC 0x006d1fc0
TGStream::TGStream( void ) {
	file = NULL;
	writeMode = 0;
	exception = new TGStreamException( TGSTREAM_OK );
	unknown10 = 0.0f;
	unknown14 = 0.0f;
	unknown18 = 0.0f;
}

/*
================
TGStream::~TGStream
================
*/
// FUNCTION: STBC 0x006d2050
TGStream::~TGStream( void ) {
	Close();
	delete exception;
}

/*
================
TGStream::VirtualSlot00

Opens fileName with the fopen mode.
================
*/
// FUNCTION: STBC 0x006d2080
void TGStream::VirtualSlot00( char *fileName, char *mode ) {
	if ( file != NULL ) {
		Fail( TGSTREAM_ALREADY_OPEN );
	}
	writeMode = 0;
	exception->code = TGSTREAM_OK;
	file = fopen( fileName, mode );
	if ( file == NULL ) {
		exception->code = TGSTREAM_OPEN_FAILED;
	}
}

/*
================
TGStream::Close
================
*/
// FUNCTION: STBC 0x006d20e0
void TGStream::Close( void ) {
	if ( file != NULL ) {
		fclose( file );
		file = NULL;
	}
}

/*
================
TGStream::GetLength

The file's length, or -1 with TGSTREAM_NOT_OPEN recorded (not thrown).
================
*/
// FUNCTION: STBC 0x006d2100
int TGStream::GetLength( void ) {
	if ( file != NULL ) {
		return _filelength( file->_file );
	}
	exception->code = TGSTREAM_NOT_OPEN;
	return -1;
}

/*
================
TGStream::SetWriteMode

A file stream takes its mode from fopen.
================
*/
// FUNCTION: STBC 0x006d2130
void TGStream::SetWriteMode( int writeMode ) {
}

/*
================
TGStream::GetWriteMode
================
*/
// FUNCTION: STBC 0x006d2140
int TGStream::GetWriteMode( void ) {
	return writeMode;
}

/*
================
TGStream::Read
================
*/
// FUNCTION: STBC 0x006d2150
void TGStream::Read( void *buffer, int size ) {
	fread( buffer, size, 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
}

/*
================
TGStream::Write
================
*/
// FUNCTION: STBC 0x006d21a0
void TGStream::Write( void *buffer, int size ) {
	fwrite( buffer, size, 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::ReadCLine

Reads up to size - 1 characters, through terminator. A newline terminator
reads with fgets, and running into the end of the file isn't an error.
================
*/
// FUNCTION: STBC 0x006d21f0
void TGStream::ReadCLine( char *buffer, int size, char terminator ) {
	if ( terminator == '\n' ) {
		if ( fgets( buffer, size, file ) == NULL ) {
			exception->code = TGSTREAM_READ_FAILED;
			if ( !feof( file ) ) {
				throw exception;
			}
		}
		return;
	}

	int i;
	for ( i = 0; i < size - 1 && !feof( file ); i++ ) {
		buffer[i] = fgetc( file );
		if ( buffer[i] == terminator ) {
			i++;
			break;
		}
	}
	buffer[i] = '\0';
}

/*
================
TGStream::WriteCLine
================
*/
// FUNCTION: STBC 0x006d22a0
void TGStream::WriteCLine( char *string ) {
	if ( fputs( string, file ) == EOF ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::ReadCString

Reads a string WriteCString wrote, cut to size - 1 characters.
================
*/
// FUNCTION: STBC 0x006d22e0
void TGStream::ReadCString( char *buffer, int size ) {
	long length = ReadLong();
	if ( length > size - 1 ) {
		length = size - 1;
	}
	if ( length > 0 ) {
		Read( buffer, length );
	}
	buffer[length] = '\0';
}

/*
================
TGStream::WriteCString

Writes the length, then the characters without the terminator.
================
*/
// FUNCTION: STBC 0x006d2330
void TGStream::WriteCString( char *string ) {
	int length;
	for ( length = 0; string[length] != '\0'; length++ ) {
	}
	WriteLong( length );
	Write( string, length );
}

/*
================
TGStream::VirtualSlot10

Reads a string VirtualSlot11 wrote into a new buffer; NULL for an empty
string or one longer than a positive maxLength.
================
*/
// FUNCTION: STBC 0x006d2370
char *TGStream::VirtualSlot10( int maxLength ) {
	long length = ReadLong();
	if ( length <= 0 ) {
		return NULL;
	}
	if ( length > maxLength && maxLength > 0 ) {
		return NULL;
	}
	char *string = new char[length];
	Read( string, length );
	return string;
}

/*
================
TGStream::VirtualSlot11

Writes the length with the terminator, then the characters and the
terminator; 0 alone for NULL.
================
*/
// FUNCTION: STBC 0x006d23c0
void TGStream::VirtualSlot11( char *string ) {
	if ( string == NULL ) {
		WriteLong( 0 );
		return;
	}
	int length = strlen( string ) + 1;
	WriteLong( length );
	Write( string, length );
}

/*
================
TGStream::ReadCWLine

Reads up to size - 1 wide characters, through terminator.
================
*/
// FUNCTION: STBC 0x006d2400
void TGStream::ReadCWLine( unsigned short *buffer, int size, unsigned short terminator ) {
	for ( int i = 0; i < size - 1 && !feof( file ); i++ ) {
		unsigned short c = GetWChar();
		buffer[i] = c;
		if ( c == 0 ) {
			return;
		}
		if ( c == terminator ) {
			buffer[i + 1] = 0;
			return;
		}
	}
}

/*
================
TGStream::WriteCWLine

Writes the wide characters without the terminator.
================
*/
// FUNCTION: STBC 0x006d2470
void TGStream::WriteCWLine( unsigned short *string ) {
	while ( *string != 0 ) {
		PutWChar( *string );
		string++;
	}
}

/*
================
TGStream::ReadChar
================
*/
// FUNCTION: STBC 0x006d24b0
char TGStream::ReadChar( void ) {
	char value = fgetc( file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadWChar
================
*/
// FUNCTION: STBC 0x006d24f0
short TGStream::ReadWChar( void ) {
	unsigned short value = GetWChar();
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadShort
================
*/
// FUNCTION: STBC 0x006d2540
short TGStream::ReadShort( void ) {
	short value;
	fread( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadInt
================
*/
// FUNCTION: STBC 0x006d2590
int TGStream::ReadInt( void ) {
	int value;
	fread( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadLong
================
*/
// FUNCTION: STBC 0x006d25e0
long TGStream::ReadLong( void ) {
	long value;
	fread( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadID
================
*/
// FUNCTION: STBC 0x006d2630
long TGStream::ReadID( void ) {
	long id;
	fread( &id, sizeof( id ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return id;
}

/*
================
TGStream::ReadFloat
================
*/
// FUNCTION: STBC 0x006d2680
float TGStream::ReadFloat( void ) {
	float value;
	fread( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::ReadDouble
================
*/
// FUNCTION: STBC 0x006d26d0
double TGStream::ReadDouble( void ) {
	double value;
	fread( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_READ_FAILED );
	}
	return value;
}

/*
================
TGStream::WriteChar
================
*/
// FUNCTION: STBC 0x006d2720
void TGStream::WriteChar( char value ) {
	fputc( value, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteWChar
================
*/
// FUNCTION: STBC 0x006d2770
void TGStream::WriteWChar( unsigned short c ) {
	PutWChar( c );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteShort
================
*/
// FUNCTION: STBC 0x006d27c0
void TGStream::WriteShort( short value ) {
	fwrite( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteInt
================
*/
// FUNCTION: STBC 0x006d2810
void TGStream::WriteInt( int value ) {
	fwrite( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteLong
================
*/
// FUNCTION: STBC 0x006d2860
void TGStream::WriteLong( long value ) {
	fwrite( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteFloat
================
*/
// FUNCTION: STBC 0x006d28b0
void TGStream::WriteFloat( float value ) {
	fwrite( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteDouble
================
*/
// FUNCTION: STBC 0x006d2900
void TGStream::WriteDouble( double value ) {
	fwrite( &value, sizeof( value ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::WriteID
================
*/
// FUNCTION: STBC 0x006d2950
void TGStream::WriteID( long id ) {
	fwrite( &id, sizeof( id ), 1, file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_WRITE_FAILED );
	}
}

/*
================
TGStream::VirtualSlot40

Compresses a vector to its direction and its length.
================
*/
// FUNCTION: STBC 0x006d29a0
void TGStream::VirtualSlot40( float x, float y, float z, char *outX, char *outY, char *outZ, float *outLength ) {
	float length = (float)sqrt( x * x + y * y + z * z );
	*outLength = length;
	if ( length > VECTOR_EPSILON ) {
		float scale = 1.0f / length;
		x *= scale;
		y *= scale;
		z *= scale;
	} else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		*outLength = 0.0f;
	}
	*outX = (char)( x * AXIS_SCALE );
	*outY = (char)( y * AXIS_SCALE );
	*outZ = (char)( z * AXIS_SCALE );
}

/*
================
TGStream::VirtualSlot44

Expands a vector of VirtualSlot40.
================
*/
// FUNCTION: STBC 0x006d2a60
// PARKED: instructions match but for where VC6 schedules the y axis's byte widening; no source form found yet
void TGStream::VirtualSlot44( float *outX, float *outY, float *outZ, char x, char y, char z, float length ) {
	*outX = x * ( 1.0f / AXIS_SCALE );
	*outY = y * ( 1.0f / AXIS_SCALE );
	*outZ = z * ( 1.0f / AXIS_SCALE );
	*outX *= length;
	*outY *= length;
	*outZ *= length;
}

/*
================
TGStream::VirtualSlot41

Compresses a vector to its direction and a 16-bit length.
================
*/
// FUNCTION: STBC 0x006d2ad0
void TGStream::VirtualSlot41( float x, float y, float z, char *outX, char *outY, char *outZ, unsigned short *outLength ) {
	float length = (float)sqrt( x * x + y * y + z * z );
	if ( length > VECTOR_EPSILON ) {
		float scale = 1.0f / length;
		x *= scale;
		y *= scale;
		z *= scale;
	} else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		length = 0.0f;
	}
	*outX = (char)( x * AXIS_SCALE );
	*outY = (char)( y * AXIS_SCALE );
	*outZ = (char)( z * AXIS_SCALE );
	*outLength = Unknown6D3A90( length );
}

/*
================
TGStream::VirtualSlot45

Expands a vector of VirtualSlot41.
================
*/
// FUNCTION: STBC 0x006d2ba0
// PARKED: instructions match but for where VC6 schedules the y axis's byte widening; no source form found yet
void TGStream::VirtualSlot45( float *outX, float *outY, float *outZ, char x, char y, char z, unsigned short length ) {
	float scale = Unknown6D3B30( length );
	*outX = x * ( 1.0f / AXIS_SCALE );
	*outY = y * ( 1.0f / AXIS_SCALE );
	*outZ = z * ( 1.0f / AXIS_SCALE );
	*outX *= scale;
	*outY *= scale;
	*outZ *= scale;
}

/*
================
TGStream::VirtualSlot42

Compresses a unit vector without its length.
================
*/
// FUNCTION: STBC 0x006d2c10
void TGStream::VirtualSlot42( float x, float y, float z, char *outX, char *outY, char *outZ ) {
	*outX = (char)( x * AXIS_SCALE );
	*outY = (char)( y * AXIS_SCALE );
	*outZ = (char)( z * AXIS_SCALE );
}

/*
================
ExpandDirection

The direction of a compressed vector: its axes, normalized again.
================
*/
static inline void ExpandDirection( float *outX, float *outY, float *outZ, char x, char y, char z ) {
	*outX = x * ( 1.0f / AXIS_SCALE );
	*outY = y * ( 1.0f / AXIS_SCALE );
	*outZ = z * ( 1.0f / AXIS_SCALE );
	float length = (float)sqrt( *outX * *outX + *outY * *outY + *outZ * *outZ );
	if ( length > VECTOR_EPSILON ) {
		float scale = 1.0f / length;
		*outX *= scale;
		*outY *= scale;
		*outZ *= scale;
	} else {
		*outX = 0.0f;
		*outY = 0.0f;
		*outZ = 0.0f;
	}
}

/*
================
TGStream::VirtualSlot46

Expands a vector of VirtualSlot42, normalized again.
================
*/
// FUNCTION: STBC 0x006d2c60
// PARKED: instructions match but for where VC6 schedules the y axis's byte widening; no source form found yet
void TGStream::VirtualSlot46( float *outX, float *outY, float *outZ, char x, char y, char z ) {
	ExpandDirection( outX, outY, outZ, x, y, z );
}

/*
================
TGStream::VirtualSlot43

Compresses a vector to its direction and a byte of its length relative to
maxLength.
================
*/
// FUNCTION: STBC 0x006d2d10
void TGStream::VirtualSlot43( float x, float y, float z, char *outX, char *outY, char *outZ, unsigned char *outLength, float maxLength ) {
	float length;
	VirtualSlot40( x, y, z, outX, outY, outZ, &length );
	if ( length > maxLength ) {
		length = maxLength;
	}
	*outLength = (unsigned char)( length / maxLength * LENGTH_SCALE );
}

/*
================
TGStream::VirtualSlot47

Expands a vector of VirtualSlot43.
================
*/
// FUNCTION: STBC 0x006d2d70
// PARKED: instructions match but for where VC6 schedules the y axis's byte widening; no source form found yet
void TGStream::VirtualSlot47( float *outX, float *outY, float *outZ, char x, char y, char z, unsigned char length, float maxLength ) {
	ExpandDirection( outX, outY, outZ, x, y, z );
	float scale = length * maxLength * ( 1.0f / LENGTH_SCALE );
	*outX *= scale;
	*outY *= scale;
	*outZ *= scale;
}

/*
================
TGStream::VirtualSlot34

Writes a unit vector in three bytes.
================
*/
// FUNCTION: STBC 0x006d2e50
void TGStream::VirtualSlot34( float x, float y, float z ) {
	char packedX, packedY, packedZ;
	VirtualSlot42( x, y, z, &packedX, &packedY, &packedZ );
	WriteChar( packedX );
	WriteChar( packedY );
	WriteChar( packedZ );
}

/*
================
TGStream::VirtualSlot35

Reads a vector of VirtualSlot34.
================
*/
// FUNCTION: STBC 0x006d2eb0
void TGStream::VirtualSlot35( float *x, float *y, float *z ) {
	char packedX = ReadChar();
	char packedY = ReadChar();
	char packedZ = ReadChar();
	VirtualSlot46( x, y, z, packedX, packedY, packedZ );
}

/*
================
TGStream::VirtualSlot36

Writes a vector in three bytes and its length as a float or, with
packedLength, in 16 bits.
================
*/
// FUNCTION: STBC 0x006d2f10
void TGStream::VirtualSlot36( float x, float y, float z, bool packedLength ) {
	char packedX, packedY, packedZ;
	unsigned short shortLength;
	float length;
	if ( packedLength ) {
		VirtualSlot41( x, y, z, &packedX, &packedY, &packedZ, &shortLength );
	} else {
		VirtualSlot40( x, y, z, &packedX, &packedY, &packedZ, &length );
	}
	WriteChar( packedX );
	WriteChar( packedY );
	WriteChar( packedZ );
	if ( packedLength ) {
		WriteShort( shortLength );
	} else {
		WriteFloat( length );
	}
}

/*
================
TGStream::VirtualSlot37

Reads a vector of VirtualSlot36.
================
*/
// FUNCTION: STBC 0x006d2fd0
void TGStream::VirtualSlot37( float *x, float *y, float *z, bool packedLength ) {
	char packedX = ReadChar();
	char packedY = ReadChar();
	char packedZ = ReadChar();
	if ( packedLength ) {
		unsigned short length = ReadShort();
		VirtualSlot45( x, y, z, packedX, packedY, packedZ, length );
	} else {
		float length = ReadFloat();
		VirtualSlot44( x, y, z, packedX, packedY, packedZ, length );
	}
}

/*
================
TGStream::VirtualSlot38

Writes a vector in four bytes, its length relative to maxLength.
================
*/
// FUNCTION: STBC 0x006d3070
void TGStream::VirtualSlot38( float x, float y, float z, float maxLength ) {
	char packedX, packedY, packedZ;
	unsigned char packedLength;
	VirtualSlot43( x, y, z, &packedX, &packedY, &packedZ, &packedLength, maxLength );
	WriteChar( packedX );
	WriteChar( packedY );
	WriteChar( packedZ );
	WriteChar( packedLength );
}

/*
================
TGStream::VirtualSlot39

Reads a vector of VirtualSlot38.
================
*/
// FUNCTION: STBC 0x006d30e0
void TGStream::VirtualSlot39( float *x, float *y, float *z, float maxLength ) {
	char packedX = ReadChar();
	char packedY = ReadChar();
	char packedZ = ReadChar();
	unsigned char packedLength = ReadChar();
	VirtualSlot47( x, y, z, packedX, packedY, packedZ, packedLength, maxLength );
}

/*
================
TGStream::VirtualSlot55

Moves to offset, fseek style.
================
*/
// FUNCTION: STBC 0x006d3150
void TGStream::VirtualSlot55( long offset, int origin ) {
	fseek( file, offset, origin );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_SEEK_FAILED );
	}
}

/*
================
TGStream::GetPos
================
*/
// FUNCTION: STBC 0x006d31a0
long TGStream::GetPos( void ) {
	long position = ftell( file );
	if ( ferror( file ) ) {
		Fail( TGSTREAM_SEEK_FAILED );
	}
	return position;
}

/*
================
TGStream::Eof
================
*/
// FUNCTION: STBC 0x006d31e0
bool TGStream::Eof( void ) {
	return feof( file ) ? true : false;
}

/*
================
TGStream::VirtualSlot58

The code of the last failure.
================
*/
// FUNCTION: STBC 0x006d31f0
int TGStream::VirtualSlot58( void ) {
	return exception->code;
}

/*
================
TGStream::VirtualSlot59

Clears the last failure.
================
*/
// FUNCTION: STBC 0x006d3200
void TGStream::VirtualSlot59( void ) {
	exception->code = TGSTREAM_OK;
	clearerr( file );
}

/*
================
TGStreamException::TGStreamException
================
*/
// FUNCTION: STBC 0x006d3220
TGStreamException::TGStreamException( int code ) {
	this->code = code;
	unknown04 = 0;
}

/*
================
TGStreamException::~TGStreamException
================
*/
// FUNCTION: STBC 0x006d3240
TGStreamException::~TGStreamException( void ) {
}
