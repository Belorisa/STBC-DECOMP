/*
===============================================================================

	MultiplayerWindow.cpp

	The multiplayer menus' main window.

===============================================================================
*/

#include <stdio.h>
#include <string.h>

#include "MultiplayerWindow.h"
#include "Game.h"
#include "MultiplayerGame.h"
#include "TGConfigMapping.h"
#include "goaceng.h"
#include "TGBoolEvent.h"
#include "TGIntEvent.h"
#include "TGPane.h"
#include "TGUIObject.h"
#include "MultiplayerPlayer.h"
#include "NetFile.h"
#include "OptionsWindow.h"
#include "ProximityManager.h"
#include "PyEmbed.h"
#include "STMenu.h"
#include "STStylizedWindow.h"
#include "TGParagraph.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include "UtopiaModule.h"
#include "STButton.h"
#include "TGBootPlayerMessage.h"
#include "TGBufferStream.h"
#include "TGDirectory.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMessage.h"
#include "TGMessageEvent.h"
#include "TGMouseEvent.h"
#include "TGNetwork.h"
#include "TGPlayerEvent.h"
#include "TGStream.h"
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "TGStringEvent.h"
#include "VarManagerClass.h"
#include <iostream>
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiTriBasedGeomData.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "GameSpy.h"
#include "ServerListEvent.h"
#include "TGBoolEvent.h"
#include "TGIntEvent.h"
#include "TGPane.h"
#include "TGUIObject.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005041d0 _$E33
// FUNCTION: STBC 0x005041e0 _$E32
// FUNCTION: STBC 0x005041f0 _$E37
// FUNCTION: STBC 0x00504200 _$E36
// FUNCTION: STBC 0x00504210 _$E48
// FUNCTION: STBC 0x00504220 _$E47
// FUNCTION: STBC 0x00504230 _$E52
// FUNCTION: STBC 0x00504240 _$E51
// FUNCTION: STBC 0x00504250 _$E56
// FUNCTION: STBC 0x00504260 _$E55
// FUNCTION: STBC 0x00504270 _$E60
// FUNCTION: STBC 0x00504280 _$E59
// FUNCTION: STBC 0x00504290 _$E184
// FUNCTION: STBC 0x005042a0 _$E183
// FUNCTION: STBC 0x005042b0 _$E126
// FUNCTION: STBC 0x005042c0 _$E125
// FUNCTION: STBC 0x005042d0 _$E139
// FUNCTION: STBC 0x005042e0 _$E138
// FUNCTION: STBC 0x005042f0 _$E189
// FUNCTION: STBC 0x00504300 _$E188
// FUNCTION: STBC 0x00504870 _$E194
// FUNCTION: STBC 0x00504880 _$E193
// FUNCTION: STBC 0x00504bf0 _$E198
// FUNCTION: STBC 0x00504c00 _$E197
// FUNCTION: STBC 0x00505460 _$E202
// FUNCTION: STBC 0x00505470 _$E201
// FUNCTION: STBC 0x00505750 _$E209
// FUNCTION: STBC 0x00505760 _$E208
// FUNCTION: STBC 0x00505de0 _$E334
// FUNCTION: STBC 0x00505df0 _$E333
// FUNCTION: STBC 0x00506130 _$E338
// FUNCTION: STBC 0x00506140 _$E337
// FUNCTION: STBC 0x00506150 _$E84
// FUNCTION: STBC 0x00506160 _$E83
// FUNCTION: STBC 0x005068f0 _$E313
// FUNCTION: STBC 0x00506900 _$E312
// FUNCTION: STBC 0x00506bb0 _$E320
// FUNCTION: STBC 0x00506bc0 _$E319
// FUNCTION: STBC 0x00506ef0 _$E324
// FUNCTION: STBC 0x00506f00 _$E323
// FUNCTION: STBC 0x00506f50 _$E350
// FUNCTION: STBC 0x00506f60 _$E349
// FUNCTION: STBC 0x00506f70 _$E390
// FUNCTION: STBC 0x00506f80 _$E389
// FUNCTION: STBC 0x00506f90 _$E396
// FUNCTION: STBC 0x00506fa0 _$E395
// FUNCTION: STBC 0x00506fb0 _$E342
// FUNCTION: STBC 0x00506fc0 _$E341
// FUNCTION: STBC 0x00506fd0 _$E346
// FUNCTION: STBC 0x00506fe0 _$E345
// FUNCTION: STBC 0x00506ff0 _$E354
// FUNCTION: STBC 0x00507000 _$E353
// FUNCTION: STBC 0x00507010 _$E358
// FUNCTION: STBC 0x00507020 _$E357
// FUNCTION: STBC 0x00507030 _$E362
// FUNCTION: STBC 0x00507040 _$E361
// FUNCTION: STBC 0x00507050 _$E366
// FUNCTION: STBC 0x00507060 _$E365
// FUNCTION: STBC 0x00507070 _$E370
// FUNCTION: STBC 0x00507080 _$E369
// FUNCTION: STBC 0x00507090 _$E374
// FUNCTION: STBC 0x005070a0 _$E373
// FUNCTION: STBC 0x005070b0 _$E378
// FUNCTION: STBC 0x005070c0 _$E377
// FUNCTION: STBC 0x005070d0 _$E382
// FUNCTION: STBC 0x005070e0 _$E381
// FUNCTION: STBC 0x005070f0 _$E89
// FUNCTION: STBC 0x00507100 _$E88
// FUNCTION: STBC 0x00507110 _$E386
// FUNCTION: STBC 0x00507120 _$E385
// FUNCTION: STBC 0x00507130 _$E94
// FUNCTION: STBC 0x00507140 _$E93
// FUNCTION: STBC 0x00507150 _$E400
// FUNCTION: STBC 0x00507160 _$E399
// FUNCTION: STBC 0x00507170 _$E404
// FUNCTION: STBC 0x00507180 _$E403
// FUNCTION: STBC 0x00507190 _$E408
// FUNCTION: STBC 0x005071a0 _$E407
// FUNCTION: STBC 0x005071b0 _$E412
// FUNCTION: STBC 0x005071c0 _$E411
// FUNCTION: STBC 0x005071d0 _$E103
// FUNCTION: STBC 0x005071e0 _$E102
// FUNCTION: STBC 0x005071f0 _$E416
// FUNCTION: STBC 0x00507200 _$E415
// FUNCTION: STBC 0x00507210 _$E420
// FUNCTION: STBC 0x00507220 _$E419
// FUNCTION: STBC 0x00507230 _$E424
// FUNCTION: STBC 0x00507240 _$E423
// FUNCTION: STBC 0x00507250 _$E109
// FUNCTION: STBC 0x00507260 _$E108
// FUNCTION: STBC 0x00507270 _$E114
// FUNCTION: STBC 0x00507280 _$E113
// FUNCTION: STBC 0x00507290 _$E144
// FUNCTION: STBC 0x005072a0 _$E143
// FUNCTION: STBC 0x005072b0 _$E428
// FUNCTION: STBC 0x005072c0 _$E427
// FUNCTION: STBC 0x005072d0 _$E149
// FUNCTION: STBC 0x005072e0 _$E148
// FUNCTION: STBC 0x005072f0 _$E432
// FUNCTION: STBC 0x00507300 _$E431
// FUNCTION: STBC 0x00507310 _$E436
// FUNCTION: STBC 0x00507320 _$E435
// FUNCTION: STBC 0x00507330 _$E154
// FUNCTION: STBC 0x00507340 _$E153
// FUNCTION: STBC 0x00507350 _$E440
// FUNCTION: STBC 0x00507360 _$E439
// FUNCTION: STBC 0x00507370 _$E119
// FUNCTION: STBC 0x00507380 _$E118
// FUNCTION: STBC 0x00507390 _$E444
// FUNCTION: STBC 0x005073a0 _$E443
// FUNCTION: STBC 0x005073b0 _$E448
// FUNCTION: STBC 0x005073c0 _$E447
// FUNCTION: STBC 0x005073d0 _$E159
// FUNCTION: STBC 0x005073e0 _$E158
// FUNCTION: STBC 0x005073f0 _$E164
// FUNCTION: STBC 0x00507400 _$E163
// FUNCTION: STBC 0x00507410 _$E452
// FUNCTION: STBC 0x00507420 _$E451
// FUNCTION: STBC 0x00507430 _$E456
// FUNCTION: STBC 0x00507440 _$E455
// FUNCTION: STBC 0x00507450 _$E460
// FUNCTION: STBC 0x00507460 _$E459
// FUNCTION: STBC 0x00507470 _$E464
// FUNCTION: STBC 0x00507480 _$E463
// FUNCTION: STBC 0x00507490 _$E468
// FUNCTION: STBC 0x005074a0 _$E467
// FUNCTION: STBC 0x005074b0 _$E472
// FUNCTION: STBC 0x005074c0 _$E471
// FUNCTION: STBC 0x005074d0 _$E476
// FUNCTION: STBC 0x005074e0 _$E475
// FUNCTION: STBC 0x005074f0 _$E480
// FUNCTION: STBC 0x00507500 _$E479
// FUNCTION: STBC 0x00507510 _$E169
// FUNCTION: STBC 0x00507520 _$E168
// FUNCTION: STBC 0x00507530 _$E484
// FUNCTION: STBC 0x00507540 _$E483
// FUNCTION: STBC 0x00507550 _$E174
// FUNCTION: STBC 0x00507560 _$E173
// FUNCTION: STBC 0x00507570 _$E179
// FUNCTION: STBC 0x00507580 _$E178
// FUNCTION: STBC 0x005075c0 _$E488
// FUNCTION: STBC 0x005075d0 _$E487
// FUNCTION: STBC 0x005075e0 _$E493
// FUNCTION: STBC 0x005075f0 _$E492
// FUNCTION: STBC 0x00507600 _$E497
// FUNCTION: STBC 0x00507610 _$E496

