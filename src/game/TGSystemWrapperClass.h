/*
===============================================================================

	TGSystemWrapperClass.h

	The system services the scripts reach through g_kSystemWrapper: the game
	clock, the random numbers, the INI settings and screenshots.

	Only the members the SWIG wrappers prove are declared. The clock fields
	are those of the inline accessors TGSystemWrapperClass_GetTimeInSeconds
	(+0x0C), GetTimeInMilliseconds (+0x10), GetTimeElapsedInSeconds
	(+0x14), GetTimeElapsedInMilliseconds (+0x18) and GetUpdateNumber
	(+0x38). The renderer at 0x00 is the one the cameras draw with:
	CameraObjectClass::SetFrustumValues (0x42A690) takes a counted
	reference to it for its screen size, and CreateFromNiCamera (0x42A120)
	gives it to the NetImmerse camera. The rest of the layout is not
	recovered yet. The object is 0x140 bytes:
	g_kSystemWrapper_set (0x65B140) copies it as one block.

===============================================================================
*/

#ifndef __TGSYSTEMWRAPPERCLASS_H__
#define __TGSYSTEMWRAPPERCLASS_H__

#include "NiPointer.h"

class NiRender;
struct IDirectInput7A;
struct HWND__;
typedef struct HWND__ *HWND;
struct _iobuf;
typedef struct _iobuf FILE;

// Where FileSeek counts the offset from (invented names; the values are
// FileSeek's cases, the CD check (0x4FC610) seeks with FILE_SEEK_BEGIN).
enum fileSeekOrigin_t {
	FILE_SEEK_CURRENT,
	FILE_SEEK_END,
	FILE_SEEK_BEGIN
};

// The joysticks the wrapper polls through the multimedia API
// (joyGetDevCaps, joyGetPosEx): one per joystick ID, 0x3C bytes. The axes
// are the positions less their centres, which the device's caps give the
// first time it is polled. The struct and its names are invented.
struct TGJoystickState {
	unsigned long	buttons;		// 0x00 dwButtons
	int				x;				// 0x04
	unsigned int	xCenter;		// 0x08 half the caps' X range
	int				y;				// 0x0C
	unsigned int	yCenter;		// 0x10
	int				z;				// 0x14
	unsigned int	zCenter;		// 0x18 0 without a Z axis
	int				r;				// 0x1C
	unsigned int	rCenter;		// 0x20 0 without a rudder
	int				u;				// 0x24
	unsigned int	uCenter;		// 0x28 0 without a U axis
	int				v;				// 0x2C
	unsigned int	vCenter;		// 0x30 0 without a V axis
	int				pov;			// 0x34 degrees, or -1 when centred or absent
	bool			hasPOV;			// 0x38 the caps report a point-of-view hat
	bool			capsRead;		// 0x39 the caps and centres are known
};

// How many joysticks the wrapper polls (invented name).
#define MAX_JOYSTICKS		4

class TGSystemWrapperClass {
public:
	// Clears the clocks, the joysticks and the screenshot path, and reads
	// the performance counter's frequency.
					TGSystemWrapperClass( void );
					~TGSystemWrapperClass( void );

	// TakeScreenshot's file formats (the SWIG constants
	// TGSystemWrapperClass_IMAGE_FORMAT_*).
	enum {
		IMAGE_FORMAT_TGA,
		IMAGE_FORMAT_BMP
	};

	double	GetTimeSinceFrameStart( void );
	float	GetVerticalAspectRatio( void );
	bool	IsForeground( void );
	int	GetRandomNumber( int );
	void	SetRandomSeed( int );
	// Advances the clocks to the current tick count and counts the update
	// (invented name).
	void	UpdateTime( void );
	int		GetIniInt( char *section, char *key, int defaultValue, char *fileName );
	float	GetIniFloat( char *section, char *key, float defaultValue, char *fileName );
	bool	GetIniString( char *section, char *key, char *defaultValue, char *buffer, int size, char *fileName );
	void	SetDefaultScreenShotPath( char * );
	// The process's working directory, through the CRT (invented names;
	// TGDirectory's scan saves and restores it).
	char *	GetWorkingDirectory( char *buffer, int size );
	bool	SetWorkingDirectory( char *directory );
	// Deletes the files and subdirectories of the directory, recursively,
	// and copies a directory tree into another unless both are the same
	// path (invented names; UtopiaModule's episode sounds use them).
	bool	DeleteDirectoryContents( char *path );
	bool	CopyDirectory( char *destination, char *source );

