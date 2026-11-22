/*
===============================================================================

	TGGamepadEvent.h

	A gamepad event (ET_GAMEPAD): three numbers and a value read from the
	pad, which the SWIG wrappers get and set together.

	0x38 bytes. Evidence: the load constructor (0x6E3DF0, built by
	TGInputModule's creator 0x6E3DC0 in 0x38 bytes) calls TGEvent's and
	stores the vtable 0x894648 (TGEvent's 15 slots); GetObjType returns
	0x406. TGGamepadEvent_GetInfo returns 0x34 and stores 0x2C, 0x30 and
	0x28 through its three pointers; TGGamepadEvent_SetInfo stores its
	arguments to 0x34, 0x2C, 0x30 and 0x28. The class overrides Copy
	(0x6E8800), WriteToStream (0x6E8840) and ReadFromStream (0x6E8890),
	which write 0x28 as a float, 0x2C and 0x30 as ints and 0x34 as a long.
	The destructor is implicit. What the values mean is not known: the
	member names are placeholders.

===============================================================================
*/

#ifndef __TGGAMEPADEVENT_H__
#define __TGGAMEPADEVENT_H__

#include "TGIEvent.h"

// The gamepad event type (the SWIG constant ET_GAMEPAD).
#define ET_GAMEPAD				0x30003

// GetObjType's value for a TGGamepadEvent (its slot 1, 0x5CA3D0).
#define TGGAMEPADEVENT_TYPE		0x406

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005ca450 ??_GTGGamepadEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005ca480 ??1TGGamepadEvent@@UAE@XZ @game/AppWrap
class TGGamepadEvent : public TGIEvent {
public:
	// The event the input manager builds: 0x5CA3A0 calls TGEvent's, clears the
	// values and stores the event type ET_GAMEPAD (0x30003).
	// FUNCTION: STBC 0x005ca3a0 ??0TGGamepadEvent@@QAE@XZ @game/AppWrap
					TGGamepadEvent( void ) {
						unknown2C = 0;
						unknown34 = 0;
						unknown30 = 0;
						unknown28 = 0.0f;
						SetEventType( ET_GAMEPAD );
					}
	// FUNCTION: STBC 0x006e3df0 ??0TGGamepadEvent@@QAE@J@Z @game/TGInputModule
					TGGamepadEvent( long id ) : TGIEvent( id ) {}

	// TGObject
	// FUNCTION: STBC 0x005ca3d0 ?GetObjType@TGGamepadEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGGAMEPADEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005ca420 ?GetClassName@TGGamepadEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGGamepadEvent"; }
	// FUNCTION: STBC 0x005ca430 ?GetSwigTypeName@TGGamepadEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGGamepadEvent"; }
	// FUNCTION: STBC 0x005ca440 ?GetPythonClassName@TGGamepadEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGGamepadEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	long			GetInfo( int *a, int *b, float *c ) { *a = unknown2C; *b = unknown30; *c = unknown28; return unknown34; }
	void			SetInfo( long value, int a, int b, float c ) { unknown2C = a; unknown34 = value; unknown28 = c; unknown30 = b; }

	float			unknown28;				// 0x28
	int				unknown2C;				// 0x2C
	int				unknown30;				// 0x30
	long			unknown34;				// 0x34
};

/*
================
TGGamepadEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005ca3e0 ?IsTypeOf@TGGamepadEvent@@UAE_NH@Z @game/AppWrap
inline bool TGGamepadEvent::IsTypeOf( int type ) {
	if ( type == TGGAMEPADEVENT_TYPE ) {
		return true;
	}
	return TGIEvent::IsTypeOf( type );
}

#endif /* !__TGGAMEPADEVENT_H__ */
