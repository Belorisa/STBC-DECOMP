/*
===============================================================================

	BezierCurve.cpp

	A curve through a list of control points (see BezierCurve.h).

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <new>

#include <mbstring.h>
#include <string.h>
#include <windows.h>

#include "BezierCurve.h"
#include "DiscCheck.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0045fe30 _$E424
// FUNCTION: STBC 0x0045fe50 _$E423

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0045fa50 _$E32
// FUNCTION: STBC 0x0045fa60 _$E29
// FUNCTION: STBC 0x0045fa70 _$E31
// FUNCTION: STBC 0x0045fa80 _$E30
// FUNCTION: STBC 0x0045fa90 _$E38
// FUNCTION: STBC 0x0045faa0 _$E35
// FUNCTION: STBC 0x0045fab0 _$E37
// FUNCTION: STBC 0x0045fac0 _$E36

// The disc data's key repeats every 400 bytes.
#define DISC_DATA_KEY_SIZE	400

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0045fad0 _$E50
// FUNCTION: STBC 0x0045fae0 _$E49
// FUNCTION: STBC 0x0045faf0 _$E54
// FUNCTION: STBC 0x0045fb00 _$E53
// FUNCTION: STBC 0x0045fb10 _$E65
// FUNCTION: STBC 0x0045fb20 _$E64
// FUNCTION: STBC 0x0045fb30 _$E69
// FUNCTION: STBC 0x0045fb40 _$E68
// FUNCTION: STBC 0x0045fb50 _$E73
// FUNCTION: STBC 0x0045fb60 _$E72
// FUNCTION: STBC 0x0045fb70 _$E77
// FUNCTION: STBC 0x0045fb80 _$E76
// FUNCTION: STBC 0x0045fb90 _$E196
// FUNCTION: STBC 0x0045fba0 _$E195
// FUNCTION: STBC 0x0045fbb0 _$E138
// FUNCTION: STBC 0x0045fbc0 _$E137
// FUNCTION: STBC 0x0045fbd0 _$E151
// FUNCTION: STBC 0x0045fbe0 _$E150
// FUNCTION: STBC 0x0045fbf0 _$E201
// FUNCTION: STBC 0x0045fc00 _$E200
// FUNCTION: STBC 0x0045fc10 _$E206
// FUNCTION: STBC 0x0045fc20 _$E205
// FUNCTION: STBC 0x0045fc30 _$E210
// FUNCTION: STBC 0x0045fc40 _$E209
// FUNCTION: STBC 0x0045fc50 _$E214
// FUNCTION: STBC 0x0045fc60 _$E213
// FUNCTION: STBC 0x0045fc70 _$E221
// FUNCTION: STBC 0x0045fc80 _$E220
// FUNCTION: STBC 0x0045fc90 _$E246
// FUNCTION: STBC 0x0045fca0 _$E245
// FUNCTION: STBC 0x0045fcb0 _$E250
// FUNCTION: STBC 0x0045fcc0 _$E249
// FUNCTION: STBC 0x0045fcd0 _$E96
// FUNCTION: STBC 0x0045fce0 _$E95
// FUNCTION: STBC 0x0045fcf0 _$E232
// FUNCTION: STBC 0x0045fd00 _$E231
// FUNCTION: STBC 0x0045fd10 _$E239
// FUNCTION: STBC 0x0045fd20 _$E238
// FUNCTION: STBC 0x0045fd30 _$E262
// FUNCTION: STBC 0x0045fd40 _$E261
// FUNCTION: STBC 0x0045fd50 _$E266
// FUNCTION: STBC 0x0045fd60 _$E265
// FUNCTION: STBC 0x0045fd70 _$E306
// FUNCTION: STBC 0x0045fd80 _$E305
// FUNCTION: STBC 0x0045fd90 _$E312
// FUNCTION: STBC 0x0045fda0 _$E311
// FUNCTION: STBC 0x0045fdb0 _$E254
// FUNCTION: STBC 0x0045fdc0 _$E253
// FUNCTION: STBC 0x0045fdd0 _$E258
// FUNCTION: STBC 0x0045fde0 _$E257
// FUNCTION: STBC 0x0045fdf0 _$E270
// FUNCTION: STBC 0x0045fe00 _$E269
// FUNCTION: STBC 0x0045fe10 _$E274
// FUNCTION: STBC 0x0045fe20 _$E273
// FUNCTION: STBC 0x0045fe60 _$E278
// FUNCTION: STBC 0x0045fe70 _$E277
// FUNCTION: STBC 0x0045fe80 _$E282
// FUNCTION: STBC 0x0045fe90 _$E281
// FUNCTION: STBC 0x0045fea0 _$E286
// FUNCTION: STBC 0x0045feb0 _$E285
// FUNCTION: STBC 0x0045fec0 _$E290
// FUNCTION: STBC 0x0045fed0 _$E289
// FUNCTION: STBC 0x0045fee0 _$E294
// FUNCTION: STBC 0x0045fef0 _$E293
// FUNCTION: STBC 0x0045ff00 _$E298
// FUNCTION: STBC 0x0045ff10 _$E297
// FUNCTION: STBC 0x0045ff20 _$E101
// FUNCTION: STBC 0x0045ff30 _$E100
// FUNCTION: STBC 0x0045ff40 _$E302
// FUNCTION: STBC 0x0045ff50 _$E301
// FUNCTION: STBC 0x0045ff60 _$E106
// FUNCTION: STBC 0x0045ff70 _$E105
// FUNCTION: STBC 0x0045ff80 _$E316
// FUNCTION: STBC 0x0045ff90 _$E315
// FUNCTION: STBC 0x0045ffa0 _$E320
// FUNCTION: STBC 0x0045ffb0 _$E319
// FUNCTION: STBC 0x0045ffc0 _$E324
// FUNCTION: STBC 0x0045ffd0 _$E323
// FUNCTION: STBC 0x0045ffe0 _$E328
// FUNCTION: STBC 0x0045fff0 _$E327
// FUNCTION: STBC 0x00460000 _$E115
// FUNCTION: STBC 0x00460010 _$E114
// FUNCTION: STBC 0x00460020 _$E332
// FUNCTION: STBC 0x00460030 _$E331
// FUNCTION: STBC 0x00460040 _$E336
// FUNCTION: STBC 0x00460050 _$E335
// FUNCTION: STBC 0x00460060 _$E340
// FUNCTION: STBC 0x00460070 _$E339
// FUNCTION: STBC 0x00460080 _$E121
// FUNCTION: STBC 0x00460090 _$E120
// FUNCTION: STBC 0x004600a0 _$E126
// FUNCTION: STBC 0x004600b0 _$E125
// FUNCTION: STBC 0x004600c0 _$E156
// FUNCTION: STBC 0x004600d0 _$E155
// FUNCTION: STBC 0x004600e0 _$E344
// FUNCTION: STBC 0x004600f0 _$E343
// FUNCTION: STBC 0x00460100 _$E161
// FUNCTION: STBC 0x00460110 _$E160
// FUNCTION: STBC 0x00460120 _$E348
// FUNCTION: STBC 0x00460130 _$E347
// FUNCTION: STBC 0x00460140 _$E352
// FUNCTION: STBC 0x00460150 _$E351
// FUNCTION: STBC 0x00460160 _$E166
// FUNCTION: STBC 0x00460170 _$E165
// FUNCTION: STBC 0x00460180 _$E356
// FUNCTION: STBC 0x00460190 _$E355
// FUNCTION: STBC 0x004601a0 _$E131
// FUNCTION: STBC 0x004601b0 _$E130
// FUNCTION: STBC 0x004601c0 _$E360
// FUNCTION: STBC 0x004601d0 _$E359
// FUNCTION: STBC 0x004601e0 _$E364
// FUNCTION: STBC 0x004601f0 _$E363
// FUNCTION: STBC 0x00460200 _$E171
// FUNCTION: STBC 0x00460210 _$E170
// FUNCTION: STBC 0x00460220 _$E176
// FUNCTION: STBC 0x00460230 _$E175
// FUNCTION: STBC 0x00460240 _$E368
// FUNCTION: STBC 0x00460250 _$E367
// FUNCTION: STBC 0x00460260 _$E372
// FUNCTION: STBC 0x00460270 _$E371
// FUNCTION: STBC 0x00460280 _$E376
// FUNCTION: STBC 0x00460290 _$E375
// FUNCTION: STBC 0x004602a0 _$E380
// FUNCTION: STBC 0x004602b0 _$E379
// FUNCTION: STBC 0x004602c0 _$E384
// FUNCTION: STBC 0x004602d0 _$E383
// FUNCTION: STBC 0x004602e0 _$E388
// FUNCTION: STBC 0x004602f0 _$E387
// FUNCTION: STBC 0x00460300 _$E392
// FUNCTION: STBC 0x00460310 _$E391
// FUNCTION: STBC 0x00460320 _$E396
// FUNCTION: STBC 0x00460330 _$E395
// FUNCTION: STBC 0x00460340 _$E181
// FUNCTION: STBC 0x00460350 _$E180
// FUNCTION: STBC 0x00460360 _$E400
// FUNCTION: STBC 0x00460370 _$E399
// FUNCTION: STBC 0x00460380 _$E186
// FUNCTION: STBC 0x00460390 _$E185
// FUNCTION: STBC 0x004603a0 _$E191
// FUNCTION: STBC 0x004603b0 _$E190
// FUNCTION: STBC 0x004603c0 _$E404
// FUNCTION: STBC 0x004603d0 _$E403
// FUNCTION: STBC 0x004603e0 _$E409
// FUNCTION: STBC 0x004603f0 _$E408
// FUNCTION: STBC 0x00460400 _$E413
// FUNCTION: STBC 0x00460410 _$E412

/*
================
DecodeDiscData
================
*/
// FUNCTION: STBC 0x00460420
void DecodeDiscData( unsigned char *buffer, unsigned int size ) {
	unsigned char *key = GetDiscDataKey();
	for ( unsigned int i = 0; i < size; i++ ) {
		buffer[i] ^= key[i % DISC_DATA_KEY_SIZE];
	}
}

