/*
===============================================================================

	STSubsystemMenu.h

	The menu of a ship's subsystems in the target list: an item for each
	subsystem that can be targeted (a component menu for a subsystem with
	children), kept up to date as subsystems are destroyed, rebuilt and
	renamed, and the ship's name shown as unknown until the sensors
	identify it.

	0xB0 bytes (its scalar deleting destructor frees 0xB0, and
	STTargetMenu allocates that much).

	Evidence: the constructor (0x535170, called by STTargetMenu 0x538790
	with the ship and an event) builds the menu from the ship's name,
	stores the vtable 0x890C24 (117 slots: STMenu's 115 and two new), keeps
	the ship's ID (0x5C), puts a fill gauge of the hull in place of the
	title bar, builds the items (slot 115) and adds the broadcast handlers
	HandleSubsystemDestroyed, HandleSubsystemRebuilt and HandleNameChange.
	GetObjType returns 0x80F3. The SWIG wrappers STSubsystemMenu_* name
	GetShip, ShowUnknownName and ShowRealName. Slot 116 is the method the
	handler name "STSubsystemMenu::TargetButtonClicked" points at (through
	a thunk that calls the slot).

===============================================================================
*/

#ifndef __STSUBSYSTEMMENU_H__
#define __STSUBSYSTEMMENU_H__

#include "STMenu.h"

class ShipClass;
class ShipSubsystem;
class TGObject;
class TGUIObject;

// GetObjType's value for an STSubsystemMenu.
#define STSUBSYSTEMMENU_TYPE	0x80F3

// The events the items send when clicked (invented names).
#define ET_SUBSYSTEM_BUTTON_CLICKED	0x8000A5
#define ET_COMPONENT_BUTTON_CLICKED	0x8000A6

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005353f0 ??_GSTSubsystemMenu@@UAEPAXI@Z
class STSubsystemMenu : public STMenu {
public:
					STSubsystemMenu( long id );
					STSubsystemMenu( ShipClass *ship, TGEvent *event );
	// FUNCTION: STBC 0x00535480 ??1STSubsystemMenu@@UAE@XZ
	virtual			~STSubsystemMenu( void ) {}

	// TGObject
	// FUNCTION: STBC 0x00535330 ?GetObjType@STSubsystemMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STSUBSYSTEMMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005353b0 ?GetClassName@STSubsystemMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STSubsystemMenu"; }
	// FUNCTION: STBC 0x005353c0 ?GetSwigTypeName@STSubsystemMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STSubsystemMenu"; }
	// FUNCTION: STBC 0x005353d0 ?GetPythonClassName@STSubsystemMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STSubsystemMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x005353e0 ?GetClassHandlers@STSubsystemMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );

	// TGPane
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	// STMenu
	virtual void	Close( bool layout = true );
	virtual void	VirtualSlot112( int state );

	// Adds an item for each subsystem of the ship that can be targeted.
	virtual void	VirtualSlot115( void );
	virtual void	TargetButtonClicked( TGEvent *event );

	static STSubsystemMenu *	Cast( TGObject * );
	ShipClass *		GetShip( void );
	// Names the menu after the kind of object, while the ship is not
	// identified.
	void			ShowUnknownName( void );
	void			ShowRealName( void );
	// Whether the menu shows the ship's own name (invented name).
	bool			IsShowingRealName( void );

	// The item of the subsystem: a component menu's own, or an item of
	// one; NULL without one (invented name).
	TGUIObject *	FindItem( ShipSubsystem *subsystem );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleSubsystemDestroyed( TGEvent *event );
	void			HandleSubsystemRebuilt( TGEvent *event );
	void			HandleNameChange( TGEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	long			shipID;					// 0x5C
	char			unknown60[0x50];		// 0x60 not used by the menu's own code

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STSubsystemMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00535340
inline bool STSubsystemMenu::IsTypeOf( int type ) {
	if ( type == STSUBSYSTEMMENU_TYPE ) {
		return true;
	}
	return STMenu::IsTypeOf( type );
}

#endif /* !__STSUBSYSTEMMENU_H__ */
