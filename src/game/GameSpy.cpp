/*
===============================================================================

	GameSpy.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "GameSpy.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGLocalizationManager.h"
#include "TGNetwork.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGWinsockNetwork.h"
#include "TGLocalizationDatabase.h"
#include "TGEvent.h"
#include "TGStringEvent.h"
#include "TGNetPlayer.h"
#include "TGPlayerList.h"
#include "TopWindow.h"
#include "MainWindow.h"
#include "MultiplayerWindow.h"
#include "MultiplayerGame.h"
#include "Game.h"
#include "UtopiaModule.h"
#include "VarManagerClass.h"
#include "PyEmbed.h"
// Last: the SDK's header brings in <windows.h>, whose ANSI/wide macros would
// rename the game's methods the headers above declare.
#include "gqueryreporting.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0069b580 _$E32
// FUNCTION: STBC 0x0069b590 _$E29
// FUNCTION: STBC 0x0069b5a0 _$E31
// FUNCTION: STBC 0x0069b5b0 _$E30
// FUNCTION: STBC 0x0069b5c0 _$E38
// FUNCTION: STBC 0x0069b5d0 _$E35
// FUNCTION: STBC 0x0069b5e0 _$E37
// FUNCTION: STBC 0x0069b5f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0069b600 _$E61
// FUNCTION: STBC 0x0069b610 _$E60
// FUNCTION: STBC 0x0069b620 _$E65
// FUNCTION: STBC 0x0069b630 _$E64
// FUNCTION: STBC 0x0069b640 _$E76
// FUNCTION: STBC 0x0069b650 _$E75
// FUNCTION: STBC 0x0069b660 _$E80
// FUNCTION: STBC 0x0069b670 _$E79
// FUNCTION: STBC 0x0069b680 _$E84
// FUNCTION: STBC 0x0069b690 _$E83
// FUNCTION: STBC 0x0069b6a0 _$E88
// FUNCTION: STBC 0x0069b6b0 _$E87
// FUNCTION: STBC 0x0069b6c0 _$E212
// FUNCTION: STBC 0x0069b6d0 _$E211
// FUNCTION: STBC 0x0069b6e0 _$E154
// FUNCTION: STBC 0x0069b6f0 _$E153
// FUNCTION: STBC 0x0069b700 _$E167
// FUNCTION: STBC 0x0069b710 _$E166
// FUNCTION: STBC 0x0069b720 _$E217
// FUNCTION: STBC 0x0069b730 _$E216
// FUNCTION: STBC 0x0069b740 _$E222
// FUNCTION: STBC 0x0069b750 _$E221
// FUNCTION: STBC 0x0069b760 _$E226
// FUNCTION: STBC 0x0069b770 _$E225
// FUNCTION: STBC 0x0069b780 _$E230
// FUNCTION: STBC 0x0069b790 _$E229
// FUNCTION: STBC 0x0069b7a0 _$E237
// FUNCTION: STBC 0x0069b7b0 _$E236
// FUNCTION: STBC 0x0069b7c0 _$E262
// FUNCTION: STBC 0x0069b7d0 _$E261
// FUNCTION: STBC 0x0069b7e0 _$E266
// FUNCTION: STBC 0x0069b7f0 _$E265
// FUNCTION: STBC 0x0069b800 _$E112
// FUNCTION: STBC 0x0069b810 _$E111
// FUNCTION: STBC 0x0069b820 _$E248
// FUNCTION: STBC 0x0069b830 _$E247
// FUNCTION: STBC 0x0069b840 _$E255
// FUNCTION: STBC 0x0069b850 _$E254
// FUNCTION: STBC 0x0069b860 _$E278
// FUNCTION: STBC 0x0069b870 _$E277
// FUNCTION: STBC 0x0069b880 _$E282
// FUNCTION: STBC 0x0069b890 _$E281
// FUNCTION: STBC 0x0069b8a0 _$E322
// FUNCTION: STBC 0x0069b8b0 _$E321
// FUNCTION: STBC 0x0069b8c0 _$E328
// FUNCTION: STBC 0x0069b8d0 _$E327
// FUNCTION: STBC 0x0069b8e0 _$E270
// FUNCTION: STBC 0x0069b8f0 _$E269
// FUNCTION: STBC 0x0069b900 _$E274
// FUNCTION: STBC 0x0069b910 _$E273
// FUNCTION: STBC 0x0069b920 _$E286
// FUNCTION: STBC 0x0069b930 _$E285
// FUNCTION: STBC 0x0069b940 _$E290
// FUNCTION: STBC 0x0069b950 _$E289
// FUNCTION: STBC 0x0069b960 _$E294
// FUNCTION: STBC 0x0069b970 _$E293
// FUNCTION: STBC 0x0069b980 _$E298
// FUNCTION: STBC 0x0069b990 _$E297
// FUNCTION: STBC 0x0069b9a0 _$E302
// FUNCTION: STBC 0x0069b9b0 _$E301
// FUNCTION: STBC 0x0069b9c0 _$E306
// FUNCTION: STBC 0x0069b9d0 _$E305
// FUNCTION: STBC 0x0069b9e0 _$E310
// FUNCTION: STBC 0x0069b9f0 _$E309
// FUNCTION: STBC 0x0069ba30 _$E314
// FUNCTION: STBC 0x0069ba40 _$E313
// FUNCTION: STBC 0x0069ba50 _$E117
// FUNCTION: STBC 0x0069ba60 _$E116
// FUNCTION: STBC 0x0069ba70 _$E318
// FUNCTION: STBC 0x0069ba80 _$E317
// FUNCTION: STBC 0x0069ba90 _$E122
// FUNCTION: STBC 0x0069baa0 _$E121
// FUNCTION: STBC 0x0069bab0 _$E332
// FUNCTION: STBC 0x0069bac0 _$E331
// FUNCTION: STBC 0x0069bad0 _$E336
// FUNCTION: STBC 0x0069bae0 _$E335
// FUNCTION: STBC 0x0069baf0 _$E340
// FUNCTION: STBC 0x0069bb00 _$E339
// FUNCTION: STBC 0x0069bb10 _$E344
// FUNCTION: STBC 0x0069bb20 _$E343
// FUNCTION: STBC 0x0069bb30 _$E131
// FUNCTION: STBC 0x0069bb40 _$E130
// FUNCTION: STBC 0x0069bb50 _$E348
// FUNCTION: STBC 0x0069bb60 _$E347
// FUNCTION: STBC 0x0069bb70 _$E352
// FUNCTION: STBC 0x0069bb80 _$E351
// FUNCTION: STBC 0x0069bb90 _$E356
// FUNCTION: STBC 0x0069bba0 _$E355
// FUNCTION: STBC 0x0069bbb0 _$E137
// FUNCTION: STBC 0x0069bbc0 _$E136
// FUNCTION: STBC 0x0069bbd0 _$E142
// FUNCTION: STBC 0x0069bbe0 _$E141
// FUNCTION: STBC 0x0069bbf0 _$E172
// FUNCTION: STBC 0x0069bc00 _$E171
// FUNCTION: STBC 0x0069bc10 _$E360
// FUNCTION: STBC 0x0069bc20 _$E359
// FUNCTION: STBC 0x0069bc30 _$E177
// FUNCTION: STBC 0x0069bc40 _$E176
// FUNCTION: STBC 0x0069bc50 _$E364
// FUNCTION: STBC 0x0069bc60 _$E363
// FUNCTION: STBC 0x0069bc70 _$E368
// FUNCTION: STBC 0x0069bc80 _$E367
// FUNCTION: STBC 0x0069bc90 _$E182
// FUNCTION: STBC 0x0069bca0 _$E181
// FUNCTION: STBC 0x0069bcb0 _$E372
// FUNCTION: STBC 0x0069bcc0 _$E371
// FUNCTION: STBC 0x0069bcd0 _$E147
// FUNCTION: STBC 0x0069bce0 _$E146
// FUNCTION: STBC 0x0069bcf0 _$E376
// FUNCTION: STBC 0x0069bd00 _$E375
// FUNCTION: STBC 0x0069bd10 _$E380
// FUNCTION: STBC 0x0069bd20 _$E379
// FUNCTION: STBC 0x0069bd30 _$E187
// FUNCTION: STBC 0x0069bd40 _$E186
// FUNCTION: STBC 0x0069bd50 _$E192
// FUNCTION: STBC 0x0069bd60 _$E191
// FUNCTION: STBC 0x0069bd70 _$E384
// FUNCTION: STBC 0x0069bd80 _$E383
// FUNCTION: STBC 0x0069bd90 _$E388
// FUNCTION: STBC 0x0069bda0 _$E387
// FUNCTION: STBC 0x0069bdb0 _$E392
// FUNCTION: STBC 0x0069bdc0 _$E391
// FUNCTION: STBC 0x0069bdd0 _$E396
// FUNCTION: STBC 0x0069bde0 _$E395
// FUNCTION: STBC 0x0069bdf0 _$E400
// FUNCTION: STBC 0x0069be00 _$E399
// FUNCTION: STBC 0x0069be10 _$E404
// FUNCTION: STBC 0x0069be20 _$E403
// FUNCTION: STBC 0x0069be30 _$E408
// FUNCTION: STBC 0x0069be40 _$E407
// FUNCTION: STBC 0x0069be50 _$E412
// FUNCTION: STBC 0x0069be60 _$E411
// FUNCTION: STBC 0x0069be70 _$E197
// FUNCTION: STBC 0x0069be80 _$E196
// FUNCTION: STBC 0x0069be90 _$E416
// FUNCTION: STBC 0x0069bea0 _$E415
// FUNCTION: STBC 0x0069beb0 _$E202
// FUNCTION: STBC 0x0069bec0 _$E201
// FUNCTION: STBC 0x0069bed0 _$E207
// FUNCTION: STBC 0x0069bee0 _$E206
// FUNCTION: STBC 0x0069bef0 _$E420
// FUNCTION: STBC 0x0069bf00 _$E419
// FUNCTION: STBC 0x0069bf10 _$E425
// FUNCTION: STBC 0x0069bf20 _$E424
// FUNCTION: STBC 0x0069bf30 _$E429
// FUNCTION: STBC 0x0069bf40 _$E428

/*
================
GameSpy::GameSpy

Starts in the "settings" mode and listens for the network's GameSpy messages.
================
*/
// FUNCTION: STBC 0x0069bfa0
GameSpy::GameSpy( long id ) : TGEventHandlerObject( id ) {
	listIdle = true;
	started = false;
	hosting = true;
	lastUpdateTime = 0;
	local = false;
	sortAscending = false;
	database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	strcpy( gameMode, "settings" );
	queryReporting = NULL;
	serverList = NULL;
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_GAMESPY_MESSAGE, this, "GameSpy :: ProcessQueryHandler", ANY_TARGET );
}

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x0069bf50 _$E483
// FUNCTION: STBC 0x0069bf60 _$E479
// FUNCTION: STBC 0x0069bf70 _$E482
// FUNCTION: STBC 0x0069bf80 _$E480
// DATA: STBC 0x00994688
TGEventHandlerTable GameSpy::classHandlers;

