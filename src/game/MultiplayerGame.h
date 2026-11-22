/*
===============================================================================

	MultiplayerGame.h

	The game of a multiplayer session: it relays the players' ship events
	over the network as messages and turns the messages it receives back
	into events, and keeps a place for each player.

	0x200 bytes, allocated from the pools. Evidence: the constructor
	0x69E590 calls Game's, builds the sixteen places at 0x74, stores the
	vtable 0x88B480 (27 slots: Game's, with OnIdle and Terminate
	overridden) and adds the broadcast handlers whose names
	("MultiplayerGame :: ...Handler") RegisterHandlerNames (0x69EFE0)
	registers; the destructor is 0x69EBB0. The stream constructor, the
	type functions and the scalar deleting destructor are inline: their
	copies are in UtopiaModule's unit (0x442830-0x442940). GetObjType
	returns 0x80D4; the class strings are "MultiplayerGame",
	"_p_MultiplayerGame" and "MultiplayerGamePtr".

===============================================================================
*/

#ifndef __MULTIPLAYERGAME_H__
#define __MULTIPLAYERGAME_H__

#include "Game.h"
#include "MultiplayerPlayer.h"

// GetObjType's value for a MultiplayerGame (the type its Cast checks).
#define MULTIPLAYERGAME_TYPE	0x80D4

// Network message types: the first byte of a game message's data, as the
// SWIG constants of the App module give them.
#define GAME_INITIALIZE_MESSAGE				0x00
#define GAME_INITIALIZE_DONE_MESSAGE		0x01
#define CREATE_OBJECT_MESSAGE				0x02
#define CREATE_PLAYER_OBJECT_MESSAGE		0x03
#define DESTROY_OBJECT_MESSAGE				0x04
#define TORPEDO_POSITION_MESSAGE			0x05
#define HOST_EVENT_MESSAGE					0x06
#define START_FIRING_MESSAGE				0x07
#define STOP_FIRING_MESSAGE					0x08
#define STOP_FIRING_AT_TARGET_MESSAGE		0x09
#define SUBSYSTEM_STATE_CHANGED_MESSAGE		0x0A
#define ADD_TO_REPAIR_LIST_MESSAGE			0x0B
#define CLIENT_EVENT_MESSAGE				0x0C
#define CHANGED_TARGET_MESSAGE				0x0D
#define START_CLOAKING_MESSAGE				0x0E
#define STOP_CLOAKING_MESSAGE				0x0F
#define START_WARP_MESSAGE					0x10
#define REPAIR_LIST_PRIORITY_MESSAGE		0x11
#define SET_PHASER_LEVEL_MESSAGE			0x12
#define SELF_DESTRUCT_REQUEST_MESSAGE		0x13
#define DELETE_OBJECT_FROM_GAME_MESSAGE		0x14
#define CLIENT_COLLISION_MESSAGE			0x15
#define COLLISION_ENABLED_MESSAGE			0x16
#define NEW_PLAYER_IN_GAME_MESSAGE			0x17
#define DELETE_PLAYER_FROM_GAME_MESSAGE		0x18
#define CREATE_TORP_MESSAGE					0x19
#define CREATE_PULSE_MESSAGE				0x1A
#define TORPEDO_TYPE_CHANGED_MESSAGE		0x1B
#define SHIP_UPDATE_MESSAGE					0x1C
#define VERIFY_ENTER_SET_MESSAGE			0x1D
#define SEND_OBJECT_MESSAGE					0x1E
#define VERIFY_EXITED_WARP_MESSAGE			0x1F
#define DAMAGE_VOLUME_MESSAGE				0x29
#define CLIENT_READY_MESSAGE				0x2A

// The size of the buffers the game's messages are written into.
#define MAX_MESSAGE_SIZE					0x400

// The part of the buffer GAME_INITIALIZE_MESSAGE is written into (invented name).
#define GAME_INITIALIZE_SIZE				256

// How long the joined and left notices stay up, in seconds (invented name).
#define NOTICE_DURATION						5.0f

// Events of the game's (the SWIG constants of the App module).
#define ET_OBJECT_EXPLODING					0x80004E
#define ET_START_CLOAKING_NOTIFY			0x8000E2
#define ET_STOP_CLOAKING_NOTIFY				0x8000E4
#define ET_KILL_GAME						0x8000E9
#define ET_START_WARP_NOTIFY				0x8000EC
#define ET_NEW_PLAYER_IN_GAME				0x8000F1
#define ET_RETRY_CONNECTION					0x8000FF

class ObjectClass;
class ShipClass;
class TGEvent;
class TGMessage;
class TGMessageEvent;
class TGObject;
class TGPlayerEvent;
class TGString;

// The players of a game.
#define MAX_PLAYERS				16

// The SWIG inline accessors read IsReadyForNewPlayers 0x1F8 and
// Get/SetMaxPlayers 0x1FC.
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00442940 ??_GMultiplayerGame@@UAEPAXI@Z @game/UtopiaModule
class MultiplayerGame : public Game {
public:
	virtual			~MultiplayerGame( void );

