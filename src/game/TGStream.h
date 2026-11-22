/*
===============================================================================

	TGStream.h

	A binary stream over a C runtime FILE, the base of TGBufferStream. Errors
	are reported by throwing the stream's TGStreamException (by pointer: the
	throw information at 0x8ADE80 names "PAVTGStreamException@@"), with its
	code set to one of the tgStreamError_t values.

	The class name is SWIG's type string "TGStream *" (0x912788), and the
	method names are those of the TGBufferStream_* wrappers for the same
	vtable slots. The destructor is not virtual: TGBufferStream's destructor
	calls it directly, and slot 0 opens a file.

	Slots 34-47 read and write vectors compressed to three signed bytes (one
	byte per axis, 127 = 1.0), with the length kept as a float, a 16-bit
	value or a byte scaled to a maximum. They use no member, but they are
	virtual all the same.

===============================================================================
*/

#ifndef __TGSTREAM_H__
#define __TGSTREAM_H__

#include <stdio.h>

// The code a TGStreamException carries. The names are invented; the values
// are the ones each failure stores.
enum tgStreamError_t {
	TGSTREAM_OK				= 0,
	TGSTREAM_OPEN_FAILED	= -1,
	TGSTREAM_NOT_OPEN		= -2,
	TGSTREAM_ALREADY_OPEN	= -3,
	TGSTREAM_READ_FAILED	= -4,
	TGSTREAM_WRITE_FAILED	= -5,
	TGSTREAM_SEEK_FAILED	= -6
};

/*
===============================================================================

	TGStreamException

	What a stream throws. 0x14 bytes; only the first two members are ever
	written.

===============================================================================
*/

class TGStreamException {
public:
					TGStreamException( int code );
					~TGStreamException( void );

	int				code;			// 0x00 a tgStreamError_t
	int				unknown04;		// 0x04
	int				unknown08;		// 0x08
	int				unknown0C;		// 0x0C
	int				unknown10;		// 0x10
};

// The 16-bit length codec of the compressed vectors, in the unit after
// TGStream's (0x6D3290..); its owner and names are unknown.
unsigned short		Unknown6D3A90( float length );
float				Unknown6D3B30( unsigned short packed );

/*
===============================================================================

	TGStream

	0x1C bytes.

===============================================================================
*/

class TGStream {
public:
					TGStream( void );
					~TGStream( void );

	virtual void	VirtualSlot00( char *fileName, char *mode );
	virtual void	Close( void );
	virtual void	SetWriteMode( int writeMode );
	virtual int		GetWriteMode( void );
	virtual void	Read( void *buffer, int size );
	virtual void	Write( void *buffer, int size );
	virtual void	ReadCLine( char *buffer, int size, char terminator );
	virtual void	WriteCLine( char *string );
	virtual void	ReadCString( char *buffer, int size );
	virtual void	WriteCString( char *string );
	virtual char *	VirtualSlot10( int maxLength );
	virtual void	VirtualSlot11( char *string );
	virtual void	ReadCWLine( unsigned short *buffer, int size, unsigned short terminator );
	virtual void	WriteCWLine( unsigned short *string );
	virtual short	ReadWChar( void );
	virtual void	WriteWChar( unsigned short c );
	virtual long	VirtualSlot16( void );
	virtual void	VirtualSlot17( long value );
	virtual bool	ReadBool( void );
	virtual void	WriteBool( bool value );
	virtual char	ReadChar( void );
	virtual void	WriteChar( char value );
	virtual short	ReadShort( void );
	virtual void	WriteShort( short value );
	virtual int		ReadInt( void );
	virtual void	WriteInt( int value );
	virtual long	ReadLong( void );
	virtual void	WriteLong( long value );
	virtual float	ReadFloat( void );
	virtual void	WriteFloat( float value );
	virtual double	ReadDouble( void );
	virtual void	WriteDouble( double value );
	virtual long	ReadID( void );
	virtual void	WriteID( long id );
	virtual void	VirtualSlot34( float x, float y, float z );
	virtual void	VirtualSlot35( float *x, float *y, float *z );
	virtual void	VirtualSlot36( float x, float y, float z, bool packedLength );
	virtual void	VirtualSlot37( float *x, float *y, float *z, bool packedLength );
	virtual void	VirtualSlot38( float x, float y, float z, float maxLength );
	virtual void	VirtualSlot39( float *x, float *y, float *z, float maxLength );
	virtual void	VirtualSlot40( float x, float y, float z, char *outX, char *outY, char *outZ, float *outLength );
	virtual void	VirtualSlot41( float x, float y, float z, char *outX, char *outY, char *outZ, unsigned short *outLength );
	virtual void	VirtualSlot42( float x, float y, float z, char *outX, char *outY, char *outZ );
	virtual void	VirtualSlot43( float x, float y, float z, char *outX, char *outY, char *outZ, unsigned char *outLength, float maxLength );
	virtual void	VirtualSlot44( float *outX, float *outY, float *outZ, char x, char y, char z, float length );
	virtual void	VirtualSlot45( float *outX, float *outY, float *outZ, char x, char y, char z, unsigned short length );
	virtual void	VirtualSlot46( float *outX, float *outY, float *outZ, char x, char y, char z );
	virtual void	VirtualSlot47( float *outX, float *outY, float *outZ, char x, char y, char z, unsigned char length, float maxLength );
	virtual void	VirtualSlot48( float maxLength );
	virtual void	VirtualSlot49( void );
	virtual void	VirtualSlot50( void );
	virtual float	VirtualSlot51( void );
	virtual float	VirtualSlot52( void );
	virtual float	VirtualSlot53( void );
	virtual long	GetPos( void );
	virtual void	VirtualSlot55( long offset, int origin );
	virtual int		GetLength( void );
	virtual bool	Eof( void );
	virtual int		VirtualSlot58( void );
	virtual void	VirtualSlot59( void );

protected:
	void			Fail( int code );
	void			PutWChar( unsigned short c );
	unsigned short	GetWChar( void );

