/*
===============================================================================

	NetFile.cpp

	The multiplayer script check.

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "NetFile.h"
#include "PyEmbed.h"
#include "MainWindow.h"
#include "ModalDialogWindow.h"
#include "MultiplayerWindow.h"
#include "TGBufferStream.h"
#include "TGDirectory.h"
#include "TGEventManager.h"
#include "TGHandler.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGMessage.h"
#include "TGMessageEvent.h"
#include "TGNetwork.h"
#include "TGPlayerEvent.h"
#include "TGString.h"
#include "TGSystemWrapperClass.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006a7650 _$E484
// FUNCTION: STBC 0x006a7670 _$E483

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006a5210 _$E32
// FUNCTION: STBC 0x006a5220 _$E29
// FUNCTION: STBC 0x006a5230 _$E31
// FUNCTION: STBC 0x006a5240 _$E30
// FUNCTION: STBC 0x006a5250 _$E38
// FUNCTION: STBC 0x006a5260 _$E35
// FUNCTION: STBC 0x006a5270 _$E37
// FUNCTION: STBC 0x006a5280 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006a6170 _$E78
// FUNCTION: STBC 0x006a6180 _$E77
// FUNCTION: STBC 0x006a62d0 _$E82
// FUNCTION: STBC 0x006a62e0 _$E81
// FUNCTION: STBC 0x006a6860 _$E93
// FUNCTION: STBC 0x006a6870 _$E92
// FUNCTION: STBC 0x006a6880 _$E97
// FUNCTION: STBC 0x006a6890 _$E96
// FUNCTION: STBC 0x006a68a0 _$E101
// FUNCTION: STBC 0x006a68b0 _$E100
// FUNCTION: STBC 0x006a68c0 _$E105
// FUNCTION: STBC 0x006a68d0 _$E104
// FUNCTION: STBC 0x006a68e0 _$E229
// FUNCTION: STBC 0x006a68f0 _$E228
// FUNCTION: STBC 0x006a6900 _$E171
// FUNCTION: STBC 0x006a6910 _$E170
// FUNCTION: STBC 0x006a6920 _$E184
// FUNCTION: STBC 0x006a6930 _$E183
// FUNCTION: STBC 0x006a6940 _$E234
// FUNCTION: STBC 0x006a6950 _$E233
// FUNCTION: STBC 0x006a6960 _$E239
// FUNCTION: STBC 0x006a6970 _$E238
// FUNCTION: STBC 0x006a6980 _$E243
// FUNCTION: STBC 0x006a6990 _$E242
// FUNCTION: STBC 0x006a69a0 _$E247
// FUNCTION: STBC 0x006a69b0 _$E246
// FUNCTION: STBC 0x006a69c0 _$E254
// FUNCTION: STBC 0x006a69d0 _$E253
// FUNCTION: STBC 0x006a69e0 _$E303
// FUNCTION: STBC 0x006a69f0 _$E302
// FUNCTION: STBC 0x006a6a00 _$E307
// FUNCTION: STBC 0x006a6a10 _$E306
// FUNCTION: STBC 0x006a6a20 _$E129
// FUNCTION: STBC 0x006a6a30 _$E128
// FUNCTION: STBC 0x006a6a40 _$E289
// FUNCTION: STBC 0x006a6a50 _$E288
// FUNCTION: STBC 0x006a6a60 _$E296
// FUNCTION: STBC 0x006a6a70 _$E295
// FUNCTION: STBC 0x006a6a80 _$E319
// FUNCTION: STBC 0x006a6a90 _$E318
// FUNCTION: STBC 0x006a6aa0 _$E323
// FUNCTION: STBC 0x006a6ab0 _$E322
// FUNCTION: STBC 0x006a6ac0 _$E363
// FUNCTION: STBC 0x006a6ad0 _$E362
// FUNCTION: STBC 0x006a6ae0 _$E369
// FUNCTION: STBC 0x006a6af0 _$E368
// FUNCTION: STBC 0x006a6b00 _$E311
// FUNCTION: STBC 0x006a6b10 _$E310
// FUNCTION: STBC 0x006a6b20 _$E315
// FUNCTION: STBC 0x006a6b30 _$E314
// FUNCTION: STBC 0x006a6b60 _$E327
// FUNCTION: STBC 0x006a6b70 _$E326
// FUNCTION: STBC 0x006a6b80 _$E331
// FUNCTION: STBC 0x006a6b90 _$E330
// FUNCTION: STBC 0x006a6ba0 _$E335
// FUNCTION: STBC 0x006a6bb0 _$E334
// FUNCTION: STBC 0x006a6bc0 _$E339
// FUNCTION: STBC 0x006a6bd0 _$E338
// FUNCTION: STBC 0x006a6be0 _$E343
// FUNCTION: STBC 0x006a6bf0 _$E342
// FUNCTION: STBC 0x006a6c00 _$E347
// FUNCTION: STBC 0x006a6c10 _$E346
// FUNCTION: STBC 0x006a6c20 _$E351
// FUNCTION: STBC 0x006a6c30 _$E350
// FUNCTION: STBC 0x006a6c40 _$E355
// FUNCTION: STBC 0x006a6c50 _$E354
// FUNCTION: STBC 0x006a6c60 _$E134
// FUNCTION: STBC 0x006a6c70 _$E133
// FUNCTION: STBC 0x006a6c80 _$E359
// FUNCTION: STBC 0x006a6c90 _$E358
// FUNCTION: STBC 0x006a6ca0 _$E139
// FUNCTION: STBC 0x006a6cb0 _$E138
// FUNCTION: STBC 0x006a6cc0 _$E373
// FUNCTION: STBC 0x006a6cd0 _$E372
// FUNCTION: STBC 0x006a6ce0 _$E377
// FUNCTION: STBC 0x006a6cf0 _$E376
// FUNCTION: STBC 0x006a6d00 _$E381
// FUNCTION: STBC 0x006a6d10 _$E380
// FUNCTION: STBC 0x006a6d20 _$E385
// FUNCTION: STBC 0x006a6d30 _$E384
// FUNCTION: STBC 0x006a6d40 _$E148
// FUNCTION: STBC 0x006a6d50 _$E147
// FUNCTION: STBC 0x006a6d60 _$E389
// FUNCTION: STBC 0x006a6d70 _$E388
// FUNCTION: STBC 0x006a6d80 _$E393
// FUNCTION: STBC 0x006a6d90 _$E392
// FUNCTION: STBC 0x006a6da0 _$E397
// FUNCTION: STBC 0x006a6db0 _$E396
// FUNCTION: STBC 0x006a6dc0 _$E154
// FUNCTION: STBC 0x006a6dd0 _$E153
// FUNCTION: STBC 0x006a6eb0 _$E159
// FUNCTION: STBC 0x006a6ec0 _$E158
// FUNCTION: STBC 0x006a6fa0 _$E189
// FUNCTION: STBC 0x006a6fb0 _$E188
// FUNCTION: STBC 0x006a71f0 _$E401
// FUNCTION: STBC 0x006a7200 _$E400
// FUNCTION: STBC 0x006a7210 _$E194
// FUNCTION: STBC 0x006a7220 _$E193
// FUNCTION: STBC 0x006a7230 _$E405
// FUNCTION: STBC 0x006a7240 _$E404
// FUNCTION: STBC 0x006a73d0 _$E409
// FUNCTION: STBC 0x006a73e0 _$E408
// FUNCTION: STBC 0x006a73f0 _$E199
// FUNCTION: STBC 0x006a7400 _$E198
// FUNCTION: STBC 0x006a7410 _$E413
// FUNCTION: STBC 0x006a7420 _$E412
// FUNCTION: STBC 0x006a7430 _$E164
// FUNCTION: STBC 0x006a7440 _$E163
// FUNCTION: STBC 0x006a7450 _$E417
// FUNCTION: STBC 0x006a7460 _$E416
// FUNCTION: STBC 0x006a7470 _$E421
// FUNCTION: STBC 0x006a7480 _$E420
// FUNCTION: STBC 0x006a7490 _$E204
// FUNCTION: STBC 0x006a74a0 _$E203
// FUNCTION: STBC 0x006a74b0 _$E209
// FUNCTION: STBC 0x006a74c0 _$E208
// FUNCTION: STBC 0x006a74d0 _$E425
// FUNCTION: STBC 0x006a74e0 _$E424
// FUNCTION: STBC 0x006a74f0 _$E429
// FUNCTION: STBC 0x006a7500 _$E428
// FUNCTION: STBC 0x006a7510 _$E433
// FUNCTION: STBC 0x006a7520 _$E432
// FUNCTION: STBC 0x006a7530 _$E437
// FUNCTION: STBC 0x006a7540 _$E436
// FUNCTION: STBC 0x006a7550 _$E441
// FUNCTION: STBC 0x006a7560 _$E440
// FUNCTION: STBC 0x006a7570 _$E445
// FUNCTION: STBC 0x006a7580 _$E444
// FUNCTION: STBC 0x006a7590 _$E449
// FUNCTION: STBC 0x006a75a0 _$E448
// FUNCTION: STBC 0x006a75b0 _$E453
// FUNCTION: STBC 0x006a75c0 _$E452
// FUNCTION: STBC 0x006a75d0 _$E214
// FUNCTION: STBC 0x006a75e0 _$E213
// FUNCTION: STBC 0x006a7630 _$E457
// FUNCTION: STBC 0x006a7640 _$E456
// FUNCTION: STBC 0x006a7680 _$E219
// FUNCTION: STBC 0x006a7690 _$E218
// FUNCTION: STBC 0x006a76a0 _$E224
// FUNCTION: STBC 0x006a76b0 _$E223
// FUNCTION: STBC 0x006a76c0 _$E461
// FUNCTION: STBC 0x006a76d0 _$E460
// FUNCTION: STBC 0x006a76e0 _$E466
// FUNCTION: STBC 0x006a76f0 _$E465
// FUNCTION: STBC 0x006a7700 _$E470
// FUNCTION: STBC 0x006a7710 _$E469

// winuser.h, through the includes above, renames GetMessage.
#undef GetMessage

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006a3070 _$E476
// FUNCTION: STBC 0x006a3080 _$E472
// FUNCTION: STBC 0x006a3090 _$E475
// FUNCTION: STBC 0x006a30a0 _$E473
// DATA: STBC 0x009948f0
TGEventHandlerTable NetFile::classHandlers;

/*
================
NetFile::NetFile

Listens for every network message.
================
*/
// FUNCTION: STBC 0x006a30c0
NetFile::NetFile( long id ) : TGEventHandlerObject( id ) {
	unknown15 = false;
	unknown14 = false;
	g_kEventManager.AddBroadcastMethodHandler( ET_NETWORK_MESSAGE_EVENT, this, "NetFile :: ReceiveMessageHandler" );
}

