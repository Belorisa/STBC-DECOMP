/*
===============================================================================

	RepairSubsystem.h

	The ship's repair teams and the list of subsystems they repair.

	0xC0 bytes: the SWIG wrapper RepairSubsystem_Create allocates that much.

	Evidence: the constructor (0x565090) calls PoweredSubsystem's, builds the
	list at 0xA8 (an NiTList with two items per block, inline), stores the
	vtable 0x892E24 (34 slots, none new) and clears the members. GetObjType
	returns 0x8029, the type Cast checks. AddSubsystem (0x565520) looks the
	subsystem up in the list and appends it.

===============================================================================
*/

#ifndef __REPAIRSUBSYSTEM_H__
#define __REPAIRSUBSYSTEM_H__

#include "PoweredSubsystem.h"
#include "NiTList.h"

class RepairSubsystemProperty;
class ShipSubsystem;
class TGObject;

// GetObjType's value for a RepairSubsystem.
#define REPAIRSUBSYSTEM_TYPE		0x8029

// Event types, as the SWIG constants of the App module give them.
#define ET_REPAIR_COMPLETED				0x800074
#define ET_REPAIR_CANNOT_BE_COMPLETED	0x800075
#define ET_REPAIR_INCREASE_PRIORITY		0x800076
#define ET_ADD_TO_REPAIR_LIST			0x8000DF

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class RepairSubsystem : public PoweredSubsystem {
public:
					RepairSubsystem( long id = 0 );
	virtual			~RepairSubsystem( void );

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

	static RepairSubsystem *	Create( void ) { return new RepairSubsystem; }
	static RepairSubsystem *	Cast( TGObject * );
	RepairSubsystemProperty *	GetProperty( void );
	bool	AddSubsystem( ShipSubsystem * );
	void	AddToRepairList( ShipSubsystem *, bool );
	bool	IsBeingRepaired( ShipSubsystem * );
	// Updates the repair pane's areas, when there is a pane (invented name).
	void	Unknown565D30( void );
	// Move the subsystem one place towards the front, or the end, of the
	// repair list; unreferenced (invented names).
	void	Unknown565600( ShipSubsystem *subsystem );
	void	Unknown565750( ShipSubsystem *subsystem );
	// The subsystems being repaired, first to last; EngRepairPane lists
	// them (invented name).
	NiTList<ShipSubsystem *> *	GetRepairList( void ) { return &repairList; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleHitEvent( TGEvent *event );
	void			HandleRepairCompleted( TGEvent *event );
	void			HandleSubsystemRebuilt( TGEvent *event );
	void			HandleRepairCannotBeCompleted( TGEvent *event );
	void			HandleIncreasePriorityEvent( TGEvent *event );
	void			HandleAddToRepairList( TGEvent *event );
	static void		HandleSetPlayer( TGEventHandlerObject *object, TGEvent *event );

protected:
	NiTList<ShipSubsystem *>	repairList;	// 0xA8 the subsystems being repaired (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
RepairSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x005650f0
inline bool RepairSubsystem::IsTypeOf( int type ) {
	if ( type == REPAIRSUBSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__REPAIRSUBSYSTEM_H__ */
