/*
===============================================================================

	TGBoolEvent.h

	An event carrying a truth value, kept in a bit of TGEvent's flags
	(EVENT_BOOL_VALUE): the class adds no member.

	0x28 bytes. Evidence: the inline constructor (0x4371B0) calls TGEvent's,
	clears bit 8 of the flags and stores the vtable 0x889A5C; GetObjType
	returns 0x104, the type TGBoolEvent::Cast checks. The accessor names are
	those of the SWIG wrappers TGBoolEvent_GetBool, SetBool and ToggleBool,
	whose inline bodies test, set and flip that bit. The overrides of slots
	4, 5, 13 and 14 are in TGEvent's unit.

===============================================================================
*/

#ifndef __TGBOOLEVENT_H__
#define __TGBOOLEVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGBoolEvent.
#define TGBOOLEVENT_TYPE		0x104

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00437240 ??_GTGBoolEvent@@UAEPAXI@Z @game/TGParagraph
// FUNCTION: STBC 0x00437270 ??1TGBoolEvent@@UAE@XZ @game/TGParagraph
class TGBoolEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x004371b0 ??0TGBoolEvent@@QAE@J@Z @game/TGParagraph
					TGBoolEvent( long id = 0 ) : TGEvent( id ) { SetBool( false ); }

	// TGObject
	// FUNCTION: STBC 0x004371d0 ?GetObjType@TGBoolEvent@@UAEHXZ @game/TGParagraph
	virtual int		GetObjType( void ) { return TGBOOLEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00437210 ?GetClassName@TGBoolEvent@@UAEPADXZ @game/TGParagraph
	virtual char *	GetClassName( void ) { return "TGBoolEvent"; }
	// FUNCTION: STBC 0x00437220 ?GetSwigTypeName@TGBoolEvent@@UAEPADXZ @game/TGParagraph
	virtual char *	GetSwigTypeName( void ) { return "_p_TGBoolEvent"; }
	// FUNCTION: STBC 0x00437230 ?GetPythonClassName@TGBoolEvent@@UAEPADXZ @game/TGParagraph
	virtual char *	GetPythonClassName( void ) { return "TGBoolEventPtr"; }

	// TGEvent
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGBoolEvent *	Create( void ) { return new TGBoolEvent; }
	static TGBoolEvent *	Cast( TGObject *object );

	bool			GetBool( void ) { return ( flags >> 8 ) & 1; }
	void			SetBool( bool value );
	void			ToggleBool( void ) { flags ^= EVENT_BOOL_VALUE; }
};

/*
================
TGBoolEvent::SetBool
================
*/
inline void TGBoolEvent::SetBool( bool value ) {
	if ( value ) {
		flags |= EVENT_BOOL_VALUE;
	} else {
		flags &= ~EVENT_BOOL_VALUE;
	}
}

/*
================
TGBoolEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x004371e0 ?IsTypeOf@TGBoolEvent@@UAE_NH@Z @game/TGParagraph
inline bool TGBoolEvent::IsTypeOf( int type ) {
	if ( type == TGBOOLEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGBOOLEVENT_H__ */
