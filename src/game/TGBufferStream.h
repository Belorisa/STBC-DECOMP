/*
===============================================================================

	TGBufferStream.h

	A TGStream over a memory buffer. Network messages are built and read
	with it (TGMessage::GetBufferStream). Unlike the file stream it doesn't
	throw: a failure only records its code in the stream's exception.

	Bools are packed: up to five share a byte, whose low five bits hold the
	values and whose top three bits count them.

	The SWIG wrappers TGBufferStream_* name the slots they call; the others
	keep TGStream's names. 0x30 bytes (new_TGBufferStream allocates 0x30).

===============================================================================
*/

#ifndef __TGBUFFERSTREAM_H__
#define __TGBUFFERSTREAM_H__

#include "TGStream.h"

// The most bools one byte holds.
#define MAX_PACKED_BOOLS	5

class TGBufferStream : public TGStream {
public:
					TGBufferStream( void );
					~TGBufferStream( void );

	// TGStream
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
	virtual void	ReadCWLine( unsigned short *buffer, int size, unsigned short terminator );
	virtual void	WriteCWLine( unsigned short *string );
	virtual short	ReadWChar( void );
	virtual void	WriteWChar( unsigned short c );
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
	virtual long	GetPos( void );
	virtual void	VirtualSlot55( long offset, int origin );
	virtual int		GetLength( void );
	virtual bool	Eof( void );

	// new slots
	virtual void	VirtualSlot60( char *data, int size );
	virtual char *	GetBuffer( void );
	virtual void	OpenBuffer( int size );
	virtual void	CloseBuffer( void );

protected:
	char *			buffer;			// 0x1C
	int				size;			// 0x20
	int				position;		// 0x24
	int				boolPosition;	// 0x28 the byte the packed bools go to
	unsigned char	boolMask;		// 0x2C the next bool's bit; 0 starts a new byte
};

#endif /* !__TGBUFFERSTREAM_H__ */
