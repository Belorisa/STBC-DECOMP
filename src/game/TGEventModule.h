/*
===============================================================================

	TGEventModule.h

	The module of the event system: at startup it files the creators of the
	event classes for the load (types 0x101 to 0x10C), each frame it hands
	the queued events to their handlers. The class and its names are
	invented.

	Evidence: the one instance at 0x998260 is built by 0x6D4510 (TGModule's
	constructor with the priority -1, then the vtable 0x895F58). The
	vtable's slots 0, 1, 3, 4 and 5 are 0x6D4560, 0x6D4B20, 0x6D4B40,
	0x6D4B60 and 0x6D4BA0; slots 2, 6 and 7 are TGModule's empty ones. The
	startup (0x6D4560) files the creators by type, then calls 0x6D94E0 and
	0x6D4BF0 (not written yet).

===============================================================================
*/

#ifndef __TGEVENTMODULE_H__
#define __TGEVENTMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006d4550 ??1TGEventModule@@QAE@XZ
class TGEventModule : public TGModule {		// invented name
public:
	// FUNCTION: STBC 0x006d4510 ??0TGEventModule@@QAE@XZ
					TGEventModule( void ) : TGModule( (unsigned int)-1 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot03( void );		// each frame: processes the queued events
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the state functions of TGPythonInstanceWrapper's shadow instances (invented name).
	void			RegisterStateFunctions( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGEvent( long id );
	static TGObject *	CreateTGEventHandlerObject( long id );
	static TGObject *	CreateTGTimer( long id );
	static TGObject *	CreateTGBoolEvent( long id );
	static TGObject *	CreateTGCharEvent( long id );
	static TGObject *	CreateTGShortEvent( long id );
	static TGObject *	CreateTGIntEvent( long id );
	static TGObject *	CreateTGFloatEvent( long id );
	static TGObject *	CreateTGStringEvent( long id );
	static TGObject *	CreateTGVoidPtrEvent( long id );
	static TGObject *	CreateTGPythonInstanceWrapper( long id );
	static TGObject *	CreateTGObjPtrEvent( long id );
};

#endif /* !__TGEVENTMODULE_H__ */
