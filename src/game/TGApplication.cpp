/*
===============================================================================

	TGApplication.cpp

===============================================================================
*/

#include <iostream>

#include "TGApplication.h"
#include "TGInputManager.h"
#include "TGMouseEvent.h"
#include "NiFrameRate.h"
#include "TGSystemWrapperClass.h"
#include "NiAppWindow.h"
#include "NiMouse.h"

#include <string.h>
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006cce50 _$E32
// FUNCTION: STBC 0x006cce60 _$E29
// FUNCTION: STBC 0x006cce70 _$E31
// FUNCTION: STBC 0x006cce80 _$E30
// FUNCTION: STBC 0x006cce90 _$E38
// FUNCTION: STBC 0x006ccea0 _$E35
// FUNCTION: STBC 0x006cceb0 _$E37
// FUNCTION: STBC 0x006ccec0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006cced0 _$E49
// FUNCTION: STBC 0x006ccee0 _$E48
// FUNCTION: STBC 0x006ccef0 _$E205
// FUNCTION: STBC 0x006ccf00 _$E204
// FUNCTION: STBC 0x006ccf10 _$E60
// FUNCTION: STBC 0x006ccf20 _$E59
// FUNCTION: STBC 0x006ccf30 _$E64
// FUNCTION: STBC 0x006ccf40 _$E63
// FUNCTION: STBC 0x006ccf50 _$E68
// FUNCTION: STBC 0x006ccf60 _$E67
// FUNCTION: STBC 0x006ccf70 _$E72
// FUNCTION: STBC 0x006ccf80 _$E71
// FUNCTION: STBC 0x006ccf90 _$E192
// FUNCTION: STBC 0x006ccfa0 _$E191
// FUNCTION: STBC 0x006ccfb0 _$E134
// FUNCTION: STBC 0x006ccfc0 _$E133
// FUNCTION: STBC 0x006ccfd0 _$E147
// FUNCTION: STBC 0x006ccfe0 _$E146
// FUNCTION: STBC 0x006cd010 _$E210
// FUNCTION: STBC 0x006cd020 _$E209
// FUNCTION: STBC 0x006cd030 _$E201
// FUNCTION: STBC 0x006cd040 _$E200
// FUNCTION: STBC 0x006cd050 _$E214
// FUNCTION: STBC 0x006cd060 _$E213
// FUNCTION: STBC 0x006cd070 _$E221
// FUNCTION: STBC 0x006cd080 _$E220
// FUNCTION: STBC 0x006cd0d0 _$E96
// FUNCTION: STBC 0x006cd0e0 _$E95
// FUNCTION: STBC 0x006cd2f0 _$E101
// FUNCTION: STBC 0x006cd300 _$E100
// FUNCTION: STBC 0x006cd330 _$E106
// FUNCTION: STBC 0x006cd340 _$E105
// FUNCTION: STBC 0x006cd3d0 _$E111
// FUNCTION: STBC 0x006cd3e0 _$E110
// FUNCTION: STBC 0x006cd450 _$E117
// FUNCTION: STBC 0x006cd460 _$E116
// FUNCTION: STBC 0x006cd470 _$E122
// FUNCTION: STBC 0x006cd480 _$E121
// FUNCTION: STBC 0x006cd490 _$E152
// FUNCTION: STBC 0x006cd4a0 _$E151
// FUNCTION: STBC 0x006cd4d0 _$E157
// FUNCTION: STBC 0x006cd4e0 _$E156
// FUNCTION: STBC 0x006cd530 _$E162
// FUNCTION: STBC 0x006cd540 _$E161
// FUNCTION: STBC 0x006cd570 _$E127
// FUNCTION: STBC 0x006cd580 _$E126
// FUNCTION: STBC 0x006cd5d0 _$E167
// FUNCTION: STBC 0x006cd5e0 _$E166
// FUNCTION: STBC 0x006cd5f0 _$E172
// FUNCTION: STBC 0x006cd600 _$E171
// FUNCTION: STBC 0x006cd710 _$E177
// FUNCTION: STBC 0x006cd720 _$E176
// FUNCTION: STBC 0x006cd750 _$E182
// FUNCTION: STBC 0x006cd760 _$E181
// FUNCTION: STBC 0x006cd770 _$E187
// FUNCTION: STBC 0x006cd780 _$E186
// FUNCTION: STBC 0x006ccff0 _$E197
// FUNCTION: STBC 0x006cd000 _$E196
// FUNCTION: STBC 0x006cd090 _$E262
// FUNCTION: STBC 0x006cd0a0 _$E261
// FUNCTION: STBC 0x006cd0b0 _$E266
// FUNCTION: STBC 0x006cd0c0 _$E265
// FUNCTION: STBC 0x006cd0f0 _$E248
// FUNCTION: STBC 0x006cd100 _$E247
// FUNCTION: STBC 0x006cd110 _$E255
// FUNCTION: STBC 0x006cd120 _$E254
// FUNCTION: STBC 0x006cd130 _$E278
// FUNCTION: STBC 0x006cd140 _$E277
// FUNCTION: STBC 0x006cd150 _$E282
// FUNCTION: STBC 0x006cd160 _$E281
// FUNCTION: STBC 0x006cd170 _$E322
// FUNCTION: STBC 0x006cd180 _$E321
// FUNCTION: STBC 0x006cd190 _$E328
// FUNCTION: STBC 0x006cd1a0 _$E327
// FUNCTION: STBC 0x006cd1b0 _$E270
// FUNCTION: STBC 0x006cd1c0 _$E269
// FUNCTION: STBC 0x006cd1d0 _$E274
// FUNCTION: STBC 0x006cd1e0 _$E273
// FUNCTION: STBC 0x006cd1f0 _$E286
// FUNCTION: STBC 0x006cd200 _$E285
// FUNCTION: STBC 0x006cd210 _$E290
// FUNCTION: STBC 0x006cd220 _$E289
// FUNCTION: STBC 0x006cd230 _$E294
// FUNCTION: STBC 0x006cd240 _$E293
// FUNCTION: STBC 0x006cd250 _$E298
// FUNCTION: STBC 0x006cd260 _$E297
// FUNCTION: STBC 0x006cd270 _$E302
// FUNCTION: STBC 0x006cd280 _$E301
// FUNCTION: STBC 0x006cd290 _$E306
// FUNCTION: STBC 0x006cd2a0 _$E305
// FUNCTION: STBC 0x006cd2b0 _$E310
// FUNCTION: STBC 0x006cd2c0 _$E309
// FUNCTION: STBC 0x006cd2d0 _$E314
// FUNCTION: STBC 0x006cd2e0 _$E313
// FUNCTION: STBC 0x006cd310 _$E318
// FUNCTION: STBC 0x006cd320 _$E317
// FUNCTION: STBC 0x006cd350 _$E332
// FUNCTION: STBC 0x006cd360 _$E331
// FUNCTION: STBC 0x006cd370 _$E336
// FUNCTION: STBC 0x006cd380 _$E335
// FUNCTION: STBC 0x006cd390 _$E340
// FUNCTION: STBC 0x006cd3a0 _$E339
// FUNCTION: STBC 0x006cd3b0 _$E344
// FUNCTION: STBC 0x006cd3c0 _$E343
// FUNCTION: STBC 0x006cd3f0 _$E348
// FUNCTION: STBC 0x006cd400 _$E347
// FUNCTION: STBC 0x006cd410 _$E352
// FUNCTION: STBC 0x006cd420 _$E351
// FUNCTION: STBC 0x006cd430 _$E356
// FUNCTION: STBC 0x006cd440 _$E355
// FUNCTION: STBC 0x006cd4b0 _$E360
// FUNCTION: STBC 0x006cd4c0 _$E359
// FUNCTION: STBC 0x006cd4f0 _$E364
// FUNCTION: STBC 0x006cd500 _$E363
// FUNCTION: STBC 0x006cd510 _$E368
// FUNCTION: STBC 0x006cd520 _$E367
// FUNCTION: STBC 0x006cd550 _$E372
// FUNCTION: STBC 0x006cd560 _$E371
// FUNCTION: STBC 0x006cd590 _$E376
// FUNCTION: STBC 0x006cd5a0 _$E375
// FUNCTION: STBC 0x006cd5b0 _$E380
// FUNCTION: STBC 0x006cd5c0 _$E379
// FUNCTION: STBC 0x006cd610 _$E384
// FUNCTION: STBC 0x006cd620 _$E383
// FUNCTION: STBC 0x006cd630 _$E388
// FUNCTION: STBC 0x006cd640 _$E387
// FUNCTION: STBC 0x006cd650 _$E392
// FUNCTION: STBC 0x006cd660 _$E391
// FUNCTION: STBC 0x006cd670 _$E396
// FUNCTION: STBC 0x006cd680 _$E395
// FUNCTION: STBC 0x006cd690 _$E400
// FUNCTION: STBC 0x006cd6a0 _$E399
// FUNCTION: STBC 0x006cd6b0 _$E404
// FUNCTION: STBC 0x006cd6c0 _$E403
// FUNCTION: STBC 0x006cd6d0 _$E408
// FUNCTION: STBC 0x006cd6e0 _$E407
// FUNCTION: STBC 0x006cd6f0 _$E412
// FUNCTION: STBC 0x006cd700 _$E411
// FUNCTION: STBC 0x006cd730 _$E416
// FUNCTION: STBC 0x006cd740 _$E415

