/*
===============================================================================

	TGLocalizationDatabase.h

	The strings of a localization file, found by name. Evidence: the SWIG
	wrappers TGLocalizationDatabase_* name the members; Create (0x6D11D0)
	allocates 0x2C bytes and constructs one in place.

	Layout (from Create's constructor and the lookups): the file name at
	0x04, an array of entries at 0x14 with its count at 0x18, and the
	string that stands for a missing one at 0x1C. The entry and member
	names other than those of the SWIG wrappers are invented.

===============================================================================
*/

#ifndef __TGLOCALIZATIONDATABASE_H__
#define __TGLOCALIZATIONDATABASE_H__

#include <stdio.h>
#include "TGString.h"

// A string of the database with the name it is found by and the file it
// came from (invented name). Its implicit constructor and destructor are the
// ones the loader's array new hands the CRT.
// FUNCTION: STBC 0x006d1b90 ??0TGLocalizedString@@QAE@XZ
// FUNCTION: STBC 0x006d13b0 ??1TGLocalizedString@@QAE@XZ
// FUNCTION: STBC 0x006d1350 ??_ETGLocalizedString@@QAEPAXI@Z
struct TGLocalizedString {
	unsigned int	keyHash;		// 0x00 TGHashString of the name; the entries are sorted by it
	TGString	text;			// 0x04
	char *		fileName;		// 0x14 what GetFilename returns
};

class TGLocalizationDatabase {
public:
	// Create's empty database, built in place (invented spelling).
					TGLocalizationDatabase( void ) : missingString( "" ) { name = NULL; entries = NULL; unknown18 = NULL; registered = false; }
	~TGLocalizationDatabase( void );

	static TGLocalizationDatabase *	Create( char *, bool );
	TGString *	GetString( char * );
	bool	HasString( char * );
	char *	GetFilename( char * );
	static void	WriteSample( char * );

	char *	GetName( void ) { return name; }		// invented name

protected:
	friend class TGLocalizationManager;

	// The index of the entry with the key; -1 when there is none.
	int		FindString( char *key );

	// The loader (invented names): Load opens the file and runs the steps;
	// ReadHeader checks the header and keeps the file name; ReadBlocks reads
	// the entry table and the key, text and file-name blocks; BuildEntries
	// fills the entries sorted by key hash, placing each with
	// FindInsertIndex; FailLoad frees the blocks and closes the file.
	bool	Load( char *fileName );
	bool	ReadHeader( FILE *file, char *fileName );
	bool	ReadBlocks( FILE *file, char *fileName, int **table, char **keys, unsigned short **texts );
	bool	FailLoad( FILE *file, char *fileName, int **table, char **keys, unsigned short **texts );
	bool	BuildEntries( FILE *file, char *fileName, int *table, char *keys, unsigned short *texts );
	unsigned int	FindInsertIndex( unsigned int keyHash, unsigned int count );

	bool				registered;		// 0x00 set by the manager's RegisterDatabase (invented name)
	char *				name;			// 0x04 the file the database was loaded from: the manager's lookup by name (0x6D03F0) compares it (invented name)
	int					unknown08;		// 0x08 the file's second header word, 1 (ReadHeader rejects others)
	int					unknown0C;		// 0x0C the file's third header word, 0 (ReadHeader rejects others)
	int					entryCount;		// 0x10
	TGLocalizedString *	entries;		// 0x14
	char *				unknown18;		// 0x18 a heap block, freed by the destructor
	TGString			missingString;	// 0x1C what GetString returns for an unknown key
};

// The save's handlers for the shadow instances: the state is the database's
// file name, which the load hands the manager again (the SWIG wrappers name
// them).
bool	TGLocDBWrapperSerialize( struct _object *instance, struct _object **state );
bool	TGLocDBWrapperUnserialize( struct _object *instance, struct _object *state );

#endif /* !__TGLOCALIZATIONDATABASE_H__ */