/*
================
NetFile::~NetFile

Stops listening and deletes the messages and lists the maps hold.
================
*/
// FUNCTION: STBC 0x006a32b0
NetFile::~NetFile( void ) {
	g_kEventManager.RemoveAllBroadcastHandlersForObject( this );

	int playerID;
	TGMessage *message;
	NiTMapIterator pos = sentMessages.GetFirstPos();
	while ( pos ) {
		sentMessages.GetNext( pos, playerID, message );
		delete message;
	}
	sentMessages.RemoveAll();

	DeleteAllMessageLists();

	NetFileNameList *fileList;
	pos = fileLists.GetFirstPos();
	while ( pos ) {
		fileLists.GetNext( pos, playerID, fileList );
		delete fileList;
	}
	fileLists.RemoveAll();
}

/*
================
NetFile::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006a3560
void NetFile::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&NetFile::ReceiveMessageHandler, "NetFile :: ReceiveMessageHandler" );
}

/*
================
NetFile::RegisterHandlers

The class has no class handlers: the constructor adds a broadcast handler.
================
*/
// FUNCTION: STBC 0x006a3580
void NetFile::RegisterHandlers( void ) {
}

/*
================
NetFile::RequestSystemChecksum

A copy of the request is kept until the answer comes.
================
*/
// FUNCTION: STBC 0x006a35b0
// PARKED: inline budget wall: retail calls SortedArray<TGNetPlayer, int>::Find (0x401CC0) out of line inside the player lookup, this build expands it; the rest is identical
bool NetFile::RequestSystemChecksum( int playerID, char *directory, char *filter, bool recursive ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return false;
	}
	if ( !g_kUtopiaModule.IsHost() ) {
		return false;
	}

	TGMessage *sent;
	if ( sentMessages.GetAt( playerID, sent ) && network->GetPlayerList()->GetPlayerByNetID( playerID ) != NULL ) {
		return false;
	}

	TGMessage *message = new TGMessage;
	message->SetGuaranteed( true );

	TGBufferStream stream;
	char buffer[0x400];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( NETFILE_CHECKSUM_REQUEST_MESSAGE );
	stream.WriteChar( -1 );
	stream.WriteShort( strlen( directory ) );
	stream.Write( directory, strlen( directory ) );
	if ( filter != NULL ) {
		stream.WriteShort( strlen( filter ) );
		stream.Write( filter, strlen( filter ) );
	} else {
		stream.WriteShort( 0 );
	}
	stream.WriteBool( recursive );
	message->SetData( buffer, stream.GetPos() );

	TGMessage *copy = message->Copy();
	network->SendTGMessage( playerID, message, 0 );
	sentMessages.SetAt( playerID, copy );
	return true;
}

