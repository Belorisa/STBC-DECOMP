/*
===============================================================================

	TGObjPtrEvent.h

	An event naming an object, kept by ID so that the event survives the
	object.

	0x2C bytes. Evidence: the inline constructor (0x403290) calls TGEvent's,
	stores the vtable 0x88869C and clears 0x28; GetObjType returns 0x10C, the
	type TGObjPtrEvent::Cast checks. The SWIG wrapper TGObjPtrEvent_SetObjPtr
	stores the object's ID at 0x28, and GetObjPtr looks it up
	(GetTGObjectPtr). The overrides of slots 4, 5 and 12-14 are in TGEvent's
	unit.

===============================================================================
*/

#ifndef __TGOBJPTREVENT_H__
#define __TGOBJPTREVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGObjPtrEvent.
#define TGOBJPTREVENT_TYPE		0x10C

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00403320 ??_GTGObjPtrEvent@@UAEPAXI@Z @game/Game
// FUNCTION: STBC 0x00403350 ??1TGObjPtrEvent@@UAE@XZ @game/Game
class TGObjPtrEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x00403290 ??0TGObjPtrEvent@@QAE@J@Z @game/Game
					TGObjPtrEvent( long id = 0 ) : TGEvent( id ) { objectID = 0; }

	// TGObject
	// FUNCTION: STBC 0x004032b0 ?GetObjType@TGObjPtrEvent@@UAEHXZ @game/Game
	virtual int		GetObjType( void ) { return TGOBJPTREVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x004032f0 ?GetClassName@TGObjPtrEvent@@UAEPADXZ @game/Game
	virtual char *	GetClassName( void ) { return "TGObjPtrEvent"; }
	// FUNCTION: STBC 0x00403300 ?GetSwigTypeName@TGObjPtrEvent@@UAEPADXZ @game/Game
	virtual char *	GetSwigTypeName( void ) { return "_p_TGObjPtrEvent"; }
	// FUNCTION: STBC 0x00403310 ?GetPythonClassName@TGObjPtrEvent@@UAEPADXZ @game/Game
	virtual char *	GetPythonClassName( void ) { return "TGObjPtrEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGObjPtrEvent *	Create( void ) { return new TGObjPtrEvent; }
	static TGObjPtrEvent *	Cast( TGObject *object );

	TGObject *		GetObjPtr( void ) { return GetTGObjectPtr( objectID ); }
	void			SetObjPtr( TGObject *object ) { objectID = ( object == NULL ) ? 0 : object->GetObjID(); }

protected:
	long			objectID;				// 0x28
};

/*
================
TGObjPtrEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x004032c0 ?IsTypeOf@TGObjPtrEvent@@UAE_NH@Z @game/Game
inline bool TGObjPtrEvent::IsTypeOf( int type ) {
	if ( type == TGOBJPTREVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGOBJPTREVENT_H__ */
