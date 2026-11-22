/*
===============================================================================

	TGNetwork.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGNetwork.h"
#include "TGAckMessage.h"
#include "TGBootPlayerMessage.h"
#include "TGConnectMessage.h"
#include "TGDisconnectMessage.h"
#include "TGDoNothingMessage.h"
#include "TGEncrypt.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGMessageEvent.h"
#include "TGMessage.h"
#include "TGNameChangeMessage.h"
#include "TGNetPlayer.h"
#include "TGPlayerEvent.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b7cb0 _$E438
// FUNCTION: STBC 0x006b7cd0 _$E437

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006b6c40 _$E32
// FUNCTION: STBC 0x006b6c50 _$E29
// FUNCTION: STBC 0x006b6c60 _$E31
// FUNCTION: STBC 0x006b6c70 _$E30
// FUNCTION: STBC 0x006b6c80 _$E38
// FUNCTION: STBC 0x006b6c90 _$E35
// FUNCTION: STBC 0x006b6ca0 _$E37
// FUNCTION: STBC 0x006b6cb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006b7750 _$E64
// FUNCTION: STBC 0x006b7760 _$E63
// FUNCTION: STBC 0x006b7770 _$E68
// FUNCTION: STBC 0x006b7780 _$E67
// FUNCTION: STBC 0x006b7790 _$E79
// FUNCTION: STBC 0x006b77a0 _$E78
// FUNCTION: STBC 0x006b77b0 _$E83
// FUNCTION: STBC 0x006b77c0 _$E82
// FUNCTION: STBC 0x006b77d0 _$E87
// FUNCTION: STBC 0x006b77e0 _$E86
// FUNCTION: STBC 0x006b77f0 _$E91
// FUNCTION: STBC 0x006b7800 _$E90
// FUNCTION: STBC 0x006b7810 _$E215
// FUNCTION: STBC 0x006b7820 _$E214
// FUNCTION: STBC 0x006b7830 _$E157
// FUNCTION: STBC 0x006b7840 _$E156
// FUNCTION: STBC 0x006b7850 _$E170
// FUNCTION: STBC 0x006b7860 _$E169
// FUNCTION: STBC 0x006b7870 _$E220
// FUNCTION: STBC 0x006b7880 _$E219
// FUNCTION: STBC 0x006b7890 _$E225
// FUNCTION: STBC 0x006b78a0 _$E224
// FUNCTION: STBC 0x006b78b0 _$E229
// FUNCTION: STBC 0x006b78c0 _$E228
// FUNCTION: STBC 0x006b7970 _$E233
// FUNCTION: STBC 0x006b7980 _$E232
// FUNCTION: STBC 0x006b7990 _$E240
// FUNCTION: STBC 0x006b79a0 _$E239
// FUNCTION: STBC 0x006b7a00 _$E265
// FUNCTION: STBC 0x006b7a10 _$E264
// FUNCTION: STBC 0x006b7a70 _$E269
// FUNCTION: STBC 0x006b7a80 _$E268
// FUNCTION: STBC 0x006b7a90 _$E115
// FUNCTION: STBC 0x006b7aa0 _$E114
// FUNCTION: STBC 0x006b7ab0 _$E251
// FUNCTION: STBC 0x006b7ac0 _$E250
// FUNCTION: STBC 0x006b7ad0 _$E258
// FUNCTION: STBC 0x006b7ae0 _$E257
// FUNCTION: STBC 0x006b7af0 _$E281
// FUNCTION: STBC 0x006b7b00 _$E280
// FUNCTION: STBC 0x006b7b10 _$E285
// FUNCTION: STBC 0x006b7b20 _$E284
// FUNCTION: STBC 0x006b7b30 _$E325
// FUNCTION: STBC 0x006b7b40 _$E324
// FUNCTION: STBC 0x006b7b50 _$E331
// FUNCTION: STBC 0x006b7b60 _$E330
// FUNCTION: STBC 0x006b7b70 _$E273
// FUNCTION: STBC 0x006b7b80 _$E272
// FUNCTION: STBC 0x006b7b90 _$E277
// FUNCTION: STBC 0x006b7ba0 _$E276
// FUNCTION: STBC 0x006b7bb0 _$E289
// FUNCTION: STBC 0x006b7bc0 _$E288
// FUNCTION: STBC 0x006b7c70 _$E293
// FUNCTION: STBC 0x006b7c80 _$E292
// FUNCTION: STBC 0x006b7c90 _$E297
// FUNCTION: STBC 0x006b7ca0 _$E296
// FUNCTION: STBC 0x006b7ce0 _$E301
// FUNCTION: STBC 0x006b7cf0 _$E300
// FUNCTION: STBC 0x006b7d00 _$E305
// FUNCTION: STBC 0x006b7d10 _$E304
// FUNCTION: STBC 0x006b7d20 _$E309
// FUNCTION: STBC 0x006b7d30 _$E308
// FUNCTION: STBC 0x006b7d40 _$E313
// FUNCTION: STBC 0x006b7d50 _$E312
// FUNCTION: STBC 0x006b7d60 _$E317
// FUNCTION: STBC 0x006b7d70 _$E316
// FUNCTION: STBC 0x006b7d80 _$E120
// FUNCTION: STBC 0x006b7d90 _$E119
// FUNCTION: STBC 0x006b7da0 _$E321
// FUNCTION: STBC 0x006b7db0 _$E320
// FUNCTION: STBC 0x006b7dc0 _$E125
// FUNCTION: STBC 0x006b7dd0 _$E124
// FUNCTION: STBC 0x006b7de0 _$E335
// FUNCTION: STBC 0x006b7df0 _$E334
// FUNCTION: STBC 0x006b7e00 _$E339
// FUNCTION: STBC 0x006b7e10 _$E338
// FUNCTION: STBC 0x006b7e20 _$E343
// FUNCTION: STBC 0x006b7e30 _$E342
// FUNCTION: STBC 0x006b7e40 _$E347
// FUNCTION: STBC 0x006b7e50 _$E346
// FUNCTION: STBC 0x006b7e60 _$E134
// FUNCTION: STBC 0x006b7e70 _$E133
// FUNCTION: STBC 0x006b7e80 _$E351
// FUNCTION: STBC 0x006b7e90 _$E350
// FUNCTION: STBC 0x006b7ea0 _$E355
// FUNCTION: STBC 0x006b7eb0 _$E354
// FUNCTION: STBC 0x006b7ec0 _$E359
// FUNCTION: STBC 0x006b7ed0 _$E358
// FUNCTION: STBC 0x006b7ee0 _$E140
// FUNCTION: STBC 0x006b7ef0 _$E139
// FUNCTION: STBC 0x006b7f00 _$E145
// FUNCTION: STBC 0x006b7f10 _$E144
// FUNCTION: STBC 0x006b7f20 _$E175
// FUNCTION: STBC 0x006b7f30 _$E174
// FUNCTION: STBC 0x006b7f40 _$E363
// FUNCTION: STBC 0x006b7f50 _$E362
// FUNCTION: STBC 0x006b7f60 _$E180
// FUNCTION: STBC 0x006b7f70 _$E179
// FUNCTION: STBC 0x006b7f80 _$E367
// FUNCTION: STBC 0x006b7f90 _$E366
// FUNCTION: STBC 0x006b7fa0 _$E371
// FUNCTION: STBC 0x006b7fb0 _$E370
// FUNCTION: STBC 0x006b7fc0 _$E185
// FUNCTION: STBC 0x006b7fd0 _$E184
// FUNCTION: STBC 0x006b7fe0 _$E375
// FUNCTION: STBC 0x006b7ff0 _$E374
// FUNCTION: STBC 0x006b8000 _$E150
// FUNCTION: STBC 0x006b8010 _$E149
// FUNCTION: STBC 0x006b8020 _$E379
// FUNCTION: STBC 0x006b8030 _$E378
// FUNCTION: STBC 0x006b8040 _$E383
// FUNCTION: STBC 0x006b8050 _$E382
// FUNCTION: STBC 0x006b8060 _$E190
// FUNCTION: STBC 0x006b8070 _$E189
// FUNCTION: STBC 0x006b8080 _$E195
// FUNCTION: STBC 0x006b8090 _$E194
// FUNCTION: STBC 0x006b80a0 _$E387
// FUNCTION: STBC 0x006b80b0 _$E386
// FUNCTION: STBC 0x006b80c0 _$E391
// FUNCTION: STBC 0x006b80d0 _$E390
// FUNCTION: STBC 0x006b80e0 _$E395
// FUNCTION: STBC 0x006b80f0 _$E394
// FUNCTION: STBC 0x006b8100 _$E399
// FUNCTION: STBC 0x006b8110 _$E398
// FUNCTION: STBC 0x006b8120 _$E403
// FUNCTION: STBC 0x006b8130 _$E402
// FUNCTION: STBC 0x006b8140 _$E407
// FUNCTION: STBC 0x006b8150 _$E406
// FUNCTION: STBC 0x006b8160 _$E411
// FUNCTION: STBC 0x006b8170 _$E410
// FUNCTION: STBC 0x006b8180 _$E415
// FUNCTION: STBC 0x006b8190 _$E414
// FUNCTION: STBC 0x006b81a0 _$E200
// FUNCTION: STBC 0x006b81b0 _$E199
// FUNCTION: STBC 0x006b81c0 _$E419
// FUNCTION: STBC 0x006b81d0 _$E418
// FUNCTION: STBC 0x006b81e0 _$E205
// FUNCTION: STBC 0x006b81f0 _$E204
// FUNCTION: STBC 0x006b8200 _$E210
// FUNCTION: STBC 0x006b8210 _$E209

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006b3890 _$E425
// FUNCTION: STBC 0x006b38a0 _$E421
// FUNCTION: STBC 0x006b38b0 _$E424
// FUNCTION: STBC 0x006b38c0 _$E422
// DATA: STBC 0x00995e80
TGEventHandlerTable TGNetwork::classHandlers;

// Its initialiser and guarded exit-time destructor.
// FUNCTION: STBC 0x006b38e0 _$E430
// FUNCTION: STBC 0x006b38f0 _$E427
// FUNCTION: STBC 0x006b3920 _$E429
// FUNCTION: STBC 0x006b3930 _$E428
// DATA: STBC 0x00995e48
TGNetworkListType TGNetwork::networkList;

// DATA: STBC 0x009962d4
TGMessageUnSerializer TGNetwork::messageTypes[MESSAGE_TYPE_TABLE_SIZE];

// How long Disconnect keeps the network running for its disconnect
// messages to go, in seconds (invented name).
#define DISCONNECT_TIMEOUT	15.0f

// How long a player may go without a message from us before Update sends
// it a do-nothing message, in seconds (invented name).
#define KEEP_ALIVE_TIME		5.0f

/*
================
TGNetwork::VirtualSlot24

Opens the connection; a network that can't connect does nothing.
================
*/
// FUNCTION: STBC 0x006b39a0
int TGNetwork::VirtualSlot24( char *address, bool isHost, TGString *password ) {
	bootReason = 0;
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::VirtualSlot25

Drops a connection that is open or being opened.
================
*/
// FUNCTION: STBC 0x006b39b0
int TGNetwork::VirtualSlot25( void ) {
	if ( connectStatus == TGNETWORK_CONNECTED || connectStatus == TGNETWORK_CONNECT_IN_PROGRESS ) {
		connectStatus = TGNETWORK_ERROR_NOT_CONNECTED;
		return TGNETWORK_OKAY;
	}
	return TGNETWORK_ERROR_NOT_CONNECTED;
}

/*
================
TGNetwork::VirtualSlot27
================
*/
// FUNCTION: STBC 0x006b39d0
char *TGNetwork::VirtualSlot27( int *length, int *address ) {
	return NULL;
}

/*
================
TGNetwork::VirtualSlot28
================
*/
// FUNCTION: STBC 0x006b39e0
bool TGNetwork::VirtualSlot28( int address, char *data, int length ) {
	return true;
}

/*
================
TGNetwork::VirtualSlot26
================
*/
// FUNCTION: STBC 0x006b39f0
bool TGNetwork::VirtualSlot26( int ) {
	return true;
}

/*
================
TGNetwork::TGNetwork

The first network registers the message types. A network starts
disconnected, with the host's ID taken and the default encryption.
================
*/
// FUNCTION: STBC 0x006b3a00 ??0TGNetwork@@QAE@XZ
TGNetwork::TGNetwork( void ) {
	if ( networkList.GetCount() == 0 ) {
		TGMessage::RegisterMessageType();
		TGNameChangeMessage::RegisterMessageType();
		TGAckMessage::RegisterMessageType();
		TGDoNothingMessage::RegisterMessageType();
		TGBootPlayerMessage::RegisterMessageType();
		TGConnectMessage::RegisterMessageType();
		TGDisconnectMessage::RegisterMessageType();
	}
	connectStatus = TGNETWORK_ERROR_NOT_CONNECTED;
	localID = -1;
	unknown1C = 0;
	hostID = HOST_PLAYER_ID;
	unknown24 = 0;
	isHost = false;
	unknownA8 = 0x8000;
	unknown10C = true;
	unknownB0 = 0;
	sendTimeout = 360.0f;
	connectionTimeout = 45.0f;
	lastHostPingTime = g_kSystemWrapper.GetTimeInSeconds();
	unknownAC = 0x400;
	bootReason = 0;
	profiling = false;
	nextPlayerID = FIRST_PLAYER_ID;
	networkList.AddTail( this );
	memset( playerIDInUse, 0, sizeof( playerIDInUse ) );
	playerIDInUse[0] = true;
	playerIDInUse[HOST_PLAYER_ID] = true;
	playerIDInUse[0x5C] = true;
	encryptor = new TGEncrypt;
	unknown10F = false;
	unknown10D = false;
}

/*
================
TGNetwork::~TGNetwork
================
*/
// FUNCTION: STBC 0x006b3c40
// FUNCTION: STBC 0x006b3590 ??_GTGNetwork@@UAEPAXI@Z
TGNetwork::~TGNetwork( void ) {
	if ( encryptor != NULL ) {
		delete encryptor;
		encryptor = NULL;
	}
	networkList.Remove( this );
	Disconnect();
}

/*
================
TGNetwork::Connect

Connects to the address (a host name or a dotted address), or hosts when
there is none. The host is connected at
once; a client waits for the host's answer.
================
*/
// FUNCTION: STBC 0x006b3ec0
int TGNetwork::Connect( char *address, TGString *password ) {
	if ( connectStatus != TGNETWORK_ERROR_NOT_CONNECTED ) {
		switch ( connectStatus ) {
		case TGNETWORK_CONNECTED:
			return TGNETWORK_ERROR_ALREADY_CONNECTED;
		case TGNETWORK_CONNECT_IN_PROGRESS:
			return TGNETWORK_CONNECT_IN_PROGRESS;
		default:
			return TGNETWORK_ERROR;
		}
	}

	if ( address == NULL ) {
		isHost = true;
	} else {
		isHost = false;
	}
	int result = VirtualSlot24( address, isHost, password );
	if ( result == TGNETWORK_OKAY ) {
		if ( isHost ) {
			connectStatus = TGNETWORK_CONNECTED;
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_NETWORK_CONNECT_EVENT );
			event->SetDestination( this );
			event->SetSource( this );
			g_kEventManager.AddEvent( event );
		} else {
			connectStatus = TGNETWORK_CONNECT_IN_PROGRESS;
			unknown10F = true;
		}
		unknown10D = false;
		if ( password != NULL ) {
			SetPassword( password );
		} else {
			TGString empty;
			empty.AssignC( "" );
			SetPassword( &empty );
		}
	} else {
		connectStatus = TGNETWORK_ERROR_NOT_CONNECTED;
	}
	return result;
}