/*
================
NetFile::RequestChecksums

The directories checked, in order: the two scripts every game runs, the
ships (with their subdirectories) and the main menu.
================
*/
// FUNCTION: STBC 0x006a3820
bool NetFile::RequestChecksums( int playerID ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return false;
	}
	if ( !g_kUtopiaModule.IsHost() ) {
		return false;
	}

	NiTList<TGMessage *> *messageList;
	if ( messageLists.GetAt( playerID, messageList ) && !messageList->IsEmpty() && network->GetPlayerList()->GetPlayerByNetID( playerID ) != NULL ) {
		return false;
	}
	NetFileNameList *fileList;
	if ( fileLists.GetAt( playerID, fileList ) && !fileList->IsEmpty() ) {
		return false;
	}

	DeleteMessageList( playerID );

	struct ChecksumDirectory {
		char *	directory;
		char *	filter;
		bool	recursive;
	} directories[] = {
		{ "scripts/", "App.pyc", false },
		{ "scripts/", "Autoexec.pyc", false },
		{ "scripts/ships", "*.pyc", true },
		{ "scripts/mainmenu", "*.pyc", false }
	};
	for ( unsigned int i = 0; i < sizeof( directories ) / sizeof( directories[0] ); i++ ) {
		SendChecksumRequest( i, playerID, directories[i].directory, directories[i].filter, directories[i].recursive );
	}
	return true;
}

/*
================
NetFile::SendChecksumRequest
================
*/
// FUNCTION: STBC 0x006a39b0
void NetFile::SendChecksumRequest( int index, int playerID, char *directory, char *filter, bool recursive ) {
	TGMessage *message = new TGMessage;
	message->SetGuaranteed( true );

	TGBufferStream stream;
	char buffer[0x400];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( NETFILE_CHECKSUM_REQUEST_MESSAGE );
	stream.WriteChar( index );
	stream.WriteShort( strlen( directory ) );
	stream.Write( directory, strlen( directory ) );
	if ( filter != NULL ) {
		stream.WriteShort( strlen( filter ) );
		stream.Write( filter, strlen( filter ) );
	} else {
		stream.WriteShort( 0 );
	}
	stream.WriteBool( recursive );
	message->SetData( buffer, stream.GetPos() );

	TGMessage *queued = message;
	if ( index == 0 ) {
		queued = message->Copy();
		g_kUtopiaModule.GetNetwork()->SendTGMessage( playerID, message, 0 );
	}

	NiTList<TGMessage *> *list;
	if ( !messageLists.GetAt( playerID, list ) ) {
		list = new NiTList<TGMessage *>( 2 );
		messageLists.SetAt( playerID, list );
	}
	list->AddTail( queued );
}

/*
================
NetFile::ReceiveMessageHandler

Hands each script check message to its handler. The first file a player
is sent asks it first: the dialog's OK sends the message back through this
handler, Cancel gives up the connection.
================
*/
// FUNCTION: STBC 0x006a3cd0
void NetFile::ReceiveMessageHandler( TGMessageEvent *event ) {
	g_kUtopiaModule.SetProcessingPackets( true );

	TGMessage *message = event->GetMessage();
	if ( message->VirtualSlot00() == TGMESSAGE_TYPE ) {
		switch ( *message->GetData( NULL ) ) {
		case NETFILE_CHECKSUM_REQUEST_MESSAGE:
			ReceiveChecksumRequest( message );
			unknown14 = false;
			break;
		case NETFILE_CHECKSUM_MESSAGE:
			ReceiveChecksums( message );
			break;
		case NETFILE_FILE_MESSAGE:
			if ( !unknown14 ) {
				unknown14 = true;

				TGMessageEvent *okEvent = new TGMessageEvent;
				okEvent->Copy( event );

				TopWindow *topWindow = TopWindow::GetTopWindow();
				MainWindow *multiplayerWindow = topWindow->FindMainWindow( MWT_MULTIPLAYER );
				TGEvent *cancelEvent = new TGEvent;
				cancelEvent->SetEventType( ET_CANCEL_CONNECT );
				cancelEvent->SetDestination( multiplayerWindow );

				TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
				TGString *warning = database->GetString( "Receive File Warning" );
				ModalDialogWindow *dialog = (ModalDialogWindow *)topWindow->FindMainWindow( MWT_MODAL_DIALOG );
				dialog->Run( NULL, warning, database->GetString( "OK" ), okEvent, database->GetString( "Cancel" ), cancelEvent );
				g_kLocalizationManager.Unload( database );
			} else {
				ReceiveFile( message );
			}
			break;
		case NETFILE_FILE_RECEIVED_MESSAGE:
			ReceiveFileReceived( message );
			break;
		case NETFILE_SYSTEM_CHECKSUM_FAIL_MESSAGE:
		case NETFILE_VERSION_DIFFERENT_MESSAGE:
			ReceiveChecksumFailure( message );
			break;
		}
	}

	g_kUtopiaModule.SetProcessingPackets( false );
}

