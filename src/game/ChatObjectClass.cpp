/*
===============================================================================

	ChatObjectClass.cpp

	The chat connection the multiplayer menus use.

===============================================================================
*/

#include "ChatObjectClass.h"
#include <string.h>
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGEvent.h"
#include "TGMessageEvent.h"
#include "TGMessage.h"
#include "TGNetwork.h"
#include "TGWinsockNetwork.h"
#undef GetMessage		// <windows.h>, through winsock.h: TGMessageEvent::GetMessage
#include "TGPlayerList.h"
#include "TGNetPlayer.h"
#include "TGString.h"
#include "TopWindow.h"
#include "ConsoleWindow.h"
#include "TGConsole.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00461810 _$E246
// FUNCTION: STBC 0x00461820 _$E243
// FUNCTION: STBC 0x00461830 _$E245
// FUNCTION: STBC 0x00461840 _$E244
// FUNCTION: STBC 0x00461850 _$E252
// FUNCTION: STBC 0x00461860 _$E249
// FUNCTION: STBC 0x00461870 _$E251
// FUNCTION: STBC 0x00461880 _$E250

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00461890 _$E61
// FUNCTION: STBC 0x004618a0 _$E60
// FUNCTION: STBC 0x004618b0 _$E65
// FUNCTION: STBC 0x004618c0 _$E64
// FUNCTION: STBC 0x004618d0 _$E76
// FUNCTION: STBC 0x004618e0 _$E75
// FUNCTION: STBC 0x004618f0 _$E80
// FUNCTION: STBC 0x00461900 _$E79
// FUNCTION: STBC 0x00461910 _$E84
// FUNCTION: STBC 0x00461920 _$E83
// FUNCTION: STBC 0x00461930 _$E88
// FUNCTION: STBC 0x00461940 _$E87
// FUNCTION: STBC 0x00461950 _$E208
// FUNCTION: STBC 0x00461960 _$E207
// FUNCTION: STBC 0x00461970 _$E150
// FUNCTION: STBC 0x00461980 _$E149
// FUNCTION: STBC 0x00461990 _$E163
// FUNCTION: STBC 0x004619a0 _$E162
// FUNCTION: STBC 0x004619b0 _$E213
// FUNCTION: STBC 0x004619c0 _$E212
// FUNCTION: STBC 0x004619d0 _$E218
// FUNCTION: STBC 0x004619e0 _$E217
// FUNCTION: STBC 0x004619f0 _$E222
// FUNCTION: STBC 0x00461a00 _$E221
// FUNCTION: STBC 0x00461a10 _$E226
// FUNCTION: STBC 0x00461a20 _$E225
// FUNCTION: STBC 0x00461a30 _$E233
// FUNCTION: STBC 0x00461a40 _$E232
// FUNCTION: STBC 0x00461a50 _$E274
// FUNCTION: STBC 0x00461a60 _$E273
// FUNCTION: STBC 0x00461a70 _$E278
// FUNCTION: STBC 0x00461a80 _$E277
// FUNCTION: STBC 0x00461a90 _$E112
// FUNCTION: STBC 0x00461aa0 _$E111
// FUNCTION: STBC 0x00461ab0 _$E260
// FUNCTION: STBC 0x00461ac0 _$E259
// FUNCTION: STBC 0x00461ad0 _$E267
// FUNCTION: STBC 0x00461ae0 _$E266
// FUNCTION: STBC 0x00461af0 _$E290
// FUNCTION: STBC 0x00461b00 _$E289
// FUNCTION: STBC 0x00461b10 _$E294
// FUNCTION: STBC 0x00461b20 _$E293
// FUNCTION: STBC 0x00461b30 _$E334
// FUNCTION: STBC 0x00461b40 _$E333
// FUNCTION: STBC 0x00461b50 _$E340
// FUNCTION: STBC 0x00461b60 _$E339
// FUNCTION: STBC 0x00461b70 _$E282
// FUNCTION: STBC 0x00461b80 _$E281
// FUNCTION: STBC 0x00461b90 _$E286
// FUNCTION: STBC 0x00461ba0 _$E285
// FUNCTION: STBC 0x00461bb0 _$E298
// FUNCTION: STBC 0x00461bc0 _$E297
// FUNCTION: STBC 0x00461bd0 _$E302
// FUNCTION: STBC 0x00461be0 _$E301
// FUNCTION: STBC 0x00461bf0 _$E306
// FUNCTION: STBC 0x00461c00 _$E305
// FUNCTION: STBC 0x00461c10 _$E310
// FUNCTION: STBC 0x00461c20 _$E309
// FUNCTION: STBC 0x00461c30 _$E314
// FUNCTION: STBC 0x00461c40 _$E313
// FUNCTION: STBC 0x00461c50 _$E318
// FUNCTION: STBC 0x00461c60 _$E317
// FUNCTION: STBC 0x00461c70 _$E322
// FUNCTION: STBC 0x00461c80 _$E321
// FUNCTION: STBC 0x00461c90 _$E326
// FUNCTION: STBC 0x00461ca0 _$E325
// FUNCTION: STBC 0x00461cb0 _$E117
// FUNCTION: STBC 0x00461cc0 _$E116
// FUNCTION: STBC 0x00461d00 _$E330
// FUNCTION: STBC 0x00461d10 _$E329
// FUNCTION: STBC 0x00461d20 _$E122
// FUNCTION: STBC 0x00461d30 _$E121
// FUNCTION: STBC 0x00461d40 _$E344
// FUNCTION: STBC 0x00461d50 _$E343
// FUNCTION: STBC 0x00461d60 _$E348
// FUNCTION: STBC 0x00461d70 _$E347
// FUNCTION: STBC 0x00461d80 _$E352
// FUNCTION: STBC 0x00461d90 _$E351
// FUNCTION: STBC 0x00461da0 _$E356
// FUNCTION: STBC 0x00461db0 _$E355
// FUNCTION: STBC 0x00461dc0 _$E127
// FUNCTION: STBC 0x00461dd0 _$E126
// FUNCTION: STBC 0x00461de0 _$E360
// FUNCTION: STBC 0x00461df0 _$E359
// FUNCTION: STBC 0x00461e00 _$E364
// FUNCTION: STBC 0x00461e10 _$E363
// FUNCTION: STBC 0x00461e20 _$E368
// FUNCTION: STBC 0x00461e30 _$E367
// FUNCTION: STBC 0x00461e40 _$E133
// FUNCTION: STBC 0x00461e50 _$E132
// FUNCTION: STBC 0x00461e60 _$E138
// FUNCTION: STBC 0x00461e70 _$E137
// FUNCTION: STBC 0x00461e80 _$E168
// FUNCTION: STBC 0x00461e90 _$E167
// FUNCTION: STBC 0x00461ea0 _$E372
// FUNCTION: STBC 0x00461eb0 _$E371
// FUNCTION: STBC 0x00461ec0 _$E173
// FUNCTION: STBC 0x00461ed0 _$E172
// FUNCTION: STBC 0x00461ee0 _$E376
// FUNCTION: STBC 0x00461ef0 _$E375
// FUNCTION: STBC 0x00461f00 _$E380
// FUNCTION: STBC 0x00461f10 _$E379
// FUNCTION: STBC 0x00461f20 _$E178
// FUNCTION: STBC 0x00461f30 _$E177
// FUNCTION: STBC 0x00461f40 _$E384
// FUNCTION: STBC 0x00461f50 _$E383
// FUNCTION: STBC 0x00461f60 _$E143
// FUNCTION: STBC 0x00461f70 _$E142
// FUNCTION: STBC 0x00461f80 _$E388
// FUNCTION: STBC 0x00461f90 _$E387
// FUNCTION: STBC 0x00461fa0 _$E392
// FUNCTION: STBC 0x00461fb0 _$E391
// FUNCTION: STBC 0x00461fc0 _$E183
// FUNCTION: STBC 0x00461fd0 _$E182
// FUNCTION: STBC 0x00461fe0 _$E188
// FUNCTION: STBC 0x00461ff0 _$E187
// FUNCTION: STBC 0x00462000 _$E396
// FUNCTION: STBC 0x00462010 _$E395
// FUNCTION: STBC 0x00462020 _$E400
// FUNCTION: STBC 0x00462030 _$E399
// FUNCTION: STBC 0x00462040 _$E404
// FUNCTION: STBC 0x00462050 _$E403
// FUNCTION: STBC 0x00462060 _$E408
// FUNCTION: STBC 0x00462070 _$E407
// FUNCTION: STBC 0x00462080 _$E412
// FUNCTION: STBC 0x00462090 _$E411
// FUNCTION: STBC 0x004620a0 _$E416
// FUNCTION: STBC 0x004620b0 _$E415
// FUNCTION: STBC 0x004620c0 _$E420
// FUNCTION: STBC 0x004620d0 _$E419
// FUNCTION: STBC 0x004620e0 _$E424
// FUNCTION: STBC 0x004620f0 _$E423
// FUNCTION: STBC 0x00462100 _$E193
// FUNCTION: STBC 0x00462110 _$E192
// FUNCTION: STBC 0x00462120 _$E428
// FUNCTION: STBC 0x00462130 _$E427
// FUNCTION: STBC 0x00462140 _$E198
// FUNCTION: STBC 0x00462150 _$E197
// FUNCTION: STBC 0x00462160 _$E203
// FUNCTION: STBC 0x00462170 _$E202
// FUNCTION: STBC 0x00462180 _$E432
// FUNCTION: STBC 0x00462190 _$E431
// FUNCTION: STBC 0x004621a0 _$E437
// FUNCTION: STBC 0x004621b0 _$E436
// FUNCTION: STBC 0x004621c0 _$E441
// FUNCTION: STBC 0x004621d0 _$E440

