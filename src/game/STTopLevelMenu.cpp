/*
===============================================================================

	STTopLevelMenu.cpp

===============================================================================
*/

#include <iostream>
#include "STTopLevelMenu.h"
#include "STSubPane.h"
#include "STStylizedWindow.h"
#include "GraphicsModeInfo.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "ObjectClass.h"
#include "PyEmbed.h"
#include "TacticalControlWindow.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "TGIcon.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGRect.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "NiPoint2.h"

#include <stdio.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0053e1f0 _$E504
// FUNCTION: STBC 0x0053e210 _$E503

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0053db30 _$E57
// FUNCTION: STBC 0x0053db40 _$E56
// FUNCTION: STBC 0x0053db50 _$E61
// FUNCTION: STBC 0x0053db60 _$E60
// FUNCTION: STBC 0x0053db70 _$E72
// FUNCTION: STBC 0x0053db80 _$E71
// FUNCTION: STBC 0x0053db90 _$E76
// FUNCTION: STBC 0x0053dba0 _$E75
// FUNCTION: STBC 0x0053dbb0 _$E80
// FUNCTION: STBC 0x0053dbc0 _$E79
// FUNCTION: STBC 0x0053dbd0 _$E84
// FUNCTION: STBC 0x0053dbe0 _$E83
// FUNCTION: STBC 0x0053dbf0 _$E208
// FUNCTION: STBC 0x0053dc00 _$E207
// FUNCTION: STBC 0x0053dc10 _$E150
// FUNCTION: STBC 0x0053dc20 _$E149
// FUNCTION: STBC 0x0053dc30 _$E163
// FUNCTION: STBC 0x0053dc40 _$E162
// FUNCTION: STBC 0x0053dc50 _$E213
// FUNCTION: STBC 0x0053dc60 _$E212
// FUNCTION: STBC 0x0053dc70 _$E218
// FUNCTION: STBC 0x0053dc80 _$E217
// FUNCTION: STBC 0x0053dc90 _$E222
// FUNCTION: STBC 0x0053dca0 _$E221
// FUNCTION: STBC 0x0053dcb0 _$E226
// FUNCTION: STBC 0x0053dcc0 _$E225
// FUNCTION: STBC 0x0053dcd0 _$E233
// FUNCTION: STBC 0x0053dce0 _$E232
// FUNCTION: STBC 0x0053dcf0 _$E323
// FUNCTION: STBC 0x0053dd00 _$E322
// FUNCTION: STBC 0x0053dd10 _$E327
// FUNCTION: STBC 0x0053dd20 _$E326
// FUNCTION: STBC 0x0053dd30 _$E108
// FUNCTION: STBC 0x0053dd40 _$E107
// FUNCTION: STBC 0x0053dd50 _$E309
// FUNCTION: STBC 0x0053dd60 _$E308
// FUNCTION: STBC 0x0053dd70 _$E316
// FUNCTION: STBC 0x0053dd80 _$E315
// FUNCTION: STBC 0x0053dd90 _$E339
// FUNCTION: STBC 0x0053dda0 _$E338
// FUNCTION: STBC 0x0053ddb0 _$E343
// FUNCTION: STBC 0x0053ddc0 _$E342
// FUNCTION: STBC 0x0053ddd0 _$E383
// FUNCTION: STBC 0x0053dde0 _$E382
// FUNCTION: STBC 0x0053ddf0 _$E389
// FUNCTION: STBC 0x0053de00 _$E388
// FUNCTION: STBC 0x0053de10 _$E331
// FUNCTION: STBC 0x0053de20 _$E330
// FUNCTION: STBC 0x0053de30 _$E335
// FUNCTION: STBC 0x0053de40 _$E334
// FUNCTION: STBC 0x0053de50 _$E347
// FUNCTION: STBC 0x0053de60 _$E346
// FUNCTION: STBC 0x0053de70 _$E351
// FUNCTION: STBC 0x0053de80 _$E350
// FUNCTION: STBC 0x0053de90 _$E355
// FUNCTION: STBC 0x0053dea0 _$E354
// FUNCTION: STBC 0x0053deb0 _$E359
// FUNCTION: STBC 0x0053dec0 _$E358
// FUNCTION: STBC 0x0053ded0 _$E363
// FUNCTION: STBC 0x0053dee0 _$E362
// FUNCTION: STBC 0x0053def0 _$E367
// FUNCTION: STBC 0x0053df00 _$E366
// FUNCTION: STBC 0x0053df10 _$E371
// FUNCTION: STBC 0x0053df20 _$E370
// FUNCTION: STBC 0x0053df30 _$E375
// FUNCTION: STBC 0x0053df40 _$E374
// FUNCTION: STBC 0x0053df50 _$E113
// FUNCTION: STBC 0x0053df60 _$E112
// FUNCTION: STBC 0x0053df70 _$E379
// FUNCTION: STBC 0x0053df80 _$E378
// FUNCTION: STBC 0x0053df90 _$E118
// FUNCTION: STBC 0x0053dfa0 _$E117
// FUNCTION: STBC 0x0053dfb0 _$E393
// FUNCTION: STBC 0x0053dfc0 _$E392
// FUNCTION: STBC 0x0053dfd0 _$E397
// FUNCTION: STBC 0x0053dfe0 _$E396
// FUNCTION: STBC 0x0053dff0 _$E401
// FUNCTION: STBC 0x0053e000 _$E400
// FUNCTION: STBC 0x0053e010 _$E405
// FUNCTION: STBC 0x0053e020 _$E404
// FUNCTION: STBC 0x0053e030 _$E127
// FUNCTION: STBC 0x0053e040 _$E126
// FUNCTION: STBC 0x0053e050 _$E409
// FUNCTION: STBC 0x0053e060 _$E408
// FUNCTION: STBC 0x0053e070 _$E413
// FUNCTION: STBC 0x0053e080 _$E412
// FUNCTION: STBC 0x0053e090 _$E417
// FUNCTION: STBC 0x0053e0a0 _$E416
// FUNCTION: STBC 0x0053e0b0 _$E133
// FUNCTION: STBC 0x0053e0c0 _$E132
// FUNCTION: STBC 0x0053e0d0 _$E138
// FUNCTION: STBC 0x0053e0e0 _$E137
// FUNCTION: STBC 0x0053e0f0 _$E168
// FUNCTION: STBC 0x0053e100 _$E167
// FUNCTION: STBC 0x0053e110 _$E421
// FUNCTION: STBC 0x0053e120 _$E420
// FUNCTION: STBC 0x0053e130 _$E173
// FUNCTION: STBC 0x0053e140 _$E172
// FUNCTION: STBC 0x0053e150 _$E425
// FUNCTION: STBC 0x0053e160 _$E424
// FUNCTION: STBC 0x0053e170 _$E429
// FUNCTION: STBC 0x0053e180 _$E428
// FUNCTION: STBC 0x0053e190 _$E178
// FUNCTION: STBC 0x0053e1a0 _$E177
// FUNCTION: STBC 0x0053e1b0 _$E433
// FUNCTION: STBC 0x0053e1c0 _$E432
// FUNCTION: STBC 0x0053e1d0 _$E143
// FUNCTION: STBC 0x0053e1e0 _$E142
// FUNCTION: STBC 0x0053e220 _$E437
// FUNCTION: STBC 0x0053e230 _$E436
// FUNCTION: STBC 0x0053e240 _$E441
// FUNCTION: STBC 0x0053e250 _$E440
// FUNCTION: STBC 0x0053e260 _$E183
// FUNCTION: STBC 0x0053e270 _$E182
// FUNCTION: STBC 0x0053e280 _$E188
// FUNCTION: STBC 0x0053e290 _$E187
// FUNCTION: STBC 0x0053e2a0 _$E445
// FUNCTION: STBC 0x0053e2b0 _$E444
// FUNCTION: STBC 0x0053e2c0 _$E449
// FUNCTION: STBC 0x0053e2d0 _$E448
// FUNCTION: STBC 0x0053e2e0 _$E453
// FUNCTION: STBC 0x0053e2f0 _$E452
// FUNCTION: STBC 0x0053e300 _$E457
// FUNCTION: STBC 0x0053e310 _$E456
// FUNCTION: STBC 0x0053e320 _$E461
// FUNCTION: STBC 0x0053e330 _$E460
// FUNCTION: STBC 0x0053e340 _$E465
// FUNCTION: STBC 0x0053e350 _$E464
// FUNCTION: STBC 0x0053e360 _$E469
// FUNCTION: STBC 0x0053e370 _$E468
// FUNCTION: STBC 0x0053e380 _$E473
// FUNCTION: STBC 0x0053e390 _$E472
// FUNCTION: STBC 0x0053e3a0 _$E193
// FUNCTION: STBC 0x0053e3b0 _$E192
// FUNCTION: STBC 0x0053e3c0 _$E477
// FUNCTION: STBC 0x0053e3d0 _$E476
// FUNCTION: STBC 0x0053e3e0 _$E198
// FUNCTION: STBC 0x0053e3f0 _$E197
// FUNCTION: STBC 0x0053e400 _$E203
// FUNCTION: STBC 0x0053e410 _$E202
// FUNCTION: STBC 0x0053e420 _$E481
// FUNCTION: STBC 0x0053e430 _$E480
// FUNCTION: STBC 0x0053e440 _$E486
// FUNCTION: STBC 0x0053e450 _$E485
// FUNCTION: STBC 0x0053e460 _$E490
// FUNCTION: STBC 0x0053e470 _$E489

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0053cdc0 _$E32
// FUNCTION: STBC 0x0053cdd0 _$E29
// FUNCTION: STBC 0x0053cde0 _$E31
// FUNCTION: STBC 0x0053cdf0 _$E30
// FUNCTION: STBC 0x0053ce00 _$E38
// FUNCTION: STBC 0x0053ce10 _$E35
// FUNCTION: STBC 0x0053ce20 _$E37
// FUNCTION: STBC 0x0053ce30 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0053cc40 _$E496
// FUNCTION: STBC 0x0053cc50 _$E492
// FUNCTION: STBC 0x0053cc60 _$E495
// FUNCTION: STBC 0x0053cc70 _$E493

