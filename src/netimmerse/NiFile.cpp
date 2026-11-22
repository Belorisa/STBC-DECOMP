// NiFile.cpp -- buffered disk files

#include <new>
#include <string.h>
#include "NiRTLib.h"
#include "NiFile.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x0086e9f0 _$E33
// FUNCTION: STBC 0x0086ea10 _$E17
// FUNCTION: STBC 0x0086ea30 _$E15
// FUNCTION: STBC 0x0086ea40 _$E23
// FUNCTION: STBC 0x0086ea60 _$E21

// DATA: STBC 0x89C4C4 ?seekSet@NiFile@@2HB
const int NiFile::seekSet = SEEK_SET;
// DATA: STBC 0x89C4C8 ?seekCur@NiFile@@2HB
const int NiFile::seekCur = SEEK_CUR;
// DATA: STBC 0x89C4CC ?seekEnd@NiFile@@2HB
const int NiFile::seekEnd = SEEK_END;

/*
================
NiFile::NiFile

Opens the file; the buffer is allocated only for an open file.
================
*/
// FUNCTION: STBC 0x86E550
NiFile::NiFile( const char *name, OpenMode mode, unsigned int bufferSize ) {
	reading = ( mode == READ_ONLY );
	file = fopen( name, reading ? "rb" : "wb" );
	good = ( file != 0 );
	bufferAllocSize = bufferSize;
	bufferReadSize = 0;
	pos = 0;
	if ( good && bufferSize > 0 ) {
		buffer = new char[bufferSize];
	} else {
		buffer = 0;
	}
}

/*
================
NiFile::~NiFile
================
*/
// FUNCTION: STBC 0x86E620
NiFile::~NiFile() {
	if ( good ) {
		Flush();
		fclose( file );
	}
	delete [] buffer;
}

/*
================
NiFile::operator bool
================
*/
// FUNCTION: STBC 0x86E690 ??BNiFile@@UBE_NXZ
NiFile::operator bool() const {
	return good;
}

/*
================
NiFile::Seek
================
*/
// FUNCTION: STBC 0x86E6A0
void NiFile::Seek( int offset, int whence ) {
	if ( !good ) {
		return;
	}

	if ( whence == SEEK_CUR ) {
		int newPos = (int)pos + offset;
		if ( newPos >= 0 && newPos < (int)bufferReadSize ) {
			pos = newPos;
			return;
		}
		// the file is ahead of the buffer position by the unread bytes
		offset += pos - bufferReadSize;
	}

	Flush();
	good = ( fseek( file, offset, whence ) == 0 );
}

/*
================
NiFile::Read

From the buffer, refilling it as needed; a read larger than the buffer
goes straight to the disk. Returns the bytes read.
================
*/
// FUNCTION: STBC 0x86E740
unsigned int NiFile::Read( void *buffer_, unsigned int bytes ) {
	if ( good ) {
		unsigned int available = bufferReadSize - pos;
		unsigned int bytesRead = 0;
		if ( bytes > available ) {
			if ( available > 0 ) {
				memcpy( buffer_, buffer + pos, available );
				buffer_ = (char *)buffer_ + available;
				bytes -= available;
				bytesRead = available;
			}

			Flush();
			if ( bytes > bufferAllocSize ) {
				return bytesRead + DiskRead( buffer_, bytes );
			}

			bufferReadSize = DiskRead( buffer, bufferAllocSize );
			if ( bufferReadSize < bytes ) {
				bytes = bufferReadSize;
			}
		}

		memcpy( buffer_, buffer + pos, bytes );
		pos += bytes;
		return bytesRead + bytes;
	}
	return 0;
}

/*
================
NiFile::Write

Into the buffer, writing it out when full; a write as large as the
buffer goes straight to the disk. Returns the bytes written.
================
*/
// FUNCTION: STBC 0x86E860
unsigned int NiFile::Write( const void *buffer_, unsigned int bytes ) {
	if ( good ) {
		unsigned int available = bufferAllocSize - pos;
		unsigned int bytesWritten = 0;
		if ( bytes > available ) {
			if ( available > 0 ) {
				memcpy( buffer + pos, buffer_, available );
				buffer_ = (const char *)buffer_ + available;
				bytes -= available;
				bytesWritten = available;
				pos = bufferAllocSize;
			}

			if ( !Flush() ) {
				return 0;
			}
			if ( bytes >= bufferAllocSize ) {
				return bytesWritten + DiskWrite( buffer_, bytes );
			}
		}

		memcpy( buffer + pos, buffer_, bytes );
		pos += bytes;
		return bytesWritten + bytes;
	}
	return 0;
}

/*
================
NiFile::DiskWrite
================
*/
// FUNCTION: STBC 0x86E960
unsigned int NiFile::DiskWrite( const void *buffer_, unsigned int bytes ) {
	return fwrite( buffer_, 1, bytes, file );
}

/*
================
NiFile::DiskRead
================
*/
// FUNCTION: STBC 0x86E980
unsigned int NiFile::DiskRead( void *buffer_, unsigned int bytes ) {
	return fread( buffer_, 1, bytes, file );
}

/*
================
NiFile::Flush
================
*/
// FUNCTION: STBC 0x86E9A0
bool NiFile::Flush() {
	if ( reading ) {
		bufferReadSize = 0;
	} else if ( pos > 0 ) {
		if ( DiskWrite( buffer, pos ) != pos ) {
			good = false;
			return false;
		}
	}
	pos = 0;
	return true;
}
