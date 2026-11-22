/*
===============================================================================

	NiMessageLoop.h

	The application framework's message loop: it runs the application until
	WM_QUIT and returns the exit code, and asks the main window to close. It
	works on the application instance and has no state of its own.

	Evidence: NiApplication's constructor (0x7B7180) allocates it (1 byte)
	and calls the constructor (0x7BA590), which does nothing. 0x7BA5A0 calls
	the application's Process until it returns false and returns its exit
	code (WinMain, 0x86EFF0, calls it); 0x7BA5C0 posts WM_DESTROY to the
	main window. The window procedure (0x7BA5E0) follows in the same unit;
	WinMain registers the window class with it. The class and method names
	are invented; no string names them.

===============================================================================
*/

#ifndef __NIMESSAGELOOP_H__
#define __NIMESSAGELOOP_H__

#include <windows.h>

class NiMessageLoop {
public:
							NiMessageLoop();

	int						Run();
	void					Quit();

	// the application window's procedure: hands the messages to the
	// application's slots, keeping the keyboard and mouse state, and
	// passes what they do not handle to DefWindowProc
	static LRESULT CALLBACK	WndProc( HWND window, UINT message, WPARAM wParam, LPARAM lParam );
};

#endif /* !__NIMESSAGELOOP_H__ */