// DATA: STBC 0x0098a318
TGEventHandlerTable STTopLevelMenu::classHandlers;

// DATA: STBC 0x0098a34c
STTopLevelMenu *STTopLevelMenu::openMenu;

/*
================
STTopLevelMenu::Cast

The object as a top level menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0053cc90
STTopLevelMenu *STTopLevelMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STTOPLEVELMENU_TYPE ) ) {
		return (STTopLevelMenu *)object;
	}
	return NULL;
}

/*
================
STTopLevelMenu::STTopLevelMenu
================
*/
// FUNCTION: STBC 0x0053ccc0 ??0STTopLevelMenu@@QAE@PAVTGString@@IPAVTGEvent@@PAVSTSubPane@@@Z
STTopLevelMenu::STTopLevelMenu( TGString *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane )
	: STMenu( name, menuFlags, event, subPane ) {
	InitTopLevel( true );
}

/*
================
STTopLevelMenu::STTopLevelMenu
================
*/
// FUNCTION: STBC 0x0053cd30 ??0STTopLevelMenu@@QAE@PADIPAVTGEvent@@PAVSTSubPane@@@Z
STTopLevelMenu::STTopLevelMenu( char *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane )
	: STMenu( name, menuFlags, event, subPane ) {
	// BUG: never used; retail constructs and destroys it all the same
	TGString wideName( name );
	InitTopLevel( true );
}

