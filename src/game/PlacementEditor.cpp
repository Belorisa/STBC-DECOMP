/*
===============================================================================

	PlacementEditor.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "PlacementEditor.h"
#include "PlacementEditorMode.h"
#include "AsteroidFieldPlacement.h"
#include "CameraObjectClass.h"
#include "LightPlacement.h"
#include "Waypoint.h"
#include "ObjectClass.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGDialogWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGParagraph.h"
#include "TGStringDialog.h"
#include "TGTextButton.h"
#include "PlacementObject.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "ShipClass.h"
#include "CameraMode.h"
#include "GridClass.h"
#include "NiCamera.h"
#include "STNumericBar.h"
#include "TGFloatEvent.h"
#include "LightObjectClass.h"
#include "TGSystemWrapperClass.h"
#include "Backdrop.h"
#include "TGString.h"
#include "PyEmbed.h"
#include "TGRect.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004a0f70 _$E535
// FUNCTION: STBC 0x004a0f90 _$E534

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0049b200 _$E32
// FUNCTION: STBC 0x0049b210 _$E29
// FUNCTION: STBC 0x0049b220 _$E31
// FUNCTION: STBC 0x0049b230 _$E30
// FUNCTION: STBC 0x0049b240 _$E38
// FUNCTION: STBC 0x0049b250 _$E35
// FUNCTION: STBC 0x0049b260 _$E37
// FUNCTION: STBC 0x0049b270 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0049b280 _$E57
// FUNCTION: STBC 0x0049b290 _$E56
// FUNCTION: STBC 0x0049b2a0 _$E61
// FUNCTION: STBC 0x0049b2b0 _$E60
// FUNCTION: STBC 0x0049c3f0 _$E72
// FUNCTION: STBC 0x0049c400 _$E71
// FUNCTION: STBC 0x0049c650 _$E76
// FUNCTION: STBC 0x0049c660 _$E75
// FUNCTION: STBC 0x0049caf0 _$E80
// FUNCTION: STBC 0x0049cb00 _$E79
// FUNCTION: STBC 0x0049ce20 _$E84
// FUNCTION: STBC 0x0049ce30 _$E83
// FUNCTION: STBC 0x0049d190 _$E208
// FUNCTION: STBC 0x0049d1a0 _$E207
// FUNCTION: STBC 0x0049efc0 _$E150
// FUNCTION: STBC 0x0049efd0 _$E149
// FUNCTION: STBC 0x0049f210 _$E163
// FUNCTION: STBC 0x0049f220 _$E162
// FUNCTION: STBC 0x0049f600 _$E213
// FUNCTION: STBC 0x0049f610 _$E212
// FUNCTION: STBC 0x0049fff0 _$E218
// FUNCTION: STBC 0x004a0000 _$E217
// FUNCTION: STBC 0x004a0290 _$E222
// FUNCTION: STBC 0x004a02a0 _$E221
// FUNCTION: STBC 0x004a0760 _$E226
// FUNCTION: STBC 0x004a0770 _$E225
// FUNCTION: STBC 0x004a0780 _$E233
// FUNCTION: STBC 0x004a0790 _$E232
// FUNCTION: STBC 0x004a07a0 _$E371
// FUNCTION: STBC 0x004a07b0 _$E370
// FUNCTION: STBC 0x004a07c0 _$E375
// FUNCTION: STBC 0x004a07d0 _$E374
// FUNCTION: STBC 0x004a07e0 _$E108
// FUNCTION: STBC 0x004a07f0 _$E107
// FUNCTION: STBC 0x004a0800 _$E290
// FUNCTION: STBC 0x004a0810 _$E289
// FUNCTION: STBC 0x004a0820 _$E297
// FUNCTION: STBC 0x004a0830 _$E296
// FUNCTION: STBC 0x004a0840 _$E301
// FUNCTION: STBC 0x004a0850 _$E300
// FUNCTION: STBC 0x004a0860 _$E305
// FUNCTION: STBC 0x004a0870 _$E304
// FUNCTION: STBC 0x004a0880 _$E309
// FUNCTION: STBC 0x004a0890 _$E308
// FUNCTION: STBC 0x004a08a0 _$E315
// FUNCTION: STBC 0x004a08b0 _$E314
// FUNCTION: STBC 0x004a08c0 _$E379
// FUNCTION: STBC 0x004a08d0 _$E378
// FUNCTION: STBC 0x004a08e0 _$E383
// FUNCTION: STBC 0x004a08f0 _$E382
// FUNCTION: STBC 0x004a0900 _$E387
// FUNCTION: STBC 0x004a0910 _$E386
// FUNCTION: STBC 0x004a0920 _$E391
// FUNCTION: STBC 0x004a0930 _$E390
// FUNCTION: STBC 0x004a0940 _$E395
// FUNCTION: STBC 0x004a0950 _$E394
// FUNCTION: STBC 0x004a0960 _$E399
// FUNCTION: STBC 0x004a0970 _$E398
// FUNCTION: STBC 0x004a0980 _$E403
// FUNCTION: STBC 0x004a0990 _$E402
// FUNCTION: STBC 0x004a09a0 _$E407
// FUNCTION: STBC 0x004a09b0 _$E406
// FUNCTION: STBC 0x004a09c0 _$E411
// FUNCTION: STBC 0x004a09d0 _$E410
// FUNCTION: STBC 0x004a09e0 _$E415
// FUNCTION: STBC 0x004a09f0 _$E414
// FUNCTION: STBC 0x004a0a00 _$E113
// FUNCTION: STBC 0x004a0a10 _$E112
// FUNCTION: STBC 0x004a0a20 _$E419
// FUNCTION: STBC 0x004a0a30 _$E418
// FUNCTION: STBC 0x004a0a40 _$E118
// FUNCTION: STBC 0x004a0a50 _$E117
// FUNCTION: STBC 0x004a0a60 _$E423
// FUNCTION: STBC 0x004a0a70 _$E422
// FUNCTION: STBC 0x004a0a80 _$E427
// FUNCTION: STBC 0x004a0a90 _$E426
// FUNCTION: STBC 0x004a0aa0 _$E431
// FUNCTION: STBC 0x004a0ab0 _$E430
// FUNCTION: STBC 0x004a0ac0 _$E435
// FUNCTION: STBC 0x004a0ad0 _$E434
// FUNCTION: STBC 0x004a0ae0 _$E127
// FUNCTION: STBC 0x004a0af0 _$E126
// FUNCTION: STBC 0x004a0b00 _$E439
// FUNCTION: STBC 0x004a0b10 _$E438
// FUNCTION: STBC 0x004a0b20 _$E443
// FUNCTION: STBC 0x004a0b30 _$E442
// FUNCTION: STBC 0x004a0b40 _$E447
// FUNCTION: STBC 0x004a0b50 _$E446
// FUNCTION: STBC 0x004a0b60 _$E133
// FUNCTION: STBC 0x004a0b70 _$E132
// FUNCTION: STBC 0x004a0b80 _$E138
// FUNCTION: STBC 0x004a0b90 _$E137
// FUNCTION: STBC 0x004a0ba0 _$E168
// FUNCTION: STBC 0x004a0bb0 _$E167
// FUNCTION: STBC 0x004a0bc0 _$E451
// FUNCTION: STBC 0x004a0bd0 _$E450
// FUNCTION: STBC 0x004a0be0 _$E173
// FUNCTION: STBC 0x004a0bf0 _$E172
// FUNCTION: STBC 0x004a0c00 _$E455
// FUNCTION: STBC 0x004a0c10 _$E454
// FUNCTION: STBC 0x004a0c20 _$E459
// FUNCTION: STBC 0x004a0c30 _$E458
// FUNCTION: STBC 0x004a0c40 _$E178
// FUNCTION: STBC 0x004a0c50 _$E177
// FUNCTION: STBC 0x004a0c60 _$E463
// FUNCTION: STBC 0x004a0c70 _$E462
// FUNCTION: STBC 0x004a0c80 _$E143
// FUNCTION: STBC 0x004a0c90 _$E142
// FUNCTION: STBC 0x004a0ca0 _$E467
// FUNCTION: STBC 0x004a0cb0 _$E466
// FUNCTION: STBC 0x004a0cc0 _$E471
// FUNCTION: STBC 0x004a0cd0 _$E470
// FUNCTION: STBC 0x004a0ce0 _$E183
// FUNCTION: STBC 0x004a0cf0 _$E182
// FUNCTION: STBC 0x004a0d00 _$E188
// FUNCTION: STBC 0x004a0d10 _$E187
// FUNCTION: STBC 0x004a0d20 _$E475
// FUNCTION: STBC 0x004a0d30 _$E474
// FUNCTION: STBC 0x004a0d40 _$E479
// FUNCTION: STBC 0x004a0d50 _$E478
// FUNCTION: STBC 0x004a0d60 _$E483
// FUNCTION: STBC 0x004a0d70 _$E482
// FUNCTION: STBC 0x004a0d80 _$E487
// FUNCTION: STBC 0x004a0d90 _$E486
// FUNCTION: STBC 0x004a0da0 _$E491
// FUNCTION: STBC 0x004a0db0 _$E490
// FUNCTION: STBC 0x004a0dc0 _$E495
// FUNCTION: STBC 0x004a0dd0 _$E494
// FUNCTION: STBC 0x004a0de0 _$E499
// FUNCTION: STBC 0x004a0df0 _$E498
// FUNCTION: STBC 0x004a0e00 _$E503
// FUNCTION: STBC 0x004a0e10 _$E502
// FUNCTION: STBC 0x004a0e20 _$E193
// FUNCTION: STBC 0x004a0e30 _$E192
// FUNCTION: STBC 0x004a0e40 _$E507
// FUNCTION: STBC 0x004a0e50 _$E506
// FUNCTION: STBC 0x004a0e60 _$E198
// FUNCTION: STBC 0x004a0e70 _$E197
// FUNCTION: STBC 0x004a0e80 _$E203
// FUNCTION: STBC 0x004a0e90 _$E202
// FUNCTION: STBC 0x004a0fa0 _$E511
// FUNCTION: STBC 0x004a0fb0 _$E510
// FUNCTION: STBC 0x004a0fc0 _$E516
// FUNCTION: STBC 0x004a0fd0 _$E515
// FUNCTION: STBC 0x004a0fe0 _$E520
// FUNCTION: STBC 0x004a0ff0 _$E519

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0049b2c0 _$E526
// FUNCTION: STBC 0x0049b2d0 _$E522
// FUNCTION: STBC 0x0049b2e0 _$E525
// FUNCTION: STBC 0x0049b2f0 _$E523
// DATA: STBC 0x009834f0
TGEventHandlerTable PlacementEditor::classHandlers;

/*
================
PlacementEditor::PlacementEditor

Builds the dialogs and the position paragraph; the editing modes come with
CreateModes.
================
*/
// FUNCTION: STBC 0x0049b310
PlacementEditor::PlacementEditor( void ) {
	selectMode = NULL;
	attachWaypointMode = NULL;
	insertWaypointsMode = NULL;
	placementNumber = 0;
	speed = 8.0f;
	CreateSaveDialog();

	editNameDialog = TGStringDialog::CreateFromTheme( 0.3f, 0.15f, "Edit Name", "Enter new name:", "", NULL, NULL );
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "PlacementEditor::EditNameDialogHandler", editNameDialog );
	editNameDialog->SetNotVisible( false );
	AddChild( editNameDialog, 0.5f, 0.5f );
	CreateLightTypeDialog();

	positionText = new TGParagraph( "X,Y,Z", 1.0f, NULL, "", 0.0f, 0x80000000 );
	AddChild( positionText, 0.0f, 1.0f - positionText->GetHeight() );
}