// NiMouse's buttons, as the message loop numbers them (invented names).
#define NIMOUSE_BUTTON_LEFT		1
#define NIMOUSE_BUTTON_MIDDLE	2
#define NIMOUSE_BUTTON_RIGHT	3

// The wheel message and its unit, which VC6's headers give only for
// Windows NT 4 and later.
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL			0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA				120
#endif

// DATA: STBC 0x00997bc0
char workingDirectory[MAX_PATH];
// DATA: STBC 0x00997cc4
char iniFileName[MAX_PATH];

/*
================
TGApplication::TGApplication

Starts the clocks, then reads the window size and the renderer from the
INI file named after the caption in the working directory, and binds the
Vtune sampling entry points when the library is there.
================
*/
// FUNCTION: STBC 0x006cd790
TGApplication::TGApplication( const char *caption, unsigned int width, unsigned int height,
							  unsigned int menuID, unsigned int numStatusPanes, unsigned int bitDepth )
	: NiApplication( caption, width, height, menuID, numStatusPanes, bitDepth ) {
	pauseState = PAUSE_STATE_RUNNING;
	gameTime = 0.0f;
	timeSkipped = 0.0f;
	frameRendered = true;
	timeRate = 1.0f;
	timeScale = 1.0f;
	clicks = -1;

	g_kSystemWrapper.GetWorkingDirectory( workingDirectory, MAX_PATH );
	workingDirectory[strlen( workingDirectory ) + 1] = '\0';
	workingDirectory[strlen( workingDirectory )] = '\\';
	strcpy( iniFileName, workingDirectory );
	strcat( iniFileName, caption );
	strcat( iniFileName, ".ini" );

	int windowWidth = g_kSystemWrapper.GetIniInt( "UI Preferences", "Window Width", width, iniFileName );
	int windowHeight = g_kSystemWrapper.GetIniInt( "UI Preferences", "Window Height", height, iniFileName );
	appWindow->SetWidth( windowWidth );
	appWindow->SetHeight( windowHeight );
	appWindow->SetParentWidth( windowWidth );
	appWindow->SetParentHeight( windowHeight );

	char rendererName[MAX_PATH];
	rendererName[0] = '\0';
	GetPrivateProfileString( "UI Preferences", "Renderer", rendererName, rendererName, MAX_PATH, iniFileName );
	if ( strcmp( rendererName, "DIRECT3D" ) == 0 ) {
		rendererType = RENDERER_DIRECT3D;
	} else if ( strcmp( rendererName, "OPENGL" ) == 0 ) {
		rendererType = RENDERER_OPENGL;
	} else if ( strcmp( rendererName, "SOFTWARE" ) == 0 ) {
		rendererType = RENDERER_SOFTWARE;
	} else if ( strcmp( rendererName, "PSX2" ) == 0 ) {
		rendererType = RENDERER_PSX2;
	} else {
		rendererType = RENDERER_DIRECT3D;
	}

	vtuneLibrary = LoadLibrary( "vtuneapi.dll" );
	if ( vtuneLibrary != NULL ) {
		vtResumeSampling = GetProcAddress( vtuneLibrary, "VtResumeSampling" );
		vtPauseSampling = GetProcAddress( vtuneLibrary, "VtPauseSampling" );
	} else {
		vtResumeSampling = NULL;
		vtPauseSampling = NULL;
	}
}

