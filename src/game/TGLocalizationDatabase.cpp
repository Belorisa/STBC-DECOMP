/*
===============================================================================

	TGLocalizationDatabase.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGLocalizationDatabase.h"
#include "TGHandler.h"
#include "TGLocalizationManager.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d0890 _$E32
// FUNCTION: STBC 0x006d08a0 _$E29
// FUNCTION: STBC 0x006d08b0 _$E31
// FUNCTION: STBC 0x006d08c0 _$E30
// FUNCTION: STBC 0x006d08d0 _$E38
// FUNCTION: STBC 0x006d08e0 _$E35
// FUNCTION: STBC 0x006d08f0 _$E37
// FUNCTION: STBC 0x006d0900 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d0910 _$E75
// FUNCTION: STBC 0x006d0920 _$E74
// FUNCTION: STBC 0x006d0930 _$E79
// FUNCTION: STBC 0x006d0940 _$E78
// FUNCTION: STBC 0x006d0950 _$E90
// FUNCTION: STBC 0x006d0960 _$E89
// FUNCTION: STBC 0x006d0970 _$E94
// FUNCTION: STBC 0x006d0980 _$E93
// FUNCTION: STBC 0x006d0990 _$E98
// FUNCTION: STBC 0x006d09a0 _$E97
// FUNCTION: STBC 0x006d09b0 _$E102
// FUNCTION: STBC 0x006d09c0 _$E101
// FUNCTION: STBC 0x006d09d0 _$E226
// FUNCTION: STBC 0x006d09e0 _$E225
// FUNCTION: STBC 0x006d09f0 _$E168
// FUNCTION: STBC 0x006d0a00 _$E167
// FUNCTION: STBC 0x006d0a10 _$E181
// FUNCTION: STBC 0x006d0a20 _$E180
// FUNCTION: STBC 0x006d0a30 _$E231
// FUNCTION: STBC 0x006d0a40 _$E230
// FUNCTION: STBC 0x006d0a50 _$E236
// FUNCTION: STBC 0x006d0a60 _$E235
// FUNCTION: STBC 0x006d0a70 _$E240
// FUNCTION: STBC 0x006d0a80 _$E239
// FUNCTION: STBC 0x006d0a90 _$E244
// FUNCTION: STBC 0x006d0aa0 _$E243
// FUNCTION: STBC 0x006d0ab0 _$E251
// FUNCTION: STBC 0x006d0ac0 _$E250
// FUNCTION: STBC 0x006d0ad0 _$E276
// FUNCTION: STBC 0x006d0ae0 _$E275
// FUNCTION: STBC 0x006d0af0 _$E280
// FUNCTION: STBC 0x006d0b00 _$E279
// FUNCTION: STBC 0x006d0b10 _$E126
// FUNCTION: STBC 0x006d0b20 _$E125
// FUNCTION: STBC 0x006d0b30 _$E262
// FUNCTION: STBC 0x006d0b40 _$E261
// FUNCTION: STBC 0x006d0b50 _$E269
// FUNCTION: STBC 0x006d0b60 _$E268
// FUNCTION: STBC 0x006d0b70 _$E292
// FUNCTION: STBC 0x006d0b80 _$E291
// FUNCTION: STBC 0x006d0b90 _$E296
// FUNCTION: STBC 0x006d0ba0 _$E295
// FUNCTION: STBC 0x006d0bb0 _$E336
// FUNCTION: STBC 0x006d0bc0 _$E335
// FUNCTION: STBC 0x006d0bd0 _$E342
// FUNCTION: STBC 0x006d0be0 _$E341
// FUNCTION: STBC 0x006d0bf0 _$E284
// FUNCTION: STBC 0x006d0c00 _$E283
// FUNCTION: STBC 0x006d0c10 _$E288
// FUNCTION: STBC 0x006d0c20 _$E287
// FUNCTION: STBC 0x006d0c30 _$E300
// FUNCTION: STBC 0x006d0c40 _$E299
// FUNCTION: STBC 0x006d0c50 _$E304
// FUNCTION: STBC 0x006d0c60 _$E303
// FUNCTION: STBC 0x006d0c70 _$E308
// FUNCTION: STBC 0x006d0c80 _$E307
// FUNCTION: STBC 0x006d0c90 _$E312
// FUNCTION: STBC 0x006d0ca0 _$E311
// FUNCTION: STBC 0x006d0cb0 _$E316
// FUNCTION: STBC 0x006d0cc0 _$E315
// FUNCTION: STBC 0x006d0cd0 _$E320
// FUNCTION: STBC 0x006d0ce0 _$E319
// FUNCTION: STBC 0x006d0cf0 _$E324
// FUNCTION: STBC 0x006d0d00 _$E323
// FUNCTION: STBC 0x006d0d10 _$E328
// FUNCTION: STBC 0x006d0d20 _$E327
// FUNCTION: STBC 0x006d0d30 _$E131
// FUNCTION: STBC 0x006d0d40 _$E130
// FUNCTION: STBC 0x006d0d50 _$E332
// FUNCTION: STBC 0x006d0d60 _$E331
// FUNCTION: STBC 0x006d0d70 _$E136
// FUNCTION: STBC 0x006d0d80 _$E135
// FUNCTION: STBC 0x006d0d90 _$E346
// FUNCTION: STBC 0x006d0da0 _$E345
// FUNCTION: STBC 0x006d0db0 _$E350
// FUNCTION: STBC 0x006d0dc0 _$E349
// FUNCTION: STBC 0x006d0dd0 _$E354
// FUNCTION: STBC 0x006d0de0 _$E353
// FUNCTION: STBC 0x006d0df0 _$E358
// FUNCTION: STBC 0x006d0e00 _$E357
// FUNCTION: STBC 0x006d0e10 _$E145
// FUNCTION: STBC 0x006d0e20 _$E144
// FUNCTION: STBC 0x006d0e30 _$E362
// FUNCTION: STBC 0x006d0e40 _$E361
// FUNCTION: STBC 0x006d0e50 _$E366
// FUNCTION: STBC 0x006d0e60 _$E365
// FUNCTION: STBC 0x006d0e70 _$E370
// FUNCTION: STBC 0x006d0e80 _$E369
// FUNCTION: STBC 0x006d0e90 _$E151
// FUNCTION: STBC 0x006d0ea0 _$E150
// FUNCTION: STBC 0x006d0eb0 _$E156
// FUNCTION: STBC 0x006d0ec0 _$E155
// FUNCTION: STBC 0x006d0ed0 _$E186
// FUNCTION: STBC 0x006d0ee0 _$E185
// FUNCTION: STBC 0x006d0ef0 _$E374
// FUNCTION: STBC 0x006d0f00 _$E373
// FUNCTION: STBC 0x006d0f10 _$E191
// FUNCTION: STBC 0x006d0f20 _$E190
// FUNCTION: STBC 0x006d0f30 _$E378
// FUNCTION: STBC 0x006d0f40 _$E377
// FUNCTION: STBC 0x006d0f50 _$E382
// FUNCTION: STBC 0x006d0f60 _$E381
// FUNCTION: STBC 0x006d0f70 _$E196
// FUNCTION: STBC 0x006d0f80 _$E195
// FUNCTION: STBC 0x006d0f90 _$E386
// FUNCTION: STBC 0x006d0fa0 _$E385
// FUNCTION: STBC 0x006d0fb0 _$E161
// FUNCTION: STBC 0x006d0fc0 _$E160
// FUNCTION: STBC 0x006d0fd0 _$E390
// FUNCTION: STBC 0x006d0fe0 _$E389
// FUNCTION: STBC 0x006d0ff0 _$E394
// FUNCTION: STBC 0x006d1000 _$E393
// FUNCTION: STBC 0x006d1010 _$E201
// FUNCTION: STBC 0x006d1020 _$E200
// FUNCTION: STBC 0x006d1030 _$E206
// FUNCTION: STBC 0x006d1040 _$E205
// FUNCTION: STBC 0x006d1050 _$E398
// FUNCTION: STBC 0x006d1060 _$E397
// FUNCTION: STBC 0x006d1070 _$E402
// FUNCTION: STBC 0x006d1080 _$E401
// FUNCTION: STBC 0x006d1090 _$E406
// FUNCTION: STBC 0x006d10a0 _$E405
// FUNCTION: STBC 0x006d10b0 _$E410
// FUNCTION: STBC 0x006d10c0 _$E409
// FUNCTION: STBC 0x006d10d0 _$E414
// FUNCTION: STBC 0x006d10e0 _$E413
// FUNCTION: STBC 0x006d10f0 _$E418
// FUNCTION: STBC 0x006d1100 _$E417
// FUNCTION: STBC 0x006d1110 _$E422
// FUNCTION: STBC 0x006d1120 _$E421
// FUNCTION: STBC 0x006d1130 _$E426
// FUNCTION: STBC 0x006d1140 _$E425
// FUNCTION: STBC 0x006d1150 _$E211
// FUNCTION: STBC 0x006d1160 _$E210
// FUNCTION: STBC 0x006d1170 _$E430
// FUNCTION: STBC 0x006d1180 _$E429
// FUNCTION: STBC 0x006d1190 _$E216
// FUNCTION: STBC 0x006d11a0 _$E215
// FUNCTION: STBC 0x006d11b0 _$E221
// FUNCTION: STBC 0x006d11c0 _$E220

/*
================
TGLocalizationDatabase::Create

The database of the file: the registered one when it is loaded already,
else a new one loaded from the file and registered, with "???" for a
missing string; NULL when the file does not load.
================
*/
// FUNCTION: STBC 0x006d11d0
TGLocalizationDatabase *TGLocalizationDatabase::Create( char *fileName, bool addUser ) {
	TGLocalizationDatabase *database = g_kLocalizationManager.GetIfRegistered( fileName, addUser );
	if ( database != NULL ) {
		return database;
	}
	database = new TGLocalizationDatabase;
	if ( database == NULL ) {
		return NULL;
	}
	if ( database->Load( fileName ) ) {
		g_kLocalizationManager.RegisterDatabase( database );
		database->missingString.SetString( "???" );
	} else {
		delete database;
		database = NULL;
	}
	return database;
}

