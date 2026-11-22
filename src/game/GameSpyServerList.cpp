/*
===============================================================================

	GameSpyServerList.cpp

	GameSpy's server list side: what the SDK's server list reports, passed
	to the multiplayer window as events, and the queries of a hosted game
	the network hands over. The file name is invented: retail's second
	GameSpy object starts at 0x69CEB0 with its <iostream> initialisers.

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "GameSpy.h"
#include "gqueryreporting.h"

// gqueryreporting.h brings in <windows.h> (through nonport.h), whose ANSI/wide
// macro would rename TGEvent's method in the event headers below.
#undef GetClassName

#include "MainWindow.h"
#include "ServerListEvent.h"
#include "TGEventManager.h"
#include "TGGameSpyEvent.h"
#include "TGIntEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGStringEvent.h"
#include "TopWindow.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0069ddc0 _$E452
// FUNCTION: STBC 0x0069dde0 _$E451

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0069d760 _$E58
// FUNCTION: STBC 0x0069d770 _$E57
// FUNCTION: STBC 0x0069d780 _$E62
// FUNCTION: STBC 0x0069d790 _$E61
// FUNCTION: STBC 0x0069d7a0 _$E73
// FUNCTION: STBC 0x0069d7b0 _$E72
// FUNCTION: STBC 0x0069d7c0 _$E77
// FUNCTION: STBC 0x0069d7d0 _$E76
// FUNCTION: STBC 0x0069d7e0 _$E81
// FUNCTION: STBC 0x0069d7f0 _$E80
// FUNCTION: STBC 0x0069d800 _$E85
// FUNCTION: STBC 0x0069d810 _$E84
// FUNCTION: STBC 0x0069d820 _$E209
// FUNCTION: STBC 0x0069d830 _$E208
// FUNCTION: STBC 0x0069d840 _$E151
// FUNCTION: STBC 0x0069d850 _$E150
// FUNCTION: STBC 0x0069d860 _$E164
// FUNCTION: STBC 0x0069d870 _$E163
// FUNCTION: STBC 0x0069d880 _$E214
// FUNCTION: STBC 0x0069d890 _$E213
// FUNCTION: STBC 0x0069d8a0 _$E219
// FUNCTION: STBC 0x0069d8b0 _$E218
// FUNCTION: STBC 0x0069d8c0 _$E223
// FUNCTION: STBC 0x0069d8d0 _$E222
// FUNCTION: STBC 0x0069d8e0 _$E227
// FUNCTION: STBC 0x0069d8f0 _$E226
// FUNCTION: STBC 0x0069d900 _$E234
// FUNCTION: STBC 0x0069d910 _$E233
// FUNCTION: STBC 0x0069d920 _$E277
// FUNCTION: STBC 0x0069d930 _$E276
// FUNCTION: STBC 0x0069d940 _$E281
// FUNCTION: STBC 0x0069d950 _$E280
// FUNCTION: STBC 0x0069d960 _$E109
// FUNCTION: STBC 0x0069d970 _$E108
// FUNCTION: STBC 0x0069d980 _$E263
// FUNCTION: STBC 0x0069d990 _$E262
// FUNCTION: STBC 0x0069d9a0 _$E270
// FUNCTION: STBC 0x0069d9b0 _$E269
// FUNCTION: STBC 0x0069d9c0 _$E293
// FUNCTION: STBC 0x0069d9d0 _$E292
// FUNCTION: STBC 0x0069d9e0 _$E297
// FUNCTION: STBC 0x0069d9f0 _$E296
// FUNCTION: STBC 0x0069da00 _$E337
// FUNCTION: STBC 0x0069da10 _$E336
// FUNCTION: STBC 0x0069da20 _$E343
// FUNCTION: STBC 0x0069da30 _$E342
// FUNCTION: STBC 0x0069da40 _$E285
// FUNCTION: STBC 0x0069da50 _$E284
// FUNCTION: STBC 0x0069da60 _$E289
// FUNCTION: STBC 0x0069da70 _$E288
// FUNCTION: STBC 0x0069da80 _$E301
// FUNCTION: STBC 0x0069da90 _$E300
// FUNCTION: STBC 0x0069daa0 _$E305
// FUNCTION: STBC 0x0069dab0 _$E304
// FUNCTION: STBC 0x0069dac0 _$E309
// FUNCTION: STBC 0x0069dad0 _$E308
// FUNCTION: STBC 0x0069dae0 _$E313
// FUNCTION: STBC 0x0069daf0 _$E312
// FUNCTION: STBC 0x0069db00 _$E317
// FUNCTION: STBC 0x0069db10 _$E316
// FUNCTION: STBC 0x0069db20 _$E321
// FUNCTION: STBC 0x0069db30 _$E320
// FUNCTION: STBC 0x0069db40 _$E325
// FUNCTION: STBC 0x0069db50 _$E324
// FUNCTION: STBC 0x0069db60 _$E329
// FUNCTION: STBC 0x0069db70 _$E328
// FUNCTION: STBC 0x0069db80 _$E114
// FUNCTION: STBC 0x0069db90 _$E113
// FUNCTION: STBC 0x0069dba0 _$E333
// FUNCTION: STBC 0x0069dbb0 _$E332
// FUNCTION: STBC 0x0069dbc0 _$E119
// FUNCTION: STBC 0x0069dbd0 _$E118
// FUNCTION: STBC 0x0069dbe0 _$E347
// FUNCTION: STBC 0x0069dbf0 _$E346
// FUNCTION: STBC 0x0069dc00 _$E351
// FUNCTION: STBC 0x0069dc10 _$E350
// FUNCTION: STBC 0x0069dc20 _$E355
// FUNCTION: STBC 0x0069dc30 _$E354
// FUNCTION: STBC 0x0069dc40 _$E359
// FUNCTION: STBC 0x0069dc50 _$E358
// FUNCTION: STBC 0x0069dc60 _$E128
// FUNCTION: STBC 0x0069dc70 _$E127
// FUNCTION: STBC 0x0069dc80 _$E363
// FUNCTION: STBC 0x0069dc90 _$E362
// FUNCTION: STBC 0x0069dca0 _$E367
// FUNCTION: STBC 0x0069dcb0 _$E366
// FUNCTION: STBC 0x0069dcc0 _$E371
// FUNCTION: STBC 0x0069dcd0 _$E370
// FUNCTION: STBC 0x0069dce0 _$E134
// FUNCTION: STBC 0x0069dcf0 _$E133
// FUNCTION: STBC 0x0069dd00 _$E139
// FUNCTION: STBC 0x0069dd10 _$E138
// FUNCTION: STBC 0x0069dd20 _$E169
// FUNCTION: STBC 0x0069dd30 _$E168
// FUNCTION: STBC 0x0069dd40 _$E375
// FUNCTION: STBC 0x0069dd50 _$E374
// FUNCTION: STBC 0x0069dd60 _$E174
// FUNCTION: STBC 0x0069dd70 _$E173
// FUNCTION: STBC 0x0069dd80 _$E379
// FUNCTION: STBC 0x0069dd90 _$E378
// FUNCTION: STBC 0x0069dda0 _$E383
// FUNCTION: STBC 0x0069ddb0 _$E382
// FUNCTION: STBC 0x0069ddf0 _$E179
// FUNCTION: STBC 0x0069de00 _$E178
// FUNCTION: STBC 0x0069de10 _$E387
// FUNCTION: STBC 0x0069de20 _$E386
// FUNCTION: STBC 0x0069de30 _$E144
// FUNCTION: STBC 0x0069de40 _$E143
// FUNCTION: STBC 0x0069de50 _$E391
// FUNCTION: STBC 0x0069de60 _$E390
// FUNCTION: STBC 0x0069de70 _$E395
// FUNCTION: STBC 0x0069de80 _$E394
// FUNCTION: STBC 0x0069de90 _$E184
// FUNCTION: STBC 0x0069dea0 _$E183
// FUNCTION: STBC 0x0069deb0 _$E189
// FUNCTION: STBC 0x0069dec0 _$E188
// FUNCTION: STBC 0x0069ded0 _$E399
// FUNCTION: STBC 0x0069dee0 _$E398
// FUNCTION: STBC 0x0069def0 _$E403
// FUNCTION: STBC 0x0069df00 _$E402
// FUNCTION: STBC 0x0069df10 _$E407
// FUNCTION: STBC 0x0069df20 _$E406
// FUNCTION: STBC 0x0069df30 _$E411
// FUNCTION: STBC 0x0069df40 _$E410
// FUNCTION: STBC 0x0069df50 _$E415
// FUNCTION: STBC 0x0069df60 _$E414
// FUNCTION: STBC 0x0069df70 _$E419
// FUNCTION: STBC 0x0069df80 _$E418
// FUNCTION: STBC 0x0069df90 _$E423
// FUNCTION: STBC 0x0069dfa0 _$E422
// FUNCTION: STBC 0x0069dfb0 _$E427
// FUNCTION: STBC 0x0069dfc0 _$E426
// FUNCTION: STBC 0x0069dfd0 _$E194
// FUNCTION: STBC 0x0069dfe0 _$E193
// FUNCTION: STBC 0x0069dff0 _$E431
// FUNCTION: STBC 0x0069e000 _$E430
// FUNCTION: STBC 0x0069e010 _$E199
// FUNCTION: STBC 0x0069e020 _$E198
// FUNCTION: STBC 0x0069e030 _$E204
// FUNCTION: STBC 0x0069e040 _$E203
// FUNCTION: STBC 0x0069e050 _$E435
// FUNCTION: STBC 0x0069e060 _$E434
// FUNCTION: STBC 0x0069e070 _$E440
// FUNCTION: STBC 0x0069e080 _$E439
// FUNCTION: STBC 0x0069e090 _$E444
// FUNCTION: STBC 0x0069e0a0 _$E443

// The <iostream> initialisers (ios_base::Init and _Winit) that open the object.
// FUNCTION: STBC 0x0069ceb0 _$E32
// FUNCTION: STBC 0x0069cec0 _$E29
// FUNCTION: STBC 0x0069ced0 _$E31
// FUNCTION: STBC 0x0069cee0 _$E30
// FUNCTION: STBC 0x0069cef0 _$E38
// FUNCTION: STBC 0x0069cf00 _$E35
// FUNCTION: STBC 0x0069cf10 _$E37
// FUNCTION: STBC 0x0069cf20 _$E36

/*
================
GameSpy::ServerListCallBack

A server answered (LIST_PROGRESS: the server and the percentage done go to
the multiplayer window with the list's state), or the list's state changed.
================
*/
// FUNCTION: STBC 0x0069cf30
void GameSpy::ServerListCallBack( GServerList serverList, int msg, void *param1, void *param2 ) {
	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window == NULL ) {
		return;
	}

	TGString *text;
	if ( msg == LIST_PROGRESS ) {
		ServerListEvent *entry = new ServerListEvent;
		entry->SetEventType( ET_SERVER_ENTRY_EVENT );
		entry->SetDestination( window );
		entry->SetServer( (GServer)param1 );
		g_kEventManager.AddEvent( entry );

		switch ( ServerListState( serverList ) ) {
			case sl_idle:
				text = database->GetString( "Done Updating List" );
				break;
			case sl_listxfer:
				text = database->GetString( "Getting List" );
				break;
			case sl_lanlist:
				text = database->GetString( "Searching LAN" );
				break;
			case sl_querying:
				text = database->GetString( "Querying Servers" );
				break;
			default:
				return;
		}
		if ( text == NULL ) {
			return;
		}

		TGIntEvent *percentage = new TGIntEvent;
		percentage->SetEventType( ET_SERVER_LIST_PERCENTAGE );
		percentage->SetDestination( window );
		percentage->SetInt( (int)param2 );
		g_kEventManager.AddEvent( percentage );

		TGStringEvent *state = new TGStringEvent;
		state->SetEventType( ET_SERVER_LIST_STATE );
		state->SetDestination( window );
		state->SetString( text->Unknown6F56F0() );	// BUG: the narrowed copy is never freed
		g_kEventManager.AddEvent( state );
		return;
	}

	switch ( ServerListState( serverList ) ) {
		case sl_idle: {
			text = database->GetString( "Done Updating List" );
			listIdle = true;

			TGEvent *done = new TGEvent;
			done->SetEventType( ET_REFRESH_SERVER_LIST_DONE );
			done->SetDestination( window );
			g_kEventManager.AddEvent( done );

			TGIntEvent *percentage = new TGIntEvent;
			percentage->SetEventType( ET_SERVER_LIST_PERCENTAGE );
			percentage->SetDestination( window );
			percentage->SetInt( 100 );
			g_kEventManager.AddEvent( percentage );
			break;
		}
		case sl_listxfer:
			text = database->GetString( "Getting List" );
			break;
		case sl_lanlist:
			text = database->GetString( "Searching LAN" );
			break;
		case sl_querying:
			text = database->GetString( "Querying Servers" );
			break;
		default:
			return;
	}
	if ( text != NULL ) {
		TGStringEvent *state = new TGStringEvent;
		state->SetEventType( ET_SERVER_LIST_STATE );
		state->SetDestination( window );
		TGString message = *text;
		state->SetString( message );
		g_kEventManager.AddEvent( state );
	}
}