/*
================
PlacementEditor::~PlacementEditor

No mode is current while the modes go.
================
*/
// FUNCTION: STBC 0x0049b460
PlacementEditor::~PlacementEditor( void ) {
	mode = NULL;
	delete selectMode;
	delete attachWaypointMode;
	delete insertWaypointsMode;
	delete moveMode;
}

/*
================
PlacementEditor::Create
================
*/
// FUNCTION: STBC 0x0049b4f0
PlacementEditor *PlacementEditor::Create( void ) {
	PlacementEditor *editor = new PlacementEditor;
	editor->CreateMenu( "Placement Editor" );
	editor->CreateModes();
	return editor;
}

/*
================
PlacementEditor::RefreshDisplay

Updates the rendered set's camera, grids, placements and lights, which
the editor shows while the game is paused, and shows the camera's
position, and the selected object's with its distance from the camera.
================
*/
// FUNCTION: STBC 0x0049b540
void PlacementEditor::RefreshDisplay( void ) {
	Editor::RefreshDisplay();
	SetClass *set = g_kSetManager.GetRenderedSet();
	float time = g_kSystemWrapper.GetTimeInSeconds();
	CameraObjectClass *camera = set->GetActiveCamera();
	camera->Update( g_kSystemWrapper.GetTimeInSeconds() );

	int numGrids;
	GridClass **grids = GridClass::GetClassObjects( set, &numGrids );
	while ( --numGrids >= 0 ) {
		grids[numGrids]->Update( time );
	}
	int count;
	PlacementObject **placements = PlacementObject::GetClassObjects( set, &count );
	while ( --count >= 0 ) {
		placements[count]->Update( time );
	}
	LightObjectClass **lights = LightObjectClass::GetClassObjects( set, &count );
	while ( --count >= 0 ) {
		lights[count]->Update( time );
	}

	if ( camera != NULL ) {
		char text[1024];
		NiPoint3 cameraPosition = camera->GetTranslate();
		sprintf( text, "Cam(%1.4f, %1.4f, %1.4f)", cameraPosition.x, cameraPosition.y, cameraPosition.z );
		ObjectClass *object = GetSelectedObject();
		if ( object != NULL ) {
			NiPoint3 objectPosition = object->GetTranslate();
			NiPoint3 offset = cameraPosition - objectPosition;
			char objectText[256];
			sprintf( objectText, ", Obj(%1.4f, %1.4f, %1.4f), Distance: %1.4f", objectPosition.x, objectPosition.y, objectPosition.z,
					 offset.Length() );
			strcat( text, objectText );
		}
		positionText->SetString( text );
	}
}

/*
================
PlacementEditor::LostFocus
================
*/
// FUNCTION: STBC 0x0049b750
void PlacementEditor::LostFocus( bool layout ) {
	Editor::LostFocus( layout );
}

/*
================
PlacementEditor::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0049b760
void PlacementEditor::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::MenuHandler, "PlacementEditor::MenuHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::SaveDialogHandler, "PlacementEditor::SaveDialogHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::EditNameDialogHandler,
										   "PlacementEditor::EditNameDialogHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::DeleteDialogHandler, "PlacementEditor::DeleteDialogHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::SwitchSetsDialogHandler,
										   "PlacementEditor::SwitchSetsDialogHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::HandleLightConfig, "PlacementEditor::HandleLightConfig" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::HandleAsteroidFieldConfig,
										   "PlacementEditor::HandleAsteroidFieldConfig" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PlacementEditor::HandleWaypointConfig, "PlacementEditor::HandleWaypointConfig" );
}

/*
================
PlacementEditor::RegisterHandlers

The three dialog handlers are not class handlers: each dialog's result is
a broadcast handler added where the dialog is made.
================
*/
// FUNCTION: STBC 0x0049b810
void PlacementEditor::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_PLACEMENT_EDITOR_MENU, "PlacementEditor::MenuHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_DIALOG_RESULT, "PlacementEditor::SaveDialogHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_PLACEMENT_EDITOR_LIGHT_CONFIG, "PlacementEditor::HandleLightConfig" );
	AddMethodHandlerForClass( &classHandlers, ET_PLACEMENT_EDITOR_FIELD_CONFIG, "PlacementEditor::HandleAsteroidFieldConfig" );
	AddMethodHandlerForClass( &classHandlers, ET_PLACEMENT_EDITOR_WAYPOINT_CONFIG, "PlacementEditor::HandleWaypointConfig" );
}

/*
================
PlacementEditor::MenuHandler

Runs the menu entry the event names. The light type dialog's entries make
a light under the first free "Light %d" name in front of the camera and
open its configuration dialog.
================
*/
// FUNCTION: STBC 0x0049b880
void PlacementEditor::MenuHandler( TGEvent *event ) {
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
	case PEM_OBJECT_TO_CAMERA:
		MoveObjectToCamera();
		break;
	case PEM_CAMERA_TO_OBJECT:
		MoveCameraToObject();
		break;
	case PEM_CHASE_SELECTED:
		ChaseSelected();
		break;
	case PEM_CHASE_PREV:
		ChasePrev();
		break;
	case PEM_CHASE_NEXT:
		ChaseNext();
		break;
	case PEM_TARGET_PREV:
		TargetPrev();
		break;
	case PEM_TARGET_NEXT:
		TargetNext();
		break;
	case PEM_TOGGLE_GRID:
		ToggleGrid();
		break;
	case PEM_TOGGLE_PLACEMENTS:
		PlacementObject::TogglePlacements();
		break;
	case PEM_EDIT_NAME:
		EditSelectedName();
		break;
	case PEM_EDIT_SPEED:
		EditWaypointSpeed();
		break;
	case PEM_TOGGLE_STATIC:
		ToggleSelectedStatic();
		break;
	case PEM_TOGGLE_NAVPOINT:
		ToggleSelectedNavPoint();
		break;
	case PEM_SAVE:
		ShowSaveDialog();
		break;
	case PEM_SWITCH_SETS:
		ShowSwitchSetsDialog();
		break;
	case PEM_INSERT_OBJECT:
		InsertObject();
		break;
	case PEM_CREATE_LIGHT:
		ShowLightTypeDialog();
		break;
	case PEM_EDIT_LIGHT:
		ConfigureSelectedLight();
		break;
	case PEM_CREATE_ASTEROID_FIELD:
		CreateAsteroidField();
		break;
	case PEM_EDIT_ASTEROID_FIELD:
		ConfigureSelectedField();
		break;
	case PEM_OVERHEAD_VIEW:
		OverheadView();
		break;
	case PEM_ATTACH_WAYPOINT:
		ToggleAttachWaypointMode();
		break;
	case PEM_PATH_MODE:
		ToggleInsertWaypointsMode();
		break;
	case PEM_MOVE_OBJECT:
		ToggleMoveMode();
		break;
	case PEM_DELETE_OBJECT:
		ConfirmDeleteObject();
		break;
	case PEM_DELETE_PLACEMENTS:
		DeletePlacements();
		break;
	case PEM_CREATE_AMBIENT_LIGHT: {
		lightTypeDialog->SetNotVisible( true );
		SetClass *set = g_kSetManager.GetRenderedSet();
		char name[64];
		int number = 1;
		do {
			sprintf( name, "Light %d", number++ );
			placementNumber++;
		} while ( PlacementObject::GetObject( set, name ) != NULL || ObjectClass::GetObject( set, name ) != NULL );

		char *setName = set->GetName();
		LightPlacement *light = new LightPlacement( name, setName, NULL );
		light->ConfigAmbientLight( 1.0f, 1.0f, 1.0f, 1.0f );
		PlaceInFrontOfCamera( light );
		set->AddObjectToSet( light, light->GetName() );
		ConfigureLight( "Configure Ambient Light", light );
		break;
	}
	case PEM_CREATE_DIRECTIONAL_LIGHT: {
		lightTypeDialog->SetNotVisible( true );
		SetClass *set = g_kSetManager.GetRenderedSet();
		char name[64];
		int number = 1;
		do {
			sprintf( name, "Light %d", number++ );
			placementNumber++;
		} while ( PlacementObject::GetObject( set, name ) != NULL || ObjectClass::GetObject( set, name ) != NULL );

		char *setName = set->GetName();
		LightPlacement *light = new LightPlacement( name, setName, NULL );
		PlaceInFrontOfCamera( light );
		light->ConfigDirectionalLight( 1.0f, 1.0f, 1.0f, 1.0f );
		set->AddObjectToSet( light, light->GetName() );
		ConfigureLight( "Configure Directional Light", light );
		break;
	}
	}
	CallNextHandler( event );
}

/*
================
PlacementEditor::SaveDialogHandler

Saves the rendered set under the region file name the dialog holds, with
the mission placements file beside it; the set is named for the region
file. The dialog stays up when the save fails.
================
*/
// FUNCTION: STBC 0x0049bd50
void PlacementEditor::SaveDialogHandler( TGEvent *event ) {
	TGString placementsName;
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
	case TGDialogWindow::DFT_OK: {
		char *regionFile = saveDialog->GetCString();
		saveFileName->GetString( &placementsName );
		char *placementsFile = placementsName.Unknown6F56F0();
		char *name;
		int length;
		GetBaseName( regionFile, &name, &length );
		char *setName = new char[length + 1];
		strncpy( setName, name, length );
		setName[length] = '\0';
		if ( !SaveSet( regionFile, placementsFile, setName ) ) {
			ShowError( "Unable to save file." );
		} else {
			saveDialog->SetNotVisible( true );
		}
		delete [] placementsFile;
		delete [] regionFile;
		delete [] setName;
		break;
	}
	case TGDialogWindow::DFT_CANCEL:
		saveDialog->SetNotVisible( true );
		break;
	}
	CallNextHandler( event );
}

/*
================
PlacementEditor::GetBaseName
================
*/
// FUNCTION: STBC 0x0049be90
void PlacementEditor::GetBaseName( char *path, char **name, int *length ) {
	*name = path;
	for ( char *c = path; *c != '\0'; c++ ) {
		if ( *c == '\\' || *c == '/' ) {
			*name = c + 1;
		}
	}
	*length = strlen( *name );
	for ( int i = *length - 1; i >= 0; i-- ) {
		if ( ( *name )[i] == '.' ) {
			*length = i;
		}
	}
}

