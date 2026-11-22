// NiMouse.cpp -- the application framework's mouse

#include "NiRTLib.h"
#include "NiMouse.h"
#include "NiApplication.h"
#include "NiKeyboard.h"
#include "NiMain.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007bd190 _$E399
// FUNCTION: STBC 0x007bd110 _$E17
// FUNCTION: STBC 0x007bd130 _$E15
// FUNCTION: STBC 0x007bd140 _$E23
// FUNCTION: STBC 0x007bd160 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007bd170 _$E34
// FUNCTION: STBC 0x007bd180 _$E190
// FUNCTION: STBC 0x007bd1b0 _$E45
// FUNCTION: STBC 0x007bd1c0 _$E49
// FUNCTION: STBC 0x007bd1d0 _$E53
// FUNCTION: STBC 0x007bd1e0 _$E57
// FUNCTION: STBC 0x007bd1f0 _$E177
// FUNCTION: STBC 0x007bd200 _$E119
// FUNCTION: STBC 0x007bd210 _$E132
// FUNCTION: STBC 0x007bd220 _$E182
// FUNCTION: STBC 0x007bd230 _$E195
// FUNCTION: STBC 0x007bd240 _$E186
// FUNCTION: STBC 0x007bd250 _$E199
// FUNCTION: STBC 0x007bd260 _$E206
// FUNCTION: STBC 0x007bd270 _$E237
// FUNCTION: STBC 0x007bd280 _$E241
// FUNCTION: STBC 0x007bd290 _$E81
// FUNCTION: STBC 0x007bd2a0 _$E223
// FUNCTION: STBC 0x007bd2b0 _$E230
// FUNCTION: STBC 0x007bd2c0 _$E253
// FUNCTION: STBC 0x007bd2d0 _$E257
// FUNCTION: STBC 0x007bd2e0 _$E297
// FUNCTION: STBC 0x007bd2f0 _$E303
// FUNCTION: STBC 0x007bd300 _$E245
// FUNCTION: STBC 0x007bd310 _$E249
// FUNCTION: STBC 0x007bd320 _$E261
// FUNCTION: STBC 0x007bd330 _$E265
// FUNCTION: STBC 0x007bd340 _$E269
// FUNCTION: STBC 0x007bd350 _$E273
// FUNCTION: STBC 0x007bd360 _$E277
// FUNCTION: STBC 0x007bd370 _$E281
// FUNCTION: STBC 0x007bd380 _$E285
// FUNCTION: STBC 0x007bd390 _$E289
// FUNCTION: STBC 0x007bd3a0 _$E86
// FUNCTION: STBC 0x007bd3b0 _$E293
// FUNCTION: STBC 0x007bd3c0 _$E91
// FUNCTION: STBC 0x007bd3d0 _$E307
// FUNCTION: STBC 0x007bd3e0 _$E311
// FUNCTION: STBC 0x007bd3f0 _$E315
// FUNCTION: STBC 0x007bd400 _$E319
// FUNCTION: STBC 0x007bd410 _$E96
// FUNCTION: STBC 0x007bd420 _$E323
// FUNCTION: STBC 0x007bd430 _$E327
// FUNCTION: STBC 0x007bd440 _$E331
// FUNCTION: STBC 0x007bd450 _$E102
// FUNCTION: STBC 0x007bd460 _$E107
// FUNCTION: STBC 0x007bd470 _$E137
// FUNCTION: STBC 0x007bd480 _$E335
// FUNCTION: STBC 0x007bd490 _$E142
// FUNCTION: STBC 0x007bd4a0 _$E339
// FUNCTION: STBC 0x007bd4b0 _$E343
// FUNCTION: STBC 0x007bd4c0 _$E147
// FUNCTION: STBC 0x007bd4d0 _$E347
// FUNCTION: STBC 0x007bd4e0 _$E112
// FUNCTION: STBC 0x007bd4f0 _$E351
// FUNCTION: STBC 0x007bd500 _$E355
// FUNCTION: STBC 0x007bd510 _$E152
// FUNCTION: STBC 0x007bd520 _$E157
// FUNCTION: STBC 0x007bd530 _$E359
// FUNCTION: STBC 0x007bd540 _$E363
// FUNCTION: STBC 0x007bd550 _$E367
// FUNCTION: STBC 0x007bd560 _$E371
// FUNCTION: STBC 0x007bd570 _$E375
// FUNCTION: STBC 0x007bd580 _$E379
// FUNCTION: STBC 0x007bd590 _$E383
// FUNCTION: STBC 0x007bd5a0 _$E387
// FUNCTION: STBC 0x007bd5b0 _$E162
// FUNCTION: STBC 0x007bd5c0 _$E391
// FUNCTION: STBC 0x007bd5d0 _$E167
// FUNCTION: STBC 0x007bd5e0 _$E172

