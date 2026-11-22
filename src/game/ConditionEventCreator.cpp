/*
===============================================================================

	ConditionEventCreator.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>

#include "ConditionEventCreator.h"
#include "TGStream.h"
#include "NiTList.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0047b7e0 _$E435
// FUNCTION: STBC 0x0047b800 _$E434

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0047b380 _$E32
// FUNCTION: STBC 0x0047b390 _$E29
// FUNCTION: STBC 0x0047b3a0 _$E31
// FUNCTION: STBC 0x0047b3b0 _$E30
// FUNCTION: STBC 0x0047b3c0 _$E38
// FUNCTION: STBC 0x0047b3d0 _$E35
// FUNCTION: STBC 0x0047b3e0 _$E37
// FUNCTION: STBC 0x0047b3f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047b400 _$E59
// FUNCTION: STBC 0x0047b410 _$E58
// FUNCTION: STBC 0x0047b420 _$E63
// FUNCTION: STBC 0x0047b430 _$E62
// FUNCTION: STBC 0x0047b440 _$E74
// FUNCTION: STBC 0x0047b450 _$E73
// FUNCTION: STBC 0x0047b460 _$E78
// FUNCTION: STBC 0x0047b470 _$E77
// FUNCTION: STBC 0x0047b480 _$E82
// FUNCTION: STBC 0x0047b490 _$E81
// FUNCTION: STBC 0x0047b4a0 _$E86
// FUNCTION: STBC 0x0047b4b0 _$E85
// FUNCTION: STBC 0x0047b4c0 _$E210
// FUNCTION: STBC 0x0047b4d0 _$E209
// FUNCTION: STBC 0x0047b4e0 _$E152
// FUNCTION: STBC 0x0047b4f0 _$E151
// FUNCTION: STBC 0x0047b500 _$E165
// FUNCTION: STBC 0x0047b510 _$E164
// FUNCTION: STBC 0x0047b520 _$E215
// FUNCTION: STBC 0x0047b530 _$E214
// FUNCTION: STBC 0x0047b540 _$E220
// FUNCTION: STBC 0x0047b550 _$E219
// FUNCTION: STBC 0x0047b560 _$E224
// FUNCTION: STBC 0x0047b570 _$E223
// FUNCTION: STBC 0x0047b580 _$E228
// FUNCTION: STBC 0x0047b590 _$E227
// FUNCTION: STBC 0x0047b5a0 _$E235
// FUNCTION: STBC 0x0047b5b0 _$E234
// FUNCTION: STBC 0x0047b5c0 _$E260
// FUNCTION: STBC 0x0047b5d0 _$E259
// FUNCTION: STBC 0x0047b5e0 _$E264
// FUNCTION: STBC 0x0047b5f0 _$E263
// FUNCTION: STBC 0x0047b600 _$E110
// FUNCTION: STBC 0x0047b610 _$E109
// FUNCTION: STBC 0x0047b620 _$E246
// FUNCTION: STBC 0x0047b630 _$E245
// FUNCTION: STBC 0x0047b640 _$E253
// FUNCTION: STBC 0x0047b650 _$E252
// FUNCTION: STBC 0x0047b660 _$E276
// FUNCTION: STBC 0x0047b670 _$E275
// FUNCTION: STBC 0x0047b680 _$E280
// FUNCTION: STBC 0x0047b690 _$E279
// FUNCTION: STBC 0x0047b6a0 _$E320
// FUNCTION: STBC 0x0047b6b0 _$E319
// FUNCTION: STBC 0x0047b6c0 _$E326
// FUNCTION: STBC 0x0047b6d0 _$E325
// FUNCTION: STBC 0x0047b6e0 _$E268
// FUNCTION: STBC 0x0047b6f0 _$E267
// FUNCTION: STBC 0x0047b700 _$E272
// FUNCTION: STBC 0x0047b710 _$E271
// FUNCTION: STBC 0x0047b720 _$E284
// FUNCTION: STBC 0x0047b730 _$E283
// FUNCTION: STBC 0x0047b740 _$E288
// FUNCTION: STBC 0x0047b750 _$E287
// FUNCTION: STBC 0x0047b760 _$E292
// FUNCTION: STBC 0x0047b770 _$E291
// FUNCTION: STBC 0x0047b780 _$E296
// FUNCTION: STBC 0x0047b790 _$E295
// FUNCTION: STBC 0x0047b7a0 _$E300
// FUNCTION: STBC 0x0047b7b0 _$E299
// FUNCTION: STBC 0x0047b7c0 _$E304
// FUNCTION: STBC 0x0047b7d0 _$E303
// FUNCTION: STBC 0x0047b810 _$E308
// FUNCTION: STBC 0x0047b820 _$E307
// FUNCTION: STBC 0x0047b830 _$E312
// FUNCTION: STBC 0x0047b840 _$E311
// FUNCTION: STBC 0x0047b850 _$E115
// FUNCTION: STBC 0x0047b860 _$E114
// FUNCTION: STBC 0x0047b870 _$E316
// FUNCTION: STBC 0x0047b880 _$E315
// FUNCTION: STBC 0x0047b890 _$E120
// FUNCTION: STBC 0x0047b8a0 _$E119
// FUNCTION: STBC 0x0047b8b0 _$E330
// FUNCTION: STBC 0x0047b8c0 _$E329
// FUNCTION: STBC 0x0047b8d0 _$E334
// FUNCTION: STBC 0x0047b8e0 _$E333
// FUNCTION: STBC 0x0047b8f0 _$E338
// FUNCTION: STBC 0x0047b900 _$E337
// FUNCTION: STBC 0x0047b910 _$E342
// FUNCTION: STBC 0x0047b920 _$E341
// FUNCTION: STBC 0x0047b930 _$E129
// FUNCTION: STBC 0x0047b940 _$E128
// FUNCTION: STBC 0x0047b950 _$E346
// FUNCTION: STBC 0x0047b960 _$E345
// FUNCTION: STBC 0x0047b970 _$E350
// FUNCTION: STBC 0x0047b980 _$E349
// FUNCTION: STBC 0x0047b990 _$E354
// FUNCTION: STBC 0x0047b9a0 _$E353
// FUNCTION: STBC 0x0047b9b0 _$E135
// FUNCTION: STBC 0x0047b9c0 _$E134
// FUNCTION: STBC 0x0047b9d0 _$E140
// FUNCTION: STBC 0x0047b9e0 _$E139
// FUNCTION: STBC 0x0047b9f0 _$E170
// FUNCTION: STBC 0x0047ba00 _$E169
// FUNCTION: STBC 0x0047ba10 _$E358
// FUNCTION: STBC 0x0047ba20 _$E357
// FUNCTION: STBC 0x0047ba30 _$E175
// FUNCTION: STBC 0x0047ba40 _$E174
// FUNCTION: STBC 0x0047ba50 _$E362
// FUNCTION: STBC 0x0047ba60 _$E361
// FUNCTION: STBC 0x0047ba70 _$E366
// FUNCTION: STBC 0x0047ba80 _$E365
// FUNCTION: STBC 0x0047ba90 _$E180
// FUNCTION: STBC 0x0047baa0 _$E179
// FUNCTION: STBC 0x0047bab0 _$E370
// FUNCTION: STBC 0x0047bac0 _$E369
// FUNCTION: STBC 0x0047bad0 _$E145
// FUNCTION: STBC 0x0047bae0 _$E144
// FUNCTION: STBC 0x0047baf0 _$E374
// FUNCTION: STBC 0x0047bb00 _$E373
// FUNCTION: STBC 0x0047bb10 _$E378
// FUNCTION: STBC 0x0047bb20 _$E377
// FUNCTION: STBC 0x0047bb30 _$E185
// FUNCTION: STBC 0x0047bb40 _$E184
// FUNCTION: STBC 0x0047bb50 _$E190
// FUNCTION: STBC 0x0047bb60 _$E189
// FUNCTION: STBC 0x0047bb70 _$E382
// FUNCTION: STBC 0x0047bb80 _$E381
// FUNCTION: STBC 0x0047bb90 _$E386
// FUNCTION: STBC 0x0047bba0 _$E385
// FUNCTION: STBC 0x0047bbb0 _$E390
// FUNCTION: STBC 0x0047bbc0 _$E389
// FUNCTION: STBC 0x0047bbd0 _$E394
// FUNCTION: STBC 0x0047bbe0 _$E393
// FUNCTION: STBC 0x0047bbf0 _$E398
// FUNCTION: STBC 0x0047bc00 _$E397
// FUNCTION: STBC 0x0047bc10 _$E402
// FUNCTION: STBC 0x0047bc20 _$E401
// FUNCTION: STBC 0x0047bc30 _$E406
// FUNCTION: STBC 0x0047bc40 _$E405
// FUNCTION: STBC 0x0047bc50 _$E410
// FUNCTION: STBC 0x0047bc60 _$E409
// FUNCTION: STBC 0x0047bc70 _$E195
// FUNCTION: STBC 0x0047bc80 _$E194
// FUNCTION: STBC 0x0047bc90 _$E414
// FUNCTION: STBC 0x0047bca0 _$E413
// FUNCTION: STBC 0x0047bcb0 _$E200
// FUNCTION: STBC 0x0047bcc0 _$E199
// FUNCTION: STBC 0x0047bcd0 _$E205
// FUNCTION: STBC 0x0047bce0 _$E204
// FUNCTION: STBC 0x0047bcf0 _$E418
// FUNCTION: STBC 0x0047bd00 _$E417
// FUNCTION: STBC 0x0047bd10 _$E423
// FUNCTION: STBC 0x0047bd20 _$E422
// FUNCTION: STBC 0x0047bd30 _$E427
// FUNCTION: STBC 0x0047bd40 _$E426

/*
================
ConditionEventCreator::WriteToStream
================
*/
// FUNCTION: STBC 0x0047b030 ?WriteToStream@ConditionEventCreator@@UAE_NPAVTGStream@@@Z
bool ConditionEventCreator::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}
	if ( !TGConditionHandler::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( event != NULL ? event->GetObjID() : 0 );
	return true;
}