/*
================
PlacementEditor::EditNameDialogHandler

Renames the selected object to the dialog's string when the dialog was
accepted; an empty string clears the name.
================
*/
// FUNCTION: STBC 0x0049bef0
void PlacementEditor::EditNameDialogHandler( TGEvent *event ) {
	if ( ( (TGIntEvent *)event )->GetInt() == TGDialogWindow::DFT_OK ) {
		char *name = editNameDialog->GetCString();
		ObjectClass *object = ObjectClass::GetObjectByID( g_kSetManager.GetRenderedSet(), selectedID );
		if ( name[0] != '\0' ) {
			object->SetName( name );
		} else {
			object->SetName( NULL );
		}
		delete [] name;
	}
	editNameDialog->SetNotVisible( true );
	CallNextHandler( event );
}

/*
================
PlacementEditor::DeleteDialogHandler

Deletes the selected object from its set when the dialog was accepted,
then takes the dialog down.
================
*/
// FUNCTION: STBC 0x0049bf60
void PlacementEditor::DeleteDialogHandler( TGEvent *event ) {
	if ( ( (TGIntEvent *)event )->GetInt() == TGDialogWindow::DFT_OK ) {
		ObjectClass *object = GetSelectedObject();
		if ( object != NULL ) {
			SelectObject( NULL );
			object->GetContainingSet()->DeleteObjectFromSet( object->GetObjID() );
		}
	}
	TGDialogWindow *dialog = (TGDialogWindow *)event->GetSource();
	dialog->SetNotVisible( true );
	dialog->GetParent()->RemoveChild( dialog, true );
	delete dialog;
}

/*
================
PlacementEditor::SwitchSetsDialogHandler

Renders the set the dialog names, running its module's Initialize and
taking GetSet's set when the set does not exist yet, then takes the
dialog down.
================
*/
// FUNCTION: STBC 0x0049bfc0
// PARKED: register allocation and tail merge: retail keeps the dialog in EDI and gives SetManager::GetSet's two NULL returns separate exits, this build merges them and uses ESI (SortedArray::Get is game-72's header)
void PlacementEditor::SwitchSetsDialogHandler( TGEvent *event ) {
	TGStringDialog *dialog = (TGStringDialog *)event->GetSource();
	if ( ( (TGIntEvent *)event )->GetInt() == TGDialogWindow::DFT_OK ) {
		char *name = dialog->GetCString();
		SetClass *set = g_kSetManager.GetSet( name );
		if ( set == NULL ) {
			if ( PyEmbed::CallFunction( name, "Initialize", "", NULL, "()" ) != -1 ) {
				PyObject *result = NULL;
				if ( PyEmbed::CallFunction( name, "GetSet", "O", &result, "()" ) != -1 && result != NULL ) {
					PyEmbed::Unknown6F98E0( result, (void **)&set, "SetClass" );
				}
				Py_XDECREF( result );
			}
			PyErr_Clear();
		}
		delete [] name;
		if ( set != NULL ) {
			DeleteEditorCamera();
			g_kSetManager.MakeRenderedSet( set );
			CreateEditorCamera();
		}
	}
	dialog->SetNotVisible( true );
	dialog->GetParent()->RemoveChild( dialog, true );
	delete dialog;
}

/*
================
PlacementEditor::MoveObjectToCamera

Puts the selected object where the camera is, facing its way, and frees
the camera from any mode that follows an object.
================
*/
// FUNCTION: STBC 0x0049c140
void PlacementEditor::MoveObjectToCamera( void ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	if ( camera != NULL ) {
		ObjectClass *object = GetSelectedObject();
		if ( object != NULL ) {
			object->SetTranslate( camera->GetTranslate() );
			object->AlignToVectors( camera->GetWorldForward(), camera->GetWorldUp() );
			object->UpdateNodeOnly();
		}
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
}

/*
================
PlacementEditor::MoveCameraToObject

Puts the camera where the selected object is, looking the way it faces,
and frees the camera from any mode that follows an object.
================
*/
// FUNCTION: STBC 0x0049c210
void PlacementEditor::MoveCameraToObject( void ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	if ( camera != NULL ) {
		ObjectClass *object = GetSelectedObject();
		if ( object != NULL ) {
			camera->SetTranslate( object->GetTranslate() );
			NiCamera *niCamera = camera->GetNiCamera();
			niCamera->Update( 0.0f );
			niCamera->LookAtWorldPoint( object->GetTranslate() + object->GetWorldForward(), object->GetWorldUp() );
		}
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
}

/*
================
PlacementEditor::ChaseSelected

Makes the camera's chase mode follow the selected object.
================
*/
// FUNCTION: STBC 0x0049c310
void PlacementEditor::ChaseSelected( void ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	if ( camera != NULL ) {
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode != NULL && mode->GetObjType() == CAMERAMODE_TYPE ) {
			camera->PopCameraMode( NULL );
		}
		ChaseCameraMode *chase = ChaseCameraMode::Cast( camera->GetNamedCameraMode( "Chase" ) );
		if ( chase != NULL ) {
			chase->SetAttrIDObject( "Target", GetSelectedObject() );
			chase->Unknown421410();
			if ( camera->GetCurrentCameraMode( true ) != chase ) {
				camera->PopCameraMode( NULL );
				camera->PushCameraMode( chase );
			}
		}
	}
}

/*
================
PlacementEditor::ChasePrev
================
*/
// FUNCTION: STBC 0x0049c3b0
void PlacementEditor::ChasePrev( void ) {
	SelectPrev();
	ChaseSelected();
}

/*
================
PlacementEditor::ChaseNext
================
*/
// FUNCTION: STBC 0x0049c3d0
void PlacementEditor::ChaseNext( void ) {
	SelectNext();
	ChaseSelected();
}

/*
================
PlacementEditor::TargetPrev

Keeps the object the camera follows and makes the target mode look from
it at the previous object.
================
*/
// FUNCTION: STBC 0x0049c410
void PlacementEditor::TargetPrev( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	CameraObjectClass *camera = set->GetActiveCamera();
	if ( camera != NULL ) {
		ObjectClass *source = NULL;
		TargetCameraMode *target = TargetCameraMode::Cast( camera->GetCurrentCameraMode( true ) );
		if ( target != NULL ) {
			source = target->GetAttrIDObject( "Source" );
		}
		ChaseCameraMode *chase = ChaseCameraMode::Cast( camera->GetCurrentCameraMode( true ) );
		if ( chase != NULL ) {
			source = chase->GetAttrIDObject( "Target" );
		}
		if ( source != NULL ) {
			SelectPrev();
			ObjectClass *selected = ObjectClass::GetObjectByID( set, selectedID );
			if ( selected == source ) {
				SelectPrev();
				selected = ObjectClass::GetObjectByID( set, selectedID );
			}
			if ( selected != NULL && selected != source ) {
				TargetCameraMode *mode = TargetCameraMode::Cast( camera->GetNamedCameraMode( "Target" ) );
				if ( mode != NULL ) {
					mode->SetAttrIDObject( "Source", source );
					mode->SetAttrIDObject( "Target", selected );
				}
			}
		}
	}
}

/*
================
PlacementEditor::TargetNext

Keeps the object the camera follows and makes the target mode look from
it at the next object.
================
*/
// FUNCTION: STBC 0x0049c500
void PlacementEditor::TargetNext( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	CameraObjectClass *camera = set->GetActiveCamera();
	if ( camera != NULL ) {
		ObjectClass *source = NULL;
		TargetCameraMode *target = TargetCameraMode::Cast( camera->GetCurrentCameraMode( true ) );
		if ( target != NULL ) {
			source = target->GetAttrIDObject( "Source" );
		}
		ChaseCameraMode *chase = ChaseCameraMode::Cast( camera->GetCurrentCameraMode( true ) );
		if ( chase != NULL ) {
			source = chase->GetAttrIDObject( "Target" );
		}
		if ( source != NULL ) {
			SelectNext();
			ObjectClass *selected = ObjectClass::GetObjectByID( set, selectedID );
			if ( selected == source ) {
				SelectNext();
				selected = ObjectClass::GetObjectByID( set, selectedID );
			}
			if ( selected != NULL && selected != source ) {
				TargetCameraMode *mode = TargetCameraMode::Cast( camera->GetNamedCameraMode( "Target" ) );
				if ( mode != NULL ) {
					mode->SetAttrIDObject( "Source", source );
					mode->SetAttrIDObject( "Target", selected );
				}
			}
		}
	}
}

/*
================
PlacementEditor::ToggleGrid

Shows or hides each grid of the rendered set.
================
*/
// FUNCTION: STBC 0x0049c5f0
void PlacementEditor::ToggleGrid( void ) {
	int count;
	GridClass **grids = GridClass::GetClassObjects( g_kSetManager.GetRenderedSet(), &count );
	while ( --count >= 0 ) {
		GridClass *grid = grids[count];
		grid->SetHidden( !grid->IsHidden() );
	}
}

/*
================
PlacementEditor::EditSelectedName

Opens the name dialog on the selected object's name.
================
*/
// FUNCTION: STBC 0x0049c670
void PlacementEditor::EditSelectedName( void ) {
	ObjectClass *object = GetSelectedObject();
	if ( object != NULL ) {
		editNameDialog->SetString( object->GetName() );
		SetFocus( editNameDialog, false );
		editNameDialog->SetVisible();
	}
}

/*
================
PlacementEditor::EditWaypointSpeed

Opens a dialog with a bar for the selected waypoint's speed, 0 to 30: each
change sends the waypoint ET_SET_WAYPOINT_SPEED, and "Done" sends the
editor the waypoint configuration event that takes the dialog down.
================
*/
// FUNCTION: STBC 0x0049c6d0
void PlacementEditor::EditWaypointSpeed( void ) {
	Waypoint *waypoint = Waypoint::Cast( GetSelectedObject() );
	if ( waypoint != NULL ) {
		char *title = new char[strlen( "Edit \"" ) + strlen( waypoint->GetName() ) + strlen( "\" Speed" ) + 1];
		strcpy( title, "Edit \"" );
		strcat( title, waypoint->GetName() );
		strcat( title, "\" Speed" );
		TGDialogWindow *dialog = TGDialogWindow::CreateFromTheme( 0.35f, 0.15f, title, TGDialogWindow::DFT_DRAGGABLE, NULL );
		delete [] title;

		STNumericBar *bar = new STNumericBar( 0.3f, 0.05f );
		bar->SetRange( 0.0f, 30.0f );
		bar->SetValue( waypoint->GetSpeed() );
		TGFloatEvent *event = new TGFloatEvent;
		event->SetEventType( ET_SET_WAYPOINT_SPEED );
		event->SetDestination( waypoint );
		bar->SetUpdateEvent( event );
		dialog->AddChild( bar, 0.0f, 0.003f );

		TGTextButton *button = new TGTextButton( "Done", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
		dialog->AddChild( button, 0.0f, bar->GetBottom() );
		TGIntEvent *done = new TGIntEvent;
		done->SetSource( dialog );
		done->SetDestination( this );
		done->SetEventType( ET_PLACEMENT_EDITOR_WAYPOINT_CONFIG );
		done->SetInt( PEM_EDIT_SPEED );
		button->SetActivationEvent( done );

		AddChild( dialog, 1.0f - dialog->GetWidth(), 0.0f, false );
		SetFocus( dialog, false );
		dialog->SetVisible( false );
		dialog->Layout();
	}
}

/*
================
PlacementEditor::ToggleSelectedStatic
================
*/
// FUNCTION: STBC 0x0049c970
void PlacementEditor::ToggleSelectedStatic( void ) {
	PlacementObject *placement = PlacementObject::Cast( GetSelectedObject() );
	if ( placement != NULL ) {
		placement->SetStatic( !placement->IsStatic() );
	}
}

/*
================
PlacementEditor::ToggleSelectedNavPoint
================
*/
// FUNCTION: STBC 0x0049c9a0
void PlacementEditor::ToggleSelectedNavPoint( void ) {
	PlacementObject *placement = PlacementObject::Cast( GetSelectedObject() );
	if ( placement != NULL ) {
		placement->SetNavPoint( !placement->IsNavPoint() );
	}
}

/*
================
PlacementEditor::ShowSaveDialog

Offers the rendered set's region script, "scripts/" and the module with
its dots made slashes, as the file name.
================
*/
// FUNCTION: STBC 0x0049c9d0
// PARKED: register allocation: retail's inline strcpy keeps its length in EAX and clears EAX again for the loop's strlen, this build uses EDX; one instruction differs
void PlacementEditor::ShowSaveDialog( void ) {
	char *module = g_kSetManager.GetRenderedSet()->GetRegionModule();
	if ( module != NULL ) {
		char *fileName = new char[strlen( module ) + 12];
		strcpy( fileName, "scripts/" );
		unsigned int i;
		for ( i = 0; i < strlen( module ); i++ ) {
			if ( module[i] == '.' ) {
				fileName[i + 8] = '/';
			} else {
				fileName[i + 8] = module[i];
			}
		}
		fileName[i + 8] = '\0';
		strcat( fileName, ".py" );
		saveDialog->SetString( fileName );
		delete [] fileName;
	}
	SetFocus( saveDialog, false );
	saveDialog->SetVisible();
}

/*
================
PlacementEditor::ShowSwitchSetsDialog

Asks for the set to edit, the rendered set's name first.
================
*/
// FUNCTION: STBC 0x0049cb10
void PlacementEditor::ShowSwitchSetsDialog( void ) {
	TGStringDialog *dialog = TGStringDialog::CreateFromTheme( 0.35f, 0.3f, "Switch Sets", "New set:", g_kSetManager.GetRenderedSet()->GetName(), NULL, NULL );
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "PlacementEditor::SwitchSetsDialogHandler", dialog );
	// BUG: adds the dialog's size where centring would subtract it
	AddChild( dialog, ( dialog->GetWidth() + 1.0f ) * 0.5f, ( dialog->GetHeight() + 0.9f ) * 0.5f, false );
	SetFocus( dialog, false );
	dialog->Layout();
}

/*
================
PlacementEditor::InsertObject

The current mode's new waypoint, in front of the camera.
================
*/
// FUNCTION: STBC 0x0049cbb0
void PlacementEditor::InsertObject( void ) {
	PlaceInFrontOfCamera( ( (PlacementEditorMode *)mode )->CreateWaypoint() );
}

/*
================
PlacementEditor::ShowLightTypeDialog
================
*/
// FUNCTION: STBC 0x0049cbd0
void PlacementEditor::ShowLightTypeDialog( void ) {
	SetFocus( lightTypeDialog, false );
	lightTypeDialog->SetVisible( true );
}

/*
================
PlacementEditor::ConfigureSelectedLight

Opens the configuration dialog of the selected light placement; point and
spot lights have none.
================
*/
// FUNCTION: STBC 0x0049cc00
void PlacementEditor::ConfigureSelectedLight( void ) {
	LightPlacement *light = LightPlacement::Cast( GetSelectedObject() );
	if ( light != NULL ) {
		switch ( light->GetLightType() ) {
		case LIGHT_TYPE_AMBIENT:
			ConfigureLight( "Configure Ambient Light", light );
			break;
		case LIGHT_TYPE_DIRECTIONAL:
			ConfigureLight( "Configure Directional Light", light );
			break;
		}
	}
}

/*
================
PlacementEditor::CreateAsteroidField

Places a new asteroid field under the first free "Asteroid Field %d" name
in front of the camera and opens its configuration dialog.
================
*/
// FUNCTION: STBC 0x0049cc50
void PlacementEditor::CreateAsteroidField( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	char name[64];
	int number = 1;
	do {
		sprintf( name, "Asteroid Field %d", number++ );
		placementNumber++;
	} while ( PlacementObject::GetObject( set, name ) != NULL || ObjectClass::GetObject( set, name ) != NULL );

	char *setName = set->GetName();
	AsteroidFieldPlacement *field = new AsteroidFieldPlacement( name, setName, NULL );
	PlaceInFrontOfCamera( field );
	set->AddObjectToSet( field, field->GetName() );
	ConfigureAsteroidField( field );
}

/*
================
PlacementEditor::ConfigureSelectedField
================
*/
// FUNCTION: STBC 0x0049cd10
void PlacementEditor::ConfigureSelectedField( void ) {
	AsteroidFieldPlacement *field = AsteroidFieldPlacement::Cast( GetSelectedObject() );
	if ( field != NULL ) {
		ConfigureAsteroidField( field );
	}
}

/*
================
PlacementEditor::OverheadView

Frees the camera from any mode that follows an object and takes it high
above where it is, looking down.
================
*/
// FUNCTION: STBC 0x0049cd30
void PlacementEditor::OverheadView( void ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	CameraMode *mode = camera->GetCurrentCameraMode( true );
	if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
		camera->PushCameraMode( new CameraMode( camera ) );
	}
	NiPoint3 forward = -NiPoint3::UNIT_Z;
	NiPoint3 up = NiPoint3::UNIT_Y;
	camera->LookToward( &forward, &up, 1.0f );
	const TGPoint3 &location = camera->GetWorldLocation();
	NiPoint3 position;
	position.x = location.x;
	position.y = location.y;
	position.z = 750.0f;
	camera->SweepTo( &position, 1.0f );
}

