/*
===============================================================================

	ProximityCheck.h

	An object that watches the objects of its list (by ID or by class) and
	sends a ProximityEvent when one comes within its radius. The methods are
	the ones the SWIG wrappers ProximityCheck_* call.

	Evidence: the vtable 0x889FCC (67 slots, ObjectClass's count) differs from
	ObjectClass's 0x889950 in slots 0-2, 4-7, 9-11, 19, 46, 47 and 57; the
	inline ones (0x43F400-0x43F4B0) are kept from UtopiaModule, whose
	CreateProximityCheck (0x43F2B0) calls the loader's constructor 0x43F2F0.

===============================================================================
*/

#ifndef __PROXIMITYCHECK_H__
#define __PROXIMITYCHECK_H__

#include <list>
#include <utility>
#include "ObjectClass.h"
#include "NiTMap.h"
#include "NiTList.h"

class SetClass;
class TGObject;
class ProximityManager;

// GetObjType's value for a ProximityCheck (0x43F400, beside the class-name
// getters that return "ProximityCheck").
#define PROXIMITYCHECK_TYPE		0x8125

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGEvent;
class TGEventHandlerObject;
// FUNCTION: STBC 0x0043f4b0 ??_GProximityCheck@@UAEPAXI@Z @game/UtopiaModule
class ProximityCheck : public ObjectClass {
public:
	// FUNCTION: STBC 0x0043f2f0 ??0ProximityCheck@@QAE@J@Z @game/UtopiaModule
					ProximityCheck( long id ) : ObjectClass( id ) {}	// the loader's: the stream fills the members
					ProximityCheck( TGEvent *event );
					ProximityCheck( int triggerEventType_, TGEventHandlerObject *triggerDestination_ );
	// The script-side constructor (the SWIG wrapper ProximityCheck_Create expands it).
	static ProximityCheck *	Create( int triggerEventType, TGEventHandlerObject *triggerDestination ) { return new ProximityCheck( triggerEventType, triggerDestination ); }
	virtual			~ProximityCheck( void );

	// TGObject
	// FUNCTION: STBC 0x0043f400 ?GetObjType@ProximityCheck@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return PROXIMITYCHECK_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// 0x5A6460
	virtual bool	ReadFromStream( TGStream *stream );		// 0x5A6600
	virtual bool	FixUpPointers( void );					// 0x5A67B0
	virtual void	PostFixUp( void );						// 0x5A6800
	// FUNCTION: STBC 0x0043f470 ?GetClassName@ProximityCheck@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ProximityCheck"; }
	// FUNCTION: STBC 0x0043f480 ?GetSwigTypeName@ProximityCheck@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ProximityCheck"; }
	// FUNCTION: STBC 0x0043f490 ?GetPythonClassName@ProximityCheck@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ProximityCheckPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043f4a0 ?GetClassHandlers@ProximityCheck@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	AddToSet( SetClass *set );				// 0x5A51B0
	virtual void	RemoveFromSet( SetClass *set );			// 0x5A51E0

	// ObjectClass
	virtual NiBound &	GetWorldBound( void );				// 0x5A5B80

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	static ProximityCheck *	Cast( TGObject * );
	// The object's ID goes on the list with the trigger type.
	void	AddObjectToCheckList( ObjectClass *object, int triggerType );
	void	AddObjectToCheckListByID( long, int );
	void	AddObjectTypeToCheckList( int );
	int	GetTriggerType( ObjectClass * );
	// Whether the object is on the list, by its ID or by its type; the trigger type goes
	// to triggerType when that is not NULL (invented name; 0x5A5DE0).
	bool	FindTriggerType( ObjectClass *object, int *triggerType );
	void	SetTriggerType( ObjectClass *, int );
	void	RemoveObjectFromCheckList( ObjectClass *object );
	void	RemoveObjectFromCheckListByID( long );
	void	RemoveObjectTypeFromCheckList( int );
	void	ClearCheckList( void );
	void	CheckProximity( ObjectClass * );
	void	RemoveAndDelete( void );
	void	SendTriggerEvents( ObjectClass *object );		// invented name
	// Updates at most maxChecks checks a call, round the list (0x5A59C0;
	// invented name).
	static void	UpdateChecks( int maxChecks );
	// Checks the objects no check flagged and clears the flags (0x5A5A30;
	// invented name).
	void	UpdateObjects( int frame );
	void	SetManager( ProximityManager *newManager ) { manager = newManager; }
	static ProximityCheck *	GetObject( SetClass *set, char *name );
	static ProximityCheck *	GetObjectByID( SetClass *set, long id );		// invented name
	static int	GetNumClassObjects( SetClass *set );
	static ProximityCheck **	GetClassObjects( SetClass *set, int *count );		// invented name

	void			SetRadius( float radius_ ) { radius = radius_; }
	void			SetIgnoreObjectSize( bool ignoreObjectSize_ ) { ignoreObjectSize = ignoreObjectSize_; }
	bool			GetIgnoreObjectSize( void ) { return ignoreObjectSize; }

protected:
	// The class is 0xB0 bytes (AsteroidField's event handler 0x419F00 allocates
	// that much).
	ProximityManager *	manager;			// 0x68 the set's manager the check is in; RemoveAndDelete takes it out (invented name)
	int				triggerEventType;		// 0x6C the type of the ProximityEvent sent when an object triggers; 0 sends none (invented name)
	TGEventHandlerObject *	triggerDestination;	// 0x70 the ProximityEvent's destination; the triggering object when NULL (invented name)
	TGEvent *		triggerEvent;			// 0x74 an event sent in a copy of its own when an object triggers (invented name)
	float			radius;					// 0x78 (ProximityCheck_SetRadius)
	bool			ignoreObjectSize;		// 0x7C (ProximityCheck_Get/SetIgnoreObjectSize)
	// The objects to check by ID, each with its trigger type (invented name).
	NiTMap<long, int>	checkList;			// 0x80 the map is vtable 0x88A0EC
	// The classes to check, by GetObjType value, each with a second value that
	// is 0 when added (identity unproven: a std::list of 8-byte elements; invented name).
	std::list< std::pair<int, int> >	typeCheckList;	// 0x90
	// Every object ID that was added, with a flag that starts false (identity
	// unproven; invented name).
	NiTMap<long, bool>	objectFlags;		// 0x9C the map is vtable 0x88A0D8
	int				updateFrame;			// 0xAC the frame of UpdateChecks the check was last updated in (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static ProximityCheck *	CreateWithEvent( TGEvent *event ) { return new ProximityCheck( event ); }	// the SWIG wrapper ProximityCheck_CreateWithEvent expands it
	void	AddObjectListToCheckList( ObjectClass **objects, int count, int triggerType );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
	// Every check there is (invented name; 0x98E4A0, two items a block): the
	// constructors add to it, the destructor takes the check out.
	static NiTList<ProximityCheck *>	checks;
	// The frame UpdateChecks brings the checks up to (invented name; 0x8E5F54).
	static int		currentFrame;
};

/*
================
ProximityCheck::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043f410 ?IsTypeOf@ProximityCheck@@UAE_NH@Z @game/UtopiaModule
inline bool ProximityCheck::IsTypeOf( int type ) {
	if ( type == PROXIMITYCHECK_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

#endif /* !__PROXIMITYCHECK_H__ */
