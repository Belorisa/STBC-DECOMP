/*
===============================================================================

	TGStringToStringMap.cpp

===============================================================================
*/

#include <iostream>

#include "TGStringToStringMap.h"
#include "TGStream.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>), and the locale-id guard.
// FUNCTION: STBC 0x0045ec70 _$E32
// FUNCTION: STBC 0x0045ec80 _$E29
// FUNCTION: STBC 0x0045ec90 _$E31
// FUNCTION: STBC 0x0045eca0 _$E30
// FUNCTION: STBC 0x0045ecb0 _$E38
// FUNCTION: STBC 0x0045ecc0 _$E35
// FUNCTION: STBC 0x0045ecd0 _$E37
// FUNCTION: STBC 0x0045ece0 _$E36
// FUNCTION: STBC 0x0045f090 _$E421
// FUNCTION: STBC 0x0045f0b0 _$E420

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0045ecf0 _$E45
// FUNCTION: STBC 0x0045ed00 _$E44
// FUNCTION: STBC 0x0045ed10 _$E49
// FUNCTION: STBC 0x0045ed20 _$E48
// FUNCTION: STBC 0x0045ed30 _$E60
// FUNCTION: STBC 0x0045ed40 _$E59
// FUNCTION: STBC 0x0045ed50 _$E64
// FUNCTION: STBC 0x0045ed60 _$E63
// FUNCTION: STBC 0x0045ed70 _$E68
// FUNCTION: STBC 0x0045ed80 _$E67
// FUNCTION: STBC 0x0045ed90 _$E72
// FUNCTION: STBC 0x0045eda0 _$E71
// FUNCTION: STBC 0x0045edb0 _$E196
// FUNCTION: STBC 0x0045edc0 _$E195
// FUNCTION: STBC 0x0045edd0 _$E138
// FUNCTION: STBC 0x0045ede0 _$E137
// FUNCTION: STBC 0x0045edf0 _$E151
// FUNCTION: STBC 0x0045ee00 _$E150
// FUNCTION: STBC 0x0045ee10 _$E201
// FUNCTION: STBC 0x0045ee20 _$E200
// FUNCTION: STBC 0x0045ee30 _$E206
// FUNCTION: STBC 0x0045ee40 _$E205
// FUNCTION: STBC 0x0045ee50 _$E210
// FUNCTION: STBC 0x0045ee60 _$E209
// FUNCTION: STBC 0x0045ee70 _$E214
// FUNCTION: STBC 0x0045ee80 _$E213
// FUNCTION: STBC 0x0045ee90 _$E221
// FUNCTION: STBC 0x0045eea0 _$E220
// FUNCTION: STBC 0x0045eeb0 _$E246
// FUNCTION: STBC 0x0045eec0 _$E245
// FUNCTION: STBC 0x0045eed0 _$E250
// FUNCTION: STBC 0x0045eee0 _$E249
// FUNCTION: STBC 0x0045eef0 _$E96
// FUNCTION: STBC 0x0045ef00 _$E95
// FUNCTION: STBC 0x0045ef10 _$E232
// FUNCTION: STBC 0x0045ef20 _$E231
// FUNCTION: STBC 0x0045ef30 _$E239
// FUNCTION: STBC 0x0045ef40 _$E238
// FUNCTION: STBC 0x0045ef50 _$E262
// FUNCTION: STBC 0x0045ef60 _$E261
// FUNCTION: STBC 0x0045ef70 _$E266
// FUNCTION: STBC 0x0045ef80 _$E265
// FUNCTION: STBC 0x0045ef90 _$E306
// FUNCTION: STBC 0x0045efa0 _$E305
// FUNCTION: STBC 0x0045efb0 _$E312
// FUNCTION: STBC 0x0045efc0 _$E311
// FUNCTION: STBC 0x0045efd0 _$E254
// FUNCTION: STBC 0x0045efe0 _$E253
// FUNCTION: STBC 0x0045eff0 _$E258
// FUNCTION: STBC 0x0045f000 _$E257
// FUNCTION: STBC 0x0045f010 _$E270
// FUNCTION: STBC 0x0045f020 _$E269
// FUNCTION: STBC 0x0045f030 _$E274
// FUNCTION: STBC 0x0045f040 _$E273
// FUNCTION: STBC 0x0045f050 _$E278
// FUNCTION: STBC 0x0045f060 _$E277
// FUNCTION: STBC 0x0045f070 _$E282
// FUNCTION: STBC 0x0045f080 _$E281
// FUNCTION: STBC 0x0045f0c0 _$E286
// FUNCTION: STBC 0x0045f0d0 _$E285
// FUNCTION: STBC 0x0045f0e0 _$E290
// FUNCTION: STBC 0x0045f0f0 _$E289
// FUNCTION: STBC 0x0045f100 _$E294
// FUNCTION: STBC 0x0045f110 _$E293
// FUNCTION: STBC 0x0045f120 _$E298
// FUNCTION: STBC 0x0045f130 _$E297
// FUNCTION: STBC 0x0045f140 _$E101
// FUNCTION: STBC 0x0045f150 _$E100
// FUNCTION: STBC 0x0045f160 _$E302
// FUNCTION: STBC 0x0045f170 _$E301
// FUNCTION: STBC 0x0045f180 _$E106
// FUNCTION: STBC 0x0045f190 _$E105
// FUNCTION: STBC 0x0045f1a0 _$E316
// FUNCTION: STBC 0x0045f1b0 _$E315
// FUNCTION: STBC 0x0045f1c0 _$E320
// FUNCTION: STBC 0x0045f1d0 _$E319
// FUNCTION: STBC 0x0045f1e0 _$E324
// FUNCTION: STBC 0x0045f1f0 _$E323
// FUNCTION: STBC 0x0045f200 _$E328
// FUNCTION: STBC 0x0045f210 _$E327
// FUNCTION: STBC 0x0045f220 _$E115
// FUNCTION: STBC 0x0045f230 _$E114
// FUNCTION: STBC 0x0045f240 _$E332
// FUNCTION: STBC 0x0045f250 _$E331
// FUNCTION: STBC 0x0045f260 _$E336
// FUNCTION: STBC 0x0045f270 _$E335
// FUNCTION: STBC 0x0045f280 _$E340
// FUNCTION: STBC 0x0045f290 _$E339
// FUNCTION: STBC 0x0045f2a0 _$E121
// FUNCTION: STBC 0x0045f2b0 _$E120
// FUNCTION: STBC 0x0045f2c0 _$E126
// FUNCTION: STBC 0x0045f2d0 _$E125
// FUNCTION: STBC 0x0045f2e0 _$E156
// FUNCTION: STBC 0x0045f2f0 _$E155
// FUNCTION: STBC 0x0045f300 _$E344
// FUNCTION: STBC 0x0045f310 _$E343
// FUNCTION: STBC 0x0045f320 _$E161
// FUNCTION: STBC 0x0045f330 _$E160
// FUNCTION: STBC 0x0045f340 _$E348
// FUNCTION: STBC 0x0045f350 _$E347
// FUNCTION: STBC 0x0045f360 _$E352
// FUNCTION: STBC 0x0045f370 _$E351
// FUNCTION: STBC 0x0045f380 _$E166
// FUNCTION: STBC 0x0045f390 _$E165
// FUNCTION: STBC 0x0045f3a0 _$E356
// FUNCTION: STBC 0x0045f3b0 _$E355
// FUNCTION: STBC 0x0045f3c0 _$E131
// FUNCTION: STBC 0x0045f3d0 _$E130
// FUNCTION: STBC 0x0045f3e0 _$E360
// FUNCTION: STBC 0x0045f3f0 _$E359
// FUNCTION: STBC 0x0045f400 _$E364
// FUNCTION: STBC 0x0045f410 _$E363
// FUNCTION: STBC 0x0045f420 _$E171
// FUNCTION: STBC 0x0045f430 _$E170
// FUNCTION: STBC 0x0045f440 _$E176
// FUNCTION: STBC 0x0045f450 _$E175
// FUNCTION: STBC 0x0045f460 _$E368
// FUNCTION: STBC 0x0045f470 _$E367
// FUNCTION: STBC 0x0045f480 _$E372
// FUNCTION: STBC 0x0045f490 _$E371
// FUNCTION: STBC 0x0045f4a0 _$E376
// FUNCTION: STBC 0x0045f4b0 _$E375
// FUNCTION: STBC 0x0045f4c0 _$E380
// FUNCTION: STBC 0x0045f4d0 _$E379
// FUNCTION: STBC 0x0045f4e0 _$E384
// FUNCTION: STBC 0x0045f4f0 _$E383
// FUNCTION: STBC 0x0045f500 _$E388
// FUNCTION: STBC 0x0045f510 _$E387
// FUNCTION: STBC 0x0045f520 _$E392
// FUNCTION: STBC 0x0045f530 _$E391
// FUNCTION: STBC 0x0045f540 _$E396
// FUNCTION: STBC 0x0045f550 _$E395
// FUNCTION: STBC 0x0045f560 _$E181
// FUNCTION: STBC 0x0045f570 _$E180
// FUNCTION: STBC 0x0045f580 _$E400
// FUNCTION: STBC 0x0045f590 _$E399
// FUNCTION: STBC 0x0045f5a0 _$E186
// FUNCTION: STBC 0x0045f5b0 _$E185
// FUNCTION: STBC 0x0045f5c0 _$E191
// FUNCTION: STBC 0x0045f5d0 _$E190
// FUNCTION: STBC 0x0045f5e0 _$E404
// FUNCTION: STBC 0x0045f5f0 _$E403
// FUNCTION: STBC 0x0045f600 _$E409
// FUNCTION: STBC 0x0045f610 _$E408
// FUNCTION: STBC 0x0045f620 _$E413
// FUNCTION: STBC 0x0045f630 _$E412

