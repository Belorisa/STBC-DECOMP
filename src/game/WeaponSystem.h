/*
===============================================================================

	WeaponSystem.h

	A group of weapons of one kind (the phasers, the torpedo tubes, ...):
	the weapons are its child subsystems. It keeps the targets it fires at,
	its firing chains (the groups of weapons that fire together) and the
	firing state.

	0xF0 bytes, allocated from the pools (the scalar deleting destructor
	0x584240 frees through the pool of that size).

	Evidence: the constructor (0x5840A0) calls PoweredSubsystem's, builds
	the lists at 0xC0 and 0xD8 (NiTLists with two items per block, inline),
	stores the vtable 0x8938C4 (38 slots; 34 to 37 are new) and sets the
	members; created locally (ID 0), it adds the broadcast handler
	"WeaponSystem::ObjectExitedSet" for event 0x80005F. GetObjType returns
	0x801D, the type Cast checks. The SWIG wrappers WeaponSystem_* read
	IsTryingToFire (0xA8), IsFiring (0xA9), CanFire (0xAB),
	GetGroupFireMode (0xB0), GetFiringChainMode (0xB8), GetLastGroupFired
	(0xBC) and GetNumFiringChains (0xD8, the chains' count) inline, write
	SetForceUpdate (0xAC) inline, and call slots 35 (StartFiring), 36
	(StopFiring) and 37 (SetGroupFireMode). The target list's element is a
	0x10-byte record: the target's ID, then the offset aimed at (slot 34
	adds it; 0x585360 looks it up by the ID).

===============================================================================
*/

#ifndef __WEAPONSYSTEM_H__
#define __WEAPONSYSTEM_H__

#include "PoweredSubsystem.h"
#include "NiTList.h"
#include "WeaponSystemTarget.h"

class FiringChain;
class ObjectClass;
class TGEvent;
class TGObject;
class TGString;
class Weapon;
class WeaponSystemProperty;

// GetObjType's value for a WeaponSystem.
#define WEAPONSYSTEM_TYPE			0x801D

// Event types, as the SWIG constants of the App module give them (the
// constant table is in value order).
#define ET_START_FIRING				0x8000D7
#define ET_START_FIRING_NOTIFY		0x8000D8
#define ET_STOP_FIRING				0x8000D9
#define ET_STOP_FIRING_NOTIFY		0x8000DA
#define ET_STOP_FIRING_AT_TARGET	0x8000DB
#define ET_STOP_FIRING_AT_TARGET_NOTIFY	0x8000DC

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WeaponSystem : public PoweredSubsystem {
public:
					WeaponSystem( long id = 0 );
	virtual			~WeaponSystem( void );

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
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );

	// PoweredSubsystem
	virtual void	TurnOff( void );

	// Adds the target to the target list (with the offset, or the target's
	// own aim point) and starts trying to fire.
	virtual void	VirtualSlot34( ObjectClass *target, NiPoint3 *offset );
	virtual void	StartFiring( ObjectClass *target = NULL, NiPoint3 *offset = NULL );
	virtual void	StopFiring( void );
	virtual void	SetGroupFireMode( int mode );

	static WeaponSystem *	Create( void ) { return new WeaponSystem; }
	static WeaponSystem *	Cast( TGObject * );
	WeaponSystemProperty *	GetProperty( void );
	bool	IsSingleFire( void );
	bool	ShouldBeAimed( void );
	WeaponSystemTarget *	IsInTargetList( ObjectClass *target );
	void	StopFiringAtTarget( ObjectClass * );
	void	ClearTargetList( void );
	void	RemoveFromTargetList( ObjectClass *target );
	Weapon *	UpdateWeapons( float, bool * );
	void	UpdateTargetList( void );
	bool	UpdateWeapon( Weapon *weapon, float deltaTime );		// invented name
	void	SetFiringChainMode( int );
	FiringChain *	GetFiringChain( int );
	char *	GetFiringChainName( int );
	// Builds the firing chains from the property's string (0x585020; invented name).
	void	SetFiringChains( TGString *chains );
	Weapon *	GetLastWeaponFired( void );
	Weapon *	GetNextWeapon( void );
	void	Fire( void );

	bool			IsTryingToFire( void ) { return tryingToFire; }
	bool			IsFiring( void ) { return firing; }
	bool			CanFire( void ) { return canFire; }
	void			SetForceUpdate( bool forceUpdate_ ) { forceUpdate = forceUpdate_; }
	bool			IsForceUpdate( void ) { return forceUpdate; }		// invented name
	int				GetGroupFireMode( void ) { return groupFireMode; }
	int				GetFiringChainMode( void ) { return firingChainMode; }
	int				GetLastGroupFired( void ) { return lastGroupFired; }
	int				GetNumFiringChains( void ) { return firingChains.GetSize(); }
	NiTList<WeaponSystemTarget *> *	GetTargets( void ) { return &targets; }	// invented name
	Weapon *		GetWeapon( int index ) { return (Weapon *)GetChildSubsystem( index ); }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	void			ObjectExitedSet( TGEvent *event );

	// TODO identity: run for each ship of a set as the set steps (SetClass::
	// Update 0x40FFB0) before the ship's phasers and tractor beams; it
	// empties a static list at 0x98D328 and walks the ship's torpedoes,
	// phasers, pulse weapons, sensors and tractor beams (0x5856D0). Its class
	// is the unit it lies in.
	static void		Unknown5856D0( ShipClass *ship );
	// Adds the objects the system has as targets to the list, each once (invented name).
	static void		GatherTargets( WeaponSystem *system, NiTList<ObjectClass *> *list );
	// Whether the object is among the targets that Unknown5856D0 found the
	// ship's sensors see; a phaser bank fires only at those (invented name).
	static bool		IsVisibleTarget( ObjectClass *object );

	// The target with the ID in the target list; NULL when it is not there
	// (invented name). ShipClass::UseTargetOffsetTG calls it.
	WeaponSystemTarget *	FindTarget( long targetID );
	// Takes the record off the target list and deletes it (0x585490; invented name).
	void			RemoveTarget( WeaponSystemTarget *target );
	// Aims at a new point of a target: the target list's entry takes the
	// offset, and the weapons aiming at the target drop it (invented name).
	void			SetTargetOffset( ObjectClass *target, const NiPoint3 &offset );

