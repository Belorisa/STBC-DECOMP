// NiAppEvents.cpp -- NiApplication's event handlers, its message pump and its idle loop
//
// Gamebryo 1.1.2: NiAppEvents.cpp holds these handlers apart from NiApplication.cpp; retail
// keeps them in a unit of their own, after NiApplication's statics (0x7B8400).

#include "NiRTLib.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "NiFrameRate.h"
#include <commctrl.h>
#include "NiMain.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007b88c0 _$E17
// FUNCTION: STBC 0x007b88e0 _$E15
// FUNCTION: STBC 0x007b88f0 _$E23
// FUNCTION: STBC 0x007b8910 _$E21
// FUNCTION: STBC 0x007b8960 _$E398

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b8920 _$E34
// FUNCTION: STBC 0x007b8930 _$E190
// FUNCTION: STBC 0x007b8940 _$E45
// FUNCTION: STBC 0x007b8950 _$E49
// FUNCTION: STBC 0x007b8980 _$E53
// FUNCTION: STBC 0x007b8990 _$E57
// FUNCTION: STBC 0x007b89a0 _$E177
// FUNCTION: STBC 0x007b89b0 _$E119
// FUNCTION: STBC 0x007b89c0 _$E132
// FUNCTION: STBC 0x007b89d0 _$E182
// FUNCTION: STBC 0x007b89e0 _$E195
// FUNCTION: STBC 0x007b89f0 _$E186
// FUNCTION: STBC 0x007b8a00 _$E199
// FUNCTION: STBC 0x007b8a10 _$E206
// FUNCTION: STBC 0x007b8a20 _$E236
// FUNCTION: STBC 0x007b8a30 _$E240
// FUNCTION: STBC 0x007b8a40 _$E81
// FUNCTION: STBC 0x007b8a50 _$E222
// FUNCTION: STBC 0x007b8a60 _$E229
// FUNCTION: STBC 0x007b8a70 _$E252
// FUNCTION: STBC 0x007b8a80 _$E256
// FUNCTION: STBC 0x007b8a90 _$E296
// FUNCTION: STBC 0x007b8aa0 _$E302
// FUNCTION: STBC 0x007b8ab0 _$E244
// FUNCTION: STBC 0x007b8ac0 _$E248
// FUNCTION: STBC 0x007b8ad0 _$E260
// FUNCTION: STBC 0x007b8ae0 _$E264
// FUNCTION: STBC 0x007b8af0 _$E268
// FUNCTION: STBC 0x007b8b00 _$E272
// FUNCTION: STBC 0x007b8b10 _$E276
// FUNCTION: STBC 0x007b8b20 _$E280
// FUNCTION: STBC 0x007b8b30 _$E284
// FUNCTION: STBC 0x007b8b40 _$E288
// FUNCTION: STBC 0x007b8b50 _$E86
// FUNCTION: STBC 0x007b8b60 _$E292
// FUNCTION: STBC 0x007b8b70 _$E91
// FUNCTION: STBC 0x007b8b80 _$E306
// FUNCTION: STBC 0x007b8b90 _$E310
// FUNCTION: STBC 0x007b8ba0 _$E314
// FUNCTION: STBC 0x007b8bb0 _$E318
// FUNCTION: STBC 0x007b8bc0 _$E96
// FUNCTION: STBC 0x007b8bd0 _$E322
// FUNCTION: STBC 0x007b8be0 _$E326
// FUNCTION: STBC 0x007b8bf0 _$E330
// FUNCTION: STBC 0x007b8c00 _$E102
// FUNCTION: STBC 0x007b8c10 _$E107
// FUNCTION: STBC 0x007b8c20 _$E137
// FUNCTION: STBC 0x007b8c30 _$E334
// FUNCTION: STBC 0x007b8c40 _$E142
// FUNCTION: STBC 0x007b8c50 _$E338
// FUNCTION: STBC 0x007b8c60 _$E342
// FUNCTION: STBC 0x007b8c70 _$E147
// FUNCTION: STBC 0x007b8c80 _$E346
// FUNCTION: STBC 0x007b8c90 _$E112
// FUNCTION: STBC 0x007b8ca0 _$E350
// FUNCTION: STBC 0x007b8cb0 _$E354
// FUNCTION: STBC 0x007b8cc0 _$E152
// FUNCTION: STBC 0x007b8cd0 _$E157
// FUNCTION: STBC 0x007b8ce0 _$E358
// FUNCTION: STBC 0x007b8cf0 _$E362
// FUNCTION: STBC 0x007b8d00 _$E366
// FUNCTION: STBC 0x007b8d10 _$E370
// FUNCTION: STBC 0x007b8d20 _$E374
// FUNCTION: STBC 0x007b8d30 _$E378
// FUNCTION: STBC 0x007b8d40 _$E382
// FUNCTION: STBC 0x007b8d50 _$E386
// FUNCTION: STBC 0x007b8d60 _$E162
// FUNCTION: STBC 0x007b8d70 _$E390
// FUNCTION: STBC 0x007b8d80 _$E167
// FUNCTION: STBC 0x007b8d90 _$E172