/*
================
TGStringToStringMap::TGStringToStringMap

An empty map.
================
*/
// FUNCTION: STBC 0x0045f640
TGStringToStringMap::TGStringToStringMap( void ) {
	mappings = NULL;
	names = NULL;
	dests = NULL;
	namesSize = 0;
	destsSize = 0;
	numMappings = 0;
}

/*
================
TGStringToStringMap::~TGStringToStringMap
================
*/
// FUNCTION: STBC 0x0045f660
TGStringToStringMap::~TGStringToStringMap( void ) {
	DeleteMappings();
}

/*
================
TGStringToStringMap::AddMapping

Adds a mapping, keeping the mappings sorted by name; the strings are
copied.
================
*/
// FUNCTION: STBC 0x0045f670
void TGStringToStringMap::AddMapping( char *name, char *dest ) {
	mappings = (Mapping *)realloc( mappings, ( numMappings + 1 ) * sizeof( Mapping ) );

	unsigned int i;
	for ( i = 0; i < numMappings; i++ ) {
		if ( strcmp( names + mappings[i].nameOffset, name ) > 0 ) {
			memmove( &mappings[i + 1], &mappings[i], ( numMappings - i ) * sizeof( Mapping ) );
			break;
		}
	}

	size_t nameLength = strlen( name ) + 1;
	size_t destLength = strlen( dest ) + 1;
	names = (char *)realloc( names, namesSize + nameLength );
	dests = (char *)realloc( dests, destsSize + destLength );
	strcpy( names + namesSize, name );
	strcpy( dests + destsSize, dest );
	mappings[i].nameOffset = namesSize;
	mappings[i].destOffset = destsSize;
	namesSize += nameLength;
	destsSize += destLength;
	numMappings++;
}

