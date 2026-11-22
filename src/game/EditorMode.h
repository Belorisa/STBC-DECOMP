/*
===============================================================================

	EditorMode.h

	What an editor does with the mouse and the keys: the editor passes its
	input to its current mode, and each mode turns it into an edit. The base
	mode selects what the mouse clicks, turns the camera with the left
	button held and turns the editor's menu keys into menu events; the
	placement editor's modes (select, attach waypoint, insert waypoints,
	move) and the background editor's mode derive from it.

	8 bytes (the base mode Editor::CreateModes makes: operator new of 8).
	Evidence: the inline constructor stores the vtable 0x88C2CC and the
	editor at 4 (Editor::CreateModes, 0x497B90); the vtable has 14 slots
	(GridClass's vtable follows at 0x88C304). The editor's handlers call
	the slots: MouseHandler (0x496E50) slots 1 to 5 by button and state,
	KeyboardHandler (0x496F90) slots 7 to 9 by key state, Update (0x496CE0)
	slot 10, SetMode (0x497170) slots 12 and 13; the searches for an object
	to select (0x497C90, 0x497CF0, 0x497D50) ask slot 11. Slot 6 is called
	by neither editor. The names of the class and its slots are invented.

===============================================================================
*/

#ifndef __EDITORMODE_H__
#define __EDITORMODE_H__

class Editor;
class ObjectClass;
class NiPoint2;
class TGEvent;
class TGKeyboardEvent;
class TGMouseEvent;

class EditorMode {
public:
					EditorMode( Editor *editor ) { this->editor = editor; }
	// FUNCTION: STBC 0x00495e70 ??_GEditorMode@@UAEPAXI@Z
	virtual			~EditorMode( void ) {}

	// The mouse, as Editor::MouseHandler passes it on.
	// A left click: the button went up with no drag since it went down.
	virtual void	LeftClick( TGMouseEvent *event );
	// The mouse moved with the left button down, from where it was last.
	virtual void	LeftDrag( NiPoint2 *lastPosition, TGMouseEvent *event );
	// FUNCTION: STBC 0x00495db0 ?RightClick@EditorMode@@UAEXPAVTGMouseEvent@@@Z
	virtual void	RightClick( TGMouseEvent *event ) {}
	// FUNCTION: STBC 0x00495dc0 ?RightDrag@EditorMode@@UAEXPAVNiPoint2@@PAVTGMouseEvent@@@Z
	virtual void	RightDrag( NiPoint2 *lastPosition, TGMouseEvent *event ) {}
	// The mouse moved with no button down.
	// FUNCTION: STBC 0x00495dd0 ?MouseMove@EditorMode@@UAEXPAVTGMouseEvent@@@Z
	virtual void	MouseMove( TGMouseEvent *event ) {}
	// FUNCTION: STBC 0x00495de0 ?VirtualSlot06@EditorMode@@UAEXPAVTGEvent@@@Z
	virtual void	VirtualSlot06( TGEvent *event ) {}

	// The keys, by key state (Editor::KeyboardHandler).
	// FUNCTION: STBC 0x00495df0 ?KeyDown@EditorMode@@UAEXPAVTGKeyboardEvent@@@Z
	virtual void	KeyDown( TGKeyboardEvent *event ) {}
	// A character typed: the editor's menu keys.
	virtual void	KeyTyped( TGKeyboardEvent *event );
	// FUNCTION: STBC 0x00495e00 ?KeyUp@EditorMode@@UAEXPAVTGKeyboardEvent@@@Z
	virtual void	KeyUp( TGKeyboardEvent *event ) {}

	// Each frame the editor has the focus: updates the rendered set's camera.
	virtual void	Update( void );
	// Whether the object may be selected in this mode.
	// FUNCTION: STBC 0x00495e60 ?CanSelect@EditorMode@@UAE_NPAVObjectClass@@@Z
	virtual bool	CanSelect( ObjectClass *object ) { return false; }
	// The editor makes the mode current, or leaves it.
	// FUNCTION: STBC 0x00495e10 ?Start@EditorMode@@UAEXXZ
	virtual void	Start( void ) {}
	// FUNCTION: STBC 0x00495e20 ?End@EditorMode@@UAEXXZ
	virtual void	End( void ) {}

protected:
	Editor *		editor;				// 0x04 the editor the mode works for
};

#endif /* !__EDITORMODE_H__ */
