/*
===============================================================================

	Editor.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "Editor.h"
#include "BackgroundEditor.h"
#include "CameraObjectClass.h"
#include "EditorMode.h"
#include "NiCamera.h"
#include "NiMaterialProperty.h"
#include "NiNode.h"
#include "NiPick.h"
#include "NiVertexColorProperty.h"
#include "ObjectClass.h"
#include "PlacementEditor.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGDialogWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGParagraph.h"
#include "TGRect.h"
#include "TGTextButton.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include "Waypoint.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00498aa0 _$E486
// FUNCTION: STBC 0x00498ac0 _$E485

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004971a0 _$E32
// FUNCTION: STBC 0x004971b0 _$E29
// FUNCTION: STBC 0x004971c0 _$E31
// FUNCTION: STBC 0x004971d0 _$E30
// FUNCTION: STBC 0x00497220 _$E38
// FUNCTION: STBC 0x00497230 _$E35
// FUNCTION: STBC 0x00497240 _$E37
// FUNCTION: STBC 0x00497250 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00498040 _$E57
// FUNCTION: STBC 0x00498050 _$E56
// FUNCTION: STBC 0x00498160 _$E61
// FUNCTION: STBC 0x00498170 _$E60
// FUNCTION: STBC 0x004981b0 _$E72
// FUNCTION: STBC 0x004981c0 _$E71
// FUNCTION: STBC 0x004981d0 _$E76
// FUNCTION: STBC 0x004981e0 _$E75
// FUNCTION: STBC 0x004981f0 _$E80
// FUNCTION: STBC 0x00498200 _$E79
// FUNCTION: STBC 0x00498210 _$E84
// FUNCTION: STBC 0x00498220 _$E83
// FUNCTION: STBC 0x00498230 _$E208
// FUNCTION: STBC 0x00498240 _$E207
// FUNCTION: STBC 0x004983c0 _$E150
// FUNCTION: STBC 0x004983d0 _$E149
// FUNCTION: STBC 0x004983e0 _$E163
// FUNCTION: STBC 0x004983f0 _$E162
// FUNCTION: STBC 0x00498400 _$E213
// FUNCTION: STBC 0x00498410 _$E212
// FUNCTION: STBC 0x00498420 _$E218
// FUNCTION: STBC 0x00498430 _$E217
// FUNCTION: STBC 0x00498440 _$E222
// FUNCTION: STBC 0x00498450 _$E221
// FUNCTION: STBC 0x00498460 _$E226
// FUNCTION: STBC 0x00498470 _$E225
// FUNCTION: STBC 0x00498480 _$E233
// FUNCTION: STBC 0x00498490 _$E232
// FUNCTION: STBC 0x004984a0 _$E305
// FUNCTION: STBC 0x004984b0 _$E304
// FUNCTION: STBC 0x004984c0 _$E309
// FUNCTION: STBC 0x004984d0 _$E308
// FUNCTION: STBC 0x004984e0 _$E108
// FUNCTION: STBC 0x004984f0 _$E107
// FUNCTION: STBC 0x00498500 _$E291
// FUNCTION: STBC 0x00498510 _$E290
// FUNCTION: STBC 0x00498520 _$E298
// FUNCTION: STBC 0x00498530 _$E297
// FUNCTION: STBC 0x00498540 _$E321
// FUNCTION: STBC 0x00498550 _$E320
// FUNCTION: STBC 0x00498560 _$E325
// FUNCTION: STBC 0x00498570 _$E324
// FUNCTION: STBC 0x00498580 _$E365
// FUNCTION: STBC 0x00498590 _$E364
// FUNCTION: STBC 0x004985a0 _$E371
// FUNCTION: STBC 0x004985b0 _$E370
// FUNCTION: STBC 0x004985c0 _$E313
// FUNCTION: STBC 0x004985d0 _$E312
// FUNCTION: STBC 0x004985e0 _$E317
// FUNCTION: STBC 0x004985f0 _$E316
// FUNCTION: STBC 0x00498600 _$E329
// FUNCTION: STBC 0x00498610 _$E328
// FUNCTION: STBC 0x00498620 _$E333
// FUNCTION: STBC 0x00498630 _$E332
// FUNCTION: STBC 0x00498640 _$E337
// FUNCTION: STBC 0x00498650 _$E336
// FUNCTION: STBC 0x00498660 _$E341
// FUNCTION: STBC 0x00498670 _$E340
// FUNCTION: STBC 0x00498680 _$E345
// FUNCTION: STBC 0x00498690 _$E344
// FUNCTION: STBC 0x004986a0 _$E349
// FUNCTION: STBC 0x004986b0 _$E348
// FUNCTION: STBC 0x004986c0 _$E353
// FUNCTION: STBC 0x004986d0 _$E352
// FUNCTION: STBC 0x004986e0 _$E357
// FUNCTION: STBC 0x004986f0 _$E356
// FUNCTION: STBC 0x00498700 _$E113
// FUNCTION: STBC 0x00498710 _$E112
// FUNCTION: STBC 0x00498720 _$E361
// FUNCTION: STBC 0x00498730 _$E360
// FUNCTION: STBC 0x00498740 _$E118
// FUNCTION: STBC 0x00498750 _$E117
// FUNCTION: STBC 0x00498760 _$E375
// FUNCTION: STBC 0x00498770 _$E374
// FUNCTION: STBC 0x00498780 _$E379
// FUNCTION: STBC 0x00498790 _$E378
// FUNCTION: STBC 0x004987a0 _$E383
// FUNCTION: STBC 0x004987b0 _$E382
// FUNCTION: STBC 0x004987c0 _$E387
// FUNCTION: STBC 0x004987d0 _$E386
// FUNCTION: STBC 0x004987e0 _$E127
// FUNCTION: STBC 0x004987f0 _$E126
// FUNCTION: STBC 0x00498800 _$E391
// FUNCTION: STBC 0x00498810 _$E390
// FUNCTION: STBC 0x00498820 _$E395
// FUNCTION: STBC 0x00498830 _$E394
// FUNCTION: STBC 0x00498840 _$E399
// FUNCTION: STBC 0x00498850 _$E398
// FUNCTION: STBC 0x00498860 _$E133
// FUNCTION: STBC 0x00498870 _$E132
// FUNCTION: STBC 0x00498880 _$E138
// FUNCTION: STBC 0x00498890 _$E137
// FUNCTION: STBC 0x004988a0 _$E168
// FUNCTION: STBC 0x004988b0 _$E167
// FUNCTION: STBC 0x004988c0 _$E403
// FUNCTION: STBC 0x004988d0 _$E402
// FUNCTION: STBC 0x004988e0 _$E173
// FUNCTION: STBC 0x004988f0 _$E172
// FUNCTION: STBC 0x00498900 _$E407
// FUNCTION: STBC 0x00498910 _$E406
// FUNCTION: STBC 0x00498920 _$E411
// FUNCTION: STBC 0x00498930 _$E410
// FUNCTION: STBC 0x00498940 _$E178
// FUNCTION: STBC 0x00498950 _$E177
// FUNCTION: STBC 0x00498960 _$E415
// FUNCTION: STBC 0x00498970 _$E414
// FUNCTION: STBC 0x00498980 _$E143
// FUNCTION: STBC 0x00498990 _$E142
// FUNCTION: STBC 0x004989a0 _$E419
// FUNCTION: STBC 0x004989b0 _$E418
// FUNCTION: STBC 0x004989c0 _$E423
// FUNCTION: STBC 0x004989d0 _$E422
// FUNCTION: STBC 0x004989e0 _$E183
// FUNCTION: STBC 0x004989f0 _$E182
// FUNCTION: STBC 0x00498a00 _$E188
// FUNCTION: STBC 0x00498a10 _$E187
// FUNCTION: STBC 0x00498a20 _$E427
// FUNCTION: STBC 0x00498a30 _$E426
// FUNCTION: STBC 0x00498a40 _$E431
// FUNCTION: STBC 0x00498a50 _$E430
// FUNCTION: STBC 0x00498a60 _$E435
// FUNCTION: STBC 0x00498a70 _$E434
// FUNCTION: STBC 0x00498a80 _$E439
// FUNCTION: STBC 0x00498a90 _$E438
// FUNCTION: STBC 0x00498ad0 _$E443
// FUNCTION: STBC 0x00498ae0 _$E442
// FUNCTION: STBC 0x00498af0 _$E447
// FUNCTION: STBC 0x00498b00 _$E446
// FUNCTION: STBC 0x00498b10 _$E451
// FUNCTION: STBC 0x00498b20 _$E450
// FUNCTION: STBC 0x00498b30 _$E455
// FUNCTION: STBC 0x00498b40 _$E454
// FUNCTION: STBC 0x00498b50 _$E193
// FUNCTION: STBC 0x00498b60 _$E192
// FUNCTION: STBC 0x00498b70 _$E459
// FUNCTION: STBC 0x00498b80 _$E458
// FUNCTION: STBC 0x00498b90 _$E198
// FUNCTION: STBC 0x00498ba0 _$E197
// FUNCTION: STBC 0x00498bb0 _$E203
// FUNCTION: STBC 0x00498bc0 _$E202
// FUNCTION: STBC 0x00498bf0 _$E463
// FUNCTION: STBC 0x00498c00 _$E462
// FUNCTION: STBC 0x00498c10 _$E468
// FUNCTION: STBC 0x00498c20 _$E467
// FUNCTION: STBC 0x00498c30 _$E472
// FUNCTION: STBC 0x00498c40 _$E471

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00496a10 _$E478
// FUNCTION: STBC 0x00496a20 _$E474
// FUNCTION: STBC 0x00496a30 _$E477
// FUNCTION: STBC 0x00496a40 _$E475
// DATA: STBC 0x00983190
TGEventHandlerTable Editor::classHandlers;

// DATA: STBC 0x0098320c
Editor **Editor::editors;
// DATA: STBC 0x00983208
int Editor::numEditors;

// The name of the camera the editors look through.
#define EDITOR_CAMERA_NAME		"EditorCamera"

/*
================
Editor::Editor

Starts hidden, and joins the list of editors.
================
*/
// FUNCTION: STBC 0x00496a60
Editor::Editor( void ) : MainWindow( MWT_EDITOR, 1.0f, 1.0f ),
	nextEditor( NULL ), prevEditor( NULL ), menu( NULL ), mode( NULL ),
	leftClicked( false ), leftDown( false ), rightClicked( false ), rightDown( false ), lastMouse( 0.0f, 0.0f ),
	selectedID( 0 ), highlightedID( 0 ), nameLabel( NULL ), lastButton( NULL ) {
	SetNotVisible();

	Editor **list = new Editor *[numEditors + 1];
	if ( editors != NULL ) {
		memcpy( list, editors, numEditors * sizeof( Editor * ) );
		delete [] editors;
	}
	editors = list;
	editors[numEditors] = this;
	numEditors++;
}

