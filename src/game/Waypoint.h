/*
===============================================================================

	Waypoint.h

	A placement on a path: the waypoints of a path are linked both ways, and
	each keeps the speed to go at and a curve from itself to the next one.

	0x84 bytes. Evidence: the constructor (0x4A4270) runs PlacementObject's,
	sets the speed to 25 and clears the members, and stores the vtable
	0x88A804 (71 slots, PlacementObject's with 18 replaced); Waypoint_Create
	allocates it from the pool of its size. The SWIG inline accessors read
	GetSpeed and SetSpeed at 0x70, GetNext at 0x74 and GetPrev at 0x78. The
	saved scripts the file writers (slots 67 to 69) produce call
	Waypoint_Create, SetSpeed and InsertAfterObj, which links the given
	waypoint after this one. The type functions (slots 1, 2, 9 to 11 and
	19) are inline copies kept beside the class factories (0x440EB0 to
	0x440F50).

	0x80 is a BezierCurve through four points, from this waypoint to the
	next; 0x7C draws the path while the set is shown (WaypointPathDisplay,
	built by 0x58F760, destroyed by 0x58F7C0).
	UpdateCurve (0x4A4910) builds both.

===============================================================================
*/

#ifndef __WAYPOINT_H__
#define __WAYPOINT_H__

#include "PlacementObject.h"
#include "BezierCurve.h"

class TGObject;
class TGEvent;
class TGEventHandlerTable;

// The path display at 0x7C (WaypointPathDisplay.h); the name is invented.
class WaypointPathDisplay;

// GetObjType's value for a Waypoint (the type its Cast checks).
#define WAYPOINT_TYPE		0x80E0

// The event that sets a waypoint's speed (App constant ET_SET_WAYPOINT_SPEED).
#define ET_SET_WAYPOINT_SPEED	0x8000C3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00440f60 ??_GWaypoint@@UAEPAXI@Z
class Waypoint : public PlacementObject {
public:
	// FUNCTION: STBC 0x00440e90 ??0Waypoint@@QAE@J@Z @game/UtopiaModule
					Waypoint( long id ) : PlacementObject( id ) {}	// the loader's: the stream fills the members
					Waypoint( char *name, char *setName, char *modelName );
	virtual			~Waypoint( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// PlacementObject
	virtual bool	SaveObjectSecondPass( FILE *file, char *setName );
	virtual bool	VirtualSlot68( FILE *file, char *setName );
	virtual bool	VirtualSlot69( FILE *file );

	static Waypoint *	Create( char *name, char *setName, char *modelName );
	static Waypoint *	Cast( TGObject *object );

	// links waypoint after this one
	void			InsertAfterObj( Waypoint *waypoint );
	// links waypoint before this one (invented name)
	void			InsertBeforeObj( Waypoint *waypoint );
	// links the named object of the named set before this one (invented
	// name)
	void			InsertBefore( char *setName, char *name );
	// links the object with the ID in the named set before this one
	// (invented name)
	void			InsertBefore( char *setName, long id );

	// the point at t along the curve to the next waypoint; the waypoint's
	// own location when it has no curve (invented name)
	NiPoint3		GetPathPoint( float t );

	float			GetSpeed( void ) { return speed; }
	void			SetSpeed( float speed_ ) { speed = speed_; }
	Waypoint *		GetNext( void ) { return next; }
	Waypoint *		GetPrev( void ) { return prev; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleSetSpeed( TGEvent *event );

protected:
	// sets both links; the curve is rebuilt when the next waypoint changes
	// (invented name)
	void			SetLinks( Waypoint *next_, Waypoint *prev_ );
	// takes the waypoint out of its path (invented name)
	void			Unlink( void );
	// rebuilds the curve to the next waypoint and the path display
	// (invented name)
	void			UpdateCurve( void );

	float			speed;				// 0x70
	Waypoint *		next;				// 0x74
	Waypoint *		prev;				// 0x78
	WaypointPathDisplay *	pathDisplay;	// 0x7C owned
	BezierCurve *	curve;				// 0x80 owned

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
Waypoint::GetObjType
================
*/
// FUNCTION: STBC 0x00440eb0
inline int Waypoint::GetObjType( void ) {
	return WAYPOINT_TYPE;
}

/*
================
Waypoint::IsTypeOf
================
*/
// FUNCTION: STBC 0x00440ec0
inline bool Waypoint::IsTypeOf( int type ) {
	if ( type == WAYPOINT_TYPE ) {
		return true;
	}
	return PlacementObject::IsTypeOf( type );
}

/*
================
Waypoint::GetClassName
================
*/
// FUNCTION: STBC 0x00440f20
inline char *Waypoint::GetClassName( void ) {
	return "Waypoint";
}

/*
================
Waypoint::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00440f30
inline char *Waypoint::GetSwigTypeName( void ) {
	return "_p_Waypoint";
}

/*
================
Waypoint::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00440f40
inline char *Waypoint::GetPythonClassName( void ) {
	return "WaypointPtr";
}

/*
================
Waypoint::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00440f50
inline TGEventHandlerTable *Waypoint::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__WAYPOINT_H__ */
