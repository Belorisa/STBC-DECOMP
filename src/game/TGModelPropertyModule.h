/*
===============================================================================

	TGModelPropertyModule.h

	The module of the model properties: at startup it files the creators
	of TGModelProperty and TGModelPropertySet for the load (types 0x601 and
	0x602) and, once, the "Generic Property" filter; at the end it clears
	the property templates, and at the shutdown the registered filters.
	The class and its names are invented.

	Evidence: the one instance at 0x999CC0 is built by 0x6E8960 (TGModule's
	constructor with the priority 0x20000000, then the vtable 0x896180) and
	destroyed at exit through TGModule's destructor (0x6E8950). The
	vtable's slots 0, 4 and 5 are 0x6E8980, 0x6E8B10 and 0x6E8B30; slots 1,
	2, 3, 6 and 7 are TGModule's empty ones.

===============================================================================
*/

#ifndef __TGMODELPROPERTYMODULE_H__
#define __TGMODELPROPERTYMODULE_H__

#include "TGModule.h"

class TGObject;
class TGModelProperty;

// FUNCTION: STBC 0x006e8950 ??1TGModelPropertyModule@@QAE@XZ
class TGModelPropertyModule : public TGModule {
public:
	// FUNCTION: STBC 0x006e8960 ??0TGModelPropertyModule@@QAE@XZ
					TGModelPropertyModule( void ) : TGModule( 0x20000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot04( void );		// the end: the templates go
	virtual void	VirtualSlot05( void );		// the shutdown: the filters go

	// The creators: the object of the class with the ID.
	static TGObject *	CreateTGModelProperty( long id );
	static TGObject *	CreateTGModelPropertySet( long id );

	// The "Generic Property" filter: takes every model property.
	static bool		GenericPropertyFilter( TGModelProperty *property );
};

#endif /* !__TGMODELPROPERTYMODULE_H__ */