/*
================
Editor::~Editor

Takes the editor camera away, restores the selected object's colours and
restarts the sets' update times, then deletes the other editors.
================
*/
// FUNCTION: STBC 0x00496b40
// PARKED: x87 copy: retail stores the game time through fld/fstp into a frame slot and pushes it with an integer move; the direct accessor, UtopiaModule's and named locals tried give integer moves or an fstp straight into the argument
Editor::~Editor( void ) {
	DeleteEditorCamera();
	ClearHighlight();
	g_kSetManager.Unknown418630( UtopiaApp::GetApp()->GetGameTime() );

	if ( editors != NULL ) {
		Editor **list = editors;
		editors = NULL;
		for ( int i = 0; i < numEditors; i++ ) {
			if ( list[i] != this ) {
				delete list[i];
			}
		}
		// BUG: the list itself is never freed
		numEditors = 0;
	}
}

/*
================
CreateEditors
================
*/
// FUNCTION: STBC 0x00496c10
PlacementEditor *CreateEditors( void ) {
	PlacementEditor *placementEditor = PlacementEditor::Create();
	BackgroundEditor *backgroundEditor = BackgroundEditor::Create();
	placementEditor->nextEditor = backgroundEditor;
	placementEditor->prevEditor = backgroundEditor;
	backgroundEditor->nextEditor = placementEditor;
	backgroundEditor->prevEditor = placementEditor;
	return placementEditor;
}