/*
================
NetFile::ReceiveFile

Writes a file the host sent, making a read-only copy writable, and asks
for the next. A compiled script is reloaded at once.
================
*/
// FUNCTION: STBC 0x006a3ea0
void NetFile::ReceiveFile( TGMessage *message ) {
	TGBufferStream stream;
	int length;
	char *data = message->GetData( &length );
	stream.VirtualSlot60( data, length );

	stream.ReadChar();
	int nameLength = stream.ReadShort();
	char name[0x400];
	stream.Read( name, nameLength );
	name[nameLength] = '\0';

	if ( CreateDirectories( name ) ) {
		FILE *file = g_kSystemWrapper.FileOpen( name, "wb" );
		if ( file == NULL ) {
			g_kSystemWrapper.SetFileReadOnly( name, false );
			file = g_kSystemWrapper.FileOpen( name, "wb" );
		}
		if ( file != NULL ) {
			int position = stream.GetPos();
			data += position;
			length -= position;
			g_kSystemWrapper.FileWrite( file, data, length );
			g_kSystemWrapper.FileClose( file );
		}
	}

	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL ) {
		TGMessage *reply = new TGMessage;
		reply->SetGuaranteed( true );
		char type = NETFILE_FILE_RECEIVED_MESSAGE;
		reply->SetData( &type, sizeof( type ) );
		network->SendTGMessage( network->GetHostID(), reply, 0 );
	}

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *receiving = database->GetString( "Receiving File" );
	TGString status;
	status = *receiving;
	status.Unknown6F4A40( name );
	MultiplayerWindow::SetStatusMessage( &status );

	int nameEnd = strlen( name );
	if ( nameEnd >= 4 && name[nameEnd - 4] == '.' && name[nameEnd - 3] == 'p' && name[nameEnd - 2] == 'y' && name[nameEnd - 1] == 'c' ) {
		char *module = name;
		if ( strnicmp( name, "Scripts/", strlen( "Scripts/" ) ) == 0 ) {
			module = name + strlen( "Scripts/" );
		}
		for ( char *c = module; *c != '\0'; c++ ) {
			if ( *c == '.' ) {
				*c = '\0';
				break;
			}
			if ( *c == '/' || *c == '\\' ) {
				*c = '.';
			}
		}
		PyEmbed::ReloadModule( module );
	}

	g_kLocalizationManager.Unload( database );
}

/*
================
NetFile::CreateDirectories
================
*/
// FUNCTION: STBC 0x006a4140
bool NetFile::CreateDirectories( char *path ) {
	char workingDirectory[0x400];
	g_kSystemWrapper.GetWorkingDirectory( workingDirectory, sizeof( workingDirectory ) );

	bool result = true;
	TGDirectory *directory = new TGDirectory;
	char name[0x400];
	int length = strlen( path );
	int j = 0;
	for ( int i = 0; i < length; i++, j++ ) {
		if ( path[i] == '/' ) {
			name[j] = '\0';
			if ( !directory->Exists( name ) && !g_kSystemWrapper.MakeDirectory( name ) ) {
				result = false;
				break;
			}
			g_kSystemWrapper.SetWorkingDirectory( name );
			j = -1;
		} else {
			name[j] = path[i];
		}
	}
	delete directory;

	g_kSystemWrapper.SetWorkingDirectory( workingDirectory );
	return result;
}

/*
================
NetFile::ReceiveFileReceived

The player has the last file: send it the next.
================
*/
// FUNCTION: STBC 0x006a4250
void NetFile::ReceiveFileReceived( TGMessage *message ) {
	SendNextFile( message->GetFromID() );
}

/*
================
NetFile::ReceiveChecksums

The answer to a system checksum request (index -1) is compared with the
directory the kept request names: a directory of another name sends the
player all of it; the request is then forgotten and the first file sent.
================
*/
// FUNCTION: STBC 0x006a4260
void NetFile::ReceiveChecksums( TGMessage *message ) {
	if ( g_kUtopiaModule.GetNetwork() == NULL ) {
		return;
	}
	char index = message->GetData( NULL )[1];
	if ( index != -1 ) {
		ReceiveScriptChecksums( message );
		return;
	}

	TGBufferStream stream;
	TGMessage *request;
	if ( sentMessages.GetAt( message->GetFromID(), request ) ) {
		int length;
		char *data = request->GetData( &length );
		stream.VirtualSlot60( data, length );
		stream.ReadChar();
		stream.ReadChar();

		char directoryName[0x400];
		int directoryLength = stream.ReadShort();
		stream.Read( directoryName, directoryLength );
		directoryName[directoryLength] = '\0';

		char filter[0x400];
		int filterLength = stream.ReadShort();
		if ( filterLength > 0 ) {
			stream.Read( filter, filterLength );
			filter[filterLength] = '\0';
		} else {
			filter[0] = '\0';
		}

		bool recursive = stream.ReadBool();
		stream.Close();

		TGDirectory *directory = new TGDirectory;
		if ( !directory->Read( directoryName, filter, recursive ) ) {
			delete directory;
			return;
		}

		data = message->GetData( &length );
		stream.VirtualSlot60( data, length );
		stream.ReadChar();
		stream.ReadChar();
		long nameHash = stream.ReadLong();
		if ( TGHashString( directory->GetName() ) != nameHash ) {
			AddDirectoryToSend( directory, message->GetFromID() );
		} else {
			CompareDirectoryChecksums( &stream, directory, message->GetFromID(), false );
		}
		stream.Close();
		delete directory;

		delete request;
		sentMessages.RemoveAt( message->GetFromID() );
		SendNextFile( message->GetFromID() );
	}
}

