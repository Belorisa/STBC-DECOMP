/*
===============================================================================

	ConsoleWindow.h

	The main window (MWT_CONSOLE) that holds the Python console.

	0x50 bytes. Evidence: the constructor (0x50EBC0) calls MainWindow's
	with MWT_CONSOLE, 1.0 by 0.5, stores the vtable 0x88F098 (84 slots;
	the destructor, GetObjType, IsTypeOf, the class strings,
	GetClassHandlers and SetFocus overridden), hides the window and adds the
	console pane (font "Tahoma", 14) and its background icon ("Console");
	the class strings are "ConsoleWindow", "_p_ConsoleWindow" and
	"ConsoleWindowPtr"; GetObjType returns 0x8113. The top window's
	constructor (0x50C430) allocates 0x50 bytes for it. No SWIG wrappers
	name its methods.

===============================================================================
*/

#ifndef __CONSOLEWINDOW_H__
#define __CONSOLEWINDOW_H__

#include "MainWindow.h"

class TGConsole;
class TGEventHandlerTable;

// GetObjType's value for a ConsoleWindow.
#define CONSOLEWINDOW_TYPE		0x8113

// FUNCTION: STBC 0x0050ed80 ??_GConsoleWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x0050edb0 ??1ConsoleWindow@@UAE@XZ
class ConsoleWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558450 ??0ConsoleWindow@@QAE@J@Z @game/InterfaceModule
					ConsoleWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					ConsoleWindow( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGPane
	// The focus always goes to the first child, the console.
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	// The console pane; NULL when there is none (invented name; the chat
	// window's functions call it).
	TGConsole *		GetConsole( void );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__CONSOLEWINDOW_H__ */
