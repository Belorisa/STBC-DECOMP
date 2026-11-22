/*
===============================================================================

	TGIEvent.h

	An input event: the base of the mouse, keyboard and gamepad events. A
	handler that uses the event marks it handled in a bit of TGEvent's
	flags.

	Evidence: the SWIG wrapper TGIEvent_SetHandled sets bit 8 of the flags
	word (0x19 |= 1), and TGIEvent_EventHandled tests it; the SWIG type
	"_p_TGIEvent" lists TGMouseEvent, TGGamepadEvent and TGKeyboardEvent as
	its subclasses. The load constructor (0x6E3D00, built by
	TGInputModule's creator 0x6E3CD0 in 0x28 bytes) calls TGEvent's and
	stores the vtable 0x894594 (TGEvent's 15 slots); GetObjType returns
	0x403. The destructor is implicit.

===============================================================================
*/

#ifndef __TGIEVENT_H__
#define __TGIEVENT_H__

#include "TGEvent.h"

// An input event a handler has used: the bit TGBoolEvent keeps its value in.
#define EVENT_HANDLED			0x0100
// The flag bits of input events: the handled bit and the subclasses' own
// (the mouse buttons, MEF_*).
#define INPUT_EVENT_FLAGS		0xFF00

// GetObjType's value for a TGIEvent (its slot 1, 0x5C9550).
#define TGIEVENT_TYPE			0x403

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005c95c0 ??_GTGIEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005c95f0 ??1TGIEvent@@UAE@XZ @game/AppWrap
class TGIEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x006e3d00 ??0TGIEvent@@QAE@J@Z @game/TGInputModule
					TGIEvent( long id ) : TGEvent( id ) {}
					// A new input event starts with its input flags clear (the
					// out-of-line copy is 0x5C9530).
	// FUNCTION: STBC 0x005c9530 ??0TGIEvent@@QAE@XZ @game/AppWrap
					TGIEvent( void ) { flags &= ~INPUT_EVENT_FLAGS; }

	// Sets input flag bits; the others are left alone (invented name).
	void			AddInputFlags( int inputFlags ) { flags |= inputFlags & INPUT_EVENT_FLAGS; }

	// TGObject
	// FUNCTION: STBC 0x005c9550 ?GetObjType@TGIEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGIEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x005c9590 ?GetClassName@TGIEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGIEvent"; }
	// FUNCTION: STBC 0x005c95a0 ?GetSwigTypeName@TGIEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGIEvent"; }
	// FUNCTION: STBC 0x005c95b0 ?GetPythonClassName@TGIEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGIEventPtr"; }

	bool			EventHandled( void ) { return ( flags >> 8 ) & 1; }
	void			SetHandled( void ) { flags |= EVENT_HANDLED; }
};

/*
================
TGIEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005c9560 ?IsTypeOf@TGIEvent@@UAE_NH@Z @game/AppWrap
inline bool TGIEvent::IsTypeOf( int type ) {
	if ( type == TGIEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGIEVENT_H__ */
