// NiGamePad.cpp -- the application framework's game pad

#include "NiRTLib.h"
#include "NiGamePad.h"
#include "NiMain.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007bf000 _$E393
// FUNCTION: STBC 0x007bef70 _$E17
// FUNCTION: STBC 0x007bef90 _$E15
// FUNCTION: STBC 0x007befa0 _$E23
// FUNCTION: STBC 0x007befc0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007befd0 _$E30
// FUNCTION: STBC 0x007befe0 _$E34
// FUNCTION: STBC 0x007beff0 _$E45
// FUNCTION: STBC 0x007bf020 _$E49
// FUNCTION: STBC 0x007bf030 _$E53
// FUNCTION: STBC 0x007bf040 _$E57
// FUNCTION: STBC 0x007bf050 _$E181
// FUNCTION: STBC 0x007bf060 _$E123
// FUNCTION: STBC 0x007bf070 _$E136
// FUNCTION: STBC 0x007bf080 _$E186
// FUNCTION: STBC 0x007bf090 _$E191
// FUNCTION: STBC 0x007bf0a0 _$E195
// FUNCTION: STBC 0x007bf0b0 _$E199
// FUNCTION: STBC 0x007bf0c0 _$E206
// FUNCTION: STBC 0x007bf0d0 _$E231
// FUNCTION: STBC 0x007bf0e0 _$E235
// FUNCTION: STBC 0x007bf0f0 _$E81
// FUNCTION: STBC 0x007bf100 _$E217
// FUNCTION: STBC 0x007bf110 _$E224
// FUNCTION: STBC 0x007bf120 _$E247
// FUNCTION: STBC 0x007bf130 _$E251
// FUNCTION: STBC 0x007bf140 _$E291
// FUNCTION: STBC 0x007bf150 _$E297
// FUNCTION: STBC 0x007bf160 _$E239
// FUNCTION: STBC 0x007bf170 _$E243
// FUNCTION: STBC 0x007bf180 _$E255
// FUNCTION: STBC 0x007bf190 _$E259
// FUNCTION: STBC 0x007bf1a0 _$E263
// FUNCTION: STBC 0x007bf1b0 _$E267
// FUNCTION: STBC 0x007bf1c0 _$E271
// FUNCTION: STBC 0x007bf1d0 _$E275
// FUNCTION: STBC 0x007bf1e0 _$E279
// FUNCTION: STBC 0x007bf1f0 _$E283
// FUNCTION: STBC 0x007bf200 _$E86
// FUNCTION: STBC 0x007bf210 _$E287
// FUNCTION: STBC 0x007bf220 _$E91
// FUNCTION: STBC 0x007bf230 _$E301
// FUNCTION: STBC 0x007bf240 _$E305
// FUNCTION: STBC 0x007bf250 _$E309
// FUNCTION: STBC 0x007bf260 _$E313
// FUNCTION: STBC 0x007bf270 _$E100
// FUNCTION: STBC 0x007bf280 _$E317
// FUNCTION: STBC 0x007bf290 _$E321
// FUNCTION: STBC 0x007bf2a0 _$E325
// FUNCTION: STBC 0x007bf2b0 _$E106
// FUNCTION: STBC 0x007bf2c0 _$E111
// FUNCTION: STBC 0x007bf2d0 _$E141
// FUNCTION: STBC 0x007bf2e0 _$E329
// FUNCTION: STBC 0x007bf2f0 _$E146
// FUNCTION: STBC 0x007bf300 _$E333
// FUNCTION: STBC 0x007bf310 _$E337
// FUNCTION: STBC 0x007bf320 _$E151
// FUNCTION: STBC 0x007bf330 _$E341
// FUNCTION: STBC 0x007bf340 _$E116
// FUNCTION: STBC 0x007bf350 _$E345
// FUNCTION: STBC 0x007bf360 _$E349
// FUNCTION: STBC 0x007bf370 _$E156
// FUNCTION: STBC 0x007bf380 _$E161
// FUNCTION: STBC 0x007bf390 _$E353
// FUNCTION: STBC 0x007bf3a0 _$E357
// FUNCTION: STBC 0x007bf3b0 _$E361
// FUNCTION: STBC 0x007bf3c0 _$E365
// FUNCTION: STBC 0x007bf3d0 _$E369
// FUNCTION: STBC 0x007bf3e0 _$E373
// FUNCTION: STBC 0x007bf3f0 _$E377
// FUNCTION: STBC 0x007bf400 _$E381
// FUNCTION: STBC 0x007bf410 _$E166
// FUNCTION: STBC 0x007bf420 _$E385
// FUNCTION: STBC 0x007bf430 _$E171
// FUNCTION: STBC 0x007bf440 _$E176

