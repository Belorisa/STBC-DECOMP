/*
===============================================================================

	TGWinsockNetwork.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <winsock.h>

#include "TGWinsockNetwork.h"
#include "TGEncrypt.h"
#include "TGEventManager.h"
#include "TGGameSpyEvent.h"
#include "TGNetPlayer.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006ba490 _$E426
// FUNCTION: STBC 0x006ba4b0 _$E425

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ba2b0 _$E32
// FUNCTION: STBC 0x006ba2c0 _$E29
// FUNCTION: STBC 0x006ba2d0 _$E31
// FUNCTION: STBC 0x006ba2e0 _$E30
// FUNCTION: STBC 0x006ba2f0 _$E38
// FUNCTION: STBC 0x006ba300 _$E35
// FUNCTION: STBC 0x006ba310 _$E37
// FUNCTION: STBC 0x006ba320 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ba330 _$E63
// FUNCTION: STBC 0x006ba340 _$E62
// FUNCTION: STBC 0x006ba350 _$E67
// FUNCTION: STBC 0x006ba360 _$E66
// FUNCTION: STBC 0x006ba370 _$E78
// FUNCTION: STBC 0x006ba380 _$E77
// FUNCTION: STBC 0x006ba390 _$E82
// FUNCTION: STBC 0x006ba3a0 _$E81
// FUNCTION: STBC 0x006ba3b0 _$E86
// FUNCTION: STBC 0x006ba3c0 _$E85
// FUNCTION: STBC 0x006ba3d0 _$E90
// FUNCTION: STBC 0x006ba3e0 _$E89
// FUNCTION: STBC 0x006ba3f0 _$E214
// FUNCTION: STBC 0x006ba400 _$E213
// FUNCTION: STBC 0x006ba410 _$E156
// FUNCTION: STBC 0x006ba420 _$E155
// FUNCTION: STBC 0x006ba430 _$E169
// FUNCTION: STBC 0x006ba440 _$E168
// FUNCTION: STBC 0x006ba450 _$E219
// FUNCTION: STBC 0x006ba460 _$E218
// FUNCTION: STBC 0x006ba470 _$E224
// FUNCTION: STBC 0x006ba480 _$E223
// FUNCTION: STBC 0x006ba4c0 _$E228
// FUNCTION: STBC 0x006ba4d0 _$E227
// FUNCTION: STBC 0x006ba4e0 _$E232
// FUNCTION: STBC 0x006ba4f0 _$E231
// FUNCTION: STBC 0x006ba500 _$E239
// FUNCTION: STBC 0x006ba510 _$E238
// FUNCTION: STBC 0x006ba520 _$E264
// FUNCTION: STBC 0x006ba530 _$E263
// FUNCTION: STBC 0x006ba540 _$E268
// FUNCTION: STBC 0x006ba550 _$E267
// FUNCTION: STBC 0x006ba560 _$E114
// FUNCTION: STBC 0x006ba570 _$E113
// FUNCTION: STBC 0x006ba580 _$E250
// FUNCTION: STBC 0x006ba590 _$E249
// FUNCTION: STBC 0x006ba5a0 _$E257
// FUNCTION: STBC 0x006ba5b0 _$E256
// FUNCTION: STBC 0x006ba5c0 _$E280
// FUNCTION: STBC 0x006ba5d0 _$E279
// FUNCTION: STBC 0x006ba5e0 _$E284
// FUNCTION: STBC 0x006ba5f0 _$E283
// FUNCTION: STBC 0x006ba600 _$E324
// FUNCTION: STBC 0x006ba610 _$E323
// FUNCTION: STBC 0x006ba620 _$E330
// FUNCTION: STBC 0x006ba630 _$E329
// FUNCTION: STBC 0x006ba640 _$E272
// FUNCTION: STBC 0x006ba650 _$E271
// FUNCTION: STBC 0x006ba660 _$E276
// FUNCTION: STBC 0x006ba670 _$E275
// FUNCTION: STBC 0x006ba680 _$E288
// FUNCTION: STBC 0x006ba690 _$E287
// FUNCTION: STBC 0x006ba6a0 _$E292
// FUNCTION: STBC 0x006ba6b0 _$E291
// FUNCTION: STBC 0x006ba6c0 _$E296
// FUNCTION: STBC 0x006ba6d0 _$E295
// FUNCTION: STBC 0x006ba6e0 _$E300
// FUNCTION: STBC 0x006ba6f0 _$E299
// FUNCTION: STBC 0x006ba700 _$E304
// FUNCTION: STBC 0x006ba710 _$E303
// FUNCTION: STBC 0x006ba720 _$E308
// FUNCTION: STBC 0x006ba730 _$E307
// FUNCTION: STBC 0x006ba740 _$E312
// FUNCTION: STBC 0x006ba750 _$E311
// FUNCTION: STBC 0x006ba760 _$E316
// FUNCTION: STBC 0x006ba770 _$E315
// FUNCTION: STBC 0x006ba780 _$E119
// FUNCTION: STBC 0x006ba790 _$E118
// FUNCTION: STBC 0x006ba7a0 _$E320
// FUNCTION: STBC 0x006ba7b0 _$E319
// FUNCTION: STBC 0x006ba7c0 _$E124
// FUNCTION: STBC 0x006ba7d0 _$E123
// FUNCTION: STBC 0x006ba7e0 _$E334
// FUNCTION: STBC 0x006ba7f0 _$E333
// FUNCTION: STBC 0x006ba800 _$E338
// FUNCTION: STBC 0x006ba810 _$E337
// FUNCTION: STBC 0x006ba820 _$E342
// FUNCTION: STBC 0x006ba830 _$E341
// FUNCTION: STBC 0x006ba840 _$E346
// FUNCTION: STBC 0x006ba850 _$E345
// FUNCTION: STBC 0x006ba860 _$E133
// FUNCTION: STBC 0x006ba870 _$E132
// FUNCTION: STBC 0x006ba880 _$E350
// FUNCTION: STBC 0x006ba890 _$E349
// FUNCTION: STBC 0x006ba8a0 _$E354
// FUNCTION: STBC 0x006ba8b0 _$E353
// FUNCTION: STBC 0x006ba8c0 _$E358
// FUNCTION: STBC 0x006ba8d0 _$E357
// FUNCTION: STBC 0x006ba8e0 _$E139
// FUNCTION: STBC 0x006ba8f0 _$E138
// FUNCTION: STBC 0x006ba900 _$E144
// FUNCTION: STBC 0x006ba910 _$E143
// FUNCTION: STBC 0x006ba920 _$E174
// FUNCTION: STBC 0x006ba930 _$E173
// FUNCTION: STBC 0x006ba940 _$E362
// FUNCTION: STBC 0x006ba950 _$E361
// FUNCTION: STBC 0x006ba960 _$E179
// FUNCTION: STBC 0x006ba970 _$E178
// FUNCTION: STBC 0x006ba980 _$E366
// FUNCTION: STBC 0x006ba990 _$E365
// FUNCTION: STBC 0x006ba9a0 _$E370
// FUNCTION: STBC 0x006ba9b0 _$E369
// FUNCTION: STBC 0x006ba9c0 _$E184
// FUNCTION: STBC 0x006ba9d0 _$E183
// FUNCTION: STBC 0x006ba9e0 _$E374
// FUNCTION: STBC 0x006ba9f0 _$E373
// FUNCTION: STBC 0x006baa00 _$E149
// FUNCTION: STBC 0x006baa10 _$E148
// FUNCTION: STBC 0x006baa20 _$E378
// FUNCTION: STBC 0x006baa30 _$E377
// FUNCTION: STBC 0x006baa40 _$E382
// FUNCTION: STBC 0x006baa50 _$E381
// FUNCTION: STBC 0x006baa60 _$E189
// FUNCTION: STBC 0x006baa70 _$E188
// FUNCTION: STBC 0x006baa80 _$E194
// FUNCTION: STBC 0x006baa90 _$E193
// FUNCTION: STBC 0x006baaa0 _$E386
// FUNCTION: STBC 0x006baab0 _$E385
// FUNCTION: STBC 0x006baac0 _$E390
// FUNCTION: STBC 0x006baad0 _$E389
// FUNCTION: STBC 0x006baae0 _$E394
// FUNCTION: STBC 0x006baaf0 _$E393
// FUNCTION: STBC 0x006bab00 _$E398
// FUNCTION: STBC 0x006bab10 _$E397
// FUNCTION: STBC 0x006bab20 _$E402
// FUNCTION: STBC 0x006bab30 _$E401
// FUNCTION: STBC 0x006bab40 _$E406
// FUNCTION: STBC 0x006bab50 _$E405
// FUNCTION: STBC 0x006bab60 _$E410
// FUNCTION: STBC 0x006bab70 _$E409
// FUNCTION: STBC 0x006bab80 _$E414
// FUNCTION: STBC 0x006bab90 _$E413
// FUNCTION: STBC 0x006baba0 _$E199
// FUNCTION: STBC 0x006babb0 _$E198
// FUNCTION: STBC 0x006babc0 _$E418
// FUNCTION: STBC 0x006babd0 _$E417
// FUNCTION: STBC 0x006babe0 _$E204
// FUNCTION: STBC 0x006babf0 _$E203
// FUNCTION: STBC 0x006bac00 _$E209
// FUNCTION: STBC 0x006bac10 _$E208

// The ports a game may listen on when the port is checked: the first
// above the well-known ones, and the last of the dynamic range below.
#define PORT_MINIMUM_REGISTERED		0x3FF
#define PORT_MINIMUM_GAME			0x1388
#define PORT_MAXIMUM_GAME			0xBFFF

/*
================
TGWinsockNetwork::VirtualSlot24

Starts winsock, takes the local address from the host's name and opens
the socket. A host is connected to itself; a client takes the address it
connects to, which must not be its own.
================
*/
// FUNCTION: STBC 0x006b9460
int TGWinsockNetwork::VirtualSlot24( char *address, bool isHost, TGString *password ) {
	char hostName[0x400];
	if ( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 ) {
		return TGNETWORK_ERROR_DRIVER_STARTUP;
	}
	if ( gethostname( hostName, sizeof( hostName ) ) != 0 ) {
		return TGNETWORK_ERROR_DRIVER_STARTUP;
	}
	if ( !SetLocalAddress( hostName ) ) {
		return TGNETWORK_ERROR_DRIVER_STARTUP;
	}
	unknown1C = unknown19C.s_addr;
	OpenSocket( portNumber );
	if ( this->isHost ) {
		unknown24 = unknown1C;
		localID = hostID;
		return TGNETWORK_OKAY;
	}
	if ( !SetLocalAddress( address ) ) {
		CloseSocket();
		return TGNETWORK_ERROR_INVALID_ADDRESS;
	}
	unknown24 = unknown19C.s_addr;
	if ( unknown19C.s_addr == unknown1C ) {
		CloseSocket();
		return TGNETWORK_ERROR_INVALID_ADDRESS;
	}
	return TGNETWORK_OKAY;
}

