// NiMessageLoop.cpp -- the application framework's message loop

#include "NiRTLib.h"
#include "NiMessageLoop.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "NiKeyboard.h"
#include "NiMouse.h"
#include "NiMain.h"
#include <ctype.h>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007baf60 _$E34
// FUNCTION: STBC 0x007baf70 _$E190
// FUNCTION: STBC 0x007baf80 _$E45
// FUNCTION: STBC 0x007baf90 _$E49
// FUNCTION: STBC 0x007bafc0 _$E53
// FUNCTION: STBC 0x007bafd0 _$E57
// FUNCTION: STBC 0x007bafe0 _$E177
// FUNCTION: STBC 0x007baff0 _$E119
// FUNCTION: STBC 0x007bb000 _$E132
// FUNCTION: STBC 0x007bb010 _$E182
// FUNCTION: STBC 0x007bb020 _$E195
// FUNCTION: STBC 0x007bb030 _$E186
// FUNCTION: STBC 0x007bb040 _$E199
// FUNCTION: STBC 0x007bb050 _$E206
// FUNCTION: STBC 0x007bb060 _$E237
// FUNCTION: STBC 0x007bb070 _$E241
// FUNCTION: STBC 0x007bb080 _$E81
// FUNCTION: STBC 0x007bb090 _$E223
// FUNCTION: STBC 0x007bb0a0 _$E230
// FUNCTION: STBC 0x007bb0b0 _$E253
// FUNCTION: STBC 0x007bb0c0 _$E257
// FUNCTION: STBC 0x007bb0d0 _$E297
// FUNCTION: STBC 0x007bb0e0 _$E303
// FUNCTION: STBC 0x007bb0f0 _$E245
// FUNCTION: STBC 0x007bb100 _$E249
// FUNCTION: STBC 0x007bb110 _$E261
// FUNCTION: STBC 0x007bb120 _$E265
// FUNCTION: STBC 0x007bb130 _$E269
// FUNCTION: STBC 0x007bb140 _$E273
// FUNCTION: STBC 0x007bb150 _$E277
// FUNCTION: STBC 0x007bb160 _$E281
// FUNCTION: STBC 0x007bb170 _$E285
// FUNCTION: STBC 0x007bb180 _$E289
// FUNCTION: STBC 0x007bb190 _$E86
// FUNCTION: STBC 0x007bb1a0 _$E293
// FUNCTION: STBC 0x007bb1b0 _$E91
// FUNCTION: STBC 0x007bb1c0 _$E307
// FUNCTION: STBC 0x007bb1d0 _$E311
// FUNCTION: STBC 0x007bb1e0 _$E315
// FUNCTION: STBC 0x007bb1f0 _$E319
// FUNCTION: STBC 0x007bb200 _$E96
// FUNCTION: STBC 0x007bb210 _$E323
// FUNCTION: STBC 0x007bb220 _$E327
// FUNCTION: STBC 0x007bb230 _$E331
// FUNCTION: STBC 0x007bb240 _$E102
// FUNCTION: STBC 0x007bb250 _$E107
// FUNCTION: STBC 0x007bb260 _$E137
// FUNCTION: STBC 0x007bb270 _$E335
// FUNCTION: STBC 0x007bb280 _$E142
// FUNCTION: STBC 0x007bb290 _$E339
// FUNCTION: STBC 0x007bb2a0 _$E343
// FUNCTION: STBC 0x007bb2b0 _$E147
// FUNCTION: STBC 0x007bb2c0 _$E347
// FUNCTION: STBC 0x007bb2d0 _$E112
// FUNCTION: STBC 0x007bb2e0 _$E351
// FUNCTION: STBC 0x007bb2f0 _$E355
// FUNCTION: STBC 0x007bb300 _$E152
// FUNCTION: STBC 0x007bb310 _$E157
// FUNCTION: STBC 0x007bb320 _$E359
// FUNCTION: STBC 0x007bb330 _$E363
// FUNCTION: STBC 0x007bb340 _$E367
// FUNCTION: STBC 0x007bb350 _$E371
// FUNCTION: STBC 0x007bb360 _$E375
// FUNCTION: STBC 0x007bb370 _$E379
// FUNCTION: STBC 0x007bb380 _$E383
// FUNCTION: STBC 0x007bb390 _$E387
// FUNCTION: STBC 0x007bb3a0 _$E162
// FUNCTION: STBC 0x007bb3b0 _$E391
// FUNCTION: STBC 0x007bb3c0 _$E167
// FUNCTION: STBC 0x007bb3d0 _$E172

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007bafa0 _$E399
// FUNCTION: STBC 0x007baf00 _$E17
// FUNCTION: STBC 0x007baf20 _$E15
// FUNCTION: STBC 0x007baf30 _$E23
// FUNCTION: STBC 0x007baf50 _$E21

