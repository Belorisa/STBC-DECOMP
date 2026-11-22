/*
===============================================================================

	PoweredSubsystem.h

	A subsystem that draws power from the ship's power subsystem: the power
	it wants (a percentage of its property's normal power per second), the
	power it received, and whether it is on.

	0xA8 bytes, allocated from the pools (the scalar deleting destructor
	0x562330 frees through the pool of that size).

	Evidence: the constructor (0x562240) calls ShipSubsystem's, stores the
	vtable 0x892D98 (34 slots; 30 to 33 are new) and sets the members.
	GetObjType returns 0x801C, the type Cast checks. The SWIG wrappers
	PoweredSubsystem_* read the members inline (GetPowerReceived 0x88,
	GetPowerWanted 0x8C, GetPowerPercentageWanted 0x90, GetPowerPercentage
	0x94, GetNormalPowerPercentage 0x98, IsOn 0x9C) and call slots 30
	(GetNormalPowerWanted), 31 (TurnOn) and 32 (TurnOff).

===============================================================================
*/

#ifndef __POWEREDSUBSYSTEM_H__
#define __POWEREDSUBSYSTEM_H__

#include "ShipSubsystem.h"

class PowerSubsystem;
class PoweredSubsystemProperty;
class TGObject;

// GetObjType's value for a PoweredSubsystem.
#define POWEREDSUBSYSTEM_TYPE		0x801C

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PoweredSubsystem : public ShipSubsystem {
public:
					PoweredSubsystem( long id = 0 );
	virtual			~PoweredSubsystem( void );

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
	virtual void	SetParentShip( ShipClass *ship );
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );
	virtual void	VirtualSlot28( TGStream *stream, bool );
	virtual void	VirtualSlot29( TGStream *stream, float time );

	virtual float	GetNormalPowerWanted( void );
	virtual void	TurnOn( void );
	virtual void	TurnOff( void );
	virtual bool	VirtualSlot33( float percentage, float time );

	// Where Update draws the power from (invented names): the PowerSubsystem's
	// DeductPower, StealPower or StealPowerFromReserve.
	enum powerMode_t {
		POWER_MODE_DEDUCT,
		POWER_MODE_STEAL,
		POWER_MODE_STEAL_FROM_RESERVE
	};

	static PoweredSubsystem *	Cast( TGObject * );
	PoweredSubsystemProperty *	GetProperty( void );
	void	SetPowerPercentageWanted( float );
	void	SetPowerSource( PowerSubsystem * );

	float			GetPowerReceived( void ) { return powerReceived; }
	float			GetPowerWanted( void ) { return powerWanted; }
	void			SetPowerWanted( float powerWanted_ ) { powerWanted = powerWanted_; }
	// The named local is (invented): retail's callers copy the setting through
	// fld/fstp, which needs a second copy level here; an inner inline, a named
	// local or a value wrapper give the same code, so its spelling is unknown.
	float			GetPowerPercentageWanted( void ) { float percentage = powerPercentageWanted; return percentage; }
	// The two named locals are (invented): ImpulseEngineSubsystem::Update copies
	// the percentage through fld/fstp into its spill before each call, which
	// needs two copy levels here (one gives integer moves); the spelling is unknown.
	float			GetPowerPercentage( void ) { float percentage = powerPercentage; float value = percentage; return value; }
	float			GetNormalPowerPercentage( void ) { return normalPowerPercentage; }
	bool			IsOn( void ) { return on; }
	void			Turn( int on_ ) { if ( on_ ) { TurnOn(); } else { TurnOff(); } }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			StateChangedHandler( TGEvent *event );

protected:
	float			powerReceived;			// 0x88
	float			powerWanted;			// 0x8C
	float			powerPercentageWanted;	// 0x90
	float			powerPercentage;		// 0x94
	float			normalPowerPercentage;	// 0x98
	bool			on;						// 0x9C
	int				powerMode;				// 0xA0 a powerMode_t: where Update draws its power from (invented name)
	bool			unknownA4;				// 0xA4 in multiplayer, TurnOn and TurnOff also send ET_SUBSYSTEM_STATE_CHANGED_NOTIFY when set

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PoweredSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005622a0
inline bool PoweredSubsystem::IsTypeOf( int type ) {
	if ( type == POWEREDSUBSYSTEM_TYPE ) {
		return true;
	}
	return ShipSubsystem::IsTypeOf( type );
}

#endif /* !__POWEREDSUBSYSTEM_H__ */