/*
================
TGWinsockNetwork::VirtualSlot25

Closes the connection; the packets not yet read are freed.
================
*/
// FUNCTION: STBC 0x006b9560
int TGWinsockNetwork::VirtualSlot25( void ) {
	if ( connectStatus != TGNETWORK_CONNECTED && connectStatus != TGNETWORK_CONNECT_IN_PROGRESS ) {
		return TGNETWORK_ERROR_NOT_CONNECTED;
	}
	CloseSocket();
	while ( packetHead != NULL ) {
		Packet *packet = packetHead;
		packetHead = packet->next;
		if ( packet->data != NULL ) {
			delete packet->data;
		}
		delete packet;
	}
	packetHead = NULL;
	packetTail = NULL;
	connectStatus = TGNETWORK_ERROR_NOT_CONNECTED;
	return TGNETWORK_OKAY;
}

/*
================
TGWinsockNetwork::CloseSocket
================
*/
// FUNCTION: STBC 0x006b95e0
void TGWinsockNetwork::CloseSocket( void ) {
	socket.Close();
	WSACleanup();
}

/*
================
TGWinsockNetwork::VirtualSlot27

The next received packet, its size and its sender's address; NULL when
there is none. Calls take turns between the packets sent to the local
address and the socket. A GameSpy query (a packet that starts with a
backslash) goes out as an ET_NETWORK_GAMESPY_MESSAGE event instead, and
a closed connection marks the network.
================
*/
// FUNCTION: STBC 0x006b95f0
// PARKED: same calls; retail expands the local-queue pop twice (sharing only its else arm) where this build jumps to one copy, and keeps zero in ebp where this build uses edi
char *TGWinsockNetwork::VirtualSlot27( int *length, int *address ) {
	bool localFirst = unknown344;
	unknown344 = !unknown344;
	if ( localFirst && packetHead != NULL ) {
		// BUG: the length and the address of a local packet are not filled in.
		return PopLocalPacket();
	}

	fd_set readSet;
	FD_ZERO( &readSet );
	FD_SET( socket.handle, &readSet );
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if ( select( 0, &readSet, NULL, NULL, &timeout ) > 0 ) {
		sockaddr_in from;
		int fromLength = sizeof( from );
		char *data = new char[unknownAC];
		int received = recvfrom( socket.handle, data, unknownAC, 0, (sockaddr *)&from, &fromLength );
		if ( received > 0 ) {
			*length = received;
			*address = from.sin_addr.s_addr;
			SetPlayerPort( *address, ntohs( from.sin_port ) );
			if ( data[0] == '\\' ) {
				TGGameSpyEvent *event = new TGGameSpyEvent;
				event->SetEventType( ET_NETWORK_GAMESPY_MESSAGE );
				event->SetDestination( this );
				// BUG: a packet that fills the buffer has its terminator written past the end.
				data[received] = '\0';
				event->SetText( data );
				delete [] data;
				sockaddr_in *sender = new sockaddr_in;
				*sender = from;
				event->SetAddress( sender );
				g_kEventManager.AddEvent( event );
				return NULL;
			}
			if ( encryptor != NULL ) {
				encryptor->Decrypt( data + 1, *length - 1 );
			}
			return data;
		}
		if ( received == 0 ) {
			delete [] data;
			unknown10D = true;
			return NULL;
		}
		WSAGetLastError();
		delete [] data;
		return NULL;
	}
	return PopLocalPacket();
}