/*
================
NiMouse::NiMouse

No button is down.
================
*/
// FUNCTION: STBC 0x7BCF50
NiMouse::NiMouse() {
	buttonsDown[0] = false;
	buttonsDown[1] = false;
	buttonsDown[2] = false;
}

/*
================
NiMouse::GetModifiers
================
*/
// FUNCTION: STBC 0x7BCF60
unsigned int NiMouse::GetModifiers( unsigned int keyState ) {
	NiKeyboard *keyboard = NiApplication::application->GetKeyboard();
	unsigned int modifiers = 0;
	if ( keyState & MK_CONTROL ) {
		modifiers = 1;
	}
	if ( keyboard->IsKeyDown( VK_MENU ) ) {
		modifiers |= 2;
	}
	if ( keyboard->IsKeyDown( VK_LWIN ) || keyboard->IsKeyDown( VK_RWIN ) ) {
		modifiers |= 4;
	}
	if ( keyState & MK_SHIFT ) {
		modifiers |= 8;
	}
	if ( keyState & MK_LBUTTON ) {
		modifiers |= 0x10;
	}
	if ( keyState & MK_RBUTTON ) {
		modifiers |= 0x20;
	}
	if ( keyState & MK_MBUTTON ) {
		modifiers |= 0x40;
	}
	return modifiers;
}

/*
================
NiMouse::IsButtonDown
================
*/
// FUNCTION: STBC 0x7BCFD0
bool NiMouse::IsButtonDown( int button ) {
	if ( !button ) {
		return false;
	}
	return buttonsDown[button - 1];
}

/*
================
NiMouse::GetPosition

The cursor relative to the window rectangle. Outside fullscreen the cursor
is taken in client coordinates but the rectangle stays in screen
coordinates (a quirk of the original).
================
*/
// FUNCTION: STBC 0x7BCFF0 ?GetPosition@NiMouse@@QAEXAAM0PAUHWND__@@@Z
void NiMouse::GetPosition( float &x, float &y, HWND window ) {
	POINT point;
	GetCursorPos( &point );
	RECT rect;
	GetWindowRect( window, &rect );
	if ( !NiApplication::application->GetFullscreen() ) {
		ScreenToClient( window, &point );
	}
	x = (float)( point.x - rect.left ) / ( rect.right - rect.left );
	y = 1.0f - (float)( point.y - rect.top ) / ( rect.bottom - rect.top );
}

/*
================
NiMouse::GetPosition
================
*/
// FUNCTION: STBC 0x7BD090 ?GetPosition@NiMouse@@QAEXAAH0PAUHWND__@@@Z
void NiMouse::GetPosition( int &x, int &y, HWND window ) {
	POINT point;
	GetCursorPos( &point );
	if ( !NiApplication::application->GetFullscreen() ) {
		ScreenToClient( window, &point );
	}
	x = point.x;
	y = point.y;
}

/*
================
NiMouse::ButtonDown
================
*/
// FUNCTION: STBC 0x7BD0F0
void NiMouse::ButtonDown( int button ) {
	buttonsDown[button - 1] = true;
}

/*
================
NiMouse::ButtonUp
================
*/
// FUNCTION: STBC 0x7BD100
void NiMouse::ButtonUp( int button ) {
	buttonsDown[button - 1] = false;
}
