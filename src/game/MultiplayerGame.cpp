/*
===============================================================================

	MultiplayerGame.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "MultiplayerGame.h"
#include "CloakingSubsystem.h"
#include "DamageableObject.h"
#include "Episode.h"
#include "GameSpy.h"
#include "Mission.h"
#include "PyEmbed.h"
#include "HullClass.h"
#include "MainWindow.h"
#include "MetaVolume.h"
#include "MultiplayerWindow.h"
#include "NetFile.h"
#include "PhaserSystem.h"
#include "PhysicsObjectClass.h"
#include "ProximityManager.h"
#include "PulseWeapon.h"
#include "RepairSubsystem.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "ShipSubsystem.h"
#include "SubtitleAction.h"
#include "TGBootPlayerMessage.h"
#include "TGBufferStream.h"
#include "TGConfigMapping.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "NetworkAI.h"
#include "CollisionEvent.h"
#include "ObjectExplodingEvent.h"
#include "TGMessage.h"
#include "TGMessageEvent.h"
#include "TGNetPlayer.h"
#include "TGNetwork.h"
#include "TGPlayerEvent.h"
#include "TGSequence.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TopWindow.h"
#include "TorpedoTube.h"
#include "Torpedo.h"
#include "TorpedoSystem.h"
#include "UtopiaApp.h"
#include "UtopiaModule.h"
#include "WarpEngineSubsystem.h"
#include "WeaponSystem.h"
#include "NiSpotLight.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006a3040 _$E551
// FUNCTION: STBC 0x006a3060 _$E550

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0069e0b0 _$E32
// FUNCTION: STBC 0x0069e0c0 _$E29
// FUNCTION: STBC 0x0069e0d0 _$E31
// FUNCTION: STBC 0x0069e0e0 _$E30
// FUNCTION: STBC 0x0069e0f0 _$E38
// FUNCTION: STBC 0x0069e100 _$E35
// FUNCTION: STBC 0x0069e110 _$E37
// FUNCTION: STBC 0x0069e120 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0069e130 _$E86
// FUNCTION: STBC 0x0069e140 _$E85
// FUNCTION: STBC 0x0069e150 _$E317
// FUNCTION: STBC 0x0069e160 _$E316
// FUNCTION: STBC 0x0069e170 _$E97
// FUNCTION: STBC 0x0069e180 _$E96
// FUNCTION: STBC 0x0069e190 _$E101
// FUNCTION: STBC 0x0069e1a0 _$E100
// FUNCTION: STBC 0x0069e1b0 _$E105
// FUNCTION: STBC 0x0069e1c0 _$E104
// FUNCTION: STBC 0x0069e1d0 _$E109
// FUNCTION: STBC 0x0069e1e0 _$E108
// FUNCTION: STBC 0x0069e1f0 _$E224
// FUNCTION: STBC 0x0069e200 _$E223
// FUNCTION: STBC 0x0069e210 _$E166
// FUNCTION: STBC 0x0069e220 _$E165
// FUNCTION: STBC 0x0069e230 _$E179
// FUNCTION: STBC 0x0069e240 _$E178
// FUNCTION: STBC 0x0069e250 _$E229
// FUNCTION: STBC 0x0069e260 _$E228
// FUNCTION: STBC 0x0069e270 _$E322
// FUNCTION: STBC 0x0069e280 _$E321
// FUNCTION: STBC 0x0069e290 _$E244
// FUNCTION: STBC 0x0069e2a0 _$E243
// FUNCTION: STBC 0x0069e2b0 _$E326
// FUNCTION: STBC 0x0069e2c0 _$E325
// FUNCTION: STBC 0x0069e2d0 _$E333
// FUNCTION: STBC 0x0069e2e0 _$E332
// FUNCTION: STBC 0x0069e2f0 _$E403
// FUNCTION: STBC 0x0069e300 _$E402
// FUNCTION: STBC 0x0069e310 _$E407
// FUNCTION: STBC 0x0069e320 _$E406
// FUNCTION: STBC 0x0069e330 _$E128
// FUNCTION: STBC 0x0069e340 _$E127
// FUNCTION: STBC 0x0069e350 _$E260
// FUNCTION: STBC 0x0069e360 _$E259
// FUNCTION: STBC 0x0069e370 _$E267
// FUNCTION: STBC 0x0069e380 _$E266
// FUNCTION: STBC 0x0069e390 _$E271
// FUNCTION: STBC 0x0069e3a0 _$E270
// FUNCTION: STBC 0x0069e3b0 _$E275
// FUNCTION: STBC 0x0069e3c0 _$E274
// FUNCTION: STBC 0x0069e3d0 _$E279
// FUNCTION: STBC 0x0069e3e0 _$E278
// FUNCTION: STBC 0x0069e3f0 _$E285
// FUNCTION: STBC 0x0069e400 _$E284
// FUNCTION: STBC 0x0069e410 _$E233
// FUNCTION: STBC 0x0069e420 _$E232
// FUNCTION: STBC 0x0069e430 _$E237
// FUNCTION: STBC 0x0069e440 _$E236
// FUNCTION: STBC 0x0069e450 _$E411
// FUNCTION: STBC 0x0069e460 _$E410
// FUNCTION: STBC 0x0069e470 _$E415
// FUNCTION: STBC 0x0069e480 _$E414
// FUNCTION: STBC 0x0069e490 _$E419
// FUNCTION: STBC 0x0069e4a0 _$E418
// FUNCTION: STBC 0x0069e4b0 _$E423
// FUNCTION: STBC 0x0069e4c0 _$E422
// FUNCTION: STBC 0x0069e4d0 _$E427
// FUNCTION: STBC 0x0069e4e0 _$E426
// FUNCTION: STBC 0x0069e4f0 _$E431
// FUNCTION: STBC 0x0069e500 _$E430
// FUNCTION: STBC 0x0069eb70 _$E435
// FUNCTION: STBC 0x0069eb80 _$E434
// FUNCTION: STBC 0x0069eb90 _$E439
// FUNCTION: STBC 0x0069eba0 _$E438
// FUNCTION: STBC 0x0069eda0 _$E133
// FUNCTION: STBC 0x0069edb0 _$E132
// FUNCTION: STBC 0x0069ef50 _$E443
// FUNCTION: STBC 0x0069ef60 _$E442
// FUNCTION: STBC 0x0069f230 _$E138
// FUNCTION: STBC 0x0069f240 _$E137
// FUNCTION: STBC 0x0069f280 _$E447
// FUNCTION: STBC 0x0069f290 _$E446
// FUNCTION: STBC 0x0069f5e0 _$E451
// FUNCTION: STBC 0x0069f5f0 _$E450
// FUNCTION: STBC 0x0069f600 _$E455
// FUNCTION: STBC 0x0069f610 _$E454
// FUNCTION: STBC 0x0069f840 _$E459
// FUNCTION: STBC 0x0069f850 _$E458
// FUNCTION: STBC 0x0069f860 _$E143
// FUNCTION: STBC 0x0069f870 _$E142
// FUNCTION: STBC 0x0069fb90 _$E463
// FUNCTION: STBC 0x0069fba0 _$E462
// FUNCTION: STBC 0x0069fd80 _$E467
// FUNCTION: STBC 0x0069fd90 _$E466
// FUNCTION: STBC 0x0069ff30 _$E471
// FUNCTION: STBC 0x0069ff40 _$E470
// FUNCTION: STBC 0x006a0060 _$E149
// FUNCTION: STBC 0x006a0070 _$E148
// FUNCTION: STBC 0x006a0470 _$E154
// FUNCTION: STBC 0x006a0480 _$E153
// FUNCTION: STBC 0x006a07b0 _$E184
// FUNCTION: STBC 0x006a07c0 _$E183
// FUNCTION: STBC 0x006a1e50 _$E389
// FUNCTION: STBC 0x006a1e60 _$E388
// FUNCTION: STBC 0x006a2430 _$E189
// FUNCTION: STBC 0x006a2440 _$E188
// FUNCTION: STBC 0x006a2450 _$E475
// FUNCTION: STBC 0x006a2460 _$E474
// FUNCTION: STBC 0x006a2670 _$E479
// FUNCTION: STBC 0x006a2680 _$E478
// FUNCTION: STBC 0x006a2810 _$E194
// FUNCTION: STBC 0x006a2820 _$E193
// FUNCTION: STBC 0x006a2a20 _$E393
// FUNCTION: STBC 0x006a2a30 _$E392
// FUNCTION: STBC 0x006a2b70 _$E159
// FUNCTION: STBC 0x006a2b80 _$E158
// FUNCTION: STBC 0x006a2b90 _$E483
// FUNCTION: STBC 0x006a2ba0 _$E482
// FUNCTION: STBC 0x006a2bb0 _$E487
// FUNCTION: STBC 0x006a2bc0 _$E486
// FUNCTION: STBC 0x006a2bd0 _$E199
// FUNCTION: STBC 0x006a2be0 _$E198
// FUNCTION: STBC 0x006a2bf0 _$E204
// FUNCTION: STBC 0x006a2c00 _$E203
// FUNCTION: STBC 0x006a2c10 _$E491
// FUNCTION: STBC 0x006a2c20 _$E490
// FUNCTION: STBC 0x006a2c30 _$E495
// FUNCTION: STBC 0x006a2c40 _$E494
// FUNCTION: STBC 0x006a2c50 _$E499
// FUNCTION: STBC 0x006a2c60 _$E498
// FUNCTION: STBC 0x006a2c70 _$E503
// FUNCTION: STBC 0x006a2c80 _$E502
// FUNCTION: STBC 0x006a2c90 _$E507
// FUNCTION: STBC 0x006a2ca0 _$E506
// FUNCTION: STBC 0x006a2cb0 _$E511
// FUNCTION: STBC 0x006a2cc0 _$E510
// FUNCTION: STBC 0x006a2cd0 _$E515
// FUNCTION: STBC 0x006a2ce0 _$E514
// FUNCTION: STBC 0x006a2cf0 _$E519
// FUNCTION: STBC 0x006a2d00 _$E518
// FUNCTION: STBC 0x006a2d10 _$E209
// FUNCTION: STBC 0x006a2d20 _$E208
// FUNCTION: STBC 0x006a2d30 _$E523
// FUNCTION: STBC 0x006a2d40 _$E522
// FUNCTION: STBC 0x006a2d50 _$E214
// FUNCTION: STBC 0x006a2d60 _$E213
// FUNCTION: STBC 0x006a2d70 _$E219
// FUNCTION: STBC 0x006a2d80 _$E218
// FUNCTION: STBC 0x006a2eb0 _$E527
// FUNCTION: STBC 0x006a2ec0 _$E526
// FUNCTION: STBC 0x006a2ed0 _$E532
// FUNCTION: STBC 0x006a2ee0 _$E531
// FUNCTION: STBC 0x006a2ef0 _$E536
// FUNCTION: STBC 0x006a2f00 _$E535

// UtopiaApp.h brings in windows.h, whose GetMessage macro would rename
// TGMessageEvent::GetMessage.
#undef GetMessage

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0069e510 _$E542
// FUNCTION: STBC 0x0069e520 _$E538
// FUNCTION: STBC 0x0069e530 _$E541
// FUNCTION: STBC 0x0069e540 _$E539
// DATA: STBC 0x00994788
TGEventHandlerTable MultiplayerGame::classHandlers;

/*
================
MultiplayerGame::IsTypeOf

Game's test is expanded here, and ScriptObject's inside it.
================
*/
// FUNCTION: STBC 0x004428a0 ?IsTypeOf@MultiplayerGame@@UAE_NH@Z
bool MultiplayerGame::IsTypeOf( int type ) {
	if ( type == MULTIPLAYERGAME_TYPE ) {
		return true;
	}
	if ( type == GAME_TYPE ) {
		return true;
	}
	return ScriptObject::IsTypeOf( type );
}

