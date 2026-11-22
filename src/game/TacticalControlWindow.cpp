/*
===============================================================================

	TacticalControlWindow.cpp

	The tactical interface's window: its menus, and the cursor the weapons
	use while the mouse picks a target.

===============================================================================
*/

#include "TacticalControlWindow.h"
#include "BridgeSet.h"
#include "CameraObjectClass.h"
#include "SetManager.h"
#include "ViewScreenNode.h"
#include "ViewScreenObject.h"
#include "NiCamera.h"
#include "NiMath.h"
#include "NiPick.h"
#include "InterfaceModule.h"
#include "MainWindow.h"
#include "PyEmbed.h"
#include "RadarDisplay.h"
#include "ShipDisplay.h"
#include "TacWeaponsCtrl.h"
#include "Game.h"
#include "ShipClass.h"
#include "WeaponsDisplay.h"
#include "STButton.h"
#include "STMenu.h"
#include "STStylizedWindow.h"
#include "STTargetMenu.h"
#include "STTopLevelMenu.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGStream.h"
#include "TGString.h"
#include "TopWindow.h"
#include <iostream>
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005095f0 _$E33
// FUNCTION: STBC 0x00509600 _$E32
// FUNCTION: STBC 0x00509610 _$E37
// FUNCTION: STBC 0x00509620 _$E36
// FUNCTION: STBC 0x00509630 _$E48
// FUNCTION: STBC 0x00509640 _$E47
// FUNCTION: STBC 0x00509650 _$E52
// FUNCTION: STBC 0x00509660 _$E51
// FUNCTION: STBC 0x00509670 _$E56
// FUNCTION: STBC 0x00509680 _$E55
// FUNCTION: STBC 0x00509690 _$E60
// FUNCTION: STBC 0x005096a0 _$E59
// FUNCTION: STBC 0x005096b0 _$E184
// FUNCTION: STBC 0x005096c0 _$E183
// FUNCTION: STBC 0x0050a9a0 _$E126
// FUNCTION: STBC 0x0050a9b0 _$E125
// FUNCTION: STBC 0x0050a9c0 _$E139
// FUNCTION: STBC 0x0050a9d0 _$E138
// FUNCTION: STBC 0x0050a9e0 _$E189
// FUNCTION: STBC 0x0050a9f0 _$E188
// FUNCTION: STBC 0x0050aa00 _$E194
// FUNCTION: STBC 0x0050aa10 _$E193
// FUNCTION: STBC 0x0050aa20 _$E198
// FUNCTION: STBC 0x0050aa30 _$E197
// FUNCTION: STBC 0x0050aa40 _$E202
// FUNCTION: STBC 0x0050aa50 _$E201
// FUNCTION: STBC 0x0050aa60 _$E209
// FUNCTION: STBC 0x0050aa70 _$E208
// FUNCTION: STBC 0x0050aa80 _$E366
// FUNCTION: STBC 0x0050aa90 _$E365
// FUNCTION: STBC 0x0050aaa0 _$E370
// FUNCTION: STBC 0x0050aab0 _$E369
// FUNCTION: STBC 0x0050aac0 _$E84
// FUNCTION: STBC 0x0050aad0 _$E83
// FUNCTION: STBC 0x0050aae0 _$E293
// FUNCTION: STBC 0x0050aaf0 _$E292
// FUNCTION: STBC 0x0050ab00 _$E300
// FUNCTION: STBC 0x0050ab10 _$E299
// FUNCTION: STBC 0x0050ab20 _$E304
// FUNCTION: STBC 0x0050ab30 _$E303
// FUNCTION: STBC 0x0050ab40 _$E308
// FUNCTION: STBC 0x0050ab50 _$E307
// FUNCTION: STBC 0x0050ab60 _$E312
// FUNCTION: STBC 0x0050ab70 _$E311
// FUNCTION: STBC 0x0050ab80 _$E318
// FUNCTION: STBC 0x0050ab90 _$E317
// FUNCTION: STBC 0x0050aba0 _$E374
// FUNCTION: STBC 0x0050abb0 _$E373
// FUNCTION: STBC 0x0050abc0 _$E378
// FUNCTION: STBC 0x0050abd0 _$E377
// FUNCTION: STBC 0x0050abe0 _$E382
// FUNCTION: STBC 0x0050abf0 _$E381
// FUNCTION: STBC 0x0050ac00 _$E386
// FUNCTION: STBC 0x0050ac10 _$E385
// FUNCTION: STBC 0x0050ac20 _$E390
// FUNCTION: STBC 0x0050ac30 _$E389
// FUNCTION: STBC 0x0050ac40 _$E394
// FUNCTION: STBC 0x0050ac50 _$E393
// FUNCTION: STBC 0x0050ac60 _$E398
// FUNCTION: STBC 0x0050ac70 _$E397
// FUNCTION: STBC 0x0050ac80 _$E402
// FUNCTION: STBC 0x0050ac90 _$E401
// FUNCTION: STBC 0x0050aca0 _$E406
// FUNCTION: STBC 0x0050acb0 _$E405
// FUNCTION: STBC 0x0050acc0 _$E410
// FUNCTION: STBC 0x0050acd0 _$E409
// FUNCTION: STBC 0x0050ace0 _$E89
// FUNCTION: STBC 0x0050acf0 _$E88
// FUNCTION: STBC 0x0050ad00 _$E414
// FUNCTION: STBC 0x0050ad10 _$E413
// FUNCTION: STBC 0x0050ad20 _$E94
// FUNCTION: STBC 0x0050ad30 _$E93
// FUNCTION: STBC 0x0050ad40 _$E418
// FUNCTION: STBC 0x0050ad50 _$E417
// FUNCTION: STBC 0x0050ad60 _$E422
// FUNCTION: STBC 0x0050ad70 _$E421
// FUNCTION: STBC 0x0050ad80 _$E426
// FUNCTION: STBC 0x0050ad90 _$E425
// FUNCTION: STBC 0x0050ada0 _$E430
// FUNCTION: STBC 0x0050adb0 _$E429
// FUNCTION: STBC 0x0050adc0 _$E103
// FUNCTION: STBC 0x0050add0 _$E102
// FUNCTION: STBC 0x0050ade0 _$E434
// FUNCTION: STBC 0x0050adf0 _$E433
// FUNCTION: STBC 0x0050ae00 _$E438
// FUNCTION: STBC 0x0050ae10 _$E437
// FUNCTION: STBC 0x0050ae20 _$E442
// FUNCTION: STBC 0x0050ae30 _$E441
// FUNCTION: STBC 0x0050ae40 _$E109
// FUNCTION: STBC 0x0050ae50 _$E108
// FUNCTION: STBC 0x0050ae60 _$E114
// FUNCTION: STBC 0x0050ae70 _$E113
// FUNCTION: STBC 0x0050ae80 _$E144
// FUNCTION: STBC 0x0050ae90 _$E143
// FUNCTION: STBC 0x0050aea0 _$E446
// FUNCTION: STBC 0x0050aeb0 _$E445
// FUNCTION: STBC 0x0050aec0 _$E149
// FUNCTION: STBC 0x0050aed0 _$E148
// FUNCTION: STBC 0x0050aee0 _$E450
// FUNCTION: STBC 0x0050aef0 _$E449
// FUNCTION: STBC 0x0050af00 _$E454
// FUNCTION: STBC 0x0050af10 _$E453
// FUNCTION: STBC 0x0050af20 _$E154
// FUNCTION: STBC 0x0050af30 _$E153
// FUNCTION: STBC 0x0050af40 _$E458
// FUNCTION: STBC 0x0050af50 _$E457
// FUNCTION: STBC 0x0050af60 _$E119
// FUNCTION: STBC 0x0050af70 _$E118
// FUNCTION: STBC 0x0050af80 _$E462
// FUNCTION: STBC 0x0050af90 _$E461
// FUNCTION: STBC 0x0050afa0 _$E466
// FUNCTION: STBC 0x0050afb0 _$E465
// FUNCTION: STBC 0x0050afc0 _$E159
// FUNCTION: STBC 0x0050afd0 _$E158
// FUNCTION: STBC 0x0050afe0 _$E164
// FUNCTION: STBC 0x0050aff0 _$E163
// FUNCTION: STBC 0x0050b000 _$E470
// FUNCTION: STBC 0x0050b010 _$E469
// FUNCTION: STBC 0x0050b020 _$E474
// FUNCTION: STBC 0x0050b030 _$E473
// FUNCTION: STBC 0x0050b040 _$E478
// FUNCTION: STBC 0x0050b050 _$E477
// FUNCTION: STBC 0x0050b060 _$E482
// FUNCTION: STBC 0x0050b070 _$E481
// FUNCTION: STBC 0x0050b080 _$E486
// FUNCTION: STBC 0x0050b090 _$E485
// FUNCTION: STBC 0x0050b0a0 _$E490
// FUNCTION: STBC 0x0050b0b0 _$E489
// FUNCTION: STBC 0x0050b0c0 _$E494
// FUNCTION: STBC 0x0050b0d0 _$E493
// FUNCTION: STBC 0x0050b0e0 _$E498
// FUNCTION: STBC 0x0050b0f0 _$E497
// FUNCTION: STBC 0x0050b100 _$E169
// FUNCTION: STBC 0x0050b110 _$E168
// FUNCTION: STBC 0x0050b120 _$E502
// FUNCTION: STBC 0x0050b130 _$E501
// FUNCTION: STBC 0x0050b140 _$E174
// FUNCTION: STBC 0x0050b150 _$E173
// FUNCTION: STBC 0x0050b160 _$E179
// FUNCTION: STBC 0x0050b170 _$E178
// FUNCTION: STBC 0x0050b1b0 _$E506
// FUNCTION: STBC 0x0050b1c0 _$E505
// FUNCTION: STBC 0x0050b1d0 _$E511
// FUNCTION: STBC 0x0050b1e0 _$E510
// FUNCTION: STBC 0x0050b1f0 _$E515
// FUNCTION: STBC 0x0050b200 _$E514

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x005091e0 _$E529
// FUNCTION: STBC 0x005091c0 _$E530

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00509580 _$E347
// FUNCTION: STBC 0x005095a0 _$E348
// FUNCTION: STBC 0x00509590 _$E349
// FUNCTION: STBC 0x00509570 _$E350
// FUNCTION: STBC 0x005095c0 _$E353
// FUNCTION: STBC 0x005095e0 _$E354
// FUNCTION: STBC 0x005095d0 _$E355
// FUNCTION: STBC 0x005095b0 _$E356

