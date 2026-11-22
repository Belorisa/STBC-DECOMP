/*
===============================================================================

	TGEventHandlerTable.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGEventHandlerTable.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d71c0 _$E420
// FUNCTION: STBC 0x006d71e0 _$E419
// FUNCTION: STBC 0x006d71f0 _$E32
// FUNCTION: STBC 0x006d7200 _$E29
// FUNCTION: STBC 0x006d7210 _$E31
// FUNCTION: STBC 0x006d7220 _$E30
// FUNCTION: STBC 0x006d7230 _$E38
// FUNCTION: STBC 0x006d7240 _$E35
// FUNCTION: STBC 0x006d7250 _$E37
// FUNCTION: STBC 0x006d7260 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d7270 _$E57
// FUNCTION: STBC 0x006d7280 _$E56
// FUNCTION: STBC 0x006d7290 _$E61
// FUNCTION: STBC 0x006d72a0 _$E60
// FUNCTION: STBC 0x006d72b0 _$E72
// FUNCTION: STBC 0x006d72c0 _$E71
// FUNCTION: STBC 0x006d72d0 _$E76
// FUNCTION: STBC 0x006d72e0 _$E75
// FUNCTION: STBC 0x006d72f0 _$E80
// FUNCTION: STBC 0x006d7300 _$E79
// FUNCTION: STBC 0x006d7310 _$E84
// FUNCTION: STBC 0x006d7320 _$E83
// FUNCTION: STBC 0x006d7330 _$E208
// FUNCTION: STBC 0x006d7340 _$E207
// FUNCTION: STBC 0x006d7350 _$E150
// FUNCTION: STBC 0x006d7360 _$E149
// FUNCTION: STBC 0x006d7370 _$E163
// FUNCTION: STBC 0x006d7380 _$E162
// FUNCTION: STBC 0x006d7390 _$E213
// FUNCTION: STBC 0x006d73a0 _$E212
// FUNCTION: STBC 0x006d73b0 _$E218
// FUNCTION: STBC 0x006d73c0 _$E217
// FUNCTION: STBC 0x006d73d0 _$E222
// FUNCTION: STBC 0x006d73e0 _$E221
// FUNCTION: STBC 0x006d73f0 _$E226
// FUNCTION: STBC 0x006d7400 _$E225
// FUNCTION: STBC 0x006d7410 _$E233
// FUNCTION: STBC 0x006d7420 _$E232
// FUNCTION: STBC 0x006d7430 _$E258
// FUNCTION: STBC 0x006d7440 _$E257
// FUNCTION: STBC 0x006d7450 _$E262
// FUNCTION: STBC 0x006d7460 _$E261
// FUNCTION: STBC 0x006d7470 _$E108
// FUNCTION: STBC 0x006d7480 _$E107
// FUNCTION: STBC 0x006d7490 _$E244
// FUNCTION: STBC 0x006d74a0 _$E243
// FUNCTION: STBC 0x006d74b0 _$E251
// FUNCTION: STBC 0x006d74c0 _$E250
// FUNCTION: STBC 0x006d74d0 _$E274
// FUNCTION: STBC 0x006d74e0 _$E273
// FUNCTION: STBC 0x006d74f0 _$E278
// FUNCTION: STBC 0x006d7500 _$E277
// FUNCTION: STBC 0x006d7510 _$E318
// FUNCTION: STBC 0x006d7520 _$E317
// FUNCTION: STBC 0x006d7530 _$E324
// FUNCTION: STBC 0x006d7540 _$E323
// FUNCTION: STBC 0x006d7550 _$E266
// FUNCTION: STBC 0x006d7560 _$E265
// FUNCTION: STBC 0x006d7570 _$E270
// FUNCTION: STBC 0x006d7580 _$E269
// FUNCTION: STBC 0x006d7590 _$E282
// FUNCTION: STBC 0x006d75a0 _$E281
// FUNCTION: STBC 0x006d75b0 _$E286
// FUNCTION: STBC 0x006d75c0 _$E285
// FUNCTION: STBC 0x006d75d0 _$E290
// FUNCTION: STBC 0x006d75e0 _$E289
// FUNCTION: STBC 0x006d75f0 _$E294
// FUNCTION: STBC 0x006d7600 _$E293
// FUNCTION: STBC 0x006d7610 _$E298
// FUNCTION: STBC 0x006d7620 _$E297
// FUNCTION: STBC 0x006d7630 _$E302
// FUNCTION: STBC 0x006d7640 _$E301
// FUNCTION: STBC 0x006d7650 _$E306
// FUNCTION: STBC 0x006d7660 _$E305
// FUNCTION: STBC 0x006d7670 _$E310
// FUNCTION: STBC 0x006d7680 _$E309
// FUNCTION: STBC 0x006d7690 _$E113
// FUNCTION: STBC 0x006d76a0 _$E112
// FUNCTION: STBC 0x006d76b0 _$E314
// FUNCTION: STBC 0x006d76c0 _$E313
// FUNCTION: STBC 0x006d76d0 _$E118
// FUNCTION: STBC 0x006d76e0 _$E117
// FUNCTION: STBC 0x006d76f0 _$E328
// FUNCTION: STBC 0x006d7700 _$E327
// FUNCTION: STBC 0x006d7710 _$E332
// FUNCTION: STBC 0x006d7720 _$E331
// FUNCTION: STBC 0x006d7730 _$E336
// FUNCTION: STBC 0x006d7740 _$E335
// FUNCTION: STBC 0x006d7750 _$E340
// FUNCTION: STBC 0x006d7760 _$E339
// FUNCTION: STBC 0x006d7770 _$E127
// FUNCTION: STBC 0x006d7780 _$E126
// FUNCTION: STBC 0x006d7790 _$E344
// FUNCTION: STBC 0x006d77a0 _$E343
// FUNCTION: STBC 0x006d77b0 _$E348
// FUNCTION: STBC 0x006d77c0 _$E347
// FUNCTION: STBC 0x006d77d0 _$E352
// FUNCTION: STBC 0x006d77e0 _$E351
// FUNCTION: STBC 0x006d77f0 _$E133
// FUNCTION: STBC 0x006d7800 _$E132
// FUNCTION: STBC 0x006d7810 _$E138
// FUNCTION: STBC 0x006d7820 _$E137
// FUNCTION: STBC 0x006d7830 _$E168
// FUNCTION: STBC 0x006d7840 _$E167
// FUNCTION: STBC 0x006d7850 _$E356
// FUNCTION: STBC 0x006d7860 _$E355
// FUNCTION: STBC 0x006d7870 _$E173
// FUNCTION: STBC 0x006d7880 _$E172
// FUNCTION: STBC 0x006d7890 _$E360
// FUNCTION: STBC 0x006d78a0 _$E359
// FUNCTION: STBC 0x006d78b0 _$E364
// FUNCTION: STBC 0x006d78c0 _$E363
// FUNCTION: STBC 0x006d78d0 _$E178
// FUNCTION: STBC 0x006d78e0 _$E177
// FUNCTION: STBC 0x006d78f0 _$E368
// FUNCTION: STBC 0x006d7900 _$E367
// FUNCTION: STBC 0x006d7910 _$E143
// FUNCTION: STBC 0x006d7920 _$E142
// FUNCTION: STBC 0x006d7930 _$E372
// FUNCTION: STBC 0x006d7940 _$E371
// FUNCTION: STBC 0x006d7950 _$E376
// FUNCTION: STBC 0x006d7960 _$E375
// FUNCTION: STBC 0x006d7970 _$E183
// FUNCTION: STBC 0x006d7980 _$E182
// FUNCTION: STBC 0x006d7990 _$E188
// FUNCTION: STBC 0x006d79a0 _$E187
// FUNCTION: STBC 0x006d79b0 _$E380
// FUNCTION: STBC 0x006d79c0 _$E379
// FUNCTION: STBC 0x006d79d0 _$E384
// FUNCTION: STBC 0x006d79e0 _$E383
// FUNCTION: STBC 0x006d79f0 _$E388
// FUNCTION: STBC 0x006d7a00 _$E387
// FUNCTION: STBC 0x006d7a10 _$E392
// FUNCTION: STBC 0x006d7a20 _$E391
// FUNCTION: STBC 0x006d7a30 _$E396
// FUNCTION: STBC 0x006d7a40 _$E395
// FUNCTION: STBC 0x006d7a50 _$E400
// FUNCTION: STBC 0x006d7a60 _$E399
// FUNCTION: STBC 0x006d7a70 _$E404
// FUNCTION: STBC 0x006d7a80 _$E403
// FUNCTION: STBC 0x006d7a90 _$E408
// FUNCTION: STBC 0x006d7aa0 _$E407
// FUNCTION: STBC 0x006d7ab0 _$E193
// FUNCTION: STBC 0x006d7ac0 _$E192
// FUNCTION: STBC 0x006d7ad0 _$E412
// FUNCTION: STBC 0x006d7ae0 _$E411
// FUNCTION: STBC 0x006d7af0 _$E198
// FUNCTION: STBC 0x006d7b00 _$E197
// FUNCTION: STBC 0x006d7b10 _$E203
// FUNCTION: STBC 0x006d7b20 _$E202

/*
================
TGEventHandlerTable::TGEventHandlerTable
================
*/
// FUNCTION: STBC 0x006d7b30
TGEventHandlerTable::TGEventHandlerTable( void ) {
	parent = NULL;
}