/*
================
TGNetwork::Disconnect

A connected client first tells the host it is going, then keeps the
network running (and its received messages handed out) until no player
has a disconnect message waiting to go, or DISCONNECT_TIMEOUT seconds
pass, not counting the time spent handling the messages. Then everything
is let go: the profile is written, the players, queues and groups are
deleted, and the disconnect event goes out.
================
*/
// FUNCTION: STBC 0x006b4060
int TGNetwork::Disconnect( void ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}

	if ( connectStatus == TGNETWORK_CONNECTED ) {
		TGDisconnectMessage *message = new TGDisconnectMessage;
		message->SetBackoffType( BACKOFF_CONSTANT );
		message->SetBackoffFactor( 0.5f );
		char data[5];
		data[0] = (char)localID;
		memcpy( &data[1], &unknown1C, sizeof( unknown1C ) );
		message->SetData( data, sizeof( data ) );
		SendTGMessage( hostID, message, 0 );

		float startTime = g_kSystemWrapper.GetTimeInSeconds();
		float handlingTime = 0.0f;
		float now = startTime;
		while ( now - startTime - handlingTime < DISCONNECT_TIMEOUT ) {
			SendMessages();
			ReceiveMessages();
			ProcessMessages();

			TGMessage *received;
			while ( ( received = GetNextMessage( false ) ) != NULL ) {
				TGMessageEvent *event = new TGMessageEvent;
				event->SetEventType( ET_NETWORK_MESSAGE_EVENT );
				event->SetMessage( received );
				event->SetDestination( this );
				event->SetSource( this );
				g_kSystemWrapper.UpdateTime();
				float handlingStart = g_kSystemWrapper.GetTimeInSeconds();
				g_kEventManager.ProcessEvent( event );
				if ( event->GetRefCount() == 0 && event != NULL ) {
					delete event;
				}
				g_kSystemWrapper.UpdateTime();
				handlingTime += g_kSystemWrapper.GetTimeInSeconds() - handlingStart;
			}

			int count = playerList.GetNumPlayers();
			TGNetPlayer **players = playerList.GetPlayerList();
			int i;
			for ( i = 0; i < count; i++ ) {
				TGNetPlayer *player = players[i];
				if ( !player->disconnected ) {
					TGMessage *pending;
					for ( pending = player->unknown64.GetFirst(); pending != NULL; pending = player->unknown64.GetNext() ) {
						if ( pending->VirtualSlot00() == TGDISCONNECTMESSAGE_TYPE ) {
							break;
						}
					}
					if ( pending != NULL ) {
						break;
					}
					for ( pending = player->unknown80.GetFirst(); pending != NULL; pending = player->unknown80.GetNext() ) {
						if ( pending->VirtualSlot00() == TGDISCONNECTMESSAGE_TYPE ) {
							break;
						}
					}
					if ( pending != NULL ) {
						break;
					}
				}
			}
			if ( i == count ) {
				break;
			}
			g_kSystemWrapper.UpdateTime();
			now = g_kSystemWrapper.GetTimeInSeconds();
		}
	}

	WriteProfile();
	playerList.DeleteAllPlayers();
	unknown38.DeleteAll();
	unknown54.DeleteAll();
	unknown70.DeleteAll();
	unknown8C.DeleteAll();
	unknownC4.DeleteAll();
	groups.DeleteAll();

	TGEvent *event = new TGEvent;
	event->SetEventType( ET_NETWORK_DISCONNECT_EVENT );
	event->SetDestination( this );
	event->SetSource( this );
	g_kEventManager.AddEvent( event );
	return VirtualSlot25();
}