	// TGObject
	// FUNCTION: STBC 0x00442890 ?GetObjType@MultiplayerGame@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return MULTIPLAYERGAME_TYPE; }
	// Tests its own type, then Game's, with ScriptObject's chain inline.
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00442900 ?GetClassName@MultiplayerGame@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "MultiplayerGame"; }
	// FUNCTION: STBC 0x00442910 ?GetSwigTypeName@MultiplayerGame@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_MultiplayerGame"; }
	// FUNCTION: STBC 0x00442920 ?GetPythonClassName@MultiplayerGame@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "MultiplayerGamePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00442930 ?GetClassHandlers@MultiplayerGame@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ScriptObject
	virtual void	OnIdle( void );
	virtual void	Terminate( TGEvent *event );

	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	// The handlers. The ones that only pass an event on send it to the
	// host, or, on the host, to every other player.
	void			ReceiveMessageHandler( TGMessageEvent *event );
	void			DisconnectHandler( TGEvent *event );
	void			NewPlayerHandler( TGPlayerEvent *event );
	void			SystemChecksumPassedHandler( TGPlayerEvent *event );
	void			SystemChecksumFailedHandler( TGEvent *event );
	void			DeletePlayerHandler( TGPlayerEvent *event );
	void			ObjectCreatedHandler( TGEvent *event );
	void			HostEventHandler( TGEvent *event );
	void			ObjectExplodingHandler( TGEvent *event );
	void			NewPlayerInGameHandler( TGEvent *event );
	void			EnterSetHandler( TGEvent *event );
	void			ExitedWarpHandler( TGEvent *event );
	void			DeleteObjectHandler( TGEvent *event );
	void			ChangedTargetHandler( TGEvent *event );
	void			ChecksumCompleteHandler( TGEvent *event );
	void			KillGameHandler( TGEvent *event );
	void			RetryConnectHandler( TGEvent *event );
	void			StartFiringHandler( TGEvent *event );
	void			StartWarpHandler( TGEvent *event );
	void			TorpedoTypeChangedHandler( TGEvent *event );
	void			StopFiringHandler( TGEvent *event );
	void			StopFiringAtTargetHandler( TGEvent *event );
	void			StartCloakingHandler( TGEvent *event );
	void			StopCloakingHandler( TGEvent *event );
	void			SubsystemStateChangedHandler( TGEvent *event );
	void			AddToRepairListHandler( TGEvent *event );
	void			ClientEventHandler( TGEvent *event );
	void			RepairListPriorityHandler( TGEvent *event );
	void			SetPhaserLevelHandler( TGEvent *event );

	static MultiplayerGame *	Cast( TGObject * );
	int	GetPlayerNumberFromID( long );
	void	SetReadyForNewPlayers( bool );
	TGString *	GetPlayerName( int );
	int	GetNumberPlayersInGame( void );
	bool	IsPlayerInGame( int );
	ShipClass *	GetShipFromPlayerID( int );
	void	DeletePlayerShipsAndTorps( void );
	void	DeleteObjectFromGame( ObjectClass * );
	bool	IsPlayerUsingModem( int );

	bool	IsReadyForNewPlayers( void ) { return readyForNewPlayers; }
	int		GetMaxPlayers( void ) { return maxPlayers; }
	void	SetMaxPlayers( int maxPlayers_ ) { maxPlayers = maxPlayers_; }

protected:
	// The index of the player with the network ID; 0 (the first) when
	// none has it (invented name).
	int		FindPlayerIndex( int playerID );

	// Empties every place (invented name).
	void	InitPlayers( void );

	// Sends the state of the ships to the other players: the host each
	// player that of every object in the set of the player's ship, a
	// client the host that of its own ship (invented name).
	void	SendShipUpdates( void );

	// The handlers of the messages ReceiveMessageHandler dispatches on
	// their type (invented names).
	void	ReceiveCreateObjectMessage( TGMessage *message, bool playerObject );
	void	ReceiveHostEventMessage( TGMessage *message );
	void	ReceiveCreateTorpMessage( TGMessage *message );
	void	ReceiveCreatePulseMessage( TGMessage *message );
	void	ReceiveEventMessage( TGMessage *message, int eventType );
	void	ReceiveShipUpdateMessage( TGMessage *message );
	void	ReceiveDamageVolumeMessage( TGMessage *message );
	void	ReceiveSelfDestructRequestMessage( TGMessage *message );
	void	ReceiveDeleteObjectFromGameMessage( TGMessage *message );
	void	ReceiveSendObjectMessage( TGMessage *message );
	void	ReceiveVerifyEnterSetMessage( TGMessage *message );
	void	ReceiveVerifyExitedWarpMessage( TGMessage *message );
	void	ReceiveNewPlayerInGameMessage( TGMessage *message );
	void	ReceiveDeletePlayerFromGameMessage( TGMessage *message );
	void	ReceiveClientReadyMessage( TGMessage *message );
	void	ReceiveClientCollisionMessage( TGMessage *message );
	void	Unknown69FFF0( TGMessage *message );

	// Writes the event into a message of the type and sends it to the
	// host, or, on the host, to every other player (invented name).
	// Public: ShipClass::CollisionEffectHandler (0x5AF9C0) calls it too.
public:
	void	SendEventMessage( TGEvent *event, char messageType );

protected:
	MultiplayerPlayer	players[MAX_PLAYERS];	// 0x74 by player number
	int					unknown1F4;				// 0x1F4
	bool				readyForNewPlayers;		// 0x1F8
	int					maxPlayers;				// 0x1FC

	static TGEventHandlerTable	classHandlers;	// invented name

	// Declared for the SWIG wrappers that call them.
public:
					MultiplayerGame( char *script, int maxPlayers_ = MAX_PLAYERS );
	// The loader's: UtopiaModule's creator for MULTIPLAYERGAME_TYPE (0x4427F0) expands it.
	// FUNCTION: STBC 0x00442830 ??0MultiplayerGame@@QAE@J@Z @game/UtopiaModule
					MultiplayerGame( long id ) : Game( id ) {}
	static MultiplayerGame *	Create( char *script ) { return new MultiplayerGame( script ); }	// the SWIG wrapper MultiplayerGame_Create expands it
};

#endif /* !__MULTIPLAYERGAME_H__ */
