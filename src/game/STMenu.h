/*
===============================================================================

	STMenu.h

	A menu of the interface: a window whose sub pane (an STSubPane) holds
	the menu's items, opened and closed by its title button.

	Evidence: the vtable 0x890238 has 115 slots; 83 to 114 are new.
	GetObjType returns 0x80E3. The SWIG wrappers STMenu_* name the new
	slots: GetActualHeight (83), GetTitleBarHeight (84),
	GetDesiredHeight (85), GetSubmenu to RemoveItemW (86 to 91, which
	hand the call to the sub pane), GetClientArea (92), the flag pairs
	(93 to 102), Set/Clear/SendActivationEvent (103 to 105), SetName and
	GetName (106, 107), ResetColorScheme (108), Open (109), Close (110)
	and ResizeToContents (111). The flag values are the SWIG constants
	STMenu_STMF_*. Slots 112 to 114 are not named yet.

===============================================================================
*/

#ifndef __STMENU_H__
#define __STMENU_H__

#include "TGWindow.h"
#include "TGParagraph.h"
#include "TGString.h"

class NiColorA;
class STButton;
class STStylizedWindow;
class STSubPane;
class TGEvent;
class TGIntEvent;
class TGObject;

// GetObjType's value for an STMenu.
#define STMENU_TYPE				0x80E3

// The menu's flags, above the TGUIO_* flags (the SWIG constants
// STMenu_STMF_*).
#define STMF_DONT_USE_END_CAP_SPACE	0x01000000
#define STMF_CHOSEN				0x02000000
#define STMF_OPENED				0x04000000
#define STMF_CLICKED_ONCE		0x08000000
#define STMF_AUTO_CHOOSE		0x10000000
#define STMF_CLOSEABLE			0x20000000
#define STMF_OPENABLE			0x40000000
#define STMF_TWO_CLICKS			0x80000000
#define STMF_FLAGS_MASK			0xF0000000
#define STMF_DEFAULTS			( STMF_OPENABLE | STMF_CLOSEABLE )

// The input events a menu handles besides the sub pane's (the SWIG
// constants).
#define ET_INPUT_PRE_SELECT_OPTION	0x800491
#define ET_INPUT_CLOSE_MENU		0x800493

// The menu's children, in the order the menu adds them (invented names):
// the sub pane the window holds, the title pane holding the name, the
// title bar and the icons around it.
#define STMENU_SUB_PANE			0
#define STMENU_TITLE_PANE		1
#define STMENU_TITLE_BAR		2
#define STMENU_MARKER			3
#define STMENU_OPENED_ARROW		4		// shown while an open menu can close
#define STMENU_CLOSED_ARROW		5		// shown while a closed menu can open
#define STMENU_LEFT_END_CAP		6
#define STMENU_RIGHT_END_CAP	7

// The icon numbers of the menu's parts in the LCARS icon group.
#define STMENU_ICON_TITLE_BAR		0xC8
#define STMENU_ICON_MARKER			0xBB9
#define STMENU_ICON_LEFT_END_CAP	0xBBA
#define STMENU_ICON_RIGHT_END_CAP	0xBBB
#define STMENU_ICON_OPENED_ARROW	0xBCC
#define STMENU_ICON_CLOSED_ARROW	0xBCD

// The menu's colour states: the index of the colours a menu shows in
// its colour scheme (STMenu::Unknown5273F0 picks it from the menu's
// flags). States 6 to 8 are not picked by anything seen yet.
#define STMENU_STATE_NORMAL				0
#define STMENU_STATE_HIGHLIGHTED		1
#define STMENU_STATE_OPENED				2
#define STMENU_STATE_DISABLED			3
#define STMENU_STATE_OPENED_HIGHLIGHTED	4
#define STMENU_STATE_OPENED_DISABLED	5
#define STMENU_STATE_SELECTED			9
#define STMENU_NUM_STATES				10

// The menu depths that have their own colour scheme.
#define STMENU_NUM_UI_HEIGHTS			4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00525720 ??_GSTMenu@@UAEPAXI@Z
class STMenu : public TGWindow {
public:
	// The colours of one state: the title bar's and icons' and the
	// name's (invented names).
	struct STMenuStateColors {
		NiColorA *	base;
		NiColorA *	text;
	};

	// The colours a menu takes at one depth (the SWIG type
	// STMenu::STMenuColorScheme), indexed by the STMENU_STATE_* values.
	struct STMenuColorScheme {
		STMenuStateColors	states[STMENU_NUM_STATES];
	};

					// The menu a stream loads; the subclasses' loading constructors expand it.
	// FUNCTION: STBC 0x00557630 ??0STMenu@@QAE@J@Z @game/InterfaceModule
					STMenu( long id ) : TGWindow( id ) {}
					STMenu( TGString *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane );
					STMenu( char *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane );
	virtual			~STMenu( void );

