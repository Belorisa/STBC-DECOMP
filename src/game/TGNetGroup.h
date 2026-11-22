/*
===============================================================================

	TGNetGroup.h

	A named group of players that messages can be sent to
	(TGNetwork::SendTGMessageToGroup). The network keeps its groups in an
	array sorted by name.

	Evidence: new_TGNetGroup allocates 0x14 bytes and stores the vtable
	0x894684 (one slot, the scalar deleting destructor 0x5E6630, which
	deletes the name at 0x04 and the elements of the array at 0x08);
	TGNetGroup_GetPlayerList returns the address of the array at 0x08 as a
	TGGroupList. TGNetwork::AddGroup orders the groups by strcmp of their
	names.

===============================================================================
*/

#ifndef __TGNETGROUP_H__
#define __TGNETGROUP_H__

#include <string.h>

#include "SortedArray.h"
#include "TGPlayerList.h"

// A member of a group: a player's net ID, in its own allocation (4 bytes,
// TGNetGroup_AddPlayerToGroup). The SWIG interface names it, and its one
// member (TGGroupPlayer_m_iNetID_set and _get, at 0x00).
struct TGGroupPlayer {
	int				netID;				// 0x00

	int				Compare( TGGroupPlayer *other ) { return other->netID - netID; }
	int				CompareKey( int key ) { return key - netID; }
};

// The members of a group, by player ID. SortedArray's out-of-line DeleteAt
// for it is 0x6A2E60.
typedef SortedArray<TGGroupPlayer, int>	TGGroupList;

// FUNCTION: STBC 0x005e6630 ??_GTGNetGroup@@UAEPAXI@Z @game/AppWrap
class TGNetGroup {
public:
					TGNetGroup( void ) { name = NULL; }
	// FUNCTION: STBC 0x005e6650 ??1TGNetGroup@@UAE@XZ @game/AppWrap
	virtual			~TGNetGroup( void ) {
						if ( name != NULL ) {
							delete name;
						}
						players.DeleteAll();
					}

	// A copy of the name; NULL keeps none.
	void			SetName( char *name_ ) {
						if ( name != NULL ) {
							delete name;
							name = NULL;
						}
						if ( name_ != NULL ) {
							name = new char[strlen( name_ ) + 1];
							strcpy( name, name_ );
						}
					}

	char *			GetName( void ) { return name; }
	TGGroupList *	GetPlayerList( void ) { return &players; }

	// The members by player ID: a member is the player's ID in its own
	// allocation.
	void			AddPlayerToGroup( int playerID ) {
						TGGroupPlayer *member = new TGGroupPlayer;
						member->netID = playerID;
						players.Insert( member );
					}
	void			DeletePlayerFromGroup( int playerID ) {
						players.Delete( playerID );
					}
	bool			IsPlayerInGroup( int playerID ) { return players.Get( playerID ) != NULL; }

	// Drops the members that are not in the player list or have gone.
	void			Validate( TGPlayerList *playerList ) {
						int count = players.GetCount();
						TGGroupPlayer **items = players.GetItems();
						for ( int i = 0; i < count; i++ ) {
							if ( items[i] != NULL ) {
								TGNetPlayer *player = playerList->GetPlayerByNetID( items[i]->netID );
								if ( player == NULL || player->disconnected ) {
									players.DeleteAt( i );
									i--;
									count--;
								}
							}
						}
					}

	// A new group with the name and the members (the SWIG wrapper TGNetGroup_Copy).
	// FUNCTION: STBC 0x005e67d0 ?Copy@TGNetGroup@@QAEPAV1@XZ @game/AppWrap
	TGNetGroup *	Copy( void ) {
						TGNetGroup *copy = new TGNetGroup;
						copy->SetName( name );
						int count = players.GetCount();
						TGGroupPlayer **items = players.GetItems();
						for ( int i = 0; i < count; i++ ) {
							copy->AddPlayerToGroup( items[i]->netID );
						}
						return copy;
					}

	// The order of the network's groups: by name.
	int				Compare( TGNetGroup *other ) { return strcmp( other->name, name ); }
	int				CompareKey( char *key ) { return strcmp( key, name ); }

protected:
	char *			name;				// 0x04 owned
	TGGroupList		players;			// 0x08 (TGNetGroup_GetPlayerList)
};

#endif /* !__TGNETGROUP_H__ */