/*
================
STTopLevelMenu::InitTopLevel

Takes the parts a top level menu does not show (the marker, the end
caps, the title bar and the arrows) out of sight, enabled and open.
================
*/
// FUNCTION: STBC 0x0053ce40
void STTopLevelMenu::InitTopLevel( bool layout ) {
	SetNotCloseable();
	// BUG: the resolution is not used
	GraphicsModeInfo::GetCurrentMode()->GetCurrentResolution();
	owner = NULL;
	TGIcon *marker = TGIcon::Cast( TGPane::GetNthChild( STMENU_MARKER ) );
	TGIcon *leftEndCap = TGIcon::Cast( TGPane::GetNthChild( STMENU_LEFT_END_CAP ) );
	TGIcon *rightEndCap = TGIcon::Cast( TGPane::GetNthChild( STMENU_RIGHT_END_CAP ) );
	TGIcon *titleBar = TGIcon::Cast( TGPane::GetNthChild( STMENU_TITLE_BAR ) );
	TGPane *titlePane = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) );
	TGIcon *openedArrow = TGIcon::Cast( TGPane::GetNthChild( STMENU_OPENED_ARROW ) );
	TGIcon *closedArrow = TGIcon::Cast( TGPane::GetNthChild( STMENU_CLOSED_ARROW ) );
	resizeSubpane = true;
	marker->SetNotVisible( false );
	leftEndCap->SetNotVisible( false );
	rightEndCap->SetNotVisible( false );
	titleBar->SetNotVisible( false );
	titlePane->SetNotVisible( false );
	openedArrow->SetNotVisible( false );
	closedArrow->SetNotVisible( false );
	SetEnabled( false );
	Open( false );
}

