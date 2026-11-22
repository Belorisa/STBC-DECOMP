/*
===============================================================================

	TopWindow.cpp

===============================================================================
*/

#include "TopWindow.h"
#include "Game.h"
#include "BridgeWindow.h"
#include "CDCheckWindow.h"
#include "CinematicWindow.h"
#include "ConsoleWindow.h"
#include "FocusManager.h"
#include "GraphicsMenu.h"
#include "GraphicsModeInfo.h"
#include "HullClass.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "MainWindow.h"
#include "MapWindow.h"
#include "ModalDialogWindow.h"
#include "MultiplayerGame.h"
#include "MultiplayerWindow.h"
#include "OptionsWindow.h"
#include "PlacementEditor.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "STStylizedWindow.h"
#include "SubtitleWindow.h"
#include "TacticalWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMessage.h"
#include "TGMouseEvent.h"
#include "TGMovieManager.h"
#include "TGNetwork.h"
#include "TGRootPane.h"
#include "TGSystemWrapperClass.h"
#include "TGIcon.h"
#include "TGIntEvent.h"
#include "TGIconManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "UtopiaApp.h"
#include "UtopiaModule.h"
#include "NiApplication.h"
#include "renderer/NiD3DRender.h"
#include "VarManagerClass.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0050c180 _$E37
// FUNCTION: STBC 0x0050c190 _$E36
// FUNCTION: STBC 0x0050c1a0 _$E41
// FUNCTION: STBC 0x0050c1b0 _$E40
// FUNCTION: STBC 0x0050c1c0 _$E52
// FUNCTION: STBC 0x0050c1d0 _$E51
// FUNCTION: STBC 0x0050c1e0 _$E56
// FUNCTION: STBC 0x0050c1f0 _$E55
// FUNCTION: STBC 0x0050c200 _$E60
// FUNCTION: STBC 0x0050c210 _$E59
// FUNCTION: STBC 0x0050c220 _$E64
// FUNCTION: STBC 0x0050c230 _$E63
// FUNCTION: STBC 0x0050c240 _$E184
// FUNCTION: STBC 0x0050c250 _$E183
// FUNCTION: STBC 0x0050c260 _$E126
// FUNCTION: STBC 0x0050c270 _$E125
// FUNCTION: STBC 0x0050c280 _$E139
// FUNCTION: STBC 0x0050c290 _$E138
// FUNCTION: STBC 0x0050c340 _$E189
// FUNCTION: STBC 0x0050c350 _$E188
// FUNCTION: STBC 0x0050d050 _$E194
// FUNCTION: STBC 0x0050d060 _$E193
// FUNCTION: STBC 0x0050d2c0 _$E198
// FUNCTION: STBC 0x0050d2d0 _$E197
// FUNCTION: STBC 0x0050d7c0 _$E202
// FUNCTION: STBC 0x0050d7d0 _$E201
// FUNCTION: STBC 0x0050dbc0 _$E209
// FUNCTION: STBC 0x0050dbd0 _$E208
// FUNCTION: STBC 0x0050e150 _$E430
// FUNCTION: STBC 0x0050e160 _$E429
// FUNCTION: STBC 0x0050e200 _$E434
// FUNCTION: STBC 0x0050e210 _$E433
// FUNCTION: STBC 0x0050e2e0 _$E88
// FUNCTION: STBC 0x0050e2f0 _$E87
// FUNCTION: STBC 0x0050e7a0 _$E313
// FUNCTION: STBC 0x0050e7b0 _$E312
// FUNCTION: STBC 0x0050eae0 _$E320
// FUNCTION: STBC 0x0050eaf0 _$E319
// FUNCTION: STBC 0x0050ef20 _$E324
// FUNCTION: STBC 0x0050ef30 _$E323
// FUNCTION: STBC 0x0050f080 _$E328
// FUNCTION: STBC 0x0050f090 _$E327
// FUNCTION: STBC 0x0050f160 _$E332
// FUNCTION: STBC 0x0050f170 _$E331
// FUNCTION: STBC 0x0050f450 _$E338
// FUNCTION: STBC 0x0050f460 _$E337
// FUNCTION: STBC 0x0050f620 _$E417
// FUNCTION: STBC 0x0050f630 _$E416
// FUNCTION: STBC 0x0050f640 _$E438
// FUNCTION: STBC 0x0050f650 _$E437
// FUNCTION: STBC 0x0050f660 _$E442
// FUNCTION: STBC 0x0050f670 _$E441
// FUNCTION: STBC 0x0050f680 _$E446
// FUNCTION: STBC 0x0050f690 _$E445
// FUNCTION: STBC 0x0050f6a0 _$E450
// FUNCTION: STBC 0x0050f6b0 _$E449
// FUNCTION: STBC 0x0050f6c0 _$E454
// FUNCTION: STBC 0x0050f6d0 _$E453
// FUNCTION: STBC 0x0050f6e0 _$E458
// FUNCTION: STBC 0x0050f6f0 _$E457
// FUNCTION: STBC 0x0050f700 _$E462
// FUNCTION: STBC 0x0050f710 _$E461
// FUNCTION: STBC 0x0050f720 _$E466
// FUNCTION: STBC 0x0050f730 _$E465
// FUNCTION: STBC 0x0050f740 _$E470
// FUNCTION: STBC 0x0050f750 _$E469
// FUNCTION: STBC 0x0050f760 _$E93
// FUNCTION: STBC 0x0050f770 _$E92
// FUNCTION: STBC 0x0050f780 _$E474
// FUNCTION: STBC 0x0050f790 _$E473
// FUNCTION: STBC 0x0050f7a0 _$E98
// FUNCTION: STBC 0x0050f7b0 _$E97
// FUNCTION: STBC 0x0050f7c0 _$E478
// FUNCTION: STBC 0x0050f7d0 _$E477
// FUNCTION: STBC 0x0050f7e0 _$E482
// FUNCTION: STBC 0x0050f7f0 _$E481
// FUNCTION: STBC 0x0050f800 _$E486
// FUNCTION: STBC 0x0050f810 _$E485
// FUNCTION: STBC 0x0050f820 _$E490
// FUNCTION: STBC 0x0050f830 _$E489
// FUNCTION: STBC 0x0050f840 _$E103
// FUNCTION: STBC 0x0050f850 _$E102
// FUNCTION: STBC 0x0050f860 _$E494
// FUNCTION: STBC 0x0050f870 _$E493
// FUNCTION: STBC 0x0050f880 _$E498
// FUNCTION: STBC 0x0050f890 _$E497
// FUNCTION: STBC 0x0050f8a0 _$E502
// FUNCTION: STBC 0x0050f8b0 _$E501
// FUNCTION: STBC 0x0050f8c0 _$E109
// FUNCTION: STBC 0x0050f8d0 _$E108
// FUNCTION: STBC 0x0050f8e0 _$E114
// FUNCTION: STBC 0x0050f8f0 _$E113
// FUNCTION: STBC 0x0050f900 _$E144
// FUNCTION: STBC 0x0050f910 _$E143
// FUNCTION: STBC 0x0050f920 _$E506
// FUNCTION: STBC 0x0050f930 _$E505
// FUNCTION: STBC 0x0050f940 _$E149
// FUNCTION: STBC 0x0050f950 _$E148
// FUNCTION: STBC 0x0050f960 _$E510
// FUNCTION: STBC 0x0050f970 _$E509
// FUNCTION: STBC 0x0050f980 _$E514
// FUNCTION: STBC 0x0050f990 _$E513
// FUNCTION: STBC 0x0050f9a0 _$E154
// FUNCTION: STBC 0x0050f9b0 _$E153
// FUNCTION: STBC 0x0050f9c0 _$E518
// FUNCTION: STBC 0x0050f9d0 _$E517
// FUNCTION: STBC 0x0050f9e0 _$E119
// FUNCTION: STBC 0x0050f9f0 _$E118
// FUNCTION: STBC 0x0050fa00 _$E522
// FUNCTION: STBC 0x0050fa10 _$E521
// FUNCTION: STBC 0x0050fa20 _$E526
// FUNCTION: STBC 0x0050fa30 _$E525
// FUNCTION: STBC 0x0050fa40 _$E159
// FUNCTION: STBC 0x0050fa50 _$E158
// FUNCTION: STBC 0x0050fa60 _$E164
// FUNCTION: STBC 0x0050fa70 _$E163
// FUNCTION: STBC 0x0050fa80 _$E530
// FUNCTION: STBC 0x0050fa90 _$E529
// FUNCTION: STBC 0x0050faa0 _$E534
// FUNCTION: STBC 0x0050fab0 _$E533
// FUNCTION: STBC 0x0050fac0 _$E538
// FUNCTION: STBC 0x0050fad0 _$E537
// FUNCTION: STBC 0x0050fae0 _$E542
// FUNCTION: STBC 0x0050faf0 _$E541
// FUNCTION: STBC 0x0050fb00 _$E546
// FUNCTION: STBC 0x0050fb10 _$E545
// FUNCTION: STBC 0x0050fb20 _$E550
// FUNCTION: STBC 0x0050fb30 _$E549
// FUNCTION: STBC 0x0050fb40 _$E554
// FUNCTION: STBC 0x0050fb50 _$E553
// FUNCTION: STBC 0x0050fb60 _$E558
// FUNCTION: STBC 0x0050fb70 _$E557
// FUNCTION: STBC 0x0050fb80 _$E169
// FUNCTION: STBC 0x0050fb90 _$E168
// FUNCTION: STBC 0x0050fba0 _$E562
// FUNCTION: STBC 0x0050fbb0 _$E561
// FUNCTION: STBC 0x0050fbc0 _$E174
// FUNCTION: STBC 0x0050fbd0 _$E173
// FUNCTION: STBC 0x0050fbe0 _$E179
// FUNCTION: STBC 0x0050fbf0 _$E178
// FUNCTION: STBC 0x0050fd10 _$E566
// FUNCTION: STBC 0x0050fd20 _$E565
// FUNCTION: STBC 0x0050fd30 _$E571
// FUNCTION: STBC 0x0050fd40 _$E570
// FUNCTION: STBC 0x0050fd50 _$E575
// FUNCTION: STBC 0x0050fd60 _$E574

