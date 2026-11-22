/*
===============================================================================

	TGDialogWindow.h

	A frame window that answers its dialog events by sending an event of
	its own type to a handler object.

	0x60 bytes, allocated from the pools (the scalar deleting destructor
	0x7261C0 frees through the pool of that size).

	Evidence: the vtable 0x896FD8 (83 slots, none new) overrides the
	destructor (0x738C70, which only restores the vtable before
	TGFrameWindow's), the type members, the stream slots (0x7392D0,
	0x739370, 0x7393F0, 0x739420) and GotFocus (0x739260). The stream
	slots write 0x4C, the flag at 0x50, the ID of the object at 0x54 and
	the objects at 0x58 and 0x5C. DialogEventHandler (0x739220) posts a
	copy of the event to the object at 0x54, from the window, with the type
	at 0x4C. The handlers are named by the strings
	"TGDialogWindow::MouseHandler" and
	"TGDialogWindow::DialogEventHandler". GetObjType returns 0x20E. The
	load constructor and the type's inline members are emitted with the UI
	module's class factories (0x7260E0 to 0x7261C0).

===============================================================================
*/

#ifndef __TGDIALOGWINDOW_H__
#define __TGDIALOGWINDOW_H__

#include "TGFrameWindow.h"

class TGEvent;
class TGParagraph;
class TGEventHandlerObject;
class TGMouseEvent;

// GetObjType's value for a TGDialogWindow.
#define TGDIALOGWINDOW_TYPE		0x20E

// The event type DialogEventHandler takes (invented name).
#define ET_DIALOG				0x10002

// The type of the event a dialog sends when none is given: the
// constructor (0x738A90) sets eventType to it (invented name).
#define ET_DIALOG_RESULT		0x10003

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x007261c0 ??_GTGDialogWindow@@UAEPAXI@Z
class TGDialogWindow : public TGFrameWindow {
public:
	// The dialog's flags (the SWIG constants TGDialogWindow_DFT_*): the
	// buttons, the default button and the features.
	enum dialogFlags_t {
		DFT_OK					= 0x0001,
		DFT_CANCEL				= 0x0002,
		DFT_YES					= 0x0004,
		DFT_NO					= 0x0008,
		DFT_ABORT				= 0x0010,
		DFT_RETRY				= 0x0020,
		DFT_CONTINUE			= 0x0040,
		DFT_IGNORE				= 0x0080,
		DFT_HELP				= 0x0100,
		DFT_OKCANCEL			= DFT_OK | DFT_CANCEL,
		DFT_YESNO				= DFT_YES | DFT_NO,
		DFT_YESNOCANCEL			= DFT_YES | DFT_NO | DFT_CANCEL,
		DFT_ABORTRETRYCONTINUE	= DFT_ABORT | DFT_RETRY | DFT_CONTINUE,
		DFT_OKCANCELHELP		= DFT_OK | DFT_CANCEL | DFT_HELP,
		DFT_YESNOHELP			= DFT_YES | DFT_NO | DFT_HELP,
		DFT_DEFAULTBUTTON1		= 0x10000,
		DFT_DEFAULTBUTTON2		= 0x20000,
		DFT_DEFAULTBUTTON3		= 0x40000,
		DFT_DEFAULTBUTTON4		= 0x80000,
		DFT_HIDETITLE			= 0x100000,
		DFT_DRAGGABLE			= 0x200000,
		DFT_BUTTONMASK			= 0xFFFF,
		DFT_FEATUREMASK			= 0xFFFF0000
	};

	// FUNCTION: STBC 0x007260e0 ??0TGDialogWindow@@QAE@J@Z
					TGDialogWindow( long id = 0 ) : TGFrameWindow( id ) {}
					TGDialogWindow( float width, float height, char *message, char *frameName, float frameSize,
						char *buttonFrameName, float buttonFrameSize, int dialogFlags, TGEventHandlerObject *destination );
	virtual			~TGDialogWindow( void );

	// TGObject
	// FUNCTION: STBC 0x00726100 ?GetObjType@TGDialogWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return TGDIALOGWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00726180 ?GetClassName@TGDialogWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGDialogWindow"; }
	// FUNCTION: STBC 0x00726190 ?GetSwigTypeName@TGDialogWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGDialogWindow"; }
	// FUNCTION: STBC 0x007261a0 ?GetPythonClassName@TGDialogWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGDialogWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x007261b0 ?GetClassHandlers@TGDialogWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );

	// Converts without a type test: the SWIG wrapper TGDialogWindow_Cast passes the
	// pointer through unchanged.
	static TGDialogWindow *	Cast( TGObject *object ) { return (TGDialogWindow *)object; }
	static TGDialogWindow *	CreateFromTheme( float, float, char *, int, TGEventHandlerObject * );
	TGParagraph *	GetText( void ) { return text; }		// invented name
	static TGDialogWindow *	Create( float, float, char *, char *, float, char *, float, int, TGEventHandlerObject * );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			DialogEventHandler( TGEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	int	GetEventType( void ) { return eventType; }
	void	SetEventType( int eventType_ ) { eventType = eventType_; }

protected:
	// Adds a button for each of the DFT_* button bits set, in the frame (invented name).
	void			AddButtons( int buttons, char *frameName, float frameSize );
	// Adds a button that sends ET_DIALOG with the button's bit, at position,
	// and moves position past it (invented name).
	void			AddButton( char *label, NiPoint2 *position, int button, char *frameName, float frameSize );
	// Sizes the button pane to its buttons and centres it at the bottom (invented name).
	void			LayoutButtons( void );

	int				eventType;				// 0x4C the type of the event sent to the destination (invented name)
	bool			draggable;				// 0x50 DFT_DRAGGABLE: the body drags the dialog (invented name)
	TGEventHandlerObject *	destination;	// 0x54 (invented name); an ID between ReadFromStream and PostFixUp
	TGParagraph *	text;					// 0x58 the message, the window's second child (invented name)
	TGPane *		buttonPane;				// 0x5C the pane holding the buttons (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGDialogWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00726110
inline bool TGDialogWindow::IsTypeOf( int type ) {
	if ( type == TGDIALOGWINDOW_TYPE ) {
		return true;
	}
	return TGFrameWindow::IsTypeOf( type );
}

#endif /* !__TGDIALOGWINDOW_H__ */
