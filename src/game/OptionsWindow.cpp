/*
===============================================================================

	OptionsWindow.cpp

	The options menu's main window: the panes it hides while it is up, and the
	object that had the focus.

===============================================================================
*/

#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "OptionsWindow.h"
#include "Game.h"
#include "ModalDialogWindow.h"
#include "NiMessageLoop.h"
#include "PyEmbed.h"
#include "TGConfigMapping.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGStringEvent.h"
#include "TGUIObject.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"

/*
================
OptionsWindow::Initialize

Has the scripts build the main menu, then shows the window.
================
*/
// FUNCTION: STBC 0x0050EE30
void OptionsWindow::Initialize( void ) {
	if ( PyEmbed::CallFunction( "MainMenu.mainmenu", "BuildMainMenu", NULL, NULL, NULL ) == -1 ) {
		PyErr_Print();
		return;
	}
	SetVisible( true );
}

/*
================
OptionsWindow::KeyboardHandler
================
*/
// FUNCTION: STBC 0x0050ee70
void OptionsWindow::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
OptionsWindow::NewGameHandler

A game that runs is ended first, with the event kept for the next one to
start from; otherwise the network and the AI setup are reset and the game
the event names starts.
================
*/
// FUNCTION: STBC 0x0050ee90
void OptionsWindow::NewGameHandler( TGStringEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		event->IncRefCount();
		game->Terminate( event );
		return;
	}
	g_kUtopiaModule.TerminateNetwork();
	PyEmbed::CallFunction( "AI.Setup", "GameInit", "", NULL, "()" );
	PyErr_Print();	// BUG: the result of the call isn't tested, so this prints whether it failed or not
	char *script = event->GetCString();
	new Game( script );
	delete [] script;
}

/*
================
OptionsWindow::NewMultiplayerGameHandler

A game that runs is ended first, with the event kept; otherwise the multiplayer
window comes up.
================
*/
// FUNCTION: STBC 0x0050ef40
void OptionsWindow::NewMultiplayerGameHandler( TGEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		event->IncRefCount();
		game->Terminate( event );
		return;
	}
	SetNotVisible( true );
	TopWindow *topWindow = TopWindow::GetTopWindow();
	MainWindow *multiplayer = topWindow->FindMainWindow( MWT_MULTIPLAYER );
	topWindow->MoveToFront( multiplayer, true );
}

/*
================
OptionsWindow::QuitHandler

While the options menu or the modal dialog isn't the active window the quit
waits. A quit that isn't forced asks first, unless the options file sets the
confirmation to 0; the program then ends at once when no event is queued, or
once the queued ones have run.
================
*/
// FUNCTION: STBC 0x0050ef90
// DRAFT: complete; register allocation: retail keeps the top window in ebx, this in edi and the dialog in esi (then the event), this build rotates them
void OptionsWindow::QuitHandler( TGIntEvent *event ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	MainWindow *dialog = topWindow->FindMainWindow( MWT_MODAL_DIALOG );
	if ( !( IsWindowActive() && IsCompletelyVisible() ) && dialog != NULL && !dialog->IsWindowActive() ) {
		CallNextHandler( event );
		return;
	}
	if ( event->GetInt() == 0 ) {
		if ( !g_kConfigMapping.HasValue( "Annoyances", "Quit Game Confirmation" ) || g_kConfigMapping.GetIntValue( "Annoyances", "Quit Game Confirmation" ) != 0 ) {
			( (ModalDialogWindow *)topWindow->FindMainWindow( MWT_MODAL_DIALOG ) )->RunQuitDialog();
			CallNextHandler( event );
			return;
		}
	}
	if ( g_kEventManager.GetNumQueuedEvents() == 0 ) {
		NiApplication::application->GetMessageLoop()->Quit();
	} else {
		g_kEventManager.AddEvent( event );
	}
	CallNextHandler( event );
}

/*
================
OptionsWindow::GotFocus
================
*/
// FUNCTION: STBC 0x0050F0A0
void OptionsWindow::GotFocus( bool layout ) {
	TGUIObject::GotFocus( false );

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "MainMenu.mainmenu", "GotFocus", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}

/*
================
OptionsWindow::LostFocus
================
*/
// FUNCTION: STBC 0x0050F100
void OptionsWindow::LostFocus( bool layout ) {
	TGUIObject::LostFocus( false );

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "MainMenu.mainmenu", "LostFocus", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}

