/*
===============================================================================

	TGBroadcastHandlerList.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGBroadcastHandlerTable.h"
#include "TGEventHandlerObject.h"
#include "TGHandler.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006e0f00 _$E420
// FUNCTION: STBC 0x006e0f20 _$E419
// FUNCTION: STBC 0x006e0f30 _$E32
// FUNCTION: STBC 0x006e0f40 _$E29
// FUNCTION: STBC 0x006e0f50 _$E31
// FUNCTION: STBC 0x006e0f60 _$E30
// FUNCTION: STBC 0x006e0f70 _$E38
// FUNCTION: STBC 0x006e0f80 _$E35
// FUNCTION: STBC 0x006e0f90 _$E37
// FUNCTION: STBC 0x006e0fa0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006e0fb0 _$E57
// FUNCTION: STBC 0x006e0fc0 _$E56
// FUNCTION: STBC 0x006e0fd0 _$E61
// FUNCTION: STBC 0x006e0fe0 _$E60
// FUNCTION: STBC 0x006e0ff0 _$E72
// FUNCTION: STBC 0x006e1000 _$E71
// FUNCTION: STBC 0x006e1010 _$E76
// FUNCTION: STBC 0x006e1020 _$E75
// FUNCTION: STBC 0x006e1030 _$E80
// FUNCTION: STBC 0x006e1040 _$E79
// FUNCTION: STBC 0x006e1050 _$E84
// FUNCTION: STBC 0x006e1060 _$E83
// FUNCTION: STBC 0x006e1070 _$E208
// FUNCTION: STBC 0x006e1080 _$E207
// FUNCTION: STBC 0x006e1090 _$E150
// FUNCTION: STBC 0x006e10a0 _$E149
// FUNCTION: STBC 0x006e10b0 _$E163
// FUNCTION: STBC 0x006e10c0 _$E162
// FUNCTION: STBC 0x006e10d0 _$E213
// FUNCTION: STBC 0x006e10e0 _$E212
// FUNCTION: STBC 0x006e10f0 _$E218
// FUNCTION: STBC 0x006e1100 _$E217
// FUNCTION: STBC 0x006e1110 _$E222
// FUNCTION: STBC 0x006e1120 _$E221
// FUNCTION: STBC 0x006e1130 _$E226
// FUNCTION: STBC 0x006e1140 _$E225
// FUNCTION: STBC 0x006e1150 _$E233
// FUNCTION: STBC 0x006e1160 _$E232
// FUNCTION: STBC 0x006e1170 _$E258
// FUNCTION: STBC 0x006e1180 _$E257
// FUNCTION: STBC 0x006e1190 _$E262
// FUNCTION: STBC 0x006e11a0 _$E261
// FUNCTION: STBC 0x006e11b0 _$E108
// FUNCTION: STBC 0x006e11c0 _$E107
// FUNCTION: STBC 0x006e11d0 _$E244
// FUNCTION: STBC 0x006e11e0 _$E243
// FUNCTION: STBC 0x006e11f0 _$E251
// FUNCTION: STBC 0x006e1200 _$E250
// FUNCTION: STBC 0x006e1210 _$E274
// FUNCTION: STBC 0x006e1220 _$E273
// FUNCTION: STBC 0x006e1230 _$E278
// FUNCTION: STBC 0x006e1240 _$E277
// FUNCTION: STBC 0x006e1250 _$E318
// FUNCTION: STBC 0x006e1260 _$E317
// FUNCTION: STBC 0x006e1270 _$E324
// FUNCTION: STBC 0x006e1280 _$E323
// FUNCTION: STBC 0x006e1290 _$E266
// FUNCTION: STBC 0x006e12a0 _$E265
// FUNCTION: STBC 0x006e12b0 _$E270
// FUNCTION: STBC 0x006e12c0 _$E269
// FUNCTION: STBC 0x006e12d0 _$E282
// FUNCTION: STBC 0x006e12e0 _$E281
// FUNCTION: STBC 0x006e12f0 _$E286
// FUNCTION: STBC 0x006e1300 _$E285
// FUNCTION: STBC 0x006e1310 _$E290
// FUNCTION: STBC 0x006e1320 _$E289
// FUNCTION: STBC 0x006e1330 _$E294
// FUNCTION: STBC 0x006e1340 _$E293
// FUNCTION: STBC 0x006e1350 _$E298
// FUNCTION: STBC 0x006e1360 _$E297
// FUNCTION: STBC 0x006e1370 _$E302
// FUNCTION: STBC 0x006e1380 _$E301
// FUNCTION: STBC 0x006e1390 _$E306
// FUNCTION: STBC 0x006e13a0 _$E305
// FUNCTION: STBC 0x006e13b0 _$E310
// FUNCTION: STBC 0x006e13c0 _$E309
// FUNCTION: STBC 0x006e13d0 _$E113
// FUNCTION: STBC 0x006e13e0 _$E112
// FUNCTION: STBC 0x006e13f0 _$E314
// FUNCTION: STBC 0x006e1400 _$E313
// FUNCTION: STBC 0x006e1410 _$E118
// FUNCTION: STBC 0x006e1420 _$E117
// FUNCTION: STBC 0x006e1430 _$E328
// FUNCTION: STBC 0x006e1440 _$E327
// FUNCTION: STBC 0x006e1450 _$E332
// FUNCTION: STBC 0x006e1460 _$E331
// FUNCTION: STBC 0x006e1470 _$E336
// FUNCTION: STBC 0x006e1480 _$E335
// FUNCTION: STBC 0x006e1490 _$E340
// FUNCTION: STBC 0x006e14a0 _$E339
// FUNCTION: STBC 0x006e14b0 _$E127
// FUNCTION: STBC 0x006e14c0 _$E126
// FUNCTION: STBC 0x006e14d0 _$E344
// FUNCTION: STBC 0x006e14e0 _$E343
// FUNCTION: STBC 0x006e14f0 _$E348
// FUNCTION: STBC 0x006e1500 _$E347
// FUNCTION: STBC 0x006e1510 _$E352
// FUNCTION: STBC 0x006e1520 _$E351
// FUNCTION: STBC 0x006e1530 _$E133
// FUNCTION: STBC 0x006e1540 _$E132
// FUNCTION: STBC 0x006e1550 _$E138
// FUNCTION: STBC 0x006e1560 _$E137
// FUNCTION: STBC 0x006e1570 _$E168
// FUNCTION: STBC 0x006e1580 _$E167
// FUNCTION: STBC 0x006e1590 _$E356
// FUNCTION: STBC 0x006e15a0 _$E355
// FUNCTION: STBC 0x006e15b0 _$E173
// FUNCTION: STBC 0x006e15c0 _$E172
// FUNCTION: STBC 0x006e15d0 _$E360
// FUNCTION: STBC 0x006e15e0 _$E359
// FUNCTION: STBC 0x006e15f0 _$E364
// FUNCTION: STBC 0x006e1600 _$E363
// FUNCTION: STBC 0x006e1610 _$E178
// FUNCTION: STBC 0x006e1620 _$E177
// FUNCTION: STBC 0x006e1630 _$E368
// FUNCTION: STBC 0x006e1640 _$E367
// FUNCTION: STBC 0x006e1650 _$E143
// FUNCTION: STBC 0x006e1660 _$E142
// FUNCTION: STBC 0x006e1670 _$E372
// FUNCTION: STBC 0x006e1680 _$E371
// FUNCTION: STBC 0x006e1690 _$E376
// FUNCTION: STBC 0x006e16a0 _$E375
// FUNCTION: STBC 0x006e16b0 _$E183
// FUNCTION: STBC 0x006e16c0 _$E182
// FUNCTION: STBC 0x006e16d0 _$E188
// FUNCTION: STBC 0x006e16e0 _$E187
// FUNCTION: STBC 0x006e16f0 _$E380
// FUNCTION: STBC 0x006e1700 _$E379
// FUNCTION: STBC 0x006e1710 _$E384
// FUNCTION: STBC 0x006e1720 _$E383
// FUNCTION: STBC 0x006e1730 _$E388
// FUNCTION: STBC 0x006e1740 _$E387
// FUNCTION: STBC 0x006e1750 _$E392
// FUNCTION: STBC 0x006e1760 _$E391
// FUNCTION: STBC 0x006e1770 _$E396
// FUNCTION: STBC 0x006e1780 _$E395
// FUNCTION: STBC 0x006e1790 _$E400
// FUNCTION: STBC 0x006e17a0 _$E399
// FUNCTION: STBC 0x006e17b0 _$E404
// FUNCTION: STBC 0x006e17c0 _$E403
// FUNCTION: STBC 0x006e17d0 _$E408
// FUNCTION: STBC 0x006e17e0 _$E407
// FUNCTION: STBC 0x006e17f0 _$E193
// FUNCTION: STBC 0x006e1800 _$E192
// FUNCTION: STBC 0x006e1810 _$E412
// FUNCTION: STBC 0x006e1820 _$E411
// FUNCTION: STBC 0x006e1830 _$E198
// FUNCTION: STBC 0x006e1840 _$E197
// FUNCTION: STBC 0x006e1850 _$E203
// FUNCTION: STBC 0x006e1860 _$E202

/*
================
TGBroadcastHandlerList::TGBroadcastHandlerList
================
*/
// FUNCTION: STBC 0x006e1870
TGBroadcastHandlerList::TGBroadcastHandlerList( void ) : handlers( 2, 4 ) {
	callDepth = 0;
}

