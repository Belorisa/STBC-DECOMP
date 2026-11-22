/*
===============================================================================

	TGInputManager.cpp

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale
// id pair.
// FUNCTION: STBC 0x006e3e70 _$E32
// FUNCTION: STBC 0x006e3e80 _$E29
// FUNCTION: STBC 0x006e3e90 _$E31
// FUNCTION: STBC 0x006e3ea0 _$E30
// FUNCTION: STBC 0x006e3eb0 _$E38
// FUNCTION: STBC 0x006e3ec0 _$E35
// FUNCTION: STBC 0x006e3ed0 _$E37
// FUNCTION: STBC 0x006e3ee0 _$E36
// FUNCTION: STBC 0x006e7c20 _$E464
// FUNCTION: STBC 0x006e7c40 _$E463
#include <iostream>
#include <math.h>
#include <string.h>
#include <dinput.h>

#include "TGInputManager.h"
#include "TGDirectInput7.h"
#include "TGGamepad.h"
#include "NiPoint2.h"
#include "UtopiaApp.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "TGGamepadEvent.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "TGSystemWrapperClass.h"
#include "TGRootPane.h"
#include "TGEventManager.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "TGLocalizationDatabase.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006e3ef0 _$E55
// FUNCTION: STBC 0x006e3f00 _$E54
// FUNCTION: STBC 0x006e3f10 _$E66
// FUNCTION: STBC 0x006e3f20 _$E65
// FUNCTION: STBC 0x006e3f30 _$E70
// FUNCTION: STBC 0x006e3f40 _$E69
// FUNCTION: STBC 0x006e3f50 _$E74
// FUNCTION: STBC 0x006e3f60 _$E73
// FUNCTION: STBC 0x006e3f70 _$E78
// FUNCTION: STBC 0x006e3f80 _$E77
// FUNCTION: STBC 0x006e3f90 _$E198
// FUNCTION: STBC 0x006e3fa0 _$E197
// FUNCTION: STBC 0x006e3fb0 _$E211
// FUNCTION: STBC 0x006e3fc0 _$E210
// FUNCTION: STBC 0x006e3fd0 _$E140
// FUNCTION: STBC 0x006e3fe0 _$E139
// FUNCTION: STBC 0x006e3ff0 _$E153
// FUNCTION: STBC 0x006e4000 _$E152
// FUNCTION: STBC 0x006e4010 _$E203
// FUNCTION: STBC 0x006e4020 _$E202
// FUNCTION: STBC 0x006e4030 _$E216
// FUNCTION: STBC 0x006e4040 _$E215
// FUNCTION: STBC 0x006e4050 _$E207
// FUNCTION: STBC 0x006e4060 _$E206
// FUNCTION: STBC 0x006e4070 _$E220
// FUNCTION: STBC 0x006e4080 _$E219
// FUNCTION: STBC 0x006e4090 _$E227
// FUNCTION: STBC 0x006e40a0 _$E226
// FUNCTION: STBC 0x006e40b0 _$E291
// FUNCTION: STBC 0x006e40c0 _$E290
// FUNCTION: STBC 0x006e40d0 _$E295
// FUNCTION: STBC 0x006e40e0 _$E294
// FUNCTION: STBC 0x006e40f0 _$E102
// FUNCTION: STBC 0x006e4100 _$E101
// FUNCTION: STBC 0x006e4110 _$E277
// FUNCTION: STBC 0x006e4120 _$E276
// FUNCTION: STBC 0x006e4130 _$E284
// FUNCTION: STBC 0x006e4140 _$E283
// FUNCTION: STBC 0x006e4150 _$E307
// FUNCTION: STBC 0x006e4160 _$E306
// FUNCTION: STBC 0x006e4170 _$E311
// FUNCTION: STBC 0x006e4180 _$E310
// FUNCTION: STBC 0x006e4190 _$E351
// FUNCTION: STBC 0x006e41a0 _$E350
// FUNCTION: STBC 0x006e41b0 _$E357
// FUNCTION: STBC 0x006e41c0 _$E356
// FUNCTION: STBC 0x006e41d0 _$E299
// FUNCTION: STBC 0x006e41e0 _$E298
// FUNCTION: STBC 0x006e41f0 _$E303
// FUNCTION: STBC 0x006e4200 _$E302
// FUNCTION: STBC 0x006e4210 _$E315
// FUNCTION: STBC 0x006e4220 _$E314
// FUNCTION: STBC 0x006e4230 _$E319
// FUNCTION: STBC 0x006e4240 _$E318
// FUNCTION: STBC 0x006e4250 _$E323
// FUNCTION: STBC 0x006e4260 _$E322
// FUNCTION: STBC 0x006e4270 _$E327
// FUNCTION: STBC 0x006e4280 _$E326
// FUNCTION: STBC 0x006e4290 _$E331
// FUNCTION: STBC 0x006e42a0 _$E330
// FUNCTION: STBC 0x006e42b0 _$E335
// FUNCTION: STBC 0x006e42c0 _$E334
// FUNCTION: STBC 0x006e42d0 _$E339
// FUNCTION: STBC 0x006e42e0 _$E338
// FUNCTION: STBC 0x006e42f0 _$E343
// FUNCTION: STBC 0x006e4300 _$E342
// FUNCTION: STBC 0x006e4310 _$E107
// FUNCTION: STBC 0x006e4320 _$E106
// FUNCTION: STBC 0x006e4330 _$E347
// FUNCTION: STBC 0x006e4340 _$E346
// FUNCTION: STBC 0x006e4350 _$E112
// FUNCTION: STBC 0x006e4360 _$E111
// FUNCTION: STBC 0x006e4370 _$E361
// FUNCTION: STBC 0x006e4380 _$E360
// FUNCTION: STBC 0x006e4390 _$E365
// FUNCTION: STBC 0x006e43a0 _$E364
// FUNCTION: STBC 0x006e43b0 _$E369
// FUNCTION: STBC 0x006e43c0 _$E368
// FUNCTION: STBC 0x006e43d0 _$E373
// FUNCTION: STBC 0x006e43e0 _$E372
// FUNCTION: STBC 0x006e43f0 _$E117
// FUNCTION: STBC 0x006e4400 _$E116
// FUNCTION: STBC 0x006e4410 _$E377
// FUNCTION: STBC 0x006e4420 _$E376
// FUNCTION: STBC 0x006e4430 _$E381
// FUNCTION: STBC 0x006e4440 _$E380
// FUNCTION: STBC 0x006e4450 _$E385
// FUNCTION: STBC 0x006e4460 _$E384
// FUNCTION: STBC 0x006e4470 _$E123
// FUNCTION: STBC 0x006e4480 _$E122
// FUNCTION: STBC 0x006e4490 _$E128
// FUNCTION: STBC 0x006e44a0 _$E127
// FUNCTION: STBC 0x006e44b0 _$E158
// FUNCTION: STBC 0x006e44c0 _$E157
// FUNCTION: STBC 0x006e44d0 _$E389
// FUNCTION: STBC 0x006e44e0 _$E388
// FUNCTION: STBC 0x006e44f0 _$E163
// FUNCTION: STBC 0x006e4500 _$E162
// FUNCTION: STBC 0x006e4510 _$E393
// FUNCTION: STBC 0x006e4520 _$E392
// FUNCTION: STBC 0x006e4530 _$E397
// FUNCTION: STBC 0x006e4540 _$E396
// FUNCTION: STBC 0x006e4550 _$E168
// FUNCTION: STBC 0x006e4560 _$E167
// FUNCTION: STBC 0x006e4570 _$E401
// FUNCTION: STBC 0x006e4580 _$E400
// FUNCTION: STBC 0x006e4590 _$E133
// FUNCTION: STBC 0x006e45a0 _$E132
// FUNCTION: STBC 0x006e45b0 _$E405
// FUNCTION: STBC 0x006e45c0 _$E404
// FUNCTION: STBC 0x006e45d0 _$E409
// FUNCTION: STBC 0x006e45e0 _$E408
// FUNCTION: STBC 0x006e45f0 _$E173
// FUNCTION: STBC 0x006e4600 _$E172
// FUNCTION: STBC 0x006e4610 _$E178
// FUNCTION: STBC 0x006e4620 _$E177
// FUNCTION: STBC 0x006e4630 _$E413
// FUNCTION: STBC 0x006e4640 _$E412
// FUNCTION: STBC 0x006e4650 _$E417
// FUNCTION: STBC 0x006e4660 _$E416
// FUNCTION: STBC 0x006e4670 _$E421
// FUNCTION: STBC 0x006e4680 _$E420
// FUNCTION: STBC 0x006e4690 _$E425
// FUNCTION: STBC 0x006e46a0 _$E424
// FUNCTION: STBC 0x006e46b0 _$E429
// FUNCTION: STBC 0x006e46c0 _$E428
// FUNCTION: STBC 0x006e46d0 _$E433
// FUNCTION: STBC 0x006e46e0 _$E432
// FUNCTION: STBC 0x006e46f0 _$E437
// FUNCTION: STBC 0x006e4700 _$E436
// FUNCTION: STBC 0x006e4710 _$E441
// FUNCTION: STBC 0x006e4720 _$E440
// FUNCTION: STBC 0x006e4730 _$E183
// FUNCTION: STBC 0x006e4740 _$E182
// FUNCTION: STBC 0x006e4750 _$E445
// FUNCTION: STBC 0x006e4760 _$E444
// FUNCTION: STBC 0x006e4770 _$E188
// FUNCTION: STBC 0x006e4780 _$E187
// FUNCTION: STBC 0x006e4790 _$E193
// FUNCTION: STBC 0x006e47a0 _$E192

// A quarter turn: a cursor move follows a sine over it, fast at the start
// and slowing to a stop.
#define HALF_PI		( 3.1415927f / 2.0 )

// FUNCTION: STBC 0x006e47b0 _$E450
// FUNCTION: STBC 0x006e47c0 _$E447
// FUNCTION: STBC 0x006e47d0 _$E449
// FUNCTION: STBC 0x006e47e0 _$E448
// DATA: STBC 0x009992f0
TGInputManager g_kInputManager;

// Where the input code last saw the mouse, and the mouse's movement the editors use (invented names; see TGMouseEvent.h).
// DATA: STBC 0x0099985c
NiPoint2	lastMousePosition;
// DATA: STBC 0x00999864
NiPoint2	mouseMovement;

// Debugging switches that nothing in the retail build ever sets: a non-zero value
// makes the keyboard and mouse message handlers skip their work (invented names).
// DATA: STBC 0x00999870
int		ignoreKeyboardMessages;
// DATA: STBC 0x00999874
int		ignoreMouseMessages;

// The events DirectInput keeps for the keyboard and the mouse between
// polls.
#define TGINPUT_BUFFER_SIZE		0x200

// Each gamepad button's value at the last poll; Poll posts an event when
// it changes (invented name).
// DATA: STBC 0x0099990c
static float gamepadButtonValues[TGINPUT_MAX_GAMEPADS][32];

// A bit per gamepad button (invented name).
// DATA: STBC 0x00999b64
static unsigned int gamepadButtonFlags[TGINPUT_MAX_GAMEPADS];

/*
================
TGInputManager::IsGamepadButtonFlagged
================
*/
// FUNCTION: STBC 0x006e47f0
bool TGInputManager::IsGamepadButtonFlagged( int gamepad, int button ) {
	return ( gamepadButtonFlags[gamepad] & ( 1 << button ) ) ? true : false;
}