/*
================
STTopLevelMenu::~STTopLevelMenu

The open menu closes first.
================
*/
// FUNCTION: STBC 0x0053cf80
STTopLevelMenu::~STTopLevelMenu( void ) {
	if ( openMenu == this ) {
		SetNotVisible( true );
		openMenu = NULL;
	}
}

/*
================
STTopLevelMenu::GotFocus
================
*/
// FUNCTION: STBC 0x0053cff0
void STTopLevelMenu::GotFocus( bool layout ) {
	STMenu::GotFocus( layout );
}

/*
================
STTopLevelMenu::LostFocus
================
*/
// FUNCTION: STBC 0x0053d000
void STTopLevelMenu::LostFocus( bool layout ) {
	STMenu::LostFocus( layout );
}

/*
================
STTopLevelMenu::SetVisible

Closes the menu open before, then shows this one, its menu pane and the
tactical control window, with the focus on the menu. A disabled menu
stays hidden. Either way the bridge drops its tool tips and the tactical
control window refreshes.
================
*/
// FUNCTION: STBC 0x0053d010
void STTopLevelMenu::SetVisible( bool layout ) {
	if ( openMenu != NULL && openMenu != this ) {
		openMenu->SetNotVisible( true );
	}
	if ( !IsEnabled() ) {
		PyEmbed::CallFunction( "BridgeHandlers", "DropCharacterToolTips", NULL, NULL, NULL, NULL );
		PyEmbed::CallFunction( "Tactical.Interface.TacticalControlWindow", "Refresh", NULL, NULL, NULL );
		return;
	}
	if ( !g_kUtopiaModule.IsLoading() ) {
		openMenu = this;
	}
	TGPane::SetVisible( layout );
	TGPane *conceptualParent = GetConceptualParent();
	if ( conceptualParent != NULL ) {
		conceptualParent->SetVisible( layout );
	}
	if ( parent != NULL ) {
		parent->SetVisible( layout );
	}
	InterfaceModule::ForceFocusOnObject( this );

	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	TGString name( "" );
	GetName( &name );
	TGPane *menuPane = window->GetMenuParentPane( &name );
	menuPane->SetVisible( true );
	STSubPane *subPane = (STSubPane *)GetSubPane();
	subPane->ResizeToContents( true );
	window->SetFocus( menuPane, false );
	window->SetVisible( true );
	PyEmbed::CallFunction( "BridgeHandlers", "DropCharacterToolTips", NULL, NULL, NULL, NULL );
	PyEmbed::CallFunction( "Tactical.Interface.TacticalControlWindow", "Refresh", NULL, NULL, NULL );
}