/*
================
GameSpy::SendServerPlayers

Sends the multiplayer window the names of a server's players.
================
*/
// FUNCTION: STBC 0x0069d2e0
void GameSpy::SendServerPlayers( GServer server ) {
	if ( serverList == NULL || server == NULL ) {
		return;
	}
	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window == NULL ) {
		return;
	}

	int numPlayers = ServerGetIntValue( server, "numplayers", 0 );
	for ( int i = 0; i < numPlayers + 1; i++ ) {
		char *name = ServerGetPlayerStringValue( server, i, "player", UNKNOWN_SERVER_VALUE );
		if ( strcmp( name, UNKNOWN_SERVER_VALUE ) != 0 ) {
			TGStringEvent *event = new TGStringEvent;
			event->SetEventType( ET_SERVER_PLAYER_EVENT );
			event->SetDestination( window );
			event->SetString( name );
			g_kEventManager.AddEvent( event );
		}
	}
}

/*
================
GameSpy::GetServerInfo

A server's time and frag limits (-1 when it doesn't say), its system and
its mission script ("Unknown").
================
*/
// FUNCTION: STBC 0x0069d3f0
bool GameSpy::GetServerInfo( GServer server, int *timeLimit, int *fragLimit, char *system, int systemLength, char *mission, int missionLength ) {
	*timeLimit = ServerGetIntValue( server, "timelimit", -1 );
	*fragLimit = ServerGetIntValue( server, "fraglimit", -1 );
	char *value = ServerGetStringValue( server, "system", "Unknown" );
	strncpy( system, value, systemLength );
	system[systemLength] = '\0';
	value = ServerGetStringValue( server, "missionscript", "Unknown" );
	strncpy( mission, value, missionLength );
	mission[missionLength] = '\0';
	return true;
}

