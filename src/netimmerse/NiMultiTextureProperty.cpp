// NiMultiTextureProperty.cpp -- the extra texture maps of a surface

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiMultiTextureProperty.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00803670 _$E112
// FUNCTION: STBC 0x00803410 _$E17
// FUNCTION: STBC 0x00803430 _$E15
// FUNCTION: STBC 0x00803440 _$E23
// FUNCTION: STBC 0x00803460 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00803690 _$E30
// FUNCTION: STBC 0x008036a0 _$E70
// FUNCTION: STBC 0x008036b0 _$E83
// FUNCTION: STBC 0x008036c0 _$E41
// FUNCTION: STBC 0x008036d0 _$E45
// FUNCTION: STBC 0x008036e0 _$E49
// FUNCTION: STBC 0x008036f0 _$E53
// FUNCTION: STBC 0x00803700 _$E58
// FUNCTION: STBC 0x00803740 _$E88

// FUNCTION: STBC 0x00801cf0 _$E94
// FUNCTION: STBC 0x00801d10 _$E91
// DATA: STBC 0x9A2268 ?rtti@NiMultiTextureProperty@@2VNiRTTI@@B
const NiRTTI NiMultiTextureProperty::rtti( "NiMultiTextureProperty", &NiProperty::rtti );

/*
================
NiMultiTextureProperty::Register
================
*/
// FUNCTION: STBC 0x801D30 ?Register@NiMultiTextureProperty@@SAHXZ
NiImplementRegister( NiMultiTextureProperty )

// FUNCTION: STBC 0x00801e80 _$E100
// FUNCTION: STBC 0x00801ea0 _$E97
// DATA: STBC 0x9A2260
NiPointer<NiMultiTextureProperty> NiMultiTextureProperty::defaultProperty;

/*
===============================================================================

	NiMultiTexturePropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiMultiTexturePropertyInit {
public:
	// FUNCTION: STBC 0x801F00 ??0NiMultiTexturePropertyInit@@QAE@XZ
	NiMultiTexturePropertyInit() { NiMultiTextureProperty::defaultProperty = new NiMultiTextureProperty; }
	~NiMultiTexturePropertyInit() { NiMultiTextureProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00801ee0 _$E105
// FUNCTION: STBC 0x00802050 _$E103
static NiMultiTexturePropertyInit init;

/*
================
NiMultiTextureProperty::NiMultiTextureProperty

Starts with empty slots for the dark, detail, gloss, glow, glow alpha and
first decal maps.
================
*/
// FUNCTION: STBC 0x802080
// DRAFT: inline budget: retail calls SetSize out of line at all six sites and expands SetAt only for the glow and glow alpha slots; this build expands SetAt at the glow slot and SetSize at the decal slot
NiMultiTextureProperty::NiMultiTextureProperty() : maps( 1, 1 ) {
	maps.SetAtGrow( DARK_INDEX, NULL );
	maps.SetAtGrow( DETAIL_INDEX, NULL );
	maps.SetAtGrow( GLOSS_INDEX, NULL );
	maps.SetAtGrow( GLOW_INDEX, NULL );
	maps.SetAtGrow( GLOW_ALPHA_INDEX, NULL );
	maps.SetAtGrow( DECAL_INDEX, NULL );
	unknown30 = 0;
}

/*
================
NiMultiTextureProperty::~NiMultiTextureProperty

Deletes the maps, which release their images.
================
*/
// FUNCTION: STBC 0x8021F0
NiMultiTextureProperty::~NiMultiTextureProperty() {
	for ( unsigned int i = 0; i < maps.GetSize(); i++ ) {
		delete maps.GetAt( i );
	}
}

/*
================
NiMultiTextureProperty::CreateClone
================
*/
// FUNCTION: STBC 0x8022A0 ?CreateClone@NiMultiTextureProperty@@UAEPAVNiObject@@XZ
// PARKED: declaration-sensitive (config/declaration_sensitive.txt): exact or a register/order shift with the NetImmerse declarations it sees; NiRawImageData.h's three static NiPixelDescs flip it (user's ruling 2026-11-22, card group hdr-nirawimagedata-descs)
NiObject *NiMultiTextureProperty::CreateClone() {
	NiMultiTextureProperty *clone = new NiMultiTextureProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiMultiTextureProperty::CopyMembers

The clone gets a copy of each map, sharing its image.
================
*/
// FUNCTION: STBC 0x8024A0
void NiMultiTextureProperty::CopyMembers( NiMultiTextureProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->unknown30 = unknown30;
	unsigned int thisCount = maps.GetSize();	// (name invented; fits the /Od frame order)
	for ( unsigned int i = 0; i < thisCount; i++ ) {
		if ( maps.GetAt( i ) ) {
			dest->maps.SetAtGrow( i, new Map( *maps.GetAt( i ) ) );
		}
	}
}

/*
================
NiMultiTextureProperty::CreateObject

Creates the property and loads it, with a record for the maps' image link
IDs.
================
*/
// FUNCTION: STBC 0x802630
void NiMultiTextureProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMultiTextureProperty *property = new NiMultiTextureProperty;
	stream.SetLoadRecord( new NiMultiTexturePropertyLoadRecord );
	property->LoadBinary( stream );
	object = property;
}

