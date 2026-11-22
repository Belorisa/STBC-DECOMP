/*
===============================================================================

	NiBinaryStream.h

	The abstract byte stream a NiStream reads objects from and writes them to.
	NiFile is the disk implementation.

	Evidence: NiBinaryStream's vtable (0x89C4E0) holds the scalar deleting
	destructor (0x86EC20) and three _purecall slots; NiFile's (0x89C4D0)
	fills them with the good-state test (0x86E690), a read (0x86E740) and a
	write (0x86E860). GetLine (0x86EC40) and PutS (0x86ECB0) follow the
	destructor (0x86EC10). NiColor::LoadBinary calls slot 2 with a buffer and a
	byte count; NiColor::SaveBinary calls slot 3 the same way.

===============================================================================
*/

#ifndef __NIBINARYSTREAM_H__
#define __NIBINARYSTREAM_H__

// FUNCTION: STBC 0x86EC20 ??_GNiBinaryStream@@UAEPAXI@Z @netimmerse/NiBinaryStream
class NiBinaryStream {
public:
	virtual					~NiBinaryStream();

	virtual					operator bool() const = 0;
	virtual unsigned int	Read( void *buffer, unsigned int bytes ) = 0;
	virtual unsigned int	Write( const void *buffer, unsigned int bytes ) = 0;

	// reads up to a newline, the end of the stream or maxBytes - 1 bytes, and
	// terminates the line; returns the bytes read (Gamebryo lineage names)
	unsigned int			GetLine( char *buffer, unsigned int maxBytes );
	// writes a string without its terminator; returns the bytes written
	unsigned int			PutS( const char *buffer );
};

#endif /* !__NIBINARYSTREAM_H__ */
