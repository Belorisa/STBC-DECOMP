/*
===============================================================================

	STTopLevelMenu.h

	A menu at the top of the tactical control window's menu panes (the
	helm, tactical, science... menus). Showing one shows it, with its menu
	pane, in the tactical control window and closes the one open before;
	only one is open at a time (GetOpenMenu). It has no title bar of its
	own: the title pane sits at the top and the sub pane fills the rest.
	A menu may have an owner, the object (a bridge character) whose menu
	it is.

	0x64 bytes (its scalar deleting destructor frees 0x64).

	Evidence: GetObjType returns 0x80E4; the class name is
	"STTopLevelMenu". The vtable 0x891314 has STMenu's 115 slots and adds
	slot 115, CloseMenu (the SWIG wrapper STTopLevelMenu_CloseMenu calls
	it). The wrappers STTopLevelMenu_* name SetResizeSubpane and
	IsResizeSubpane (0x5C), GetOwner and SetOwner (0x60, an ObjectClass)
	and GetOpenMenu (the static at 0x98A34C); the handler names are the
	strings "STTopLevelMenu::MouseHandler", "::KeyboardHandler" and
	"::HandleSelectOption". The inline functions' out-of-line copies were
	kept from a derived menu's unit (0x538010 to 0x5380D0).

===============================================================================
*/

#ifndef __STTOPLEVELMENU_H__
#define __STTOPLEVELMENU_H__

#include "STMenu.h"

class ObjectClass;
class TGIntEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGObject;
class TGStream;

// GetObjType's value for an STTopLevelMenu.
#define STTOPLEVELMENU_TYPE		0x80E4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005380d0 ??_GSTTopLevelMenu@@UAEPAXI@Z
class STTopLevelMenu : public STMenu {
public:
					STTopLevelMenu( TGString *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane );
					STTopLevelMenu( char *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane );
	// FUNCTION: STBC 0x00557c60 ??0STTopLevelMenu@@QAE@J@Z @game/InterfaceModule
					STTopLevelMenu( long id ) : STMenu( id ) {}
	static STTopLevelMenu *	Create( char *name, unsigned int menuFlags, TGEvent *event ) { return new STTopLevelMenu( name, menuFlags, event, NULL ); }	// the SWIG wrapper STTopLevelMenu_Create expands it
	static STTopLevelMenu *	CreateW( TGString *name, unsigned int menuFlags, TGEvent *event ) { return new STTopLevelMenu( name, menuFlags, event, NULL ); }	// the SWIG wrapper STTopLevelMenu_CreateW expands it
	static STTopLevelMenu *	CreateNull( void ) { return NULL; }
	virtual			~STTopLevelMenu( void );

	// TGObject
	// FUNCTION: STBC 0x00538010 ?GetObjType@STTopLevelMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STTOPLEVELMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00538090 ?GetClassName@STTopLevelMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STTopLevelMenu"; }
	// FUNCTION: STBC 0x005380a0 ?GetSwigTypeName@STTopLevelMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STTopLevelMenu"; }
	// FUNCTION: STBC 0x005380b0 ?GetPythonClassName@STTopLevelMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STTopLevelMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x005380c0 ?GetClassHandlers@STTopLevelMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );

	// STMenu
	virtual float	GetDesiredHeight( void );
	virtual TGRect	GetClientArea( void );
	virtual void	ResizeToContents( bool layout = true );
	virtual void	VirtualSlot113( void );
	virtual void	VirtualSlot114( void );

	// Hides the menu and its menu pane and gives the focus back to the
	// tactical control window.
	virtual void	CloseMenu( bool layout = true );

	static STTopLevelMenu *	Cast( TGObject *object );

	void			SetResizeSubpane( bool resize, bool layout = true );
	bool			IsResizeSubpane( void ) { return resizeSubpane; }
	ObjectClass *	GetOwner( void ) { return owner; }
	void			SetOwner( ObjectClass *owner ) { this->owner = owner; }

	// The menu that is open; NULL when none is.
	static STTopLevelMenu *	GetOpenMenu( void ) { return openMenu; }

	// Save and load the open menu, after the menus' colour schemes
	// (invented names).
	static bool		WriteOpenMenu( TGStream *stream );
	static bool		ReadOpenMenu( TGStream *stream );
	static bool		FixUpOpenMenu( void );
	static void		PostFixUpOpenMenu( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			HandleSelectOption( TGIntEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// Hides the parts a top level menu does not show and opens it; the
	// constructors' common part (invented name). The argument is not used.
	void			InitTopLevel( bool layout );
	// Activates the menu and focuses it in its parent, for a click on the
	// title (invented name). The event is not used.
	void			ActivateFromMouse( TGMouseEvent *event );

	bool			resizeSubpane;			// 0x5C
	ObjectClass *	owner;					// 0x60 the object whose menu this is; an ID between ReadFromStream and PostFixUp

	static TGEventHandlerTable	classHandlers;		// invented name
	static STTopLevelMenu *		openMenu;
};

/*
================
STTopLevelMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00538020
inline bool STTopLevelMenu::IsTypeOf( int type ) {
	if ( type == STTOPLEVELMENU_TYPE ) {
		return true;
	}
	return STMenu::IsTypeOf( type );
}

#endif /* !__STTOPLEVELMENU_H__ */