/*
================
TGWinsockNetwork::VirtualSlot28

Sends the packet to the address: one to the local address is queued for
VirtualSlot27, others are encrypted (all but the sender's ID) and sent to
the player's port.
================
*/
// FUNCTION: STBC 0x006b9870
bool TGWinsockNetwork::VirtualSlot28( int address, char *data, int length ) {
	if ( address == unknown1C ) {
		char *copy = new char[length];
		memcpy( copy, data, length );
		Packet *packet = new Packet;
		packet->data = copy;
		packet->next = NULL;
		if ( packetTail == NULL ) {
			packetTail = packet;
			packetHead = packet;
		} else {
			packetTail->next = packet;
			packetTail = packet;
		}
		return true;
	}

	if ( encryptor != NULL ) {
		encryptor->Encrypt( data + 1, length - 1 );
	}
	sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = address;
	to.sin_port = htons( GetPlayerPort( address ) );
	sendto( socket.handle, data, length, 0, (sockaddr *)&to, sizeof( to ) );
	return true;
}

/*
================
TGWinsockNetwork::VirtualSlot26

Whether the socket can take a packet now; the address is not used.
================
*/
// FUNCTION: STBC 0x006b9950
bool TGWinsockNetwork::VirtualSlot26( int address ) {
	fd_set writeSet;
	FD_ZERO( &writeSet );
	FD_SET( socket.handle, &writeSet );
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	return select( 0, NULL, &writeSet, NULL, &timeout ) > 0 ? true : false;
}