// The type MoveToFront keeps the options menu above (invented name).
#define MOVE_TO_FRONT_EXCEPTION_TYPE	0x80D5

// The LCARS group's plain fill icon, drawn tinted for the letterbox bars
// and the screen fade (invented name).
#define LCARS_FILL_ICON		200

// DATA: STBC 0x009878cc
TopWindow *TopWindow::topWindow;

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x0050be90 _$E604
// FUNCTION: STBC 0x0050be70 _$E605

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0050c110 _$E389
// FUNCTION: STBC 0x0050c130 _$E390
// FUNCTION: STBC 0x0050c120 _$E391
// FUNCTION: STBC 0x0050c100 _$E392
// FUNCTION: STBC 0x0050c150 _$E395
// FUNCTION: STBC 0x0050c170 _$E396
// FUNCTION: STBC 0x0050c160 _$E397
// FUNCTION: STBC 0x0050c140 _$E398

// The handler tables' dynamic initialisers and exit-time destructors: the top
// window's and those of the main windows its unit holds (they share one guard
// byte).
// FUNCTION: STBC 0x0050c2b0 _$E577
// FUNCTION: STBC 0x0050c2d0 _$E578
// FUNCTION: STBC 0x0050c2c0 _$E580
// FUNCTION: STBC 0x0050c2a0 _$E581
// FUNCTION: STBC 0x0050c300 _$E583
// FUNCTION: STBC 0x0050c320 _$E584
// FUNCTION: STBC 0x0050c310 _$E585
// FUNCTION: STBC 0x0050c2f0 _$E586
// FUNCTION: STBC 0x0050c370 _$E588
// FUNCTION: STBC 0x0050c390 _$E589
// FUNCTION: STBC 0x0050c380 _$E590
// FUNCTION: STBC 0x0050c360 _$E591
// FUNCTION: STBC 0x0050c3c0 _$E593
// FUNCTION: STBC 0x0050c3e0 _$E594
// FUNCTION: STBC 0x0050c3d0 _$E595
// FUNCTION: STBC 0x0050c3b0 _$E596
// DATA: STBC 0x00987878
TGEventHandlerTable TopWindow::classHandlers;
// DATA: STBC 0x00987768
TGEventHandlerTable MainWindow::classHandlers;
// DATA: STBC 0x00987860
TGEventHandlerTable ConsoleWindow::classHandlers;
// DATA: STBC 0x009877a8
TGEventHandlerTable OptionsWindow::classHandlers;

/*
================
OptionsWindow::Cast

The object as an OptionsWindow; NULL when it is none. Defined in the
top window's unit.
================
*/
// FUNCTION: STBC 0x0050c400
OptionsWindow *OptionsWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( OPTIONSWINDOW_TYPE ) ) {
		return (OptionsWindow *)object;
	}
	return NULL;
}

/*
================
TopWindow::TopWindow

The one top window: it adds the options menu, the console, the
multiplayer window, the CD check and the modal dialog, and runs the
TopWindow script's Initialize.
================
*/
// FUNCTION: STBC 0x0050c430
// PARKED: complete; retail calls NiTArray<TGUIObject *>'s constructor (0x50FC00) out of line inside the inlined OptionsWindow constructor, where NiTArray.h (NetImmerse lane) defines it inline
TopWindow::TopWindow( void ) : TGPane( 1.0f, 1.0f ) {
	topWindow = this;
	badConnectionText = NULL;
	editMode = false;
	keyboardInputAllowed = true;
	mouseInputAllowed = true;
	optionsMenuDisabled = false;
	AddChild( new OptionsWindow, 0.225f, 0.175f, false );
	AddChild( new ConsoleWindow, 0.0f, 0.0f, false );
	AddChild( new MultiplayerWindow, 0.0f, 0.0f, false );
	AddChild( new CDCheckWindow, 0.225f, 0.375f, false );
	AddChild( new ModalDialogWindow, 0.0f, 0.0f, false );
	letterboxTop = NULL;
	letterboxBottom = NULL;
	cutsceneState = CUTSCENE_NONE;
	lastRenderedSetID = 0;
	Initialize();
}

