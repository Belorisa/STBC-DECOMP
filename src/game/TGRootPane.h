/*
===============================================================================

	TGRootPane.h

	The pane at the root of the user interface: it owns the mouse cursor
	(an icon) and a queue of pushed cursors, the object that has grabbed
	the mouse, the object with the true focus (the end of the chain of
	focused children), the stack of objects under the mouse, one per depth
	of the pane tree, and the frame rate display. g_kRootWindow (0x99D040,
	the SWIG variable) is the one instance.

	0xBC bytes, allocated from the pools (the scalar deleting destructor
	0x727760 frees through the pool of that size).

	Evidence: the constructor (0x727620) calls TGPane's with a 1 by 1 size,
	builds the cursor queue (0x4C), stores the vtable 0x897A8C (82 slots,
	none new) and calls Init (0x727790), which clears the rest. GetObjType
	returns 0x208. The SWIG wrappers TGRootPane_* name the cursor and focus
	members and the inline GetTrueFocus (0x74) and GetMouseGrabOwner (0x78);
	SetMouseCursor places the cursor's hot spot by the SWIG constants
	TGRootPane_MCPP_*. The handler names are the strings
	"TGRootPane::MouseHandler", "TGRootPane::KeyboardHandler" and
	"TGRootPane::GamepadHandler". UpdateFPS (0x727A40, called each frame)
	counts frames over 0.5, 5 and 60 seconds and shows them as "%d : %d : %d".

===============================================================================
*/

#ifndef __TGROOTPANE_H__
#define __TGROOTPANE_H__

#include "TGPane.h"
#include "TGQueue.h"

class TGIcon;
class TGParagraph;
class TGIEvent;

// GetObjType's value for a TGRootPane.
#define TGROOTPANE_TYPE			0x208

// Where SetMouseCursor puts the hot spot in the cursor (the SWIG constants
// TGRootPane_MCPP_*); a value outside the masks puts it at the right or
// the bottom.
#define MCPP_VERTICAL_TOP			0x00001
#define MCPP_VERTICAL_CENTER		0x00002
#define MCPP_VERTICAL_BOTTOM		0x00003
#define MCPP_VERTICAL_MASK			0x00003
#define MCPP_HORIZONTAL_LEFT		0x10000
#define MCPP_HORIZONTAL_CENTER		0x20000
#define MCPP_HORIZONTAL_RIGHT		0x30000
#define MCPP_HORIZONTAL_MASK		0x30000

// The frame rate is counted over three periods (UpdateFPS).
#define FPS_PERIODS				3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00727760 ??_GTGRootPane@@UAEPAXI@Z
class TGRootPane : public TGPane {
public:
					TGRootPane( long id = 0 );
	virtual			~TGRootPane( void );

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
	virtual void	Layout( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot63( void *argument );

	void			SetMouseCursor( char *iconGroupName, int iconNum, int hotSpot, float scale );
	void			SetCursorVisible( bool visible, bool layout = true );
	void			PushCursor( char *iconGroupName, int iconNum, int hotSpot, float scale );
	void			PopCursor( void );
	bool			GrabMouse( TGUIObject *object );
	bool			ReleaseMouse( TGUIObject *object );
	void			UpdateTrueFocus( bool layout = true );
	void			ShowFPS( void );
	void			HideFPS( void );
	void			UpdateFPS( void );								// invented name

	TGIcon *		GetMouseCursor( void ) { return cursor; }
	TGUIObject *	GetTrueFocus( void ) { return trueFocus; }
	TGUIObject *	GetMouseGrabOwner( void ) { return mouseGrabOwner; }

	void			Init( void );									// invented name
	void			Shutdown( void );								// invented name
	void			Unknown728D90( void );							// forgets everything without destroying it
	void			Unknown727EE0( TGUIObject *object );			// the object under the mouse at the current depth
	void			Unknown727FA0( TGUIObject *object );			// forgets the object as being under the mouse

	static void		RegisterHandlerNames( void );					// invented name
	static void		RegisterHandlers( void );						// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			GamepadHandler( TGIEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// Panes step mouseDepth as they pass mouse events down the tree.
	friend class	TGPane;

	void			DeleteCursor( void );							// invented name
	void			PassMouseEvent( TGMouseEvent *event );			// invented name
	void			LoseTrueFocusPath( TGUIObject *oldFocus, TGUIObject *ancestor, bool layout );	// invented name
	void			GainTrueFocusPath( TGUIObject *ancestor, bool layout );	// invented name

	TGQueue<TGIcon *>	cursorStack;		// 0x4C the cursors PushCursor replaced
	TGIcon *		cursor;					// 0x5C NULL without a cursor
	float			cursorScale;			// 0x60
	int				cursorHotSpot;			// 0x64 MCPP_*
	int				cursorVisible;			// 0x68 shown while above 0 (SetCursorVisible counts)
	float			hotSpotX;				// 0x6C from the cursor's left, in screen fractions
	float			hotSpotY;				// 0x70 from the cursor's top
	TGUIObject *	trueFocus;				// 0x74
	TGUIObject *	mouseGrabOwner;			// 0x78
	unsigned short	mouseDepth;				// 0x7C the depth Unknown727EE0 stores at
	unsigned short	deepestMouseDepth;		// 0x7E
	unsigned short	grabDepth;				// 0x80 the grab owner's depth
	unsigned short	numMouseObjects;		// 0x82 entries of mouseObjects
	TGUIObject **	mouseObjects;			// 0x84 the object under the mouse at each depth
	bool			showFPS;				// 0x88
	TGParagraph *	fpsText;				// 0x8C NULL while hidden
	unsigned long	lastFPSTime;			// 0x90 timeGetTime at the last frame
	int				unknown94;				// 0x94 set to 1
	unsigned long	fpsValue[FPS_PERIODS];	// 0x98 frames per second over each period
	unsigned long	fpsTime[FPS_PERIODS];	// 0xA4 milliseconds counted in each period
	unsigned long	fpsFrames[FPS_PERIODS];	// 0xB0 frames counted in each period

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The root of the user interface.
extern TGRootPane	g_kRootWindow;

#endif /* !__TGROOTPANE_H__ */
