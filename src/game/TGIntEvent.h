/*
===============================================================================

	TGIntEvent.h

	An event carrying an integer.

	0x2C bytes. Evidence: the inline constructor calls TGEvent's
	and stores the vtable 0x88B5C8; GetObjType returns 0x107, the
	type TGIntEvent::Cast checks. The accessor names are those of the SWIG wrappers
	TGIntEvent_GetInt and TGIntEvent_SetInt, whose inline bodies read and write
	the value at 0x28. The overrides of slots 4, 5 and 12-14 are in TGEvent's
	unit.

===============================================================================
*/

#ifndef __TGINTEVENT_H__
#define __TGINTEVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGIntEvent.
#define TGINTEVENT_TYPE		0x107

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00445b80 ??_GTGIntEvent@@UAEPAXI@Z
// FUNCTION: STBC 0x00445bb0 ??1TGIntEvent@@UAE@XZ
class TGIntEvent : public TGEvent {
public:
					// FUNCTION: STBC 0x00498bd0 ??0TGIntEvent@@QAE@J@Z @game/Editor
					TGIntEvent( long id = 0 ) : TGEvent( id ) { value = 0; }

	// TGObject
	// FUNCTION: STBC 0x00445b10 ?GetObjType@TGIntEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return TGINTEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00445b50 ?GetClassName@TGIntEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGIntEvent"; }
	// FUNCTION: STBC 0x00445b60 ?GetSwigTypeName@TGIntEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGIntEvent"; }
	// FUNCTION: STBC 0x00445b70 ?GetPythonClassName@TGIntEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGIntEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGIntEvent *	Create( void ) { return new TGIntEvent; }
	static TGIntEvent *	Cast( TGObject *object );

	int		GetInt( void ) { return value; }
	void			SetInt( int value ) { this->value = value; }

protected:
	int		value;					// 0x28
};

/*
================
TGIntEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00445b20
inline bool TGIntEvent::IsTypeOf( int type ) {
	if ( type == TGINTEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGINTEVENT_H__ */
