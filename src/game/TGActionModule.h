/*
===============================================================================

	TGActionModule.h

	The module of the TG actions: at startup it files the creators of the
	action classes for the load (types 0x701 to 0x70F), the action
	manager's event handlers and the Python method TGConditionAction.
	SetEvaluationFunction; each frame it advances the phoneme sequences by
	the game frame time (at most the time rate); at the end it deletes the
	live actions. The class and its names are invented.

	Evidence: the one instance at 0x99AEB0 is built by 0x6FCB60 (TGModule's
	constructor with the priority -2, then the vtable 0x896380) and
	destroyed at exit through TGModule's destructor. The vtable's slots 0,
	1, 2, 4 and 5 are 0x6FCBB0, 0x6FDA10, 0x6FDA20, 0x6FDA70 and 0x6FDA90;
	slots 3, 6 and 7 are TGModule's empty ones.

===============================================================================
*/

#ifndef __TGACTIONMODULE_H__
#define __TGACTIONMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006fcba0 ??1TGActionModule@@QAE@XZ
class TGActionModule : public TGModule {
public:
	// FUNCTION: STBC 0x006fcb60 ??0TGActionModule@@QAE@XZ
					TGActionModule( void ) : TGModule( (unsigned int)-2 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );		// each frame
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the creator of each action class by its type.
	void			RegisterClasses( void );
	// The action manager's handlers: their names, then the handlers.
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGAction( long id );
	static TGObject *	CreateTGActionManager( long id );
	static TGObject *	CreateTGSequence( long id );
	static TGObject *	CreateTGSequenceEvent( long id );
	static TGObject *	CreateTGConditionAction( long id );
	static TGObject *	CreateTGAnimPosition( long id );
	static TGObject *	CreateTGSoundAction( long id );
	static TGObject *	CreateTGPhonemeAction( long id );
	static TGObject *	CreateTGPhonemeSequence( long id );
	static TGObject *	CreateTGOverlayAction( long id );
	static TGObject *	CreateTGScriptAction( long id );
	static TGObject *	CreateTGTimedAction( long id );
	static TGObject *	CreateTGAnimAction( long id );
};

#endif /* !__TGACTIONMODULE_H__ */