/*
================
DestroyEditors
================
*/
// FUNCTION: STBC 0x00496c30
void DestroyEditors( void ) {
	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_EDITOR );
	if ( window != NULL ) {
		delete window;
	}
}

/*
================
Editor::ShowEditor
================
*/
// FUNCTION: STBC 0x00496c50
void Editor::ShowEditor( Editor *editor ) {
	TopWindow *top = TopWindow::GetTopWindow();
	TGUIObject *child = NULL;
	int i;
	for ( i = 0; i < top->GetNumChildren(); i++ ) {
		child = top->GetNthChild( i );
		if ( child->IsTypeOf( EDITOR_TYPE ) ) {
			break;
		}
	}
	if ( i < top->GetNumChildren() ) {
		top->RemoveChild( child, false );
	}
	top->InsertChild( i, editor );
	if ( i == 0 ) {
		top->MoveToFront( editor );
	}
}

/*
================
Editor::Update
================
*/
// FUNCTION: STBC 0x00496ce0
void Editor::Update( float time ) {
	if ( HasFocus() ) {
		mode->Update();
		RefreshDisplay();
	}
}

/*
================
Editor::RefreshDisplay
================
*/
// FUNCTION: STBC 0x00496d10
void Editor::RefreshDisplay( void ) {
	ShowObjectName( GetSelectedObject() );
}

