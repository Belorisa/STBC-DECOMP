/*
===============================================================================

	ProximityEvent.h

	The event a proximity check sends when an object it watches crosses
	its trigger: the check and the object.

	0x30 bytes, allocated from the pools.

	Evidence: the class strings "ProximityEvent", "_p_ProximityEvent" and
	"ProximityEventPtr"; GetObjType (0x43F530) returns 0x8126, the type the
	game's class registration files the creator 0x43F4E0 under. The
	vtable 0x88A100 overrides TGEvent's type, name, stream and fix-up
	slots and Copy. The proximity check (0x5A63A0) makes the events with
	the default constructor (0x5A6430), which clears both pointers; the
	load's constructor (0x43F510) leaves them to ReadFromStream. The SWIG
	wrappers Set/GetProximityCheck and Set/GetObject write and read 0x28
	and 0x2C. The stream saves both by ID; FixUpPointers checks that the
	IDs resolve and PostFixUp turns them back into pointers. Every
	function is inline: the out-of-line copies are those of the class
	registration's unit and of the proximity check's.

===============================================================================
*/

#ifndef __PROXIMITYEVENT_H__
#define __PROXIMITYEVENT_H__

#include "TGEvent.h"
#include "TGStream.h"
#include "ProximityCheck.h"

// GetObjType's value for a ProximityEvent.
#define PROXIMITYEVENT_TYPE		0x8126

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043f6f0 ??_GProximityEvent@@UAEPAXI@Z
class ProximityEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x005a6430 ??0ProximityEvent@@QAE@XZ
					ProximityEvent( void ) : proximityCheck( NULL ), object( NULL ) {}
	// FUNCTION: STBC 0x0043f510 ??0ProximityEvent@@QAE@J@Z
					ProximityEvent( long id ) : TGEvent( id ) {}
	// FUNCTION: STBC 0x0043f720 ??1ProximityEvent@@UAE@XZ
	virtual			~ProximityEvent( void ) {}

	// TGObject
	// FUNCTION: STBC 0x0043f530 ?GetObjType@ProximityEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return PROXIMITYEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0043f570 ?GetClassName@ProximityEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ProximityEvent"; }
	// FUNCTION: STBC 0x0043f580 ?GetSwigTypeName@ProximityEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ProximityEvent"; }
	// FUNCTION: STBC 0x0043f590 ?GetPythonClassName@ProximityEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ProximityEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	void			SetProximityCheck( ProximityCheck *check ) { proximityCheck = check; }
	ProximityCheck *	GetProximityCheck( void ) { return proximityCheck; }
	void			SetObject( ObjectClass *object_ ) { object = object_; }
	ObjectClass *	GetObject( void ) { return object; }

protected:
	ProximityCheck *	proximityCheck;		// 0x28 IDs between ReadFromStream and PostFixUp
	ObjectClass *	object;					// 0x2C the object that crossed the trigger
};

/*
================
ProximityEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043f540 ?IsTypeOf@ProximityEvent@@UAE_NH@Z
inline bool ProximityEvent::IsTypeOf( int type ) {
	if ( type == PROXIMITYEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
ProximityEvent::Copy

The event is taken to be a ProximityEvent.
================
*/
// FUNCTION: STBC 0x0043f5a0 ?Copy@ProximityEvent@@UAE_NPAVTGEvent@@@Z
inline bool ProximityEvent::Copy( TGEvent *event ) {
	proximityCheck = ( (ProximityEvent *)event )->proximityCheck;
	object = ( (ProximityEvent *)event )->object;
	return TGEvent::Copy( event );
}

/*
================
ProximityEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x0043f5c0 ?WriteToStream@ProximityEvent@@UAE_NPAVTGStream@@@Z
inline bool ProximityEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( proximityCheck != NULL ? proximityCheck->GetObjID() : 0 );
	stream->WriteID( object != NULL ? object->GetObjID() : 0 );
	return true;
}

/*
================
ProximityEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x0043f620 ?ReadFromStream@ProximityEvent@@UAE_NPAVTGStream@@@Z
inline bool ProximityEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	proximityCheck = (ProximityCheck *)stream->ReadID();
	object = (ObjectClass *)stream->ReadID();
	return true;
}

/*
================
ProximityEvent::FixUpPointers

Fails while either saved ID has no object yet.
================
*/
// FUNCTION: STBC 0x0043f660 ?FixUpPointers@ProximityEvent@@UAE_NXZ
inline bool ProximityEvent::FixUpPointers( void ) {
	if ( !TGEvent::FixUpPointers() ) {
		return false;
	}
	if ( proximityCheck != NULL && GetTGObjectPtr( (long)proximityCheck ) == NULL ) {
		return false;
	}
	if ( object != NULL && GetTGObjectPtr( (long)object ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ProximityEvent::PostFixUp
================
*/
// FUNCTION: STBC 0x0043f6b0 ?PostFixUp@ProximityEvent@@UAEXXZ
inline void ProximityEvent::PostFixUp( void ) {
	TGEvent::PostFixUp();
	proximityCheck = (ProximityCheck *)GetSavedObject( (long)proximityCheck );
	// BUG: the object is looked up by the check's new pointer, not by its
	// own saved ID.
	object = ( object == NULL ) ? NULL : (ObjectClass *)GetTGObjectPtr( (long)proximityCheck );
}

#endif /* !__PROXIMITYEVENT_H__ */
