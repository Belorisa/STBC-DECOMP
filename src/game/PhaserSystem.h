/*
===============================================================================

	PhaserSystem.h

	The ship's phaser banks, as one weapon system, and the power level they
	fire at.

	0xF4 bytes: the SWIG wrapper PhaserSystem_Create allocates that much.

	Evidence: the constructor (0x573C90) calls WeaponSystem's, stores the
	vtable 0x893240, sets the power level (0xF0) to 1 and sets the firing
	flag (0xA9). The SWIG wrapper PhaserSystem_GetPowerLevel reads the power
	level inline.

===============================================================================
*/

#ifndef __PHASERSYSTEM_H__
#define __PHASERSYSTEM_H__

#include "WeaponSystem.h"

class TGEvent;
class TGObject;

// The type PhaserSystem::Cast (0x573C60) checks.
#define PHASERSYSTEM_TYPE		0x801F

// An event that sets the power level (the App constant).
#define ET_SET_PHASER_LEVEL		0x8000E0

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PhaserSystem : public WeaponSystem {
public:
					PhaserSystem( long id = 0 );
	virtual			~PhaserSystem( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ShipSubsystem
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );

	// WeaponSystem
	virtual void	VirtualSlot34( ObjectClass *target, NiPoint3 *offset );
	virtual void	StopFiring( void );

	virtual void	VirtualSlot38( ObjectClass *target );	// stops firing at the target (StopFiringAtTargetHandler passes the event's object)

	static PhaserSystem *	Create( void ) { return new PhaserSystem; }
	static PhaserSystem *	Cast( TGObject * );
	void	SetPowerLevel( int );
	int				GetPowerLevel( void ) { return powerLevel; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	void			StartFiringHandler( TGEvent *event );
	void			StopFiringHandler( TGEvent *event );
	void			StopFiringAtTargetHandler( TGEvent *event );
	void			SetPhaserLevelHandler( TGEvent *event );

protected:
	int				powerLevel;				// 0xF0

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PhaserSystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x00573d00
inline bool PhaserSystem::IsTypeOf( int type ) {
	if ( type == PHASERSYSTEM_TYPE ) {
		return true;
	}
	return WeaponSystem::IsTypeOf( type );
}

#endif /* !__PHASERSYSTEM_H__ */
