/*
===============================================================================

	TGMovieManager.h

	The Bink movie player: g_kMovieManager (0x995A00) is the one instance.
	It plays up to MAX_MOVIES_AT_ONCE movies, each for the TGMovieAction
	that started it, copying their frames straight into the renderer's back
	buffer. Movie mode recreates the renderer at 640 by 480 for the movies
	and restores the display mode it saved when the movies end. The
	loading screen's "forced" movie is a separate Bink handle.

	Evidence: the SWIG wrappers name PlayNextFrame, SwitchToMovieMode,
	SwitchOutOfMovieMode, SetMovieVolume and LoadForcedMovie, the constant
	MAX_MOVIES_AT_ONCE (5) and the inline IsAnyPlaying, which tests the
	count at 0x3C ( > 0 ). The constructor (0x6AECD0, the static
	initialiser 0x6AECA0 calls it on 0x995A00) clears the movie mode flag,
	the count and the forced movie, sets 0x5E and the volume (1.0) and
	makes an empty display device (0x1C bytes, vtable 0x88B9F8); the
	destructor (0x6AED90) switches out of movie mode and deletes the
	device. Each movie entry is 0x0C bytes: the action (its Skip and the
	action manager's FindAction are called on it), the Bink handle
	(BinkClose) and the BinkWait result of the last frame. UtopiaModule's
	save and load (0x443940) call WriteToStream, ReadFromStream,
	FixUpPointers and PostFixUp (0x6AFD40 to 0x6AFEC0). The member names
	are invented.

===============================================================================
*/

#ifndef __TGMOVIEMANAGER_H__
#define __TGMOVIEMANAGER_H__

struct BINK;
struct BINKBUFFER;
class Ni3DDeviceDesc;
class TGMovieAction;
class TGStream;

// One movie playing (invented name).
struct TGMovieEntry {
	TGMovieAction *	action;				// 0x00 the action that plays it; its ID between ReadFromStream and PostFixUp
	BINK *			bink;				// 0x04 NULL for a free entry
	bool			waiting;			// 0x08 the last frame is not due yet (BinkWait)
};

class TGMovieManager {
public:
	enum {
		MAX_MOVIES_AT_ONCE = 5
	};

					TGMovieManager( void );
					~TGMovieManager( void );

	void			PlayNextFrame( void );
	void			SwitchToMovieMode( void );
	void			SwitchOutOfMovieMode( void );
	void			SetMovieVolume( float );
	void			LoadForcedMovie( char * );

	// Draws the forced movie's next frame (invented name).
	void			PlayForcedMovieFrame( void );
	// Closes the forced movie (invented name).
	void			CloseForcedMovie( void );

	bool			IsAnyPlaying( void ) { return numMovies > 0; }
	bool			IsForcedMoviePlaying( void ) { return forcedMovie != NULL; }	// invented name
	// the application's activation starts and stops the movies (invented name)
	void			SetPlayMovies( bool playMovies_ ) { playMovies = playMovies_; }

	// Opens the movie file for the action; false when the entries are full
	// or the file does not open (invented name).
	bool			AddMovie( char *file, TGMovieAction *action, bool skipFrames );
	// Closes the movie of the entry and closes up the entries after it;
	// skipAction also skips its action, if it is still alive (invented name).
	void			RemoveMovie( int index, bool skipAction );
	// Whether the action's movie plays; drops the entries whose actions
	// are gone (invented name).
	bool			IsPlaying( TGMovieAction *action );
	// Stops the action's movie, and drops the entries whose actions are
	// gone; the last one closes the buffer (invented name).
	void			StopMovie( TGMovieAction *action );
	// Stops every movie and skips their actions (invented name).
	void			StopAllMovies( void );
	// Opens the blitting buffer on the renderer's DirectDraw object
	// (invented name).
	bool			OpenBuffer( void );

	// The save and load of the movies playing and of the movie mode.
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );

protected:
	// The renderer settings to switch to: the movies' 640 by 480 out of
	// movie mode, the saved ones in it (invented names).
	int				GetRendererMode( void );
	unsigned int	GetRendererWidth( void );
	unsigned int	GetRendererHeight( void );
	unsigned int	GetRendererFlags( void );
	void *			GetRendererWindow( void );
	Ni3DDeviceDesc *	GetDisplayDevice( void );

	TGMovieEntry	movies[MAX_MOVIES_AT_ONCE];	// 0x00
	int				numMovies;				// 0x3C entries in use
	bool			movieMode;				// 0x40 SwitchToMovieMode recreated the renderer
	BINKBUFFER *	buffer;					// 0x44 OpenBuffer's
	BINK *			forcedMovie;			// 0x48 the movie LoadForcedMovie opened
	Ni3DDeviceDesc *	savedDevice;	// 0x4C the display device before movie mode
	unsigned int	savedWidth;				// 0x50 the display size before movie mode
	unsigned int	savedHeight;			// 0x54
	unsigned int	savedBitDepth;			// 0x58 16 or 32
	bool			savedWindowed;			// 0x5C
	bool			unknown5D;				// 0x5D cleared by SwitchToMovieMode; selects the renderer flag 2 over 1
	bool			playMovies;				// 0x5E PlayNextFrame does nothing while it is clear
	float			volume;					// 0x60 SetMovieVolume's volume, 0 to 1
};

// The movie player (the SWIG variable g_kMovieManager).
extern TGMovieManager	g_kMovieManager;

#endif /* !__TGMOVIEMANAGER_H__ */