/*
================
NiApplication::OnHighLevelEvent
================
*/
// FUNCTION: STBC 0x7B8400
bool NiApplication::OnHighLevelEvent( unsigned int unknown ) {
	return false;
}

/*
================
NiApplication::OnOSEvent
================
*/
// FUNCTION: STBC 0x7B8410
bool NiApplication::OnOSEvent( unsigned int unknown ) {
	return false;
}

/*
================
NiApplication::OnDiskEvent
================
*/
// FUNCTION: STBC 0x7B8420
bool NiApplication::OnDiskEvent( unsigned int unknown ) {
	return false;
}

/*
================
NiApplication::OnCommand
================
*/
// FUNCTION: STBC 0x7B8430
bool NiApplication::OnCommand( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 ) {
	return false;
}

/*
================
NiApplication::OnWindowDraw
================
*/
// FUNCTION: STBC 0x7B8440
bool NiApplication::OnWindowDraw( HDC dc ) {
	return false;
}

/*
================
NiApplication::OnWindowErase
================
*/
// FUNCTION: STBC 0x7B8450
bool NiApplication::OnWindowErase( HWND window ) {
	return false;
}

/*
================
NiApplication::OnWindowActivate
================
*/
// FUNCTION: STBC 0x7B8460
bool NiApplication::OnWindowActivate( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::OnWindowCreate

The window may be created.
================
*/
// FUNCTION: STBC 0x7B8470
bool NiApplication::OnWindowCreate( unsigned int unknown ) {
	return true;
}

/*
================
NiApplication::OnWindowResize

Keeps the new client size, spreads the status bar's panes evenly over the
width, and fits the render window between the top and the status bar.
================
*/
// FUNCTION: STBC 0x7B8480
bool NiApplication::OnWindowResize( int width, int height, unsigned int sizeType, HWND window ) {
	RECT rect;

	appWindow->SetParentWidth( width );
	appWindow->SetParentHeight( height );

	unsigned int numPanes = appWindow->GetNumStatusPanes();
	if ( numPanes && appWindow->GetStatusBarCreated() ) {
		HWND statusWindow = appWindow->GetStatusWindow();
		SendMessage( statusWindow, WM_SIZE, sizeType, MAKELPARAM( width, height ) );

		GetClientRect( appWindow->GetWindow(), &rect );
		int *edges = new int[numPanes];
		for ( unsigned int i = 0; i < numPanes - 1; i++ ) {
			edges[i] = ( i + 1 ) * rect.right / numPanes;
		}
		edges[numPanes - 1] = rect.right;
		SendMessage( statusWindow, SB_SETPARTS, numPanes, (LPARAM)edges );
		SendMessage( statusWindow, WM_PAINT, 0, 0 );
		delete [] edges;
	}

	HWND renderWindow = appWindow->GetRenderWindow();
	if ( renderWindow ) {
		RECT client;
		GetClientRect( appWindow->GetWindow(), &client );
		int renderWidth = client.right - client.left;
		int renderHeight = client.bottom - client.top;
		if ( appWindow->GetStatusBarCreated() ) {
			RECT status;
			GetWindowRect( appWindow->GetStatusWindow(), &status );
			renderHeight -= status.bottom - status.top;
		}
		MoveWindow( renderWindow, 0, 0, renderWidth, renderHeight, FALSE );
		if ( appWindow->GetStatusBarCreated() ) {
			SendMessage( appWindow->GetStatusWindow(), WM_PAINT, 0, 0 );
		}

		GetWindowRect( renderWindow, &rect );
		appWindow->SetWidth( rect.right - rect.left );
		appWindow->SetHeight( rect.bottom - rect.top );
	}
	return true;
}

/*
================
NiApplication::OnWindowZoom
================
*/
// FUNCTION: STBC 0x7B8660
bool NiApplication::OnWindowZoom( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::OnWindowDestroy

The renderer goes with the window.
================
*/
// FUNCTION: STBC 0x7B8670
bool NiApplication::OnWindowDestroy( HWND window, bool unknown ) {
	renderer = 0;
	return true;
}

/*
================
NiApplication::OnWindowMove
================
*/
// FUNCTION: STBC 0x7B86A0
bool NiApplication::OnWindowMove( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot16
================
*/
// FUNCTION: STBC 0x7B86B0
short NiApplication::VirtualSlot16( NiEventRecord *event ) {
	return 0;
}

/*
================
NiApplication::VirtualSlot17
================
*/
// FUNCTION: STBC 0x7B86C0
bool NiApplication::VirtualSlot17( char key, unsigned int modifiers ) {
	return false;
}

/*
================
NiApplication::VirtualSlot18
================
*/
// FUNCTION: STBC 0x7B86D0
bool NiApplication::VirtualSlot18( unsigned int key, unsigned int modifiers ) {
	return false;
}

/*
================
NiApplication::VirtualSlot19
================
*/
// FUNCTION: STBC 0x7B86E0
bool NiApplication::VirtualSlot19( unsigned int key, unsigned int modifiers ) {
	return false;
}

/*
================
NiApplication::VirtualSlot20
================
*/
// FUNCTION: STBC 0x7B86F0
bool NiApplication::VirtualSlot20( char key, unsigned int modifiers ) {
	return false;
}

/*
================
NiApplication::VirtualSlot21
================
*/
// FUNCTION: STBC 0x7B8700
bool NiApplication::VirtualSlot21( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot22
================
*/
// FUNCTION: STBC 0x7B8710
bool NiApplication::VirtualSlot22( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot23
================
*/
// FUNCTION: STBC 0x7B8720
short NiApplication::VirtualSlot23( NiEventRecord *event ) {
	return 0;
}

/*
================
NiApplication::VirtualSlot24
================
*/
// FUNCTION: STBC 0x7B8730
bool NiApplication::VirtualSlot24( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot25
================
*/
// FUNCTION: STBC 0x7B8740
bool NiApplication::VirtualSlot25( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3, unsigned int unknown4 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot26
================
*/
// FUNCTION: STBC 0x7B8750
bool NiApplication::VirtualSlot26( unsigned int unknown1, unsigned int unknown2, unsigned int unknown3 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot27
================
*/
// FUNCTION: STBC 0x7B8760
unsigned short NiApplication::VirtualSlot27( unsigned int unknown ) {
	return 0;
}

/*
================
NiApplication::VirtualSlot28
================
*/
// FUNCTION: STBC 0x7B8770
bool NiApplication::VirtualSlot28( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::VirtualSlot29
================
*/
// FUNCTION: STBC 0x7B8780
bool NiApplication::VirtualSlot29( unsigned int unknown1, unsigned int unknown2 ) {
	return false;
}

/*
================
NiApplication::Process

Handles one waiting message, or runs an idle frame when there is none.
It works on the instance pointer, not on this.

Quirk: after an idle frame exitCode takes the wParam of a message that
was never received.
================
*/
// FUNCTION: STBC 0x7B8790
bool NiApplication::Process( int &exitCode ) {
	MSG message;

	if ( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) {
		if ( message.message == WM_QUIT ) {
			return false;
		}
		if ( !TranslateAccelerator( application->appWindow->GetWindow(), accelerator, &message ) ) {
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	} else {
		application->OnIdle();
	}
	exitCode = message.wParam;
	return true;
}

/*
================
NiApplication::OnDefault

No message is handled by default (Gamebryo 1.1.2: NiApplication::OnDefault).
================
*/
// FUNCTION: STBC 0x7B8830
bool NiApplication::OnDefault( NiEventRecord *event ) {
	return false;
}

/*
================
NiApplication::OnIdle

Renders a frame once the frame period has passed: the overlay is updated,
the camera clears its port, draws and presents, and the status bar shows
the frame rate.
================
*/
// FUNCTION: STBC 0x7B8840
void NiApplication::OnIdle() {
	MeasureTime();
	if ( accumTime >= lastFrame + minFramePeriod ) {
		lastFrame = accumTime;
		if ( frameRateEnabled && frameRateDisplay ) {
			frameRateDisplay->TakeSample();
			frameRateDisplay->Update();
		}
		camera->Clear( 5 );
		camera->Click();
		camera->Unknown7E81B0();
		if ( appWindow->GetStatusBarCreated() && appWindow->GetNumStatusPanes() ) {
			DrawFrameRate();
		}
		clicks++;
	}
}
