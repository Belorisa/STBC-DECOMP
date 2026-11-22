/*
===============================================================================

	TGNetworkModule.h

	The module of the network classes: at startup it files the creators of
	the network and of its three events for the load (types 0x801, 0x865,
	0x866 and 0x867) and TGNetwork's handler table; each frame it updates
	every network; at the end it lets go of the list of networks without
	deleting them. The class and its names are invented.

	Evidence: the one instance at 0x995D68 is built by 0x6B2570 (TGModule's
	constructor with the priority 0x80000000, then the vtable 0x895770) and
	destroyed at exit through TGModule's destructor. The vtable's slots 0,
	1, 2, 4 and 5 are 0x6B2590, 0x6B25B0, 0x6B2620, 0x6B25C0 and 0x6B2610;
	slots 3, 6 and 7 are TGModule's empty ones. The unit's code runs from
	0x6B24F0 to 0x6B3890: the module, the static initialisers of the
	NetImmerse headers, the creators and the inline functions of the
	classes they create.

===============================================================================
*/

#ifndef __TGNETWORKMODULE_H__
#define __TGNETWORKMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006b2560 ??1TGNetworkModule@@QAE@XZ
class TGNetworkModule : public TGModule {
public:
	// FUNCTION: STBC 0x006b2570 ??0TGNetworkModule@@QAE@XZ
					TGNetworkModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );		// each frame
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );		// the end

	// Files the creator of each network class by its type, and the
	// network's handler table.
	void			RegisterClasses( void );
	// The network's handlers: their names, then the handlers.
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGNetwork( long id );
	static TGObject *	CreateTGMessageEvent( long id );
	static TGObject *	CreateTGPlayerEvent( long id );
	static TGObject *	CreateTGGameSpyEvent( long id );
};

#endif /* !__TGNETWORKMODULE_H__ */