/*
================
NiGamePad::NiGamePad

Sixteen buttons, none down, and two centred sticks.
================
*/
// FUNCTION: STBC 0x7BECF0
NiGamePad::NiGamePad() {
	numButtons = 16;
	numSticks = 2;
	buttons = new Button *[numButtons];
	sticks = new Stick *[numSticks];

	unsigned int i;
	for ( i = 0; i < numButtons; i++ ) {
		buttons[i] = new Button( 1 << i );
	}
	for ( i = 0; i < numSticks; i++ ) {
		sticks[i] = new Stick;
	}
	unknown08 = 0;
	unknown04 = 0;
	unknown00 = 0;
}

/*
================
NiGamePad::~NiGamePad
================
*/
// FUNCTION: STBC 0x7BEDA0
NiGamePad::~NiGamePad() {
	unsigned int i;

	if ( buttons ) {
		for ( i = 0; i < numButtons; i++ ) {
			delete buttons[i];
		}
		delete [] buttons;
	}
	if ( sticks ) {
		for ( i = 0; i < numSticks; i++ ) {
			delete sticks[i];
		}
		delete [] sticks;
	}
}

/*
================
NiGamePad::Unknown7BEE10

Nothing in the game calls this or the queries after it; the linker kept
them. Their names are not known.
================
*/
// FUNCTION: STBC 0x7BEE10
bool NiGamePad::Unknown7BEE10() {
	return false;
}

/*
================
NiGamePad::Unknown7BEE20
================
*/
// FUNCTION: STBC 0x7BEE20
bool NiGamePad::Unknown7BEE20() {
	return false;
}

/*
================
NiGamePad::Unknown7BEE30
================
*/
// FUNCTION: STBC 0x7BEE30
int NiGamePad::Unknown7BEE30( int ) {
	return 0;
}

/*
================
NiGamePad::GetStick

The stick at index, NULL outside the first four (name invented).
================
*/
// FUNCTION: STBC 0x7BEE40
NiGamePad::Stick *NiGamePad::GetStick( int index ) {
	Stick *stick = 0;
	if ( index > -1 && index < 4 ) {
		stick = sticks[index];
	}
	return stick;
}

/*
================
NiGamePad::Unknown7BEE60
================
*/
// FUNCTION: STBC 0x7BEE60
int NiGamePad::Unknown7BEE60( int ) {
	return 0;
}

/*
================
NiGamePad::Unknown7BEE70

Sets the first word of the button at index and returns its old value; 0
for an index outside the sixteen buttons.
================
*/
// FUNCTION: STBC 0x7BEE70
int NiGamePad::Unknown7BEE70( int index, int value ) {
	int old = 0;
	if ( index >= 0 && index <= 15 ) {
		Button *button = buttons[index];
		if ( button ) {
			old = button->unknown00;
			button->unknown00 = value;
		}
	}
	return old;
}

/*
================
NiGamePad::Unknown7BEEA0

Sets the first word of count buttons from first.
================
*/
// FUNCTION: STBC 0x7BEEA0
void NiGamePad::Unknown7BEEA0( int value, int first, unsigned int count ) {
	for ( int i = first; i < first + count; i++ ) {
		if ( i >= 0 && i <= 15 ) {
			Button *button = buttons[i];
			if ( button ) {
				button->unknown00 = value;
			}
		}
	}
}

/*
================
NiGamePad::IsButtonDown

Whether the button at index is down; with a non-zero mask, a button
outside the mask reads as up (name invented).
================
*/
// FUNCTION: STBC 0x7BEEE0
bool NiGamePad::IsButtonDown( int index, unsigned int *mask ) {
	bool down = false;
	if ( index >= 0 && index <= 15 ) {
		Button *button = buttons[index];
		if ( button ) {
			if ( mask && *mask && !( button->unknown0C & *mask ) ) {
				return false;
			}
			down = button->down;
		}
	}
	return down;
}

/*
================
NiGamePad::Unknown7BEF20
================
*/
// FUNCTION: STBC 0x7BEF20
void NiGamePad::Unknown7BEF20( int ) {
}

/*
================
NiGamePad::Unknown7BEF30
================
*/
// FUNCTION: STBC 0x7BEF30
void NiGamePad::Unknown7BEF30( int ) {
}

/*
================
NiGamePad::Unknown7BEF40
================
*/
// FUNCTION: STBC 0x7BEF40
void NiGamePad::Unknown7BEF40( int value ) {
	unknown00 = value;
}

/*
================
NiGamePad::Unknown7BEF50
================
*/
// FUNCTION: STBC 0x7BEF50
bool NiGamePad::Unknown7BEF50() {
	return false;
}

/*
================
NiGamePad::Unknown7BEF60
================
*/
// FUNCTION: STBC 0x7BEF60
bool NiGamePad::Unknown7BEF60( int, int, int ) {
	return false;
}
