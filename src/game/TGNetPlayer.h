/*
===============================================================================

	TGNetPlayer.h

	A player of a networked game, with the message queues the network
	keeps for it. 0xC0 bytes.

	Evidence: the constructor 0x6C08D0 (TGNetwork allocates 0xC0 bytes for
	it) stores the vtable 0x895A2C: the scalar deleting destructor
	0x6C09F0 and Copy (0x6C1700, the slot the SWIG wrapper TGNetPlayer_Copy
	calls), which builds a player through the copy constructor 0x6C0B60;
	it builds the name and three TGLinkedLists. The
	player list keeps the players in an array sorted by the member at 0x18
	and finds them by the member at 0x1C (GetPlayerFromAddress,
	ChangePlayerNetID).

===============================================================================
*/

#ifndef __TGNETPLAYER_H__
#define __TGNETPLAYER_H__

#include <stdio.h>

#include "TGLinkedList.h"
#include "TGString.h"

class TGAckMessage;
class TGMessage;

class TGNetPlayer {
public:
					TGNetPlayer( void );
					TGNetPlayer( const TGNetPlayer &player );
	virtual			~TGNetPlayer( void );

	// A new player made with the copy constructor.
	virtual TGNetPlayer *	Copy( void );

	void	SetName( TGString * );
	int	GetBytesPerSecondTo( void );
	int	GetBytesPerSecondFrom( void );

	// The profile's samples: each whole second up to the time closes a
	// sample, splitting the bytes counted since the last one by the part
	// of the time that falls in it (invented name).
	void	UpdateProfile( float time );
	// The player's averages, for the network's profile file (invented name).
	void	WriteProfile( FILE *file );

	// Count a packet in the open profile sample, with and without the
	// transport's header (invented names).
	void	AddBytesReceived( int bytes, int headerSize ) {
				sampleBytesReceived += bytes;
				sampleBytesReceivedWithHeaders += bytes + headerSize;
			}
	void	AddBytesSent( int bytes, int headerSize ) {
				sampleBytesSent += bytes;
				sampleBytesSentWithHeaders += bytes + headerSize;
			}

	float	GetLastReceiveTime( void ) { return unknown2C; }	// invented name
	float	GetLastSendTime( void ) { return unknown30; }		// invented name

	int				GetConnectAddress( void ) { return connectAddress; }
	TGString *		GetName( void ) { return &name; }
	void			SetConnectAddress( int address ) { connectAddress = address; }
	int				GetNetID( void ) { return netID; }
	void			SetNetID( int id ) { netID = id; }
	int				GetNetAddress( void ) { return address; }
	void			SetNetAddress( int address ) { this->address = address; }
	int				GetConnectUID( void ) { return connectUID; }
	bool			IsDisconnected( void ) { return disconnected; }
	void			SetDisconnected( bool disconnected ) { this->disconnected = disconnected; }
	void			SetConnectUID( int uid ) { connectUID = uid; }

	// The order of the player list: by net ID.
	int		Compare( TGNetPlayer *other ) { return other->netID - netID; }
	int		CompareKey( int key ) { return key - netID; }

	TGString		name;				// 0x04 SetName stores it; GetName and GetCName of TGNetwork return it
	int				connectAddress;		// 0x14 (TGNetPlayer_GetConnectAddress, SetConnectAddress)
	int				netID;				// 0x18 the key of TGPlayerList; ChangePlayerNetID sets it (TGNetPlayer_GetNetID)
	int				address;			// 0x1C what GetPlayerFromAddress matches (TGNetPlayer_GetNetAddress)
	int				connectUID;			// 0x20 (TGNetPlayer_GetConnectUID, SetConnectUID)
	unsigned short	unknown24;			// 0x24
	unsigned short	unknown26;			// 0x26
	unsigned short	unknown28;			// 0x28 the sequence number of the next guaranteed message expected from the player
	unsigned short	unknown2A;			// 0x2A
	float			unknown2C;			// 0x2C the clock when the player was made, and when a message last came from it
	float			unknown30;			// 0x30 the clock when the player was made, and when a message last went to it
	int				bytesReceived;		// 0x34 the profile's totals and the counts of the open sample ("Received BPS")
	int				sampleBytesReceived;	// 0x38
	int				bytesReceivedWithHeaders;	// 0x3C ("Received Plus Header BPS")
	int				sampleBytesReceivedWithHeaders;	// 0x40
	int				bytesSent;			// 0x44 ("Sent BPS")
	int				sampleBytesSent;	// 0x48
	int				bytesSentWithHeaders;	// 0x4C ("Sent Plus Header BPS")
	int				numSamples;			// 0x50 ("Num samples")
	int				sampleBytesSentWithHeaders;	// 0x54
	int				bytesPerSecondFrom;	// 0x58 (TGNetPlayer_GetBytesPerSecondFrom)
	int				bytesPerSecondTo;	// 0x5C (TGNetPlayer_GetBytesPerSecondTo)
	float			lastSampleTime;		// 0x60 the time of the last profile sample
	TGLinkedList<TGMessage>	unknown64;	// 0x64
	TGLinkedList<TGMessage>	unknown80;	// 0x80
	TGLinkedList<TGMessage>	unknown9C;	// 0x9C the acknowledgements (TGAckMessage) to send to the player; a message list (SendMessages removes from it through the message list's RemoveAt 0x6B78D0)
	float			disconnectTime;		// 0xB8 the clock when the network marked the player disconnected (invented name)
	bool			disconnected;		// 0xBC set by TGNetwork when the player is to go (invented name)
};

#endif /* !__TGNETPLAYER_H__ */
