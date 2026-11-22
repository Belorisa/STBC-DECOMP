/*
===============================================================================

	TGApplication.h

	The TG framework's application, between NetImmerse's NiApplication and
	the game's UtopiaApp: it keeps the game clock, which runs at a scale of
	the frame time, stops while the game is paused and never steps further
	than a limit per frame, and it forwards the window's input messages to
	the input manager.

	0xA8 bytes: UtopiaApp's own members start there.

	Evidence: the constructor (0x6CD790) calls NiApplication's, stores the
	vtable 0x895B8C (43 slots: NiApplication's 39 and four of its own) and
	clears the clock; it reads the renderer and the window size from the
	".ini" options ("UI Preferences": "Renderer", "Window Width", "Window
	Height") and loads "vtuneapi.dll" for VtPauseSampling and
	VtResumeSampling. MeasureTime (slot 37, 0x6CDC00) advances the game
	clock at 0x90 by the frame time, less 0x98, at most 0x9C and scaled by
	0xA0; the SWIG wrappers UtopiaModule_GetGameTime and _SetGameTime read
	and write 0x90 through the instance pointer, UtopiaModule_SetTimeRate
	writes 0x9C, UtopiaModule_SetTimeScale 0xA0, UtopiaModule_GetRealTime
	reads NiApplication's current time and UtopiaModule_IsGamePaused tests
	the pause state at 0x8C. UtopiaModule::Pause (0x43BBD0) calls slot 39;
	OnIdle (slot 32, 0x6CDD20) calls slot 40 when a pause takes effect and
	slot 41 when it ends.

	No string names the class: the name is invented. The names of the
	members and of slots 39 to 41 are invented from their use.

===============================================================================
*/

#ifndef __TGAPPLICATION_H__
#define __TGAPPLICATION_H__

#include "NiApplication.h"

// NiApplication.h brings in <windows.h>, whose ANSI/wide macros would rename
// the game's own methods of the same names.
#undef GetClassName
#undef GetObject
#undef PlaySound

// the pause state: Pause asks, OnIdle carries it out at the next frame
enum pauseState_t {
	PAUSE_STATE_RUNNING,			// not paused
	PAUSE_STATE_PAUSING,			// paused at the next frame
	PAUSE_STATE_PAUSED,				// paused
	PAUSE_STATE_RESUMING			// running again at the next frame
};

// FUNCTION: STBC 0x006CDAF0 ??_GTGApplication@@UAEPAXI@Z
class TGApplication : public NiApplication {
public:
							TGApplication( const char *caption, unsigned int width, unsigned int height,
										   unsigned int menuID, unsigned int numStatusPanes, unsigned int bitDepth );
	virtual					~TGApplication();

	// NiApplication
	virtual bool			OnWindowResize( int width, int height, unsigned int sizeType, HWND window );
	virtual bool			VirtualSlot21( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			VirtualSlot22( unsigned int unknown1, unsigned int unknown2 );
	virtual bool			VirtualSlot24( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 );
	virtual bool			VirtualSlot25( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 );
	virtual bool			VirtualSlot26( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 );
	virtual bool			OnDefault( NiEventRecord *event );
	virtual void			OnIdle();
	virtual void			MeasureTime();

	// asks for the game to pause or to run again
	virtual void			Pause( bool pause );
	// the pause has taken effect; nothing to do here (the copy the linker
	// kept comes from UtopiaApp's unit)
	// FUNCTION: STBC 0x00439af0 ?OnPause@TGApplication@@UAEXXZ @game/UtopiaApp
	virtual void			OnPause() {}
	// the pause has ended
	virtual void			OnUnpause();
	virtual bool			VirtualSlot42( int unknown1, unsigned int unknown2 );

	// FUNCTION: STBC 0x005af400 ?GetGameTime@TGApplication@@QAEMXZ @game/ShipClass
	float					GetGameTime( void ) { return gameTime; }
	void					SetGameTime( float gameTime_ ) { gameTime = gameTime_; }
	float					GetRealTime( void ) { return currentTime; }
	bool					IsGamePaused( void ) { return pauseState == PAUSE_STATE_PAUSED || pauseState == PAUSE_STATE_RESUMING; }
	void					SetTimeRate( float timeRate_ ) { timeRate = timeRate_; }
	// The game time the last frame advanced, at most the time rate
	// (invented name).
	float					GetGameFrameStep( void ) { return gameFrameTime <= timeRate ? gameFrameTime : timeRate; }
	// The saved game restores the step (invented name).
	void					SetGameFrameTime( float gameFrameTime_ ) { gameFrameTime = gameFrameTime_; }
	void					SetTimeScale( float timeScale_ ) { timeScale = timeScale_; }

	// Looks for the file on each CD-ROM drive: the letter of the drive that
	// has it, or 0 (UtopiaModule_CDCheck calls it on the application).
	char					CDCheck( char *file );

protected:
	bool					NonClientToClient( int &x, int &y );

	int						unknown7C;				// 0x7C
	HMODULE					vtuneLibrary;			// 0x80 "vtuneapi.dll"
	FARPROC					vtResumeSampling;		// 0x84 "VtResumeSampling" (the constructor looks it up first)
	FARPROC					vtPauseSampling;		// 0x88 "VtPauseSampling"
	pauseState_t			pauseState;				// 0x8C
	float					gameTime;				// 0x90 seconds of game time
	float					gameFrameTime;			// 0x94 the game time the last frame advanced
	float					timeSkipped;			// 0x98 frame time to leave out of the next step
	float					timeRate;				// 0x9C the longest step of game time in a frame
	float					timeScale;				// 0xA0 game seconds per real second
	bool					frameRendered;			// 0xA4 the last idle turn rendered a frame
};

// The working directory at startup, ending in a backslash, and the INI file
// the window size and renderer come from: the working directory, the
// caption and ".ini" (invented names; the constructor fills both, and
// TGIconGroup::LoadIconTexture reads the directory).
extern char workingDirectory[MAX_PATH];
extern char iniFileName[MAX_PATH];

#endif /* !__TGAPPLICATION_H__ */
