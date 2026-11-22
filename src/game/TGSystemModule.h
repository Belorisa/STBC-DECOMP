/*
===============================================================================

	TGSystemModule.h

	The module of the framework's system services: at its start it reads the
	trace configuration, hands the system wrapper the render window and the
	renderer and seeds the random numbers; at the shutdown it ends the
	profiling and deletes the trace outputs. The class and its names are
	invented.

	Evidence: the one instance at 0x99BFC8 is built by 0x715FD0 (TGModule's
	constructor with the priority 0xA0000000, then the vtable 0x896AA8) and
	destroyed at exit through 0x715FC0 (TGModule's destructor). The vtable's
	slots 0 and 5 are 0x715FF0 and 0x716090; slots 1, 2, 3, 4, 6 and 7 are
	TGModule's empty ones.

===============================================================================
*/

#ifndef __TGSYSTEMMODULE_H__
#define __TGSYSTEMMODULE_H__

#include "TGModule.h"

// FUNCTION: STBC 0x715FC0 ??1TGSystemModule@@QAE@XZ
class TGSystemModule : public TGModule {
public:
	// FUNCTION: STBC 0x715FD0 ??0TGSystemModule@@QAE@XZ
					TGSystemModule( void ) : TGModule( 0xA0000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the trace configuration, the window and renderer, the seed
	virtual void	VirtualSlot05( void );		// the profiling and the trace outputs go
};

#endif /* !__TGSYSTEMMODULE_H__ */
