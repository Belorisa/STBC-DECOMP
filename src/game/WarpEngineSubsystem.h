/*
===============================================================================

	WarpEngineSubsystem.h

	The ship's warp engines: the warp state, the warp sequence that plays
	the warp, and where the ship leaves warp.

	0xD4 bytes, allocated from the pools (the scalar deleting destructor
	0x56DFA0 frees through the pool of that size).

	Evidence: the constructor (0x56DE70) calls PoweredSubsystem's, stores the
	vtable 0x893040 (34 slots, none new) and clears the members. GetObjType
	returns 0x8025, the type Cast checks. The SWIG wrappers
	WarpEngineSubsystem_* read and write the warp state (0xB4) and the
	placement (0xBC) inline, copy the exit point into 0xC0, look the warp
	sequence up by the ID at 0xAC, and read and write the class's warp effect
	time and warp speed (0x8E5328, 0x8E532C). The destructor clears the AI
	whose ID 0xCC holds, as ShipClass::ClearAI (0x5AE600) does through it.

===============================================================================
*/

#ifndef __WARPENGINESUBSYSTEM_H__
#define __WARPENGINESUBSYSTEM_H__

#include "PoweredSubsystem.h"
#include "WarpEngineProperty.h"
#include "TGAction.h"
#include "TGMatrix3.h"

class ArtificialIntelligence;
class PlacementObject;
class TGObject;

// GetObjType's value for a WarpEngineSubsystem.
#define WARPENGINESUBSYSTEM_TYPE	0x8025

// The ship's warp sequence is set (the SWIG constant ET_SET_WARP_SEQUENCE).
#define ET_SET_WARP_SEQUENCE		0x8000EE

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WarpEngineSubsystem : public PoweredSubsystem {
public:
	// The warp states (the shadow class's constants).
	enum warpState_t {
		WES_NOT_WARPING,
		WES_WARP_INITIATED,
		WES_WARP_BEGINNING,
		WES_WARP_ENDING,
		WES_WARPING,
		WES_DEWARP_INITIATED,
		WES_DEWARP_BEGINNING,
		WES_DEWARP_ENDING
	};

					WarpEngineSubsystem( long id = 0 );
	virtual			~WarpEngineSubsystem( void );

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
	virtual void	Update( float deltaTime );

	static WarpEngineSubsystem *	Create( void ) { return new WarpEngineSubsystem; }
	static WarpEngineSubsystem *	Cast( TGObject * );

	WarpEngineProperty *	GetProperty( void ) { return (WarpEngineProperty *)property; }
	int				GetWarpState( void ) { return warpState; }
	void			SetWarpState( int state ) { warpState = state; }
	void			SetShipInvincible( bool invincible ) { shipInvincible = invincible; }	// invented name
	PlacementObject *	GetPlacement( void ) { return placement; }
	void			SetPlacement( PlacementObject *placement_ ) { placement = placement_; }
	void			SetExitPoint( TGPoint3 *point ) { exitPoint = *point; }
	TGAction *		GetWarpSequence( void ) { return TGAction::Cast( TGObject::GetTGObjectPtr( warpSequenceID ) ); }

	TGPoint3		GetWarpExitLocation( void );
	TGMatrix3		GetWarpExitRotation( void );
	bool	Warp( void );
	void	TransitionToState( int );

	static float	GetWarpEffectTime( void ) { return warpEffectTime; }
	static void		SetWarpEffectTime( float time ) { warpEffectTime = time; }
	static float	GetWarpSpeed( void ) { return warpSpeed; }
	static void		SetWarpSpeed( float speed ) { warpSpeed = speed; }

	// Clears the AI the engines set on the ship, as ShipClass::ClearAI does (invented name).
	void	ClearAI( bool unknown = true, ArtificialIntelligence *ai = NULL );
	// While a warp is under way, keeps the AI to set on the ship afterwards;
	// otherwise sets it on the ship at once (invented name).
	void	SetAI( ArtificialIntelligence *ai, bool unknown1, bool unknown2 );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			SetWarpSequence( TGEvent *event );

protected:
	bool			shipInvincible;			// 0xA8 ShipClass::SetInvincible (0x5AE090) copies the ship's flag here while a warp is under way (invented name)
	long			warpSequenceID;			// 0xAC the ID of the warp sequence (invented name)
	float			unknownB0;				// 0xB0
	int				warpState;				// 0xB4
	int				unknownB8;				// 0xB8
	PlacementObject *	placement;			// 0xBC
	NiPoint3		exitPoint;				// 0xC0
	long			aiID;					// 0xCC the ID of the AI the engines set (invented name)
	bool			unknownD0;				// 0xD0 set with the AI
	bool			unknownD1;				// 0xD1 set with the AI

	static float	warpEffectTime;			// seconds
	static float	warpSpeed;
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
WarpEngineSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x0056df00
inline bool WarpEngineSubsystem::IsTypeOf( int type ) {
	if ( type == WARPENGINESUBSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__WARPENGINESUBSYSTEM_H__ */