/*
================
TGStringToStringMap::GetDest

What the name maps to; NULL for a name that isn't mapped.
================
*/
// FUNCTION: STBC 0x0045f810
char *TGStringToStringMap::GetDest( char *name ) {
	int high = numMappings - 1;
	int low = 0;
	while ( high >= low ) {
		int middle = ( low + high ) >> 1;
		int order = strcmp( names + mappings[middle].nameOffset, name );
		if ( order == 0 ) {
			return dests + mappings[middle].destOffset;
		}
		if ( order < 0 ) {
			low = middle + 1;
		} else {
			high = middle - 1;
		}
	}
	return NULL;
}

/*
================
TGStringToStringMap::DeleteMappings
================
*/
// FUNCTION: STBC 0x0045f8b0
void TGStringToStringMap::DeleteMappings( void ) {
	if ( mappings != NULL ) {
		free( mappings );
		mappings = NULL;
	}
	if ( names != NULL ) {
		free( names );
		names = NULL;
	}
	if ( dests != NULL ) {
		free( dests );
		dests = NULL;
	}
	namesSize = 0;
	destsSize = 0;
	numMappings = 0;
}

/*
================
TGStringToStringMap::WriteToStream

The two string pools, then the mappings' offsets.
================
*/
// FUNCTION: STBC 0x0045f900
bool TGStringToStringMap::WriteToStream( TGStream *stream ) {
	stream->WriteLong( namesSize );
	if ( namesSize > 0 ) {
		stream->Write( names, namesSize );
	}
	stream->WriteLong( destsSize );
	if ( destsSize > 0 ) {
		stream->Write( dests, destsSize );
	}
	stream->WriteLong( numMappings );
	for ( unsigned int i = 0; i < numMappings; i++ ) {
		stream->WriteLong( mappings[i].nameOffset );
		stream->WriteLong( mappings[i].destOffset );
	}
	return true;
}

/*
================
TGStringToStringMap::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x0045f990
bool TGStringToStringMap::ReadFromStream( TGStream *stream ) {
	unsigned int i;

	names = NULL;
	namesSize = stream->ReadLong();
	if ( namesSize > 0 ) {
		names = (char *)malloc( namesSize );
		stream->Read( names, namesSize );
	}
	dests = NULL;
	destsSize = stream->ReadLong();
	if ( destsSize > 0 ) {
		dests = (char *)malloc( destsSize );
		stream->Read( dests, destsSize );
	}
	numMappings = stream->ReadLong();
	mappings = (Mapping *)malloc( numMappings * sizeof( Mapping ) );
	for ( i = 0; i < numMappings; i++ ) {
		mappings[i].nameOffset = stream->ReadLong();
		mappings[i].destOffset = stream->ReadLong();
	}
	return true;
}
