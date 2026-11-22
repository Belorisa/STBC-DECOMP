/*
===============================================================================

	TGKeyboardEvent.h

	A keyboard event (ET_KEYBOARD): the character typed and the key.

	Evidence: the SWIG wrappers TGKeyboardEvent_GetUnicode, GetKeyData and
	GetKeyState read 0x28, 0x2C and 0x30. The load constructor (0x6E3DA0,
	built by TGInputModule's creator 0x6E3D70 in 0x34 bytes) stores the
	vtable 0x89460C; it overrides Copy (0x6E8740), WriteToStream (0x6E8770)
	and ReadFromStream (0x6E87B0). The destructor is implicit. The
	character is a 32-bit member: ReadFromStream stores the 16-bit
	character it reads as a dword, Copy copies a dword, and the SWIG
	getter passes the whole dword to PyInt_FromLong.

===============================================================================
*/

#ifndef __TGKEYBOARDEVENT_H__
#define __TGKEYBOARDEVENT_H__

#include "TGIEvent.h"

// GetObjType's value for a TGKeyboardEvent (its slot 1, 0x5C9E30).
#define TGKEYBOARDEVENT_TYPE	0x404

// The event type of keyboard events (the SWIG constant ET_KEYBOARD).
#define ET_KEYBOARD				0x30002

// The key states (the SWIG constants TGKeyboardEvent_KS_*).
#define KS_NORMAL				0
#define KS_KEYDOWN				1
#define KS_KEYUP				2

// Characters of the keys without one, in the Unicode private use area
// (the SWIG constants WC_*).
#define WC_BACKSPACE			0x0008
#define WC_PRIVATE_USE_START	0xE000
#define WC_ESCAPE				0xE001
#define WC_LEFT					0xE007
#define WC_UP					0xE008
#define WC_RIGHT				0xE009
#define WC_DOWN					0xE00A
#define WC_INSERT				0xE00E
#define WC_DELETE				0xE00F
#define WC_PAGEUP				0xE011
#define WC_PAGEDOWN				0xE012
#define WC_END					0xE013
#define WC_HOME					0xE014
#define WC_NUMPAD0				0xE021
#define WC_NUMPAD9				0xE02A
#define WC_DECIMAL				0xE02B
#define WC_ADD					0xE02C
#define WC_SUBTRACT				0xE02D
#define WC_MULTIPLY				0xE02E
#define WC_DIVIDE				0xE02F
#define WC_CTRL_A				0xE061
#define WC_CTRL_E				0xE065
#define WC_CTRL_K				0xE06B
#define WC_LBUTTON				0xE095

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005c9eb0 ??_GTGKeyboardEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005c9ee0 ??1TGKeyboardEvent@@UAE@XZ @game/AppWrap
class TGKeyboardEvent : public TGIEvent {
public:
	// FUNCTION: STBC 0x006e3da0 ??0TGKeyboardEvent@@QAE@J@Z @game/TGInputModule
					TGKeyboardEvent( long id ) : TGIEvent( id ) {}
	// FUNCTION: STBC 0x005c9f50 ??0TGKeyboardEvent@@QAE@XZ @game/AppWrap
					TGKeyboardEvent( void ) { SetEventType( ET_KEYBOARD ); }
	// FUNCTION: STBC 0x005c9df0 ??0TGKeyboardEvent@@QAE@HJH@Z @game/AppWrap
					TGKeyboardEvent( int unicode_, long keyData_, int keyState_ ) { SetEventType( ET_KEYBOARD ); unicode = unicode_; keyData = keyData_; keyState = keyState_; }
	// The script-side constructor (the SWIG wrapper TGKeyboardEvent_Create expands it).
	static TGKeyboardEvent *	Create( int unicode, long keyData, int keyState ) { return new TGKeyboardEvent( unicode, keyData, keyState ); }

	// TGObject
	// FUNCTION: STBC 0x005c9e30 ?GetObjType@TGKeyboardEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGKEYBOARDEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005c9e80 ?GetClassName@TGKeyboardEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGKeyboardEvent"; }
	// FUNCTION: STBC 0x005c9e90 ?GetSwigTypeName@TGKeyboardEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGKeyboardEvent"; }
	// FUNCTION: STBC 0x005c9ea0 ?GetPythonClassName@TGKeyboardEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGKeyboardEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	int				GetUnicode( void ) { return unicode; }
	long			GetKeyData( void ) { return keyData; }
	int				GetKeyState( void ) { return keyState; }
	void	SetKeyState( int keyState_ ) { keyState = keyState_; }
	void	SetKeyData( long keyData_ ) { keyData = keyData_; }
	void	SetUnicode( int unicode_ ) { unicode = unicode_; }

	int				unicode;				// 0x28 the character typed
	long			keyData;				// 0x2C
	int				keyState;				// 0x30 KS_*
};

/*
================
TGKeyboardEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005c9e40 ?IsTypeOf@TGKeyboardEvent@@UAE_NH@Z @game/AppWrap
inline bool TGKeyboardEvent::IsTypeOf( int type ) {
	if ( type == TGKEYBOARDEVENT_TYPE ) {
		return true;
	}
	return TGIEvent::IsTypeOf( type );
}

#endif /* !__TGKEYBOARDEVENT_H__ */
