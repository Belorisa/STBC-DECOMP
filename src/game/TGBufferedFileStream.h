/*
===============================================================================

	TGBufferedFileStream.h

	A TGStream that reads and writes through a buffer of its own: the file is
	read a buffer at a time, or written when the buffer fills, and reads and
	writes that do not fit the buffer go straight to the file. The save game
	writes through one (the constructor takes the buffer size, 1 MB there).

	0x30 bytes, vtable 0x895E58 (the constructor, 0x6D32D0, is the only code
	that stores it). The class name is invented: no string names it. Slots 60
	and 61 are its own (TGStream ends at 59). A file opened with a 'w' or 'W'
	in its mode is a writing stream; any other is a reading stream.

===============================================================================
*/

#ifndef __TGBUFFEREDFILESTREAM_H__
#define __TGBUFFEREDFILESTREAM_H__

#include "TGStream.h"

class TGBufferedFileStream : public TGStream {		// invented name
public:
					TGBufferedFileStream( int bufferSize_ );

	// TGStream
	virtual void	VirtualSlot00( char *fileName, char *mode );
	virtual void	Close( void );
	virtual void	Read( void *data, int size );
	virtual void	Write( void *data, int size );
	virtual void	ReadCLine( char *string, int size, char terminator );
	virtual void	WriteCLine( char *string );
	virtual void	ReadCString( char *string, int size );
	virtual void	WriteCString( char *string );
	virtual void	ReadCWLine( unsigned short *string, int size, unsigned short terminator );
	virtual void	WriteCWLine( unsigned short *string );
	// FUNCTION: STBC 0x006d3740 ?ReadWChar@TGBufferedFileStream@@UAEFXZ @game/TGBufferedFileStream
	virtual short	ReadWChar( void ) { return ReadShort(); }
	// FUNCTION: STBC 0x006d3880 ?WriteWChar@TGBufferedFileStream@@UAEXG@Z @game/TGBufferedFileStream
	virtual void	WriteWChar( unsigned short c ) { WriteShort( c ); }
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
	virtual bool	Eof( void );

	// new slots
	virtual void	SetBufferSize( int size );		// invented name
	// FUNCTION: STBC 0x006d3300 ?GetBufferSize@TGBufferedFileStream@@UAEHXZ @game/TGBufferedFileStream
	virtual int		GetBufferSize( void ) { return bufferSize; }		// invented name

protected:
	void			Flush( void );
	void			Refill( void );

	bool			writing;		// 0x1C opened with a 'w' or 'W' in its mode
	char *			buffer;			// 0x20 bufferSize + 1 bytes
	int				bufferSize;		// 0x24
	long			filePos;		// 0x28 where the buffer starts in the file
	int				bufferPos;		// 0x2C the next byte of the buffer
};

#endif /* !__TGBUFFEREDFILESTREAM_H__ */