/*
================
TGLocalizationDatabase::~TGLocalizationDatabase
================
*/
// FUNCTION: STBC 0x006d12c0
TGLocalizationDatabase::~TGLocalizationDatabase( void ) {
	if ( name != NULL ) {
		delete name;
	}
	if ( entries != NULL ) {
		delete[] entries;
	}
	if ( unknown18 != NULL ) {
		delete unknown18;
	}
}

/*
================
TGLocalizationDatabase::WriteSample

Writes a sample localization file of five strings: the header, the entries
(each the offsets of its key, its text in characters and its file name), then
the blocks of keys ("ID1" to "ID5"), texts ("STRING1" to "STRING5", wide) and
file names ("FILE1" to "FILE5").
================
*/
// FUNCTION: STBC 0x006d13c0 ?WriteSample@TGLocalizationDatabase@@SAXPAD@Z
void TGLocalizationDatabase::WriteSample( char *fileName ) {
	FILE *file = fopen( fileName, "wb" );

	int version = 0x1701;
	fwrite( &version, 4, 1, file );
	int unknown1 = 1;
	fwrite( &unknown1, 4, 1, file );
	int unknown2 = 0;
	fwrite( &unknown2, 4, 1, file );
	int count = 5;
	fwrite( &count, 4, 1, file );

	int *entries = new int[count * 3];
	entries[0] = 0;
	entries[1] = 0;
	entries[2] = 0;
	entries[3] = 4;
	entries[4] = 8;
	entries[5] = 6;
	entries[6] = 8;
	entries[7] = 16;
	entries[8] = 12;
	entries[9] = 12;
	entries[10] = 24;
	entries[11] = 18;
	entries[12] = 16;
	entries[13] = 32;
	entries[14] = 24;
	for ( unsigned int i = 0; i < (unsigned int)count; i++ ) {
		fwrite( &entries[i * 3], 4, 1, file );
		fwrite( &entries[i * 3 + 1], 4, 1, file );
		fwrite( &entries[i * 3 + 2], 4, 1, file );
	}

	int keySize = 20;
	fwrite( &keySize, 4, 1, file );
	char keys[20] = { 'I', 'D', '1', 0, 'I', 'D', '2', 0, 'I', 'D', '3', 0, 'I', 'D', '4', 0, 'I', 'D', '5', 0 };
	fwrite( keys, keySize, 1, file );

	int textSize = 40;
	fwrite( &textSize, 4, 1, file );
	unsigned short texts[40] = { 'S', 'T', 'R', 'I', 'N', 'G', '1', 0, 'S', 'T', 'R', 'I', 'N', 'G', '2', 0,
		'S', 'T', 'R', 'I', 'N', 'G', '3', 0, 'S', 'T', 'R', 'I', 'N', 'G', '4', 0, 'S', 'T', 'R', 'I', 'N', 'G', '5', 0 };
	fwrite( texts, textSize * 2, 1, file );

	int fileNameSize = 30;
	fwrite( &fileNameSize, 4, 1, file );
	char fileNames[30] = { 'F', 'I', 'L', 'E', '1', 0, 'F', 'I', 'L', 'E', '2', 0, 'F', 'I', 'L', 'E', '3', 0,
		'F', 'I', 'L', 'E', '4', 0, 'F', 'I', 'L', 'E', '5', 0 };
	fwrite( fileNames, fileNameSize, 1, file );
	fclose( file );
}

