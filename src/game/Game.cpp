/*
===============================================================================

	Game.cpp

===============================================================================
*/

#include <string.h>
#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName


#include "Game.h"
#include "Backdrop.h"
#include "BridgeWindow.h"
#include "CameraObjectClass.h"
#include "CDCheckWindow.h"
#include "CinematicWindow.h"
#include "Episode.h"
#include "PlacementObject.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "ReticleWindow.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "ShipDisplay.h"
#include "STMissionLog.h"
#include "SubtitleWindow.h"
#include "TacticalControlWindow.h"
#include "TacticalWindow.h"
#include "LODModelManager.h"
#include "TGEventHandlerTable.h"
#include "TGActionManager.h"
#include "TGBoolEvent.h"
#include "TGEventManager.h"
#include "TGIcon.h"
#include "TGModelManager.h"
#include "TGMovieManager.h"
#include "MapWindow.h"
#include "Mission.h"
#include "ObjectGroup.h"
#include "TGObjPtrEvent.h"
#include "TGSound.h"
#include "TGStringEvent.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "WeaponsDisplay.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00407df0 _$E581
// FUNCTION: STBC 0x00407e10 _$E580

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004058e0 _$E62
// FUNCTION: STBC 0x004058f0 _$E61
// FUNCTION: STBC 0x00405900 _$E225
// FUNCTION: STBC 0x00405910 _$E224
// FUNCTION: STBC 0x00405920 _$E73
// FUNCTION: STBC 0x00405930 _$E72
// FUNCTION: STBC 0x00405940 _$E77
// FUNCTION: STBC 0x00405950 _$E76
// FUNCTION: STBC 0x00405960 _$E81
// FUNCTION: STBC 0x00405970 _$E80
// FUNCTION: STBC 0x00405980 _$E85
// FUNCTION: STBC 0x00405990 _$E84
// FUNCTION: STBC 0x004059a0 _$E202
// FUNCTION: STBC 0x004059b0 _$E201
// FUNCTION: STBC 0x004059c0 _$E144
// FUNCTION: STBC 0x004059d0 _$E143
// FUNCTION: STBC 0x004059e0 _$E157
// FUNCTION: STBC 0x004059f0 _$E156
// FUNCTION: STBC 0x00405a00 _$E207
// FUNCTION: STBC 0x00405a10 _$E206
// FUNCTION: STBC 0x00405a20 _$E230
// FUNCTION: STBC 0x00405a30 _$E229
// FUNCTION: STBC 0x00405a40 _$E211
// FUNCTION: STBC 0x00405a50 _$E210
// FUNCTION: STBC 0x00405a60 _$E234
// FUNCTION: STBC 0x00405a70 _$E233
// FUNCTION: STBC 0x00406020 _$E241
// FUNCTION: STBC 0x00406030 _$E240
// FUNCTION: STBC 0x00406600 _$E413
// FUNCTION: STBC 0x00406610 _$E412
// FUNCTION: STBC 0x004066b0 _$E417
// FUNCTION: STBC 0x004066c0 _$E416
// FUNCTION: STBC 0x00406750 _$E102
// FUNCTION: STBC 0x00406760 _$E101
// FUNCTION: STBC 0x00406c70 _$E295
// FUNCTION: STBC 0x00406c80 _$E294
// FUNCTION: STBC 0x00406f10 _$E302
// FUNCTION: STBC 0x00406f20 _$E301
// FUNCTION: STBC 0x00407560 _$E306
// FUNCTION: STBC 0x00407570 _$E305
// FUNCTION: STBC 0x00407600 _$E310
// FUNCTION: STBC 0x00407610 _$E309
// FUNCTION: STBC 0x00407630 _$E314
// FUNCTION: STBC 0x00407640 _$E313
// FUNCTION: STBC 0x00407650 _$E320
// FUNCTION: STBC 0x00407660 _$E319
// FUNCTION: STBC 0x00407670 _$E421
// FUNCTION: STBC 0x00407680 _$E420
// FUNCTION: STBC 0x00407690 _$E425
// FUNCTION: STBC 0x004076a0 _$E424
// FUNCTION: STBC 0x004076b0 _$E429
// FUNCTION: STBC 0x004076c0 _$E428
// FUNCTION: STBC 0x004076d0 _$E433
// FUNCTION: STBC 0x004076e0 _$E432
// FUNCTION: STBC 0x004076f0 _$E437
// FUNCTION: STBC 0x00407700 _$E436
// FUNCTION: STBC 0x00407710 _$E441
// FUNCTION: STBC 0x00407720 _$E440
// FUNCTION: STBC 0x00407730 _$E445
// FUNCTION: STBC 0x00407740 _$E444
// FUNCTION: STBC 0x00407750 _$E449
// FUNCTION: STBC 0x00407760 _$E448
// FUNCTION: STBC 0x00407770 _$E453
// FUNCTION: STBC 0x00407780 _$E452
// FUNCTION: STBC 0x00407790 _$E457
// FUNCTION: STBC 0x004077a0 _$E456
// FUNCTION: STBC 0x004077b0 _$E107
// FUNCTION: STBC 0x004077c0 _$E106
// FUNCTION: STBC 0x004077d0 _$E461
// FUNCTION: STBC 0x004077e0 _$E460
// FUNCTION: STBC 0x004077f0 _$E112
// FUNCTION: STBC 0x00407800 _$E111
// FUNCTION: STBC 0x00407810 _$E465
// FUNCTION: STBC 0x00407820 _$E464
// FUNCTION: STBC 0x00407830 _$E469
// FUNCTION: STBC 0x00407840 _$E468
// FUNCTION: STBC 0x00407850 _$E473
// FUNCTION: STBC 0x00407860 _$E472
// FUNCTION: STBC 0x00407870 _$E477
// FUNCTION: STBC 0x00407880 _$E476
// FUNCTION: STBC 0x00407890 _$E121
// FUNCTION: STBC 0x004078a0 _$E120
// FUNCTION: STBC 0x004078b0 _$E481
// FUNCTION: STBC 0x004078c0 _$E480
// FUNCTION: STBC 0x004078d0 _$E485
// FUNCTION: STBC 0x004078e0 _$E484
// FUNCTION: STBC 0x004078f0 _$E489
// FUNCTION: STBC 0x00407900 _$E488
// FUNCTION: STBC 0x00407910 _$E127
// FUNCTION: STBC 0x00407920 _$E126
// FUNCTION: STBC 0x00407930 _$E132
// FUNCTION: STBC 0x00407940 _$E131
// FUNCTION: STBC 0x00407950 _$E162
// FUNCTION: STBC 0x00407960 _$E161
// FUNCTION: STBC 0x00407970 _$E493
// FUNCTION: STBC 0x00407980 _$E492
// FUNCTION: STBC 0x00407990 _$E167
// FUNCTION: STBC 0x004079a0 _$E166
// FUNCTION: STBC 0x004079b0 _$E497
// FUNCTION: STBC 0x004079c0 _$E496
// FUNCTION: STBC 0x004079d0 _$E501
// FUNCTION: STBC 0x004079e0 _$E500
// FUNCTION: STBC 0x004079f0 _$E172
// FUNCTION: STBC 0x00407a00 _$E171
// FUNCTION: STBC 0x00407a10 _$E505
// FUNCTION: STBC 0x00407a20 _$E504
// FUNCTION: STBC 0x00407a30 _$E137
// FUNCTION: STBC 0x00407a40 _$E136
// FUNCTION: STBC 0x00407a50 _$E509
// FUNCTION: STBC 0x00407a60 _$E508
// FUNCTION: STBC 0x00407a70 _$E513
// FUNCTION: STBC 0x00407a80 _$E512
// FUNCTION: STBC 0x00407a90 _$E177
// FUNCTION: STBC 0x00407aa0 _$E176
// FUNCTION: STBC 0x00407ab0 _$E182
// FUNCTION: STBC 0x00407ac0 _$E181
// FUNCTION: STBC 0x00407ad0 _$E517
// FUNCTION: STBC 0x00407ae0 _$E516
// FUNCTION: STBC 0x00407af0 _$E521
// FUNCTION: STBC 0x00407b00 _$E520
// FUNCTION: STBC 0x00407b10 _$E525
// FUNCTION: STBC 0x00407b20 _$E524
// FUNCTION: STBC 0x00407b30 _$E529
// FUNCTION: STBC 0x00407b40 _$E528
// FUNCTION: STBC 0x00407b50 _$E533
// FUNCTION: STBC 0x00407b60 _$E532
// FUNCTION: STBC 0x00407b70 _$E537
// FUNCTION: STBC 0x00407b80 _$E536
// FUNCTION: STBC 0x00407b90 _$E541
// FUNCTION: STBC 0x00407ba0 _$E540
// FUNCTION: STBC 0x00407bb0 _$E545
// FUNCTION: STBC 0x00407bc0 _$E544
// FUNCTION: STBC 0x00407bd0 _$E187
// FUNCTION: STBC 0x00407be0 _$E186
// FUNCTION: STBC 0x00407bf0 _$E549
// FUNCTION: STBC 0x00407c00 _$E548
// FUNCTION: STBC 0x00407c10 _$E192
// FUNCTION: STBC 0x00407c20 _$E191
// FUNCTION: STBC 0x00407c30 _$E197
// FUNCTION: STBC 0x00407c40 _$E196
// FUNCTION: STBC 0x00407e20 _$E553
// FUNCTION: STBC 0x00407e30 _$E552
// FUNCTION: STBC 0x00407e40 _$E558
// FUNCTION: STBC 0x00407e50 _$E557
// FUNCTION: STBC 0x00407e60 _$E376
// FUNCTION: STBC 0x00407e70 _$E375
// FUNCTION: STBC 0x00407e80 _$E562
// FUNCTION: STBC 0x00407e90 _$E561
// FUNCTION: STBC 0x00407ea0 _$E566
// FUNCTION: STBC 0x00407eb0 _$E565

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00405860 _$E397
// FUNCTION: STBC 0x00405870 _$E394
// FUNCTION: STBC 0x00405880 _$E396
// FUNCTION: STBC 0x00405890 _$E395
// FUNCTION: STBC 0x004058a0 _$E403
// FUNCTION: STBC 0x004058b0 _$E400
// FUNCTION: STBC 0x004058c0 _$E402
// FUNCTION: STBC 0x004058d0 _$E401

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00405a80 _$E572
// FUNCTION: STBC 0x00405a90 _$E568
// FUNCTION: STBC 0x00405aa0 _$E571
// FUNCTION: STBC 0x00405ab0 _$E569
// DATA: STBC 0x0097e150
TGEventHandlerTable Game::classHandlers;

