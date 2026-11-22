/*
===============================================================================

	STMenuEvents.cpp

	The rest of STMenu, in a unit of its own: the focus changes, the
	colours of the menu's states, the event handlers and the saving and
	loading. Retail links these functions after a second copy of the
	static initializers the main unit's headers define, so they are a
	second translation unit; its name is invented.

===============================================================================
*/

#include "STMenu.h"
#include "STSubPane.h"
#include "STStylizedWindow.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "TGEvent.h"
#include "TGIcon.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGObject.h"
#include "TGParagraph.h"
#include "TGPointerMap.h"
#include "TGRect.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "STFillGauge.h"
#include "NiPoint2.h"

#include <stdio.h>

/*
================
STMenu::GotFocus
================
*/
// FUNCTION: STBC 0x005270c0
void STMenu::GotFocus( bool layout ) {
	TGWindow::GotFocus( false );
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::GotTrueFocusPath
================
*/
// FUNCTION: STBC 0x005270f0
void STMenu::GotTrueFocusPath( bool layout ) {
	TGWindow::GotTrueFocusPath( layout );
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::LostFocus
================
*/
// FUNCTION: STBC 0x00527120
void STMenu::LostFocus( bool layout ) {
	TGWindow::LostFocus( false );
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::LostTrueFocusPath

Forgets the first of two clicks.
================
*/
// FUNCTION: STBC 0x00527170
void STMenu::LostTrueFocusPath( bool layout ) {
	TGWindow::LostTrueFocusPath( false );
	ClearClickedOnce();
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::RemoveChild

Removes the child from the sub pane and resizes to the contents.
================
*/
// FUNCTION: STBC 0x005271b0
void STMenu::RemoveChild( TGUIObject *child, bool layout ) {
	TGWindow::RemoveChild( child, false );
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( layout );
	} else {
		ResizeToContents( layout );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::DeleteChild
================
*/
// FUNCTION: STBC 0x00527210
void STMenu::DeleteChild( TGUIObject *child, bool layout ) {
	TGWindow::DeleteChild( child, false );
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( layout );
	} else {
		ResizeToContents( layout );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::VirtualSlot112

Colours the name, the end caps, the title bar and the arrows from the
colour scheme's colours of the state. The marker shows the default
colour while the menu is in the true focus path, and gray while its
window is not but the window's focus is the menu or within it.
================
*/
// FUNCTION: STBC 0x00527270
void STMenu::VirtualSlot112( int state ) {
	TGParagraph *text = (TGParagraph *)TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	TGIcon *leftEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_LEFT_END_CAP );
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	TGIcon *rightEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
	TGIcon *marker = (TGIcon *)TGPane::GetNthChild( STMENU_MARKER );
	TGIcon *openedArrow = (TGIcon *)TGPane::GetNthChild( STMENU_OPENED_ARROW );
	TGIcon *closedArrow = (TGIcon *)TGPane::GetNthChild( STMENU_CLOSED_ARROW );

	text->SetColor( colorScheme->states[state].text );
	NiColorA *base = colorScheme->states[state].base;
	if ( IsInTrueFocusPath() ) {
		marker->SetColor( &g_kSTButtonMarkerDefault );
	} else {
		STStylizedWindow *window = GetContainingWindow();
		if ( window != NULL && !window->IsInTrueFocusPath() ) {
			TGUIObject *focus = window->GetFocusLeaf();
			if ( focus != NULL && ( focus->HasAncestor( this ) || focus == this ) ) {
				marker->SetColor( &g_kSTButtonMarkerGray );
			}
		}
	}
	leftEndCap->SetColor( base );
	openedArrow->SetColor( &g_kSTMenuArrowColor );
	closedArrow->SetColor( &g_kSTMenuArrowColor );
	if ( titleBar->IsTypeOf( TGICON_TYPE ) ) {
		( (TGIcon *)titleBar )->SetColor( base );
	} else if ( titleBar->IsTypeOf( STFILLGAUGE_TYPE ) ) {
		( (STFillGauge *)titleBar )->SetFillColor( *base );
	}
	rightEndCap->SetColor( base );
}

/*
================
STMenu::Unknown5273F0

Takes the colours of the menu's state: selected, or by the openable,
enabled, opened and highlighted flags.
================
*/
// FUNCTION: STBC 0x005273f0
void STMenu::Unknown5273F0( void ) {
	int state = colorStates[IsOpenable()][flags & TGUIO_ENABLED][IsOpened()][( flags >> 2 ) & 1];
	if ( flags & TGUIO_SELECTED ) {
		state = STMENU_STATE_SELECTED;
	}
	VirtualSlot112( state );
}

/*
================
STMenu::ResetColorScheme

Takes the colour scheme of the menu's depth, when the menu follows it.
================
*/
// FUNCTION: STBC 0x00527440
void STMenu::ResetColorScheme( void ) {
	if ( useUIHeight == true ) {
		int height = GetUIHeight();
		if ( height < 0 ) {
			height = 0;
		} else if ( height >= STMENU_NUM_UI_HEIGHTS ) {
			height = STMENU_NUM_UI_HEIGHTS - 1;
		}
		uiHeight = height;
		colorScheme = g_kSTMenuColors[height];
		Unknown5273F0();
	}
}

/*
================
STMenu::Unknown527480

Activates the menu: sends the activation event, toggles an auto-chosen
menu, takes the parent's focus, and opens or closes it. A menu that
wants two clicks only remembers the first click while it did not have
the focus.
================
*/
// FUNCTION: STBC 0x00527480
void STMenu::Unknown527480( void ) {
	TGPane *parentPane = GetParent();
	SendActivationEvent();
	if ( IsOpened() == true ) {
		if ( !IsCloseable() ) {
			return;
		}
	}
	if ( !IsOpened() && !IsOpenable() ) {
		return;
	}
	if ( IsAutoChoose() == true ) {
		if ( !IsChosen() ) {
			SetChosen( true, true );
		} else {
			SetChosen( false, true );
		}
	}
	bool hadFocus = HasFocus();
	parentPane->SetFocus( this, true );
	if ( ( flags & STMF_TWO_CLICKS ) && hadFocus != true && !IsClickedOnce() && !IsOpened() ) {
		SetClickedOnce();
		return;
	}
	if ( IsOpened() ) {
		Close( true );
	} else {
		Open( true );
	}
	parentPane->Layout();
	if ( IsClickedOnce() ) {
		ClearClickedOnce();
	}
}

/*
================
STMenu::MouseHandler

A left button event over the sub pane goes to the items. Elsewhere,
the press selects the menu and takes the mouse, and the release over
the title row activates it.
================
*/
// FUNCTION: STBC 0x00527570
// PARKED: complete; retail tests the button bits with byte tests and loads the end cap corner through the FPU; frame layout differs by 8 bytes
void STMenu::MouseHandler( TGMouseEvent *event ) {
	if ( !IsCompletelyVisible() || !( flags & TGUIO_ENABLED ) ) {
		if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
			SetNotSelected( true );
			g_kRootWindow.ReleaseMouse( this );
		}
		CallNextHandler( event );
		event->SetHandled();
		return;
	}

	if ( ( event->GetFlags() & MEF_BUTTON_MASK ) && ( event->GetFlags() & MEF_BUTTON_LEFT ) ) {
		g_kRootWindow.Unknown727EE0( this );
		TGPane *subPane = GetSubPane();
		NiPoint2 point;
		point.x = event->GetX();
		point.y = event->GetY();
		NiPoint2 position;
		subPane->GetPosition( &position );
		point -= position;
		if ( point.x < 0.0f || subPane->GetWidth() < point.x || point.y < 0.0f || subPane->GetHeight() < point.y
				|| g_kRootWindow.GetMouseGrabOwner() == this ) {
			if ( event->GetFlags() & MEF_BUTTON_DOWN ) {
				SetSelected( true );
				g_kRootWindow.GrabMouse( this );
			}
			if ( event->GetFlags() & MEF_BUTTON_UP ) {
				SetNotSelected( true );
				g_kRootWindow.ReleaseMouse( this );
				point += position;
				TGUIObject *leftEndCap = TGPane::GetNthChild( STMENU_LEFT_END_CAP );
				TGUIObject *rightEndCap = TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
				// The title row, from the left end cap's corner; the right
				// end cap's right and bottom edges are passed as the size.
				TGRect titleRow( leftEndCap->GetLeft(), leftEndCap->GetTop(), rightEndCap->GetRight(), rightEndCap->GetBottom() );
				if ( titleRow.IsWithin( point.x, point.y ) ) {
					Unknown527480();
				}
			}
		} else {
			CallNextHandler( event );
		}
	} else if ( g_kRootWindow.GetMouseGrabOwner() != this ) {
		CallNextHandler( event );
	}
	event->SetHandled();
}

/*
================
STMenu::KeyboardHandler

Enter presses a closed enabled menu: down selects it, up activates it.
Other keys run their binding for the menu. An open menu passes what is
left to the sub pane.
================
*/
// FUNCTION: STBC 0x005277a0
void STMenu::KeyboardHandler( TGKeyboardEvent *event ) {
	unsigned short unicode = event->GetUnicode();
	int keyState = event->GetKeyState();
	if ( unicode == '\r' && !IsOpened() && ( flags & TGUIO_ENABLED ) ) {
		if ( keyState == KS_KEYDOWN ) {
			SetSelected( true );
		} else if ( keyState == KS_KEYUP ) {
			SetNotSelected( true );
			Unknown527480();
		}
		event->SetHandled();
	} else if ( ( flags & TGUIO_ENABLED ) && g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), keyState, this ) ) {
		event->SetHandled();
	}
	if ( IsOpened() && ( flags & TGUIO_ENABLED ) && !event->EventHandled() ) {
		event->SetDestination( GetSubPane() );
		GetSubPane()->ProcessEvent( event );
	}
}

/*
================
STMenu::HandleSelectOption

Closes the menu the focus is on (or, from one level down, the menu
holding it) inside a sub pane; highlights or activates it when it has
the focus; an open menu passes the rest to the sub pane. A used event
is taken from its destination.
================
*/
// FUNCTION: STBC 0x00527860
void STMenu::HandleSelectOption( TGIntEvent *event ) {
	int height = FocusHeight();
	TGPane *parentPane = GetParent();
	if ( event->GetEventType() == ET_INPUT_CLOSE_MENU ) {
		if ( height == 0 ) {
			if ( parentPane->IsTypeOf( STSUBPANE_TYPE ) ) {
				Close( false );
				SetHighlighted( false );
				parentPane->Layout();
				event->SetDestination( NULL );
			}
		} else {
			event->SetDestination( GetSubPane() );
			GetSubPane()->ProcessEvent( event );
			if ( height == 1 && event->GetDestination() != NULL && parentPane->IsTypeOf( STSUBPANE_TYPE ) ) {
				Close( false );
				SetHighlighted( false );
				parentPane->Layout();
				event->SetDestination( NULL );
			}
		}
		return;
	}
	if ( event->GetEventType() == ET_INPUT_PRE_SELECT_OPTION && height == 0 ) {
		SetHighlighted( false );
		event->SetDestination( NULL );
	}
	if ( event->GetEventType() == ET_INPUT_SELECT_OPTION && height == 0 ) {
		Unknown527480();
		event->SetDestination( NULL );
	}
	if ( IsOpened() && event->GetDestination() != NULL ) {
		event->SetDestination( GetSubPane() );
		GetSubPane()->ProcessEvent( event );
	}
}

/*
================
STMenu::WriteColorSchemes

Writes the colour schemes' addresses, for the menus that point to them.
================
*/
// FUNCTION: STBC 0x005279d0
bool STMenu::WriteColorSchemes( TGStream *stream ) {
	for ( int i = 0; i < STMENU_NUM_UI_HEIGHTS; i++ ) {
		stream->VirtualSlot17( (long)g_kSTMenuColors[i] );
	}
	return true;
}

/*
================
STMenu::ReadColorSchemes

Files each colour scheme under the address it had when the game was
saved.
================
*/
// FUNCTION: STBC 0x00527a00
bool STMenu::ReadColorSchemes( TGStream *stream ) {
	for ( int i = 0; i < STMENU_NUM_UI_HEIGHTS; i++ ) {
		pointerMap.Add( stream->VirtualSlot16(), g_kSTMenuColors[i] );
	}
	return true;
}

/*
================
STMenu::WriteToStream

The activation event is written as its ID, the colour scheme as its
address.
================
*/
// FUNCTION: STBC 0x00527ae0
bool STMenu::WriteToStream( TGStream *stream ) {
	if ( !TGWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( activationEvent != NULL ? activationEvent->GetObjID() : 0 );
	stream->VirtualSlot17( (long)colorScheme );
	stream->WriteInt( uiHeight );
	stream->WriteBool( useUIHeight );
	stream->WriteBool( fullTitleBar );
	return true;
}

/*
================
STMenu::ReadFromStream

The activation event and the colour scheme are read as the ID and the
address PostFixUp turns back into pointers.
================
*/
// FUNCTION: STBC 0x00527b50
bool STMenu::ReadFromStream( TGStream *stream ) {
	if ( !TGWindow::ReadFromStream( stream ) ) {
		return false;
	}
	activationEvent = (TGEvent *)stream->ReadID();
	colorScheme = (STMenuColorScheme *)stream->VirtualSlot16();
	uiHeight = stream->ReadInt();
	useUIHeight = stream->ReadBool();
	fullTitleBar = stream->ReadBool();
	return true;
}

/*
================
STMenu::FixUpPointers

Fails until the activation event and the colour scheme are loaded.
================
*/
// FUNCTION: STBC 0x00527bb0
bool STMenu::FixUpPointers( void ) {
	char message[256];
	if ( !TGWindow::FixUpPointers() ) {
		return false;
	}
	// The activation event holds the ID ReadFromStream read.
	long eventID = (long)activationEvent;
	if ( eventID != 0 ) {
		long id = GetObjID();
		char *className = GetClassName();
		sprintf( message, "ActivationEvent %d, checking to fixup for %s (%d)\n", eventID, className, id );
		TGObject::Log( message );
		if ( TGObject::GetTGObjectPtr( eventID ) == NULL ) {
			return false;
		}
	}
	if ( colorScheme != NULL && pointerMap.Find( (long)colorScheme ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
STMenu::PostFixUp

Replaces the activation event's ID and the colour scheme's address with
the pointers.
================
*/
// FUNCTION: STBC 0x00527c80
void STMenu::PostFixUp( void ) {
	char message[256];
	TGWindow::PostFixUp();
	if ( activationEvent != NULL ) {
		sprintf( message, "ActivationEvent %d, attempting to fixup for %s\n", activationEvent, GetClassName() );
		TGObject::Log( message );
	}
	activationEvent = (TGEvent *)TGObject::GetSavedObject( (long)activationEvent );
	colorScheme = (STMenuColorScheme *)pointerMap.Find( (long)colorScheme );
}

/*
================
STMenu::UseFullTitleBar
================
*/
// FUNCTION: STBC 0x00527d30
void STMenu::UseFullTitleBar( bool layout ) {
	fullTitleBar = true;
	if ( layout ) {
		Layout();
	}
}

/*
================
STMenu::DisableFullTitleBar
================
*/
// FUNCTION: STBC 0x00527d50
void STMenu::DisableFullTitleBar( bool layout ) {
	fullTitleBar = false;
	if ( layout ) {
		Layout();
	}
}
