/*
===============================================================================

	PhaserBank.h

	A phaser bank: an energy weapon whose arcs, orientation and beam come
	from its PhaserProperty.

	0x128 bytes: the SWIG wrapper PhaserBank_Create allocates that much.

	Evidence: the constructor (0x570D70) calls EnergyWeapon's, builds a
	0x60-byte object with its own vtable (0x893228) at 0xC8 (0x573100) and
	stores the vtable 0x893194. Cast checks the type 0x802C. The accessors
	(0x570B50 to 0x570D60) read the property; GetOrientationRight crosses
	the forward and up vectors. The class's maximum phaser range (0x8E53DC)
	is read and written by the SWIG wrappers. The vtable has 37 slots, none
	new. SetProperty and SetPowerSetting scale the payload's damage radius
	factor; GetPosition returns the position at 0x11C, which the stream
	functions write after the payload.

===============================================================================
*/

#ifndef __PHASERBANK_H__
#define __PHASERBANK_H__

#include "EnergyWeapon.h"
#include "NiPointer.h"
#include "PhaserGraphic.h"
#include "TGPoint3.h"
#include "WeaponPayload.h"

class PhaserProperty;
class TGObject;

/*
===============================================================================

	PhaserPayload

	A phaser bank's beam: where it starts, which way it points and how far
	it reaches, the graphic that draws it, and what it hits. The class name
	is invented: the payload type is WP_PHASER.

	0x54 bytes, at 0xC8 in a PhaserBank. Evidence: the constructor
	(0x573100) calls WeaponPayload's, stores the vtable 0x893228 and sets
	the members; the destructor (0x573220) detaches the graphic from its
	parent. The stream functions (0x5732F0, 0x5733F0) write every member
	but the graphic. PhaserBank's beam update (0x572440) gives the payload a
	new ID when the beam reaches a new target, and fills in the target, the
	hit point and normal and the time on target.

===============================================================================
*/

class PhaserPayload : public WeaponPayload {
public:
					PhaserPayload( void );
	virtual			~PhaserPayload( void );

	// WeaponPayload
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	NiPoint3		start;					// 0x10 NiPoint3::ZERO at first
	NiPoint3		direction;				// 0x1C NiPoint3::UNIT_Y at first
	float			length;					// 0x28
	int				state;					// 0x2C 1, 2 or 3 while firing
	NiPointer<PhaserGraphic>	graphic;	// 0x30 created when the bank first fires
	long			targetID;				// 0x34 the object the beam hits
	NiPoint3		hitPoint;				// 0x38
	NiPoint3		hitNormal;				// 0x44
	float			timeOnTarget;			// 0x50
};

// GetObjType's value for a PhaserBank.
#define PHASERBANK_TYPE			0x802C

