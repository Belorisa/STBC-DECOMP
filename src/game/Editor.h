/*
===============================================================================

	Editor.h

	The base of the in-game editors (the placement editor and the background
	editor): an MWT_EDITOR main window with a menu, the object being edited
	and the editing mode that turns mouse and keys into edits. Every editor
	made is in one list; deleting one deletes the others with it. The menu
	and the keys switch between the editors (the "Next Editor" and "Prev
	Editor" links) and step the selection through the rendered set's
	objects. The selected object is shown in red, its name follows it on
	the screen, and the editor looks through a camera of its own
	("EditorCamera") while it has the focus.

	0x7C bytes. Evidence: the class strings "Editor", "_p_Editor" and
	"EditorPtr" (the copies at 0x441190 to 0x4411B0); GetObjType returns
	0x80DA, the SWIG constant CT_EDITOR (0x441120); the constructor
	(0x496A60) runs MainWindow's for MWT_EDITOR, 1.0 by 1.0, clears the
	members from 0x50 to 0x78, stores the vtable 0x88AA3C (87 slots:
	MainWindow's 84 and three new) and hides the window, then appends the
	editor to the list of editors (0x98320C, count 0x983208); the deleting
	destructor (0x4411D0) frees 0x7C bytes. CreateEditors (0x496C10) stores
	each editor in the other's 0x50 and 0x54. The handler names come from
	the registration strings "Editor::MouseHandler" and the others; the
	menu event is the SWIG constant ET_EDITOR_MENU. The other names are
	invented.

===============================================================================
*/

#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "MainWindow.h"
#include "NiPoint2.h"

class EditorMode;
class NiAVObject;
class NiPoint3;
class ObjectClass;
class PlacementEditor;
class TGDialogWindow;
class TGEvent;
class TGKeyboardEvent;
class TGMouseEvent;
class TGParagraph;
class TGRect;
class TGTextButton;

// GetObjType's value for an Editor (the SWIG constant CT_EDITOR).
#define EDITOR_TYPE			0x80DA

// The editors' menu event: a TGIntEvent whose value is the menu entry (the
// SWIG constant ET_EDITOR_MENU).
#define ET_EDITOR_MENU		0x8000BC

// The entries of the menu every editor has.
enum editorMenu_t {
	EM_TOGGLE_MENU,				// "Hide Menu", '\'
	EM_NEXT_EDITOR,				// "Next Editor", '>'
	EM_PREV_EDITOR,				// "Prev Editor", '<'
	EM_CLEAR_SELECTION,			// "Clear Selection", 'x'
	EM_SELECT_PREV,				// "Select Prev", 'S'
	EM_SELECT_NEXT				// "Select Next", 's'
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004411d0 ??_GEditor@@UAEPAXI@Z
class Editor : public MainWindow {
public:
	// FUNCTION: STBC 0x004410f0 ??0Editor@@QAE@J@Z @game/UtopiaModule
					Editor( long id ) : MainWindow( id ) {}	// the loader's: the stream fills the members
					Editor( void );
	virtual			~Editor( void );

	// TGObject; the out-of-line copies are at 0x441120 to 0x4411C0
	// FUNCTION: STBC 0x00441120 ?GetObjType@Editor@@UAEHXZ
	virtual int		GetObjType( void ) { return EDITOR_TYPE; }
	// FUNCTION: STBC 0x00441130 ?IsTypeOf@Editor@@UAE_NH@Z
	virtual bool	IsTypeOf( int type ) {
		if ( type == EDITOR_TYPE ) {
			return true;
		}
		return MainWindow::IsTypeOf( type );
	}
	// FUNCTION: STBC 0x00441190 ?GetClassName@Editor@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "Editor"; }
	// FUNCTION: STBC 0x004411a0 ?GetSwigTypeName@Editor@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_Editor"; }
	// FUNCTION: STBC 0x004411b0 ?GetPythonClassName@Editor@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EditorPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x004411c0 ?GetClassHandlers@Editor@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );

	// MainWindow
	virtual void	Update( float time );

	// Shows what is selected: the selected object's name over it.
	virtual void	RefreshDisplay( void );
	// Builds the menu window under the title.
	virtual void	CreateMenu( char *title );
	// Makes the editing modes and the first one current.
	virtual void	CreateModes( void );

	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			MenuHandler( TGEvent *event );
	void			ErrorDialogHandler( TGEvent *event );

	// Puts the editor in the top window, in place of the editor there.
	static void		ShowEditor( Editor *editor );

	// The selected object, looked up by its ID in the rendered set.
	ObjectClass *	GetSelectedObject( void );
	// Selects the object, or nothing for NULL.
	void			SelectObject( ObjectClass *object );
	// The object the ray from the camera hits nearest that the mode lets
	// the editor select; NULL when there is none.
	ObjectClass *	PickObject( NiPoint3 *origin, NiPoint3 *direction );

protected:
	// Makes the mode the current one, telling the old one it ends and the
	// new one it starts.
	void			SetMode( EditorMode *mode );

	// The objects of the rendered set the mode lets the editor select,
	// after and before the one with the ID, wrapping round.
	ObjectClass *	FindNextSelectable( long id );
	ObjectClass *	FindPrevSelectable( long id );
	void			SelectPrev( void );
	void			SelectNext( void );

	// Shows the object in red, its whole node tree, until the next
	// ClearHighlight.
	void			Highlight( ObjectClass *object );
	void			HighlightNode( NiAVObject *node, bool top );
	void			ClearHighlight( void );
	// Moves the name paragraph over the object, or removes it for NULL.
	void			ShowObjectName( ObjectClass *object );

	// Adds a button that sends a TGIntEvent of the type and value to the
	// editor: the menu buttons flow left to right in rows, each the fraction
	// of the menu's width, the menu growing to hold them.
	TGTextButton *	AddMenuButton( char *label, int eventType, int value, float width );
	TGTextButton *	CreateButton( char *label, int eventType, int value, TGRect rect );

	// Shows the message in a dialog the editor takes down.
	void			ShowError( char *message );

	// The editor's own camera in the rendered set.
	void			CreateEditorCamera( void );
	void			DeleteEditorCamera( void );

	Editor *		nextEditor;			// 0x50 "Next Editor"
	Editor *		prevEditor;			// 0x54 "Prev Editor"
	TGDialogWindow *	menu;			// 0x58 the menu window CreateMenu makes
	EditorMode *	mode;				// 0x5C the current mode
	bool			leftClicked;		// 0x60 the left button went down, and the mouse has not dragged since
	bool			leftDown;			// 0x61
	bool			rightClicked;		// 0x62
	bool			rightDown;			// 0x63
	NiPoint2		lastMouse;			// 0x64 where the mouse was at the last button down or drag
	long			selectedID;			// 0x6C the selected object's ID, 0 for none
	long			highlightedID;		// 0x70 the object shown in red, 0 for none
	TGParagraph *	nameLabel;			// 0x74 the selected object's name, a child of the editor
	TGTextButton *	lastButton;			// 0x78 the last menu button added

	static TGEventHandlerTable	classHandlers;		// 0x983190

	// Every editor made, in the order made.
	static Editor **	editors;
	static int		numEditors;

	friend PlacementEditor *	CreateEditors( void );
};

// Makes the placement editor and its background editor; the top window
// adds the placement editor.
PlacementEditor *	CreateEditors( void );

// Deletes the editor main window, and the other editors with it.
void				DestroyEditors( void );

#endif /* !__EDITOR_H__ */
