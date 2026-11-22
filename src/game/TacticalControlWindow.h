/*
===============================================================================

	TacticalControlWindow.h

	The tactical interface's window: the ship and target displays, the
	weapons controls, the target menu, the radar and the tactical menu.
	There is one at a time (tacticalControlWindow).

	0x108 bytes. Evidence: the constructor (0x509750) builds the pane at
	full screen, clears the two tables of flags at 0x4C and 0x8C through
	0x509C70 and 0x509CA0 (an index of -1 clears all 64), the flags at
	0xCC and 0xCD and the eight interface pointers 0xD0 to 0xEC, and builds
	the list at 0xF0 (two items per block); AddMenuToList (0x509BD0)
	appends a menu's ID to it unless it is there. The SWIG inline wrappers
	name the pointers and give their types (Get/SetShipDisplay at 0xD0,
	EnemyShipDisplay 0xD4, WeaponsDisplay 0xD8, WeaponsControl 0xDC,
	TargetMenu 0xE0, RadarDisplay 0xE4, RadarToggle 0xE8, TacticalMenu
	0xEC) and the flag at 0xCD (IgnoreNextMouseKeyEvent). The vtable
	(0x88E9F8) has TGPane's 82 slots and two of its own, 82 and 83; the
	next vtable, TacticalWindow's, starts right after it at 0x88EB48.

===============================================================================
*/

#ifndef __TACTICALCONTROLWINDOW_H__
#define __TACTICALCONTROLWINDOW_H__

#include "NiTList.h"
#include "TGPane.h"

class ObjectClass;
class RadarDisplay;
class ShipClass;
class STButton;
class STMenu;
class STTargetMenu;
class STTopLevelMenu;
class ShipDisplay;
class TacWeaponsCtrl;
class WeaponsDisplay;
class TGEvent;
class TGEventHandlerTable;
class TGObject;
class TGStream;
class TGString;

// GetObjType's value for a TacticalControlWindow (the type its Cast checks).
#define TACTICALCONTROLWINDOW_TYPE	0x8112

// The event the radar toggle button sends (the App constant).
#define ET_RADAR_TOGGLE_CLICKED		0x80009F

