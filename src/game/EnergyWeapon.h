/*
===============================================================================

	EnergyWeapon.h

	A weapon that fires from a charge (a phaser bank, a tractor beam
	projector): the charge level, the power setting and the watcher on the
	charge.

	Evidence: the constructor (0x56F950) calls Weapon's, builds the charge
	watcher at 0xB0 (pointed at 0xBC), stores the vtable 0x8930D8 (37 slots;
	36 is new) and sets the members. The SWIG wrappers EnergyWeapon_* read
	and write the charge level (0xA0) and read the power setting (0xAC)
	inline, return the watcher, and call slots 35 (CalculateWeaponAppeal)
	and 36 (SetPowerSetting). GetFireStartSound and GetFireLoopSound build
	the fire sound's name with "Start" and "Loop" into 0xC0 and 0xC4, which
	the destructor frees. 0xA4 and 0xA8 start as the sound code's value
	at 0x896A9C.

===============================================================================
*/

#ifndef __ENERGYWEAPON_H__
#define __ENERGYWEAPON_H__

#include "Weapon.h"
#include "FloatRangeWatcher.h"

class EnergyWeaponProperty;
class TGObject;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EnergyWeapon : public Weapon {
public:
	// The SWIG constants EnergyWeapon_EW_*: the power settings.
	enum powerSetting_t {
		EW_LOW,
		EW_MEDIUM,
		EW_HIGH
	};

					EnergyWeapon( long id = 0 );
	virtual			~EnergyWeapon( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// ShipSubsystem
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );

	// Weapon
	virtual float	CalculateWeaponAppeal( void );

	virtual void	SetPowerSetting( int powerSetting_ );

	static EnergyWeapon *	Cast( TGObject * );
	EnergyWeaponProperty *	GetProperty( void );
	float	GetRechargeRate( void );
	float	GetNormalDischargeRate( void );
	float	GetMaxCharge( void );
	float	GetMinFiringCharge( void );
	char *	GetFireSound( void );
	char *	GetFireStartSound( void );
	char *	GetFireLoopSound( void );
	float	GetMaxDamage( void );
	float	GetMaxDamageDistance( void );
	float	GetChargePercentage( void );
	void	UpdateCharge( float, float );

	int				GetPowerSetting( void ) { return powerSetting; }
	FloatRangeWatcher *	GetChargeWatcher( void ) { return &chargeWatcher; }
	// The named local is (invented): TractorBeamProjector::CanReach copies
	// the charge through fld/fstp, which needs a second copy level here.
	float			GetChargeLevel( void ) { float level = chargeLevel; return level; }
	void			SetChargeLevel( float chargeLevel_ ) { chargeLevel = chargeLevel_; }

protected:
	float			chargeLevel;			// 0xA0
	int				unknownA4;				// 0xA4 unknown896A9C at first
	int				unknownA8;				// 0xA8 unknown896A9C at first
	int				powerSetting;			// 0xAC 1 at first
	FloatRangeWatcher	chargeWatcher;		// 0xB0 watches chargePercentage
	float			chargePercentage;		// 0xBC chargeLevel over the max charge, the charge watcher's variable (invented name)
	char *			fireStartSound;			// 0xC0 the fire sound's name with "Start", built on demand (invented name)
	char *			fireLoopSound;			// 0xC4 the fire sound's name with "Loop", built on demand (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

// GetObjType's value for an EnergyWeapon.
#define ENERGYWEAPON_TYPE		0x802B

/*
================
EnergyWeapon::IsTypeOf
================
*/
// FUNCTION: STBC 0x0056fa60
inline bool EnergyWeapon::IsTypeOf( int type ) {
	if ( type == ENERGYWEAPON_TYPE ) {
		return true;
	}
	return Weapon::IsTypeOf( type );
}

#endif /* !__ENERGYWEAPON_H__ */