/*
================
PlacementEditor::ToggleAttachWaypointMode

Leaves the mode, or enters it when a waypoint is selected.
================
*/
// FUNCTION: STBC 0x0049ce40
void PlacementEditor::ToggleAttachWaypointMode( void ) {
	if ( mode == attachWaypointMode ) {
		SetMode( selectMode );
	} else if ( GetSelectedObject()->IsTypeOf( WAYPOINT_TYPE ) ) {
		SetMode( attachWaypointMode );
	}
}

/*
================
PlacementEditor::ToggleInsertWaypointsMode
================
*/
// FUNCTION: STBC 0x0049ce90
void PlacementEditor::ToggleInsertWaypointsMode( void ) {
	SetMode( mode == insertWaypointsMode ? selectMode : insertWaypointsMode );
}

/*
================
PlacementEditor::ToggleMoveMode

Leaves the mode, or enters it when an object is selected.
================
*/
// FUNCTION: STBC 0x0049ceb0
void PlacementEditor::ToggleMoveMode( void ) {
	if ( mode == moveMode ) {
		SetMode( selectMode );
	} else if ( GetSelectedObject() != NULL ) {
		SetMode( moveMode );
	}
}

/*
================
PlacementEditor::ConfirmDeleteObject

Asks before DeleteDialogHandler deletes the selected object.
================
*/
// FUNCTION: STBC 0x0049cef0
void PlacementEditor::ConfirmDeleteObject( void ) {
	if ( GetSelectedObject() != NULL ) {
		TGDialogWindow *dialog = TGDialogWindow::CreateFromTheme( 0.3f, 0.15f, "Delete?", TGDialogWindow::DFT_OKCANCEL | TGDialogWindow::DFT_DRAGGABLE, NULL );
		TGParagraph *text = new TGParagraph( "Are you sure you want to delete this object?", 1.0f, NULL, "", 0.0f, 0x80000000 );
		text->SetWrapWidth( dialog->GetSubPane()->GetWidth() );
		dialog->AddChild( text );
		// BUG: adds the dialog's size where centring would subtract it
	AddChild( dialog, ( dialog->GetWidth() + 1.0f ) * 0.5f, ( dialog->GetHeight() + 1.0f ) * 0.5f, false );
		SetFocus( dialog, false );
		dialog->Layout();
		g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "PlacementEditor::DeleteDialogHandler", dialog );
	}
}
/*
================
PlacementEditor::DeletePlacements
================
*/
// FUNCTION: STBC 0x0049d010
void PlacementEditor::DeletePlacements( void ) {
	PlacementObject::DeleteAll();
}

/*
================
PlacementEditor::CreateSaveDialog

The region file name is the dialog's string; the mission placements file
name sits under it, read-only.
================
*/
// FUNCTION: STBC 0x0049d020
// PARKED: x87 schedule: retail adds the label's top before loading the dialog's vtable for AddChild, this build after; GetBottom, both operand orders and a named local tried
void PlacementEditor::CreateSaveDialog( void ) {
	saveDialog = TGStringDialog::CreateFromTheme( 0.35f, 0.3f, "Save", "Region filename:", "Region.py", NULL, NULL );
	TGParagraph *label = new TGParagraph( "Mission placements filename:", 1.0f, NULL, "", 0.0f, 0x80000000 );
	saveDialog->AddChild( label, 0.0f, 0.08f, false );
	saveFileName = new TGParagraph( "MissionPlacements.py", 1.0f, NULL, "", 0.0f, 0x80000000 );
	saveFileName->SetReadOnly( false );
	saveDialog->AddChild( saveFileName, 0.0f, label->GetBottom() );
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "PlacementEditor::SaveDialogHandler", saveDialog );
	saveDialog->SetNotVisible();
	AddChild( saveDialog, 0.5f, 0.5f );
}

