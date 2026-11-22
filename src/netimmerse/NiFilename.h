/*
===============================================================================

	NiFilename.h

	A path split into its drive, directory, file name and extension, each
	of which can be replaced before the path is put back together.

	Evidence: the constructor (0x86EA70) allocates the four parts (the
	drive _MAX_DRIVE bytes, the others 256) and fills them with _splitpath;
	GetFullPath (0x86EB00) joins them with _makepath into the caller's
	buffer; the destructor (0x86EAD0) frees them. A static function
	(0x86EB20) returns _MAX_PATH, the size callers allocate for the joined
	path. NiStream's search-path lookup (0x818680) and NiImage's loader
	(0x7E0F6D) replace parts with inline copies. The class, member and
	method names follow the Gamebryo lineage, which keeps the class
	(m_acDir, SetDir, GetFullPath); GetMaxPathLength is invented.

===============================================================================
*/

#ifndef __NIFILENAME_H__
#define __NIFILENAME_H__

#include <string.h>

class NiFilename {
public:
							NiFilename( const char *fullPath );
							~NiFilename();

	void					SetDir( const char *dir_ ) { strcpy( dir, dir_ ); }
	void					SetDrive( const char *drive_ ) { strcpy( drive, drive_ ); }
	const char *			GetExt() const { return ext; }	// Gamebryo lineage name
	void					SetExt( const char *ext_ ) { strcpy( ext, ext_ ); }
	void					SetFilename( const char *fname_ ) { strcpy( fname, fname_ ); }

	// joins the parts into fullPath, GetMaxPathLength() bytes
	void					GetFullPath( char *fullPath ) const;

	static unsigned short	GetMaxPathLength();

	// _splitpath and _makepath on the caller's buffers; nothing calls them
	// and they leave the object alone (Gamebryo 1.2.2: NiFilename::Splitpath
	// and Makepath, which there fill and read the members)
	void					Splitpath( const char *path, char *drive_, char *dir_, char *fname_, char *ext_ );
	void					Makepath( char *path, const char *drive_, const char *dir_, const char *fname_, const char *ext_ ) const;

protected:
	char *					dir;
	char *					drive;
	char *					ext;
	char *					fname;
};

#endif /* !__NIFILENAME_H__ */