// DATA: STBC 0x0097e238
Game *Game::currentGame;

// DATA: STBC 0x0097e23c
char *Game::playerHardpointFileName;

// DATA: STBC 0x008d879c
int Game::difficulty = MEDIUM;

// DATA: STBC 0x008d87a0
float Game::difficultyMultipliers[6] = { 1.25f, 1.25f, 1.0f, 1.0f, 0.75f, 0.75f };

// DATA: STBC 0x008d87b8
float Game::defaultDifficultyMultipliers[6] = { 1.25f, 1.25f, 1.0f, 1.0f, 0.75f, 0.75f };

/*
================
Game::Init
================
*/
// FUNCTION: STBC 0x00405ad0
void Game::Init( void ) {
	unknown62 = true;
	currentEpisode = NULL;
	score = 0;
	rating = 0;
	kills = 0;
	torpsFired = 0;
	torpsHit = 0;
	unknown4C = 0.0f;
	lastSavedGame = NULL;
	player = NULL;
	godMode = false;
	missionLoaded = false;
	unknown64 = NULL;
	preLoadDoneEvent = NULL;
	unknown6C = NULL;
}

/*
================
Game::Game

The game a stream loads; the player's group is read with it.
================
*/
// FUNCTION: STBC 0x00405b10 ??0Game@@QAE@J@Z
Game::Game( long id ) : ScriptObject( id ) {
	Init();
	playerGroup = NULL;
}

