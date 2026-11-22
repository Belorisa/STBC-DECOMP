/*
===============================================================================

	PulseWeapon.h

	A weapon that fires pulses (a disruptor cannon): when it last fired and
	how fast its pulses leave it.

	0xD4 bytes: the SWIG wrapper PulseWeapon_Create allocates that much.

	Evidence: the constructor (0x574FD0) calls EnergyWeapon's, stores the
	vtable 0x893318, sets the last fire time (0xC8) to -100000 and the launch
	speed (0xCC) to 1 and clears the launch sound (0xD0). The SWIG wrappers
	PulseWeapon_* read the last fire time and the launch speed inline.
	SetProperty asks the property's script module for the launch speed and
	sound ("GetLaunchSpeed", "GetLaunchSound"). The vtable has 38 slots; 37
	is new.

===============================================================================
*/

#ifndef __PULSEWEAPON_H__
#define __PULSEWEAPON_H__

#include "EnergyWeapon.h"

class NiPoint3;
class PulseWeaponProperty;
class TGObject;
class Torpedo;

// The flags of a CREATE_PULSE_MESSAGE.
enum {
	CREATE_PULSE_PLAY_SOUND	= 0x01,
	CREATE_PULSE_HAS_TARGET	= 0x02
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PulseWeapon : public EnergyWeapon {
public:
					PulseWeapon( long id = 0 );
	virtual			~PulseWeapon( void );

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

	// ShipSubsystem
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );

	// Weapon
	virtual bool	Fire( float, bool );
	virtual bool	FireDumb( float, bool );
	virtual bool	CanFire( void );
	virtual TGPoint3	CalculateRoughDirection( void );
	virtual float	CalculateWeaponAppeal( void );

	virtual float	VirtualSlot37( void );

	static PulseWeapon *	Create( void ) { return new PulseWeapon; }
	static PulseWeapon *	Cast( TGObject * );
	bool	CanHitTarget( NiPoint3 *, NiPoint3 * );
	float	GetPowerScaled( void );
	float	GetDamageScale( void );
	PulseWeaponProperty *	GetProperty( void );
	float	GetArcHeightAngleMin( void );
	float	GetArcHeightAngleMax( void );
	float	GetArcWidthAngleMin( void );
	float	GetArcWidthAngleMax( void );
	void	GetArcHeightAngles( float *, float * );
	void	GetArcWidthAngles( float *, float * );
	float	GetCooldownTime( void );
	bool	IsInArc( NiPoint3 );
	Torpedo *	CreatePulse( NiPoint3 *direction, bool playSound );						// invented name
	// Tells the other players of the pulse (invented name, after CREATE_PULSE_MESSAGE).
	void	SendCreatePulseMessage( Torpedo *pulse, bool playSound, NiPoint3 *direction );
	// The pulse another player fired, from a CREATE_PULSE_MESSAGE
	// (MultiplayerGame::ReceiveCreatePulseMessage); NULL without a parent
	// ship in a set (invented name).
	Torpedo *	CreateNetPulse( bool playSound, int playerID, long targetID, NiPoint3 *direction );

	float			GetLastFireTime( void ) { return lastFireTime; }
	float			GetLaunchSpeed( void ) { return launchSpeed; }

protected:
	float			lastFireTime;			// 0xC8 -100000 at first
	float			launchSpeed;			// 0xCC 1 at first
	char *			launchSound;			// 0xD0 the launch sound's name, from the property's script module

	static TGEventHandlerTable	classHandlers;		// invented name
};

// GetObjType's value for a PulseWeapon.
#define PULSEWEAPON_TYPE		0x802D

/*
================
PulseWeapon::IsTypeOf
================
*/
// FUNCTION: STBC 0x00575020
inline bool PulseWeapon::IsTypeOf( int type ) {
	if ( type == PULSEWEAPON_TYPE ) {
		return true;
	}
	return EnergyWeapon::IsTypeOf( type );
}

#endif /* !__PULSEWEAPON_H__ */
