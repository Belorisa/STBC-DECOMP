/*
===============================================================================

	TorpedoTube.h

	A torpedo tube: how many torpedoes it has ready, when it last fired, and
	whether it fires skewed.

	0xB0 bytes: the SWIG wrapper TorpedoTube_Create allocates that much.

	Evidence: the constructor (0x57C4B0) calls Weapon's, stores the vtable
	0x893630 (36 slots, none new), sets the last fire time (0xA4) to -1000
	and clears the members. The SWIG wrappers TorpedoTube_* read and write
	the number ready (0xA0), the last fire time and the skew fire flag
	(0xA8) inline; TorpedoSystem::IsTubeReady (0x57B710) reads the number
	ready of its child tube. SetProperty allocates one reload time per
	torpedo the tube can hold ready (0xAC), each -1 at first; the
	destructor frees them.

===============================================================================
*/

#ifndef __TORPEDOTUBE_H__
#define __TORPEDOTUBE_H__

#include "Weapon.h"

class NiPoint3;
class TGObject;
class TGPoint3;
class Torpedo;
class TorpedoTubeProperty;

// The flags of a CREATE_TORP_MESSAGE.
enum {
	CREATE_TORP_PLAY_SOUND	= 0x01,
	CREATE_TORP_SKEW_FIRE	= 0x02,
	CREATE_TORP_NO_TARGET	= 0x04
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TorpedoTube : public Weapon {
public:
					TorpedoTube( long id = 0 );
	virtual			~TorpedoTube( void );

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
	virtual void	SetParentSubsystem( ShipSubsystem *parent );
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );

	// Weapon
	virtual bool	Fire( float, bool );
	virtual bool	FireDumb( float, bool );
	virtual bool	CanFire( void );
	virtual TGPoint3	CalculateRoughDirection( void );
	virtual float	CalculateWeaponAppeal( void );

	static TorpedoTube *	Create( void ) { return new TorpedoTube; }
	static TorpedoTube *	Cast( TGObject * );

	// Register the class's handler methods by name, and add them to its table (the class has none).
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	TorpedoTubeProperty *	GetProperty( void );
	float	GetImmediateDelay( void );
	float	GetReloadDelay( void );
	int	GetMaxReady( void );
	void	ReloadTorpedo( void );
	void	UnloadTorpedo( void );
	void	ClearReloadTimes( void );							// invented name
	TGPoint3	GetDirection( void );
	TGPoint3	GetRight( void );
	bool	CanHit( NiPoint3 );
	bool	IsInArc( NiPoint3 );
	Torpedo *	CreateTorpedo( bool playSound );					// invented name
	void	SendTorpedoFiredEvent( Torpedo *torpedo );			// invented name
	// Tells the other players of the torpedo (invented name, after CREATE_TORP_MESSAGE).
	void	SendCreateTorpMessage( Torpedo *torpedo, bool playSound, bool dumbFire );
	// The torpedo another player fired, from a CREATE_TORP_MESSAGE
	// (MultiplayerGame::ReceiveCreateTorpMessage): it leaves the tube
	// along the direction at the launch speed and homes on the target's
	// point unless there is none (invented names).
	Torpedo *	CreateNetTorpedo( bool skewFire, bool playSound, bool noTarget, int playerID, long targetID, const NiPoint3 &offset, const NiPoint3 &direction );

	int				GetNumReady( void ) { return numReady; }
	void			SetNumReady( int numReady_ ) { numReady = numReady_; }
	void			IncNumReady( void ) { numReady++; }
	void			DecNumReady( void ) { numReady--; }
	float			GetLastFireTime( void ) { return lastFireTime; }
	void			SetLastFireTime( float lastFireTime_ ) { lastFireTime = lastFireTime_; }
	bool			IsSkewFire( void ) { return skewFire; }
	void			SetSkewFire( bool skewFire_ ) { skewFire = skewFire_; }

protected:
	int				numReady;				// 0xA0
	float			lastFireTime;			// 0xA4 -1000 at first
	bool			skewFire;				// 0xA8
	float *			reloadTimes;			// 0xAC one per torpedo the tube can hold ready, -1 when not reloading (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name

public:
	// A skewed tube's torpedo starts homing this many seconds after launch
	// (invented name).
	static const unsigned char	SKEW_HOMING_DELAY;
};

// A skewed tube's torpedo leaves along the tube's direction plus this much
// of its right (invented name).
#define SKEW_OFFSET				0.033f

// The priority of a launch sound: higher for the player's ship (invented names).
#define LAUNCH_PRIORITY			0.6f
#define PLAYER_LAUNCH_PRIORITY	0.9f

// The least time between the shots of two tubes of a system, in seconds (invented name).
#define TUBE_FIRE_INTERVAL		0.5f

// A tube reloaded a torpedo (the SWIG constant ET_TORPEDO_RELOAD).
#define ET_TORPEDO_RELOAD		0x800065

// A tube launched a torpedo (the SWIG constant ET_TORPEDO_FIRED).
#define ET_TORPEDO_FIRED		0x800066

// GetObjType's value for a TorpedoTube.
#define TORPEDOTUBE_TYPE		0x802F

/*
================
TorpedoTube::IsTypeOf
================
*/
// FUNCTION: STBC 0x0057c500
inline bool TorpedoTube::IsTypeOf( int type ) {
	if ( type == TORPEDOTUBE_TYPE ) {
		return true;
	}
	return Weapon::IsTypeOf( type );
}

#endif /* !__TORPEDOTUBE_H__ */
