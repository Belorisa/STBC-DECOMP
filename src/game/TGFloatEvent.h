/*
===============================================================================

	TGFloatEvent.h

	An event carrying a float.

	0x2C bytes. Evidence: the inline constructor calls TGEvent's
	and stores the vtable 0x88C50C; GetObjType returns 0x108, the
	type TGFloatEvent::Cast checks. The accessor names are those of the SWIG wrappers
	TGFloatEvent_GetFloat and TGFloatEvent_SetFloat, whose inline bodies read and write
	the value at 0x28. The overrides of slots 4, 5 and 12-14 are in TGEvent's
	unit.

===============================================================================
*/

#ifndef __TGFLOATEVENT_H__
#define __TGFLOATEVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGFloatEvent.
#define TGFLOATEVENT_TYPE		0x108

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004a0f30 ??_GTGFloatEvent@@UAEPAXI@Z @game/PlacementEditor
// FUNCTION: STBC 0x004a0f60 ??1TGFloatEvent@@UAE@XZ @game/PlacementEditor
class TGFloatEvent : public TGEvent {
public:
					TGFloatEvent( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x004a0ec0 ?GetObjType@TGFloatEvent@@UAEHXZ @game/PlacementEditor
	virtual int		GetObjType( void ) { return TGFLOATEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x004a0f00 ?GetClassName@TGFloatEvent@@UAEPADXZ @game/PlacementEditor
	virtual char *	GetClassName( void ) { return "TGFloatEvent"; }
	// FUNCTION: STBC 0x004a0f10 ?GetSwigTypeName@TGFloatEvent@@UAEPADXZ @game/PlacementEditor
	virtual char *	GetSwigTypeName( void ) { return "_p_TGFloatEvent"; }
	// FUNCTION: STBC 0x004a0f20 ?GetPythonClassName@TGFloatEvent@@UAEPADXZ @game/PlacementEditor
	virtual char *	GetPythonClassName( void ) { return "TGFloatEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGFloatEvent *	Create( void ) { return new TGFloatEvent; }
	static TGFloatEvent *	Cast( TGObject *object );

	float		GetFloat( void ) { return value; }
	void			SetFloat( float value ) { this->value = value; }

protected:
	float		value;					// 0x28
};

/*
================
TGFloatEvent::TGFloatEvent
================
*/
// FUNCTION: STBC 0x004a0ea0
inline TGFloatEvent::TGFloatEvent( long id ) : TGEvent( id ) {
	value = 0;
}

/*
================
TGFloatEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x004a0ed0 @game/PlacementEditor
inline bool TGFloatEvent::IsTypeOf( int type ) {
	if ( type == TGFLOATEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGFLOATEVENT_H__ */
