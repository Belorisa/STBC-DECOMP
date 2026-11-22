/*
===============================================================================

	PlacementEditorMode.h

	The placement editor's editing modes. The base mode selects placement
	objects and ships, flies the camera with the arrow, page and home/end
	keys at the editor's speed, turns the placement editor's menu keys into
	its menu events and makes waypoints. Three modes derive from it, each
	with a line of help shown at the bottom of the editor while it is the
	current mode: attaching the selected waypoint before the one clicked,
	inserting waypoints after the selected one, and moving the selected
	object with the mouse and the keys.

	Evidence: PlacementEditor::CreateModes (0x49E3B0) allocates 0x14 bytes
	for the base mode (constructor 0x49F1A0: the editor at 4, the ten key
	flags 8 to 0x11 cleared, the vtable 0x88C418: EditorMode's 14 slots and
	one new) and 0x18 bytes for each of the others, whose constructors
	(0x49FC20, 0x49FE60, 0x4A0060) run the base's, store their vtables
	(0x88C458, 0x88C494, 0x88C4D0, 15 slots each) and make the help
	paragraph at 0x14 from the strings "Click on the waypoint to attach
	after the currently selected ...", "Insert waypoints, and they will
	automatically attach themselves ..." and "Move mode: If an object is
	selected, it will be moved instead ...". The key flags are set by
	KeyDown (0x49F3A0) and cleared by KeyUp (0x49F620) for WC_LEFT,
	WC_RIGHT, WC_UP, WC_DOWN, WC_PAGEUP, WC_PAGEDOWN, WC_END and WC_HOME;
	End (0x49FB30) clears all ten. The class names are invented.

===============================================================================
*/

#ifndef __PLACEMENTEDITORMODE_H__
#define __PLACEMENTEDITORMODE_H__

#include "EditorMode.h"

class PlacementEditor;
class TGParagraph;

// The base mode: selection, the camera keys and new waypoints.
// FUNCTION: STBC 0x0049f1f0 ??_GPlacementEditorMode@@UAEPAXI@Z
// FUNCTION: STBC 0x0049f230 ??1PlacementEditorMode@@UAE@XZ
class PlacementEditorMode : public EditorMode {
public:
					PlacementEditorMode( PlacementEditor *editor );

	// EditorMode
	virtual void	RightClick( TGMouseEvent *event );
	// FUNCTION: STBC 0x0049f1e0 ?VirtualSlot06@PlacementEditorMode@@UAEXPAVTGEvent@@@Z
	virtual void	VirtualSlot06( TGEvent *event ) {}
	virtual void	KeyDown( TGKeyboardEvent *event );
	virtual void	KeyTyped( TGKeyboardEvent *event );
	virtual void	KeyUp( TGKeyboardEvent *event );
	virtual void	Update( void );
	virtual bool	CanSelect( ObjectClass *object );
	virtual void	End( void );

	// Makes a waypoint with a free "Placement %d" name in the rendered set
	// and selects it.
	virtual ObjectClass *	CreateWaypoint( void );

protected:
	PlacementEditor *	GetEditor( void ) { return (PlacementEditor *)editor; }

	bool			leftKey;			// 0x08 WC_LEFT held
	bool			rightKey;			// 0x09 WC_RIGHT
	bool			unknown0A;			// 0x0A cleared with the others; no key sets it
	bool			unknown0B;			// 0x0B likewise
	bool			upKey;				// 0x0C WC_UP
	bool			downKey;			// 0x0D WC_DOWN
	bool			endKey;				// 0x0E WC_END
	bool			pageDownKey;		// 0x0F WC_PAGEDOWN
	bool			pageUpKey;			// 0x10 WC_PAGEUP
	bool			homeKey;			// 0x11 WC_HOME
};

// Attaches the selected waypoint before the waypoint clicked, then goes
// back to the base mode.
// FUNCTION: STBC 0x0049FCB0 ??_GAttachWaypointMode@@UAEPAXI@Z
class AttachWaypointMode : public PlacementEditorMode {
public:
					AttachWaypointMode( PlacementEditor *editor );
	virtual			~AttachWaypointMode( void );

	virtual void	LeftClick( TGMouseEvent *event );
	virtual bool	CanSelect( ObjectClass *object );
	virtual void	Start( void );
	virtual void	End( void );

protected:
	TGParagraph *	help;				// 0x14 shown at the bottom of the editor while current
};

// Each waypoint made follows the one selected.
// FUNCTION: STBC 0x0049FEF0 ??_GInsertWaypointsMode@@UAEPAXI@Z
class InsertWaypointsMode : public PlacementEditorMode {
public:
					InsertWaypointsMode( PlacementEditor *editor );
	virtual			~InsertWaypointsMode( void );

	virtual void	Start( void );
	virtual void	End( void );
	virtual ObjectClass *	CreateWaypoint( void );

protected:
	TGParagraph *	help;				// 0x14
};

// The mouse and the camera keys move the selected object instead of the
// camera.
// FUNCTION: STBC 0x004A00F0 ??_GMoveMode@@UAEPAXI@Z
class MoveMode : public PlacementEditorMode {
public:
					MoveMode( PlacementEditor *editor );
	virtual			~MoveMode( void );

	virtual void	LeftDrag( NiPoint2 *lastPosition, TGMouseEvent *event );
	virtual void	Update( void );
	virtual void	Start( void );
	virtual void	End( void );

protected:
	TGParagraph *	help;				// 0x14
};

#endif /* !__PLACEMENTEDITORMODE_H__ */
