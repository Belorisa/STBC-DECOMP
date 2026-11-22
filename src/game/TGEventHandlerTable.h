/*
===============================================================================

	TGEventHandlerTable.h

	The handlers of an event handler: for each event type, the list of its
	TGHandlers, newest first. Every handler class has a static table of the
	handlers its class registers (TGEventHandlerObject::GetClassHandlers);
	an object adds a table of its own on its first instance handler. A
	table whose list for a type is empty passes the lookup on to its
	parent, the table of the class.

	0x14 bytes. Evidence: the constructor (0x6D7B30) builds a map of 37
	buckets in place, with the vtable 0x896030 (KeyToHashIndex,
	IsKeysEqual, SetValue and ClearValue of an int-keyed NiTMap), and clears
	the parent at 0x10; the destructor (0x6D7B80) frees the handlers, then
	destroys the map with the vtable store, RemoveAll and the bucket array
	of NiTMap's inline destructor. The map is a member, not a base: the
	table itself has no vtable, so its owners delete it with a direct call
	to the destructor, and the map's out-of-line destructor (0x6D8450) is
	called only by the unwind of the table's destructor.

	The class name is invented: no string names it.

===============================================================================
*/

#ifndef __TGEVENTHANDLERTABLE_H__
#define __TGEVENTHANDLERTABLE_H__

#include "NiTMap.h"
#include "TGHandler.h"

class TGEvent;
class TGStream;
class TGEventHandlerObject;

class TGEventHandlerTable {		// invented name
public:
					TGEventHandlerTable( void );
					~TGEventHandlerTable( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	bool			AddHandler( int eventType, char *name, bool registered, bool method );	// invented name
	bool			RemoveHandler( int eventType, char *name );								// invented name
	TGHandler *		GetHandlers( int eventType );											// invented name
	TGHandler *		FindHandlers( int eventType, TGEventHandlerTable **owner );				// invented name
	void			RemoveAllHandlers( void );												// invented name
	bool			CallNextHandler( TGEvent *event, TGEventHandlerObject *object );

	void			SetParent( TGEventHandlerTable *table ) { parent = table; }			// invented name

private:
	NiTMap<int, TGHandler *>	handlers;		// 0x00 the newest handler of each event type
	TGEventHandlerTable *		parent;			// 0x10 the class's table; NULL for TGEventHandlerObject's
};

#endif /* !__TGEVENTHANDLERTABLE_H__ */