/*
================
ReadDriveContents

Opens the MCI cdaudio device of the drive and reads the disc's tracks. The
error codes, here and in ReadDiscTracks, look like the original's source
line numbers.
================
*/
// FUNCTION: STBC 0x00460460
int ReadDriveContents( DiscContents *contents, char drive ) {
	MCI_OPEN_PARMS open;
	char element[16];

	memset( &open, 0, sizeof( open ) );
	open.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_CD_AUDIO;
	open.lpstrAlias = NULL;		// BUG: redundant: the memset cleared it
	element[0] = drive;
	element[1] = ':';
	element[2] = '\0';
	open.lpstrElementName = element;
	// BUG: redundant: the memset cleared both
	open.dwCallback = 0;
	open.wDeviceID = 0;
	if ( mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT | MCI_OPEN_SHAREABLE | MCI_WAIT,
						 (DWORD)&open ) != 0 ) {
		return 0xFA;
	}

	int result = ReadDiscTracks( contents, open.wDeviceID );
	mciSendCommand( open.wDeviceID, MCI_CLOSE, 0, 0 );
	return result;
}

/*
================
ReadDiscTracks

The tracks' lengths in milliseconds, a failed track's as 0; then the whole
disc's length must lie between 74.5 and 80.5 minutes.
================
*/
// FUNCTION: STBC 0x00460500
int ReadDiscTracks( DiscContents *contents, MCIDEVICEID device ) {
	MCI_SET_PARMS set;
	memset( &set, 0, sizeof( set ) );
	set.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
	if ( mciSendCommand( device, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&set ) != 0 ) {
		return 0x97;
	}

	MCI_STATUS_PARMS status;
	memset( &status, 0, sizeof( status ) );
	status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	if ( mciSendCommand( device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status ) != 0 ) {
		return 0xA1;
	}

	contents->count = status.dwReturn;
	if ( contents->count > 128 ) {
		return 0xA7;
	}

	for ( unsigned int track = 1; track <= contents->count; track++ ) {
		memset( &status, 0, sizeof( status ) );
		status.dwItem = MCI_STATUS_LENGTH;
		status.dwTrack = track;
		contents->table[track - 1] = mciSendCommand( device, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD)&status ) == 0
									 ? status.dwReturn : 0;
	}

	status.dwItem = MCI_STATUS_LENGTH;
	if ( mciSendCommand( device, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status ) != 0 ) {
		return 0xC0;
	}
	int length = status.dwReturn;
	if ( length < 4470000 ) {
		return 0xCC;
	}
	return length > 4830000 ? 0xD0 : 0;
}

