/*
===============================================================================

	Torpedo.h

	A torpedo or disruptor shot in flight: a physics object that homes on
	its target for a while, lights the ships it passes with its glow, and is
	the payload of the weapon that fired it.

	0x170 bytes, allocated from the pools (the scalar deleting destructor
	0x578630 frees through the pool of that size).

	Evidence: the constructor (0x5783D0) calls PhysicsObjectClass's and
	WeaponPayload's (at 0x108), stores the vtables 0x893458 (80 slots, all
	PhysicsObjectClass's) and 0x89344C (WeaponPayload's three, the adjustor
	thunks 0x57ADB0-0x57ADD0) and sets the members; the destructor
	(0x578660) frees the module name. The stream functions (0x57A280,
	0x57A400) give the order of the saved members. The SWIG wrappers name
	the members they read and write inline (GetModuleName 0x12C,
	Get/SetParent 0x128, SetTarget and GetTargetID 0x118, SetTargetOffset
	0x11C, Get/SetMaxAngularAccel 0x134, Get/SetLifetime 0x138,
	GetGuidanceLifeTime 0x13C, SetGuidanceLifetime 0x13C and 0x140,
	Get/SetDamage 0x144, Get/SetPlayerID 0x14C).

	The target is two members, not a WeaponSystemTarget: the constructor
	sets only the ID and leaves the offset unset, where an embedded
	WeaponSystemTarget would run its constructor (Weapon's does, for its
	target at 0x8C); the wrappers set the ID and the offset separately
	(Torpedo_SetTarget takes a long, Torpedo_SetTargetOffset a point).

===============================================================================
*/

#ifndef __TORPEDO_H__
#define __TORPEDO_H__

#include "PhysicsObjectClass.h"
#include "WeaponPayload.h"
#include "WeaponSystemTarget.h"

class BaseObjectClass;
class MultiplayerPlayer;
class NiPointLight;
class ObjectClass;
class SetClass;
class TGColorA;
class TGObject;
class TGPoint3;
class TGEvent;
class TGMessage;
class TGStream;

// GetObjType's value for a Torpedo.
#define TORPEDO_TYPE			0x8009

// A torpedo's homing delay has passed (the App constant).
#define ET_TORPEDO_START_HOMING	0x800086