/*
================
GameSpy::~GameSpy

Tells the master server a hosted game is going away, or frees the server
list.
================
*/
// FUNCTION: STBC 0x0069c140
GameSpy::~GameSpy( void ) {
	if ( database != NULL ) {
		g_kLocalizationManager.Unload( database );
		database = NULL;
	}
	g_kEventManager.RemoveAllBroadcastHandlersForObject( this );
	if ( hosting && queryReporting != NULL ) {
		strcpy( gameMode, "exiting" );
		qr_send_statechanged( queryReporting );
		qr_shutdown( queryReporting );
	} else if ( serverList != NULL ) {
		ServerListFree( serverList );
		serverList = NULL;
	}
}

/*
================
GameSpy::InitQueryReporting

Reports the hosted game on the network's own socket; the network passes
the queries that arrive on it as ET_NETWORK_GAMESPY_MESSAGE events.
================
*/
// FUNCTION: STBC 0x0069c240
bool GameSpy::InitQueryReporting( bool local ) {
	if ( started == true ) {
		return false;
	}
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return false;
	}

	// The game's secret key, built here so that it isn't one string in the binary.
	char secretKey[9];
	secretKey[0] = 'N';
	secretKey[1] = 'm';
	secretKey[2] = '3';
	secretKey[3] = 'a';
	secretKey[4] = 'Z';
	secretKey[5] = '9';
	secretKey[6] = '\0';
	if ( qr_init_socket( &queryReporting, ( (TGWinsockNetwork *)network )->GetSocket(), GAMESPY_GAME_NAME, secretKey,
			OnBasicQuery, OnInfoQuery, OnRulesQuery, OnPlayersQuery, this ) != E_GOA_NOERROR ) {
		return false;
	}
	strcpy( gameMode, "openplaying" );
	started = true;
	hosting = true;
	this->local = local;
	return true;
}

