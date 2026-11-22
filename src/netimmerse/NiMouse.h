/*
===============================================================================

	NiMouse.h

	The application framework's mouse: the state of its three buttons, the
	cursor position and the modifier keys of a mouse message.

	Evidence: NiApplication's constructor (0x7B7180) allocates it (3 bytes)
	and calls the constructor (0x7BCF50), which clears three flags.
	0x7BCF60 turns a mouse message's MK_ flags and the keyboard's Alt and
	Windows keys into modifier bits; 0x7BCFF0 and 0x7BD090 read the cursor
	with GetCursorPos and ScreenToClient. The class name is invented in the
	framework's Ni style; no string names it.

===============================================================================
*/

#ifndef __NIMOUSE_H__
#define __NIMOUSE_H__

#include <windows.h>

class NiMouse {
public:
							NiMouse();
							~NiMouse() {}

	// the modifier bits of a mouse message's MK_ flags: 1 Control, 2 Alt,
	// 4 Windows (from the keyboard), 8 Shift, 0x10 left, 0x20 right and
	// 0x40 middle button
	unsigned int			GetModifiers( unsigned int keyState );

	// buttons 1 to 3; button 0 is never down
	bool					IsButtonDown( int button );
	void					ButtonDown( int button );
	void					ButtonUp( int button );

	// the cursor over the window, from 0 to 1 left to right and bottom to
	// top, and in pixels (the client area unless the application is
	// fullscreen)
	void					GetPosition( float &x, float &y, HWND window );
	void					GetPosition( int &x, int &y, HWND window );

protected:
	bool					buttonsDown[3];			// 0x00 left, middle, right
};

#endif /* !__NIMOUSE_H__ */
