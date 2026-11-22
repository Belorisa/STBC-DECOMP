/*
===============================================================================

	CDCheckWindow.h

	The main window (MWT_CD_CHECK) that checks for the game CD before a
	game starts or a saved game loads, and offers a retry.

	Evidence: the class strings "CDCheckWindow", "_p_CDCheckWindow" and
	"CDCheckWindowPtr", and the handlers "CDCheckWindow::AbortAttemptHandler",
	"::RetryCDHandler" and "::ShowRetryHandler" (of ET_ABORT_ATTEMPT,
	ET_RETRY_CD and ET_SHOW_RETRY_DIALOG). The check (0x4FC610) keeps
	its flag at 0x54, looks for "setup\stbc.exe" on the CD, and when it
	passes either continues the current game's start (Game 0x4060C0) or
	hands the saved game named at 0x58 to
	UtopiaModule::SetInternalLoadFileName; otherwise it sends itself
	ET_SHOW_RETRY_DIALOG, whose handler asks through the modal dialog
	whether to retry (ET_RETRY_CD) or quit (ET_ABORT_ATTEMPT). Game's
	start (0x406040) calls it with true; UtopiaModule's load (0x4447F0)
	names the saved game and calls it with false.

	0x5C bytes. Evidence: the constructor (0x4FC480) calls MainWindow's
	with MWT_CD_CHECK, 0.55 by 0.25, stores the vtable 0x88E344 (84 slots;
	the destructor, GetObjType, IsTypeOf, PostFixUp, the class strings,
	GetClassHandlers and Update overridden), hides the window, sets 0x50 to
	-1.0 and clears 0x54 and 0x58; the top window's constructor (0x50C430)
	allocates 0x5C bytes for it; GetObjType returns 0x8119
	(CT_CD_CHECK_WINDOW).

===============================================================================
*/

#ifndef __CDCHECKWINDOW_H__
#define __CDCHECKWINDOW_H__

#include "MainWindow.h"

class TGEvent;
class TGEventHandlerTable;

// GetObjType's value for a CDCheckWindow (the SWIG constant CT_CD_CHECK_WINDOW).
#define CDCHECKWINDOW_TYPE		0x8119

// The window's events (the SWIG constants of the App module).
#define ET_RETRY_CD				0x800101
#define ET_ABORT_ATTEMPT		0x800102
#define ET_SHOW_RETRY_DIALOG	0x800103

// FUNCTION: STBC 0x004fc5a0 ??_GCDCheckWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x004fc5d0 ??1CDCheckWindow@@UAE@XZ
class CDCheckWindow : public MainWindow {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					CDCheckWindow( void );
					CDCheckWindow( long id );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// MainWindow
	virtual void	Update( float time );

	// Checks for the CD; startingGame chooses what follows a pass
	// (invented names).
	void			RunCheck( bool startingGame );
	// Keeps a copy of the saved game to load once the check passes
	// (invented name).
	void			SetLoadFileName( char *name );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			ShowRetryHandler( TGEvent *event );
	void			RetryCDHandler( TGEvent *event );
	void			AbortAttemptHandler( TGEvent *event );

protected:
	float			unknown50;				// 0x50 -1.0 after construction
	bool			startingGame;			// 0x54 RunCheck's argument (invented name)
	char *			loadFileName;			// 0x58 owned: the saved game to load after a pass (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__CDCHECKWINDOW_H__ */