// The scripts' module that builds the multiplayer menus and keeps their panes in globals.
#define MULTIPLAYER_MENUS		"Multiplayer.MultiplayerMenus"

// What the Tactical script's empty area is when the script can't answer, and the gaps
// the chat window keeps from it (fractions of the screen).
#define DEFAULT_EMPTY_AREA_LEFT		0.25625f
#define DEFAULT_EMPTY_AREA_BOTTOM	0.72f
#define CHAT_MARGIN					0.02f
#define PARAGRAPH_MARGIN			0.01f

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x00503e40 _$E511
// FUNCTION: STBC 0x00503e20 _$E512

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00504160 _$E299
// FUNCTION: STBC 0x00504180 _$E300
// FUNCTION: STBC 0x00504170 _$E301
// FUNCTION: STBC 0x00504150 _$E302
// FUNCTION: STBC 0x005041a0 _$E305
// FUNCTION: STBC 0x005041c0 _$E306
// FUNCTION: STBC 0x005041b0 _$E307
// FUNCTION: STBC 0x00504190 _$E308

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00504320 _$E500
// FUNCTION: STBC 0x00504340 _$E501
// FUNCTION: STBC 0x00504330 _$E503
// FUNCTION: STBC 0x00504310 _$E504
// DATA: STBC 0x009872e0
TGEventHandlerTable MultiplayerWindow::classHandlers;

// The server a game was joined from, kept while the game that runs ends first (invented names).
// DATA: STBC 0x00987318
static char *	savedServerAddress;
// DATA: STBC 0x0098731c
static bool		haveSavedServer;
// DATA: STBC 0x00987320
static int		savedServerPort;

/*
================
MultiplayerWindow::Cast

The object as a MultiplayerWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00504360
MultiplayerWindow *MultiplayerWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( MULTIPLAYERWINDOW_TYPE ) ) {
		return (MultiplayerWindow *)object;
	}
	return NULL;
}

/*
================
MultiplayerWindow::MultiplayerWindow

Hidden at first, the host on a modem until told otherwise.
================
*/
// FUNCTION: STBC 0x00504390
MultiplayerWindow::MultiplayerWindow( void ) : MainWindow( MWT_MULTIPLAYER, 1.0f, 1.0f ) {
	multiplayerMenu = NULL;
	hostMenu = NULL;
	dedicatedServerMenu = NULL;
	joinMenu = NULL;
	missionPane = NULL;
	statusWindow = NULL;
	endWindow = NULL;
	chatWindow = NULL;
	playerListWindow = NULL;
	currentServer = NULL;
	directJoinMenu = NULL;
	hostUsingModem = true;
	connected = false;
	gameOver = true;
	directJoin = false;
	teamChat = false;
	chosenMissionButtonID = 0;
	// The window's own SetNotVisible only takes it off the options window's
	// list while that is open; the base call hides it in any case.
	SetNotVisible();
	MainWindow::SetNotVisible();
}

/*
================
MultiplayerWindow::GetObjType
================
*/
// FUNCTION: STBC 0x00504480
int MultiplayerWindow::GetObjType( void ) {
	return MULTIPLAYERWINDOW_TYPE;
}

/*
================
MultiplayerWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00504490
bool MultiplayerWindow::IsTypeOf( int type ) {
	if ( type == MULTIPLAYERWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
MultiplayerWindow::GetClassName
================
*/
// FUNCTION: STBC 0x005044f0
char *MultiplayerWindow::GetClassName( void ) {
	return "MultiplayerWindow";
}