/*
================
GameSpy::OnBasicQuery

The query reporting's callbacks pass the queries to the GameSpy.
================
*/
// FUNCTION: STBC 0x0069c320
void GameSpy::OnBasicQuery( char *outbuf, int maxlen, void *userdata ) {
	( (GameSpy *)userdata )->BasicCallback( outbuf, maxlen );
}

// FUNCTION: STBC 0x0069c340
void GameSpy::OnInfoQuery( char *outbuf, int maxlen, void *userdata ) {
	( (GameSpy *)userdata )->InfoCallback( outbuf, maxlen );
}

// FUNCTION: STBC 0x0069c360
void GameSpy::OnRulesQuery( char *outbuf, int maxlen, void *userdata ) {
	( (GameSpy *)userdata )->RulesCallback( outbuf, maxlen );
}

// FUNCTION: STBC 0x0069c380
void GameSpy::OnPlayersQuery( char *outbuf, int maxlen, void *userdata ) {
	( (GameSpy *)userdata )->PlayersCallback( outbuf, maxlen );
}

/*
================
GameSpy::CreateServerList

A server list of up to 10 concurrent queries; false when the reporting or
a list runs already.
================
*/
// FUNCTION: STBC 0x0069c3a0
bool GameSpy::CreateServerList( void ) {
	if ( started == true ) {
		return false;
	}

	// The game's secret key, built here so that it isn't one string in the binary.
	char secretKey[9];
	secretKey[0] = 'N';
	secretKey[1] = 'm';
	secretKey[2] = '3';
	secretKey[3] = 'a';
	secretKey[4] = 'Z';
	secretKey[5] = '9';
	secretKey[6] = '\0';
	serverList = ServerListNew( GAMESPY_GAME_NAME, GAMESPY_GAME_NAME, secretKey, 10, OnServerList, 1, this );
	started = true;
	hosting = false;
	return true;
}

