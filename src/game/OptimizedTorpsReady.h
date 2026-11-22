/*
===============================================================================

	OptimizedTorpsReady.h

	The C++ version of the condition script ConditionTorpsReady
	(Conditions.ConditionTorpsReady): true while a torpedo tube of the
	ship that faces the given direction has a torpedo ready and neither
	the tube nor the torpedo system is disabled. Its event handler, an
	OTREventHandler, forwards the system and tube events.

	0x24 bytes from the object pool. Evidence: the static initialiser
	0x48B5C0 files "Conditions.ConditionTorpsReady" and
	"ConditionTorpsReady" with the create function 0x48BDC0 among the
	C++ conditions ConditionScript_Create uses; the constructors (0x48BA20,
	0x48BC90) store the vtable 0x88C044 (TGCondition's 15 slots; 0, 1, 2,
	4, 5 and 8 to 11 are overridden); GetObjType returns the App constant
	CT_OPT_TORPS_READY and the class strings are "OptimizedTorpsReady",
	"_p_OptimizedTorpsReady" and "OptimizedTorpsReadyPtr". OTREventHandler
	(vtable 0x88C080, 21 slots, 0x18 bytes) has the type
	CT_OPT_TR_EVENT_HANDLER and its handler methods are registered under
	"OTREventHandler::SystemDisabled" and the rest. Its module (0x982798,
	vtable 0x88C024, priority 0x20000000) files both creators and the
	handler table at startup; the module's name is invented.

===============================================================================
*/

#ifndef __OPTIMIZEDTORPSREADY_H__
#define __OPTIMIZEDTORPSREADY_H__

#include "AppConstants.h"
#include "Python.h"
#include "TGCondition.h"
#include "TGEventHandlerObject.h"
#include "TGModule.h"

class NiPoint3;
class OptimizedTorpsReady;
class ShipClass;
class TGEvent;

/*
================
OTREventHandler

Hands the events of an OptimizedTorpsReady to it.
================
*/
// FUNCTION: STBC 0x0048bc10 ??_GOTREventHandler@@UAEPAXI@Z @game/OptimizedTorpsReady
// FUNCTION: STBC 0x0048bc40 ??1OTREventHandler@@UAE@XZ @game/OptimizedTorpsReady
class OTREventHandler : public TGEventHandlerObject {
public:
					// FUNCTION: STBC 0x0048bb20 ??0OTREventHandler@@QAE@J@Z @game/OptimizedTorpsReady
					OTREventHandler( long id = 0 ) : TGEventHandlerObject( id ) {}
					OTREventHandler( OptimizedTorpsReady *condition_ ) { condition = condition_; }