/*
================
MultiplayerGame::Cast

The object as a MultiplayerGame; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0069e560
MultiplayerGame *MultiplayerGame::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( MULTIPLAYERGAME_TYPE ) ) {
		return (MultiplayerGame *)object;
	}
	return NULL;
}

/*
================
MultiplayerGame::MultiplayerGame

The game takes the network's events and the players' ship events. The
host also makes the groups its messages go to ("NoMe", every player but
the host, and "Forward"), takes the events only it acts on and, when it
plays itself, takes the first place and announces itself.
================
*/
// FUNCTION: STBC 0x0069e590
MultiplayerGame::MultiplayerGame( char *script, int maxPlayers_ ) : Game( script ) {
	if ( maxPlayers_ > MAX_PLAYERS ) {
		maxPlayers_ = MAX_PLAYERS;
	}
	maxPlayers = maxPlayers_;

	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_MESSAGE_EVENT, this, "MultiplayerGame :: ReceiveMessageHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_DISCONNECT_EVENT, this, "MultiplayerGame :: DisconnectHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_NEW_PLAYER, this, "MultiplayerGame :: NewPlayerHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_DELETE_PLAYER, this, "MultiplayerGame :: DeletePlayerHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_CREATED, this, "MultiplayerGame :: ObjectCreatedHandler" );

	if ( g_kUtopiaModule.IsHost() ) {
		ProximityManager::multiplayerPlayerCollisionsEnabled = ProximityManager::playerCollisionsEnabled;
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		if ( network != NULL ) {
			TGNetGroup *group = new TGNetGroup;
			group->SetName( "NoMe" );
			network->AddGroup( group );
			group = new TGNetGroup;
			group->SetName( "Forward" );
			network->AddGroup( group );
		}
		g_kEventManager.AddBroadcastMethodHandler( ET_ADD_TO_REPAIR_LIST, this, "MultiplayerGame :: HostEventHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_REPAIR_COMPLETED, this, "MultiplayerGame :: HostEventHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_REPAIR_CANNOT_BE_COMPLETED, this, "MultiplayerGame :: HostEventHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_SYSTEM_CHECKSUM_COMPLETE, this, "MultiplayerGame::SystemChecksumPassedHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_SYSTEM_CHECKSUM_FAILED, this, "MultiplayerGame::SystemChecksumFailedHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_CHECKSUM_COMPLETE, this, "MultiplayerGame :: ChecksumCompleteHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "MultiplayerGame :: EnterSetHandler" );
		g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_WARP, this, "MultiplayerGame :: ExitedWarpHandler" );
	}

	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_EXPLODING, this, "MultiplayerGame :: ObjectExplodingHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NEW_PLAYER_IN_GAME, this, "MultiplayerGame :: NewPlayerInGameHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_START_FIRING_NOTIFY, this, "MultiplayerGame :: StartFiringHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_STOP_FIRING_NOTIFY, this, "MultiplayerGame :: StopFiringHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_STOP_FIRING_AT_TARGET_NOTIFY, this, "MultiplayerGame :: StopFiringAtTargetHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_STATE_CHANGED_NOTIFY, this, "MultiplayerGame :: SubsystemStateChangedHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_REPAIR_INCREASE_PRIORITY, this, "MultiplayerGame :: RepairListPriorityHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PHASER_LEVEL, this, "MultiplayerGame :: SetPhaserLevelHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_START_CLOAKING_NOTIFY, this, "MultiplayerGame :: StartCloakingHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_STOP_CLOAKING_NOTIFY, this, "MultiplayerGame :: StopCloakingHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_START_WARP_NOTIFY, this, "MultiplayerGame :: StartWarpHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NET_TORPEDO_TYPE_CHANGED_NOTIFY, this, "MultiplayerGame :: TorpedoTypeChangedHandler" );
	if ( !g_kUtopiaModule.IsHost() ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "MultiplayerGame :: ChangedTargetHandler" );
	}

	InitPlayers();

	if ( g_kUtopiaModule.IsHost() && g_kUtopiaModule.IsClient() ) {
		TGPlayerEvent *event = new TGPlayerEvent;
		event->SetEventType( ET_NEW_PLAYER_IN_GAME );
		event->SetPlayerID( g_kUtopiaModule.GetNetwork()->GetHostID() );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
		players[0].inUse = true;
		players[0].playerID = g_kUtopiaModule.GetNetwork()->GetHostID();
	}
	readyForNewPlayers = false;

	MultiplayerWindow *window = (MultiplayerWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window != NULL ) {
		window->SetGameOver( false );
	}
	g_kUtopiaModule.SetMultiplayer( false );
	g_kUtopiaModule.ForceUnpause();
	g_kUtopiaModule.SetMultiplayer( true );
	if ( g_kUtopiaModule.IsHost() ) {
		window->SetHostUsingModem( g_kUtopiaModule.IsModemConnection() );
	}
}

/*
================
MultiplayerGame::~MultiplayerGame
================
*/
// FUNCTION: STBC 0x0069ebb0
MultiplayerGame::~MultiplayerGame( void ) {
	g_kEventManager.RemoveAllBroadcastHandlersForObject( this );
	if ( g_kUtopiaModule.IsHost() ) {
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		if ( network != NULL ) {
			network->DeleteGroup( "NoMe" );
			network->DeleteGroup( "Forward" );
		}
	}
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		topWindow->ShowBadConnectionText( false );
	}
}

// The seconds without a ping from the host after which the connection is
// shown as bad (invented name).
#define BAD_CONNECTION_TIME		5.0f

/*
================
MultiplayerGame::OnIdle

Shows whether the host has gone quiet, and keeps the game running at
full speed.
================
*/
// FUNCTION: STBC 0x0069edc0
void MultiplayerGame::OnIdle( void ) {
	Game::OnIdle();
	SendShipUpdates();
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL ) {
		TopWindow *topWindow = TopWindow::GetTopWindow();
		if ( topWindow != NULL ) {
			if ( network->GetTimeElapsedSinceLastHostPing() > BAD_CONNECTION_TIME && network->GetConnectStatus() == TGNETWORK_CONNECTED ) {
				topWindow->ShowBadConnectionText( true );
			} else {
				topWindow->ShowBadConnectionText( false );
			}
		}
	}
	UtopiaApp::GetApp()->SetTimeScale( 1.0f );
}

/*
================
MultiplayerGame::SendShipUpdates
================
*/
// FUNCTION: STBC 0x0069ee50
// PARKED: register wall: retail gives the ship ID and the count's address EAX/ECX and loads the network before the player ID for the send; same calls, blocks and frame
void MultiplayerGame::SendShipUpdates( void ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	if ( g_kUtopiaModule.IsHost() ) {
		for ( int i = 0; i < MAX_PLAYERS; i++ ) {
			if ( players[i].inUse && players[i].playerID != network->GetHostID() ) {
				ShipClass *ship = ShipClass::GetObjectByID( NULL, players[i].shipID );
				if ( ship != NULL && ship->GetContainingSet() != NULL ) {
					int count;
					PhysicsObjectClass **objects = PhysicsObjectClass::GetClassObjects( ship->GetContainingSet(), &count );
					for ( int j = 0; j < count; j++ ) {
						TGMessage *message = objects[j]->VirtualSlot72( &players[i] );
						if ( message != NULL ) {
							network->SendTGMessage( players[i].playerID, message, 0 );
						}
					}
				}
			}
		}
	} else if ( player != NULL ) {
		TGMessage *message = player->VirtualSlot72( &players[g_kUtopiaModule.GetPlayerNumber()] );
		if ( message != NULL ) {
			network->SendTGMessage( network->GetHostID(), message, 0 );
		}
	}
}

/*
================
MultiplayerGame::Terminate

The network goes with the game; the multiplayer window shows the game
as over.
================
*/
// FUNCTION: STBC 0x0069ef70
void MultiplayerGame::Terminate( TGEvent *event ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		topWindow->ShowBadConnectionText( false );
	}
	Game::Terminate( event );
	g_kUtopiaModule.TerminateNetwork();
	MultiplayerWindow *window = (MultiplayerWindow *)topWindow->FindMainWindow( MWT_MULTIPLAYER );
	if ( window != NULL ) {
		window->SetGameOver( true );
	}
}

/*
================
MultiplayerGame::InitPlayers
================
*/
// FUNCTION: STBC 0x0069efc0
void MultiplayerGame::InitPlayers( void ) {
	for ( int i = 0; i < MAX_PLAYERS; i++ ) {
		players[i].Init( i );
	}
}

