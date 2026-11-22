/*
===============================================================================

	STWarpButton.cpp

===============================================================================
*/

#include <iostream>
#include "STWarpButton.h"
#include "STMenu.h"
#include "Game.h"
#include "Episode.h"
#include "Mission.h"
#include "ArtificialIntelligence.h"
#include "LoadEpisodeAction.h"
#include "LoadMissionAction.h"
#include "SortedRegionMenu.h"
#include "TGScriptAction.h"
#include "WarpSequence.h"
#include "ShipClass.h"
#include "SetClass.h"
#include "PyEmbed.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGSequence.h"
#include "TGActionManager.h"
#include "TGStream.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00540340 _$E518
// FUNCTION: STBC 0x00540360 _$E517

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0053e500 _$E57
// FUNCTION: STBC 0x0053e510 _$E56
// FUNCTION: STBC 0x0053e520 _$E61
// FUNCTION: STBC 0x0053e530 _$E60
// FUNCTION: STBC 0x0053e540 _$E72
// FUNCTION: STBC 0x0053e550 _$E71
// FUNCTION: STBC 0x0053e5e0 _$E76
// FUNCTION: STBC 0x0053e5f0 _$E75
// FUNCTION: STBC 0x0053eba0 _$E80
// FUNCTION: STBC 0x0053ebb0 _$E79
// FUNCTION: STBC 0x0053eda0 _$E84
// FUNCTION: STBC 0x0053edb0 _$E83
// FUNCTION: STBC 0x0053f580 _$E208
// FUNCTION: STBC 0x0053f590 _$E207
// FUNCTION: STBC 0x0053fb60 _$E150
// FUNCTION: STBC 0x0053fb70 _$E149
// FUNCTION: STBC 0x0053fb80 _$E163
// FUNCTION: STBC 0x0053fb90 _$E162
// FUNCTION: STBC 0x0053fba0 _$E213
// FUNCTION: STBC 0x0053fbb0 _$E212
// FUNCTION: STBC 0x0053fbc0 _$E218
// FUNCTION: STBC 0x0053fbd0 _$E217
// FUNCTION: STBC 0x0053fbe0 _$E222
// FUNCTION: STBC 0x0053fbf0 _$E221
// FUNCTION: STBC 0x0053fc00 _$E226
// FUNCTION: STBC 0x0053fc10 _$E225
// FUNCTION: STBC 0x0053fc20 _$E233
// FUNCTION: STBC 0x0053fc30 _$E232
// FUNCTION: STBC 0x0053fc40 _$E354
// FUNCTION: STBC 0x0053fc50 _$E353
// FUNCTION: STBC 0x0053fc60 _$E358
// FUNCTION: STBC 0x0053fc70 _$E357
// FUNCTION: STBC 0x0053fc80 _$E108
// FUNCTION: STBC 0x0053fc90 _$E107
// FUNCTION: STBC 0x0053fca0 _$E283
// FUNCTION: STBC 0x0053fcb0 _$E282
// FUNCTION: STBC 0x0053fcc0 _$E290
// FUNCTION: STBC 0x0053fcd0 _$E289
// FUNCTION: STBC 0x0053fce0 _$E294
// FUNCTION: STBC 0x0053fcf0 _$E293
// FUNCTION: STBC 0x0053fd00 _$E298
// FUNCTION: STBC 0x0053fd10 _$E297
// FUNCTION: STBC 0x0053fd20 _$E302
// FUNCTION: STBC 0x0053fd30 _$E301
// FUNCTION: STBC 0x0053fd40 _$E308
// FUNCTION: STBC 0x0053fd50 _$E307
// FUNCTION: STBC 0x0053fd60 _$E362
// FUNCTION: STBC 0x0053fd70 _$E361
// FUNCTION: STBC 0x0053fd80 _$E366
// FUNCTION: STBC 0x0053fd90 _$E365
// FUNCTION: STBC 0x0053fda0 _$E370
// FUNCTION: STBC 0x0053fdb0 _$E369
// FUNCTION: STBC 0x0053fdc0 _$E374
// FUNCTION: STBC 0x0053fdd0 _$E373
// FUNCTION: STBC 0x0053fde0 _$E378
// FUNCTION: STBC 0x0053fdf0 _$E377
// FUNCTION: STBC 0x0053fe00 _$E382
// FUNCTION: STBC 0x0053fe10 _$E381
// FUNCTION: STBC 0x0053fe20 _$E386
// FUNCTION: STBC 0x0053fe30 _$E385
// FUNCTION: STBC 0x0053fe40 _$E390
// FUNCTION: STBC 0x0053fe50 _$E389
// FUNCTION: STBC 0x0053fe60 _$E394
// FUNCTION: STBC 0x0053fe70 _$E393
// FUNCTION: STBC 0x0053fe80 _$E398
// FUNCTION: STBC 0x0053fe90 _$E397
// FUNCTION: STBC 0x0053fea0 _$E113
// FUNCTION: STBC 0x0053feb0 _$E112
// FUNCTION: STBC 0x0053fec0 _$E402
// FUNCTION: STBC 0x0053fed0 _$E401
// FUNCTION: STBC 0x0053fee0 _$E118
// FUNCTION: STBC 0x0053fef0 _$E117
// FUNCTION: STBC 0x0053ff00 _$E406
// FUNCTION: STBC 0x0053ff10 _$E405
// FUNCTION: STBC 0x0053ff20 _$E410
// FUNCTION: STBC 0x0053ff30 _$E409
// FUNCTION: STBC 0x0053ff40 _$E414
// FUNCTION: STBC 0x0053ff50 _$E413
// FUNCTION: STBC 0x0053ff60 _$E418
// FUNCTION: STBC 0x0053ff70 _$E417
// FUNCTION: STBC 0x0053ff80 _$E127
// FUNCTION: STBC 0x0053ff90 _$E126
// FUNCTION: STBC 0x0053ffa0 _$E422
// FUNCTION: STBC 0x0053ffb0 _$E421
// FUNCTION: STBC 0x0053ffc0 _$E426
// FUNCTION: STBC 0x0053ffd0 _$E425
// FUNCTION: STBC 0x0053ffe0 _$E430
// FUNCTION: STBC 0x0053fff0 _$E429
// FUNCTION: STBC 0x00540000 _$E133
// FUNCTION: STBC 0x00540010 _$E132
// FUNCTION: STBC 0x00540020 _$E138
// FUNCTION: STBC 0x00540030 _$E137
// FUNCTION: STBC 0x00540040 _$E168
// FUNCTION: STBC 0x00540050 _$E167
// FUNCTION: STBC 0x00540060 _$E434
// FUNCTION: STBC 0x00540070 _$E433
// FUNCTION: STBC 0x00540080 _$E173
// FUNCTION: STBC 0x00540090 _$E172
// FUNCTION: STBC 0x005400a0 _$E438
// FUNCTION: STBC 0x005400b0 _$E437
// FUNCTION: STBC 0x005400c0 _$E442
// FUNCTION: STBC 0x005400d0 _$E441
// FUNCTION: STBC 0x005400e0 _$E178
// FUNCTION: STBC 0x005400f0 _$E177
// FUNCTION: STBC 0x00540100 _$E446
// FUNCTION: STBC 0x00540110 _$E445
// FUNCTION: STBC 0x00540120 _$E143
// FUNCTION: STBC 0x00540130 _$E142
// FUNCTION: STBC 0x00540140 _$E450
// FUNCTION: STBC 0x00540150 _$E449
// FUNCTION: STBC 0x00540160 _$E454
// FUNCTION: STBC 0x00540170 _$E453
// FUNCTION: STBC 0x00540180 _$E183
// FUNCTION: STBC 0x00540190 _$E182
// FUNCTION: STBC 0x005401a0 _$E188
// FUNCTION: STBC 0x005401b0 _$E187
// FUNCTION: STBC 0x005401c0 _$E458
// FUNCTION: STBC 0x005401d0 _$E457
// FUNCTION: STBC 0x005401e0 _$E462
// FUNCTION: STBC 0x005401f0 _$E461
// FUNCTION: STBC 0x00540200 _$E466
// FUNCTION: STBC 0x00540210 _$E465
// FUNCTION: STBC 0x00540220 _$E470
// FUNCTION: STBC 0x00540230 _$E469
// FUNCTION: STBC 0x00540240 _$E474
// FUNCTION: STBC 0x00540250 _$E473
// FUNCTION: STBC 0x00540260 _$E478
// FUNCTION: STBC 0x00540270 _$E477
// FUNCTION: STBC 0x00540280 _$E482
// FUNCTION: STBC 0x00540290 _$E481
// FUNCTION: STBC 0x005402a0 _$E486
// FUNCTION: STBC 0x005402b0 _$E485
// FUNCTION: STBC 0x005402c0 _$E193
// FUNCTION: STBC 0x005402d0 _$E192
// FUNCTION: STBC 0x005402e0 _$E490
// FUNCTION: STBC 0x005402f0 _$E489
// FUNCTION: STBC 0x00540300 _$E198
// FUNCTION: STBC 0x00540310 _$E197
// FUNCTION: STBC 0x00540320 _$E203
// FUNCTION: STBC 0x00540330 _$E202
// FUNCTION: STBC 0x00540370 _$E494
// FUNCTION: STBC 0x00540380 _$E493
// FUNCTION: STBC 0x00540390 _$E499
// FUNCTION: STBC 0x005403a0 _$E498
// FUNCTION: STBC 0x005403b0 _$E503
// FUNCTION: STBC 0x005403c0 _$E502

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0053e480 _$E32
// FUNCTION: STBC 0x0053e490 _$E29
// FUNCTION: STBC 0x0053e4a0 _$E31
// FUNCTION: STBC 0x0053e4b0 _$E30
// FUNCTION: STBC 0x0053e4c0 _$E38
// FUNCTION: STBC 0x0053e4d0 _$E35
// FUNCTION: STBC 0x0053e4e0 _$E37
// FUNCTION: STBC 0x0053e4f0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0053e560 _$E509
// FUNCTION: STBC 0x0053e570 _$E505
// FUNCTION: STBC 0x0053e580 _$E508
// FUNCTION: STBC 0x0053e590 _$E506