/*
================
Editor::GotFocus

Shows the editor and looks through the editor camera, the selection in red.
================
*/
// FUNCTION: STBC 0x00496d30
void Editor::GotFocus( bool layout ) {
	TGUIObject::GotFocus( false );
	SetVisible( layout );
	CreateEditorCamera();
	ObjectClass *object = ObjectClass::GetObjectByID( g_kSetManager.GetRenderedSet(), selectedID );
	if ( object != NULL ) {
		Highlight( object );
	}
}

/*
================
Editor::LostFocus

Forgets the buttons held.
================
*/
// FUNCTION: STBC 0x00496d80
void Editor::LostFocus( bool layout ) {
	TGUIObject::LostFocus( layout );
	leftClicked = false;
	leftDown = false;
	rightClicked = false;
	rightDown = false;
}

/*
================
Editor::RegisterHandlerNames

Registers the handlers of every editor class.
================
*/
// FUNCTION: STBC 0x00496da0
void Editor::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Editor::MouseHandler, "Editor::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Editor::KeyboardHandler, "Editor::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Editor::MenuHandler, "Editor::MenuHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Editor::ErrorDialogHandler, "Editor::ErrorDialogHandler" );
	PlacementEditor::RegisterHandlerNames();
	BackgroundEditor::RegisterHandlerNames();
	Waypoint::RegisterHandlerNames();
}

/*
================
Editor::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00496e00
void Editor::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "Editor::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "Editor::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_EDITOR_MENU, "Editor::MenuHandler" );
	PlacementEditor::RegisterHandlers();
	BackgroundEditor::RegisterHandlers();
	Waypoint::RegisterHandlers();
}

/*
================
Editor::MouseHandler

Passes the mouse to the mode: a click is a button going down and up with
no drag between; a drag reports where the mouse was before.
================
*/
// FUNCTION: STBC 0x00496e50
void Editor::MouseHandler( TGMouseEvent *event ) {
	CallNextHandler( event );
	if ( event->EventHandled() ) {
		return;
	}
	if ( event->IsButtonEvent() ) {
		if ( event->GetButtonNum() == MEF_BUTTON_LEFT ) {
			if ( event->GetFlags() & MEF_BUTTON_DOWN ) {
				leftClicked = true;
				leftDown = true;
				event->GetPosition( &lastMouse );
			} else if ( event->GetFlags() & MEF_BUTTON_UP ) {
				leftDown = false;
				if ( leftClicked ) {
					mode->LeftClick( event );
				}
			}
		} else if ( event->GetButtonNum() == MEF_BUTTON_RIGHT ) {
			if ( event->GetFlags() & MEF_BUTTON_DOWN ) {
				rightClicked = true;
				rightDown = true;
				event->GetPosition( &lastMouse );
			} else if ( event->GetFlags() & MEF_BUTTON_UP ) {
				rightDown = false;
				if ( rightClicked ) {
					mode->RightClick( event );
				}
			}
		}
	} else if ( leftDown ) {
		leftClicked = false;
		mode->LeftDrag( &lastMouse, event );
		event->GetPosition( &lastMouse );
	} else if ( rightDown ) {
		rightClicked = false;
		mode->RightDrag( &lastMouse, event );
		event->GetPosition( &lastMouse );
	} else {
		mode->MouseMove( event );
	}
	event->SetHandled();
}

/*
================
Editor::KeyboardHandler
================
*/
// FUNCTION: STBC 0x00496f90
void Editor::KeyboardHandler( TGKeyboardEvent *event ) {
	CallNextHandler( event );
	if ( !event->EventHandled() ) {
		switch ( event->GetKeyState() ) {
		case KS_NORMAL:
			mode->KeyTyped( event );
			break;
		case KS_KEYDOWN:
			mode->KeyDown( event );
			break;
		case KS_KEYUP:
			mode->KeyUp( event );
			break;
		}
	}
}