/*
================
NetFile::ReceiveScriptChecksums

The answer is matched to its queued request by the index. The first
answer carries the game version as well. A player missing a directory or
with another version is rejected; otherwise the request is dropped from
the queue and, unless SkipChecksum is set, a player whose scripts differ
is rejected with the first file named. With no request left the player
passed; otherwise the next request is sent.
================
*/
// FUNCTION: STBC 0x006a4560
void NetFile::ReceiveScriptChecksums( TGMessage *message ) {
	NiTList<TGMessage *> *list;
	if ( !messageLists.GetAt( message->GetFromID(), list ) || list->IsEmpty() ) {
		return;
	}

	char index = message->GetData( NULL )[1];
	TGMessage *request = NULL;
	NiTListIterator pos = list->GetHeadPos();
	while ( pos ) {
		TGMessage *queued = list->GetNext( pos );
		if ( queued->GetData( NULL )[1] == index ) {
			request = queued;
			break;
		}
	}
	if ( request == NULL ) {
		return;
	}

	char directoryName[0x400];
	char filter[0x400];
	bool recursive;
	if ( !ReadChecksumRequest( request, directoryName, filter, &recursive ) ) {
		return;
	}

	TGDirectory *directory = new TGDirectory;
	if ( !directory->Read( directoryName, filter, recursive ) ) {
		delete directory;
		return;
	}

	int length;
	char *data = message->GetData( &length );
	TGBufferStream stream;
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	stream.ReadChar();
	if ( index == 0 ) {
		long versionHash = stream.ReadLong();
		if ( versionHash != TGHashString( UtopiaModule::GetGameVersion() ) ) {
			// BUG: the directory listing is never deleted on this path.
			RejectPlayer( message->GetFromID(), UtopiaModule::GetGameVersion(), true );
			return;
		}
	}

	long nameHash = stream.ReadLong();
	if ( TGHashString( directory->GetName() ) != nameHash ) {
		// BUG: the directory listing is never deleted on this path.
		RejectPlayer( message->GetFromID(), directory->GetName(), false );
		return;
	}

	CompareScriptChecksums( &stream, directory, message->GetFromID(), false );
	stream.Close();
	delete directory;

	pos = list->GetHeadPos();
	while ( pos ) {
		NiTListIterator current = pos;
		TGMessage *queued = list->GetNext( pos );
		if ( queued->GetData( NULL )[1] == index ) {
			delete queued;
			list->RemovePos( current );
			break;
		}
	}

	NetFileNameList *fileList;
	if ( fileLists.GetAt( message->GetFromID(), fileList ) && !skipChecksum && !fileList->IsEmpty() ) {
		RejectPlayer( message->GetFromID(), fileList->GetHead(), false );
		while ( !fileList->IsEmpty() ) {
			delete fileList->RemoveTail();
		}
		delete fileList;
		fileLists.RemoveAt( message->GetFromID() );
		DeleteMessageList( message->GetFromID() );
	} else if ( list->IsEmpty() ) {
		PostSystemChecksumComplete( message->GetFromID() );
	} else {
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		network->SendTGMessage( message->GetFromID(), list->GetHead()->Copy(), 0 );
	}
}

/*
================
NetFile::RejectPlayer
================
*/
// FUNCTION: STBC 0x006a4a00
void NetFile::RejectPlayer( int playerID, char *fileName, bool versionDifferent ) {
	TGPlayerEvent *event = new TGPlayerEvent;
	event->SetEventType( ET_SYSTEM_CHECKSUM_FAILED );
	event->SetPlayerID( playerID );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );

	TGMessage *message = new TGMessage;
	message->SetGuaranteed( true );

	TGBufferStream stream;
	char buffer[0x400];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	if ( versionDifferent ) {
		stream.WriteChar( NETFILE_VERSION_DIFFERENT_MESSAGE );
		stream.WriteShort( strlen( UtopiaModule::GetGameVersion() ) );
		// BUG: the length is the version's, but the bytes written are the file name's.
		stream.Write( fileName, strlen( UtopiaModule::GetGameVersion() ) );
	} else {
		stream.WriteChar( NETFILE_SYSTEM_CHECKSUM_FAIL_MESSAGE );
		stream.WriteShort( strlen( fileName ) );
		stream.Write( fileName, strlen( fileName ) );
	}
	message->SetData( buffer, stream.GetPos() );
	g_kUtopiaModule.GetNetwork()->SendTGMessage( playerID, message, 0 );
}

/*
================
NetFile::PostSystemChecksumComplete
================
*/
// FUNCTION: STBC 0x006a4bb0
void NetFile::PostSystemChecksumComplete( int playerID ) {
	TGPlayerEvent *event = new TGPlayerEvent;
	event->SetEventType( ET_SYSTEM_CHECKSUM_COMPLETE );
	event->SetPlayerID( playerID );
	event->SetDestination( this );
	g_kEventManager.AddEvent( event );
}

/*
================
NetFile::ReceiveChecksumFailure

The host turned this player away: show why, with the file it names, and
disconnect.
================
*/
// FUNCTION: STBC 0x006a4c10
void NetFile::ReceiveChecksumFailure( TGMessage *message ) {
	if ( g_kUtopiaModule.IsClient() ) {
		TGBufferStream stream;
		int length;
		char *data = message->GetData( &length );
		stream.VirtualSlot60( data, length );

		int type = stream.ReadChar();
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
		char *fileName;
		TGString *reason;
		if ( type == NETFILE_SYSTEM_CHECKSUM_FAIL_MESSAGE ) {
			// BUG: the name is never freed.
			short nameLength = stream.ReadShort();
			fileName = new char[nameLength + 1];
			stream.Read( fileName, nameLength );
			fileName[nameLength] = '\0';
			reason = database->GetString( "SystemChecksumFail" );
		} else {
			// BUG: the name is never freed.
			short nameLength = stream.ReadShort();
			fileName = new char[nameLength + 1];
			stream.Read( fileName, nameLength );
			fileName[nameLength] = '\0';
			reason = database->GetString( "VersionDifferent" );
		}
		TGString status = *reason + fileName;
		MultiplayerWindow::SetStatusMessage( &status );
		g_kLocalizationManager.Unload( database );

		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		network->SetBootReason( BOOT_SCRIPT_CHECK_FAILED );
		network->Disconnect();
	}
}

/*
================
NetFile::ReadChecksumRequest

The request's first two bytes, its type and its index, are skipped. An
empty filter is sent as a zero length.
================
*/
// FUNCTION: STBC 0x006a4d80
bool NetFile::ReadChecksumRequest( TGMessage *message, char *directory, char *filter, bool *recursive ) {
	TGBufferStream stream;
	int length;
	char *data = message->GetData( &length );
	stream.VirtualSlot60( data, length );

	stream.ReadChar();
	stream.ReadChar();

	int directoryLength = stream.ReadShort();
	stream.Read( directory, directoryLength );
	directory[directoryLength] = '\0';

	int filterLength = stream.ReadShort();
	if ( filterLength > 0 ) {
		stream.Read( filter, filterLength );
		filter[filterLength] = '\0';
	} else {
		filter[0] = '\0';
	}

	*recursive = stream.ReadBool();
	stream.Close();
	return true;
}

