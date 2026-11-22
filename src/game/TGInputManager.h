/*
===============================================================================

	TGInputManager.h

	The members of TGInputManager that the SWIG wrappers TGInputManager_* call, and the
	state of the mouse cursor that they use. Two virtual functions save and
	load the cursor's move (the vtable 0x896128 the constructor 0x6E4850
	stores).

	Evidence: the startup (0x6E4D80) fills 0x548 to 0x57E with the cursor's
	state, the cursor starting in the middle of the window (0x56C and 0x570
	hold 0.5). The cursor is a position in 0 to 1 of the window: GetMouseX
	and GetMouseY scale it back by the window's client size (0x6E6310,
	0x6E6350), the deltas of the last event likewise. MoveMouseCursorTo
	(0x6E7010) sets a move of the cursor from where it is to a point over
	a time; the update (0x6E71C0) steps it along a sine curve and posts a
	mouse event at each step. Pause and resume hold the moves back and put
	the cursor where it was. The cursor (0x56C) and the delta (0x574) are
	NiPoint2s: the SWIG wrappers GetMousePos, GetMouseDelta and
	SetMousePosition copy them as one. The members before 0x548 and after
	0x59C are not decoded yet; the names of the cursor's members are
	invented.

===============================================================================
*/

#ifndef __TGINPUTMANAGER_H__
#define __TGINPUTMANAGER_H__

#include "NiPoint2.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "TGString.h"

class TGGamepad;
class TGStream;
struct IDirectInputDevice;
struct DIDEVICEOBJECTDATA;

// A keyboard layout's characters: the keys that must be held for it (the
// shift states), and the character each key gives with them. The input
// manager picks the list whose keys are all down (SetCurrentUnicodeKeyList).
// 0x28 bytes. The name is invented.
//
// Evidence: the constructor (0x6E4850) allocates 0x28 bytes, builds a
// NiTList at 0x00 and a NiTMap of 37 buckets with the vtable 0x896144 at
// 0x18; GetUnicodeKey looks keys up in the map, SetCurrentUnicodeKeyList
// matches the list against the keys held.
// FUNCTION: STBC 0x006e52b0 ??1TGUnicodeKeyList@@QAE@XZ
class TGUnicodeKeyList {
public:
					TGUnicodeKeyList( void ) : keys( 2 ) {}

	NiTList<int>		keys;			// 0x00 the keys that must be held
	NiTMap<int, int>	characters;		// 0x18 key to character
};


// What a dead key (an accent) makes of the key typed after it: the base
// character and the character it becomes (invented names).
struct TGDeadKeyCombination {
	short			base;
	short			result;
};

// A dead key and the characters it combines with, filled in the order
// RegisterDeadKey is called (invented name). 0x82 bytes.
struct TGDeadKey {
	short					deadKey;			// 0 while the entry is unused
	TGDeadKeyCombination	combinations[32];
};

#define TGINPUT_MAX_DEAD_KEYS		8

// The gamepads the startup opens.
#define TGINPUT_MAX_GAMEPADS		4

// The mouse buttons, in DirectInput's order (invented names).
#define MOUSE_BUTTON_LEFT			0
#define MOUSE_BUTTON_RIGHT			1
#define MOUSE_BUTTON_MIDDLE			2
#define TGINPUT_NUM_MOUSE_BUTTONS	3
// The buttons DirectInput reports for a mouse (DIMOUSESTATE's rgbButtons).
#define TGINPUT_MAX_MOUSE_BUTTONS	4

// The keys KeyEvent gets for the mouse buttons (invented names).
#define KEY_MOUSE_LEFT			0xF1
#define KEY_MOUSE_RIGHT			0xF2
#define KEY_MOUSE_MIDDLE		0xF3
// The keys KeyEvent gets for a mouse-wheel step (invented names).
#define KEY_MOUSE_WHEEL_UP		0xEF
#define KEY_MOUSE_WHEEL_DOWN		0xF0