/*
================
GameSpy::StopServerList

Halts and frees the server list, and tells the multiplayer window.
================
*/
// FUNCTION: STBC 0x0069d470
void GameSpy::StopServerList( void ) {
	if ( serverList != NULL ) {
		ServerListHalt( serverList );
		ServerListFree( serverList );
		serverList = NULL;
		started = false;
	}

	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window == NULL ) {
		return;
	}
	TGString *text = database->GetString( "Done Updating List" );

	TGEvent *done = new TGEvent;
	done->SetEventType( ET_REFRESH_SERVER_LIST_DONE );
	done->SetDestination( window );
	g_kEventManager.AddEvent( done );

	if ( text != NULL ) {
		TGStringEvent *state = new TGStringEvent;
		state->SetEventType( ET_SERVER_LIST_STATE );
		state->SetDestination( window );
		TGString message = *text;
		state->SetString( message );
		g_kEventManager.AddEvent( state );
	}
}

/*
================
GameSpy::SortServerList

Sorts the list by key, the other way from last time, and sends the
multiplayer window the servers again.
================
*/
// FUNCTION: STBC 0x0069d600
void GameSpy::SortServerList( char *key, bool isString ) {
	if ( serverList == NULL ) {
		return;
	}
	MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window == NULL ) {
		return;
	}

	sortAscending = !sortAscending;
	if ( isString ) {
		ServerListSort( serverList, sortAscending, key, cm_stricase );
	} else {
		ServerListSort( serverList, sortAscending, key, cm_int );
	}
	for ( int i = 0; i < ServerListCount( serverList ); i++ ) {
		GServer server = ServerListGetServer( serverList, i );
		if ( ServerGetStringValue( server, "hostname", NULL ) != NULL ) {
			ServerListEvent *entry = new ServerListEvent;
			entry->SetEventType( ET_SERVER_ENTRY_EVENT );
			entry->SetDestination( window );
			entry->SetServer( ServerListGetServer( serverList, i ) );
			g_kEventManager.AddEvent( entry );
		}
	}
}

/*
================
GameSpy::ProcessQueryHandler

The network's GameSpy messages: a query to the hosted game, answered by
the query reporting.
================
*/
// FUNCTION: STBC 0x0069d720
void GameSpy::ProcessQueryHandler( TGEvent *event ) {
	if ( event->IsTypeOf( TGGAMESPYEVENT_TYPE ) && queryReporting != NULL ) {
		TGGameSpyEvent *query = (TGGameSpyEvent *)event;
		qr_parse_query( queryReporting, query->GetText(), (struct sockaddr *)query->GetAddress() );
	}
}
