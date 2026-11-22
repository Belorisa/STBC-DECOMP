/*
===============================================================================

	TGPlayerList.cpp

===============================================================================
*/

#include <iostream>

#include "TGPlayerList.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006bb910 _$E32
// FUNCTION: STBC 0x006bb920 _$E29
// FUNCTION: STBC 0x006bb930 _$E31
// FUNCTION: STBC 0x006bb940 _$E30
// FUNCTION: STBC 0x006bb950 _$E38
// FUNCTION: STBC 0x006bb960 _$E35
// FUNCTION: STBC 0x006bb970 _$E37
// FUNCTION: STBC 0x006bb980 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bb8e0 _$E46
// FUNCTION: STBC 0x006bb900 _$E45

/*
================
TGPlayerList::TGPlayerList
================
*/
// FUNCTION: STBC 0x006bb990
TGPlayerList::TGPlayerList( void ) {
}

/*
================
TGPlayerList::GetPlayerAtIndex

NULL past the end.
================
*/
// FUNCTION: STBC 0x006bb9b0 ?GetPlayerAtIndex@TGPlayerList@@QAEPAVTGNetPlayer@@H@Z
TGNetPlayer *TGPlayerList::GetPlayerAtIndex( int index ) {
	if ( index < players.GetCount() ) {
		return players.GetAt( index );
	}
	return NULL;
}

/*
================
TGPlayerList::GetPlayerFromAddress

The first player with the address; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006bb9d0 ?GetPlayerFromAddress@TGPlayerList@@QAEPAVTGNetPlayer@@H@Z
TGNetPlayer *TGPlayerList::GetPlayerFromAddress( int address ) {
	int count = players.GetCount();
	TGNetPlayer **items = players.GetItems();
	for ( int i = 0; i < count; i++ ) {
		if ( items[i]->address == address ) {
			return items[i];
		}
	}
	return NULL;
}

/*
================
TGPlayerList::ChangePlayerNetID

Gives the player of the address a new net ID and puts it in its new place in
the array.
================
*/
// FUNCTION: STBC 0x006bba10 ?ChangePlayerNetID@TGPlayerList@@QAE_NHH@Z
bool TGPlayerList::ChangePlayerNetID( int address, int netID ) {
	TGNetPlayer *player = GetPlayerFromAddress( address );
	if ( player != NULL ) {
		// BUG: the index is not tested; a missing net ID would remove element -1
		players.RemoveAt( players.Find( player->netID ) );
		player->netID = netID;
		players.Insert( player );
		return true;
	}
	return false;
}

/*
================
TGPlayerList::DeletePlayerByAddress

Deletes the player of the address.
================
*/
// FUNCTION: STBC 0x006bbb80 ?DeletePlayerByAddress@TGPlayerList@@QAEXH@Z
void TGPlayerList::DeletePlayerByAddress( int address ) {
	TGNetPlayer *player = GetPlayerFromAddress( address );
	if ( player != NULL ) {
		// BUG: the index is not tested; a missing net ID would delete element -1
		players.DeleteAt( players.Find( player->netID ) );
	}
}
