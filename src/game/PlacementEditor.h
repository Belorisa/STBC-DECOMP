/*
===============================================================================

	PlacementEditor.h

	The in-game editors the top window's edit mode shows: the placement
	editor (the MWT_EDITOR main window) and the background editor that
	works with it.

	PlacementEditor places and configures the placement objects of a set
	(waypoints, lights, asteroid fields) and saves them as a script.
	Evidence: the class strings "PlacementEditor", "_p_PlacementEditor"
	and "PlacementEditorPtr" (the copies at 0x4412E0 to 0x441300);
	GetObjType returns 0x80DB (0x441260). The constructor (0x49B310, on a
	pooled 0xA8-byte block) runs Editor's, stores the vtable 0x88AB98 (87
	slots: Editor's with 13 replaced), clears 0x7C to 0x84 and 0x8C, sets
	0xA4 to 8, builds the save dialog (0x98, with the read-only file name
	paragraph at 0x94), the "Edit Name" string dialog (0x9C), the "Select
	Light Type" dialog (0x90) and the "X,Y,Z" paragraph (0xA0). Slot 86
	(0x49E3B0) makes the four editing modes at 0x7C to 0x88; the destructor
	(0x49B460) clears Editor's current mode and deletes them. The handler
	names come from the registration strings "PlacementEditor::...".

	The background editor (BackgroundEditor.h) is an Editor as well.
	0x496C10 makes both and links each to the other at 0x50 and 0x54;
	0x496C30 deletes the MWT_EDITOR window.
	TopWindow::SetEditMode (0x50E5E0) calls the two.

===============================================================================
*/

#ifndef __PLACEMENTEDITOR_H__
#define __PLACEMENTEDITOR_H__

#include "Editor.h"

class AsteroidFieldPlacement;
class LightPlacement;
class TGEvent;
class TGParagraph;
class TGStringDialog;

// GetObjType's value for a PlacementEditor.
#define PLACEMENTEDITOR_TYPE	0x80DB

// The menu and configuration events the placement editor handles (the
// SWIG constants of the App module have no names for them).
#define ET_PLACEMENT_EDITOR_MENU			0x8000BD	// invented name
#define ET_PLACEMENT_EDITOR_LIGHT_CONFIG	0x8000BE	// invented name
#define ET_PLACEMENT_EDITOR_FIELD_CONFIG	0x8000BF	// invented name
#define ET_PLACEMENT_EDITOR_WAYPOINT_CONFIG	0x8000C2	// invented name

// The entries of the placement editor's menu (the values of its
// ET_PLACEMENT_EDITOR_MENU events; the labels of PlacementEditor::CreateMenu).
enum placementEditorMenu_t {
	PEM_CHASE_SELECTED,			// "Chase Selected"
	PEM_CHASE_PREV,				// "Chase Prev (C)"
	PEM_CHASE_NEXT,				// "Chase Next (c)"
	PEM_TARGET_PREV,			// "Target Prev (T)"
	PEM_TARGET_NEXT,			// "Target Next (t)"
	PEM_TOGGLE_GRID,			// "Toggle Grid (g)"
	PEM_TOGGLE_PLACEMENTS,		// "Togl Plcmnts (p)"
	PEM_EDIT_NAME,				// "Edit Name"
	PEM_EDIT_SPEED,				// "Edit Speed"
	PEM_TOGGLE_STATIC,			// "Toggle Static (d)"
	PEM_TOGGLE_NAVPOINT,		// "Toggle NavPoint"
	PEM_SAVE,					// "Save"
	PEM_SWITCH_SETS,			// "Switch Sets"
	PEM_INSERT_OBJECT,			// "Insert Object"
	PEM_CREATE_LIGHT,			// "Create Light"
	PEM_EDIT_LIGHT,				// "Edit Light"
	PEM_OVERHEAD_VIEW,			// "Overhead View (o)"
	PEM_ATTACH_WAYPOINT,		// "Attach Waypoint"
	PEM_PATH_MODE,				// "Path Mode"
	PEM_MOVE_OBJECT,			// "Move Object" ('m')
	PEM_DELETE_OBJECT,			// "Delete Object"
	PEM_DELETE_PLACEMENTS,		// "Delete Placements"
	PEM_CREATE_ASTEROID_FIELD,	// "Create Asteroid Field"
	PEM_EDIT_ASTEROID_FIELD,	// "Edit Asteroid Field"
	PEM_OBJECT_TO_CAMERA,		// "Obj To Cam (q)"
	PEM_CAMERA_TO_OBJECT,		// "Cam To Obj (Q)"
	PEM_CREATE_AMBIENT_LIGHT,	// the light type dialog's "Ambient"
	PEM_CREATE_DIRECTIONAL_LIGHT	// its "Directional"
};