/*
================
STTopLevelMenu::SetNotVisible
================
*/
// FUNCTION: STBC 0x0053d1b0
void STTopLevelMenu::SetNotVisible( bool layout ) {
	CloseMenu( layout );
}

/*
================
STTopLevelMenu::CloseMenu
================
*/
// FUNCTION: STBC 0x0053d1c0
void STTopLevelMenu::CloseMenu( bool layout ) {
	STMenu *focus = STMenu::Cast( GetFocus() );
	if ( focus != NULL ) {
		focus->Close( true );
	}
	TGPane *conceptualParent = GetConceptualParent();
	TGPane::SetNotVisible( layout );
	if ( conceptualParent != NULL ) {
		conceptualParent->SetNotVisible( layout );
	}

	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	if ( window == NULL ) {
		return;
	}
	TGString name( "" );
	GetName( &name );
	TGPane *menuPane = window->GetMenuParentPane( &name );
	if ( menuPane != NULL ) {
		menuPane->SetNotVisible( true );
	}
	TGPane *windowParent = window->GetParent();
	if ( windowParent != NULL ) {
		windowParent->SetFocus( window, false );
	}
	if ( owner != NULL ) {
		owner = NULL;
	}
	if ( windowParent != NULL ) {
		PyEmbed::CallFunction( "Tactical.Interface.TacticalControlWindow", "Refresh", NULL, NULL, NULL );
	}
	if ( TopWindow::GetTopWindow()->IsTacticalVisible() ) {
		TGUIObject *first = window->GetFirstChild();
		if ( first != NULL ) {
			first->SetVisible( false );
			window->SetFocus( first, layout );
		}
	}
	if ( openMenu == this ) {
		openMenu = NULL;
	}
}

/*
================
STTopLevelMenu::VirtualSlot113

Sizes the sub pane to its contents while the menu is open; to nothing
while it is closed.
================
*/
// FUNCTION: STBC 0x0053d330
void STTopLevelMenu::VirtualSlot113( void ) {
	TGPane *subPane = GetSubPane();
	// BUG: not used
	TGUIObject *text = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	if ( !IsOpened() ) {
		subPane->Resize( 0.0f, 0.0f, false );
		return;
	}
	// BUG: not used
	TGRect clientArea = GetClientArea();
	NiPoint2 size;
	GetSubPaneDesiredSize( &size );
	subPane->Resize( size.x, size.y, false );
}

/*
================
STTopLevelMenu::VirtualSlot114

The title bar at the corner, the title pane beside it and the sub pane
in the client area.
================
*/
// FUNCTION: STBC 0x0053d400
void STTopLevelMenu::VirtualSlot114( void ) {
	TGPane *subPane = GetSubPane();
	// BUG: not used
	TGUIObject *text = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	TGPane *titlePane = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) );
	titleBar->SetPosition( 0.0f, 0.0f, false );
	float left = titleBar->GetLeft();
	titlePane->SetPosition( left + DEFAULT_ST_INDENT_HORIZ, 0.0f, false );
	TGRect clientArea = GetClientArea();
	// Retail copies the top through fld/fstp into its argument slot and the
	// left with integer moves: the copy level of this named local (the same in
	// STMenu::VirtualSlot114 and STTopLevelMenu::VirtualSlot114).
	float top = clientArea.top;
	subPane->SetPosition( clientArea.left, top, false );
}

/*
================
STTopLevelMenu::ResizeToContents
================
*/
// FUNCTION: STBC 0x0053d4e0
void STTopLevelMenu::ResizeToContents( bool layout ) {
	STMenu::ResizeToContents( false );
	Layout();
}