/*
================
NetFile::CompareDirectoryChecksums

The player's files and subdirectories are matched to this machine's by
the hashes of their names.
================
*/
// FUNCTION: STBC 0x006a4e70
void NetFile::CompareDirectoryChecksums( TGStream *stream, TGDirectory *directory, int playerID, bool skip ) {
	int i;
	int numFiles = stream->ReadShort();
	if ( skip ) {
		for ( i = 0; i < numFiles; i++ ) {
			stream->ReadLong();
			stream->ReadLong();
		}
		int numSubdirectories = stream->ReadChar();
		for ( i = 0; i < numSubdirectories; i++ ) {
			stream->ReadLong();
		}
		for ( i = 0; i < numSubdirectories; i++ ) {
			CompareDirectoryChecksums( stream, NULL, playerID, true );
		}
		return;
	}

	int *hashes = new int[numFiles];
	int *checksums = new int[numFiles];
	for ( i = 0; i < numFiles; i++ ) {
		hashes[i] = stream->ReadLong();
		checksums[i] = stream->ReadLong();
	}

	for ( TGDirectoryFile *file = directory->GetFiles(); file != NULL; file = file->next ) {
		int nameHash = TGHashString( file->name );
		char path[0x400];
		int j;
		for ( j = 0; j < numFiles; j++ ) {
			if ( hashes[j] == nameHash ) {
				strcpy( path, directory->GetPath() );
				strcat( path, "/" );
				strcat( path, file->name );
				if ( GetFileChecksum( path ) != checksums[j] ) {
					AddFileToSend( path, playerID );
				}
				break;
			}
		}
		if ( j >= numFiles ) {
			strcpy( path, directory->GetPath() );
			strcat( path, "/" );
			strcat( path, file->name );
			AddFileToSend( path, playerID );
		}
	}
	delete hashes;
	delete checksums;

	int numSubdirectories = stream->ReadChar();
	hashes = new int[numSubdirectories];
	for ( i = 0; i < numSubdirectories; i++ ) {
		hashes[i] = stream->ReadLong();
	}

	TGDirectory *subdirectory;
	for ( subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
		int nameHash = TGHashString( subdirectory->GetName() );
		int j;
		for ( j = 0; j < numSubdirectories; j++ ) {
			if ( hashes[j] == nameHash ) {
				break;
			}
		}
		if ( j >= numSubdirectories ) {
			AddDirectoryToSend( subdirectory, playerID );
		}
	}

	for ( i = 0; i < numSubdirectories; i++ ) {
		for ( subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
			if ( TGHashString( subdirectory->GetName() ) == hashes[i] ) {
				CompareDirectoryChecksums( stream, subdirectory, playerID, false );
				break;
			}
		}
		if ( subdirectory == NULL ) {
			CompareDirectoryChecksums( stream, NULL, playerID, true );
		}
	}
	delete hashes;
}

/*
================
NetFile::CompareScriptChecksums

As CompareDirectoryChecksums, except that only files both machines have
are compared: a file or subdirectory the player lacks is not listed.
================
*/
// FUNCTION: STBC 0x006a5290
// PARKED: frame slot wall, the same as CompareDirectoryChecksums': retail gives the file count, the name hashes and the copy-loop counter other stack slots
void NetFile::CompareScriptChecksums( TGStream *stream, TGDirectory *directory, int playerID, bool skip ) {
	int i;
	int numFiles = stream->ReadShort();
	if ( skip ) {
		for ( i = 0; i < numFiles; i++ ) {
			stream->ReadLong();
			stream->ReadLong();
		}
		int numSubdirectories = stream->ReadChar();
		for ( i = 0; i < numSubdirectories; i++ ) {
			stream->ReadLong();
		}
		for ( i = 0; i < numSubdirectories; i++ ) {
			CompareScriptChecksums( stream, NULL, playerID, true );
		}
		return;
	}

	int *nameHashes = new int[numFiles];
	int *checksums = new int[numFiles];
	for ( i = 0; i < numFiles; i++ ) {
		nameHashes[i] = stream->ReadLong();
		checksums[i] = stream->ReadLong();
	}

	for ( TGDirectoryFile *file = directory->GetFiles(); file != NULL; file = file->next ) {
		int nameHash = TGHashString( file->name );
		for ( int j = 0; j < numFiles; j++ ) {
			if ( nameHashes[j] == nameHash ) {
				char path[0x400];
				strcpy( path, directory->GetPath() );
				strcat( path, "/" );
				strcat( path, file->name );
				if ( GetFileChecksum( path ) != checksums[j] ) {
					AddFileToSend( path, playerID );
				}
				break;
			}
		}
	}
	delete nameHashes;
	delete checksums;

	int numSubdirectories = stream->ReadChar();
	int *subdirectoryHashes = new int[numSubdirectories];
	for ( i = 0; i < numSubdirectories; i++ ) {
		subdirectoryHashes[i] = stream->ReadLong();
	}

	for ( i = 0; i < numSubdirectories; i++ ) {
		TGDirectory *subdirectory;
		for ( subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
			if ( TGHashString( subdirectory->GetName() ) == subdirectoryHashes[i] ) {
				CompareScriptChecksums( stream, subdirectory, playerID, false );
				break;
			}
		}
		if ( subdirectory == NULL ) {
			CompareScriptChecksums( stream, NULL, playerID, true );
		}
	}
	delete subdirectoryHashes;
}

/*
================
NetFile::AddDirectoryToSend
================
*/
// FUNCTION: STBC 0x006a5570
void NetFile::AddDirectoryToSend( TGDirectory *directory, int playerID ) {
	for ( TGDirectoryFile *file = directory->GetFiles(); file != NULL; file = file->next ) {
		char path[0x400];
		strcpy( path, directory->GetPath() );
		strcat( path, "/" );
		strcat( path, file->name );
		AddFileToSend( path, playerID );
	}

	for ( TGDirectory *subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
		AddDirectoryToSend( subdirectory, playerID );
	}
}

/*
================
NetFile::AddFileToSend

The list keeps its own copy of the name.
================
*/
// FUNCTION: STBC 0x006a5660
void NetFile::AddFileToSend( char *name, int playerID ) {
	NetFileNameList *list;
	if ( !fileLists.GetAt( playerID, list ) ) {
		list = new NetFileNameList;
		fileLists.SetAt( playerID, list );
	}

	char *copy = new char[strlen( name ) + 1];
	strcpy( copy, name );
	list->AddTail( copy );
}