/*
================
Game::GetObjType
================
*/
// FUNCTION: STBC 0x00405b40
int Game::GetObjType( void ) {
	return GAME_TYPE;
}

/*
================
Game::IsTypeOf
================
*/
// FUNCTION: STBC 0x00405b50
bool Game::IsTypeOf( int type ) {
	if ( type == GAME_TYPE ) {
		return true;
	}
	return ScriptObject::IsTypeOf( type );
}

/*
================
Game::GetClassName
================
*/
// FUNCTION: STBC 0x00405ba0
char *Game::GetClassName( void ) {
	return "Game";
}

/*
================
Game::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00405bb0
char *Game::GetSwigTypeName( void ) {
	return "_p_Game";
}

/*
================
Game::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00405bc0
char *Game::GetPythonClassName( void ) {
	return "GamePtr";
}

/*
================
Game::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00405bd0
TGEventHandlerTable *Game::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
Game::Game

A new game: outside multiplayer the module is made host and client of a
local game. The player's group and the game's windows are made; the
console and the options menu are taken down if they are up, and the
tactical view is hidden behind the bridge.
================
*/
// FUNCTION: STBC 0x00405c10 ??0Game@@QAE@PAD@Z
Game::Game( char *script ) : ScriptObject( script, ET_FIRST_GAME_SCRIPT_EVENT ) {
	if ( g_kUtopiaModule.GetNetwork() == NULL ) {
		TopWindow *topWindow = TopWindow::GetTopWindow();
		g_kUtopiaModule.SetIsClient( true );
		g_kUtopiaModule.SetIsHost( true );
		g_kUtopiaModule.SetMultiplayer( false );
		MainWindow *multiplayerWindow = topWindow->FindMainWindow( MWT_MULTIPLAYER );
		if ( multiplayerWindow != NULL ) {
			topWindow->MoveToBack( multiplayerWindow );
		}
	}
	currentGame = this;
	Init();
	playerGroup = new ObjectGroup;

	TopWindow *topWindow = TopWindow::GetTopWindow();
	BridgeWindow *bridgeWindow = new BridgeWindow;
	TacticalWindow *tacticalWindow = new TacticalWindow;
	MapWindow *mapWindow = new MapWindow;
	CinematicWindow *cinematicWindow = new CinematicWindow;
	STMissionLog *missionLog = new STMissionLog;
	topWindow->AddChild( tacticalWindow );
	topWindow->AddChild( bridgeWindow );
	topWindow->AddChild( mapWindow );
	topWindow->AddChild( cinematicWindow );
	topWindow->AddChild( missionLog, 0.0f, 0.0f, false );
	tacticalWindow->Initialize();
	bridgeWindow->Initialize();
	mapWindow->Initialize();
	cinematicWindow->Initialize();
	if ( topWindow->FindMainWindow( MWT_CONSOLE )->IsVisible() ) {
		topWindow->ToggleConsole();
	}
	if ( topWindow->FindMainWindow( MWT_OPTIONS )->IsVisible() ) {
		topWindow->ToggleOptionsMenu();
	}
	topWindow->MoveToBack( tacticalWindow, false );
	tacticalWindow->SetNotVisible( false );
	ClearMissionTempDir();
	Start( NULL );
}

/*
================
Game::~Game

The windows the game made go with it, and the tactical control window
when nothing holds it; the options menu is brought back up.
================
*/
// FUNCTION: STBC 0x00405ec0
Game::~Game( void ) {
	currentGame = NULL;
	if ( playerGroup != NULL ) {
		delete playerGroup;
	}
	playerGroup = NULL;
	if ( lastSavedGame != NULL ) {
		delete lastSavedGame;
	}

	TacticalControlWindow *tacticalControlWindow = TacticalControlWindow::GetTacticalControlWindow();
	if ( tacticalControlWindow != NULL && tacticalControlWindow->GetParent() == NULL ) {
		delete tacticalControlWindow;
	}

	TopWindow *topWindow = TopWindow::GetTopWindow();
	MainWindow *window = topWindow->FindMainWindow( MWT_BRIDGE );
	if ( window != NULL ) {
		delete window;
	}
	window = topWindow->FindMainWindow( MWT_TACTICAL );
	if ( window != NULL ) {
		delete window;
	}
	window = topWindow->FindMainWindow( MWT_TACTICAL_MAP );
	if ( window != NULL ) {
		delete window;
	}
	window = topWindow->FindMainWindow( MWT_CINEMATIC );
	if ( window != NULL ) {
		delete window;
	}
	if ( STMissionLog::GetMissionLog() != NULL ) {
		topWindow->DeleteChild( STMissionLog::GetMissionLog(), false );
	}
	window = topWindow->FindMainWindow( MWT_OPTIONS );
	if ( window != NULL && !window->IsVisible() ) {
		topWindow->ToggleOptionsMenu();
	}

	ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticleManagerWindow != NULL ) {
		reticleManagerWindow->KillChildren();
	}
	ClearMissionTempDir();

	CameraObjectClass *camera = CameraObjectClass::GetObjectByID( NULL, playerCameraID );
	if ( camera != NULL && camera->GetContainingSet() == NULL ) {
		delete camera;
	}
	if ( playerHardpointFileName != NULL ) {
		delete playerHardpointFileName;
		playerHardpointFileName = NULL;
	}
}