/*
================
PlacementEditor::CreateLightTypeDialog

Its two buttons send the menu entries that make an ambient or a
directional light.
================
*/
// FUNCTION: STBC 0x0049d1b0
void PlacementEditor::CreateLightTypeDialog( void ) {
	lightTypeDialog = TGDialogWindow::CreateFromTheme( 0.35f, 0.1f, "Select Light Type", TGDialogWindow::DFT_DRAGGABLE, NULL );
	lightTypeDialog->SetNotVisible();
	AddChild( lightTypeDialog, 0.4f, 0.4f );

	TGTextButton *button = new TGTextButton( "Ambient", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	lightTypeDialog->AddChild( button, 0.0f, 0.01f );
	float x = button->GetWidth();
	TGIntEvent *event = new TGIntEvent;
	event->SetDestination( this );
	event->SetEventType( ET_PLACEMENT_EDITOR_MENU );
	event->SetInt( PEM_CREATE_AMBIENT_LIGHT );
	button->SetActivationEvent( event );

	button = new TGTextButton( "Directional", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	lightTypeDialog->AddChild( button, x, 0.01f );
	event = new TGIntEvent;
	event->SetDestination( this );
	event->SetEventType( ET_PLACEMENT_EDITOR_MENU );
	event->SetInt( PEM_CREATE_DIRECTIONAL_LIGHT );
	button->SetActivationEvent( event );
}

/*
================
PlacementEditor::ConfigureLight

Opens the configuration dialog of the light under the title: its name,
then a row each for the red, green and blue of its colour (the ambient
colour of an ambient light, the diffuse of a directional one) and for its
dimmer, each with its value and "-" and "+" buttons, then "Done"; the
buttons send HandleLightConfig the dialog. Without a light the values
are 1. The dialog grows to fit the buttons and sits at the editor's
right edge.
================
*/
// FUNCTION: STBC 0x0049d390
// DRAFT: float copies (as ConfigureAsteroidField): retail copies each row's GetTop and the up button's x through a float slot (fld/fstp, then mov) where this build moves them as integers, which costs it an extra frame slot (0xCC against 0xC4)
void PlacementEditor::ConfigureLight( char *title, LightPlacement *light ) {
	TGDialogWindow *dialog = TGDialogWindow::CreateFromTheme( 0.25f, 0.15f, title, TGDialogWindow::DFT_DRAGGABLE, NULL );

	// a row's label and the values its buttons send
	struct LightConfigRow {
		char *	label;
		int		down;
		int		up;
	};
	LightConfigRow rows[] = {
		{ "Red:", LCB_RED_DOWN, LCB_RED_UP },
		{ "Green:", LCB_GREEN_DOWN, LCB_GREEN_UP },
		{ "Blue:", LCB_BLUE_DOWN, LCB_BLUE_UP },
		{ "Intensity:", LCB_DIMMER_DOWN, LCB_DIMMER_UP },
		{ NULL, 0, 0 }
	};
	float values[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	LightObjectClass *lightObject = light->GetLight();
	if ( lightObject != NULL ) {
		NiLight *niLight = lightObject->GetNiLight();
		if ( niLight != NULL ) {
			NiColor color( 0.0f, 0.0f, 0.0f );
			switch ( light->GetLightType() ) {
			case LIGHT_TYPE_AMBIENT:
				color = niLight->GetAmbientColor();
				break;
			case LIGHT_TYPE_DIRECTIONAL:
				color = niLight->GetDiffuseColor();
				break;
			}
			values[0] = color.r;
			values[1] = color.g;
			values[2] = color.b;
			values[3] = niLight->GetDimmer();
		}
	}

	TGParagraph *label = new TGParagraph( light->GetName(), 1.0f, NULL, "", 0.0f, 0x80000000 );
	dialog->AddChild( label, 0.0f, 0.01f );
	float y = label->GetBottom();

	char text[64];
	float *value = values;
	for ( LightConfigRow *row = rows; row->label != NULL; row++, value++ ) {
		label = new TGParagraph( row->label, 1.0f, NULL, "", 0.0f, 0x80000000 );
		dialog->AddChild( label, 0.0f, y );
		sprintf( text, "%1.1f", *value );
		TGParagraph *valueText = new TGParagraph( text, 1.0f, NULL, "", 0.0f, 0x80000000 );
		valueText->SetReadOnly( false );
		float x = 0.1f;
		if ( label->GetRight() > 0.1f ) {
			x = label->GetRight();
		}
		dialog->AddChild( valueText, x, label->GetTop() );

		TGTextButton *down = new TGTextButton( "-", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
		x = 0.15f;
		if ( valueText->GetRight() > 0.15f ) {
			x = valueText->GetRight();
		}
		dialog->AddChild( down, x, valueText->GetTop() );
		TGIntEvent *event = new TGIntEvent;
		event->SetSource( dialog );
		event->SetDestination( this );
		event->SetEventType( ET_PLACEMENT_EDITOR_LIGHT_CONFIG );
		event->SetInt( row->down );
		down->SetActivationEvent( event );

		TGTextButton *up = new TGTextButton( "+", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
		dialog->AddChild( up, down->GetRight(), down->GetTop() );
		event = new TGIntEvent;
		event->SetSource( dialog );
		event->SetDestination( this );
		event->SetEventType( ET_PLACEMENT_EDITOR_LIGHT_CONFIG );
		event->SetInt( row->up );
		up->SetActivationEvent( event );

		y = label->GetBottom();
	}

	TGTextButton *done = new TGTextButton( "Done", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	dialog->AddChild( done, 0.0f, y );
	TGIntEvent *event = new TGIntEvent;
	event->SetSource( dialog );
	event->SetDestination( this );
	event->SetEventType( ET_PLACEMENT_EDITOR_LIGHT_CONFIG );
	event->SetInt( LCB_DONE );
	done->SetActivationEvent( event );

	TGRect bounds;
	dialog->GetSubPane()->GetBounds( &bounds );
	float overflow = done->GetBottom() - bounds.GetHeight();
	if ( overflow > 0.0f ) {
		dialog->Resize( dialog->GetWidth(), dialog->GetHeight() + overflow );
	}
	AddChild( dialog, 1.0f - dialog->GetWidth(), 0.0f, false );
	SetFocus( dialog, false );
	dialog->SetVisible( false );
	dialog->Layout();
}

/*
================
PlacementEditor::ConfigureAsteroidField

Opens the configuration dialog of the field: its name, then a row each
for the radius, the tiles per axis, the asteroids per tile and the size
factor with the value editable, then "Done" and "Apply", which send
HandleAsteroidFieldConfig the dialog. The dialog grows to fit the buttons
and sits at the editor's right edge.
================
*/
// FUNCTION: STBC 0x0049da50
// PARKED: float copies: retail copies the Apply button's x and the dialog's x through a float slot of their own (fld/fstp, then mov) and the width for Resize through the FPU, this build with integer moves; named float locals drop the slot instead
void PlacementEditor::ConfigureAsteroidField( AsteroidFieldPlacement *field ) {
	TGDialogWindow *dialog = TGDialogWindow::CreateFromTheme( 0.4f, 0.33f, "Edit Asteroid Field", TGDialogWindow::DFT_DRAGGABLE, NULL );
	TGParagraph *value = new TGParagraph( field->GetName(), 1.0f, NULL, "", 0.0f, 0x80000000 );
	dialog->AddChild( value, 0.0f, 0.01f );
	float y = value->GetBottom();

	char text[128];
	dialog->AddChild( new TGParagraph( "Radius: ", 1.0f, NULL, "", 0.0f, 0x80000000 ), 0.0f, y );
	sprintf( text, "%f", field->GetFieldRadius() );
	value = new TGParagraph( text, 1.0f, NULL, "", 0.0f, 0x80000000 );
	value->SetReadOnly( false );
	dialog->AddChild( value, 0.25f, y );
	y = value->GetBottom();

	dialog->AddChild( new TGParagraph( "Tiles/axis: ", 1.0f, NULL, "", 0.0f, 0x80000000 ), 0.0f, y );
	sprintf( text, "%d", field->GetNumTilesPerAxis() );
	value = new TGParagraph( text, 1.0f, NULL, "", 0.0f, 0x80000000 );
	value->SetReadOnly( false );
	dialog->AddChild( value, 0.25f, y );
	y = value->GetBottom();

	dialog->AddChild( new TGParagraph( "Asteroids/tile: ", 1.0f, NULL, "", 0.0f, 0x80000000 ), 0.0f, y );
	sprintf( text, "%d", field->GetNumAsteroidsPerTile() );
	value = new TGParagraph( text, 1.0f, NULL, "", 0.0f, 0x80000000 );
	value->SetReadOnly( false );
	dialog->AddChild( value, 0.25f, y );
	y = value->GetBottom();

	dialog->AddChild( new TGParagraph( "Asteroid size factor: ", 1.0f, NULL, "", 0.0f, 0x80000000 ), 0.0f, y );
	sprintf( text, "%f", field->GetAsteroidSizeFactor() );
	value = new TGParagraph( text, 1.0f, NULL, "", 0.0f, 0x80000000 );
	value->SetReadOnly( false );
	dialog->AddChild( value, 0.25f, y );
	y = value->GetBottom();

	TGTextButton *done = new TGTextButton( "Done", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	dialog->AddChild( done, 0.0f, y );
	TGIntEvent *event = new TGIntEvent;
	event->SetSource( dialog );
	event->SetDestination( this );
	event->SetEventType( ET_PLACEMENT_EDITOR_FIELD_CONFIG );
	event->SetInt( FCB_DONE );
	done->SetActivationEvent( event );

	TGTextButton *apply = new TGTextButton( "Apply", "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	dialog->AddChild( apply, done->GetWidth(), y );
	event = new TGIntEvent;
	event->SetSource( dialog );
	event->SetDestination( this );
	event->SetEventType( ET_PLACEMENT_EDITOR_FIELD_CONFIG );
	event->SetInt( FCB_APPLY );
	apply->SetActivationEvent( event );

	TGRect bounds;
	dialog->GetSubPane()->GetBounds( &bounds );
	float overflow = done->GetBottom() - bounds.GetHeight();
	if ( overflow > 0.0f ) {
		dialog->Resize( dialog->GetWidth(), dialog->GetHeight() + overflow );
	}
	AddChild( dialog, 1.0f - dialog->GetWidth(), 0.0f, false );
	SetFocus( dialog, false );
	dialog->SetVisible();
}

/*
================
PlacementEditor::CreateMenu

The editor's own entries after every editor's.
================
*/
// FUNCTION: STBC 0x0049e120
void PlacementEditor::CreateMenu( char *title ) {
	Editor::CreateMenu( title );
	AddMenuButton( "Chase Selected", ET_PLACEMENT_EDITOR_MENU, PEM_CHASE_SELECTED, 1.0f );
	AddMenuButton( "Chase Prev (C)", ET_PLACEMENT_EDITOR_MENU, PEM_CHASE_PREV, 0.5f );
	AddMenuButton( "Chase Next (c)", ET_PLACEMENT_EDITOR_MENU, PEM_CHASE_NEXT, 0.5f );
	AddMenuButton( "Target Prev (T)", ET_PLACEMENT_EDITOR_MENU, PEM_TARGET_PREV, 0.5f );
	AddMenuButton( "Target Next (t)", ET_PLACEMENT_EDITOR_MENU, PEM_TARGET_NEXT, 0.5f );
	AddMenuButton( "Toggle Grid (g)", ET_PLACEMENT_EDITOR_MENU, PEM_TOGGLE_GRID, 0.5f );
	AddMenuButton( "Togl Plcmnts (p)", ET_PLACEMENT_EDITOR_MENU, PEM_TOGGLE_PLACEMENTS, 0.5f );
	AddMenuButton( "Obj To Cam (q)", ET_PLACEMENT_EDITOR_MENU, PEM_OBJECT_TO_CAMERA, 0.5f );
	AddMenuButton( "Cam To Obj (Q)", ET_PLACEMENT_EDITOR_MENU, PEM_CAMERA_TO_OBJECT, 0.5f );
	AddMenuButton( "Edit Name", ET_PLACEMENT_EDITOR_MENU, PEM_EDIT_NAME, 1.0f );
	AddMenuButton( "Edit Speed", ET_PLACEMENT_EDITOR_MENU, PEM_EDIT_SPEED, 1.0f );
	AddMenuButton( "Toggle Static (d)", ET_PLACEMENT_EDITOR_MENU, PEM_TOGGLE_STATIC, 1.0f );
	AddMenuButton( "Toggle NavPoint", ET_PLACEMENT_EDITOR_MENU, PEM_TOGGLE_NAVPOINT, 1.0f );
	AddMenuButton( "Save", ET_PLACEMENT_EDITOR_MENU, PEM_SAVE, 1.0f );
	AddMenuButton( "Switch Sets", ET_PLACEMENT_EDITOR_MENU, PEM_SWITCH_SETS, 1.0f );
	AddMenuButton( "Insert Object", ET_PLACEMENT_EDITOR_MENU, PEM_INSERT_OBJECT, 1.0f );
	AddMenuButton( "Create Light", ET_PLACEMENT_EDITOR_MENU, PEM_CREATE_LIGHT, 1.0f );
	AddMenuButton( "Edit Light", ET_PLACEMENT_EDITOR_MENU, PEM_EDIT_LIGHT, 1.0f );
	AddMenuButton( "Create Asteroid Field", ET_PLACEMENT_EDITOR_MENU, PEM_CREATE_ASTEROID_FIELD, 1.0f );
	AddMenuButton( "Edit Asteroid Field", ET_PLACEMENT_EDITOR_MENU, PEM_EDIT_ASTEROID_FIELD, 1.0f );
	AddMenuButton( "Overhead View (o)", ET_PLACEMENT_EDITOR_MENU, PEM_OVERHEAD_VIEW, 1.0f );
	AddMenuButton( "Attach Waypoint", ET_PLACEMENT_EDITOR_MENU, PEM_ATTACH_WAYPOINT, 1.0f );
	AddMenuButton( "Path Mode", ET_PLACEMENT_EDITOR_MENU, PEM_PATH_MODE, 1.0f );
	AddMenuButton( "Move Object", ET_PLACEMENT_EDITOR_MENU, PEM_MOVE_OBJECT, 1.0f );
	AddMenuButton( "Delete Object", ET_PLACEMENT_EDITOR_MENU, PEM_DELETE_OBJECT, 1.0f );
	AddMenuButton( "Delete Placements", ET_PLACEMENT_EDITOR_MENU, PEM_DELETE_PLACEMENTS, 1.0f );
}

/*
================
PlacementEditor::CreateModes

The base mode is the first current one.
================
*/
// FUNCTION: STBC 0x0049e3b0
void PlacementEditor::CreateModes( void ) {
	selectMode = new PlacementEditorMode( this );
	attachWaypointMode = new AttachWaypointMode( this );
	insertWaypointsMode = new InsertWaypointsMode( this );
	moveMode = new MoveMode( this );
	SetMode( selectMode );
}

// A step of the light dialog's arrows (invented name).
static const float LIGHT_CONFIG_STEP = 0.1f;

/*
================
PlacementEditor::HandleLightConfig

Steps the value the button names, then gives the light the dialog's red,
green, blue and dimmer; "Done" takes the dialog down instead.
================
*/
// FUNCTION: STBC 0x0049e4b0
// PARKED: x87: in the step-up cases retail adds 0.1 with fld/fadd st(1), stores and reloads the sum where this build uses fadd/fst; x + 0.1f, 0.1f + x, += and a reversed compare tried
void PlacementEditor::HandleLightConfig( TGEvent *event ) {
	TGDialogWindow *dialog = (TGDialogWindow *)event->GetSource();
	TGPane *pane = dialog->GetSubPane();
	float value;
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
	case LCB_RED_DOWN:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_RED ) ) - LIGHT_CONFIG_STEP;
		if ( value < 0.0f ) {
			value = 0.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_RED ), value );
		break;
	case LCB_RED_UP:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_RED ) ) + LIGHT_CONFIG_STEP;
		if ( value > 1.0f ) {
			value = 1.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_RED ), value );
		break;
	case LCB_GREEN_DOWN:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_GREEN ) ) - LIGHT_CONFIG_STEP;
		if ( value < 0.0f ) {
			value = 0.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_GREEN ), value );
		break;
	case LCB_GREEN_UP:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_GREEN ) ) + LIGHT_CONFIG_STEP;
		if ( value > 1.0f ) {
			value = 1.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_GREEN ), value );
		break;
	case LCB_BLUE_DOWN:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_BLUE ) ) - LIGHT_CONFIG_STEP;
		if ( value < 0.0f ) {
			value = 0.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_BLUE ), value );
		break;
	case LCB_BLUE_UP:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_BLUE ) ) + LIGHT_CONFIG_STEP;
		if ( value > 1.0f ) {
			value = 1.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_BLUE ), value );
		break;
	case LCB_DIMMER_DOWN:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_DIMMER ) ) - LIGHT_CONFIG_STEP;
		if ( value < 0.0f ) {
			value = 0.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_DIMMER ), value );
		break;
	case LCB_DIMMER_UP:
		value = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_DIMMER ) ) + LIGHT_CONFIG_STEP;
		if ( value > 1.0f ) {
			value = 1.0f;
		}
		SetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_DIMMER ), value );
		break;
	case LCB_DONE:
		RemoveChild( dialog, true );
		delete dialog;
		return;
	}

	if ( dialog != NULL ) {
		TGParagraph *nameText = (TGParagraph *)pane->GetNthChild( CONFIG_NAME );
		TGString string;
		nameText->GetString( &string );
		char *name = string.Unknown6F56F0();
		LightPlacement *light = (LightPlacement *)PlacementObject::GetObject( g_kSetManager.GetRenderedSet(), name );
		delete [] name;
		if ( light != NULL ) {
			float red = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_RED ) );
			float green = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_GREEN ) );
			float blue = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_BLUE ) );
			float dimmer = GetParagraphValue( (TGParagraph *)pane->GetNthChild( LIGHT_CONFIG_DIMMER ) );
			dialog->GetText()->GetString( &string );
			char *title = string.Unknown6F56F0();
			if ( strstr( title, "Ambient" ) != NULL ) {
				light->ConfigAmbientLight( red, green, blue, dimmer );
			} else if ( strstr( title, "Directional" ) != NULL ) {
				light->ConfigDirectionalLight( red, green, blue, dimmer );
			}
			delete [] title;
		}
	}
}

