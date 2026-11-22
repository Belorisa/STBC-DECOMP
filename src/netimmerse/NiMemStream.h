/*
===============================================================================

	NiMemStream.h

	A NiBinaryStream over a block of memory: read-only over a caller's
	buffer, or read-write over a buffer of its own that grows as it is
	written. NiStream loads from and saves to memory through it.

	Evidence: the vtable at 0x89C4F0 holds the scalar deleting destructor
	(0x86EDA0), the good-state test (0x86EE50), the read (0x86EE60) and the
	write (0x86EEA0). The constructors (0x86ED70 over a buffer, 0x86EDC0
	with 1024 bytes of its own) and the destructor (0x86EE20) give the
	layout. The class, member and method names are Gamebryo 1.1.2's
	(NiMemStream.h), which keeps the same layout.

===============================================================================
*/

#ifndef __NIMEMSTREAM_H__
#define __NIMEMSTREAM_H__

#include "NiBinaryStream.h"

class NiMemStream : public NiBinaryStream {
public:
	// a read-only stream over a caller's buffer
							NiMemStream( const void *buffer_, unsigned int size );
	// a read-write stream over a buffer of its own
							NiMemStream();
	virtual					~NiMemStream();

	virtual					operator bool() const;
	virtual unsigned int	Read( void *buffer_, unsigned int bytes );
	virtual unsigned int	Write( const void *buffer_, unsigned int bytes );

	// the buffer, which the stream then leaves to the caller
	void *					Str();
	void					Freeze( bool freeze_ );
	unsigned int			GetSize() const { return write; }

protected:
	char *					buffer;
	unsigned int			read;			// read position
	unsigned int			write;			// write position: the bytes held
	unsigned int			allocSize;		// 0 for a caller's buffer
	bool					userMemory;		// never set
	bool					freeze;			// the buffer belongs to the caller
};

#endif /* !__NIMEMSTREAM_H__ */
