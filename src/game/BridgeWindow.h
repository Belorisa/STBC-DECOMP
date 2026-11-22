/*
===============================================================================

	BridgeWindow.h

	The main window of the bridge view. Its Initialize, GotFocus and
	LostFocus pass the window to the scripts (Bridge.BridgeMenus and
	BridgeHandlers); getting the focus makes the bridge set (or the set the
	top window remembered) the rendered one and brings the subtitles to the
	front, placed for SM_BRIDGE.

	0x50 bytes. Evidence: the constructor (0x4FB750) calls MainWindow's
	with MWT_BRIDGE (1.0 by 1.0) and stores the vtable 0x88E1F4 (84 slots;
	the destructor, GetObjType, IsTypeOf, the class strings, GetClassHandlers,
	GotFocus, LostFocus and Initialize overridden); the class strings
	"BridgeWindow", "_p_BridgeWindow" and "BridgeWindowPtr"; the handler
	table at 0x986C38 and its mouse and keyboard handlers
	"BridgeHandlers.HandleMouse" and "BridgeHandlers.HandleKeyboard".

===============================================================================
*/

#ifndef __BRIDGEWINDOW_H__
#define __BRIDGEWINDOW_H__

#include "MainWindow.h"

class TGEventHandlerTable;
class TGObject;

// GetObjType's value for a BridgeWindow (the type its Cast checks).
#define BRIDGEWINDOW_TYPE		0x8110

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004fb830 ??_GBridgeWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x004fb860 ??1BridgeWindow@@UAE@XZ
class BridgeWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558550 ??0BridgeWindow@@QAE@J@Z @game/InterfaceModule
					BridgeWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					BridgeWindow( void );

	static BridgeWindow *	Cast( TGObject * );

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

#endif /* !__BRIDGEWINDOW_H__ */
