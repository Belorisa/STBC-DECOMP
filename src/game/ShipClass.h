/*
===============================================================================

	ShipClass.h

	A ship: a damageable object with subsystems, a target, an alert level,
	scripts, and the impulse and turning state the AI drives.

	0x328 bytes, allocated from the pools (Create 0x5AB930, and the scalar
	deleting destructor 0x5ABFE0 frees through the pool of that size).

	Evidence: the constructors (0x5ABDC0 from a model name, 0x5AC010 by
	ID) call DamageableObject's, construct the list at 0x264 and the
	subsystems at 0x27C, store the vtable 0x894340 (92 slots; 90 and 91
	are new) and call InitObject (0x5AB970), which sets the members from
	0x1E4 on. The destructor is 0x5AC0A0. GetObjType returns 0x8008, the
	type Cast checks. The handler names are strings (0x8E5FF8-0x8E615C).
	The SWIG wrappers name slots 50 (IsTargetable), 51 (CanTargetObject),
	75 (TurnTowardOrientation) and 91 (SetAcceleration) through the calls
	they make, and the members they read or write inline: GetScript 0x2F0,
	GetDeathScript 0x2F4, IsTargetable 0x2E8, IsInvincible 0x2E9,
	IsHurtable 0x2EA, Set/IsDestroyBrokenSystems 0x2EB, the subsystems
	0x2B0-0x2DC, GetShipProperty 0x2E0, GetImpulse 0x1F8,
	SetDisabledEngineDeceleration 0x20C, IsDoingInSystemWarp 0x210,
	Set/IsDocked 0x1E6, Set/GetNetPlayerID 0x2E4, Get/SetAffiliation 0x234,
	GetAlertLevel 0x238, DisableCollisionDamage and
	IsCollisionDamageDisabled 0x2EC, IncrementAIDoneIgnore 0x2F8.

===============================================================================
*/

#ifndef __SHIPCLASS_H__
#define __SHIPCLASS_H__

#include "DamageableObject.h"
#include "ShipSubsystems.h"

class MultiplayerPlayer;
class ObjectClass;
class SetClass;
class ShipProperty;
class ShipSubsystem;
class SubsystemProperty;
class TGEvent;
class TGMessage;
class TGObject;
class TractorBeamProjector;
class WeaponSystem;

// GetObjType's value for a ShipClass.
#define SHIPCLASS_TYPE		0x8008

// Event types, as the SWIG constants of the App module give them.
#define ET_AI_DONE							0x800016
#define ET_AI_DORMANT						0x800017
#define ET_SET_ALERT_LEVEL					0x800038
#define ET_PLANET_COLLISION					0x800052
#define ET_ENVIRONMENT_DAMAGE				0x800057
#define ET_CLOAKED_COLLISION				0x800053
#define ET_WEAPON_HIT						0x800064
#define ET_TRACTOR_BEAM_STARTED_HITTING		0x80007E
#define ET_TRACTOR_BEAM_STOPPED_HITTING		0x800080
#define ET_SET_TARGET						0x8000E1
#define ET_TARGET_WAS_CHANGED				0x800058
#define ET_TARGET_OFFSET_CHANGED			0x800059
#define ET_TARGETED_SUBSYSTEM_CHANGED		0x80005A
#define ET_START_WARP						0x8000ED
#define ET_EXITED_WARP						0x8000C5	// a ship has left warp
// A TGBoolEvent to the ship itself: true when an in-system warp starts,
// false when it stops.
#define ET_IN_SYSTEM_WARP					0x8000EF

// In-system warp: the speed, the clearance kept from other objects' bounds
// (and the least distance worth warping), and the cosine of the largest
// angle off the ship's heading it warps at without turning first (15
// degrees). Invented names.
#define IN_SYSTEM_WARP_SPEED			75.0f
#define IN_SYSTEM_WARP_CLEARANCE		50.0f
#define IN_SYSTEM_WARP_FACING			0.96592583f

// The least time Update moves the ship over, in seconds (invented name).
#define MIN_UPDATE_INTERVAL		0.001f

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WeaponHitEvent;
class ShieldPatchParticles;
class AtmosphereEffect;

