/*
===============================================================================

	ShipSubsystems.h

	A ship's subsystems: every subsystem in a list, and the main one of
	each kind. The class name is invented: no string or SWIG type names
	it.

	0x64 bytes, embedded in ShipClass at 0x27C.

	Evidence: the constructor (0x5B5D00) stores the vtable 0x8944C8 (4
	slots), builds two lists of 2-item blocks (0x04, 0x1C) and clears the
	pointers at 0x34-0x60; the destructor is 0x5B5D80. The SWIG wrappers
	ShipClass_Get*Subsystem/System, GetShields and GetHull read the
	pointers through the ship (0x2B0-0x2DC). ShipClass::GetSubsystemByProperty
	(0x5AC3F0) and StartGetSubsystemMatch (0x5AC370) walk the list at 0x04.

===============================================================================
*/

#ifndef __SHIPSUBSYSTEMS_H__
#define __SHIPSUBSYSTEMS_H__

#include "NiTList.h"

class CloakingSubsystem;
class HullClass;
class ImpulseEngineSubsystem;
class PhaserSystem;
class PowerSubsystem;
class PulseWeaponSystem;
class RepairSubsystem;
class SensorSubsystem;
class ShieldClass;
class ShipSubsystem;
class TGStream;
class TorpedoSystem;
class TractorBeamSystem;
class WarpEngineSubsystem;
class Weapon;
class WeaponSystem;

// invented name
class ShipSubsystems {
public:
					ShipSubsystems( void );
					~ShipSubsystems( void );

	// ShipClass's WriteToStream and ReadFromStream (0x5B0F00, 0x5B1220)
	// pass them the stream (invented names).
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	VirtualSlot02( void );	// fails until the subsystems it names are loaded
	virtual void	VirtualSlot03( void );

	// The checksum of the subsystems' figures that VirtualSlot72/73 send
	// and compare: each figure is scaled to an integer, xored into the
	// checksum and the checksum rotated one bit (invented description).
	// The checksum of every subsystem the ship has, with the figures of
	// their properties (VirtualSlot72 sends it, VirtualSlot73 compares it).
	int				Unknown5B5EB0( void );
	void			Unknown5B6C10( float value, void *checksum );
	// The checksum of a subsystem's property figures and flags, its
	// children's included.
	float			Unknown5B6170( ShipSubsystem *subsystem );
	// The checksum of a weapon system: its subsystem checksum, its flags,
	// its weapons' (Unknown5B6560) and, for torpedoes, each ammo type's
	// count, script and ammo names and launch figures.
	float			Unknown5B6330( WeaponSystem *system );
	float			Unknown5B6560( Weapon *weapon );	// the checksum of a weapon's figures, by its kind

	// The warp engines (invented name). ShipClass::SetInvincible's store of the
	// flag stays ahead of this load in retail, which the scheduler does only when
	// the load is addressed through another pointer than the store's `this`
	// (docs/c2-backend.md, "Scheduling": memory dependences); shown by the
	// schedule alone, so it stays MATCH-PENDING (docs/rulings.md).
	WarpEngineSubsystem *	GetWarpEngineSubsystem( void ) { return warpEngineSubsystem; }

	NiTList<ShipSubsystem *>	list;		// 0x04 every subsystem (invented name)
	NiTList<ShipSubsystem *>	unknown1C;	// 0x1C the subsystems that are no weapon system, weapon, warp engines or cloak (ShipClass::AddSubsystem 0x5B3E50)
	PowerSubsystem *	powerSubsystem;		// 0x34
	TorpedoSystem *	torpedoSystem;			// 0x38
	PhaserSystem *	phaserSystem;			// 0x3C
	PulseWeaponSystem *	pulseWeaponSystem;	// 0x40
	ShieldClass *	shields;				// 0x44
	HullClass *		hull;					// 0x48
	SensorSubsystem *	sensorSubsystem;	// 0x4C
	ImpulseEngineSubsystem *	impulseEngineSubsystem;	// 0x50
	WarpEngineSubsystem *	warpEngineSubsystem;	// 0x54
	TractorBeamSystem *	tractorBeamSystem;	// 0x58
	RepairSubsystem *	repairSubsystem;	// 0x5C
	CloakingSubsystem *	cloakingSubsystem;	// 0x60
};

#endif /* !__SHIPSUBSYSTEMS_H__ */