/*
================
MultiplayerGame::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0069efe0
void MultiplayerGame::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ReceiveMessageHandler, "MultiplayerGame :: ReceiveMessageHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::DisconnectHandler, "MultiplayerGame :: DisconnectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::NewPlayerHandler, "MultiplayerGame :: NewPlayerHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::SystemChecksumPassedHandler, "MultiplayerGame::SystemChecksumPassedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::SystemChecksumFailedHandler, "MultiplayerGame::SystemChecksumFailedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::DeletePlayerHandler, "MultiplayerGame :: DeletePlayerHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ObjectCreatedHandler, "MultiplayerGame :: ObjectCreatedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::HostEventHandler, "MultiplayerGame :: HostEventHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StartFiringHandler, "MultiplayerGame :: StartFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StopFiringHandler, "MultiplayerGame :: StopFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StopFiringAtTargetHandler, "MultiplayerGame :: StopFiringAtTargetHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::SubsystemStateChangedHandler, "MultiplayerGame :: SubsystemStateChangedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::DeleteObjectHandler, "MultiplayerGame :: DeleteObjectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::AddToRepairListHandler, "MultiplayerGame :: AddToRepairListHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::RepairListPriorityHandler, "MultiplayerGame :: RepairListPriorityHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ClientEventHandler, "MultiplayerGame :: ClientEventHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ChangedTargetHandler, "MultiplayerGame :: ChangedTargetHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StartCloakingHandler, "MultiplayerGame :: StartCloakingHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StopCloakingHandler, "MultiplayerGame :: StopCloakingHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ChecksumCompleteHandler, "MultiplayerGame :: ChecksumCompleteHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::KillGameHandler, "MultiplayerGame :: KillGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::StartWarpHandler, "MultiplayerGame :: StartWarpHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::SetPhaserLevelHandler, "MultiplayerGame :: SetPhaserLevelHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::NewPlayerInGameHandler, "MultiplayerGame :: NewPlayerInGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ObjectExplodingHandler, "MultiplayerGame :: ObjectExplodingHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::EnterSetHandler, "MultiplayerGame :: EnterSetHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::ExitedWarpHandler, "MultiplayerGame :: ExitedWarpHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::TorpedoTypeChangedHandler, "MultiplayerGame :: TorpedoTypeChangedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerGame::RetryConnectHandler, "MultiplayerGame :: RetryConnectHandler" );
}

/*
================
MultiplayerGame::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0069f250
void MultiplayerGame::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KILL_GAME, "MultiplayerGame :: KillGameHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_RETRY_CONNECTION, "MultiplayerGame :: RetryConnectHandler" );
}

/*
================
MultiplayerGame::ReceiveMessageHandler

Hands each game message to its handler by its type, once a mission is
running.
================
*/
// FUNCTION: STBC 0x0069f2a0
void MultiplayerGame::ReceiveMessageHandler( TGMessageEvent *event ) {
	if ( currentEpisode == NULL || currentEpisode->GetCurrentMission() == NULL ) {
		return;
	}
	g_kUtopiaModule.SetProcessingPackets( true );

	TGMessage *message = event->GetMessage();
	if ( message->VirtualSlot00() == TGMESSAGE_TYPE ) {
		switch ( *message->GetData( NULL ) ) {
			case SELF_DESTRUCT_REQUEST_MESSAGE:
				ReceiveSelfDestructRequestMessage( message );
				break;
			case CLIENT_READY_MESSAGE:
				ReceiveClientReadyMessage( message );
				break;
			case CREATE_OBJECT_MESSAGE:
				ReceiveCreateObjectMessage( message, false );
				break;
			case CREATE_PLAYER_OBJECT_MESSAGE:
				ReceiveCreateObjectMessage( message, true );
				break;
			case START_FIRING_MESSAGE:
				ReceiveEventMessage( message, ET_START_FIRING );
				break;
			case STOP_FIRING_MESSAGE:
				ReceiveEventMessage( message, ET_STOP_FIRING );
				break;
			case STOP_FIRING_AT_TARGET_MESSAGE:
				ReceiveEventMessage( message, ET_STOP_FIRING_AT_TARGET );
				break;
			case SUBSYSTEM_STATE_CHANGED_MESSAGE:
				ReceiveEventMessage( message, ET_SUBSYSTEM_STATE_CHANGED );
				break;
			case ADD_TO_REPAIR_LIST_MESSAGE:
				ReceiveEventMessage( message, ET_ADD_TO_REPAIR_LIST );
				break;
			case CLIENT_EVENT_MESSAGE:
			case REPAIR_LIST_PRIORITY_MESSAGE:
			case SET_PHASER_LEVEL_MESSAGE:
				ReceiveEventMessage( message, 0 );
				break;
			case SHIP_UPDATE_MESSAGE:
				ReceiveShipUpdateMessage( message );
				break;
			case HOST_EVENT_MESSAGE:
			case CHANGED_TARGET_MESSAGE:
				ReceiveHostEventMessage( message );
				break;
			case START_CLOAKING_MESSAGE:
				ReceiveEventMessage( message, ET_START_CLOAKING );
				break;
			case STOP_CLOAKING_MESSAGE:
				ReceiveEventMessage( message, ET_STOP_CLOAKING );
				break;
			case START_WARP_MESSAGE:
				ReceiveEventMessage( message, ET_START_WARP );
				break;
			case TORPEDO_TYPE_CHANGED_MESSAGE:
				ReceiveEventMessage( message, ET_NET_TORPEDO_TYPE_CHANGED );
				break;
			case DAMAGE_VOLUME_MESSAGE:
				ReceiveDamageVolumeMessage( message );
				break;
			case DELETE_OBJECT_FROM_GAME_MESSAGE:
				ReceiveDeleteObjectFromGameMessage( message );
				break;
			case CLIENT_COLLISION_MESSAGE:
				ReceiveClientCollisionMessage( message );
				break;
			case NEW_PLAYER_IN_GAME_MESSAGE:
				ReceiveNewPlayerInGameMessage( message );
				break;
			case DELETE_PLAYER_FROM_GAME_MESSAGE:
				ReceiveDeletePlayerFromGameMessage( message );
				break;
			case CREATE_TORP_MESSAGE:
				ReceiveCreateTorpMessage( message );
				break;
			case CREATE_PULSE_MESSAGE:
				ReceiveCreatePulseMessage( message );
				break;
			case VERIFY_ENTER_SET_MESSAGE:
				ReceiveVerifyEnterSetMessage( message );
				break;
			case VERIFY_EXITED_WARP_MESSAGE:
				ReceiveVerifyExitedWarpMessage( message );
				break;
			case SEND_OBJECT_MESSAGE:
				ReceiveSendObjectMessage( message );
				break;
		}
	}
	g_kUtopiaModule.SetProcessingPackets( false );
}

/*
================
MultiplayerGame::ReceiveCreateObjectMessage

Makes the object another player created, with the IDs of the sender's
place: the module's player number and next ID are switched to the
sender's while the object is made, and switched back after. The host
passes the message on to every other client and records a player's ship;
objects other than torpedoes are then driven by a NetworkAI, on the host
only the other players' ships.
================
*/
// FUNCTION: STBC 0x0069f620
// PARKED: 70.4, complete; register allocation: retail keeps this in ebx until the ID swap, reads playerObject from its argument slot each time and keeps the message in the frame through the forwarding loop; same calls and blocks
void MultiplayerGame::ReceiveCreateObjectMessage( TGMessage *message, bool playerObject ) {
	int length;
	char *data = message->GetData( &length );
	TGObject::SetIncrementID( false );
	int playerNumber = data[1];
	int headerSize = 2;
	int netPlayerID;
	if ( playerObject ) {
		netPlayerID = data[2];
		headerSize = 3;
	}

	int localPlayerNumber = g_kUtopiaModule.GetPlayerNumber();
	players[localPlayerNumber].firstObjectID = g_kUtopiaModule.GetUnusedClientID();
	g_kUtopiaModule.SetPlayerNumber( playerNumber );
	g_kUtopiaModule.SetUnusedClientID( players[playerNumber].firstObjectID );
	PhysicsObjectClass *object = PhysicsObjectClass::CreateFromNetBuffer( data + headerSize, length - headerSize );
	players[playerNumber].firstObjectID = g_kUtopiaModule.GetUnusedClientID();
	g_kUtopiaModule.SetPlayerNumber( localPlayerNumber );
	g_kUtopiaModule.SetUnusedClientID( players[localPlayerNumber].firstObjectID );
	TGObject::SetIncrementID( true );
	if ( object == NULL ) {
		return;
	}
	if ( playerObject ) {
		( (ShipClass *)object )->SetNetPlayerID( netPlayerID );
	}

	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	NetworkAI *ai;
	if ( g_kUtopiaModule.IsHost() ) {
		for ( int i = 0; i < MAX_PLAYERS; i++ ) {
			if ( players[i].inUse ) {
				if ( players[i].playerID == message->GetFromID() ) {
					if ( playerObject ) {
						players[i].shipID = object->GetObjID();
					}
				} else if ( players[i].playerID != network->GetHostID() ) {
					TGMessage *copy = message->Copy();
					network->SendTGMessage( players[i].playerID, copy, 0 );
				}
			}
		}
		if ( g_kUtopiaModule.IsClient() ) {
			if ( !playerObject || object->GetObjID() == players[0].shipID || object->GetObjType() == TORPEDO_TYPE ) {
				return;
			}
			ai = new NetworkAI( object, "Network" );
		} else {
			if ( !playerObject || object->GetObjType() == TORPEDO_TYPE ) {
				return;
			}
			ai = new NetworkAI( object, "Network" );
		}
	} else {
		if ( object->GetObjType() == TORPEDO_TYPE ) {
			return;
		}
		ai = new NetworkAI( object, "Network" );
	}
	object->SetAI( ai, true, true );
	object->SetUsePhysics( false );
}