/*
================
Editor::MenuHandler
================
*/
// FUNCTION: STBC 0x00496fe0
void Editor::MenuHandler( TGEvent *event ) {
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
	case EM_TOGGLE_MENU:
		if ( menu->IsVisible() ) {
			menu->SetNotVisible();
		} else {
			menu->SetVisible();
		}
		break;
	case EM_NEXT_EDITOR:
		ShowEditor( nextEditor );
		break;
	case EM_PREV_EDITOR:
		ShowEditor( prevEditor );
		break;
	case EM_CLEAR_SELECTION:
		SelectObject( NULL );
		break;
	case EM_SELECT_PREV:
		SelectPrev();
		break;
	case EM_SELECT_NEXT:
		SelectNext();
		break;
	}
	CallNextHandler( event );
}

/*
================
Editor::ErrorDialogHandler

Takes down the error dialog that sent the event.
================
*/
// FUNCTION: STBC 0x004970c0
void Editor::ErrorDialogHandler( TGEvent *event ) {
	TGDialogWindow *dialog = (TGDialogWindow *)event->GetSource();
	dialog->SetNotVisible();
	RemoveChild( dialog );
	delete dialog;
}

/*
================
Editor::SelectObject
================
*/
// FUNCTION: STBC 0x00497100
void Editor::SelectObject( ObjectClass *object ) {
	if ( object != NULL ) {
		selectedID = object->GetObjID();
		Highlight( object );
		ShowObjectName( object );
	} else {
		selectedID = 0;
		ClearHighlight();
		ShowObjectName( NULL );
	}
}

/*
================
Editor::GetSelectedObject
================
*/
// FUNCTION: STBC 0x00497150
ObjectClass *Editor::GetSelectedObject( void ) {
	return ObjectClass::GetObjectByID( g_kSetManager.GetRenderedSet(), selectedID );
}

/*
================
Editor::SetMode
================
*/
// FUNCTION: STBC 0x00497170
void Editor::SetMode( EditorMode *newMode ) {
	if ( newMode != mode ) {
		if ( mode != NULL ) {
			mode->End();
		}
		mode = newMode;
		mode->Start();
	}
}

/*
================
Editor::SelectPrev

"Select Prev" steps forward through the IDs, and "Select Next" back.
================
*/
// FUNCTION: STBC 0x004971e0
void Editor::SelectPrev( void ) {
	ObjectClass *object = FindNextSelectable( selectedID );
	if ( object != NULL ) {
		SelectObject( object );
	}
}

/*
================
Editor::SelectNext
================
*/
// FUNCTION: STBC 0x00497200
void Editor::SelectNext( void ) {
	ObjectClass *object = FindPrevSelectable( selectedID );
	if ( object != NULL ) {
		SelectObject( object );
	}
}

/*
================
Editor::Highlight
================
*/
// FUNCTION: STBC 0x00497260
void Editor::Highlight( ObjectClass *object ) {
	ClearHighlight();
	if ( object != NULL ) {
		highlightedID = object->GetObjID();
		HighlightNode( object->GetNiObject(), true );
	}
}

/*
================
Editor::HighlightNode

Gives the node and its children red materials and unlit vertex colours
long enough to bake them into their render state, then puts the old
properties back: the red lasts until the next UpdateProperties.
================
*/
// FUNCTION: STBC 0x00497290
void Editor::HighlightNode( NiAVObject *node, bool top ) {
	bool changed = false;
	NiVertexColorProperty *vertexColor = NULL;
	NiMaterialProperty *material = NULL;

	NiPointer<NiProperty> oldVertexColor = node->GetProperty( NiVertexColorProperty::VERTEX_COLOR );
	if ( oldVertexColor ) {
		vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		if ( oldVertexColor ) {
			node->DetachProperty( oldVertexColor );
		}
		node->AttachProperty( vertexColor );
		changed = true;
	}

	NiPointer<NiProperty> oldMaterial = node->GetProperty( NiMaterialProperty::MATERIAL );
	if ( oldMaterial ) {
		material = new NiMaterialProperty;
		material->SetAmbientColor( NiColor( 1.0f, 0.0f, 0.0f ) );
		material->SetDiffuseColor( NiColor( 1.0f, 0.0f, 0.0f ) );
		material->SetEmittance( NiColor( 1.0f, 0.0f, 0.0f ) );
		if ( oldMaterial ) {
			node->DetachProperty( oldMaterial );
		}
		node->AttachProperty( material );
		changed = true;
	}

	if ( changed ) {
		node->UpdateProperties();
	}
	if ( vertexColor ) {
		node->DetachProperty( vertexColor );
	}
	if ( oldVertexColor ) {
		node->AttachProperty( oldVertexColor );
	}
	if ( material ) {
		node->DetachProperty( material );
	}
	if ( oldMaterial ) {
		node->AttachProperty( oldMaterial );
	}

	NiNode *parent = NiDynamicCast( NiNode, node );
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetChildCount(); i++ ) {
			NiAVObject *child = parent->GetAt( i );
			if ( child != NULL ) {
				HighlightNode( child, false );
			}
		}
	}
}