/*
================
OptionsWindow::SetVisible

Lets the scripts react, hides every other pane of the top window (listing
the visible ones to show again, and the one that had the focus), shows
the window and brings it to the front.
================
*/
// FUNCTION: STBC 0x0050F180
void OptionsWindow::SetVisible( bool layout ) {
	g_kEventManager.Unknown6DA2C0();

	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "MainMenu.mainmenu", "SetVisible", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}

	TopWindow *top = TopWindow::GetTopWindow();
	if ( top != NULL ) {
		int i;

		previouslyVisible.RemoveAll();
		int numChildren = top->GetNumChildren();
		for ( i = 0; i < numChildren; i++ ) {
			TGUIObject *child = top->GetNthChild( i );
			if ( child != NULL && child != this && ( child->GetFlags() & TGUIO_VISIBLE ) ) {
				previouslyVisible.Add( (TGUIObject *)child->GetObjID() );
				if ( child->HasFocus() ) {
					hasFocusObjectID = child->GetObjID();
				}
				child->SetNotVisible( true );
			}
		}
	}

	MainWindow::SetVisible( layout );
	TopWindow::GetTopWindow()->MoveToFront( this, true );
}

/*
================
OptionsWindow::SetNotVisible

Lets the scripts react, shows the panes that were visible before and
gives the focus back, then hides the window.
================
*/
// FUNCTION: STBC 0x0050F350
void OptionsWindow::SetNotVisible( bool layout ) {
	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "MainMenu.mainmenu", "SetNotVisible", "", NULL, "(Oi)", self, layout );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}

	MainWindow::SetNotVisible( layout );
	TopWindow *top = TopWindow::GetTopWindow();
	if ( top != NULL ) {
		int count = previouslyVisible.GetSize();
		int i;

		for ( i = 0; i < count; i++ ) {
			TGUIObject *pane = (TGUIObject *)TGObject::GetTGObjectPtr( (long)previouslyVisible.GetAt( i ) );
			if ( pane != NULL ) {
				pane->SetVisible( true );
			}
		}
		if ( hasFocusObjectID != 0 ) {
			TGUIObject *focusObject = (TGUIObject *)TGObject::GetTGObjectPtr( hasFocusObjectID );
			top->SetFocus( focusObject, true );
			hasFocusObjectID = 0;
		}
		previouslyVisible.RemoveAll();
	}
	TopWindow::GetTopWindow()->MoveToBack( this, true );
}

/*
================
OptionsWindow::SetHasFocusObject

Remembers the object that has the focus, by its ID.
================
*/
// FUNCTION: STBC 0x0050F470
void OptionsWindow::SetHasFocusObject( TGUIObject *object ) {
	hasFocusObjectID = object->GetObjID();
}

/*
================
OptionsWindow::AddToPreviouslyVisible

Lists a pane to show again, unless it is listed already.
================
*/
// FUNCTION: STBC 0x0050F480
void OptionsWindow::AddToPreviouslyVisible( TGUIObject *object ) {
	int i;

	for ( i = 0; i < (int)previouslyVisible.GetSize(); i++ ) {
		if ( (long)previouslyVisible.GetAt( i ) == object->GetObjID() ) {
			return;
		}
	}
	previouslyVisible.Add( (TGUIObject *)object->GetObjID() );
}

/*
================
OptionsWindow::RemoveFromPreviouslyVisible

Takes a pane out of the list.
================
*/
// FUNCTION: STBC 0x0050F590
void OptionsWindow::RemoveFromPreviouslyVisible( TGUIObject *object ) {
	int i;

	for ( i = 0; i < (int)previouslyVisible.GetSize(); i++ ) {
		if ( (long)previouslyVisible.GetAt( i ) == object->GetObjID() ) {
			if ( i < (int)previouslyVisible.GetSize() ) {
				previouslyVisible.RemoveAt( i );
			}
			return;
		}
	}
}

/*
================
OptionsWindow::RemoveAllPreviouslyVisible

Empties the list and forgets the focus object.
================
*/
// FUNCTION: STBC 0x0050F5F0
void OptionsWindow::RemoveAllPreviouslyVisible( void ) {
	previouslyVisible.RemoveAll();
	hasFocusObjectID = 0;
}