	float	GetTimeInSeconds( void ) { return timeInSeconds; }
	int		GetTimeInMilliseconds( void ) { return timeInMilliseconds; }
	float	GetTimeElapsedInSeconds( void ) { return timeElapsedInSeconds; }
	int		GetTimeElapsedInMilliseconds( void ) { return timeElapsedInMilliseconds; }
	int		GetUpdateNumber( void ) { return updateNumber; }
	NiRender *	GetRenderer( void ) { return renderer; }		// invented name
	void		SetRenderer( NiRender *renderer_ ) { renderer = renderer_; }		// invented name
	HWND	GetWindowHandle( void ) { return window; }		// invented name
	// The DirectInput object, created on first use (DirectX 7), and its
	// release (invented names).
	IDirectInput7A *	GetDirectInput( void );
	void	ReleaseDirectInput( void );

	// The file helpers of the INI readers, the profiler, the script check
	// (NetFile) and the CD check (0x4FC610) (invented names; each wraps the
	// CRT call named in its comment).
	FILE *	FileOpen( char *name, char *mode );					// fopen
	void	FileClose( FILE *file );							// fclose
	bool	FileExists( char *name );							// whether fopen can read it
	int		FileLength( FILE *file );							// the length of the file, leaving the position as it was
	bool	FileRead( FILE *file, char *buffer, int length );	// fread of length bytes in one block
	bool	FileWrite( FILE *file, char *buffer, int length );	// fwrite of length bytes in one block
	bool	FileSeek( FILE *file, long offset, int origin );	// fseek from a fileSeekOrigin_t
	// _chmod: read-only, or readable and writable.
	void	SetFileReadOnly( char *name, bool readOnly );
	// CreateDirectory with the default security; false when it fails.
	bool	MakeDirectory( char *name );

	// Keeps the window and the renderer and creates the DirectInput object
	// (placeholder name: the call at 0x716036 is its only caller).
	void	Unknown71A970( HWND window_, NiRender *renderer_ );
	// Joins the path parts after the first with the separator the path
	// already uses (a backslash once there is one, else a slash), up to a
	// NULL part (invented name; nothing in retail calls it).
	void	BuildPath( char *path, char *first, ... );
	// The directory (drive and directory) and the file (name and
	// extension) of the path, each when its buffer is given (invented
	// name; nothing in retail calls it).
	void	SplitPath( char *path, char *directory, char *file );
	// Polls the joystick: false when its ID is out of range, its caps can't
	// be read or it doesn't answer (invented name; nothing in retail calls
	// it).
	bool	PollJoystick( unsigned int index );

protected:
	// Finds the key's value in the section of the INI text of the file and
	// copies it to the buffer (invented name).
	bool	FindIniValue( FILE *file, char *section, char *key, char *buffer, int size );

	NiRender *		renderer;		// 0x00 the game's renderer (invented name); a plain pointer: the SWIG setter g_kSystemWrapper_set copies the object as one block
	HWND	window;						// 0x04 the game's window (invented name; GetVerticalAspectRatio and IsForeground use it)
	unsigned long	startTick;			// 0x08 the tick count the millisecond clock counts from (invented name)
	float	timeInSeconds;				// 0x0C the game time of this frame
	int		timeInMilliseconds;			// 0x10
	float	timeElapsedInSeconds;		// 0x14 since the last frame
	int		timeElapsedInMilliseconds;	// 0x18
	bool	usePerformanceCounter;		// 0x1C set: GetTimeSinceFrameStart uses the performance counter (invented name)
	__int64	frameStartCount;			// 0x20 the counter at the frame start (invented name)
	__int64	countFrequency;				// 0x28 the counter's frequency (invented name)
	int		unknown30;					// 0x30
	bool	firstUpdate;				// 0x34 set until the first UpdateTime starts the clock (invented name)
	int		updateNumber;				// 0x38 frames so far
	int		unknown3C;					// 0x3C
	char *	defaultScreenShotPath;		// 0x40 owned copy, or NULL (invented name)
	IDirectInput7A *	directInput;	// 0x44 counted reference: GetDirectInput creates it (invented name)
	unsigned int	numJoysticks;		// 0x48 joyGetNumDevs, read on the first poll; -1 before (invented name)
	TGJoystickState	joysticks[MAX_JOYSTICKS];	// 0x4C (invented name)
	int		unknown13C;					// 0x13C the object is 0x140 bytes (the SWIG setter copies 0x50 dwords)

	// Declared for the SWIG wrappers that call them.
public:
	// Saves the back buffer in the format, as the file name in the default
	// screenshot path, or as the first free ScreenShot%03i name when there is
	// none (the SWIG wrapper TGSystemWrapperClass_TakeScreenshot calls it).
	void	TakeScreenshot( int format, char *fileName );
};

// The one instance (the App module's g_kSystemWrapper).
extern TGSystemWrapperClass		g_kSystemWrapper;

#endif /* !__TGSYSTEMWRAPPERCLASS_H__ */