	// TGObject
	// FUNCTION: STBC 0x0048bb40 ?GetObjType@OTREventHandler@@UAEHXZ @game/OptimizedTorpsReady
	virtual int		GetObjType( void ) { return CT_OPT_TR_EVENT_HANDLER; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0048bb90 ?GetClassName@OTREventHandler@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetClassName( void ) { return "OTREventHandler"; }
	// FUNCTION: STBC 0x0048bba0 ?GetSwigTypeName@OTREventHandler@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetSwigTypeName( void ) { return "_p_OTREventHandler"; }
	// FUNCTION: STBC 0x0048bbb0 ?GetPythonClassName@OTREventHandler@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetPythonClassName( void ) { return "OTREventHandlerPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0048bbc0 ?GetClassHandlers@OTREventHandler@@UAEPAVTGEventHandlerTable@@XZ @game/OptimizedTorpsReady
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The handler methods.
	void			SystemDisabled( TGEvent *event );
	void			SystemEnabled( TGEvent *event );
	void			TorpReloaded( TGEvent *event );
	void			TorpFired( TGEvent *event );

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// The condition it works for; its ID between a load and the fix-up.
	OptimizedTorpsReady *	condition;		// 0x14

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
OTREventHandler::IsTypeOf
================
*/
// FUNCTION: STBC 0x0048bb50 ?IsTypeOf@OTREventHandler@@UAE_NH@Z @game/OptimizedTorpsReady
inline bool OTREventHandler::IsTypeOf( int type ) {
	if ( type == CT_OPT_TR_EVENT_HANDLER ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
OTREventHandlerModule

Files OptimizedTorpsReady and OTREventHandler at startup. The name is
invented.
================
*/
// FUNCTION: STBC 0x0048bc80 ??1OTREventHandlerModule@@QAE@XZ @game/OptimizedTorpsReady
class OTREventHandlerModule : public TGModule {
public:
					// FUNCTION: STBC 0x0048b820 ??0OTREventHandlerModule@@QAE@XZ @game/OptimizedTorpsReady
					OTREventHandlerModule( void ) : TGModule( 0x20000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot05( void );

	static TGObject *	CreateOptimizedTorpsReady( long id );
	static TGObject *	CreateOTREventHandler( long id );
};

class OptimizedTorpsReady : public TGCondition {
public:
					// FUNCTION: STBC 0x0048ba20 ??0OptimizedTorpsReady@@QAE@J@Z @game/OptimizedTorpsReady
					OptimizedTorpsReady( long id = 0 ) : TGCondition( id ) {}
	// Watches the tubes of the named ship that face the direction.
					OptimizedTorpsReady( char *shipName, const NiPoint3 &direction );
	virtual			~OptimizedTorpsReady( void );

	// TGObject
	// FUNCTION: STBC 0x0048ba40 ?GetObjType@OptimizedTorpsReady@@UAEHXZ @game/OptimizedTorpsReady
	virtual int		GetObjType( void ) { return CT_OPT_TORPS_READY; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual PyObject *	VirtualSlot08( void );
	// FUNCTION: STBC 0x0048ba80 ?GetClassName@OptimizedTorpsReady@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetClassName( void ) { return "OptimizedTorpsReady"; }
	// FUNCTION: STBC 0x0048ba90 ?GetSwigTypeName@OptimizedTorpsReady@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetSwigTypeName( void ) { return "_p_OptimizedTorpsReady"; }
	// FUNCTION: STBC 0x0048baa0 ?GetPythonClassName@OptimizedTorpsReady@@UAEPADXZ @game/OptimizedTorpsReady
	virtual char *	GetPythonClassName( void ) { return "OptimizedTorpsReadyPtr"; }

	// The event handler's calls (invented names).
	void			SystemDisabled( TGEvent *event );
	void			SystemEnabled( TGEvent *event );
	void			TorpReloaded( TGEvent *event );
	void			TorpFired( TGEvent *event );

	// ConditionScript_Create's arguments: the module and class names, the
	// ship's name and, optionally, the direction (a TGPoint3; the zero
	// vector by default).
	static TGObject *	Create( PyObject *args );

protected:
	// Listens to the torpedo system and to the facing tubes, and keeps the
	// tubes' IDs (invented name).
	void			WatchTubes( ShipClass *ship, const NiPoint3 &direction );
	// Sets the status from the watched tubes (invented name).
	void			UpdateStatus( void );

	OTREventHandler *	eventHandler;		// 0x18 owned
	long *			tubeIDs;				// 0x1C new[], numTubes of them (invented name)
	int				numTubes;				// 0x20 (invented name)
};

/*
================
OptimizedTorpsReady::IsTypeOf
================
*/
// FUNCTION: STBC 0x0048ba50 ?IsTypeOf@OptimizedTorpsReady@@UAE_NH@Z @game/OptimizedTorpsReady
inline bool OptimizedTorpsReady::IsTypeOf( int type ) {
	if ( type == CT_OPT_TORPS_READY ) {
		return true;
	}
	return TGCondition::IsTypeOf( type );
}

#endif /* !__OPTIMIZEDTORPSREADY_H__ */