/*
================
TopWindow::GetObjType
================
*/
// FUNCTION: STBC 0x0050c650
int TopWindow::GetObjType( void ) {
	return TOPWINDOW_TYPE;
}

/*
================
TopWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x0050c660
bool TopWindow::IsTypeOf( int type ) {
	if ( type == TOPWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
TopWindow::GetClassName
================
*/
// FUNCTION: STBC 0x0050c6c0
char *TopWindow::GetClassName( void ) {
	return "TopWindow";
}

/*
================
TopWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0050c6d0
char *TopWindow::GetSwigTypeName( void ) {
	return "_p_TopWindow";
}

/*
================
TopWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0050c6e0
char *TopWindow::GetPythonClassName( void ) {
	return "TopWindowPtr";
}

/*
================
TopWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0050c6f0
TGEventHandlerTable *TopWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TopWindow::~TopWindow

The letterbox bars are the window's own while no pane holds them.
================
*/
// FUNCTION: STBC 0x0050c830
TopWindow::~TopWindow( void ) {
	if ( letterboxTop != NULL && letterboxTop->GetParent() == NULL ) {
		delete letterboxTop;
	}
	if ( letterboxBottom != NULL && letterboxBottom->GetParent() == NULL ) {
		delete letterboxBottom;
	}
}

/*
================
TopWindow::RegisterHandlerNames

The top window's handlers and the options window's, then those of the other
main windows.
================
*/
// FUNCTION: STBC 0x0050c8b0
void TopWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::KeyboardHandler, "TopWindow::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ToggleConsoleHandler, "TopWindow::ToggleConsoleHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ToggleOptionsHandler, "TopWindow::ToggleOptionsHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ToggleEditHandler, "TopWindow::ToggleEditHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::TabFocusHandler, "TopWindow::TabFocusHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::PrintScreenHandler, "TopWindow::PrintScreenHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ToggleBridgeAndTacticalHandler, "TopWindow::ToggleBridgeAndTacticalHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::MouseHandler, "TopWindow::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::SelfDestructHandler, "TopWindow::SelfDestructHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::KeyboardHandler, "OptionsWindow::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::LoadGameHandler, "OptionsWindow::LoadGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::SaveGameHandler, "OptionsWindow::SaveGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::NewGameHandler, "OptionsWindow::NewGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::NewMultiplayerGameHandler, "OptionsWindow::NewMultiplayerGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OptionsWindow::QuitHandler, "OptionsWindow::QuitHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ResolutionChangeHandler, "TopWindow::ResolutionChangeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TopWindow::ResolutionChangeBackHandler, "TopWindow::ResolutionChangeBackHandler" );
	BridgeWindow::RegisterHandlerNames();
	TacticalWindow::RegisterHandlerNames();
	MapWindow::RegisterHandlerNames();
	CinematicWindow::RegisterHandlerNames();
	CDCheckWindow::RegisterHandlerNames();
	ModalDialogWindow::RegisterHandlerNames();
}

/*
================
IsMenuMode

Whether a display mode is one the resolution menu offers: the colour depth
given, in one of the LCARS sizes. The name is invented; the handler below
tests each mode through an inline helper (its inline budget leaves the
renderer temporary's DecRefCount out of line, as retail does).
================
*/
static inline bool IsMenuMode( NiD3DDisplayMode *displayMode, int depth ) {
	return displayMode->bitDepth == (unsigned int)depth && displayMode->width >= 640 && displayMode->height >= 480
		&& ( ( displayMode->width == 640 && displayMode->height == 480 ) || ( displayMode->width == 800 && displayMode->height == 600 )
			|| ( displayMode->width == 1024 && displayMode->height == 768 ) || ( displayMode->width == 1280 && displayMode->height == 1024 )
			|| ( displayMode->width == 1600 && displayMode->height == 1200 ) );
}

/*
================
TopWindow::RegisterHandlers

The events of the top window's handlers and of the options window's, then
those of the other main windows.
================
*/
// FUNCTION: STBC 0x0050ca50
void TopWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TopWindow::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_TOGGLE_CONSOLE, "TopWindow::ToggleConsoleHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_TOGGLE_OPTIONS, "TopWindow::ToggleOptionsHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_DEBUG_TOGGLE_EDIT_MODE, "TopWindow::ToggleEditHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_TAB_FOCUS_CHANGE, "TopWindow::TabFocusHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_PRINT_SCREEN, "TopWindow::PrintScreenHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_TOGGLE_BRIDGE_AND_TACTICAL, "TopWindow::ToggleBridgeAndTacticalHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELF_DESTRUCT, "TopWindow::SelfDestructHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TopWindow::MouseHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_KEYBOARD, "OptionsWindow::KeyboardHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_NEW_GAME, "OptionsWindow::NewGameHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_NEW_MULTIPLAYER_GAME, "OptionsWindow::NewMultiplayerGameHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_LOAD_GAME, "OptionsWindow::LoadGameHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_SAVE_GAME, "OptionsWindow::SaveGameHandler" );
	AddMethodHandlerForClass( &OptionsWindow::classHandlers, ET_QUIT, "OptionsWindow::QuitHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2, "TopWindow::ResolutionChangeHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_CHANGE_STAGE3, "TopWindow::ResolutionChangeBackHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_CHANGE_STAGE4, "TopWindow::ResolutionChangeBackHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GRAPHICS_RESOLUTION_REVERT, "TopWindow::ResolutionChangeHandler" );
	BridgeWindow::RegisterHandlers();
	TacticalWindow::RegisterHandlers();
	MapWindow::RegisterHandlers();
	CinematicWindow::RegisterHandlers();
	MultiplayerWindow::RegisterHandlers();
	CDCheckWindow::RegisterHandlers();
	ModalDialogWindow::RegisterHandlers();
}

