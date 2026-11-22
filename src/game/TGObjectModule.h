/*
===============================================================================

	TGObjectModule.h

	The module of the TG objects: at startup it files the creators of the
	attribute objects for the load (types 3 and 4), the Python state
	functions of TGObject and NiAVObject shadow instances and the Python
	method TGString.GetCString; after a load it clears the map of the
	loaded objects' saved addresses; at the end it releases the table of
	the objects by ID. The class and its names are invented.

	Evidence: the one instance at 0x99A4E8 is built by 0x6EF5C0 (TGModule's
	constructor with the priority 0x80000000, then the vtable 0x89621C) and
	destroyed at exit through TGModule's destructor (0x6EF5B0). The
	vtable's slots 0, 1, 4 and 5 are 0x6EF5E0, 0x6EF7C0, 0x6EF830 and
	0x6EF840; slots 2, 3, 6 and 7 are TGModule's empty ones.

===============================================================================
*/

#ifndef __TGOBJECTMODULE_H__
#define __TGOBJECTMODULE_H__

#include "Python.h"
#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006ef5b0 ??1TGObjectModule@@QAE@XZ
class TGObjectModule : public TGModule {
public:
	// FUNCTION: STBC 0x006ef5c0 ??0TGObjectModule@@QAE@XZ
					TGObjectModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the Python state functions of the shadow instances.
	void			RegisterStateFunctions( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGAttrObject( long id );
	static TGObject *	CreateTGTemplatedAttrObject( long id );

	// The Python state functions, called with a shadow instance and its
	// state dictionary. A TGObject's state carries its ID ("_ID_"), and the
	// instance is pointed at the loaded object of that ID once the load's
	// pointers are fixed up; a NiAVObject's instance comes back invalid.
	static bool		GetTGObjectState( PyObject *instance, PyObject **state );
	static bool		SetTGObjectState( PyObject *instance, PyObject *state );
	static bool		SetNiAVObjectState( PyObject *instance, PyObject *state );
	// The fix-up callback: points the instance's "this" at the object.
	static bool		FixUpInstance( void *instance, void *id );
};

#endif /* !__TGOBJECTMODULE_H__ */