/*
================
ReadDiscData
================
*/
// FUNCTION: STBC 0x00460660
DiscData *ReadDiscData( DiscData *data, const char *fileName ) {
	char path[MAX_PATH];
	if ( _mbschr( (const unsigned char *)fileName, ':' ) == NULL && strncmp( fileName, "\\\\", 2 ) != 0 ) {
		GetModuleFileName( NULL, path, MAX_PATH );
		*_mbsrchr( (unsigned char *)path, '\\' ) = '\0';
		strcat( path, "\\" );
		strcat( path, fileName );
	} else {
		strcpy( path, fileName );
	}

	HANDLE file = CreateFile( path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if ( file != INVALID_HANDLE_VALUE ) {
		DWORD size = GetFileSize( file, NULL );
		if ( size < 4 || ( size & 3 ) != 0 ) {
			CloseHandle( file );
		} else {
			DWORD read;
			ReadFile( file, data, size, &read, NULL );
			CloseHandle( file );
			if ( read >= size ) {
				data->contents.count = size / sizeof( unsigned int );
				DecodeDiscData( (unsigned char *)data->contents.table, size - sizeof( unsigned int ) );
				return data;
			}
		}
	}
	return NULL;
}

/*
================
GetDiscDataVersion
================
*/
// FUNCTION: STBC 0x004607d0
char *GetDiscDataVersion( DiscData *data ) {
	char *version = data->tagged.version;
	if ( version[0] != 'V' || version[3] != '.' ) {
		return NULL;
	}
	return version;
}

/*
================
CheckDisc
================
*/
// FUNCTION: STBC 0x004607f0
// PATCHED: retail's 0x460867 is xor eax, eax where the original's ? 3 : 0 has sbb eax, eax (neg; sbb; and 3): the patch throws FindDiscWithContents' result away, so the check returns 0 whenever "texture.dat" reads
char CheckDisc( char *version ) {
	DiscData data;
	if ( version != NULL ) {
		*version = '\0';
	}
	if ( ReadDiscData( &data, "texture.dat" ) == NULL ) {
		return 2;
	}
	char *tag = GetDiscDataVersion( &data );
	if ( tag != NULL ) {
		if ( version != NULL ) {
			strncpy( version, tag, 30 );
		}
		// the table follows the tag: its count leaves out the tag's 8 dwords
		data.tagged.contents.count = data.tagged.count - 8;
		return FindDiscWithContents( &data.tagged.contents ) != 0 ? 3 : 0;
	}
	return FindDiscWithContents( &data.contents ) != 0 ? 3 : 0;
}

/*
================
FindDiscWithContents

Closes the Windows CD Player, which holds the drives it plays, then reads
the table of contents of every CD-ROM drive and compares it with contents,
leaving out the first track's position: 0 when a drive holds the disc, else
an error code.
================
*/
// FUNCTION: STBC 0x00460880
// PATCHED: retail's bytes are test; nop; nop; mov eax, 0; return: the patch nopped the jne over the NULL check's return and wrote 0 over its error code, so the check always passes; the body below is retail's dead code after that return
int FindDiscWithContents( DiscContents *contents ) {
	if ( contents == NULL ) {
		// the original error code is lost: the patch wrote 0 over it
		return 1;
	}

	int tries = 0;
	HWND player = FindWindow( "SJE_CdPlayerClass", NULL );
	while ( player != NULL && tries < 100 ) {
		SendMessage( player, WM_CLOSE, 0, 0 );
		tries++;
		player = FindWindow( "SJE_CdPlayerClass", NULL );
	}

	char root[MAX_PATH];
	DiscContents disc;
	for ( int drive = 0; drive < 26; drive++ ) {
		root[0] = 'A' + drive;
		root[1] = ':';
		root[2] = '\\';
		root[3] = '\0';
		if ( GetDriveType( root ) != DRIVE_CDROM || ReadDriveContents( &disc, root[0] ) != 0 ) {
			continue;
		}
		int count = disc.count;
		if ( count != contents->count ) {
			continue;
		}
		int i;
		for ( i = 2; i < count; i++ ) {
			if ( disc.table[i - 1] != contents->table[i - 1] ) {
				break;
			}
		}
		if ( i == count ) {
			return 0;
		}
	}
	return 0x150;
}

// The user's "NoDriveTypeAutoRun" policy as DisableAutoRun found it; 0 until
// then (invented name).
// DATA: STBC 0x009810b4
static DWORD savedAutoRun;

/*
================
DisableAutoRun

Turns off AutoRun for every drive type in the user's Explorer policies,
keeping the setting it replaces for RestoreAutoRun; left alone when the
setting can't be read. Nothing in retail calls it (invented name).
================
*/
// FUNCTION: STBC 0x00460990
void DisableAutoRun( void ) {
	HKEY key = NULL;
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", 0,
					   KEY_ALL_ACCESS, &key ) != ERROR_SUCCESS ) {
		return;
	}
	if ( savedAutoRun == 0 ) {
		DWORD size = sizeof( savedAutoRun );
		if ( RegQueryValueEx( key, "NoDriveTypeAutoRun", NULL, NULL, (LPBYTE)&savedAutoRun, &size ) != ERROR_SUCCESS ) {
			goto done;
		}
	}
	{	// its own scope: the goto may not jump past the initialised value
		DWORD noAutoRun = 0xFF;
		RegSetValueEx( key, "NoDriveTypeAutoRun", 0, REG_BINARY, (LPBYTE)&noAutoRun, sizeof( noAutoRun ) );
	}