class TGLocalizationDatabase;
// The assignment is the compiler's; the SWIG setter of g_kInputManager
// calls it.
// FUNCTION: STBC 0x0065b5a0 ??4TGInputManager@@QAEAAV0@ABV0@@Z @game/AppWrap
class TGInputManager {
public:
	bool	GetCenterMouse( void ) { return centerMouse; }
	void	SetCenterMouse( bool centerMouse_ ) { centerMouse = centerMouse_; }
	void	SetKeyboardInputEnabled( bool keyboardInputEnabled_ ) { keyboardInputEnabled = keyboardInputEnabled_; }
	// The mouse buttons and what happened to them (the SWIG constants
	// TGInputManager_TG_*BUTTON* and TGInputManager_KS_*, from the App
	// constant table's register stores).
	enum {
		TG_NO_BUTTONS,
		TG_LMBUTTON,
		TG_CMBUTTON,
		TG_RMBUTTON
	};
	enum {
		KS_MOVE,
		KS_DOWN,
		KS_UP,
		KS_RELEASED,			// TGInputManager_KS_NORMAL (TGKeyboardEvent.h's KS_NORMAL macro takes the name)
		KS_MOVE_DELTA
	};

					TGInputManager( void );
					~TGInputManager( void );

	virtual bool	WriteToStream( TGStream *stream );		// invented name
	virtual bool	ReadFromStream( TGStream *stream );		// invented name

	void	Poll( void );
	void	UpdateDevices( void );
	bool	KeyEvent( int, long, int );
	bool	MouseEvent( int, float, float, int );
	int	GetMouseDeltaX( void );
	int	GetMouseDeltaY( void );
	int	GetMouseX( void );
	int	GetMouseY( void );
	NiPoint2	GetMousePos( void ) { return cursor; }
	NiPoint2	GetMouseDelta( void ) { return mouseDelta; }
	void	SetMousePosition( const NiPoint2 &position ) { cursor = position; }
	bool	GamepadEvent( int, int, float, int );
	void	RegisterDeadKey( short, short, short );
	int	GetUnicodeKey( int, bool );
	int	GetScanCode( int );
	void	SetCurrentUnicodeKeyList( void );
	void	SetDisplayStringForUnicode( int unicode, TGString displayString );	// retail 0x6E5C40: the SWIG wrapper TGInputManager_SetDisplayStringForUnicode calls it
	TGString	GetDisplayStringFromUnicode( int unicode );	// retail 0x6E5DC0: the SWIG wrapper TGInputManager_GetDisplayStringFromUnicode calls it
	void	MoveMouseCursorTo( float, float, float, int );
	void	PauseMouseCursorMoveTo( void );
	void	ResumeMouseCursorMoveTo( void );

	// What the input module calls at startup (with 7), each frame and at
	// the end; not identified yet.
	void	Unknown6E4D80( int );
	void	Unknown6E71C0( void );
	void	Unknown6E5090( void );
	// Combines a dead key with the key typed after it, and posts the
	// keyboard events (invented names).
	void	HandleDeadKey( short character, int keyState, long keyData );
	void	PostKeyboardEvent( unsigned short character, int keyState, long keyData );
	// Runs the script that maps the keyboard's scan codes (KeyConfig.
	// MapScancodes; invented name).
	void	MapScancodes( void );

	// A flag per gamepad button (invented names): Poll clears a button's
	// when it is released; nothing in retail sets one or reads them.
	static bool	IsGamepadButtonFlagged( int gamepad, int button );
	static void	SetGamepadButtonFlag( int gamepad, int button, bool flag );