// Further static initialisers of the unit.

// DATA: STBC 0x0098a3d0
TGEventHandlerTable STWarpButton::classHandlers;

/*
================
STWarpButton::Cast

The object as a warp button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0053e5b0
STWarpButton *STWarpButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STWARPBUTTON_TYPE ) ) {
		return (STWarpButton *)object;
	}
	return NULL;
}

/*
================
STWarpButton::STWarpButton

A button as wide as its parent that sends the warp event to itself.
================
*/
// FUNCTION: STBC 0x0053e600 ??0STWarpButton@@QAE@PAD@Z
STWarpButton::STWarpButton( char *name ) : STButton( name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	courseMenu = NULL;
	ship = NULL;
	warpSequence = NULL;
	destination = NULL;
	location = NULL;
	unknown114 = NULL;
	placementName = NULL;
	unknown118 = NULL;
	beforeWarpSequence = NULL;
	beforeDuringWarpSequence = NULL;
	duringWarpSequence = NULL;
	afterDuringWarpSequence = NULL;
	afterWarpSequence = NULL;
	useLargeLoadingScreen = false;
	warpTime = 4.0f;
	STButton::SetDisabled( false );

	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_ST_WARP_BUTTON );
	SetActivationEvent( event );
}

/*
================
STWarpButton::STWarpButton
================
*/
// FUNCTION: STBC 0x0053e800 ??0STWarpButton@@QAE@PAVTGString@@@Z
STWarpButton::STWarpButton( TGString *name ) : STButton( name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	courseMenu = NULL;
	ship = NULL;
	warpSequence = NULL;
	destination = NULL;
	location = NULL;
	unknown114 = NULL;
	placementName = NULL;
	unknown118 = NULL;
	beforeWarpSequence = NULL;
	beforeDuringWarpSequence = NULL;
	duringWarpSequence = NULL;
	afterDuringWarpSequence = NULL;
	afterWarpSequence = NULL;
	useLargeLoadingScreen = false;
	warpTime = 4.0f;
	STButton::SetDisabled( false );

	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_ST_WARP_BUTTON );
	SetActivationEvent( event );
}

