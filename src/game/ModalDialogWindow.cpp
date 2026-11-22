/*
===============================================================================

	ModalDialogWindow.cpp

===============================================================================
*/

#include "ModalDialogWindow.h"
#include "Game.h"
#include "GraphicsMenu.h"
#include "InterfaceModule.h"
#include "PyEmbed.h"
#include "STButton.h"
#include "STColors.h"
#include "STStylizedWindow.h"
#include "TGBoolEvent.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGMouseEvent.h"
#include "TGParagraph.h"
#include "TGRootPane.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGStream.h"
#include "TGString.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x00502240 _$E501
// FUNCTION: STBC 0x00502220 _$E502

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00503390 _$E479
// FUNCTION: STBC 0x005033b0 _$E480
// FUNCTION: STBC 0x005033a0 _$E481
// FUNCTION: STBC 0x00503380 _$E482
// FUNCTION: STBC 0x005035d0 _$E485
// FUNCTION: STBC 0x005035f0 _$E486
// FUNCTION: STBC 0x005035e0 _$E487
// FUNCTION: STBC 0x005035c0 _$E488

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00502350 _$E490
// FUNCTION: STBC 0x00502370 _$E491
// FUNCTION: STBC 0x00502360 _$E493
// FUNCTION: STBC 0x00502340 _$E494
// DATA: STBC 0x00987168
TGEventHandlerTable ModalDialogWindow::classHandlers;

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00503800 _$E33
// FUNCTION: STBC 0x00503810 _$E32
// FUNCTION: STBC 0x00503820 _$E37
// FUNCTION: STBC 0x00503830 _$E36
// FUNCTION: STBC 0x00503840 _$E48
// FUNCTION: STBC 0x00503850 _$E47
// FUNCTION: STBC 0x00503860 _$E52
// FUNCTION: STBC 0x00503870 _$E51
// FUNCTION: STBC 0x00503880 _$E56
// FUNCTION: STBC 0x00503890 _$E55
// FUNCTION: STBC 0x005038a0 _$E60
// FUNCTION: STBC 0x005038b0 _$E59
// FUNCTION: STBC 0x005038c0 _$E184
// FUNCTION: STBC 0x005038d0 _$E183
// FUNCTION: STBC 0x005038e0 _$E126
// FUNCTION: STBC 0x005038f0 _$E125
// FUNCTION: STBC 0x00503900 _$E139
// FUNCTION: STBC 0x00503910 _$E138
// FUNCTION: STBC 0x00503920 _$E189
// FUNCTION: STBC 0x00503930 _$E188
// FUNCTION: STBC 0x00503940 _$E194
// FUNCTION: STBC 0x00503950 _$E193
// FUNCTION: STBC 0x00503960 _$E198
// FUNCTION: STBC 0x00503970 _$E197
// FUNCTION: STBC 0x00503980 _$E202
// FUNCTION: STBC 0x00503990 _$E201
// FUNCTION: STBC 0x005039a0 _$E209
// FUNCTION: STBC 0x005039b0 _$E208
// FUNCTION: STBC 0x005039c0 _$E314
// FUNCTION: STBC 0x005039d0 _$E313
// FUNCTION: STBC 0x005039e0 _$E318
// FUNCTION: STBC 0x005039f0 _$E317
// FUNCTION: STBC 0x00503a00 _$E84
// FUNCTION: STBC 0x00503a10 _$E83
// FUNCTION: STBC 0x00503a20 _$E300
// FUNCTION: STBC 0x00503a30 _$E299
// FUNCTION: STBC 0x00503a40 _$E307
// FUNCTION: STBC 0x00503a50 _$E306
// FUNCTION: STBC 0x00503a60 _$E330
// FUNCTION: STBC 0x00503a70 _$E329
// FUNCTION: STBC 0x00503a80 _$E334
// FUNCTION: STBC 0x00503a90 _$E333
// FUNCTION: STBC 0x00503aa0 _$E374
// FUNCTION: STBC 0x00503ab0 _$E373
// FUNCTION: STBC 0x00503ac0 _$E380
// FUNCTION: STBC 0x00503ad0 _$E379
// FUNCTION: STBC 0x00503ae0 _$E322
// FUNCTION: STBC 0x00503af0 _$E321
// FUNCTION: STBC 0x00503b00 _$E326
// FUNCTION: STBC 0x00503b10 _$E325
// FUNCTION: STBC 0x00503b20 _$E338
// FUNCTION: STBC 0x00503b30 _$E337
// FUNCTION: STBC 0x00503b40 _$E342
// FUNCTION: STBC 0x00503b50 _$E341
// FUNCTION: STBC 0x00503b60 _$E346
// FUNCTION: STBC 0x00503b70 _$E345
// FUNCTION: STBC 0x00503b80 _$E350
// FUNCTION: STBC 0x00503b90 _$E349
// FUNCTION: STBC 0x00503ba0 _$E354
// FUNCTION: STBC 0x00503bb0 _$E353
// FUNCTION: STBC 0x00503bc0 _$E358
// FUNCTION: STBC 0x00503bd0 _$E357
// FUNCTION: STBC 0x00503be0 _$E362
// FUNCTION: STBC 0x00503bf0 _$E361
// FUNCTION: STBC 0x00503c00 _$E366
// FUNCTION: STBC 0x00503c10 _$E365
// FUNCTION: STBC 0x00503c20 _$E89
// FUNCTION: STBC 0x00503c30 _$E88
// FUNCTION: STBC 0x00503c40 _$E370
// FUNCTION: STBC 0x00503c50 _$E369
// FUNCTION: STBC 0x00503c60 _$E94
// FUNCTION: STBC 0x00503c70 _$E93
// FUNCTION: STBC 0x00503c80 _$E384
// FUNCTION: STBC 0x00503c90 _$E383
// FUNCTION: STBC 0x00503ca0 _$E388
// FUNCTION: STBC 0x00503cb0 _$E387
// FUNCTION: STBC 0x00503cc0 _$E392
// FUNCTION: STBC 0x00503cd0 _$E391
// FUNCTION: STBC 0x00503ce0 _$E396
// FUNCTION: STBC 0x00503cf0 _$E395
// FUNCTION: STBC 0x00503d00 _$E103
// FUNCTION: STBC 0x00503d10 _$E102
// FUNCTION: STBC 0x00503d20 _$E400
// FUNCTION: STBC 0x00503d30 _$E399
// FUNCTION: STBC 0x00503d40 _$E404
// FUNCTION: STBC 0x00503d50 _$E403
// FUNCTION: STBC 0x00503d60 _$E408
// FUNCTION: STBC 0x00503d70 _$E407
// FUNCTION: STBC 0x00503d80 _$E109
// FUNCTION: STBC 0x00503d90 _$E108
// FUNCTION: STBC 0x00503da0 _$E114
// FUNCTION: STBC 0x00503db0 _$E113
// FUNCTION: STBC 0x00503dc0 _$E144
// FUNCTION: STBC 0x00503dd0 _$E143
// FUNCTION: STBC 0x00503de0 _$E412
// FUNCTION: STBC 0x00503df0 _$E411
// FUNCTION: STBC 0x00503e00 _$E149
// FUNCTION: STBC 0x00503e10 _$E148
// FUNCTION: STBC 0x00503e50 _$E416
// FUNCTION: STBC 0x00503e60 _$E415
// FUNCTION: STBC 0x00503e70 _$E420
// FUNCTION: STBC 0x00503e80 _$E419
// FUNCTION: STBC 0x00503e90 _$E154
// FUNCTION: STBC 0x00503ea0 _$E153
// FUNCTION: STBC 0x00503eb0 _$E424
// FUNCTION: STBC 0x00503ec0 _$E423
// FUNCTION: STBC 0x00503ed0 _$E119
// FUNCTION: STBC 0x00503ee0 _$E118
// FUNCTION: STBC 0x00503ef0 _$E428
// FUNCTION: STBC 0x00503f00 _$E427
// FUNCTION: STBC 0x00503f10 _$E432
// FUNCTION: STBC 0x00503f20 _$E431
// FUNCTION: STBC 0x00503f30 _$E159
// FUNCTION: STBC 0x00503f40 _$E158
// FUNCTION: STBC 0x00503f50 _$E164
// FUNCTION: STBC 0x00503f60 _$E163
// FUNCTION: STBC 0x00503f70 _$E436
// FUNCTION: STBC 0x00503f80 _$E435
// FUNCTION: STBC 0x00503f90 _$E440
// FUNCTION: STBC 0x00503fa0 _$E439
// FUNCTION: STBC 0x00503fb0 _$E444
// FUNCTION: STBC 0x00503fc0 _$E443
// FUNCTION: STBC 0x00503fd0 _$E448
// FUNCTION: STBC 0x00503fe0 _$E447
// FUNCTION: STBC 0x00503ff0 _$E452
// FUNCTION: STBC 0x00504000 _$E451
// FUNCTION: STBC 0x00504010 _$E456
// FUNCTION: STBC 0x00504020 _$E455
// FUNCTION: STBC 0x00504030 _$E460
// FUNCTION: STBC 0x00504040 _$E459
// FUNCTION: STBC 0x00504050 _$E464
// FUNCTION: STBC 0x00504060 _$E463
// FUNCTION: STBC 0x00504070 _$E169
// FUNCTION: STBC 0x00504080 _$E168
// FUNCTION: STBC 0x00504090 _$E468
// FUNCTION: STBC 0x005040a0 _$E467
// FUNCTION: STBC 0x005040b0 _$E174
// FUNCTION: STBC 0x005040c0 _$E173
// FUNCTION: STBC 0x005040d0 _$E179
// FUNCTION: STBC 0x005040e0 _$E178
// FUNCTION: STBC 0x005040f0 _$E472
// FUNCTION: STBC 0x00504100 _$E471
// FUNCTION: STBC 0x00504110 _$E477
// FUNCTION: STBC 0x00504120 _$E476
// FUNCTION: STBC 0x00504130 _$E279
// FUNCTION: STBC 0x00504140 _$E278

