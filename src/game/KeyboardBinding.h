/*
===============================================================================

	KeyboardBinding.h

	The members of KeyboardBinding that the SWIG wrappers KeyboardBinding_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __KEYBOARDBINDING_H__
#define __KEYBOARDBINDING_H__

#include "NiTMap.h"
#include "NiTList.h"

class TGEventHandlerObject;
class TGString;
class TGObject;

// A key bound to an event. Evidence: GenerateMappingFile (0x55B1A0) writes
// one "BindKey" line per binding, choosing the event getter from the first
// member (GET_EVENT, GET_INT_EVENT, GET_BOOL_EVENT, GET_FLOAT_EVENT) and the
// last from KS_NORMAL, KS_KEYDOWN and KS_KEYUP; GetNumKeys (0x55A3D0) and
// FindKey (0x55A4C0) compare these members with their arguments. The names
// are invented.
struct KeyBind {
	int				eventKind;			// 0x00 0 event, 1 int event, 2 bool event, 3 float event
	int				eventType;			// 0x04 the event's type
	float			value;				// 0x08 the event's value, compared within KEY_BIND_EPSILON
	int				keyState;			// 0x0C KS_NORMAL, KS_KEYDOWN or KS_KEYUP
	float			time;				// 0x10 the application's time when it was bound

	// BindKey's new binding (0x559B2B) clears the first four members, not
	// the time.
					KeyBind( void ) { eventKind = 0; eventType = 0; value = 0.0f; keyState = 0; }
};

// A key and the time of its newest binding: BindKey (0x5599D0) lists the
// keys bound to an event in one of these each (8 bytes, from new), sorted
// newest first, to drop the bindings of the oldest keys past maxBoundKeys.
// The struct and its names are invented.
struct BoundKey {
	int				key;				// 0x00
	float			time;				// 0x04 KeyBind::time

	// BindKey's news of one (0x559AA1, 0x559D5C) store both members after
	// the null test.
					BoundKey( int key_, float time_ ) { key = key_; time = time_; }
};

class KeyboardBinding {
public:
	// The kind of event a binding sends (KeyBind::eventKind).
	enum eventKind_t {
		GET_EVENT,
		GET_INT_EVENT,
		GET_BOOL_EVENT,
		GET_FLOAT_EVENT
	};

	// BindKey's flags (the bits it tests; the names are invented, and the
	// wrappers' default of 1 sets a bit BindKey doesn't read).
	enum bindFlags_t {
		BIND_REPLACE			= 0x02,		// the key's other bindings go
		BIND_UNBIND_OTHER_KEYS	= 0x04,		// the binding goes from every other key
		BIND_LOCK_KEY			= 0x08		// the key joins the locked keys, which BindKey won't rebind
	};

					KeyboardBinding( void );
					~KeyboardBinding( void );

	// Binds the key in the key state to the event; false when the key is
	// locked. The event keeps at most maxBoundKeys keys, the newest.
	bool	BindKey( int key, int keyState, int eventType, int eventKind, float value, long flags,
					 bool writeMappingFile );
	void	SetMaxBoundKeys( int maxBoundKeys_ ) { maxBoundKeys = maxBoundKeys_; }
	int	GetNumKeys( int, int, float );
	int	FindKey( int, int, float, int );
	// The string of the keys bound to an event (retail 0x55A5C0: the SWIG
	// wrapper KeyboardBinding_FindKeyString calls it, defaulting the three
	// characters to '(', ',' and ')'; the parameter names are invented).
	void	FindKeyString( TGString *, int, int, float, char open, char separator, char close );
	void	ClearBinding( int, int, float );
	void	GenerateMappingFile( void );
	void	Unknown55AF50( void );
	void	RebuildMappingFromFile( char * );

	// Sends the events bound to the key in the key state, then those of the
	// key's other character on the same scan code (bool events only); true
	// when each was handled.
	bool	LaunchEvent( int key, int keyState, TGEventHandlerObject *destination, TGObject *source = NULL,
						 bool alternateKey = false );

protected:
	// 0x00-0x1B are not modelled yet.
	int				maxBoundKeys;			// 0x00
	NiTList<int>	unknown04;			// 0x04 the locked keys (BindKey's BIND_LOCK_KEY); two items per block (the constructor 0x559860); its NewItem is NiTList<int>'s (0x486880), shared with TGInputManager::keysDown
	NiTMap<int, NiTList<KeyBind *> *>	keyBinds;	// 0x1C the bindings of each key (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	// BindKey for a key given as its character: the SWIG wrapper KeyboardBinding_BindKeyChar
	// widens the character and calls BindKey (0x5599D0) itself, so this is an inline (the
	// wrappers default the last two arguments to 1 and false).
	bool	BindKeyChar( char key, int keyState, int eventType, int eventKind, float value, long flags,
						 bool writeMappingFile ) {
		return BindKey( key, keyState, eventType, eventKind, value, flags, writeMappingFile );
	}
};

// The game's key bindings (the SWIG variable g_kKeyboardBinding).
extern KeyboardBinding	g_kKeyboardBinding;

#endif /* !__KEYBOARDBINDING_H__ */
