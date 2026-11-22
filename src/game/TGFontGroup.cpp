/*
===============================================================================

	TGFontGroup.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGFontGroup.h"
#include "TGFontManager.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>),
// each initialiser merged with its atexit registration (this unit is built
// with /Ob2: config/project.json).
// FUNCTION: STBC 0x00743db0 _$E32
// FUNCTION: STBC 0x00743dd0 _$E30
// FUNCTION: STBC 0x00743de0 _$E38
// FUNCTION: STBC 0x00743e00 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its
// exit-time registration, merged into one function.
// FUNCTION: STBC 0x00744670 _$E430

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00743e10 _$E45
// FUNCTION: STBC 0x00743e20 _$E101
// FUNCTION: STBC 0x00743e30 _$E56
// FUNCTION: STBC 0x00743e40 _$E60
// FUNCTION: STBC 0x00743e50 _$E64
// FUNCTION: STBC 0x00743e60 _$E68
// FUNCTION: STBC 0x00743e70 _$E200
// FUNCTION: STBC 0x00743e80 _$E75
// FUNCTION: STBC 0x00743e90 _$E88
// FUNCTION: STBC 0x00743ea0 _$E205
// FUNCTION: STBC 0x00743eb0 _$E210
// FUNCTION: STBC 0x00743ec0 _$E214
// FUNCTION: STBC 0x00743ed0 _$E218
// FUNCTION: STBC 0x00743ee0 _$E225
// FUNCTION: STBC 0x00743ef0 _$E267
// FUNCTION: STBC 0x00743f00 _$E271
// FUNCTION: STBC 0x00743f10 _$E125
// FUNCTION: STBC 0x00743f20 _$E253
// FUNCTION: STBC 0x00743f30 _$E260
// FUNCTION: STBC 0x00743f40 _$E283
// FUNCTION: STBC 0x00743f50 _$E287
// FUNCTION: STBC 0x00743f60 _$E327
// FUNCTION: STBC 0x00743f70 _$E333
// FUNCTION: STBC 0x00743f80 _$E275
// FUNCTION: STBC 0x00743f90 _$E279
// FUNCTION: STBC 0x00743fa0 _$E291
// FUNCTION: STBC 0x00743fb0 _$E295
// FUNCTION: STBC 0x00743fc0 _$E299
// FUNCTION: STBC 0x00743fd0 _$E303
// FUNCTION: STBC 0x00743fe0 _$E307
// FUNCTION: STBC 0x00743ff0 _$E311
// FUNCTION: STBC 0x00744000 _$E315
// FUNCTION: STBC 0x00744010 _$E319
// FUNCTION: STBC 0x00744020 _$E130
// FUNCTION: STBC 0x00744030 _$E323
// FUNCTION: STBC 0x00744040 _$E135
// FUNCTION: STBC 0x00744050 _$E337
// FUNCTION: STBC 0x00744060 _$E341
// FUNCTION: STBC 0x00744070 _$E345
// FUNCTION: STBC 0x00744080 _$E349
// FUNCTION: STBC 0x00744090 _$E144
// FUNCTION: STBC 0x007440a0 _$E353
// FUNCTION: STBC 0x007440b0 _$E357
// FUNCTION: STBC 0x007440c0 _$E361
// FUNCTION: STBC 0x007440d0 _$E150
// FUNCTION: STBC 0x007440e0 _$E155
// FUNCTION: STBC 0x007440f0 _$E165
// FUNCTION: STBC 0x00744100 _$E365
// FUNCTION: STBC 0x00744110 _$E170
// FUNCTION: STBC 0x00744120 _$E369
// FUNCTION: STBC 0x00744130 _$E373
// FUNCTION: STBC 0x00744140 _$E175
// FUNCTION: STBC 0x00744150 _$E377
// FUNCTION: STBC 0x00744160 _$E160
// FUNCTION: STBC 0x00744170 _$E381
// FUNCTION: STBC 0x00744180 _$E385
// FUNCTION: STBC 0x00744190 _$E93
// FUNCTION: STBC 0x007441a0 _$E180
// FUNCTION: STBC 0x007441b0 _$E389
// FUNCTION: STBC 0x007441c0 _$E393
// FUNCTION: STBC 0x007441d0 _$E397
// FUNCTION: STBC 0x007441e0 _$E401
// FUNCTION: STBC 0x007441f0 _$E405
// FUNCTION: STBC 0x00744200 _$E409
// FUNCTION: STBC 0x00744210 _$E413
// FUNCTION: STBC 0x00744220 _$E417
// FUNCTION: STBC 0x00744230 _$E185
// FUNCTION: STBC 0x00744240 _$E421
// FUNCTION: STBC 0x00744250 _$E190
// FUNCTION: STBC 0x00744260 _$E195

/*
================
TGFontGroup::TGFontGroup

Keeps a copy of the font's name and names the group "<name><size>".
================
*/
// FUNCTION: STBC 0x00744270
TGFontGroup::TGFontGroup( char *fontName_, float fontSize_ ) : TGIconGroup( NULL ) {
	if ( fontName_ != NULL ) {
		fontName = new char[strlen( fontName_ ) + 1];
		strcpy( fontName, fontName_ );
	} else {
		fontName = NULL;
	}
	fontSize = fontSize_;

	char groupName[128];
	sprintf( groupName, "%s%.2f", fontName_, fontSize_ );
	SetName( groupName );

	tabWidth = 0.0f;
	spaceWidth = 0.0f;
	horizontalSpacing = 0.0f;
	verticalSpacing = 0.0f;
	maxHeight = 0.0f;
}

