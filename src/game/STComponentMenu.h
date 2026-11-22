/*
===============================================================================

	STComponentMenu.h

	A menu of a subsystem that has child subsystems: its title bar is a
	fill gauge of the subsystem's condition, and its items are the child
	subsystems (STComponentMenuItem) that can be targeted.

	0x64 bytes (its scalar deleting destructor frees 0x64).

	Evidence: the constructor (0x5364B0) builds the menu, stores the vtable
	0x890DFC (STMenu's 115 slots and BuildItems at slot 115), names the title after the
	subsystem, keeps the ship's ID (0x5C) and the subsystem's ID (0x60),
	puts a fill gauge in place of the title bar and adds the items
	(0x5367C0). GetObjType returns 0x80F4. The SWIG wrappers
	STComponentMenu_GetShip and STComponentMenu_GetSubsystem look the
	objects up by the IDs.

===============================================================================
*/

#ifndef __STCOMPONENTMENU_H__
#define __STCOMPONENTMENU_H__

#include "STMenu.h"

class ShipClass;
class ShipSubsystem;
class STComponentMenuItem;
class TGObject;

// GetObjType's value for an STComponentMenu.
#define STCOMPONENTMENU_TYPE	0x80F4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00536750 ??_GSTComponentMenu@@UAEPAXI@Z
class STComponentMenu : public STMenu {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557d30 ??0STComponentMenu@@QAE@J@Z @game/InterfaceModule
					STComponentMenu( long id ) : STMenu( id ) {}
					STComponentMenu( ShipClass *ship, ShipSubsystem *subsystem, TGEvent *event );
	// FUNCTION: STBC 0x00536780 ??1STComponentMenu@@UAE@XZ
	virtual			~STComponentMenu( void ) {}

	// TGObject
	// FUNCTION: STBC 0x00536690 ?GetObjType@STComponentMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STCOMPONENTMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00536710 ?GetClassName@STComponentMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STComponentMenu"; }
	// FUNCTION: STBC 0x00536720 ?GetSwigTypeName@STComponentMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STComponentMenu"; }
	// FUNCTION: STBC 0x00536730 ?GetPythonClassName@STComponentMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STComponentMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00536740 ?GetClassHandlers@STComponentMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGPane
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	static STComponentMenu *	Cast( TGObject * );
	ShipClass *		GetShip( void );
	ShipSubsystem *	GetSubsystem( void );

	// The item of the subsystem; NULL without one (invented name).
	STComponentMenuItem *	FindItem( ShipSubsystem *subsystem );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// Targets the subsystem of the item the event comes from.
	void			TargetButtonClicked( TGEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// Adds an item for each child subsystem that can be targeted and is
	// not destroyed (invented name; vtable slot 115, 0x890FC8).
	virtual void	BuildItems( void );

	long			shipID;					// 0x5C
	long			subsystemID;			// 0x60

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STComponentMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x005366a0
inline bool STComponentMenu::IsTypeOf( int type ) {
	if ( type == STCOMPONENTMENU_TYPE ) {
		return true;
	}
	return STMenu::IsTypeOf( type );
}

#endif /* !__STCOMPONENTMENU_H__ */
