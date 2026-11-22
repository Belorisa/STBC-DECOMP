/*
===============================================================================

	BackgroundEditor.h

	The editor of the backdrops: it makes backdrop spheres from a texture,
	changes their polygon counts and places them, and saves them as a
	script ("backdrops.py").

	0x84 bytes. Evidence: the class strings "BackgroundEditor",
	"_p_BackgroundEditor" and "BackgroundEditorPtr" (the copies at
	0x441430 to 0x441450); GetObjType returns 0x80DD, the SWIG constant
	CT_BACKGROUND_EDITOR (0x4413B0). The constructor (0x4954F0) runs
	Editor's, stores the vtable 0x88ACF4 (Editor's 87 slots, with slots 84
	to 86 replaced) and builds the "Save Backdrop Objects" string dialog at
	0x7C and the "Create Background Object" one at 0x80; Create (0x495620)
	makes it on a pooled 0x84-byte block and titles it "Background Editor".
	The destructor (0x4414A0) only stores the vtable and runs Editor's. The
	handler names come from the registration strings
	"BackgroundEditor::MenuHandler" and the others; the menu event is
	0x8000C0. The menu's labels name the actions; the action names are
	invented.

	The editor's one mode (vtable 0x88C294, 0xC bytes, made by CreateModes)
	is an EditorMode that selects only backdrop spheres.

===============================================================================
*/

#ifndef __BACKGROUNDEDITOR_H__
#define __BACKGROUNDEDITOR_H__

#include "Editor.h"
#include "EditorMode.h"

class Backdrop;
class BackdropSphere;
class TGStringDialog;

// The background editor's menu event (no SWIG constant names it).
#define ET_BACKGROUND_EDITOR_MENU	0x8000C0	// invented name

// The entries of the background editor's menu (the values of its
// ET_BACKGROUND_EDITOR_MENU events; the labels of CreateMenu).
enum backgroundEditorMenu_t {
	BEM_CREATE_BACKDROP,		// "Create Backdrop"
	BEM_SAVE,					// no menu entry sends it
	BEM_SET_ORIENTATION,		// "Set Orientation"
	BEM_UNUSED,					// no menu entry, no action
	BEM_MORE_POLYS,				// "+ Polys"
	BEM_FEWER_POLYS,			// "- Polys"
	BEM_WIDER_HORIZONTAL_SPAN,	// "+ Horiz Span"
	BEM_NARROWER_HORIZONTAL_SPAN,	// "- Horiz Span"
	BEM_WIDER_VERTICAL_SPAN,	// "+ Vert Span"
	BEM_NARROWER_VERTICAL_SPAN,	// "- Vert Span"
	BEM_MORE_TILING,			// "+ Tiling"
	BEM_LESS_TILING,			// "- Tiling"
	BEM_MOVE_FORWARD,			// "Move Frwd"
	BEM_MOVE_BACK				// "Move Back"
};

// GetObjType's value for a BackgroundEditor (the SWIG constant CT_BACKGROUND_EDITOR).
#define BACKGROUNDEDITOR_TYPE	0x80DD

// FUNCTION: STBC 0x00441470 ??_GBackgroundEditor@@UAEPAXI@Z @game/UtopiaModule
class BackgroundEditor : public Editor {
public:
	// FUNCTION: STBC 0x00441390 ??0BackgroundEditor@@QAE@J@Z @game/UtopiaModule
					BackgroundEditor( long id ) : Editor( id ) {}	// the loader's: the stream fills the members
					BackgroundEditor( void );
	// FUNCTION: STBC 0x004414a0 ??1BackgroundEditor@@UAE@XZ @game/UtopiaModule
	virtual			~BackgroundEditor( void ) {}

	// TGObject
	// FUNCTION: STBC 0x004413b0 ?GetObjType@BackgroundEditor@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return BACKGROUNDEDITOR_TYPE; }
	// FUNCTION: STBC 0x004413c0 ?IsTypeOf@BackgroundEditor@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == BACKGROUNDEDITOR_TYPE ) {
			return true;
		}
		return Editor::IsTypeOf( type );
	}
	// FUNCTION: STBC 0x00441430 ?GetClassName@BackgroundEditor@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "BackgroundEditor"; }
	// FUNCTION: STBC 0x00441440 ?GetSwigTypeName@BackgroundEditor@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_BackgroundEditor"; }
	// FUNCTION: STBC 0x00441450 ?GetPythonClassName@BackgroundEditor@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "BackgroundEditorPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00441460 ?GetClassHandlers@BackgroundEditor@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// Editor
	virtual void	RefreshDisplay( void );					// 0x495670 empty
	virtual void	CreateMenu( char *title );				// 0x495C40
	virtual void	CreateModes( void );					// 0x495D80

	// Makes the background editor (invented name).
	static BackgroundEditor *	Create( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MenuHandler( TGEvent *event );
	void			SaveDialogHandler( TGEvent *event );
	void			CreateDialogHandler( TGEvent *event );

protected:
	// Makes a backdrop sphere of the texture, named for it, faces it the way
	// the camera looks and adds it to the rendered set; NULL when the file
	// does not open (invented name).
	BackdropSphere *	CreateBackdrop( char *textureName );
	// Rebuilds the sphere after a change and updates it (invented name).
	void			RebuildSphere( BackdropSphere *sphere );

	// The menu's actions (invented names).
	void			ShowSaveDialog( void );
	void			ShowCreateDialog( void );
	void			SetOrientation( BackdropSphere *sphere );
	void			AddPolys( BackdropSphere *sphere );
	void			RemovePolys( BackdropSphere *sphere );
	void			WidenHorizontalSpan( BackdropSphere *sphere );
	void			NarrowHorizontalSpan( BackdropSphere *sphere );
	void			WidenVerticalSpan( BackdropSphere *sphere );
	void			NarrowVerticalSpan( BackdropSphere *sphere );
	void			AddTiling( BackdropSphere *sphere );
	void			RemoveTiling( BackdropSphere *sphere );
	void			MoveForward( Backdrop *backdrop );
	void			MoveBack( Backdrop *backdrop );

	TGStringDialog *	saveDialog;		// 0x7C "Save Backdrop Objects" (invented name)
	TGStringDialog *	createDialog;	// 0x80 "Create Background Object" (invented name)

	static TGEventHandlerTable	classHandlers;		// 0x982FC0 (invented name)
};

// The background editor's mode: an EditorMode that selects only backdrop
// spheres (invented name).
// FUNCTION: STBC 0x00495e30 ??_GBackgroundEditorMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00495e50 ??1BackgroundEditorMode@@UAE@XZ
class BackgroundEditorMode : public EditorMode {
public:
					BackgroundEditorMode( BackgroundEditor *editor ) : EditorMode( editor ) {}

	// EditorMode
	virtual bool	CanSelect( ObjectClass *object );

protected:
	int				unknown08;			// 0x08 never set
};

#endif /* !__BACKGROUNDEDITOR_H__ */