/*
================
PlacementEditor::HandleWaypointConfig

Takes down the dialog that sent the event.
================
*/
// FUNCTION: STBC 0x0049e8d0
void PlacementEditor::HandleWaypointConfig( TGEvent *event ) {
	TGUIObject *dialog = (TGUIObject *)event->GetSource();
	if ( dialog != NULL && dialog->GetParent() != NULL ) {
		dialog->GetParent()->DeleteChild( dialog, true );
	}
}

/*
================
PlacementEditor::HandleAsteroidFieldConfig

Gives the field the dialog's values and rebuilds it; "Done" also takes
the dialog down.
================
*/
// FUNCTION: STBC 0x0049e8f0
void PlacementEditor::HandleAsteroidFieldConfig( TGEvent *event ) {
	TGDialogWindow *dialog = (TGDialogWindow *)event->GetSource();
	bool done = false;
	if ( dialog != NULL ) {
		TGPane *pane = dialog->GetSubPane();
		TGParagraph *nameText = (TGParagraph *)pane->GetNthChild( CONFIG_NAME );
		TGString string;
		nameText->GetString( &string );
		char *name = string.Unknown6F56F0();
		AsteroidFieldPlacement *field = AsteroidFieldPlacement::GetObject( g_kSetManager.GetRenderedSet(), name );
		delete [] name;
		switch ( ( (TGIntEvent *)event )->GetInt() ) {
		case FCB_DONE:
			done = true;
			// fall through
		case FCB_APPLY: {
			float radius = GetParagraphValue( (TGParagraph *)pane->GetNthChild( FIELD_CONFIG_RADIUS ) );
			int tiles = (int)GetParagraphValue( (TGParagraph *)pane->GetNthChild( FIELD_CONFIG_TILES ) );
			int asteroids = (int)GetParagraphValue( (TGParagraph *)pane->GetNthChild( FIELD_CONFIG_ASTEROIDS ) );
			float size = GetParagraphValue( (TGParagraph *)pane->GetNthChild( FIELD_CONFIG_SIZE ) );
			field->SetFieldRadius( radius );
			field->SetNumTilesPerAxis( tiles );
			field->SetNumAsteroidsPerTile( asteroids );
			field->SetAsteroidSizeFactor( size );
			field->ConfigField();
			if ( done == true ) {
				RemoveChild( dialog, true );
				delete dialog;
			}
			break;
		}
		}
	}
}

/*
================
PlacementEditor::GetParagraphValue
================
*/
// FUNCTION: STBC 0x0049ea90
float PlacementEditor::GetParagraphValue( TGParagraph *paragraph ) {
	TGString string;
	paragraph->GetString( &string );
	char *text = string.Unknown6F56F0();
	float value = (float)atof( text );
	delete [] text;
	return value;
}

/*
================
PlacementEditor::SetParagraphValue
================
*/
// FUNCTION: STBC 0x0049eb20
void PlacementEditor::SetParagraphValue( TGParagraph *paragraph, float value ) {
	char text[64];
	sprintf( text, "%1.1f", value );
	TGString string( text );
	paragraph->SetString( &string );
}

/*
================
PlacementEditor::SaveSet

Both scripts are written even when the first fails.
================
*/
// FUNCTION: STBC 0x0049eba0
bool PlacementEditor::SaveSet( char *regionFile, char *placementsFile, char *setName ) {
	bool result = true;
	result &= WriteRegionScript( regionFile, setName );
	result &= WritePlacementsScript( placementsFile, setName );
	return result;
}

