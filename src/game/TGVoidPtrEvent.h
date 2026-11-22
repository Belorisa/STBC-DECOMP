/*
===============================================================================

	TGVoidPtrEvent.h

	An event carrying a pointer that means nothing outside the process: it
	is neither saved nor read back (WriteToStream and ReadFromStream fail),
	and sent over the network without it.

	0x2C bytes. Evidence: the inline constructor (0x5C7C50) calls TGEvent's,
	stores the vtable 0x894558 and clears 0x28; GetObjType returns 0x10A and
	GetClassName "TGVoidPtrEvent". Slot 12 (Copy) is in TGEvent's unit.

===============================================================================
*/

#ifndef __TGVOIDPTREVENT_H__
#define __TGVOIDPTREVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGVoidPtrEvent.
#define TGVOIDPTREVENT_TYPE		0x10A

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005c7d00 ??_GTGVoidPtrEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005c7d30 ??1TGVoidPtrEvent@@UAE@XZ @game/AppWrap
class TGVoidPtrEvent : public TGEvent {
public:
					TGVoidPtrEvent( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x005C7C70 ?GetObjType@TGVoidPtrEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGVOIDPTREVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x005c7ce0 ?WriteToStream@TGVoidPtrEvent@@UAE_NPAVTGStream@@@Z @game/AppWrap
	virtual bool	WriteToStream( TGStream *stream ) { return false; }
	// FUNCTION: STBC 0x005c7cf0 ?ReadFromStream@TGVoidPtrEvent@@UAE_NPAVTGStream@@@Z @game/AppWrap
	virtual bool	ReadFromStream( TGStream *stream ) { return false; }
	// FUNCTION: STBC 0x005c7cb0 ?GetClassName@TGVoidPtrEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGVoidPtrEvent"; }
	// FUNCTION: STBC 0x005c7cc0 ?GetSwigTypeName@TGVoidPtrEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGVoidPtrEvent"; }
	// FUNCTION: STBC 0x005c7cd0 ?GetPythonClassName@TGVoidPtrEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGVoidPtrEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	static TGVoidPtrEvent *	Create( void ) { return new TGVoidPtrEvent; }

	void *			GetVoidPtr( void ) { return value; }
	void			SetVoidPtr( void *ptr ) { value = ptr; }

protected:
	void *			value;					// 0x28
};

/*
================
TGVoidPtrEvent::TGVoidPtrEvent
================
*/
// FUNCTION: STBC 0x005c7c50
inline TGVoidPtrEvent::TGVoidPtrEvent( long id ) : TGEvent( id ) {
	value = NULL;
}

/*
================
TGVoidPtrEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005c7c80
inline bool TGVoidPtrEvent::IsTypeOf( int type ) {
	if ( type == TGVOIDPTREVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGVOIDPTREVENT_H__ */