class ShipClass : public DamageableObject {
	// ShieldClass::Unknown56A690 reads the box (boxExtent, boxCenter).
	friend class ShieldClass;
public:
	// A walk over the ship's subsystems of one type (the SWIG type
	// ShipClass::MatchIterator): StartGetSubsystemMatch allocates it,
	// GetNextSubsystemMatch advances it and EndGetSubsystemMatch frees it.
	struct MatchIterator {
		int				type;				// 0x00 the type IsTypeOf checks
		NiTListIterator	pos;				// 0x04 the next item of the subsystem list
	};

	// The alert levels, as the SWIG constants ShipClass_GREEN_ALERT,
	// ShipClass_YELLOW_ALERT and ShipClass_RED_ALERT give them.
	enum alertLevel_t {
		GREEN_ALERT,
		YELLOW_ALERT,
		RED_ALERT
	};

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

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	SetScale( float scale );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// ObjectClass
	// FUNCTION: STBC 0x005abf20 ?IsTargetable@ShipClass@@UAE_NXZ
	virtual bool	IsTargetable( void ) { return targetable; }
	virtual bool	CanTargetObject( ObjectClass *object );
	virtual void	VirtualSlot58( NiPoint3 *point1, NiPoint3 *point2 );
	virtual void	VirtualSlot66( void );

	// PhysicsObjectClass
	virtual int		VirtualSlot69( TGStream *stream );	// writes the object's state to a network message
	virtual int		VirtualSlot70( TGStream *stream );	// reads the net type from a network message
	virtual int		VirtualSlot71( TGStream *stream );	// reads the state VirtualSlot69 writes
	virtual TGMessage *	VirtualSlot72( MultiplayerPlayer *player );
	virtual void	VirtualSlot73( TGMessage *message );
	virtual void	SetupModel( char *modelName );
	virtual float	TurnTowardOrientation( TGPoint3 *forward, TGPoint3 *up );
	virtual int		SetAI( ArtificialIntelligence *ai, bool unknown1 = true, bool unknown2 = true );
	virtual void	ClearAI( bool unknown = true, ArtificialIntelligence *ai = NULL );

	// DamageableObject
	virtual bool	VirtualSlot80( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, bool ignoreShields );
	virtual bool	VirtualSlot81( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, float *damage, WeaponPayload *payload, TGObject *source, ObjectClass *firingObject );
	virtual bool	VirtualSlot82( DamageableObject *other, CollisionRecorder *recorder );
	virtual bool	VirtualSlot83( CollisionTest *other, CollisionRecorder *recorder );
	virtual void	VirtualSlot84( CollisionRecorder *recorder, DamageableObject *other, int index );
	virtual void	AddDamage( NiAVObject *, float, float );
	virtual void	VirtualSlot87( void );
	virtual void	VirtualSlot88( TGModelProperty *property );
	virtual void	VirtualSlot89( void );

	virtual void	VirtualSlot90( NiPoint3 requested );		// the parameter's meaning is open; a NiPoint3, not a TGPoint3: PlanetCollisionHandler passes it with no destructor to unwind
	// FUNCTION: STBC 0x005abf30 ?SetAcceleration@ShipClass@@UAEXVTGPoint3@@@Z
	virtual void	SetAcceleration( TGPoint3 acceleration ) { VirtualSlot90( acceleration ); }

					ShipClass( char *modelName );
					ShipClass( long id );
	virtual			~ShipClass( void );