/*
================
TGBroadcastHandlerList::~TGBroadcastHandlerList
================
*/
// FUNCTION: STBC 0x006e1900
TGBroadcastHandlerList::~TGBroadcastHandlerList( void ) {
	RemoveAllHandlers();
}

/*
================
TGBroadcastHandlerList::WriteToStream

The number of slots, then for each a flag for a handler, the ID of its
destination (0 when that object is gone) and the handler.
================
*/
// FUNCTION: STBC 0x006e1960
bool TGBroadcastHandlerList::WriteToStream( TGStream *stream ) {
	bool result = true;

	// BUG: writes the non-null count but a flag per slot; equal as long as the array has no holes, which RemoveAt keeps true
	stream->WriteInt( handlers.GetEffectiveSize() );
	for ( unsigned int i = 0; i < handlers.GetSize(); i++ ) {
		TGBroadcastHandler *entry = handlers.GetAt( i );
		if ( entry != NULL ) {
			stream->WriteBool( true );
			if ( entry->destination != NULL ) {
				stream->WriteBool( true );
				long id = entry->destination->GetObjID();
				if ( TGObject::GetTGObjectPtr( id ) != NULL ) {
					stream->WriteID( id );
				} else {
					stream->WriteID( 0 );
				}
			} else {
				stream->WriteBool( false );
			}
			if ( !entry->handler->WriteToStream( stream ) ) {
				result = false;
			}
		} else {
			stream->WriteBool( false );
		}
	}
	return result;
}