/*
================
TGInputManager::SetGamepadButtonFlag
================
*/
// FUNCTION: STBC 0x006e4810
// DRAFT: retail computes each new value in eax (mask | load, then a store); this build ors and ands memory in place
void TGInputManager::SetGamepadButtonFlag( int gamepad, int button, bool flag ) {
	if ( flag ) {
		gamepadButtonFlags[gamepad] = ( 1 << button ) | gamepadButtonFlags[gamepad];
	} else {
		gamepadButtonFlags[gamepad] = ~( 1 << button ) & gamepadButtonFlags[gamepad];
	}
}

/*
================
TGInputManager::TGInputManager

The default key list needs no key held; the devices start in the
startup (Unknown6E4D80).
================
*/
// FUNCTION: STBC 0x006e4850 ??0TGInputManager@@QAE@XZ
// PARKED: inline budget; retail expands NewItem in keyLists.AddTail (calling the block constructor 0x6E7CA0), this build calls NewItem out of line; the rest is identical
TGInputManager::TGInputManager( void ) : keyLists( 2 ), keysDown( 2 ) {
	int i;
	for ( i = 0; i < TGINPUT_MAX_GAMEPADS; i++ ) {
		gamepads[i] = NULL;
	}
	unknownC8 = 0;
	unknownCC = 0;
	lastRepeatTime = 0;

	currentKeyList = new TGUnicodeKeyList;
	currentKeyList->keys.AddTail( 0 );
	keyLists.AddTail( currentKeyList );

	for ( i = 0; i < TGINPUT_MAX_DEAD_KEYS; i++ ) {
		deadKeys[i].deadKey = 0;
		for ( int j = 0; j < 32; j++ ) {
			deadKeys[i].combinations[j].base = 0;
			deadKeys[i].combinations[j].result = 0;
		}
	}

	pendingDeadKey = 0;
	pendingKeyData = 0;
	deadKeyHeld = false;
	keyboardInputEnabled = true;
	keyboard = NULL;
	events = NULL;
	mouse = NULL;
	keyState = new unsigned char[256];
	memset( keyState, 0, 256 );
	lastKeyState = new unsigned char[256];
	memset( lastKeyState, 0, 256 );
}

/*
================
TGInputManager::~TGInputManager

The key lists themselves go in the shutdown (Unknown6E5090).
================
*/
// FUNCTION: STBC 0x006e4bb0 ??1TGInputManager@@QAE@XZ
TGInputManager::~TGInputManager( void ) {
	Unknown6E5090();
	delete [] keyState;
	delete [] lastKeyState;
}