/*
================
Game::Start

The main menu is told, the player's camera is made, and the CD check
runs; CDCheckPassed goes on from there.
================
*/
// FUNCTION: STBC 0x00406040
void Game::Start( TGEvent *event ) {
	initializeEvent = event;
	if ( event != NULL ) {
		event->IncRefCount();
	}
	PyEmbed::CallFunction( "MainMenu.mainmenu", "GameStarted", NULL, NULL, NULL, NULL );
	PyErr_Print();
	PyEmbed::CallFunction( "Camera", "MakePlayerCamera", "i", &playerCameraID, "()" );
	PyErr_Print();
	CDCheckWindow *cdCheckWindow = (CDCheckWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CD_CHECK );
	if ( cdCheckWindow != NULL ) {
		cdCheckWindow->RunCheck( true );
	}
}

/*
================
Game::CDCheckPassed

The script is initialized behind the loading movie (outside
multiplayer), the player's reticle is made, and the viewscreen and the
friendly-fire counts are reset.
================
*/
// FUNCTION: STBC 0x004060c0
void Game::CDCheckPassed( void ) {
	TGEvent *event = initializeEvent;
	initializeEvent = NULL;
	PyEmbed::CallFunction( "MainMenu.mainmenu", "SetupGameFont", NULL, NULL, NULL, NULL );
	PyErr_Print();
	if ( !g_kUtopiaModule.IsMultiplayer() ) {
		TGIcon *cursor = g_kRootWindow.GetMouseCursor();
		if ( cursor != NULL ) {
			cursor->SetNotVisible();
		}
		g_kMovieManager.LoadForcedMovie( "data/Movies/Loading.bik" );
	}
	g_kMovieManager.PlayForcedMovieFrame();
	ScriptObject::Initialize( event );
	g_kMovieManager.PlayForcedMovieFrame();
	if ( event != NULL ) {
		event->DecRefCount();
	}

	ReticleManagerWindow *reticleManagerWindow = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticleManagerWindow != NULL ) {
		reticleManagerWindow->AddChild( new PlayerReticleWindow );
	}
	g_kMovieManager.PlayForcedMovieFrame();
	if ( PyEmbed::CallFunction( "MissionLib", "ResetViewscreen", NULL, NULL, NULL ) == -1 ) {
		PyErr_Print();
	} else {
		g_kUtopiaModule.SetCurrentFriendlyFire( 0.0f );
		g_kUtopiaModule.SetFriendlyTractorTime( 0.0f );
	}
}

/*
================
Game::OnIdle

While an episode preloads, the models load a piece at a time for a
hundredth of a second each frame, with a frame of the loading movie
every tenth pass; the done event is sent when nothing is left.
================
*/
// FUNCTION: STBC 0x004061d0
void Game::OnIdle( void ) {
	ScriptObject::OnIdle();
	if ( currentEpisode != NULL ) {
		currentEpisode->OnIdle();
	}
	if ( preLoadDoneEvent != NULL ) {
		double stopTime = g_kSystemWrapper.GetTimeSinceFrameStart() + 0.01;
		while ( g_kSystemWrapper.GetTimeSinceFrameStart() < stopTime ) {
			static int passes;
			if ( passes++ % 10 == 0 ) {
				g_kMovieManager.PlayForcedMovieFrame();
			}
			if ( !g_kModelManager.DoIncrementalLoad() ) {
				g_kEventManager.AddEvent( preLoadDoneEvent );
				preLoadDoneEvent->DecRefCount();
				preLoadDoneEvent = NULL;
				return;
			}
		}
	} else {
		g_kMovieManager.CloseForcedMovie();
	}
}

/*
================
Game::Terminate

The event is kept for ReallyTerminate.
================
*/
// FUNCTION: STBC 0x004062b0
void Game::Terminate( TGEvent *event ) {
	unknown6C = event;
	ReallyTerminate( NULL );
}