/*
================
TGApplication::OnUnpause

The pause has ended; nothing to do here.
================
*/
// FUNCTION: STBC 0x006cdae0
void TGApplication::OnUnpause() {}

/*
================
TGApplication::~TGApplication

Drop the Vtune library the application loaded, and the entry points taken from
it; the base application then tears itself down.
================
*/
// FUNCTION: STBC 0x006cdb10 ??1TGApplication@@UAE@XZ
TGApplication::~TGApplication( void ) {
	if ( vtuneLibrary != NULL ) {
		FreeLibrary( vtuneLibrary );
		vtResumeSampling = NULL;
		vtPauseSampling = NULL;
	}
}

/*
================
TGApplication::NonClientToClient

Turns a non-client mouse position into the client area's when the
application is fullscreen: less the sizing border, and the caption too
vertically (invented name). False in a window.
================
*/
// FUNCTION: STBC 0x006cdb50
bool TGApplication::NonClientToClient( int &x, int &y ) {
	if ( !fullscreen ) {
		return false;
	}
	x -= GetSystemMetrics( SM_CXSIZEFRAME );
	y -= GetSystemMetrics( SM_CYSIZEFRAME );
	y -= GetSystemMetrics( SM_CYCAPTION );
	return true;
}

/*
================
TGApplication::Pause

Ask the game to stop running, or to run again. Each request is honoured only from
one state, and moves the state one step: RUNNING starts PAUSING, PAUSED starts
RESUMING, RESUMING goes back to PAUSED, PAUSING goes back to RUNNING. A request
that does not fit the current state changes nothing.
================
*/
// FUNCTION: STBC 0x006cdb90
void TGApplication::Pause( bool pause ) {
	if ( pauseState == PAUSE_STATE_RUNNING && pause ) {
		pauseState = PAUSE_STATE_PAUSING;
		return;
	}
	if ( pauseState == PAUSE_STATE_PAUSED && !pause ) {
		pauseState = PAUSE_STATE_RESUMING;
		return;
	}
	if ( pauseState == PAUSE_STATE_PAUSING && !pause ) {
		pauseState = PAUSE_STATE_RUNNING;
		return;
	}
	if ( pauseState == PAUSE_STATE_RESUMING && pause ) {
		pauseState = PAUSE_STATE_PAUSED;
	}
}