// DATA: STBC 0x0088e9f4
const unsigned int TCW_MOUSE_PICK_FIRE = 0x80000000;

// DATA: STBC 0x009875f8
TacticalControlWindow *TacticalControlWindow::tacticalControlWindow;

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005096e0 _$E517
// FUNCTION: STBC 0x00509700 _$E518
// FUNCTION: STBC 0x005096f0 _$E520
// FUNCTION: STBC 0x005096d0 _$E521
// DATA: STBC 0x00987578
TGEventHandlerTable TacticalControlWindow::classHandlers;

/*
================
TacticalControlWindow::Cast

The object as a TacticalControlWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00509720
TacticalControlWindow *TacticalControlWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TACTICALCONTROLWINDOW_TYPE ) ) {
		return (TacticalControlWindow *)object;
	}
	return NULL;
}

/*
================
TacticalControlWindow::TacticalControlWindow

The one tactical control window: it takes the scripts' interface events, sets
up the interface through its scripts and holds the menus they add.
================
*/
// FUNCTION: STBC 0x00509750 ??0TacticalControlWindow@@QAE@XZ
TacticalControlWindow::TacticalControlWindow( void ) : TGPane( 1.0f, 1.0f ), menuIDs( 2 ) {
	tacticalControlWindow = this;
	shipDisplay = NULL;
	enemyShipDisplay = NULL;
	weaponsDisplay = NULL;
	weaponsControl = NULL;
	targetMenu = NULL;
	radarDisplay = NULL;
	radarToggle = NULL;
	tacticalMenu = NULL;
	SetUnknown4C( -1, 0, false );
	SetUnknown8C( -1, 0, false );
	unknownCC = false;
	ignoreNextMouseKeyEvent = false;
	VirtualSlot82();
	g_kEventManager.AddBroadcastPythonFuncHandler( ET_UI_SWITCH_ICON_GROUPS, this, "Tactical.Interface.TacticalControlWindow.HandleSwitchIconGroups" );
	g_kEventManager.AddBroadcastPythonFuncHandler( ET_UI_RESIZE, this, "Tactical.Interface.TacticalControlWindow.HandleResizeUI" );
	g_kEventManager.AddBroadcastPythonFuncHandler( ET_UI_REPOSITION, this, "Tactical.Interface.TacticalControlWindow.HandleRepositionUI" );
	g_kEventManager.AddBroadcastPythonFuncHandler( ET_ST_MINIMIZE, this, "Tactical.Interface.TacticalControlWindow.HandleMinimized" );
}