/*
================
TGInputManager::Unknown6E4D80

The startup: each gamepad slot is cleared and gets its gamepad, the
cursor rests in the middle of the window, the scan codes are mapped, and
the keyboard and the mouse are opened through DirectInput: buffered, in
the foreground without exclusive access, the mouse with relative axes.
================
*/
// FUNCTION: STBC 0x006e4d80
// PARKED: same calls, stores and frame; retail clears the mouse buttons with direct ebp-relative stores where this build's memset goes through a lea'd pointer, which shifts ecx/edx in the device calls after it
void TGInputManager::Unknown6E4D80( int arg ) {
	unknown04 = arg;
	for ( int i = 0; i < TGINPUT_MAX_GAMEPADS; i++ ) {
		lastButtons[i] = 0;
		lastZ[i] = 0;
		lastY[i] = 0;
		lastX[i] = 0;
		lastRy[i] = 0;
		lastRx[i] = 0;
		lastRz[i] = 0;
		lastSlider1[i] = 0;
		lastSlider0[i] = 0;
		lastPov[i] = -1.0f;
		lastPovDirections[i] = 0;
		gamepads[i] = new TGGamepad( NiApplication::application, i );
		memset( gamepadButtonValues[i], 0, sizeof( gamepadButtonValues[i] ) );
	}

	moveDuration = 0.0f;
	moveElapsed = 0.0f;
	moveStart = NiPoint2::ZERO;
	moveEnd = NiPoint2::ZERO;
	moveSuspendCount = 0;
	savedCursor = NiPoint2::ZERO;
	cursor.x = 0.5f;
	cursor.y = 0.5f;
	mouseDelta.x = 0.0f;
	mouseDelta.y = 0.0f;
	memset( mouseButtonsDown, 0, TGINPUT_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	MapScancodes();
	centerMouse = false;
	moveNotifyDestination = 0;

	IDirectInput7A *input = g_kSystemWrapper.GetDirectInput();
	if ( input == NULL ) {
		return;
	}
	events = new DIDEVICEOBJECTDATA[TGINPUT_BUFFER_SIZE];

	if ( FAILED( input->CreateDeviceEx( GUID_SysKeyboard, IID_IDirectInputDevice7A, (void **)&keyboard, NULL ) ) ) {
		return;
	}
	if ( FAILED( keyboard->SetDataFormat( &c_dfDIKeyboard ) ) ) {
		return;
	}
	if ( FAILED( keyboard->SetCooperativeLevel( NiApplication::application->GetAppWindow()->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) ) {
		return;
	}
	DIPROPDWORD property;
	ZeroMemory( &property, sizeof( property ) );
	property.diph.dwSize = sizeof( DIPROPDWORD );
	property.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	property.diph.dwHow = DIPH_DEVICE;
	property.dwData = TGINPUT_BUFFER_SIZE;
	if ( FAILED( keyboard->SetProperty( DIPROP_BUFFERSIZE, &property.diph ) ) ) {
		return;
	}

	if ( FAILED( input->CreateDeviceEx( GUID_SysMouse, IID_IDirectInputDevice7A, (void **)&mouse, NULL ) ) ) {
		return;
	}
	if ( FAILED( mouse->SetDataFormat( &c_dfDIMouse ) ) ) {
		return;
	}
	if ( FAILED( mouse->SetCooperativeLevel( NiApplication::application->GetAppWindow()->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) ) {
		return;
	}
	ZeroMemory( &property, sizeof( property ) );
	property.diph.dwSize = sizeof( DIPROPDWORD );
	property.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	property.diph.dwHow = DIPH_DEVICE;
	property.dwData = TGINPUT_BUFFER_SIZE;
	if ( FAILED( mouse->SetProperty( DIPROP_BUFFERSIZE, &property.diph ) ) ) {
		return;
	}
	property.dwData = DIPROPAXISMODE_REL;
	mouse->SetProperty( DIPROP_AXISMODE, &property.diph );
}

/*
================
TGInputManager::MapScancodes

The script KeyConfig maps the keyboard's scan codes; its errors are
printed.
================
*/
// FUNCTION: STBC 0x006e5060
void TGInputManager::MapScancodes( void ) {
	PyEmbed::CallFunction( "KeyConfig", "MapScancodes", "", NULL, "()" );
	PyErr_Print();
}

/*
================
TGInputManager::Unknown6E5090

The shutdown: the buffer and the DirectInput devices are released, the
key lists emptied and deleted, the display strings dropped and the
gamepads deleted.
================
*/
// FUNCTION: STBC 0x006e5090
void TGInputManager::Unknown6E5090( void ) {
	if ( events ) {
		delete[] events;
	}
	events = NULL;

	if ( keyboard ) {
		keyboard->Unacquire();
		keyboard->Release();
	}
	keyboard = NULL;
	if ( mouse ) {
		mouse->Unacquire();
		mouse->Release();
	}
	mouse = NULL;

	NiTListIterator pos = keyLists.GetHeadPos();
	while ( pos ) {
		TGUnicodeKeyList *keyList = keyLists.GetNext( pos );
		keyList->keys.RemoveAll();
		keyList->characters.RemoveAll();
		keyLists.Remove( keyList );
		delete keyList;
	}

	displayStrings.RemoveAll();

	for ( int i = 0; i < TGINPUT_MAX_GAMEPADS; i++ ) {
		if ( gamepads[i] ) {
			delete gamepads[i];
			gamepads[i] = NULL;
		}
	}
	unknown04 = 0;
}

/*
================
TGInputManager::KeyEvent

A key going down, up, or typed (KS_RELEASED): the keys held are tracked,
the key is turned into its character, and the keyboard event goes out
through the dead-key handling. The right shift and control keys count as
the left ones.
================
*/
// FUNCTION: STBC 0x006e5380 ?KeyEvent@TGInputManager@@QAE_NHJH@Z
bool TGInputManager::KeyEvent( int scanCode, long keyData, int state ) {
	if ( !keyboardInputEnabled ) {
		return false;
	}
	if ( scanCode == DIK_RSHIFT ) {
		scanCode = DIK_LSHIFT;
	} else if ( scanCode == DIK_RCONTROL ) {
		scanCode = DIK_LCONTROL;
	}
	int character = GetUnicodeKey( scanCode, true );
	if ( scanCode == DIK_SPACE ) {
		character = GetUnicodeKey( scanCode, false );
	} else if ( scanCode == DIK_BACK ) {
		character = GetUnicodeKey( scanCode, false );
	} else if ( scanCode == DIK_UP ) {
		character = GetUnicodeKey( scanCode, false );
	} else if ( scanCode == DIK_DOWN ) {
		character = GetUnicodeKey( scanCode, false );
	} else if ( scanCode == DIK_LEFT ) {
		character = GetUnicodeKey( scanCode, false );
	} else if ( scanCode == DIK_RIGHT ) {
		character = GetUnicodeKey( scanCode, false );
	}

	int keyState;
	switch ( state ) {
		case KS_DOWN:
			keyState = KS_KEYDOWN;
			keysDown.AddTail( scanCode );
			SetCurrentUnicodeKeyList();
			unknownC8 = character;
			unknownCC = NiApplication::application->GetCurrentTime();
			lastRepeatTime = 0;
			break;
		case KS_UP:
			keyState = KS_KEYUP;
			keysDown.Remove( scanCode );
			SetCurrentUnicodeKeyList();
			if ( GetScanCode( (unsigned short)unknownC8 ) == scanCode ) {
				unknownCC = 0;
				lastRepeatTime = 0;
				unknownC8 = 0;
			}
			break;
		case KS_RELEASED:
			keyState = KS_NORMAL;
			break;
		default:
			return false;
	}
	HandleDeadKey( character, keyState, keyData );
	return true;
}

/*
================
TGInputManager::HandleDeadKey

A dead key waits for the next key typed: a key it combines with becomes
the combined character, any other is posted after the dead key's own
character. The modifier keys pass through without ending the wait.
================
*/
// The keys that pass through a waiting dead key: the shift, control and
// alt keys (invented names).
#define DEAD_KEY_MODIFIERS_FIRST	0xE002
#define DEAD_KEY_MODIFIERS_LAST		0xE006

static inline bool IsModifierKey( int key ) {
	return key >= DEAD_KEY_MODIFIERS_FIRST && key <= DEAD_KEY_MODIFIERS_LAST;
}

// FUNCTION: STBC 0x006e55e0
void TGInputManager::HandleDeadKey( short character, int keyState, long keyData ) {
	if ( pendingDeadKey == NULL ) {
		for ( int i = 0; i < TGINPUT_MAX_DEAD_KEYS; i++ ) {
			TGDeadKey *entry = &deadKeys[i];
			if ( entry->deadKey == 0 ) {
				break;
			}
			if ( entry->deadKey == character ) {
				pendingDeadKey = entry;
				deadKeyHeld = true;
				return;
			}
		}
	} else if ( deadKeyHeld == true ) {
		if ( pendingDeadKey->deadKey == character ) {
			if ( keyState == KS_NORMAL ) {
				pendingKeyData = keyData;
				return;
			}
			if ( keyState == KS_KEYUP ) {
				deadKeyHeld = false;
			}
			return;
		}
	} else if ( !IsModifierKey( (unsigned short)character ) && keyState == KS_NORMAL ) {
		TGDeadKey *deadKey = pendingDeadKey;
		pendingDeadKey = NULL;
		bool combined = false;
		for ( int j = 0; j < 32; j++ ) {
			TGDeadKeyCombination *combination = &deadKey->combinations[j];
			if ( combination->base == 0 ) {
				break;
			}
			if ( combination->base == character ) {
				character = combination->result;
				combined = true;
				break;
			}
		}
		if ( !combined ) {
			PostKeyboardEvent( deadKey->deadKey, KS_NORMAL, pendingKeyData );
		}
	}
	PostKeyboardEvent( character, keyState, keyData );
}

/*
================
TGInputManager::PostKeyboardEvent
================
*/
// FUNCTION: STBC 0x006e56f0
void TGInputManager::PostKeyboardEvent( unsigned short character, int keyState, long keyData ) {
	TGKeyboardEvent *event = new TGKeyboardEvent;
	event->keyState = keyState;
	event->keyData = keyData;
	event->unicode = character;
	event->SetTimestamp( g_kSystemWrapper.GetTimeInSeconds() );
	event->SetDestination( &g_kRootWindow );
	g_kEventManager.AddEvent( event );
}

/*
================
TGInputManager::RegisterUnicodeKey

Maps a key, with the shift key held, to a character: the key list for
the shift key is made when there is none, and the character gets its
display string (looked up in the database when there is one) and its
scan code.
================
*/
// FUNCTION: STBC 0x006e5760 ?RegisterUnicodeKey@TGInputManager@@QAEXHHPAVTGLocalizationDatabase@@PADH@Z
void TGInputManager::RegisterUnicodeKey( int unicode, int key, TGLocalizationDatabase *database, char *displayName, int shiftKey ) {
	TGUnicodeKeyList *keyList = NULL;
	NiTListIterator pos = keyLists.GetHeadPos();
	while ( pos != NULL ) {
		keyList = keyLists.GetNext( pos );
		if ( shiftKey == 0 ) {
			break;
		}
		if ( keyList != NULL ) {
			NiTListIterator keyPos = keyList->keys.GetHeadPos();
			while ( true ) {
				if ( keyPos == NULL ) {
					break;
				}
				if ( keyList->keys.GetNext( keyPos ) != shiftKey ) {
					keyList = NULL;
					break;
				}
			}
		}
	}

	if ( keyList == NULL ) {
		keyList = new TGUnicodeKeyList;
		keyList->keys.AddTail( shiftKey );
		keyLists.AddTail( keyList );
	}

	int character;
	keyList->characters.GetAt( key, character );	// BUG: redundant: the lookup's result is never used
	keyList->characters.SetAt( key, unicode );

	if ( displayName != NULL ) {
		TGString displayString;
		if ( database != NULL ) {
			displayString.Unknown6F4D90( database->GetString( displayName ) );
		} else {
			displayString.SetString( displayName );
		}
		SetDisplayStringForUnicode( unicode, displayString );
	}

	scanCodes.SetAt( unicode, key );
}

/*
================
TGInputManager::RegisterDeadKey

Adds the combination of the dead key with the base character to the first
entry that has the dead key or none yet and has room for it. Does nothing
when there is none.
================
*/
// FUNCTION: STBC 0x006e5a00 ?RegisterDeadKey@TGInputManager@@QAEXFFF@Z
void TGInputManager::RegisterDeadKey( short deadKey, short base, short result ) {
	for ( int i = 0; i < TGINPUT_MAX_DEAD_KEYS; i++ ) {
		TGDeadKey *entry = &deadKeys[i];
		if ( entry->deadKey == deadKey || entry->deadKey == 0 ) {
			entry->deadKey = deadKey;
			for ( int j = 0; j < 32; j++ ) {
				TGDeadKeyCombination *combination = &entry->combinations[j];
				if ( combination->base == base || combination->base == 0 ) {
					combination->base = base;
					combination->result = result;
					return;
				}
			}
		}
	}
}

/*
================
TGInputManager::GetUnicodeKey

The character the key gives in the current key list, or in the default one;
0 when it gives none.
================
*/
// FUNCTION: STBC 0x006e5a70
int TGInputManager::GetUnicodeKey( int key, bool useCurrentList ) {
	int character;
	if ( useCurrentList ) {
		if ( currentKeyList->characters.GetAt( key, character ) ) {
			return character;
		}
	} else {
		if ( keyLists.GetHead()->characters.GetAt( key, character ) ) {
			return character;
		}
	}
	return 0;
}

/*
================
TGInputManager::GetScanCode

The scan code filed under the key; 0 when there is none.
================
*/
// FUNCTION: STBC 0x006e5b00
int TGInputManager::GetScanCode( int key ) {
	int scanCode;
	if ( scanCodes.GetAt( key, scanCode ) ) {
		return scanCode;
	}
	return 0;
}

/*
================
TGInputManager::SetCurrentUnicodeKeyList

Makes current the key list with the most keys that are all held; the first
list when none of the others is.
================
*/
// FUNCTION: STBC 0x006e5b50
void TGInputManager::SetCurrentUnicodeKeyList( void ) {
	TGUnicodeKeyList *best = keyLists.GetHead();
	unsigned int bestCount = 0;
	if ( keysDown.GetSize() != 0 ) {
		NiTListIterator pos = keyLists.GetHeadPos();
		best = keyLists.GetNext( pos );
		while ( pos != NULL ) {
			TGUnicodeKeyList *keyList = keyLists.GetNext( pos );
			unsigned int matches = 0;
			if ( keyList->keys.GetSize() > bestCount ) {
				NiTListIterator keyPos = keyList->keys.GetHeadPos();
				while ( keyPos != NULL ) {
					if ( keysDown.FindPos( keyList->keys.GetNext( keyPos ) ) == NULL ) {
						break;
					}
					matches++;
				}
				if ( keyList->keys.GetSize() == matches ) {
					best = keyList;
					bestCount = matches;
				}
			}
		}
	}
	currentKeyList = best;
}

/*
================
TGInputManager::SetDisplayStringForUnicode

Sets the string shown for the character (keys are listed with it in the key
bindings screen).
================
*/
// FUNCTION: STBC 0x006e5c40
void TGInputManager::SetDisplayStringForUnicode( int unicode, TGString displayString ) {
	displayStrings.SetAt( unicode, displayString );
}

/*
================
TGInputManager::GetDisplayStringFromUnicode

The string shown for the character; empty when none was set.
================
*/
// FUNCTION: STBC 0x006e5dc0
TGString TGInputManager::GetDisplayStringFromUnicode( int unicode ) {
	TGString displayString( "" );
	displayStrings.GetAt( unicode, displayString );
	return displayString;
}
/*
================
TGInputManager::MouseEvent

Posts a mouse event: a move to a point or by a delta, or a button going
down or up, which the keyboard handlers also see as a key. Nothing is
posted while a scripted cursor move runs.
================
*/
// FUNCTION: STBC 0x006e5f90 ?MouseEvent@TGInputManager@@QAE_NHMMH@Z
bool TGInputManager::MouseEvent( int button, float x, float y, int state ) {
	if ( state == KS_MOVE_DELTA ) {
		x = NiApplication::application->GetAppWindow()->GetWidth() * x * 0.0015625f;
		y = NiApplication::application->GetAppWindow()->GetHeight() * y * 0.0020833334f;
	}
	if ( moveDuration != 0.0f && moveSuspendCount <= 0 ) {
		return false;
	}

	int flags = 0;
	if ( state == KS_DOWN ) {
		flags = MEF_BUTTON_DOWN;
	} else if ( state == KS_UP ) {
		flags = MEF_BUTTON_UP;
	}
	switch ( button ) {
		case TG_LMBUTTON:
			flags |= MEF_BUTTON_LEFT;
			if ( state == KS_DOWN ) {
				mouseButtonsDown[MOUSE_BUTTON_LEFT] = true;
				break;
			}
			if ( state == KS_UP ) {
				mouseButtonsDown[MOUSE_BUTTON_LEFT] = false;
			}
			break;
		case TG_CMBUTTON:
			flags |= MEF_BUTTON_MIDDLE;
			if ( state == KS_DOWN ) {
				mouseButtonsDown[MOUSE_BUTTON_MIDDLE] = true;
				break;
			}
			if ( state == KS_UP ) {
				mouseButtonsDown[MOUSE_BUTTON_MIDDLE] = false;
			}
			break;
		case TG_RMBUTTON:
			flags |= MEF_BUTTON_RIGHT;
			if ( state == KS_DOWN ) {
				mouseButtonsDown[MOUSE_BUTTON_RIGHT] = true;
				break;
			}
			if ( state == KS_UP ) {
				mouseButtonsDown[MOUSE_BUTTON_RIGHT] = false;
			}
			break;
	}
	if ( state == KS_MOVE ) {
		cursor.x = x;
		cursor.y = y;
	} else if ( state == KS_MOVE_DELTA ) {
		cursor.x += x;
		cursor.y += y;
	}
	if ( cursor.x < 0.0f ) {
		cursor.x = 0.0f;
	}
	if ( cursor.y < 0.0f ) {
		cursor.y = 0.0f;
	}
	if ( cursor.x > 1.0f ) {
		cursor.x = 1.0f;
	}
	if ( cursor.y > 1.0f ) {
		cursor.y = 1.0f;
	}

	TGMouseEvent *event = new TGMouseEvent;
	event->AddInputFlags( flags );
	event->x = cursor.x;
	event->y = cursor.y;
	event->SetTimestamp( g_kSystemWrapper.GetTimeInSeconds() );
	event->SetDestination( &g_kRootWindow );
	g_kEventManager.AddEvent( event );

	switch ( button ) {
		case TG_LMBUTTON:
			if ( state == KS_DOWN ) {
				KeyEvent( KEY_MOUSE_LEFT, 0, KS_DOWN );
				return true;
			}
			if ( state == KS_UP ) {
				KeyEvent( KEY_MOUSE_LEFT, 0, KS_UP );
				KeyEvent( KEY_MOUSE_LEFT, 0, KS_RELEASED );
				return true;
			}
			break;
		case TG_CMBUTTON:
			if ( state == KS_DOWN ) {
				KeyEvent( KEY_MOUSE_MIDDLE, 0, KS_DOWN );
				return true;
			}
			if ( state == KS_UP ) {
				KeyEvent( KEY_MOUSE_MIDDLE, 0, KS_UP );
				KeyEvent( KEY_MOUSE_MIDDLE, 0, KS_RELEASED );
				return true;
			}
			break;
		case TG_RMBUTTON:
			if ( state == KS_DOWN ) {
				KeyEvent( KEY_MOUSE_RIGHT, 0, KS_DOWN );
				return true;
			}
			if ( state == KS_UP ) {
				KeyEvent( KEY_MOUSE_RIGHT, 0, KS_UP );
				KeyEvent( KEY_MOUSE_RIGHT, 0, KS_RELEASED );
				return true;
			}
			break;
	}
	return true;
}

/*
================
TGInputManager::GetMouseDeltaX

The movement of the cursor in the last event, in pixels of the window.
================
*/
// FUNCTION: STBC 0x006e6290 ?GetMouseDeltaX@TGInputManager@@QAEHXZ
int TGInputManager::GetMouseDeltaX( void ) {
	float scale = NiApplication::application->GetAppWindow()->GetWidth() * 0.0015625f;
	return (int)( scale * mouseDelta.x * 640.0f );
}

/*
================
TGInputManager::GetMouseDeltaY
================
*/
// FUNCTION: STBC 0x006e62d0 ?GetMouseDeltaY@TGInputManager@@QAEHXZ
int TGInputManager::GetMouseDeltaY( void ) {
	float scale = NiApplication::application->GetAppWindow()->GetHeight() * 0.0020833334f;
	return (int)( scale * mouseDelta.y * 480.0f );
}

/*
================
TGInputManager::GetMouseX

The cursor's place in pixels of the window.
================
*/
// FUNCTION: STBC 0x006e6310 ?GetMouseX@TGInputManager@@QAEHXZ
int TGInputManager::GetMouseX( void ) {
	float scale = NiApplication::application->GetAppWindow()->GetWidth() * 0.0015625f;
	return (int)( scale * cursor.x * 640.0f );
}

/*
================
TGInputManager::GetMouseY
================
*/
// FUNCTION: STBC 0x006e6350 ?GetMouseY@TGInputManager@@QAEHXZ
int TGInputManager::GetMouseY( void ) {
	float scale = NiApplication::application->GetAppWindow()->GetHeight() * 0.0020833334f;
	return (int)( scale * cursor.y * 480.0f );
}

/*
================
TGInputManager::GamepadEvent

Posts a gamepad event for the root window: the two numbers and the value
read from the pad, and whether the control was pressed (1), released (2) or
neither.
================
*/
// FUNCTION: STBC 0x006e6390 ?GamepadEvent@TGInputManager@@QAE_NHHMH@Z
bool TGInputManager::GamepadEvent( int a, int b, float c, int state ) {
	int action = 0;
	if ( state == 2 ) {
		action = state;
	} else if ( state == 1 ) {
		action = state;
	}
	TGGamepadEvent *event = new TGGamepadEvent;
	event->unknown2C = a;
	event->unknown34 = action;
	event->unknown30 = b;
	event->unknown28 = c;
	event->SetTimestamp( g_kSystemWrapper.GetTimeInSeconds() );
	event->SetDestination( &g_kRootWindow );
	g_kEventManager.AddEvent( event );
	return true;
}

/*
================
TGInputManager::UpdateDevices
================
*/
// FUNCTION: STBC 0x006e6420 ?UpdateDevices@TGInputManager@@QAEXXZ
void TGInputManager::UpdateDevices( void ) {
}

// The control numbers Poll posts for a gamepad's axes and its hat's four
// directions; its buttons go by their index (invented names).
#define GAMEPAD_CONTROL_X			0x100
#define GAMEPAD_CONTROL_Y			0x101
#define GAMEPAD_CONTROL_Z			0x102
#define GAMEPAD_CONTROL_RZ			0x103
#define GAMEPAD_CONTROL_RX			0x104
#define GAMEPAD_CONTROL_RY			0x105
#define GAMEPAD_CONTROL_POV			0x106
#define GAMEPAD_CONTROL_SLIDER0		0x107
#define GAMEPAD_CONTROL_SLIDER1		0x108
#define GAMEPAD_CONTROL_POV_UP		0x20
#define GAMEPAD_CONTROL_POV_RIGHT	0x21
#define GAMEPAD_CONTROL_POV_DOWN	0x22
#define GAMEPAD_CONTROL_POV_LEFT	0x23

// The state GamepadEvent reports as an axis move (TGGamepadEvent_PS_AXIS;
// invented name).
#define GAMEPAD_STATE_AXIS			3

// Seconds before a held key repeats, and between repeats (invented names).
#define KEY_REPEAT_DELAY			0.5
#define KEY_REPEAT_INTERVAL			0.05

/*
================
TGInputManager::Poll

Reads what DirectInput buffered since the last poll and posts it: the
keyboard's keys, then the mouse's moves and buttons; a lost device is
acquired again and its held keys and buttons read from its state. Repeats
the last key held, keeps the cursor at the window's centre when asked, and
posts each gamepad axis, hat direction and button that changed.
================
*/
// FUNCTION: STBC 0x006e6430
// DRAFT: retail tests the gamepad index against TGINPUT_MAX_GAMEPADS again when TGGamepad::Poll fails; the frame is 4 bytes larger and the event loop picks other registers
void TGInputManager::Poll( void ) {
	DWORD count = 0;
	HRESULT result;
	int tries;
	unsigned int i;

	if ( keyboard != NULL && events != NULL ) {
		unsigned char *swap = keyState;
		keyState = lastKeyState;
		lastKeyState = swap;

		tries = 0;
		do {
			count = TGINPUT_BUFFER_SIZE;
			result = keyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), events, &count, 0 );
			if ( result != DI_OK ) {
				result = keyboard->Acquire();
				if ( FAILED( result ) ) {
					break;
				}
				keysDown.RemoveAll();
				unknownC8 = 0;
				if ( SUCCEEDED( keyboard->GetDeviceState( 256, lastKeyState ) ) ) {
					for ( unsigned int key = 0; key < 256; key++ ) {
						if ( lastKeyState[key] & 0x80 ) {
							keysDown.AddTail( key );
						}
					}
				}
				result = DIERR_INPUTLOST;
			}
		} while ( tries++ <= 3 && result != DI_OK );

		if ( count > 0 && SUCCEEDED( result ) ) {
			for ( i = 0; i < count; i++ ) {
				if ( events[i].dwData & 0x80 ) {
					KeyEvent( events[i].dwOfs, 0, KS_DOWN );
					KeyEvent( events[i].dwOfs, 0, KS_RELEASED );
				} else {
					KeyEvent( events[i].dwOfs, 0, KS_UP );
				}
			}
		}
	}

	if ( unknownC8 != 0 ) {
		float now = NiApplication::application->GetCurrentTime();
		if ( unknownCC + KEY_REPEAT_DELAY > now ) {
			lastRepeatTime = now;
		} else if ( lastRepeatTime + KEY_REPEAT_INTERVAL <= now ) {
			lastRepeatTime = now;
			PostKeyboardEvent( unknownC8, 0, 0 );
		}
	} else {
		unknownCC = 0;
		lastRepeatTime = 0;
	}

	count = 0;
	if ( mouse != NULL && events != NULL ) {
		mouseDelta.x = 0;
		mouseDelta.y = 0;

		tries = 0;
		do {
			count = TGINPUT_BUFFER_SIZE;
			result = mouse->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), events, &count, 0 );
			if ( result != DI_OK ) {
				result = mouse->Acquire();
				if ( FAILED( result ) ) {
					count = 0;
					break;
				}
				DIMOUSESTATE state;
				if ( SUCCEEDED( mouse->GetDeviceState( sizeof( state ), &state ) ) ) {
					if ( ( state.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_LEFT] ) {
						MouseEvent( TG_LMBUTTON, cursor.x, cursor.y, KS_DOWN );
					}
					if ( ( state.rgbButtons[MOUSE_BUTTON_MIDDLE] & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_MIDDLE] ) {
						MouseEvent( TG_CMBUTTON, cursor.x, cursor.y, KS_DOWN );
					}
					if ( ( state.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_RIGHT] ) {
						MouseEvent( TG_RMBUTTON, cursor.x, cursor.y, KS_DOWN );
					}
					if ( !( state.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_LEFT] ) {
						MouseEvent( TG_LMBUTTON, cursor.x, cursor.y, KS_UP );
					}
					if ( !( state.rgbButtons[MOUSE_BUTTON_MIDDLE] & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_MIDDLE] ) {
						MouseEvent( TG_CMBUTTON, cursor.x, cursor.y, KS_UP );
					}
					if ( !( state.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_RIGHT] ) {
						MouseEvent( TG_RMBUTTON, cursor.x, cursor.y, KS_UP );
					}
				} else {
					if ( mouseButtonsDown[MOUSE_BUTTON_LEFT] ) {
						MouseEvent( TG_LMBUTTON, cursor.x, cursor.y, KS_UP );
					}
					if ( mouseButtonsDown[MOUSE_BUTTON_MIDDLE] ) {
						MouseEvent( TG_CMBUTTON, cursor.x, cursor.y, KS_UP );
					}
					if ( mouseButtonsDown[MOUSE_BUTTON_RIGHT] ) {
						MouseEvent( TG_RMBUTTON, cursor.x, cursor.y, KS_UP );
					}
				}
			}
		} while ( tries++ <= 3 && result != DI_OK );

		if ( count > 0 && SUCCEEDED( result ) ) {
			float width = (float)NiApplication::application->GetAppWindow()->GetWidth();
			float height = (float)NiApplication::application->GetAppWindow()->GetHeight();
			for ( i = 0; i < count; i++ ) {
				DIDEVICEOBJECTDATA *event = &events[i];
				float delta;
				switch ( event->dwOfs ) {
					case DIMOFS_X:
						delta = (int)event->dwData / width;
						MouseEvent( 0, delta, 0, KS_MOVE_DELTA );
						mouseDelta.x += delta;
						break;
					case DIMOFS_Y:
						delta = (int)event->dwData / height;
						MouseEvent( 0, 0, delta, KS_MOVE_DELTA );
						mouseDelta.y += delta;
						break;
					case DIMOFS_Z:
						if ( (int)event->dwData > 0 ) {
							KeyEvent( KEY_MOUSE_WHEEL_UP, 0, KS_DOWN );
							KeyEvent( KEY_MOUSE_WHEEL_UP, 0, KS_UP );
							KeyEvent( KEY_MOUSE_WHEEL_UP, 0, KS_RELEASED );
						} else if ( (int)event->dwData < 0 ) {
							KeyEvent( KEY_MOUSE_WHEEL_DOWN, 0, KS_DOWN );
							KeyEvent( KEY_MOUSE_WHEEL_DOWN, 0, KS_UP );
							KeyEvent( KEY_MOUSE_WHEEL_DOWN, 0, KS_RELEASED );
						}
						break;
					case DIMOFS_BUTTON0:
						if ( ( event->dwData & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_LEFT] ) {
							MouseEvent( TG_LMBUTTON, cursor.x, cursor.y, KS_DOWN );
						} else if ( !( event->dwData & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_LEFT] ) {
							MouseEvent( TG_LMBUTTON, cursor.x, cursor.y, KS_UP );
						}
						break;
					case DIMOFS_BUTTON2:
						if ( ( event->dwData & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_MIDDLE] ) {
							MouseEvent( TG_CMBUTTON, cursor.x, cursor.y, KS_DOWN );
						} else if ( !( event->dwData & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_MIDDLE] ) {
							MouseEvent( TG_CMBUTTON, cursor.x, cursor.y, KS_UP );
						}
						break;
					case DIMOFS_BUTTON1:
						if ( ( event->dwData & 0x80 ) && !mouseButtonsDown[MOUSE_BUTTON_RIGHT] ) {
							MouseEvent( TG_RMBUTTON, cursor.x, cursor.y, KS_DOWN );
						} else if ( !( event->dwData & 0x80 ) && mouseButtonsDown[MOUSE_BUTTON_RIGHT] ) {
							MouseEvent( TG_RMBUTTON, cursor.x, cursor.y, KS_UP );
						}
						break;
				}
			}
		}
	}

	if ( centerMouse ) {
		HWND window = NiApplication::application->GetAppWindow()->GetWindow();
		if ( GetForegroundWindow() == window
			|| IsChild( NiApplication::application->GetAppWindow()->GetWindow(), GetForegroundWindow() )
			|| IsChild( GetForegroundWindow(), NiApplication::application->GetAppWindow()->GetWindow() ) ) {
			NiAppWindow *appWindow = NiApplication::application->GetAppWindow();
			int x = appWindow->GetWidth() / 2;
			int y = appWindow->GetHeight() / 2;
			RECT rect;
			if ( GetWindowRect( appWindow->GetWindow(), &rect ) ) {
				x += rect.left;
				y += rect.top;
			}
			SetCursorPos( x, y );
		}
	}

	if ( unknown04 & 4 ) {
		for ( int gamepad = 0; gamepad < TGINPUT_MAX_GAMEPADS; gamepad++ ) {
			TGGamepad *pad = gamepads[gamepad];
			if ( pad == NULL ) {
				continue;
			}
			if ( !pad->Poll() ) {
				continue;
			}
			const TGGamepadState *state = pad->GetState();
			unsigned int buttons = state->buttons;
			float value;

			value = (float)state->values[GAMEPAD_AXIS_X] / state->maximums[GAMEPAD_AXIS_X];
			if ( value != lastX[gamepad] ) {
				GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_X, value, GAMEPAD_STATE_AXIS );
				lastX[gamepad] = value;
			}
			value = (float)state->values[GAMEPAD_AXIS_Y] / state->maximums[GAMEPAD_AXIS_Y];
			if ( value != lastY[gamepad] ) {
				GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_Y, value, GAMEPAD_STATE_AXIS );
				lastY[gamepad] = value;
			}
			if ( state->maximums[GAMEPAD_AXIS_Z] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_Z] / state->maximums[GAMEPAD_AXIS_Z];
				if ( value != lastZ[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_Z, value, GAMEPAD_STATE_AXIS );
					lastZ[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_RZ] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_RZ] / state->maximums[GAMEPAD_AXIS_RZ];
				if ( value != lastRz[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_RZ, value, GAMEPAD_STATE_AXIS );
					lastRz[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_RX] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_RX] / state->maximums[GAMEPAD_AXIS_RX];
				if ( value != lastRx[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_RX, value, GAMEPAD_STATE_AXIS );
					lastRx[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_RY] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_RY] / state->maximums[GAMEPAD_AXIS_RY];
				if ( value != lastRy[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_RY, value, GAMEPAD_STATE_AXIS );
					lastRy[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_SLIDER0] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_SLIDER0] / state->maximums[GAMEPAD_AXIS_SLIDER0];
				if ( value != lastSlider0[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_SLIDER0, value, GAMEPAD_STATE_AXIS );
					lastSlider0[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_SLIDER1] != 0 ) {
				value = (float)state->values[GAMEPAD_AXIS_SLIDER1] / state->maximums[GAMEPAD_AXIS_SLIDER1];
				if ( value != lastSlider1[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_SLIDER1, value, GAMEPAD_STATE_AXIS );
					lastSlider1[gamepad] = value;
				}
			}
			if ( state->maximums[GAMEPAD_AXIS_POV] != 0 ) {
				float pov;
				if ( (float)state->values[GAMEPAD_AXIS_POV] != -1.0f ) {
					pov = (float)state->values[GAMEPAD_AXIS_POV]
						/ ( state->maximums[GAMEPAD_AXIS_POV] - state->minimums[GAMEPAD_AXIS_POV] + 1.0f );
				} else {
					pov = -1.0f;
				}
				if ( pov != lastPov[gamepad] ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV, pov, GAMEPAD_STATE_AXIS );
					lastPov[gamepad] = pov;
				}

				// The hat's eighth of a turn, and from it the directions held:
				// up, right, down, left.
				int octant = (int)( pov * 8.0f );
				int directions = 0;
				if ( octant == 7 || octant == 0 || octant == 1 ) {
					directions = 1;
				}
				if ( octant == 1 || octant == 2 || octant == 3 ) {
					directions |= 2;
				}
				if ( octant == 3 || octant == 4 || octant == 5 ) {
					directions |= 4;
				}
				if ( octant == 5 || octant == 6 || octant == 7 ) {
					directions |= 8;
				}
				if ( !( lastPovDirections[gamepad] & 1 ) && ( directions & 1 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_UP, pov, KS_DOWN );
				} else if ( ( lastPovDirections[gamepad] & 1 ) && !( directions & 1 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_UP, pov, KS_UP );
				}
				if ( !( lastPovDirections[gamepad] & 2 ) && ( directions & 2 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_RIGHT, pov, KS_DOWN );
				} else if ( ( lastPovDirections[gamepad] & 2 ) && !( directions & 2 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_RIGHT, pov, KS_UP );
				}
				if ( !( lastPovDirections[gamepad] & 4 ) && ( directions & 4 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_DOWN, pov, KS_DOWN );
				} else if ( ( lastPovDirections[gamepad] & 4 ) && !( directions & 4 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_DOWN, pov, KS_UP );
				}
				if ( !( lastPovDirections[gamepad] & 8 ) && ( directions & 8 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_LEFT, pov, KS_DOWN );
				} else if ( ( lastPovDirections[gamepad] & 8 ) && !( directions & 8 ) ) {
					GamepadEvent( gamepad + 2, GAMEPAD_CONTROL_POV_LEFT, pov, KS_UP );
				}
				lastPovDirections[gamepad] = directions;
			}

			for ( int button = 0; button < TGGAMEPAD_MAX_BUTTONS; button++ ) {
				unsigned int bit = 1 << button;
				float buttonValue = state->buttonStates[button < TGGAMEPAD_MAX_BUTTONS ? button : TGGAMEPAD_MAX_BUTTONS] * ( 1.0f / 255.0f );
				if ( ( buttons & bit ) && gamepadButtonValues[gamepad][button] != buttonValue ) {
					GamepadEvent( gamepad + 2, button, buttonValue, KS_DOWN );
				} else if ( ( lastButtons[gamepad] & bit ) && !( buttons & bit ) ) {
					GamepadEvent( gamepad + 2, button, buttonValue, KS_UP );
					SetGamepadButtonFlag( gamepad, button, false );
				}
				gamepadButtonValues[gamepad][button] = buttonValue;
			}
			lastButtons[gamepad] = buttons;
		}
	}
}

