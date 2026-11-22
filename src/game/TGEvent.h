/*
===============================================================================

	TGEvent.h

	Something that happened: a type, the object it came from (the source),
	the handler it goes to (the destination) and a time stamp. Events are
	reference counted and live in two tables: every event by its ID, and,
	for each object an event names as source or destination, the list of
	those events, so that an object's destructor can clear itself out of
	them (RemoveObjectReferences).

	0x28 bytes, allocated from the pools (TGObject's TGPOOL_ALLOCATION).

	Evidence: the constructor (0x6D5C00) calls TGObject's and stores the
	vtable 0x895FF4 (15 slots); GetObjType returns 0x101, the type
	TGEvent::Cast checks. The member names are those of the SWIG accessors
	TGEvent_GetEventType, GetSource, GetDestination, GetTimestamp,
	IsLogged, IsPrivate, IsNotSaved and IncRefCount, whose inline bodies
	read these offsets; slot 12 is TGEvent_Copy. The global
	INVALID_DESTINATION is the SWIG variable of that name.

===============================================================================
*/

#ifndef __TGEVENT_H__
#define __TGEVENT_H__

#include "TGObject.h"
#include "NiTList.h"

class TGEventHandlerObject;
class TGEventHandlerTable;
class TGHandler;

// GetObjType's value for a TGEvent.
#define TGEVENT_TYPE			0x101

// The flags word: what the SWIG accessors IsLogged, IsPrivate and
// IsNotSaved test.
#define EVENT_LOGGED			0x0001
#define EVENT_PRIVATE			0x0002
#define EVENT_NOT_SAVED			0x0004
// TGBoolEvent's value.
#define EVENT_BOOL_VALUE		0x0100

// Buckets in the table of events by object.
#define OBJECT_EVENT_TABLE_SIZE	251

// Items per block of an object's event list.
#define OBJECT_EVENT_LIST_BLOCK	2

// The stream value of a destination of INVALID_DESTINATION.
#define INVALID_DESTINATION_ID	-1

// A destination that is no handler: what an event's destination becomes
// when its handler is destroyed. Never registered in the object table.
extern TGEventHandlerObject *	INVALID_DESTINATION;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006d5d40 ??_GTGEvent@@UAEPAXI@Z @game/TGEvent
class TGEvent : public TGObject {
public:
					TGEvent( long id = 0 );
	virtual			~TGEvent( void );

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

	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );	// invented name
	virtual bool	ReadFromNetStream( TGStream *stream );	// invented name

	static TGEvent *	Create( void ) { return new TGEvent; }
	static TGEvent *	Cast( TGObject *object );
	TGEvent *		Duplicate( void );

	int				GetEventType( void ) { return eventType; }
	void			SetEventType( int type ) { eventType = type; }
	TGObject *		GetSource( void ) { return source; }
	void			SetSource( TGObject *source );
	TGEventHandlerObject *	GetDestination( void ) { return destination; }
	void			SetDestination( TGEventHandlerObject *destination );
	float			GetTimestamp( void ) { return timestamp; }
	void			SetTimestamp( float time ) { timestamp = time; }

	bool			IsLogged( void ) { return flags & EVENT_LOGGED; }
	void			SetLogged( bool logged );
	bool			IsPrivate( void ) { return ( flags >> 1 ) & 1; }
	void			SetPrivate( bool isPrivate );
	bool			IsNotSaved( void ) { return ( flags >> 2 ) & 1; }
	void			SetNotSaved( bool notSaved );

	__int16			GetRefCount( void ) { return refCount; }	// __int16: SWIG wraps the result as _p___int16
	void			IncRefCount( void ) { refCount++; }
	void			DecRefCount( void );
	// The link of the event manager's queue (invented name).
	void			SetUnknown24( TGEvent *event ) { unknown24 = event; }

	static TGEvent *	CreateFromNetStream( TGStream *stream );	// invented name
	static TGEvent *	GetTGEventPtr( long id );					// invented name
	// A saved event ID read back into the event, NULL for 0 (invented name; TGObject::GetSavedObject's twin).
	static TGEvent *	GetSavedEvent( long id ) { if ( id == 0 ) return NULL; return GetTGEventPtr( id ); }

	// Clears the source and destination of every event that names the object.
	static void		RemoveObjectReferences( TGObject *object );	// invented name

	static bool		SaveEvents( TGStream *stream );				// invented name
	static bool		LoadEvents( TGStream *stream );				// invented name
	static void		ReleaseEventTable( void );					// invented name
	static void		RemoveAllEvents( void );					// invented name
	static void		DeleteAllEvents( void );					// invented name

protected:
	// The handlers walk an event through its handler lists.
	friend class	TGEventHandlerObject;
	friend class	TGEventHandlerTable;
	friend class	TGEventQueue;
	friend class	TGBroadcastHandlerList;

	static bool		AddEvent( TGEvent *event );								// invented name
	static bool		RemoveEvent( TGEvent *event );							// invented name
	static void		AddObjectReference( TGObject *object, TGEvent *event );	// invented name
	static void		RemoveObjectReference( TGObject *object, TGEvent *event );	// invented name

	TGObject *		source;					// 0x08 an ID between ReadFromStream and PostFixUp
	TGEventHandlerObject *	destination;	// 0x0C likewise
	int				eventType;				// 0x10
	float			timestamp;				// 0x14 -1 until the event is sent
	unsigned short	flags;					// 0x18 EVENT_*
	__int16			refCount;				// 0x1A
	TGEventHandlerTable *	currentTable;	// 0x1C while handled: the table of currentHandler; invented name
	TGHandler *		currentHandler;			// 0x20 while handled: the handler running; invented name
	TGEvent *		unknown24;				// 0x24 an ID between ReadFromStream and PostFixUp; the next event while it waits in the event manager's queue

	// Every live event, by ID.
	static NiTMap<long, TGEvent *> *	eventTable;
	// For each object named as a source or destination, the events naming it.
	static NiTMap<TGObject *, NiTList<TGEvent *> *> *	objectEventTable;
	static bool		keepEventTable;			// cleared at shutdown: the table is deleted once empty
};

/*
================
TGEvent::IsTypeOf

Derived classes inline it into their own.
================
*/
// FUNCTION: STBC 0x006d5cf0
inline bool TGEvent::IsTypeOf( int type ) {
	if ( type == TGEVENT_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGEVENT_H__ */