/*
================
Editor::ClearHighlight

Restores the colours of the object shown in red.
================
*/
// FUNCTION: STBC 0x004975b0
void Editor::ClearHighlight( void ) {
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, highlightedID );
	if ( object != NULL ) {
		object->GetNiObject()->UpdateProperties();
	}
	highlightedID = 0;
}

/*
================
Editor::ShowObjectName

Keeps the name paragraph over the object's place on the screen, made anew
when the name changes; the middle of the screen when the object is out of
view.
================
*/
// FUNCTION: STBC 0x004975e0
void Editor::ShowObjectName( ObjectClass *object ) {
	char *name = "{unnamed}";
	if ( object != NULL ) {
		if ( object->GetName() != NULL ) {
			name = object->GetName();
		}
		if ( nameLabel != NULL ) {
			TGString text( "" );
			nameLabel->GetString( &text );
			if ( text.CompareC( name, true ) != 0 ) {
				DeleteChild( nameLabel, false );
				nameLabel = NULL;
			}
		}
		if ( nameLabel == NULL ) {
			nameLabel = new TGParagraph( name, 1.0f, NULL, "", 0.0f, 0x80000000 );
			AddChild( nameLabel );
		}

		NiCamera *camera = object->GetContainingSet()->GetActiveNiCamera();
		if ( camera != NULL ) {
			NiPoint3 location = object->GetWorldLocation();
			NiPoint2 screen;
			if ( !camera->WorldPtToScreenPt( location, screen.x, screen.y ) ) {
				screen.x = 0.5f;
				screen.y = 0.5f;
			}
			NiPoint2 position;
			nameLabel->GetPosition( &position );
			// BUG: compares the top with the screen y, but places the paragraph at 1 - y
			if ( position.x != screen.x || position.y != screen.y ) {
				nameLabel->SetPosition( screen.x, 1.0f - screen.y );
			}
		}
	} else if ( nameLabel != NULL ) {
		delete nameLabel;
		nameLabel = NULL;
	}
}

/*
================
Editor::CreateMenu
================
*/
// FUNCTION: STBC 0x004977c0
void Editor::CreateMenu( char *title ) {
	menu = TGDialogWindow::CreateFromTheme( 0.3f, 0.65f, title, TGDialogWindow::DFT_DRAGGABLE, NULL );
	AddChild( menu );
	AddMenuButton( "Hide Menu", ET_EDITOR_MENU, EM_TOGGLE_MENU, 1.0f );
	AddMenuButton( "Prev Editor", ET_EDITOR_MENU, EM_PREV_EDITOR, 0.5f );
	AddMenuButton( "Next Editor", ET_EDITOR_MENU, EM_NEXT_EDITOR, 0.5f );
	AddMenuButton( "Clear Selection", ET_EDITOR_MENU, EM_CLEAR_SELECTION, 1.0f );
	AddMenuButton( "Select Prev", ET_EDITOR_MENU, EM_SELECT_PREV, 0.5f );
	AddMenuButton( "Select Next", ET_EDITOR_MENU, EM_SELECT_NEXT, 0.5f );
}

/*
================
Editor::AddMenuButton
================
*/
// FUNCTION: STBC 0x00497890
// PARKED: x87: retail copies the last button's top through the FPU and evaluates the pane's width before its left in the wrap test; GetRight, explicit operand orders tried
TGTextButton *Editor::AddMenuButton( char *label, int eventType, int value, float width ) {
	TGRect paneBounds;
	TGRect rect;
	menu->GetSubPane()->GetBounds( &paneBounds );
	rect.width = paneBounds.width * width;
	rect.height = -1.0f;
	if ( lastButton != NULL ) {
		rect.left = lastButton->GetRight();
		rect.top = lastButton->GetTop();
		if ( rect.GetRight() > paneBounds.GetRight() ) {
			rect.left = 0.0f;
			rect.top = lastButton->GetBottom();
		}
	} else {
		rect.left = 0.0f;
		rect.top = 0.025f;
	}
	lastButton = CreateButton( label, eventType, value, rect );

	float overflow = lastButton->GetBottom() - paneBounds.height;
	if ( overflow > 0.0f ) {
		menu->Resize( menu->GetWidth(), menu->GetHeight() + overflow );
	}
	return lastButton;
}