	static ShipClass *	Cast( TGObject * );
	static ShipClass *	Create( char * );
	void	SetScript( char * );
	void	SetDeathScript( char * );
	void	RunDeathScript( void );
	void	SetTargetable( bool );
	void	SetInvincible( bool );
	void	SetHurtable( bool );
	WeaponSystem *	GetWeaponSystemGroup( int );
	int	GetWeaponGroupForSystem( WeaponSystem * );
	void	StopFiringWeapons( void );
	void	CompleteStop( void );
	MatchIterator *	StartGetSubsystemMatch( int type );
	ShipSubsystem *	GetNextSubsystemMatch( MatchIterator *iterator );
	// CompleteStop (0x5B0780) frees the iterator with operator delete inline.
	void			EndGetSubsystemMatch( MatchIterator *iterator ) { delete iterator; }
	// The last parameter's type is unproven (the SWIG format gives "i").
	void	SetImpulse( float impulse_, const NiPoint3 &direction, int directionType );
	// The TGPoint3 overloads (the scripts' and the AIs' points) pass a NiPoint3
	// copy of the point on: their callers copy it into a frame temporary and
	// read the float argument into a local first.
	void	SetImpulse( float impulse_, const TGPoint3 &direction, int directionType ) { SetImpulse( impulse_, (NiPoint3)direction, directionType ); }
	// The speed as a fraction of the impulse engines' current maximum.
	void	SetSpeed( float speed, const NiPoint3 &direction, int directionType );
	void	SetSpeed( float speed, const TGPoint3 &direction, int directionType ) { SetSpeed( speed, (NiPoint3)direction, directionType ); }
	void	SetTargetAngularVelocityDirect( const NiPoint3 &velocity );
	void	SetTargetAngularVelocityDirect( const TGPoint3 &velocity ) { SetTargetAngularVelocityDirect( (NiPoint3)velocity ); }
	void	AddSubsystem( ShipSubsystem * );
	ShipSubsystem *	GetSubsystemByProperty( SubsystemProperty * );
	bool	IsReverse( void );
	bool	InSystemWarp( ObjectClass *, float );
	void	StopInSystemWarp( void );
	float	TurnTowardLocation( const NiPoint3 &location );
	float	TurnDirectionsToDirections( const TGPoint3 &curForward, const TGPoint3 &newForward, const TGPoint3 &curUp, const TGPoint3 &newUp ) { return TurnDirectionsToDirections( (NiPoint3)curForward, (NiPoint3)newForward, (NiPoint3)curUp, (NiPoint3)newUp ); }
	float	TurnTowardLocation( const TGPoint3 &location ) { return TurnTowardLocation( (NiPoint3)location ); }
	float	TurnTowardDirection( const NiPoint3 &direction );
	float	TurnTowardDirection( const TGPoint3 &direction ) { return TurnTowardDirection( (NiPoint3)direction ); }
	// Turns the ship's forward and up directions toward newForward and
	// newUp (the SWIG wrapper ShipClass_TurnDirectionsToDirections); with
	// either up direction zero, only the forward direction is turned.
	float	TurnDirectionsToDirections( const NiPoint3 &curForward, const NiPoint3 &newForward, const NiPoint3 &curUp = NiPoint3::ZERO, const NiPoint3 &newUp = NiPoint3::ZERO );
	bool	IsCloaked( void );
	bool	IsPlayerShip( void );
	void	GetNextTarget( void );
	ObjectClass *	GetTarget( void );
	void	SetTarget( char * );
	void	SetTargetByID( long id );								// invented name
	void	SetTarget( ObjectClass *target, ShipSubsystem *subsystem );
	void	SetTargetSubsystem( ShipSubsystem * );
	ShipSubsystem *	GetTargetSubsystem( void );
	// The point aimed at on the target: the one UseTargetOffsetTG set, else
	// the targeted subsystem's damage point (SWIG GetTargetOffsetTG).
	const NiPoint3 &	GetTargetOffset( void );
	// FUNCTION: STBC 0x0060bce0 ?GetTargetOffsetTG@ShipClass@@QAE?AVTGPoint3@@XZ @game/AppWrap
	TGPoint3		GetTargetOffsetTG( void ) { return GetTargetOffset(); }
	// Aims the weapon systems at the offset (or, unused, at the targeted
	// subsystem again) and posts ET_TARGET_OFFSET_CHANGED.
	void			UseTargetOffsetTG( bool use, TGPoint3 *offset );
	float	DestroySystem( ShipSubsystem * );
	// Damages the subsystem by the amount (DestroySystem passes its maximum
	// condition); the meaning of the last two parameters is open.
	// Takes the damage off the subsystem's condition and returns what was
	// left over below zero; with destroy the subsystem is emptied. An
	// invincible ship keeps a critical subsystem at a hundredth of its
	// maximum, an invincible subsystem stays above its disabled level, and a
	// critical subsystem emptied kills the ship (Unknown5AFEA0). Nothing
	// happens to the player's ship in god mode or to a ship that is not
	// hurtable.
	float	DamageSystem( ShipSubsystem *system, float damage, TGObject *source, bool destroy );	// source: what did the damage, handed to Unknown5AFEA0 when the ship dies
	// The ship starts to die: it drops its target and stops firing, its
	// weapon systems are emptied, the death script runs and an
	// ET_OBJECT_EXPLODING event names the player whose weapon did it.
	void	Unknown5AFEA0( TGObject *source );
	void	Unknown5AF460( void );	// empties every weapon system's condition
	void	SetAlertLevel( int );
	static ShipClass *	GetObject( SetClass *, char * );
	static ShipClass *	GetObjectByID( SetClass *set, long id );
	static ShipClass **	GetClassObjects( SetClass *set, int *count );		// invented name
	static int	GetNumClassObjects( SetClass *set );

