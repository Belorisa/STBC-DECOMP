/*
===============================================================================

	TGDirectory.h

	A directory listing: the files of a directory that match a filter,
	sorted by name, and, read recursively, the subdirectories, each a
	listing of its own. The names are invented; no string names the
	classes.

	0x1C bytes, no vtable. Evidence: STFileMenu::Refresh (0x5211F0) and
	the other callers build one on the stack (the seven words cleared),
	call Read (0x71F270) and walk the files from 0x08 through each file's
	next link (0x08), reading its name (0x04); the destructor (0x71EED0)
	frees the files (0x71F010), deletes the next sibling (0x10) and the
	subdirectories (0x14) with the same destructor, then frees the name
	(0x00) and the path (0x04). AddFile (0x71F050) and AddSubdirectory
	(0x71F140) insert in name order and count (0x0C, 0x18). The scan
	(0x71F340) walks FindFirstFile/FindNextFile, saving and restoring the
	working directory through g_kSystemWrapper.

===============================================================================
*/

#ifndef __TGDIRECTORY_H__
#define __TGDIRECTORY_H__

/*
================
TGDirectoryFile

A file of a listing (invented name). 0x0C bytes, with a vtable of its
own (0x896B8C: the scalar deleting destructor 0x71F850).
================
*/
// FUNCTION: STBC 0x0071f850 ??_GTGDirectoryFile@@UAEPAXI@Z
class TGDirectoryFile {
public:
					TGDirectoryFile( void ) { name = NULL; next = NULL; }
	virtual			~TGDirectoryFile( void ) {
		if ( name ) {
			delete [] name;
		}
		next = NULL;
	}

	char *			name;					// 0x04 the file's name, a heap copy
	TGDirectoryFile *	next;				// 0x08 the next file by name
};

class TGDirectory {
public:
					TGDirectory( void ) { files = NULL; numFiles = 0; next = NULL; subdirectories = NULL; name = NULL; path = NULL; numSubdirectories = 0; }
					~TGDirectory( void );

	// Lists the directory path: its files matching filter (every file for
	// NULL) and, with recurse, its subdirectories'. False when a directory
	// could not be read.
	bool			Read( char *path, char *filter, bool recurse );
	// True when a file or directory of that name exists.
	bool			Exists( char *path );

	void			SetName( char *name );
	void			SetPath( char *path );
	void			ClearFiles( void );
	void			AddFile( TGDirectoryFile *file );
	void			AddSubdirectory( TGDirectory *directory );

	TGDirectoryFile *	GetFiles( void ) { return files; }
	char *			GetName( void ) { return name; }
	char *			GetPath( void ) { return path; }
	int				GetNumFiles( void ) { return numFiles; }
	int				GetNumSubdirectories( void ) { return numSubdirectories; }
	TGDirectory *	GetNext( void ) { return next; }
	TGDirectory *	GetSubdirectories( void ) { return subdirectories; }

protected:
	bool			Scan( char *path, char *filter, bool recurse );
	// Turns backslashes into slashes and drops a trailing line end and slash.
	void			FixPath( char *path );
	// pattern = "<directory>/<filter>", "*" for no filter.
	void			MakePattern( char *pattern, char *directory, char *filter );

	char *			name;					// 0x00 a heap copy, or NULL
	char *			path;					// 0x04 a heap copy, or NULL
	TGDirectoryFile *	files;				// 0x08 sorted by name
	int				numFiles;				// 0x0C
	TGDirectory *	next;					// 0x10 the next subdirectory of the parent, by name
	TGDirectory *	subdirectories;			// 0x14 sorted by name
	int				numSubdirectories;		// 0x18
};

#endif /* !__TGDIRECTORY_H__ */