// GetConnectStatus' value of a connection that is up.
#define NETWORK_CONNECTED		2

// The locale id's guard (std::ctype<unsigned short>::id) and its exit-time
// registration, after the unit's NetImmerse registrations.
// FUNCTION: STBC 0x00461cd0 _$E455
// FUNCTION: STBC 0x00461cf0 _$E454

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004611d0 _$E447
// FUNCTION: STBC 0x004611e0 _$E443
// FUNCTION: STBC 0x004611f0 _$E446
// FUNCTION: STBC 0x00461200 _$E444
// DATA: STBC 0x00981138
TGEventHandlerTable ChatObjectClass::classHandlers;

/*
================
ChatObjectClass::ChatObjectClass

Opens a network connection to the address and adds the chat's handlers for
its events.
================
*/
// FUNCTION: STBC 0x00461220
ChatObjectClass::ChatObjectClass( char *address ) : TGEventHandlerObject( 0 ) {
	network = new TGWinsockNetwork;
	if ( network != NULL ) {
		network->Connect( address, NULL );
	}
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_MESSAGE_EVENT, this, "ChatObjectClass :: ReceiveMessageHandler", network );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_CONNECT_EVENT, this, "ChatObjectClass :: ConnectHandler", network );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_DISCONNECT_EVENT, this, "ChatObjectClass :: DisconnectHandler", network );
	unknown18 = true;
}

