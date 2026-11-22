/*
===============================================================================

	TGSoundRegion.cpp

===============================================================================
*/

#include <stdio.h>
#include <string.h>

#include "TGPointerMap.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGSound.h"
#include "TGSoundRegion.h"
#include "TGStream.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0070ec00 _$E237
// FUNCTION: STBC 0x0070ec20 _$E236

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0070ec90 _$E21
// FUNCTION: STBC 0x0070eca0 _$E20
// FUNCTION: STBC 0x0070ecb0 _$E32
// FUNCTION: STBC 0x0070ecc0 _$E31
// FUNCTION: STBC 0x0070ecd0 _$E36
// FUNCTION: STBC 0x0070ece0 _$E35
// FUNCTION: STBC 0x0070ecf0 _$E40
// FUNCTION: STBC 0x0070ed00 _$E39
// FUNCTION: STBC 0x0070ed10 _$E44
// FUNCTION: STBC 0x0070ed20 _$E43
// FUNCTION: STBC 0x0070ed30 _$E168
// FUNCTION: STBC 0x0070ed40 _$E167
// FUNCTION: STBC 0x0070ed50 _$E172
// FUNCTION: STBC 0x0070ed60 _$E171
// FUNCTION: STBC 0x0070ed70 _$E177
// FUNCTION: STBC 0x0070ed80 _$E176
// FUNCTION: STBC 0x0070ed90 _$E181
// FUNCTION: STBC 0x0070eda0 _$E180
// FUNCTION: STBC 0x0070edb0 _$E186
// FUNCTION: STBC 0x0070edc0 _$E185
// FUNCTION: STBC 0x0070edd0 _$E190
// FUNCTION: STBC 0x0070ede0 _$E189

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).
// FUNCTION: STBC 0x0070e7e0 _$E218
// FUNCTION: STBC 0x0070e7f0 _$E215
// FUNCTION: STBC 0x0070e800 _$E217
// FUNCTION: STBC 0x0070e810 _$E216
// FUNCTION: STBC 0x0070e820 _$E224
// FUNCTION: STBC 0x0070e830 _$E221
// FUNCTION: STBC 0x0070e840 _$E223
// FUNCTION: STBC 0x0070e850 _$E222
// FUNCTION: STBC 0x0070dcd0 _$E230
// FUNCTION: STBC 0x0070dce0 _$E226
// FUNCTION: STBC 0x0070dcf0 _$E229
// FUNCTION: STBC 0x0070dd00 _$E227

// DATA: STBC 0x0099b988
NiTStringMap<TGSoundRegion *> TGSoundRegion::regions;

/*
================
TGSoundRegion::TGSoundRegion

The load constructor: ReadFromStream fills the members.
================
*/
// FUNCTION: STBC 0x0070dd20 ??0TGSoundRegion@@QAE@J@Z
TGSoundRegion::TGSoundRegion( long id ) : TGObject( id ), sounds( 2 ) {
	filter = SOUND_FILTER_NONE;
	name = NULL;
}

/*
================
TGSoundRegion::TGSoundRegion

A region of the name replaces the one the map holds under it.
================
*/
// FUNCTION: STBC 0x0070dd90 ??0TGSoundRegion@@QAE@PAD@Z
TGSoundRegion::TGSoundRegion( char *name_ ) :
	position( NiPoint3::ZERO ),
	orientation( NiMatrix3::IDENTITY ),
	sounds( 2 ) {
	TGSoundRegion *old;
	if ( regions.GetAt( name_, old ) && old != NULL ) {
		delete old;
	}

	filter = SOUND_FILTER_NONE;
	name = new char[strlen( name_ ) + 1];
	strcpy( name, name_ );
	regions.SetAt( name_, this );
}

/*
================
TGSoundRegion::~TGSoundRegion

The sounds leave the region, the filter goes, and the region leaves the
map.
================
*/
// FUNCTION: STBC 0x0070df40
TGSoundRegion::~TGSoundRegion( void ) {
	while ( sounds.GetSize() != 0 ) {
		RemoveSound( sounds.GetHead() );
	}
	SetFilter( SOUND_FILTER_NONE );

	regions.RemoveAt( name );
	delete name;
	name = NULL;
}

/*
================
TGSoundRegion::GetRegion
================
*/
// FUNCTION: STBC 0x0070e0a0
TGSoundRegion *TGSoundRegion::GetRegion( char *name_ ) {
	if ( name_ == NULL ) {
		return NULL;
	}

	TGSoundRegion *region;
	if ( regions.GetAt( name_, region ) ) {
		return region;
	}
	return NULL;
}

/*
================
TGSoundRegion::AddSound

The sound leaves its old region first; the filter then applies to it.
================
*/
// FUNCTION: STBC 0x0070e0f0
bool TGSoundRegion::AddSound( TGSound *sound ) {
	if ( sound == NULL ) {
		return false;
	}

	if ( sound->region != this ) {
		if ( sound->region != NULL ) {
			sound->region->RemoveSound( sound );
		}
		sound->region = this;
		sounds.AddTail( sound );
		ApplyFilter( sound );
	}
	return true;
}

/*
================
TGSoundRegion::RemoveSound

False when the sound is not in this region.
================
*/
// FUNCTION: STBC 0x0070e180
bool TGSoundRegion::RemoveSound( TGSound *sound ) {
	if ( sound == NULL || sound->region != this ) {
		return false;
	}

	NiTListIterator pos = sounds.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator itemPos = pos;
		if ( sounds.GetNext( pos ) == sound ) {
			sounds.RemovePos( itemPos );
			RemoveFilter( sound );
			sound->region = NULL;
			return true;
		}
	}
	return false;
}

