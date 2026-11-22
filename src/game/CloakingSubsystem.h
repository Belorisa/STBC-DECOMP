/*
===============================================================================

	CloakingSubsystem.h

	The cloaking device: the cloak state machine, and the ambient light the
	ship is drawn with while it cloaks.

	0xBC bytes, allocated from the pools (the scalar deleting destructor
	0x55E400 frees through the pool of that size).

	Evidence: the constructor (0x55E2B0) calls PoweredSubsystem's, stores the
	vtable 0x892C04 (35 slots; 34 is new), clears the members, turns the
	device off when it is created locally, and sets PoweredSubsystem's 0xA0
	to 2. GetObjType returns 0x8024, the type Cast checks. The destructor
	releases the reference at 0xA8, which 0x55E6B0 fills with a new
	NiAmbientLight (0x148 bytes, constructor 0x7F3FC0). The SWIG wrappers
	CloakingSubsystem_* read IsCloaked (0xAC) and IsTryingToCloak (0xAD)
	inline, compare the state at 0xB0 for IsCloaking (1, 2) and
	IsDecloaking (4, 5), and read and write the class's cloak time and
	shield delay (0x8E4E1C, 0x8E4E20).

===============================================================================
*/

#ifndef __CLOAKINGSUBSYSTEM_H__
#define __CLOAKINGSUBSYSTEM_H__

#include "PoweredSubsystem.h"
#include "CloakingSubsystemProperty.h"
#include "NiAmbientLight.h"
#include "NiPointer.h"

class TGObject;
class NiObject;

// GetObjType's value for a CloakingSubsystem.
#define CLOAKINGSUBSYSTEM_TYPE		0x8024

// The ship starts or stops trying to cloak (the SWIG constants ET_START_CLOAKING
// and ET_STOP_CLOAKING).
#define ET_START_CLOAKING			0x8000E3
#define ET_STOP_CLOAKING			0x8000E5

// The ship has decloaked (the SWIG constant ET_DECLOAK_COMPLETED; the value follows from the constant table's order).
#define ET_DECLOAK_COMPLETED		0x80007A
// The ship has cloaked, and starts to decloak (the SWIG constants).
#define ET_CLOAK_COMPLETED			0x800078
#define ET_DECLOAK_BEGINNING		0x800079

// The ship starts to cloak (the SWIG constant ET_CLOAK_BEGINNING).
#define ET_CLOAK_BEGINNING			0x800077
// The cloak state while the ship is fully cloaked (invented name).
#define CLOAK_STATE_CLOAKED			3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class CloakingSubsystem : public PoweredSubsystem {
public:
					CloakingSubsystem( long id = 0 );
	virtual			~CloakingSubsystem( void );

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
	virtual void	SetParentShip( ShipClass *ship );
	virtual void	Update( float deltaTime );
	virtual void	VirtualSlot26( TGStream *stream );
	virtual void	VirtualSlot27( TGStream *stream );

	// PoweredSubsystem
	virtual void	TurnOff( void );

	virtual void	VirtualSlot34( void );

	static CloakingSubsystem *	Create( void ) { return new CloakingSubsystem; }
	static CloakingSubsystem *	Cast( TGObject * );
	CloakingSubsystemProperty *	GetProperty( void );

	bool			IsCloaked( void ) { return cloaked; }
	bool			IsTryingToCloak( void ) { return tryingToCloak; }
	bool			IsCloaking( void ) { return cloakState == 1 || cloakState == 2; }
	bool			IsDecloaking( void ) { return cloakState == 4 || cloakState == 5; }

	void	StartCloaking( void );
	void	StopCloaking( void );
	void	InstantCloak( void );
	void	InstantDecloak( void );
	bool	Unknown55F110( bool );	// TurnOff calls it with false after StopCloaking
	void	Unknown55E640( float fraction );		// the fraction of the cloak done; fades the ship and lights it
	void	Unknown55E6B0( void );					// makes the ship's tree cloakable, with the ambient light for the player's ship
	void	Unknown55E800( void );					// the undoing of 0x55E6B0
	void	Unknown55E840( NiObject *object );	// gives every triangle shape of the tree what the fade needs
	void	Unknown55EE10( float fraction, NiObject *object );	// sets every vertex alpha of the tree for the fade
	void	Unknown55F5F0( void );					// reapplies the cloak's fade to the ship's tree while a cloak is on or in progress
	void	Unknown55F660( void );					// the cloak starts: effects and sound
	void	Unknown55F6D0( void );					// the cloak is complete: the ship is hidden
	void	Unknown55F770( void );					// the decloak starts: the ship shows again, with a sound
	void	Unknown55F7F0( void );					// the decloak is complete: the shields come back after their delay

	static float	GetCloakTime( void ) { return cloakTime; }
	static void		SetCloakTime( float time ) { cloakTime = time; }
	static float	GetShieldDelay( void ) { return shieldDelay; }
	static void		SetShieldDelay( float delay ) { shieldDelay = delay; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			StartCloakingHandler( TGEvent *event );
	void			StopCloakingHandler( TGEvent *event );

protected:
	NiPointer<NiAmbientLight>	ambientLight;	// 0xA8 created on first use by 0x55E6B0 (invented name)
	bool			cloaked;				// 0xAC
	bool			tryingToCloak;			// 0xAD
	int				cloakState;				// 0xB0 1 and 2 while cloaking, 4 and 5 while decloaking (invented name)
	float			unknownB4;				// 0xB4
	bool			unknownB8;				// 0xB8

	static float	cloakTime;				// seconds
	static float	shieldDelay;			// seconds
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
CloakingSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x0055e360
inline bool CloakingSubsystem::IsTypeOf( int type ) {
	if ( type == CLOAKINGSUBSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__CLOAKINGSUBSYSTEM_H__ */
