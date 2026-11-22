/*
===============================================================================

	TGInputModule.h

	The module of the TG input: at startup it files the creators of the
	input event classes for the load (types 0x401 to 0x406) and sets the
	input manager up. The class and its names are invented.

	Evidence: the one instance at 0x9991D0 is built by 0x6E3970
	(TGModule's constructor with the priority 0x80000000, then the vtable
	0x896108) and destroyed at exit through TGModule's destructor. The
	vtable's slots 0, 1, 2, 4 and 5 are 0x6E3990, 0x6E39B0, 0x6E39D0,
	0x6E39E0 and 0x6E39F0; slots 3, 6 and 7 are TGModule's empty ones.
	Slot 0 calls the class registration (0x6E3A00), whose creators the UI
	module files again for the keyboard (0x404, 0x6E3D70) and mouse (0x405,
	0x6E3D20) events.

===============================================================================
*/

#ifndef __TGINPUTMODULE_H__
#define __TGINPUTMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006e3960 ??1TGInputModule@@QAE@XZ
class TGInputModule : public TGModule {
public:
	// FUNCTION: STBC 0x006e3970 ??0TGInputModule@@QAE@XZ
					TGInputModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );

	// Files the creator of each input event class by its type.
	void			RegisterClasses( void );
	// The module's handlers: their names, then the handlers. Both are
	// empty, the input classes adding no handlers (names invented: every
	// other module with RegisterClasses calls RegisterHandlerNames after it
	// in slot 0 and RegisterHandlers in slot 1).
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );
	// Empty in this build; slot 1 calls it after RegisterHandlers.
	void			Unknown6E3E30( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGIEvent( long id );
	static TGObject *	CreateTGKeyboardEvent( long id );
	static TGObject *	CreateTGMouseEvent( long id );
	static TGObject *	CreateTGGamepadEvent( long id );
};

#endif /* !__TGINPUTMODULE_H__ */