/*
================
TacticalControlWindow::GetObjType
================
*/
// FUNCTION: STBC 0x005098a0
int TacticalControlWindow::GetObjType( void ) {
	return TACTICALCONTROLWINDOW_TYPE;
}

/*
================
TacticalControlWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x005098b0
bool TacticalControlWindow::IsTypeOf( int type ) {
	if ( type == TACTICALCONTROLWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
TacticalControlWindow::GetClassName
================
*/
// FUNCTION: STBC 0x00509910
char *TacticalControlWindow::GetClassName( void ) {
	return "TacticalControlWindow";
}

/*
================
TacticalControlWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00509920
char *TacticalControlWindow::GetSwigTypeName( void ) {
	return "_p_TacticalControlWindow";
}

/*
================
TacticalControlWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00509930
char *TacticalControlWindow::GetPythonClassName( void ) {
	return "TacticalControlWindowPtr";
}

/*
================
TacticalControlWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00509940
TGEventHandlerTable *TacticalControlWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TacticalControlWindow::~TacticalControlWindow

The window stops being the one, and the interface events go back.
================
*/
// FUNCTION: STBC 0x00509980 ??1TacticalControlWindow@@UAE@XZ
TacticalControlWindow::~TacticalControlWindow( void ) {
	if ( tacticalControlWindow == this ) {
		tacticalControlWindow = NULL;
	}
	if ( TGEventManager::isAlive == true ) {
		g_kEventManager.RemoveBroadcastHandler( ET_UI_SWITCH_ICON_GROUPS, this, "Tactical.Interface.TacticalControlWindow.HandleSwitchIconGroups" );
		if ( TGEventManager::isAlive == true ) {
			g_kEventManager.RemoveBroadcastHandler( ET_UI_RESIZE, this, "Tactical.Interface.TacticalControlWindow.HandleResizeUI" );
			if ( TGEventManager::isAlive == true ) {
				g_kEventManager.RemoveBroadcastHandler( ET_UI_REPOSITION, this, "Tactical.Interface.TacticalControlWindow.HandleRepositionUI" );
				if ( TGEventManager::isAlive == true ) {
					g_kEventManager.RemoveBroadcastHandler( ET_ST_MINIMIZE, this, "Tactical.Interface.TacticalControlWindow.HandleMinimized" );
				}
			}
		}
	}
}