/*
================
Game::ReallyTerminate

The ET_QUIT handler, and Terminate's second stage. The cutscene ends and
the input comes back; a quit event whose value is false only drops the
held termination event. Otherwise the episode, the placements and the
backdrops go, the scripts clear their globals, the script object
terminates with the held event and the models are freed; the main menu
learns the game has ended, and the actions are aborted.
================
*/
// FUNCTION: STBC 0x004062d0
void Game::ReallyTerminate( TGEvent *event ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		topWindow->AbortCutscene();
		topWindow->AllowKeyboardInput( true );
		topWindow->AllowMouseInput( true );
	}
	if ( event != NULL && !( (TGBoolEvent *)event )->GetBool() ) {
		if ( unknown6C != NULL ) {
			unknown6C->DecRefCount();
		}
		unknown6C = NULL;
		return;
	}

	if ( currentEpisode != NULL ) {
		currentEpisode->Terminate( NULL );
		currentEpisode = NULL;
	}
	PlacementObject::DeleteAll();
	Backdrop::DeleteAll();
	PyEmbed::CallFunction( "LoadBridge", "Terminate", NULL, NULL, NULL, NULL );
	PyEmbed::CallFunction( "Camera", "ClearGlobals", NULL, NULL, NULL, NULL );
	ScriptObject::Terminate( unknown6C );
	if ( unknown6C != NULL ) {
		unknown6C->DecRefCount();
	}
	g_kLODModelManager.ForgetAll();
	g_kModelManager.FreeAllModels();

	topWindow = TopWindow::GetTopWindow();
	MainWindow *options = topWindow->FindMainWindow( MWT_OPTIONS );
	NiRender *renderer = UtopiaApp::GetRenderer();
	if ( options != NULL && renderer != NULL ) {
		PyEmbed::CallFunction( "MainMenu.mainmenu", "GameEnded", NULL, NULL, NULL, NULL );
		PyErr_Print();
	}
	SubtitleWindow *subtitles = (SubtitleWindow *)topWindow->FindMainWindow( MWT_SUBTITLE );
	if ( subtitles != NULL ) {
		subtitles->SetPositionForMode( SM_END_CINEMATIC, false );
	}
	g_kTGActionManager.AbortAllActions();
	if ( PyEmbed::CallFunction( "MissionLib", "ResetViewscreen", NULL, NULL, NULL ) == -1 ) {
		PyErr_Print();
	}
}

/*
================
Game::LoadEpisode

An episode of the same name that has loaded only has the event sent. A
different one is terminated with an ET_LOAD_EPISODE event, whose
handler loads the named episode once the old one is gone. One that has
not loaded yet is initialized instead.
================
*/
// FUNCTION: STBC 0x00406460
void Game::LoadEpisode( char *name, TGEvent *doneEvent ) {
	Episode *episode = currentEpisode;
	if ( episode != NULL ) {
		if ( episode->AreAssetsLoaded() == true ) {
			if ( strcmp( episode->GetScript(), name ) == 0 ) {
				if ( doneEvent != NULL ) {
					g_kEventManager.AddEvent( doneEvent );
				}
				return;
			}
			TGStringEvent *event = new TGStringEvent;
			event->SetDestination( this );
			event->SetEventType( ET_LOAD_EPISODE );
			event->SetString( name );
			episode->Terminate( event );
			currentEpisode = NULL;
			unknown64 = doneEvent;
			if ( doneEvent != NULL ) {
				doneEvent->IncRefCount();
			}
			return;
		}
		episode->SetAssetsLoaded( true );
		episode->Initialize( NULL );
		return;
	}

	episode = new Episode( name );
	if ( episode == NULL ) {
		return;
	}
	currentEpisode = episode;
	if ( unknown64 == NULL ) {
		unknown64 = doneEvent;
		if ( doneEvent != NULL ) {
			doneEvent->IncRefCount();
		}
	}
	episode->PreLoadAssets();
	TGStringEvent *event = new TGStringEvent;
	event->SetDestination( this );
	event->SetEventType( ET_LOAD_EPISODE );
	event->SetString( name );
	SetPreLoadDoneEvent( event );
}

/*
================
Game::SetPreLoadDoneEvent
================
*/
// FUNCTION: STBC 0x00406620
void Game::SetPreLoadDoneEvent( TGEvent *event ) {
	if ( event != NULL ) {
		preLoadDoneEvent = event;
		event->IncRefCount();
	}
}

/*
================
Game::SetLastSavedGame
================
*/
// FUNCTION: STBC 0x00406640
void Game::SetLastSavedGame( char *name ) {
	if ( lastSavedGame != NULL ) {
		delete lastSavedGame;
		lastSavedGame = NULL;
	}
	if ( name != NULL ) {
		lastSavedGame = new char[strlen( name ) + 1];
		strcpy( lastSavedGame, name );
	}
}

/*
================
Game::SetPlayer

ET_SET_PLAYER goes from the old ship to the game, naming the new one.
================
*/
// FUNCTION: STBC 0x004066d0
void Game::SetPlayer( ShipClass *ship ) {
	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetEventType( ET_SET_PLAYER );
	event->SetSource( player );
	event->SetDestination( this );
	event->SetObjPtr( ship );
	event->IncRefCount();
	g_kEventManager.ProcessEvent( event );
	event->DecRefCount();
}

/*
================
Game::SetUIShipID
================
*/
// FUNCTION: STBC 0x00406770
void Game::SetUIShipID( void ) {
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	ShipDisplay *shipDisplay = window->GetShipDisplay();
	ShipDisplay *enemyShipDisplay = window->GetEnemyShipDisplay();
	WeaponsDisplay *weaponsDisplay = window->GetWeaponsDisplay();
	ShipClass *ship = ShipClass::Cast( player );
	if ( shipDisplay != NULL ) {
		shipDisplay->SetShipID( ship->GetObjID() );
	}
	if ( weaponsDisplay != NULL ) {
		weaponsDisplay->SetShipID( ship->GetObjID() );
	}
	if ( enemyShipDisplay != NULL ) {
		enemyShipDisplay->AddTargetChangeEvent();
	}
	PyEmbed::CallFunction( "Tactical.Interface.TacticalControlWindow", "Refresh", NULL, NULL, NULL );
}

