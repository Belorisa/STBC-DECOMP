/*
===============================================================================

	TGSystemWrapperClass.cpp

===============================================================================
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include <windows.h>

#include "TGSystemWrapperClass.h"
#include "TGDirectInput7.h"
#include "TGDirectory.h"
#include "ImageUtils.h"
#include "NiRawImageData.h"
#include "renderer/NiD3DRender.h"
#include "NiApplication.h"
#include "AppConstants.h"

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x0071a8b0 _$E252
// FUNCTION: STBC 0x0071a8c0 _$E249
// FUNCTION: STBC 0x0071a8d0 _$E251
// FUNCTION: STBC 0x0071a8e0 _$E250
// DATA: STBC 0x0099c6b0
TGSystemWrapperClass g_kSystemWrapper;

/*
================
TGSystemWrapperClass::TGSystemWrapperClass
================
*/
// FUNCTION: STBC 0x0071a8f0
TGSystemWrapperClass::TGSystemWrapperClass( void ) {
	window = NULL;
	firstUpdate = true;
	startTick = 0;
	renderer = NULL;
	timeInMilliseconds = 0;
	timeInSeconds = 0.0f;
	timeElapsedInMilliseconds = 0;
	timeElapsedInSeconds = 0.0f;
	if ( QueryPerformanceFrequency( (LARGE_INTEGER *)&countFrequency ) ) {
		usePerformanceCounter = true;
		frameStartCount = 0;
	} else {
		usePerformanceCounter = false;
	}
	updateNumber = 0;
	unknown3C = PFID_INVALID;
	defaultScreenShotPath = NULL;
	directInput = NULL;
	for ( int i = 0; i < MAX_JOYSTICKS; i++ ) {
		joysticks[i].capsRead = false;
	}
	numJoysticks = (unsigned int)-1;
}

/*
================
TGSystemWrapperClass::~TGSystemWrapperClass
================
*/
// FUNCTION: STBC 0x0071a960
TGSystemWrapperClass::~TGSystemWrapperClass( void ) {
}

/*
================
TGSystemWrapperClass::Unknown71A970
================
*/
// FUNCTION: STBC 0x0071a970
void TGSystemWrapperClass::Unknown71A970( HWND window_, NiRender *renderer_ ) {
	window = window_;
	renderer = renderer_;
	GetDirectInput();
}

/*
================
TGSystemWrapperClass::ReleaseDirectInput
================
*/
// FUNCTION: STBC 0x0071a990
void TGSystemWrapperClass::ReleaseDirectInput( void ) {
	if ( directInput != NULL ) {
		directInput->Release();
	}
	directInput = NULL;
}

/*
================
TGSystemWrapperClass::GetDirectInput

NULL when DirectInput 7 can't be created.
================
*/
// FUNCTION: STBC 0x0071a9b0
IDirectInput7A *TGSystemWrapperClass::GetDirectInput( void ) {
	if ( directInput == NULL ) {
		DirectInputCreateEx( NiApplication::GetInstanceReference(), DIRECTINPUT_VERSION, IID_IDirectInput7A, (void **)&directInput, NULL );
		return directInput;
	}
	return directInput;
}

/*
================
TGSystemWrapperClass::UpdateTime

Advances the game clocks to the current tick count and counts the update. The
first update starts the millisecond clock.
================
*/
// FUNCTION: STBC 0x0071a9e0 ?UpdateTime@TGSystemWrapperClass@@QAEXXZ
void TGSystemWrapperClass::UpdateTime( void ) {
	updateNumber++;
	if ( firstUpdate ) {
		firstUpdate = false;
		startTick = timeGetTime();
	}

	unsigned long now = timeGetTime();
	__int64 elapsed = now - startTick;
	startTick = now;
	timeInMilliseconds += elapsed;
	timeElapsedInMilliseconds = elapsed;
	timeElapsedInSeconds = elapsed / 1000.0f;
	timeInSeconds += timeElapsedInSeconds;
	if ( usePerformanceCounter ) {
		QueryPerformanceCounter( (LARGE_INTEGER *)&frameStartCount );
	}
}

/*
================
TGSystemWrapperClass::FileOpen
================
*/
// FUNCTION: STBC 0x0071aa60 ?FileOpen@TGSystemWrapperClass@@QAEPAU_iobuf@@PAD0@Z
FILE *TGSystemWrapperClass::FileOpen( char *name, char *mode ) {
	return fopen( name, mode );
}