	TGStreamException *	exception;		// 0x04 thrown on every failure
	FILE *			file;			// 0x08
	int				writeMode;		// 0x0C
	float			unknown10;		// 0x10 the vector slots 48-53 read into
	float			unknown14;		// 0x14
	float			unknown18;		// 0x18
};

/*
================
TGStream::Fail

Records code in the stream's exception and throws it.
================
*/
inline void TGStream::Fail( int code ) {
	exception->code = code;
	throw exception;
}

/*
================
TGStream::PutWChar

Writes one wide character, unchecked.
================
*/
inline void TGStream::PutWChar( unsigned short c ) {
	fwrite( &c, sizeof( c ), 1, file );
}

/*
================
TGStream::GetWChar

Reads one wide character, unchecked.
================
*/
inline unsigned short TGStream::GetWChar( void ) {
	unsigned short c;
	fread( &c, sizeof( c ), 1, file );
	return c;
}

/*
================
TGStream::VirtualSlot16
================
*/
// FUNCTION: STBC 0x006cf010 @game/TGBufferStream
// FUNCTION: STBC 0x006d3310 @game/TGBufferedFileStream
inline long TGStream::VirtualSlot16( void ) {
	return ReadLong();
}

/*
================
TGStream::VirtualSlot17
================
*/
// FUNCTION: STBC 0x006cf020 @game/TGBufferStream
// FUNCTION: STBC 0x006d3320 @game/TGBufferedFileStream
inline void TGStream::VirtualSlot17( long value ) {
	WriteLong( value );
}

/*
================
TGStream::ReadBool
================
*/
// FUNCTION: STBC 0x006d2030 @game/TGStream
// FUNCTION: STBC 0x006d3330 @game/TGBufferedFileStream
inline bool TGStream::ReadBool( void ) {
	return ReadChar() ? true : false;
}

/*
================
TGStream::WriteBool
================
*/
// FUNCTION: STBC 0x006d2040 @game/TGStream
// FUNCTION: STBC 0x006d3340 @game/TGBufferedFileStream
inline void TGStream::WriteBool( bool value ) {
	WriteChar( value );
}

/*
================
TGStream::VirtualSlot48

Reads a vector of slot 39 into unknown10..unknown18.
================
*/
// FUNCTION: STBC 0x006cf030
inline void TGStream::VirtualSlot48( float maxLength ) {
	VirtualSlot39( &unknown10, &unknown14, &unknown18, maxLength );
}

/*
================
TGStream::VirtualSlot49

Reads a vector of slot 35 into unknown10..unknown18.
================
*/
// FUNCTION: STBC 0x006cf050
inline void TGStream::VirtualSlot49( void ) {
	VirtualSlot35( &unknown10, &unknown14, &unknown18 );
}

/*
================
TGStream::VirtualSlot50

Reads a vector of slot 37 into unknown10..unknown18.
================
*/
// FUNCTION: STBC 0x006cf070
inline void TGStream::VirtualSlot50( void ) {
	VirtualSlot37( &unknown10, &unknown14, &unknown18, false );
}

/*
================
TGStream::VirtualSlot51
================
*/
// FUNCTION: STBC 0x006cf090
inline float TGStream::VirtualSlot51( void ) {
	return unknown10;
}

/*
================
TGStream::VirtualSlot52
================
*/
// FUNCTION: STBC 0x006cf0a0
inline float TGStream::VirtualSlot52( void ) {
	return unknown14;
}

/*
================
TGStream::VirtualSlot53
================
*/
// FUNCTION: STBC 0x006cf0b0
inline float TGStream::VirtualSlot53( void ) {
	return unknown18;
}

#endif /* !__TGSTREAM_H__ */
