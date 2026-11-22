/*
===============================================================================

	WeaponsDisplay.h

	The tactical window's weapons display: the ship icon with its weapons,
	coloured by their state, and the ship's speed in the window's name.

	Evidence: the constructors (0x54AEA0 with a size, 0x558B10 with an ID)
	call STStylizedWindow's and store the vtable 0x8923F8 (83 slots,
	STStylizedWindow's count); it overrides the destructor, GetObjType
	(0x8102), IsTypeOf, the stream slots, the three class strings and
	GetClassHandlers. The destructor (0x54B070) frees the list at 0xAC
	after deleting the refresh timer (0xC8).

===============================================================================
*/

#ifndef __WEAPONSDISPLAY_H__
#define __WEAPONSDISPLAY_H__

#include "STStylizedWindow.h"
#include "NiTList.h"

class TGIcon;
class EnergyWeapon;
class WeaponSystem;

class ShipClass;
class ShipSubsystem;
class TGPane;
class TGObject;

// GetObjType's value for a WeaponsDisplay (the type its Cast checks).
#define WEAPONSDISPLAY_TYPE		0x8102

// The event of the display's refresh timer (the SWIG constant).
#define ET_WEAPON_DISPLAY_REFRESH_TIMER	0x80006A
// The event of an energy weapon's charge watcher (the SWIG constant).
#define ET_WEAPON_CHARGE_UPDATED		0x800069

// The charge range checks a display adds to an energy weapon's charge
// watcher, to remove them again (invented name): nearly empty and every
// twentieth of the charge.
struct WeaponChargeChecks {
	EnergyWeapon *	weapon;				// 0x00
	int				numChecks;			// 0x04 21
	int *			checkIDs;			// 0x08 numChecks IDs, from FloatRangeWatcher::AddRangeCheck
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0054b040 ??_GWeaponsDisplay@@UAEPAXI@Z
class WeaponsDisplay : public STStylizedWindow {
public:
					WeaponsDisplay( float width, float height );
	// The script-side constructor (the SWIG wrapper WeaponsDisplay_Create expands it).
	static WeaponsDisplay *	Create( float width, float height ) { return new WeaponsDisplay( width, height ); }
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558b10 ??0WeaponsDisplay@@QAE@J@Z @game/InterfaceModule
					WeaponsDisplay( long id ) : STStylizedWindow( id ), chargeChecks( 2 ) {}
	virtual			~WeaponsDisplay( void );

	// TGObject
	// FUNCTION: STBC 0x0054af80 ?GetObjType@WeaponsDisplay@@UAEHXZ
	virtual int		GetObjType( void ) { return WEAPONSDISPLAY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0054b000 ?GetClassName@WeaponsDisplay@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WeaponsDisplay"; }
	// FUNCTION: STBC 0x0054b010 ?GetSwigTypeName@WeaponsDisplay@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WeaponsDisplay"; }
	// FUNCTION: STBC 0x0054b020 ?GetPythonClassName@WeaponsDisplay@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WeaponsDisplayPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0054b030 ?GetClassHandlers@WeaponsDisplay@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	long	GetShipID( void ) { return shipID; }
	static WeaponsDisplay *	Cast( TGObject * );
	void	SetShipID( long );
	void	Unknown54C110( ShipClass *ship );
	void	Unknown54C200( ShipClass *ship );
	void	Unknown54BEE0( ShipClass *ship );
	void	Unknown54BFD0( ShipSubsystem *system, TGPane *above, TGPane *below );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	Update( TGEvent *event );				// handler "WeaponsDisplay::Update"
	void	HandleTargetChanged( TGEvent *event );	// handler "WeaponsDisplay::HandleTargetChanged"
	void	PeriodicUpdate( TGEvent *event );		// handler "WeaponsDisplay::PeriodicUpdate"
protected:
	// Builds the display's panes in Python: Tactical.Interface.WeaponsDisplay's
	// Create (invented name).
	void	CreateInterface( float width, float height );
	// Listens for the player ship's weapon events and watches its energy
	// weapons' charge (invented name).
	void	WatchShip( void );
	// Undoes WatchShip for the ship of shipID (invented name).
	void	StopWatchingShip( void );
	// Watches the charge of the system's energy weapons (invented names).
	void	WatchWeaponSystem( ShipSubsystem *system );
	void	WatchWeaponCharge( EnergyWeapon *weapon );
	// Sets the Python ship icon (Tactical.Interface.WeaponsDisplay's
	// SetShipIcon; invented name).
	void	SetShipIcon( void );
	// Starts the timer of the next PeriodicUpdate (invented name).
	void	StartRefreshTimer( void );
	// Marks the phasers and pulse weapons that can hit the ship's target,
	// in the icon panes and the tactical window (invented names).
	void	UpdateTargeting( ShipClass *ship );
	void	UpdateWeaponTargeting( WeaponSystem *system, TGPane *above, TGPane *below );

	static TGEventHandlerTable	classHandlers;	// invented name

	NiTList<WeaponChargeChecks *>	chargeChecks;	// 0xAC one per watched energy weapon (invented name)
	long			shipID;					// 0xC4 (WeaponsDisplay_GetShipID, SetShipID)
	long			refreshTimerID;			// 0xC8 the timer of the next PeriodicUpdate (invented name)
};

#endif /* !__WEAPONSDISPLAY_H__ */