/*
================
ChatObjectClass::~ChatObjectClass

Closes the connection and has the network delete itself.
================
*/
// FUNCTION: STBC 0x00461310
ChatObjectClass::~ChatObjectClass( void ) {
	if ( network != NULL ) {
		network->Disconnect();
		ConsoleWindow *consoleWindow = (ConsoleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CONSOLE );
		if ( consoleWindow != NULL ) {
			TGConsole *console = consoleWindow->GetConsole();
			if ( console != NULL ) {
				console->AddConsoleString( "Network connection closed." );
			}
		}
		TGEvent *deleteEvent = new TGEvent;
		deleteEvent->SetEventType( ET_DELETE_ME_STAGE_1 );
		deleteEvent->SetDestination( network );
		g_kEventManager.AddEvent( deleteEvent );
		network = NULL;
	}
}

/*
================
ChatObjectClass::Create
================
*/
// FUNCTION: STBC 0x004613f0
ChatObjectClass *ChatObjectClass::Create( char *address ) {
	return new ChatObjectClass( address );
}

/*
================
ChatObjectClass::Disconnect
================
*/
// FUNCTION: STBC 0x00461420
void ChatObjectClass::Disconnect( void ) {
	if ( network != NULL ) {
		network->Disconnect();
	}
}

/*
================
ChatObjectClass::IsChatObjectReady
================
*/
// FUNCTION: STBC 0x00461430
bool ChatObjectClass::IsChatObjectReady( void ) {
	if ( network != NULL ) {
		return network->GetConnectStatus() == NETWORK_CONNECTED ? true : false;
	}
	return false;
}

/*
================
ChatObjectClass::GetLocalIPAddress

Prints the network's local address on the console window, when there is
one.
================
*/
// FUNCTION: STBC 0x00461450
void ChatObjectClass::GetLocalIPAddress( void ) {
	if ( network != NULL ) {
		char *address = network->GetLocalIPAddress();
		ConsoleWindow *consoleWindow = (ConsoleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CONSOLE );
		if ( consoleWindow != NULL ) {
			TGConsole *console = consoleWindow->GetConsole();
			if ( console != NULL ) {
				console->AddConsoleString( address );
			}
		}
	}
}