/*
================
TGNetwork::Update

Each frame of a connected network. A client still connecting sends its
connect message once (its ID, a random key and the password) and runs
the network. Otherwise the players that have heard nothing from us for
KEEP_ALIVE_TIME seconds get a do-nothing message, the network runs, and
each received message goes out as an ET_NETWORK_MESSAGE_EVENT. Then the
host drops the players it has not heard from within the connection
timeout, and a client that has not heard from the host disconnects.
================
*/
// FUNCTION: STBC 0x006b4560
void TGNetwork::Update( void ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		return;
	}

	float now = g_kSystemWrapper.GetTimeInSeconds();
	if ( connectStatus == TGNETWORK_CONNECT_IN_PROGRESS ) {
		if ( unknown10F ) {
			unknown10F = false;
			TGConnectMessage *message = new TGConnectMessage;
			char data[256];
			memcpy( &data[0], &unknown1C, sizeof( unknown1C ) );
			g_kSystemWrapper.SetRandomSeed( g_kSystemWrapper.GetTimeInMilliseconds() + g_kSystemWrapper.GetTimeElapsedInMilliseconds() );
			int key;
			do {
				key = g_kSystemWrapper.GetRandomNumber( 0x7FFFFFF );
			} while ( key == 0 );
			memcpy( &data[4], &key, sizeof( key ) );
			int length = password.length();
			memcpy( &data[8], password.c_str(), ( length + 1 ) * sizeof( unsigned short ) );
			message->SetData( data, 8 + ( length + 1 ) * sizeof( unsigned short ) );
			message->SetBackoffType( BACKOFF_CONSTANT );
			message->SetBackoffFactor( 1.0f );
			message->SetBackoffTime( 1.0f );
			message->SetFirstResendTime( 1.0f );
			SendTGMessage( hostID, message, 0 );
		}
		SendMessages();
		ReceiveMessages();
		ProcessMessages();
	} else {
		if ( !isHost ) {
			if ( now - lastSendToHostTime > KEEP_ALIVE_TIME ) {
				SendTGMessage( hostID, new TGDoNothingMessage, 0 );
			}
		} else {
			int count = playerList.GetNumPlayers();
			TGNetPlayer **players = playerList.GetPlayerList();
			for ( int i = 0; i < count; i++ ) {
				TGNetPlayer *player = players[i];
				if ( player->netID != localID && !player->disconnected && now - player->GetLastSendTime() > KEEP_ALIVE_TIME ) {
					SendTGMessage( player->netID, new TGDoNothingMessage, 0 );
				}
			}
		}
		SendMessages();
		ReceiveMessages();
		ProcessMessages();

		TGMessage *received;
		while ( ( received = GetNextMessage( false ) ) != NULL ) {
			TGMessageEvent *event = new TGMessageEvent;
			event->SetEventType( ET_NETWORK_MESSAGE_EVENT );
			event->SetMessage( received );
			event->SetDestination( this );
			event->SetSource( this );
			g_kEventManager.AddEvent( event );
		}
	}

	if ( isHost ) {
		int i;
		int count;
		do {
			count = playerList.GetNumPlayers();
			TGNetPlayer **players = playerList.GetPlayerList();
			for ( i = 0; i < count; i++ ) {
				TGNetPlayer *player = players[i];
				if ( player->netID != localID && !player->disconnected && now - player->GetLastReceiveTime() > connectionTimeout ) {
					if ( player->netID != -1 ) {
						TGBootPlayerMessage *message = new TGBootPlayerMessage;
						message->SetBootReason( TIMED_OUT );
						char playerID = (char)player->netID;
						message->SetData( &playerID, sizeof( playerID ) );
						DisconnectPlayer( player->netID );
						SendTGMessage( 0, message, 0 );
					} else {
						// A player that never got an ID is known only by its address.
						playerList.DeletePlayerByAddress( player->address );
					}
					break;
				}
			}
		} while ( i < count );
	} else {
		if ( now - lastHostPingTime > connectionTimeout || unknown10D ) {
			if ( !unknown10D ) {
				SetBootReason( TIMED_OUT );
			}
			Disconnect();
		}
	}
	UpdateProfile();
}

/*
================
TGNetwork::GetConnectStatus
================
*/
// FUNCTION: STBC 0x006b4930
int TGNetwork::GetConnectStatus( void ) {
	return connectStatus;
}