/*
================
NiMultiTextureProperty::Map::LoadBinary

The image's link ID, the modes and the texture set; "m_sL" and "m_sK" and
the two bytes from "3.04" on.
================
*/
// FUNCTION: STBC 0x802810
void NiMultiTextureProperty::Map::LoadBinary( NiStream &stream, unsigned int index ) {
	NiImage *imageLinkID = 0;
	NiStreamLoadBinary( stream, imageLinkID );
	NiStreamLoadEnum( stream, clamp );
	NiStreamLoadEnum( stream, filter );
	NiStreamLoadBinary( stream, texCoord );
	if ( strcmp( stream.GetFileVersion(), "3.04" ) >= 0 ) {
		NiStreamLoadBinary( stream, l );
		NiStreamLoadBinary( stream, k );
		NiStreamLoadBinary( stream, unknown14 );
		NiStreamLoadBinary( stream, unknown15 );
	}

	NiMultiTexturePropertyLoadRecord *record = (NiMultiTexturePropertyLoadRecord *)stream.GetLoadRecord();
	record->imageLinkIDs.SetAtGrow( index, imageLinkID );
}

/*
================
NiMultiTextureProperty::LoadBinary

The map count, then for each map but the glow alpha map a "has a map" flag
and the map. Only a file of exactly five maps still holds the glow alpha
map. The record starts with no link ID for the first five maps.
================
*/
// FUNCTION: STBC 0x8029D0 ?LoadBinary@NiMultiTextureProperty@@UAEXAAVNiStream@@@Z
void NiMultiTextureProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );

	NiMultiTexturePropertyLoadRecord *record = (NiMultiTexturePropertyLoadRecord *)stream.GetLoadRecord();
	record->imageLinkIDs.SetAtGrow( DARK_INDEX, NULL );
	record->imageLinkIDs.SetAtGrow( DETAIL_INDEX, NULL );
	record->imageLinkIDs.SetAtGrow( GLOSS_INDEX, NULL );
	record->imageLinkIDs.SetAtGrow( GLOW_INDEX, NULL );
	record->imageLinkIDs.SetAtGrow( GLOW_ALPHA_INDEX, NULL );

	unsigned int numSaved;	// (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, numSaved );
	for ( unsigned int i = 0; i < numSaved; i++ ) {
		Map *map = 0;
		if ( i != GLOW_ALPHA_INDEX || numSaved == DECAL_INDEX ) {
			NiStreamLoadBinary( stream, map );
			if ( map ) {
				map = new Map;
				assert( map );
				map->LoadBinary( stream, i );
			}
		}
		maps.SetAtGrow( i, map );
	}
}

/*
================
NiMultiTextureProperty::LinkObject

Gives each map its image.
================
*/
// FUNCTION: STBC 0x802C60 ?LinkObject@NiMultiTextureProperty@@UAEXAAVNiStream@@@Z
void NiMultiTextureProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );

	NiMultiTexturePropertyLoadRecord *record = (NiMultiTexturePropertyLoadRecord *)stream.GetLoadRecord();
	for ( unsigned int i = 0; i < record->imageLinkIDs.GetSize(); i++ ) {
		unsigned int linkID = (unsigned int)record->imageLinkIDs.GetAt( i );
		if ( linkID ) {
			assert( maps.GetAt( i ) );
			// BUG: LoadBinary records each link ID under its own map's index,
			// so the maps past the glow alpha map get the image of the map
			// before them
			if ( i < GLOW_ALPHA_INDEX ) {
				maps.GetAt( i )->SetImage( (NiImage *)stream.GetObjectFromLinkID( linkID ) );
			} else {
				maps.GetAt( i + 1 )->SetImage( (NiImage *)stream.GetObjectFromLinkID( linkID ) );
			}
		}
	}
}

/*
================
NiMultiTextureProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x802D10
bool NiMultiTextureProperty::RegisterStreamables( NiStream &stream ) {
	if ( !NiProperty::RegisterStreamables( stream ) ) {
		return false;
	}

	for ( unsigned int i = 0; i < maps.GetSize(); i++ ) {
		if ( i != GLOW_ALPHA_INDEX ) {
			Map *map = maps.GetAt( i );
			if ( map && map->image ) {
				map->image->RegisterStreamables( stream );
			}
		}
	}
	return true;
}

/*
================
NiMultiTextureProperty::Map::SaveBinary
================
*/
// FUNCTION: STBC 0x802D60
void NiMultiTextureProperty::Map::SaveBinary( NiStream &stream ) {
	NiStreamSaveBinary( stream, image );
	NiStreamSaveEnum( stream, clamp );
	NiStreamSaveEnum( stream, filter );
	NiStreamSaveBinary( stream, texCoord );
	NiStreamSaveBinary( stream, l );
	NiStreamSaveBinary( stream, k );
	NiStreamSaveBinary( stream, unknown14 );
	NiStreamSaveBinary( stream, unknown15 );
}

