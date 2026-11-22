/*
===============================================================================

	TGSoundFile.h

	A sound file read into memory, shared by the sounds that use it: the
	cache files each by its name with a count of its users.

	0x10 bytes. Evidence: TGSound::Load (0x70B730) asks for the file of its
	name (0x70C7A0, which reads it on the first request) and hands the size
	and the image at +4 and +8 and the name at +0xC to NiSource::LoadSample;
	the constructor (0x70C6F0) sizes and reads the file through the Miles
	calls AIL_file_size and AIL_file_read and adds the size to the sounds'
	rough memory; the destructor (0x70C770) frees the image through
	AIL_mem_free_lock; the cache is a string map at 0x99B858 and the release
	(0x70C940) drops the file from it when its last user lets go. The names
	are invented: no string names the class.

===============================================================================
*/

#ifndef __TGSOUNDFILE_H__
#define __TGSOUNDFILE_H__

#include "NiTStringMap.h"

class TGSoundFile {
public:
	// Reads the file; the image is NULL when it cannot be.
	TGSoundFile( char *fileName_ );
	~TGSoundFile( void );

	// The file of the name with a user added; with create set, read on the
	// first request. NULL when there is none.
	static TGSoundFile *	Get( char *fileName, bool create );
	// A user lets go of the file of the name; the last one frees it.
	static void		Release( char *fileName, TGSoundFile *file );

	int				users;				// 0x00
	unsigned int	size;				// 0x04 of the image
	void *			image;				// 0x08 as AIL_file_read gives it
	char *			fileName;			// 0x0C owned copy

protected:
	// Every file read, by name.
	static NiTStringMap<TGSoundFile *>	files;
};

#endif /* !__TGSOUNDFILE_H__ */