/*
================
TGWinsockNetwork::SetLocalAddress
================
*/
// FUNCTION: STBC 0x006b99a0
bool TGWinsockNetwork::SetLocalAddress( char *hostName ) {
	if ( hostName != NULL ) {
		char name[0x100];
		strcpy( name, hostName );
		if ( name[strlen( name ) - 1] == '\n' || name[strlen( name ) - 1] == '\r' ) {
			name[strlen( name ) - 1] = '\0';
		}
		int address = inet_addr( name );
		if ( address != INADDR_NONE ) {
			unknown19C.s_addr = address;
		} else {
			hostent *host = gethostbyname( name );
			if ( host == NULL ) {
				return false;
			}
			int i = 0;
			while ( true ) {
				if ( host->h_addr_list[i] == NULL ) {
					break;
				}
				unknown19C = *(in_addr *)host->h_addr_list[i];
				i++;
			}
		}
		return true;
	}
	return false;
}

/*
================
TGWinsockNetwork::ResolveAddress
================
*/
// FUNCTION: STBC 0x006b9a70
int TGWinsockNetwork::ResolveAddress( char *hostName ) {
	if ( hostName != NULL ) {
		char name[0x100];
		strcpy( name, hostName );
		if ( name[strlen( name ) - 1] == '\n' || name[strlen( name ) - 1] == '\r' ) {
			name[strlen( name ) - 1] = '\0';
		}
		int address = inet_addr( name );
		if ( address != INADDR_NONE ) {
			return address;
		}
		hostent *host = gethostbyname( name );
		if ( host != NULL ) {
			return *(int *)host->h_addr_list[0];
		}
	}
	return INADDR_NONE;
}