/*
================
TGApplication::MeasureTime

The game clock: the frame time minus whatever was skipped, capped at the time
rate, scaled by the time scale, and never negative. The step is doubled until it
actually moves the accumulated game time, so a tiny step still advances it.
================
*/
// FUNCTION: STBC 0x006cdc00
void TGApplication::MeasureTime() {
	NiApplication::MeasureTime();
	if ( !IsGamePaused() ) {
		gameFrameTime = frameTime - timeSkipped;
		if ( gameFrameTime > timeRate ) {
			gameFrameTime = timeRate;
		}
		gameFrameTime = timeScale * gameFrameTime;
		timeSkipped = 0.0f;
		if ( gameFrameTime <= 0.0f ) {
			gameFrameTime = 0.0f;
		}
		float oldGameTime = gameTime;
		gameTime = gameFrameTime + gameTime;
		// a step too small to leave the stored game time is doubled until it does
		while ( gameFrameTime != 0.0f && gameTime == oldGameTime ) {
			gameFrameTime += gameFrameTime;
			gameTime = gameFrameTime + gameTime;
		}
		if ( gameTime < 0.0f ) {
			gameTime = 0.0f;
		}
		return;
	}
	// paused: the clock does not advance on this frame
	gameFrameTime = 0.0f;
	timeSkipped = 0.0f;
}