/*
================
TopWindow::ResolutionChangeHandler

Changes the display to the nth of the device's modes that suit the game (the
event's value; the mode of the same colour depth in one of the LCARS sizes),
keeping the old mode to revert to. The next step is posted: a change that took
asks about keeping it, one that failed says so; the value is the index of the
mode the screen had.
================
*/
// FUNCTION: STBC 0x0050cc00
void TopWindow::ResolutionChangeHandler( TGIntEvent *event ) {
	NiRender *renderer = UtopiaApp::GetRenderer();
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	if ( renderer == NULL ) {
		return;
	}
	Ni3DDeviceDesc *device = ( (NiD3DRender *)renderer )->Unknown7C2EE0();
	if ( device == NULL || device->modes == NULL ) {
		return;
	}
	GraphicsMenu *menu;
	if ( graphicsMenuInfo != NULL ) {
		menu = graphicsMenuInfo->graphicsMenu;
	} else {
		menu = NULL;
	}

	int previous = 0;
	int index = 0;
	NiTListIterator pos = device->modes->GetHeadPos();
	while ( pos ) {
		NiD3DDisplayMode *displayMode = device->modes->GetNext( pos );
		if ( IsMenuMode( displayMode, mode->GetColorDepth() ) ) {
			if ( displayMode->width == mode->width && displayMode->height == (unsigned int)mode->GetHeight() ) {
				previous = index;
				break;
			}
			index++;
		}
	}

	index = 0;
	pos = device->modes->GetHeadPos();
	while ( pos ) {
		NiD3DDisplayMode *displayMode = device->modes->GetNext( pos );
		if ( IsMenuMode( displayMode, mode->GetColorDepth() ) ) {
			if ( event->GetInt() == index ) {
				graphicsMenuInfo->oldMode = *mode;
				mode->width = displayMode->width;
				mode->height = displayMode->height;
				graphicsMenuInfo->detect = false;
				if ( !menu->ChangeDisplayMode() ) {
					if ( event->GetEventType() == ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2 ) {
						TGIntEvent *next = new TGIntEvent;
						next->SetEventType( ET_GRAPHICS_RESOLUTION_CHANGE_STAGE3 );
						next->SetDestination( this );
						next->SetInt( previous );
						g_kEventManager.AddEvent( next );
					}
				} else {
					if ( event->GetEventType() == ET_GRAPHICS_RESOLUTION_CHANGE_STAGE2 ) {
						TGIntEvent *next = new TGIntEvent;
						next->SetEventType( ET_GRAPHICS_RESOLUTION_CHANGE_STAGE4 );
						next->SetDestination( this );
						next->SetInt( previous );
						g_kEventManager.AddEvent( next );
					}
				}
				return;
			}
			index++;
		}
	}
}

/*
================
TopWindow::ResolutionChangeBackHandler

The last two steps of a resolution change. A change that failed is told in the
modal dialog, which sends the revert event on OK. One that took is asked
about: Keep leaves it, and Revert sends the revert event.
================
*/
// FUNCTION: STBC 0x0050ceb0
void TopWindow::ResolutionChangeBackHandler( TGIntEvent *event ) {
	int value = event->GetInt();
	if ( event->GetEventType() == ET_GRAPHICS_RESOLUTION_CHANGE_STAGE3 ) {
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
		TGString *message = database->GetString( "Change Resolution Failed" );
		ModalDialogWindow *dialog = (ModalDialogWindow *)FindMainWindow( MWT_MODAL_DIALOG );
		TGIntEvent *revert = new TGIntEvent;
		revert->SetEventType( ET_GRAPHICS_RESOLUTION_REVERT );
		revert->SetInt( value );
		revert->SetDestination( this );
		dialog->Run( NULL, message, database->GetString( "OK" ), revert, NULL, NULL );
		g_kLocalizationManager.Unload( database );
		return;
	}
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
	TGString *message = database->GetString( "Keep Resolution" );
	ModalDialogWindow *dialog = (ModalDialogWindow *)FindMainWindow( MWT_MODAL_DIALOG );
	TGIntEvent *revert = new TGIntEvent;
	revert->SetEventType( ET_GRAPHICS_RESOLUTION_REVERT );
	revert->SetInt( value );
	revert->SetDestination( this );
	dialog->Run( NULL, message, database->GetString( "Keep" ), NULL, database->GetString( "Revert" ), revert );
	STStylizedWindow *window = STStylizedWindow::Cast( dialog->GetFirstChild() );
	window->SetFocus( window->GetNthChild( 2 ), true );
	g_kLocalizationManager.Unload( database );
}

/*
================
TopWindow::SelfDestructHandler

In multiplayer a client asks the host; the host and a single-player game
that isn't paused destroy the player's ship through its hull.
================
*/
// FUNCTION: STBC 0x0050d070
// DRAFT: complete; retail doesn't share the hull load of the host and single-player paths (edx and ecx) and pops esi before the client path's CallNextHandler
void TopWindow::SelfDestructHandler( TGEvent *event ) {
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		if ( !g_kUtopiaModule.IsHost() ) {
			TGNetwork *network = g_kUtopiaModule.GetNetwork();
			if ( network != NULL ) {
				TGMessage *message = new TGMessage;
				char data = SELF_DESTRUCT_REQUEST_MESSAGE;
				message->SetData( &data, 1 );
				network->SendTGMessage( network->GetHostID(), message, 0 );
				CallNextHandler( event );
				return;
			}
		} else {
			ShipClass *player = Game::GetCurrentPlayer();
			if ( player != NULL ) {
				player->DestroySystem( player->GetHull() );
			}
		}
	} else if ( !UtopiaApp::GetApp()->IsGamePaused() ) {
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player != NULL ) {
			player->DestroySystem( player->GetHull() );
		}
	}
	CallNextHandler( event );
}

/*
================
TopWindow::KeyboardHandler

A forced movie takes the keys; so does the modal dialog while it is up. The
keys bound to the console, the options menu, edit mode and the focus change
are launched at once when the mouse isn't held. With the keyboard off only
the console and the options menu get keys, and the one that skips events.
================
*/
// FUNCTION: STBC 0x0050d150
void TopWindow::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( g_kMovieManager.IsForcedMoviePlaying() ) {
		event->SetHandled();
		return;
	}
	MainWindow *console = FindMainWindow( MWT_CONSOLE );
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	MainWindow *dialog = FindMainWindow( MWT_MODAL_DIALOG );
	if ( dialog != NULL && dialog->IsVisible() ) {
		( (ModalDialogWindow *)dialog )->KeyboardHandler( event );
		return;
	}
	unsigned short key = event->GetUnicode();
	if ( key == g_kKeyboardBinding.FindKey( ET_INPUT_TOGGLE_CONSOLE, 0, 0.0f, 0 )
		|| key == g_kKeyboardBinding.FindKey( ET_INPUT_TOGGLE_OPTIONS, 0, 0.0f, 0 )
		|| key == g_kKeyboardBinding.FindKey( ET_INPUT_DEBUG_TOGGLE_EDIT_MODE, 0, 0.0f, 0 )
		|| key == g_kKeyboardBinding.FindKey( ET_INPUT_TAB_FOCUS_CHANGE, 0, 0.0f, 0 ) ) {
		if ( g_kRootWindow.GetMouseGrabOwner() == NULL ) {
			if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), this, NULL, false ) ) {
				event->SetHandled();
			}
		}
	}
	if ( !keyboardInputAllowed && !console->IsVisible() && !options->IsVisible()
		&& key != g_kKeyboardBinding.FindKey( ET_INPUT_SKIP_EVENTS, 0, 0.0f, 0 ) ) {
		event->SetHandled();
		return;
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
	if ( !event->EventHandled() ) {
		if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), this, NULL, false ) ) {
			event->SetHandled();
		}
	}
}

/*
================
TopWindow::ToggleConsoleHandler

The console comes up only when the test menu is.
================
*/
// FUNCTION: STBC 0x0050d2e0
void TopWindow::ToggleConsoleHandler( TGEvent *event ) {
	if ( UtopiaApp::GetApp()->GetTestMenuState() >= 2 ) {
		ToggleConsole();
	}
	event->SetDestination( NULL );
}

