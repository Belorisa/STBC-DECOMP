/*
===============================================================================

	ModalDialogWindow.h

	The window that asks the player a question (a message and one or two
	buttons), or whether to quit. Its buttons send events of their own to
	the window, whose handlers pass on the events the callers gave and
	close the dialog.

	0x64 bytes. Evidence: the constructor (0x5023C0) calls MainWindow's
	with MWT_MODAL_DIALOG, 1.0 by 1.0, stores the vtable 0x88E5FC (84
	slots; the destructor, GetObjType, IsTypeOf, the stream functions,
	FixUpPointers, PostFixUp, the class strings, GetClassHandlers,
	SetNotVisible and Update overridden), clears 0x50-0x60 and calls
	SetNotVisible; the class strings "ModalDialogWindow",
	"_p_ModalDialogWindow" and "ModalDialogWindowPtr"; the handler table at
	0x987168 and the handler names "ModalDialogWindow::...Handler"
	(0x502FA0); the top window's constructor (0x50C430) allocates 0x64
	bytes for it.

===============================================================================
*/

#ifndef __MODALDIALOGWINDOW_H__
#define __MODALDIALOGWINDOW_H__

#include "MainWindow.h"

// Event types of the dialog's buttons (the App constants), and the handlers the class registers for them:
// OkayHandler, CancelHandler, ExitGameHandler and ExitProgramHandler.
#define ET_OKAY						0x8000CE
#define ET_CANCEL					0x8000CF
#define ET_EXIT_GAME				0x8000D0
#define ET_EXIT_PROGRAM				0x8000D1

// GetObjType's value for a ModalDialogWindow.
#define MODALDIALOGWINDOW_TYPE		0x811B

class STButton;
class TGEvent;
class TGEventHandlerTable;
class TGIntEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGObject;
class TGStream;
class TGString;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005024e0 ??_GModalDialogWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x00502510 ??1ModalDialogWindow@@UAE@XZ
class ModalDialogWindow : public MainWindow {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					ModalDialogWindow( void );
					// The window the stream loader makes before reading it (0x502520).
					ModalDialogWindow( long id );
	void	SetAsKeyboardConfig( bool keyboardConfig_ ) { keyboardConfig = keyboardConfig_; }

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

	// TGUIObject
	virtual void	SetNotVisible( bool layout = true );

	// MainWindow
	virtual void	Update( float time );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static ModalDialogWindow *	Cast( TGObject * );
	void	Run( TGString *, TGString *, TGString *, TGEvent *, TGString *, TGEvent * );
	void	RunQuitDialog( void );		// invented name: asks whether to quit, with Exit Program, Exit Game (not in multiplayer) and Cancel
	void	Cancel( void );

	// The handlers, by their registered names; the top window passes the keyboard on to KeyboardHandler.
	void			KeyboardHandler( TGKeyboardEvent *event );

protected:
	void			OkayHandler( TGEvent *event );
	void			CancelHandler( TGEvent *event );
	void			ExitGameHandler( TGEvent *event );
	void			ExitProgramHandler( TGEvent *event );
	void			MouseHandler( TGMouseEvent *event );
	void			RestoreGraphicsModeHandler( TGIntEvent *event );

	static TGEventHandlerTable	classHandlers;		// invented name

	TGEvent *		firstEvent;				// 0x50 a reference held, sent for the first button (invented name)
	TGEvent *		secondEvent;			// 0x54 a reference held, sent for the second button (invented name)
	bool			quitDialog;				// 0x58 the dialog RunQuitDialog built (invented name)
	bool			keyboardConfig;			// 0x59
	bool			mouseGrabbed;				// 0x5A set while one of the buttons holds the mouse during key configuration (invented name)
	STButton *		firstButton;			// 0x5C (invented name)
	STButton *		secondButton;			// 0x60 (invented name)
};

#endif /* !__MODALDIALOGWINDOW_H__ */
