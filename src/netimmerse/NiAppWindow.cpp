// NiAppWindow.cpp -- the application framework's main window

#include "NiRTLib.h"
#include <windows.h>
#include <commctrl.h>
#include "NiAppWindow.h"
#include "NiApplication.h"
#include <string.h>
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007be870 _$E34
// FUNCTION: STBC 0x007be880 _$E190
// FUNCTION: STBC 0x007be8b0 _$E45
// FUNCTION: STBC 0x007be8c0 _$E49
// FUNCTION: STBC 0x007be8d0 _$E53
// FUNCTION: STBC 0x007be8e0 _$E57
// FUNCTION: STBC 0x007be8f0 _$E177
// FUNCTION: STBC 0x007be900 _$E119
// FUNCTION: STBC 0x007be910 _$E132
// FUNCTION: STBC 0x007be920 _$E182
// FUNCTION: STBC 0x007be930 _$E195
// FUNCTION: STBC 0x007be940 _$E186
// FUNCTION: STBC 0x007be950 _$E199
// FUNCTION: STBC 0x007be960 _$E206
// FUNCTION: STBC 0x007be970 _$E236
// FUNCTION: STBC 0x007be980 _$E240
// FUNCTION: STBC 0x007be990 _$E81
// FUNCTION: STBC 0x007be9a0 _$E222
// FUNCTION: STBC 0x007be9b0 _$E229
// FUNCTION: STBC 0x007be9c0 _$E252
// FUNCTION: STBC 0x007be9d0 _$E256
// FUNCTION: STBC 0x007be9e0 _$E296
// FUNCTION: STBC 0x007be9f0 _$E302
// FUNCTION: STBC 0x007bea00 _$E244
// FUNCTION: STBC 0x007bea10 _$E248
// FUNCTION: STBC 0x007bea20 _$E260
// FUNCTION: STBC 0x007bea30 _$E264
// FUNCTION: STBC 0x007bea40 _$E268
// FUNCTION: STBC 0x007bea50 _$E272
// FUNCTION: STBC 0x007bea60 _$E276
// FUNCTION: STBC 0x007bea70 _$E280
// FUNCTION: STBC 0x007bea80 _$E284
// FUNCTION: STBC 0x007bea90 _$E288
// FUNCTION: STBC 0x007beaa0 _$E86
// FUNCTION: STBC 0x007beab0 _$E292
// FUNCTION: STBC 0x007beac0 _$E91
// FUNCTION: STBC 0x007bead0 _$E306
// FUNCTION: STBC 0x007beae0 _$E310
// FUNCTION: STBC 0x007beaf0 _$E314
// FUNCTION: STBC 0x007beb00 _$E318
// FUNCTION: STBC 0x007beb10 _$E96
// FUNCTION: STBC 0x007beb20 _$E322
// FUNCTION: STBC 0x007beb30 _$E326
// FUNCTION: STBC 0x007beb40 _$E330
// FUNCTION: STBC 0x007beb50 _$E102
// FUNCTION: STBC 0x007beb60 _$E107
// FUNCTION: STBC 0x007beb70 _$E137
// FUNCTION: STBC 0x007beb80 _$E334
// FUNCTION: STBC 0x007beb90 _$E142
// FUNCTION: STBC 0x007beba0 _$E338
// FUNCTION: STBC 0x007bebb0 _$E342
// FUNCTION: STBC 0x007bebc0 _$E147
// FUNCTION: STBC 0x007bebd0 _$E346
// FUNCTION: STBC 0x007bebe0 _$E112
// FUNCTION: STBC 0x007bebf0 _$E350
// FUNCTION: STBC 0x007bec00 _$E354
// FUNCTION: STBC 0x007bec10 _$E152
// FUNCTION: STBC 0x007bec20 _$E157
// FUNCTION: STBC 0x007bec30 _$E358
// FUNCTION: STBC 0x007bec40 _$E362
// FUNCTION: STBC 0x007bec50 _$E366
// FUNCTION: STBC 0x007bec60 _$E370
// FUNCTION: STBC 0x007bec70 _$E374
// FUNCTION: STBC 0x007bec80 _$E378
// FUNCTION: STBC 0x007bec90 _$E382
// FUNCTION: STBC 0x007beca0 _$E386
// FUNCTION: STBC 0x007becb0 _$E162
// FUNCTION: STBC 0x007becc0 _$E390
// FUNCTION: STBC 0x007becd0 _$E167
// FUNCTION: STBC 0x007bece0 _$E172

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007be890 _$E398
// FUNCTION: STBC 0x007be810 _$E17
// FUNCTION: STBC 0x007be830 _$E15
// FUNCTION: STBC 0x007be840 _$E23
// FUNCTION: STBC 0x007be860 _$E21

/*
================
NiAppWindow::NiAppWindow

Keeps a copy of the caption, "NetImmerse Application" when none is given.
The window is created later.
================
*/
// FUNCTION: STBC 0x7BE270
NiAppWindow::NiAppWindow( const char *caption_, unsigned int width_, unsigned int height_,
						  unsigned int numStatusPanes_, unsigned long style_ ) {
	if ( caption_ && *caption_ ) {
		caption = new char[strlen( caption_ ) + 1];
		strcpy( caption, caption_ );
	} else {
		caption = new char[strlen( "NetImmerse Application" ) + 1];
		strcpy( caption, "NetImmerse Application" );
	}
	width = parentWidth = width_;
	height = parentHeight = height_;
	window = 0;
	renderWindow = 0;
	statusWindows = 0;
	style = style_;
	numStatusPanes = numStatusPanes_;
	statusBarCreated = false;
}

