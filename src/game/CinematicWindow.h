/*
===============================================================================

	CinematicWindow.h

	The main window of a cinematic. Getting the focus renders the player's
	set (unless the ship is warping or the options or console window has
	the focus), switches the player's camera to the cinematic view through
	the script Camera, and places the subtitles for SM_CINEMATIC; losing it
	places them for SM_END_CINEMATIC. Initialize passes the window to the
	script CinematicInterfaceHandlers.

	0x50 bytes. Evidence: the constructor (0x4FD6F0) calls MainWindow's
	with MWT_CINEMATIC (1.0 by 1.0), stores the vtable 0x88E494 (84 slots;
	the destructor, GetObjType, IsTypeOf, the stream functions, the class
	strings, GetClassHandlers, GotFocus, LostFocus, Initialize and Update
	overridden), hides the window and calls SetInteractive( true ); the
	class strings "CinematicWindow", "_p_CinematicWindow" and
	"CinematicWindowPtr"; GetObjType returns 0x8117; the handler table at
	0x986E30 and its mouse and keyboard handlers
	"CinematicInterfaceHandlers.HandleMouse" and
	"CinematicInterfaceHandlers.HandleKeyboard".

===============================================================================
*/

#ifndef __CINEMATICWINDOW_H__
#define __CINEMATICWINDOW_H__

#include "MainWindow.h"

class TGEventHandlerTable;
class TGStream;

// GetObjType's value for a CinematicWindow.
#define CINEMATICWINDOW_TYPE		0x8117

// The window's state flag for taking input during a cinematic (invented name; SetInteractive sets and clears it).
#define CINEMATICWINDOW_INTERACTIVE	0x80000000

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004fd800 ??_GCinematicWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x004fd830 ??1CinematicWindow@@UAE@XZ
class CinematicWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558740 ??0CinematicWindow@@QAE@J@Z @game/InterfaceModule
					CinematicWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					CinematicWindow( void );

	// Converts without a type test: the SWIG wrapper CinematicWindow_Cast passes the
	// pointer through unchanged.
	static CinematicWindow *	Cast( TGObject *object ) { return (CinematicWindow *)object; }

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
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );

	// MainWindow
	virtual void	Initialize( void );
	virtual void	Update( float time );

	void	SetInteractive( bool );
	bool	IsInteractive( void ) { return ( flags & CINEMATICWINDOW_INTERACTIVE ) != 0; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__CINEMATICWINDOW_H__ */