/*
================
ConditionEventCreator::ReadFromStream
================
*/
// FUNCTION: STBC 0x0047b090 ?ReadFromStream@ConditionEventCreator@@UAE_NPAVTGStream@@@Z
bool ConditionEventCreator::ReadFromStream( TGStream *stream ) {
	if ( !TGConditionHandler::ReadFromStream( stream ) ) {
		return false;
	}
	event = (TGEvent *)stream->ReadID();
	return true;
}

/*
================
ConditionEventCreator::FixUpPointers

Fails while the saved event has not been loaded.
================
*/
// FUNCTION: STBC 0x0047b0c0 ?FixUpPointers@ConditionEventCreator@@UAE_NXZ
bool ConditionEventCreator::FixUpPointers( void ) {
	if ( !TGConditionHandler::FixUpPointers() ) {
		return false;
	}
	if ( event != NULL && TGEvent::GetTGEventPtr( (long)event ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ConditionEventCreator::PostFixUp
================
*/
// FUNCTION: STBC 0x0047b0f0 ?PostFixUp@ConditionEventCreator@@UAEXXZ
void ConditionEventCreator::PostFixUp( void ) {
	TGConditionHandler::PostFixUp();
	event = TGEvent::GetSavedEvent( (long)event );
}

/*
================
ConditionEventCreator::SaveConditionEventCreators

They are gathered from the object table first, for the count.
================
*/
// FUNCTION: STBC 0x0047b120
bool ConditionEventCreator::SaveConditionEventCreators( TGStream *stream ) {
	bool saved = true;
	NiTList<TGObject *> creators( LOAD_LIST_BLOCK );

	NiTMap<long, TGObject *> *objects = objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object->IsTypeOf( CONDITIONEVENTCREATOR_TYPE ) ) {
			creators.AddTail( object );
		}
	}

	stream->WriteInt( creators.GetSize() );
	char text[256];
	sprintf( text, "Saving %d ConditionEventCreator(s)\n", creators.GetSize() );
	Log( text );
	while ( !creators.IsEmpty() && saved ) {
		saved &= creators.RemoveHead()->WriteToStream( stream );
	}
	return saved;
}

/*
================
ConditionEventCreator::LoadConditionEventCreators

Reads what SaveConditionEventCreators wrote; once one fails, or is not a
ConditionEventCreator, the rest are skipped.
================
*/
// FUNCTION: STBC 0x0047b310
bool ConditionEventCreator::LoadConditionEventCreators( TGStream *stream ) {
	bool loaded = true;
	int count = stream->ReadInt();
	char text[256];
	sprintf( text, "Loading %d ConditionEventCreator(s)\n", count );
	Log( text );
	while ( --count >= 0 && loaded ) {
		TGObject *object = LoadObject( stream, 0 );
		if ( object == NULL || !object->IsTypeOf( CONDITIONEVENTCREATOR_TYPE ) ) {
			loaded = false;
		}
	}
	return loaded;
}