/*
================
TGWinsockNetwork::OpenSocket
================
*/
// FUNCTION: STBC 0x006b9b20
bool TGWinsockNetwork::OpenSocket( int port ) {
	SOCKET handle = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	socket.handle = handle;
	if ( handle != INVALID_SOCKET && isHost ) {
		sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = htonl( INADDR_ANY );
		local.sin_port = htons( port );
		if ( bind( handle, (sockaddr *)&local, sizeof( local ) ) != SOCKET_ERROR ) {
			u_long nonBlocking = 1;
			ioctlsocket( handle, FIONBIO, &nonBlocking );
			socket.handle = handle;
			return true;
		}
	}
	return false;
}

/*
================
TGWinsockNetwork::SetPortNumber

Sets the port the game listens on. A checked port must be inside the range
games use.
================
*/
// FUNCTION: STBC 0x006b9bb0
bool TGWinsockNetwork::SetPortNumber( int port, bool check ) {
	if ( check ) {
		if ( port <= 0 ) {
			return false;
		}
		if ( port < PORT_MINIMUM_REGISTERED ) {
			return false;
		}
		if ( port >= PORT_MAXIMUM_GAME || port < PORT_MINIMUM_GAME ) {
			return false;
		}
	}
	portNumber = port;
	return true;
}

/*
================
TGWinsockNetwork::TGWinsockNetwork
================
*/
// FUNCTION: STBC 0x006b9bf0 ??0TGWinsockNetwork@@QAE@XZ
TGWinsockNetwork::TGWinsockNetwork( void ) : packetHead( NULL ), packetTail( NULL ), unknown344( false ), ports( NULL ) {
	unknownAC = 0x200;
	portNumber = WINSOCK_PORT;
}

/*
================
TGWinsockNetwork::~TGWinsockNetwork

The packets not yet read are freed.
================
*/
// FUNCTION: STBC 0x006b9c80 ??1TGWinsockNetwork@@UAE@XZ
// FUNCTION: STBC 0x006b9c50 ??_GTGWinsockNetwork@@UAEPAXI@Z
TGWinsockNetwork::~TGWinsockNetwork( void ) {
	while ( packetHead != NULL ) {
		Packet *packet = packetHead;
		packetHead = packet->next;
		if ( packet->data != NULL ) {
			delete packet->data;
		}
		delete packet;
	}
}

// The dotted local address GetLocalIPAddress returns.
// DATA: STBC 0x00996900
static char localIPAddress[16];

/*
================
TGWinsockNetwork::GetLocalIPAddress

The local address, in dotted form. Without a connection winsock is started
just for the lookup.
================
*/
// FUNCTION: STBC 0x006b9d20
char *TGWinsockNetwork::GetLocalIPAddress( void ) {
	char hostName[0x400];
	if ( connectStatus == TGNETWORK_CONNECTED || connectStatus == TGNETWORK_CONNECT_IN_PROGRESS ) {
		if ( gethostname( hostName, sizeof( hostName ) ) != 0 ) {
			return NULL;
		}
		SetLocalAddress( hostName );
		strcpy( localIPAddress, inet_ntoa( unknown19C ) );
		return localIPAddress;
	}
	if ( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 ) {
		return NULL;
	}
	if ( gethostname( hostName, sizeof( hostName ) ) != 0 ) {
		WSACleanup();
		return NULL;
	}
	SetLocalAddress( hostName );
	strcpy( localIPAddress, inet_ntoa( unknown19C ) );
	WSACleanup();
	return localIPAddress;
}

/*
================
TGWinsockNetwork::VirtualSlot29

The player's port entry goes with it.
================
*/
// FUNCTION: STBC 0x006b9e40
void TGWinsockNetwork::VirtualSlot29( int playerID ) {
	TGNetPlayer *player = playerList.GetPlayerByNetID( playerID );
	RemovePlayerPort( player->GetNetAddress() );
	TGNetwork::VirtualSlot29( playerID );
}

/*
================
TGWinsockNetwork::GetPlayerPort
================
*/
// FUNCTION: STBC 0x006b9eb0
int TGWinsockNetwork::GetPlayerPort( int address ) {
	for ( PortEntry *entry = ports; entry != NULL; entry = entry->next ) {
		if ( entry->address == address ) {
			return entry->port;
		}
	}
	return portNumber;
}

/*
================
TGWinsockNetwork::SetPlayerPort
================
*/
// FUNCTION: STBC 0x006b9ee0
void TGWinsockNetwork::SetPlayerPort( int address, int port ) {
	PortEntry *entry;
	for ( entry = ports; entry != NULL; entry = entry->next ) {
		if ( entry->address == address ) {
			entry->port = port;
			return;
		}
	}
	entry = new PortEntry;
	entry->address = address;
	entry->port = port;
	entry->next = ports;
	ports = entry;
}

