/*
===============================================================================

	TGEventManager.h

	The event manager: g_kEventManager (0x97F838) is the one instance.
	Besides the members the SWIG wrappers TGEventManager_* call, what they
	and game code reach inline is declared: the broadcast handlers at 0x2C
	and the flag that is set while the manager exists.

	0x3C bytes, no vtable. Evidence: the constructor (0x6DA040) builds two
	event queues (0x6DE1B0), a TGEventHandlerTable at 0x18 and the
	broadcast handler table at 0x2C (0x6DAE80), then stores itself in
	instance (0x998744) and sets isAlive (0x998748); the destructor
	(0x6DA0B0) clears the handler registry 0x998638, then the flag. The
	saved game's load (UtopiaModule::ProcessLoadFromFile) calls the stream
	functions 0x6DA190-0x6DA280 in turn. The SWIG wrapper
	TGEventManager_RemoveAllBroadcastHandlersForObject inlines the test of
	the flag and the call to the broadcast table at 0x2C.

===============================================================================
*/

#ifndef __TGEVENTMANAGER_H__
#define __TGEVENTMANAGER_H__

#include "TGEventHandlerObject.h"
#include "TGEventHandlerTable.h"
#include "TGBroadcastHandlerTable.h"

class TGEvent;

/*
===============================================================================

	TGEventQueue

	A 0xC-byte member of the event manager (the constructor 0x6DE1B0 builds
	two). AddEvent (0x6DA2A0) hands its event to the one at offset 0,
	ProcessEvent passes the logged events to the one at 0x0C. The
	constructor, the destructor and Unknown6DE300 are out of line in the
	queue's unit (0x6DE1B0, 0x6DE1C0, 0x6DE300). Invented name.

===============================================================================
*/

class TGEventQueue {		// invented name
public:
					TGEventQueue( void );
					~TGEventQueue( void );
	// Appends the event, which the queue holds a reference to (invented name).
	bool			Unknown6DE330( TGEvent *event );
	// Drops the first event and its reference (invented name).
	void			Unknown6DE370( void );
	// Takes the event with the ID out of the queue and drops its reference
	// (invented name).
	bool			Unknown6DE3A0( long eventID );
	// Takes the event out of the queue (invented name).
	bool			RemoveEvent( TGEvent *event );
	// Takes out the events sent to the destination, of the type when it is not 0 (invented name).
	bool			RemoveEvents( TGEventHandlerObject *destination, int eventType );
	// Clears the source of every event with this source (invented name).
	bool			ClearSource( TGObject *source );

	// Drops every event (invented name).
	void			Unknown6DE310( void );
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );
	// Empties the queue without dropping the references (invented name).
	void			Unknown6DE300( void );

	TGEvent *		GetHead( void ) { return head; }
	int				GetCount( void ) { return count; }

private:
	TGEvent *		head;				// 0x00 the events are chained through their 0x24 (invented name)
	TGEvent *		tail;				// 0x04 (invented name)
	int				count;				// 0x08 (invented name)
};

class TGEventManager {
public:
					TGEventManager( void );
					~TGEventManager( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );		// invented name, as TGObject's
	void			PostFixUp( void );			// invented name, as TGObject's
	// Empties the queues and the broadcast handlers without dropping or deleting
	// what they hold: a saved game's are about to be read (invented name).
	void			ForgetAll( void );

	// Drop the events of the queues (invented names).
	void			ClearEvents( void );
	void			ClearLoggedEvents( void );
	void			RemoveAllBroadcastHandlers( void );		// invented name
	// Removes the handlers of every class's table (invented name).
	void			RemoveAllClassHandlers( void );

