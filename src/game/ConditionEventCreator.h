/*
===============================================================================

	ConditionEventCreator.h

	Sends a copy of its event to the event manager whenever one of its
	conditions changes.

	0x1C bytes, allocated from the pools.

	Evidence: the vtables 0x88B2E4 (TGObject's 12 slots) and 0x88B314
	(TGConditionHandler's 8, the handler at 0x08). GetObjType returns
	0x8038, the type the save and load (0x47B120, 0x47B310) check and their
	log strings "Saving %d ConditionEventCreator(s)" and "Loading ..."
	name; the class strings are "ConditionEventCreator",
	"_p_ConditionEventCreator" and "ConditionEventCreatorPtr". The
	constructor (0x4420D0), the destructor (0x4421F0, which deletes the
	event at 0x18), ConditionChanged (0x4421A0: Duplicate, then
	TGEventManager::AddEvent) and the other inline virtuals are the copies
	of the creators' unit (0x4420C0 is the load creator). The stream
	functions save the event by ID.

===============================================================================
*/

#ifndef __CONDITIONEVENTCREATOR_H__
#define __CONDITIONEVENTCREATOR_H__

#include "TGObject.h"
#include "TGConditionHandler.h"
#include "TGEvent.h"
#include "TGEventManager.h"

class TGStream;

// GetObjType's value for a ConditionEventCreator.
#define CONDITIONEVENTCREATOR_TYPE	0x8038

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004421c0 ??_GConditionEventCreator@@UAEPAXI@Z @game/UtopiaModule
// The adjustor thunks of the TGConditionHandler base's vtable (0x88B320).
// FUNCTION: STBC 0x00447430 ?WriteToStream@ConditionEventCreator@@W7AE_NPAVTGStream@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x00447440 ?ReadFromStream@ConditionEventCreator@@W7AE_NPAVTGStream@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x00447450 ?FixUpPointers@ConditionEventCreator@@W7AE_NXZ @game/UtopiaModule
// FUNCTION: STBC 0x00447460 ?PostFixUp@ConditionEventCreator@@W7AEXXZ @game/UtopiaModule
// FUNCTION: STBC 0x00447470 ??_EConditionEventCreator@@W7AEPAXI@Z @game/UtopiaModule
class ConditionEventCreator : public TGObject, public TGConditionHandler {
public:
	// FUNCTION: STBC 0x004420d0 ??0ConditionEventCreator@@QAE@J@Z @game/UtopiaModule
					ConditionEventCreator( long id = 0 ) : TGObject( id ) { event = NULL; }
	// FUNCTION: STBC 0x004421f0 ??1ConditionEventCreator@@UAE@XZ @game/UtopiaModule
	virtual			~ConditionEventCreator( void ) { delete event; }

	// TGObject
	// FUNCTION: STBC 0x00442140 ?GetObjType@ConditionEventCreator@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return CONDITIONEVENTCREATOR_TYPE; }
	// FUNCTION: STBC 0x00442150 ?IsTypeOf@ConditionEventCreator@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) { if ( type == CONDITIONEVENTCREATOR_TYPE ) { return true; } return TGObject::IsTypeOf( type ); }
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00442170 ?GetClassName@ConditionEventCreator@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ConditionEventCreator"; }
	// FUNCTION: STBC 0x00442180 ?GetSwigTypeName@ConditionEventCreator@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ConditionEventCreator"; }
	// FUNCTION: STBC 0x00442190 ?GetPythonClassName@ConditionEventCreator@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ConditionEventCreatorPtr"; }
	void	SetEvent( TGEvent * event_ ) { event = event_; }

	// TGConditionHandler
	// FUNCTION: STBC 0x004421a0 ?ConditionChanged@ConditionEventCreator@@UAEXPAVTGCondition@@@Z @game/UtopiaModule
	virtual void	ConditionChanged( TGCondition *condition ) { g_kEventManager.AddEvent( event->Duplicate() ); }

	// Saved games: every live ConditionEventCreator, after the count;
	// false when one of them failed (invented names).
	static bool		SaveConditionEventCreators( TGStream *stream );
	static bool		LoadConditionEventCreators( TGStream *stream );

protected:
	TGEvent *		event;			// 0x18 owned; holds the saved ID until PostFixUp (invented name)
};

#endif /* !__CONDITIONEVENTCREATOR_H__ */