/*
================
TacticalControlWindow::SetMousePickFire

While the mouse picks the target, a visible window shows the circle cursor;
the trek cursor comes back afterwards.
================
*/
// FUNCTION: STBC 0x00509AC0
void TacticalControlWindow::SetMousePickFire( bool pick ) {
	if ( pick ) {
		flags |= TCW_MOUSE_PICK_FIRE;
		if ( flags & TGUIO_VISIBLE ) {
			PyEmbed::CallFunction( "SystemIcons", "SetCircleCursor", NULL, NULL, NULL );
		}
	} else {
		flags &= ~TCW_MOUSE_PICK_FIRE;
		if ( flags & TGUIO_VISIBLE ) {
			PyEmbed::CallFunction( "SystemIcons", "SetTrekCursor", NULL, NULL, NULL );
		}
	}
}

/*
================
TacticalControlWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00509b20
void TacticalControlWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacticalControlWindow::HandleRadarToggle, "TacticalControlWindow::HandleRadarToggle" );
}

/*
================
TacticalControlWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00509b40
void TacticalControlWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_RADAR_TOGGLE_CLICKED, "TacticalControlWindow::HandleRadarToggle" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "TacticalControlHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "TacticalControlHandlers.HandleKeyboard" );
}

/*
================
TacticalControlWindow::VirtualSlot82

Lets the scripts set up the tactical control interface.
================
*/
// FUNCTION: STBC 0x00509B80
void TacticalControlWindow::VirtualSlot82( void ) {
	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "TacticalControlHandlers", "Initialize", "", NULL, "(O)", self );
	Py_DECREF( self );
	if ( status == -1 ) {
		PyErr_Print();
	}
}

