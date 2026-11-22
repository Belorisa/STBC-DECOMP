/*
===============================================================================

	TractorBeamProjector.h

	A tractor beam projector: an energy weapon that holds its target.

	0x100 bytes: the SWIG wrapper TractorBeamProjector_Create allocates
	that much.

	Evidence: the constructor (0x57EC70) calls EnergyWeapon's, builds its
	payload at 0xC8 (0x581350), stores the vtable 0x8936F0 (37 slots, none
	new) and sets 0xFC, which the stream functions write before the
	payload. Update keeps the projector at its maximum charge.

===============================================================================
*/

#ifndef __TRACTORBEAMPROJECTOR_H__
#define __TRACTORBEAMPROJECTOR_H__

#include "EnergyWeapon.h"
#include "NiPointer.h"
#include "TractorBeamGraphic.h"
#include "WeaponPayload.h"

class DamageableObject;
class ObjectClass;
class PhysicsObjectClass;
class TGObject;

// The fractions of the main battery's limit a tractor beam needs to keep
// firing and to start (invented names).
#define TRACTOR_KEEP_FIRING_BATTERY		0.01f
#define TRACTOR_START_FIRING_BATTERY	0.05f
class TractorBeamProperty;

/*
===============================================================================

	TractorBeamPayload

	A tractor beam projector's beam: where it starts, which way it points
	and how far it reaches, the graphic that draws it and its target. The
	class name is invented; the constructor sets the payload type to
	WP_PHASER, as PhaserPayload's does.

	0x34 bytes, at 0xC8 in a TractorBeamProjector. Evidence: the
	constructor (0x581350) calls WeaponPayload's and stores the vtable
	0x893788; the destructor (0x581420) detaches the graphic from its
	parent and releases it. The stream functions (0x581590, 0x581610) write
	every member but the graphic.

===============================================================================
*/

class TractorBeamPayload : public WeaponPayload {
public:
					TractorBeamPayload( void );
	virtual			~TractorBeamPayload( void );

	// WeaponPayload
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	NiPoint3		start;					// 0x10 NiPoint3::ZERO at first
	NiPoint3		direction;				// 0x1C NiPoint3::UNIT_Y at first
	float			length;					// 0x28
	NiPointer<TractorBeamGraphic>	graphic;	// 0x2C created when the projector fires
	long			targetID;				// 0x30 the object the beam holds
};

// GetObjType's value for a TractorBeamProjector.
#define TRACTORBEAMPROJECTOR_TYPE	0x802E

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// The projector has started firing (the SWIG constant ET_TRACTOR_BEAM_STARTED_FIRING).
#define ET_TRACTOR_BEAM_STARTED_FIRING		0x80007D
// The projector has stopped firing (the SWIG constant ET_TRACTOR_BEAM_STOPPED_FIRING).
#define ET_TRACTOR_BEAM_STOPPED_FIRING		0x80007F
// The docking beam brought its target in (the SWIG constant ET_TRACTOR_TARGET_DOCKED).
#define ET_TRACTOR_TARGET_DOCKED			0x800085

class TractorBeamProjector : public EnergyWeapon {
public:
					TractorBeamProjector( long id = 0 );
	virtual			~TractorBeamProjector( void );

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
	virtual void	StopFiring( void );
	virtual bool	Fire( float, bool );
	virtual bool	CanFire( void );
	virtual TGPoint3	CalculateRoughDirection( void );

	static TractorBeamProjector *	Create( void ) { return new TractorBeamProjector; }
	static TractorBeamProjector *	Cast( TGObject * );
	TractorBeamProperty *	GetProperty( void );
	// The property's values, as PhaserBank's accessors of the same names
	// give them (invented names: no SWIG wrapper calls these).
	TGPoint3		GetOrientationForward( void );
	TGPoint3		GetOrientationUp( void );
	void			GetOrientation( TGPoint3 *forward, TGPoint3 *up );
	void			GetArcWidthAngles( float *min, float *max );
	void			GetArcHeightAngles( float *min, float *max );
	float			GetArcWidthAngleMin( void );
	float			GetArcWidthAngleMax( void );
	float			GetArcHeightAngleMin( void );
	float			GetArcHeightAngleMax( void );
	float			GetMaxDamageDistance( void );
	// Where the beam starts in the world: half a unit along the forward axis from the damage point (invented name).
	NiPoint3		GetBeamOrigin( void );
	// Whether the beam reaches the offset from the damage point, in the
	// ship's frame: within the charge's range and the maximum damage
	// distance, and inside the arcs (invented names, after PhaserBank's
	// CanReach and PulseWeapon's IsInArc).
	bool			CanReach( NiPoint3 offset );
	bool			IsInArc( NiPoint3 offset );
	// The charge that gives the beam its full range (invented name: a
	// constant 1, which only CanReach calls).
	float			Unknown581010( void );
	// The nearest damageable object, other than the ship, the segment from
	// start to end hits, and its distance; NULL when none (invented name).
	DamageableObject *	FindHitObject( const NiPoint3 *start, const NiPoint3 *end, float *distance );
	// Starts or moves the beam from start to the target point, holds the
	// target and plays the beam (invented name, after PhaserBank's).
	void			UpdateBeam( const NiPoint3 &start, const NiPoint3 &target, float time, bool playSound );
	// Sets the beam's start, direction and length from start to end
	// (invented name).
	void			SetBeamEnds( const NiPoint3 &start, const NiPoint3 &end );
	// Creates the graphic the first time, puts it in the set's effects and
	// starts it over the beam's length (invented name, after PhaserBank's).
	void			CreateBeam( const NiPoint3 &start, const NiPoint3 &end );
	// The damage the beam does over the time at the distance (invented name,
	// after PhaserBank's).
	float			CalculateDamage( float time, float distance );
	// Nothing in retail calls it: close to CalculateDamage run backwards, the
	// damage per unit of time and condition at full reach (placeholder name;
	// the parameters' names are guesses).
	float			Unknown580FC0( float damage, float time );
	// Finds what the beam hits along its length, moves the hit lights and
	// events to it, and works the system's mode on it (invented name).
	void			UpdateTarget( float time );
	// The modes' work on the target, with the damage the beam can do over
	// the time; each returns the damage left unused (invented names, after
	// the TractorBeamSystem modes that pick them).
	float			HoldTarget( PhysicsObjectClass *target, float damage, float time );
	float			TowTarget( PhysicsObjectClass *target, float damage, float time );
	float			PullTarget( PhysicsObjectClass *target, float damage, float time );
	float			PushTarget( PhysicsObjectClass *target, float damage, float time );
	float			DockTarget( PhysicsObjectClass *target, float damage, float time );

protected:
	TractorBeamPayload	payload;			// 0xC8
	bool			unknownFC;				// 0xFC true at first

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TractorBeamProjector::IsTypeOf
================
*/
// FUNCTION: STBC 0x0057ece0
inline bool TractorBeamProjector::IsTypeOf( int type ) {
	if ( type == TRACTORBEAMPROJECTOR_TYPE ) {
		return true;
	}
	return EnergyWeapon::IsTypeOf( type );
}

#endif /* !__TRACTORBEAMPROJECTOR_H__ */