done:
	RegFlushKey( key );
	RegCloseKey( key );
}

/*
================
RestoreAutoRun

Writes back the AutoRun setting DisableAutoRun replaced. Nothing in retail
calls it (invented name).
================
*/
// FUNCTION: STBC 0x00460a30
void RestoreAutoRun( void ) {
	HKEY key = NULL;
	if ( savedAutoRun == 0 ) {
		return;
	}
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", 0,
					   KEY_ALL_ACCESS, &key ) != ERROR_SUCCESS ) {
		return;
	}
	RegSetValueEx( key, "NoDriveTypeAutoRun", 0, REG_BINARY, (LPBYTE)&savedAutoRun, sizeof( savedAutoRun ) );
	RegFlushKey( key );
	RegCloseKey( key );
}

/*
================
BezierCurve::BezierCurve

A curve of count_ points, left unset.
================
*/
// FUNCTION: STBC 0x00460aa0 ??0BezierCurve@@QAE@H@Z
BezierCurve::BezierCurve( int count_ ) : count( count_ ) {
	points = new NiPoint3[count];
	// BUG: coefficients is never allocated here, and ComputeCoefficients
	// writes through it
	ComputeCoefficients();
}

/*
================
BezierCurve::BezierCurve

A curve through a copy of count_ points.
================
*/
// FUNCTION: STBC 0x00460b30 ??0BezierCurve@@QAE@HPAVNiPoint3@@@Z
BezierCurve::BezierCurve( int count_, NiPoint3 *points_ ) : count( 0 ) {
	SetPoints( count_, points_ );
}