	char *			GetScript( void ) { return script; }
	char *			GetDeathScript( void ) { return deathScript; }
	bool			IsInvincible( void ) { return invincible; }
	bool			IsHurtable( void ) { return hurtable; }
	void			SetDestroyBrokenSystems( bool destroy ) { destroyBrokenSystems = destroy; }
	bool			IsDestroyBrokenSystems( void ) { return destroyBrokenSystems; }
	PowerSubsystem *	GetPowerSubsystem( void ) { return subsystems.powerSubsystem; }
	TorpedoSystem *	GetTorpedoSystem( void ) { return subsystems.torpedoSystem; }
	PhaserSystem *	GetPhaserSystem( void ) { return subsystems.phaserSystem; }
	PulseWeaponSystem *	GetPulseWeaponSystem( void ) { return subsystems.pulseWeaponSystem; }
	ShieldClass *	GetShields( void ) { return subsystems.shields; }
	HullClass *		GetHull( void ) { return subsystems.hull; }
	SensorSubsystem *	GetSensorSubsystem( void ) { return subsystems.sensorSubsystem; }
	ImpulseEngineSubsystem *	GetImpulseEngineSubsystem( void ) { return subsystems.impulseEngineSubsystem; }
	WarpEngineSubsystem *	GetWarpEngineSubsystem( void ) { return subsystems.GetWarpEngineSubsystem(); }
	TractorBeamSystem *	GetTractorBeamSystem( void ) { return subsystems.tractorBeamSystem; }
	RepairSubsystem *	GetRepairSubsystem( void ) { return subsystems.repairSubsystem; }
	CloakingSubsystem *	GetCloakingSubsystem( void ) { return subsystems.cloakingSubsystem; }
	// Every subsystem of the ship, in the order AddSubsystem added them (invented name).
	NiTList<ShipSubsystem *> &	GetSubsystemList( void ) { return subsystems.list; }
	ShipProperty *	GetShipProperty( void ) { return shipProperty; }
	// Takes the ship's affiliation, mass, rotational inertia and whether it
	// is stationary from the property (invented name; VirtualSlot88 calls it).
	void			SetShipProperty( ShipProperty *property );
	// Attaches a blinking light of a BlinkingLightProperty under the ship's
	// node (VirtualSlot88 calls it with each property).
	void			Unknown5B5280( TGModelProperty *property );
	float			GetImpulse( void ) { return impulse; }
	void			SetDisabledEngineDeceleration( float deceleration ) { disabledEngineDeceleration = deceleration; }
	bool			IsDoingInSystemWarp( void ) { return doingInSystemWarp; }
	void			SetDocked( bool docked_ ) { docked = docked_; }
	bool			IsDocked( void ) { return docked; }
	void			SetNetPlayerID( int id ) { netPlayerID = id; }
	int				GetNetPlayerID( void ) { return netPlayerID; }
	int				GetAffiliation( void ) { return affiliation; }
	void			SetAffiliation( int affiliation_ ) { affiliation = affiliation_; }
	int				GetAlertLevel( void ) { return alertLevel; }
	void			DisableCollisionDamage( bool disable ) { collisionDamageDisabled = disable; }
	bool			IsCollisionDamageDisabled( void ) { return collisionDamageDisabled; }
	void			IncrementAIDoneIgnore( void ) { aiDoneIgnore++; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	// Empty: called last by RegisterHandlerNames, and by RegisterHandlers
	// on a multiplayer client.
	static void		Unknown5AB910( void );
	static void		Unknown5AB920( void );

	void			WeaponHitHandler( TGEvent *event );
	// A hit on the shields lights a patch of the shield bubble at the hit,
	// in the shield glow color scaled by the hit's condition; a hit with no
	// condition left clears the patches facing it (invented name).
	void			ShowShieldHit( WeaponHitEvent *event );
	// Phaser and torpedo hits on the hull add a damage volume at the hit:
	// twice the hit's radius, half its damage (invented name).
	void			AddWeaponHitDamage( WeaponHitEvent *event );
	// The push and the damage of a weapon hit at the point, in the model
	// frame (invented name).
	void			ApplyWeaponHit( const NiPoint3 *point, float *strength, WeaponPayload *payload, TGObject *source );
	void			HandleChangeAlertLevel( TGEvent *event );
	void			SetNoAIState( TGEvent *event );
	void			HandleTractorHitStart( TGEvent *event );
	void			HandleTractorHitStop( TGEvent *event );
	void			SetTargetHandler( TGEvent *event );
	void			CollisionEffectHandler( TGEvent *event );
	void			HostCollisionEffectHandler( TGEvent *event );
	void			PlanetCollisionHandler( TGEvent *event );
	void			StartWarpHandler( TGEvent *event );
	void			EnvironmentDamageHandler( TGEvent *event );
	void			Destroyed( TGEvent *event );

	// The parameter meanings are open; AddDamage passes the hit position, the
	// strength to reduce, the radius, false and true. Public: the splash
	// damage of DamageableObject (0x593C10) calls it too.
	void			Unknown5AFD70( const NiPoint3 &position, float *strength, float radius, TGObject *source, bool includeHull );	// source: what did the damage (the collision event's source)

protected:
	void			InitObject( void );		// invented name
	void			Unknown5AFAD0( TGEvent *event );
	// Adds the subsystem and its children whose damage point lies within the
	// radius of the position (their own radius added) to the list, if they
	// are targetable and not destroyed (invented name).
	void			FindSubsystemsInRange( const NiPoint3 &position, float radius, ShipSubsystem *subsystem, NiTList<ShipSubsystem *> &found );
	// The subsystems within the radius of the position, and the hull always
	// (invented name).
	void			FindSubsystemsNear( const NiPoint3 &position, float radius, NiTList<ShipSubsystem *> &found );
	void			Unknown5B5030( ShipSubsystem *subsystem );
	// The atmosphere effect of a planet the ship flies through: the rumble
	// sound at a volume of twice the depth (at most 1), and the atmosphere
	// particles facing the direction; both stop at a depth of 0.
	// EnvironmentDamageHandler passes what the planet's slots 67/68 return.
	void			Unknown5B2BF0( float depth, NiPoint3 *direction );
	// Adds the subsystem and its children that are hittable, not invincible
	// and not destroyed, but whose damage cell is no longer set in the
	// visible damage, to the list (VirtualSlot87 destroys them).
	void			Unknown5B3670( VisibleDamage *damage, ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *found );
	// Records the visible-damage cell of the subsystem's damage point, and of
	// its children's: the cell itself when it is filled, else the first
	// filled neighbour.
	void			Unknown5B3740( VisibleDamage *damage, ShipSubsystem *subsystem );
	void			Unknown5B0900( float elapsed );	// Update passes the time since the last update
	// The tractor beam systems of the projectors on tractorBeams, their
	// unknownFC added up (invented reading; nothing calls it).
	float			Unknown5ADB80( void );
	void			Unknown5ADBD0( void );
	// The turn toward an orientation, which TurnTowardOrientation hands copies of its two vectors.
	float			Unknown5AD490( NiPoint3 *forward, NiPoint3 *up );
	// Set the angular acceleration (and its linear change over a time), each
	// shortened to the impulse engines' maximum angular acceleration
	// (invented names). TurnTowardDifference and PlanetCollisionHandler call them.
	void			SetLimitedAngularAccelerationLinear( const NiPoint3 &acceleration_, const NiPoint3 &change, float time, int directionType );
	void			SetLimitedAngularAcceleration( NiPoint3 acceleration_, int directionType );
	void			Unknown5ABAC0( void );	// the constructor, SetScale and SetupModel call it last

	bool			unknown1E4;			// 0x1E4
	bool			usingTargetAngularVelocity;	// 0x1E5 set by SetTargetAngularVelocityDirect (invented name)
	bool			docked;				// 0x1E6
	NiPoint3		targetAngularVelocity;	// 0x1E8 (SetTargetAngularVelocityDirect)
	int				targetAngularDirectionType;	// 0x1F4 the angular direction type when the target velocity was set (invented name)
	float			impulse;			// 0x1F8
	NiPoint3		impulseDirection;	// 0x1FC (SetImpulse)
	int				impulseDirectionType;	// 0x208 0: impulseDirection is in the model frame (IsReverse)
	float			disabledEngineDeceleration;	// 0x20C 0.1 at first
	bool			doingInSystemWarp;	// 0x210
	long			inSystemWarpTargetID;	// 0x214 the ID of the object an in-system warp heads for (invented name)
	float			inSystemWarpDistance;	// 0x218 how far short of the destination's bound an in-system warp stops (invented name)
	long			targetID;			// 0x21C the target's object ID; 0 without one (invented name)
	long			targetSubsystemID;	// 0x220 the targeted subsystem's object ID; 0 without one (invented name)
	bool			usingTargetOffset;	// 0x224 targetOffset was set by UseTargetOffsetTG (invented name)
	NiPoint3		targetOffset;		// 0x228 the point aimed at on the target; zero at first
	int				affiliation;		// 0x234
	int				alertLevel;			// 0x238 2 at first
	int				unknown23C;			// 0x23C a sound ID; SetContainingSet stops the sound
	int				hitShield;			// 0x240 the shield facing the last hit test chose, consumed and reset by ApplyWeaponHit; -1 for none (invented name)
	ShieldPatchParticles *	shieldPatches;	// 0x244 the shield hit effect, attached to the node on the first hit; SetContainingSet detaches it (invented name)
	AtmosphereEffect *	atmosphereEffect;	// 0x248 the effect of flying through a planet's atmosphere, attached to the node on the first hit (Unknown5B2BF0); not owned; SetContainingSet detaches it from its parent (invented name)
	NiPoint3		boxExtent;			// 0x24C half the size of the ship's box in the model frame, scaled (invented name)
	NiPoint3		boxCenter;			// 0x258 the center of that box in the model frame (invented name)
	NiTList<TractorBeamProjector *>	tractorBeams;	// 0x264 the projectors whose beams hit the ship; IDs between ReadFromStream and PostFixUp (invented name)
	ShipSubsystems	subsystems;			// 0x27C
	ShipProperty *	shipProperty;		// 0x2E0
	int				netPlayerID;		// 0x2E4
	bool			targetable;			// 0x2E8 true at first
	bool			invincible;			// 0x2E9
	bool			hurtable;			// 0x2EA true at first
	bool			destroyBrokenSystems;	// 0x2EB true at first
	bool			collisionDamageDisabled;	// 0x2EC
	char *			script;				// 0x2F0 owned
	char *			deathScript;		// 0x2F4 owned
	int				aiDoneIgnore;		// 0x2F8
	bool			unknown2FC;			// 0x2FC set when VirtualSlot84 records the player's ship below
	NiPoint3		unknown300;			// 0x300 the player's ship's world location, recorded by VirtualSlot84 in multiplayer
	NiPoint3		unknown30C;			// 0x30C its world forward
	NiPoint3		unknown318;			// 0x318 its world up
	float			unknown324;			// 0x324 its speed, negative in reverse

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	void	SetTargetAngularVelocityFraction( NiPoint3 *fraction );	// retail 0x5AD2D0
	// the scripts' form: the SWIG wrapper ShipClass_SetTargetAngularVelocityFraction passes a copy of the TGPoint3
	void	SetTargetAngularVelocityFraction( TGPoint3 *fraction ) { NiPoint3 point = *fraction; SetTargetAngularVelocityFraction( &point ); }
	// The target angular velocity as a fraction of the engines' maximum, per
	// axis (invented name, after the SWIG wrapper's GetTargetAngularVelocityTG).
	NiPoint3	GetTargetAngularVelocity( void );
	// FUNCTION: STBC 0x0060b1b0 ?GetTargetAngularVelocityTG@ShipClass@@QAE?AVTGPoint3@@XZ @game/AppWrap
	TGPoint3	GetTargetAngularVelocityTG( void ) { return GetTargetAngularVelocity(); }
	float	TurnTowardDifference( NiPoint3 *difference );	// retail 0x5AD4D0
	// the scripts' form: the SWIG wrapper ShipClass_TurnTowardDifference passes a copy of the TGPoint3
	float	TurnTowardDifference( TGPoint3 *difference ) { NiPoint3 point = *difference; return TurnTowardDifference( &point ); }
};

/*
================
ShipClass::IsTypeOf

================
*/
// FUNCTION: STBC 0x005abe70
inline bool ShipClass::IsTypeOf( int type ) {
	if ( type == SHIPCLASS_TYPE ) {
		return true;
	}
	return DamageableObject::IsTypeOf( type );
}

struct _object;
typedef _object PyObject;

// The Python methods ShipClass.GetSubsystems (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		ShipClass_GetSubsystems( PyObject *self, PyObject *args );

#endif /* !__SHIPCLASS_H__ */