/*
================
TGEventHandlerTable::~TGEventHandlerTable
================
*/
// FUNCTION: STBC 0x006d7b80
TGEventHandlerTable::~TGEventHandlerTable( void ) {
	RemoveAllHandlers();
	parent = NULL;
}

/*
================
TGEventHandlerTable::WriteToStream

The number of event types, then for each the type, the number of its
handlers and the handlers.
================
*/
// FUNCTION: STBC 0x006d7c30
bool TGEventHandlerTable::WriteToStream( TGStream *stream ) {
	stream->WriteInt( handlers.GetCount() );

	NiTMapIterator pos = handlers.GetFirstPos();
	while ( pos != NULL ) {
		int eventType;
		TGHandler *first;
		handlers.GetNext( pos, eventType, first );
		stream->WriteInt( eventType );

		int numHandlers = 0;
		TGHandler *handler;
		for ( handler = first; handler != NULL; handler = handler->next ) {
			numHandlers++;
		}
		stream->WriteInt( numHandlers );

		for ( handler = first; handler != NULL; handler = handler->next ) {
			handler->WriteToStream( stream );
		}
	}
	return true;
}

/*
================
TGEventHandlerTable::ReadFromStream

Reads what WriteToStream wrote, keeping the order of each list.
================
*/
// FUNCTION: STBC 0x006d7d00
bool TGEventHandlerTable::ReadFromStream( TGStream *stream ) {
	bool result = true;

	int numTypes = stream->ReadInt();
	for ( int i = 0; i < numTypes; i++ ) {
		int eventType = stream->ReadInt();
		int numHandlers = stream->ReadInt();

		TGHandler *last = NULL;
		for ( int j = 0; j < numHandlers; j++ ) {
			TGHandler *handler = new TGHandler;
			if ( handler == NULL ) {
				result = false;
				continue;
			}
			if ( !handler->ReadFromStream( stream ) ) {
				result = false;
			}

			if ( j == 0 ) {
				handlers.SetAt( eventType, handler );
				last = handler;
			} else {
				last->next = handler;
				last = handler;
			}
		}
	}
	return result;
}

