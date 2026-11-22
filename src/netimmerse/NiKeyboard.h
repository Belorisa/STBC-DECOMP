/*
===============================================================================

	NiKeyboard.h

	The application framework's keyboard: which keys are down, one bit per
	key code, and the caps-lock toggle. A table shared by all instances
	maps the Windows virtual-key codes to the framework's key codes.

	Evidence: NiApplication's constructor (0x7B7180) allocates it (0x11
	bytes) and calls the constructor (0x7BD5F0), which reads the toggle of
	VK_CAPITAL with GetKeyboardState and, on first use, allocates and
	fills the 256-entry key table (0x9A0F08). The destructor (0x7BDC70)
	frees the table. 0x7BDCF0 tests a key's bit, 0x7BDD30 sets it and
	0x7BDD50 clears it (toggling caps lock for key 0x14); the mouse's
	modifier query (0x7BCF60) asks it for the Alt and Windows keys. The
	class name is invented in the framework's Ni style; no string names
	it.

===============================================================================
*/

#ifndef __NIKEYBOARD_H__
#define __NIKEYBOARD_H__

class NiKeyboard {
public:
							NiKeyboard();
							~NiKeyboard();

	// the framework's key code of a virtual-key code
	unsigned int			GetKeyCode( unsigned int virtualKey );
	// the modifier bits of the keys down: 1 Control, 2 Alt, 4 Windows,
	// 8 Shift, 0x10 caps lock (the argument is not read)
	unsigned int			GetModifiers( unsigned int unused );

	bool					IsKeyDown( int key );
	void					KeyDown( int key );
	// releasing caps lock toggles it
	void					KeyUp( int key );

protected:
	bool					capsLock;				// 0x00 the caps-lock toggle
	char					keysDown[16];			// 0x01 one bit per key code

	static unsigned int *	keyMap;					// virtual-key code to key code
};

#endif /* !__NIKEYBOARD_H__ */