// The buttons of the light configuration dialog (the values of its
// ET_PLACEMENT_EDITOR_LIGHT_CONFIG events; invented names): each pair steps
// a value of the light by 0.1 within 0 to 1.
enum lightConfigButton_t {
	LCB_RED_DOWN,
	LCB_RED_UP,
	LCB_GREEN_DOWN,
	LCB_GREEN_UP,
	LCB_BLUE_DOWN,
	LCB_BLUE_UP,
	LCB_DIMMER_DOWN,
	LCB_DIMMER_UP,
	LCB_DONE
};

// The buttons of the asteroid field configuration dialog (the values of
// its ET_PLACEMENT_EDITOR_FIELD_CONFIG events; invented names).
enum fieldConfigButton_t {
	FCB_APPLY,
	FCB_DONE
};

// The children of the configuration dialogs' panes (invented names): the
// name paragraph, and the value paragraphs of the light's red, green,
// blue and dimmer and of the field's radius, tiles per axis, asteroids
// per tile and size factor.
#define CONFIG_NAME					1
#define LIGHT_CONFIG_RED			3
#define LIGHT_CONFIG_GREEN			7
#define LIGHT_CONFIG_BLUE			11
#define LIGHT_CONFIG_DIMMER			15
#define FIELD_CONFIG_RADIUS			3
#define FIELD_CONFIG_TILES			5
#define FIELD_CONFIG_ASTEROIDS		7
#define FIELD_CONFIG_SIZE			9