/*
================
TGInputManager::MoveMouseCursorTo

Starts a move of the cursor from where it is to the point, over the
duration; a move of no duration puts it there at once. The destination, when
there is one, hears of the start and the end.
================
*/
// FUNCTION: STBC 0x006e7010 ?MoveMouseCursorTo@TGInputManager@@QAEXMMMH@Z
// PARKED: register schedule: retail loads duration into edx before the copies and x, y after them; this build loads x first (NiPoint2( x, y ) the same)
void TGInputManager::MoveMouseCursorTo( float x, float y, float duration, int notifyDestination ) {
	moveStart = cursor;
	moveEnd.x = x;
	moveEnd.y = y;
	moveDuration = duration;
	moveElapsed = 0.0f;
	if ( x > 1.0f ) {
		moveEnd.x = 1.0f;
	}
	if ( moveEnd.x < 0.0f ) {
		moveEnd.x = 0.0f;
	}
	if ( y > 1.0f ) {
		moveEnd.y = 1.0f;
	}
	if ( moveEnd.y < 0.0f ) {
		moveEnd.y = 0.0f;
	}
	moveNotifyDestination = notifyDestination;
	if ( duration == 0.0f ) {
		if ( moveSuspendCount != 0 ) {
			savedCursor = moveEnd;
		} else {
			MouseEvent( 0, moveEnd.x, moveEnd.y, 0 );
		}
	}
	if ( moveNotifyDestination != 0 ) {
		float savedDuration = moveDuration;
		moveDuration = 0.0f;
		MouseEvent( moveNotifyDestination, moveStart.x, moveStart.y, 1 );
		moveDuration = savedDuration;
	}
}

