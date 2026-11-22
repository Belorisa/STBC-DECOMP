/*
===============================================================================

	TopWindow.h

	The window above every other: it holds the main windows (MainWindow,
	found by type), switches between the bridge, the tactical view and the
	map, runs the console and the options menu, the cutscene letterbox and
	the screen fades, and gates the keyboard and mouse input. There is one
	(topWindow).

	0x90 bytes, allocated from the pools. Evidence: the constructor
	(0x50C430) calls TGPane's (1.0 by 1.0), stores the vtable 0x88EDEC (86
	slots: TGPane's, with 75 MoveToFront overridden and 82-85 new), sets
	topWindow (0x9878CC), adds the options, console, editor, subtitle and
	modal-dialog windows and calls Initialize; the destructor (0x50C830)
	deletes the letterbox bars at 0x68 and 0x6C that have no parent.
	StartCutscene makes the bars (TGIcons of the LCARS group) and slides
	them in; the fades keep a black TGIcon by ID at 0x70, the time the
	fade ends at 0x74 and the colour faded to at 0x78. ToggleOptionsMenu
	keeps whether the mouse was locked ("fLockMouse") at 0x4E; the
	inline DisableOptionsMenu writes 0x89. GetObjType
	returns 0x810E; the class strings are "TopWindow", "_p_TopWindow" and
	"TopWindowPtr". The SWIG wrappers name slots 82-85 (Allow/Is
	KeyboardInput 0x54, Allow/Is MouseInput 0x55); SetEditMode keeps 0x4C,
	ShowBadConnectionText the paragraph at 0x50, the cutscene functions
	their state at 0x58, AbortFade the fade icon's ID at 0x70 and
	SetLastRenderedSet the set's ID at 0x8C.

===============================================================================
*/

#ifndef __TOPWINDOW_H__
#define __TOPWINDOW_H__

#include "NiColor.h"
#include "TGPane.h"

class MainWindow;
class SetClass;
class TGIcon;
class TGEvent;
class TGIntEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGParagraph;

// GetObjType's value for a TopWindow.
#define TOPWINDOW_TYPE			0x810E

// The input events the key bindings send the top window (the App constants).
#define ET_INPUT_SELF_DESTRUCT					0x8001DD
#define ET_INPUT_DEBUG_TOGGLE_EDIT_MODE			0x8003CC
#define ET_INPUT_TOGGLE_CONSOLE					0x800494
#define ET_INPUT_TOGGLE_OPTIONS					0x800495
#define ET_INPUT_TAB_FOCUS_CHANGE				0x800496
#define ET_INPUT_PRINT_SCREEN					0x800497
#define ET_INPUT_TOGGLE_BRIDGE_AND_TACTICAL		0x800498
#define ET_INPUT_SKIP_EVENTS					0x800364

// The cutscene states (invented names): EndCutscene takes a running
// cutscene out over the time given.
enum cutsceneState_t {
	CUTSCENE_NONE,
	CUTSCENE_STARTING,
	CUTSCENE_RUNNING,
	CUTSCENE_ENDING
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0050c700 ??_GTopWindow@@UAEPAXI@Z
class TopWindow : public TGPane {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005583a0 ??0TopWindow@@QAE@J@Z @game/InterfaceModule
					TopWindow( long id ) : TGPane( id ) {}
	void	DisableOptionsMenu( bool disable ) { optionsMenuDisabled = disable; }
					TopWindow( void );
	virtual			~TopWindow( void );

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

	// TGPane
	virtual void	MoveToFront( TGUIObject *child, bool layout = true );

	// FUNCTION: STBC 0x0050dc70 ?AllowKeyboardInput@TopWindow@@UAEX_N@Z @game/TopWindow
	virtual void	AllowKeyboardInput( bool allow ) { keyboardInputAllowed = allow; }
	// FUNCTION: STBC 0x0050c630 ?IsKeyboardInputAllowed@TopWindow@@UAE_NXZ @game/TopWindow
	virtual bool	IsKeyboardInputAllowed( void ) { return keyboardInputAllowed; }
	// FUNCTION: STBC 0x0050dc80 ?AllowMouseInput@TopWindow@@UAEX_N@Z @game/TopWindow
	virtual void	AllowMouseInput( bool allow ) { mouseInputAllowed = allow; }
	// FUNCTION: STBC 0x0050c640 ?IsMouseInputAllowed@TopWindow@@UAE_NXZ @game/TopWindow
	virtual bool	IsMouseInputAllowed( void ) { return mouseInputAllowed; }

