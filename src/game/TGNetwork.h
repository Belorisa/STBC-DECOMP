/*
===============================================================================

	TGNetwork.h

	The network layer of a multiplayer game: the players, the message
	queues and the groups. TGWinsockNetwork implements it over sockets.

===============================================================================
*/

#ifndef __TGNETWORK_H__
#define __TGNETWORK_H__

#include "TGEventHandlerObject.h"
#include "TGLinkedList.h"
#include "TGNetGroup.h"
#include "TGPlayerList.h"
#include "TGPoolManager.h"
#include "TGString.h"

class TGAckMessage;
class TGBootPlayerMessage;
class TGEncrypt;
class TGEvent;
class TGMessage;
class TGString;

// The connect states and the results of the network calls (the SWIG
// constants of the App module, in their table's order).
enum tgNetworkStatus_t {
	TGNETWORK_OKAY,
	TGNETWORK_ERROR,
	TGNETWORK_CONNECTED,
	TGNETWORK_CONNECT_IN_PROGRESS,
	TGNETWORK_ERROR_NOT_CONNECTED,
	TGNETWORK_ERROR_DISCONNECTED,
	TGNETWORK_ERROR_CONNECT_DENIED,
	TGNETWORK_ERROR_CONNECT_REQUIRE_PASSWORD,
	TGNETWORK_ERROR_CONNECT_TIMED_OUT,
	TGNETWORK_ERROR_ALREADY_CONNECTED,
	TGNETWORK_ERROR_TOO_MANY_SENDS_PENDING,
	TGNETWORK_ERROR_INVALID_ADDRESS,
	TGNETWORK_ERROR_LARGE_PACKET_NOT_GUARANTEED,
	TGNETWORK_ERROR_DRIVER_STARTUP,
	TGNETWORK_ERROR_SHUTDOWN_FAILED,
	TGNETWORK_ERROR_DO_BEFORE_CONNECTION,
	TGNETWORK_ERROR_GROUP_NOT_FOUND
};

// The network's events (the SWIG constants of the App module).
#define ET_NETWORK_MESSAGE_EVENT		0x60001
#define ET_NETWORK_CONNECT_EVENT		0x60002
#define ET_NETWORK_DISCONNECT_EVENT		0x60003
#define ET_NETWORK_NEW_PLAYER			0x60004
#define ET_NETWORK_DELETE_PLAYER		0x60005
#define ET_NETWORK_GAMESPY_MESSAGE		0x60006
#define ET_NETWORK_NAME_CHANGE_EVENT	0x60007

// The player IDs: 0 is never given out, 1 is the host's, and the others
// are handed out from 2 to MAX_PLAYER_ID.
#define HOST_PLAYER_ID			1
#define FIRST_PLAYER_ID			2
#define MAX_PLAYER_ID			0x7E

// GetObjType's value for a TGNetwork.
#define TGNETWORK_TYPE			0x801

// The engine's own message types (the SWIG constant App.MAX_MESSAGE_TYPES): the
// scripts number theirs from here (App.MAX_MESSAGE_TYPES + n).
#define MAX_MESSAGE_TYPES		43

// The size of the reader table: a message's type is its first byte (invented name).
#define MESSAGE_TYPE_TABLE_SIZE	256

// What reads a message of one type from the buffer it was received in.
typedef TGMessage *( *TGMessageUnSerializer )( char *buffer );

class TGNetwork;
typedef TGLinkedList<TGNetwork>	TGNetworkListType;		// the SWIG type of GetTGNetworkList

/*
	The network layer of a multiplayer game. 0x190 bytes, allocated from its
	own pool.

	Evidence: the constructor 0x6B3A00 (which TGWinsockNetwork's 0x6B9BF0
	calls) and the stream constructor 0x6B3300 store the vtable 0x895790
	(30 slots); the scalar deleting destructor 0x6B3590 frees 0x190 bytes.
	The SWIG wrappers TGNetwork_* read GetLocalID 0x18, GetHostID 0x20,
	GetNumPlayers 0x30, GetPlayerList 0x28, GetPassword 0xE0, GetEncryptor
	0xF0, GetGroup the array at 0xF4, GetBootReason 0x100, IsHost 0x10E,
	and write SetConnectionTimeout 0xB8 and EnableProfiling 0x110; they call
	GetLocalIPAddress through slot 21 and GetIPPacketHeaderSize through slot
	22. The bases are the ones the SWIG interface declares (the shadow
	classes in App.pyc).
*/
class TGNetwork : public TGEventHandlerObject {
public:
	TGPOOL_ALLOCATION

	// Why a player was booted (the SWIG constants TGNetwork_DEFAULT_BOOT to
	// TGNetwork_YOU_ARE_BANNED, in their table's order: Update boots with
	// TIMED_OUT, the connect handler with INCORRECT_PASSWORD and
	// YOU_ARE_BANNED).
	enum bootReason_t {
		DEFAULT_BOOT,
		TIMED_OUT,
		INCORRECT_PASSWORD,
		TOO_MANY_PLAYERS,
		SERVER_BOOTED_YOU,
		YOU_ARE_BANNED
	};