/*
================
GameSpy::OnServerList

The server list's callback passes what the list does to the GameSpy.
================
*/
// FUNCTION: STBC 0x0069c420
void GameSpy::OnServerList( GServerList serverList, int msg, void *instance, void *param1, void *param2 ) {
	( (GameSpy *)instance )->ServerListCallBack( serverList, msg, param1, param2 );
}

/*
================
GameSpy::VirtualSlot21

A host answers the waiting queries (and sends the heartbeats unless the
game is local); a client lets its server list work at most every 0.02
seconds while an update runs.
================
*/
// FUNCTION: STBC 0x0069c440
void GameSpy::VirtualSlot21( float time ) {
	if ( !started ) {
		return;
	}
	if ( hosting ) {
		if ( local ) {
			qr_process_queries_no_heartbeat( queryReporting );
		} else {
			qr_process_queries( queryReporting );
		}
	} else if ( time - lastUpdateTime > 0.02 && !listIdle ) {
		ServerListThink( serverList );
	}
	lastUpdateTime = time;
}

/*
================
GameSpy::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0069c4e0
void GameSpy::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&GameSpy::SetGameModeHandler, "GameSpy :: SetGameModeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&GameSpy::ProcessQueryHandler, "GameSpy :: ProcessQueryHandler" );
}

/*
================
GameSpy::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0069c510
void GameSpy::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SET_GAME_MODE, "GameSpy :: SetGameModeHandler" );
}

/*
================
GameSpy::BasicCallback

The basic query: the game's name, its version and the location.
================
*/
// FUNCTION: STBC 0x0069c550
void GameSpy::BasicCallback( char *outbuf, int maxlen ) {
	sprintf( outbuf, "\\gamename\\%s\\gamever\\%s\\location\\%d", GAMESPY_GAME_NAME, UtopiaModule::GetGameVersion(), 1 );
}