/*
================
ChatObjectClass::SendChatMessage

Sends the text, with its terminating zero, as a message; true when the
network took it.
================
*/
// FUNCTION: STBC 0x00461490
bool ChatObjectClass::SendChatMessage( char *text ) {
	if ( IsChatObjectReady() ) {
		TGMessage *message = new TGMessage;
		message->SetData( text, strlen( text ) + 1 );
		return network->SendTGMessage( 0, message, 0 ) == 0;
	}
	return false;
}

/*
================
ChatObjectClass::RegisterHandlerNames

The chat's handler methods by their class-qualified names: the received
message, and the opening and closing of the connection.
================
*/
// FUNCTION: STBC 0x00461500
void ChatObjectClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ChatObjectClass::ReceiveMessageHandler, "ChatObjectClass :: ReceiveMessageHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ChatObjectClass::ConnectHandler, "ChatObjectClass :: ConnectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ChatObjectClass::DisconnectHandler, "ChatObjectClass :: DisconnectHandler" );
}

/*
================
ChatObjectClass::RegisterHandlers

Nothing goes in the class's table: the handlers are added for each instance.
================
*/
// FUNCTION: STBC 0x00461540
void ChatObjectClass::RegisterHandlers( void ) {}

/*
================
ChatObjectClass::ReceiveMessageHandler

A chat message came for this chat's network: the console window shows it as
"<player name>: <text>".
================
*/
// FUNCTION: STBC 0x00461550
void ChatObjectClass::ReceiveMessageHandler( TGMessageEvent *event ) {
	if ( event->GetDestination() == network && IsChatObjectReady() ) {
		TGMessage *message = event->GetMessage();
		if ( message->VirtualSlot00() == TGMESSAGE_TYPE ) {
			int length;
			char *data = message->GetData( &length );
			// BUG: no NULL test: a message from an unknown player reads the name of a NULL player
			char *name = network->GetPlayerList()->GetPlayer( message->GetFromID() )->GetName()->Unknown6F56F0();
			int size = strlen( name != NULL ? name : "Unknown" );
			if ( data != NULL ) {
				size += length;
			}
			char *line = new char[ size + 3 ];
			line[0] = '\0';
			if ( name != NULL ) {
				strcpy( line, name );
				delete name;
			} else {
				strcpy( line, "Unknown" );
			}
			strcat( line, ": " );
			if ( data != NULL ) {
				strcat( line, data );
			}
			ConsoleWindow *consoleWindow = (ConsoleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CONSOLE );
			if ( consoleWindow != NULL ) {
				TGConsole *console = consoleWindow->GetConsole();
				if ( console != NULL ) {
					console->AddConsoleString( line );
				}
			}
		}
	}
}

/*
================
ChatObjectClass::ConnectHandler

The connection came up for this chat: the local player joins under the name
"host" or "client", and the console window says so. Only while the flag of
the closing connection is still set, and once per connection.
================
*/
// FUNCTION: STBC 0x004616f0
void ChatObjectClass::ConnectHandler( TGEvent *event ) {
	if ( event->GetDestination() == network && unknown18 ) {
		TGString name;
		if ( network->IsHost() ) {
			name.AssignC( "host" );
		} else {
			name.AssignC( "client" );
		}
		network->CreateLocalPlayer( &name );
		unknown18 = false;
		ConsoleWindow *consoleWindow = (ConsoleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CONSOLE );
		if ( consoleWindow != NULL ) {
			TGConsole *console = consoleWindow->GetConsole();
			if ( console != NULL ) {
				console->AddConsoleString( "Network connection completed." );
			}
		}
	}
}

/*
================
ChatObjectClass::DisconnectHandler

The connection closed: the console window says so, and the network the chat
goes over is dropped.
================
*/
// FUNCTION: STBC 0x004617c0
void ChatObjectClass::DisconnectHandler( TGEvent *event ) {
	unknown18 = true;
	ConsoleWindow *consoleWindow = (ConsoleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CONSOLE );
	if ( consoleWindow != NULL ) {
		TGConsole *console = consoleWindow->GetConsole();
		if ( console != NULL ) {
			console->AddConsoleString( "Network connection closed." );
		}
	}
	if ( network != NULL ) {
		delete network;
		network = NULL;
	}
}
