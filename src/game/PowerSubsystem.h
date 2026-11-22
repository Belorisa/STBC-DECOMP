/*
===============================================================================

	PowerSubsystem.h

	The ship's power plant: the power it makes, its main and backup
	batteries, and the subsystems it powers.

	0xDC bytes: the SWIG wrapper PowerSubsystem_Create allocates that much.

	Evidence: the constructor (0x563530) calls ShipSubsystem's, builds the
	two battery watchers at 0x88 and 0x94 (pointed at 0xB0 and 0xB8) and the
	list at 0xC4 (an NiTList with two items per block, inline), stores the
	vtable 0x88A1F0 and clears the members; 0xC0 takes the game clock less a
	constant. The SWIG wrappers PowerSubsystem_* read and write the available
	power (0xA0) and the battery levels (0xAC, 0xB4) inline, read the power
	dispensed (0xBC), and return the watchers. The vtable overrides 17 of
	ShipSubsystem's 30 slots and adds none; the type functions (slots 1, 2,
	9 to 11 and 19, 0x440130 to 0x4401C0) are inline, emitted with the
	vtable in another unit. Update (slot 25, 0x563780) runs the power plant
	once every POWER_UPDATE_INTERVAL seconds of game time, from 0xC0.

===============================================================================
*/

#ifndef __POWERSUBSYSTEM_H__
#define __POWERSUBSYSTEM_H__

#include "ShipSubsystem.h"
#include "FloatRangeWatcher.h"
#include "NiTList.h"

class PowerProperty;
class PoweredSubsystem;
class TGObject;

// GetObjType's value for a PowerSubsystem.
#define POWERSUBSYSTEM_TYPE		0x8022

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PowerSubsystem : public ShipSubsystem {
public:
					PowerSubsystem( void );
	// FUNCTION: STBC 0x00440090 ??0PowerSubsystem@@QAE@J@Z @game/UtopiaModule
					PowerSubsystem( long id ) : ShipSubsystem( id ), poweredSubsystems( 2 ) {}	// the loader's: the stream fills the members
	virtual			~PowerSubsystem( void );

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
	virtual void	SetParentShip( ShipClass *ship );
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );
	virtual void	VirtualSlot28( TGStream *stream, bool );
	virtual void	VirtualSlot29( TGStream *stream, float time );

	static PowerSubsystem *	Create( void ) { return new PowerSubsystem; }
	static PowerSubsystem *	Cast( TGObject * );
	PowerProperty *	GetProperty( void );
	float	GetPowerOutput( void );
	float	GetMainBatteryLimit( void );
	float	GetBackupBatteryLimit( void );
	float	GetMaxMainConduitCapacity( void );
	float	GetMainConduitCapacity( void );
	float	GetBackupConduitCapacity( void );
	void	AddPower( float );
	float	DeductPower( float );
	float	StealPower( float );
	float	StealPowerFromReserve( float );
	float	GetPowerWanted( float );

	// The power the batteries give over a number of turns (invented name).
	float	DrawBatteryPower( float turns );

	// Adds the subsystem to those this one powers (invented name).
	void	AddPoweredSubsystem( PoweredSubsystem *subsystem );
	// Takes the subsystem off those this one powers; unreferenced (invented name).
	void	RemovePoweredSubsystem( PoweredSubsystem *subsystem );

	float			GetAvailablePower( void ) { return availablePower; }
	void			SetAvailablePower( float availablePower_ ) { availablePower = availablePower_; }
	// The named local is (invented): retail's callers copy the charge through
	// fld/fstp, which needs a second copy level here; an inner inline, a named
	// local or a value wrapper give the same code, so its spelling is unknown.
	// FUNCTION: STBC 0x00522000 ?GetMainBatteryPower@PowerSubsystem@@QAEMXZ @game/STFillGauge
	float			GetMainBatteryPower( void ) { float power = mainBatteryPower; return power; }
	void			SetMainBatteryPower( float mainBatteryPower_ ) { mainBatteryPower = mainBatteryPower_; }
	// The named local is (invented), as GetMainBatteryPower's: retail's callers
	// (OptimizedManagePower::SetPowerLevel) copy the level through fld/fstp.
	// FUNCTION: STBC 0x00522010 ?GetBackupBatteryPower@PowerSubsystem@@QAEMXZ @game/STFillGauge
	float			GetBackupBatteryPower( void ) { float power = backupBatteryPower; return power; }
	void			SetBackupBatteryPower( float backupBatteryPower_ ) { backupBatteryPower = backupBatteryPower_; }
	FloatRangeWatcher *	GetMainBatteryWatcher( void ) { return &mainBatteryWatcher; }
	FloatRangeWatcher *	GetBackupBatteryWatcher( void ) { return &backupBatteryWatcher; }
	float			GetPowerDispensed( void ) { return powerDispensed; }
	// The subsystems it powers (invented name).
	NiTList<PoweredSubsystem *> &	GetPoweredSubsystems( void ) { return poweredSubsystems; }

protected:
	FloatRangeWatcher	mainBatteryWatcher;		// 0x88 watches mainBatteryPercentage
	FloatRangeWatcher	backupBatteryWatcher;	// 0x94 watches backupBatteryPercentage
	float			availablePower;			// 0xA0
	float			mainConduitPower;		// 0xA4 what the main battery gives this turn, at most its conduit's capacity (invented name)
	float			backupConduitPower;		// 0xA8 the same for the backup battery (invented name)
	float			mainBatteryPower;		// 0xAC
	float			mainBatteryPercentage;	// 0xB0 the main battery watcher's variable (invented name)
	float			backupBatteryPower;		// 0xB4
	float			backupBatteryPercentage;	// 0xB8 the backup battery watcher's variable (invented name)
	float			powerDispensed;			// 0xBC
	float			lastPowerUpdateTime;	// 0xC0 the game time of the last turn of the power plant (invented name)
	NiTList<PoweredSubsystem *>	poweredSubsystems;	// 0xC4 the subsystems it powers (invented name)

	static const float	POWER_UPDATE_INTERVAL;	// seconds of game time between turns of the power plant (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PowerSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x00440130
inline int PowerSubsystem::GetObjType( void ) {
	return POWERSUBSYSTEM_TYPE;
}

/*
================
PowerSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x00440140
inline bool PowerSubsystem::IsTypeOf( int type ) {
	if ( type == POWERSUBSYSTEM_TYPE ) {
		return true;
	}
	return ShipSubsystem::IsTypeOf( type );
}

/*
================
PowerSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x00440190
inline char *PowerSubsystem::GetClassName( void ) {
	return "PowerSubsystem";
}

/*
================
PowerSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004401a0
inline char *PowerSubsystem::GetSwigTypeName( void ) {
	return "_p_PowerSubsystem";
}

/*
================
PowerSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004401b0
inline char *PowerSubsystem::GetPythonClassName( void ) {
	return "PowerSubsystemPtr";
}

/*
================
PowerSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004401c0
inline TGEventHandlerTable *PowerSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__POWERSUBSYSTEM_H__ */