/*
================
Game::SetDifficulty

Ignored while a single-player game runs.
================
*/
// FUNCTION: STBC 0x004067e0
void Game::SetDifficulty( int difficulty_ ) {
	if ( currentGame == NULL && !g_kUtopiaModule.IsMultiplayer() ) {
		difficulty = difficulty_;
	}
}

/*
================
Game::SetDifficultyReallyIMeanIt

Sets the difficulty during a game too; still not in multiplayer.
================
*/
// FUNCTION: STBC 0x00406800
void Game::SetDifficultyReallyIMeanIt( int difficulty_ ) {
	if ( !g_kUtopiaModule.IsMultiplayer() ) {
		difficulty = difficulty_;
	}
}

/*
================
Game::SetDifficultyMultipliers
================
*/
// FUNCTION: STBC 0x00406820
void Game::SetDifficultyMultipliers( float easyOffensive, float easyDefensive, float mediumOffensive, float mediumDefensive, float hardOffensive, float hardDefensive ) {
	difficultyMultipliers[0] = easyOffensive;
	difficultyMultipliers[1] = easyDefensive;
	difficultyMultipliers[2] = mediumOffensive;
	difficultyMultipliers[3] = mediumDefensive;
	difficultyMultipliers[4] = hardOffensive;
	difficultyMultipliers[5] = hardDefensive;
}

/*
================
Game::SetDefaultDifficultyMultipliers
================
*/
// FUNCTION: STBC 0x00406860
void Game::SetDefaultDifficultyMultipliers( float easyOffensive, float easyDefensive, float mediumOffensive, float mediumDefensive, float hardOffensive, float hardDefensive ) {
	defaultDifficultyMultipliers[0] = easyOffensive;
	defaultDifficultyMultipliers[1] = easyDefensive;
	defaultDifficultyMultipliers[2] = mediumOffensive;
	defaultDifficultyMultipliers[3] = mediumDefensive;
	defaultDifficultyMultipliers[4] = hardOffensive;
	defaultDifficultyMultipliers[5] = hardDefensive;
}

/*
================
Game::ResetDifficultyMultipliers
================
*/
// FUNCTION: STBC 0x004068a0
void Game::ResetDifficultyMultipliers( void ) {
	memcpy( difficultyMultipliers, defaultDifficultyMultipliers, sizeof( difficultyMultipliers ) );
}

/*
================
Game::GetOffensiveDifficultyMultiplier
================
*/
// FUNCTION: STBC 0x004068c0
float Game::GetOffensiveDifficultyMultiplier( void ) {
	switch ( difficulty ) {
		case EASY:
			return difficultyMultipliers[0];
		case MEDIUM:
			return difficultyMultipliers[2];
		case HARD:
			return difficultyMultipliers[4];
	}
	return 1.0f;
}

/*
================
Game::GetDefensiveDifficultyMultiplier
================
*/
// FUNCTION: STBC 0x004068f0
float Game::GetDefensiveDifficultyMultiplier( void ) {
	switch ( difficulty ) {
		case EASY:
			return difficultyMultipliers[1];
		case MEDIUM:
			return difficultyMultipliers[3];
		case HARD:
			return difficultyMultipliers[5];
	}
	return 1.0f;
}

/*
================
Game::SetPlayerHardpointFileName
================
*/
// FUNCTION: STBC 0x00406920
void Game::SetPlayerHardpointFileName( char *name ) {
	if ( playerHardpointFileName != NULL ) {
		delete playerHardpointFileName;
		playerHardpointFileName = NULL;
	}
	if ( name != NULL ) {
		playerHardpointFileName = new char[strlen( name ) + 1];
		strcpy( playerHardpointFileName, name );
	}
}

/*
================
Game::AddPersistentModule

The episode being played keeps the module too.
================
*/
// FUNCTION: STBC 0x00406990
void Game::AddPersistentModule( char *module ) {
	ScriptObject::AddPersistentModule( module );
	if ( currentEpisode != NULL ) {
		currentEpisode->AddPersistentModule( module );
	}
}

/*
================
Game::GetCurrentPlayer
================
*/
// FUNCTION: STBC 0x004069b0
ShipClass *Game::GetCurrentPlayer( void ) {
	if ( currentGame != NULL ) {
		return currentGame->GetPlayer();
	}
	return NULL;
}

/*
================
Game::GetPlayerSet
================
*/
// FUNCTION: STBC 0x004069c0
SetClass *Game::GetPlayerSet( void ) {
	if ( player != NULL ) {
		return player->GetContainingSet();
	}
	return NULL;
}

/*
================
Game::GetPlayerCamera

The camera is made by the Camera script when it is missing.
================
*/
// FUNCTION: STBC 0x004069d0
CameraObjectClass *Game::GetPlayerCamera( void ) {
	CameraObjectClass *camera = CameraObjectClass::GetObjectByID( NULL, playerCameraID );
	if ( camera == NULL ) {
		PyEmbed::CallFunction( "Camera", "MakePlayerCamera", "i", &playerCameraID, "()" );
		PyErr_Print();
		if ( playerCameraID != 0 ) {
			return CameraObjectClass::GetObjectByID( NULL, playerCameraID );
		}
	}
	return camera;
}

