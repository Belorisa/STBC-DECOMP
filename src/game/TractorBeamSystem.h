/*
===============================================================================

	TractorBeamSystem.h

	The ship's tractor beam projectors, as one weapon system, and what the
	beams do to their target (the mode).

	0x100 bytes: the SWIG wrapper TractorBeamSystem_Create allocates that
	much.

	Evidence: the constructor (0x582080) calls WeaponSystem's, stores the
	vtable 0x893794, sets the mode (0xF4) and PoweredSubsystem's 0xA0 to 1
	and clears the members. The SWIG wrappers TractorBeamSystem_* read and
	write the mode inline.

===============================================================================
*/

#ifndef __TRACTORBEAMSYSTEM_H__
#define __TRACTORBEAMSYSTEM_H__

#include "WeaponSystem.h"

class ImpulseEngineSubsystem;
class TGEvent;
class TGObject;

// The type TractorBeamSystem::Cast (0x582050) checks.
#define TRACTORBEAMSYSTEM_TYPE		0x8021

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TractorBeamSystem : public WeaponSystem {
public:
	// The modes (the SWIG constants TractorBeamSystem_TBS_*).
	enum {
		TBS_HOLD,
		TBS_TOW,
		TBS_PULL,
		TBS_PUSH,
		TBS_DOCK_STAGE_1,
		TBS_DOCK_STAGE_2
	};

					TractorBeamSystem( long id = 0 );
	virtual			~TractorBeamSystem( void );

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
	virtual void	SetParentShip( ShipClass *ship );
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );

	// PoweredSubsystem
	virtual float	GetNormalPowerWanted( void );

	// WeaponSystem
	virtual void	VirtualSlot34( ObjectClass *target, NiPoint3 *offset );
	virtual void	StopFiring( void );

	virtual void	VirtualSlot38( ObjectClass *target );	// stops firing at the target (StopFiringAtTargetHandler passes the event's object)

	static TractorBeamSystem *	Create( void ) { return new TractorBeamSystem; }
	static TractorBeamSystem *	Cast( TGObject * );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name
	void			StartFiringHandler( TGEvent *event );
	void			StopFiringHandler( TGEvent *event );
	void			StopFiringAtTargetHandler( TGEvent *event );
	int				GetMode( void ) { return mode; }
	void			SetMode( int mode_ ) { mode = mode_; }
	void			SetImpulseEngineSubsystem( ImpulseEngineSubsystem *engines ) { impulseEngineSubsystem = engines; }	// invented name
	ImpulseEngineSubsystem *	GetImpulseEngineSubsystem( void ) { return impulseEngineSubsystem; }	// invented name
	void			AddLoad( float load ) { unknownFC += load; }	// invented name
	float			GetUnknownFC( void ) { return unknownFC; }	// invented name

	// The sum of the projectors' maximum damage (invented name).
	float	GetProjectorsMaxDamage( void );
	float	GetLoad( void );		// invented name
	// Where the ship launches its shuttles, in the world (0x582300; invented name).
	NiPoint3	GetDockingPoint( void );

protected:
	ImpulseEngineSubsystem *	impulseEngineSubsystem;	// 0xF0 the engines it drags on; ImpulseEngineSubsystem::SetParentShip sets it (invented name)
	int				mode;					// 0xF4 1 at first
	float			unknownF8;				// 0xF8 the projectors' total maximum damage, taken each Update
	float			unknownFC;				// 0xFC

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TractorBeamSystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005820d0
inline bool TractorBeamSystem::IsTypeOf( int type ) {
	if ( type == TRACTORBEAMSYSTEM_TYPE ) {
		return true;
	}
	return WeaponSystem::IsTypeOf( type );
}

#endif /* !__TRACTORBEAMSYSTEM_H__ */