/*
================
STWarpButton::~STWarpButton

Frees the strings.
================
*/
// FUNCTION: STBC 0x0053e910
STWarpButton::~STWarpButton( void ) {
	if ( location != NULL ) {
		delete location;
	}
	if ( destination != NULL ) {
		delete destination;
	}
	if ( unknown114 != NULL ) {
		delete unknown114;
	}
	if ( placementName != NULL ) {
		delete placementName;
	}
	if ( unknown118 != NULL ) {
		delete unknown118;
	}
}

/*
================
STWarpButton::SetDestination

Sets where the button warps to, and so whether it is usable: not to the
place it is at, and only while its action is gone. The caller
(0x5176A0) hands it the five values of a stored warp destination.
================
*/
// FUNCTION: STBC 0x0053e990
void STWarpButton::SetDestination( char *newDestination, char *name, char *placement, char *otherName, long id ) {
	if ( destination != NULL ) {
		delete destination;
	}
	if ( newDestination != NULL ) {
		if ( location != NULL && strcmp( location, newDestination ) == 0 ) {
			destination = NULL;
			SetDisabled( false );
			return;
		}
		destination = new char[strlen( newDestination ) + 1];
		strcpy( destination, newDestination );
	} else {
		destination = NULL;
	}
	if ( unknown114 != NULL ) {
		delete unknown114;
	}
	unknown114 = NULL;
	if ( name != NULL ) {
		unknown114 = new char[strlen( name ) + 1];
		strcpy( unknown114, name );
	}
	if ( unknown118 != NULL ) {
		delete unknown118;
	}
	unknown118 = NULL;
	if ( otherName != NULL ) {
		unknown118 = new char[strlen( otherName ) + 1];
		strcpy( unknown118, otherName );
	}
	SetPlacementName( placement );
	if ( courseMenu != NULL && destination != NULL ) {
		if ( g_kTGActionManager.FindAction( warpSequence ) == -1 ) {
			SetEnabled( false );
		} else {
			SetDisabled( false );
		}
		destinationButtonID = id;
	} else {
		SetDisabled( false );
	}
}