/*
================
NiMessageLoop::NiMessageLoop
================
*/
// FUNCTION: STBC 0x7BA590
NiMessageLoop::NiMessageLoop() {
}

/*
================
NiMessageLoop::Run

Runs the application until WM_QUIT; the exit code is the message's wParam.
================
*/
// FUNCTION: STBC 0x7BA5A0
int NiMessageLoop::Run() {
	int exitCode;
	bool running;

	do {
		running = NiApplication::application->Process( exitCode );
	} while ( running );
	return exitCode;
}

/*
================
NiMessageLoop::Quit

Destroys the main window, which ends the loop.
================
*/
// FUNCTION: STBC 0x7BA5C0
void NiMessageLoop::Quit() {
	PostMessage( NiApplication::application->GetAppWindow()->GetWindow(), WM_DESTROY, 0, 0 );
}

/*
================
NiMessageLoop::WndProc

Each message goes to an application slot; a slot that returns false leaves
the message to DefWindowProc. Keyboard and mouse messages first update the
keyboard and mouse state and gather the modifiers, whose high word comes
from slots 16 and 23. Messages without a case go to slot 31 whole.
================
*/
// FUNCTION: STBC 0x7BA5E0
// PARKED: tail merge: retail keeps WM_MOUSEMOVE's handled-return (test al; je; return 0) apart, this build cross-jumps it into WM_RBUTTONUP's shared copy like the other cases
LRESULT CALLBACK NiMessageLoop::WndProc( HWND window, UINT message, WPARAM wParam, LPARAM lParam ) {
	NiApplication *app = NiApplication::application;
	if ( !app ) {
		return DefWindowProc( window, message, wParam, lParam );
	}

	switch ( message ) {
		case WM_CREATE:
			return app->OnWindowCreate( lParam ) ? 0 : -1;

		case WM_DESTROY:
			app->OnWindowDestroy( 0, false );
			PostQuitMessage( 0 );
			return 0;

		case WM_MOVE:
			if ( app->OnWindowMove( LOWORD( lParam ), HIWORD( lParam ), 0 ) ) {
				return 0;
			}
			break;

		case WM_SIZE:
			if ( app->OnWindowResize( LOWORD( lParam ), HIWORD( lParam ), wParam, 0 ) ) {
				return 0;
			}
			break;

		case WM_PAINT: {
			PAINTSTRUCT paint;
			HDC dc = BeginPaint( window, &paint );
			if ( app->OnWindowDraw( dc ) ) {
				EndPaint( window, &paint );
				return 0;
			}
			EndPaint( window, &paint );
			break;
		}

		case WM_ERASEBKGND:
			if ( app->OnWindowErase( window ) ) {
				return 0;
			}
			break;

		case WM_CHAR: {
			NiKeyboard *keyboard = app->GetKeyboard();
			char key = (char)wParam;
			if ( iscntrl( wParam & 0xFF ) ) {
				key = (char)keyboard->GetKeyCode( wParam );
			}
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			if ( app->VirtualSlot20( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_KEYDOWN: {
			NiKeyboard *keyboard = app->GetKeyboard();
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			unsigned int key = keyboard->GetKeyCode( wParam );
			keyboard->KeyDown( key );
			if ( app->VirtualSlot21( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_KEYUP: {
			NiKeyboard *keyboard = app->GetKeyboard();
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			unsigned int key = keyboard->GetKeyCode( wParam );
			keyboard->KeyUp( key );
			if ( app->VirtualSlot22( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_SYSKEYDOWN: {
			NiKeyboard *keyboard = app->GetKeyboard();
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = WM_SYSKEYDOWN;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			unsigned int key = keyboard->GetKeyCode( wParam );
			if ( !keyboard->IsKeyDown( VK_MENU ) ) {
				keyboard->KeyDown( key );
			}
			if ( app->VirtualSlot18( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_COMMAND:
			if ( app->OnCommand( HIWORD( wParam ), LOWORD( wParam ), lParam ) ) {
				return 0;
			}
			break;

		case WM_SYSCHAR: {
			NiKeyboard *keyboard = app->GetKeyboard();
			char key = (char)wParam;
			if ( iscntrl( wParam & 0xFF ) ) {
				key = (char)keyboard->GetKeyCode( wParam );
			}
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			if ( app->VirtualSlot17( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_SYSKEYUP: {
			NiKeyboard *keyboard = app->GetKeyboard();
			unsigned int modifiers = keyboard->GetModifiers( lParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot16( &event ) << 16;
			unsigned int key = keyboard->GetKeyCode( wParam );
			keyboard->KeyUp( key );
			if ( app->VirtualSlot19( key, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_MOUSEMOVE: {
			unsigned int modifiers = app->GetMouse()->GetModifiers( wParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot23( &event ) << 16;
			if ( app->VirtualSlot26( LOWORD( lParam ), HIWORD( lParam ), modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_LBUTTONDOWN: {
			NiMouse *mouse = app->GetMouse();
			unsigned int modifiers = mouse->GetModifiers( wParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = WM_LBUTTONDOWN;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot23( &event ) << 16;
			mouse->ButtonDown( 1 );
			if ( app->VirtualSlot24( LOWORD( lParam ), HIWORD( lParam ), 1, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_LBUTTONUP: {
			NiMouse *mouse = app->GetMouse();
			unsigned int modifiers = mouse->GetModifiers( wParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = WM_LBUTTONUP;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot23( &event ) << 16;
			mouse->ButtonUp( 1 );
			if ( app->VirtualSlot25( LOWORD( lParam ), HIWORD( lParam ), 1, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_RBUTTONDOWN: {
			NiMouse *mouse = app->GetMouse();
			unsigned int modifiers = mouse->GetModifiers( wParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = WM_RBUTTONDOWN;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot23( &event ) << 16;
			mouse->ButtonDown( 3 );
			if ( app->VirtualSlot24( LOWORD( lParam ), HIWORD( lParam ), 3, modifiers ) ) {
				return 0;
			}
			break;
		}

		case WM_RBUTTONUP: {
			NiMouse *mouse = app->GetMouse();
			unsigned int modifiers = mouse->GetModifiers( wParam );
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = WM_RBUTTONUP;
			event.wParam = wParam;
			event.lParam = lParam;
			modifiers |= app->VirtualSlot23( &event ) << 16;
			mouse->ButtonUp( 3 );
			if ( app->VirtualSlot25( LOWORD( lParam ), HIWORD( lParam ), 3, modifiers ) ) {
				return 0;
			}
			break;
		}

		default: {
			NiEventRecord event;
			event.hWnd = window;
			event.uiMsg = message;
			event.wParam = wParam;
			event.lParam = lParam;
			if ( app->OnDefault( &event ) ) {
				return 0;
			}
			break;
		}
	}

	return DefWindowProc( window, message, wParam, lParam );
}