/*
================
MultiplayerGame::ReceiveHostEventMessage

The event the message carries is handled here at once.
================
*/
// FUNCTION: STBC 0x0069f880
void MultiplayerGame::ReceiveHostEventMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	TGEvent *event = TGEvent::CreateFromNetStream( &stream );
	TGObject::FixUpObject( event );
	event->SetUnknown24( NULL );
	g_kEventManager.ProcessEvent( event );
	if ( event->GetRefCount() == 0 ) {
		delete event;
	}
}

// The target radius a torpedo's aim point is scaled by when its target is
// gone, and the least one used (invented names).
#define DEFAULT_TARGET_RADIUS	1.0f
#define MIN_TARGET_RADIUS		0.01f

/*
================
MultiplayerGame::ReceiveCreateTorpMessage

The host forwards another player's torpedo to the "Forward" group,
leaving the sender out of it for the send; everyone but the sender then
fires it from the tube the message names.
================
*/
// FUNCTION: STBC 0x0069f930
void MultiplayerGame::ReceiveCreateTorpMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	if ( g_kUtopiaModule.IsHost() && message->GetFromID() != network->GetLocalID() ) {
		TGMessage *copy = message->Copy();
		TGNetGroup *group = network->GetGroup( "Forward" );
		if ( group != NULL ) {
			bool senderInGroup = group->IsPlayerInGroup( message->GetFromID() );
			if ( senderInGroup ) {
				group->DeletePlayerFromGroup( message->GetFromID() );
			}
			network->SendTGMessageToGroup( group, copy );
			if ( senderInGroup ) {
				group->AddPlayerToGroup( message->GetFromID() );
			}
		}
	}
	if ( message->GetFromID() != network->GetLocalID() ) {
		int length;
		char *data = message->GetData( &length ) + 1;
		TGBufferStream stream;
		long targetID = 0;
		stream.VirtualSlot60( data, length - 1 );
		long tubeID = stream.ReadID();
		int playerID = stream.ReadChar();
		char flags = stream.ReadChar();
		NiPoint3 direction;
		stream.VirtualSlot35( &direction.x, &direction.y, &direction.z );
		bool playSound = flags & CREATE_TORP_PLAY_SOUND;
		bool skewFire = ( flags & CREATE_TORP_SKEW_FIRE ) != 0;
		bool noTarget = ( flags & CREATE_TORP_NO_TARGET ) != 0;

		TorpedoTube *tube = (TorpedoTube *)TGObject::GetTGObjectPtr( tubeID );
		if ( tube != NULL ) {
			NiPoint3 offset;
			if ( !noTarget ) {
				targetID = stream.ReadID();
				float radius = DEFAULT_TARGET_RADIUS;
				ObjectClass *target = ObjectClass::GetObjectByID( NULL, targetID );
				if ( target != NULL ) {
					radius = target->GetRadius();
					if ( radius <= 0.0f ) {
						radius = MIN_TARGET_RADIUS;
					}
				} else {
					noTarget = true;
				}
				stream.VirtualSlot39( &offset.x, &offset.y, &offset.z, radius );
			}
			// BUG: the sound flag goes to CreateNetTorpedo's skew parameter and
			// the skew flag to its sound parameter
			tube->CreateNetTorpedo( playSound, skewFire, noTarget, playerID, targetID, offset, direction );
		}
	}
}

/*
================
MultiplayerGame::ReceiveCreatePulseMessage

The host forwards another player's pulse to the "Forward" group,
leaving the sender out of it for the send; everyone but the sender then
fires it from the weapon the message names.
================
*/
// FUNCTION: STBC 0x0069fbb0
// PARKED: register wall: the call set and blocks agree with retail; the registers and frame slots of the forward and the stream read differ
void MultiplayerGame::ReceiveCreatePulseMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	if ( g_kUtopiaModule.IsHost() && message->GetFromID() != network->GetLocalID() ) {
		TGMessage *copy = message->Copy();
		TGNetGroup *group = network->GetGroup( "Forward" );
		if ( group != NULL ) {
			bool senderInGroup = group->IsPlayerInGroup( message->GetFromID() );
			if ( senderInGroup ) {
				group->DeletePlayerFromGroup( message->GetFromID() );
			}
			network->SendTGMessageToGroup( group, copy );
			if ( senderInGroup ) {
				group->AddPlayerToGroup( message->GetFromID() );
			}
		}
	}
	if ( message->GetFromID() != network->GetLocalID() ) {
		long targetID = 0;
		int length;
		char *data = message->GetData( &length ) + 1;
		TGBufferStream stream;
		stream.VirtualSlot60( data, length - 1 );
		long weaponID = stream.ReadID();
		int playerID = stream.ReadChar();
		NiPoint3 direction;
		stream.VirtualSlot35( &direction.x, &direction.y, &direction.z );
		char flags = stream.ReadChar();
		bool playSound = flags & CREATE_PULSE_PLAY_SOUND;
		if ( flags & CREATE_PULSE_HAS_TARGET ) {
			targetID = stream.ReadID();
		}

		PulseWeapon *weapon = (PulseWeapon *)TGObject::GetTGObjectPtr( weaponID );
		if ( weapon != NULL ) {
			weapon->CreateNetPulse( playSound, playerID, targetID, &direction );
		}
	}
}

/*
================
MultiplayerGame::ReceiveEventMessage

The host forwards a copy to the "Forward" group, leaving the sender out
of it for the send. Everyone but the sender then handles the event the
message carries, as the type given when it is not 0.
================
*/
// FUNCTION: STBC 0x0069fda0
void MultiplayerGame::ReceiveEventMessage( TGMessage *message, int eventType ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	if ( g_kUtopiaModule.IsHost() ) {
		TGMessage *copy = message->Copy();
		TGNetGroup *group = network->GetGroup( "Forward" );
		if ( group != NULL ) {
			bool senderInGroup = group->IsPlayerInGroup( message->GetFromID() );
			if ( senderInGroup ) {
				group->DeletePlayerFromGroup( message->GetFromID() );
			}
			network->SendTGMessageToGroup( group, copy );
			if ( senderInGroup ) {
				group->AddPlayerToGroup( message->GetFromID() );
			}
		}
	}
	if ( message->GetFromID() != network->GetLocalID() ) {
		int length;
		char *data = message->GetData( &length ) + 1;
		TGBufferStream stream;
		stream.VirtualSlot60( data, length - 1 );
		TGEvent *event = TGEvent::CreateFromNetStream( &stream );
		TGObject::FixUpObject( event );
		event->SetUnknown24( NULL );
		if ( eventType != 0 ) {
			event->SetEventType( eventType );
		}
		g_kEventManager.ProcessEvent( event );
		if ( event->GetRefCount() == 0 ) {
			delete event;
		}
	}
}

/*
================
MultiplayerGame::ReceiveShipUpdateMessage

The object the update names reads it.
================
*/
// FUNCTION: STBC 0x0069ff50
void MultiplayerGame::ReceiveShipUpdateMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length );
	TGBufferStream stream;
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	PhysicsObjectClass *object = PhysicsObjectClass::GetObjectByID( NULL, stream.ReadLong() );
	if ( object != NULL ) {
		object->VirtualSlot73( message );
	}
}

/*
================
MultiplayerGame::Unknown69FFF0

Opens the message past its type and reads nothing: no message type
reaches it.
================
*/
// FUNCTION: STBC 0x0069fff0
void MultiplayerGame::Unknown69FFF0( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
}

/*
================
MultiplayerGame::ReceiveDamageVolumeMessage

Adds the damage volume the message carries to the object it names.
================
*/
// FUNCTION: STBC 0x006a0080
void MultiplayerGame::ReceiveDamageVolumeMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	DamageableObject *object = DamageableObject::GetObjectByID( NULL, stream.ReadID() );
	if ( object != NULL ) {
		float x, y, z;
		stream.VirtualSlot37( &x, &y, &z, true );
		NiPoint3 center;
		center.x = x;
		center.y = y;
		center.z = z;
		float radius = Unknown6D3B30( stream.ReadShort() );
		float strength = Unknown6D3B30( stream.ReadShort() );
		object->AddMetaVolume( new MetaVolume( center, radius, strength ) );
	}
}

/*
================
MultiplayerGame::ReceiveSelfDestructRequestMessage

The host destroys the hull of the sender's ship.
================
*/
// FUNCTION: STBC 0x006a01b0
void MultiplayerGame::ReceiveSelfDestructRequestMessage( TGMessage *message ) {
	if ( g_kUtopiaModule.IsHost() ) {
		ShipClass *ship = GetShipFromPlayerID( message->GetFromID() );
		if ( ship != NULL ) {
			ship->DestroySystem( ship->GetHull() );
		}
	}
}

/*
================
MultiplayerGame::ReceiveDeleteObjectFromGameMessage

The named object leaves its set; one in no set loses its AIs and is
deleted.
================
*/
// FUNCTION: STBC 0x006a01e0
void MultiplayerGame::ReceiveDeleteObjectFromGameMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	long id = stream.ReadID();
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, id );
	if ( object != NULL ) {
		SetClass *set = object->GetContainingSet();
		if ( set != NULL ) {
			set->DeleteObjectFromSet( id );
		} else {
			PhysicsObjectClass *physicsObject = PhysicsObjectClass::Cast( object );
			if ( physicsObject != NULL ) {
				physicsObject->ClearAI();
			}
			delete object;
		}
	}
}

/*
================
MultiplayerGame::ReceiveSendObjectMessage

A player asks for an object it doesn't have: it is sent the object's
create message, and its damage volumes, unless the object is dying or
dead.
================
*/
// FUNCTION: STBC 0x006a02a0
void MultiplayerGame::ReceiveSendObjectMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL ) {
		int playerID = message->GetFromID();
		int length;
		char *data = message->GetData( &length ) + 1;
		TGBufferStream stream;
		stream.VirtualSlot60( data, length - 1 );
		PhysicsObjectClass *object = PhysicsObjectClass::GetObjectByID( NULL, stream.ReadLong() );
		if ( object != NULL && object->GetNetType() != 0 ) {
			DamageableObject *damageable = DamageableObject::Cast( object );
			if ( damageable == NULL || ( damageable->GetLifeTime() >= INFINITE_LIFE && !damageable->IsDead() ) ) {
				char buffer[MAX_MESSAGE_SIZE];
				int size = 2;
				ShipClass *ship = ShipClass::Cast( object );
				if ( ship != NULL && ship->IsPlayerShip() ) {
					buffer[0] = CREATE_PLAYER_OBJECT_MESSAGE;
					size = 3;
				} else {
					buffer[0] = CREATE_OBJECT_MESSAGE;
				}
				buffer[1] = GetPlayerNumberFromID( object->GetObjID() );
				if ( size == 3 ) {
					buffer[2] = ship->GetNetPlayerID();
				}
				size += object->VirtualSlot67( buffer + size, sizeof( buffer ) - size );

				TGMessage *reply = new TGMessage;
				reply->SetData( buffer, size );
				reply->SetGuaranteed( true );
				reply->SetAggregate( false );
				network->SendTGMessage( playerID, reply, 0 );
				if ( damageable != NULL ) {
					damageable->SendDamageVolumes( playerID );
				}
			}
		}
	}
}