/*
================
TGApplication::OnIdle

One idle turn: measures the time, and renders a frame when the shortest frame
period has passed since the last one (frameRendered says whether it did). The
frame-rate overlay samples every turn, the input devices are refreshed and
polled, and a pause or a resume that was asked for takes effect here.
================
*/
// FUNCTION: STBC 0x006cdd20
void TGApplication::OnIdle() {
	MeasureTime();
	if ( GetCurrentTime() < minFramePeriod + lastFrame ) {
		frameRendered = false;
	} else {
		lastFrame = GetCurrentTime();
		frameRendered = true;
	}
	if ( frameRateEnabled && frameRateDisplay ) {
		frameRateDisplay->TakeSample();
		frameRateDisplay->Update();
	}
	g_kInputManager.UpdateDevices();
	g_kInputManager.Poll();
	clicks++;
	if ( pauseState == PAUSE_STATE_PAUSING ) {
		OnPause();
		pauseState = PAUSE_STATE_PAUSED;
	}
	if ( pauseState == PAUSE_STATE_RESUMING ) {
		OnUnpause();
		pauseState = PAUSE_STATE_RUNNING;
	}
}

/*
================
TGApplication::VirtualSlot21

A key went down: hand it to the input manager as a press, unless the keyboard
messages are being ignored.
================
*/
// FUNCTION: STBC 0x006cddd0
bool TGApplication::VirtualSlot21( unsigned int key, unsigned int modifiers ) {
	if ( ! ignoreKeyboardMessages ) {
		return g_kInputManager.KeyEvent( key, modifiers, TGInputManager::KS_DOWN );
	}
	return true;
}

/*
================
TGApplication::VirtualSlot22

A key came up: hand it to the input manager as both a release (KS_UP) and a
normal-up event (KS_RELEASED), and report success only if both took the key.
================
*/
// FUNCTION: STBC 0x006cde00
bool TGApplication::VirtualSlot22( unsigned int key, unsigned int modifiers ) {
	if ( ! ignoreKeyboardMessages ) {
		bool handled = true;
		handled &= g_kInputManager.KeyEvent( key, modifiers, TGInputManager::KS_UP );
		handled &= g_kInputManager.KeyEvent( key, modifiers, TGInputManager::KS_RELEASED );
		return handled;
	}
	return true;
}

/*
================
TGApplication::VirtualSlot24

A mouse button went down: the message carries no coordinates, so the press is
recorded at the position the mouse was last at, and handed to the input manager
unless the mouse messages are being ignored.
================
*/
// FUNCTION: STBC 0x006cde50
bool TGApplication::VirtualSlot24( unsigned int, unsigned int, unsigned int button, unsigned int ) {
	if ( ! ignoreMouseMessages ) {
		// the message arrives at whatever position the mouse was last at
		NiPoint2 position = lastMousePosition;
		// the message's button code maps onto the input manager's; anything
		// else gets 4, a code with no name
		int inputButton;
		switch ( button ) {
		case TGInputManager::TG_RMBUTTON:
			inputButton = TGInputManager::TG_RMBUTTON;
			break;
		case TGInputManager::TG_CMBUTTON:
			inputButton = TGInputManager::TG_CMBUTTON;
			break;
		case TGInputManager::TG_LMBUTTON:
			inputButton = TGInputManager::TG_LMBUTTON;
			break;
		default:
			inputButton = 4;
			break;
		}
		return g_kInputManager.MouseEvent( inputButton, position.x, position.y, TGInputManager::KS_DOWN );
	}
	// the mouse handler is switched off: report the message as handled
	return true;
}

