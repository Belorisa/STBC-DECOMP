/*
===============================================================================

	TGPlayerList.h

	The players of a networked game, in an array sorted by net ID.

	Evidence: the SWIG wrappers TGPlayerList_* name the members. The array
	(items at 0x04, count at 0x08, capacity at 0x0C) is the one the sorted
	array template expands. The constructor 0x6BB990 stores the vtable
	0x895808, whose one slot is the scalar deleting destructor 0x6B34B0.

===============================================================================
*/

#ifndef __TGPLAYERLIST_H__
#define __TGPLAYERLIST_H__

#include "SortedArray.h"
#include "TGNetPlayer.h"

// FUNCTION: STBC 0x006b34b0 ??_GTGPlayerList@@UAEPAXI@Z @game/TGNetwork
class TGPlayerList {
public:
					TGPlayerList( void );
					// FUNCTION: STBC 0x006b3450 ??1TGPlayerList@@UAE@XZ @game/TGNetwork
	virtual			~TGPlayerList( void ) {}

	int				GetNumPlayers( void ) { return players.GetCount(); }
	TGNetPlayer **	GetPlayerList( void ) { return players.GetItems(); }
	TGNetPlayer *	GetPlayerAtIndex( int index );
	TGNetPlayer *	GetPlayerByNetID( int netID ) { return players.Get( netID ); }		// invented name
	TGNetPlayer *	GetPlayerFromAddress( int address );
	bool	ChangePlayerNetID( int address, int netID );
	// The player with the net ID, found by the binary search; NULL when there is none (invented name).
	TGNetPlayer *	GetPlayerFromID( int netID ) { return players.Get( netID ); }
	void	DeletePlayerByAddress( int address );
	int		AddPlayer( TGNetPlayer *player ) { return players.Insert( player ); }
	// Deletes the player with the net ID, if there is one.
	void	DeletePlayer( int netID ) { players.Delete( netID ); }
	// Deletes the player at the index of the array.
	void	DeletePlayerAtIndex( int index ) { players.DeleteAt( index ); }
	// The player with the net ID, or NULL (the SWIG name).
	TGNetPlayer *	GetPlayer( int netID ) { return players.Get( netID ); }
	// Deletes every player (invented name).
	void	DeleteAllPlayers( void ) { players.DeleteAll(); }

protected:
	SortedArray<TGNetPlayer, int>	players;		// 0x04 owned, by net ID
};

#endif /* !__TGPLAYERLIST_H__ */
