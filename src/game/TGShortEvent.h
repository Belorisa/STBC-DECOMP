/*
===============================================================================

	TGShortEvent.h

	An event carrying a 16-bit integer.

	0x2C bytes. Evidence: the inline constructor calls TGEvent's
	and stores the vtable 0x89451C; GetObjType returns 0x106, the
	type TGShortEvent::Cast checks. The accessor names are those of the SWIG wrappers
	TGShortEvent_GetShort and TGShortEvent_SetShort, whose inline bodies read and write
	the value at 0x28. The overrides of slots 4, 5 and 12-14 are in TGEvent's
	unit.

===============================================================================
*/

#ifndef __TGSHORTEVENT_H__
#define __TGSHORTEVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGShortEvent.
#define TGSHORTEVENT_TYPE		0x106

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005c70c0 ??_GTGShortEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005c70f0 ??1TGShortEvent@@UAE@XZ @game/AppWrap
class TGShortEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x005c7030 ??0TGShortEvent@@QAE@J@Z @game/AppWrap
					TGShortEvent( long id = 0 ) : TGEvent( id ) { value = 0; }

	// TGObject
	// FUNCTION: STBC 0x005c7050 ?GetObjType@TGShortEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGSHORTEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005c7090 ?GetClassName@TGShortEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGShortEvent"; }
	// FUNCTION: STBC 0x005c70a0 ?GetSwigTypeName@TGShortEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGShortEvent"; }
	// FUNCTION: STBC 0x005c70b0 ?GetPythonClassName@TGShortEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGShortEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGShortEvent *	Create( void ) { return new TGShortEvent; }
	static TGShortEvent *	Cast( TGObject *object );

	short		GetShort( void ) { return value; }
	void			SetShort( short value ) { this->value = value; }

protected:
	short		value;					// 0x28
};

/*
================
TGShortEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005c7060 ?IsTypeOf@TGShortEvent@@UAE_NH@Z @game/AppWrap
inline bool TGShortEvent::IsTypeOf( int type ) {
	if ( type == TGSHORTEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGSHORTEVENT_H__ */