/*
================
TGSystemWrapperClass::FileExists
================
*/
// FUNCTION: STBC 0x0071aa80
bool TGSystemWrapperClass::FileExists( char *name ) {
	FILE *file = FileOpen( name, "rb" );
	if ( file != NULL ) {
		FileClose( file );
		return true;
	}
	return false;
}

/*
================
TGSystemWrapperClass::GetWorkingDirectory
================
*/
// FUNCTION: STBC 0x0071aab0 ?GetWorkingDirectory@TGSystemWrapperClass@@QAEPADPADH@Z
char *TGSystemWrapperClass::GetWorkingDirectory( char *buffer, int size ) {
	return _getcwd( buffer, size );
}

/*
================
TGSystemWrapperClass::SetWorkingDirectory
================
*/
// FUNCTION: STBC 0x0071aad0 ?SetWorkingDirectory@TGSystemWrapperClass@@QAE_NPAD@Z
bool TGSystemWrapperClass::SetWorkingDirectory( char *directory ) {
	return _chdir( directory ) ? false : true;
}

/*
================
TGSystemWrapperClass::MakeDirectory
================
*/
// FUNCTION: STBC 0x0071aaf0
bool TGSystemWrapperClass::MakeDirectory( char *name ) {
	return CreateDirectory( name, NULL ) != 0;
}

/*
================
TGSystemWrapperClass::BuildPath
================
*/
// FUNCTION: STBC 0x0071ab10
// PARKED: frame: the va pointer takes the lowest home slot that doesn't interfere, path's (path lives in ebx); retail's choice of first's slot needs path's home live over the loop, which no spelling of the body gives (the decorated name fixes the two named parameters)
void TGSystemWrapperClass::BuildPath( char *path, char *first, ... ) {
	strcpy( path, first );
	va_list parts;
	va_start( parts, first );
	char *part = va_arg( parts, char * );
	while ( part != NULL ) {
		char last = path[strlen( path ) - 1];
		if ( last != '/' && last != '\\' ) {
			strcat( path, strchr( path, '\\' ) != NULL ? "\\" : "/" );
		}
		strcat( path, part );
		part = va_arg( parts, char * );
	}
	va_end( parts );
}

/*
================
TGSystemWrapperClass::SetFileReadOnly
================
*/
// FUNCTION: STBC 0x0071abf0
void TGSystemWrapperClass::SetFileReadOnly( char *name, bool readOnly ) {
	if ( readOnly ) {
		_chmod( name, _S_IREAD );
	} else {
		_chmod( name, _S_IWRITE );
	}
}

/*
================
TGSystemWrapperClass::SplitPath
================
*/
// FUNCTION: STBC 0x0071ac30
void TGSystemWrapperClass::SplitPath( char *path, char *directory, char *file ) {
	char drive[_MAX_DRIVE + 1];
	char extension[_MAX_EXT];
	char name[_MAX_FNAME];
	char dir[_MAX_DIR];
	_splitpath( path, drive, dir, name, extension );
	if ( file != NULL ) {
		sprintf( file, "%s%s", name, extension );
	}
	if ( directory != NULL ) {
		sprintf( directory, "%s%s", drive, dir );
	}
}

/*
================
TGSystemWrapperClass::GetTimeSinceFrameStart

Seconds since the frame began, from the performance counter when there is
one and the millisecond tick count otherwise.
================
*/
// FUNCTION: STBC 0x0071acc0 ?GetTimeSinceFrameStart@TGSystemWrapperClass@@QAENXZ
double TGSystemWrapperClass::GetTimeSinceFrameStart( void ) {
	if ( usePerformanceCounter ) {
		__int64 now;
		QueryPerformanceCounter( (LARGE_INTEGER *)&now );
		__int64 elapsed = now - frameStartCount;
		return (double)elapsed / (double)countFrequency;
	}
	__int64 elapsed = timeGetTime() - startTick;
	return elapsed * 0.001;
}

/*
================
TGSystemWrapperClass::FileRead
================
*/
// FUNCTION: STBC 0x0071ad30 ?FileRead@TGSystemWrapperClass@@QAE_NPAU_iobuf@@PADH@Z
bool TGSystemWrapperClass::FileRead( FILE *file, char *buffer, int length ) {
	return fread( buffer, 1, length, file ) == length ? true : false;
}