/*
================
MultiplayerWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00504500
char *MultiplayerWindow::GetSwigTypeName( void ) {
	return "_p_MultiplayerWindow";
}

/*
================
MultiplayerWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00504510
char *MultiplayerWindow::GetPythonClassName( void ) {
	return "MultiplayerWindowPtr";
}

/*
================
MultiplayerWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00504520
TGEventHandlerTable *MultiplayerWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
MultiplayerWindow::~MultiplayerWindow
================
*/
// FUNCTION: STBC 0x00504560
MultiplayerWindow::~MultiplayerWindow( void ) {
}

/*
================
MultiplayerWindow::Initialize

The scripts build the menus and set up their handlers; the window then
takes the network's message, connect and disconnect events.
================
*/
// FUNCTION: STBC 0x005045e0
void MultiplayerWindow::Initialize( void ) {
	if ( PyEmbed::CallFunction( MULTIPLAYER_MENUS, "BuildMultiplayerMenus", NULL, NULL, NULL, NULL ) == -1 ) {
		PyErr_Print();
		return;
	}

	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "MultiplayerInterfaceHandlers", "Initialize", "", NULL, "(O)", self );
	Py_DECREF( self );

	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_MESSAGE_EVENT, this, "MultiplayerWindow::ReceiveMessageHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_CONNECT_EVENT, this, "MultiplayerWindow::ConnectHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_DISCONNECT_EVENT, this, "MultiplayerWindow::DisconnectHandler" );
}

/*
================
MultiplayerWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005046b0
void MultiplayerWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::StartGameHandler, "MultiplayerWindow::StartGameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::ReceiveMessageHandler, "MultiplayerWindow::ReceiveMessageHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::ConnectHandler, "MultiplayerWindow::ConnectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::DisconnectHandler, "MultiplayerWindow::DisconnectHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::SetMissionNameHandler, "MultiplayerWindow::SetMissionNameHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::RefreshServerListHandler, "MultiplayerWindow::RefreshServerListHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::SelectServerHandler, "MultiplayerWindow::SelectServerHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::SortServerListHandler, "MultiplayerWindow::SortServerListHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MultiplayerWindow::BootPlayerHandler, "MultiplayerWindow::BootPlayerHandler" );
}

/*
================
MultiplayerWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00504770
void MultiplayerWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SET_MISSION_NAME, "MultiplayerWindow::SetMissionNameHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_START, "MultiplayerWindow::StartGameHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_REFRESH_SERVER_LIST, "MultiplayerWindow::RefreshServerListHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_SELECT_SERVER_ENTRY, "MultiplayerWindow::SelectServerHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_SORT_SERVER_LIST, "MultiplayerWindow::SortServerListHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_PLAYER_BOOT_EVENT, "MultiplayerWindow::BootPlayerHandler" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "MultiplayerInterfaceHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "MultiplayerInterfaceHandlers.HandleKeyboard" );
}

/*
================
MultiplayerWindow::Update
================
*/
// FUNCTION: STBC 0x00504820
void MultiplayerWindow::Update( float time ) {
}

/*
================
MultiplayerWindow::GotFocus
================
*/
// FUNCTION: STBC 0x00504830
void MultiplayerWindow::GotFocus( bool layout ) {
	TGUIObject::GotFocus( false );
	SetVisible( layout );
}

/*
================
MultiplayerWindow::LostFocus
================
*/
// FUNCTION: STBC 0x00504850
void MultiplayerWindow::LostFocus( bool layout ) {
	TGUIObject::LostFocus( false );
	SetNotVisible( layout );
}

/*
================
MultiplayerWindow::StartGameHandler

Starts the game the menus set up. A game that runs ends first: the server
chosen in the list is kept for the start that follows, and the event is held
for the game to send on. Without one the window takes the screen, says it is
connecting and starts the network: as a host or a client, under the names and
password the options file holds, joining by the direct join address, the kept
server or the server chosen in the list. A failure other than the plain
ones is shown in the status message.
================
*/
// FUNCTION: STBC 0x00504890
// PARKED: register: retail compares the chosen server against its NULL address register (cmp eax, edi) where this build tests it, and schedules the password's address load after the GetTGStringValue call; declaring address in the else branch costs 7 points
void MultiplayerWindow::StartGameHandler( TGEvent *event ) {
	TGString dedicatedName( "" );
	Game *game = Game::GetCurrentGame();
	char *address = NULL;
	if ( game != NULL ) {
		if ( !g_kUtopiaModule.IsHost() && !directJoin && currentServer != NULL ) {
			char *serverAddress = ServerGetAddress( (GServer)currentServer );
			if ( serverAddress != NULL ) {
				haveSavedServer = true;
				savedServerPort = ServerGetQueryPort( (GServer)currentServer );
				savedServerAddress = new char[ strlen( serverAddress ) + 1 ];
				strcpy( savedServerAddress, serverAddress );
			}
		}
		event->IncRefCount();
		game->Terminate( event );
	} else {
		HideAllChildren();
		TopWindow *topWindow = TopWindow::GetTopWindow();
		topWindow->MoveToFront( this, false );
		statusWindow->SetVisible( true );
		MainWindow *options = topWindow->FindMainWindow( MWT_OPTIONS );
		if ( options->IsVisible() ) {
			topWindow->ToggleOptionsMenu();
		}
		SetVisible( true );

		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
		TGString *connecting = database->GetString( "Connecting" );
		TGString status( "" );
		status = *connecting;
		int port = 0x5655;
		SetStatusMessage( &status );

		if ( g_kUtopiaModule.IsHost() ) {
			gameName = *g_kConfigMapping.GetTGStringValue( "Multiplayer Options", "Game Name" );
			if ( !g_kUtopiaModule.IsClient() ) {
				dedicatedName.AssignC( "Dedicated Server0123" );
				playerName = dedicatedName;
			} else {
				playerName = *g_kConfigMapping.GetTGStringValue( "Multiplayer Options", "Player Name" );
			}
		} else {
			if ( directJoin ) {
				address = g_kConfigMapping.GetTGStringValue( "Multiplayer Options", "Direct Join Address" )->Unknown6F56F0();
			} else if ( haveSavedServer ) {
				address = savedServerAddress;
				port = savedServerPort;
			} else if ( currentServer != NULL ) {
				address = ServerGetAddress( (GServer)currentServer );
				port = ServerGetQueryPort( (GServer)currentServer );
			} else {
				address = (char *)event;	// BUG: the event is taken for the address
			}
			playerName = *g_kConfigMapping.GetTGStringValue( "Multiplayer Options", "Player Name" );
		}
		password = *g_kConfigMapping.GetTGStringValue( "Multiplayer Options", "Password" );

		int result = g_kUtopiaModule.InitializeNetwork( address, &password, port );
		connected = false;
		if ( address != NULL && directJoin ) {
			delete address;
		}
		if ( haveSavedServer ) {
			if ( savedServerAddress != NULL ) {
				delete savedServerAddress;
				savedServerAddress = NULL;
			}
			haveSavedServer = false;
		}
		if ( result != 0 && result != 2 && result != 3 ) {
			TGString *text;
			if ( result != 0xB ) {
				text = database->GetString( "Synchronizing Game Data" );
			} else {
				text = database->GetString( "Invalid Address" );
			}
			if ( text != NULL ) {
				TGString message( "" );
				message = *text;
				SetStatusMessage( &message );
			}
		}

		g_kLocalizationManager.Unload( database );
	}
	CallNextHandler( event );
}
/*
================
MultiplayerWindow::ReceiveMessageHandler

Passes the game set-up messages on by their types, once connected.
================
*/
// FUNCTION: STBC 0x00504c10
void MultiplayerWindow::ReceiveMessageHandler( TGMessageEvent *event ) {
	if ( connected ) {
		TGMessage *message = event->GetMessage();
		if ( message->VirtualSlot00() == TGMESSAGE_TYPE ) {
			switch ( *message->GetData( NULL ) ) {
				case GAME_INITIALIZE_MESSAGE:
					ReceiveGameInitializeMessage( message );
					break;
				case GAME_INITIALIZE_DONE_MESSAGE:
					ReceiveGameInitializeDoneMessage( message );
					break;
				case COLLISION_ENABLED_MESSAGE:
					ReceiveCollisionEnabledMessage( message );
					break;
			}
		}
	}
}

