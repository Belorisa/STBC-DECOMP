/*
===============================================================================

	ShipSubsystem.h

	A part of a ship that can be damaged and repaired: built from a
	SubsystemProperty, with a condition, child subsystems, and watchers
	that send the subsystem events when its condition crosses the disabled
	and destroyed thresholds.

	0x88 bytes, allocated from the pools (the scalar deleting destructor
	0x56BB60 frees through the pool of that size).

	Evidence: the constructor (0x56B970) calls TGEventHandlerObject's,
	constructs the watchers at 0x4C and 0x58, stores the vtable 0x892FC4
	(30 slots; 21 to 29 are new) and sets the members; the destructor
	(0x56BB90) deletes the children (0x1C, 0x20) and the name (0x14).
	GetObjType returns 0x801B, the type Cast checks. The SWIG wrappers
	ShipSubsystem_* name the accessors: GetRadius, GetRepairComplexity and
	GetDisabledPercentage read the property (0x18), SetInvincible writes
	0x44 down the children, Repair raises the condition (0x30).
	AddChildSubsystem appends to the children (0x1C, 0x20) and passes the
	child this subsystem (slot 23, 0x24) and the ship (slot 22, 0x40).

===============================================================================
*/

#ifndef __SHIPSUBSYSTEM_H__
#define __SHIPSUBSYSTEM_H__

#include <math.h>
#include "TGEventHandlerObject.h"
#include "FloatRangeWatcher.h"
#include "NiPoint3.h"
#include "TGString.h"
#include "TGPoint3.h"

class ShipClass;
class SubsystemProperty;
class TGObject;

// GetObjType's value for a ShipSubsystem.
#define SHIPSUBSYSTEM_TYPE		0x801B

// Event types, as the SWIG constants of the App module give them.
#define ET_SUBSYSTEM_DAMAGED				0x80006B
#define ET_SUBSYSTEM_STATE_CHANGED			0x80006C
#define ET_SUBSYSTEM_DISABLED				0x80006D
#define ET_SUBSYSTEM_OPERATIONAL			0x80006E
#define ET_SUBSYSTEM_DESTROYED				0x80006F
#define ET_SUBSYSTEM_REBUILT				0x800070
#define ET_SUBSYSTEM_COMPLETELY_DESTROYED	0x800071
#define ET_SUBSYSTEM_COMPLETELY_DISABLED	0x800072
#define ET_SUBSYSTEM_PARTIALLY_OPERATIONAL	0x800073
#define ET_SUBSYSTEM_STATE_CHANGED_NOTIFY	0x8000DD

// The condition percentage below which a subsystem counts as destroyed (invented name).
#define SUBSYSTEM_DESTROYED_PERCENTAGE		1e-6f

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ShipSubsystem : public TGEventHandlerObject {
public:
					ShipSubsystem( long id = 0 );
	virtual			~ShipSubsystem( void );

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

	virtual NiPoint3	GetPosition( void );
	virtual void	SetParentShip( ShipClass *ship );
	virtual void	SetParentSubsystem( ShipSubsystem *parent );
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );
	virtual void	VirtualSlot28( TGStream *stream, bool );
	virtual void	VirtualSlot29( TGStream *stream, float time );

	static ShipSubsystem *	Create( void ) { return new ShipSubsystem; }

	// Adds the time and, once UPDATE_INTERVAL has accumulated, updates the
	// subsystem for the whole intervals in it (invented name).
	void			TimeSlicedUpdate( float deltaTime );

	SubsystemProperty *	GetProperty( void ) { return property; }
	ShipClass *		GetParentShip( void ) { return parentShip; }
	ShipSubsystem *	GetParentSubsystem( void ) { return parentSubsystem; }
	int				GetNumChildSubsystems( void ) { return numChildSubsystems; }
	ShipSubsystem **	GetChildSubsystems( void ) { return childSubsystems; }
	// The named local is (invented): retail's callers copy the condition through
	// fld/fstp, which needs a second copy level here; an inner inline, a named
	// local or a value wrapper give the same code, so its spelling is unknown.
	// FUNCTION: STBC 0x00521fe0 ?GetCondition@ShipSubsystem@@QAEMXZ @game/STFillGauge
	float			GetCondition( void ) { float value = condition; return value; }
	// The named local is (invented), as GetCondition's: retail's callers
	// (ShipSubsystem::IsDisabled, PhaserBank::Update) copy it through fld/fstp.
	// FUNCTION: STBC 0x00521ff0 ?GetConditionPercentage@ShipSubsystem@@QAEMXZ @game/STFillGauge
	float			GetConditionPercentage( void ) { float value = conditionPercentage; return value; }
	// The named local is (invented), as GetCondition's: PoweredSubsystem::Update
	// copies the length through fld/fstp.
	float			GetLastUpdateLength( void ) { float length = lastUpdateLength; return length; }
	bool			IsInvincible( void ) { return invincible; }
	NiPoint3		GetDamagePoint( void ) { return damagePoint; }
	FloatRangeWatcher *	GetConditionWatcher( void ) { return &conditionWatcher; }
	FloatRangeWatcher *	GetCombinedPercentageWatcher( void ) { return &combinedPercentageWatcher; }

	// The repair points that bring the subsystem back to its full condition.
	float			GetRepairPointsNeeded( void ) { return ( GetMaxCondition() - condition ) * GetRepairComplexity(); }

	char *	GetName( void );
	TGString	GetDisplayName( void );
	// Inline: GetWorldLocation's budget spends its cost at both calls, which
	// the compiler then rejects late (a TGPoint3 returned in an EH context).
	// FUNCTION: STBC 0x0056c8b0 ?GetPositionTG@ShipSubsystem@@QAE?AVTGPoint3@@XZ @game/ShipSubsystem
	TGPoint3	GetPositionTG( void ) { return GetPosition(); }
	TGPoint3	GetWorldLocation( void );
	static ShipSubsystem *	Cast( TGObject * );
	float	GetMaxCondition( void );
	float	GetCombinedConditionPercentage( void );
	float	GetRadius( void );
	bool	IsCritical( void );
	bool	IsTargetable( void );
	float	GetRepairComplexity( void );
	float	GetDisabledPercentage( void );
	bool	IsDisabled( void );
	bool	IsHittableFromLocation( TGPoint3 * );
	ShipSubsystem *	GetChildSubsystem( int );
	int	GetChildSubsystemIndex( ShipSubsystem * );
	int	GetLastChildSubsystemIndex( void );
	bool	HasTargetableChildSubsystem( void );
	ShipSubsystem *	GetNextTargetableChildSubsystem( ShipSubsystem * );
	void	AddChildSubsystem( ShipSubsystem * );
	void	SetCondition( float );
	void	SetConditionPercentage( float );
	void	SetInvincible( bool );
	void	Repair( float );
	void	UpdateDamagePoint( void );

	// Adds the range checks that send the subsystem events (invented name).
	void	SetupRangeChecks( void );

