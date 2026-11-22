/*
===============================================================================

	EngRepairPane.h

	Evidence: the constructors (0x550770 with a size, 0x550E00 with an ID)
	call TGPane's and store the vtable 0x8927F4 (82 slots, TGPane's count);
	it overrides the destructor, GetObjType (0x8109), IsTypeOf, the stream
	slots, the three class strings, GetClassHandlers, Resize and Layout.
	The SWIG constants EngRepairPane_REPAIR_AREA_LABEL to _DIVIDER_2 name
	the eight children the size constructor adds (in that order).

	The engineering pane that lists the repair team assignments and the
	damaged and destroyed systems ("Repair team assignments:", "Damaged
	systems:", "Destroyed systems:"). There is one: the constructor
	(0x550770) stores itself in 0x98B188, which the SWIG wrapper
	EngRepairPane_GetRepairPane returns and RepairSubsystem's handlers
	test before telling the pane about a subsystem.

===============================================================================
*/

#ifndef __ENGREPAIRPANE_H__
#define __ENGREPAIRPANE_H__

#include "TGPane.h"

// GetObjType's value for an EngRepairPane (the type its Cast checks).
#define ENGREPAIRPANE_TYPE		0x8109

class STButton;
class ShipSubsystem;
class RepairSubsystem;
class TGObject;
class TGKeyboardEvent;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00550dd0 ??_GEngRepairPane@@UAEPAXI@Z
class EngRepairPane : public TGPane {
public:
	// The pane's children, in the order the constructor adds them (the SWIG
	// constants EngRepairPane_*).
	enum {
		REPAIR_AREA_LABEL,
		REPAIR_AREA,
		WAITING_AREA_LABEL,
		WAITING_AREA,
		DESTROYED_AREA_LABEL,
		DESTROYED_AREA,
		DIVIDER,
		DIVIDER_2
	};

	virtual			~EngRepairPane( void );

	// TGObject
	// FUNCTION: STBC 0x00550d00 ?GetObjType@EngRepairPane@@UAEHXZ
	virtual int		GetObjType( void ) { return ENGREPAIRPANE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00550d70 ?GetClassName@EngRepairPane@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EngRepairPane"; }
	// FUNCTION: STBC 0x00550d80 ?GetSwigTypeName@EngRepairPane@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EngRepairPane"; }
	// FUNCTION: STBC 0x00550d90 ?GetPythonClassName@EngRepairPane@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EngRepairPanePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00550da0 ?GetClassHandlers@EngRepairPane@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	Layout( void );

	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	static EngRepairPane *	Cast( TGObject * );
	STButton *	GetSubsystemButton( ShipSubsystem * );
	void	ResizeToContents( void );
	// The pane; NULL before it is built.
	static EngRepairPane *	GetRepairPane( void ) { return repairPane; }
	// Shows the subsystem among the destroyed systems; it leaves the
	// list it was on first (invented name).
	void	AddDestroyedSubsystem( ShipSubsystem *subsystem );
	// Takes the subsystem's button off the list that holds it, and lays
	// the pane out again (invented name).
	void	RemoveSubsystem( ShipSubsystem *subsystem );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleKeyboard( TGKeyboardEvent *event );			// handler "EngRepairPane::HandleKeyboard"
	void	HandleSelectOption( TGEvent *event );		// handler "EngRepairPane::HandleSelectOption"
	void	HandleSetPlayer( TGEvent *event );			// handler "EngRepairPane::HandleSetPlayer"

	// RepairSubsystem::HandleSetPlayer gives the pane the new player's
	// repair subsystem and its number of repair teams (invented names).
	void	SetRepairSubsystem( RepairSubsystem *repair );
	void	SetNumRepairTeams( int numRepairTeams );
	// Empties the three lists (invented name).
	void	ClearAreas( void );
	// Brings the three lists in line with the repair subsystem's repair
	// list (invented name).
	void	UpdateAreas( void );
	// Gives the subsystem a button on the repair team assignments or the
	// damaged systems (invented name).
	void	AddSubsystem( ShipSubsystem *subsystem, bool repairing );

protected:
	// The subsystem's button and the list holding it: 0 the repair team
	// assignments, 1 the damaged systems, 2 the destroyed systems (invented
	// name).
	void	FindSubsystemButton( ShipSubsystem *subsystem, STButton **button, int *list );
	// Sizes the repair team assignments for numRepairTeams buttons and the
	// other lists to their contents, and stacks the children (invented name).
	void	ArrangeChildren( void );
	// A new button for the subsystem's place on the lists; NULL without a
	// repair subsystem (invented name).
	STButton *	CreateRepairButton( ShipSubsystem *subsystem );
	// Inserts the button among the repair team assignments, moving the
	// last one to the damaged systems when they are full (invented name).
	void	AddToRepairArea( STButton *button, int index );

	static EngRepairPane *	repairPane;		// set by the constructor (invented name)
	static TGEventHandlerTable	classHandlers;	// invented name

	int				numRepairTeams;			// 0x4C the buttons the repair team assignments hold (the repair property's)
	int				repairSubsystemID;		// 0x50 the player's repair subsystem, 0 without one

	// Declared for the SWIG wrappers that call them.
public:
					EngRepairPane( float width, float height, int numRepairTeams );
					// The stream loader's: the stream fills the members.
					EngRepairPane( long id );
	static EngRepairPane *	Create( float width, float height, int numRepairTeams ) { return new EngRepairPane( width, height, numRepairTeams ); }	// the SWIG wrapper EngRepairPane_Create expands it
};

#endif /* !__ENGREPAIRPANE_H__ */