/*
================
TopWindow::ToggleOptionsHandler

The options menu is only for a game that runs.
================
*/
// FUNCTION: STBC 0x0050d310
void TopWindow::ToggleOptionsHandler( TGEvent *event ) {
	if ( Game::GetCurrentGame() != NULL && !optionsMenuDisabled ) {
		ToggleOptionsMenu();
		event->SetDestination( NULL );
	}
}

/*
================
TopWindow::ToggleEditHandler

Entering edit mode brings up the editor through the tactical view.
================
*/
// FUNCTION: STBC 0x0050d340
void TopWindow::ToggleEditHandler( TGEvent *event ) {
	ToggleEditMode();
	if ( editMode ) {
		ForceTacticalVisible();
		ToggleBridgeAndTactical();
	}
	event->SetDestination( NULL );
}

/*
================
TopWindow::ToggleBridgeAndTacticalHandler

Not in multiplayer, while paused or with the keyboard off: the bridge script
drops its menus first, when the player's ship lives.
================
*/
// FUNCTION: STBC 0x0050d370
void TopWindow::ToggleBridgeAndTacticalHandler( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsMultiplayer() && !UtopiaApp::GetApp()->IsGamePaused() && keyboardInputAllowed == true && Game::GetCurrentGame() != NULL ) {
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player != NULL && player->GetLifeTime() >= INFINITE_LIFE ) {
			if ( PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", "", NULL, "" ) == -1 ) {
				PyErr_Print();
			}
			ToggleBridgeAndTactical();
		}
		event->SetDestination( NULL );
	}
}

/*
================
TopWindow::TabFocusHandler
================
*/
// FUNCTION: STBC 0x0050d400
void TopWindow::TabFocusHandler( TGEvent *event ) {
	g_kFocusManager.Tab();
	event->SetDestination( NULL );
}

/*
================
TopWindow::PrintScreenHandler
================
*/
// FUNCTION: STBC 0x0050d420
void TopWindow::PrintScreenHandler( TGEvent *event ) {
	g_kSystemWrapper.TakeScreenshot( 1, NULL );
	event->SetDestination( NULL );
}

/*
================
TopWindow::MouseHandler

A forced movie takes the mouse; so does the modal dialog while it is up.
With the mouse off only the console and the options menu get it.
================
*/
// FUNCTION: STBC 0x0050d440
void TopWindow::MouseHandler( TGMouseEvent *event ) {
	if ( g_kMovieManager.IsForcedMoviePlaying() ) {
		event->SetHandled();
		return;
	}
	MainWindow *console = FindMainWindow( MWT_CONSOLE );
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	MainWindow *dialog = FindMainWindow( MWT_MODAL_DIALOG );
	if ( dialog != NULL && dialog->IsVisible() ) {
		dialog->TGPane::MouseHandler( event );
		return;
	}
	if ( !mouseInputAllowed && !console->IsVisible() && !options->IsVisible() ) {
		event->SetHandled();
		return;
	}
	CallNextHandler( event );
}

/*
================
TopWindow::ToggleMapWindow

Leaving the bridge for the map turns the bridge menus back.
================
*/
// FUNCTION: STBC 0x0050d4d0
void TopWindow::ToggleMapWindow( void ) {
	TGUIObject *focus = GetFocus();
	MainWindow *mapWindow = FindMainWindow( MWT_TACTICAL_MAP );
	if ( focus != mapWindow ) {
		MainWindow *bridgeWindow = FindMainWindow( MWT_BRIDGE );
		if ( bridgeWindow != NULL && focus == bridgeWindow && Game::GetCurrentGame() != NULL ) {
			PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", "", NULL, "" );
		}
		MoveToFront( mapWindow );
	} else {
		MoveToBack( mapWindow );
	}
}

/*
================
TopWindow::ShowBadConnectionText

The text goes at the window's right edge.
================
*/
// FUNCTION: STBC 0x0050d550
void TopWindow::ShowBadConnectionText( bool show ) {
	if ( show == true ) {
		if ( badConnectionText == NULL ) {
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/tgl/Options.tgl", true );
			TGString *text = database->GetString( "Bad Connection" );
			badConnectionText = new TGParagraph( text, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
			g_kLocalizationManager.Unload( database );
			float width = GetWidth();
			AddChild( badConnectionText, width - badConnectionText->GetWidth(), 0.0f );
		}
	} else if ( badConnectionText != NULL ) {
		DeleteChild( badConnectionText );
		badConnectionText = NULL;
	}
}

/*
================
TopWindow::ToggleOptionsMenu

Not while the console has the focus, and not from inside itself. The
game pauses while the menu shows, the mouse is unlocked and the screen
cleared to black; closing it brings the subtitles back and restores the
mouse lock.
================
*/
// FUNCTION: STBC 0x0050d620
void TopWindow::ToggleOptionsMenu( void ) {
	static int toggling;

	MainWindow *console = FindMainWindow( MWT_CONSOLE );
	TGUIObject *focus = GetFocus();
	if ( focus == console && focus->IsVisible() ) {
		return;
	}
	if ( toggling == 1 ) {
		return;
	}
	toggling = 1;
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	if ( !options->IsVisible() ) {
		g_kUtopiaModule.Pause( true );
	}
	options->ToggleVisibility();
	if ( !options->IsVisible() ) {
		g_kUtopiaModule.Pause( false );
		MainWindow *subtitles = FindMainWindow( MWT_SUBTITLE );
		if ( subtitles != NULL ) {
			subtitles->SetVisible();
		}
		if ( !mouseWasLocked ) {
			g_kVarManager.SetFloatVariable( "global", "fLockMouse", 0.0f );
		}
	} else {
		SetFocus( options );
		if ( g_kVarManager.GetFloatVariable( "global", "fLockMouse" ) == 1.0f ) {
			mouseWasLocked = true;
		} else {
			mouseWasLocked = false;
		}
		g_kVarManager.SetFloatVariable( "global", "fLockMouse", 1.0f );
		SetLastRenderedSet( g_kSetManager.GetRenderedSet() );
		NiColor black = NiColor::BLACK;
		NiCamera *camera = g_kIconManager.GetCamera();
		camera->GetRenderer()->SetBackgroundColor( black );
	}
	ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticleManagerWindow != NULL ) {
		reticleManagerWindow->UpdateVisibility();
	}
	toggling = 0;
}

/*
================
TopWindow::ToggleConsole

Not in a multiplayer game. The game is paused while the console shows.
================
*/
// FUNCTION: STBC 0x0050d7e0
void TopWindow::ToggleConsole( void ) {
	if ( MultiplayerGame::Cast( Game::GetCurrentGame() ) == NULL ) {
		MainWindow *console = FindMainWindow( MWT_CONSOLE );
		if ( !console->IsVisible() ) {
			g_kUtopiaModule.Pause( true );
		}
		console->ToggleVisibility();
		if ( !console->IsVisible() ) {
			g_kUtopiaModule.Pause( false );
		}
	}
}

/*
================
TopWindow::ToggleCinematicWindow
================
*/
// FUNCTION: STBC 0x0050d830
void TopWindow::ToggleCinematicWindow( void ) {
	TGUIObject *focus = GetFocus();
	MainWindow *cinematicWindow = FindMainWindow( MWT_CINEMATIC );
	if ( focus == cinematicWindow ) {
		MoveToBack( cinematicWindow );
	} else {
		MoveToFront( cinematicWindow );
		if ( PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", NULL, NULL, NULL ) == -1 ) {
			PyErr_Print();
		}
	}
}