protected:
	char *			name;					// 0x14 built from the property's name by GetName
	SubsystemProperty *	property;			// 0x18
	int				numChildSubsystems;		// 0x1C
	ShipSubsystem **	childSubsystems;	// 0x20 (invented name)
	ShipSubsystem *	parentSubsystem;		// 0x24 the subsystem this one is a child of
	unsigned short	unknown28;				// 0x28 0xFFFF at first
	unsigned short	unknown2A;				// 0x2A 0xFFFF at first
	unsigned short	unknown2C;				// 0x2C 0xFFFF at first
	float			condition;				// 0x30
	float			conditionPercentage;	// 0x34 condition / max condition
	float			combinedConditionPercentage;	// 0x38 with the children's
	float			lastUpdateLength;		// 0x3C the time step of the last Update
	ShipClass *		parentShip;				// 0x40 the events' destination
	bool			invincible;				// 0x44
	bool			disabled;				// 0x45 as the last update found it (invented name)
	float			unknown48;				// 0x48 time since the last Update, random in [0, UPDATE_INTERVAL) at first
	FloatRangeWatcher	conditionWatcher;			// 0x4C watches conditionPercentage
	FloatRangeWatcher	combinedPercentageWatcher;	// 0x58 watches combinedConditionPercentage
	int				disabledCheck;			// 0x64 range check IDs, RID_INVALID without one (invented names)
	int				operationalCheck;		// 0x68
	int				destroyedCheck;			// 0x6C
	int				rebuiltCheck;			// 0x70
	int				completelyDestroyedCheck;	// 0x74
	NiPoint3		damagePoint;			// 0x78 NiPoint3::ZERO at first (the SWIG wrapper ShipSubsystem_GetDamagePoint copies it)
	float			unknown84;				// 0x84 the time of the newest state that VirtualSlot29 (and PoweredSubsystem's slot 33) took

	static TGEventHandlerTable	classHandlers;		// invented name

	// The ship steps the warp engines, the cloak and the other subsystems
	// itself (ShipClass::Unknown5B0900 reads and writes unknown48).
	friend class	ShipClass;

public:
	// The ship updates a subsystem once this much time has accumulated (invented name).
	static const float	UPDATE_INTERVAL;

	// Declared for the SWIG wrappers that call them.
public:
	float	GetDamage( void ) { return GetMaxCondition() - condition; }	// the SWIG wrapper ShipSubsystem_GetDamage expands it
};

/*
================
ShipSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x0056bae0
inline bool ShipSubsystem::IsTypeOf( int type ) {
	if ( type == SHIPSUBSYSTEM_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
ShipSubsystem::SetParentSubsystem

Retail keeps the copy emitted beside PowerSubsystem's inline functions.
================
*/
// FUNCTION: STBC 0x00440120
inline void ShipSubsystem::SetParentSubsystem( ShipSubsystem *parent ) {
	parentSubsystem = parent;
}

/*
================
ShipSubsystem::TimeSlicedUpdate
================
*/
inline void ShipSubsystem::TimeSlicedUpdate( float deltaTime ) {
	unknown48 += deltaTime;
	float elapsed = unknown48;
	if ( elapsed > UPDATE_INTERVAL ) {
		float remainder = (float)fmod( elapsed, UPDATE_INTERVAL );
		Update( elapsed - remainder );
		unknown48 = remainder;
	}
}

#endif /* !__SHIPSUBSYSTEM_H__ */