/*
================
TGSystemWrapperClass::FileWrite
================
*/
// FUNCTION: STBC 0x0071ad60
bool TGSystemWrapperClass::FileWrite( FILE *file, char *buffer, int length ) {
	return fwrite( buffer, 1, length, file ) == length ? true : false;
}

/*
================
TGSystemWrapperClass::FileSeek

False when the seek fails or the origin is not a fileSeekOrigin_t.
================
*/
// FUNCTION: STBC 0x0071ad90
bool TGSystemWrapperClass::FileSeek( FILE *file, long offset, int origin ) {
	int result = 1;
	switch ( origin ) {
	case FILE_SEEK_BEGIN:
		result = fseek( file, offset, SEEK_SET );
		break;
	case FILE_SEEK_END:
		result = fseek( file, offset, SEEK_END );
		break;
	case FILE_SEEK_CURRENT:
		result = fseek( file, offset, SEEK_CUR );
		break;
	}
	return result == 0;
}

/*
================
TGSystemWrapperClass::FileLength

The length of the file; the position is left where it was.
================
*/
// FUNCTION: STBC 0x0071ae10 ?FileLength@TGSystemWrapperClass@@QAEHPAU_iobuf@@@Z
int TGSystemWrapperClass::FileLength( FILE *file ) {
	if ( file ) {
		long position = ftell( file );
		if ( fseek( file, 0, SEEK_END ) == 0 ) {
			long length = ftell( file );
			fseek( file, position, SEEK_SET );
			return length;
		}
	}
	return 0;
}

/*
================
TGSystemWrapperClass::FileClose
================
*/
// FUNCTION: STBC 0x0071ae60 ?FileClose@TGSystemWrapperClass@@QAEXPAU_iobuf@@@Z
void TGSystemWrapperClass::FileClose( FILE *file ) {
	fclose( file );
}

/*
================
TGSystemWrapperClass::IsForeground

True when the game's window, or a child of it, is the foreground window.
================
*/
// FUNCTION: STBC 0x0071ae70 ?IsForeground@TGSystemWrapperClass@@QAE_NXZ
bool TGSystemWrapperClass::IsForeground( void ) {
	return GetForegroundWindow() == GetWindowHandle() || IsChild( GetWindowHandle(), GetForegroundWindow() ) || IsChild( GetForegroundWindow(), GetWindowHandle() );
}

/*
================
TGSystemWrapperClass::GetRandomNumber

A number from 0 up to, not including, the limit.
================
*/
// FUNCTION: STBC 0x0071aec0 ?GetRandomNumber@TGSystemWrapperClass@@QAEHH@Z
int TGSystemWrapperClass::GetRandomNumber( int limit ) {
	return (unsigned int)rand() % (unsigned int)limit;
}

/*
================
TGSystemWrapperClass::SetRandomSeed
================
*/
// FUNCTION: STBC 0x0071aed0 ?SetRandomSeed@TGSystemWrapperClass@@QAEXH@Z
void TGSystemWrapperClass::SetRandomSeed( int seed ) {
	srand( seed );
}

