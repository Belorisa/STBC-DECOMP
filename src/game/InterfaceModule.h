/*
===============================================================================

	InterfaceModule.h

	The module of the game's interface, with the colours and indents the
	ST* interface classes share. The members of InterfaceModule that the
	SWIG wrappers InterfaceModule_* call are declared, with the slots and
	data the module's object shows.

	0x18 bytes.

	Evidence: g_kInterfaceModule (0x98B320, the SWIG variable) is built by
	0x555950, which calls TGModule's constructor with the priority
	0x20000000, stores the vtable 0x892A84 (TGModule's 8 slots; 0, 1, 2, 4
	and 5 overridden) and clears the bytes at 0x14 and 0x15. The SWIG
	variable setter copies 0x04 to 0x15. The interface's buttons and menus
	set 0x15 when they are highlighted (STButton::SetHighlighted 0x51A040).
	The colour variables (STColors.h) and the indents follow
	g_kInterfaceModule in the SWIG module's variable list (initApp,
	0x65A6C0..).

===============================================================================
*/

#ifndef __INTERFACEMODULE_H__
#define __INTERFACEMODULE_H__

#include "TGModule.h"
#include "STColors.h"

// Event types (the SWIG constants of the App module); each has the
// InterfaceModule handler of its name.
#define ET_UI_SWITCH_ICON_GROUPS	0x800091
#define ET_UI_RESIZE				0x800092
#define ET_UI_REPOSITION			0x800093

class TGEvent;
class TGEventHandlerObject;
class TGFontGroup;
class TGObject;
class TGStream;
class TGPane;
class TGUIObject;

class InterfaceModule : public TGModule {
public:
					InterfaceModule( void );

	void			Mute( void ) { muted = true; }
	void			UnMute( void ) { muted = false; }
	void			PlayClickSound( void ) { playClickSound = true; }

	// TGModule
	virtual void	VirtualSlot00( void );
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );
	virtual void	VirtualSlot04( void );
	virtual void	VirtualSlot05( void );

	static bool	IsUIElement( TGUIObject *, int );
	static bool	HasUIElement( TGUIObject *, int );
	static TGUIObject *	GetFirstUIElement( TGPane *, int );
	static TGUIObject *	GetLastUIElement( TGPane *, int );
	static TGUIObject *	GetPrevUIElement( TGPane *, int );
	static TGUIObject *	GetNextUIElement( TGPane *, int );
	static void	ForceFocusOnObject( TGUIObject * );
	static void	SwitchFonts( TGFontGroup *, TGFontGroup * );
	static void	DoTheRightThing( void );

	// The interface classes' creators, handler names and class handlers,
	// for slots 0 and 1 (invented names, after TGSoundModule's).
	void			RegisterClasses( void );
	void			RegisterHandlerNames( void );
	void			RegisterHandlers( void );

	// The interface's statics in a saved game: the menus' colour schemes,
	// the set course menu and the open top-level menu (invented names, after
	// TGObject's; called by UtopiaModule's save and load).
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );

	// The stream loader's creators of the interface classes (invented names, after UtopiaModule's).
	static TGObject *	CreateSTButton( long id );
	static TGObject *	CreateSTRoundedButton( long id );
	static TGObject *	CreateSTRepairButton( long id );
	static TGObject *	CreateSTToggle( long id );
	static TGObject *	CreateSTMenu( long id );
	static TGObject *	CreateSTTopLevelMenu( long id );
	static TGObject *	CreateSTCharacterMenu( long id );
	static TGObject *	CreateSTSubPane( long id );
	static TGObject *	CreateSTTargetMenuSubPane( long id );
	static TGObject *	CreateSTFillGauge( long id );
	static TGObject *	CreateSTFileMenu( long id );
	static TGObject *	CreateSTFileDialog( long id );
	static TGObject *	CreateSTLoadDialog( long id );
	static TGObject *	CreateSTSaveDialog( long id );
	static TGObject *	CreateSTTargetMenu( long id );
	static TGObject *	CreateSTSubsystemMenu( long id );
	static TGObject *	CreateSTComponentMenu( long id );
	static TGObject *	CreateSTComponentMenuItem( long id );
	static TGObject *	CreateEngRepairPane( long id );
	static TGObject *	CreateSortedRegionMenu( long id );
	static TGObject *	CreateSTWarpButton( long id );
	static TGObject *	CreateSTCheckbox( long id );
	static TGObject *	CreateSTStylizedWindow( long id );
	static TGObject *	CreateSTMissionLog( long id );
	static TGObject *	CreateSTTiledIcon( long id );
	static TGObject *	CreateTopWindow( long id );
	static TGObject *	CreateMainWindow( long id );
	static TGObject *	CreateBridgeWindow( long id );
	static TGObject *	CreateTacticalWindow( long id );
	static TGObject *	CreateTacticalControlWindow( long id );
	static TGObject *	CreateConsoleWindow( long id );
	static TGObject *	CreateOptionsWindow( long id );
	static TGObject *	CreateSubtitleWindow( long id );
	static TGObject *	CreateCinematicWindow( long id );
	static TGObject *	CreateGraphicsMenu( long id );
	static TGObject *	CreateCDCheckWindow( long id );
	static TGObject *	CreateModalDialogWindow( long id );
	static TGObject *	CreateShipDisplay( long id );
	static TGObject *	CreateShieldsDisplay( long id );
	static TGObject *	CreateDamageDisplay( long id );
	static TGObject *	CreateDamageIcon( long id );
	static TGObject *	CreateWeaponsDisplay( long id );
	static TGObject *	CreateRadarDisplay( long id );
	static TGObject *	CreateRadarScope( long id );
	static TGObject *	CreateRadarBlip( long id );
	static TGObject *	CreateTacWeaponsCtrl( long id );
	static TGObject *	CreateEngPowerCtrl( long id );
	static TGObject *	CreateEngPowerDisplay( long id );
	static TGObject *	CreateReticleWindow( long id );
	static TGObject *	CreateNamedReticleWindow( long id );
	static TGObject *	CreatePlayerReticleWindow( long id );
	static TGObject *	CreateMultiplayerWindow( long id );

	// The module's handler functions, by the names they are registered under.
	static void		OverrideHandler( TGEventHandlerObject *object, TGEvent *event );
	static void		HandleUISwitchIconGroups( TGEventHandlerObject *object, TGEvent *event );
	static void		HandleUIResize( TGEventHandlerObject *object, TGEvent *event );
	static void		HandleUIReposition( TGEventHandlerObject *object, TGEvent *event );
	static void		TGParagraphSoundHandler( TGEventHandlerObject *object, TGEvent *event );
	static void		STStylizedWindowMinimizeHandler( TGEventHandlerObject *object, TGEvent *event );

	bool			muted;				// 0x14 cleared by the constructor
	bool			playClickSound;				// 0x15 set when an interface button or menu is highlighted
};

// The interface module (the SWIG variable g_kInterfaceModule).
extern InterfaceModule	g_kInterfaceModule;

// The space around a button's name and beside its marker, in screen
// fractions.
extern float		DEFAULT_ST_INDENT_HORIZ;
extern float		DEFAULT_ST_INDENT_VERT;
extern float		MARKER_INDENT_HORIZ;

// The tactical weapons control's indent, the same kind of fraction.
extern float		TAC_WEAPONS_CTRL_INDENT_HORIZ;

#endif /* !__INTERFACEMODULE_H__ */