/*
================
MultiplayerGame::ReceiveVerifyEnterSetMessage

A player's object has entered a set on the host's word: when this
side doesn't know the object, it asks the host for it.
================
*/
// FUNCTION: STBC 0x006a0490
void MultiplayerGame::ReceiveVerifyEnterSetMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	long id = stream.ReadLong();
	if ( ObjectClass::GetObjectByID( NULL, id ) == NULL ) {
		TGBufferStream requestStream;
		char buffer[MAX_MESSAGE_SIZE];
		requestStream.VirtualSlot60( buffer, sizeof( buffer ) );
		requestStream.WriteChar( SEND_OBJECT_MESSAGE );
		requestStream.WriteLong( id );
		TGMessage *request = new TGMessage;
		request->SetData( buffer, requestStream.GetPos() );
		request->SetGuaranteed( true );
		network->SendTGMessage( 0, request, 0 );
	}
}

/*
================
MultiplayerGame::ReceiveVerifyExitedWarpMessage

A ship has left warp into the named set on the host's word: an unknown
ship is asked for; a known one that isn't warping moves into the set.
================
*/
// FUNCTION: STBC 0x006a05e0
void MultiplayerGame::ReceiveVerifyExitedWarpMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	long id = stream.ReadLong();
	char *setName = stream.VirtualSlot10( -1 );
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, id );
	if ( object == NULL ) {
		TGBufferStream requestStream;
		char buffer[MAX_MESSAGE_SIZE];
		requestStream.VirtualSlot60( buffer, sizeof( buffer ) );
		requestStream.WriteChar( SEND_OBJECT_MESSAGE );
		requestStream.WriteLong( id );
		TGMessage *request = new TGMessage;
		request->SetData( buffer, requestStream.GetPos() );
		request->SetGuaranteed( true );
		network->SendTGMessage( 0, request, 0 );
	} else {
		ShipClass *ship = ShipClass::Cast( object );
		if ( ship != NULL && ship->GetWarpEngineSubsystem() != NULL && ship->GetWarpEngineSubsystem()->GetWarpState() == WarpEngineSubsystem::WES_NOT_WARPING ) {
			SetClass *set = g_kSetManager.GetSet( setName );
			if ( ship->GetContainingSet() != set ) {
				if ( ship->GetContainingSet() != NULL ) {
					ship->GetContainingSet()->RemoveObjectFromSet( ship->GetObjID() );
				}
				set->AddObjectToSet( ship, ship->GetName() );
			}
		}
	}
	delete setName;
}

/*
================
MultiplayerGame::EnterSetHandler

The host tells the other players that a ship of theirs has entered a
set: a warping ship once it is out of the warp set, naming its set.
================
*/
// FUNCTION: STBC 0x006a07d0
void MultiplayerGame::EnterSetHandler( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsHost() ) {
		return;
	}
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL || event->GetDestination() == NULL ) {
		return;
	}
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship == NULL || ship->GetNetType() == 0 ) {
		return;
	}
	if ( ship->GetWarpEngineSubsystem() != NULL && ship->GetWarpEngineSubsystem()->GetWarpState() != WarpEngineSubsystem::WES_NOT_WARPING ) {
		TGBufferStream stream;
		char buffer[MAX_MESSAGE_SIZE];
		stream.VirtualSlot60( buffer, sizeof( buffer ) );
		SetClass *set = ship->GetContainingSet();
		if ( set != NULL ) {
			char *setName = set->GetName();
			if ( strcmp( setName, "warp" ) != 0 ) {
				stream.WriteChar( VERIFY_EXITED_WARP_MESSAGE );
				stream.WriteLong( ship->GetObjID() );
				stream.VirtualSlot11( setName );
				TGMessage *message = new TGMessage;
				message->SetData( buffer, stream.GetPos() );
				message->SetGuaranteed( true );
				network->SendTGMessageToGroup( "NoMe", message );
			}
		}
	} else {
		TGBufferStream stream;
		char buffer[MAX_MESSAGE_SIZE];
		stream.VirtualSlot60( buffer, sizeof( buffer ) );
		stream.WriteChar( VERIFY_ENTER_SET_MESSAGE );
		stream.WriteLong( ship->GetObjID() );
		TGMessage *message = new TGMessage;
		message->SetData( buffer, stream.GetPos() );
		message->SetGuaranteed( true );
		network->SendTGMessageToGroup( "NoMe", message );
	}
}

/*
================
MultiplayerGame::ExitedWarpHandler
================
*/
// FUNCTION: STBC 0x006a0a10
void MultiplayerGame::ExitedWarpHandler( TGEvent *event ) {
}

/*
================
MultiplayerGame::DisconnectHandler
================
*/
// FUNCTION: STBC 0x006a0a20
void MultiplayerGame::DisconnectHandler( TGEvent *event ) {
}

// The seconds before a player who joins too early is let in again
// (invented name).
#define NEW_PLAYER_RETRY_DELAY	1.0f

/*
================
MultiplayerGame::NewPlayerHandler

On the host: a player who joins before the game takes players is tried
again a second later (ET_RETRY_CONNECTION); one who finds a free place
gets it and is asked for its checksums; one who doesn't is booted.
================
*/
// FUNCTION: STBC 0x006a0a30
// PARKED: prologue scheduling: retail saves EBX with the other registers at entry, so the retry branch shares the one epilogue; this build saves it in the second branch only; the bodies are identical
void MultiplayerGame::NewPlayerHandler( TGPlayerEvent *event ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL && g_kUtopiaModule.IsHost() ) {
		if ( !readyForNewPlayers ) {
			TGIntEvent *retryEvent = new TGIntEvent;
			retryEvent->SetEventType( ET_RETRY_CONNECTION );
			retryEvent->SetDestination( this );
			retryEvent->SetInt( 0 );
			retryEvent->SetSource( event );
			event->IncRefCount();
			TGTimer *timer = new TGTimer;
			timer->SetEvent( retryEvent );
			timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + NEW_PLAYER_RETRY_DELAY );
			g_kTimerManager.AddTimer( timer );
		} else {
			// BUG: the stream is never used, and nothing uses the place the first
			// search finds
			TGBufferStream stream;
			int i;
			for ( i = 0; i < MAX_PLAYERS; i++ ) {
				if ( players[i].inUse && players[i].playerID == event->GetPlayerID() ) {
					break;
				}
			}

			int numPlayers = 0;
			for ( i = 0; i < MAX_PLAYERS; i++ ) {
				if ( players[i].inUse ) {
					numPlayers++;
				}
			}
			int place = MAX_PLAYERS;
			if ( numPlayers < maxPlayers ) {
				for ( place = 0; place < MAX_PLAYERS; place++ ) {
					if ( !players[place].inUse ) {
						players[place].Init( place );
						players[place].inUse = true;
						players[place].playerID = event->GetPlayerID();
						break;
					}
				}
			}
			if ( place < MAX_PLAYERS && numPlayers < maxPlayers ) {
				g_kUtopiaModule.GetNetFile()->RequestChecksums( event->GetPlayerID() );
			} else {
				char playerID = event->GetPlayerID();
				TGBootPlayerMessage *message = new TGBootPlayerMessage;
				message->SetBootReason( TGNetwork::TOO_MANY_PLAYERS );
				message->SetData( &playerID, sizeof( playerID ) );
				network->SendTGMessage( event->GetPlayerID(), message, 0 );
			}
		}
	}
}

/*
================
MultiplayerGame::SystemChecksumPassedHandler

The player's system files check out: the multiplayer scripts are
checked next.
================
*/
// FUNCTION: STBC 0x006a0c60
void MultiplayerGame::SystemChecksumPassedHandler( TGPlayerEvent *event ) {
	g_kUtopiaModule.GetNetFile()->RequestSystemChecksum( event->GetPlayerID(), "Scripts/Multiplayer", "*.pyc", true );
}

/*
================
MultiplayerGame::SystemChecksumFailedHandler
================
*/
// FUNCTION: STBC 0x006a0c90
void MultiplayerGame::SystemChecksumFailedHandler( TGEvent *event ) {
}

/*
================
MultiplayerGame::DeletePlayerHandler

A player has left: the host tells the others by name and destroys the
player's ship; the place is emptied and the player leaves the groups.
================
*/
// FUNCTION: STBC 0x006a0ca0
// PARKED: register wall: the call set and blocks agree; retail compares the player ID straight from memory (cmp edx, [esi+0x28]) where this build loads both
void MultiplayerGame::DeletePlayerHandler( TGPlayerEvent *event ) {
	for ( int i = 0; i < MAX_PLAYERS; i++ ) {
		if ( players[i].inUse && players[i].playerID == event->GetPlayerID() ) {
				ShipClass *ship = ShipClass::GetObjectByID( NULL, players[i].shipID );
				if ( ship != NULL && g_kUtopiaModule.IsHost() ) {
					TGNetwork *network = g_kUtopiaModule.GetNetwork();
					if ( network != NULL ) {
						char buffer[MAX_MESSAGE_SIZE];
						buffer[0] = DELETE_PLAYER_FROM_GAME_MESSAGE;
						TGNetPlayer *player = network->GetPlayerList()->GetPlayerFromID( event->GetPlayerID() );
						if ( player != NULL && player->GetName() != NULL ) {
							char *name = player->GetName()->Unknown6F56F0();
							TGBufferStream stream;
							stream.VirtualSlot60( buffer + 1, sizeof( buffer ) - 1 );
							stream.WriteCString( name );
							delete name;
							int length = stream.GetPos() + 1;
							TGMessage *message = new TGMessage;
							message->SetData( buffer, length );
							message->SetGuaranteed( true );
							network->SendTGMessage( 0, message, 0 );
						}
					}
					ship->DestroySystem( ship->GetHull() );
				}
				players[i].Init( i );

				TGNetwork *network = g_kUtopiaModule.GetNetwork();
				if ( network != NULL ) {
					TGNetGroup *group = network->GetGroup( "NoMe" );
					if ( group != NULL ) {
						group->DeletePlayerFromGroup( event->GetPlayerID() );
					}
					group = network->GetGroup( "Forward" );
					if ( group != NULL ) {
						group->DeletePlayerFromGroup( event->GetPlayerID() );
					}
				}
			return;
		}
	}
}

