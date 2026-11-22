/*
===============================================================================

	NiGamePad.h

	The application framework's game pad: sixteen buttons and two sticks.

	Evidence: the constructor (0x7BECF0) allocates sixteen 0x10-byte button
	records, each with its mask 1 << index, and two 2-byte stick records
	centred at 0x7F; the destructor (0x7BEDA0) frees both arrays. Only
	NiApplication's destructor (0x7B7390) deletes the one at +0x24; the
	framework itself never creates it. The class name is invented in the
	framework's Ni style; no string names it.

===============================================================================
*/

#ifndef __NIGAMEPAD_H__
#define __NIGAMEPAD_H__

class NiGamePad {
public:
							NiGamePad();
							~NiGamePad();

	// queries nothing in the game calls (names not known)
	bool					Unknown7BEE10();
	bool					Unknown7BEE20();
	int						Unknown7BEE30( int );
	int						Unknown7BEE60( int );
	int						Unknown7BEE70( int index, int value );
	void					Unknown7BEEA0( int value, int first, unsigned int count );
	bool					IsButtonDown( int index, unsigned int *mask );
	void					Unknown7BEF20( int );
	void					Unknown7BEF30( int );
	void					Unknown7BEF40( int value );
	bool					Unknown7BEF50();
	bool					Unknown7BEF60( int, int, int );

protected:
	// a button: its mask among the buttons and whether it is down
	class Button {
	public:
						Button( unsigned int mask_ ) { unknown00 = 0; down = false; unknown0C = 0; mask = mask_; }

		int				unknown00;		// 0x00
		unsigned int	mask;			// 0x04 1 << index
		bool			down;			// 0x08
		int				unknown0C;		// 0x0C
	};

	// a stick: its two axes, 0x7F at rest
	class Stick {
	public:
						Stick() { x = 0x7F; y = 0x7F; }

		unsigned char	x;				// 0x00
		unsigned char	y;				// 0x01
	};

public:
	Stick *					GetStick( int index );

protected:

	int						unknown00;				// 0x00
	int						unknown04;				// 0x04
	int						unknown08;				// 0x08
	unsigned int			numButtons;				// 0x0C
	Button **				buttons;				// 0x10
	unsigned int			numSticks;				// 0x14
	Stick **				sticks;					// 0x18
};

#endif /* !__NIGAMEPAD_H__ */
