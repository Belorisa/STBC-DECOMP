/*
===============================================================================

	TGCharEvent.h

	An event carrying a character.

	0x2C bytes. Evidence: the inline constructor (0x574C20) calls
	TGEvent's and stores the vtable 0x8932DC; GetObjType returns 0x105, the
	type TGCharEvent::Cast checks. The accessor names are those of the SWIG wrappers
	TGCharEvent_GetChar and TGCharEvent_SetChar, whose inline bodies read and write
	the value at 0x28. The overrides of slots 4, 5 and 12-14 are in TGEvent's
	unit.

===============================================================================
*/

#ifndef __TGCHAREVENT_H__
#define __TGCHAREVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGCharEvent.
#define TGCHAREVENT_TYPE		0x105

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00574cb0 ??_GTGCharEvent@@UAEPAXI@Z @game/TorpedoSystem
// FUNCTION: STBC 0x00574ce0 ??1TGCharEvent@@UAE@XZ @game/TorpedoSystem
class TGCharEvent : public TGEvent {
public:
					// FUNCTION: STBC 0x00574c20 ??0TGCharEvent@@QAE@J@Z @game/TorpedoSystem
					TGCharEvent( long id = 0 ) : TGEvent( id ) { value = 0; }

	// TGObject
	// FUNCTION: STBC 0x00574c40 ?GetObjType@TGCharEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return TGCHAREVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00574c80 ?GetClassName@TGCharEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGCharEvent"; }
	// FUNCTION: STBC 0x00574c90 ?GetSwigTypeName@TGCharEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGCharEvent"; }
	// FUNCTION: STBC 0x00574ca0 ?GetPythonClassName@TGCharEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGCharEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGCharEvent *	Create( void ) { return new TGCharEvent; }
	static TGCharEvent *	Cast( TGObject *object );

	char		GetChar( void ) { return value; }
	void			SetChar( char value ) { this->value = value; }

protected:
	char		value;					// 0x28
};

/*
================
TGCharEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00574c50
inline bool TGCharEvent::IsTypeOf( int type ) {
	if ( type == TGCHAREVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGCHAREVENT_H__ */
