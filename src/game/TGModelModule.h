/*
===============================================================================

	TGModelModule.h

	The module of the model and animation managers: at the end it purges
	the unused models, and at the shutdown it clears the animations and
	frees every model. The class and its names are invented.

	Evidence: the one instance at 0x997648 is built by 0x6C2F90 (TGModule's
	constructor with the priority 0x80000000, then the vtable 0x895A34) and
	destroyed at exit through TGModule's destructor (0x6C2F80). The vtable's
	slots 0, 4 and 5 are 0x6C2FB0 (empty), 0x6C2FC0 (g_kModelManager.Purge)
	and 0x6C2FD0 (g_kAnimationManager.ClearAnimations, then
	g_kModelManager.FreeAllModels); slots 1, 2, 3, 6 and 7 are TGModule's
	empty ones. Its unit (TGModelModule.cpp) runs from the module's static
	quad at 0x6C2F40 through the module and its locale-id pair to its
	<iostream> quads and NetImmerse registrations (0x6C3020-0x6C31FF).

===============================================================================
*/

#ifndef __TGMODELMODULE_H__
#define __TGMODELMODULE_H__

#include "TGModule.h"

// FUNCTION: STBC 0x006c2f80 ??1TGModelModule@@QAE@XZ
class TGModelModule : public TGModule {
public:
	// FUNCTION: STBC 0x006c2f90 ??0TGModelModule@@QAE@XZ
					TGModelModule( void ) : TGModule( 0x80000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );
	virtual void	VirtualSlot04( void );		// the unused models go
	virtual void	VirtualSlot05( void );		// every model and animation goes
};

#endif /* !__TGMODELMODULE_H__ */