// The adjustor thunks of WeaponPayload's vtable in a Torpedo:
// FUNCTION: STBC 0x0057adb0 ?WriteToStream@Torpedo@@WBAI@AE_NPAVTGStream@@@Z
// FUNCTION: STBC 0x0057adc0 ?ReadFromStream@Torpedo@@WBAI@AE_NPAVTGStream@@@Z
// FUNCTION: STBC 0x0057add0 ??_ETorpedo@@WBAI@AEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class Torpedo : public PhysicsObjectClass, public WeaponPayload {
public:
					Torpedo( long id = 0 );
	virtual			~Torpedo( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// also WeaponPayload's
	virtual bool	ReadFromStream( TGStream *stream );		// also WeaponPayload's
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// PhysicsObjectClass
	virtual int		VirtualSlot69( TGStream *stream );
	virtual int		VirtualSlot70( TGStream *stream );
	virtual int		VirtualSlot71( TGStream *stream );
	virtual TGMessage *	VirtualSlot72( MultiplayerPlayer *player );
	virtual float	TurnTowardOrientation( TGPoint3 *forward, TGPoint3 *up );

	// A torpedo from its script, at the position; it starts homing on its
	// target after the delay, in seconds (the SWIG wrapper Torpedo_Create).
	static Torpedo *	Create( char *moduleName, const TGPoint3 &position, unsigned char homingDelay );
	static Torpedo *	Cast( TGObject * );
	void	CreateDisruptorModel( TGColorA *, TGColorA *, float, float, int, float, float );
	void	CreateTorpedoModel( char *, TGColorA *, float, float, char *, TGColorA *, float, float, float, char *, TGColorA *, int, float, float );
	float	GetLaunchSpeed( void );
	char *	GetLaunchSound( void );
	void	DetectCollision( BaseObjectClass * );
	static Torpedo *	GetObject( SetClass *, char * );
	static Torpedo *	GetObjectByID( SetClass *, long );
	static int	GetNumClassObjects( SetClass * );
	// The set's torpedoes and their number; NULL and 0 when there are none.
	static Torpedo **	GetClassObjects( SetClass *set, int *count );		// invented name
	// The script's GetPowerCost (no SWIG wrapper calls it; invented name).
	float	GetPowerCost( void );
	// Steers the torpedo toward its target for the elapsed time (invented name).
	void	Unknown578CB0( float elapsed );
	// The hit on a planet DetectCollision passes on: sends the planet a weapon hit
	// event and runs Effects.TorpedoHullHit (invented name).
	void	Unknown5791D0( ObjectClass *planet );
	// Turns a disruptor shot's model along its flight (invented name).
	void	Unknown578800( void );
	// Turns the torpedo toward the orientation; TurnTowardOrientation passes
	// it copies of its points (invented name).
	float	Unknown579530( NiPoint3 *forward, NiPoint3 *up );
	// Torpedo's copies of ShipClass::TurnTowardDifference and
	// SetLimitedAngularAccelerationLinear, bounded by the torpedo's own
	// maximum angular acceleration (names invented, after ShipClass's).
	float	TurnTowardDifference( NiPoint3 *difference );
	void	SetLimitedAngularAccelerationLinear( const NiPoint3 &acceleration_, const NiPoint3 &change, float time, int directionType );
	// Target the named object of the torpedo's set; no target when there is none (no callers; invented name).
	void	SetTargetName( char *name );
	// Target the object; no target without one (no callers; invented name).
	void	SetTargetObject( ObjectClass *object );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	void			HandleStartHomingEvent( TGEvent *event );

	char *			GetModuleName( void ) { return moduleName; }
	void			SetTarget( long targetID_ ) { targetID = targetID_; }
	long			GetTargetID( void ) { return targetID; }
	void			SetTargetOffset( const NiPoint3 &offset ) { targetOffset = offset; }
	// The weapon's target and the point it aims at (invented name).
	void			SetTarget( const WeaponSystemTarget &target ) { targetID = target.targetID; targetOffset = target.offset; }
	// The firing object; nothing changes without one (the parameter type is unproven).
	void	SetParent( ObjectClass *parent );
	void			SetParent( long parentID_ ) { parentID = parentID_; }
	long			GetParentID( void ) { return parentID; }
	float			GetMaxAngularAccel( void ) { return maxAngularAccel; }
	void			SetMaxAngularAccel( float maxAngularAccel_ ) { maxAngularAccel = maxAngularAccel_; }
	float			GetLifetime( void ) { return lifetime; }
	void			SetLifetime( float lifetime_ ) { lifetime = lifetime_; }
	static float	GetMaxLifetime( void ) { return 60.0f; }
	float			GetGuidanceLifeTime( void ) { return guidanceLifetime; }
	void			SetGuidanceLifetime( float guidanceLifetime_ ) { guidanceLifetime = guidanceLifetime_; guidanceTimeLeft = guidanceLifetime_; }
	float			GetDamage( void ) { return damage; }
	void			SetDamage( float damage_ ) { damage = damage_; }
	int				GetPlayerID( void ) { return playerID; }
	void			SetPlayerID( int playerID_ ) { playerID = playerID_; }

protected:
	long			targetID;			// 0x118 the object the torpedo homes on; 0 for none
	NiPoint3		targetOffset;		// 0x11C the point aimed at, in the target's frame; not set by the constructor
	long			parentID;			// 0x128 the firing object's ID
	char *			moduleName;			// 0x12C owned; the script module Create copies, whose functions give the launch speed and sound
	bool			isDisruptor;		// 0x130 the disruptor model, which Update turns along the flight (invented name)
	float			maxAngularAccel;	// 0x134 0.125 at first
	float			lifetime;			// 0x138 seconds left; 60 at first
	float			guidanceLifetime;	// 0x13C seconds of guidance; 4 at first
	float			guidanceTimeLeft;	// 0x140 seconds of guidance left; 4 at first (invented name)
	float			damage;				// 0x144 100 at first
	unsigned char	homingDelay;		// 0x148 seconds before the torpedo homes (invented name)
	int				playerID;			// 0x14C the firing player's network ID, or the firing ship's ID outside multiplayer
	NiPoint3		lastLocation;		// 0x150 the world location before the last Update (invented name)
	NiPoint3		unknown15C;			// 0x15C
	NiPointLight *	light;				// 0x168 the torpedo model's glow, which lights the ships near it (invented name)
	bool			homing;				// 0x16C true at first; false until the homing delay has passed (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__TORPEDO_H__ */