/*
================
MultiplayerWindow::ReceiveCollisionEnabledMessage

The host's collision setting, shown on the main menu's collision button.
================
*/
// FUNCTION: STBC 0x00504c70
void MultiplayerWindow::ReceiveCollisionEnabledMessage( TGMessage *message ) {
	TGBufferStream stream;
	int length;
	char *data = message->GetData( &length );
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	bool enabled = stream.ReadBool();
	ProximityManager::multiplayerPlayerCollisionsEnabled = enabled;

	PyObject *object;
	if ( PyEmbed::GetValue( "MainMenu.mainmenu", "g_pCollisionButton", "O", &object ) != -1 ) {
		STButton *button = (STButton *)PyEmbed::GetObjectPointer( object );
		button->SetChosen( enabled );
	}
}

/*
================
MultiplayerWindow::ReceiveGameInitializeMessage

The host's game time, collision setting and modem flag, the player number
the host gave this player, the mission and the server's name.
================
*/
// FUNCTION: STBC 0x00504d30
void MultiplayerWindow::ReceiveGameInitializeMessage( TGMessage *message ) {
	TGBufferStream stream;
	int length;
	char *data = message->GetData( &length );
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	float gameTime = stream.ReadFloat();
	UtopiaApp::GetApp()->SetGameTime( gameTime );
	ProximityManager::multiplayerPlayerCollisionsEnabled = stream.ReadBool();
	hostUsingModem = stream.ReadBool();

	int playerNumber = stream.ReadChar();
	g_kUtopiaModule.SetPlayerNumber( playerNumber );
	g_kUtopiaModule.SetUnusedClientID( playerNumber * CLIENT_ID_BLOCK_SIZE + FIRST_CLIENT_ID );

	int missionLength = stream.ReadShort();
	if ( missionLength > 0 ) {
		char mission[1024];
		stream.Read( mission, missionLength );
		mission[missionLength] = '\0';
		g_kVarManager.SetStringVariable( "Multiplayer", "Mission", mission );
	}

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *synchronizing = database->GetString( "Synchronizing Game Data" );
	TGString status;
	status = *synchronizing;
	SetStatusMessage( &status );
	g_kLocalizationManager.Unload( database );

	if ( stream.ReadBool() ) {
		TGString name;
		name.ReadFromStream( &stream );
		if ( g_kUtopiaModule.GetNetwork() != NULL ) {
			g_kUtopiaModule.GetNetwork()->SetName( &name );
		}
	}
}

/*
================
MultiplayerWindow::ReceiveGameInitializeDoneMessage

Starts the multiplayer game; the host takes its player limit from the
mission menus.
================
*/
// FUNCTION: STBC 0x00504f10
void MultiplayerWindow::ReceiveGameInitializeDoneMessage( TGMessage *message ) {
	PyEmbed::CallFunction( "AI.Setup", "GameInit", "", NULL, "()" );
	PyErr_Print();

	MultiplayerGame *game = new MultiplayerGame( "Multiplayer.MultiplayerGame", MAX_PLAYERS );
	if ( g_kUtopiaModule.IsHost() ) {
		int playerLimit = MAX_PLAYERS;
		PyEmbed::GetValue( "Multiplayer.MissionMenusShared", "g_iPlayerLimit", "i", &playerLimit );
		PyErr_Clear();
		game->SetMaxPlayers( playerLimit );
	}

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *completed = database->GetString( "Connection Completed" );
	TGString status;
	status = *completed;
	SetStatusMessage( &status );
	g_kLocalizationManager.Unload( database );
}

/*
================
MultiplayerWindow::ConnectHandler

Joins the game under the player's name ("Player" without one); the host
then sends itself the end of the game's set-up and takes the first player
number.
================
*/
// FUNCTION: STBC 0x00505040
void MultiplayerWindow::ConnectHandler( TGEvent *event ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL && !connected ) {
		connected = true;
		if ( playerName.GetLength() == 0 ) {
			playerName.AssignC( "Player" );
		}
		network->CreateLocalPlayer( &playerName );

		if ( g_kUtopiaModule.IsHost() ) {
			TGMessage *message = new TGMessage;
			char data[64];
			data[0] = GAME_INITIALIZE_DONE_MESSAGE;
			message->SetData( data, 1 );
			message->SetGuaranteed( true );
			network->SendTGMessage( network->GetHostID(), message, 0 );
			g_kUtopiaModule.SetPlayerNumber( 0 );
			g_kUtopiaModule.SetUnusedClientID( FIRST_CLIENT_ID );
		}
	}
}

