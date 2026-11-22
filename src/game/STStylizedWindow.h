/*
===============================================================================

	STStylizedWindow.h

	The framed window of the interface: a title bar and border around an
	interior pane that can scroll, be minimized to its title, and be held
	to a maximum or fixed size.

	0xAC bytes (its scalar deleting destructor frees 0xAC).

	Evidence: the constructor (0x530D80) builds the window, constructs
	the name (0x54), stores the vtable 0x890974, copies two strings
	(0x4C, 0x50) that name the script function it calls for 0xA4, and
	clears the sizes. GetObjType returns 0x80F7. The SWIG wrappers
	STStylizedWindow_* name the members: GetMaximumWidth and
	GetMaximumHeight read 0x70 and 0x74 when 0x64 is set and give 1
	otherwise, GetBorderWidth and GetBorderHeight read 0x78 and 0x7C,
	IsFixedSize reads 0x6C, SetTitleBarThickness writes 0x88,
	SetUseScrolling 0xA8, SetMinimizable 0xA9 and SetUseFocusGlass 0xAA.
	The vtable 0x890974 has TGWindow's 83 slots, none new. The window
	calls methods of the Python object it made (0xA4) by the names in
	the table 0x8E34C0, with itself, its exterior pane (the last child)
	and its sub pane.

===============================================================================
*/

#ifndef __STSTYLIZEDWINDOW_H__
#define __STSTYLIZEDWINDOW_H__

#include "TGWindow.h"
#include "TGString.h"
#include "NiColor.h"

class TGBoolEvent;
class TGEvent;
class TGIntEvent;
class TGObject;
class TGPane;
class TGParagraph;
class TGUIObject;

// GetObjType's value for an STStylizedWindow.
#define STSTYLIZEDWINDOW_TYPE	0x80F7

// The window's state flags, above the TGUIO_* and TGWINDOW_* flags
// (invented names).
#define STSW_MINIMIZED			0x00010000	// IsMinimized: only the title shows
#define STSW_IN_LAYOUT			0x00020000	// set while Layout runs, so that it does not run again inside

// The events that drive the window (invented names): its minimize button
// sends a bool event, its scroll arrows an int event (-1 up, 0 stop, 1 down)
// and the repeat timers the periodic scroll events.
#define ET_ST_MINIMIZE				0x800094
#define ET_ST_SCROLL				0x800096
#define ET_ST_PERIODIC_SCROLL_UP	0x800097
#define ET_ST_PERIODIC_SCROLL_DOWN	0x800098

// The Python methods of the window's script object, by their place in
// STStylizedWindow::methodNames (invented names).
enum stylizedWindowMethod_t {
	STSW_METHOD_SETUP,
	STSW_METHOD_RESIZE,
	STSW_METHOD_GET_BORDER_WIDTH,
	STSW_METHOD_UPDATE_SCROLL_ARROWS,
	STSW_METHOD_GET_NAME_PARAGRAPH,
	STSW_METHOD_UPDATE_MINIMIZE,
	STSW_METHOD_UPDATE_GLASS,
	STSW_NUM_METHODS
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005310f0 ??_GSTStylizedWindow@@UAEPAXI@Z
class STStylizedWindow : public TGWindow {
	friend PyObject *	STStylizedWindow_GetStyleInstance( PyObject *self, PyObject *args );
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558280 ??0STStylizedWindow@@QAE@J@Z @game/InterfaceModule
					STStylizedWindow( long id ) : TGWindow( id ) {}
					STStylizedWindow( char *module, char *function, TGString *name, float indentX, float indentY, TGPane *interior,
									  bool fixedSize, float maximumWidth, float maximumHeight, NiColorA *color );
	static STStylizedWindow *	CreateW( char *module, char *function, TGString *name, float indentX, float indentY, TGPane *interior,
												  bool fixedSize, float maximumWidth, float maximumHeight, NiColorA *color ) {
													return new STStylizedWindow( module, function, name, indentX, indentY, interior, fixedSize, maximumWidth, maximumHeight, color );
											}	// the SWIG wrapper STStylizedWindow_CreateW expands it
	// The same with the name as a C string (the SWIG wrapper STStylizedWindow_Create).
	// FUNCTION: STBC 0x00624880 ?Create@STStylizedWindow@@SAPAV1@PAD00MMPAVTGPane@@_NMMPAVNiColorA@@@Z @game/AppWrap
	static STStylizedWindow *	Create( char *module, char *function, char *name, float indentX, float indentY, TGPane *interior,
										bool fixedSize, float maximumWidth, float maximumHeight, NiColorA *color ) {
									if ( name != NULL ) {
										TGString text( name );
										return new STStylizedWindow( module, function, &text, indentX, indentY, interior, fixedSize, maximumWidth, maximumHeight, color );
									}
									return new STStylizedWindow( module, function, NULL, indentX, indentY, interior, fixedSize, maximumWidth, maximumHeight, color );
								}
	virtual			~STStylizedWindow( void );

	// TGObject
	// FUNCTION: STBC 0x00531040 ?GetObjType@STStylizedWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return STSTYLIZEDWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005310b0 ?GetClassName@STStylizedWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STStylizedWindow"; }
	// FUNCTION: STBC 0x005310c0 ?GetSwigTypeName@STStylizedWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STStylizedWindow"; }
	// FUNCTION: STBC 0x005310d0 ?GetPythonClassName@STStylizedWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STStylizedWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x005310e0 ?GetClassHandlers@STStylizedWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotTrueFocusPath( bool layout = true );
	virtual void	LostTrueFocusPath( bool layout = true );
	virtual void	Layout( void );