/*
================
TacticalControlWindow::AddMenuToList

Lists a menu by its ID, unless it is listed already.
================
*/
// FUNCTION: STBC 0x00509BD0
void TacticalControlWindow::AddMenuToList( STMenu *menu ) {
	if ( menu != NULL ) {
		long id = menu->GetObjID();
		if ( menuIDs.FindPos( id ) == NULL ) {
			menuIDs.AddTail( id );
		}
	}
}

/*
================
TacticalControlWindow::SetUnknown4C
================
*/
// FUNCTION: STBC 0x00509c70
void TacticalControlWindow::SetUnknown4C( int row, int column, bool value ) {
	if ( row == -1 ) {
		for ( int i = 0; i < 8; i++ ) {
			for ( int j = 0; j < 8; j++ ) {
				unknown4C[i][j] = false;
			}
		}
		return;
	}
	unknown4C[row][column] = value;
}

/*
================
TacticalControlWindow::SetUnknown8C
================
*/
// FUNCTION: STBC 0x00509ca0
void TacticalControlWindow::SetUnknown8C( int row, int column, bool value ) {
	if ( row == -1 ) {
		for ( int i = 0; i < 8; i++ ) {
			for ( int j = 0; j < 8; j++ ) {
				unknown8C[i][j] = false;
			}
		}
		return;
	}
	unknown8C[row][column] = value;
}

/*
================
TacticalControlWindow::Unknown509CE0

While the mouse picks the target: over the window, the cursor turns to the
target cursor when a flag is set in both tables at once, and back to the
circle when none is; then the ray through the cursor, from the active camera
or the view screen's remote camera, picks the target's model, and the
nearest hit becomes the point the player aims at, in the target's model
space.
================
*/
// FUNCTION: STBC 0x00509ce0
// DRAFT: calls and control flow from the asm; retail releases the old camera with DecRefCount inlined where this build calls 0x40CFE0, and lays out the frame differently (viewX/viewY, origin, direction, the pick)
void TacticalControlWindow::Unknown509CE0( ShipClass *player, ObjectClass *target ) {
	if ( !GetMousePickFire() ) {
		unknownCC = false;
		return;
	}
	if ( flags & TGUIO_VISIBLE ) {
		bool targetCursor = false;
		if ( target != NULL ) {
			for ( int row = 0; row < 8 && !targetCursor; row++ ) {
				for ( int column = 0; column < 8 && !targetCursor; column++ ) {
					if ( unknown4C[row][column] && unknown8C[row][column] ) {
						targetCursor = true;
					}
				}
			}
		}
		if ( targetCursor ) {
			if ( !unknownCC ) {
				PyEmbed::CallFunction( "SystemIcons", "SetTargetCursor", NULL, NULL, NULL );
			}
		} else if ( unknownCC ) {
			PyEmbed::CallFunction( "SystemIcons", "SetCircleCursor", NULL, NULL, NULL );
		}
		unknownCC = targetCursor;
	}
	if ( target == NULL ) {
		return;
	}

	SetClass *set = g_kSetManager.GetRenderedSet();
	NiPoint2 cursor = lastMousePosition;
	BridgeSet *bridge = BridgeSet::Cast( set );
	ViewScreenObject *viewScreen = NULL;
	CameraObjectClass *camera;
	if ( bridge != NULL ) {
		viewScreen = bridge->GetViewScreen();
		if ( viewScreen == NULL || !viewScreen->IsOn() ) {
			return;
		}
		camera = viewScreen->GetRemoteCam();
		if ( camera->GetContainingSet() != target->GetContainingSet() ) {
			return;
		}
	} else {
		camera = set->GetActiveCamera();
	}
	if ( camera == NULL ) {
		return;
	}

	NiPointer<NiCamera> niCamera = camera->GetNiCamera();
	if ( viewScreen != NULL ) {
		CameraObjectClass *activeCamera = set->GetActiveCamera();
		niCamera = viewScreen->CreateScreenCamera( activeCamera );
		if ( niCamera == NULL ) {
			return;
		}
		ViewScreenNode *screenNode = viewScreen->GetViewScreenNode();
		screenNode->FitScreenCamera( activeCamera->GetNiCamera(), niCamera );
	}
	float viewX, viewY;
	NiPoint3 origin, direction;
	if ( niCamera != NULL && niCamera->MapBufferPointToViewPoint( cursor.x, 1.0f - cursor.y, viewX, viewY )
		&& niCamera->ViewPointToRay( viewX, viewY, origin, direction ) ) {
		NiPick pick;
		target->VirtualSlot63( &origin, direction, &pick );
		float nearest = NI_INFINITY;
		NiPoint3 point;
		for ( unsigned int i = 0; i < pick.GetResults().GetSize(); i++ ) {
			NiPick::Record *record = pick.GetRecord( i );
			if ( record != NULL && record->GetDistance() < nearest ) {
				point = record->intersection;
				nearest = record->GetDistance();
			}
		}
		if ( nearest != NI_INFINITY ) {
			TGPoint3 offset;
			offset = TGModelUtils::WorldToLocalPoint( target->GetNode(), point );
			player->UseTargetOffsetTG( true, &offset );
		}
	}
}

