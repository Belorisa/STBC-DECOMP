/*
===============================================================================

	TGMusicFadeEvent.cpp

===============================================================================
*/

#include <iostream>
#include "TGMusicFadeEvent.h"
#include "TGStream.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00715190 _$E32
// FUNCTION: STBC 0x007151a0 _$E29
// FUNCTION: STBC 0x007151b0 _$E31
// FUNCTION: STBC 0x007151c0 _$E30
// FUNCTION: STBC 0x007151d0 _$E38
// FUNCTION: STBC 0x007151e0 _$E35
// FUNCTION: STBC 0x007151f0 _$E37
// FUNCTION: STBC 0x00715200 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007155e0 _$E256
// FUNCTION: STBC 0x007155f0 _$E255
// FUNCTION: STBC 0x00715600 _$E260
// FUNCTION: STBC 0x00715610 _$E259
// FUNCTION: STBC 0x00715640 _$E242
// FUNCTION: STBC 0x00715650 _$E241
// FUNCTION: STBC 0x00715660 _$E249
// FUNCTION: STBC 0x00715670 _$E248
// FUNCTION: STBC 0x00715680 _$E272
// FUNCTION: STBC 0x00715690 _$E271
// FUNCTION: STBC 0x007156a0 _$E276
// FUNCTION: STBC 0x007156b0 _$E275
// FUNCTION: STBC 0x007156c0 _$E316
// FUNCTION: STBC 0x007156d0 _$E315
// FUNCTION: STBC 0x007156e0 _$E322
// FUNCTION: STBC 0x007156f0 _$E321
// FUNCTION: STBC 0x00715700 _$E264
// FUNCTION: STBC 0x00715710 _$E263
// FUNCTION: STBC 0x00715720 _$E268
// FUNCTION: STBC 0x00715730 _$E267
// FUNCTION: STBC 0x00715740 _$E280
// FUNCTION: STBC 0x00715750 _$E279
// FUNCTION: STBC 0x00715760 _$E284
// FUNCTION: STBC 0x00715770 _$E283
// FUNCTION: STBC 0x00715780 _$E288
// FUNCTION: STBC 0x00715790 _$E287
// FUNCTION: STBC 0x007157a0 _$E292
// FUNCTION: STBC 0x007157b0 _$E291
// FUNCTION: STBC 0x007157c0 _$E296
// FUNCTION: STBC 0x007157d0 _$E295
// FUNCTION: STBC 0x007157e0 _$E300
// FUNCTION: STBC 0x007157f0 _$E299
// FUNCTION: STBC 0x00715800 _$E304
// FUNCTION: STBC 0x00715810 _$E303
// FUNCTION: STBC 0x00715820 _$E308
// FUNCTION: STBC 0x00715830 _$E307
// FUNCTION: STBC 0x00715860 _$E312
// FUNCTION: STBC 0x00715870 _$E311
// FUNCTION: STBC 0x007158a0 _$E326
// FUNCTION: STBC 0x007158b0 _$E325
// FUNCTION: STBC 0x007158c0 _$E330
// FUNCTION: STBC 0x007158d0 _$E329
// FUNCTION: STBC 0x007158e0 _$E334
// FUNCTION: STBC 0x007158f0 _$E333
// FUNCTION: STBC 0x00715900 _$E338
// FUNCTION: STBC 0x00715910 _$E337
// FUNCTION: STBC 0x00715940 _$E342
// FUNCTION: STBC 0x00715950 _$E341
// FUNCTION: STBC 0x00715960 _$E346
// FUNCTION: STBC 0x00715970 _$E345
// FUNCTION: STBC 0x00715980 _$E350
// FUNCTION: STBC 0x00715990 _$E349
// FUNCTION: STBC 0x00715a00 _$E354
// FUNCTION: STBC 0x00715a10 _$E353
// FUNCTION: STBC 0x00715a40 _$E358
// FUNCTION: STBC 0x00715a50 _$E357
// FUNCTION: STBC 0x00715a60 _$E362
// FUNCTION: STBC 0x00715a70 _$E361
// FUNCTION: STBC 0x00715aa0 _$E366
// FUNCTION: STBC 0x00715ab0 _$E365
// FUNCTION: STBC 0x00715ae0 _$E370
// FUNCTION: STBC 0x00715af0 _$E369
// FUNCTION: STBC 0x00715b00 _$E374
// FUNCTION: STBC 0x00715b10 _$E373
// FUNCTION: STBC 0x00715b60 _$E378
// FUNCTION: STBC 0x00715b70 _$E377
// FUNCTION: STBC 0x00715b80 _$E382
// FUNCTION: STBC 0x00715b90 _$E381
// FUNCTION: STBC 0x00715ba0 _$E386
// FUNCTION: STBC 0x00715bb0 _$E385
// FUNCTION: STBC 0x00715bc0 _$E390
// FUNCTION: STBC 0x00715bd0 _$E389
// FUNCTION: STBC 0x00715be0 _$E394
// FUNCTION: STBC 0x00715bf0 _$E393
// FUNCTION: STBC 0x00715c00 _$E398
// FUNCTION: STBC 0x00715c10 _$E397
// FUNCTION: STBC 0x00715c20 _$E402
// FUNCTION: STBC 0x00715c30 _$E401
// FUNCTION: STBC 0x00715c40 _$E406
// FUNCTION: STBC 0x00715c50 _$E405
// FUNCTION: STBC 0x00715c80 _$E410
// FUNCTION: STBC 0x00715c90 _$E409
// FUNCTION: STBC 0x00715ce0 _$E414
// FUNCTION: STBC 0x00715cf0 _$E413
// FUNCTION: STBC 0x00715d00 _$E419
// FUNCTION: STBC 0x00715d10 _$E418