/*
================
TGInputManager::PauseMouseCursorMoveTo

Holds the moves of the cursor back; the first pause keeps the cursor's place.
================
*/
// FUNCTION: STBC 0x006e7150 ?PauseMouseCursorMoveTo@TGInputManager@@QAEXXZ
void TGInputManager::PauseMouseCursorMoveTo( void ) {
	if ( moveSuspendCount == 0 ) {
		savedCursor = cursor;
	}
	moveSuspendCount++;
}

/*
================
TGInputManager::ResumeMouseCursorMoveTo

The last resume puts the cursor where it was held.
================
*/
// FUNCTION: STBC 0x006e7180 ?ResumeMouseCursorMoveTo@TGInputManager@@QAEXXZ
void TGInputManager::ResumeMouseCursorMoveTo( void ) {
	moveSuspendCount--;
	if ( moveSuspendCount <= 0 ) {
		moveSuspendCount = 0;
		MouseEvent( 0, savedCursor.x, savedCursor.y, 0 );
	}
}

/*
================
TGInputManager::Unknown6E71C0

Steps the cursor's move by the frame's game time, unless it is paused, and
moves the cursor along it. At the end of the move the destination, when
there is one, hears of it.
================
*/
// FUNCTION: STBC 0x006e71c0
// PARKED: x87 schedule: retail copies moveStart (the left operand of operator+) before the distance's subtraction and keeps its x on the FPU stack; a start local, distance * fraction + moveStart, += and one expression all score lower
void TGInputManager::Unknown6E71C0( void ) {
	if ( moveDuration == 0.0f ) {
		return;
	}
	if ( moveSuspendCount > 0 ) {
		return;
	}

	moveElapsed += UtopiaApp::GetApp()->GetGameFrameStep();
	if ( moveElapsed > moveDuration ) {
		moveElapsed = moveDuration;
	}
	NiPoint2 distance = moveEnd - moveStart;
	float duration = moveDuration;
	float fraction = sin( moveElapsed / moveDuration * HALF_PI );
	moveDuration = 0.0f;
	NiPoint2 position = moveStart + distance * fraction;
	MouseEvent( 0, position.x, position.y, 0 );
	moveDuration = duration;

	if ( moveElapsed == duration ) {
		moveDuration = 0.0f;
		moveElapsed = 0.0f;
		if ( moveNotifyDestination != 0 ) {
			moveDuration = 0.0f;
			MouseEvent( moveNotifyDestination, moveEnd.x, moveEnd.y, 2 );
			moveDuration = 0.0f;
		}
	}
}

