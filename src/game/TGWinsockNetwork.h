/*
===============================================================================

	TGWinsockNetwork.h

	The network over winsock datagram sockets. 0x34C bytes
	(new_TGWinsockNetwork; the scalar deleting destructor 0x6B9C50 frees
	0x34C bytes to the pool).

===============================================================================
*/

#ifndef __TGWINSOCKNETWORK_H__
#define __TGWINSOCKNETWORK_H__

#include <winsock.h>

// <winsock.h> brings in <windows.h>, whose ANSI/wide macros would rename
// the game's own methods of the same names.
#undef GetClassName
#undef GetObject
#undef PlaySound

#include "TGNetwork.h"
#include "TGSocket.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// Its vtable 0x8958F0 overrides the destructor (0x6B9C50) and slots 21-29
// (0x6B9D20, 0x6BAC50, 0x6B9FA0, 0x6B9460, 0x6B9560, 0x6B9950, 0x6B95F0,
// 0x6B9870, 0x6B9E40).
class TGWinsockNetwork : public TGNetwork {
public:
	// The game's default port (the SWIG constant TGWinsockNetwork_WINSOCK_PORT).
	enum { WINSOCK_PORT = 0x5655 };

					TGWinsockNetwork( void );
	virtual			~TGWinsockNetwork( void );
	int	GetPortNumber( void ) { return portNumber; }
	// The SOCKET; GameSpy's query reporting shares it (invented name).
	unsigned int	GetSocket( void ) { return socket.handle; }

	// TGNetwork
	virtual char *	GetLocalIPAddress( void );
	virtual int		GetIPPacketHeaderSize( void );
	virtual bool	VirtualSlot23( int );
	virtual int		VirtualSlot24( char *address, bool isHost, TGString *password );
	virtual int		VirtualSlot25( void );
	virtual bool	VirtualSlot26( int );
	virtual char *	VirtualSlot27( int *length, int *address );
	virtual bool	VirtualSlot28( int address, char *data, int length );
	virtual void	VirtualSlot29( int playerID );

	bool	SetPortNumber( int, bool );
	static void	BanPlayerByIP( int );
	static void	BanPlayerByName( char * );

protected:
	// A received packet waiting for VirtualSlot27 (invented name).
	struct Packet {
		char *			data;			// 0x00 owned
		Packet *		next;			// 0x04
	};

	// The port a player at an address sends to (invented name).
	struct PortEntry {
						PortEntry( void ) { next = NULL; }
						~PortEntry( void ) {}

		int				address;		// 0x00
		int				port;			// 0x04
		PortEntry *		next;			// 0x08
	};

	// Closes the socket and ends the winsock session (invented name).
	void			CloseSocket( void );
	// Takes the local address from the host name or the dotted address:
	// the last of the host's addresses; false when there is none (invented
	// name).
	bool			SetLocalAddress( char *hostName );
	// The address of a host name or a dotted address; -1 when there is none
	// (invented name).
	int				ResolveAddress( char *hostName );
	// Opens the datagram socket; a host binds it to the port and makes it
	// non-blocking. False on failure, and always for a client (invented
	// name).
	bool			OpenSocket( int port );
	// Takes the oldest packet sent to the local address off the queue; NULL
	// when there is none (invented name).
	char *			PopLocalPacket( void ) {
						Packet *packet = packetHead;
						if ( packet == NULL ) {
							return NULL;
						}
						if ( packet == packetTail ) {
							packetTail = NULL;
							packetHead = NULL;
						} else {
							packetHead = packet->next;
						}
						char *data = packet->data;
						delete packet;
						return data;
					}
	// The port the player at the address listens on: the one recorded for
	// it, or the game's own (invented names).
	int				GetPlayerPort( int address );
	void			SetPlayerPort( int address, int port );
	void			RemovePlayerPort( int address );

	TGSocket		socket;				// 0x190 (invented name)
	int				unknown198;			// 0x198
	in_addr			unknown19C;			// 0x19C the address SetLocalAddress found last
	int				unknown1A0;			// 0x1A0
	int				unknown1A4;			// 0x1A4
	WSADATA			wsaData;			// 0x1A8 what WSAStartup fills (invented name)
	int				portNumber;			// 0x338 the port the game listens on (invented name; SetPortNumber stores it)
	Packet *		packetHead;			// 0x33C (invented name)
	Packet *		packetTail;			// 0x340 (invented name)
	bool			unknown344;			// 0x344 flips on each VirtualSlot27: when set, a packet to the local address goes first
	PortEntry *		ports;				// 0x348 (invented name)
};

#endif /* !__TGWINSOCKNETWORK_H__ */