/*
================
MultiplayerGame::ObjectCreatedHandler

A networked physics object other than a torpedo was created here: the
other players are told to create it (the host tells every other player,
a client the host). The local player's ship is recorded as the player's
ship; on a client every other object is moved by a NetworkAI instead of
its own physics.
================
*/
// FUNCTION: STBC 0x006a0f90
void MultiplayerGame::ObjectCreatedHandler( TGEvent *event ) {
	TGEventHandlerObject *created = event->GetDestination();
	if ( created == NULL || !created->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) || created->IsTypeOf( TORPEDO_TYPE ) ) {
		return;
	}
	PhysicsObjectClass *object = (PhysicsObjectClass *)created;
	if ( object->GetNetType() == 0 ) {
		return;
	}
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}

	char buffer[MAX_MESSAGE_SIZE];
	bool playerObject = false;
	if ( object == GetPlayer() ) {
		buffer[0] = CREATE_PLAYER_OBJECT_MESSAGE;
		players[g_kUtopiaModule.GetPlayerNumber()].shipID = object->GetObjID();
		playerObject = true;
	} else {
		buffer[0] = CREATE_OBJECT_MESSAGE;
		if ( !g_kUtopiaModule.IsHost() && object->GetObjType() != TORPEDO_TYPE ) {
			object->SetAI( new NetworkAI( object, "Network" ) );
			object->SetUsePhysics( false );
		}
	}
	buffer[1] = g_kUtopiaModule.GetPlayerNumber();
	int size = 2;
	if ( playerObject ) {
		buffer[2] = network->GetLocalID();
		size = 3;
	}
	size += object->VirtualSlot67( buffer + size, sizeof( buffer ) - size );

	TGMessage *message = new TGMessage;
	message->SetData( buffer, size );
	message->SetGuaranteed( true );
	if ( g_kUtopiaModule.IsHost() ) {
		network->SendTGMessageToGroup( "NoMe", message );
	} else {
		network->SendTGMessage( network->GetHostID(), message, 0 );
	}
}

/*
================
MultiplayerGame::HostEventHandler

The host sends the event to every other player.
================
*/
// FUNCTION: STBC 0x006a1150
void MultiplayerGame::HostEventHandler( TGEvent *event ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	char buffer[MAX_MESSAGE_SIZE];
	buffer[0] = HOST_EVENT_MESSAGE;
	TGBufferStream stream;
	stream.VirtualSlot60( buffer + 1, sizeof( buffer ) - 1 );
	event->WriteToNetStream( &stream );
	int length = stream.GetPos() + 1;

	TGMessage *message = new TGMessage;
	message->SetData( buffer, length );
	message->SetGuaranteed( true );
	network->SendTGMessageToGroup( "NoMe", message );
}

/*
================
MultiplayerGame::ObjectExplodingHandler

The host tells the other players about the explosion; a client runs the
exploding ship's death script with the explosion's lifetime.
================
*/
// FUNCTION: STBC 0x006a1240
void MultiplayerGame::ObjectExplodingHandler( TGEvent *event ) {
	ObjectExplodingEvent *exploding = (ObjectExplodingEvent *)event;
	if ( g_kUtopiaModule.IsHost() ) {
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		if ( network == NULL ) {
			return;
		}
		char buffer[MAX_MESSAGE_SIZE];
		buffer[0] = HOST_EVENT_MESSAGE;
		TGBufferStream stream;
		stream.VirtualSlot60( buffer + 1, sizeof( buffer ) - 1 );
		exploding->WriteToNetStream( &stream );
		int length = stream.GetPos() + 1;

		TGMessage *message = new TGMessage;
		message->SetData( buffer, length );
		message->SetGuaranteed( true );
		network->SendTGMessageToGroup( "NoMe", message );
	} else {
		ShipClass *ship = ShipClass::Cast( exploding->GetDestination() );
		if ( ship != NULL ) {
			ship->SetLifeTime( exploding->GetLifetime() );
			ship->RunDeathScript();
		}
	}
}

/*
================
MultiplayerGame::ReceiveNewPlayerInGameMessage

The host's ET_NEW_PLAYER_IN_GAME event comes to this game.
================
*/
// FUNCTION: STBC 0x006a1360
void MultiplayerGame::ReceiveNewPlayerInGameMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	TGEvent *event = TGEvent::CreateFromNetStream( &stream );
	TGObject::FixUpObject( event );
	event->SetDestination( this );
	event->SetUnknown24( NULL );
	g_kEventManager.ProcessEvent( event );
	if ( event->GetRefCount() == 0 ) {
		delete event;
	}
}

/*
================
MultiplayerGame::ReceiveDeletePlayerFromGameMessage

Shows the host's notice that a player left, with the player's name.
================
*/
// FUNCTION: STBC 0x006a1420
void MultiplayerGame::ReceiveDeletePlayerFromGameMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	char name[256];
	stream.ReadCString( name, sizeof( name ) );

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *format = database->GetString( "Delete Player" );
	g_kLocalizationManager.Unload( database );
	char *formatString = format->Unknown6F56F0();
	char text[256];
	sprintf( text, formatString, name );
	delete formatString;

	TGSequence *sequence = new TGSequence;
	SubtitleAction *subtitle = new SubtitleAction( text );
	subtitle->SetDuration( NOTICE_DURATION );
	sequence->AddAction( subtitle );
	sequence->Play();
}

/*
================
MultiplayerGame::NewPlayerInGameHandler

The host passes the event on to the other players; every player but the
new one shows a notice with the new player's name.
================
*/
// FUNCTION: STBC 0x006a1590
void MultiplayerGame::NewPlayerInGameHandler( TGEvent *event ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	char buffer[MAX_MESSAGE_SIZE];
	if ( g_kUtopiaModule.IsHost() ) {
		buffer[0] = NEW_PLAYER_IN_GAME_MESSAGE;
		TGBufferStream stream;
		stream.VirtualSlot60( buffer + 1, sizeof( buffer ) - 1 );
		event->WriteToNetStream( &stream );
		int length = stream.GetPos() + 1;
		TGMessage *message = new TGMessage;
		message->SetData( buffer, length );
		message->SetGuaranteed( true );
		network->SendTGMessageToGroup( "NoMe", message );
	}

	int playerID = ( (TGPlayerEvent *)event )->GetPlayerID();
	if ( playerID == network->GetLocalID() ) {
		return;
	}
	TGNetPlayer *player = network->GetPlayerList()->GetPlayerFromID( playerID );
	// BUG: the player is not tested for NULL before its name is taken
	if ( player->GetName() == NULL ) {
		return;
	}
	char *name = player->GetName()->Unknown6F56F0();
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *format = database->GetString( "New Player" );
	g_kLocalizationManager.Unload( database );
	char *formatString = format->Unknown6F56F0();
	sprintf( buffer, formatString, name );
	delete name;
	delete formatString;

	TGSequence *sequence = new TGSequence;
	SubtitleAction *subtitle = new SubtitleAction( buffer );
	subtitle->SetDuration( NOTICE_DURATION );
	sequence->AddAction( subtitle );
	sequence->Play();
}

/*
================
MultiplayerGame::StartFiringHandler
================
*/
// FUNCTION: STBC 0x006a1790
void MultiplayerGame::StartFiringHandler( TGEvent *event ) {
	SendEventMessage( event, START_FIRING_MESSAGE );
}

/*
================
MultiplayerGame::StartWarpHandler
================
*/
// FUNCTION: STBC 0x006a17a0
void MultiplayerGame::StartWarpHandler( TGEvent *event ) {
	SendEventMessage( event, START_WARP_MESSAGE );
}

/*
================
MultiplayerGame::TorpedoTypeChangedHandler
================
*/
// FUNCTION: STBC 0x006a17b0
void MultiplayerGame::TorpedoTypeChangedHandler( TGEvent *event ) {
	SendEventMessage( event, TORPEDO_TYPE_CHANGED_MESSAGE );
}

/*
================
MultiplayerGame::SendEventMessage
================
*/
// FUNCTION: STBC 0x006a17c0
void MultiplayerGame::SendEventMessage( TGEvent *event, char messageType ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	char buffer[MAX_MESSAGE_SIZE];
	buffer[0] = messageType;
	TGBufferStream stream;
	stream.VirtualSlot60( buffer + 1, sizeof( buffer ) - 1 );
	event->WriteToNetStream( &stream );
	int length = stream.GetPos() + 1;

	TGMessage *message = new TGMessage;
	message->SetData( buffer, length );
	message->SetGuaranteed( true );
	if ( g_kUtopiaModule.IsHost() ) {
		network->SendTGMessageToGroup( "NoMe", message );
	} else {
		network->SendTGMessage( network->GetHostID(), message, 0 );
	}
}

/*
================
MultiplayerGame::StopFiringHandler
================
*/
// FUNCTION: STBC 0x006a18d0
void MultiplayerGame::StopFiringHandler( TGEvent *event ) {
	SendEventMessage( event, STOP_FIRING_MESSAGE );
}

/*
================
MultiplayerGame::StopFiringAtTargetHandler
================
*/
// FUNCTION: STBC 0x006a18e0
void MultiplayerGame::StopFiringAtTargetHandler( TGEvent *event ) {
	SendEventMessage( event, STOP_FIRING_AT_TARGET_MESSAGE );
}

