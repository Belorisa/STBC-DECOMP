/*
===============================================================================

	GraphicsMenu.h

	The options pane that picks the display device, the resolution, the
	colour depth and full screen or windowed, and the record of the mode
	choices it keeps between its uses (GraphicsMenuInfo).

	GraphicsMenu is 0x4C bytes: a TGPane with no members of its own (the
	SWIG wrapper GraphicsMenu_CreateW allocates 0x4C bytes).

	Evidence: the constructor (0x552AA0) calls TGPane's with no ID, stores
	the vtable 0x89293C (TGPane's 82 slots; the destructor, GetObjType,
	IsTypeOf, PostFixUp, the three class names and GetClassHandlers are
	overridden) and records the menu in the GraphicsMenuInfo (0x552BE0).
	GetObjType returns 0x8118. The handler names are
	"GraphicsMenu::HandleButtons", "GraphicsMenu::DeviceChangeHandler" and
	"GraphicsMenu::ResolutionChangePreHandler", for the SWIG event types
	ET_GRAPHICS_BUTTON_CLICKED, ET_GRAPHICS_DEVICE_CHANGE and
	ET_GRAPHICS_RESOLUTION_CHANGE.

	GraphicsMenuInfo is 0x64 bytes (operator new of 0x64 in the dynamic
	initialiser 0x5529D0). The SWIG wrappers GraphicsMenuInfo_*_get/set
	name its members and give their offsets and types: m_pSizeToggle 0x40,
	m_pColorDepthToggle 0x44, m_pApplyButton 0x48 (an STButton),
	ms_pDatabase 0x4C, m_kSavedTitle 0x50, ms_pGraphicsMenu 0x60 (read by
	GraphicsMenu_GetGraphicsMenu). The five flags fill 0x00 to 0x04 and
	the two GraphicsModeInfo (0x1C bytes each) 0x08 and 0x24.

===============================================================================
*/

#ifndef __GRAPHICSMENU_H__
#define __GRAPHICSMENU_H__

#include "TGPane.h"
#include "TGString.h"
#include "GraphicsModeInfo.h"

class STButton;
class STMenu;
class STToggle;
class TGEvent;
class TGLocalizationDatabase;
class TGObject;

// GetObjType's value for a GraphicsMenu (the type its Cast checks).
#define GRAPHICSMENU_TYPE		0x8118

// Event types (the SWIG constants of the App module).
#define ET_GRAPHICS_DEVICE_CHANGE				0x8000B5
#define ET_GRAPHICS_RESOLUTION_CHANGE			0x8000B6
#define ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2	0x8000B7
#define ET_GRAPHICS_RESOLUTION_CHANGE_STAGE3	0x8000B8
#define ET_GRAPHICS_RESOLUTION_CHANGE_STAGE4	0x8000B9
#define ET_GRAPHICS_RESOLUTION_REVERT			0x8000BA
#define ET_GRAPHICS_BUTTON_CLICKED				0x8000BB

class GraphicsMenu;
struct HWND__;
typedef HWND__ *HWND;

/*
================
GraphicsMenuInfo

The graphics options' state that outlives the menu: the modes to return to
and the controls of the menu that is open.
================
*/
class GraphicsMenuInfo {
public:
					GraphicsMenuInfo( void ) {
						dataWasChanged = false;
						modeWasChanged = false;
						weAreCanceling = false;
						weAreUndoing = false;
						sizeToggle = NULL;
						colorDepthToggle = NULL;
						applyButton = NULL;
						graphicsMenu = NULL;
						database = NULL;
						detect = true;
						undoMode.SetDesc( NULL );
						oldMode.SetDesc( NULL );
					}

	bool			dataWasChanged;			// 0x00
	bool			modeWasChanged;			// 0x01
	bool			weAreCanceling;			// 0x02
	bool			weAreUndoing;			// 0x03
	bool			detect;					// 0x04
	GraphicsModeInfo	undoMode;			// 0x08
	GraphicsModeInfo	oldMode;			// 0x24
	STToggle *		sizeToggle;				// 0x40
	STToggle *		colorDepthToggle;		// 0x44
	STButton *		applyButton;			// 0x48
	TGLocalizationDatabase *	database;	// 0x4C the options strings while a menu is open
	TGString		savedTitle;				// 0x50
	GraphicsMenu *	graphicsMenu;			// 0x60 the open menu, or NULL
};

// The graphics options' state, made at startup (invented name).
extern GraphicsMenuInfo *	graphicsMenuInfo;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00552bb0 ??_GGraphicsMenu@@UAEPAXI@Z
class GraphicsMenu : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558790 ??0GraphicsMenu@@QAE@J@Z @game/InterfaceModule
					GraphicsMenu( long id ) : TGPane( id ) {}
					GraphicsMenu( TGString *title );
	virtual			~GraphicsMenu( void );

	// TGObject
	// FUNCTION: STBC 0x00552af0 ?GetObjType@GraphicsMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return GRAPHICSMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00552b60 ?GetClassName@GraphicsMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "GraphicsMenu"; }
	// FUNCTION: STBC 0x00552b70 ?GetSwigTypeName@GraphicsMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_GraphicsMenu"; }
	// FUNCTION: STBC 0x00552b80 ?GetPythonClassName@GraphicsMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "GraphicsMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00552b90 ?GetClassHandlers@GraphicsMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// A loaded menu is the open one again.
	// FUNCTION: STBC 0x00552ba0 ?PostFixUp@GraphicsMenu@@UAEXXZ
	virtual void	PostFixUp( void ) {
						graphicsMenuInfo->graphicsMenu = this;
						TGPane::PostFixUp();
					}

	static GraphicsMenu *	Cast( TGObject * );
	static GraphicsMenu *	CreateW( TGString *title ) { return new GraphicsMenu( title ); }	// the SWIG wrapper GraphicsMenu_CreateW expands it
	static GraphicsMenu *	GetGraphicsMenu( void ) {
								if ( graphicsMenuInfo != NULL ) {
									return graphicsMenuInfo->graphicsMenu;
								}
								return NULL;
							}

	bool			ChangeDisplayMode( void );
	STMenu *		GetDevicesMenu( void );
	STToggle *		GetSizeToggle( void );
	STToggle *		GetColorDepthToggle( void );
	STMenu *		GetResolutionMenu( void );
	void			ResetToggles( void );
	// Empties the menu and gives it one sub pane of its size; unreferenced
	// (invented name).
	void			Unknown5547F0( void );
	// Reloads and unloads the options strings with the parent detached;
	// unreferenced (invented name).
	void			Unknown554880( void );
	// Sets the size and colour depth toggles from the current mode and what
	// the device can do; nothing without a menu (invented name).
	static void		UpdateToggles( GraphicsMenu *menu );

	// The settings of the mode a change goes to: the undo mode while
	// undoing, the old mode while cancelling, else the current one
	// (invented names). They read no member, but ChangeDisplayMode
	// calls them on the menu.
	int				IsChosenModeFullScreen( void );
	unsigned int	GetChosenModeWidth( void );
	int				GetChosenModeHeight( void );
	int				GetChosenModeFlags( void );
	HWND			GetChosenModeWindow( void );
	Ni3DDeviceDesc *	GetChosenModeDesc( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleButtons( TGEvent *event );
	void			DeviceChangeHandler( TGEvent *event );
	void			ResolutionChangePreHandler( TGEvent *event );

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// Makes this the open menu and loads the options strings (invented name).
	void			Init( void );

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
GraphicsMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00552b00
inline bool GraphicsMenu::IsTypeOf( int type ) {
	if ( type == GRAPHICSMENU_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__GRAPHICSMENU_H__ */
