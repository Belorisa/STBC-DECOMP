/*
===============================================================================

	TGBroadcastHandlerTable.h

	The event manager's broadcast handlers: handlers that take every event
	of a type, whatever its destination, optionally only those sent to one
	target (TGEventManager::AddBroadcastPythonFuncHandler and its siblings).

	Three classes, all with invented names: no string names them.

	TGBroadcastHandler (0xC bytes): one handler and the object it is called
	on behalf of. Its constructor (0x6E2F60) clears the three members; its
	destructor (0x6E2F70) clears the destination and deletes the handler;
	0x6E2F90 gives the ID of the handler's target.

	TGBroadcastHandlerList (0x34 bytes): the handlers of one event type.
	Evidence: the constructor (0x6E1870) builds two NiTArrays in place with
	the vtable 0x896104 (whose one slot, 0x6E2540, is NiTArray's scalar
	deleting destructor): 2 elements growing by 4 at 0x00, 1 growing by 1
	at 0x18, then clears the call depth at 0x30. The first array is kept
	sorted by target ID (binary searches 0x6E2330 and 0x6E2380); handlers
	added while the list is calling its handlers wait in the second, and
	handlers removed meanwhile are only marked (0x6E23F0) until the outermost
	call returns (0x6E21D0).

	TGBroadcastHandlerTable (0x10 bytes): the lists by event type. It is a
	NiTMap<int, TGBroadcastHandlerList *> member and nothing else: the
	constructor (0x6DAE80) is NiTMap's inline one with the vtable 0x89609C,
	whose slots (0x6DB850, 0x6DB7F0-0x6DB840) are NiTMap's own; the
	destructor (0x6DAED0) deletes the lists, then runs NiTMap's inline
	destructor under an unwind state for the member.

===============================================================================
*/

#ifndef __TGBROADCASTHANDLERTABLE_H__
#define __TGBROADCASTHANDLERTABLE_H__

#include "NiTArray.h"
#include "NiTMap.h"
#include "TGHandler.h"

class TGEvent;
class TGStream;
class TGEventHandlerObject;

/*
===============================================================================

	TGBroadcastHandler

===============================================================================
*/

class TGBroadcastHandler {		// invented name
public:
					TGBroadcastHandler( void );
					~TGBroadcastHandler( void );

	// The ID of the handler's target; 0 when it takes any target (invented name).
	long			GetTargetID( void );

	TGEventHandlerObject *	destination;	// 0x00 the object the handler is called on behalf of (invented name)
	TGHandler *		handler;				// 0x04 owned (invented name)
	bool			removed;				// 0x08 removed while its list was calling; deleted when the call returns (invented name)
};

/*
===============================================================================

	TGBroadcastHandlerList

===============================================================================
*/

class TGBroadcastHandlerList {		// invented name
public:
					TGBroadcastHandlerList( void );
					~TGBroadcastHandlerList( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );

	// Empties the list without deleting its handlers (invented name).
	void			ForgetHandlers( void );
	// Adds the handler called on behalf of destination (invented name).
	bool			AddHandler( TGEventHandlerObject *destination, TGHandler *handler );
	// Removes the handler with the name, sent to target, of destination (invented name).
	bool			RemoveHandler( TGEventHandlerObject *destination, char *name, TGEventHandlerObject *target );
	// Removes the handlers called on behalf of the object or taking only its events (invented name).
	bool			RemoveAllHandlersForObject( TGEventHandlerObject *object );
	// Calls the handlers that take any target, then those of the event's destination (invented name).
	void			CallHandlers( TGEvent *event );
	// Removes and deletes every handler (invented name).
	void			RemoveAllHandlers( void );

private:
	bool			Insert( TGBroadcastHandler *entry );								// invented name
	bool			IsHandler( TGBroadcastHandler *entry, TGEventHandlerObject *destination, TGHandlerFunction function, char *name, TGEventHandlerObject *target );	// invented name
	unsigned int	FindInsertIndex( long targetID );									// invented name
	unsigned int	FindFirst( long targetID );											// invented name
	bool			RemoveAt( unsigned int index );										// invented name

	NiTArray<TGBroadcastHandler *>	handlers;	// 0x00 sorted by target ID (invented name)
	NiTArray<TGBroadcastHandler *>	pending;	// 0x18 added during a call (invented name)
	int				callDepth;					// 0x30 CallHandlers running (invented name)
};

/*
===============================================================================

	TGBroadcastHandlerTable

===============================================================================
*/

class TGBroadcastHandlerTable {		// invented name
public:
					TGBroadcastHandlerTable( void );
					~TGBroadcastHandlerTable( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );
	// Empties the table without deleting the handlers (invented name).
	void			ForgetHandlers( void );

	bool			AddHandler( int eventType, TGEventHandlerObject *destination, char *name, bool registered, bool method, TGEventHandlerObject *target );	// invented name
	bool			RemoveHandler( int eventType, TGEventHandlerObject *destination, char *name, TGEventHandlerObject *target );	// invented name
	bool			RemoveAllHandlersForObject( TGEventHandlerObject *object );	// invented name
	// Calls the handlers of the event's type (invented name).
	bool			CallHandlers( TGEvent *event );
	// Deletes every list (invented name).
	void			RemoveAllHandlers( void );

private:
	NiTMap<int, TGBroadcastHandlerList *>	lists;	// 0x00 by event type (invented name)
};

#endif /* !__TGBROADCASTHANDLERTABLE_H__ */
