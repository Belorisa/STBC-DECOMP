// NiKeyboard.cpp -- the application framework's keyboard

#include "NiRTLib.h"
#include <string.h>
#include <windows.h>

#include "NiKeyboard.h"
#include "NiMain.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007bde20 _$E394
// FUNCTION: STBC 0x007bdd90 _$E17
// FUNCTION: STBC 0x007bddb0 _$E15
// FUNCTION: STBC 0x007bddc0 _$E23
// FUNCTION: STBC 0x007bdde0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007bddf0 _$E31
// FUNCTION: STBC 0x007bde00 _$E35
// FUNCTION: STBC 0x007bde10 _$E46
// FUNCTION: STBC 0x007bde40 _$E50
// FUNCTION: STBC 0x007bde50 _$E54
// FUNCTION: STBC 0x007bde60 _$E58
// FUNCTION: STBC 0x007bde70 _$E182
// FUNCTION: STBC 0x007bde80 _$E124
// FUNCTION: STBC 0x007bde90 _$E137
// FUNCTION: STBC 0x007bdea0 _$E187
// FUNCTION: STBC 0x007bdeb0 _$E192
// FUNCTION: STBC 0x007bdec0 _$E196
// FUNCTION: STBC 0x007bded0 _$E200
// FUNCTION: STBC 0x007bdee0 _$E207
// FUNCTION: STBC 0x007bdef0 _$E232
// FUNCTION: STBC 0x007bdf00 _$E236
// FUNCTION: STBC 0x007bdf10 _$E82
// FUNCTION: STBC 0x007bdf20 _$E218
// FUNCTION: STBC 0x007bdf30 _$E225
// FUNCTION: STBC 0x007bdf40 _$E248
// FUNCTION: STBC 0x007bdf50 _$E252
// FUNCTION: STBC 0x007bdf60 _$E292
// FUNCTION: STBC 0x007bdf70 _$E298
// FUNCTION: STBC 0x007bdf80 _$E240
// FUNCTION: STBC 0x007bdf90 _$E244
// FUNCTION: STBC 0x007bdfa0 _$E256
// FUNCTION: STBC 0x007bdfb0 _$E260
// FUNCTION: STBC 0x007bdfc0 _$E264
// FUNCTION: STBC 0x007bdfd0 _$E268
// FUNCTION: STBC 0x007bdfe0 _$E272
// FUNCTION: STBC 0x007bdff0 _$E276
// FUNCTION: STBC 0x007be000 _$E280
// FUNCTION: STBC 0x007be010 _$E284
// FUNCTION: STBC 0x007be020 _$E87
// FUNCTION: STBC 0x007be030 _$E288
// FUNCTION: STBC 0x007be040 _$E92
// FUNCTION: STBC 0x007be050 _$E302
// FUNCTION: STBC 0x007be060 _$E306
// FUNCTION: STBC 0x007be070 _$E310
// FUNCTION: STBC 0x007be080 _$E314
// FUNCTION: STBC 0x007be090 _$E101
// FUNCTION: STBC 0x007be0a0 _$E318
// FUNCTION: STBC 0x007be0b0 _$E322
// FUNCTION: STBC 0x007be0c0 _$E326
// FUNCTION: STBC 0x007be0d0 _$E107
// FUNCTION: STBC 0x007be0e0 _$E112
// FUNCTION: STBC 0x007be0f0 _$E142
// FUNCTION: STBC 0x007be100 _$E330
// FUNCTION: STBC 0x007be110 _$E147
// FUNCTION: STBC 0x007be120 _$E334
// FUNCTION: STBC 0x007be130 _$E338
// FUNCTION: STBC 0x007be140 _$E152
// FUNCTION: STBC 0x007be150 _$E342
// FUNCTION: STBC 0x007be160 _$E117
// FUNCTION: STBC 0x007be170 _$E346
// FUNCTION: STBC 0x007be180 _$E350
// FUNCTION: STBC 0x007be190 _$E157
// FUNCTION: STBC 0x007be1a0 _$E162
// FUNCTION: STBC 0x007be1b0 _$E354
// FUNCTION: STBC 0x007be1c0 _$E358
// FUNCTION: STBC 0x007be1d0 _$E362
// FUNCTION: STBC 0x007be1e0 _$E366
// FUNCTION: STBC 0x007be1f0 _$E370
// FUNCTION: STBC 0x007be200 _$E374
// FUNCTION: STBC 0x007be210 _$E378
// FUNCTION: STBC 0x007be220 _$E382
// FUNCTION: STBC 0x007be230 _$E167
// FUNCTION: STBC 0x007be240 _$E386
// FUNCTION: STBC 0x007be250 _$E172
// FUNCTION: STBC 0x007be260 _$E177