/*
================
TGWinsockNetwork::RemovePlayerPort
================
*/
// FUNCTION: STBC 0x006b9f40
void TGWinsockNetwork::RemovePlayerPort( int address ) {
	PortEntry *entry = ports;
	if ( entry->address == address ) {
		ports = entry->next;
		delete entry;
		return;
	}
	for ( ; entry->next != NULL; entry = entry->next ) {
		if ( entry->next->address == address ) {
			PortEntry *removed = entry->next;
			entry->next = removed->next;
			delete removed;
			return;
		}
	}
}

/*
================
TGWinsockNetwork::VirtualSlot23

Whether the address is on the ban list: each line is a host name or a
dotted address, where a 255 matches any value of its part.
================
*/
// FUNCTION: STBC 0x006b9fa0
// PARKED: same calls and branches; retail reads the third and fourth bytes of the two addresses with a byte load per compare, this build with one word load each (pointer, in_addr field, nested-if and loop spellings tried)
bool TGWinsockNetwork::VirtualSlot23( int address ) {
	TGStream *stream = new TGStream;
	bool banned = false;
	stream->VirtualSlot00( "banlist.txt", "rt" );
	if ( stream->VirtualSlot58() != 0 ) {
		// BUG: the stream is never deleted.
		return false;
	}
	while ( stream->VirtualSlot58() == 0 && !stream->Eof() ) {
		char line[0x400];
		strcpy( line, "" );
		stream->ReadCLine( line, sizeof( line ), '\n' );
		int length = strlen( line );
		if ( length > 1 ) {
			if ( line[length - 1] == '\r' || line[length - 1] == '\n' ) {
				line[length - 1] = '\0';
			}
			if ( line[length - 2] == '\r' || line[length - 2] == '\n' ) {
				line[length - 2] = '\0';
			}
		}
		if ( line[0] != '\0' ) {
			int ban = ResolveAddress( line );
			if ( ban != INADDR_NONE ) {
				if ( ban == address ) {
					banned = true;
					break;
				}
				unsigned char *banPart = (unsigned char *)&ban;
				unsigned char *part = (unsigned char *)&address;
				if ( ( part[0] == banPart[0] || banPart[0] == 0xFF ) && ( part[1] == banPart[1] || banPart[1] == 0xFF ) &&
					( part[2] == banPart[2] || banPart[2] == 0xFF ) && ( part[3] == banPart[3] || banPart[3] == 0xFF ) ) {
					banned = true;
					break;
				}
			}
		}
	}
	// BUG: the stream is closed but never deleted
	stream->Close();
	return banned;
}

/*
================
TGWinsockNetwork::BanPlayerByIP

Appends the address, in dotted form, to the ban list file, a line of its
own.
================
*/
// FUNCTION: STBC 0x006ba160
void TGWinsockNetwork::BanPlayerByIP( int address ) {
	TGStream *stream = new TGStream;
	stream->VirtualSlot00( "banlist.txt", "at" );
	if ( stream->VirtualSlot58() == 0 ) {
		in_addr ip;
		ip.s_addr = address;
		char *text = inet_ntoa( ip );
		if ( text != NULL ) {
			stream->WriteCLine( text );
			stream->WriteCLine( "\r\n" );
		}
		// BUG: the stream is closed but never deleted
		stream->Close();
	}
}

/*
================
TGWinsockNetwork::BanPlayerByName

Appends the name to the ban list file, a line of its own.
================
*/
// FUNCTION: STBC 0x006ba200
void TGWinsockNetwork::BanPlayerByName( char *name ) {
	if ( name != NULL ) {
		TGStream *stream = new TGStream;
		stream->VirtualSlot00( "banlist.txt", "at" );
		if ( stream->VirtualSlot58() == 0 ) {
			stream->WriteCLine( name );
			if ( name[strlen( name ) - 1] != '\n' ) {
				stream->WriteCLine( "\r\n" );
			}
			// BUG: the stream is closed but never deleted
			stream->Close();
		}
	}
}

/*
================
TGWinsockNetwork::GetIPPacketHeaderSize

The bytes the IP and UDP headers add to each packet.
================
*/
// FUNCTION: STBC 0x006bac50
int TGWinsockNetwork::GetIPPacketHeaderSize( void ) {
	return 0x22;
}