	// The startup (Unknown6E4D80) fills 0x04 to 0xC7: the argument, then
	// for each of the TGINPUT_MAX_GAMEPADS gamepads a set of values kept in
	// parallel arrays, and the gamepad (built from the application and the
	// gamepad's index).
	int				unknown04;				// 0x004 the startup's argument (7 from the input module)
	unsigned int	lastButtons[TGINPUT_MAX_GAMEPADS];	// 0x008 the buttons held at the last poll (invented names, to 0x0A8)
	float			lastX[TGINPUT_MAX_GAMEPADS];		// 0x018 each axis at the last poll, scaled to its maximum
	float			lastY[TGINPUT_MAX_GAMEPADS];		// 0x028
	float			lastZ[TGINPUT_MAX_GAMEPADS];		// 0x038
	float			lastRz[TGINPUT_MAX_GAMEPADS];		// 0x048
	float			lastRx[TGINPUT_MAX_GAMEPADS];		// 0x058
	float			lastRy[TGINPUT_MAX_GAMEPADS];		// 0x068
	float			lastSlider0[TGINPUT_MAX_GAMEPADS];	// 0x078
	float			lastSlider1[TGINPUT_MAX_GAMEPADS];	// 0x088
	float			lastPov[TGINPUT_MAX_GAMEPADS];	// 0x098 -1 from the startup
	int				lastPovDirections[TGINPUT_MAX_GAMEPADS];	// 0x0A8 a bit per hat direction held: up, right, down, left
	TGGamepad *		gamepads[TGINPUT_MAX_GAMEPADS];		// 0x0B8 owned (invented name)
	short			unknownC8;				// 0x0C8 the character of the last key pressed
	float			unknownCC;				// 0x0CC the application's clock when it went down
	float			lastRepeatTime;			// 0x0D0 the clock at the key's last repeat (invented name)
	NiTMap<int, int>	scanCodes;			// 0x0D4 key to scan code (identity of the key and value types unproven; invented name)
	bool			keyboardInputEnabled;				// 0x0E4 true from the constructor; KeyEvent ignores keys while it is false
	NiTList<TGUnicodeKeyList *>	keyLists;	// 0x0E8 owned; the first is the default
	TGUnicodeKeyList *	currentKeyList;		// 0x100 the list GetUnicodeKey reads by default
	NiTList<int>	keysDown;				// 0x104 the keys held (invented name); its NewItem is NiTList<int>'s (0x486880), shared with the torpedo-type list of 0x483E50 and KeyboardBinding::BindKey
	NiTMap<int, TGString>	displayStrings;		// 0x11C the string shown for a character (Set/GetDisplayStringFromUnicode; invented name)
	TGDeadKey		deadKeys[TGINPUT_MAX_DEAD_KEYS];	// 0x12C
	TGDeadKey *		pendingDeadKey;			// 0x53C the dead key typed, waiting for the next key (invented names)
	long			pendingKeyData;			// 0x540 its key data
	bool			deadKeyHeld;			// 0x544 the dead key is still down
	float			moveDuration;			// 0x548 seconds of the cursor move in progress, 0 for none
	float			moveElapsed;			// 0x54C seconds of it done
	NiPoint2		moveStart;				// 0x550 where the cursor move began (invented name)
	NiPoint2		moveEnd;				// 0x558 where it ends (invented name)
	int				moveSuspendCount;		// 0x560 pauses not yet resumed
	NiPoint2		savedCursor;			// 0x564 where the cursor goes when the moves resume (invented name)
	NiPoint2		cursor;					// 0x56C in 0 to 1 of the window
	NiPoint2		mouseDelta;				// 0x574 of the last event
	bool			mouseButtonsDown[TGINPUT_MAX_MOUSE_BUTTONS];	// 0x57C by MOUSE_BUTTON_* (invented name). Four: the implicit operator= (0x65B5A0) copies four bytes; the startup clears the three buttons used
	IDirectInputDevice *	keyboard;		// 0x580 the DirectInput keyboard: buffered, non-exclusive, in the foreground (invented name)
	IDirectInputDevice *	mouse;			// 0x584 the DirectInput mouse, likewise, with absolute axes (invented name)
	DIDEVICEOBJECTDATA *	events;			// 0x588 owned: the TGINPUT_BUFFER_SIZE events GetDeviceData reads (invented name)
	unsigned char *	keyState;				// 0x58C owned, 256 bytes: the keyboard's state, one byte per scan code (invented name)
	unsigned char *	lastKeyState;			// 0x590 owned, 256 bytes: the state at the last poll (invented name)
	bool			centerMouse;				// 0x594
	int				moveNotifyDestination;	// 0x598 where the move reports its start and end; 0 for nowhere

	// Declared for the SWIG wrappers that call them.
public:
	void	RegisterUnicodeKey( int, int, TGLocalizationDatabase *, char *, int );	// retail 0x6E5760: the SWIG wrapper TGInputManager_RegisterUnicodeKey calls it
};

// The input manager (the SWIG variable g_kInputManager).
extern TGInputManager	g_kInputManager;

// Debugging switches that nothing in the retail build ever sets: a non-zero value
// makes the keyboard and mouse message handlers skip their work (invented names).
extern int				ignoreKeyboardMessages;
extern int				ignoreMouseMessages;

#endif /* !__TGINPUTMANAGER_H__ */