/*
================
GameSpy::InfoCallback

The info query: the game's name (marked with '*' when it has a password),
the mission script and its short name, the players and the game mode.
================
*/
// FUNCTION: STBC 0x0069c580
void GameSpy::InfoCallback( char *outbuf, int maxlen ) {
	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	MultiplayerGame *game = MultiplayerGame::Cast( Game::GetCurrentGame() );
	if ( window == NULL || network == NULL || game == NULL ) {
		return;
	}

	int password = 0;
	if ( g_kUtopiaModule.GetNetwork() != NULL ) {
		TGString *networkPassword = g_kUtopiaModule.GetNetwork()->GetPassword();
		if ( networkPassword != NULL && networkPassword->GetLength() > 0 ) {
			password = 1;
		}
	}

	char keyValue[200];
	TGString gameName;
	( (MultiplayerWindow *)window )->GetGameName( &gameName );
	char *name = gameName.Unknown6F56F0();
	if ( password ) {
		sprintf( keyValue, "\\hostname\\*%s", name );
	} else {
		sprintf( keyValue, "\\hostname\\%s", name );
	}
	strcat( outbuf, keyValue );
	delete name;

	sprintf( keyValue, "\\missionscript\\%s", g_kVarManager.GetStringVariable( "Multiplayer", "Mission" ) );
	strcat( outbuf, keyValue );

	char module[1024];
	PyObject *shortName;
	sprintf( module, "%sName", g_kVarManager.GetStringVariable( "Multiplayer", "Mission" ) );
	if ( PyEmbed::CallFunction( module, "GetMissionShortName", "O", &shortName, "()" ) == -1 ) {
		PyErr_Print();
		sprintf( keyValue, "\\mapname\\%s", g_kVarManager.GetStringVariable( "Multiplayer", "Mission" ) );
	} else {
		// BUG: the short name's reference is never released
		char *mapName = ( (TGString *)PyEmbed::GetObjectPointer( shortName ) )->Unknown6F56F0();
		sprintf( keyValue, "\\mapname\\%s", mapName );
		delete mapName;
	}
	strcat( outbuf, keyValue );

	sprintf( keyValue, "\\numplayers\\%d", game->GetNumberPlayersInGame() );
	strcat( outbuf, keyValue );

	int playerLimit = 16;
	PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iPlayerLimit", "i", &playerLimit );
	PyErr_Clear();
	sprintf( keyValue, "\\maxplayers\\%d", playerLimit );
	strcat( outbuf, keyValue );

	sprintf( keyValue, "\\gamemode\\%s", gameMode );
	strcat( outbuf, keyValue );
}

/*
================
GameSpy::RulesCallback

The rules query: the time and frag limits (a score limit is sent as the
frag limit times 10000, -2 without one), the species system and whether
the game has a password.
================
*/
// FUNCTION: STBC 0x0069c8e0
void GameSpy::RulesCallback( char *outbuf, int maxlen ) {
	int timeLimit = -1;
	PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iTimeLimit", "i", &timeLimit );
	PyErr_Clear();
	int fragLimit = -1;
	PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iFragLimit", "i", &fragLimit );
	PyErr_Clear();
	int useScoreLimit = 0;
	PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iUseScoreLimit", "i", &useScoreLimit );
	PyErr_Clear();
	if ( useScoreLimit ) {
		if ( fragLimit >= 0 ) {
			fragLimit *= 10000;
		} else {
			fragLimit = -2;
		}
	}
	int system = 0;
	PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iSystem", "i", &system );
	PyErr_Clear();

	char *script = NULL;
	PyObject *result = NULL;
	if ( PyEmbed::CallFunction( "Multiplayer.SpeciesToSystem", "GetScriptFromSpecies", "O", &result, "(i)", system ) != -1 ) {
		script = PyString_AsString( result );
	}
	PyErr_Clear();

	int password = 0;
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL ) {
		TGString *networkPassword = network->GetPassword();
		if ( networkPassword != NULL && networkPassword->GetLength() > 0 ) {
			password = 1;
		}
	}
	sprintf( outbuf, "\\timelimit\\%d\\fraglimit\\%d\\system\\%s\\password\\%d", timeLimit, fragLimit, script, password );
	Py_XDECREF( result );
}

