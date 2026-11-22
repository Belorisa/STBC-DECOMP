/*
===============================================================================

	WaypointEvent.h

	An event that carries a placement (a waypoint); scripts make and read
	them through the SWIG wrappers.

	0x2C bytes, allocated from the pools.

	Evidence: the class strings "WaypointEvent", "_p_WaypointEvent" and
	"WaypointEventPtr"; GetObjType (0x441EE0) returns 0x8120, the type the
	game's class registration files the creator 0x441E90 under. The
	vtable 0x88B26C overrides TGEvent's type and name slots and Copy
	(0x441F50), which copies the placement without a type test. The
	default constructor (the copy 0x606120 the SWIG wrapper new_WaypointEvent
	calls) clears the placement; the load's constructor (0x441EC0) leaves
	it to ReadFromStream, which is TGEvent's. The SWIG wrappers SetPlacement
	and GetPlacement write and read 0x28 (a PlacementObject). The
	destructor (0x441FA0) is the implicit one. Every function is inline:
	the out-of-line copies are those of the class registration's unit and
	of the SWIG unit.

===============================================================================
*/

#ifndef __WAYPOINTEVENT_H__
#define __WAYPOINTEVENT_H__

#include "TGEvent.h"

class PlacementObject;

// GetObjType's value for a WaypointEvent.
#define WAYPOINTEVENT_TYPE		0x8120

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00441f70 ??_GWaypointEvent@@UAEPAXI@Z
// FUNCTION: STBC 0x00441fa0 ??1WaypointEvent@@UAE@XZ
class WaypointEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x00606120 ??0WaypointEvent@@QAE@XZ
					WaypointEvent( void ) { placement = NULL; }
	// FUNCTION: STBC 0x00441ec0 ??0WaypointEvent@@QAE@J@Z
					WaypointEvent( long id ) : TGEvent( id ) {}

	static WaypointEvent *	Create( void ) { return new WaypointEvent; }

	// TGObject
	// FUNCTION: STBC 0x00441ee0 ?GetObjType@WaypointEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return WAYPOINTEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00441f20 ?GetClassName@WaypointEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WaypointEvent"; }
	// FUNCTION: STBC 0x00441f30 ?GetSwigTypeName@WaypointEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WaypointEvent"; }
	// FUNCTION: STBC 0x00441f40 ?GetPythonClassName@WaypointEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WaypointEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	void			SetPlacement( PlacementObject *placement_ ) { placement = placement_; }
	PlacementObject *	GetPlacement( void ) { return placement; }

protected:
	PlacementObject *	placement;			// 0x28 not saved
};

/*
================
WaypointEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441ef0 ?IsTypeOf@WaypointEvent@@UAE_NH@Z
inline bool WaypointEvent::IsTypeOf( int type ) {
	if ( type == WAYPOINTEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
WaypointEvent::Copy

Copies the placement; the event is taken to be a WaypointEvent.
================
*/
// FUNCTION: STBC 0x00441f50 ?Copy@WaypointEvent@@UAE_NPAVTGEvent@@@Z
inline bool WaypointEvent::Copy( TGEvent *event ) {
	placement = ( (WaypointEvent *)event )->placement;
	return TGEvent::Copy( event );
}

#endif /* !__WAYPOINTEVENT_H__ */