/*
================
BezierCurve::BezierCurve
================
*/
// FUNCTION: STBC 0x00460b90 ??0BezierCurve@@QAE@ABV0@@Z
BezierCurve::BezierCurve( const BezierCurve &curve ) {
	count = 0;
	SetPoints( curve.count, curve.points );
}

/*
================
BezierCurve::BezierCurve
================
*/
// FUNCTION: STBC 0x00460bf0 ??0BezierCurve@@QAE@XZ
BezierCurve::BezierCurve() {
	count = 0;
	points = NULL;
	coefficients = NULL;
}

/*
================
BezierCurve::~BezierCurve
================
*/
// FUNCTION: STBC 0x00460c10
BezierCurve::~BezierCurve() {
	delete [] points;
	delete [] coefficients;
}

/*
================
BezierCurve::SetPoint
================
*/
// FUNCTION: STBC 0x00460c40
void BezierCurve::SetPoint( int index, const NiPoint3 &point ) {
	points[index] = point;
}

/*
================
BezierCurve::SetPoints
================
*/
// FUNCTION: STBC 0x00460c70
void BezierCurve::SetPoints( int count_, NiPoint3 *points_ ) {
	if ( count != 0 ) {
		delete [] points;
		points = NULL;
		delete [] coefficients;
		coefficients = NULL;
	}

	count = count_;
	points = new NiPoint3[count];
	coefficients = new float[count];

	for ( int i = 0; i < count; i++ ) {
		points[i] = points_[i];
	}

	ComputeCoefficients();
}