/*
================
STWarpButton::SetLocation
================
*/
// FUNCTION: STBC 0x0053ebc0
void STWarpButton::SetLocation( char *location ) {
	if ( this->location != NULL ) {
		delete this->location;
	}
	this->location = NULL;
	if ( location != NULL ) {
		this->location = new char[strlen( location ) + 1];
		strcpy( this->location, location );
	}
}

/*
================
STWarpButton::ClearBDASequences

Skips the sequences that are left and forgets them.
================
*/
// FUNCTION: STBC 0x0053ec30
void STWarpButton::ClearBDASequences( void ) {
	if ( beforeWarpSequence != NULL ) {
		beforeWarpSequence->Skip();
		beforeWarpSequence = NULL;
	}
	if ( beforeDuringWarpSequence != NULL ) {
		beforeDuringWarpSequence->Skip();
		beforeDuringWarpSequence = NULL;
	}
	if ( duringWarpSequence != NULL ) {
		duringWarpSequence->Skip();
		duringWarpSequence = NULL;
	}
	if ( afterDuringWarpSequence != NULL ) {
		afterDuringWarpSequence->Skip();
		afterDuringWarpSequence = NULL;
	}
	if ( afterWarpSequence != NULL ) {
		afterWarpSequence->Skip();
		afterWarpSequence = NULL;
	}
}

/*
================
STWarpButton::Unknown53ECB0

Nothing calls it: builds the button's pieces for the name, as wide as the
parent.
================
*/
// FUNCTION: STBC 0x0053ecb0
void STWarpButton::Unknown53ECB0( TGString *name ) {
	STButton::Init( name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR );
}

/*
================
STWarpButton::GetSetFromModule

The set the script module's GetSet function returns.
================
*/
// FUNCTION: STBC 0x0053ecd0
SetClass *STWarpButton::GetSetFromModule( char *moduleName ) {
	PyObject *setObject;
	if ( PyEmbed::CallFunction( moduleName, "GetSet", "O", &setObject, "" ) == -1 ) {
		PyErr_Print();
		return NULL;
	}
	SetClass *set = (SetClass *)PyEmbed::GetObjectPointer( setObject );
	Py_DECREF( setObject );
	return set;
}

/*
================
STWarpButton::InitializeSetModule

Runs the script module's Initialize function, then gets its set.
================
*/
// FUNCTION: STBC 0x0053ed40
SetClass *STWarpButton::InitializeSetModule( char *moduleName ) {
	PyObject *result;
	if ( PyEmbed::CallFunction( moduleName, "Initialize", "O", &result, "" ) == -1 ) {
		PyErr_Print();
		return NULL;
	}
	Py_DECREF( result );
	return GetSetFromModule( moduleName );
}

/*
================
STWarpButton::AddActionBeforeWarp
================
*/
// FUNCTION: STBC 0x0053edc0
void STWarpButton::AddActionBeforeWarp( TGAction *action, float delay ) {
	if ( beforeWarpSequence == NULL ) {
		beforeWarpSequence = new TGSequence;
	}
	beforeWarpSequence->AddAction( action, NULL, delay );
}

