/*
===============================================================================

	TacticalWindow.h

	The main window of the tactical view. Its Initialize, GotFocus and
	LostFocus pass the window to the script TacticalInterfaceHandlers;
	getting the focus renders the player's set (or the set the top window
	remembered), places the subtitles for SM_TACTICAL and takes the tactical
	control window in as the focused child.

	0x50 bytes. Evidence: the constructor (0x50B290) calls MainWindow's
	with MWT_TACTICAL (1.0 by 1.0) and stores the vtable 0x88EB48 (84
	slots; the destructor, GetObjType, IsTypeOf, the class strings,
	GetClassHandlers, GotFocus, LostFocus and Initialize overridden); the
	class strings "TacticalWindow", "_p_TacticalWindow" and
	"TacticalWindowPtr"; the handler table at 0x987688 and its mouse and
	keyboard handlers "TacticalInterfaceHandlers.HandleMouse" and
	"TacticalInterfaceHandlers.HandleKeyboard".

===============================================================================
*/

#ifndef __TACTICALWINDOW_H__
#define __TACTICALWINDOW_H__

#include "MainWindow.h"

class TGEventHandlerTable;
class TGObject;

// GetObjType's value for a TacticalWindow (the type its Cast checks).
#define TACTICALWINDOW_TYPE		0x8111

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0050b360 ??_GTacticalWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x0050b390 ??1TacticalWindow@@UAE@XZ
class TacticalWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005585a0 ??0TacticalWindow@@QAE@J@Z @game/InterfaceModule
					TacticalWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					TacticalWindow( void );

	static TacticalWindow *	Cast( TGObject * );
	static void	SpaceCameraRotation( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );

	// MainWindow
	virtual void	Initialize( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__TACTICALWINDOW_H__ */