/*
================
TGNetwork::CreateLocalPlayer

Adds the local player, and the host when this is a client, to the player
list if they aren't there yet, then names the local player.
================
*/
// FUNCTION: STBC 0x006b4940
int TGNetwork::CreateLocalPlayer( TGString *name ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	if ( playerList.GetPlayerByNetID( localID ) == NULL ) {
		TGNetPlayer *player = CreatePlayer( localID, unknown1C );
		player->connectAddress = unknown1C;
	}
	if ( localID != hostID ) {
		if ( playerList.GetPlayerByNetID( hostID ) == NULL ) {
			TGNetPlayer *player = CreatePlayer( hostID, unknown24 );
			player->connectAddress = unknown24;
		}
	}
	SetName( name );
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::DeleteLocalPlayer
================
*/
// FUNCTION: STBC 0x006b4a30
int TGNetwork::DeleteLocalPlayer( void ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	VirtualSlot29( localID );
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::SetName

Renames the local player and tells the host: the message holds the
player's ID, its address and the name's wide characters with their
terminator.
================
*/
// FUNCTION: STBC 0x006b4a50
int TGNetwork::SetName( TGString *name ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	TGNetPlayer *player = playerList.GetPlayerByNetID( localID );
	if ( player != NULL ) {
		player->SetName( name );

		TGMessage *message = new TGNameChangeMessage;
		char buffer[256];
		buffer[0] = localID;
		*(int *)&buffer[1] = unknown1C;
		memcpy( &buffer[5], name->c_str(), ( name->length() + 1 ) * sizeof( wchar_t ) );
		message->SetData( buffer, ( name->length() + 1 ) * sizeof( wchar_t ) + 5 );
		SendTGMessage( hostID, message, 0 );
		return TGNETWORK_OKAY;
	}
	return TGNETWORK_ERROR;
}

/*
================
TGNetwork::GetName

The local player's name; NULL unless connected.
================
*/
// FUNCTION: STBC 0x006b4b50
TGString *TGNetwork::GetName( void ) {
	if ( connectStatus == TGNETWORK_CONNECTED ) {
		TGNetPlayer *player = playerList.GetPlayerByNetID( localID );
		if ( player != NULL ) {
			return &player->name;
		}
	}
	return NULL;
}

/*
================
TGNetwork::GetCName

The local player's name as a C string; NULL unless connected.
================
*/
// FUNCTION: STBC 0x006b4bb0
char *TGNetwork::GetCName( void ) {
	if ( connectStatus == TGNETWORK_CONNECTED ) {
		TGNetPlayer *player = playerList.GetPlayerByNetID( localID );
		if ( player != NULL ) {
			return player->name.Unknown6F56F0();
		}
	}
	return NULL;
}

/*
================
TGNetwork::SendTGMessage

Sends the message to the player with the ID, to every player when the ID
is 0, or to the player at the address when it is -1. The network takes
the message: it is deleted when it can't be sent.
================
*/
// FUNCTION: STBC 0x006b4c10
int TGNetwork::SendTGMessage( int toID, TGMessage *message, int address ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		delete message;
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}

	if ( toID == -1 ) {
		TGNetPlayer *player = playerList.GetPlayerFromAddress( address );
		int result = TGNETWORK_ERROR_INVALID_ADDRESS;
		if ( player != NULL ) {
			result = QueueMessage( message, player );
			player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
		}
		return result;
	}

	if ( toID == 0 ) {
		int result = TGNETWORK_OKAY;
		bool messageSent = false;
		int count = playerList.GetNumPlayers();
		TGNetPlayer **players = playerList.GetPlayerList();
		for ( int i = 0; i < count; i++ ) {
			TGNetPlayer *player = players[i];
			if ( player->disconnected != true ) {
				message->SetFromID( localID );
				TGMessage *copy;
				if ( i == count - 1 ) {
					messageSent = true;
					copy = message;
				} else {
					copy = message->Copy();
				}
				int error = QueueMessage( copy, player );
				if ( error != TGNETWORK_OKAY ) {
					result = error;
					break;
				}
				player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
			}
		}
		if ( !messageSent ) {
			delete message;
		}
		lastSendToHostTime = g_kSystemWrapper.GetTimeInSeconds();
		return result;
	}

	TGNetPlayer *player = playerList.GetPlayerByNetID( toID );
	if ( player == NULL ) {
		if ( toID == hostID ) {
			player = CreatePlayer( toID, unknown24 );
			player->connectAddress = unknown24;
		} else {
			return TGNETWORK_ERROR_INVALID_ADDRESS;
		}
	}
	int result = QueueMessage( message, player );
	player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
	if ( toID == hostID ) {
		lastSendToHostTime = g_kSystemWrapper.GetTimeInSeconds();
	}
	return result;
}

/*
================
TGNetwork::SendTGMessageToGroup

Sends the message to the players of the named group.
================
*/
// FUNCTION: STBC 0x006b4de0 ?SendTGMessageToGroup@TGNetwork@@QAEHPADPAVTGMessage@@@Z
int TGNetwork::SendTGMessageToGroup( char *groupName, TGMessage *message ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	TGNetGroup *group = groups.Get( groupName );
	if ( group != NULL ) {
		return SendTGMessageToGroup( group, message );
	}
	delete message;
	return TGNETWORK_ERROR_GROUP_NOT_FOUND;
}

/*
================
TGNetwork::SendTGMessageToGroup

Drops the members that have gone, then sends the message to the others.
================
*/
// FUNCTION: STBC 0x006b4ec0 ?SendTGMessageToGroup@TGNetwork@@QAEHPAVTGNetGroup@@PAVTGMessage@@@Z
int TGNetwork::SendTGMessageToGroup( TGNetGroup *group, TGMessage *message ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	if ( group != NULL ) {
		group->Validate( &playerList );

		TGGroupList *members = group->GetPlayerList();
		TGGroupPlayer **items = members->GetItems();
		int count = members->GetCount();
		int result = TGNETWORK_OKAY;
		bool messageSent = false;
		for ( int i = 0; i < count; i++ ) {
			TGNetPlayer *player = playerList.GetPlayerByNetID( items[i]->netID );
			if ( player != NULL ) {
				TGMessage *copy;
				if ( i == count - 1 ) {
					messageSent = true;
					copy = message;
				} else {
					copy = message->Copy();
				}
				int error = QueueMessage( copy, player );
				if ( error != TGNETWORK_OKAY ) {
					result = error;
					break;
				}
				player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
			} else {
				members->DeleteAt( i );
				i--;
			}
			// BUG: after a member is dropped, i may be -1 here, and count
			// and items are not refreshed
			if ( items[i]->netID == hostID ) {
				lastSendToHostTime = g_kSystemWrapper.GetTimeInSeconds();
			}
		}
		if ( !messageSent ) {
			delete message;
		}
		return result;
	}
	return TGNETWORK_ERROR_GROUP_NOT_FOUND;
}

/*
================
TGNetwork::QueueMessage

Breaks the message into parts that fit a packet and queues them for the
player, high-priority parts in front. A guaranteed message takes the next
sequence number of its kind (system messages, whose types are below
TGMESSAGE_TYPE, count apart).
================
*/
// FUNCTION: STBC 0x006b5080
int TGNetwork::QueueMessage( TGMessage *message, TGNetPlayer *player ) {
	int sequenceKind = 0;
	unsigned short sequenceNumber = 0;
	if ( message->VirtualSlot00() < TGMESSAGE_TYPE ) {
		if ( message->IsGuaranteed() ) {
			sequenceNumber = player->unknown26;
			sequenceKind = 2;
		}
	} else {
		if ( message->IsGuaranteed() ) {
			sequenceNumber = player->unknown2A;
			sequenceKind = 1;
		}
	}

	int count;
	TGMessage **parts = message->BreakUpMessage( &count, unknownAC - 100 );
	if ( player->unknown64.GetCount() + count >= unknownA8 ) {
		delete message;
		return TGNETWORK_ERROR_TOO_MANY_SENDS_PENDING;
	}
	for ( int i = 0; i < count; i++ ) {
		TGMessage *part = parts[i];
		part->SetToID( player->netID );
		part->SetSequenceNumber( sequenceNumber );
		if ( part->IsHighPriority() ) {
			player->unknown64.AddHead( part );
		} else {
			player->unknown64.AddTail( part );
		}
	}
	if ( sequenceKind == 1 ) {
		player->unknown2A++;
	} else if ( sequenceKind == 2 ) {
		player->unknown26++;
	}
	delete [] parts;
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::Unknown6B51E0

Sends a copy of the message to every player but the host and the ones
that have gone; the last error, or 0.
================
*/
// FUNCTION: STBC 0x006b51e0
int TGNetwork::Unknown6B51E0( TGMessage *message ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	int result = TGNETWORK_OKAY;
	int count = playerList.GetNumPlayers();
	TGNetPlayer **players = playerList.GetPlayerList();
	for ( int i = 0; i < count; i++ ) {
		TGNetPlayer *player = players[i];
		if ( player->netID != hostID && !player->disconnected ) {
			TGMessage *copy = message->Copy();
			int error = QueueMessage( copy, player );
			player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
			if ( error != TGNETWORK_OKAY ) {
				result = error;
			}
		}
	}
	return result;
}

/*
================
TGNetwork::SetSendTimeout

Only while connected, and not negative.
================
*/
// FUNCTION: STBC 0x006b5270
int TGNetwork::SetSendTimeout( float timeout ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	if ( timeout < 0.0f ) {
		return TGNETWORK_ERROR;
	}
	sendTimeout = timeout;
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::GetNextMessage

The next received message to hand on, or NULL. A high-priority message at
the head of the received queue goes first; otherwise the guaranteed message
next in its sender's sequence, or the older of it and the head of the
received queue. Guaranteed messages from players that have gone, and old
duplicates, are dropped on the way; do-nothing messages are deleted and
skipped. With peek, a high-priority message is left in the queue.
================
*/
// FUNCTION: STBC 0x006b52b0
TGMessage *TGNetwork::GetNextMessage( bool peek ) {
	if ( connectStatus != TGNETWORK_CONNECTED ) {
		return NULL;
	}

	TGMessage *result = NULL;
	while ( true ) {
		TGMessage *message = unknown38.GetFirst();
		if ( message != NULL && message->IsHighPriority() ) {
			if ( !peek ) {
				unknown38.RemoveAt( 0 );
			}
			return message;
		}

		int index = 0;
		TGNetPlayer *player;
		TGMessage *next = NULL;
		TGMessage *guaranteed = unknown54.GetFirst();
		while ( guaranteed != NULL ) {
			player = playerList.GetPlayerByNetID( guaranteed->GetFromID() );
			if ( player == NULL ) {
				unknown54.DeleteAt( index );
				index--;
			} else if ( player->unknown28 == guaranteed->GetSequenceNumber() ) {
				if ( !guaranteed->IsMultiPart() ) {
					next = guaranteed;
				}
				break;
			} else {
				int age = guaranteed->GetSequenceNumber() - player->unknown28;
				if ( age < 0 && age > -0x4000 ) {
					unknown54.DeleteAt( index );
					index--;
				}
			}
			guaranteed = unknown54.GetNext();
			index++;
		}

		if ( next != NULL ) {
			if ( message != NULL ) {
				if ( next->GetTimeStamp() < message->GetTimeStamp() ) {
					result = next;
					player->unknown28 = next->GetSequenceNumber() + 1;
					unknown54.RemoveAt( index );
				} else {
					result = message;
					unknown38.RemoveAt( 0 );
				}
			} else {
				result = next;
				player->unknown28 = next->GetSequenceNumber() + 1;
				unknown54.RemoveAt( index );
			}
		} else if ( message != NULL ) {
			result = message;
			unknown38.RemoveAt( 0 );
		}

		if ( result == NULL || result->VirtualSlot00() != TGDONOTHINGMESSAGE_TYPE ) {
			return result;
		}
		delete result;
		result = NULL;
	}
}

/*
================
TGNetwork::GetHostName

The host's name; NULL unless connected.
================
*/
// FUNCTION: STBC 0x006b5550
TGString *TGNetwork::GetHostName( void ) {
	if ( connectStatus == TGNETWORK_CONNECTED ) {
		TGNetPlayer *player = playerList.GetPlayerByNetID( hostID );
		if ( player != NULL ) {
			return &player->name;
		}
	}
	return NULL;
}

/*
================
TGNetwork::SendMessages

Sends each player one packet, starting with the player after the one
served first last time. A packet is the local ID, a message count and the
serialized messages: the acknowledgements not yet sent three times, the
first unacknowledged guaranteed message that is due again, then the
queued messages (all the aggregate ones, but only one other); with
something in it, the acknowledgements sent three times or more go along
again, and are dropped after the ninth. Guaranteed messages that have gone
out wait for their acknowledgement, until the send timeout passes. Then
the players marked disconnected for DISCONNECT_TIMEOUT seconds are let go.
================
*/
// FUNCTION: STBC 0x006b55b0
// PARKED: same calls, branches and inline set; retail gives the spilled this the frame slot below the packet cursor (this build swaps the two) and loads the free space into eax rather than ebp before the profile's header-size call
void TGNetwork::SendMessages( void ) {
	if ( !unknown10C ) {
		return;
	}

	TGNetPlayer **players = playerList.GetPlayerList();
	int count = playerList.GetNumPlayers();
	int remaining = count;
	int index = unknownB0 + 1;
	if ( index >= count ) {
		index = 0;
	}
	unknownB0 = index;

	char *packet = new char[unknownAC];
	while ( remaining != 0 ) {
		TGNetPlayer *player = players[index];
		int address = player->GetNetAddress();
		if ( !VirtualSlot26( address ) ) {
			index++;
			if ( index >= count ) {
				index = 0;
			}
			remaining--;
			continue;
		}

		if ( player->unknown64.GetCount() > 0 || player->unknown9C.GetCount() > 0 || player->unknown80.GetCount() > 0 ) {
			int numMessages = 0;
			int space = unknownAC - 2;
			char *data = packet + 2;

			if ( player->unknown9C.GetCount() > 0 ) {
				for ( TGMessage *ack = player->unknown9C.GetFirst(); ack != NULL; ack = player->unknown9C.GetNext() ) {
					if ( ack->ReadyToResend() && ack->GetNumRetries() < 3 ) {
						int size = ack->Serialize( data, space );
						if ( size == 0 ) {
							break;
						}
						ack->SetTimeStamp( g_kSystemWrapper.GetTimeInSeconds() );
						numMessages++;
						ack->IncrementNumRetries();
						data += size;
						space -= size;
						if ( numMessages >= 255 ) {
							break;
						}
					}
				}
			}

			if ( player->unknown80.GetCount() > 0 && numMessages < 255 ) {
				for ( TGMessage *message = player->unknown80.GetFirst(); message != NULL; message = player->unknown80.GetNext() ) {
					if ( message->ReadyToResend() ) {
						int size = message->Serialize( data, space );
						if ( size != 0 ) {
							numMessages++;
							message->SetTimeStamp( g_kSystemWrapper.GetTimeInSeconds() );
							message->IncrementNumRetries();
							data += size;
							space -= size;
						}
						break;
					}
					if ( g_kSystemWrapper.GetTimeInSeconds() - message->GetFirstSendTime() > sendTimeout ) {
						player->unknown80.DeleteAt( player->unknown80.GetCursorIndex() );
					}
				}
			}

			if ( player->unknown64.GetCount() > 0 && numMessages < 255 ) {
				TGMessage *message = player->unknown64.GetFirst();
				bool first = true;
				while ( message != NULL ) {
					if ( !message->IsAggregate() ) {
						if ( !first ) {
							message = player->unknown64.GetNext();
							continue;
						}
						first = false;
					}
					int size = message->Serialize( data, space );
					if ( size == 0 ) {
						break;
					}
					numMessages++;
					player->unknown64.RemoveAt( player->unknown64.GetCursorIndex() );
					if ( message->IsGuaranteed() ) {
						float now = g_kSystemWrapper.GetTimeInSeconds();
						message->SetTimeStamp( now );
						message->SetFirstSendTime( now );
						message->SetBackoffTime( message->GetFirstResendTime() );
						message->SetNumRetries( 0 );
						if ( sendTimeout > 0.0f ) {
							player->unknown80.AddTail( message );
						} else {
							delete message;
						}
					} else {
						delete message;
					}
					data += size;
					space -= size;
					if ( numMessages >= 255 ) {
						break;
					}
					message = player->unknown64.GetNext();
				}
			}

			if ( ( numMessages > 0 || player->IsDisconnected() ) && player->unknown9C.GetCount() > 0 ) {
				for ( TGMessage *ack = player->unknown9C.GetFirst(); ack != NULL; ack = player->unknown9C.GetNext() ) {
					if ( ack->ReadyToResend() && ack->GetNumRetries() >= 3 ) {
						int size = ack->Serialize( data, space );
						if ( size == 0 ) {
							break;
						}
						ack->SetTimeStamp( g_kSystemWrapper.GetTimeInSeconds() );
						numMessages++;
						ack->IncrementNumRetries();
						if ( ack->GetNumRetries() >= 9 ) {
							player->unknown9C.DeleteAt( player->unknown9C.GetCursorIndex() );
						}
						data += size;
						space -= size;
						if ( numMessages >= 255 ) {
							break;
						}
					}
				}
			}

			if ( numMessages > 0 ) {
				packet[0] = (char)localID;
				packet[1] = (char)numMessages;
				if ( profiling && player->GetNetID() != localID ) {
					int size = unknownAC - space;
					player->AddBytesSent( size, GetIPPacketHeaderSize() );
					TGNetPlayer *localPlayer = playerList.GetPlayerByNetID( localID );
					if ( localPlayer != NULL ) {
						localPlayer->AddBytesSent( size, GetIPPacketHeaderSize() );
					}
				}
				VirtualSlot28( address, packet, unknownAC - space );
			}
		}

		unknownB0 = index;
		index++;
		if ( index >= count ) {
			index = 0;
		}
		remaining--;
	}
	delete [] packet;

	int numPlayers;
	int i;
	do {
		numPlayers = playerList.GetNumPlayers();
		TGNetPlayer **list = playerList.GetPlayerList();
		float now = g_kSystemWrapper.GetTimeInSeconds();
		for ( i = 0; i < numPlayers; i++ ) {
			TGNetPlayer *player = list[i];
			if ( player->disconnected && now - player->disconnectTime > DISCONNECT_TIMEOUT ) {
				VirtualSlot29( player->netID );
				break;
			}
		}
	} while ( i < numPlayers );
}

/*
================
TGNetwork::ReceiveMessages

Reads each received packet into its messages: a packet is the sender's ID,
a message count and the serialized messages. The sender is found by its ID,
or, from a sender without one, by its address in its connect or disconnect
message; a new sender is made a player. Each message is acknowledged if
guaranteed and filed in its queue.
================
*/
// FUNCTION: STBC 0x006b5c90
// PARKED: same calls and branches; retail materialises the same-address test with sete before the UID test (this build sinks it into the branch), keeps the sender ID in ebp and lays out the locals in another order
void TGNetwork::ReceiveMessages( void ) {
	while ( true ) {
		TGNetPlayer *player = NULL;
		int length;
		int address;
		char *data = VirtualSlot27( &length, &address );
		if ( data == NULL ) {
			return;
		}

		char *packet = data;
		int fromID = *data++;
		for ( char count = *data++; count != 0; count-- ) {
			unsigned char type = *data;
			if ( messageTypes[type] == NULL ) {
				// BUG: the packet is not freed.
				return;
			}
			TGMessage *message = messageTypes[type]( data );
			message->SetFromID( fromID );
			message->SetFromAddress( address );
			message->SetToID( localID );
			data += message->GetBufferSpaceRequired();

			if ( player == NULL ) {
				if ( fromID == -1 ) {
					if ( message->VirtualSlot00() == TGCONNECTMESSAGE_TYPE ) {
						int *connectData = (int *)message->GetData( NULL );
						int connectAddress = connectData[0];
						int connectUID = connectData[1];
						player = playerList.GetPlayerFromAddress( address );
						if ( player == NULL ) {
							player = playerList.GetPlayerFromAddress( connectAddress );
						}
						if ( player != NULL ) {
							bool sameAddress = player->GetConnectAddress() == connectAddress;
							if ( player->GetConnectUID() != connectUID ) {
								if ( sameAddress ) {
									DisconnectPlayer( player->GetNetID() );
									VirtualSlot29( player->GetNetID() );
								}
								player = CreatePlayer( -1, address );
								player->SetConnectAddress( connectAddress );
							}
						} else {
							player = CreatePlayer( -1, address );
							player->SetConnectAddress( connectAddress );
						}
						player->SetConnectUID( connectUID );
					} else if ( message->VirtualSlot00() == TGDISCONNECTMESSAGE_TYPE ) {
						char *disconnectData = message->GetData( NULL );
						disconnectData++;
						int connectAddress = *(int *)disconnectData;
						player = playerList.GetPlayerFromAddress( address );
						if ( player == NULL ) {
							player = playerList.GetPlayerFromAddress( connectAddress );
							if ( player == NULL ) {
								player = CreatePlayer( -1, address );
								player->SetConnectAddress( connectAddress );
							}
						}
					} else {
						break;
					}
				} else {
					player = playerList.GetPlayerByNetID( fromID );
					if ( fromID == hostID ) {
						lastHostPingTime = g_kSystemWrapper.GetTimeInSeconds();
					}
					if ( player == NULL ) {
						player = CreatePlayer( fromID, address );
						player->SetConnectAddress( address );
					}
				}

				if ( profiling && player->GetNetID() != localID ) {
					int size = length;
					player->AddBytesReceived( size, GetIPPacketHeaderSize() );
					TGNetPlayer *localPlayer = playerList.GetPlayerByNetID( localID );
					if ( localPlayer != NULL ) {
						localPlayer->AddBytesReceived( size, GetIPPacketHeaderSize() );
					}
				}
				player->unknown2C = g_kSystemWrapper.GetTimeInSeconds();
			}

			if ( message->IsGuaranteed() ) {
				QueueAck( message, player );
			}
			QueueReceivedMessage( message, player );
		}
		if ( packet != NULL ) {
			delete packet;
		}
	}
}

/*
================
TGNetwork::ProcessMessages

Hands each received system message to its handler: the ones at the head of
the plain queue first, then the guaranteed one next in its sender's
sequence. Guaranteed messages from players that have gone, and old
duplicates, are dropped on the way; a multi-part message waits for its
other parts.
================
*/
// FUNCTION: STBC 0x006b5f70
void TGNetwork::ProcessMessages( void ) {
	while ( true ) {
		int index = 0;
		TGLinkedList<TGMessage> *list;
		TGNetPlayer *player;
		TGMessage *message = unknown70.GetFirst();
		if ( message != NULL ) {
			if ( message->GetFromID() == -1 ) {
				player = playerList.GetPlayerFromAddress( message->GetFromAddress() );
			} else {
				player = playerList.GetPlayerByNetID( message->GetFromID() );
			}
			list = &unknown70;
		} else {
			list = &unknown8C;
			message = list->GetFirst();
			while ( message != NULL ) {
				if ( message->GetFromID() == -1 ) {
					player = playerList.GetPlayerFromAddress( message->GetFromAddress() );
				} else {
					player = playerList.GetPlayerByNetID( message->GetFromID() );
				}
				if ( player != NULL ) {
					if ( player->unknown24 == message->GetSequenceNumber() ) {
						if ( message->IsMultiPart() ) {
							return;
						}
						break;
					}
					int age = message->GetSequenceNumber() - player->unknown24;
					if ( age < 0 && age > -0x4000 ) {
						list->DeleteAt( index );
						index--;
					}
				} else {
					list->DeleteAt( index );
					index--;
				}
				message = list->GetNext();
				index++;
			}
		}
		if ( message == NULL ) {
			return;
		}

		if ( message->IsGuaranteed() ) {
			player->unknown24 = message->GetSequenceNumber() + 1;
		}
		switch ( message->VirtualSlot00() ) {
			case TGNAMECHANGEMESSAGE_TYPE:
				HandleNameChangeMessage( message, player );
				break;
			case TGACKMESSAGE_TYPE:
				HandleAckMessage( (TGAckMessage *)message, player );
				break;
			case TGCONNECTMESSAGE_TYPE:
				HandleConnectMessage( message, player );
				break;
			case TGBOOTPLAYERMESSAGE_TYPE:
				HandleBootPlayerMessage( (TGBootPlayerMessage *)message, player );
				break;
			case TGDISCONNECTMESSAGE_TYPE:
				HandleDisconnectMessage( message, player );
				break;
		}
		delete list->RemoveAt( list->GetCursorIndex() );
	}
}

/*
================
TGNetwork::QueueAck

An acknowledgement goes to the player for each guaranteed message received
from it; one already waiting for the same message is sent again instead.
================
*/
// FUNCTION: STBC 0x006b61e0
void TGNetwork::QueueAck( TGMessage *message, TGNetPlayer *player ) {
	int sequenceNumber = message->GetSequenceNumber();
	bool system = message->VirtualSlot00() < TGMESSAGE_TYPE;
	bool multiPart = message->IsMultiPart();
	float now = g_kSystemWrapper.GetTimeInSeconds();
	int part = message->GetMultiPartSequenceNumber();

	TGAckMessage *ack;
	if ( player->unknown9C.GetCount() > 0 ) {
		for ( ack = (TGAckMessage *)player->unknown9C.GetFirst(); ack != NULL; ack = (TGAckMessage *)player->unknown9C.GetNext() ) {
			if ( ack->GetSequenceNumber() == sequenceNumber && ack->IsSystemMessage() == system && ack->IsMultiPart() == multiPart && ack->GetMultiPartSequenceNumber() == part ) {
				ack->SetTimeStamp( now );
				ack->SetFirstSendTime( now );
				ack->SetNumRetries( 0 );
				player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
				if ( player->GetNetID() == hostID ) {
					lastSendToHostTime = g_kSystemWrapper.GetTimeInSeconds();
				}
				return;
			}
		}
	}

	ack = new TGAckMessage;
	ack->SetToID( player->GetNetID() );
	ack->SetTimeStamp( now );
	ack->SetFirstSendTime( now );
	ack->SetNumRetries( 0 );
	ack->SetSequenceNumber( sequenceNumber );
	ack->SetSystemMessage( system );
	ack->SetMultiPart( multiPart );
	ack->SetMultiPartSequenceNumber( part );
	player->unknown9C.AddTail( ack );
	player->unknown30 = g_kSystemWrapper.GetTimeInSeconds();
	if ( player->GetNetID() == hostID ) {
		lastSendToHostTime = g_kSystemWrapper.GetTimeInSeconds();
	}
}

/*
================
TGNetwork::HandleNameChangeMessage

Renames the player the message names, making it first if it is new. The
host passes the message on to the others; everyone posts
ET_NETWORK_NAME_CHANGE_EVENT.
================
*/
// FUNCTION: STBC 0x006b63a0
void TGNetwork::HandleNameChangeMessage( TGMessage *message, TGNetPlayer *player ) {
	char *data = message->GetData( NULL );
	int playerID = *data++;
	int address = *(int *)data;
	data += sizeof( int );

	TGNetPlayer *namedPlayer = playerList.GetPlayerByNetID( playerID );
	if ( namedPlayer == NULL ) {
		namedPlayer = CreatePlayer( playerID, address );
		namedPlayer->SetConnectAddress( address );
	}
	TGString name;
	name.Unknown6F4CE0( (unsigned short *)data );
	namedPlayer->SetName( &name );
	if ( isHost ) {
		Unknown6B51E0( message );
	}

	TGPlayerEvent *event = new TGPlayerEvent;
	event->SetEventType( ET_NETWORK_NAME_CHANGE_EVENT );
	event->SetPlayerID( namedPlayer->GetNetID() );
	event->SetDestination( this );
	event->SetSource( this );
	g_kEventManager.AddEvent( event );
}

/*
================
TGNetwork::HandleAckMessage

The message the acknowledgement answers needs no more resending: it is
taken out of the player's sent queue. A multi-part message's parts are
acknowledged one by one.
================
*/
// FUNCTION: STBC 0x006b64d0
void TGNetwork::HandleAckMessage( TGAckMessage *ack, TGNetPlayer *player ) {
	for ( TGMessage *sent = player->unknown80.GetFirst(); sent != NULL; sent = player->unknown80.GetNext() ) {
		if ( ack->IsSystemMessage() == ( sent->VirtualSlot00() < TGMESSAGE_TYPE ? true : false ) && sent->GetSequenceNumber() == ack->GetSequenceNumber() ) {
			if ( sent->IsMultiPart() && ack->IsMultiPart() ) {
				if ( sent->GetMultiPartSequenceNumber() == ack->GetMultiPartSequenceNumber() ) {
					player->unknown80.DeleteAt( player->unknown80.GetCursorIndex() );
					return;
				}
			} else if ( !sent->IsMultiPart() && !ack->IsMultiPart() ) {
				delete player->unknown80.RemoveAt( player->unknown80.GetCursorIndex() );
				return;
			}
		}
	}
}

/*
================
TGNetwork::HandleConnectMessage

On the host, a connect request: a banned address or a wrong password gets
a boot message; otherwise the player gets its ID, the names of the players
already there, and ET_NETWORK_NEW_PLAYER goes out. On a client, the host's
answer: the local player's ID, and ET_NETWORK_CONNECT_EVENT.
================
*/
// FUNCTION: STBC 0x006b6640
// PARKED: same calls and branches; register allocation differs: retail keeps the new player ID in ebx (spilled across the name loop), the other player in ebx and the name message on the stack, and holds the accepted flag in bl
void TGNetwork::HandleConnectMessage( TGMessage *message, TGNetPlayer *player ) {
	if ( isHost ) {
		int length;
		int *data = (int *)message->GetData( &length );
		int address = data[0];
		player->SetConnectUID( data[1] );
		unsigned short *givenPassword = (unsigned short *)&data[2];
		if ( VirtualSlot23( address ) == true ) {
			char playerID = player->GetNetID();
			TGBootPlayerMessage *boot = new TGBootPlayerMessage;
			boot->SetBootReason( YOU_ARE_BANNED );
			boot->SetData( &playerID, 1 );
			SendTGMessage( player->GetNetID(), boot, player->GetNetAddress() );
			return;
		}

		bool accepted = true;
		if ( password.GetLength() > 0 ) {
			if ( length > 4 ) {
				TGString given;
				given.Unknown6F4CE0( givenPassword );
				if ( password.Compare( &given, true ) != 0 ) {
					accepted = false;
				}
			} else {
				accepted = false;
			}
		}
		if ( accepted ) {
			int newID = GetNewPlayerID();
			if ( !playerList.ChangePlayerNetID( address, newID ) ) {
				playerList.ChangePlayerNetID( player->GetNetAddress(), newID );
			}
			TGMessage *reply = new TGConnectMessage;
			char id = newID;
			reply->SetData( &id, 1 );
			SendTGMessage( player->GetNetID(), reply, 0 );

			TGNetPlayer **players = playerList.GetPlayerList();
			int count = playerList.GetNumPlayers();
			for ( int i = 0; i < count; i++ ) {
				TGNetPlayer *other = players[i];
				if ( other->GetNetID() != player->GetNetID() && !other->IsDisconnected() ) {
					TGMessage *nameMessage = new TGNameChangeMessage;
					char buffer[256];
					buffer[0] = other->GetNetID();
					*(int *)&buffer[1] = other->GetNetAddress();
					memcpy( &buffer[5], other->name.c_str(), ( other->name.length() + 1 ) * sizeof( wchar_t ) );
					nameMessage->SetData( buffer, ( other->name.length() + 1 ) * sizeof( wchar_t ) + 5 );
					SendTGMessage( player->GetNetID(), nameMessage, 0 );
				}
			}

			TGPlayerEvent *newPlayer = new TGPlayerEvent;
			newPlayer->SetEventType( ET_NETWORK_NEW_PLAYER );
			newPlayer->SetPlayerID( newID );
			newPlayer->SetDestination( this );
			newPlayer->SetSource( this );
			g_kEventManager.AddEvent( newPlayer );
		} else {
			char playerID = player->GetNetID();
			TGBootPlayerMessage *boot = new TGBootPlayerMessage;
			boot->SetBootReason( INCORRECT_PASSWORD );
			boot->SetData( &playerID, 1 );
			SendTGMessage( player->GetNetID(), boot, player->GetNetAddress() );
			return;
		}
	} else {
		if ( connectStatus == TGNETWORK_OKAY ) {
			return;
		}
		connectStatus = TGNETWORK_CONNECTED;
		localID = *message->GetData( NULL );
		playerList.ChangePlayerNetID( unknown1C, localID );
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_NETWORK_CONNECT_EVENT );
		event->SetDestination( this );
		event->SetSource( this );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
TGNetwork::HandleDisconnectMessage

A player has left; the local player's own disconnect, or the host's, ends
the connection. The host passes the message on to the others.
================
*/
// FUNCTION: STBC 0x006b6a20
void TGNetwork::HandleDisconnectMessage( TGMessage *message, TGNetPlayer *player ) {
	int playerID = *message->GetData( NULL );
	if ( playerID == localID ) {
		unknown10D = true;
	} else {
		DisconnectPlayer( playerID );
	}
	if ( playerID == hostID ) {
		unknown10D = true;
	}
	if ( isHost ) {
		Unknown6B51E0( message );
	}
}

/*
================
TGNetwork::HandleBootPlayerMessage

A boot of everyone (ID -1) or of the local player ends the connection with
the message's reason; another player's boot disconnects that player.
================
*/
// FUNCTION: STBC 0x006b6a70
void TGNetwork::HandleBootPlayerMessage( TGBootPlayerMessage *message, TGNetPlayer *player ) {
	int playerID = *message->GetData( NULL );
	if ( playerID == -1 ) {
		unknown10D = true;
		bootReason = message->GetBootReason();
		connectStatus = TGNETWORK_CONNECTED;
		return;
	}
	DisconnectPlayer( playerID );
	if ( playerID == localID ) {
		unknown10D = true;
		bootReason = message->GetBootReason();
	}
}

/*
================
TGNetwork::QueueReceivedMessage

System messages go to the system queues, the others to the user queues;
guaranteed ones to the queues kept in sequence. A guaranteed message older
than the next one expected from its sender is a duplicate and is deleted.
A high-priority message goes in after the high-priority ones already
queued.
================
*/
// FUNCTION: STBC 0x006b6ad0
// PARKED: same calls and branches; retail keeps zero in ebx for every NULL and false test and store (this build tests with test/immediates), and orders the two link stores of InsertAt the other way
void TGNetwork::QueueReceivedMessage( TGMessage *message, TGNetPlayer *player ) {
	message->SetTimeStamp( g_kSystemWrapper.GetTimeInSeconds() );

	TGLinkedList<TGMessage> *list;
	if ( message->IsGuaranteed() ) {
		int expected;
		if ( message->VirtualSlot00() < TGMESSAGE_TYPE ) {
			list = &unknown8C;
			expected = player->unknown24;
		} else {
			list = &unknown54;
			expected = player->unknown28;
		}
		int age = message->GetSequenceNumber() - expected;
		if ( age >= -0x4000 && ( age < 0 || age >= 0x4000 ) ) {
			delete message;
			return;
		}
		if ( message->IsMultiPart() ) {
			message = AssembleMultiPart( message );
		}
	} else {
		list = &unknown70;
		if ( message->VirtualSlot00() >= TGMESSAGE_TYPE ) {
			list = &unknown38;
		}
	}

	if ( message->OverrideOldPackets() ) {
		RemoveOverriddenMessages( message );
	}

	if ( message->IsHighPriority() && !message->IsGuaranteed() ) {
		int index = 0;
		for ( TGMessage *queued = list->GetFirst(); queued != NULL && queued->IsHighPriority(); queued = list->GetNext() ) {
			index++;
		}
		if ( index < list->GetCount() ) {
			list->InsertAt( message, index );
			return;
		}
	}
	list->AddTail( message );
}

/*
================
TGNetwork::AssembleMultiPart

Collects the received parts of the message's multi-part message, by part
number. Once all of them are in, the message takes their joined data and
stops being multi-part, and the other parts are deleted; until then the
part is returned as it is.
================
*/
// FUNCTION: STBC 0x006b6cc0
TGMessage *TGNetwork::AssembleMultiPart( TGMessage *message ) {
	int count = 0;
	int size = 0;
	TGLinkedList<TGMessage> *list;
	if ( message->VirtualSlot00() < TGMESSAGE_TYPE ) {
		list = &unknown8C;
	} else {
		list = &unknown54;
	}

	TGMessage *parts[256];
	memset( parts, 0, sizeof( parts ) );
	TGMessage *queued;
	for ( queued = list->GetFirst(); queued != NULL; queued = list->GetNext() ) {
		if ( queued->IsMultiPart() && queued->GetSequenceNumber() == message->GetSequenceNumber() ) {
			parts[queued->GetMultiPartSequenceNumber()] = queued;
		}
	}
	parts[message->GetMultiPartSequenceNumber()] = message;

	int i;
	for ( i = 0; i < 256; i++ ) {
		if ( parts[i] != NULL ) {
			count++;
			size += parts[i]->GetDataLength();
		}
	}
	if ( parts[0] == NULL ) {
		return message;
	}
	if ( parts[0]->GetMultiPartCount() > count ) {
		return message;
	}

	char *data = new char[size];
	char *end = data;
	for ( i = 0; i < count; i++ ) {
		int length;
		char *partData = parts[i]->GetData( &length );
		memcpy( end, partData, length );
		end += length;
	}
	message->SetDataNoCopy( data, size );
	message->SetMultiPart( false );

	for ( queued = list->GetFirst(); queued != NULL; queued = list->GetNext() ) {
		if ( queued->IsMultiPart() && queued->GetSequenceNumber() == message->GetSequenceNumber() ) {
			// BUG: the index is the user queue's walk position, even when
			// the parts are in the system queue.
			delete list->RemoveAt( unknown54.GetCursorIndex() );
		}
	}
	return message;
}

/*
================
TGNetwork::RemoveOverriddenMessages
================
*/
// FUNCTION: STBC 0x006b6f30
void TGNetwork::RemoveOverriddenMessages( TGMessage *message ) {
	TGLinkedList<TGMessage> *list;
	if ( message->VirtualSlot00() < TGMESSAGE_TYPE ) {
		list = &unknown8C;
	} else if ( message->IsGuaranteed() ) {
		list = &unknown54;
	} else {
		list = &unknown38;
	}
	for ( TGMessage *queued = list->GetFirst(); queued != NULL; queued = list->GetNext() ) {
		if ( queued->VirtualSlot00() == message->VirtualSlot00() && message->Merge( queued ) ) {
			delete list->RemoveAt( list->GetCursorIndex() );
		}
	}
}

/*
================
TGNetwork::SetPassword
================
*/
// FUNCTION: STBC 0x006b7070
void TGNetwork::SetPassword( TGString *newPassword ) {
	password = *newPassword;
}

/*
================
TGNetwork::SetEncryptor

Only before connecting; the old encryptor is deleted.
================
*/
// FUNCTION: STBC 0x006b7090
int TGNetwork::SetEncryptor( TGEncrypt *newEncryptor ) {
	if ( connectStatus == TGNETWORK_CONNECTED || connectStatus == TGNETWORK_CONNECT_IN_PROGRESS ) {
		return TGNETWORK_ERROR_DO_BEFORE_CONNECTION;
	}
	if ( encryptor != NULL ) {
		delete encryptor;
	}
	encryptor = newEncryptor;
	return TGNETWORK_OKAY;
}

/*
================
TGNetwork::AddGroup

Adds the group unless one of its name is already there.
================
*/
// FUNCTION: STBC 0x006b70d0
bool TGNetwork::AddGroup( TGNetGroup *group ) {
	if ( groups.Get( group->GetName() ) != NULL ) {
		return false;
	}
	groups.Insert( group );
	return true;
}

/*
================
TGNetwork::UpdateProfile
================
*/
// FUNCTION: STBC 0x006b72b0
void TGNetwork::UpdateProfile( void ) {
	if ( profiling ) {
		float time = g_kSystemWrapper.GetTimeInSeconds();
		TGNetPlayer **players = playerList.GetPlayerList();
		int count = playerList.GetNumPlayers();
		for ( int i = 0; i < count; i++ ) {
			players[i]->UpdateProfile( time );
		}
	}
}

/*
================
TGNetwork::WriteProfile
================
*/
// FUNCTION: STBC 0x006b72f0
void TGNetwork::WriteProfile( void ) {
	if ( !profiling ) {
		return;
	}
	TGNetPlayer *localPlayer = playerList.GetPlayerByNetID( localID );
	if ( localPlayer == NULL ) {
		return;
	}

	char fileName[256];
	int i = 0;
	while ( true ) {
		sprintf( fileName, "netprofile%d.txt", i );
		FILE *existing = fopen( fileName, "r" );
		if ( existing == NULL ) {
			break;
		}
		fclose( existing );
		i++;
	}

	FILE *file = fopen( fileName, "w" );
	if ( isHost ) {
		fprintf( file, "HOST profile\n\n" );
	} else {
		fprintf( file, "CLIENT profile\n\n" );
	}
	localPlayer->WriteProfile( file );
	TGNetPlayer **players = playerList.GetPlayerList();
	int count = playerList.GetNumPlayers();
	for ( i = 0; i < count; i++ ) {
		if ( players[i] != localPlayer ) {
			players[i]->WriteProfile( file );
		}
	}
	fclose( file );
}

/*
================
TGNetwork::CreatePlayer

A new player with the ID and address, added to the player list.
================
*/
// FUNCTION: STBC 0x006b7410
TGNetPlayer *TGNetwork::CreatePlayer( int netID, int address ) {
	TGNetPlayer *player = new TGNetPlayer;
	player->netID = netID;
	player->address = address;
	playerList.AddPlayer( player );
	return player;
}
/*
================
TGNetwork::GetNewPlayerID

The first free ID from where the last search stopped, wrapping round past
MAX_PLAYER_ID; -1 when every one is taken.
================
*/
// FUNCTION: STBC 0x006b7540
int TGNetwork::GetNewPlayerID( void ) {
	int playerID = nextPlayerID;
	for ( int i = 0; i < MAX_PLAYER_ID - 1; i++ ) {
		if ( !playerIDInUse[playerID] ) {
			playerIDInUse[playerID] = true;
			nextPlayerID = playerID + 1;
			if ( nextPlayerID > MAX_PLAYER_ID ) {
				nextPlayerID = FIRST_PLAYER_ID;
			}
			return playerID;
		}
		playerID++;
		if ( playerID > MAX_PLAYER_ID ) {
			playerID = FIRST_PLAYER_ID;
		}
	}
	return -1;
}

/*
================
TGNetwork::FreePlayerID
================
*/
// FUNCTION: STBC 0x006b7590
void TGNetwork::FreePlayerID( int playerID ) {
	if ( playerID <= MAX_PLAYER_ID && playerID >= FIRST_PLAYER_ID ) {
		playerIDInUse[playerID] = false;
	}
}

/*
================
TGNetwork::DisconnectPlayer

The player stays in the list until it times out; the event tells the
game it has gone.
================
*/
// FUNCTION: STBC 0x006b75b0
void TGNetwork::DisconnectPlayer( int playerID ) {
	TGNetPlayer *player = playerList.GetPlayerByNetID( playerID );
	if ( player != NULL ) {
		TGPlayerEvent *event = new TGPlayerEvent;
		event->SetEventType( ET_NETWORK_DELETE_PLAYER );
		event->SetPlayerID( playerID );
		event->SetDestination( this );
		event->SetSource( this );
		g_kEventManager.AddEvent( event );
		player->disconnected = true;
		player->disconnectTime = g_kSystemWrapper.GetTimeInSeconds();
	} else {
		FreePlayerID( playerID );
	}
}

/*
================
TGNetwork::VirtualSlot29

Frees the player's ID and deletes the player.
================
*/
// FUNCTION: STBC 0x006b7660
void TGNetwork::VirtualSlot29( int playerID ) {
	FreePlayerID( playerID );
	playerList.DeletePlayer( playerID );
}

/*
================
TGNetwork::GetTimeElapsedSinceLastHostPing

Always 0 on the host.
================
*/
// FUNCTION: STBC 0x006b7700
float TGNetwork::GetTimeElapsedSinceLastHostPing( void ) {
	if ( isHost ) {
		return 0.0f;
	}
	return g_kSystemWrapper.GetTimeInSeconds() - lastHostPingTime;
}

/*
================
TGNetwork::RegisterHandlers

The class has no handler names to register.
================
*/
// FUNCTION: STBC 0x006b7720
void TGNetwork::RegisterHandlers( void ) {
}

/*
================
TGNetwork::AddClassHandlers

The class has no handlers to add.
================
*/
// FUNCTION: STBC 0x006b7730
void TGNetwork::AddClassHandlers( void ) {
}

/*
================
TGNetwork::ReceiveMessageHandler

Does nothing.
================
*/
// FUNCTION: STBC 0x006b7740
void TGNetwork::ReceiveMessageHandler( TGEvent *event ) {
}

