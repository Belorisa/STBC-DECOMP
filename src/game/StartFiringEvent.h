/*
===============================================================================

	StartFiringEvent.h

	The members of StartFiringEvent that the SWIG wrappers StartFiringEvent_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __STARTFIRINGEVENT_H__
#define __STARTFIRINGEVENT_H__

#include "TGEvent.h"
#include "NiPoint3.h"

class ObjectClass;

// GetObjType's value for a StartFiringEvent.
#define STARTFIRINGEVENT_TYPE	0x8128

// 0x3C bytes, allocated from the pools: UtopiaModule's creator 0x43F760
// (type 0x8128) calls the loader constructor. The vtable 0x88A13C overrides
// TGEvent's type, name and both pairs of stream slots; the destructor is
// the implicit one (a bare jump to TGEvent's). The inline copies are those
// of the class registration's unit.
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043f840 ??_GStartFiringEvent@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x0043f870 ??1StartFiringEvent@@UAE@XZ @game/UtopiaModule
class StartFiringEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x0043f790 ??0StartFiringEvent@@QAE@J@Z @game/UtopiaModule
					StartFiringEvent( long id = 0 ) : TGEvent( id ) { targetID = 0; unknown2C = 0; offset = NiPoint3::ZERO; }

	// TGObject
	// FUNCTION: STBC 0x0043f7d0 ?GetObjType@StartFiringEvent@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return STARTFIRINGEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0043f810 ?GetClassName@StartFiringEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "StartFiringEvent"; }
	// FUNCTION: STBC 0x0043f820 ?GetSwigTypeName@StartFiringEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_StartFiringEvent"; }
	// FUNCTION: STBC 0x0043f830 ?GetPythonClassName@StartFiringEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "StartFiringEventPtr"; }

	// TGEvent
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	ObjectClass *	GetTarget( void );
	// (invented, as Torpedo's GetTargetID: WriteToNetStream's inline level
	// on the ID; SWIG shows only GetTarget and GetOffset)
	long			GetTargetID( void ) { return targetID; }
	void			SetTarget( ObjectClass *target );		// invented name
	void			SetOffset( NiPoint3 *offset_ );		// invented name
	NiPoint3 *		GetOffset( void ) { return &offset; }

protected:
	long			targetID;			// 0x28 the target's ID (invented name)
	int				unknown2C;			// 0x2C
	NiPoint3		offset;				// 0x30 where on the target to fire (invented name)
};

/*
================
StartFiringEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043f7e0 ?IsTypeOf@StartFiringEvent@@UAE_NH@Z @game/UtopiaModule
inline bool StartFiringEvent::IsTypeOf( int type ) {
	if ( type == STARTFIRINGEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__STARTFIRINGEVENT_H__ */