/*
================
TGSoundRegion::SetFilter

The old filter comes off every sound before the new one applies.
================
*/
// FUNCTION: STBC 0x0070e260
bool TGSoundRegion::SetFilter( int filter_ ) {
	NiTListIterator pos = sounds.GetHeadPos();
	while ( pos != NULL ) {
		RemoveFilter( sounds.GetNext( pos ) );
	}

	filter = filter_;

	pos = sounds.GetHeadPos();
	while ( pos != NULL ) {
		ApplyFilter( sounds.GetNext( pos ) );
	}
	return true;
}

/*
================
TGSoundRegion::SetTransform
================
*/
// FUNCTION: STBC 0x0070e2b0
void TGSoundRegion::SetTransform( const NiPoint3 &position_, const NiPoint3 &forward_, const NiPoint3 &up_ ) {
	position = position_;

	NiPoint3 forward = forward_;
	NiPoint3 up = up_;
	NiPoint3 right = forward.Cross( up );
	orientation = NiMatrix3( forward, up, right ).Transpose();
}

/*
================
TGSoundRegion::WriteToStream
================
*/
// FUNCTION: STBC 0x0070e380
bool TGSoundRegion::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot17( (long)this );
	stream->VirtualSlot11( name );
	stream->WriteLong( filter );
	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	stream->WriteFloat( orientation.GetEntry( 0, 0 ) );
	stream->WriteFloat( orientation.GetEntry( 0, 1 ) );
	stream->WriteFloat( orientation.GetEntry( 0, 2 ) );
	stream->WriteFloat( orientation.GetEntry( 1, 0 ) );
	stream->WriteFloat( orientation.GetEntry( 1, 1 ) );
	stream->WriteFloat( orientation.GetEntry( 1, 2 ) );
	stream->WriteFloat( orientation.GetEntry( 2, 0 ) );
	stream->WriteFloat( orientation.GetEntry( 2, 1 ) );
	stream->WriteFloat( orientation.GetEntry( 2, 2 ) );
	return true;
}

/*
================
TGSoundRegion::ReadFromStream

The region files itself under its saved address and its name.
================
*/
// FUNCTION: STBC 0x0070e460
bool TGSoundRegion::ReadFromStream( TGStream *stream ) {
	pointerMap.Add( stream->VirtualSlot16(), this );

	name = stream->VirtualSlot10( -1 );
	regions.SetAt( name, this );

	filter = stream->ReadLong();
	position.x = stream->ReadFloat();
	position.y = stream->ReadFloat();
	position.z = stream->ReadFloat();
	orientation.SetEntry( 0, 0, stream->ReadFloat() );
	orientation.SetEntry( 0, 1, stream->ReadFloat() );
	orientation.SetEntry( 0, 2, stream->ReadFloat() );
	orientation.SetEntry( 1, 0, stream->ReadFloat() );
	orientation.SetEntry( 1, 1, stream->ReadFloat() );
	orientation.SetEntry( 1, 2, stream->ReadFloat() );
	orientation.SetEntry( 2, 0, stream->ReadFloat() );
	orientation.SetEntry( 2, 1, stream->ReadFloat() );
	orientation.SetEntry( 2, 2, stream->ReadFloat() );
	return true;
}

/*
================
TGSoundRegion::FixUpPointers
================
*/
// FUNCTION: STBC 0x0070e5c0
bool TGSoundRegion::FixUpPointers( void ) {
	return true;
}

/*
================
TGSoundRegion::PostFixUp
================
*/
// FUNCTION: STBC 0x0070e5d0
void TGSoundRegion::PostFixUp( void ) {
}

/*
================
TGSoundRegion::SaveRegions
================
*/
// FUNCTION: STBC 0x0070e5e0
bool TGSoundRegion::SaveRegions( TGStream *stream ) {
	char text[256];

	NiTMapIterator pos = regions.GetFirstPos();
	while ( pos != NULL ) {
		const char *regionName;
		TGSoundRegion *region;
		regions.GetNext( pos, regionName, region );
		if ( region != NULL ) {
			sprintf( text, "Saving TGSoundRegion %s\n", regionName );
			TGObject::Log( text );
			stream->WriteBool( true );
			if ( !region->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
TGSoundRegion::LoadRegions
================
*/
// FUNCTION: STBC 0x0070e6d0
bool TGSoundRegion::LoadRegions( TGStream *stream ) {
	while ( stream->ReadBool() == true ) {
		TGObject::LoadObject( stream, 0 );
	}
	return true;
}

/*
================
TGSoundRegion::DeleteAllRegions

Each region leaves the map as it is deleted.
================
*/
// FUNCTION: STBC 0x0070e700
void TGSoundRegion::DeleteAllRegions( void ) {
	NiTMapIterator pos = regions.GetFirstPos();
	while ( pos != NULL ) {
		const char *regionName;
		TGSoundRegion *region;
		regions.GetNext( pos, regionName, region );
		delete region;
	}
}

/*
================
TGSoundRegion::ApplyFilter

False for a filter the region does not know.
================
*/
// FUNCTION: STBC 0x0070e790
bool TGSoundRegion::ApplyFilter( TGSound *sound ) {
	switch ( filter ) {
		case SOUND_FILTER_NONE:
			return true;
		case SOUND_FILTER_SILENCE:
			sound->Unknown70CE60( true );
			return true;
		case SOUND_FILTER_2:
			return true;
	}
	return false;
}

/*
================
TGSoundRegion::RemoveFilter
================
*/
// FUNCTION: STBC 0x0070e7c0
void TGSoundRegion::RemoveFilter( TGSound *sound ) {
	switch ( filter ) {
		case SOUND_FILTER_SILENCE:
			sound->Unknown70CE60( false );
			break;
	}
}