/*
================
TGInputManager::WriteToStream

Saves the cursor's move.
================
*/
// FUNCTION: STBC 0x006e7330
bool TGInputManager::WriteToStream( TGStream *stream ) {
	stream->WriteFloat( moveDuration );
	stream->WriteFloat( moveElapsed );
	stream->WriteFloat( moveStart.x );
	stream->WriteFloat( moveStart.y );
	stream->WriteFloat( moveEnd.x );
	stream->WriteFloat( moveEnd.y );
	stream->WriteLong( moveSuspendCount );
	stream->WriteFloat( savedCursor.x );
	stream->WriteFloat( savedCursor.y );
	stream->WriteInt( moveNotifyDestination );
	return true;
}

/*
================
TGInputManager::ReadFromStream

Loads the cursor's move.
================
*/
// FUNCTION: STBC 0x006e73d0
bool TGInputManager::ReadFromStream( TGStream *stream ) {
	moveDuration = stream->ReadFloat();
	moveElapsed = stream->ReadFloat();
	moveStart.x = stream->ReadFloat();
	moveStart.y = stream->ReadFloat();
	moveEnd.x = stream->ReadFloat();
	moveEnd.y = stream->ReadFloat();
	moveSuspendCount = stream->ReadLong();
	savedCursor.x = stream->ReadFloat();
	savedCursor.y = stream->ReadFloat();
	moveNotifyDestination = stream->ReadInt();
	return true;
}

