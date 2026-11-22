/*
===============================================================================

	MainWindow.h

	One of the top window's full-screen windows (the bridge, the tactical
	view, the console, the options menu and so on), told apart by its type,
	one of the MWT_* constants. TopWindow::FindMainWindow finds one by type,
	and the top window's Initialize and Update are passed down to each.

	0x50 bytes. Evidence: the constructor (0x50E920) calls TGPane's with
	the width and height, stores the vtable 0x88EF48 (84 slots) and the
	type at 0x4C; the destructor is the compiler's (its copy, 0x43ECA0, is
	in UtopiaModule's unit); GetObjType returns 0x810F; the class strings are
	"MainWindow", "_p_MainWindow" and "MainWindowPtr"; the stream functions
	(0x50EB60, 0x50EB90) save the type. Slots 82 and 83 are new and empty
	in the class (their copies are at 0x441110 and 0x4FB770); the top
	window calls them from its Initialize and Update.

===============================================================================
*/

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "TGPane.h"

// GetObjType's value for a MainWindow.
#define MAINWINDOW_TYPE			0x810F

// The main windows (the SWIG constants of the App module).
#define MWT_BRIDGE				0
#define MWT_TACTICAL			1
#define MWT_CONSOLE				2
#define MWT_EDITOR				3
#define MWT_OPTIONS				4
#define MWT_SUBTITLE			5
#define MWT_TACTICAL_MAP		6
#define MWT_CINEMATIC			7
#define MWT_MULTIPLAYER			8
#define MWT_CD_CHECK			9
#define MWT_MODAL_DIALOG		10

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0050ea00 ??_GMainWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x0043eca0 ??1MainWindow@@UAE@XZ
class MainWindow : public TGPane {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					MainWindow( int type, float width, float height );
	// The window the stream loader makes before reading it; the stream
	// gives the type (CDCheckWindow's loader constructor, 0x4FC5E0,
	// expands it).
	// FUNCTION: STBC 0x00558400 ??0MainWindow@@QAE@J@Z @game/InterfaceModule
					MainWindow( long id ) : TGPane( id ) {}

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

	// TGUIObject
	virtual void	GotTrueFocusPath( bool layout = true );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );

	// Passed down by TopWindow::Initialize and TopWindow::Update (invented
	// names).
	// FUNCTION: STBC 0x00441110 ?Initialize@MainWindow@@UAEXXZ
	virtual void	Initialize( void ) {}
	// FUNCTION: STBC 0x004fb770 ?Update@MainWindow@@UAEXM@Z
	virtual void	Update( float time ) {}

	void			ToggleVisibility( void );
	bool			IsWindowActive( void );

	int				GetWindowType( void ) { return windowType; }	// invented name

	int	GetType( void ) { return windowType; }
protected:
	int				windowType;				// 0x4C an MWT_* constant

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
MainWindow::IsTypeOf

The derived windows expand it into their own IsTypeOf.
================
*/
// FUNCTION: STBC 0x0050e960 @game/MainWindow
inline bool MainWindow::IsTypeOf( int type ) {
	if ( type == MAINWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__MAINWINDOW_H__ */
