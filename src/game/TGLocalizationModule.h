/*
===============================================================================

	TGLocalizationModule.h

	The module of the localization databases: at startup it files the state
	functions that save and restore a Python shadow of a database by its file
	name, and at the end it drops every database the manager holds. The
	class and its names are invented.

	Evidence: the one instance at 0x997DD0 is built by 0x6CEDD0 (TGModule's
	constructor with the priority -1, then the vtable 0x895C38). The
	vtable's slots 0, 1, 4 and 5 are 0x6CEDF0, 0x6CEE00, 0x6CEE10 and
	0x6CEE30; slots 2, 3, 6 and 7 are TGModule's empty ones. Slot 0 tail-calls
	0x6CEE40, which files TGLocDBWrapperSerialize and
	TGLocDBWrapperUnserialize under "TGLocalizationDatabase"; slot 4 calls
	g_kLocalizationManager's DeleteAll and Purge.

===============================================================================
*/

#ifndef __TGLOCALIZATIONMODULE_H__
#define __TGLOCALIZATIONMODULE_H__

#include "TGModule.h"

// FUNCTION: STBC 0x006cedc0 ??1TGLocalizationModule@@QAE@XZ
class TGLocalizationModule : public TGModule {		// invented name
public:
	// FUNCTION: STBC 0x006cedd0 ??0TGLocalizationModule@@QAE@XZ
					TGLocalizationModule( void ) : TGModule( (unsigned int)-1 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot04( void );		// drops every database
	virtual void	VirtualSlot05( void );		// the end

	// Files the state functions of the databases' shadow instances (invented name).
	void			RegisterStateFunctions( void );
};

#endif /* !__TGLOCALIZATIONMODULE_H__ */