/*
================
TGLocalizationDatabase::Load

Reads the localization file (the format WriteSample writes).
================
*/
// FUNCTION: STBC 0x006d17e0
bool TGLocalizationDatabase::Load( char *fileName ) {
	FILE *file = fopen( fileName, "rb" );
	if ( file == NULL ) {
		return false;
	}
	if ( !ReadHeader( file, fileName ) ) {
		return false;
	}
	int *table = NULL;
	char *keys = NULL;
	unsigned short *texts = NULL;
	if ( !ReadBlocks( file, fileName, &table, &keys, &texts ) ) {
		return false;
	}
	fclose( file );
	bool result = BuildEntries( file, fileName, table, keys, texts );
	delete[] keys;
	delete[] texts;
	delete[] table;
	return result;
}

/*
================
TGLocalizationDatabase::ReadHeader

Keeps a copy of the file name and reads the header: the magic 0x1701, the
words 1 and 0, and a nonzero string count. Closes the file when it is not a
localization file.
================
*/
// FUNCTION: STBC 0x006d18b0
bool TGLocalizationDatabase::ReadHeader( FILE *file, char *fileName ) {
	name = new char[strlen( fileName ) + 1];
	strcpy( name, fileName );
	int magic;
	if ( fread( &magic, 1, 4, file ) == 4 && magic == 0x1701 &&
			fread( &unknown08, 4, 1, file ) == 1 && unknown08 == 1 &&
			fread( &unknown0C, 4, 1, file ) == 1 && unknown0C == 0 &&
			fread( &entryCount, 4, 1, file ) == 1 && entryCount != 0 ) {
		return true;
	}
	fclose( file );
	return false;
}