	// The one top window (TopWindow_GetTopWindow reads it).
	static TopWindow *	GetTopWindow( void ) { return topWindow; }
	// Forgets the top window without deleting it, before a saved game is
	// loaded (invented name).
	static void			ForgetTopWindow( void );

	MainWindow *	FindMainWindow( int type );
	bool			IsEditModeEnabled( void ) { return editMode; }
	bool			IsCutsceneMode( void ) { return cutsceneState != CUTSCENE_NONE; }
	void	SetEditMode( bool );
	void	ToggleEditMode( void );
	void	ToggleOptionsMenu( void );
	void	ToggleConsole( void );
	void	ToggleMapWindow( void );
	void	ToggleCinematicWindow( void );
	void	ToggleBridgeAndTactical( void );
	void	ForceBridgeVisible( void );
	void	ForceTacticalVisible( void );
	void	ToggleWireframe( void );
	void	SetLastRenderedSet( SetClass *set );
	SetClass *	GetLastRenderedSet( void );
	void	ShowBadConnectionText( bool );
	void	Initialize( void );
	void	Update( float time );
	void	UpdateFade( void );				// invented name: called every frame by the game module
	void	UpdateCutscene( float time );	// invented name: likewise
	bool	StartCutscene( float, float, bool );
	bool	EndCutscene( float time );
	void	AbortCutscene( void );
	bool	FadeOut( float );
	bool	FadeIn( float );
	void	AbortFade( void );
	bool	IsBridgeVisible( void );
	bool	IsTacticalVisible( void );

	static TopWindow *	topWindow;		// set by the constructor; invented name
	static TGEventHandlerTable	classHandlers;	// invented name

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// The handlers, by their registered names.
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			MouseHandler( TGMouseEvent *event );
	void			ToggleConsoleHandler( TGEvent *event );
	void			ToggleOptionsHandler( TGEvent *event );
	void			ToggleEditHandler( TGEvent *event );
	void			TabFocusHandler( TGEvent *event );
	void			PrintScreenHandler( TGEvent *event );
	void			ToggleBridgeAndTacticalHandler( TGEvent *event );
	void			SelfDestructHandler( TGEvent *event );
	void			ResolutionChangeHandler( TGIntEvent *event );
	void			ResolutionChangeBackHandler( TGIntEvent *event );

protected:
	bool			editMode;				// 0x4C
	bool			unknown4D;				// 0x4D
	bool			mouseWasLocked;			// 0x4E "fLockMouse" was set when the options menu came up (invented name)
	TGParagraph *	badConnectionText;		// 0x50 shown by ShowBadConnectionText (invented name)
	bool			keyboardInputAllowed;	// 0x54
	bool			mouseInputAllowed;		// 0x55
	int				cutsceneState;			// 0x58 a cutsceneState_t
	float			unknown5C;				// 0x5C StartCutscene's and EndCutscene's time
	float			unknown60;				// 0x60 likewise
	float			unknown64;				// 0x64 StartCutscene's second argument
	TGIcon *		letterboxTop;			// 0x68 deleted with the window when it has no parent (invented name)
	TGIcon *		letterboxBottom;		// 0x6C likewise (invented name)
	long			fadeIconID;				// 0x70 the icon of the screen fade (invented name)
	float			fadeEndTime;			// 0x74 the game time the fade reaches its colour (invented name)
	NiColorA		fadeColor;				// 0x78 black for FadeOut, clear for FadeIn (invented name)
	bool			unknown88;				// 0x88
	bool			optionsMenuDisabled;	// 0x89 set by DisableOptionsMenu (invented name)
	long			lastRenderedSetID;		// 0x8C (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static bool		IsFading( TopWindow *window ) { return TGObject::GetTGObjectPtr( window->fadeIconID ) != NULL; }	// the SWIG wrapper TopWindow_IsFading expands it
};

#endif /* !__TOPWINDOW_H__ */