/*
================
NetFile::SendNextFile

Sends the player the next file of its list that can be read. With the
list empty, the list goes; then, or when the player has no list, the
player is told the files are done and this object that its check is
complete. With SkipChecksum the list is emptied without sending.
================
*/
// FUNCTION: STBC 0x006a5860
// PARKED: inline budget: under the two-level pooled operator new this build expands ~NiTList inside `delete list` and calls NiTListBlock's ??_G and RemoveAll out of line, where retail inlines ??_G and calls ~NiTList (0x478480); TGMessage::Create / TGPlayerEvent::Create for one or two of the three allocations change nothing (97.1), for all three 94.7
void NetFile::SendNextFile( int playerID ) {
	while ( true ) {
		TGNetwork *network = g_kUtopiaModule.GetNetwork();
		if ( network == NULL ) {
			int listPlayerID;
			NetFileNameList *list;
			NiTMapIterator pos = fileLists.GetFirstPos();
			while ( pos ) {
				fileLists.GetNext( pos, listPlayerID, list );
				if ( list != NULL ) {
					delete list;
				}
			}
			fileLists.RemoveAll();
			return;
		}

		NetFileNameList *list;
		if ( !fileLists.GetAt( playerID, list ) ) {
			TGMessage *message = new TGMessage;
			message->SetGuaranteed( true );

			TGBufferStream stream;
			char buffer[8];
			stream.VirtualSlot60( buffer, sizeof( buffer ) );
			stream.WriteChar( NETFILE_FILES_DONE_MESSAGE );
			message->SetData( buffer, stream.GetPos() );
			network->SendTGMessage( playerID, message, 0 );
			stream.Close();

			TGPlayerEvent *event = new TGPlayerEvent;
			event->SetEventType( ET_CHECKSUM_COMPLETE );
			event->SetDestination( this );
			event->SetPlayerID( playerID );
			g_kEventManager.AddEvent( event );
			return;
		}

		bool done = false;
		while ( true ) {
			if ( list->IsEmpty() ) {
				done = true;
				break;
			}
			if ( skipChecksum ) {
				while ( !list->IsEmpty() ) {
					delete list->GetHead();
					list->RemoveHead();
				}
				done = true;
				break;
			}

			char *name = list->GetHead();
			FILE *file = g_kSystemWrapper.FileOpen( name, "rb" );
			if ( file == NULL ) {
				delete name;
				list->RemoveHead();
				continue;
			}

			int size = strlen( name ) + 3;
			int fileLength = g_kSystemWrapper.FileLength( file );
			size += fileLength;
			char *buffer = new char[size];
			if ( buffer == NULL ) {
				delete name;
				list->RemoveHead();
				continue;
			}

			TGBufferStream stream;
			stream.VirtualSlot60( buffer, size );
			stream.WriteChar( NETFILE_FILE_MESSAGE );
			stream.WriteShort( strlen( name ) );
			stream.Write( name, strlen( name ) );
			char *contents = buffer + stream.GetPos();
			g_kSystemWrapper.FileRead( file, contents, fileLength );
			g_kSystemWrapper.FileClose( file );

			TGMessage *message = new TGMessage;
			message->SetGuaranteed( true );
			message->SetDataNoCopy( buffer, size );
			network->SendTGMessage( playerID, message, 0 );

			delete name;
			list->RemoveHead();
			break;
		}

		if ( !list->IsEmpty() && !done ) {
			return;
		}
		if ( list != NULL ) {
			delete list;
		}
		fileLists.RemoveAt( playerID );
		if ( !skipChecksum ) {
			return;
		}
	}
}

/*
================
NetFile::ReceiveChecksumRequest

The host asks for the checksums of a directory. The first request of the
script check (index 0) first reloads the scripts the host may have sent,
and its answer carries the game version's hash. A directory that can't be
listed is answered with the hash of its name plus one.
================
*/
// FUNCTION: STBC 0x006a5df0
void NetFile::ReceiveChecksumRequest( TGMessage *message ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network == NULL ) {
		return;
	}

	TGBufferStream stream;
	int length;
	char *data = message->GetData( &length );
	stream.VirtualSlot60( data, length );

	stream.ReadChar();
	char index = stream.ReadChar();

	char directoryName[0x400];
	int directoryLength = stream.ReadShort();
	stream.Read( directoryName, directoryLength );
	directoryName[directoryLength] = '\0';

	char filter[0x400];
	int filterLength = stream.ReadShort();
	if ( filterLength > 0 ) {
		stream.Read( filter, filterLength );
		filter[filterLength] = '\0';
	} else {
		filter[0] = '\0';
	}

	bool recursive = stream.ReadBool();
	stream.Close();

	if ( index == 0 ) {
		ReloadScripts();
	}

	TGDirectory *directory = new TGDirectory;
	bool listed;
	if ( filter[0] == '\0' ) {
		listed = directory->Read( directoryName, NULL, recursive );
	} else {
		listed = directory->Read( directoryName, filter, recursive );
	}
	if ( !listed ) {
		TGMessage *reply = new TGMessage;
		reply->SetGuaranteed( true );

		char *buffer = new char[6];
		stream.VirtualSlot60( buffer, 6 );
		stream.WriteChar( NETFILE_CHECKSUM_MESSAGE );
		stream.WriteChar( index );
		if ( index == 0 ) {
			stream.WriteLong( g_kSystemWrapper.GetRandomNumber( 0xFFFF ) );
		}
		stream.WriteLong( TGHashString( directoryName ) + 1 );
		// BUG: the answer is never sent, and the listing is never deleted.
		reply->SetDataNoCopy( buffer, stream.GetPos() );
		return;
	}

	TGMessage *reply = new TGMessage;
	reply->SetGuaranteed( true );

	char *buffer = new char[0x20000];
	stream.VirtualSlot60( buffer, 0x20000 );
	stream.WriteChar( NETFILE_CHECKSUM_MESSAGE );
	stream.WriteChar( index );
	if ( index == 0 ) {
		stream.WriteLong( TGHashString( UtopiaModule::GetGameVersion() ) );
	}
	stream.WriteLong( TGHashString( directory->GetName() ) );
	WriteDirectoryChecksums( &stream, directory );
	reply->SetData( buffer, stream.GetPos() );
	network->SendTGMessage( network->GetHostID(), reply, 0 );
	delete buffer;
	delete directory;

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Multiplayer.tgl", true );
	TGString *serverFound = database->GetString( "Server Found" );
	TGString status;
	status = *serverFound;
	MultiplayerWindow::SetStatusMessage( &status );
	g_kLocalizationManager.Unload( database );
}