/*
================
TGBroadcastHandlerList::ReadFromStream

Reads what WriteToStream wrote; the destinations stay IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x006e1a30
bool TGBroadcastHandlerList::ReadFromStream( TGStream *stream ) {
	bool result = true;

	int numSlots = stream->ReadInt();
	for ( int i = 0; i < numSlots; i++ ) {
		if ( stream->ReadBool() != true ) {
			continue;
		}

		TGBroadcastHandler *entry = new TGBroadcastHandler;
		if ( entry != NULL ) {
			if ( stream->ReadBool() == true ) {
				entry->destination = (TGEventHandlerObject *)stream->ReadID();
			} else {
				entry->destination = NULL;
			}

			TGHandler *handler = new TGHandler;
			if ( handler != NULL ) {
				if ( !handler->ReadFromStream( stream ) ) {
					result = false;
				}
				entry->handler = handler;
				handlers.Add( entry );
				continue;
			}
		}
		result = false;
	}
	return result;
}

/*
================
TGBroadcastHandlerList::FixUpPointers

False when a destination is gone or a handler cannot fix up its own.
================
*/
// FUNCTION: STBC 0x006e1c00
bool TGBroadcastHandlerList::FixUpPointers( void ) {
	for ( unsigned int i = 0; i < handlers.GetSize(); i++ ) {
		TGBroadcastHandler *entry = handlers.GetAt( i );
		if ( entry == NULL ) {
			continue;
		}
		if ( entry->destination != NULL && TGObject::GetTGObjectPtr( (long)entry->destination ) == NULL ) {
			return false;
		}
		if ( entry->handler != NULL && !entry->handler->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
TGBroadcastHandlerList::PostFixUp

Turns the destination IDs back into pointers.
================
*/
// FUNCTION: STBC 0x006e1c50
void TGBroadcastHandlerList::PostFixUp( void ) {
	for ( unsigned int i = 0; i < handlers.GetSize(); i++ ) {
		TGBroadcastHandler *entry = handlers.GetAt( i );
		if ( entry == NULL ) {
			continue;
		}
		if ( entry->destination != NULL ) {
			entry->destination = (TGEventHandlerObject *)TGObject::GetTGObjectPtr( (long)entry->destination );
		}
		if ( entry->handler != NULL ) {
			entry->handler->PostFixUp();
		}
	}
}

/*
================
TGBroadcastHandlerList::ForgetHandlers
================
*/
// FUNCTION: STBC 0x006e1ca0
void TGBroadcastHandlerList::ForgetHandlers( void ) {
	handlers.RemoveAll();
}

/*
================
TGBroadcastHandlerList::AddHandler
================
*/
// FUNCTION: STBC 0x006e1cd0
bool TGBroadcastHandlerList::AddHandler( TGEventHandlerObject *destination, TGHandler *handler ) {
	TGBroadcastHandler *entry = new TGBroadcastHandler;
	if ( entry != NULL ) {
		entry->destination = destination;
		entry->handler = handler;
		return Insert( entry );
	}
	return false;
}

/*
================
TGBroadcastHandlerList::Insert

Puts the handler after those with the same target ID, keeping the list
sorted; while the list is calling, it waits in the pending array.
================
*/
// FUNCTION: STBC 0x006e1d60
bool TGBroadcastHandlerList::Insert( TGBroadcastHandler *entry ) {
	if ( callDepth != 0 ) {
		pending.Add( entry );
		return true;
	}

	unsigned int index = FindInsertIndex( entry->GetTargetID() );
	unsigned int size = handlers.GetSize();
	if ( index >= size ) {
		handlers.Add( entry );
		return true;
	}

	handlers.Add( handlers.GetAt( size - 1 ) );
	for ( int i = size - 2; i >= (int)index; i-- ) {
		handlers.SetAt( i + 1, handlers.GetAt( i ) );
	}
	handlers.SetAt( index, entry );
	return true;
}

/*
================
TGBroadcastHandlerList::RemoveHandler

Removes the handler with the name, sent to target, of destination: a
registered C++ handler when one is registered under the name, else a Python
one. A pending handler is deleted, but the result is then false.
================
*/
// FUNCTION: STBC 0x006e1ed0
bool TGBroadcastHandlerList::RemoveHandler( TGEventHandlerObject *destination, char *name, TGEventHandlerObject *target ) {
	TGHandlerFunction function = handlerRegistry.Find( TGHashString( name ) );

	for ( unsigned int i = 0; i < handlers.GetSize(); i++ ) {
		if ( IsHandler( handlers.GetAt( i ), destination, function, name, target ) ) {
			return RemoveAt( i );
		}
	}

	for ( int j = pending.GetSize() - 1; j >= 0; j-- ) {
		TGBroadcastHandler *entry = pending.GetAt( j );
		if ( IsHandler( entry, destination, function, name, target ) ) {
			delete entry;
			if ( j < (int)pending.GetSize() - 1 ) {
				pending.SetAt( j, pending.RemoveEnd() );
			} else {
				pending.RemoveEnd();
			}
		}
	}
	return false;
}

/*
================
TGBroadcastHandlerList::IsHandler

The handler is the registered function (or, without one, the Python
handler with the name) for target on behalf of destination.
================
*/
// FUNCTION: STBC 0x006e2030
bool TGBroadcastHandlerList::IsHandler( TGBroadcastHandler *entry, TGEventHandlerObject *destination, TGHandlerFunction function, char *name, TGEventHandlerObject *target ) {
	if ( entry->handler->target == target && entry->destination == destination ) {
		if ( function != NULL ) {
			if ( entry->handler->function == function ) {
				return true;
			}
		} else if ( strcmp( entry->handler->pythonName, name ) == 0 ) {
			return true;
		}
	}
	return false;
}

/*
================
TGBroadcastHandlerList::RemoveAllHandlersForObject

True when a handler of the sorted array went.
================
*/
// FUNCTION: STBC 0x006e20b0
bool TGBroadcastHandlerList::RemoveAllHandlersForObject( TGEventHandlerObject *object ) {
	bool removed = false;

	for ( int i = handlers.GetSize() - 1; i >= 0; i-- ) {
		TGBroadcastHandler *entry = handlers.GetAt( i );
		if ( entry->destination == object || entry->handler->target == object ) {
			RemoveAt( i );
			removed = true;
		}
	}

	for ( int j = pending.GetSize() - 1; j >= 0; j-- ) {
		TGBroadcastHandler *entry = pending.GetAt( j );
		if ( entry->destination == object || entry->handler->target == object ) {
			delete entry;
			if ( j < (int)pending.GetSize() - 1 ) {
				pending.SetAt( j, pending.RemoveEnd() );
			} else {
				pending.RemoveEnd();
			}
		}
	}
	return removed;
}

/*
================
TGBroadcastHandlerList::CallHandlers

The handlers for any target come first in the sorted array; then those of
the event's destination. The outermost call deletes the handlers removed
meanwhile and inserts the ones added.
================
*/
// FUNCTION: STBC 0x006e21d0
// DRAFT: calls and branches agree; retail keeps the event in edi and the destination in the argument's slot, this build the event in ebp
void TGBroadcastHandlerList::CallHandlers( TGEvent *event ) {
	callDepth++;

	long destinationID = 0;
	TGEventHandlerObject *destination = event->GetDestination();
	if ( destination != NULL ) {
		destinationID = destination->GetObjID();
	}

	unsigned int i;
	for ( i = 0; i < handlers.GetSize(); i++ ) {
		TGBroadcastHandler *entry = handlers.GetAt( i );
		if ( entry->removed ) {
			continue;
		}
		if ( entry->GetTargetID() != 0 ) {
			break;
		}
		TGHandler *handler = entry->handler;
		if ( handler->target == ANY_TARGET || ( handler->target == NULL && destination == NULL ) ) {
			event->currentHandler = handler;
			event->currentTable = NULL;
			handler->Call( event, entry->destination );
		}
	}

	if ( destinationID != 0 ) {
		for ( i = FindFirst( destinationID ); i < handlers.GetSize(); i++ ) {
			TGBroadcastHandler *entry = handlers.GetAt( i );
			if ( entry->removed ) {
				continue;
			}
			if ( entry->GetTargetID() != destinationID ) {
				break;
			}
			TGHandler *handler = entry->handler;
			event->currentTable = NULL;
			event->currentHandler = handler;
			handler->Call( event, entry->destination );
		}
	}

	if ( --callDepth == 0 ) {
		i = handlers.GetSize();
		while ( i != 0 ) {
			i--;
			if ( handlers.GetAt( i )->removed ) {
				RemoveAt( i );
			}
		}
		while ( pending.GetSize() != 0 ) {
			Insert( pending.RemoveEnd() );
		}
	}
}

/*
================
TGBroadcastHandlerList::RemoveAllHandlers
================
*/
// FUNCTION: STBC 0x006e2310
void TGBroadcastHandlerList::RemoveAllHandlers( void ) {
	for ( int i = handlers.GetSize() - 1; i >= 0; i-- ) {
		RemoveAt( i );
	}
}

/*
================
TGBroadcastHandlerList::FindInsertIndex

The index after the last handler whose target ID is not above the ID.
================
*/
// FUNCTION: STBC 0x006e2330
unsigned int TGBroadcastHandlerList::FindInsertIndex( long targetID ) {
	int low = 0;
	int high = handlers.GetSize();
	while ( low != high ) {
		int mid = ( low + high ) / 2;
		if ( handlers.GetAt( mid )->GetTargetID() <= targetID ) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

/*
================
TGBroadcastHandlerList::FindFirst

The index of the first handler with the target ID, or where it would go.
================
*/
// FUNCTION: STBC 0x006e2380
// DRAFT: same blocks; retail ends the search loop with "je exit; jmp top" and puts the not-found return last
unsigned int TGBroadcastHandlerList::FindFirst( long targetID ) {
	int low = 0;
	int high = handlers.GetSize();
	while ( low != high ) {
		int mid = ( low + high ) / 2;
		long id = handlers.GetAt( mid )->GetTargetID();
		if ( id < targetID ) {
			low = mid + 1;
		} else if ( id > targetID ) {
			high = mid;
		} else {
			do {
				mid--;
			} while ( mid >= 0 && handlers.GetAt( mid )->GetTargetID() == targetID );
			return mid + 1;
		}
	}
	return low;
}

/*
================
TGBroadcastHandlerList::RemoveAt

Deletes the handler and closes the gap; while the list is calling, the
handler is only marked.
================
*/
// FUNCTION: STBC 0x006e23f0
bool TGBroadcastHandlerList::RemoveAt( unsigned int index ) {
	TGBroadcastHandler *entry = handlers.GetAt( index );
	if ( callDepth != 0 ) {
		entry->removed = true;
		return true;
	}

	delete entry;
	for ( unsigned int i = index + 1; i < handlers.GetSize(); i++ ) {
		handlers.SetAt( i - 1, handlers.GetAt( i ) );
	}
	handlers.RemoveEnd();
	return true;
}