/*
================
Game::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00406a30
void Game::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Game::HandleSetPlayer, "Game::HandleSetPlayer" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Game::DestroyedPlayer, "Game::DestroyedPlayer" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Game::LoadEpisodeHandler, "Game::LoadEpisodeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Game::ReallyTerminate, "Game::ReallyTerminate" );
	Episode::RegisterHandlerNames();
	Mission::RegisterHandlerNames();
}

/*
================
Game::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00406a90
void Game::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SET_PLAYER, "Game::HandleSetPlayer" );
	AddMethodHandlerForClass( &classHandlers, ET_LOAD_EPISODE, "Game::LoadEpisodeHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_QUIT, "Game::ReallyTerminate" );
	Episode::RegisterHandlers();
	Mission::RegisterHandlers();
}

/*
================
Game::HandleSetPlayer

The new ship becomes the player's: it is watched for its deletion, the
tactical displays and the bridge are set up for it, and the player's
group holds its name alone.
================
*/
// FUNCTION: STBC 0x00406ae0
void Game::HandleSetPlayer( TGEvent *event ) {
	if ( player != NULL ) {
		g_kEventManager.RemoveBroadcastHandler( ET_DELETE_ME_STAGE_1, this, "Game::DestroyedPlayer", player );
		player = NULL;
	}
	while ( playerGroup->GetNumNames() != 0 ) {
		playerGroup->RemoveName( playerGroup->GetNthName( 0 ) );
	}
	ShipClass *ship = ShipClass::Cast( ( (TGObjPtrEvent *)event )->GetObjPtr() );
	player = ship;
	if ( ship == NULL ) {
		return;
	}
	g_kEventManager.AddBroadcastMethodHandler( ET_DELETE_ME_STAGE_1, this, "Game::DestroyedPlayer", ship );
	SetUIShipID();

	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	if ( bridge != NULL ) {
		PyObject *bridgeObject = bridge->VirtualSlot08();
		PyObject *shipObject = ship->VirtualSlot08();
		PyEmbed::CallFunction( "LoadBridge", "ConfigureForShip", NULL, NULL, "(OO)", bridgeObject, shipObject );
		Py_DECREF( bridgeObject );
		Py_DECREF( shipObject );
	}
	PyObject *shipObject = ship->VirtualSlot08();
	PyEmbed::CallFunction( "Bridge.EngineerMenuHandlers", "ConfigureForShip", NULL, NULL, "(O)", shipObject );
	Py_DECREF( shipObject );
	playerGroup->AddName( player->GetName() );
}

/*
================
Game::DestroyedPlayer
================
*/
// FUNCTION: STBC 0x00406c90
void Game::DestroyedPlayer( TGEvent *event ) {
	player = NULL;
}

/*
================
Game::LoadEpisodeHandler
================
*/
// FUNCTION: STBC 0x00406ca0
void Game::LoadEpisodeHandler( TGEvent *event ) {
	char *name = ( (TGStringEvent *)event )->GetCString();
	LoadEpisode( name, NULL );
	delete name;
}

/*
================
Game::GetNextEventType

A new event type of the current game's; 0 without a game.
================
*/
// FUNCTION: STBC 0x00406cd0
int Game::GetNextEventType( void ) {
	if ( currentGame != NULL ) {
		return currentGame->GetInstanceNextEventType();
	}
	return 0;
}

/*
================
Game::LoadSoundInGroup
================
*/
// FUNCTION: STBC 0x00406cf0
TGSound *Game::LoadSoundInGroup( char *fileName, char *name, char *group, long flags ) {
	TGSound *sound = NULL;
	if ( fileName != NULL && name != NULL ) {
		sound = new TGSound( fileName, name, flags );
		if ( sound != NULL ) {
			sound->SetGroup( group );
		}
	}
	return sound;
}

