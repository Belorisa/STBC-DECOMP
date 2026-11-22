/*
===============================================================================

	TGBroadcastHandlerTable.cpp

===============================================================================
*/

#include <iostream>
#include "TGBroadcastHandlerTable.h"
#include "TGHandler.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006da510 _$E420
// FUNCTION: STBC 0x006da530 _$E419
// FUNCTION: STBC 0x006da540 _$E32
// FUNCTION: STBC 0x006da550 _$E29
// FUNCTION: STBC 0x006da560 _$E31
// FUNCTION: STBC 0x006da570 _$E30
// FUNCTION: STBC 0x006da580 _$E38
// FUNCTION: STBC 0x006da590 _$E35
// FUNCTION: STBC 0x006da5a0 _$E37
// FUNCTION: STBC 0x006da5b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006da5c0 _$E57
// FUNCTION: STBC 0x006da5d0 _$E56
// FUNCTION: STBC 0x006da5e0 _$E61
// FUNCTION: STBC 0x006da5f0 _$E60
// FUNCTION: STBC 0x006da600 _$E72
// FUNCTION: STBC 0x006da610 _$E71
// FUNCTION: STBC 0x006da620 _$E76
// FUNCTION: STBC 0x006da630 _$E75
// FUNCTION: STBC 0x006da640 _$E80
// FUNCTION: STBC 0x006da650 _$E79
// FUNCTION: STBC 0x006da660 _$E84
// FUNCTION: STBC 0x006da670 _$E83
// FUNCTION: STBC 0x006da680 _$E208
// FUNCTION: STBC 0x006da690 _$E207
// FUNCTION: STBC 0x006da6a0 _$E150
// FUNCTION: STBC 0x006da6b0 _$E149
// FUNCTION: STBC 0x006da6c0 _$E163
// FUNCTION: STBC 0x006da6d0 _$E162
// FUNCTION: STBC 0x006da6e0 _$E213
// FUNCTION: STBC 0x006da6f0 _$E212
// FUNCTION: STBC 0x006da700 _$E218
// FUNCTION: STBC 0x006da710 _$E217
// FUNCTION: STBC 0x006da720 _$E222
// FUNCTION: STBC 0x006da730 _$E221
// FUNCTION: STBC 0x006da740 _$E226
// FUNCTION: STBC 0x006da750 _$E225
// FUNCTION: STBC 0x006da760 _$E233
// FUNCTION: STBC 0x006da770 _$E232
// FUNCTION: STBC 0x006da780 _$E258
// FUNCTION: STBC 0x006da790 _$E257
// FUNCTION: STBC 0x006da7a0 _$E262
// FUNCTION: STBC 0x006da7b0 _$E261
// FUNCTION: STBC 0x006da7c0 _$E108
// FUNCTION: STBC 0x006da7d0 _$E107
// FUNCTION: STBC 0x006da7e0 _$E244
// FUNCTION: STBC 0x006da7f0 _$E243
// FUNCTION: STBC 0x006da800 _$E251
// FUNCTION: STBC 0x006da810 _$E250
// FUNCTION: STBC 0x006da820 _$E274
// FUNCTION: STBC 0x006da830 _$E273
// FUNCTION: STBC 0x006da840 _$E278
// FUNCTION: STBC 0x006da850 _$E277
// FUNCTION: STBC 0x006da860 _$E318
// FUNCTION: STBC 0x006da870 _$E317
// FUNCTION: STBC 0x006da880 _$E324
// FUNCTION: STBC 0x006da890 _$E323
// FUNCTION: STBC 0x006da8a0 _$E266
// FUNCTION: STBC 0x006da8b0 _$E265
// FUNCTION: STBC 0x006da8c0 _$E270
// FUNCTION: STBC 0x006da8d0 _$E269
// FUNCTION: STBC 0x006da8e0 _$E282
// FUNCTION: STBC 0x006da8f0 _$E281
// FUNCTION: STBC 0x006da900 _$E286
// FUNCTION: STBC 0x006da910 _$E285
// FUNCTION: STBC 0x006da920 _$E290
// FUNCTION: STBC 0x006da930 _$E289
// FUNCTION: STBC 0x006da940 _$E294
// FUNCTION: STBC 0x006da950 _$E293
// FUNCTION: STBC 0x006da960 _$E298
// FUNCTION: STBC 0x006da970 _$E297
// FUNCTION: STBC 0x006da980 _$E302
// FUNCTION: STBC 0x006da990 _$E301
// FUNCTION: STBC 0x006da9a0 _$E306
// FUNCTION: STBC 0x006da9b0 _$E305
// FUNCTION: STBC 0x006da9c0 _$E310
// FUNCTION: STBC 0x006da9d0 _$E309
// FUNCTION: STBC 0x006da9e0 _$E113
// FUNCTION: STBC 0x006da9f0 _$E112
// FUNCTION: STBC 0x006daa00 _$E314
// FUNCTION: STBC 0x006daa10 _$E313
// FUNCTION: STBC 0x006daa20 _$E118
// FUNCTION: STBC 0x006daa30 _$E117
// FUNCTION: STBC 0x006daa40 _$E328
// FUNCTION: STBC 0x006daa50 _$E327
// FUNCTION: STBC 0x006daa60 _$E332
// FUNCTION: STBC 0x006daa70 _$E331
// FUNCTION: STBC 0x006daa80 _$E336
// FUNCTION: STBC 0x006daa90 _$E335
// FUNCTION: STBC 0x006daaa0 _$E340
// FUNCTION: STBC 0x006daab0 _$E339
// FUNCTION: STBC 0x006daac0 _$E127
// FUNCTION: STBC 0x006daad0 _$E126
// FUNCTION: STBC 0x006daae0 _$E344
// FUNCTION: STBC 0x006daaf0 _$E343
// FUNCTION: STBC 0x006dab00 _$E348
// FUNCTION: STBC 0x006dab10 _$E347
// FUNCTION: STBC 0x006dab20 _$E352
// FUNCTION: STBC 0x006dab30 _$E351
// FUNCTION: STBC 0x006dab40 _$E133
// FUNCTION: STBC 0x006dab50 _$E132
// FUNCTION: STBC 0x006dab60 _$E138
// FUNCTION: STBC 0x006dab70 _$E137
// FUNCTION: STBC 0x006dab80 _$E168
// FUNCTION: STBC 0x006dab90 _$E167
// FUNCTION: STBC 0x006daba0 _$E356
// FUNCTION: STBC 0x006dabb0 _$E355
// FUNCTION: STBC 0x006dabc0 _$E173
// FUNCTION: STBC 0x006dabd0 _$E172
// FUNCTION: STBC 0x006dabe0 _$E360
// FUNCTION: STBC 0x006dabf0 _$E359
// FUNCTION: STBC 0x006dac00 _$E364
// FUNCTION: STBC 0x006dac10 _$E363
// FUNCTION: STBC 0x006dac20 _$E178
// FUNCTION: STBC 0x006dac30 _$E177
// FUNCTION: STBC 0x006dac40 _$E368
// FUNCTION: STBC 0x006dac50 _$E367
// FUNCTION: STBC 0x006dac60 _$E143
// FUNCTION: STBC 0x006dac70 _$E142
// FUNCTION: STBC 0x006dac80 _$E372
// FUNCTION: STBC 0x006dac90 _$E371
// FUNCTION: STBC 0x006daca0 _$E376
// FUNCTION: STBC 0x006dacb0 _$E375
// FUNCTION: STBC 0x006dacc0 _$E183
// FUNCTION: STBC 0x006dacd0 _$E182
// FUNCTION: STBC 0x006dace0 _$E188
// FUNCTION: STBC 0x006dacf0 _$E187
// FUNCTION: STBC 0x006dad00 _$E380
// FUNCTION: STBC 0x006dad10 _$E379
// FUNCTION: STBC 0x006dad20 _$E384
// FUNCTION: STBC 0x006dad30 _$E383
// FUNCTION: STBC 0x006dad40 _$E388
// FUNCTION: STBC 0x006dad50 _$E387
// FUNCTION: STBC 0x006dad60 _$E392
// FUNCTION: STBC 0x006dad70 _$E391
// FUNCTION: STBC 0x006dad80 _$E396
// FUNCTION: STBC 0x006dad90 _$E395
// FUNCTION: STBC 0x006dada0 _$E400
// FUNCTION: STBC 0x006dadb0 _$E399
// FUNCTION: STBC 0x006dadc0 _$E404
// FUNCTION: STBC 0x006dadd0 _$E403
// FUNCTION: STBC 0x006dade0 _$E408
// FUNCTION: STBC 0x006dadf0 _$E407
// FUNCTION: STBC 0x006dae00 _$E193
// FUNCTION: STBC 0x006dae10 _$E192
// FUNCTION: STBC 0x006dae20 _$E412
// FUNCTION: STBC 0x006dae30 _$E411
// FUNCTION: STBC 0x006dae40 _$E198
// FUNCTION: STBC 0x006dae50 _$E197
// FUNCTION: STBC 0x006dae60 _$E203
// FUNCTION: STBC 0x006dae70 _$E202