/*
================
TGApplication::VirtualSlot25

A mouse button came up: the same mapping as the press, handed to the input
manager as a release unless the mouse messages are being ignored.
================
*/
// FUNCTION: STBC 0x006cdec0
bool TGApplication::VirtualSlot25( unsigned int, unsigned int, unsigned int button, unsigned int ) {
	if ( ! ignoreMouseMessages ) {
		// the message arrives at whatever position the mouse was last at
		NiPoint2 position = lastMousePosition;
		// the message's button code maps onto the input manager's; anything
		// else gets 4, a code with no name
		int inputButton;
		switch ( button ) {
		case TGInputManager::TG_RMBUTTON:
			inputButton = TGInputManager::TG_RMBUTTON;
			break;
		case TGInputManager::TG_CMBUTTON:
			inputButton = TGInputManager::TG_CMBUTTON;
			break;
		case TGInputManager::TG_LMBUTTON:
			inputButton = TGInputManager::TG_LMBUTTON;
			break;
		default:
			inputButton = 4;
			break;
		}
		return g_kInputManager.MouseEvent( inputButton, position.x, position.y, TGInputManager::KS_UP );
	}
	// the mouse handler is switched off: report the message as handled
	return true;
}

/*
================
TGApplication::VirtualSlot26

The mouse moved: handed to the input manager at the last position, with no
button, unless the mouse messages are being ignored.
================
*/
// FUNCTION: STBC 0x006cdf30
bool TGApplication::VirtualSlot26( unsigned int, unsigned int, unsigned int ) {
	if ( ! ignoreMouseMessages ) {
		NiPoint2 position = lastMousePosition;
		return g_kInputManager.MouseEvent( TGInputManager::TG_NO_BUTTONS, position.x, position.y, TGInputManager::KS_MOVE );
	}
	return true;
}

/*
================
TGApplication::VirtualSlot42

A mouse-wheel step: the scroll amount says which way the wheel turned, and the step
is handed to the input manager as a press, a release and a normal-up event on the
matching wheel key; success only when all three took it. A zero scroll does nothing.
================
*/
// FUNCTION: STBC 0x006cdf70
bool TGApplication::VirtualSlot42( int delta, unsigned int ) {
	if ( delta == 0 ) {
		return false;
	}
	int key = delta > 0 ? KEY_MOUSE_WHEEL_UP : KEY_MOUSE_WHEEL_DOWN;
	bool handled = true;
	handled &= g_kInputManager.KeyEvent( key, 0, TGInputManager::KS_DOWN );
	handled &= g_kInputManager.KeyEvent( key, 0, TGInputManager::KS_UP );
	handled &= g_kInputManager.KeyEvent( key, 0, TGInputManager::KS_RELEASED );
	return handled;
}

/*
================
TGApplication::OnWindowResize

Nothing to add here: the base application handles the new window size.
================
*/
// FUNCTION: STBC 0x006cdfd0
bool TGApplication::OnWindowResize( int width, int height, unsigned int sizeType, HWND window ) {
	return NiApplication::OnWindowResize( width, height, sizeType, window );
}