/*
================
TacticalControlWindow::GotFocus

A window that gets the focus shows itself, unless a cutscene runs or the
cinematic window has the focus.
================
*/
// FUNCTION: STBC 0x0050a090
void TacticalControlWindow::GotFocus( bool layout ) {
	TGPane::GotFocus( false );
	TopWindow *topWindow = TopWindow::GetTopWindow();
	MainWindow *cinematic = topWindow->FindMainWindow( MWT_CINEMATIC );
	if ( topWindow != NULL && !topWindow->IsCutsceneMode() && cinematic != NULL && !cinematic->IsWindowActive() ) {
		SetVisible( layout );
	}
}

/*
================
TacticalControlWindow::HandleRadarToggle

The radar button swaps the enemy ship display for the radar and back, renames
itself to the other view, and moves the target menu and the button by the
difference of their heights.
================
*/
// FUNCTION: STBC 0x0050a0e0
void TacticalControlWindow::HandleRadarToggle( TGEvent *event ) {
	STButton *toggle = radarToggle;
	if ( toggle == NULL ) {
		return;
	}
	ShipDisplay *enemy = enemyShipDisplay;
	RadarDisplay *radar = radarDisplay;
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	bool enemyWasShown = false;
	TGString *name;
	if ( enemy->IsVisible() ) {
		enemy->SetNotVisible( true );
		radar->SetVisible( true );
		enemyWasShown = true;
		name = database->GetString( "Target" );
	} else {
		enemy->SetVisible( true );
		radar->SetNotVisible( true );
		name = database->GetString( "Sensors" );
	}
	if ( name != NULL ) {
		toggle->SetName( name, true );
	}
	g_kLocalizationManager.Unload( database );
	if ( targetMenu != NULL ) {
		float delta = radar->GetHeight() - enemy->GetHeight();
		if ( enemyWasShown == true ) {	// RULE-EXCEPTION: R35, retail compares the flag byte with 1
			targetMenu->Resize( targetMenu->GetWidth(), targetMenu->GetHeight() - delta, true );
			toggle->SetPosition( toggle->GetLeft(), radar->GetTop(), true );
		} else {
			targetMenu->Resize( targetMenu->GetWidth(), targetMenu->GetHeight() + delta, true );
			toggle->SetPosition( toggle->GetLeft(), enemy->GetTop(), true );
		}
	}
}