/*
================
TGEventHandlerTable::AddHandler

Puts a new handler at the head of the event type's list: a registered C++
handler, or a Python one, called as a function or as a method.
================
*/
// FUNCTION: STBC 0x006d7eb0
bool TGEventHandlerTable::AddHandler( int eventType, char *name, bool registered, bool method ) {
	TGHandler *handler = new TGHandler;
	if ( handler == NULL ) {
		return false;
	}

	if ( registered ) {
		handler->SetFunction( name, method );
	} else {
		handler->SetPythonFunction( name, method );
	}

	TGHandler *first;
	if ( handlers.GetAt( eventType, first ) ) {
		handler->next = first;
	}
	handlers.SetAt( eventType, handler );
	return true;
}

/*
================
TGEventHandlerTable::RemoveHandler

Unlinks the handler of the event type with the name: a registered C++
handler when one is registered under it, else a Python one. A handler that
is running is only marked; it deletes itself when it returns.
================
*/
// FUNCTION: STBC 0x006d8010
bool TGEventHandlerTable::RemoveHandler( int eventType, char *name ) {
	TGHandler *handler = GetHandlers( eventType );
	TGHandler *prev = NULL;

	TGHandlerFunction function = handlerRegistry.Find( TGHashString( name ) );
	if ( function != NULL ) {
		while ( handler != NULL && function != handler->function ) {
			prev = handler;
			handler = handler->next;
		}
	} else {
		while ( handler != NULL && strcmp( name, handler->pythonName ) != 0 ) {
			prev = handler;
			handler = handler->next;
		}
	}

	if ( handler == NULL ) {
		return false;
	}

	if ( prev == NULL ) {
		if ( handler->next != NULL ) {
			handlers.SetAt( eventType, handler->next );
		} else {
			handlers.RemoveAt( eventType );
		}
	} else {
		prev->next = handler->next;
	}

	if ( handler->IsCalling() == true ) {
		handler->SetDeletePending();
		return true;
	}
	delete handler;
	return true;
}