	// TGObject
	// FUNCTION: STBC 0x00525670 ?GetObjType@STMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x005256e0 ?GetClassName@STMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STMenu"; }
	// FUNCTION: STBC 0x005256f0 ?GetSwigTypeName@STMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STMenu"; }
	// FUNCTION: STBC 0x00525700 ?GetPythonClassName@STMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00525710 ?GetClassHandlers@STMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	GotFocus( bool layout = true );
	virtual void	GotTrueFocusPath( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	LostTrueFocusPath( bool layout = true );
	virtual void	GetDesiredSize( NiPoint2 *size );
	virtual void	SetEnabled( bool layout = true );
	virtual void	SetDisabled( bool layout = true );
	virtual void	SetSelected( bool layout = true );
	virtual void	SetNotSelected( bool layout = true );
	virtual void	SetHighlighted( bool layout = true );
	virtual void	SetNotHighlighted( bool layout = true );
	virtual void	Layout( void );
	virtual void	VirtualSlot59( void );

	// TGPane
	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );
	virtual void	RemoveChild( TGUIObject *child, bool layout = true );
	virtual void	DeleteChild( TGUIObject *child, bool layout = true );
	virtual void	SetFocus( TGUIObject *child, bool layout = true );
	virtual TGUIObject *	GetFocus( void );

	virtual float	GetActualHeight( void );
	virtual float	GetTitleBarHeight( void );
	virtual float	GetDesiredHeight( void );
	// FUNCTION: STBC 0x00516ce0 ?GetSubmenu@STMenu@@UAEPAV1@PAD@Z @game/SortedRegionMenu
	virtual STMenu *	GetSubmenu( char *name ) {
		return GetSubmenuW( &TGString( name ) );
	}
	virtual STMenu *	GetSubmenuW( TGString *name );
	// FUNCTION: STBC 0x00516d50 ?GetButton@STMenu@@UAEPAVSTButton@@PAD@Z @game/SortedRegionMenu
	virtual STButton *	GetButton( char *name ) {
		return GetButtonW( &TGString( name ) );
	}
	virtual STButton *	GetButtonW( TGString *name );
	// FUNCTION: STBC 0x00516dc0 ?RemoveItem@STMenu@@UAEXPAD@Z @game/SortedRegionMenu
	virtual void	RemoveItem( char *name ) {
		RemoveItemW( &TGString( name ) );
	}
	virtual void	RemoveItemW( TGString *name );
	virtual TGRect	GetClientArea( void );
	// FUNCTION: STBC 0x00516e20 ?SetTwoClicks@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetTwoClicks( void ) { flags |= STMF_TWO_CLICKS; }
	// FUNCTION: STBC 0x00516e30 ?SetSingleClick@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetSingleClick( void ) { flags &= ~STMF_TWO_CLICKS; }
	// FUNCTION: STBC 0x00516e40 ?SetOpenable@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetOpenable( void ) { flags |= STMF_OPENABLE; }
	// FUNCTION: STBC 0x00516e50 ?SetNotOpenable@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetNotOpenable( void ) { flags &= ~STMF_OPENABLE; }
	// FUNCTION: STBC 0x00516e60 ?SetCloseable@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetCloseable( void ) { flags |= STMF_CLOSEABLE; }
	// FUNCTION: STBC 0x00516e70 ?SetNotCloseable@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetNotCloseable( void ) { flags &= ~STMF_CLOSEABLE; }
	// FUNCTION: STBC 0x00516e80 ?SetClickedOnce@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetClickedOnce( void ) { flags |= STMF_CLICKED_ONCE; }
	// FUNCTION: STBC 0x00516e90 ?ClearClickedOnce@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	ClearClickedOnce( void ) { flags &= ~STMF_CLICKED_ONCE; }
	// FUNCTION: STBC 0x00516ea0 ?SetOpened@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	SetOpened( void ) { flags |= STMF_OPENED; }
	// FUNCTION: STBC 0x00516eb0 ?ClearOpened@STMenu@@UAEXXZ @game/SortedRegionMenu
	virtual void	ClearOpened( void ) { flags &= ~STMF_OPENED; }
	virtual void	SetActivationEvent( TGEvent *event );
	virtual void	ClearActivationEvent( void );
	virtual void	SendActivationEvent( void );
	virtual void	SetName( TGString *name, bool layout = true );
	virtual void	GetName( TGString *name );
	virtual void	ResetColorScheme( void );
	virtual void	Open( bool layout = true );
	virtual void	Close( bool layout = true );
	virtual void	ResizeToContents( bool layout = true );
	virtual void	VirtualSlot112( int state );	// colours the title and icons for the state
	virtual void	VirtualSlot113( void );
	virtual void	VirtualSlot114( void );