/*
================
TGApplication::OnDefault

The mouse messages the window procedure doesn't handle (Gamebryo 1.1.2:
NiApplication::OnDefault): the non-client moves and buttons, the middle button and the wheel, passed
to the mouse handlers with their modifiers.
================
*/
// FUNCTION: STBC 0x006cdff0
// DRAFT: register: the non-client cases load wParam into edi (the event register) where ours uses eax, so retail tail-merges the middle press into the button-down call; the wheel keeps wParam in ebx
bool TGApplication::OnDefault( NiEventRecord *event ) {
	int x = LOWORD( event->lParam );
	int y = HIWORD( event->lParam );
	bool inClient = NonClientToClient( x, y );

	switch ( event->uiMsg ) {
		case WM_NCLBUTTONDOWN:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot24( x, y, NIMOUSE_BUTTON_LEFT, event->wParam );
		case WM_NCLBUTTONUP:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot25( x, y, NIMOUSE_BUTTON_LEFT, event->wParam );
		case WM_NCRBUTTONDOWN:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot24( x, y, NIMOUSE_BUTTON_RIGHT, event->wParam );
		case WM_NCRBUTTONUP:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot25( x, y, NIMOUSE_BUTTON_RIGHT, event->wParam );
		case WM_NCMBUTTONDOWN:
		// BUG: the middle button's release is passed on as a press.
		case WM_NCMBUTTONUP:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot24( x, y, NIMOUSE_BUTTON_MIDDLE, event->wParam );
		case WM_NCMOUSEMOVE:
			if ( !inClient ) {
				return false;
			}
			return VirtualSlot26( x, y, 0 );
		case WM_MBUTTONDOWN: {
			NiMouse *mouse = GetMouse();
			unsigned int modifiers = mouse->GetModifiers( event->wParam );
			NiEventRecord record = *event;
			modifiers |= VirtualSlot23( &record ) << 16;
			mouse->ButtonDown( NIMOUSE_BUTTON_MIDDLE );
			return VirtualSlot24( x, y, NIMOUSE_BUTTON_MIDDLE, modifiers );
		}
		case WM_MOUSEWHEEL: {
			// BUG: the wheel's delta is the high word of wParam; dividing the
			// whole word by 0xFFFF is close to the shift but not the same.
			int delta = (int)event->wParam / WHEEL_DELTA / 0xFFFF;
			unsigned int modifiers = mouse->GetModifiers( event->wParam );
			NiEventRecord record = *event;
			modifiers |= VirtualSlot23( &record ) << 16;
			return VirtualSlot42( delta, modifiers );
		}
		case WM_MBUTTONUP: {
			NiMouse *mouse = GetMouse();
			unsigned int modifiers = mouse->GetModifiers( event->wParam );
			NiEventRecord record = *event;
			modifiers |= VirtualSlot23( &record ) << 16;
			mouse->ButtonUp( NIMOUSE_BUTTON_MIDDLE );
			return VirtualSlot25( x, y, NIMOUSE_BUTTON_MIDDLE, modifiers );
		}
	}
	return false;
}

/*
================
TGApplication::CDCheck

Looks for the file on each CD-ROM drive; answers the letter of the drive that
has it, or 0.
================
*/
// FUNCTION: STBC 0x006ce300 ?CDCheck@TGApplication@@QAEDPAD@Z
char TGApplication::CDCheck( char *file ) {
	if ( file != NULL ) {
		char drives[260];
		char path[256];
		int length = GetLogicalDriveStringsA( 0, NULL );
		GetLogicalDriveStringsA( length, drives );
		char *drive = drives;
		for ( int i = 0; i < length; i += 4, drive += 4 ) {
			if ( GetDriveTypeA( drive ) == DRIVE_CDROM ) {
				strcpy( path, drive );
				strcat( path, file );
				SetErrorMode( SEM_FAILCRITICALERRORS );
				FILE *stream = fopen( path, "r" );
				SetErrorMode( 0 );
				if ( stream != NULL ) {
					fclose( stream );
					return *drive;
				}
			}
		}
	}
	return 0;
}