/*
================
BezierCurve::ComputeCoefficients

C(count-1,i) for each point i: the product of count-1 down to i+1, divided
by the product of count-1-i down to 2.
================
*/
// FUNCTION: STBC 0x00460d20
void BezierCurve::ComputeCoefficients() {
	for ( int i = 0; i < count; i++ ) {
		coefficients[i] = 1.0f;

		int j;
		for ( j = count - 1; j > i; j-- ) {
			coefficients[i] *= j;
		}
		for ( j = count - i - 1; j >= 2; j-- ) {
			coefficients[i] /= j;
		}
	}
}

/*
================
BezierCurve::Evaluate
================
*/
// FUNCTION: STBC 0x00460d90
NiPoint3 BezierCurve::Evaluate( float t ) {
	NiPoint3 result( 0.0f, 0.0f, 0.0f );

	for ( int i = 0; i < count; i++ ) {
		float oneMinusT = 1.0f - t;
		float weight = coefficients[i] * (float)pow( t, i ) * (float)pow( oneMinusT, count - 1 - i );
		result += points[i] * weight;
	}

	return result;
}

/*
================
BezierCurve::EvaluateDerivative
================
*/
// FUNCTION: STBC 0x00460e80
NiPoint3 BezierCurve::EvaluateDerivative( float t ) {
	NiPoint3 result( 0.0f, 0.0f, 0.0f );

	for ( int i = 0; i < count; i++ ) {
		float rising = 0.0f;
		float falling = 0.0f;

		if ( i - 1 >= 0 ) {
			rising = i * pow( t, i - 1 ) * pow( 1.0 - t, count - 1 - i );
		}
		if ( count - 1 - i - 1 >= 0 ) {
			falling = ( count - 1 - i ) * pow( t, i ) * pow( 1.0 - t, count - 1 - i - 1 );
		}

		result += points[i] * ( coefficients[i] * ( rising - falling ) );
	}

	return result;
}

/*
================
BezierCurve::GetPoints
================
*/
// FUNCTION: STBC 0x00460fc0
NiPoint3 *BezierCurve::GetPoints() {
	return points;
}

/*
================
BezierCurve::WriteToStream
================
*/
// FUNCTION: STBC 0x00460fd0
bool BezierCurve::WriteToStream( TGStream *stream ) {
	stream->WriteLong( count );

	for ( int i = 0; i < count; i++ ) {
		stream->WriteFloat( coefficients[i] );
		stream->WriteFloat( points[i].x );
		stream->WriteFloat( points[i].y );
		stream->WriteFloat( points[i].z );
	}

	return true;
}

/*
================
BezierCurve::ReadFromStream
================
*/
// FUNCTION: STBC 0x00461050
bool BezierCurve::ReadFromStream( TGStream *stream ) {
	count = stream->ReadLong();
	points = new NiPoint3[count];
	coefficients = new float[count];

	for ( int i = 0; i < count; i++ ) {
		coefficients[i] = stream->ReadFloat();
		points[i].x = stream->ReadFloat();
		points[i].y = stream->ReadFloat();
		points[i].z = stream->ReadFloat();
	}

	return true;
}
