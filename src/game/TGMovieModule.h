/*
===============================================================================

	TGMovieModule.h

	The module of the movie classes: at startup it files the creators of
	TGMovieAction and TGCreditAction for the load (types 0x901 and 0x902);
	each frame it advances the credits by the application's clock; at the
	end it stops every movie and switches out of movie mode. The class and
	its names are invented.

	Evidence: the one instance at 0x995988 is built by 0x6ADFB0 (TGModule's
	constructor with the priority 0x80000000, then the vtable 0x89568C) and
	destroyed at exit through TGModule's destructor (0x6ADFA0). The
	vtable's slots 0, 2, 4 and 5 are 0x6ADFE0, 0x6AE280, 0x6AE2A0 and
	0x6ADFD0; slots 1, 3, 6 and 7 are TGModule's empty ones. Its unit
	(TGMovieModule.cpp) runs from 0x6AD620 to 0x6AE2EF: the <iostream> pair,
	the NetImmerse registrations, the module's static quad, the module, the
	creators and the inline functions of the classes they create, then the
	locale-id pair. TGMovieManager's unit follows at 0x6AE2F0.

===============================================================================
*/

#ifndef __TGMOVIEMODULE_H__
#define __TGMOVIEMODULE_H__

#include "TGModule.h"

class TGObject;

// FUNCTION: STBC 0x006adfa0 ??1TGMovieModule@@QAE@XZ
class TGMovieModule : public TGModule {
public:
	// FUNCTION: STBC 0x006adfb0 ??0TGMovieModule@@QAE@XZ
					TGMovieModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot02( void );		// each frame
	virtual void	VirtualSlot04( void );		// the end: the movies stop
	virtual void	VirtualSlot05( void );

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGMovieAction( long id );
	static TGObject *	CreateTGCreditAction( long id );
};

#endif /* !__TGMOVIEMODULE_H__ */