/*
================
TGMusicFadeEvent::Copy

Copies the event's state, and a copy of the piece's name.
================
*/
// FUNCTION: STBC 0x00715d40
bool TGMusicFadeEvent::Copy( TGEvent *event ) {
	if ( TGEvent::Copy( event ) && event->IsTypeOf( TGMUSICFADEEVENT_TYPE ) ) {
		TGMusicFadeEvent *other = (TGMusicFadeEvent *)event;
		if ( musicName != NULL ) {
			delete musicName;
			musicName = NULL;
		}
		if ( other->musicName != NULL ) {
			musicName = new char[strlen( other->musicName ) + 1];
			strcpy( musicName, other->musicName );
		}
		fromVolume = other->fromVolume;
		toVolume = other->toVolume;
		fadeTime = other->fadeTime;
		unknown38 = other->unknown38;
		looping = other->looping;
		return true;
	}
	return false;
}

/*
================
TGMusicFadeEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x00715df0
bool TGMusicFadeEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( musicName );
	stream->WriteFloat( fromVolume );
	stream->WriteFloat( toVolume );
	stream->WriteFloat( fadeTime );
	stream->WriteFloat( unknown38 );
	stream->WriteBool( looping );
	return true;
}

/*
================
TGMusicFadeEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x00715e50
bool TGMusicFadeEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}
	musicName = stream->VirtualSlot10( -1 );
	fromVolume = stream->ReadFloat();
	toVolume = stream->ReadFloat();
	fadeTime = stream->ReadFloat();
	unknown38 = stream->ReadFloat();
	looping = stream->ReadBool();
	return true;
}

/*
================
TGMusicFadeEvent::WriteToNetStream

The event isn't sent over the network.
================
*/
// FUNCTION: STBC 0x00715eb0
bool TGMusicFadeEvent::WriteToNetStream( TGStream *stream ) {
	return false;
}

/*
================
TGMusicFadeEvent::ReadFromNetStream

The event isn't sent over the network.
================
*/
// FUNCTION: STBC 0x00715ec0
bool TGMusicFadeEvent::ReadFromNetStream( TGStream *stream ) {
	return false;
}
