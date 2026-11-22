/*
===============================================================================

	PulseWeaponSystem.h

	The ship's pulse weapons, as one weapon system.

	0xF0 bytes, WeaponSystem's size: the SWIG wrapper PulseWeaponSystem_Create
	allocates that much.

	Evidence: the constructor (0x5773B0) calls WeaponSystem's and stores the
	vtable 0x8933B0; it adds no members.

===============================================================================
*/

#ifndef __PULSEWEAPONSYSTEM_H__
#define __PULSEWEAPONSYSTEM_H__

#include "WeaponSystem.h"

class TGObject;

// The type PulseWeaponSystem::Cast (0x577380) checks.
#define PULSEWEAPONSYSTEM_TYPE		0x8020

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PulseWeaponSystem : public WeaponSystem {
public:
					PulseWeaponSystem( long id = 0 );
	virtual			~PulseWeaponSystem( void );

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

	// ShipSubsystem
	virtual void	VirtualSlot27( TGStream *stream );

	static PulseWeaponSystem *	Create( void ) { return new PulseWeaponSystem; }
	static PulseWeaponSystem *	Cast( TGObject * );

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PulseWeaponSystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005773e0
inline bool PulseWeaponSystem::IsTypeOf( int type ) {
	if ( type == PULSEWEAPONSYSTEM_TYPE ) {
		return true;
	}
	return WeaponSystem::IsTypeOf( type );
}

#endif /* !__PULSEWEAPONSYSTEM_H__ */