/*
================
TGFontGroup::~TGFontGroup

Leaves its font's family and frees the copy of the font's name.
================
*/
// FUNCTION: STBC 0x00744360
TGFontGroup::~TGFontGroup( void ) {
	g_kFontManager.RemoveGroup( this );
	if ( fontName != NULL ) {
		delete[] fontName;
	}
}

/*
================
TGFontGroup::SetIconLocation

Keeps maxHeight at the tallest character's height.
================
*/
// FUNCTION: STBC 0x007443d0
void TGFontGroup::SetIconLocation( int iconNum, long textureNum, float x, float y, float width, float height, int rotation, int mirror ) {
	if ( height > maxHeight ) {
		maxHeight = height;
	}
	TGIconGroup::SetIconLocation( iconNum, textureNum, x, y, width, height, rotation, mirror );
}

/*
================
TGFontGroup::GetPyWrapper
================
*/
// FUNCTION: STBC 0x00744420
_object *TGFontGroup::GetPyWrapper( void ) {
	return PyEmbed::Unknown6F9920( this, "TGFontGroup" );
}

/*
================
TGFontGroup::Unknown744430

Gives group this group's icons and textures, its font name and the given
size, with the metrics and every icon's pixel size scaled to that size.
================
*/
// FUNCTION: STBC 0x00744430
void TGFontGroup::Unknown744430( TGFontGroup *group, float size ) {
	Unknown72C990( group );
	float scale = size / fontSize;
	group->fontName = new char[strlen( fontName ) + 1];
	strcpy( group->fontName, fontName );
	group->fontSize = size;
	group->tabWidth = scale * tabWidth;
	group->spaceWidth = scale * spaceWidth;
	group->horizontalSpacing = scale * horizontalSpacing;
	group->verticalSpacing = scale * verticalSpacing;
	group->maxHeight = scale * maxHeight;

	NiTMapIterator pos = group->icons.GetFirstPos();
	while ( pos ) {
		int iconNum;
		TGIconLocation *location;
		group->icons.GetNext( pos, iconNum, location );
		location->pixelHeight = scale * location->pixelHeight;
		location->pixelWidth = scale * location->pixelWidth;
	}
}

/*
================
TGFontGroup::MapChar

The character's own icon number when the group has an icon for it; else 0.
================
*/
// FUNCTION: STBC 0x00744540
int TGFontGroup::MapChar( int c ) {
	TGIconLocation *location;
	if ( icons.GetAt( c, location ) ) {
		return c;
	}
	return 0;
}

/*
================
TGFontGroup::WriteToStream

The icon group, then the font's name, size and spacings.
================
*/
// FUNCTION: STBC 0x00744590
bool TGFontGroup::WriteToStream( TGStream *stream ) {
	if ( !TGIconGroup::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( fontName );
	stream->WriteFloat( fontSize );
	stream->WriteFloat( tabWidth );
	stream->WriteFloat( spaceWidth );
	stream->WriteFloat( horizontalSpacing );
	stream->WriteFloat( verticalSpacing );
	stream->WriteFloat( maxHeight );
	return true;
}

/*
================
TGFontGroup::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x00744600
bool TGFontGroup::ReadFromStream( TGStream *stream ) {
	if ( !TGIconGroup::ReadFromStream( stream ) ) {
		return false;
	}
	fontName = stream->VirtualSlot10( -1 );
	fontSize = stream->ReadFloat();
	tabWidth = stream->ReadFloat();
	spaceWidth = stream->ReadFloat();
	horizontalSpacing = stream->ReadFloat();
	verticalSpacing = stream->ReadFloat();
	maxHeight = stream->ReadFloat();
	return true;
}