/*
================
ModalDialogWindow::Cast

The object as a ModalDialogWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00502390
ModalDialogWindow *ModalDialogWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( MODALDIALOGWINDOW_TYPE ) ) {
		return (ModalDialogWindow *)object;
	}
	return NULL;
}

/*
================
ModalDialogWindow::ModalDialogWindow

Hidden at first, with no buttons.
================
*/
// FUNCTION: STBC 0x005023c0 ??0ModalDialogWindow@@QAE@XZ
ModalDialogWindow::ModalDialogWindow( void ) : MainWindow( MWT_MODAL_DIALOG, 1.0f, 1.0f ) {
	firstEvent = NULL;
	secondEvent = NULL;
	quitDialog = false;
	mouseGrabbed = false;
	firstButton = NULL;
	secondButton = NULL;
	SetNotBatchChildPolys( true );
	SetNotVisible( true );
}

/*
================
ModalDialogWindow::GetObjType
================
*/
// FUNCTION: STBC 0x00502430
int ModalDialogWindow::GetObjType( void ) {
	return MODALDIALOGWINDOW_TYPE;
}

/*
================
ModalDialogWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00502440
bool ModalDialogWindow::IsTypeOf( int type ) {
	if ( type == MODALDIALOGWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
ModalDialogWindow::GetClassName
================
*/
// FUNCTION: STBC 0x005024a0
char *ModalDialogWindow::GetClassName( void ) {
	return "ModalDialogWindow";
}