	// TGPane: the focus goes to the interior.
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	static STStylizedWindow *	Cast( TGObject * );
	TGParagraph *	GetNameParagraph( void );
	// The pane the script object draws the frame in, the window's last child.
	TGPane *		GetExteriorPane( void );
	// The pane that holds the window's contents, inside the sub window
	// (invented name).
	void			GetColor( NiColorA *out ) { *out = color; }
	TGPane *		GetInteriorPane( void ) { return ( (TGWindow *)GetSubPane() )->GetSubPane(); }
	// Sizes the interior to its visible children and the sub pane to it.
	void			InteriorChangedSize( bool force );
	void			SetMinimized( bool layout );
	void			SetNotMinimized( bool layout );
	bool			IsMinimized( void );
	void			SetMinimizable( bool minimizable );
	void			SetFixedSize( float width, float height, bool resize );
	void			SetUseScrolling( bool useScrolling );
	void			SetUseFocusGlass( bool useFocusGlass );
	void			SetTitleBarThickness( float thickness, bool resize );
	void			SetMaximumSize( float width, float height );
	void			ScrollToTop( void );
	void			ScrollToBottom( void );
	// Moves the interior by the amount, within the sub pane; false once an
	// end is reached.
	bool			Scroll( float amount );
	void			EnsureFocusIsVisible( void );

	// The size the window may grow to: 1 (the whole screen) without one.
	float			GetMaximumWidth( void ) { return hasMaximumSize ? maximumWidth : 1.0f; }
	float			GetMaximumHeight( void ) { return hasMaximumSize ? maximumHeight : 1.0f; }
	float			GetBorderWidth( void ) { return borderWidth; }
	float			GetBorderHeight( void ) { return borderHeight; }
	// The room inside the border at the maximum size.
	float			GetMaximumInteriorWidth( void ) { return GetMaximumWidth() - GetBorderWidth(); }
	float			GetMaximumInteriorHeight( void ) { return GetMaximumHeight() - GetBorderHeight(); }
	bool			IsFixedSize( void ) { return fixedSize; }

	// Forgets the interior size, so that InteriorChangedSize resizes again.
	void			Unknown5314F0( void );
	// Scrolls the interior so that the object, below the top, shows.
	void			Unknown531730( TGUIObject *object );
	// Has the script object show the scroll arrows that apply.
	void			UpdateScrollArrows( void );

	// Call the script object's methods of the same names (invented names).
	void			UISwitchIconGroups( void );
	void			UIResize( void );
	void			UIReposition( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleMinimizeEvent( TGBoolEvent *event );
	void			HandleScrollEvent( TGIntEvent *event );
	void			PeriodicScrollUp( TGEvent *event );
	void			PeriodicScrollDown( TGEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	int	IsScrolling( void ) { return scrolling; }
	float	GetScrollViewHeight( void ) { return scrollViewHeight; }
	bool	IsUseFocusGlass( void ) { return useFocusGlass; }
	void	SetScrollViewHeight( float scrollViewHeight_ ) { scrollViewHeight = scrollViewHeight_; }
	float	GetTitleBarThickness( void ) { return titleBarThickness; }
	bool	IsUseScrolling( void ) { return useScrolling; }
	bool	IsMinimizable( void ) { return minimizable; }
	void	SetScrollSpeed( float scrollSpeed_ ) { scrollSpeed = scrollSpeed_; }

protected:
	// Calls a method of the script object with the window, the exterior
	// pane and the sub pane; false without the object or when the call
	// fails (invented names).
	bool			CallPythonMethod( char *method, char *resultFormat, void *result );
	void			CallPythonMethod( char *method );
	// Asks the script object for the border's size (invented name).
	void			UpdateBorderWidth( void );
	// Scrolls by the amount, and while an arrow is held sends the event
	// again on the next frame (invented name).
	void			ScrollAndRepeat( int eventType, float amount );

	char *			pythonModule;			// 0x4C owned; the module of the function that makes the script object
	char *			pythonFunction;			// 0x50 owned
	TGString		name;					// 0x54
	bool			hasMaximumSize;			// 0x64 invented name
	int				scrolling;				// 0x68 1 up, -1 down, 0 not scrolling (invented name)
	bool			fixedSize;				// 0x6C
	float			maximumWidth;			// 0x70
	float			maximumHeight;			// 0x74
	float			borderWidth;			// 0x78
	float			borderHeight;			// 0x7C
	float			scrollViewHeight;				// 0x80 -1 at first
	float			scrollSpeed;			// 0x84 0.5 at first: screen heights a second (invented name)
	float			titleBarThickness;		// 0x88
	float			lastInteriorWidth;		// 0x8C -1: unknown (invented name)
	float			lastInteriorHeight;		// 0x90 -1: unknown (invented name)
	NiColorA		color;					// 0x94
	PyObject *		pythonObject;			// 0xA4 the script object pythonFunction made
	bool			useScrolling;			// 0xA8
	bool			minimizable;			// 0xA9
	bool			useFocusGlass;			// 0xAA

	static char *	methodNames[STSW_NUM_METHODS];		// invented name
	static TGEventHandlerTable	classHandlers;		// invented name

	// The name the window shows (the SWIG wrappers STStylizedWindow_SetName and
	// STStylizedWindow_GetName).
public:
	void	SetName( TGString *name_ ) { name = *name_; }
	TGString &	GetName( void ) { return name; }
};

/*
================
STStylizedWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00531050
inline bool STStylizedWindow::IsTypeOf( int type ) {
	if ( type == STSTYLIZEDWINDOW_TYPE ) {
		return true;
	}
	return TGWindow::IsTypeOf( type );
}

struct _object;
typedef _object PyObject;

// The Python methods STStylizedWindow.GetStyleInstance (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		STStylizedWindow_GetStyleInstance( PyObject *self, PyObject *args );

#endif /* !__STSTYLIZEDWINDOW_H__ */
