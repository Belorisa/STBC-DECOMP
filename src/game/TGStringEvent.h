/*
===============================================================================

	TGStringEvent.h

	An event carrying a string.

	Evidence: the vtable 0x8887AC; GetObjType returns 0x109, the type
	TGStringEvent::Cast checks. The SWIG wrappers TGStringEvent_GetString
	and SetString inline the copy out of and into the string. The
	constructor is an inline: most units call the copy kept at 0x407C70 (Game's), Episode's expands it. The overrides of slots 4, 5 and 12-14 are in
	TGEvent's unit: they copy, write and read the string at 0x28.

===============================================================================
*/

#ifndef __TGSTRINGEVENT_H__
#define __TGSTRINGEVENT_H__

#include "TGEvent.h"
#include "TGString.h"

// GetObjType's value for a TGStringEvent.
#define TGSTRINGEVENT_TYPE		0x109

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00405740 ??_GTGStringEvent@@UAEPAXI@Z @game/Game
// FUNCTION: STBC 0x00405770 ??1TGStringEvent@@UAE@XZ @game/Game
class TGStringEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x00407c70 ??0TGStringEvent@@QAE@J@Z @game/Game
					TGStringEvent( long id = 0 ) : TGEvent( id ) {}

	// TGObject
	// FUNCTION: STBC 0x004056d0 ?GetObjType@TGStringEvent@@UAEHXZ @game/Game
	virtual int		GetObjType( void ) { return TGSTRINGEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00405710 ?GetClassName@TGStringEvent@@UAEPADXZ @game/Game
	virtual char *	GetClassName( void ) { return "TGStringEvent"; }
	// FUNCTION: STBC 0x00405720 ?GetSwigTypeName@TGStringEvent@@UAEPADXZ @game/Game
	virtual char *	GetSwigTypeName( void ) { return "_p_TGStringEvent"; }
	// FUNCTION: STBC 0x00405730 ?GetPythonClassName@TGStringEvent@@UAEPADXZ @game/Game
	virtual char *	GetPythonClassName( void ) { return "TGStringEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGStringEvent *	Create( void ) { return new TGStringEvent; }
	static TGStringEvent *	Cast( TGObject *object );

	friend PyObject *	TGStringEvent_GetCString( PyObject *self, PyObject *args );

	void			GetString( TGString *string ) { *string = value; }
	void			SetString( char *string ) { value.AssignC( string ); }
	// Copies a whole string, for the GameSpy server list (invented overload).
	void			SetString( const TGString &string ) { value = string; }
	// A new block holding the string narrowed to chars (invented name).
	char *			GetCString( void ) { return value.Unknown6F56F0(); }

protected:
	TGString		value;					// 0x28
};

/*
================
TGStringEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x004056e0 ?IsTypeOf@TGStringEvent@@UAE_NH@Z @game/Game
inline bool TGStringEvent::IsTypeOf( int type ) {
	if ( type == TGSTRINGEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

// The Python method GetCString of the shadow class: the string as a Python
// string (PyEmbed::AddClassMethod from TGEventModule's startup).
PyObject *	TGStringEvent_GetCString( PyObject *self, PyObject *args );

#endif /* !__TGSTRINGEVENT_H__ */