/*
================
Editor::CreateButton

A width or height of -1 keeps the button's own.
================
*/
// FUNCTION: STBC 0x00497a00
// PARKED: x87 copy: retail passes rect.top, width and height through the FPU and rect.left with an integer move; member and accessor reads tried
TGTextButton *Editor::CreateButton( char *label, int eventType, int value, TGRect rect ) {
	TGTextButton *button = new TGTextButton( label, "FrameButton", 0, 0.3f, 0, 0.01f, 0.005f );
	menu->AddChild( button );
	button->Move( rect.GetLeft(), rect.GetTop(), false );
	if ( rect.GetWidth() == -1.0f ) {
		rect.width = button->GetWidth();
	}
	if ( rect.GetHeight() == -1.0f ) {
		rect.height = button->GetHeight();
	}
	button->Resize( rect.GetWidth(), rect.GetHeight(), false );
	button->AlignTextHorizontal( ALIGN_CENTER );
	button->AlignTextVertical( ALIGN_MIDDLE );

	TGIntEvent *event = new TGIntEvent;
	event->SetDestination( this );
	event->SetEventType( eventType );
	event->SetInt( value );
	button->SetActivationEvent( event );
	return button;
}

/*
================
Editor::CreateModes
================
*/
// FUNCTION: STBC 0x00497b90
void Editor::CreateModes( void ) {
	SetMode( new EditorMode( this ) );
}

/*
================
Editor::ShowError
================
*/
// FUNCTION: STBC 0x00497bc0
void Editor::ShowError( char *message ) {
	TGDialogWindow *dialog = TGDialogWindow::CreateFromTheme( 0.35f, 0.2f, "Error", TGDialogWindow::DFT_OK | TGDialogWindow::DFT_DRAGGABLE, NULL );
	dialog->AddChild( new TGParagraph( message, 1.0f, NULL, "", 0.0f, 0x80000000 ) );
	AddChild( dialog, 0.3f, 0.4f, false );
	SetFocus( dialog, false );
	dialog->Layout();
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "Editor::ErrorDialogHandler", dialog );
}

/*
================
Editor::FindPrevSelectable
================
*/
// FUNCTION: STBC 0x00497c90
ObjectClass *Editor::FindPrevSelectable( long id ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	ObjectClass *result = NULL;
	int count = set->objects.GetCount();
	while ( count-- ) {
		ObjectClass *object = set->GetPreviousObject( id );
		if ( object != NULL ) {
			id = object->GetObjID();
			if ( mode->CanSelect( object ) ) {
				result = object;
				break;
			}
		}
	}
	return result;
}

/*
================
Editor::FindNextSelectable
================
*/
// FUNCTION: STBC 0x00497cf0
ObjectClass *Editor::FindNextSelectable( long id ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	ObjectClass *result = NULL;
	int count = set->objects.GetCount();
	while ( count-- ) {
		ObjectClass *object = set->GetNextObject( id );
		if ( object != NULL ) {
			id = object->GetObjID();
			if ( mode->CanSelect( object ) ) {
				result = object;
				break;
			}
		}
	}
	return result;
}

/*
================
Editor::PickObject
================
*/
// FUNCTION: STBC 0x00497d50
ObjectClass *Editor::PickObject( NiPoint3 *origin, NiPoint3 *direction ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	// BUG: the camera is looked up and not used
	set->GetActiveCamera();
	set->StartPick( origin, direction, false );
	float nearest = 1.0e20f;
	ObjectClass *picked = NULL;
	NiPick::Record *record;
	ObjectClass *object;
	while ( ( object = set->GetPickedObject( &record ) ) != NULL ) {
		if ( mode->CanSelect( object ) ) {
			float distance = record->distance;
			if ( distance < nearest ) {
				nearest = distance;
				picked = object;
			}
		}
	}
	return picked;
}