// The bank's beam events (the SWIG constants of the same names).
#define ET_PHASER_STARTED_FIRING	0x800081
#define ET_PHASER_STARTED_HITTING	0x800082
#define ET_PHASER_STOPPED_FIRING	0x800083
#define ET_PHASER_STOPPED_HITTING	0x800084

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PhaserBank : public EnergyWeapon {
public:
					PhaserBank( long id = 0 );
	virtual			~PhaserBank( void );

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
	virtual NiPoint3	GetPosition( void );
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );

	// Weapon
	virtual void	StopFiring( void );
	virtual bool	Fire( float, bool );
	virtual bool	CanFire( void );
	virtual TGPoint3	CalculateRoughDirection( void );

	// EnergyWeapon
	virtual void	SetPowerSetting( int powerSetting_ );

	static PhaserBank *	Create( void ) { return new PhaserBank; }
	static PhaserBank *	Cast( TGObject * );

	// Register the class's handler methods by name, and add them to its
	// table; a phaser bank has none (PhaserSystem's call them; invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
	PhaserProperty *	GetProperty( void );
	TGPoint3	GetOrientationForward( void );
	TGPoint3	GetOrientationUp( void );
	TGPoint3	GetOrientationRight( void );
	// Copies the forward and up directions out; unreferenced (invented name).
	void		Unknown570C30( NiPoint3 *forward, NiPoint3 *up );
	float	GetWidth( void );
	float	GetLength( void );
	int	GetPhaserTextureStart( void );
	int	GetPhaserTextureEnd( void );
	float	GetPhaserWidth( void );
	void	GetArcWidthAngles( float *, float * );
	void	GetArcHeightAngles( float *, float * );
	float	GetArcWidthAngleMin( void );
	float	GetArcWidthAngleMax( void );
	float	GetArcHeightAngleMin( void );
	float	GetArcHeightAngleMax( void );
	void	UpdateCharge( float, float );
	bool	CanHit( TGPoint3 * );
	// Where on its arc the bank fires at a point of the ship's frame from, and
	// the width angle there: the point must be inside the arcs, in reach, and
	// no planet may stand between (invented name).
	bool	FindFiringPosition( const NiPoint3 *target, NiPoint3 *position, float *widthAngle );
	// The point on the arc at a width angle, and the height angle from there
	// to a point of the ship's frame (invented name).
	void	GetArcPosition( float widthAngle, const NiPoint3 *target, NiPoint3 *position, float *heightAngle );
	// One step of a firing beam from the world point start to target: a bank
	// that isn't firing yet creates the beam, plays the fire sounds and
	// tells the ship; then the beam charges up, grows and holds (invented name).
	void	UpdateBeam( const NiPoint3 *start, const NiPoint3 *target, float time, bool playSound, float widthAngle );
	// Gives the beam's target the damage of the time on target (invented name).
	void	ApplyDamage( void );
	// Finds the nearest object the beam hits and makes it the target,
	// telling the old and the new one; the damage is applied every
	// damageInterval seconds on target (invented name).
	void	RetargetBeam( float time );
	// Puts the beam on the ship and starts charging it (invented name).
	void	CreateBeam( const NiPoint3 *start, const NiPoint3 *target, float time, float widthAngle );
	// How much of the full rate a firing bank discharges at, by its system's power level (invented name).
	float	GetDischargeFactor( void );

	// Puts the bank in the middle of its arc (invented name; ShipClass::VirtualSlot88 calls it).
	void	Unknown572F00( void );

	// Whether a point is in reach: within the maximum phaser range unless the
	// bank is already firing, and the ship's power output covers the discharge (invented name).
	bool	CanReach( const NiPoint3 *from, const NiPoint3 *to );
	// Whether a pair of angles lies inside the bank's arc (invented name).
	bool	IsInArcAngles( float width, float height );
	// Points the beam from one point towards another, at full range (invented name).
	void	AimBeam( const NiPoint3 *from, const NiPoint3 *to, float deltaTime );
	// The damage a beam does over a time at a distance (invented name).
	float	CalculateDamage( float time, float distance );

	static float	GetMaxPhaserRange( void ) { return maxPhaserRange; }
	static void		SetMaxPhaserRange( float maxPhaserRange_ ) { maxPhaserRange = maxPhaserRange_; }

protected:
	PhaserPayload	payload;				// 0xC8
	NiPoint3		position;				// 0x11C where the bank sits, written and read by the stream functions (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
	static float	maxPhaserRange;			// the farthest a phaser bank reaches
	static float	damageInterval;			// seconds on target between two damage steps (invented name)

	// How much of the full rate a firing bank discharges at, by its system's
	// power level (invented names).
	static const float	DISCHARGE_FACTOR_LEVEL0;
	static const float	DISCHARGE_FACTOR_LEVEL1;
	static const float	DISCHARGE_FACTOR_LEVEL2;

	// The damage radius factor at each power setting (invented names).
	static const float	DAMAGE_RADIUS_SCALE_LOW;
	static const float	DAMAGE_RADIUS_SCALE_MEDIUM;
	static const float	DAMAGE_RADIUS_SCALE_HIGH;

	// How much of its damage a beam does at each power level (invented names).
	static const float	DAMAGE_FACTOR_LEVEL0;
	static const float	DAMAGE_FACTOR_LEVEL1;
	static const float	DAMAGE_FACTOR_LEVEL2;
};

/*
================
PhaserBank::IsTypeOf
================
*/
// FUNCTION: STBC 0x00570de0
inline bool PhaserBank::IsTypeOf( int type ) {
	if ( type == PHASERBANK_TYPE ) {
		return true;
	}
	return EnergyWeapon::IsTypeOf( type );
}

#endif /* !__PHASERBANK_H__ */