/*
================
MultiplayerWindow::DisconnectHandler

During a game the mission's menus show the end-game dialog; otherwise the
status line gives the reason and the network shuts down. The status block
is written out in both branches, as retail has it twice.
================
*/
// FUNCTION: STBC 0x00505110
void MultiplayerWindow::DisconnectHandler( TGEvent *event ) {
	if ( connected == true ) {
		if ( Game::GetCurrentGame() != NULL ) {
			connected = false;
			char *mission = g_kVarManager.GetStringVariable( "Multiplayer", "Mission" );
			char module[1024];
			// BUG: redundant: the format takes one string; the mission is passed twice
			sprintf( module, "%sMenus", mission, mission );
			int result;
			if ( PyEmbed::CallFunction( module, "DoEndGameDialog", "i", &result, "()" ) == -1 ) {
				PyErr_Print();
			}
			return;
		}
		{
			// the status line, as in the else branch
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
			TGString *reason = NULL;
			TGNetwork *network = g_kUtopiaModule.GetNetwork();
			if ( network != NULL ) {
				switch ( network->GetBootReason() ) {
					case TGNetwork::TIMED_OUT:
						reason = database->GetString( "Timed Out" );
						break;
					case TGNetwork::INCORRECT_PASSWORD:
						reason = database->GetString( "Incorrect Password" );
						break;
					case TGNetwork::SERVER_BOOTED_YOU:
						reason = database->GetString( "Server Booted You" );
						break;
					case TGNetwork::TOO_MANY_PLAYERS:
						reason = database->GetString( "Too Many Players" );
						break;
					case TGNetwork::YOU_ARE_BANNED:
						reason = database->GetString( "You Are Banned" );
						break;
					case BOOT_SCRIPT_CHECK_FAILED:
						break;
					default:
						reason = database->GetString( "Disconnected" );
						break;
				}
			} else {
				reason = database->GetString( "Disconnected" );
			}
			if ( reason != NULL ) {
				TGString status;
				status = *reason;
				MultiplayerWindow::SetStatusMessage( &status );
			}
			g_kLocalizationManager.Unload( database );
		}
		connected = false;
	} else {
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
		TGString *reason = NULL;
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		if ( network != NULL ) {
			switch ( network->GetBootReason() ) {
				case TGNetwork::TIMED_OUT:
					reason = database->GetString( "Timed Out" );
					break;
				case TGNetwork::INCORRECT_PASSWORD:
					reason = database->GetString( "Incorrect Password" );
					break;
				case TGNetwork::SERVER_BOOTED_YOU:
					reason = database->GetString( "Server Booted You" );
					break;
				case TGNetwork::TOO_MANY_PLAYERS:
					reason = database->GetString( "Too Many Players" );
					break;
				case TGNetwork::YOU_ARE_BANNED:
					reason = database->GetString( "You Are Banned" );
					break;
				case BOOT_SCRIPT_CHECK_FAILED:
					break;
				default:
					reason = database->GetString( "Disconnected" );
					break;
			}
		} else {
			reason = database->GetString( "Disconnected" );
		}
		if ( reason != NULL ) {
			TGString status;
			status = *reason;
			MultiplayerWindow::SetStatusMessage( &status );
		}
		g_kLocalizationManager.Unload( database );
	}
	g_kUtopiaModule.TerminateNetwork();
}

/*
================
MultiplayerWindow::HideAllChildren
================
*/
// FUNCTION: STBC 0x00505480
void MultiplayerWindow::HideAllChildren( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child != NULL ) {
			child->SetNotVisible( true );
		}
	}
}

/*
================
MultiplayerWindow::SetStatusMessage

Puts the text in the status window's paragraph.
================
*/
// FUNCTION: STBC 0x005054B0
void MultiplayerWindow::SetStatusMessage( TGString *message ) {
	MultiplayerWindow *window = (MultiplayerWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window != NULL ) {
		TGPane *pane = (TGPane *)window->statusWindow->GetNthChild( 0 );
		TGParagraph *paragraph = (TGParagraph *)pane->GetNthChild( 0 );
		paragraph->SetString( message, true );
		paragraph->Layout();
	}
}


