/*
===============================================================================

	TorpedoSystem.h

	The ship's torpedo tubes, as one weapon system: the torpedo types it
	carries, how many of each are left, and the type it loads.

	0x11C bytes: the SWIG wrapper TorpedoSystem_Create allocates that much.

	Evidence: the constructor (0x57B020) calls WeaponSystem's, stores the
	vtable 0x893598, sets 0xF0 to -1000 and clears the members.
	GetNumAvailableTorpsToType (0x57B740) reads the count at 0xF4 for types
	0 to 3. The SWIG wrappers TorpedoSystem_* read the property (0x18), the
	ammo types (0x104), the current type (0x114) and the number ready
	(0x118) inline.

===============================================================================
*/

#ifndef __TORPEDOSYSTEM_H__
#define __TORPEDOSYSTEM_H__

#include "WeaponSystem.h"
#include "TorpedoAmmoType.h"

class TGObject;
class TorpedoAmmoType;
class TorpedoSystemProperty;

// The type TorpedoSystem::Cast (0x57AFF0) checks.
#define TORPEDOSYSTEM_TYPE		0x801E

// The player's torpedo system used a torpedo (the SWIG constant ET_PLAYER_TORPEDO_COUNT_CHANGED).
#define ET_PLAYER_TORPEDO_COUNT_CHANGED	0x800067

// The torpedo type of the player's torpedo system changed (the SWIG constant ET_PLAYER_TORPEDO_TYPE_CHANGED).
#define ET_PLAYER_TORPEDO_TYPE_CHANGED	0x800068

// A torpedo system's type changed (the SWIG constant ET_NET_TORPEDO_TYPE_CHANGED), and the
// notification the host sends the other players (ET_NET_TORPEDO_TYPE_CHANGED_NOTIFY).
#define ET_NET_TORPEDO_TYPE_CHANGED			0x8000FD
#define ET_NET_TORPEDO_TYPE_CHANGED_NOTIFY	0x8000FE

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TorpedoSystem : public WeaponSystem {
public:
					TorpedoSystem( long id = 0 );
	virtual			~TorpedoSystem( void );

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
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ShipSubsystem
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );

	// WeaponSystem
	virtual void	SetGroupFireMode( int mode );

	static TorpedoSystem *	Create( void ) { return new TorpedoSystem; }
	static TorpedoSystem *	Cast( TGObject * );
	TorpedoSystemProperty *	GetProperty( void ) { return (TorpedoSystemProperty *)property; }
	TorpedoAmmoType *	GetCurrentAmmoType( void ) { return ammoTypes[currentAmmoType]; }
	int				GetCurrentAmmoTypeNumber( void ) { return currentAmmoType; }
	TorpedoAmmoType *	GetAmmoType( int type ) { return ammoTypes[type]; }
	int				GetNumReady( void ) { return numReady; }
	int				GetNumAvailableTorps( int type ) { return numAvailableTorps[type]; }	// invented name
	int				GetAmmoTypeNumber( void ) { return currentAmmoType; }
	float			GetLastFireTime( void ) { return lastFireTime; }			// invented name
	void			SetLastFireTime( float lastFireTime_ ) { lastFireTime = lastFireTime_; }	// invented name
	void	SetAmmoType( int, bool );
	void	SetSkewFire( bool );
	void	RemoveAmmoType( int );
	void	FillAmmoType( int );
	int	LoadAmmoType( int, int );
	bool	IsTubeReady( int );
	int	GetNumAvailableTorpsToType( int );
	void	UseTorpedo( void );			// invented name
	void	AddTorpedo( void );			// invented name
	void	IncNumReady( void );		// invented name
	void	DecNumReady( void );		// invented name

	// Event handler (registered as "TorpedoSystem::TorpedoTypeChangedHandler"; invented name).
	void	TorpedoTypeChangedHandler( TGEvent *event );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

protected:
	float			lastFireTime;			// 0xF0 when a tube last fired, -1000 at first (invented name)
	int				numAvailableTorps[MAX_AMMO_TYPES];	// 0xF4 per type (invented name)
	TorpedoAmmoType *	ammoTypes[MAX_AMMO_TYPES];	// 0x104
	int				currentAmmoType;		// 0x114
	int				numReady;				// 0x118

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TorpedoSystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x0057b0a0
inline bool TorpedoSystem::IsTypeOf( int type ) {
	if ( type == TORPEDOSYSTEM_TYPE ) {
		return true;
	}
	return WeaponSystem::IsTypeOf( type );
}

#endif /* !__TORPEDOSYSTEM_H__ */