/*
================
TGLocalizationDatabase::ReadBlocks

Makes the entries and reads the entry table (three offsets per string), then
the blocks of keys, of wide texts and of file names, each after its size.
The file-name block is the database's own; the others are the caller's to
free.
================
*/
// FUNCTION: STBC 0x006d1980
bool TGLocalizationDatabase::ReadBlocks( FILE *file, char *fileName, int **table, char **keys, unsigned short **texts ) {
	entries = new TGLocalizedString[entryCount];
	*table = new int[entryCount * 3];
	if ( fread( *table, 12, entryCount, file ) != (unsigned int)entryCount ) {
		// BUG: the failure is not returned: the reads go on with the file closed and the blocks freed
		FailLoad( file, fileName, table, keys, texts );
	}
	unsigned int keySize;
	if ( fread( &keySize, 4, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	*keys = new char[keySize];
	if ( fread( *keys, keySize, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	int textSize;
	if ( fread( &textSize, 4, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	*texts = new unsigned short[textSize];
	if ( fread( *texts, textSize * 2, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	unsigned int fileNameSize;
	if ( fread( &fileNameSize, 4, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	unknown18 = new char[fileNameSize];
	if ( fread( unknown18, fileNameSize, 1, file ) != 1 ) {
		return FailLoad( file, fileName, table, keys, texts );
	}
	return true;
}

/*
================
TGLocalizationDatabase::FailLoad

Frees the caller's blocks, clears its pointers and closes the file.
================
*/
// FUNCTION: STBC 0x006d1bb0
bool TGLocalizationDatabase::FailLoad( FILE *file, char *fileName, int **table, char **keys, unsigned short **texts ) {
	if ( *table != NULL ) {
		delete[] *table;
	}
	if ( *keys != NULL ) {
		delete[] *keys;
	}
	if ( *texts != NULL ) {
		delete texts;	// BUG: frees the caller's pointer variable, not the text block
	}
	*keys = NULL;
	*table = NULL;
	*texts = NULL;
	fclose( file );
	return false;
}

/*
================
TGLocalizationDatabase::BuildEntries

Fills the entries from the table, kept sorted by key hash: an entry whose
hash is above the last one's is appended, any other is inserted at its
place. Fails on two keys of one hash.
================
*/
// FUNCTION: STBC 0x006d1c20
// DRAFT: register: retail keeps table in ebx and the entry offset in edi (ours swaps them) and advances table after the body, where ours folds the step into the first load
bool TGLocalizationDatabase::BuildEntries( FILE *file, char *fileName, int *table, char *keys, unsigned short *texts ) {
	entries[0].keyHash = TGHashString( keys + table[0] );
	entries[0].text.Unknown6F4CE0( texts + table[1] );
	entries[0].fileName = unknown18 + table[2];
	for ( unsigned int i = 1; i < (unsigned int)entryCount; i++ ) {
		table += 3;
		unsigned int lastHash = entries[i - 1].keyHash;
		if ( (unsigned int)TGHashString( keys + table[0] ) > lastHash ) {
			entries[i].keyHash = TGHashString( keys + table[0] );
			entries[i].text.Unknown6F4CE0( texts + table[1] );
			entries[i].fileName = unknown18 + table[2];
		} else {
			unsigned int index = FindInsertIndex( TGHashString( keys + table[0] ), i );
			unsigned int foundHash = entries[index].keyHash;
			if ( (unsigned int)TGHashString( keys + table[0] ) == foundHash ) {
				return false;
			}
			memmove( &entries[index + 1], &entries[index], ( i - index ) * sizeof( TGLocalizedString ) );
			entries[index].keyHash = TGHashString( keys + table[0] );
			entries[index].text.Unknown6F4CE0( texts + table[1] );
			entries[index].fileName = unknown18 + table[2];
		}
	}
	return true;
}

/*
================
TGLocalizationDatabase::FindInsertIndex

Where among the first count entries (sorted by hash) an entry of the hash
goes: a binary search down to two neighbours, the upper one.
================
*/
// FUNCTION: STBC 0x006d1dd0
unsigned int TGLocalizationDatabase::FindInsertIndex( unsigned int keyHash, unsigned int count ) {
	unsigned int low = 0;
	unsigned int high = count - 1;
	do {
		unsigned int middle = ( high + low ) >> 1;
		if ( keyHash > entries[middle].keyHash ) {
			low = middle;
		} else {
			high = middle;
		}
	} while ( high - low > 1 );
	return high;
}

/*
================
TGLocalizationDatabase::GetString

The string of the key; the database's empty string for a missing key.
================
*/
// FUNCTION: STBC 0x006d1e10 ?GetString@TGLocalizationDatabase@@QAEPAVTGString@@PAD@Z
TGString *TGLocalizationDatabase::GetString( char *key ) {
	if ( key == NULL ) {
		return &missingString;
	}
	int index = FindString( key );
	if ( index == -1 ) {
		return &missingString;
	}
	return &entries[index].text;
}

/*
================
TGLocalizationDatabase::HasString
================
*/
// FUNCTION: STBC 0x006d1e50 ?HasString@TGLocalizationDatabase@@QAE_NPAD@Z
bool TGLocalizationDatabase::HasString( char *key ) {
	return FindString( key ) != -1 ? true : false;
}

/*
================
TGLocalizationDatabase::GetFilename

The file the key's string was read from; NULL for a missing key.
================
*/
// FUNCTION: STBC 0x006d1e70 ?GetFilename@TGLocalizationDatabase@@QAEPADPAD@Z
char *TGLocalizationDatabase::GetFilename( char *key ) {
	int index = FindString( key );
	if ( index == -1 ) {
		return NULL;
	}
	return entries[index].fileName;
}

/*
================
TGLocalizationDatabase::FindString

Binary search of the entries by the hash of the key.
================
*/
// FUNCTION: STBC 0x006d1ea0
int TGLocalizationDatabase::FindString( char *key ) {
	unsigned int hash = TGHashString( key );
	unsigned int low = 0;
	unsigned int high = entryCount - 1;
	while ( high >= low && high != (unsigned int)-1 ) {
		unsigned int middle = ( high + low ) >> 1;
		unsigned int entryHash = entries[middle].keyHash;
		if ( hash == entryHash ) {
			return middle;
		}
		if ( hash < entryHash ) {
			high = middle - 1;
		} else {
			low = middle + 1;
		}
	}
	return -1;
}

/*
================
TGLocDBWrapperSerialize
================
*/
// FUNCTION: STBC 0x006d1ef0
bool TGLocDBWrapperSerialize( PyObject *instance, PyObject **state ) {
	TGLocalizationDatabase *database = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&database, "TGLocalizationDatabase" );
	if ( database == NULL ) {
		return false;
	}
	*state = PyString_FromString( database->GetName() );
	return true;
}

/*
================
TGLocDBWrapperUnserialize
================
*/
// FUNCTION: STBC 0x006d1f40
bool TGLocDBWrapperUnserialize( PyObject *instance, PyObject *state ) {
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( PyString_AsString( state ), false );
	if ( database == NULL ) {
		return false;
	}
	PyObject_SetAttrString( instance, "this", PyEmbed::NewPointerString( (unsigned int)database, "TGLocalizationDatabase" ) );
	return true;
}