/*
================
TopWindow::ToggleEditMode

The game is paused while editing.
================
*/
// FUNCTION: STBC 0x0050d890
void TopWindow::ToggleEditMode( void ) {
	SetEditMode( !editMode );
	if ( editMode == true ) {
		g_kUtopiaModule.Pause( true );
	} else {
		g_kUtopiaModule.Pause( false );
	}
}

/*
================
TopWindow::ForceBridgeVisible

Brings the bridge in front without the usual transition. Coming from a
cutscene, the subtitles move back to their cinematic place.
================
*/
// FUNCTION: STBC 0x0050d8d0
// PARKED: register tie: this and bridge both at priority 57 (c2trace --regs) and bridge wins on last-def, so this build keeps this in EDI; ForceTacticalVisible has the extra MWT_TACTICAL constant that lifts this ahead
void TopWindow::ForceBridgeVisible( void ) {
	bool fromCinematic = false;
	if ( FindMainWindow( MWT_CINEMATIC )->HasFocus() ) {
		fromCinematic = true;
	}
	MainWindow *bridge = FindMainWindow( MWT_BRIDGE );
	if ( bridge != NULL ) {
		if ( !IsBridgeVisible() ) {
			if ( PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", "", NULL, "" ) == -1 ) {
				PyErr_Print();
			}
		}
		SetFocus( bridge, false );
		MoveToFront( bridge, false );
		bridge->SetVisible( false );
		bridge->Layout();
		if ( fromCinematic ) {
			SubtitleWindow *subtitle = (SubtitleWindow *)FindMainWindow( MWT_SUBTITLE );
			subtitle->SetPositionForMode( SM_CINEMATIC, false );
		}
	}
}

/*
================
TopWindow::ForceTacticalVisible

As ForceBridgeVisible, for the tactical view.
================
*/
// FUNCTION: STBC 0x0050d980
void TopWindow::ForceTacticalVisible( void ) {
	bool fromCinematic = false;
	if ( FindMainWindow( MWT_CINEMATIC )->HasFocus() ) {
		fromCinematic = true;
	}
	MainWindow *tactical = FindMainWindow( MWT_TACTICAL );
	if ( tactical != NULL ) {
		if ( !IsTacticalVisible() ) {
			if ( PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", "", NULL, "" ) == -1 ) {
				PyErr_Print();
			}
		}
		SetFocus( tactical, false );
		MoveToFront( tactical, false );
		tactical->SetVisible( false );
		tactical->Layout();
		if ( fromCinematic ) {
			SubtitleWindow *subtitle = (SubtitleWindow *)FindMainWindow( MWT_SUBTITLE );
			subtitle->SetPositionForMode( SM_CINEMATIC, false );
		}
	}
}

/*
================
TopWindow::ToggleBridgeAndTactical

Only while the player's ship lives. From the bridge, the editor or the
map the tactical view comes up; otherwise the bridge does, or the editor
in edit mode. The map is hidden either way.
================
*/
// FUNCTION: STBC 0x0050da30
void TopWindow::ToggleBridgeAndTactical( void ) {
	TGUIObject *focus = GetFocus();
	MainWindow *bridgeWindow = FindMainWindow( MWT_BRIDGE );
	MainWindow *tacticalWindow = FindMainWindow( MWT_TACTICAL );
	MainWindow *editorWindow = FindMainWindow( MWT_EDITOR );
	MainWindow *mapWindow = FindMainWindow( MWT_TACTICAL_MAP );
	if ( bridgeWindow == NULL || tacticalWindow == NULL || Game::GetCurrentGame() == NULL ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->GetLifeTime() < INFINITE_LIFE ) {
		return;
	}
	if ( IsBridgeVisible() || focus == editorWindow || focus == mapWindow ) {
		MoveToFront( tacticalWindow, false );
		tacticalWindow->SetVisible( false );
		bridgeWindow->SetNotVisible( false );
		if ( editorWindow != NULL ) {
			editorWindow->SetNotVisible( false );
		}
		tacticalWindow->Layout();
	} else if ( editMode ) {
		if ( editorWindow != NULL ) {
			MoveToFront( editorWindow, false );
			editorWindow->SetVisible( false );
			editorWindow->Layout();
		}
		tacticalWindow->SetNotVisible( false );
		bridgeWindow->SetNotVisible( false );
	} else {
		MoveToFront( bridgeWindow, false );
		bridgeWindow->SetVisible( false );
		if ( editorWindow != NULL ) {
			editorWindow->SetNotVisible( false );
		}
		tacticalWindow->SetNotVisible( false );
		bridgeWindow->Layout();
	}
	mapWindow->SetNotVisible();
}

/*
================
TopWindow::MoveToFront

A window that is an options menu's equal goes in front of a shown options
menu, and is then moved back one place behind it.
================
*/
// FUNCTION: STBC 0x0050dbe0
void TopWindow::MoveToFront( TGUIObject *child, bool layout ) {
	bool behindOptions = false;
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	if ( options != NULL && options != child && options->IsVisible() && child->IsTypeOf( MOVE_TO_FRONT_EXCEPTION_TYPE ) ) {
		behindOptions = true;
	}
	TGPane::MoveToFront( child, layout );
	if ( behindOptions ) {
		MoveTowardsBack( child, layout );
	}
}

/*
================
TopWindow::ToggleWireframe

Toggles the wireframe of the set being drawn.
================
*/
// FUNCTION: STBC 0x0050dc40
void TopWindow::ToggleWireframe( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	if ( set != NULL ) {
		set->SetWireframe( !set->GetWireframe() );
	}
}

/*
================
TopWindow::StartCutscene

The ship stops, the letterbox bars come in from the screen's top and
from height above its bottom over the time given, and the input is
shut off. False when a cutscene runs already.
================
*/
// FUNCTION: STBC 0x0050dc90
bool TopWindow::StartCutscene( float time, float height, bool updateReticles ) {
	PyEmbed::CallFunction( "TacticalInterfaceHandlers", "StopShip", NULL, NULL, NULL, NULL );
	if ( cutsceneState != CUTSCENE_NONE ) {
		return false;
	}
	unknown5C = time;
	unknown60 = time;
	unknown64 = height;
	cutsceneState = CUTSCENE_STARTING;

	char *lcarsString = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	NiColorA black( 0.0f, 0.0f, 0.0f, 1.0f );
	if ( letterboxTop == NULL ) {
		letterboxTop = new TGIcon( lcarsString, LCARS_FILL_ICON );
		letterboxTop->SetColor( &black );
	}
	AddChild( letterboxTop, 0.0f, 0.0f, false );
	if ( letterboxBottom == NULL ) {
		letterboxBottom = new TGIcon( lcarsString, LCARS_FILL_ICON );
		letterboxBottom->SetColor( &black );
	}
	AddChild( letterboxBottom, 0.0f, 1.0f - height, false );
	letterboxTop->Resize( 1.0f, letterboxTop->GetHeight(), false );
	letterboxBottom->Resize( 1.0f, letterboxTop->GetHeight(), false );
	letterboxTop->SetPosition( 0.0f, 0.0f );
	letterboxBottom->SetPosition( 0.0f, 1.0f - letterboxBottom->GetHeight() );
	AllowKeyboardInput( false );
	AllowMouseInput( false );
	if ( updateReticles ) {
		ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
		if ( reticleManagerWindow != NULL ) {
			reticleManagerWindow->UpdateVisibility();
		}
	}
	InterfaceModule::DoTheRightThing();
	return true;
}