// Set in the flags (0x28) by SetMousePickFire while the mouse picks the target
// to fire at (invented name). A constant in memory (0x88E9F4, 0x80000000): the
// SWIG wrapper GetMousePickFire and the window's mouse handler (0x509CE0)
// load it.
extern const unsigned int TCW_MOUSE_PICK_FIRE;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00509950 ??_GTacticalControlWindow@@UAEPAXI@Z
class TacticalControlWindow : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558600 ??0TacticalControlWindow@@QAE@J@Z @game/InterfaceModule
					TacticalControlWindow( long id ) : TGPane( id ), menuIDs( 2 ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual			~TacticalControlWindow( void );

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
	virtual void	GotFocus( bool layout = true );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );

	// TGPane
	// The two slots this class adds after TGPane's 82; neither is reconstructed
	// yet. Slot 83 refreshes the radar, weapons and target displays.
	virtual void	VirtualSlot82( void );
	virtual void	VirtualSlot83( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static TacticalControlWindow *	Cast( TGObject * );
	void	SetMousePickFire( bool );
	bool	GetMousePickFire( void ) { return ( flags & TCW_MOUSE_PICK_FIRE ) != 0; }
	STTopLevelMenu *	FindMenu( TGString * );
	TGPane *	GetMenuParentPane( TGString * );
	STTopLevelMenu *	GetOpenMenu( void );
	void	AddMenuToList( STMenu * );

	// The one window (the SWIG wrapper
	// TacticalControlWindow_GetTacticalControlWindow); NULL when there is none.
	static TacticalControlWindow *	GetTacticalControlWindow( void ) { return tacticalControlWindow; }

	ShipDisplay *	GetShipDisplay( void ) { return shipDisplay; }
	ShipDisplay *	GetEnemyShipDisplay( void ) { return enemyShipDisplay; }
	WeaponsDisplay *	GetWeaponsDisplay( void ) { return weaponsDisplay; }
	TacWeaponsCtrl *	GetWeaponsControl( void ) { return weaponsControl; }
	STTargetMenu *	GetTargetMenu( void ) { return targetMenu; }
	RadarDisplay *	GetRadarDisplay( void ) { return radarDisplay; }
	STButton *		GetRadarToggle( void ) { return radarToggle; }
	STMenu *		GetTacticalMenu( void ) { return tacticalMenu; }
	void	SetTacticalMenu( STMenu * tacticalMenu_ ) { tacticalMenu = tacticalMenu_; }
	void	SetRadarToggle( STButton * radarToggle_ ) { radarToggle = radarToggle_; }
	void	SetRadarDisplay( RadarDisplay * radarDisplay_ ) { radarDisplay = radarDisplay_; }
	void	SetTargetMenu( STTargetMenu * targetMenu_ ) { targetMenu = targetMenu_; }
	void	SetWeaponsControl( TacWeaponsCtrl * weaponsControl_ ) { weaponsControl = weaponsControl_; }
	void	SetWeaponsDisplay( WeaponsDisplay * weaponsDisplay_ ) { weaponsDisplay = weaponsDisplay_; }
	void	SetEnemyShipDisplay( ShipDisplay * enemyShipDisplay_ ) { enemyShipDisplay = enemyShipDisplay_; }
	void	SetShipDisplay( ShipDisplay * shipDisplay_ ) { shipDisplay = shipDisplay_; }
	bool	IgnoreNextMouseKeyEvent( void ) { return ignoreNextMouseKeyEvent; }
	void	SetIgnoreNextMouseKeyEvent( bool ignoreNextMouseKeyEvent_ ) { ignoreNextMouseKeyEvent = ignoreNextMouseKeyEvent_; }

	static TacticalControlWindow *	tacticalControlWindow;		// set by the constructor (0x509750), cleared by the destructor; invented name

	// Sets one of the 8 by 8 flags of the table at 0x4C (0x509C70), or clears all of them
	// for a row of -1 (invented names).
	void			SetUnknown4C( int row, int column, bool value );
	// The same for the table at 0x8C (0x509CA0).
	void			SetUnknown8C( int row, int column, bool value );
	// The mouse's pick of the target (0x509CE0): the cursor over a target picks the point the player's
	// ship aims at.
	void			Unknown509CE0( ShipClass *player, ObjectClass *target );

protected:
	void			HandleRadarToggle( TGEvent *event );		// registered under its name (0x509B20)

	static TGEventHandlerTable	classHandlers;		// invented name

	bool			unknown4C[8][8];		// 0x4C set one at a time by 0x509C70; all cleared by index -1
	bool			unknown8C[8][8];		// 0x8C set one at a time by 0x509CA0; all cleared by index -1
	bool			unknownCC;				// 0xCC cleared by the constructor
	bool			ignoreNextMouseKeyEvent;	// 0xCD (the SWIG IgnoreNextMouseKeyEvent)
	ShipDisplay *	shipDisplay;			// 0xD0
	ShipDisplay *	enemyShipDisplay;		// 0xD4
	WeaponsDisplay *	weaponsDisplay;		// 0xD8
	TacWeaponsCtrl *	weaponsControl;		// 0xDC
	STTargetMenu *	targetMenu;				// 0xE0
	RadarDisplay *	radarDisplay;			// 0xE4
	STButton *		radarToggle;			// 0xE8
	STMenu *		tacticalMenu;			// 0xEC
	NiTList<long>	menuIDs;				// 0xF0 the menus AddMenuToList adds, by object ID (invented name)

	// Declared for the SWIG wrappers that call them.
public:
					TacticalControlWindow( void );
	static TacticalControlWindow *	Create( void ) { return new TacticalControlWindow; }	// the SWIG wrapper TacticalControlWindow_Create expands it
};

#endif /* !__TACTICALCONTROLWINDOW_H__ */