/*
================
MultiplayerWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00505500
bool MultiplayerWindow::WriteToStream( TGStream *stream ) {
	if ( !MainWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( multiplayerMenu != NULL ? multiplayerMenu->GetObjID() : 0 );
	stream->WriteID( hostMenu != NULL ? hostMenu->GetObjID() : 0 );
	stream->WriteID( dedicatedServerMenu != NULL ? dedicatedServerMenu->GetObjID() : 0 );
	stream->WriteID( joinMenu != NULL ? joinMenu->GetObjID() : 0 );
	stream->WriteID( directJoinMenu != NULL ? directJoinMenu->GetObjID() : 0 );
	stream->WriteID( missionPane != NULL ? missionPane->GetObjID() : 0 );
	stream->WriteID( statusWindow != NULL ? statusWindow->GetObjID() : 0 );
	stream->WriteID( endWindow != NULL ? endWindow->GetObjID() : 0 );
	stream->WriteID( chatWindow != NULL ? chatWindow->GetObjID() : 0 );
	stream->WriteID( playerListWindow != NULL ? playerListWindow->GetObjID() : 0 );
	stream->WriteID( chosenMissionButtonID );
	stream->WriteChar( connected );
	stream->WriteChar( gameOver );
	stream->WriteChar( directJoin );
	return true;
}

/*
================
MultiplayerWindow::ReadFromStream

The panes are IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x00505660
bool MultiplayerWindow::ReadFromStream( TGStream *stream ) {
	if ( !MainWindow::ReadFromStream( stream ) ) {
		return false;
	}
	multiplayerMenu = (TGPane *)stream->ReadID();
	hostMenu = (TGPane *)stream->ReadID();
	dedicatedServerMenu = (TGPane *)stream->ReadID();
	joinMenu = (TGPane *)stream->ReadID();
	directJoinMenu = (TGPane *)stream->ReadID();
	missionPane = (TGPane *)stream->ReadID();
	statusWindow = (TGPane *)stream->ReadID();
	endWindow = (TGPane *)stream->ReadID();
	chatWindow = (TGPane *)stream->ReadID();
	playerListWindow = (TGPane *)stream->ReadID();
	chosenMissionButtonID = stream->ReadID();
	connected = stream->ReadChar() != 0;
	gameOver = stream->ReadChar() != 0;
	directJoin = stream->ReadChar() != 0;
	return true;
}

/*
================
MultiplayerWindow::FixUpPointers

Whether every pane the stream named has been loaded.
================
*/
// FUNCTION: STBC 0x00505770
bool MultiplayerWindow::FixUpPointers( void ) {
	if ( !TGUIObject::FixUpPointers() ) {
		return false;
	}
	if ( multiplayerMenu != NULL && GetTGObjectPtr( (long)multiplayerMenu ) == NULL ) {
		return false;
	}
	if ( hostMenu != NULL && GetTGObjectPtr( (long)hostMenu ) == NULL ) {
		return false;
	}
	if ( dedicatedServerMenu != NULL && GetTGObjectPtr( (long)dedicatedServerMenu ) == NULL ) {
		return false;
	}
	if ( joinMenu != NULL && GetTGObjectPtr( (long)joinMenu ) == NULL ) {
		return false;
	}
	if ( directJoinMenu != NULL && GetTGObjectPtr( (long)directJoinMenu ) == NULL ) {
		return false;
	}
	if ( missionPane != NULL && GetTGObjectPtr( (long)missionPane ) == NULL ) {
		return false;
	}
	if ( statusWindow != NULL && GetTGObjectPtr( (long)statusWindow ) == NULL ) {
		return false;
	}
	if ( endWindow != NULL && GetTGObjectPtr( (long)endWindow ) == NULL ) {
		return false;
	}
	if ( chatWindow != NULL && GetTGObjectPtr( (long)chatWindow ) == NULL ) {
		return false;
	}
	if ( playerListWindow != NULL && GetTGObjectPtr( (long)playerListWindow ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
MultiplayerWindow::PostFixUp

Turns the panes' IDs back into pointers.
================
*/
// FUNCTION: STBC 0x00505880
void MultiplayerWindow::PostFixUp( void ) {
	TGUIObject::PostFixUp();
	multiplayerMenu = ( multiplayerMenu != NULL ) ? (TGPane *)GetTGObjectPtr( (long)multiplayerMenu ) : NULL;
	hostMenu = ( hostMenu != NULL ) ? (TGPane *)GetTGObjectPtr( (long)hostMenu ) : NULL;
	dedicatedServerMenu = ( dedicatedServerMenu != NULL ) ? (TGPane *)GetTGObjectPtr( (long)dedicatedServerMenu ) : NULL;
	joinMenu = ( joinMenu != NULL ) ? (TGPane *)GetTGObjectPtr( (long)joinMenu ) : NULL;
	directJoinMenu = ( directJoinMenu != NULL ) ? (TGPane *)GetTGObjectPtr( (long)directJoinMenu ) : NULL;
	missionPane = ( missionPane != NULL ) ? (TGPane *)GetTGObjectPtr( (long)missionPane ) : NULL;
	statusWindow = ( statusWindow != NULL ) ? (TGPane *)GetTGObjectPtr( (long)statusWindow ) : NULL;
	endWindow = ( endWindow != NULL ) ? (TGPane *)GetTGObjectPtr( (long)endWindow ) : NULL;
	chatWindow = ( chatWindow != NULL ) ? (TGPane *)GetTGObjectPtr( (long)chatWindow ) : NULL;
	playerListWindow = ( playerListWindow != NULL ) ? (TGPane *)GetTGObjectPtr( (long)playerListWindow ) : NULL;
}

/*
================
MultiplayerWindow::BuildMissionMenu

Adds a button to the window for each mission found under
Scripts/Multiplayer/Episode, in order of their titles. A button sends the
window ET_SET_MISSION_NAME with its mission's folder name; the one for the
mission the scripts have chosen starts chosen. The title is the mission
script's GetMissionName, or the folder name without one.
================
*/
// FUNCTION: STBC 0x00505980
// PARKED: register allocation and EH state order: retail keeps the folder name in EDI and the event in ESI (this build swaps them, and the button takes EBP, which retail leaves to `this`), and stores the title string's state before the compare in the sort loop; the statements, calls and strings otherwise match
STMenu *MultiplayerWindow::BuildMissionMenu( STStylizedWindow *window ) {
	chosenMissionButtonID = 0;
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	database->GetString( "Select Mission" );

	TGDirectory *directory = new TGDirectory;
	if ( directory->Read( "Scripts/Multiplayer/Episode", NULL, true ) ) {
		TGDirectory *mission = directory->GetSubdirectories();
		while ( mission != NULL ) {
			char *folder = mission->GetName();
			TGStringEvent *event = new TGStringEvent;
			event->SetEventType( ET_SET_MISSION_NAME );
			event->SetDestination( this );
			event->SetString( folder );

			TGString folderTitle;
			char module[1024];
			sprintf( module, "Multiplayer.Episode.%s.%s", folder, folder );
			bool chosen = false;
			if ( strcmp( module, g_kVarManager.GetStringVariable( "Multiplayer", "Mission" ) ) == 0 ) {
				chosen = true;
			}

			sprintf( module, "Multiplayer.Episode.%s.%sName", folder, folder );
			PyObject *object;
			TGString *title;
			if ( PyEmbed::CallFunction( module, "GetMissionName", "O", &object, "()" ) == -1 ) {
				PyErr_Print();
				folderTitle.AssignC( folder );
				title = &folderTitle;
			} else {
				title = (TGString *)PyEmbed::GetObjectPointer( object );
				Py_DECREF( object );
			}

			STButton *button = new STButton( title, event, 0x80000000, 0.0f, 0.0f, NULL, 200 );
			event->SetSource( button );
			if ( chosen ) {
				button->SetChosen( true, true );
				chosenMissionButtonID = button->GetObjID();
			}

			int index = 0;
			while ( index < window->GetNumChildren() ) {
				STButton *child = (STButton *)window->GetNthChild( index );
				TGString childTitle;
				child->GetName( &childTitle );
				char *ours = title->Unknown6F56F0();
				char *theirs = childTitle.Unknown6F56F0();
				int order = strcmp( ours, theirs );
				delete ours;
				delete theirs;
				if ( order < 0 ) {
					break;
				}
				index++;
			}
			window->InsertChild( index, button, 0.0f, 0.0f, false );
			mission = mission->GetNext();
		}
	}
	delete directory;
	g_kLocalizationManager.Unload( database );
	return NULL;
}
/*
================
MultiplayerWindow::SetMissionNameHandler

A mission button was chosen: the one chosen before lets go.
================
*/
// FUNCTION: STBC 0x00505d70
void MultiplayerWindow::SetMissionNameHandler( TGEvent *event ) {
	STButton *previous = (STButton *)GetTGObjectPtr( chosenMissionButtonID );
	if ( previous != NULL ) {
		previous->SetChosen( false );
	}
	STButton *button = (STButton *)event->GetSource();
	if ( button != NULL ) {
		button->SetChosen( true );
		chosenMissionButtonID = button->GetObjID();
	} else {
		chosenMissionButtonID = 0;
	}
	CallNextHandler( event );
}

/*
================
MultiplayerWindow::RefreshServerListHandler

The client's refresh button (a TGBoolEvent): true stops the server list
query; false empties the list's columns and asks GameSpy for the servers
of the Internet or, when the options say so, of the LAN.
================
*/
// FUNCTION: STBC 0x00505e00
void MultiplayerWindow::RefreshServerListHandler( TGEvent *event ) {
	GameSpy *gameSpy = g_kUtopiaModule.GetGameSpy();
	if ( !( (TGBoolEvent *)event )->GetBool() ) {
		if ( gameSpy == NULL ) {
			g_kUtopiaModule.CreateGameSpy();
			gameSpy = g_kUtopiaModule.GetGameSpy();
		}
		if ( gameSpy != NULL ) {
			bool lan;
			if ( g_kConfigMapping.HasValue( "Multiplayer Options", "Internet Host" ) ) {
				lan = g_kConfigMapping.GetIntValue( "Multiplayer Options", "Internet Host" ) == 0;
			} else {
				lan = false;
			}

			MainWindow *window = TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
			if ( window != NULL ) {
				TGIntEvent *percentage = new TGIntEvent;
				percentage->SetEventType( ET_SERVER_LIST_PERCENTAGE );
				percentage->SetDestination( window );
				percentage->SetInt( 0 );
				g_kEventManager.AddEvent( percentage );

				PyObject *object;
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesNameMenu", "O", &object ) != -1 ) {
					( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesTypeMenu", "O", &object ) != -1 ) {
					( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesPingMenu", "O", &object ) != -1 ) {
					( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesPlayersMenu", "O", &object ) != -1 ) {
					( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientInternetButton", "O", &object ) != -1 ) {
					( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetDisabled( false );
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientLanButton", "O", &object ) != -1 ) {
					( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetDisabled( false );
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientStartQueryButton", "O", &object ) != -1 ) {
					( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetDisabled( false );
				}
				if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientStopQueryButton", "O", &object ) != -1 ) {
					( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetEnabled( false );
				}
				ClearCurrentServer();
				gameSpy->RefreshServerList( lan );
			}
		}
	} else {
		if ( gameSpy != NULL ) {
			gameSpy->StopServerList();
		}
		TGEvent *done = new TGEvent;
		done->SetEventType( ET_REFRESH_SERVER_LIST_DONE );
		done->SetDestination( this );
		g_kEventManager.AddEvent( done );
	}
	CallNextHandler( event );
}

/*
================
MultiplayerWindow::BootPlayerHandler

The host boots the player the event names, never itself.
================
*/
// FUNCTION: STBC 0x00506170
void MultiplayerWindow::BootPlayerHandler( TGPlayerEvent *event ) {
	if ( g_kUtopiaModule.IsHost() ) {
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		int playerID = event->GetPlayerID();
		if ( network != NULL && playerID != network->GetHostID() ) {
			char data = (char)playerID;
			TGBootPlayerMessage *message = new TGBootPlayerMessage;
			message->SetBootReason( TGNetwork::SERVER_BOOTED_YOU );
			message->SetData( &data, 1 );
			network->SendTGMessage( 0, message, 0 );
		}
	}
	CallNextHandler( event );
}

/*
================
MultiplayerWindow::SelectServerHandler

A server chosen in the list (a ServerListEvent): enables the start button
and fills the game info pane with the server's limits, system and mission
description, and has GameSpy send its players.
================
*/
// FUNCTION: STBC 0x00506200
// PARKED: frame slots: the demo's copy of this code (stbcdemo 0x505E80, same 1770 bytes) differs from retail only in two locals' frame slots and the matching register picks, so retail's slot order flips between builds; here the info lines' TGString temporaries take other slots and EH states than retail's, with statements, calls and strings agreeing
void MultiplayerWindow::SelectServerHandler( TGEvent *event ) {
	GServer server = ( (ServerListEvent *)event )->GetServer();
	if ( currentServer != server ) {
		currentServer = server;
		GameSpy *gameSpy = g_kUtopiaModule.GetGameSpy();

		PyObject *object;
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientStartButton", "O", &object ) != -1 ) {
			( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetEnabled( true );
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientPlayersMenu", "O", &object ) != -1 ) {
			( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientPlayersWindow", "O", &object ) != -1 ) {
			STStylizedWindow *window = (STStylizedWindow *)PyEmbed::GetObjectPointer( object );
			window->Layout();
			window->InteriorChangedSize( false );
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGameInfoPara", "O", &object ) != -1 ) {
			TGParagraph *paragraph = (TGParagraph *)PyEmbed::GetObjectPointer( object );
			TGString info( "" );
			int timeLimit;
			int fragLimit;
			char system[128];
			char mission[128];
			TGLocalizationDatabase *database;
			if ( gameSpy != NULL &&
				gameSpy->GetServerInfo( (GServer)currentServer, &timeLimit, &fragLimit, system, sizeof( system ) - 1, mission, sizeof( mission ) - 1 ) &&
				( database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true ) ) != NULL ) {
				char line[256];
				if ( timeLimit == -1 ) {
					info.Unknown6F4970( &( *database->GetString( "GameInfo Time Limit:" ) + *database->GetString( "None" ) + TGString( "\n" ) ) );
				} else {
					sprintf( line, "%d\n", timeLimit );
					info.Unknown6F4970( &( *database->GetString( "GameInfo Time Limit:" ) + TGString( line ) ) );
				}
				if ( fragLimit == -1 ) {
					info.Unknown6F4970( &( *database->GetString( "GameInfo Frag Limit:" ) + *database->GetString( "None" ) + TGString( "\n" ) ) );
				} else if ( fragLimit == -2 ) {
					info.Unknown6F4970( &( *database->GetString( "GameInfo Score Limit:" ) + *database->GetString( "None" ) + TGString( "\n" ) ) );
				} else if ( fragLimit < 10000 ) {
					sprintf( line, "%d\n", fragLimit );
					info.Unknown6F4970( &( *database->GetString( "GameInfo Frag Limit:" ) + TGString( line ) ) );
				} else {
					sprintf( line, "%d\n", fragLimit );
					info.Unknown6F4970( &( *database->GetString( "GameInfo Score Limit:" ) + TGString( line ) ) );
				}

				TGLocalizationDatabase *systems = g_kLocalizationManager.Load( "data/TGL/systems.tgl", true );
				TGString *systemName = systems->GetString( system );
				g_kLocalizationManager.Unload( systems );	// BUG: systemName is read below, after its database is unloaded
				if ( systemName != NULL ) {
					char *name = systemName->Unknown6F56F0();
					sprintf( line, "%s\n", name );
					delete name;
				} else {
					sprintf( line, "%s\n", system );
				}
				info.Unknown6F4970( &( *database->GetString( "GameInfo System:" ) + TGString( line ) ) );
				g_kLocalizationManager.Unload( database );

				char module[1024];
				sprintf( module, "%sName", mission );
				if ( PyEmbed::CallFunction( module, "GetMissionDescription", "O", &object, "" ) != -1 ) {
					info.Unknown6F4970( (TGString *)PyEmbed::GetObjectPointer( object ) );
				}
			}
			paragraph->SetString( &info );
			if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGameInfoWindow", "O", &object ) != -1 ) {
				STStylizedWindow *window = (STStylizedWindow *)PyEmbed::GetObjectPointer( object );
				window->InteriorChangedSize( false );
				window->ScrollToTop();
				window->Layout();
			}
		}
		if ( gameSpy != NULL && currentServer != NULL ) {
			gameSpy->SendServerPlayers( (GServer)currentServer );
		}
	}
	CallNextHandler( event );
}

/*
================
MultiplayerWindow::ClearCurrentServer

Forgets the server chosen in the list and clears the script-built panes
that show it.
================
*/
// FUNCTION: STBC 0x00506910
void MultiplayerWindow::ClearCurrentServer( void ) {
	PyObject *object;

	currentServer = NULL;
	if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientStartButton", "O", &object ) != -1 ) {
		( (TGUIObject *)PyEmbed::GetObjectPointer( object ) )->SetDisabled( true );
	}
	if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientPlayersMenu", "O", &object ) != -1 ) {
		( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
	}
	if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientPlayersWindow", "O", &object ) != -1 ) {
		STStylizedWindow *window = (STStylizedWindow *)PyEmbed::GetObjectPointer( object );
		window->Layout();
		window->InteriorChangedSize( false );
	}
	if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGameInfoPara", "O", &object ) != -1 ) {
		( (TGParagraph *)PyEmbed::GetObjectPointer( object ) )->SetString( "", true );
	}
	if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGameInfoWindow", "O", &object ) != -1 ) {
		STStylizedWindow *window = (STStylizedWindow *)PyEmbed::GetObjectPointer( object );
		window->Layout();
		window->InteriorChangedSize( false );
	}
}

/*
================
MultiplayerWindow::SortServerListHandler

A column's sort button (a SortServerListEvent): empties the columns and
has GameSpy sort the list by the event's key and send the servers again.
================
*/
// FUNCTION: STBC 0x00506a50
void MultiplayerWindow::SortServerListHandler( TGEvent *event ) {
	GameSpy *gameSpy = g_kUtopiaModule.GetGameSpy();
	if ( gameSpy == NULL ) {
		return;
	}
	if ( TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER ) != NULL ) {
		PyObject *object;
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesNameMenu", "O", &object ) != -1 ) {
			( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesTypeMenu", "O", &object ) != -1 ) {
			( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesPingMenu", "O", &object ) != -1 ) {
			( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
		}
		if ( PyEmbed::GetValue( MULTIPLAYER_MENUS, "g_pClientGamesPlayersMenu", "O", &object ) != -1 ) {
			( (TGPane *)PyEmbed::GetObjectPointer( object ) )->KillChildren();
		}
		SortServerListEvent *sort = (SortServerListEvent *)event;
		gameSpy->SortServerList( sort->GetSortBy(), sort->IsStringField() );
		ClearCurrentServer();
	}
	CallNextHandler( event );
}

/*
================
MultiplayerWindow::IsAnyChildVisible
================
*/
// FUNCTION: STBC 0x00506B80
bool MultiplayerWindow::IsAnyChildVisible( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child != NULL && child->IsVisible() ) {
			return true;
		}
	}
	return false;
}

/*
================
MultiplayerWindow::ToggleChatWindow

Shows the chat window over the tactical view and gives it the focus; when it
is up already, hides it again (the flag keeps the multiplayer window itself
on top).
================
*/
// FUNCTION: STBC 0x00506BD0
void MultiplayerWindow::ToggleChatWindow( bool keepWindow ) {
	TopWindow *top = TopWindow::GetTopWindow();
	MultiplayerWindow *window = (MultiplayerWindow *)top->FindMainWindow( MWT_MULTIPLAYER );
	if ( window == NULL || window->chatWindow == NULL ) {
		return;
	}

	TGPane *chat = window->chatWindow;
	if ( chat->IsVisible() ) {
		if ( !keepWindow ) {
			top->MoveToBack( window, false );
			window->SetVisible( true );
		}
		chat->SetNotVisible( true );
		return;
	}

	MainWindow *tactical = top->FindMainWindow( MWT_TACTICAL );
	if ( top->GetFocus() != tactical ) {
		return;
	}

	chat->SetVisible( false );
	if ( !keepWindow ) {
		Unknown506D40( chat );
	}
	top->MoveToFront( window, false );
	window->SetVisible( false );
	window->Layout();
	tactical->SetVisible( true );

	TGPane *chatPane = (TGPane *)chat->GetNthChild( 0 );
	TGUIObject *chatInput = chatPane->GetNthChild( 0 );
	top->SetFocus( window, true );
	window->SetFocus( chat, true );
	chat->SetFocus( chatPane, true );
	chatPane->SetFocus( chatInput, true );
}

/*
================
MultiplayerWindow::IsChatWindowActive
================
*/
// FUNCTION: STBC 0x00506D10
bool MultiplayerWindow::IsChatWindowActive( void ) {
	MultiplayerWindow *window = (MultiplayerWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MULTIPLAYER );
	if ( window != NULL ) {
		return ( window->chatWindow->GetFlags() >> 3 ) & 1;
	}
	return false;
}

/*
================
MultiplayerWindow::Unknown506D40

Lays the chat window's pane out over the tactical view's empty area, which
the Tactical script gives: the pane takes the width right of the area's left
edge and sits at its bottom, and its paragraph takes the width the pane's
second child leaves, with no text.
================
*/
// FUNCTION: STBC 0x00506d40
void MultiplayerWindow::Unknown506D40( TGPane *chat ) {
	if ( chat == NULL ) {
		return;
	}

	float left = GetTacticalFloat( "GetEmptyAreaLeft", DEFAULT_EMPTY_AREA_LEFT );
	float bottom = GetTacticalFloat( "GetEmptyAreaBottom", DEFAULT_EMPTY_AREA_BOTTOM );

	TGPane *pane = (TGPane *)chat->GetNthChild( 0 );
	float height = pane->GetHeight();
	float sideWidth = ( (TGPane *)pane->GetNthChild( 1 ) )->GetWidth();
	float x = left + CHAT_MARGIN;
	pane->SetPosition( x, bottom - pane->GetHeight() - CHAT_MARGIN, false );
	float width = 1.0f - x;
	pane->Resize( width, height, false );

	TGParagraph *paragraph = (TGParagraph *)pane->GetNthChild( 0 );
	paragraph->Resize( width - sideWidth - PARAGRAPH_MARGIN, height, false );
	paragraph->SetPosition( sideWidth + PARAGRAPH_MARGIN, paragraph->GetTop(), false );
	paragraph->SetString( "", false );
	pane->Layout();
}

/*
================
MultiplayerWindow::SetVisible

While the options window is open the window only waits to be shown when
it closes.
================
*/
// FUNCTION: STBC 0x00506eb0
void MultiplayerWindow::SetVisible( bool layout ) {
	if ( TopWindow::GetTopWindow() != NULL ) {
		OptionsWindow *options = (OptionsWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
		if ( options != NULL && options->IsVisible() ) {
			options->AddToPreviouslyVisible( this );
			return;
		}
		MainWindow::SetVisible( layout );
	}
}

/*
================
MultiplayerWindow::SetNotVisible

While the options window is open the window is only taken off the list
of windows it shows again when it closes.
================
*/
// FUNCTION: STBC 0x00506f10
void MultiplayerWindow::SetNotVisible( bool layout ) {
	if ( TopWindow::GetTopWindow() != NULL ) {
		OptionsWindow *options = (OptionsWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
		if ( options != NULL && options->IsVisible() ) {
			options->RemoveFromPreviouslyVisible( this );
			return;
		}
		MainWindow::SetNotVisible( layout );
	}
}