/*
================
TopWindow::EndCutscene

Ends a cutscene that has started, over the time given.
================
*/
// FUNCTION: STBC 0x0050de70
bool TopWindow::EndCutscene( float time ) {
	if ( cutsceneState != CUTSCENE_ENDING && cutsceneState != CUTSCENE_NONE ) {
		cutsceneState = CUTSCENE_ENDING;
		unknown5C = time;
		unknown60 = time;
		InterfaceModule::DoTheRightThing();
		return true;
	}
	return false;
}

/*
================
TopWindow::AbortCutscene

Ends a cutscene at once: the bars go and the input comes back.
================
*/
// FUNCTION: STBC 0x0050dea0
void TopWindow::AbortCutscene( void ) {
	cutsceneState = CUTSCENE_NONE;
	ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticleManagerWindow != NULL ) {
		reticleManagerWindow->UpdateVisibility();
	}
	AllowKeyboardInput( true );
	AllowMouseInput( true );
	RemoveChild( letterboxTop );
	RemoveChild( letterboxBottom );
	InterfaceModule::DoTheRightThing();
}

/*
================
TopWindow::FadeOut

Fades the screen to black over the time given. A fade under way goes on
from its icon's alpha, in the part of the time that is left.
================
*/
// FUNCTION: STBC 0x0050df00
bool TopWindow::FadeOut( float time ) {
	fadeColor = NiColorA::BLACK;
	TGIcon *icon = TGIcon::Cast( GetTGObjectPtr( fadeIconID ) );
	if ( icon != NULL ) {
		float remaining = fadeColor.a - icon->GetAlpha();
		if ( remaining > 0.0f ) {
			fadeEndTime = UtopiaApp::GetApp()->GetGameTime() + remaining * time;
		}
	} else {
		fadeEndTime = UtopiaApp::GetApp()->GetGameTime() + time;
		char *lcarsString = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
		icon = new TGIcon( lcarsString, LCARS_FILL_ICON );
		fadeIconID = icon->GetObjID();
		NiColorA clear( 0.0f, 0.0f, 0.0f, 0.0f );
		icon->SetColor( &clear );
		AddChild( icon, 0.0f, 0.0f, false );
		icon->Resize( 1.01f, 1.01f );
		MainWindow *options = FindMainWindow( MWT_OPTIONS );
		if ( options != NULL && options->IsVisible() ) {
			icon->SetNotVisible( false );
		}
	}
	return true;
}

/*
================
TopWindow::FadeIn

Fades the screen back in over the time given, from the fade icon's
alpha.
================
*/
// FUNCTION: STBC 0x0050e070
bool TopWindow::FadeIn( float time ) {
	fadeColor = NiColorA( 0.0f, 0.0f, 0.0f, 0.0f );
	TGIcon *icon = TGIcon::Cast( GetTGObjectPtr( fadeIconID ) );
	if ( icon != NULL ) {
		float remaining = icon->GetAlpha() - fadeColor.a;
		if ( remaining > 0.0f ) {
			fadeEndTime = UtopiaApp::GetApp()->GetGameTime() + remaining * time;
		}
	}
	return true;
}

/*
================
TopWindow::AbortFade
================
*/
// FUNCTION: STBC 0x0050e0e0
void TopWindow::AbortFade( void ) {
	TGIcon *icon = TGIcon::Cast( GetTGObjectPtr( fadeIconID ) );
	if ( icon != NULL ) {
		DeleteChild( icon );
		fadeIconID = 0;
	}
}

/*
================
TopWindow::IsBridgeVisible
================
*/
// FUNCTION: STBC 0x0050e110
bool TopWindow::IsBridgeVisible( void ) {
	return FindMainWindow( MWT_BRIDGE ) == GetFocus() ? true : false;
}

/*
================
TopWindow::IsTacticalVisible
================
*/
// FUNCTION: STBC 0x0050e130
bool TopWindow::IsTacticalVisible( void ) {
	return FindMainWindow( MWT_TACTICAL ) == GetFocus() ? true : false;
}

/*
================
TopWindow::SetLastRenderedSet
================
*/
// FUNCTION: STBC 0x0050e170
void TopWindow::SetLastRenderedSet( SetClass *set ) {
	lastRenderedSetID = GetSavedID( set );
}

/*
================
TopWindow::GetLastRenderedSet
================
*/
// FUNCTION: STBC 0x0050e190
SetClass *TopWindow::GetLastRenderedSet( void ) {
	return SetClass::Cast( GetTGObjectPtr( lastRenderedSetID ) );
}

/*
================
TopWindow::FindMainWindow

The main window of the type; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0050e1b0
MainWindow *TopWindow::FindMainWindow( int type ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( MAINWINDOW_TYPE ) && ( (MainWindow *)child )->GetWindowType() == type ) {
			return (MainWindow *)child;
		}
	}
	return NULL;
}

/*
================
TopWindow::Initialize

The TopWindow script's Initialize, then each main window's.
================
*/
// FUNCTION: STBC 0x0050e220
void TopWindow::Initialize( void ) {
	PyObject *shadow = VirtualSlot08();
	int result = PyEmbed::CallFunction( "TopWindow", "Initialize", "", NULL, "(O)", shadow );
	Py_DECREF( shadow );
	if ( result == -1 ) {
		PyErr_Print();
	}
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( MAINWINDOW_TYPE ) ) {
			( (MainWindow *)child )->Initialize();
		}
	}
}

/*
================
TopWindow::Update
================
*/
// FUNCTION: STBC 0x0050e2a0
void TopWindow::Update( float time ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( MAINWINDOW_TYPE ) ) {
			( (MainWindow *)child )->Update( time );
		}
	}
}

/*
================
TopWindow::UpdateFade

Moves the fade icon's alpha towards the colour faded to over the time that is
left, and takes the icon away once the fade is over and clear. The icon waits
while the options menu is up.
================
*/
// FUNCTION: STBC 0x0050e300
// DRAFT: complete; retail copies only the icon's r, g and b into the stack colour, this build's NiColorA constructor zeroes all four first
void TopWindow::UpdateFade( void ) {
	TGIcon *icon = TGIcon::Cast( GetTGObjectPtr( fadeIconID ) );
	if ( icon == NULL ) {
		return;
	}
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	if ( options != NULL && options->IsVisible() ) {
		icon->SetNotVisible( false );
		return;
	}
	icon->SetVisible( true );

	UtopiaApp *app = UtopiaApp::GetApp();
	float remaining = fadeEndTime - app->GetGameTime();
	if ( remaining <= 0.0f ) {
		if ( fadeColor.a == 0.0f ) {
			DeleteChild( icon, true );
			fadeIconID = 0;
			return;
		}
		icon->SetColor( &fadeColor );
		return;
	}
	float step = app->GetGameFrameStep();
	float fraction = step / ( remaining + step );
	NiColorA color;
	icon->GetColor( &color );
	color.a = color.a * ( 1.0f - fraction ) + fadeColor.a * fraction;
	icon->SetColor( &color );
}

