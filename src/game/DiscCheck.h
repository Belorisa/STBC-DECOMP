/*
===============================================================================

	DiscCheck.h

	The disc check behind CDCheckWindow: "texture.dat" in the program's
	directory holds, XORed with a 400-byte key, a version tag and the table
	of contents of the game disc's audio session, which is compared with
	the discs in the drives (0x460420 to 0x460AA0, in BezierCurve's unit;
	every name here is invented).

	Evidence: CDCheckWindow::RunCheck (0x4FC74C) calls 0x4607F0 with NULL
	and passes the check on 0; 0x4607F0 reads "texture.dat" through
	0x460660 (GetModuleFileName, CreateFile, ReadFile, then the XOR of
	0x460420 with the key 0x44C1A0 returns) and hands the table to
	0x460880, which asks the MCI cdaudio device of each drive for its
	tracks (0x460460, 0x460500).

===============================================================================
*/

#ifndef __DISCCHECK_H__
#define __DISCCHECK_H__

// A disc's table of contents as "texture.dat" stores it: the count of
// dwords (the count's own included), then the table. 128 dwords: the copy
// FindDiscWithContents reads a drive into takes 0x200 bytes of its frame,
// and the tagged form below (1 + 7 + 128 dwords) is the file's 136.
struct DiscContents {
	unsigned int	count;
	unsigned int	table[127];
};

// The decoded "texture.dat", 136 dwords: the table directly, or, when the
// data starts with a version tag, the tag and then the table.
union DiscData {
	DiscContents	contents;
	struct {
		unsigned int	count;
		char			version[28];	// "V1.1" style
		DiscContents	contents;
	}				tagged;
};

// The key the disc data is XORed with: 400 bytes (FloatRangeWatcher's unit,
// 0x44C1A0; invented name).
unsigned char *	GetDiscDataKey( void );

// XORs the buffer with the key, repeating it every 400 bytes.
void			DecodeDiscData( unsigned char *buffer, unsigned int size );

// Reads the file (relative to the program's directory unless it has a
// drive or a UNC prefix) into data, which must hold it: its size in dwords
// goes first, over the file's first dword, and the rest is decoded.
// NULL when the file can't be read or its size isn't a positive multiple
// of 4.
DiscData *		ReadDiscData( DiscData *data, const char *fileName );

// The version tag of the data ("V1.1" style, at the fifth byte), or NULL
// when it has none.
char *			GetDiscDataVersion( DiscData *data );

// Reads the table of contents of the audio disc in the drive (its letter)
// through the MCI cdaudio device into contents: 0, or an error code
// (0x460460; invented name).
int				ReadDriveContents( DiscContents *contents, char drive );

// Reads the open cdaudio device's track lengths into contents, after the
// count of tracks (at most 128), and checks the disc's length: 0, or an
// error code (0x460500; invented name).
int				ReadDiscTracks( DiscContents *contents, MCIDEVICEID device );

// Compares the table of contents in the data (its dword count, then the
// table) with the discs in the drives: 0 when one matches, else an error
// code (0x460880).
int				FindDiscWithContents( DiscContents *contents );

// The check: 0 when a drive holds the disc "texture.dat" describes, 2 when
// the file can't be read, 3 when no drive does. The data's version tag goes
// into version (30 characters) when it isn't NULL.
char			CheckDisc( char *version );

#endif /* !__DISCCHECK_H__ */