// DATA: STBC 0x9A0F08
unsigned int *NiKeyboard::keyMap;

/*
================
NiKeyboard::NiKeyboard

Reads the toggle of caps lock. The first keyboard made fills in the key
table: most virtual-key codes are their own key code, escape, num lock,
scroll lock and the punctuation keys have codes of their own.
================
*/
// FUNCTION: STBC 0x7BD5F0
NiKeyboard::NiKeyboard() {
	memset( keysDown, 0, sizeof( keysDown ) );

	BYTE keyState[256];
	GetKeyboardState( keyState );
	capsLock = keyState[VK_CAPITAL] != 0;

	if ( !keyMap ) {
		keyMap = new unsigned int[256];
		memset( keyMap, 0, 256 * sizeof( unsigned int ) );
		keyMap[VK_BACK] = 8;
		keyMap[VK_TAB] = 9;
		keyMap[VK_RETURN] = 13;
		keyMap[VK_SHIFT] = 16;
		keyMap[VK_CONTROL] = 17;
		keyMap[VK_MENU] = 18;
		keyMap[VK_PAUSE] = 19;
		keyMap[VK_CAPITAL] = 20;
		keyMap[VK_ESCAPE] = 10;
		keyMap[VK_SPACE] = 32;
		keyMap[VK_PRIOR] = 33;
		keyMap[VK_NEXT] = 34;
		keyMap[VK_END] = 35;
		keyMap[VK_HOME] = 36;
		keyMap[VK_LEFT] = 37;
		keyMap[VK_UP] = 38;
		keyMap[VK_RIGHT] = 39;
		keyMap[VK_DOWN] = 40;
		keyMap[VK_SNAPSHOT] = 44;
		keyMap[VK_INSERT] = 45;
		keyMap[VK_DELETE] = 46;
		keyMap['0'] = 48;
		keyMap['1'] = 49;
		keyMap['2'] = 50;
		keyMap['3'] = 51;
		keyMap['4'] = 52;
		keyMap['5'] = 53;
		keyMap['6'] = 54;
		keyMap['7'] = 55;
		keyMap['8'] = 56;
		keyMap['9'] = 57;
		keyMap['A'] = 65;
		keyMap['B'] = 66;
		keyMap['C'] = 67;
		keyMap['D'] = 68;
		keyMap['E'] = 69;
		keyMap['F'] = 70;
		keyMap['G'] = 71;
		keyMap['H'] = 72;
		keyMap['I'] = 73;
		keyMap['J'] = 74;
		keyMap['K'] = 75;
		keyMap['L'] = 76;
		keyMap['M'] = 77;
		keyMap['N'] = 78;
		keyMap['O'] = 79;
		keyMap['P'] = 80;
		keyMap['Q'] = 81;
		keyMap['R'] = 82;
		keyMap['S'] = 83;
		keyMap['T'] = 84;
		keyMap['U'] = 85;
		keyMap['V'] = 86;
		keyMap['W'] = 87;
		keyMap['X'] = 88;
		keyMap['Y'] = 89;
		keyMap['Z'] = 90;
		keyMap[VK_LWIN] = 91;
		keyMap[VK_RWIN] = 92;
		keyMap[VK_APPS] = 93;
		keyMap[VK_NUMPAD0] = 96;
		keyMap[VK_NUMPAD1] = 97;
		keyMap[VK_NUMPAD2] = 98;
		keyMap[VK_NUMPAD3] = 99;
		keyMap[VK_NUMPAD4] = 100;
		keyMap[VK_NUMPAD5] = 101;
		keyMap[VK_NUMPAD6] = 102;
		keyMap[VK_NUMPAD7] = 103;
		keyMap[VK_NUMPAD8] = 104;
		keyMap[VK_NUMPAD9] = 105;
		keyMap[VK_MULTIPLY] = 106;
		keyMap[VK_ADD] = 107;
		keyMap[VK_SUBTRACT] = 109;
		keyMap[VK_DECIMAL] = 110;
		keyMap[VK_DIVIDE] = 111;
		keyMap[VK_F1] = 112;
		keyMap[VK_F2] = 113;
		keyMap[VK_F3] = 114;
		keyMap[VK_F4] = 115;
		keyMap[VK_F5] = 116;
		keyMap[VK_F6] = 117;
		keyMap[VK_F7] = 118;
		keyMap[VK_F8] = 119;
		keyMap[VK_F9] = 120;
		keyMap[VK_F10] = 121;
		keyMap[VK_F11] = 122;
		keyMap[VK_F12] = 123;
		keyMap[VK_NUMLOCK] = 95;
		keyMap[VK_SCROLL] = 124;
		keyMap[0xBA /* ';' */] = 21;
		keyMap[0xBB /* '=' */] = 22;
		keyMap[0xBC /* ',' */] = 23;
		keyMap[0xBD /* '-' */] = 24;
		keyMap[0xBE /* '.' */] = 25;
		keyMap[0xBF /* '/' */] = 26;
		keyMap[0xC0 /* '`' */] = 27;
		keyMap[0xDB /* '[' */] = 28;
		keyMap[0xDC /* '\' */] = 29;
		keyMap[0xDD /* ']' */] = 30;
		keyMap[0xDE /* "'" */] = 31;
	}
}