/*
================
TGEventHandlerTable::GetHandlers

The newest handler of the event type, or NULL.
================
*/
// FUNCTION: STBC 0x006d8230
TGHandler *TGEventHandlerTable::GetHandlers( int eventType ) {
	TGHandler *handler;
	if ( handlers.GetAt( eventType, handler ) ) {
		return handler;
	}
	return NULL;
}

/*
================
TGEventHandlerTable::FindHandlers

The handlers of the event type in this table or the nearest parent that has
any; owner receives that table (the last one searched when none has).
================
*/
// FUNCTION: STBC 0x006d8270
TGHandler *TGEventHandlerTable::FindHandlers( int eventType, TGEventHandlerTable **owner ) {
	TGEventHandlerTable *table = this;
	TGHandler *handler = table->GetHandlers( eventType );
	while ( handler == NULL && table->parent != NULL ) {
		table = table->parent;
		handler = table->GetHandlers( eventType );
	}
	*owner = table;
	return handler;
}

/*
================
TGEventHandlerTable::RemoveAllHandlers

Running handlers are only marked, as in RemoveHandler.
================
*/
// FUNCTION: STBC 0x006d82b0
void TGEventHandlerTable::RemoveAllHandlers( void ) {
	while ( handlers.GetCount() != 0 ) {
		NiTMapIterator pos = handlers.GetFirstPos();
		int eventType;
		TGHandler *handler;
		handlers.GetNext( pos, eventType, handler );

		while ( handler != NULL ) {
			TGHandler *next = handler->next;
			if ( handler->IsCalling() == true ) {
				handler->SetDeletePending();
			} else {
				delete handler;
			}
			handler = next;
		}
		handlers.RemoveAt( eventType );
	}
}

/*
================
TGEventHandlerTable::CallNextHandler

Passes the event on from the handler running: to the next one in its list,
else to the handlers of the type in the parent tables.
================
*/
// FUNCTION: STBC 0x006d83e0
bool TGEventHandlerTable::CallNextHandler( TGEvent *event, TGEventHandlerObject *object ) {
	TGHandler *handler = event->currentHandler;
	if ( handler->next != NULL ) {
		event->currentHandler = handler->next;
		event->currentHandler->Call( event, object );
	} else if ( parent != NULL ) {
		TGEventHandlerTable *table = event->currentTable;
		event->currentHandler = parent->FindHandlers( event->GetEventType(), &event->currentTable );
		if ( event->currentHandler != NULL ) {
			event->currentHandler->Call( event, object );
		}
		event->currentTable = table;
	}
	event->currentHandler = handler;
	return true;
}