/*
================
NiMultiTextureProperty::SaveBinary

The map count, then each map but the glow alpha map: its address as a
"has a map" flag, then the map.
================
*/
// FUNCTION: STBC 0x802DE0
void NiMultiTextureProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );

	unsigned int numSaved = maps.GetSize();	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, numSaved );
	for ( unsigned int i = 0; i < numSaved; i++ ) {
		if ( i != GLOW_ALPHA_INDEX ) {
			Map *map = maps.GetAt( i );
			NiStreamSaveBinary( stream, map );
			if ( map ) {
				map->SaveBinary( stream );
			}
		}
	}
}

/*
================
NiMultiTextureProperty::IsEqual
================
*/
// FUNCTION: STBC 0x802ED0
bool NiMultiTextureProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiMultiTextureProperty *multi = (NiMultiTextureProperty *)object;
	unsigned int thisCount = maps.GetSize();	// (name invented; fits the /Od frame order)
	unsigned int otherCount = multi->maps.GetSize();	// (name invented; fits the /Od frame order)
	if ( thisCount != otherCount ) {
		return false;
	}
	for ( unsigned int i = 0; i < thisCount; i++ ) {
		Map *thisMap = maps.GetAt( i );	// (name invented; fits the /Od frame order)
		Map *otherMap = multi->maps.GetAt( i );	// (name invented; fits the /Od frame order)
		if ( thisMap && otherMap ) {
			if ( !thisMap->IsEqual( otherMap ) ) {
				return false;
			}
		} else if ( thisMap || otherMap ) {
			return false;
		}
	}
	return true;
}

/*
================
NiMultiTextureProperty::GetViewerStrings

Each map that is present under a title naming it by its index, then its
members.
Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x44607C): each title added in its own case, the map read through its
accessors.
================
*/
// FUNCTION: STBC 0x802F40 ?GetViewerStrings@NiMultiTextureProperty@@UAEXPAV?$NiTArray@PAD@@@Z
void NiMultiTextureProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );

	unsigned int count = maps.GetSize();
	for ( unsigned int i = 0; i < count; i++ ) {
		Map *map = maps.GetAt( i );
		if ( map ) {
			char buffer[64];
			// BUG: each title is the stack buffer itself, not a copy; the
			// strings' owner frees it, and the next sprintf overwrites it
			switch ( i ) {
			case DARK_INDEX:
				sprintf( buffer, "MultiTexture Dark Map %d", i );
				strings->Add( buffer );
				break;
			case DETAIL_INDEX:
				sprintf( buffer, "MultiTexture Detail Map %d", i );
				strings->Add( buffer );
				break;
			case GLOSS_INDEX:
				sprintf( buffer, "MultiTexture Gloss Map %d", i );
				strings->Add( buffer );
				break;
			case GLOW_INDEX:
				sprintf( buffer, "MultiTexture Glow Map %d", i );
				strings->Add( buffer );
				break;
			case GLOW_ALPHA_INDEX:
				sprintf( buffer, "MultiTexture Glow Alpha Map %d", i );
				strings->Add( buffer );
				break;
			default:
				sprintf( buffer, "MultiTexture Decal Map %d", i );
				strings->Add( buffer );
				break;
			}

			sprintf( buffer, "m_spImage " );
			strings->Add( NiGetViewerString( buffer, map->GetImage() ) );
			sprintf( buffer, "m_eClamp " );
			strings->Add( NiTextureModeProperty::GetViewerString( buffer, map->GetClampMode() ) );
			sprintf( buffer, "m_eFilter " );
			strings->Add( NiTextureModeProperty::GetViewerString( buffer, map->GetFilterMode() ) );
			sprintf( buffer, "m_uiTexCoord " );
			strings->Add( NiGetViewerString( buffer, map->GetTextureIndex() ) );
			strings->Add( NiGetViewerString( "m_sL", map->GetL() ) );
			strings->Add( NiGetViewerString( "m_sK", map->GetK() ) );
		}
	}
}

/*
================
NiMultiTextureProperty::Map::IsEqual

The images are equal when both or neither are set.
================
*/
// FUNCTION: STBC 0x803280
bool NiMultiTextureProperty::Map::IsEqual( Map *map ) {
	if ( ( image && !map->image ) || ( !image && map->image ) ) {
		return false;
	}
	if ( clamp != map->clamp || filter != map->filter || texCoord != map->texCoord
		|| l != map->l || k != map->k
		|| unknown14 != map->unknown14 || unknown15 != map->unknown15 ) {
		return false;
	}
	return true;
}
