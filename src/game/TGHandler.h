/*
===============================================================================

	TGHandler.h

	One handler of an event type: a Python function or method named by a
	string, or a registered C++ function or method. The handlers of one
	event type form a list through next; TGEventHandlerTable keeps the head
	of each list.

	0x14 bytes. Evidence: the constructor (0x6E09E0) stores the vtable
	0x8960F4 (4 slots: WriteToStream, ReadFromStream and two fix-up hooks;
	the destructor is not virtual, and the table deletes handlers with a
	direct call to 0x6E0A00) and sets the target to ANY_TARGET. The name is
	that of the messages "Saving TGHandler to ANY_TARGET" and "Loading
	TGHandler to ANY_TARGET" (0x6E0A40, 0x6E0B20).

===============================================================================
*/

#ifndef __TGHANDLER_H__
#define __TGHANDLER_H__

#include "TGEventHandlerObject.h"

class TGEvent;
class TGStream;

// The flags word.
#define HANDLER_METHOD			0x01	// called as a method of the handling object
#define HANDLER_PYTHON			0x02	// function holds a Python function name
#define HANDLER_CALLING			0x04	// set while the handler runs
#define HANDLER_DELETE_PENDING	0x08	// removed while it ran: Call deletes it

// A registered C++ handler: a plain function taking the handling object
// and the event, or (HANDLER_METHOD) a method of the handling object.
typedef void ( *TGHandlerFunction )( TGEventHandlerObject *object, TGEvent *event );	// invented name

// The registry of C++ handlers by the hash of their names, which a handler
// set by name looks up. The class, its methods and the instance (0x998638)
// are named by invention.
// One entry of the registry: the function or method, and the hash of its name.
struct TGHandlerRegistryEntry {		// invented name
	union {
		TGHandlerFunction	function;
		TGHandlerMethod		method;
	};
	int					nameHash;
};

class TGHandlerRegistry {		// invented name
public:
						TGHandlerRegistry( void ) { count = 0; allocatedSize = 0; entries = NULL; }

	TGHandlerFunction	Find( int nameHash );							// invented name
	int					FindNameHash( TGHandlerFunction function );		// invented name
	// Adds a function or a method under the hash of its name; false when the
	// name is already there or the entries cannot grow (invented name).
	bool				Add( TGHandlerFunction function, TGHandlerMethod method, int nameHash );
	// Frees the entries (invented name).
	void				Clear( void );

	unsigned int				count;			// 0x00 entries in use
	unsigned int				allocatedSize;	// 0x04 bytes allocated for entries; grows 4 entries at a time
	TGHandlerRegistryEntry *	entries;		// 0x08
};

extern TGHandlerRegistry	handlerRegistry;		// invented name

// The hash a handler name is registered under.
int					TGHashString( char *string );					// invented name

class TGHandler {
public:
					TGHandler( void );
					~TGHandler( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );		// invented name, as TGObject's
	virtual void	PostFixUp( void );			// invented name, as TGObject's

	void			Clear( void );												// invented name
	void			Call( TGEvent *event, TGEventHandlerObject *object );		// invented name
	void			CallPython( PyObject *event, PyObject *object );			// invented name
	bool			SetPythonFunction( char *name, bool method );				// invented name
	bool			SetFunction( char *name, bool method );						// invented name
	void			SetPython( bool python );									// invented name
	void			SetMethod( bool method );									// invented name

	TGHandlerFunction	GetFunction( void ) { return function; }			// invented name
	bool			IsMethod( void ) { return flags & HANDLER_METHOD; }			// invented name
	bool			IsPython( void ) { return ( flags >> 1 ) & 1; }				// invented name
	bool			IsCalling( void ) { return ( flags >> 2 ) & 1; }			// invented name
	bool			IsDeletePending( void ) { return ( flags >> 3 ) & 1; }		// invented name
	void			SetDeletePending( void ) { flags |= HANDLER_DELETE_PENDING; }	// invented name

	unsigned int	flags;				// 0x04 HANDLER_*
	TGHandler *		next;				// 0x08 the next handler of the same event type
	TGEventHandlerObject *	target;		// 0x0C ANY_TARGET, or the object whose events it takes; an ID between ReadFromStream and PostFixUp
	union {
		char *			pythonName;		// 0x10 HANDLER_PYTHON: "module.function", heap copy
		TGHandlerFunction	function;	// 0x10 a registered function
		TGHandlerMethod	method;			// 0x10 HANDLER_METHOD: a registered method
	};
};

#endif /* !__TGHANDLER_H__ */