/*
================
TGSystemWrapperClass::PollJoystick

Reads the joystick's caps the first time, then its position.
================
*/
// FUNCTION: STBC 0x0071aee0
bool TGSystemWrapperClass::PollJoystick( unsigned int index ) {
	if ( numJoysticks == (unsigned int)-1 ) {
		numJoysticks = joyGetNumDevs();
	}
	if ( index < MAX_JOYSTICKS ) {
		if ( !joysticks[index].capsRead ) {
			joysticks[index].capsRead = true;
			JOYCAPS caps;
			// BUG: the caps read are always the first joystick's, whatever the index
			if ( joyGetDevCaps( JOYSTICKID1, &caps, sizeof( caps ) ) != JOYERR_NOERROR ) {
				return false;
			}
			joysticks[index].xCenter = ( caps.wXmax - caps.wXmin ) >> 1;
			joysticks[index].yCenter = ( caps.wYmax - caps.wYmin ) >> 1;
			joysticks[index].zCenter = ( caps.wCaps & JOYCAPS_HASZ ) ? ( caps.wZmax - caps.wZmin ) >> 1 : 0;
			joysticks[index].rCenter = ( caps.wCaps & JOYCAPS_HASR ) ? ( caps.wRmax - caps.wRmin ) >> 1 : 0;
			joysticks[index].uCenter = ( caps.wCaps & JOYCAPS_HASU ) ? ( caps.wUmax - caps.wUmin ) >> 1 : 0;
			joysticks[index].vCenter = ( caps.wCaps & JOYCAPS_HASV ) ? ( caps.wVmax - caps.wVmin ) >> 1 : 0;
			joysticks[index].hasPOV = ( caps.wCaps & JOYCAPS_HASPOV ) != 0;
		}
		if ( index < numJoysticks ) {
			JOYINFOEX info;
			info.dwSize = sizeof( info );
			info.dwFlags = JOY_RETURNALL | JOY_USEDEADZONE;
			if ( joyGetPosEx( index, &info ) == JOYERR_NOERROR ) {
				joysticks[index].x = info.dwXpos - joysticks[index].xCenter;
				joysticks[index].y = info.dwYpos - joysticks[index].yCenter;
				joysticks[index].z = info.dwZpos - joysticks[index].zCenter;
				joysticks[index].r = info.dwRpos - joysticks[index].rCenter;
				joysticks[index].u = info.dwUpos - joysticks[index].uCenter;
				joysticks[index].v = info.dwVpos - joysticks[index].vCenter;
				if ( joysticks[index].hasPOV && info.dwPOV != JOY_POVCENTERED ) {
					joysticks[index].pov = info.dwPOV / 100;
				} else {
					joysticks[index].pov = -1;
				}
				joysticks[index].buttons = info.dwButtons;
				return true;
			}
			joysticks[index].x = 0;
			joysticks[index].y = 0;
			joysticks[index].z = 0;
			joysticks[index].r = 0;
			joysticks[index].u = 0;
			joysticks[index].v = 0;
			joysticks[index].pov = -1;
			joysticks[index].buttons = 0;
		}
	}
	return false;
}

/*
================
TGSystemWrapperClass::GetVerticalAspectRatio

The width of the window's client area over its height, or 1 without a window.
================
*/
// FUNCTION: STBC 0x0071b100 ?GetVerticalAspectRatio@TGSystemWrapperClass@@QAEMXZ
float TGSystemWrapperClass::GetVerticalAspectRatio( void ) {
	if ( window != NULL ) {
		RECT client;
		GetClientRect( window, &client );
		float width = (float)( client.right - client.left );
		float height = (float)( client.bottom - client.top );
		return width / height;
	}
	return 1.0f;
}

/*
================
TGSystemWrapperClass::TakeScreenshot

Saves the renderer's back buffer as a TGA or BMP file: under the name given,
in the default screenshot path when there is one, or else under the first
ScreenShot%03i name no file has yet.
================
*/
// FUNCTION: STBC 0x0071b150
void TGSystemWrapperClass::TakeScreenshot( int format, char *fileName ) {
	NiD3DRender *d3dRenderer = NiDynamicCast( NiD3DRender, renderer );
	if ( d3dRenderer == NULL ) {
		return;
	}

	NiPointer<NiRawImageData> image = d3dRenderer->GrabBackBuffer();
	char name[MAX_PATH];
	if ( fileName != NULL ) {
		switch ( format ) {
			case IMAGE_FORMAT_TGA:
				if ( defaultScreenShotPath != NULL ) {
					sprintf( name, "%s/%s", defaultScreenShotPath, fileName );
				} else {
					sprintf( name, "%s", fileName );
				}
				SaveTGAFile( name, image );
				break;
			case IMAGE_FORMAT_BMP:
				if ( defaultScreenShotPath != NULL ) {
					sprintf( name, "%s/%s", defaultScreenShotPath, fileName );
				} else {
					sprintf( name, "%s", fileName );
				}
				SaveBMPFile( name, image );
				break;
		}
	} else {
		int number = 0;
		FILE *file;
		do {
			memset( name, 0, sizeof( name ) );
			switch ( format ) {
				case IMAGE_FORMAT_TGA:
					if ( defaultScreenShotPath != NULL ) {
						sprintf( name, "%s/ScreenShot%03i.tga", defaultScreenShotPath, number++ );
					} else {
						sprintf( name, "ScreenShot%03i.tga", number++ );
					}
					break;
				case IMAGE_FORMAT_BMP:
					if ( defaultScreenShotPath != NULL ) {
						sprintf( name, "%s/ScreenShot%03i.bmp", defaultScreenShotPath, number++ );
					} else {
						sprintf( name, "ScreenShot%03i.bmp", number++ );
					}
					break;
			}
			file = fopen( name, "r" );
			if ( file != NULL ) {
				fclose( file );
			}
		} while ( file != NULL );

		switch ( format ) {
			case IMAGE_FORMAT_TGA:
				SaveTGAFile( name, image );
				break;
			case IMAGE_FORMAT_BMP:
				SaveBMPFile( name, image );
				break;
		}
	}
}