protected:
	// Removes the target with the ID from the target list (invented name).
	void			RemoveTarget( long targetID );
	// Deletes the firing chains and empties their list (invented name).
	void			DeleteFiringChains( void );
public:
	// The world position of the target record's point, and the target object (invented name).
	bool			GetTargetPosition( WeaponSystemTarget *target, NiPoint3 *position, ObjectClass **targetObject );
protected:

	bool			tryingToFire;			// 0xA8
	bool			firing;					// 0xA9
	bool			unknownAA;				// 0xAA
	bool			canFire;				// 0xAB
	bool			forceUpdate;			// 0xAC
	int				groupFireMode;			// 0xB0
	int				lastWeaponFired;		// 0xB4 the index of the weapon fired last; -1 at first (invented name)
	int				firingChainMode;		// 0xB8
	int				lastGroupFired;			// 0xBC -1 at first
	NiTList<WeaponSystemTarget *>	targets;		// 0xC0 (invented name)
	NiTList<FiringChain *>	firingChains;	// 0xD8

	static TGEventHandlerTable	classHandlers;		// invented name
	// The targets the ship being stepped sees, gathered by Unknown5856D0
	// (invented name).
	static NiTList<ObjectClass *>	visibleTargets;
};

/*
================
WeaponSystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005841a0
inline bool WeaponSystem::IsTypeOf( int type ) {
	if ( type == WEAPONSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

/*
================
WeaponSystem::StartFiring
================
*/
// FUNCTION: STBC 0x00573cc0
inline void WeaponSystem::StartFiring( ObjectClass *target, NiPoint3 *offset ) {
	VirtualSlot34( target, offset );
}

/*
================
WeaponSystem::SetGroupFireMode
================
*/
// FUNCTION: STBC 0x00573ce0
inline void WeaponSystem::SetGroupFireMode( int mode ) {
	groupFireMode = mode;
}

struct _object;
typedef _object PyObject;

// The Python methods WeaponSystem.GetTargetList (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		WeaponSystem_GetTargetList( PyObject *self, PyObject *args );

#endif /* !__WEAPONSYSTEM_H__ */
