/*
===============================================================================

	HullClass.h

	The ship's hull: a subsystem with no behaviour of its own beyond its
	type, its names and its handler table.

	0x88 bytes, ShipSubsystem's size: the scalar deleting destructor
	(0x560530) frees through the pool of that size.

	Evidence: the constructor (0x560470) calls ShipSubsystem's and stores
	the vtable 0x892C98 (ShipSubsystem's 30 slots; only the destructor,
	GetObjType, IsTypeOf, the class names and GetClassHandlers are
	overridden). GetObjType returns 0x8027, the type HullClass::Cast checks.

===============================================================================
*/

#ifndef __HULLCLASS_H__
#define __HULLCLASS_H__

#include "ShipSubsystem.h"

class TGObject;

// GetObjType's value for a HullClass.
#define HULLCLASS_TYPE			0x8027

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class HullClass : public ShipSubsystem {
public:
					HullClass( long id = 0 );
	virtual			~HullClass( void );

	// The SWIG wrapper HullClass_Create expands it.
	static HullClass *	Create( void ) { return new HullClass; }

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	static HullClass *	Cast( TGObject * );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
HullClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x005604a0
inline bool HullClass::IsTypeOf( int type ) {
	if ( type == HULLCLASS_TYPE ) {
		return true;
	}
	return ShipSubsystem::IsTypeOf( type );
}

#endif /* !__HULLCLASS_H__ */