/*
================
TopWindow::UpdateCutscene

Slides the letterbox bars in while a cutscene starts, holds them while it
runs and slides them out while it ends, over the time each was given. The bars
leave while the options menu is up, and come back after it.
================
*/
// FUNCTION: STBC 0x0050e420
void TopWindow::UpdateCutscene( float time ) {
	if ( cutsceneState == CUTSCENE_NONE ) {
		return;
	}
	MainWindow *options = FindMainWindow( MWT_OPTIONS );
	if ( options != NULL && options->IsVisible() ) {
		RemoveChild( letterboxTop, true );
		RemoveChild( letterboxBottom, true );
		return;
	}
	if ( letterboxTop->GetParent() == NULL ) {
		AddChild( letterboxTop, 0.0f, 0.0f, true );
	}
	if ( letterboxBottom->GetParent() == NULL ) {
		AddChild( letterboxBottom, 0.0f, 1.0f, true );
	}
	if ( !letterboxTop->IsVisible() ) {
		letterboxTop->SetVisible( true );
	}
	if ( !letterboxBottom->IsVisible() ) {
		letterboxBottom->SetVisible( true );
	}

	time = unknown60 - time;
	unknown60 = time;
	if ( time <= 0.0f ) {
		if ( cutsceneState == CUTSCENE_ENDING ) {
			AllowKeyboardInput( true );
			AllowMouseInput( true );
			RemoveChild( letterboxTop, true );
			RemoveChild( letterboxBottom, true );
			cutsceneState = CUTSCENE_NONE;
			ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
			if ( reticleManagerWindow != NULL ) {
				reticleManagerWindow->UpdateVisibility();
			}
			return;
		}
		if ( cutsceneState == CUTSCENE_STARTING ) {
			cutsceneState = CUTSCENE_RUNNING;
		}
	}
	switch ( cutsceneState ) {
	case CUTSCENE_ENDING:
		time = time / unknown5C * unknown64;
		break;
	case CUTSCENE_RUNNING:
		time = unknown64;
		break;
	case CUTSCENE_STARTING:
		time = ( 1.0f - time / unknown5C ) * unknown64;
		break;
	}
	letterboxTop->Resize( 1.0f, time, true );
	letterboxBottom->Resize( 1.0f, time, true );
	letterboxTop->SetPosition( 0.0f, 0.0f, true );
	letterboxBottom->SetPosition( 0.0f, 1.0f - time, true );
}

/*
================
TopWindow::SetEditMode

The editors are made when edit mode starts and deleted when it ends.
================
*/
// FUNCTION: STBC 0x0050e5e0
void TopWindow::SetEditMode( bool edit ) {
	if ( edit != editMode ) {
		editMode = edit;
		if ( edit == true ) {
			PlacementEditor *editor = CreateEditors();
			AddChild( editor );
			editor->Initialize();
		} else {
			DestroyEditors();
		}
	}
}

/*
================
TopWindow::WriteToStream

The letterbox bars are taken out of the pane first and saved after the
window's state; the bad-connection text is not saved.
================
*/
// FUNCTION: STBC 0x0050e630
bool TopWindow::WriteToStream( TGStream *stream ) {
	if ( letterboxTop != NULL ) {
		RemoveChild( letterboxTop, false );
	}
	if ( letterboxBottom != NULL ) {
		RemoveChild( letterboxBottom, false );
	}
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( editMode );
	stream->WriteBool( unknown4D );
	stream->WriteBool( mouseWasLocked );
	stream->WriteBool( optionsMenuDisabled );
	stream->WriteBool( keyboardInputAllowed );
	stream->WriteBool( mouseInputAllowed );
	stream->WriteLong( cutsceneState );
	stream->WriteFloat( unknown5C );
	stream->WriteFloat( unknown60 );
	stream->WriteFloat( unknown64 );
	stream->WriteID( fadeIconID );
	stream->WriteFloat( fadeEndTime );
	stream->WriteFloat( fadeColor.r );
	stream->WriteFloat( fadeColor.g );
	stream->WriteFloat( fadeColor.b );
	stream->WriteFloat( fadeColor.a );
	if ( letterboxTop != NULL ) {
		stream->WriteBool( true );
		letterboxTop->WriteToStream( stream );
	} else {
		stream->WriteBool( false );
	}
	if ( letterboxBottom != NULL ) {
		stream->WriteBool( true );
		letterboxBottom->WriteToStream( stream );
	} else {
		stream->WriteBool( false );
	}
	stream->WriteBool( unknown88 );
	stream->WriteID( lastRenderedSetID );
	return true;
}

/*
================
TopWindow::ReadFromStream

The loaded window becomes the top window.
================
*/
// FUNCTION: STBC 0x0050e7c0
bool TopWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	editMode = stream->ReadBool();
	unknown4D = stream->ReadBool();
	mouseWasLocked = stream->ReadBool();
	optionsMenuDisabled = stream->ReadBool();
	badConnectionText = NULL;
	keyboardInputAllowed = stream->ReadBool();
	mouseInputAllowed = stream->ReadBool();
	cutsceneState = stream->ReadLong();
	unknown5C = stream->ReadFloat();
	unknown60 = stream->ReadFloat();
	unknown64 = stream->ReadFloat();
	letterboxTop = NULL;
	fadeIconID = stream->ReadID();
	fadeEndTime = stream->ReadFloat();
	fadeColor.r = stream->ReadFloat();
	fadeColor.g = stream->ReadFloat();
	fadeColor.b = stream->ReadFloat();
	fadeColor.a = stream->ReadFloat();
	if ( stream->ReadBool() ) {
		letterboxTop = (TGIcon *)LoadObject( stream, 0 );
	}
	letterboxBottom = NULL;
	if ( stream->ReadBool() ) {
		letterboxBottom = (TGIcon *)LoadObject( stream, 0 );
	}
	unknown88 = stream->ReadBool();
	lastRenderedSetID = stream->ReadID();
	topWindow = this;
	badConnectionText = NULL;
	return true;
}

/*
================
TopWindow::FixUpPointers
================
*/
// FUNCTION: STBC 0x0050e8f0
bool TopWindow::FixUpPointers( void ) {
	return TGPane::FixUpPointers() ? true : false;
}

/*
================
TopWindow::PostFixUp
================
*/
// FUNCTION: STBC 0x0050e900
void TopWindow::PostFixUp( void ) {
	TGPane::PostFixUp();
}

/*
================
TopWindow::ForgetTopWindow

Forgets the top window without deleting it.
================
*/
// FUNCTION: STBC 0x0050e910
void TopWindow::ForgetTopWindow( void ) {
	topWindow = NULL;
}
