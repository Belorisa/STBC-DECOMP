/*
===============================================================================

	Weapon.h

	A single weapon (a phaser bank, a torpedo tube, ...): a child of its
	weapon system. It knows whether it is firing and the target it fires at.

	0xA0 bytes: TorpedoTube's constructor (0x57C4B0) places its first member
	there.

	Evidence: the constructor (0x583280) calls ShipSubsystem's, stores the
	vtable 0x893834 (36 slots; 30 to 35 are new), copies NiPoint3::ZERO into
	0x90 and sets 0x9C to a random value. The SWIG wrappers Weapon_* read
	and write the firing flag (0x88) and read the target's ID (0x8C) inline,
	multiply the parent's and the weapon's condition percentages, and call
	slots 30 (StopFiring), 31 (Fire), 32 (FireDumb), 33 (CanFire), 34
	(CalculateRoughDirection) and 35 (CalculateWeaponAppeal). GetObjType
	returns 0x802A, the type Cast checks.

===============================================================================
*/

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "ShipSubsystem.h"
#include "TGPoint3.h"
#include "WeaponSystemTarget.h"

class TGObject;
class WeaponProperty;

// GetObjType's value for a Weapon.
#define WEAPON_TYPE			0x802A

// A weapon fired (the SWIG constant ET_WEAPON_FIRED).
#define ET_WEAPON_FIRED			0x80007C

// A weapon couldn't fire at its target (the SWIG constant ET_CANT_FIRE).
#define ET_CANT_FIRE			0x800037

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class Weapon : public ShipSubsystem {
public:
					Weapon( long id = 0 );
	virtual			~Weapon( void );

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

	virtual void	StopFiring( void );
	virtual bool	Fire( float, bool );
	virtual bool	FireDumb( float, bool );
	virtual bool	CanFire( void );
	virtual TGPoint3	CalculateRoughDirection( void );
	virtual float	CalculateWeaponAppeal( void );

	static Weapon *	Cast( TGObject * );
	WeaponProperty *	GetProperty( void );
	bool	IsMemberOfGroup( int );
	float	GetDamageRadiusFactor( void );
	bool	IsDumbFire( void );

	bool			IsFiring( void ) { return firing; }
	void			SetFiring( bool firing_ ) { firing = firing_; }
	float			GetOverallConditionPercentage( void ) { return parentSubsystem->GetConditionPercentage() * conditionPercentage; }
	int				GetTargetID( void ) { return target.targetID; }
	void			SetTarget( long targetID, const NiPoint3 &offset ) { target.targetID = targetID; target.offset = offset; }	// invented name
	void			SetTarget( WeaponSystemTarget newTarget );	// invented name

	friend class WeaponSystem;

protected:
	bool			firing;					// 0x88
	WeaponSystemTarget	target;				// 0x8C the target and the point aimed at (invented name)
	float			unknown9C;				// 0x9C time since the last firing check, random in [0, FIRING_INTERVAL) at first

	static TGEventHandlerTable	classHandlers;		// invented name

public:
	// A weapon's firing is checked once this much time has accumulated (invented name).
	static const float	FIRING_INTERVAL;
};

/*
================
Weapon::IsTypeOf
================
*/
// FUNCTION: STBC 0x00583300
inline bool Weapon::IsTypeOf( int type ) {
	if ( type == WEAPON_TYPE ) {
		return true;
	}
	return ShipSubsystem::IsTypeOf( type );
}

/*
================
Weapon::Fire

A plain weapon cannot fire.
================
*/
// FUNCTION: STBC 0x0056f9f0
inline bool Weapon::Fire( float, bool ) {
	return false;
}

/*
================
Weapon::FireDumb
================
*/
// FUNCTION: STBC 0x0056fa00
inline bool Weapon::FireDumb( float, bool ) {
	return false;
}

/*
================
Weapon::CanFire
================
*/
// FUNCTION: STBC 0x0056fa10
inline bool Weapon::CanFire( void ) {
	return false;
}

/*
================
Weapon::CalculateRoughDirection

A plain weapon points nowhere.
================
*/
// FUNCTION: STBC 0x0056fa20
inline TGPoint3 Weapon::CalculateRoughDirection( void ) {
	return TGPoint3::ZERO;
}

/*
================
Weapon::SetTarget
================
*/
// FUNCTION: STBC 0x00584f70 ?SetTarget@Weapon@@QAEXUWeaponSystemTarget@@@Z @game/WeaponSystem
inline void Weapon::SetTarget( WeaponSystemTarget newTarget ) {
	target = newTarget;
}

#endif /* !__WEAPON_H__ */