/*
================
TacticalControlWindow::VirtualSlot83

Refreshes the radar, the weapons display with the player's ship and the
mouse's pick of the player's target.
================
*/
// FUNCTION: STBC 0x0050a200
// DRAFT: 83.8; register pick: retail keeps this in edi and the weapons display in ebx, here the other way round
void TacticalControlWindow::VirtualSlot83( void ) {
	if ( radarDisplay != NULL ) {
		radarDisplay->Update();
	}
	WeaponsDisplay *display = weaponsDisplay;
	ShipClass *player = Game::GetCurrentPlayer();
	if ( display != NULL && player != NULL ) {
		display->Unknown54C200( player );
	}
	if ( player != NULL ) {
		Unknown509CE0( player, player->GetTarget() );
	}
}

/*
================
TacticalControlWindow::SetVisible

A window that shows while the mouse picks the target has the target cursor.
================
*/
// FUNCTION: STBC 0x0050a250
void TacticalControlWindow::SetVisible( bool layout ) {
	TGPane::SetVisible( layout );
	if ( GetMousePickFire() ) {
		unknownCC = false;
		PyEmbed::CallFunction( "SystemIcons", "SetTargetCursor", NULL, NULL, NULL );
	}
	InterfaceModule::DoTheRightThing();
}

/*
================
TacticalControlWindow::SetNotVisible

A window that hides while the mouse picks the target gives the cursor back.
================
*/
// FUNCTION: STBC 0x0050a2a0
void TacticalControlWindow::SetNotVisible( bool layout ) {
	TGPane::SetNotVisible( layout );
	if ( GetMousePickFire() ) {
		unknownCC = false;
		PyEmbed::CallFunction( "SystemIcons", "SetTrekCursor", NULL, NULL, NULL );
	}
	InterfaceModule::DoTheRightThing();
}

/*
================
TacticalControlWindow::FindMenu

The listed menu with the name.
================
*/
// FUNCTION: STBC 0x0050A2F0
STTopLevelMenu *TacticalControlWindow::FindMenu( TGString *name ) {
	NiTListIterator pos = menuIDs.GetHeadPos();
	while ( pos != NULL ) {
		STTopLevelMenu *menu = STTopLevelMenu::Cast( TGObject::GetTGObjectPtr( menuIDs.GetNext( pos ) ) );
		if ( menu != NULL ) {
			TGString menuName( "" );
			menu->GetName( &menuName );
			if ( menuName.Compare( name, true ) == 0 ) {
				return menu;
			}
		}
	}
	return NULL;
}

/*
================
TacticalControlWindow::GetOpenMenu

The listed menu that is open and visible, other than the target menu.
================
*/
// FUNCTION: STBC 0x0050A3C0
STTopLevelMenu *TacticalControlWindow::GetOpenMenu( void ) {
	NiTListIterator pos = menuIDs.GetHeadPos();
	while ( pos != NULL ) {
		STTopLevelMenu *menu = STTopLevelMenu::Cast( TGObject::GetTGObjectPtr( menuIDs.GetNext( pos ) ) );
		if ( menu != NULL && menu->IsOpened() == true && menu->IsVisible() && menu != targetMenu ) {
			return menu;
		}
	}
	return NULL;
}

/*
================
TacticalControlWindow::GetMenuParentPane

The pane of this window that holds the menu with the name.
================
*/
// FUNCTION: STBC 0x0050A420
TGPane *TacticalControlWindow::GetMenuParentPane( TGString *name ) {
	STTopLevelMenu *menu = FindMenu( name );
	if ( menu == NULL ) {
		return NULL;
	}

	TGPane *pane = menu->GetConceptualParent();
	while ( pane->GetConceptualParent() != NULL && pane->GetConceptualParent() != this ) {
		pane = pane->GetConceptualParent();
	}
	return pane;
}