/*
================
NiAppWindow::~NiAppWindow

Takes the status bar down, shrinking the window by its height, and frees
the caption.
================
*/
// FUNCTION: STBC 0x7BE320
NiAppWindow::~NiAppWindow() {
	if ( statusBarCreated ) {
		if ( IsWindow( window ) ) {
			StatusWindowDestroy();
		}
		if ( statusWindows ) {
			delete [] statusWindows;
		}
	}
	delete [] caption;
	window = 0;
	renderWindow = 0;
}

/*
================
NiAppWindow::SetWindowCaption

Keeps a copy of the caption and shows it in the title bar (Gamebryo 1.1.2:
NiAppWindow::SetWindowCaption).
================
*/
// FUNCTION: STBC 0x7BE440
void NiAppWindow::SetWindowCaption( char *caption_ ) {
	delete [] caption;
	caption = new char[strlen( caption_ ) + 1];
	strcpy( caption, caption_ );
	SetWindowText( window, caption );
}

/*
================
NiAppWindow::StatusWindowCreate

A full screen has no status bar. Otherwise the bar is created along the
bottom of the window, which grows by its height.
================
*/
// FUNCTION: STBC 0x7BE4A0
void NiAppWindow::StatusWindowCreate() {
	if ( NiApplication::application->GetFullscreen() ) {
		numStatusPanes = 0;
	}
	if ( numStatusPanes && !statusBarCreated ) {
		INITCOMMONCONTROLSEX controls;
		controls.dwSize = sizeof( INITCOMMONCONTROLSEX );
		controls.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx( &controls );
		SetStatusWindow( CreateWindowEx( 0, STATUSCLASSNAME, "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, window, 0,
										 NiApplication::GetInstanceReference(), 0 ) );
		RECT statusRect;
		RECT windowRect;
		GetWindowRect( GetStatusWindow(), &statusRect );
		GetWindowRect( window, &windowRect );
		statusBarCreated = true;
		MoveWindow( window, windowRect.left, windowRect.top, windowRect.right - windowRect.left,
					windowRect.bottom - windowRect.top + ( statusRect.bottom - statusRect.top ), TRUE );
	}
}

/*
================
NiAppWindow::SetStatusPaneText

Sets the text of a pane of the status bar and has the bar repaint, if the
bar is up.
================
*/
// FUNCTION: STBC 0x7BE5B0
void NiAppWindow::SetStatusPaneText( unsigned int pane, const char *text ) {
	if ( pane < numStatusPanes && statusBarCreated ) {
		SendMessage( GetStatusWindow(), SB_SETTEXT, pane, (LPARAM)text );
		SendMessage( GetStatusWindow(), WM_PAINT, 0, 0 );
	}
}

/*
================
NiAppWindow::StatusWindowDestroy

Takes the status bar down and shrinks the window by its height.
================
*/
// FUNCTION: STBC 0x7BE620
void NiAppWindow::StatusWindowDestroy() {
	if ( numStatusPanes == 0 || !statusBarCreated ) {
		return;
	}
	RECT statusRect;
	RECT windowRect;
	GetWindowRect( GetStatusWindow(), &statusRect );
	GetWindowRect( window, &windowRect );
	DestroyWindow( GetStatusWindow() );
	SetStatusWindow( 0 );
	statusBarCreated = false;
	MoveWindow( window, windowRect.left, windowRect.top, windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top - ( statusRect.bottom - statusRect.top ), TRUE );
}

/*
================
NiAppWindow::CreateMainWindow

Sizes the window so that its client area is the size asked for, creates
it, the status bar and a child window over the client area, and shows it.
A full screen that is not Direct3D's takes a pop-up window and leaves the
child hidden.
================
*/
// FUNCTION: STBC 0x7BE6F0
// PARKED: register/schedule residue in the two CreateWindowEx argument lists (same calls, branches and stores)
HWND NiAppWindow::CreateMainWindow( int showCommand ) {
	RECT rect = { 0, 0, GetWidth(), GetHeight() };
	AdjustWindowRect( &rect, style, NiApplication::application->GetMenuID() );

	unsigned long childStyle = WS_CHILD;
	HWND mainWindow;
	if ( NiApplication::application->GetFullscreen() && NiApplication::application->GetRendererType() != RENDERER_DIRECT3D ) {
		mainWindow = CreateWindowEx( 0, "NetImmerse Application", caption, WS_POPUP, 0, 0, rect.right - rect.left,
									 rect.bottom - rect.top, 0, 0, NiApplication::GetInstanceReference(), 0 );
	} else {
		childStyle = WS_CHILD | WS_VISIBLE;
		mainWindow = CreateWindowEx( 0, "NetImmerse Application", caption, style, 0, 0, rect.right - rect.left,
									 rect.bottom - rect.top, 0, 0, NiApplication::GetInstanceReference(), 0 );
	}
	window = mainWindow;
	if ( numStatusPanes ) {
		StatusWindowCreate();
	}
	renderWindow = CreateWindowEx( 0, "NetImmerse Application", 0, childStyle, 0, 0, GetWidth(), GetHeight(), mainWindow, 0,
								NiApplication::GetInstanceReference(), 0 );
	ShowWindow( window, showCommand );
	UpdateWindow( window );
	return mainWindow;
}