/*
================
STWarpButton::AddActionBeforeDuringWarp
================
*/
// FUNCTION: STBC 0x0053ee20
void STWarpButton::AddActionBeforeDuringWarp( TGAction *action, float delay ) {
	if ( beforeDuringWarpSequence == NULL ) {
		beforeDuringWarpSequence = new TGSequence;
	}
	beforeDuringWarpSequence->AddAction( action, NULL, delay );
}

/*
================
STWarpButton::AddActionDuringWarp
================
*/
// FUNCTION: STBC 0x0053ee80
void STWarpButton::AddActionDuringWarp( TGAction *action, float delay ) {
	if ( duringWarpSequence == NULL ) {
		duringWarpSequence = new TGSequence;
	}
	duringWarpSequence->AddAction( action, NULL, delay );
}

/*
================
STWarpButton::AddActionAfterDuringWarp
================
*/
// FUNCTION: STBC 0x0053eee0
void STWarpButton::AddActionAfterDuringWarp( TGAction *action, float delay ) {
	if ( afterDuringWarpSequence == NULL ) {
		afterDuringWarpSequence = new TGSequence;
	}
	afterDuringWarpSequence->AddAction( action, NULL, delay );
}

/*
================
STWarpButton::AddActionAfterWarp
================
*/
// FUNCTION: STBC 0x0053ef40
void STWarpButton::AddActionAfterWarp( TGAction *action, float delay ) {
	if ( afterWarpSequence == NULL ) {
		afterWarpSequence = new TGSequence;
	}
	afterWarpSequence->AddAction( action, NULL, delay );
}