/*
================
MultiplayerGame::StartCloakingHandler
================
*/
// FUNCTION: STBC 0x006a18f0
void MultiplayerGame::StartCloakingHandler( TGEvent *event ) {
	SendEventMessage( event, START_CLOAKING_MESSAGE );
}

/*
================
MultiplayerGame::StopCloakingHandler
================
*/
// FUNCTION: STBC 0x006a1900
void MultiplayerGame::StopCloakingHandler( TGEvent *event ) {
	SendEventMessage( event, STOP_CLOAKING_MESSAGE );
}

/*
================
MultiplayerGame::SubsystemStateChangedHandler
================
*/
// FUNCTION: STBC 0x006a1910
void MultiplayerGame::SubsystemStateChangedHandler( TGEvent *event ) {
	SendEventMessage( event, SUBSYSTEM_STATE_CHANGED_MESSAGE );
}

/*
================
MultiplayerGame::AddToRepairListHandler
================
*/
// FUNCTION: STBC 0x006a1920
void MultiplayerGame::AddToRepairListHandler( TGEvent *event ) {
	SendEventMessage( event, ADD_TO_REPAIR_LIST_MESSAGE );
}

/*
================
MultiplayerGame::ClientEventHandler
================
*/
// FUNCTION: STBC 0x006a1930
void MultiplayerGame::ClientEventHandler( TGEvent *event ) {
	SendEventMessage( event, CLIENT_EVENT_MESSAGE );
}

/*
================
MultiplayerGame::RepairListPriorityHandler

Only for the subsystems of the local player's ship.
================
*/
// FUNCTION: STBC 0x006a1940
void MultiplayerGame::RepairListPriorityHandler( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)event->GetDestination();
	if ( subsystem != NULL && subsystem->GetParentShip() == player ) {
		SendEventMessage( event, REPAIR_LIST_PRIORITY_MESSAGE );
	}
}

/*
================
MultiplayerGame::SetPhaserLevelHandler

Only for the subsystems of the local player's ship.
================
*/
// FUNCTION: STBC 0x006a1970
void MultiplayerGame::SetPhaserLevelHandler( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)event->GetDestination();
	if ( subsystem != NULL && subsystem->GetParentShip() == player ) {
		SendEventMessage( event, SET_PHASER_LEVEL_MESSAGE );
	}
}

/*
================
MultiplayerGame::GetPlayerNumberFromID

The player whose block holds an object ID.
================
*/
// FUNCTION: STBC 0x006a19a0
int MultiplayerGame::GetPlayerNumberFromID( long id ) {
	return ( id - FIRST_CLIENT_ID ) / CLIENT_ID_BLOCK_SIZE;
}

/*
================
MultiplayerGame::FindPlayerIndex
================
*/
// FUNCTION: STBC 0x006a19c0
int MultiplayerGame::FindPlayerIndex( int playerID ) {
	for ( int i = 0; i < MAX_PLAYERS; i++ ) {
		if ( players[i].inUse && players[i].playerID == playerID ) {
			return i;
		}
	}
	return 0;
}

/*
================
MultiplayerGame::GetPlayerName

The name of the player with the net ID, or NULL without a network.
================
*/
// FUNCTION: STBC 0x006a19f0
// BUG: a net ID that no player has makes the result the address 4 (the name of no player)
TGString *MultiplayerGame::GetPlayerName( int playerID ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return NULL;
	}
	return &network->GetPlayerList()->GetPlayerFromID( playerID )->name;
}

/*
================
MultiplayerGame::DeleteObjectHandler
================
*/
// FUNCTION: STBC 0x006a1a60
void MultiplayerGame::DeleteObjectHandler( TGEvent *event ) {
}

/*
================
MultiplayerGame::ChangedTargetHandler

A client tells the host of its own ship's new target.
================
*/
// FUNCTION: STBC 0x006a1a70
void MultiplayerGame::ChangedTargetHandler( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsHost() && event->GetDestination() == player ) {
		SendEventMessage( event, CHANGED_TARGET_MESSAGE );
	}
}

/*
================
MultiplayerGame::GetShipFromPlayerID

The ship in any set that the player with the net ID flies; NULL when
there is none.
================
*/
// FUNCTION: STBC 0x006a1aa0
ShipClass *MultiplayerGame::GetShipFromPlayerID( int playerID ) {
	SetClass **sets = g_kSetManager.GetSets();
	int numSets = g_kSetManager.GetNumSets();
	for ( int i = 0; i < numSets; i++ ) {
		SetClass *set = sets[i];
		int count;
		ShipClass **ships = ShipClass::GetClassObjects( set, &count );
		for ( int j = 0; j < count; j++ ) {
			ShipClass *ship = ships[j];
			if ( ship->GetNetPlayerID() == playerID ) {
				return ship;
			}
		}
	}
	return NULL;
}

// How far apart the two objects' bounds may be for the host to accept a
// client's collision (invented name).
#define CLIENT_COLLISION_MAX_GAP	26.0f

/*
================
MultiplayerGame::ChecksumCompleteHandler

On the host: a client's files passed the checksums. A name another player
in the game has already gets "(n)" added until it is unique. The client
is then sent GAME_INITIALIZE_MESSAGE (the game time, the collision
setting, the modem flag, its player number, the mission script and the
new name, if any) and GAME_INITIALIZE_DONE_MESSAGE.
================
*/
// FUNCTION: STBC 0x006a1b10
// PARKED: complete, control flow as retail; inline budget: retail calls the player list's Get (0x401830) out of line for the new player (share 72 here, 71 needed: one more site after it, or 8 fewer own-cost units) and expands it in the rename loop; this build expands both
void MultiplayerGame::ChecksumCompleteHandler( TGEvent *event ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL || !g_kUtopiaModule.IsHost() ) {
		return;
	}
	TGPlayerEvent *playerEvent = (TGPlayerEvent *)event;
	int playerNumber = FindPlayerIndex( playerEvent->GetPlayerID() );
	bool renamed = false;
	char buffer[MAX_MESSAGE_SIZE];
	TGNetPlayer *player = network->GetPlayerList()->GetPlayerFromID( playerEvent->GetPlayerID() );
	if ( player != NULL ) {
		TGString name( *player->GetName() );
		int suffix = 1;
		int i;
		do {
			for ( i = 0; i < MAX_PLAYERS; i++ ) {
				if ( i != playerNumber && players[i].inUse ) {
					TGNetPlayer *other = network->GetPlayerList()->GetPlayerFromID( players[i].playerID );
					if ( other != NULL && !other->IsDisconnected() && player->GetName()->Compare( other->GetName(), true ) == 0 ) {
						char *nameString = name.Unknown6F56F0();
						sprintf( buffer, "%s(%d)", nameString, suffix );
						suffix++;
						delete nameString;
						player->GetName()->SetString( buffer );
						renamed = true;
						break;
					}
				}
			}
		} while ( i < MAX_PLAYERS );
	}

	TGBufferStream stream;
	stream.VirtualSlot60( buffer, GAME_INITIALIZE_SIZE );
	stream.WriteChar( GAME_INITIALIZE_MESSAGE );
	stream.WriteFloat( g_kUtopiaModule.GetGameTime() );
	stream.WriteBool( ProximityManager::multiplayerPlayerCollisionsEnabled );
	stream.WriteBool( g_kUtopiaModule.IsModemConnection() );
	stream.WriteChar( playerNumber );
	char *script = GetCurrentEpisode()->GetCurrentMission()->GetScript();
	stream.WriteShort( strlen( script ) );
	stream.Write( script, strlen( script ) );
	stream.WriteBool( renamed );
	if ( renamed ) {
		player->GetName()->WriteToStream( &stream );
	}
	TGMessage *message = new TGMessage;
	message->SetData( buffer, stream.GetPos() );
	message->SetGuaranteed( true );
	network->SendTGMessage( playerEvent->GetPlayerID(), message, 0 );
	stream.Close();

	buffer[0] = GAME_INITIALIZE_DONE_MESSAGE;
	message = new TGMessage;
	message->SetData( buffer, 1 );
	message->SetGuaranteed( true );
	network->SendTGMessage( playerEvent->GetPlayerID(), message, 0 );
}

/*
================
MultiplayerGame::ReceiveClientReadyMessage

On the host: a client has loaded the game. The player's modem flag is
read, ET_NEW_PLAYER_IN_GAME is posted and the mission script's
InitNetwork runs for the player; then the client is sent every networked
object of every set (a player's ship with its player number) and the
objects' damage, and joins the "NoMe" and "Forward" groups.
================
*/
// FUNCTION: STBC 0x006a1e70
void MultiplayerGame::ReceiveClientReadyMessage( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL || !g_kUtopiaModule.IsHost() ) {
		return;
	}
	int playerID = message->GetFromID();
	int index = FindPlayerIndex( playerID );
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	players[index].usingModem = stream.ReadBool();

	TGPlayerEvent *event = new TGPlayerEvent;
	event->SetEventType( ET_NEW_PLAYER_IN_GAME );
	event->SetPlayerID( playerID );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );

	int result;
	if ( PyEmbed::CallFunction( GetCurrentEpisode()->GetCurrentMission()->GetScript(), "InitNetwork", "i", &result, "(i)", playerID ) == -1 ) {
		PyErr_Print();
	}

	int numSets = g_kSetManager.GetNumSets();
	SetClass **sets = g_kSetManager.GetSets();
	for ( int i = 0; i < numSets; i++ ) {
		SetClass *set = sets[i];
		int count;
		PhysicsObjectClass **objects = PhysicsObjectClass::GetClassObjects( set, &count );
		int j;
		for ( j = 0; j < count; j++ ) {
			PhysicsObjectClass *object = objects[j];
			if ( object->GetNetType() == 0 ) {
				continue;
			}
			DamageableObject *damageable = DamageableObject::Cast( object );
			if ( damageable != NULL && ( damageable->IsDying() || damageable->IsDead() ) ) {
				continue;
			}
			char buffer[MAX_MESSAGE_SIZE];
			int size = 2;
			ShipClass *ship = ShipClass::Cast( object );
			if ( ship != NULL && ship->IsPlayerShip() ) {
				buffer[0] = CREATE_PLAYER_OBJECT_MESSAGE;
				size = 3;
			} else {
				buffer[0] = CREATE_OBJECT_MESSAGE;
			}
			buffer[1] = GetPlayerNumberFromID( object->GetObjID() );
			if ( size == 3 ) {
				buffer[2] = ship->GetNetPlayerID();
			}
			size += object->VirtualSlot67( buffer + size, sizeof( buffer ) - size );
			TGMessage *objectMessage = new TGMessage;
			objectMessage->SetData( buffer, size );
			objectMessage->SetGuaranteed( true );
			objectMessage->SetAggregate( false );
			network->SendTGMessage( playerID, objectMessage, 0 );
		}
		for ( j = 0; j < count; j++ ) {
			PhysicsObjectClass *object = objects[j];
			if ( object->GetNetType() != 0 && object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
				( (DamageableObject *)object )->SendDamageVolumes( playerID );
			}
		}
	}

	TGNetGroup *group = network->GetGroup( "NoMe" );
	if ( group != NULL ) {
		group->AddPlayerToGroup( playerID );
	}
	group = network->GetGroup( "Forward" );
	if ( group != NULL ) {
		group->AddPlayerToGroup( playerID );
	}
}