/*
================
TGSystemWrapperClass::SetDefaultScreenShotPath
================
*/
// FUNCTION: STBC 0x0071b390 ?SetDefaultScreenShotPath@TGSystemWrapperClass@@QAEXPAD@Z
void TGSystemWrapperClass::SetDefaultScreenShotPath( char *path ) {
	if ( defaultScreenShotPath ) {
		delete defaultScreenShotPath;
	}
	defaultScreenShotPath = NULL;
	if ( path ) {
		defaultScreenShotPath = new char[ strlen( path ) + 1 ];
		strcpy( defaultScreenShotPath, path );
	}
}

/*
================
TGSystemWrapperClass::GetIniInt

The integer the key holds in the section of the INI file, or the default
when the file, the section or the key is missing.
================
*/
// FUNCTION: STBC 0x0071b400 ?GetIniInt@TGSystemWrapperClass@@QAEHPAD0H0@Z
int TGSystemWrapperClass::GetIniInt( char *section, char *key, int defaultValue, char *fileName ) {
	char value[256];
	FILE *file = FileOpen( fileName, "r" );
	if ( file ) {
		if ( FindIniValue( file, section, key, value, sizeof( value ) ) ) {
			FileClose( file );
			return atoi( value );
		}
		FileClose( file );
	}
	return defaultValue;
}

/*
================
TGSystemWrapperClass::GetIniFloat

The float the key holds in the section of the INI file, or the default
when the file, the section or the key is missing.
================
*/
// FUNCTION: STBC 0x0071b480 ?GetIniFloat@TGSystemWrapperClass@@QAEMPAD0M0@Z
float TGSystemWrapperClass::GetIniFloat( char *section, char *key, float defaultValue, char *fileName ) {
	char value[256];
	FILE *file = FileOpen( fileName, "r" );
	if ( file ) {
		if ( FindIniValue( file, section, key, value, sizeof( value ) ) ) {
			FileClose( file );
			return (float)atof( value );
		}
		FileClose( file );
	}
	return defaultValue;
}

/*
================
TGSystemWrapperClass::GetIniString

Copies the string the key holds in the section of the INI file, or the
default when the file, the section or the key is missing, to the buffer;
true when it was found.
================
*/
// FUNCTION: STBC 0x0071b500 ?GetIniString@TGSystemWrapperClass@@QAE_NPAD000H0@Z
bool TGSystemWrapperClass::GetIniString( char *section, char *key, char *defaultValue, char *buffer, int size, char *fileName ) {
	FILE *file = FileOpen( fileName, "r" );
	if ( file ) {
		if ( FindIniValue( file, section, key, buffer, size ) ) {
			FileClose( file );
			return true;
		}
		FileClose( file );
		strcpy( buffer, defaultValue );
		return false;
	}
	strcpy( buffer, defaultValue );
	return false;
}

