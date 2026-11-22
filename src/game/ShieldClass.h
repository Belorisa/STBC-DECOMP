/*
===============================================================================

	ShieldClass.h

	The ship's shields: six facings, each with its current strength, its
	percentage of its property's maximum, a watcher on that percentage and
	a damaged flag.

	0x15C bytes: the SWIG wrapper ShieldClass_Create allocates that much.

	Evidence: the constructor (0x56A000) calls PoweredSubsystem's, builds
	seven FloatRangeWatchers at 0xDC (the vector constructor, 0xC bytes
	each), stores the vtable 0x892F34 and, per facing, sets the shields
	(0xA8) to 1000, the percentage (0xC0) and 0x130 to 1, clears the
	damaged flag (0x14C) and points the facing's watcher at its percentage;
	the seventh watcher watches the overall percentage (0xD8). The vtable
	has 34 slots, none new. GetObjType returns 0x8028, the type Cast
	checks. The SWIG
	wrappers ShieldClass_* read these inline, read the maximum shields and
	charge rates from the property, and return NUM_SHIELDS. The facing
	names and values come from the SWIG constants ShieldClass_NO_SHIELD
	to ShieldClass_NUM_SHIELDS.

===============================================================================
*/

#ifndef __SHIELDCLASS_H__
#define __SHIELDCLASS_H__

#include "PoweredSubsystem.h"
#include "FloatRangeWatcher.h"
#include "ShieldProperty.h"

class TGObject;

// GetObjType's value for a ShieldClass.
#define SHIELDCLASS_TYPE		0x8028

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// The shields are raised or lowered (the SWIG constant ET_SHIELDS_SET_STATE).
#define ET_SHIELDS_SET_STATE		0x80007B

// The share of the power received that charges the facings (invented name).
#define SHIELD_CHARGE_EFFICIENCY	0.85f

// A facing holding less than this is breached (invented name).
#define BREACHED_SHIELD_CHARGE		1.0f

class ShieldClass : public PoweredSubsystem {
public:
	// The facings (the enum's name is invented).
	enum shieldFacing_t {
		NO_SHIELD = -1,
		FRONT_SHIELDS,
		REAR_SHIELDS,
		TOP_SHIELDS,
		BOTTOM_SHIELDS,
		LEFT_SHIELDS,
		RIGHT_SHIELDS,
		NUM_SHIELDS
	};

					ShieldClass( long id = 0 );
	virtual			~ShieldClass( void );

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
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );
	virtual void	VirtualSlot28( TGStream *stream, bool );
	virtual void	VirtualSlot29( TGStream *stream, float time );

	// PoweredSubsystem
	virtual void	TurnOn( void );

	static ShieldClass *	Create( void ) { return new ShieldClass; }
	static ShieldClass *	Cast( TGObject * );
	ShieldProperty *	GetProperty( void ) { return (ShieldProperty *)property; }
	int				GetNumShields( void ) { return NUM_SHIELDS; }
	FloatRangeWatcher *	GetShieldWatcher( int shield ) { return &shieldWatchers[shield]; }
	float			GetSingleShieldPercentage( int shield ) { return shieldPercentages[shield]; }
	float			GetCurShields( int shield ) { return curShields[shield]; }
	float			GetMaxShields( int shield ) { return GetProperty()->GetMaxShields( shield ); }
	float			GetShieldChargePerSecond( int shield ) { return GetProperty()->GetShieldChargePerSecond( shield ); }
	bool			IsShieldDamaged( int shield ) { return shieldDamaged[shield]; }
	void			SetShieldDamaged( int shield, bool damaged ) { shieldDamaged[shield] = damaged; }
	float			GetShieldGlowDecay( void ) { return GetProperty()->GetShieldGlowDecay(); }
	// FUNCTION: STBC 0x005af3b0 ?GetShieldGlowColor@ShieldClass@@QAE?AVTGColorA@@XZ @game/ShipClass
	TGColorA		GetShieldGlowColor( void ) { return GetProperty()->GetShieldGlowColor(); }
	float	BoostShield( int, float );
	float	GetShieldPercentage( void );
	void	SetCurShields( int, float );
	bool	IsShieldBreached( int );
	bool	IsAnyShieldBreached( void );
	void	RedistributeShields( void );
	int	GetOppositeShield( int );
	// The facing that a direction in the model's frame points through: the
	// largest of its components (y forward, z up, x right) and their
	// negatives.
	int				Unknown56A8D0( const NiPoint3 *direction );
	// The facing a line between two points in the world goes through, or -1
	// (OptimizedFireScript, 0x483ABE: the result indexes curShields).
	int				Unknown56A690( const NiPoint3 *start, const NiPoint3 *end );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleSetShieldState( TGEvent *event );

protected:
	float			curShields[NUM_SHIELDS];			// 0xA8 1000 at first
	float			shieldPercentages[NUM_SHIELDS + 1];	// 0xC0 curShields over the property's maximum, then the overall percentage (0xD8)
	FloatRangeWatcher	shieldWatchers[NUM_SHIELDS + 1];	// 0xDC per facing, then the overall one (invented name)
	float			unknown130[NUM_SHIELDS + 1];	// 0x130 per facing, then overall; the facings 1 at first
	bool			shieldDamaged[NUM_SHIELDS];		// 0x14C
	float			chargeTime;						// 0x154 seconds since the facings were last charged (invented name)
	float			chargePower;					// 0x158 the power received since the facings were last charged (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
ShieldClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x0056a0c0
inline bool ShieldClass::IsTypeOf( int type ) {
	if ( type == SHIELDCLASS_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__SHIELDCLASS_H__ */