/*
================
PlacementEditor::WriteRegionScript

The region script: Initialize makes the set and loads its placements,
backdrops and grid, then imports the set's "<set>_S" module of static
objects if there is one; LoadPlacements creates the static placements and
LoadBackdrops the set's backdrops. The module name comes from the file
name, after "scripts/", with its slashes made dots.
================
*/
// FUNCTION: STBC 0x0049ebd0
// PARKED: register: the module loop's i++ goes through ecx here and eax in retail (strlen's `or ecx, -1` scheduled between); unsigned i 97.6, a cast strlen 98.2, unsigned length flat
bool PlacementEditor::WriteRegionScript( char *fileName, char *setName ) {
	bool result = true;
	FILE *file = g_kSystemWrapper.FileOpen( fileName, "w" );
	if ( file == NULL ) {
		return false;
	}

	fprintf( file, "import App\n\n" );
	fprintf( file, "def Initialize():\n" );
	fprintf( file, "\t# Create the set (\"%s\")\n", setName );
	fprintf( file, "\tpSet = App.SetClass_Create()\n" );
	fprintf( file, "\tApp.g_kSetManager.AddSet(pSet, \"%s\")\n", setName );

	char *module = new char[strlen( fileName ) + 1];
	int i = 0;
	if ( strncmp( fileName, "scripts", 7 ) == 0 ) {
		i = 8;
	}
	int length = 0;
	for ( ; i < strlen( fileName ); i++ ) {
		char c = fileName[i];
		if ( c == '\\' || c == '/' ) {
			module[length] = '.';
		} else if ( c == '.' ) {
			break;
		} else {
			module[length] = c;
		}
		length++;
	}
	module[length] = '\0';
	fprintf( file, "\n\t# Save the name of the region file that's creating the set.\n" );
	fprintf( file, "\tpSet.SetRegionModule(\"%s\")\n", module );
	delete [] module;

	fprintf( file, "\n\t# Activate the proximity manager for our set.\n" );
	fprintf( file, "\tpSet.SetProximityManagerActive(1)\n" );
	fprintf( file, "\n\t# Load the placements and backdrops for this set.\n" );
	fprintf( file, "\tLoadPlacements(\"%s\")\n", setName );
	fprintf( file, "\tLoadBackdrops(pSet)\n" );
	fprintf( file, "\n\t#Load and place the grid.\n" );
	fprintf( file, "\tpGrid = App.GridClass_Create()\n" );
	fprintf( file, "\tpSet.AddObjectToSet(pGrid, \"grid\")\n" );
	fprintf( file, "\tpGrid.SetHidden(1)\n" );

	// BUG: the static module's name is never freed
	char *staticModule = new char[strlen( setName ) + 3];
	strcpy( staticModule, setName );
	strcat( staticModule, "_S" );
	fprintf( file, "\n\t# Create static objects for this set:\n" );
	fprintf( file, "\t# If you want to create static objects for this region, make a\n" );
	fprintf( file, "\t# \"%s.py\" file with an Initialize function that creates them.\n", staticModule );
	fprintf( file, "\ttry:\n" );
	fprintf( file, "\t\timport %s\n", staticModule );
	fprintf( file, "\t\t%s.Initialize(pSet)\n", staticModule );
	fprintf( file, "\texcept ImportError:\n" );
	fprintf( file, "\t\t# Couldn't find the file.  That's ok.  Do nothing...\n" );
	fprintf( file, "\t\tpass\n" );
	fprintf( file, "\n\t# Done.\n\n" );

	fprintf( file, "def GetSetName():\n" );
	fprintf( file, "\treturn \"%s\"\n\n", setName );
	fprintf( file, "def GetSet():\n" );
	fprintf( file, "\treturn App.g_kSetManager.GetSet(\"%s\")\n\n", setName );
	fprintf( file, "def Terminate():\n" );
	fprintf( file, "\tApp.g_kSetManager.DeleteSet(\"%s\")\n\n", setName );

	fprintf( file, "def LoadPlacements(sSetName):\n" );
	{
		int written = 0;
		int count = 0;
		PlacementObject **placements = PlacementObject::GetClassObjects( g_kSetManager.GetRenderedSet(), &count );
		for ( i = 0; i < count; i++ ) {
			if ( placements[i]->IsStatic() ) {
				result &= placements[i]->SaveObject( file, setName );
				written++;
			}
		}
		for ( i = 0; i < count; i++ ) {
			if ( placements[i]->IsStatic() ) {
				result &= placements[i]->SaveObjectSecondPass( file, setName );
			}
		}
		if ( written == 0 ) {
			fprintf( file, "\t# No static placements.\n" );
			fprintf( file, "\tpass\n" );
		}
	}
	result &= Backdrop::SaveSet( g_kSetManager.GetRenderedSet(), file );
	g_kSystemWrapper.FileClose( file );
	return result;
}

/*
================
PlacementEditor::WritePlacementsScript

The mission placements script: LoadPlacements creates the placements
that are not static.
================
*/
// FUNCTION: STBC 0x0049efe0
bool PlacementEditor::WritePlacementsScript( char *fileName, char *setName ) {
	bool result = true;
	FILE *file = g_kSystemWrapper.FileOpen( fileName, "w" );
	int written = 0;
	if ( file == NULL ) {
		return false;
	}

	fprintf( file, "import App\n\n" );
	fprintf( file, "def LoadPlacements(sSetName):\n" );
	int count = 0;
	PlacementObject **placements = PlacementObject::GetClassObjects( g_kSetManager.GetRenderedSet(), &count );
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( !placements[i]->IsStatic() ) {
			result &= placements[i]->SaveObject( file, setName );
			written++;
		}
	}
	for ( i = 0; i < count; i++ ) {
		if ( !placements[i]->IsStatic() ) {
			result &= placements[i]->SaveObjectSecondPass( file, setName );
		}
	}
	if ( written == 0 ) {
		fprintf( file, "\tpass\n" );
	}
	g_kSystemWrapper.FileClose( file );
	return result;
}

/*
================
PlacementEditor::PlaceInFrontOfCamera

Puts the object six units ahead of the active camera, facing the way
the camera faces.
================
*/
// FUNCTION: STBC 0x0049f0e0
void PlacementEditor::PlaceInFrontOfCamera( ObjectClass *object ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	object->UpdateNodeOnly();
	{
		NiPoint3 position = camera->GetWorldForward() * 6.0f + camera->GetWorldLocation();
		object->SetTranslate( &position );
	}
	object->AlignToVectors( camera->GetWorldForward(), camera->GetWorldUp() );
	object->UpdateNodeOnly();
}

/*
================
PlacementEditorMode::PlacementEditorMode
================
*/
// FUNCTION: STBC 0x0049f1a0
PlacementEditorMode::PlacementEditorMode( PlacementEditor *editor ) : EditorMode( editor ),
	leftKey( false ), rightKey( false ), unknown0A( false ), unknown0B( false ), upKey( false ), downKey( false ),
	endKey( false ), pageDownKey( false ), pageUpKey( false ), homeKey( false ) {
}

/*
================
PlacementEditorMode::RightClick

Drops a new waypoint where the click's ray meets a shown grid in front of
the camera, facing +Y with +Z up.
================
*/
// FUNCTION: STBC 0x0049f240
void PlacementEditorMode::RightClick( TGMouseEvent *event ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	CameraObjectClass *camera = set->GetActiveCamera();
	NiCamera *niCamera = camera->GetNiCamera();
	int count;
	GridClass **grids = GridClass::GetClassObjects( set, &count );
	while ( --count >= 0 ) {
		GridClass *grid = grids[count];
		if ( !grid->IsHidden() ) {
			NiPoint3 origin;
			NiPoint3 direction;
			event->GetPickRay( &origin, &direction, niCamera );
			NiPoint3 point = grid->Unknown499590( origin, direction );
			NiPoint3 offset = point - camera->GetWorldLocation();
			if ( offset.Dot( camera->GetWorldForward() ) > 0.0f ) {
				ObjectClass *waypoint = CreateWaypoint();
				waypoint->AlignToVectors( NiPoint3::UNIT_Y, NiPoint3::UNIT_Z );
				waypoint->SetTranslate( &point );
				waypoint->UpdateNodeOnly();
				return;
			}
		}
	}
}

/*
================
PlacementEditorMode::KeyDown

The camera keys fly while held.
================
*/
// FUNCTION: STBC 0x0049f3a0
void PlacementEditorMode::KeyDown( TGKeyboardEvent *event ) {
	if ( !event->EventHandled() ) {
		switch ( event->GetUnicode() ) {
		case WC_LEFT:
			leftKey = true;
			event->SetHandled();
			break;
		case WC_RIGHT:
			rightKey = true;
			event->SetHandled();
			break;
		case WC_UP:
			upKey = true;
			event->SetHandled();
			break;
		case WC_DOWN:
			downKey = true;
			event->SetHandled();
			break;
		case WC_END:
			endKey = true;
			event->SetHandled();
			break;
		case WC_PAGEDOWN:
			pageDownKey = true;
			event->SetHandled();
			break;
		case WC_PAGEUP:
			pageUpKey = true;
			event->SetHandled();
			break;
		case WC_HOME:
			homeKey = true;
			event->SetHandled();
			break;
		}
	}
}

/*
================
PlacementEditorMode::KeyTyped

'-' halves the camera keys' speed, down to 1/64, and '=' doubles it; the
menu keys send the editor their menu events.
================
*/
// FUNCTION: STBC 0x0049f460
void PlacementEditorMode::KeyTyped( TGKeyboardEvent *event ) {
	EditorMode::KeyTyped( event );
	if ( !event->EventHandled() ) {
		float &speed = GetEditor()->speed;
		switch ( event->GetUnicode() ) {
		case '-':
			if ( speed > 0.015625f ) {
				speed *= 0.5f;
			}
			event->SetHandled();
			break;
		case '=':
			speed *= 2.0f;
			event->SetHandled();
			break;
		}

		struct MenuKey {
			unsigned short	key;
			int				entry;
		} menuKeys[] = {
			{ 'c', PEM_CHASE_NEXT },
			{ 'C', PEM_CHASE_PREV },
			{ 'g', PEM_TOGGLE_GRID },
			{ 'm', PEM_MOVE_OBJECT },
			{ 'o', PEM_OVERHEAD_VIEW },
			{ 'p', PEM_TOGGLE_PLACEMENTS },
			{ 't', PEM_TARGET_NEXT },
			{ 'T', PEM_TARGET_PREV },
			{ 'd', PEM_TOGGLE_STATIC },
			{ 'q', PEM_OBJECT_TO_CAMERA },
			{ 'Q', PEM_CAMERA_TO_OBJECT },
			{ 0, 0 }
		};
		for ( int i = 0; menuKeys[i].key != 0; i++ ) {
			if ( menuKeys[i].key == event->GetUnicode() ) {
				TGIntEvent *menuEvent = new TGIntEvent;
				menuEvent->SetDestination( editor );
				menuEvent->SetEventType( ET_PLACEMENT_EDITOR_MENU );
				menuEvent->SetInt( menuKeys[i].entry );
				g_kEventManager.AddEvent( menuEvent );
				event->SetHandled();
				break;
			}
		}
	}
}

/*
================
PlacementEditorMode::KeyUp
================
*/
// FUNCTION: STBC 0x0049f620
void PlacementEditorMode::KeyUp( TGKeyboardEvent *event ) {
	if ( !event->EventHandled() ) {
		switch ( event->GetUnicode() ) {
		case WC_LEFT:
			leftKey = false;
			event->SetHandled();
			break;
		case WC_RIGHT:
			rightKey = false;
			event->SetHandled();
			break;
		case WC_UP:
			upKey = false;
			event->SetHandled();
			break;
		case WC_DOWN:
			downKey = false;
			event->SetHandled();
			break;
		case WC_END:
			endKey = false;
			event->SetHandled();
			break;
		case WC_PAGEDOWN:
			pageDownKey = false;
			event->SetHandled();
			break;
		case WC_PAGEUP:
			pageUpKey = false;
			event->SetHandled();
			break;
		case WC_HOME:
			homeKey = false;
			event->SetHandled();
			break;
		}
	}
}