/*
================
Game::WriteToStream

The tactical window is saved with the game when it is not shown.
================
*/
// FUNCTION: STBC 0x00406d80
bool Game::WriteToStream( TGStream *stream ) {
	if ( !ScriptObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( score );
	stream->WriteLong( rating );
	stream->WriteLong( kills );
	stream->WriteLong( torpsFired );
	stream->WriteLong( torpsHit );
	stream->WriteFloat( unknown4C );
	stream->VirtualSlot11( lastSavedGame );
	stream->WriteID( GetSavedID( player ) );
	stream->WriteID( playerGroup != NULL ? playerGroup->GetObjID() : 0 );
	stream->WriteID( playerCameraID );
	stream->WriteBool( godMode );
	stream->WriteBool( missionLoaded );
	stream->WriteID( GetSavedID( unknown64 ) );
	stream->WriteID( GetSavedID( preLoadDoneEvent ) );
	stream->WriteBool( currentEpisode != NULL );
	if ( currentEpisode != NULL && !currentEpisode->WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( difficulty );
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	if ( window != NULL ) {
		if ( window->GetParent() == NULL ) {
			stream->WriteBool( true );
			window->WriteToStream( stream );
		} else {
			stream->WriteBool( false );
		}
	} else {
		stream->WriteBool( false );
	}
	for ( int i = 0; i < 6; i++ ) {
		stream->WriteFloat( difficultyMultipliers[i] );
		stream->WriteFloat( defaultDifficultyMultipliers[i] );
	}
	stream->VirtualSlot11( playerHardpointFileName );
	return true;
}

/*
================
Game::ReadFromStream

The loaded game becomes the current one. A saved tactical window is
read back as an object of its own.
================
*/
// FUNCTION: STBC 0x00406f30
bool Game::ReadFromStream( TGStream *stream ) {
	if ( !ScriptObject::ReadFromStream( stream ) ) {
		return false;
	}
	score = stream->ReadLong();
	rating = stream->ReadLong();
	kills = stream->ReadLong();
	torpsFired = stream->ReadLong();
	torpsHit = stream->ReadLong();
	unknown4C = stream->ReadFloat();
	lastSavedGame = stream->VirtualSlot10( -1 );
	player = (ShipClass *)stream->ReadID();
	playerGroup = (ObjectGroup *)stream->ReadID();
	playerCameraID = stream->ReadID();
	godMode = stream->ReadBool();
	missionLoaded = stream->ReadBool();
	unknown64 = (TGEvent *)stream->ReadID();
	preLoadDoneEvent = (TGEvent *)stream->ReadID();
	currentEpisode = NULL;
	if ( stream->ReadBool() == true ) {
		currentEpisode = (Episode *)LoadObject( stream, 0 );
	}
	difficulty = stream->ReadInt();
	currentGame = this;
	if ( stream->ReadBool() ) {
		LoadObject( stream, 0 );
	}
	for ( int i = 0; i < 6; i++ ) {
		difficultyMultipliers[i] = stream->ReadFloat();
		defaultDifficultyMultipliers[i] = stream->ReadFloat();
	}
	playerHardpointFileName = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
Game::FixUpPointers
================
*/
// FUNCTION: STBC 0x00407070
bool Game::FixUpPointers( void ) {
	if ( !ScriptObject::FixUpPointers() ) {
		return false;
	}
	if ( player != NULL && GetTGObjectPtr( (long)player ) == NULL ) {
		return false;
	}
	if ( playerGroup != NULL && GetTGObjectPtr( (long)playerGroup ) == NULL ) {
		return false;
	}
	if ( unknown64 != NULL && TGEvent::GetTGEventPtr( (long)unknown64 ) == NULL ) {
		return false;
	}
	if ( preLoadDoneEvent != NULL && TGEvent::GetTGEventPtr( (long)preLoadDoneEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
Game::PostFixUp
================
*/
// FUNCTION: STBC 0x004070f0
void Game::PostFixUp( void ) {
	ScriptObject::PostFixUp();
	player = (ShipClass *)GetSavedObject( (long)player );
	playerGroup = (ObjectGroup *)GetSavedObject( (long)playerGroup );
	unknown64 = TGEvent::GetSavedEvent( (long)unknown64 );
	preLoadDoneEvent = TGEvent::GetSavedEvent( (long)preLoadDoneEvent );
}

/*
================
Game::ClearMissionTempDir

The folder is relative to the current directory. A file that will not
delete gets a message built, which this build does not report.
================
*/
// FUNCTION: STBC 0x00407150
void Game::ClearMissionTempDir( void ) {
	char path[MAX_PATH];
	WIN32_FIND_DATA data;
	char folder[MAX_PATH];
	char pattern[MAX_PATH];
	char currentDir[MAX_PATH];

	if ( GetCurrentDirectory( MAX_PATH, currentDir ) == 0 ) {
		return;
	}
	char *tempFolder = NULL;
	if ( PyEmbed::GetValue( "MissionLib", "TEMP_MISSION_SAVE_FOLDER", "s", &tempFolder ) == -1 ) {
		return;
	}
	strcpy( folder, currentDir );
	strcat( folder, tempFolder );
	strcpy( pattern, folder );
	strcat( pattern, "*.msv" );
	HANDLE find = FindFirstFile( pattern, &data );
	if ( find == INVALID_HANDLE_VALUE ) {
		return;
	}

	strcpy( path, folder );
	strcat( path, data.cFileName );
	if ( !DeleteFile( path ) ) {
		LPSTR message;
		char text[256];
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&message, 0, NULL );
		strcpy( text, "Game::ClearMissionTempDir() unable to delete " );
		strcat( text, data.cFileName );
		strcat( text, ": " );
		strcat( text, message );
		LocalFree( message );
	}
	while ( FindNextFile( find, &data ) ) {
		strcpy( path, folder );
		strcat( path, data.cFileName );
		if ( !DeleteFile( path ) ) {
			LPSTR message;
			char text[256];
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&message, 0, NULL );
			strcpy( text, "Game::ClearMissionTempDir() unable to delete " );
			strcat( text, data.cFileName );
			strcat( text, ": " );
			strcat( text, message );
			LocalFree( message );
		}
	}
	FindClose( find );
}

/*
================
Game::MissionLoaded

The first time, outside multiplayer, the bridge is shown; the splash
screen goes and every pause is undone. The episode's done event is
sent each time.
================
*/
// FUNCTION: STBC 0x00407580
void Game::MissionLoaded( void ) {
	if ( !missionLoaded ) {
		missionLoaded = true;
		if ( !g_kUtopiaModule.IsMultiplayer() ) {
			TopWindow::GetTopWindow()->ForceBridgeVisible();
		}
		PyEmbed::CallFunction( "loadsplash", "RemoveSplashScreen", NULL, NULL, NULL );
		PyErr_Print();
		g_kUtopiaModule.ForceUnpause();
	}
	if ( unknown64 != NULL ) {
		g_kEventManager.AddEvent( unknown64 );
		unknown64->DecRefCount();
		unknown64 = NULL;
	}
}

/*
================
Game::ClearStatics
================
*/
// FUNCTION: STBC 0x00407620
void Game::ClearStatics( void ) {
	currentGame = NULL;
	playerHardpointFileName = NULL;
}
