/*
===============================================================================

	NiFile.h

	The disk implementation of NiBinaryStream: a stdio file read or written
	through a buffer of its own.

	Evidence: the vtable at 0x89C4D0 holds the scalar deleting destructor
	(0x86E600), the good-state test (0x86E690), the read (0x86E740) and the
	write (0x86E860). The constructor (0x86E550) opens the file "rb" or
	"wb", allocates the buffer and clears the read size at 0x10 and the
	position at 0x18; the destructor (0x86E620) flushes a written buffer and
	closes the file. The member and method names follow the Gamebryo lineage,
	which keeps the class.

===============================================================================
*/

#ifndef __NIFILE_H__
#define __NIFILE_H__

#include <stdio.h>
#include "NiBinaryStream.h"

// FUNCTION: STBC 0x86E600 ??_GNiFile@@UAEPAXI@Z
class NiFile : public NiBinaryStream {
public:
	enum OpenMode {
		READ_ONLY,
		WRITE_ONLY
	};

							NiFile( const char *name, OpenMode mode, unsigned int bufferSize );
	virtual					~NiFile();

	virtual					operator bool() const;
	virtual unsigned int	Read( void *buffer_, unsigned int bytes );
	virtual unsigned int	Write( const void *buffer_, unsigned int bytes );

	// moves the file position as fseek does (SEEK_SET, SEEK_CUR, SEEK_END);
	// a move inside the read buffer keeps the buffer
	void					Seek( int offset, int whence );

	// writes out a written buffer, or drops a read one; false when the
	// write fails
	bool					Flush();

	// unbuffered transfers
	unsigned int			DiskRead( void *buffer_, unsigned int bytes );
	unsigned int			DiskWrite( const void *buffer_, unsigned int bytes );

	// _access on the name: 0 when the file is accessible in the mode
	static int				Access( const char *name, int mode );
	// brings a path to the platform's form; nothing to do on Win32
	static void				StandardizeFilePath( char **path );
	// the size of the named file, 0 when it cannot be found (name invented)
	static unsigned int		GetFileSize( const char *name );

	// the fseek origins as data, which the image format readers load (Gamebryo
	// lineage: ms_iSeekSet, ms_iSeekCur, ms_iSeekEnd)
	static const int		seekSet;
	static const int		seekCur;
	static const int		seekEnd;

protected:
	FILE *					file;
	char *					buffer;
	unsigned int			bufferAllocSize;	// the buffer's size
	unsigned int			bufferReadSize;		// the bytes read into the buffer
	unsigned int			unknown14;			// TODO identity: never read or written by the class
	unsigned int			pos;				// the position in the buffer
	bool					reading;			// opened READ_ONLY
	bool					good;				// opened, and no transfer has failed
};

#endif /* !__NIFILE_H__ */