	bool			AddEvent( TGEvent * );
	bool			ProcessEvent( TGEvent *event );		// invented name: sends the event to its handlers at once
	bool			Unknown6DA2C0( void );				// processes the queued events (OptionsWindow::SetVisible calls it)
	bool			RemoveQueuedEvent( long eventID ) { return events.Unknown6DE3A0( eventID ); }	// invented name
	// Takes the queued events sent to the destination out, of the type when it is not 0 (invented name).
	bool			RemoveQueuedEvents( TGEventHandlerObject *destination, int eventType ) { return events.RemoveEvents( destination, eventType ); }
	int				GetNumQueuedEvents( void ) { return events.GetCount(); }	// invented name

private:
	bool			Unknown6DA2B0( TGEvent *event );

public:

	// Handlers called for every event of a type sent to the target (any
	// object by default), on behalf of destination.
	bool			AddBroadcastPythonFuncHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target = ANY_TARGET );
	bool			AddBroadcastPythonMethodHandler( int eventType, TGEventHandlerObject *destination, char *method, TGEventHandlerObject *target = ANY_TARGET );
	// The same with a C++ handler method registered under its name (invented name).
	bool			AddBroadcastMethodHandler( int eventType, TGEventHandlerObject *destination, char *method, TGEventHandlerObject *target = ANY_TARGET );
	// The same with a C++ handler function registered under its name (invented name).
	bool			AddBroadcastFuncHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target = ANY_TARGET );
	bool			RemoveBroadcastHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target = ANY_TARGET );
	bool			RemoveAllBroadcastHandlersForObject( TGEventHandlerObject *object );

	// Register a C++ handler under its name (TGHandlerRegistry).
	void			RegisterHandlerMethod( TGHandlerMethod method, char *name );		// invented name
	void			RegisterHandlerFunction( TGHandlerFunction function, char *name );	// invented name

	static TGEventManager *	instance;		// the manager, set by its constructor (invented name)
	static bool		isAlive;				// set by the constructor, cleared by the destructor

private:
	TGEventQueue	events;					// 0x00 the one AddEvent hands its event to (invented name)
	TGEventQueue	loggedEvents;			// 0x0C the logged events ProcessEvent passes on (invented name)
	TGEventHandlerTable	unknown18;			// 0x18
	TGBroadcastHandlerTable	broadcastHandlers;	// 0x2C

};

extern TGEventManager	g_kEventManager;

/*
================
TGEventManager::AddBroadcastPythonFuncHandler
================
*/
inline bool TGEventManager::AddBroadcastPythonFuncHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target ) {
	return broadcastHandlers.AddHandler( eventType, destination, function, false, false, target );
}

/*
================
TGEventManager::AddBroadcastPythonMethodHandler
================
*/
inline bool TGEventManager::AddBroadcastPythonMethodHandler( int eventType, TGEventHandlerObject *destination, char *method, TGEventHandlerObject *target ) {
	return broadcastHandlers.AddHandler( eventType, destination, method, false, true, target );
}

/*
================
TGEventManager::AddBroadcastMethodHandler
================
*/
inline bool TGEventManager::AddBroadcastMethodHandler( int eventType, TGEventHandlerObject *destination, char *method, TGEventHandlerObject *target ) {
	return broadcastHandlers.AddHandler( eventType, destination, method, true, true, target );
}

/*
================
TGEventManager::AddBroadcastFuncHandler
================
*/
inline bool TGEventManager::AddBroadcastFuncHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target ) {
	return broadcastHandlers.AddHandler( eventType, destination, function, true, false, target );
}

/*
================
TGEventManager::RemoveBroadcastHandler
================
*/
inline bool TGEventManager::RemoveBroadcastHandler( int eventType, TGEventHandlerObject *destination, char *function, TGEventHandlerObject *target ) {
	if ( isAlive == true ) {
		return broadcastHandlers.RemoveHandler( eventType, destination, function, target );
	}
	return false;
}

/*
================
TGEventManager::RemoveAllBroadcastHandlersForObject
================
*/
inline bool TGEventManager::RemoveAllBroadcastHandlersForObject( TGEventHandlerObject *object ) {
	if ( isAlive == true ) {
		return broadcastHandlers.RemoveAllHandlersForObject( object );
	}
	return false;
}

#endif /* !__TGEVENTMANAGER_H__ */
