/*
===============================================================================

	ImpulseEngineSubsystem.h

	The ship's impulse engines: the speeds and accelerations they allow now
	(their property's limits scaled by the power and damage), and the
	tractor beam system that drags on them.

	0xBC bytes: the SWIG wrapper ImpulseEngineSubsystem_Create allocates
	that much.

	Evidence: the constructor (0x561050) calls PoweredSubsystem's, stores the
	vtable 0x892D10 (34 slots, none new) and clears the members. GetObjType
	returns 0x8026, the type Cast checks. The SWIG wrappers
	ImpulseEngineSubsystem_* read the current limits (0xAC to 0xB8) and read
	and write the tractor beam system (0xA8) inline.

===============================================================================
*/

#ifndef __IMPULSEENGINESUBSYSTEM_H__
#define __IMPULSEENGINESUBSYSTEM_H__

#include "PoweredSubsystem.h"

class ImpulseEngineProperty;
class TractorBeamSystem;
class TGObject;

// GetObjType's value for an ImpulseEngineSubsystem.
#define IMPULSEENGINESUBSYSTEM_TYPE		0x8026

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ImpulseEngineSubsystem : public PoweredSubsystem {
public:
					ImpulseEngineSubsystem( long id = 0 );
	virtual			~ImpulseEngineSubsystem( void );

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

	static ImpulseEngineSubsystem *	Create( void ) { return new ImpulseEngineSubsystem; }
	static ImpulseEngineSubsystem *	Cast( TGObject * );
	ImpulseEngineProperty *	GetProperty( void );
	float	GetMaxSpeed( void );
	float	GetMaxAccel( void );
	float	GetMaxAngularVelocity( void );
	float	GetMaxAngularAccel( void );

	// The named local is (invented), as in ImpulseEngineProperty::GetMaxSpeed:
	// retail's callers copy the speed through fld/fstp, which needs a second
	// copy level here; its original spelling is unknown.
	float			GetCurMaxSpeed( void ) { float speed = curMaxSpeed; return speed; }
	float			GetCurMaxAccel( void ) { return curMaxAccel; }
	float			GetCurMaxAngularVelocity( void ) { return curMaxAngularVelocity; }
	float			GetCurMaxAngularAccel( void ) { return curMaxAngularAccel; }
	TractorBeamSystem *	GetTractorBeamSystem( void ) { return tractorBeamSystem; }
	void			SetTractorBeamSystem( TractorBeamSystem *tractorBeamSystem_ ) { tractorBeamSystem = tractorBeamSystem_; }

protected:
	TractorBeamSystem *	tractorBeamSystem;	// 0xA8
	float			curMaxSpeed;			// 0xAC
	float			curMaxAccel;			// 0xB0
	float			curMaxAngularVelocity;	// 0xB4
	float			curMaxAngularAccel;		// 0xB8

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
ImpulseEngineSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005610a0
inline bool ImpulseEngineSubsystem::IsTypeOf( int type ) {
	if ( type == IMPULSEENGINESUBSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__IMPULSEENGINESUBSYSTEM_H__ */
