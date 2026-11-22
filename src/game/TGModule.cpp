/*
===============================================================================

	TGModule.cpp

===============================================================================
*/

#include <stddef.h>

#include "TGModule.h"

// DATA: STBC 0x0099c8a4
TGModule *TGModule::first;

// DATA: STBC 0x0099c8a8
TGModule *TGModule::last;

/*
================
TGModule::TGModule
================
*/
// FUNCTION: STBC 0x0071e2b0
TGModule::TGModule( unsigned int priority_ ) {
	next = NULL;
	prev = NULL;
	priority = priority_;
	AddModule( this );
}

/*
================
TGModule::~TGModule
================
*/
// FUNCTION: STBC 0x0071e2e0
TGModule::~TGModule( void ) {
	RemoveModule( this );
}

/*
================
TGModule::AddModule
================
*/
// FUNCTION: STBC 0x0071e2f0
void TGModule::AddModule( TGModule *module ) {
	TGModule *after = first;
	TGModule *before = last;
	if ( module == NULL || module->next != NULL || module->prev != NULL ) {
		return;
	}
	for ( ; after != NULL; after = after->next ) {
		if ( module->priority > after->priority ) {
			break;
		}
	}
	if ( after != NULL ) {
		before = after->prev;
	}
	module->next = after;
	if ( after == NULL ) {
		last = module;
	} else {
		after->prev = module;
	}
	module->prev = before;
	if ( before == NULL ) {
		first = module;
	} else {
		before->next = module;
	}
}

/*
================
TGModule::RemoveModule
================
*/
// FUNCTION: STBC 0x0071e360
void TGModule::RemoveModule( TGModule *module ) {
	TGModule *found = first;
	if ( module == NULL ) {
		return;
	}
	for ( ; found != NULL; found = found->next ) {
		if ( found == module ) {
			break;
		}
	}
	if ( found == NULL ) {
		return;
	}
	if ( found->prev != NULL ) {
		found->prev->next = found->next;
	} else {
		first = found->next;
	}
	if ( found->next != NULL ) {
		found->next->prev = found->prev;
	} else {
		last = found->prev;
	}
}

/*
================
TGModule::ForEach
================
*/
// FUNCTION: STBC 0x0071e3b0
void TGModule::ForEach( moduleFunction_t function ) {
	for ( TGModule *module = first; module != NULL; module = module->next ) {
		( module->*function )();
	}
}

/*
================
TGModule::ForEachReverse
================
*/
// FUNCTION: STBC 0x0071e3d0
void TGModule::ForEachReverse( moduleFunction_t function ) {
	for ( TGModule *module = last; module != NULL; module = module->prev ) {
		( module->*function )();
	}
}

/*
================
TGModule::CallVirtualSlot00
================
*/
// FUNCTION: STBC 0x0071e3f0
void TGModule::CallVirtualSlot00( void ) {
	ForEach( &TGModule::VirtualSlot00 );
}

/*
================
TGModule::CallVirtualSlot05
================
*/
// FUNCTION: STBC 0x0071e400
void TGModule::CallVirtualSlot05( void ) {
	ForEachReverse( &TGModule::VirtualSlot05 );
}

/*
================
TGModule::CallVirtualSlot01
================
*/
// FUNCTION: STBC 0x0071e410
void TGModule::CallVirtualSlot01( void ) {
	ForEach( &TGModule::VirtualSlot01 );
}

/*
================
TGModule::CallVirtualSlot02
================
*/
// FUNCTION: STBC 0x0071e420
void TGModule::CallVirtualSlot02( void ) {
	ForEach( &TGModule::VirtualSlot02 );
}

/*
================
TGModule::CallVirtualSlots03And04
================
*/
// FUNCTION: STBC 0x0071e430
void TGModule::CallVirtualSlots03And04( void ) {
	ForEachReverse( &TGModule::VirtualSlot03 );
	ForEachReverse( &TGModule::VirtualSlot04 );
}

/*
================
TGModule::ForEachWithArgument
================
*/
// FUNCTION: STBC 0x0071e450
void TGModule::ForEachWithArgument( moduleFunctionWithArgument_t function, int argument ) {
	for ( TGModule *module = first; module != NULL; module = module->next ) {
		( module->*function )( argument );
	}
}

/*
================
TGModule::ForEachReverseWithArgument
================
*/
// FUNCTION: STBC 0x0071e480
void TGModule::ForEachReverseWithArgument( moduleFunctionWithArgument_t function, int argument ) {
	for ( TGModule *module = last; module != NULL; module = module->prev ) {
		( module->*function )( argument );
	}
}

/*
================
TGModule::CallVirtualSlot06
================
*/
// FUNCTION: STBC 0x0071e4b0
void TGModule::CallVirtualSlot06( void ) {
	ForEachReverse( &TGModule::VirtualSlot06 );
}

/*
================
TGModule::CallVirtualSlot07
================
*/
// FUNCTION: STBC 0x0071e4c0
void TGModule::CallVirtualSlot07( int argument ) {
	ForEachReverseWithArgument( &TGModule::VirtualSlot07, argument );
}