/*
================
ModalDialogWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x005024b0
char *ModalDialogWindow::GetSwigTypeName( void ) {
	return "_p_ModalDialogWindow";
}

/*
================
ModalDialogWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x005024c0
char *ModalDialogWindow::GetPythonClassName( void ) {
	return "ModalDialogWindowPtr";
}

/*
================
ModalDialogWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x005024d0
TGEventHandlerTable *ModalDialogWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ModalDialogWindow::ModalDialogWindow

The window the stream loader makes; the stream gives the rest.
================
*/
// FUNCTION: STBC 0x00502520 ??0ModalDialogWindow@@QAE@J@Z
ModalDialogWindow::ModalDialogWindow( long id ) : MainWindow( id ) {
	mouseGrabbed = false;
	firstButton = NULL;
	secondButton = NULL;
	firstEvent = NULL;
	secondEvent = NULL;
}

/*
================
ModalDialogWindow::Run

Builds the dialog: a stylized window holding the message and one or two
buttons, which send the events of their own to the dialog. The dialog
holds the callers' events, and is shown in front.
================
*/
// FUNCTION: STBC 0x00502550
// PARKED: x87 temps: retail reads the border height and the children's heights straight from memory while this build copies them through stack temporaries
void ModalDialogWindow::Run( TGString *title, TGString *message, TGString *firstText, TGEvent *firstReply, TGString *secondText, TGEvent *secondReply ) {
	if ( GetNumChildren() != 0 ) {
		KillChildren();
	}
	firstButton = NULL;
	secondButton = NULL;
	mouseGrabbed = false;
	keyboardConfig = false;
	SetNotBatchChildPolys( true );
	quitDialog = false;

	STStylizedWindow *window = new STStylizedWindow( "StylizedWindow", "SolidGlass", title, 0.0f, 0.0f, NULL, true, 0.5f, 0.35f, &g_kInterfaceBorderColor );
	window->SetUseScrolling( false );
	AddChild( window, 0.25f, 0.375f, false );

	if ( message != NULL ) {
		TGParagraph *paragraph = new TGParagraph( message, window->GetMaximumInteriorWidth() - 0.01f, NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
		float y = window->GetMaximumInteriorHeight() * 0.5f - paragraph->GetHeight();
		float x = ( window->GetMaximumInteriorWidth() - paragraph->GetWidth() ) * 0.5f;
		window->AddChild( paragraph, x, y, false );
	}

	float buttonTop = 0.0f;
	if ( firstText != NULL ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_OKAY );
		event->SetDestination( this );
		firstEvent = firstReply;
		if ( firstReply != NULL ) {
			firstReply->IncRefCount();
		}
		STButton *button = new STButton( firstText, event, STBF_SIZE_TO_TEXT, 0.0f, 0.0f, NULL, 200 );
		firstButton = button;
		if ( secondText == NULL ) {
			window->AddChild( button, window->GetMaximumInteriorWidth() * 0.5f - button->GetWidth() * 0.5f, ( window->GetHeight() - window->GetBorderHeight() ) - button->GetHeight(), false );
		} else {
			window->AddChild( button, window->GetMaximumInteriorWidth() * 0.25f - button->GetWidth() * 0.5f, ( window->GetHeight() - window->GetBorderHeight() ) - button->GetHeight(), false );
		}
		window->SetFocus( button, true );
		buttonTop = button->GetTop();
	}

	if ( secondText != NULL ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_CANCEL );
		event->SetDestination( this );
		secondEvent = secondReply;
		if ( secondReply != NULL ) {
			secondReply->IncRefCount();
		}
		STButton *button = new STButton( secondText, event, STBF_SIZE_TO_TEXT, 0.0f, 0.0f, NULL, 200 );
		secondButton = button;
		if ( firstText != NULL ) {
			window->AddChild( button, window->GetMaximumInteriorWidth() * 0.75f - button->GetWidth() * 0.5f, buttonTop, false );
		} else {
			window->AddChild( button, window->GetMaximumInteriorWidth() * 0.5f - button->GetWidth() * 0.5f, ( window->GetHeight() - window->GetBorderHeight() ) - button->GetHeight(), false );
			window->SetFocus( button, true );
		}
	}

	window->InteriorChangedSize( false );
	SetVisible( true );
	TopWindow::GetTopWindow()->MoveToFront( this, true );
}

