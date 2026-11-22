/*
===============================================================================

	TGBinkImports.h

	The Bink Video entry points the movie manager calls, declared as
	BINKW32.DLL exports them; the import table's decorated names give the
	stdcall argument sizes. The structures are declared as far as the game
	reads them, with the names of RAD's bink.h.

	Evidence: the import table lists _BinkOpenMiles@4 (IAT 0x8883C8),
	_BinkBufferSetOffset@12, _BinkBufferOpen@16, _BinkBufferSetDirectDraw@8,
	_BinkNextFrame@4, _BinkSetSoundSystem@8, _BinkSetVolume@12, _BinkOpen@8,
	_BinkWait@4, _BinkDDSurfaceType@4, _BinkBufferClose@4,
	_BinkGetRealtime@12, _BinkGoto@12, _BinkDoFrame@4, _BinkCopyToBuffer@28
	and _BinkClose@4 (IAT 0x888404). The movie manager saves a movie's
	position from the first member of a 0x38-byte BINKREALTIME, and tests
	the end of a movie by FrameNum (0x0C) against Frames (0x08).

===============================================================================
*/

#ifndef __TGBINKIMPORTS_H__
#define __TGBINKIMPORTS_H__

// The open movie (bink.h); the members after FrameNum are not used.
struct BINK {
	unsigned int	Width;				// 0x00 pixels
	unsigned int	Height;				// 0x04
	unsigned int	Frames;				// 0x08 frames in the movie
	unsigned int	FrameNum;			// 0x0C the current frame, from 1
};

// A blitting buffer (bink.h); the game only opens, places and closes it.
struct BINKBUFFER;

// The playback statistics BinkGetRealtime fills (bink.h).
struct BINKREALTIME {
	unsigned int	FrameNum;
	unsigned int	FrameRate;
	unsigned int	FrameRateDiv;
	unsigned int	Frames;
	unsigned int	FramesTime;
	unsigned int	FramesVideoDecompTime;
	unsigned int	FramesAudioDecompTime;
	unsigned int	FramesReadTime;
	unsigned int	FramesIdleReadTime;
	unsigned int	FramesThreadReadTime;
	unsigned int	FramesBlitTime;
	unsigned int	ReadBufferSize;
	unsigned int	ReadBufferUsed;
	unsigned int	FramesDataRate;
};

// BinkOpen flags
#define BINKNOSKIP				0x00080000		// play every frame, however late

// BinkGoto flags
#define BINKGOTOQUICK			1

// BinkCopyToBuffer flags: the surface type BinkDDSurfaceType returns, or'd with
#define BINKCOPYALL				0x80000000		// copy every pixel, not just the changed ones

typedef void *	( __stdcall *BINKSNDOPEN )( void *bink, unsigned int track );	// opens a sound track
typedef BINKSNDOPEN	( __stdcall *BINKSNDSYSOPEN )( unsigned long param );		// a sound system's opener

extern "C" {

__declspec( dllimport ) BINK * __stdcall		BinkOpen( const char *name, unsigned int flags );
__declspec( dllimport ) void __stdcall			BinkClose( BINK *bink );
__declspec( dllimport ) int __stdcall			BinkDoFrame( BINK *bink );
__declspec( dllimport ) void __stdcall			BinkNextFrame( BINK *bink );
__declspec( dllimport ) int __stdcall			BinkWait( BINK *bink );
__declspec( dllimport ) void __stdcall			BinkGoto( BINK *bink, unsigned int frame, int flags );
__declspec( dllimport ) int __stdcall			BinkCopyToBuffer( BINK *bink, void *dest, int destPitch, unsigned int destHeight,
																	unsigned int destX, unsigned int destY, unsigned int flags );
__declspec( dllimport ) int __stdcall			BinkGetRealtime( BINK *bink, BINKREALTIME *run, unsigned int frames );
__declspec( dllimport ) void __stdcall			BinkSetVolume( BINK *bink, unsigned int trackID, int volume );
__declspec( dllimport ) int __stdcall			BinkSetSoundSystem( BINKSNDSYSOPEN open, unsigned long param );
__declspec( dllimport ) BINKSNDOPEN __stdcall	BinkOpenMiles( unsigned long param );
__declspec( dllimport ) int __stdcall			BinkDDSurfaceType( void *surface );

__declspec( dllimport ) BINKBUFFER * __stdcall	BinkBufferOpen( void *window, unsigned int width, unsigned int height, unsigned int flags );
__declspec( dllimport ) void __stdcall			BinkBufferClose( BINKBUFFER *buffer );
__declspec( dllimport ) int __stdcall			BinkBufferSetOffset( BINKBUFFER *buffer, int destX, int destY );
__declspec( dllimport ) int __stdcall			BinkBufferSetDirectDraw( void *directDraw, void *primary );

}

#endif /* !__TGBINKIMPORTS_H__ */
