/*
===============================================================================

	TGSequenceEvent.h

	The event a sequence sends to g_kTGActionManager to start one of its
	actions: the action (by ID, TGObjPtrEvent's object) and the delay to
	start it after. A sequence adds it to the completion events of the
	action the new one waits for.

	0x30 bytes. Evidence: the constructor (0x6FF430) calls TGEvent's, clears
	0x28, stores the vtable 0x894A20 and clears 0x2C; GetObjType returns
	0x704 and IsTypeOf accepts TGObjPtrEvent's 0x10C as well. The class
	strings are "TGSequenceEvent", "_p_TGSequenceEvent" and
	"TGSequenceEventPtr". TGSequence's unit holds the overrides of slots 4,
	5 and 12, which copy and stream the delay.

===============================================================================
*/

#ifndef __TGSEQUENCEEVENT_H__
#define __TGSEQUENCEEVENT_H__

#include "TGObjPtrEvent.h"

// GetObjType's value for a TGSequenceEvent.
#define TGSEQUENCEEVENT_TYPE	0x704

// The vtable's inline members and the implicit destructor are emitted with
// CharacterAction, which makes these events.
// FUNCTION: STBC 0x00672180 ??_GTGSequenceEvent@@UAEPAXI@Z @game/CharacterAction
// FUNCTION: STBC 0x006721b0 ??1TGSequenceEvent@@UAE@XZ @game/CharacterAction
class TGSequenceEvent : public TGObjPtrEvent {
public:
					// FUNCTION: STBC 0x006ff430 ??0TGSequenceEvent@@QAE@XZ @game/TGAction
					TGSequenceEvent( void ) { delay = 0.0f; }
	// The load's: the delay is read after.
	// FUNCTION: STBC 0x006fd0f0 ??0TGSequenceEvent@@QAE@J@Z
					TGSequenceEvent( long id ) : TGObjPtrEvent( id ) {}

	// TGObject
	// FUNCTION: STBC 0x00672100 ?GetObjType@TGSequenceEvent@@UAEHXZ @game/CharacterAction
	virtual int		GetObjType( void ) { return TGSEQUENCEEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00672150 ?GetClassName@TGSequenceEvent@@UAEPADXZ @game/CharacterAction
	virtual char *	GetClassName( void ) { return "TGSequenceEvent"; }
	// FUNCTION: STBC 0x00672160 ?GetSwigTypeName@TGSequenceEvent@@UAEPADXZ @game/CharacterAction
	virtual char *	GetSwigTypeName( void ) { return "_p_TGSequenceEvent"; }
	// FUNCTION: STBC 0x00672170 ?GetPythonClassName@TGSequenceEvent@@UAEPADXZ @game/CharacterAction
	virtual char *	GetPythonClassName( void ) { return "TGSequenceEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	float			GetDelay( void ) { return delay; }					// invented name
	void			SetDelay( float delay_ ) { delay = delay_; }		// invented name

protected:
	float			delay;					// 0x2C seconds
};

/*
================
TGSequenceEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00672110 ?IsTypeOf@TGSequenceEvent@@UAE_NH@Z @game/CharacterAction
inline bool TGSequenceEvent::IsTypeOf( int type ) {
	if ( type == TGSEQUENCEEVENT_TYPE ) {
		return true;
	}
	return TGObjPtrEvent::IsTypeOf( type );
}

#endif /* !__TGSEQUENCEEVENT_H__ */
