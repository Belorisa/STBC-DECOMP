/*
===============================================================================

	FiringChain.cpp

===============================================================================
*/

#include "FiringChain.h"
#include "TGStream.h"

#include <string.h>

/*
================
FiringChain::FiringChain
================
*/
// FUNCTION: STBC 0x00586190
FiringChain::FiringChain( void ) {
	name = NULL;
	groups = 0;
}

/*
================
FiringChain::~FiringChain
================
*/
// FUNCTION: STBC 0x005861a0
FiringChain::~FiringChain( void ) {
	SetName( NULL );
}

/*
================
FiringChain::SetName

Frees the old name and keeps a copy of the new one; NULL clears it.
================
*/
// FUNCTION: STBC 0x005861b0
void FiringChain::SetName( char *name_ ) {
	if ( name != NULL ) {
		delete[] name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
FiringChain::GetFirstGroup

The lowest group in the chain; 0 when it has none.
================
*/
// FUNCTION: STBC 0x00586220
int FiringChain::GetFirstGroup( void ) {
	if ( groups != 0 ) {
		int bits = groups;
		int i = 0;
		while ( !( bits & 1 ) && i < FIRINGCHAIN_MAX_GROUPS ) {
			bits >>= 1;
			i++;
		}
		if ( i < FIRINGCHAIN_MAX_GROUPS ) {
			return i + 1;
		}
	}
	return 0;
}

/*
================
FiringChain::GetNextGroup

The next group in the chain after the given one; 0 when there is none.
================
*/
// FUNCTION: STBC 0x00586250
int FiringChain::GetNextGroup( int group ) {
	if ( groups != 0 ) {
		int next = group + 1;
		int bits = groups >> group;
		while ( !( bits & 1 ) && next < FIRINGCHAIN_MAX_GROUPS ) {
			bits >>= 1;
			next++;
		}
		if ( next < FIRINGCHAIN_MAX_GROUPS ) {
			return next;
		}
	}
	return 0;
}

/*
================
FiringChain::WriteToStream
================
*/
// FUNCTION: STBC 0x00586280
bool FiringChain::WriteToStream( TGStream *stream ) {
	stream->WriteInt( groups );
	stream->VirtualSlot11( name );
	return true;
}

/*
================
FiringChain::ReadFromStream
================
*/
// FUNCTION: STBC 0x005862b0
bool FiringChain::ReadFromStream( TGStream *stream ) {
	groups = stream->ReadInt();
	name = stream->VirtualSlot10( -1 );
	return true;
}