/*
================
PlacementEditorMode::Update

Flies the camera with the keys held, at the editor's speed: the arrows
forward, back and sideways, page up and home up and down, end and page
down roll it. Moving frees the camera from any mode that follows an
object.
================
*/
// FUNCTION: STBC 0x0049f6e0
void PlacementEditorMode::Update( void ) {
	float frameTime = g_kSystemWrapper.GetTimeElapsedInSeconds();
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	NiPoint3 movement = NiPoint3::ZERO;
	if ( upKey || downKey || leftKey || rightKey || pageUpKey || homeKey ) {
		float held = upKey;
		movement += held * camera->GetWorldForward();
		held = downKey;
		movement -= held * camera->GetWorldForward();
		held = leftKey;
		movement -= held * camera->GetWorldRight();
		held = rightKey;
		movement += held * camera->GetWorldRight();
		held = pageUpKey;
		movement += held * camera->GetWorldUp();
		held = homeKey;
		movement -= held * camera->GetWorldUp();
		movement *= frameTime * GetEditor()->speed;
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
	// The address of the sum, a temporary: VC6 accepts it (C4238, a level 4
	// warning), and the temporary shares its frame slot with the others.
	camera->SetTranslate( &( movement + camera->GetTranslate() ) );
	if ( endKey || pageDownKey ) {
		NiPoint3 forward = camera->GetWorldForward();
		NiMatrix3 rotation;
		rotation.MakeRotation( (float)( endKey - pageDownKey ) * GetEditor()->speed * frameTime * 0.05f, forward );
		camera->Rotate( rotation );
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
}

/*
================
PlacementEditorMode::CanSelect

Ships and placement objects.
================
*/
// FUNCTION: STBC 0x0049fb00
bool PlacementEditorMode::CanSelect( ObjectClass *object ) {
	if ( object->IsTypeOf( SHIPCLASS_TYPE ) || object->IsTypeOf( PLACEMENTOBJECT_TYPE ) ) {
		return true;
	}
	return false;
}

/*
================
PlacementEditorMode::End
================
*/
// FUNCTION: STBC 0x0049fb30
void PlacementEditorMode::End( void ) {
	leftKey = false;
	rightKey = false;
	unknown0A = false;
	unknown0B = false;
	upKey = false;
	downKey = false;
	endKey = false;
	pageDownKey = false;
	pageUpKey = false;
	homeKey = false;
}

/*
================
PlacementEditorMode::CreateWaypoint
================
*/
// FUNCTION: STBC 0x0049fb60
ObjectClass *PlacementEditorMode::CreateWaypoint( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	char name[64];
	do {
		sprintf( name, "Placement %d", GetEditor()->placementNumber );
		GetEditor()->placementNumber++;
	} while ( PlacementObject::GetObject( set, name ) != NULL || ObjectClass::GetObject( set, name ) != NULL );

	Waypoint *waypoint = new Waypoint( name, set->GetName(), NULL );
	set->AddObjectToSet( waypoint, name );
	editor->SelectObject( waypoint );
	return waypoint;
}

/*
================
AttachWaypointMode::AttachWaypointMode
================
*/
// FUNCTION: STBC 0x0049fc20
AttachWaypointMode::AttachWaypointMode( PlacementEditor *editor ) : PlacementEditorMode( editor ) {
	help = new TGParagraph( "Click on the waypoint to attach after the currently selected object.", 1.0f, NULL, "", 0.0f, 0x80000000 );
}

/*
================
AttachWaypointMode::~AttachWaypointMode

The help is deleted unless the editor still holds it.
================
*/
// FUNCTION: STBC 0x0049fcd0
AttachWaypointMode::~AttachWaypointMode( void ) {
	if ( help->GetParent() == NULL ) {
		delete help;
		help = NULL;
	}
}

/*
================
AttachWaypointMode::LeftClick

The clicked waypoint follows the selected one; back to the base mode
either way.
================
*/
// FUNCTION: STBC 0x0049fd30
// PARKED: schedule, as EditorMode::LeftClick: retail forms the direction's address before pushing the camera
void AttachWaypointMode::LeftClick( TGMouseEvent *event ) {
	NiPoint3 origin;
	NiPoint3 direction;
	event->GetPickRay( &origin, &direction, g_kSetManager.GetRenderedSet()->GetActiveCamera()->GetNiCamera() );
	ObjectClass *object = editor->PickObject( &origin, &direction );
	if ( object != NULL ) {
		ObjectClass *selected = editor->GetSelectedObject();
		if ( selected->IsTypeOf( WAYPOINT_TYPE ) ) {
			( (Waypoint *)selected )->InsertAfterObj( (Waypoint *)object );
			editor->SelectObject( object );
		}
	}
	GetEditor()->SetMode( GetEditor()->selectMode );
}

/*
================
AttachWaypointMode::CanSelect
================
*/
// FUNCTION: STBC 0x0049fdc0
bool AttachWaypointMode::CanSelect( ObjectClass *object ) {
	return object->IsTypeOf( WAYPOINT_TYPE );
}

/*
================
AttachWaypointMode::Start

Shows the help at the bottom middle of the editor.
================
*/
// FUNCTION: STBC 0x0049fde0
void AttachWaypointMode::Start( void ) {
	editor->AddChild( help, ( 1.0f - help->GetWidth() ) * 0.5f, 1.0f - help->GetHeight() * 2.0f );
	help->SetVisible();
}

/*
================
AttachWaypointMode::End
================
*/
// FUNCTION: STBC 0x0049fe30
void AttachWaypointMode::End( void ) {
	help->SetNotVisible();
	editor->RemoveChild( help );
}

/*
================
InsertWaypointsMode::InsertWaypointsMode
================
*/
// FUNCTION: STBC 0x0049fe60
InsertWaypointsMode::InsertWaypointsMode( PlacementEditor *editor ) : PlacementEditorMode( editor ) {
	help = new TGParagraph( "Insert waypoints, and they will automatically attach themselves to the previous (selected) point to make a path.",
							1.0f, NULL, "", 0.0f, 0x80000000 );
}

/*
================
InsertWaypointsMode::~InsertWaypointsMode
================
*/
// FUNCTION: STBC 0x0049ff10
InsertWaypointsMode::~InsertWaypointsMode( void ) {
	if ( help->GetParent() == NULL ) {
		delete help;
		help = NULL;
	}
}

/*
================
InsertWaypointsMode::Start
================
*/
// FUNCTION: STBC 0x0049ff70
void InsertWaypointsMode::Start( void ) {
	editor->AddChild( help, ( 1.0f - help->GetWidth() ) * 0.5f, 1.0f - help->GetHeight() * 2.0f );
	help->SetVisible();
}

/*
================
InsertWaypointsMode::End
================
*/
// FUNCTION: STBC 0x0049ffc0
void InsertWaypointsMode::End( void ) {
	help->SetNotVisible();
	editor->RemoveChild( help );
}

/*
================
InsertWaypointsMode::CreateWaypoint

The new waypoint follows the selected one.
================
*/
// FUNCTION: STBC 0x004a0010
ObjectClass *InsertWaypointsMode::CreateWaypoint( void ) {
	ObjectClass *selected = editor->GetSelectedObject();
	ObjectClass *waypoint = PlacementEditorMode::CreateWaypoint();
	if ( waypoint->IsTypeOf( WAYPOINT_TYPE ) ) {
		if ( selected->IsTypeOf( WAYPOINT_TYPE ) ) {
			( (Waypoint *)selected )->InsertAfterObj( (Waypoint *)waypoint );
		}
		editor->SelectObject( waypoint );
	}
	return waypoint;
}

/*
================
MoveMode::MoveMode
================
*/
// FUNCTION: STBC 0x004a0060
MoveMode::MoveMode( PlacementEditor *editor ) : PlacementEditorMode( editor ) {
	help = new TGParagraph( "Move mode: If an object is selected, it will be moved instead of the camera", 1.0f, NULL, "", 0.0f, 0x80000000 );
}

/*
================
MoveMode::~MoveMode
================
*/
// FUNCTION: STBC 0x004a0110
MoveMode::~MoveMode( void ) {
	if ( help->GetParent() == NULL ) {
		delete help;
		help = NULL;
	}
}

/*
================
MoveMode::LeftDrag

Turns the selected object by the mouse's movement, and keeps the mouse
where the drag started; with nothing selected, turns the camera.
================
*/
// FUNCTION: STBC 0x004a0170
// PARKED: frame slots, as EditorMode::LeftDrag: retail's frame is 8 bytes larger and copies the drag position's y through a float slot; float locals and an NiPoint2 copy tried
void MoveMode::LeftDrag( NiPoint2 *lastPosition, TGMouseEvent *event ) {
	ObjectClass *object = editor->GetSelectedObject();
	if ( object == NULL ) {
		EditorMode::LeftDrag( lastPosition, event );
		return;
	}
	float dx = mouseMovement.x;
	float dy = mouseMovement.y;
	if ( dx != 0.0f ) {
		NiPoint3 axis;
		axis = object->GetWorldUp();
		NiMatrix3 rotation;
		rotation.MakeRotation( dx * 2.0f, axis );
		object->Rotate( rotation );
	}
	if ( dy != 0.0f ) {
		NiPoint3 axis;
		axis = object->GetWorldRight();
		NiMatrix3 rotation;
		rotation.MakeRotation( dy * 2.0f, axis );
		object->Rotate( rotation );
	}
	float x = lastPosition->x;
	float y = lastPosition->y;
	lastMousePosition.x = x;
	lastMousePosition.y = y;
	event->x = x;
	event->y = y;
}

/*
================
MoveMode::Update

The base mode's keys move and roll the selected object along its own
axes; with nothing selected they fly the camera. Like the base mode it
frees the camera from a mode that follows an object.
================
*/
// FUNCTION: STBC 0x004a02b0
void MoveMode::Update( void ) {
	ObjectClass *object = editor->GetSelectedObject();
	if ( object == NULL ) {
		PlacementEditorMode::Update();
		return;
	}
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	NiPoint3 movement = NiPoint3::ZERO;
	float frameTime = g_kSystemWrapper.GetTimeElapsedInSeconds();
	if ( upKey || downKey || leftKey || rightKey || pageUpKey || homeKey ) {
		float held = upKey;
		movement += held * object->GetWorldForward();
		held = downKey;
		movement -= held * object->GetWorldForward();
		held = leftKey;
		movement -= held * object->GetWorldRight();
		held = rightKey;
		movement += held * object->GetWorldRight();
		held = pageUpKey;
		movement += held * object->GetWorldUp();
		held = homeKey;
		movement -= held * object->GetWorldUp();
		movement *= frameTime * GetEditor()->speed;
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
	// The address of the sum, a temporary, as in PlacementEditorMode::Update.
	object->SetTranslate( &( movement + object->GetTranslate() ) );
	if ( endKey || pageDownKey ) {
		NiPoint3 forward = object->GetWorldForward();
		NiMatrix3 rotation;
		rotation.MakeRotation( (float)( endKey - pageDownKey ) * GetEditor()->speed * frameTime * 0.05f, forward );
		object->Rotate( rotation );
		CameraMode *mode = camera->GetCurrentCameraMode( true );
		if ( mode == NULL || mode->GetObjType() != CAMERAMODE_TYPE ) {
			camera->PushCameraMode( new CameraMode( camera ) );
		}
	}
	object->UpdateNodeOnly();
}

/*
================
MoveMode::Start
================
*/
// FUNCTION: STBC 0x004a06e0
void MoveMode::Start( void ) {
	editor->AddChild( help, ( 1.0f - help->GetWidth() ) * 0.5f, 1.0f - help->GetHeight() * 2.0f );
	help->SetVisible();
}

/*
================
MoveMode::End
================
*/
// FUNCTION: STBC 0x004a0730
void MoveMode::End( void ) {
	help->SetNotVisible();
	editor->RemoveChild( help );
}