/*
================
STWarpButton::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0053efa0
void STWarpButton::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STWarpButton::ExitedWarpHandler, "STWarpButton::ExitedWarpHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STWarpButton::HandleWarpEvent, "STWarpButton::HandleWarpEvent" );
}

/*
================
STWarpButton::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0053efd0
void STWarpButton::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_EXITED_WARP, "STWarpButton::ExitedWarpHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_WARP_BUTTON, "STWarpButton::HandleWarpEvent" );
}

/*
================
STWarpButton::HandleWarpEvent

The player's ship warps to the destination: the warp sequence gets the
loading screen and text actions, the mission or episode to load, and the
sequences the button collected; the sequence then plays, and the player's
AI becomes the warp AI.
================
*/
// FUNCTION: STBC 0x0053f000
void STWarpButton::HandleWarpEvent( TGEvent *event ) {
	if ( destinationButtonID != 0 ) {
		SortedRegionMenu *menu = (SortedRegionMenu *)TGObject::GetTGObjectPtr( destinationButtonID );
		if ( menu != NULL ) {
			menu->SetWarpToHere();
		}
	}
	if ( destination == NULL ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	if ( GetSetFromModule( destination ) == NULL ) {
		InitializeSetModule( destination );
	}
	warpSequence = new WarpSequence( player, destination, warpTime, placementName );
	warpSequence->SetEventDestination( this );
	if ( unknown118 != NULL ) {
		if ( useLargeLoadingScreen == true ) {
			AddActionDuringWarp( new TGScriptAction( "MissionLib", "ShowLargeLoadingScreen" ), 0.0f );
		}
		Episode *episode = Game::GetCurrentGame()->GetCurrentEpisode();
		if ( episode != NULL && strcmp( episode->GetScript(), unknown118 ) != 0 ) {
			AddActionDuringWarp( new TGScriptAction( "MissionLib", "ShowLoadingText" ), 0.0f );
		}
		AddActionDuringWarp( new LoadEpisodeAction( unknown118 ), 0.1f );
		warpSequence->SetDestinationEpisode( unknown118 );
		if ( useLargeLoadingScreen == true ) {
			AddActionDuringWarp( new TGScriptAction( "MissionLib", "HideLargeLoadingScreen" ), 0.0f );
		}
	} else {
		Mission *mission = Game::GetCurrentGame()->GetCurrentEpisode()->GetCurrentMission();
		if ( unknown114 != NULL ) {
			if ( useLargeLoadingScreen == true ) {
				AddActionDuringWarp( new TGScriptAction( "MissionLib", "ShowLargeLoadingScreen" ), 0.0f );
			}
			if ( mission != NULL && strcmp( mission->GetScript(), unknown114 ) != 0 ) {
				AddActionDuringWarp( new TGScriptAction( "MissionLib", "ShowLoadingText" ), 0.0f );
			}
			AddActionDuringWarp( new LoadMissionAction( unknown114 ), 0.1f );
			warpSequence->SetDestinationMission( unknown114 );
			if ( useLargeLoadingScreen == true ) {
				AddActionDuringWarp( new TGScriptAction( "MissionLib", "HideLargeLoadingScreen" ), 0.0f );
			}
		}
	}
	useLargeLoadingScreen = false;
	if ( beforeWarpSequence != NULL ) {
		warpSequence->AddActionBeforeWarp( beforeWarpSequence, 0.0f );
	}
	if ( beforeDuringWarpSequence != NULL ) {
		if ( duringWarpSequence != NULL ) {
			beforeDuringWarpSequence->AppendAction( duringWarpSequence, 0.0f );
		}
		if ( afterDuringWarpSequence != NULL ) {
			beforeDuringWarpSequence->AppendAction( afterDuringWarpSequence, 0.0f );
		}
		warpSequence->AddActionDuringWarp( beforeDuringWarpSequence, 0.0f );
	} else {
		if ( afterDuringWarpSequence != NULL ) {
			if ( duringWarpSequence == NULL ) {
				duringWarpSequence = new TGSequence;
			}
			duringWarpSequence->AppendAction( afterDuringWarpSequence, 0.0f );
		}
		if ( duringWarpSequence != NULL ) {
			warpSequence->AddActionDuringWarp( duringWarpSequence, 0.0f );
		}
	}
	if ( afterWarpSequence != NULL ) {
		warpSequence->AddActionAfterWarp( afterWarpSequence, 0.0f );
	}
	beforeWarpSequence = NULL;
	beforeDuringWarpSequence = NULL;
	duringWarpSequence = NULL;
	afterDuringWarpSequence = NULL;
	afterWarpSequence = NULL;

	PyObject *ai = NULL;
	PyObject *pyPlayer = player->VirtualSlot08();
	PyObject *pySequence = warpSequence->VirtualSlot08();
	int status = PyEmbed::CallFunction( "AI.Player.PlayerWarp", "CreateAI", "O", &ai, "(OO)", pyPlayer, pySequence );
	Py_DECREF( pyPlayer );
	Py_DECREF( pySequence );
	if ( status != -1 ) {
		ArtificialIntelligence *warpAI = NULL;
		PyEmbed::Unknown6F98E0( ai, (void **)&warpAI, "ArtificialIntelligence" );
		Py_DECREF( ai );
		if ( warpAI != NULL ) {
			PyObject *pyAI = warpAI->VirtualSlot15();
			PyEmbed::CallFunction( "MissionLib", "SetPlayerAI", "", NULL, "sO", "Helm", pyAI );
			Py_DECREF( pyAI );
		}
	}
	PyErr_Print();
	CallNextHandler( event );
}

/*
================
STWarpButton::ExitedWarpHandler

The ship has left warp: the button's location is the set the ship is in.
A destination that is that very set is dropped and the button disabled;
the stored names are forgotten either way.
================
*/
// FUNCTION: STBC 0x0053f5a0
void STWarpButton::ExitedWarpHandler( TGEvent *event ) {
	if ( location != NULL ) {
		delete location;
		location = NULL;
	}
	ShipClass *ship = Game::GetCurrentPlayer();
	if ( ship != NULL ) {
		if ( this->ship != NULL ) {
			ship = (ShipClass *)this->ship;
		}
		SetClass *set = ship->GetContainingSet();
		if ( set != NULL ) {
			location = new char[strlen( set->GetRegionModule() ) + 1];
			strcpy( location, set->GetRegionModule() );
		}
		if ( destination != NULL && location != NULL && strcmp( destination, location ) == 0 ) {
			delete destination;
			destination = NULL;
			SetDisabled( true );
		}
		if ( destination != NULL ) {
			SetEnabled( true );
		} else if ( unknown114 != NULL ) {
			delete unknown114;
		}
		if ( unknown118 != NULL ) {
			delete unknown118;
			unknown118 = NULL;
		}
		// BUG: with a destination left, unknown114 is forgotten here without having been freed.
		unknown114 = NULL;
		warpSequence = NULL;
	}
}

/*
================
STWarpButton::SetPlacementName
================
*/
// FUNCTION: STBC 0x0053f6e0
void STWarpButton::SetPlacementName( char *placementName ) {
	if ( this->placementName != NULL ) {
		delete this->placementName;
		this->placementName = NULL;
	}
	if ( placementName != NULL ) {
		this->placementName = new char[strlen( placementName ) + 1];
		strcpy( this->placementName, placementName );
	}
}

/*
================
STWarpButton::WriteToStream

The objects and the sequences are saved as their IDs.
================
*/
// FUNCTION: STBC 0x0053f750
bool STWarpButton::WriteToStream( TGStream *stream ) {
	if ( !STButton::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( unknown114 );
	stream->VirtualSlot11( location );
	stream->VirtualSlot11( destination );
	stream->VirtualSlot11( unknown118 );
	stream->VirtualSlot11( placementName );
	stream->WriteBool( useLargeLoadingScreen );
	stream->WriteID( destinationButtonID );
	stream->WriteID( GetSavedID( warpSequence ) );
	stream->WriteID( GetSavedID( courseMenu ) );
	stream->WriteID( GetSavedID( ship ) );
	stream->WriteFloat( warpTime );
	stream->WriteID( GetSavedID( beforeWarpSequence ) );
	stream->WriteID( GetSavedID( beforeDuringWarpSequence ) );
	stream->WriteID( GetSavedID( duringWarpSequence ) );
	stream->WriteID( GetSavedID( afterDuringWarpSequence ) );
	stream->WriteID( GetSavedID( afterWarpSequence ) );
	return true;
}

/*
================
STWarpButton::ReadFromStream

Keeps the IDs in the pointers until PostFixUp.
================
*/
// FUNCTION: STBC 0x0053f8c0
bool STWarpButton::ReadFromStream( TGStream *stream ) {
	if ( !STButton::ReadFromStream( stream ) ) {
		return false;
	}
	unknown114 = stream->VirtualSlot10( -1 );
	location = stream->VirtualSlot10( -1 );
	destination = stream->VirtualSlot10( -1 );
	unknown118 = stream->VirtualSlot10( -1 );
	placementName = stream->VirtualSlot10( -1 );
	useLargeLoadingScreen = stream->ReadBool();
	destinationButtonID = stream->ReadID();
	warpSequence = (WarpSequence *)stream->ReadID();
	courseMenu = (TGObject *)stream->ReadID();
	ship = (TGObject *)stream->ReadID();
	warpTime = stream->ReadFloat();
	beforeWarpSequence = (TGSequence *)stream->ReadID();
	beforeDuringWarpSequence = (TGSequence *)stream->ReadID();
	duringWarpSequence = (TGSequence *)stream->ReadID();
	afterDuringWarpSequence = (TGSequence *)stream->ReadID();
	afterWarpSequence = (TGSequence *)stream->ReadID();
	return true;
}

/*
================
STWarpButton::FixUpPointers

Fails until the objects the stream named are loaded.
================
*/
// FUNCTION: STBC 0x0053f9e0
bool STWarpButton::FixUpPointers( void ) {
	if ( !STButton::FixUpPointers() ) {
		return false;
	}
	TGObject *objects[8] = { warpSequence, courseMenu, ship, beforeWarpSequence, beforeDuringWarpSequence, duringWarpSequence, afterDuringWarpSequence, afterWarpSequence };
	// BUG: the loop stops one object short of the table, so the last sequence is never checked.
	for ( int i = 0; i < 7; i++ ) {
		if ( objects[i] != NULL && GetTGObjectPtr( (long)objects[i] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
STWarpButton::PostFixUp

Replaces the IDs with the objects.
================
*/
// FUNCTION: STBC 0x0053fa80
void STWarpButton::PostFixUp( void ) {
	STButton::PostFixUp();
	warpSequence = (WarpSequence *)GetSavedObject( (long)warpSequence );
	courseMenu = GetSavedObject( (long)courseMenu );
	ship = GetSavedObject( (long)ship );
	beforeWarpSequence = (TGSequence *)GetSavedObject( (long)beforeWarpSequence );
	beforeDuringWarpSequence = (TGSequence *)GetSavedObject( (long)beforeDuringWarpSequence );
	duringWarpSequence = (TGSequence *)GetSavedObject( (long)duringWarpSequence );
	afterDuringWarpSequence = (TGSequence *)GetSavedObject( (long)afterDuringWarpSequence );
	afterWarpSequence = (TGSequence *)GetSavedObject( (long)afterWarpSequence );
}