					TGNetwork( void );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006b3300 ??0TGNetwork@@QAE@J@Z @game/TGNetworkModule
					TGNetwork( long id ) : TGEventHandlerObject( id ) {}
	virtual			~TGNetwork( void );

	// TGObject
	// FUNCTION: STBC 0x006b34d0 ?GetObjType@TGNetwork@@UAEHXZ @game/TGNetwork
	virtual int		GetObjType( void ) { return TGNETWORK_TYPE; }
	// FUNCTION: STBC 0x006b34e0 ?IsTypeOf@TGNetwork@@UAE_NH@Z @game/TGNetwork
	virtual bool	IsTypeOf( int type ) {
						if ( type == TGNETWORK_TYPE ) {
							return true;
						}
						return TGEventHandlerObject::IsTypeOf( type );
					}
	// FUNCTION: STBC 0x006b3520 ?GetClassName@TGNetwork@@UAEPADXZ @game/TGNetwork
	virtual char *	GetClassName( void ) { return "TGNetwork"; }
	// FUNCTION: STBC 0x006b3530 ?GetSwigTypeName@TGNetwork@@UAEPADXZ @game/TGNetwork
	virtual char *	GetSwigTypeName( void ) { return "_p_TGNetwork"; }
	// FUNCTION: STBC 0x006b3540 ?GetPythonClassName@TGNetwork@@UAEPADXZ @game/TGNetwork
	virtual char *	GetPythonClassName( void ) { return "TGNetworkPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x006b3550 ?GetClassHandlers@TGNetwork@@UAEPAVTGEventHandlerTable@@XZ @game/TGNetwork
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// FUNCTION: STBC 0x006b3560 ?GetLocalIPAddress@TGNetwork@@UAEPADXZ @game/TGNetwork
	virtual char *	GetLocalIPAddress( void ) { return NULL; }
	// FUNCTION: STBC 0x006b3570 ?GetIPPacketHeaderSize@TGNetwork@@UAEHXZ @game/TGNetwork
	virtual int		GetIPPacketHeaderSize( void ) { return 0; }
	// FUNCTION: STBC 0x006b3580 ?VirtualSlot23@TGNetwork@@UAE_NH@Z @game/TGNetwork
	virtual bool	VirtualSlot23( int ) { return false; }
	virtual int		VirtualSlot24( char *address, bool isHost, TGString *password );		// Connect opens the connection through it
	virtual int		VirtualSlot25( void );
	virtual bool	VirtualSlot26( int );
	virtual char *	VirtualSlot27( int *length, int *address );		// the next received packet and its size and sender's address; NULL when there is none
	virtual bool	VirtualSlot28( int address, char *data, int length );
	virtual void	VirtualSlot29( int playerID );		// the player leaves; DeleteLocalPlayer calls it

	void	Update( void );
	int	Connect( char *address, TGString *password );
	int	Disconnect( void );
	int	GetConnectStatus( void );
	int	CreateLocalPlayer( TGString * );
	int	DeleteLocalPlayer( void );
	int	SetName( TGString * );
	TGString *	GetName( void );
	char *	GetCName( void );
	int	SendTGMessage( int, TGMessage *, int );
	int	SendTGMessageToGroup( char *, TGMessage * );
	int	SendTGMessageToGroup( TGNetGroup *group, TGMessage *message );		// invented overload
	int	SetSendTimeout( float );
	TGMessage *	GetNextMessage( bool );
	TGString *	GetHostName( void );
	void	SetPassword( TGString * );
	int	SetEncryptor( TGEncrypt * );
	bool	AddGroup( TGNetGroup * );
	TGNetGroup *	GetGroup( char *name ) { return groups.Get( name ); }
	void	DeleteGroup( char *name ) { groups.Delete( name ); }
	float	GetTimeElapsedSinceLastHostPing( void );
	static void	RegisterHandlers( void );
	static void	AddClassHandlers( void );
	void	ReceiveMessageHandler( TGEvent * );

	int				GetLocalID( void ) { return localID; }
	int				GetHostID( void ) { return hostID; }
	void			SetConnectionTimeout( float timeout ) { connectionTimeout = timeout; }

	TGEncrypt *		GetEncryptor( void ) { return encryptor; }
	bool			IsHost( void ) { return isHost; }
	TGPlayerList *	GetPlayerList( void ) { return &playerList; }
	int				GetNumPlayers( void ) { return playerList.GetNumPlayers(); }
	TGString *		GetPassword( void ) { return &password; }
	int				GetBootReason( void ) { return bootReason; }
	void			SetBootReason( int reason ) { bootReason = reason; }
	void			EnableProfiling( bool enable ) { profiling = enable; }

protected:
	TGNetPlayer *	CreatePlayer( int netID, int address );	// invented name
	int				QueueMessage( TGMessage *message, TGNetPlayer *player );	// invented name
	int				Unknown6B51E0( TGMessage *message );
	int				GetNewPlayerID( void );					// invented name
	void			FreePlayerID( int playerID );			// invented name
	// The player is marked gone, and ET_NETWORK_DELETE_PLAYER goes out; an
	// ID without a player is just freed (invented name).
	void			DisconnectPlayer( int playerID );
	// The three steps of an update (invented names): the queued messages go
	// out (and the unacknowledged ones again), the received packets are read
	// into messages, and the received system messages are handled.
	void			SendMessages( void );
	void			ReceiveMessages( void );
	void			ProcessMessages( void );
	// Queues the acknowledgement of a received guaranteed message, or
	// renews the one already queued (invented name).
	void			QueueAck( TGMessage *message, TGNetPlayer *player );
	// Files a received message in its queue: system or user, plain or
	// guaranteed (invented name).
	void			QueueReceivedMessage( TGMessage *message, TGNetPlayer *player );
	// Joins the parts of a multi-part message, once they are all in
	// (invented name).
	TGMessage *		AssembleMultiPart( TGMessage *message );
	// Deletes the queued messages of the message's type that it merges
	// (invented name).
	void			RemoveOverriddenMessages( TGMessage *message );
	// The handlers of the received system messages, by type (invented names).
	void			HandleNameChangeMessage( TGMessage *message, TGNetPlayer *player );
	void			HandleAckMessage( TGAckMessage *ack, TGNetPlayer *player );
	void			HandleConnectMessage( TGMessage *message, TGNetPlayer *player );
	void			HandleBootPlayerMessage( TGBootPlayerMessage *message, TGNetPlayer *player );
	void			HandleDisconnectMessage( TGMessage *message, TGNetPlayer *player );
	// Each player's profile takes its samples up to now (invented name).
	void			UpdateProfile( void );
	// The profile goes to the first free "netprofile<n>.txt": the local
	// player's first, then the others' (invented name).
	void			WriteProfile( void );

public:
	static TGNetworkListType *	GetTGNetworkList( void ) { return &networkList; }
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	static void		RegisterMessageType( int type, TGMessageUnSerializer unSerializer ) { messageTypes[type] = unSerializer; }

protected:
	int				connectStatus;		// 0x14 a tgNetworkStatus_t (invented name)
	int				localID;			// 0x18 the local player's ID (the SWIG wrapper TGNetwork_GetLocalID reads it)
	int				unknown1C;			// 0x1C
	int				hostID;				// 0x20 the host's player ID (the SWIG wrapper TGNetwork_GetHostID reads it)
	int				unknown24;			// 0x24
	TGPlayerList	playerList;			// 0x28 (invented name)
	TGLinkedList<TGMessage>	unknown38;	// 0x38 received messages; GetNextMessage takes them from the head
	TGLinkedList<TGMessage>	unknown54;	// 0x54 received guaranteed messages, kept for their sequence numbers
	TGLinkedList<TGMessage>	unknown70;	// 0x70
	TGLinkedList<TGMessage>	unknown8C;	// 0x8C
	int				unknownA8;			// 0xA8 0x8000 from the constructor
	int				unknownAC;			// 0xAC 0x400 from the constructor; 0x200 for winsock
	int				unknownB0;			// 0xB0
	float			sendTimeout;		// 0xB4 (invented name)
	float			connectionTimeout;	// 0xB8 SetConnectionTimeout
	float			lastHostPingTime;	// 0xBC the clock at the host's last ping (invented name)
	float			lastSendToHostTime;	// 0xC0 the clock when a message last went to the host (invented name)
	TGLinkedList<TGMessage>	unknownC4;	// 0xC4
	TGString		password;			// 0xE0
	TGEncrypt *		encryptor;			// 0xF0 owned
	SortedArray<TGNetGroup, char *>	groups;	// 0xF4 owned, by name (TGNetwork_GetGroup searches it)
	int				bootReason;			// 0x100 (TGNetwork_GetBootReason, SetBootReason)
	int				unknown104;			// 0x104
	int				nextPlayerID;		// 0x108 where GetNewPlayerID starts looking (invented name)
	bool			unknown10C;			// 0x10C true from the constructor
	bool			unknown10D;			// 0x10D
	bool			isHost;				// 0x10E
	bool			unknown10F;			// 0x10F
	bool			profiling;			// 0x110 (TGNetwork_EnableProfiling)
	bool			playerIDInUse[MAX_PLAYER_ID + 1];	// 0x111 by player ID; 0, 1 and 0x5C are never given out (invented name)

	static TGEventHandlerTable		classHandlers;		// invented name
	static TGNetworkListType		networkList;		// every network (TGNetwork_GetTGNetworkList; invented name)
	static TGMessageUnSerializer	messageTypes[MESSAGE_TYPE_TABLE_SIZE];	// by type (TGNetwork_RegisterMessageType; invented name)
};

#endif /* !__TGNETWORK_H__ */