/*
================
TacticalControlWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x0050a470
bool TacticalControlWindow::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( shipDisplay ) );
	stream->WriteID( GetSavedID( enemyShipDisplay ) );
	stream->WriteID( GetSavedID( weaponsDisplay ) );
	stream->WriteID( GetSavedID( weaponsControl ) );
	stream->WriteID( GetSavedID( targetMenu ) );
	stream->WriteID( GetSavedID( radarDisplay ) );
	stream->WriteID( GetSavedID( radarToggle ) );
	stream->WriteID( GetSavedID( tacticalMenu ) );
	stream->WriteInt( menuIDs.GetSize() );
	for ( NiTListIterator pos = menuIDs.GetHeadPos(); pos; ) {
		stream->WriteID( menuIDs.GetNext( pos ) );
	}
	return true;
}

/*
================
TacticalControlWindow::ReadFromStream

The displays and menus are IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x0050a580
bool TacticalControlWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	shipDisplay = (ShipDisplay *)stream->ReadID();
	enemyShipDisplay = (ShipDisplay *)stream->ReadID();
	weaponsDisplay = (WeaponsDisplay *)stream->ReadID();
	weaponsControl = (TacWeaponsCtrl *)stream->ReadID();
	targetMenu = (STTargetMenu *)stream->ReadID();
	radarDisplay = (RadarDisplay *)stream->ReadID();
	radarToggle = (STButton *)stream->ReadID();
	tacticalMenu = (STMenu *)stream->ReadID();
	tacticalControlWindow = this;
	for ( int count = stream->ReadInt(); count > 0; count-- ) {
		menuIDs.AddTail( stream->ReadID() );
	}
	return true;
}

/*
================
TacticalControlWindow::FixUpPointers

Whether every display and menu the stream named has been loaded.
================
*/
// FUNCTION: STBC 0x0050a6a0
bool TacticalControlWindow::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	if ( shipDisplay != NULL && GetTGObjectPtr( (long)shipDisplay ) == NULL ) {
		return false;
	}
	if ( enemyShipDisplay != NULL && GetTGObjectPtr( (long)enemyShipDisplay ) == NULL ) {
		return false;
	}
	if ( weaponsDisplay != NULL && GetTGObjectPtr( (long)weaponsDisplay ) == NULL ) {
		return false;
	}
	if ( weaponsControl != NULL && GetTGObjectPtr( (long)weaponsControl ) == NULL ) {
		return false;
	}
	if ( targetMenu != NULL && GetTGObjectPtr( (long)targetMenu ) == NULL ) {
		return false;
	}
	if ( radarDisplay != NULL && GetTGObjectPtr( (long)radarDisplay ) == NULL ) {
		return false;
	}
	if ( radarToggle != NULL && GetTGObjectPtr( (long)radarToggle ) == NULL ) {
		return false;
	}
	if ( tacticalMenu != NULL && GetTGObjectPtr( (long)tacticalMenu ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TacticalControlWindow::PostFixUp

Turns the IDs into the displays and menus, and drops the listed menus that
are no longer menus.
================
*/
// FUNCTION: STBC 0x0050a790
void TacticalControlWindow::PostFixUp( void ) {
	TGPane::PostFixUp();
	shipDisplay = (ShipDisplay *)GetSavedObject( (long)shipDisplay );
	enemyShipDisplay = (ShipDisplay *)GetSavedObject( (long)enemyShipDisplay );
	weaponsDisplay = (WeaponsDisplay *)GetSavedObject( (long)weaponsDisplay );
	weaponsControl = (TacWeaponsCtrl *)GetSavedObject( (long)weaponsControl );
	targetMenu = (STTargetMenu *)GetSavedObject( (long)targetMenu );
	radarDisplay = (RadarDisplay *)GetSavedObject( (long)radarDisplay );
	radarToggle = (STButton *)GetSavedObject( (long)radarToggle );
	tacticalMenu = (STMenu *)GetSavedObject( (long)tacticalMenu );
	NiTListIterator pos = menuIDs.GetHeadPos();
	while ( pos != NULL ) {
		long id = menuIDs.Get( pos );
		NiTListIterator next = menuIDs.GetNextPos( pos );
		if ( STTopLevelMenu::Cast( GetTGObjectPtr( id ) ) == NULL ) {
			menuIDs.Remove( id );
		}
		pos = next;
	}
}