/*
================
ModalDialogWindow::RunQuitDialog

Builds the quit dialog: a stylized window holding the question and the
buttons Exit Program, Exit Game (when a game that isn't a multiplayer one
runs) and Cancel, each in a pane of its own below the other.
================
*/
// FUNCTION: STBC 0x00502990
void ModalDialogWindow::RunQuitDialog( void ) {
	if ( GetNumChildren() != 0 ) {
		KillChildren();
	}
	quitDialog = true;
	firstButton = NULL;
	secondButton = NULL;
	mouseGrabbed = false;
	keyboardConfig = false;

	Game *game = Game::GetCurrentGame();
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );

	STStylizedWindow *window = new STStylizedWindow( "StylizedWindow", "SolidGlass", NULL, 0.0f, 0.0f, NULL, true, 0.5f, 0.27f, &g_kInterfaceBorderColor );
	window->SetUseScrolling( false );
	AddChild( window, 0.25f, 0.375f, false );

	TGString *question = database->GetString( "Are you sure you want to quit" );
	TGParagraph *paragraph = new TGParagraph( question, window->GetMaximumInteriorWidth() - 0.01f, NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
	window->AddChild( paragraph, ( window->GetMaximumInteriorWidth() - paragraph->GetWidth() ) * 0.5f, 0.01f, false );
	float y = paragraph->GetHeight() + 0.01f;
	y += 0.01f;

	TGString *text = database->GetString( "Exit Program" );
	TGPane *pane = new TGPane( 0.25f, 0.1f );
	TGEvent *event = new TGEvent;
	event->SetEventType( ET_EXIT_PROGRAM );
	event->SetDestination( this );
	STButton *button = new STButton( text, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
	button->Resize( 0.25f, button->GetHeight(), false );
	pane->Resize( 0.25f, button->GetHeight(), false );
	pane->AddChild( button, 0.0f, 0.0f, false );
	window->AddChild( pane, 0.125f, y, false );
	window->SetFocus( pane, true );
	pane->SetFocus( button, true );
	button->SetHighlighted( true );
	float step = button->GetHeight() + 0.01f;
	y = step + y;

	if ( game != NULL && !g_kUtopiaModule.IsMultiplayer() ) {
		text = database->GetString( "Exit Game" );
		pane = new TGPane( 0.25f, 0.1f );
		event = new TGEvent;
		event->SetEventType( ET_EXIT_GAME );
		event->SetDestination( this );
		button = new STButton( text, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
		button->Resize( 0.25f, button->GetHeight(), false );
		firstButton = button;
		pane->Resize( 0.25f, button->GetHeight(), false );
		pane->AddChild( button, 0.0f, 0.0f, false );
		window->AddChild( pane, 0.125f, y, false );
		step = button->GetHeight() + 0.01f;
		y = step + y;
	}

	text = database->GetString( "Cancel" );
	pane = new TGPane( 0.25f, 0.1f );
	event = new TGEvent;
	event->SetEventType( ET_CANCEL );
	event->SetDestination( this );
	button = new STButton( text, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
	button->Resize( 0.25f, button->GetHeight(), false );
	secondButton = button;
	pane->Resize( 0.25f, button->GetHeight(), false );
	pane->AddChild( button, 0.0f, 0.0f, false );
	window->AddChild( pane, 0.125f, y, false );

	window->InteriorChangedSize( false );
	g_kLocalizationManager.Unload( database );
	SetVisible( true );
	TopWindow::GetTopWindow()->MoveToFront( this, true );
}

/*
================
ModalDialogWindow::Cancel

Takes the dialog's contents away and hides it, forgetting its buttons.
================
*/
// FUNCTION: STBC 0x00502f20
void ModalDialogWindow::Cancel( void ) {
	KillChildren();
	SetNotVisible( false );
	firstButton = NULL;
	secondButton = NULL;
}

/*
================
ModalDialogWindow::SetNotVisible

Hidden, the dialog goes behind the other windows.
================
*/
// FUNCTION: STBC 0x00502f50
void ModalDialogWindow::SetNotVisible( bool layout ) {
	MainWindow::SetNotVisible( layout );
	TopWindow::GetTopWindow()->MoveToBack( this, true );
}

/*
================
ModalDialogWindow::Update

A visible dialog stays in front.
================
*/
// FUNCTION: STBC 0x00502f80
void ModalDialogWindow::Update( float time ) {
	if ( IsVisible() ) {
		TopWindow::GetTopWindow()->MoveToFront( this, true );
	}
}

/*
================
ModalDialogWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00502fa0
void ModalDialogWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::OkayHandler, "ModalDialogWindow::OkayHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::CancelHandler, "ModalDialogWindow::CancelHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::ExitGameHandler, "ModalDialogWindow::ExitGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::ExitProgramHandler, "ModalDialogWindow::ExitProgramHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::KeyboardHandler, "ModalDialogWindow::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::MouseHandler, "ModalDialogWindow::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ModalDialogWindow::RestoreGraphicsModeHandler, "ModalDialogWindow::RestoreGraphicsModeHandler" );
}

/*
================
ModalDialogWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00503030
void ModalDialogWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_OKAY, "ModalDialogWindow::OkayHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_CANCEL, "ModalDialogWindow::CancelHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_EXIT_GAME, "ModalDialogWindow::ExitGameHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_EXIT_PROGRAM, "ModalDialogWindow::ExitProgramHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "ModalDialogWindow::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "ModalDialogWindow::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_REVERT, "ModalDialogWindow::RestoreGraphicsModeHandler" );
}

/*
================
ModalDialogWindow::RestoreGraphicsModeHandler

While the graphics menu is open the dialog passes the event on to the top
window, which the menu's handlers listen to.
================
*/
// FUNCTION: STBC 0x005030d0
void ModalDialogWindow::RestoreGraphicsModeHandler( TGIntEvent *event ) {
	if ( graphicsMenuInfo != NULL && graphicsMenuInfo->graphicsMenu != NULL ) {
		TGIntEvent *forward = new TGIntEvent;
		forward->SetEventType( event->GetEventType() );
		forward->SetDestination( TopWindow::GetTopWindow() );
		forward->SetInt( event->GetInt() );
		g_kEventManager.AddEvent( forward );
	}
	CallNextHandler( event );
}

/*
================
ModalDialogWindow::OkayHandler

Sends the first button's event, drops the second's and closes the dialog.
================
*/
// FUNCTION: STBC 0x00503150
void ModalDialogWindow::OkayHandler( TGEvent *event ) {
	if ( firstEvent != NULL ) {
		TGEvent *reply = firstEvent;
		firstEvent = NULL;
		g_kEventManager.ProcessEvent( reply );
		reply->DecRefCount();
	}
	if ( secondEvent != NULL ) {
		secondEvent->DecRefCount();
		secondEvent = NULL;
	}
	KillChildren();
	firstButton = NULL;
	secondButton = NULL;
	SetNotVisible( false );
}

/*
================
ModalDialogWindow::CancelHandler

Sends the second button's event, drops the first's and closes the dialog.
================
*/
// FUNCTION: STBC 0x005031b0
void ModalDialogWindow::CancelHandler( TGEvent *event ) {
	if ( secondEvent != NULL ) {
		TGEvent *reply = secondEvent;
		secondEvent = NULL;
		g_kEventManager.ProcessEvent( reply );
		reply->DecRefCount();
	}
	if ( firstEvent != NULL ) {
		firstEvent->DecRefCount();
		firstEvent = NULL;
	}
	KillChildren();
	firstButton = NULL;
	secondButton = NULL;
	SetNotVisible( false );
	TopWindow::GetTopWindow()->MoveToBack( this, true );
}

/*
================
ModalDialogWindow::ExitGameHandler

Closes the dialog and ends the game.
================
*/
// FUNCTION: STBC 0x00503220
void ModalDialogWindow::ExitGameHandler( TGEvent *event ) {
	KillChildren();
	firstButton = NULL;
	secondButton = NULL;
	SetNotVisible( false );

	Game *game = Game::GetCurrentGame();
	TGBoolEvent *quit = new TGBoolEvent;
	quit->SetEventType( ET_QUIT );
	quit->SetDestination( game );
	quit->SetBool( true );
	TGSound *sound = g_kSoundManager.GetSound( "UIQuit" );
	g_kEventManager.AddEvent( quit );
	if ( sound != NULL ) {
		sound->Play( true );
	}
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
ModalDialogWindow::ExitProgramHandler

Closes the dialog and has the options window end the program.
================
*/
// FUNCTION: STBC 0x005032d0
void ModalDialogWindow::ExitProgramHandler( TGEvent *event ) {
	KillChildren();
	firstButton = NULL;
	secondButton = NULL;

	MainWindow *options = TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
	TGIntEvent *quit = new TGIntEvent;
	quit->SetEventType( ET_QUIT );
	quit->SetDestination( options );
	quit->SetInt( 1 );
	g_kEventManager.AddEvent( quit );
	TGSound *sound = g_kSoundManager.GetSound( "UIQuit" );
	if ( sound != NULL ) {
		sound->Play( true );
	}
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
ModalDialogWindow::KeyboardHandler

Escape cancels. A question takes the arrow keys to move between its
buttons and Enter to press the focused one; the quit dialog takes Q for
Exit Program. A key configuration dialog passes every key to the script,
unless the mouse is held by one of the dialog's buttons.
================
*/
// FUNCTION: STBC 0x005033c0
void ModalDialogWindow::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( !IsVisible() ) {
		CallNextHandler( event );
	}
	int key = event->GetUnicode();
	if ( key == WC_ESCAPE && event->GetKeyState() == KS_NORMAL ) {
		CancelHandler( NULL );
		event->SetHandled();
		return;
	}
	if ( !keyboardConfig ) {
		int step;
		switch ( key ) {
		case 13:
			if ( event->GetKeyState() == KS_NORMAL ) {
				TGPane *window = (TGPane *)GetFirstChild();
				if ( window != NULL ) {
					TGPane *pane = TGPane::Cast( window->GetFocus() );
					if ( pane != NULL ) {
						STButton *button = STButton::Cast( pane );
						if ( button == NULL ) {
							button = STButton::Cast( pane->GetFocus() );
							if ( button == NULL ) {
								return;
							}
						}
						button->SendActivationEvent();
					}
				}
			}
			return;
		case 'Q':
		case 'q':
			if ( quitDialog ) {
				if ( event->GetKeyState() == KS_KEYUP ) {
					ExitProgramHandler( NULL );
				}
				event->SetHandled();
			}
			return;
		case WC_RIGHT:
		case WC_DOWN:
			step = 1;
			break;
		case WC_LEFT:
		case WC_UP:
			step = -1;
			break;
		default:
			return;
		}
		if ( event->GetKeyState() == KS_NORMAL ) {
			TGPane *window = (TGPane *)GetFirstChild();
			if ( window != NULL ) {
				TGUIObject *next;
				if ( step == 1 ) {
					next = InterfaceModule::GetNextUIElement( window, 0 );
				} else {
					next = InterfaceModule::GetPrevUIElement( window, 0 );
				}
				if ( next != NULL ) {
					next->GetParent()->SetFocus( next, true );
					next->SetHighlighted( true );
				}
			}
		}
	} else {
		TGUIObject *grab = g_kRootWindow.GetMouseGrabOwner();
		if ( grab != NULL && ( grab == firstButton || grab == secondButton ) ) {
			mouseGrabbed = true;
			return;
		}
		if ( mouseGrabbed && event->GetKeyState() == KS_KEYUP && key == WC_LBUTTON ) {
			mouseGrabbed = false;
			return;
		}
		PyObject *keyEvent = event->VirtualSlot08();
		PyEmbed::CallFunction( "MainMenu.KeyboardConfig", "NewKeySelected", NULL, NULL, "(O)", keyEvent );
		Py_DECREF( keyEvent );
	}
}

/*
================
ModalDialogWindow::MouseHandler

The dialog's contents take the mouse.
================
*/
// FUNCTION: STBC 0x00503600
void ModalDialogWindow::MouseHandler( TGMouseEvent *event ) {
	if ( !IsVisible() ) {
		CallNextHandler( event );
	}
	TGUIObject *first = GetFirstChild();
	if ( first != NULL ) {
		first->MouseHandler( event );
	}
}

/*
================
ModalDialogWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00503630
bool ModalDialogWindow::WriteToStream( TGStream *stream ) {
	if ( !MainWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( firstButton ) );
	stream->WriteID( GetSavedID( secondButton ) );
	stream->WriteID( GetSavedID( firstEvent ) );
	stream->WriteID( GetSavedID( secondEvent ) );
	return true;
}

/*
================
ModalDialogWindow::ReadFromStream

The buttons and events are IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x005036c0
bool ModalDialogWindow::ReadFromStream( TGStream *stream ) {
	if ( !MainWindow::ReadFromStream( stream ) ) {
		return false;
	}
	firstButton = (STButton *)stream->ReadID();
	secondButton = (STButton *)stream->ReadID();
	firstEvent = (TGEvent *)stream->ReadID();
	secondEvent = (TGEvent *)stream->ReadID();
	mouseGrabbed = false;
	keyboardConfig = false;
	quitDialog = false;
	return true;
}

/*
================
ModalDialogWindow::FixUpPointers

Whether every button and event the stream named has been loaded.
================
*/
// FUNCTION: STBC 0x00503720
bool ModalDialogWindow::FixUpPointers( void ) {
	if ( !TGUIObject::FixUpPointers() ) {
		return false;
	}
	if ( firstButton != NULL && GetTGObjectPtr( (long)firstButton ) == NULL ) {
		return false;
	}
	if ( secondButton != NULL && GetTGObjectPtr( (long)secondButton ) == NULL ) {
		return false;
	}
	if ( firstEvent != NULL && TGEvent::GetTGEventPtr( (long)firstEvent ) == NULL ) {
		return false;
	}
	if ( secondEvent != NULL && TGEvent::GetTGEventPtr( (long)secondEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ModalDialogWindow::PostFixUp

Turns the IDs the stream gave into the objects.
================
*/
// FUNCTION: STBC 0x005037a0
void ModalDialogWindow::PostFixUp( void ) {
	TGUIObject::PostFixUp();
	firstButton = (STButton *)GetSavedObject( (long)firstButton );
	secondButton = (STButton *)GetSavedObject( (long)secondButton );
	firstEvent = TGEvent::GetSavedEvent( (long)firstEvent );
	secondEvent = TGEvent::GetSavedEvent( (long)secondEvent );
}