// FUNCTION: STBC 0x00441320 ??_GPlacementEditor@@UAEPAXI@Z
class PlacementEditor : public Editor {
public:
	// FUNCTION: STBC 0x00441240 ??0PlacementEditor@@QAE@J@Z @game/UtopiaModule
					PlacementEditor( long id ) : Editor( id ) {}	// the loader's: the stream fills the members
					PlacementEditor( void );
	virtual			~PlacementEditor( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	LostFocus( bool layout = true );

	// Editor
	virtual void	RefreshDisplay( void );					// 0x49B540 shows the camera's and the selected object's positions
	virtual void	CreateMenu( char *title );			// 0x49E120 builds the menu
	virtual void	CreateModes( void );					// 0x49E3B0 makes the editing modes

	// Makes the placement editor (invented name).
	static PlacementEditor *	Create( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MenuHandler( TGEvent *event );
	void			SaveDialogHandler( TGEvent *event );
	void			EditNameDialogHandler( TGEvent *event );
	void			DeleteDialogHandler( TGEvent *event );
	void			SwitchSetsDialogHandler( TGEvent *event );
	void			HandleLightConfig( TGEvent *event );
	void			HandleAsteroidFieldConfig( TGEvent *event );
	void			HandleWaypointConfig( TGEvent *event );

protected:
	// The file name in the path, without its directories, and the length of
	// it before its last '.' (invented name).
	void			GetBaseName( char *path, char **name, int *length );
	// Writes the region script and the mission placements script of the
	// set (0x49EBA0, 0x49EBD0, 0x49EFE0; invented names). The region script
	// makes the set, loads the static placements and the backdrops, and the
	// placements script places the others.
	bool			SaveSet( char *regionFile, char *placementsFile, char *setName );
	bool			WriteRegionScript( char *fileName, char *setName );
	bool			WritePlacementsScript( char *fileName, char *setName );
	// The number a configuration dialog's paragraph shows, and showing one
	// with a decimal (0x49EA90, 0x49EB20; invented names).
	float			GetParagraphValue( TGParagraph *paragraph );
	void			SetParagraphValue( TGParagraph *paragraph, float value );
	// The menu's actions (invented names).
	void			MoveObjectToCamera( void );
	void			MoveCameraToObject( void );
	void			ChaseSelected( void );
	void			ChasePrev( void );
	void			ChaseNext( void );
	void			TargetPrev( void );
	void			TargetNext( void );
	void			ToggleGrid( void );
	void			EditWaypointSpeed( void );
	void			OverheadView( void );
	void			CreateAsteroidField( void );
	void			EditSelectedName( void );
	void			ShowSaveDialog( void );
	void			InsertObject( void );
	void			ShowSwitchSetsDialog( void );
	void			ConfirmDeleteObject( void );
	void			ToggleSelectedStatic( void );
	void			ToggleSelectedNavPoint( void );
	void			ShowLightTypeDialog( void );
	void			ConfigureSelectedLight( void );
	void			ConfigureSelectedField( void );
	void			ToggleAttachWaypointMode( void );
	void			ToggleInsertWaypointsMode( void );
	void			ToggleMoveMode( void );
	// "Delete Placements" (invented name).
	void			DeletePlacements( void );

	// The configuration dialogs of a light and of an asteroid field
	// (0x49D390 and 0x49DA50; invented names).
	void			ConfigureLight( char *title, LightPlacement *light );
	void			ConfigureAsteroidField( AsteroidFieldPlacement *field );

	// Puts the object in front of the rendered set's camera, facing the way
	// it faces (invented name).
	void			PlaceInFrontOfCamera( ObjectClass *object );

	// Builds the save dialog (0x49D020; invented name).
	void			CreateSaveDialog( void );
	// Builds the "Select Light Type" dialog (0x49D1B0; invented name).
	void			CreateLightTypeDialog( void );

	EditorMode *	selectMode;			// 0x7C the default mode, current after CreateModes (invented name)
	EditorMode *	attachWaypointMode;	// 0x80 "Click on the waypoint to attach after ..." (invented name)
	EditorMode *	insertWaypointsMode;	// 0x84 "Insert waypoints, and they will ..." (invented name)
	EditorMode *	moveMode;			// 0x88 "Move mode: ..." (invented name)
	int				placementNumber;	// 0x8C the number tried for the next "Light %d" name (invented name)
	TGDialogWindow *	lightTypeDialog;	// 0x90 (invented name)
	TGParagraph *	saveFileName;		// 0x94 the save dialog's read-only file name (invented name)
	TGStringDialog *	saveDialog;		// 0x98 (invented name)
	TGStringDialog *	editNameDialog;	// 0x9C (invented name)
	TGParagraph *	positionText;		// 0xA0 "X,Y,Z": the positions RefreshDisplay shows (invented name)
	float			speed;				// 0xA4 how fast the camera keys fly, 8 at construction; '=' doubles it, '-' halves it (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name

	friend class	PlacementEditorMode;
	friend class	AttachWaypointMode;
	friend class	InsertWaypointsMode;
	friend class	MoveMode;
};

/*
================
PlacementEditor::GetObjType
================
*/
// FUNCTION: STBC 0x00441260
inline int PlacementEditor::GetObjType( void ) {
	return PLACEMENTEDITOR_TYPE;
}

/*
================
PlacementEditor::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441270
inline bool PlacementEditor::IsTypeOf( int type ) {
	if ( type == PLACEMENTEDITOR_TYPE ) {
		return true;
	}
	return Editor::IsTypeOf( type );
}

/*
================
PlacementEditor::GetClassName
================
*/
// FUNCTION: STBC 0x004412e0
inline char *PlacementEditor::GetClassName( void ) {
	return "PlacementEditor";
}

/*
================
PlacementEditor::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004412f0
inline char *PlacementEditor::GetSwigTypeName( void ) {
	return "_p_PlacementEditor";
}

/*
================
PlacementEditor::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00441300
inline char *PlacementEditor::GetPythonClassName( void ) {
	return "PlacementEditorPtr";
}

/*
================
PlacementEditor::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00441310
inline TGEventHandlerTable *PlacementEditor::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__PLACEMENTEDITOR_H__ */