/*
================
NetFile::WriteDirectoryChecksums
================
*/
// FUNCTION: STBC 0x006a6190
void NetFile::WriteDirectoryChecksums( TGStream *stream, TGDirectory *directory ) {
	TGDirectoryFile *file = directory->GetFiles();
	stream->WriteShort( directory->GetNumFiles() );
	for ( ; file != NULL; file = file->next ) {
		stream->WriteLong( TGHashString( file->name ) );

		char path[0x400];
		strcpy( path, directory->GetPath() );
		strcat( path, "/" );
		strcat( path, file->name );
		int checksum = GetFileChecksum( path );
		stream->WriteLong( checksum );
	}

	TGDirectory *subdirectory = directory->GetSubdirectories();
	stream->WriteChar( directory->GetNumSubdirectories() );
	for ( ; subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
		stream->WriteLong( TGHashString( subdirectory->GetName() ) );
	}
	for ( subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
		WriteDirectoryChecksums( stream, subdirectory );
	}
}

/*
================
NetFile::GetFileChecksum

Each word is folded in with an exclusive or and a one-bit left rotation;
the rotation is an inline assembly rol.
================
*/
// FUNCTION: STBC 0x006a62f0
// ORIGINAL-ASM: the rotations are the original's inline `__asm rol` (an EBP frame and `rol dword ptr [ebp+8],1` that VC6 emits only for inline assembly)
// PARKED: register wall: retail keeps the buffer in a frame slot and folds the word through EDI (load, xor, store) where this build xors into memory
int NetFile::GetFileChecksum( char *path ) {
	FILE *file = g_kSystemWrapper.FileOpen( path, "rb" );
	if ( file == NULL ) {
		return 0;
	}

	int length = g_kSystemWrapper.FileLength( file );
	char *buffer = new char[length];
	if ( buffer == NULL ) {
		return 0;
	}
	g_kSystemWrapper.FileRead( file, buffer, length );
	g_kSystemWrapper.FileClose( file );

	int checksum = 0;
	char *data = buffer;
	int i;
	for ( i = 0; i < length / 4; i++ ) {
		if ( i != 1 ) {
			checksum ^= *(int *)data;
			__asm rol checksum, 1
		}
		data += 4;
	}
	for ( i *= 4; i < length; i++ ) {
		// BUG: every remaining byte is the same one: data isn't advanced.
		checksum ^= *data;
		__asm rol checksum, 1
	}

	delete buffer;
	return checksum;
}

/*
================
NetFile::DeleteAllMessageLists
================
*/
// FUNCTION: STBC 0x006a63b0
void NetFile::DeleteAllMessageLists( void ) {
	NiTMapIterator pos = messageLists.GetFirstPos();
	while ( pos ) {
		int playerID;
		NiTList<TGMessage *> *list;
		messageLists.GetNext( pos, playerID, list );
		while ( !list->IsEmpty() ) {
			delete list->RemoveTail();
		}
		delete list;
	}
	messageLists.RemoveAll();
}

/*
================
NetFile::DeleteMessageList
================
*/
// FUNCTION: STBC 0x006a6500
void NetFile::DeleteMessageList( int playerID ) {
	NiTList<TGMessage *> *list;
	if ( messageLists.GetAt( playerID, list ) ) {
		NiTListIterator pos = list->GetHeadPos();
		while ( pos ) {
			delete list->GetNext( pos );
		}
		delete list;
		messageLists.RemoveAt( playerID );
	}
}

/*
================
NetFile::ReloadScripts
================
*/
// FUNCTION: STBC 0x006a6630
void NetFile::ReloadScripts( void ) {
	PyEmbed::GetModule( "Autoexec", false );
	ReloadDirectory( "Scripts/ships", NULL );
	ReloadDirectory( "Scripts/Systems", NULL );
	ReloadDirectory( "Scripts/Multiplayer", NULL );
}

/*
================
NetFile::ReloadDirectory
================
*/
// FUNCTION: STBC 0x006a6670
void NetFile::ReloadDirectory( char *path, TGDirectory *directory ) {
	bool owned = false;
	if ( directory == NULL ) {
		directory = new TGDirectory;
		directory->Read( path, "*.py", true );
		owned = true;
	}

	if ( directory != NULL ) {
		char filePath[0x400];
		for ( TGDirectoryFile *file = directory->GetFiles(); file != NULL; file = file->next ) {
			sprintf( filePath, "%s/%sc", path, file->name );
			g_kSystemWrapper.SetFileReadOnly( filePath, false );
			char moduleName[0x400];
			GetModuleName( filePath, moduleName, sizeof( moduleName ) );
			PyEmbed::GetModule( moduleName, false );
		}
		for ( TGDirectory *subdirectory = directory->GetSubdirectories(); subdirectory != NULL; subdirectory = subdirectory->GetNext() ) {
			sprintf( filePath, "%s/%s", path, subdirectory->GetName() );
			ReloadDirectory( filePath, subdirectory );
		}
		if ( owned ) {
			delete directory;
		}
	}
}

/*
================
NetFile::GetModuleName
================
*/
// FUNCTION: STBC 0x006a67b0
void NetFile::GetModuleName( char *path, char *moduleName, int size ) {
	char *name = path;
	if ( strnicmp( path, "Scripts/", strlen( "Scripts/" ) ) == 0 ) {
		name = path + strlen( "Scripts/" );
	}

	int length = strlen( name );
	for ( int i = 0; i < length; i++ ) {
		if ( i >= size ) {
			break;
		}
		if ( name[i] == '/' || name[i] == '\\' ) {
			moduleName[i] = '.';
		} else if ( name[i] == '.' ) {
			moduleName[i] = '\0';
			break;
		} else {
			moduleName[i] = name[i];
		}
	}
	moduleName[size - 1] = '\0';
}
