/*
===============================================================================

	MultiplayerPlayer.h

	A player's place in a multiplayer game: MultiplayerGame keeps one for
	each of its MAX_PLAYERS places. The class name is invented.

	0x18 bytes. Evidence: MultiplayerGame's constructor builds its sixteen
	at 0x74 with an `eh vector constructor iterator` over the constructor
	0x6A7720, which stores the vtable 0x89565C (one slot, the scalar
	deleting destructor 0x6A7740) and calls Init 0x6A7770; the destructor
	0x6A7760 only restores the vtable. Init gives the place its block of
	object IDs from the place's number.

===============================================================================
*/

#ifndef __MULTIPLAYERPLAYER_H__
#define __MULTIPLAYERPLAYER_H__

// The network IDs of the players' objects: each player's block of
// 0x40000 IDs starts at FIRST_CLIENT_ID (invented names).
#define FIRST_CLIENT_ID			0x3FFFFFFF
#define CLIENT_ID_BLOCK_SIZE	0x40000

// FUNCTION: STBC 0x006a7740 ??_GMultiplayerPlayer@@UAEPAXI@Z
class MultiplayerPlayer {
public:
					MultiplayerPlayer( void );
	virtual			~MultiplayerPlayer( void );

	// Empties the place and gives it the object IDs of the player number
	// (invented name).
	void			Init( int playerNumber );

	bool			inUse;					// 0x04
	long			playerID;				// 0x08 the network ID
	long			shipID;					// 0x0C the ID of the player's ship
	long			firstObjectID;			// 0x10 the first ID of the place's block (invented name)
	bool			usingModem;				// 0x14 true until the player says otherwise
};

#endif /* !__MULTIPLAYERPLAYER_H__ */