/*
================
TGBroadcastHandlerTable::TGBroadcastHandlerTable
================
*/
// FUNCTION: STBC 0x006dae80
TGBroadcastHandlerTable::TGBroadcastHandlerTable( void ) {
}

/*
================
TGBroadcastHandlerTable::~TGBroadcastHandlerTable
================
*/
// FUNCTION: STBC 0x006daed0
TGBroadcastHandlerTable::~TGBroadcastHandlerTable( void ) {
	RemoveAllHandlers();
}

/*
================
TGBroadcastHandlerTable::WriteToStream

The number of event types, then each type and its list.
================
*/
// FUNCTION: STBC 0x006daf70
bool TGBroadcastHandlerTable::WriteToStream( TGStream *stream ) {
	stream->WriteInt( lists.GetCount() );

	NiTMapIterator pos = lists.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		stream->WriteInt( eventType );
		if ( !list->WriteToStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
TGBroadcastHandlerTable::ReadFromStream

Reads what WriteToStream wrote. A type read twice keeps its first list.
================
*/
// FUNCTION: STBC 0x006db020
bool TGBroadcastHandlerTable::ReadFromStream( TGStream *stream ) {
	bool result = true;

	int numTypes = stream->ReadInt();
	for ( int i = 0; i < numTypes; i++ ) {
		int eventType = stream->ReadInt();
		TGBroadcastHandlerList *list = new TGBroadcastHandlerList;
		if ( list == NULL ) {
			result = false;
			continue;
		}

		TGBroadcastHandlerList *existing;
		if ( lists.GetAt( eventType, existing ) ) {
			result = false;
			delete list;
			continue;
		}
		lists.SetAt( eventType, list );
		if ( !list->ReadFromStream( stream ) ) {
			result = false;
		}
	}
	return result;
}

/*
================
TGBroadcastHandlerTable::FixUpPointers
================
*/
// FUNCTION: STBC 0x006db1b0
bool TGBroadcastHandlerTable::FixUpPointers( void ) {
	NiTMapIterator pos = lists.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		if ( !list->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
TGBroadcastHandlerTable::PostFixUp
================
*/
// FUNCTION: STBC 0x006db230
void TGBroadcastHandlerTable::PostFixUp( void ) {
	NiTMapIterator pos = lists.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		list->PostFixUp();
	}
}

/*
================
TGBroadcastHandlerTable::ForgetHandlers

Deletes the lists after emptying them, so their handlers stay alive.
================
*/
// FUNCTION: STBC 0x006db2b0
void TGBroadcastHandlerTable::ForgetHandlers( void ) {
	NiTMapIterator pos = lists.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		list->ForgetHandlers();
		delete list;
	}
	lists.RemoveAll();
}

/*
================
TGBroadcastHandlerTable::AddHandler

Adds a handler for the event type, called on behalf of destination for the
events sent to target: a registered C++ handler or a Python one, called as
a function or as a method.
================
*/
// FUNCTION: STBC 0x006db380 ?AddHandler@TGBroadcastHandlerTable@@QAE_NHPAVTGEventHandlerObject@@PAD_N20@Z
bool TGBroadcastHandlerTable::AddHandler( int eventType, TGEventHandlerObject *destination, char *name, bool registered, bool method, TGEventHandlerObject *target ) {
	TGHandler *handler = new TGHandler;
	if ( handler == NULL ) {
		return false;
	}

	if ( registered ) {
		handler->SetFunction( name, method );
	} else {
		handler->SetPythonFunction( name, method );
	}
	handler->target = target;

	TGBroadcastHandlerList *list;
	if ( !lists.GetAt( eventType, list ) ) {
		list = new TGBroadcastHandlerList;
		if ( list != NULL ) {
			lists.SetAt( eventType, list );
		} else {
			return false;	// BUG: the handler leaks
		}
	}
	return list->AddHandler( destination, handler );
}

/*
================
TGBroadcastHandlerTable::RemoveHandler
================
*/
// FUNCTION: STBC 0x006db530 ?RemoveHandler@TGBroadcastHandlerTable@@QAE_NHPAVTGEventHandlerObject@@PAD0@Z
bool TGBroadcastHandlerTable::RemoveHandler( int eventType, TGEventHandlerObject *destination, char *name, TGEventHandlerObject *target ) {
	TGBroadcastHandlerList *list;
	if ( !lists.GetAt( eventType, list ) ) {
		return false;
	}
	return list->RemoveHandler( destination, name, target );
}

/*
================
TGBroadcastHandlerTable::RemoveAllHandlersForObject
================
*/
// FUNCTION: STBC 0x006db590 ?RemoveAllHandlersForObject@TGBroadcastHandlerTable@@QAE_NPAVTGEventHandlerObject@@@Z
bool TGBroadcastHandlerTable::RemoveAllHandlersForObject( TGEventHandlerObject *object ) {
	NiTMapIterator pos = lists.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		list->RemoveAllHandlersForObject( object );
	}
	return true;
}

/*
================
TGBroadcastHandlerTable::CallHandlers
================
*/
// FUNCTION: STBC 0x006db620
bool TGBroadcastHandlerTable::CallHandlers( TGEvent *event ) {
	TGBroadcastHandlerList *list;
	if ( lists.GetAt( event->GetEventType(), list ) ) {
		list->CallHandlers( event );
	}
	return true;
}

/*
================
TGBroadcastHandlerTable::RemoveAllHandlers
================
*/
// FUNCTION: STBC 0x006db670
void TGBroadcastHandlerTable::RemoveAllHandlers( void ) {
	while ( lists.GetCount() != 0 ) {
		NiTMapIterator pos = lists.GetFirstPos();
		int eventType;
		TGBroadcastHandlerList *list;
		lists.GetNext( pos, eventType, list );
		lists.RemoveAt( eventType );
		delete list;
	}
}