/*
================
STTopLevelMenu::GetClientArea

The whole width at the top: the stylized window's widest interior, the
parent's width, or the menu's own without a parent; the sub pane's
desired height.
================
*/
// FUNCTION: STBC 0x0053d500
TGRect STTopLevelMenu::GetClientArea( void ) {
	// BUG: not used
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	TGRect area;
	TGPane *parentPane = parent;
	STStylizedWindow *window = STStylizedWindow::Cast( parentPane );
	NiPoint2 size;
	GetSubPaneDesiredSize( &size );
	float width;
	if ( parentPane != NULL ) {
		if ( window != NULL ) {
			width = window->GetMaximumInteriorWidth();
		} else {
			width = parentPane->GetWidth();
		}
	} else {
		width = GetWidth();
	}
	area.Set( 0.0f, 0.0f, width, size.y );
	return area;
}

/*
================
STTopLevelMenu::GetDesiredHeight
================
*/
// FUNCTION: STBC 0x0053d5f0
float STTopLevelMenu::GetDesiredHeight( void ) {
	NiPoint2 size;
	GetDesiredSize( &size );
	return size.y;
}

/*
================
STTopLevelMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0053d610
void STTopLevelMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTopLevelMenu::MouseHandler, "STTopLevelMenu::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTopLevelMenu::KeyboardHandler, "STTopLevelMenu::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTopLevelMenu::HandleSelectOption, "STTopLevelMenu::HandleSelectOption" );
}

/*
================
STTopLevelMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0053d650
void STTopLevelMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "STTopLevelMenu::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STTopLevelMenu::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "STTopLevelMenu::HandleSelectOption" );
}

/*
================
STTopLevelMenu::MouseHandler

A left button event over the sub pane goes to the items. Elsewhere,
the press takes the mouse, and the release over the title bar
activates the menu.
================
*/
// FUNCTION: STBC 0x0053d690
// PARKED: x87 operand order: retail adds the sub pane's x to the point's (fld point, fadd position) where this build loads the position first; the y line and every other instruction match (+=, both operand orders tried)
void STTopLevelMenu::MouseHandler( TGMouseEvent *event ) {
	if ( !IsVisible() || !IsEnabled() ) {
		CallNextHandler( event );
		event->SetHandled();
		return;
	}

	if ( event->IsButtonEvent() && ( event->GetFlags() & MEF_BUTTON_LEFT ) ) {
		g_kRootWindow.Unknown727EE0( this );
		NiPoint2 position;
		NiPoint2 point;
		point.x = event->x;
		point.y = event->y;
		TGPane *subPane = GetSubPane();
		subPane->GetPosition( &position );
		point -= position;
		if ( !subPane->IsWithin( &point ) || g_kRootWindow.GetMouseGrabOwner() == this ) {
			if ( event->GetFlags() & MEF_BUTTON_DOWN ) {
				g_kRootWindow.GrabMouse( this );
			}
			if ( event->GetFlags() & MEF_BUTTON_UP ) {
				g_kRootWindow.ReleaseMouse( this );
				point += position;
				TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
				titleBar->GetPosition( &position );
				point -= position;
				if ( titleBar->IsWithin( &point ) ) {
					ActivateFromMouse( event );
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
STTopLevelMenu::ActivateFromMouse
================
*/
// FUNCTION: STBC 0x0053d850
void STTopLevelMenu::ActivateFromMouse( TGMouseEvent *event ) {
	SendActivationEvent();
	if ( parent != NULL ) {
		parent->SetFocus( this, true );
	}
}

/*
================
STTopLevelMenu::KeyboardHandler

Runs the key's binding for the menu; an open, enabled menu passes what
is left to the sub pane.
================
*/
// FUNCTION: STBC 0x0053d880
void STTopLevelMenu::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), this ) ) {
		event->SetHandled();
	}
	if ( IsOpened() && IsEnabled() && !event->EventHandled() ) {
		event->SetDestination( GetSubPane() );
		GetSubPane()->ProcessEvent( event );
	}
}

/*
================
STTopLevelMenu::HandleSelectOption

Passes the selection on to the sub pane.
================
*/
// FUNCTION: STBC 0x0053d8f0
void STTopLevelMenu::HandleSelectOption( TGIntEvent *event ) {
	// BUG: the height is not used
	FocusHeight();
	if ( event->GetEventType() == ET_INPUT_SELECT_OPTION ) {
		event->SetDestination( GetSubPane() );
		GetSubPane()->ProcessEvent( event );
	}
}

/*
================
STTopLevelMenu::SetResizeSubpane
================
*/
// FUNCTION: STBC 0x0053d940
void STTopLevelMenu::SetResizeSubpane( bool resize, bool layout ) {
	resizeSubpane = resize;
	if ( layout ) {
		Layout();
	}
}

/*
================
STTopLevelMenu::WriteOpenMenu

The open menu is saved as its ID.
================
*/
// FUNCTION: STBC 0x0053d960
bool STTopLevelMenu::WriteOpenMenu( TGStream *stream ) {
	if ( !STMenu::WriteColorSchemes( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( openMenu ) );
	return true;
}

/*
================
STTopLevelMenu::ReadOpenMenu

Keeps the ID in the pointer until PostFixUpOpenMenu.
================
*/
// FUNCTION: STBC 0x0053d9a0
bool STTopLevelMenu::ReadOpenMenu( TGStream *stream ) {
	if ( !STMenu::ReadColorSchemes( stream ) ) {
		return false;
	}
	openMenu = (STTopLevelMenu *)stream->ReadID();
	return true;
}

/*
================
STTopLevelMenu::FixUpOpenMenu
================
*/
// FUNCTION: STBC 0x0053d9d0
bool STTopLevelMenu::FixUpOpenMenu( void ) {
	if ( openMenu != NULL && GetTGObjectPtr( (long)openMenu ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
STTopLevelMenu::PostFixUpOpenMenu
================
*/
// FUNCTION: STBC 0x0053d9f0
void STTopLevelMenu::PostFixUpOpenMenu( void ) {
	if ( openMenu == NULL ) {
		openMenu = NULL;
	} else {
		openMenu = (STTopLevelMenu *)GetTGObjectPtr( (long)openMenu );
	}
}

/*
================
STTopLevelMenu::WriteToStream

The owner is saved as its ID.
================
*/
// FUNCTION: STBC 0x0053da10
bool STTopLevelMenu::WriteToStream( TGStream *stream ) {
	char message[256];
	if ( !STMenu::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( resizeSubpane );
	stream->WriteID( GetSavedID( owner ) );
	if ( owner != NULL ) {
		sprintf( message, "Saving menu with owner %s (ID %d)\n", owner->GetName(), owner->GetObjID() );
		TGObject::Log( message );
	}
	return true;
}

/*
================
STTopLevelMenu::ReadFromStream

Keeps the owner's ID in the pointer until PostFixUp.
================
*/
// FUNCTION: STBC 0x0053da90
bool STTopLevelMenu::ReadFromStream( TGStream *stream ) {
	if ( !STMenu::ReadFromStream( stream ) ) {
		return false;
	}
	resizeSubpane = stream->ReadBool();
	owner = (ObjectClass *)stream->ReadID();
	return true;
}

/*
================
STTopLevelMenu::FixUpPointers

Fails until the owner the stream named is loaded.
================
*/
// FUNCTION: STBC 0x0053dad0
bool STTopLevelMenu::FixUpPointers( void ) {
	if ( !STMenu::FixUpPointers() ) {
		return false;
	}
	if ( owner != NULL && GetTGObjectPtr( (long)owner ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
STTopLevelMenu::PostFixUp

Replaces the ID with the owner.
================
*/
// FUNCTION: STBC 0x0053db00
void STTopLevelMenu::PostFixUp( void ) {
	STMenu::PostFixUp();
	if ( owner == NULL ) {
		owner = NULL;
	} else {
		owner = (ObjectClass *)GetTGObjectPtr( (long)owner );
	}
}