/*
================
Editor::CreateEditorCamera

Makes the editor camera where the active camera is, unless it is the
active camera already.
================
*/
// FUNCTION: STBC 0x00497de0
void Editor::CreateEditorCamera( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	CameraObjectClass *active = set->GetActiveCamera();
	if ( active == NULL || strcmp( active->GetName(), EDITOR_CAMERA_NAME ) != 0 ) {
		if ( set->GetCamera( EDITOR_CAMERA_NAME ) != NULL ) {
			set->DeleteCameraFromSet( EDITOR_CAMERA_NAME );
		}
		CameraObjectClass *camera;
		if ( active != NULL ) {
			NiPoint3 location = active->GetWorldLocation();
			camera = CameraObjectClass::Create( location.x, location.y, location.z, 0.0f, 0.0f, 0.0f, 0.0f, EDITOR_CAMERA_NAME );
		} else {
			camera = CameraObjectClass::Create( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, EDITOR_CAMERA_NAME );
		}
		set->AddCameraToSet( camera, EDITOR_CAMERA_NAME );
		set->SetActiveCamera( camera->GetName() );
	}
}

/*
================
Editor::DeleteEditorCamera
================
*/
// FUNCTION: STBC 0x00497ed0
void Editor::DeleteEditorCamera( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	if ( set != NULL && set->GetCamera( EDITOR_CAMERA_NAME ) != NULL ) {
		set->DeleteCameraFromSet( EDITOR_CAMERA_NAME );
	}
}

/*
================
EditorMode::LeftClick

Selects what the mouse points at.
================
*/
// FUNCTION: STBC 0x00497f00
// PARKED: schedule: retail forms the direction's address before pushing the camera; a camera local, reference parameters and the local order tried
void EditorMode::LeftClick( TGMouseEvent *event ) {
	NiPoint3 direction;
	NiPoint3 origin;
	event->GetPickRay( &origin, &direction, g_kSetManager.GetRenderedSet()->GetActiveCamera()->GetNiCamera() );
	ObjectClass *object = editor->PickObject( &origin, &direction );
	if ( object != NULL ) {
		editor->SelectObject( object );
	}
}

/*
================
EditorMode::LeftDrag

Turns the camera by the mouse's movement.
================
*/
// FUNCTION: STBC 0x00497f60
// PARKED: frame slots: retail keeps 8 bytes between the two movement copies; an NiPoint2 copy and two float locals tried
void EditorMode::LeftDrag( NiPoint2 *lastPosition, TGMouseEvent *event ) {
	float dx = mouseMovement.x;
	float dy = mouseMovement.y;
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	if ( dx != 0.0f ) {
		NiPoint3 axis;
		axis = camera->GetWorldUp();
		NiMatrix3 rotation;
		rotation.MakeRotation( dx * 2.0f, axis );
		camera->Rotate( rotation );
	}
	if ( dy != 0.0f ) {
		NiPoint3 axis;
		axis = camera->GetWorldRight();
		NiMatrix3 rotation;
		rotation.MakeRotation( dy * 2.0f, axis );
		camera->Rotate( rotation );
	}
}

/*
================
EditorMode::KeyTyped

Turns the editor's menu keys into menu events.
================
*/
// FUNCTION: STBC 0x00498060
void EditorMode::KeyTyped( TGKeyboardEvent *event ) {
	struct {
		unsigned short	key;
		int				entry;
	} keys[] = {
		{ '\\', EM_TOGGLE_MENU },
		{ '>', EM_NEXT_EDITOR },
		{ '<', EM_PREV_EDITOR },
		{ 's', EM_SELECT_NEXT },
		{ 'S', EM_SELECT_PREV },
		{ 'x', EM_CLEAR_SELECTION },
		{ 0, 0 }
	};

	if ( event->GetKeyState() == KS_NORMAL ) {
		for ( int i = 0; keys[i].key != 0; i++ ) {
			if ( keys[i].key == event->GetUnicode() ) {
				TGIntEvent *menuEvent = new TGIntEvent;
				menuEvent->SetDestination( editor );
				menuEvent->SetInt( keys[i].entry );
				menuEvent->SetEventType( ET_EDITOR_MENU );
				g_kEventManager.AddEvent( menuEvent );
				event->SetHandled();
				return;
			}
		}
	}
}

/*
================
EditorMode::Update
================
*/
// FUNCTION: STBC 0x00498180
void EditorMode::Update( void ) {
	CameraObjectClass *camera = g_kSetManager.GetRenderedSet()->GetActiveCamera();
	camera->Update( UtopiaApp::GetApp()->GetGameTime() );
}