/*
================
NiKeyboard::~NiKeyboard

Bug: the key table all keyboards share is freed and left dangling.
================
*/
// FUNCTION: STBC 0x7BDC70
NiKeyboard::~NiKeyboard() {
	if ( keyMap ) {
		delete [] keyMap;
	}
}

/*
================
NiKeyboard::GetKeyCode
================
*/
// FUNCTION: STBC 0x7BDC90
unsigned int NiKeyboard::GetKeyCode( unsigned int virtualKey ) {
	return keyMap[virtualKey & 0xFF];
}

/*
================
NiKeyboard::GetModifiers
================
*/
// FUNCTION: STBC 0x7BDCB0
unsigned int NiKeyboard::GetModifiers( unsigned int unused ) {
	unsigned int modifiers = 0;
	if ( keysDown[VK_CONTROL >> 3] & ( 1 << ( VK_CONTROL & 7 ) ) ) {
		modifiers = 1;
	}
	if ( keysDown[VK_MENU >> 3] & ( 1 << ( VK_MENU & 7 ) ) ) {
		modifiers |= 2;
	}
	if ( keysDown[VK_LWIN >> 3] & ( ( 1 << ( VK_LWIN & 7 ) ) | ( 1 << ( VK_RWIN & 7 ) ) ) ) {
		modifiers |= 4;
	}
	if ( keysDown[VK_SHIFT >> 3] & ( 1 << ( VK_SHIFT & 7 ) ) ) {
		modifiers |= 8;
	}
	if ( capsLock ) {
		modifiers |= 0x10;
	}
	return modifiers;
}

/*
================
NiKeyboard::IsKeyDown

Caps lock counts as down while it is toggled on. Key 0 is never down.
================
*/
// FUNCTION: STBC 0x7BDCF0
bool NiKeyboard::IsKeyDown( int key ) {
	if ( key ) {
		if ( ( keysDown[key >> 3] >> ( key & 7 ) ) & 1 ) {
			return true;
		}
		if ( key == VK_CAPITAL ) {
			return capsLock;
		}
	}
	return false;
}

/*
================
NiKeyboard::KeyDown
================
*/
// FUNCTION: STBC 0x7BDD30
void NiKeyboard::KeyDown( int key ) {
	keysDown[key >> 3] |= 1 << ( key & 7 );
}

/*
================
NiKeyboard::KeyUp
================
*/
// FUNCTION: STBC 0x7BDD50
void NiKeyboard::KeyUp( int key ) {
	keysDown[key >> 3] &= ~( 1 << ( key & 7 ) );
	if ( key == VK_CAPITAL ) {
		capsLock = !capsLock;
	}
}