/*
================
GameSpy::PlayersCallback

The players query: the name of each player in the game; a dedicated
server sends "Dedicated Server" for its own player.
================
*/
// FUNCTION: STBC 0x0069ca60
// DRAFT: retail keeps the host-and-client branch apart from the other players' (ours merges their tails) and reads the count through the player list's address
void GameSpy::PlayersCallback( char *outbuf, int maxlen ) {
	MultiplayerGame *game = MultiplayerGame::Cast( Game::GetCurrentGame() );
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( game == NULL || network == NULL || network->GetPlayerList() == NULL ) {
		return;
	}

	char keyValue[160];
	int numPlayers = network->GetPlayerList()->GetNumPlayers();
	int playerIndex = 0;
	for ( int i = 0; i < numPlayers; i++ ) {
		TGNetPlayer *player = network->GetPlayerList()->GetPlayerAtIndex( i );
		if ( player == NULL ) {
			continue;
		}
		if ( game->IsPlayerInGame( player->GetNetID() ) ) {
			if ( player->GetNetID() == network->GetLocalID() ) {
				if ( g_kUtopiaModule.IsHost() && g_kUtopiaModule.IsClient() ) {
					if ( player->GetName() != NULL ) {
						char *cName = player->GetName()->Unknown6F56F0();
						sprintf( keyValue, "\player_%d\%s", playerIndex, cName );
						delete cName;
						strcat( outbuf, keyValue );
						playerIndex++;
					}
				} else {
					// BUG: the database is never unloaded
					TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
					if ( database != NULL ) {
						TGString *dedicated = database->GetString( "Dedicated Server" );
						if ( dedicated != NULL ) {
							char *cName = dedicated->Unknown6F56F0();
							sprintf( keyValue, "\player_%d\%s", playerIndex, cName );
							delete cName;
							strcat( outbuf, keyValue );
							playerIndex++;
						}
					}
				}
			} else {
				if ( player->GetName() != NULL ) {
					char *cName = player->GetName()->Unknown6F56F0();
					sprintf( keyValue, "\player_%d\%s", playerIndex, cName );
					delete cName;
					strcat( outbuf, keyValue );
					playerIndex++;
				}
			}
		} else if ( player->GetNetID() == network->GetLocalID() && g_kUtopiaModule.IsHost() && !g_kUtopiaModule.IsClient() ) {
			// BUG: the database is never unloaded
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
			if ( database != NULL ) {
				TGString *dedicated = database->GetString( "Dedicated Server" );
				if ( dedicated != NULL ) {
					char *cName = dedicated->Unknown6F56F0();
					sprintf( keyValue, "\player_%d\%s", playerIndex, cName );
					delete cName;
					strcat( outbuf, keyValue );
					playerIndex++;
				}
			}
		}
	}
}

/*
================
GameSpy::SetGameModeHandler

Sets the mode the queries report from the event's string.
================
*/
// FUNCTION: STBC 0x0069cc40
void GameSpy::SetGameModeHandler( TGEvent *event ) {
	if ( event->IsTypeOf( TGSTRINGEVENT_TYPE ) ) {
		char *mode = ( (TGStringEvent *)event )->GetCString();
		strcpy( gameMode, mode );
		delete mode;
	}
}

/*
================
GameSpy::SetGameMode

Sets the mode the queries report.
================
*/
// FUNCTION: STBC 0x0069cca0
void GameSpy::SetGameMode( char *mode ) {
	strcpy( gameMode, mode );
}

/*
================
GameSpy::RefreshServerList

Asks the master server (or the LAN's ports 22101-22201) for the servers;
when that fails the multiplayer window gets the end of the refresh and
"Failed GameSpy Connection", and the list stops.
================
*/
// FUNCTION: STBC 0x0069ccd0
void GameSpy::RefreshServerList( bool lan ) {
	CreateServerList();
	ServerListClear( serverList );
	GError error;
	if ( lan ) {
		error = ServerListLANUpdate( serverList, true, GAMESPY_LAN_FIRST_PORT, GAMESPY_LAN_LAST_PORT, 1 );
	} else {
		error = ServerListUpdate( serverList, true );
	}
	if ( error != GE_NOERROR ) {
		MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
		if ( window != NULL ) {
			TGEvent *done = new TGEvent;
			done->SetEventType( ET_REFRESH_SERVER_LIST_DONE );
			done->SetDestination( window );
			g_kEventManager.AddEvent( done );

			TGString *failed = database->GetString( "Failed GameSpy Connection" );
			if ( failed != NULL ) {
				TGStringEvent *state = new TGStringEvent;
				state->SetEventType( ET_SERVER_LIST_STATE );
				state->SetDestination( window );
				TGString text = *failed;
				state->SetString( text );
				g_kEventManager.AddEvent( state );
			}
		}
		ServerListHalt( serverList );
	} else {
		listIdle = false;
	}
}
