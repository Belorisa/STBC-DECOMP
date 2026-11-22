/*
===============================================================================

	TGSoundModule.h

	The module of the sound classes: at startup it files the creators of
	the sound manager, the music player, the music fade event and the sound
	region for the load (types 0x301, 0x302, 0x367 and 0x303), the handler
	tables of the manager and the player and their handler names, and
	starts the sound manager on the application's window; each frame it
	updates the CD player and the music; at the end it shuts the sound
	manager down. The class and its names are invented.

	Evidence: the one instance at 0x99B748 is built by 0x7096C0 (TGModule's
	constructor with the priority 0x80000000, then the vtable 0x8968B0) and
	destroyed at exit through TGModule's destructor. The vtable's slots 0
	to 3 and 5 are 0x7096E0, 0x709740, 0x709750, 0x709A30 and 0x709730;
	slots 4, 6 and 7 are TGModule's empty ones.

===============================================================================
*/

#ifndef __TGSOUNDMODULE_H__
#define __TGSOUNDMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x007096b0 ??1TGSoundModule@@QAE@XZ
class TGSoundModule : public TGModule {
public:
	// FUNCTION: STBC 0x007096c0 ??0TGSoundModule@@QAE@XZ
					TGSoundModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );		// each frame
	virtual void	VirtualSlot03( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the creator of each sound class by its type, and the handler
	// tables of the classes that handle events.
	void			RegisterClasses( void );
	// The handlers of the manager and the player: their names, then the
	// handlers.
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGSoundManager( long id );
	static TGObject *	CreateTGMusic( long id );
	static TGObject *	CreateTGMusicFadeEvent( long id );
	static TGObject *	CreateTGSoundRegion( long id );
};

#endif /* !__TGSOUNDMODULE_H__ */