/*
================
TGSystemWrapperClass::FindIniValue

Reads the whole file, finds the "[section]" line (names compare without
regard to case), then the "key=value" line under it, and copies the value to
the buffer, cut to the size. Lines that start with ';' are skipped.
================
*/
// FUNCTION: STBC 0x0071b5b0 ?FindIniValue@TGSystemWrapperClass@@IAE_NPAU_iobuf@@PAD11H@Z
bool TGSystemWrapperClass::FindIniValue( FILE *file, char *section, char *key, char *buffer, int size ) {
	int length = FileLength( file );
	char *text = new char[ length + 1 ];
	if ( text == NULL ) {
		return false;
	}
	FileRead( file, text, length );
	int i = 0;
	if ( length <= 0 ) {
		delete text;
		return false;
	}
	for ( ;; ) {
		// the next "[name]"
		while ( text[i] != '[' && i <= length ) {
			i++;
		}
		if ( text[i] != '[' ) {
			delete text;
			return false;
		}
		int nameStart = i + 1;
		while ( true ) {
			if ( i > length ) {
				break;
			}
			i++;
			if ( text[i] == ']' ) {
				break;
			}
		}
		if ( text[i] == ']' && _strnicmp( text + nameStart, section, i - nameStart - 1 ) == 0 ) {
			break;
		}
		if ( i >= length ) {
			delete text;
			return false;
		}
	}
	i++;
	while ( i <= length && ( text[i] == '\t' || text[i] == '\n' || text[i] == '\r' ) ) {
		i++;
	}
	for ( ;; ) {
		int keyStart = i;
		while ( i <= length && text[i] != '=' && text[i] != '[' && text[i] != ']' && text[i] != '\n' && text[i] != '\r' && text[i] != '\t' ) {
			i++;
		}
		if ( text[i] != '=' && text[keyStart] != ';' ) {
			delete text;
			return false;
		}
		if ( _strnicmp( text + keyStart, key, i - keyStart ) == 0 ) {
			break;
		}
		// next line
		if ( i > length ) {
			delete text;
			return false;
		}
		while ( i <= length && text[i] != '\n' && text[i] != '\t' && text[i] != '\r' ) {
			i++;
		}
		if ( i >= length ) {
			delete text;
			return false;
		}
		while ( i <= length && ( text[i] == '\n' || text[i] == '\t' || text[i] == '\r' || text[i] == ' ' ) ) {
			i++;
		}
		if ( i >= length ) {
			delete text;
			return false;
		}
	}
	i++;
	if ( i > length ) {
		*buffer = '\0';
		delete text;
		return true;
	}
	int valueStart = i;
	while ( i <= length && text[i] != '\n' && text[i] != '\t' && text[i] != '\r' ) {
		i++;
	}
	int valueLength = i - valueStart;
	int most = size - 1;
	int count = valueLength;
	if ( count >= most ) {
		count = most;
	}
	strncpy( buffer, text + valueStart, count );
	if ( valueLength >= most ) {
		valueLength = most;
	}
	buffer[valueLength] = '\0';
	delete text;
	return true;
}

/*
================
TGSystemWrapperClass::DeleteDirectoryContents

Deletes the directory's subdirectories, its files (made writable first) and
then the directory itself.
================
*/
// FUNCTION: STBC 0x0071b780
bool TGSystemWrapperClass::DeleteDirectoryContents( char *path ) {
	TGDirectory directory;
	directory.Read( path, NULL, true );

	char name[256];
	for ( TGDirectory *subdirectory = directory.GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
		char *subdirectoryName = subdirectory->GetName();
		strcpy( name, path );
		strcat( name, "/" );
		strcat( name, subdirectoryName );
		DeleteDirectoryContents( name );
	}

	TGDirectoryFile *file = directory.GetFiles();
	while ( file != NULL ) {
		TGDirectoryFile *next = file->next;
		strcpy( name, path );
		strcat( name, "/" );
		strcat( name, file->name );
		SetFileAttributes( name, FILE_ATTRIBUTE_NORMAL );
		DeleteFile( name );
		file = next;
	}

	// BUG: redundant: builds the directory's name with a separator and then
	// removes the directory by path.
	strcpy( name, path );
	strcat( name, "/" );
	RemoveDirectory( path );
	return true;
}

/*
================
TGSystemWrapperClass::CopyDirectory

Copies the source directory's tree into the destination, creating the
destination's directories; files already there are kept. Nothing happens
when both are the same path.
================
*/
// FUNCTION: STBC 0x0071b9d0
bool TGSystemWrapperClass::CopyDirectory( char *destination, char *source ) {
	if ( strcmp( destination, source ) != 0 ) {
		TGDirectory directory;
		directory.Read( source, NULL, true );
		TGDirectory *subdirectory = directory.GetSubdirectories();
		CreateDirectory( destination, NULL );

		char destinationName[256];
		char sourceName[256];
		for ( ; subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
			char *subdirectoryName = subdirectory->GetName();
			strcpy( destinationName, destination );
			strcat( destinationName, "/" );
			strcat( destinationName, subdirectoryName );
			strcpy( sourceName, source );
			strcat( sourceName, "/" );
			strcat( sourceName, subdirectoryName );
			CopyDirectory( destinationName, sourceName );
		}

		TGDirectoryFile *file = directory.GetFiles();
		while ( file != NULL ) {
			TGDirectoryFile *next = file->next;
			strcpy( sourceName, source );
			strcat( sourceName, "/" );
			strcat( sourceName, file->name );
			strcpy( destinationName, destination );
			strcat( destinationName, "/" );
			strcat( destinationName, file->name );
			CopyFile( sourceName, destinationName, TRUE );
			SetFileAttributes( destinationName, FILE_ATTRIBUTE_NORMAL );
			file = next;
		}
	}
	return true;
}