	static STMenu *	Cast( TGObject * );
	// The title's text: the first child of the menu's own second child.
	// TGPane's accessor: TGWindow's goes to the subpane's children.
	TGParagraph *	GetText( void ) { return (TGParagraph *)TGPane::Cast( TGPane::GetNthChild( 1 ) )->GetFirstChild(); }
	void	UseFullTitleBar( bool layout );
	void	DisableFullTitleBar( bool layout );
	void	SetChosen( bool chosen, bool layout );
	STStylizedWindow *	GetContainingWindow( void );
	void	ForceUpdate( bool layout = true );

	bool			IsUseEndCapSpace( void ) { return ~( flags >> 24 ) & 1; }	// invented name
	bool			IsChosen( void ) { return ( flags >> 25 ) & 1; }
	bool			IsOpened( void ) { return ( flags >> 26 ) & 1; }
	bool			IsClickedOnce( void ) { return ( flags >> 27 ) & 1; }
	bool			IsAutoChoose( void ) { return ( flags >> 28 ) & 1; }		// invented name
	bool			IsCloseable( void ) { return ( flags >> 29 ) & 1; }		// invented name
	bool			IsOpenable( void ) { return ( flags >> 30 ) & 1; }		// invented name

	void			SetAutoChoose( bool autoChoose ) {
						if ( autoChoose ) {
							flags |= STMF_AUTO_CHOOSE;
						} else {
							flags &= ~STMF_AUTO_CHOOSE;
						}
					}
	void			SetUseEndCapSpace( bool use, bool layout = true ) {
						if ( !use ) {
							flags |= STMF_DONT_USE_END_CAP_SPACE;
						} else {
							flags &= ~STMF_DONT_USE_END_CAP_SPACE;
						}
						if ( layout ) {
							Layout();
						}
					}
	int				IsTwoClicks( void ) { return flags & STMF_TWO_CLICKS; }
	void			SetUseUIHeight( bool use ) { useUIHeight = use; Layout(); }
	static STMenu *	Create( char *name, unsigned int menuFlags, TGEvent *event ) { return new STMenu( name, menuFlags, event, NULL ); }	// the SWIG wrapper STMenu_Create expands it
	static STMenu *	CreateW( TGString *name, unsigned int menuFlags, TGEvent *event ) { return new STMenu( name, menuFlags, event, NULL ); }	// the SWIG wrapper STMenu_CreateW expands it
	static STMenu *	CreateNull( void ) { return NULL; }

	float			Unknown5260B0( void );		// the indent of the menu's items

	// The horizontal offset of the arrows, from the LCARS module (invented name).
	static float	GetArrowOffsetX( bool reload );
	// Save and load the colour schemes' addresses, for the menus that
	// point to them (invented names).
	static bool		WriteColorSchemes( TGStream *stream );
	static bool		ReadColorSchemes( TGStream *stream );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			HandleSelectOption( TGIntEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	void			Unknown527480( void );
	// Takes the colours of the scheme for the menu's state (invented name;
	// STTargetMenu::ResetAffiliationColors calls it on its menus).
	void			Unknown5273F0( void );

protected:
	// Builds the title and the icons; the constructors' common part
	// (invented name). The last argument is not used.
	void			Init( TGString *name, unsigned int menuFlags, TGEvent *event, bool unused );
	// The number of menus the menu sits in, less one for a chosen menu,
	// at least 0 (invented name).
	int				GetUIHeight( void );
	// The sub pane's desired size; nothing without a sub pane (invented name).
	void			GetSubPaneDesiredSize( NiPoint2 *size );
	// The number of menus, other than top level menus, the menu sits in
	// (invented name).
	int				GetMenuDepth( void );
	TGEvent *		activationEvent;		// 0x4C posted, as a copy, when the menu is activated
	STMenuColorScheme *	colorScheme;		// 0x50 the scheme of uiHeight
	int				uiHeight;				// 0x54 the depth the colours were taken for; -1 at first
	bool			useUIHeight;			// 0x58 take the colours of the depth at each layout
	bool			fullTitleBar;			// 0x59 UseFullTitleBar sets it, DisableFullTitleBar clears it

	static TGEventHandlerTable	classHandlers;		// invented name
	// The colour state of each set of flags, by
	// [openable][enabled][opened][highlighted] (invented name).
	static int		colorStates[2][2][2][2];
	static float	arrowOffset;			// GetArrowOffsetX's value; NI_INFINITY until read (invented name)
};

// The colour scheme of each menu depth (the SWIG variable g_kSTMenuColors).
extern STMenu::STMenuColorScheme *	g_kSTMenuColors[STMENU_NUM_UI_HEIGHTS];

/*
================
STMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00525680
inline bool STMenu::IsTypeOf( int type ) {
	if ( type == STMENU_TYPE ) {
		return true;
	}
	return TGWindow::IsTypeOf( type );
}

#endif /* !__STMENU_H__ */