/*
================
MultiplayerGame::ReceiveClientCollisionMessage

On the host: a collision a client reports for its own ship. It is
dropped when the ship isn't one of the two objects, when the client's
ship hit another player's ship, or when the objects' bounds are too far
apart; otherwise it is posted as ET_HOST_OBJECT_COLLISION.
================
*/
// FUNCTION: STBC 0x006a2470
// PARKED: frame slots and x87: retail keeps the message length in a frame slot (ours its argument slot), so the distance and the destination radius trade slots, and retail loads the source radius last from memory
void MultiplayerGame::ReceiveClientCollisionMessage( TGMessage *message ) {
	int length;
	char *data = message->GetData( &length ) + 1;
	TGBufferStream stream;
	stream.VirtualSlot60( data, length - 1 );
	CollisionEvent *event = (CollisionEvent *)TGEvent::CreateFromNetStream( &stream );
	TGObject::FixUpObject( event );
	event->PointsToWorldSpace();
	event->SetUnknown24( NULL );

	ShipClass *ship = GetShipFromPlayerID( message->GetFromID() );
	if ( ship != NULL && ( ship == event->GetDestination() || ship == event->GetSource() ) ) {
		if ( ship == event->GetSource() ) {
			ShipClass *other = ShipClass::Cast( event->GetDestination() );
			if ( other != NULL && other->IsPlayerShip() ) {
				delete event;
				return;
			}
		}
		ObjectClass *destination = (ObjectClass *)event->GetDestination();
		ObjectClass *source = (ObjectClass *)event->GetSource();
		if ( destination != NULL ) {
			bool accepted = true;
			if ( source != NULL ) {
				NiPoint3 location = destination->GetWorldLocation();
				NiPoint3 sourceLocation = source->GetWorldLocation();
				float distance = ( location - sourceLocation ).Length();
				accepted = distance - destination->GetWorldBound().GetRadius() - source->GetWorldBound().GetRadius() < CLIENT_COLLISION_MAX_GAP;
			}
			if ( accepted ) {
				event->SetEventType( ET_HOST_OBJECT_COLLISION );
				g_kEventManager.AddEvent( event );
				return;
			}
		}
	}
	delete event;
}

/*
================
MultiplayerGame::KillGameHandler
================
*/
// FUNCTION: STBC 0x006a2640
void MultiplayerGame::KillGameHandler( TGEvent *event ) {
	Terminate( NULL );
}

/*
================
MultiplayerGame::GetNumberPlayersInGame
================
*/
// FUNCTION: STBC 0x006a2650
int MultiplayerGame::GetNumberPlayersInGame( void ) {
	int count = 0;
	for ( int i = 0; i < MAX_PLAYERS; i++ ) {
		if ( players[i].inUse ) {
			count++;
		}
	}
	return count;
}

/*
================
MultiplayerGame::SetReadyForNewPlayers

Ready, the game reports itself to GameSpy as open and playing (local
unless the "Internet Host" option is set); not ready, as debriefing.
================
*/
// FUNCTION: STBC 0x006a2690
void MultiplayerGame::SetReadyForNewPlayers( bool ready ) {
	readyForNewPlayers = ready;
	if ( ready == true ) {
		GameSpy *gameSpy = g_kUtopiaModule.GetGameSpy();
		if ( gameSpy != NULL ) {
			bool local;
			if ( g_kConfigMapping.HasValue( "Multiplayer Options", "Internet Host" ) ) {
				local = g_kConfigMapping.GetIntValue( "Multiplayer Options", "Internet Host" ) == 0;
			} else {
				local = false;
			}
			gameSpy->InitQueryReporting( local );
			gameSpy->SetGameMode( "openplaying" );
		}
	} else if ( g_kUtopiaModule.GetGameSpy() != NULL ) {
		g_kUtopiaModule.GetGameSpy()->SetGameMode( "debriefing" );
	}
}

/*
================
MultiplayerGame::IsPlayerInGame
================
*/
// FUNCTION: STBC 0x006a2720
bool MultiplayerGame::IsPlayerInGame( int playerID ) {
	for ( int i = 0; i < MAX_PLAYERS; i++ ) {
		if ( players[i].inUse && players[i].playerID == playerID ) {
			return true;
		}
	}
	return false;
}

/*
================
MultiplayerGame::DeletePlayerShipsAndTorps

Drops the game's player, then deletes every torpedo and every ship that
a player flies from the sets.
================
*/
// FUNCTION: STBC 0x006a2750
void MultiplayerGame::DeletePlayerShipsAndTorps( void ) {
	SetPlayer( NULL );
	int numSets = g_kSetManager.GetNumSets();
	SetClass **sets = g_kSetManager.GetSets();
	for ( int i = 0; i < numSets; i++ ) {
		SetClass *set = sets[i];
		int count;
		Torpedo **torpedoes = Torpedo::GetClassObjects( set, &count );
		for ( int j = 0; j < count; j++ ) {
			set->DeleteObjectFromSet( torpedoes[j]->GetObjID() );
		}
		ShipClass **ships = ShipClass::GetClassObjects( set, &count );
		for ( int k = 0; k < count; k++ ) {
			if ( ships[k]->IsPlayerShip() ) {
				set->DeleteObjectFromSet( ships[k]->GetObjID() );
			}
		}
	}
}

/*
================
MultiplayerGame::DeleteObjectFromGame

The host removes the object from its set, or deletes one in no set, and
tells the other players to do the same.
================
*/
// FUNCTION: STBC 0x006a2830 ?DeleteObjectFromGame@MultiplayerGame@@QAEXPAVObjectClass@@@Z
void MultiplayerGame::DeleteObjectFromGame( ObjectClass *object ) {
	if ( object == NULL || !g_kUtopiaModule.IsHost() ) {
		return;
	}
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}
	SetClass *set = object->GetContainingSet();
	long id = object->GetObjID();
	if ( set != NULL ) {
		set->DeleteObjectFromSet( id );
	} else {
		PhysicsObjectClass *physicsObject = PhysicsObjectClass::Cast( object );
		if ( physicsObject != NULL ) {
			physicsObject->ClearAI();
		}
		delete object;
	}

	TGBufferStream stream;
	char buffer[MAX_MESSAGE_SIZE];
	stream.VirtualSlot60( buffer, GAME_INITIALIZE_SIZE );		// the stream is given only a quarter of the buffer
	stream.WriteChar( DELETE_OBJECT_FROM_GAME_MESSAGE );
	stream.WriteID( id );
	TGNetGroup *group = network->GetGroup( "NoMe" );
	TGMessage *message = new TGMessage;
	message->SetData( buffer, stream.GetPos() );
	message->SetGuaranteed( true );
	network->SendTGMessageToGroup( group, message );
	stream.Close();
}

/*
================
MultiplayerGame::IsPlayerUsingModem

The first player's flag when none has the ID.
================
*/
// FUNCTION: STBC 0x006a2a00
bool MultiplayerGame::IsPlayerUsingModem( int playerID ) {
	return players[FindPlayerIndex( playerID )].usingModem;
}

// The times a joining player is tried again before the host gives up on
// it (invented name).
#define MAX_CONNECT_RETRIES		45

/*
================
MultiplayerGame::RetryConnectHandler

The retry of a player who joined before the game took players
(NewPlayerHandler's ET_RETRY_CONNECTION, its source the player's
event): once the game takes players the player's event is handled
again; until then it is tried each second, and after MAX_CONNECT_RETRIES
tries the player is booted as timed out.
================
*/
// FUNCTION: STBC 0x006a2a40
void MultiplayerGame::RetryConnectHandler( TGEvent *event ) {
	TGIntEvent *retryEvent = (TGIntEvent *)event;
	if ( !readyForNewPlayers ) {
		if ( retryEvent->GetInt() < MAX_CONNECT_RETRIES ) {
			retryEvent->SetInt( retryEvent->GetInt() + 1 );
			TGTimer *timer = new TGTimer;
			timer->SetEvent( retryEvent );
			timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + NEW_PLAYER_RETRY_DELAY );
			g_kTimerManager.AddTimer( timer );
			return;
		}

		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		TGPlayerEvent *playerEvent = (TGPlayerEvent *)retryEvent->GetSource();
		if ( network != NULL ) {
			char playerID = playerEvent->GetPlayerID();
			TGBootPlayerMessage *message = new TGBootPlayerMessage;
			message->SetBootReason( TGNetwork::TIMED_OUT );
			message->SetData( &playerID, sizeof( playerID ) );
			network->SendTGMessage( playerEvent->GetPlayerID(), message, 0 );
			playerEvent->DecRefCount();
			retryEvent->SetSource( NULL );
		}
	} else {
		TGPlayerEvent *playerEvent = (TGPlayerEvent *)retryEvent->GetSource();
		retryEvent->SetSource( NULL );
		g_kEventManager.ProcessEvent( playerEvent );
		playerEvent->DecRefCount();
	}
}
