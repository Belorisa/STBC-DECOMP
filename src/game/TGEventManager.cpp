/*
===============================================================================

	TGEventManager.cpp

===============================================================================
*/

#include <iostream>
#include "TGEventManager.h"
#include "TGHandler.h"
#include "TGEvent.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d9610 _$E431
// FUNCTION: STBC 0x006d9630 _$E430
// FUNCTION: STBC 0x006d9640 _$E32
// FUNCTION: STBC 0x006d9650 _$E29
// FUNCTION: STBC 0x006d9660 _$E31
// FUNCTION: STBC 0x006d9670 _$E30
// FUNCTION: STBC 0x006d9680 _$E38
// FUNCTION: STBC 0x006d9690 _$E35
// FUNCTION: STBC 0x006d96a0 _$E37
// FUNCTION: STBC 0x006d96b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d96c0 _$E59
// FUNCTION: STBC 0x006d96d0 _$E58
// FUNCTION: STBC 0x006d96e0 _$E63
// FUNCTION: STBC 0x006d96f0 _$E62
// FUNCTION: STBC 0x006d9700 _$E74
// FUNCTION: STBC 0x006d9710 _$E73
// FUNCTION: STBC 0x006d9720 _$E78
// FUNCTION: STBC 0x006d9730 _$E77
// FUNCTION: STBC 0x006d9740 _$E82
// FUNCTION: STBC 0x006d9750 _$E81
// FUNCTION: STBC 0x006d9760 _$E86
// FUNCTION: STBC 0x006d9770 _$E85
// FUNCTION: STBC 0x006d9780 _$E210
// FUNCTION: STBC 0x006d9790 _$E209
// FUNCTION: STBC 0x006d97a0 _$E152
// FUNCTION: STBC 0x006d97b0 _$E151
// FUNCTION: STBC 0x006d97c0 _$E165
// FUNCTION: STBC 0x006d97d0 _$E164
// FUNCTION: STBC 0x006d97e0 _$E215
// FUNCTION: STBC 0x006d97f0 _$E214
// FUNCTION: STBC 0x006d9800 _$E220
// FUNCTION: STBC 0x006d9810 _$E219
// FUNCTION: STBC 0x006d9820 _$E224
// FUNCTION: STBC 0x006d9830 _$E223
// FUNCTION: STBC 0x006d9840 _$E228
// FUNCTION: STBC 0x006d9850 _$E227
// FUNCTION: STBC 0x006d9860 _$E235
// FUNCTION: STBC 0x006d9870 _$E234
// FUNCTION: STBC 0x006d9880 _$E260
// FUNCTION: STBC 0x006d9890 _$E259
// FUNCTION: STBC 0x006d98a0 _$E264
// FUNCTION: STBC 0x006d98b0 _$E263
// FUNCTION: STBC 0x006d98c0 _$E110
// FUNCTION: STBC 0x006d98d0 _$E109
// FUNCTION: STBC 0x006d98e0 _$E246
// FUNCTION: STBC 0x006d98f0 _$E245
// FUNCTION: STBC 0x006d9900 _$E253
// FUNCTION: STBC 0x006d9910 _$E252
// FUNCTION: STBC 0x006d9920 _$E276
// FUNCTION: STBC 0x006d9930 _$E275
// FUNCTION: STBC 0x006d9940 _$E280
// FUNCTION: STBC 0x006d9950 _$E279
// FUNCTION: STBC 0x006d9960 _$E320
// FUNCTION: STBC 0x006d9970 _$E319
// FUNCTION: STBC 0x006d9980 _$E326
// FUNCTION: STBC 0x006d9990 _$E325
// FUNCTION: STBC 0x006d99a0 _$E268
// FUNCTION: STBC 0x006d99b0 _$E267
// FUNCTION: STBC 0x006d99c0 _$E272
// FUNCTION: STBC 0x006d99d0 _$E271
// FUNCTION: STBC 0x006d99e0 _$E284
// FUNCTION: STBC 0x006d99f0 _$E283
// FUNCTION: STBC 0x006d9a00 _$E288
// FUNCTION: STBC 0x006d9a10 _$E287
// FUNCTION: STBC 0x006d9a20 _$E292
// FUNCTION: STBC 0x006d9a30 _$E291
// FUNCTION: STBC 0x006d9a40 _$E296
// FUNCTION: STBC 0x006d9a50 _$E295
// FUNCTION: STBC 0x006d9a60 _$E300
// FUNCTION: STBC 0x006d9a70 _$E299
// FUNCTION: STBC 0x006d9a80 _$E304
// FUNCTION: STBC 0x006d9a90 _$E303
// FUNCTION: STBC 0x006d9aa0 _$E308
// FUNCTION: STBC 0x006d9ab0 _$E307
// FUNCTION: STBC 0x006d9ac0 _$E312
// FUNCTION: STBC 0x006d9ad0 _$E311
// FUNCTION: STBC 0x006d9ae0 _$E115
// FUNCTION: STBC 0x006d9af0 _$E114
// FUNCTION: STBC 0x006d9b00 _$E316
// FUNCTION: STBC 0x006d9b10 _$E315
// FUNCTION: STBC 0x006d9b20 _$E120
// FUNCTION: STBC 0x006d9b30 _$E119
// FUNCTION: STBC 0x006d9b40 _$E330
// FUNCTION: STBC 0x006d9b50 _$E329
// FUNCTION: STBC 0x006d9b60 _$E334
// FUNCTION: STBC 0x006d9b70 _$E333
// FUNCTION: STBC 0x006d9b80 _$E338
// FUNCTION: STBC 0x006d9b90 _$E337
// FUNCTION: STBC 0x006d9ba0 _$E342
// FUNCTION: STBC 0x006d9bb0 _$E341
// FUNCTION: STBC 0x006d9bc0 _$E129
// FUNCTION: STBC 0x006d9bd0 _$E128
// FUNCTION: STBC 0x006d9be0 _$E346
// FUNCTION: STBC 0x006d9bf0 _$E345
// FUNCTION: STBC 0x006d9c00 _$E350
// FUNCTION: STBC 0x006d9c10 _$E349
// FUNCTION: STBC 0x006d9c20 _$E354
// FUNCTION: STBC 0x006d9c30 _$E353
// FUNCTION: STBC 0x006d9c40 _$E135
// FUNCTION: STBC 0x006d9c50 _$E134
// FUNCTION: STBC 0x006d9c60 _$E140
// FUNCTION: STBC 0x006d9c70 _$E139
// FUNCTION: STBC 0x006d9c80 _$E170
// FUNCTION: STBC 0x006d9c90 _$E169
// FUNCTION: STBC 0x006d9ca0 _$E358
// FUNCTION: STBC 0x006d9cb0 _$E357
// FUNCTION: STBC 0x006d9cc0 _$E175
// FUNCTION: STBC 0x006d9cd0 _$E174
// FUNCTION: STBC 0x006d9ce0 _$E362
// FUNCTION: STBC 0x006d9cf0 _$E361
// FUNCTION: STBC 0x006d9d00 _$E366
// FUNCTION: STBC 0x006d9d10 _$E365
// FUNCTION: STBC 0x006d9d20 _$E180
// FUNCTION: STBC 0x006d9d30 _$E179
// FUNCTION: STBC 0x006d9d40 _$E370
// FUNCTION: STBC 0x006d9d50 _$E369
// FUNCTION: STBC 0x006d9d60 _$E145
// FUNCTION: STBC 0x006d9d70 _$E144
// FUNCTION: STBC 0x006d9d80 _$E374
// FUNCTION: STBC 0x006d9d90 _$E373
// FUNCTION: STBC 0x006d9da0 _$E378
// FUNCTION: STBC 0x006d9db0 _$E377
// FUNCTION: STBC 0x006d9dc0 _$E185
// FUNCTION: STBC 0x006d9dd0 _$E184
// FUNCTION: STBC 0x006d9de0 _$E190
// FUNCTION: STBC 0x006d9df0 _$E189
// FUNCTION: STBC 0x006d9e00 _$E382
// FUNCTION: STBC 0x006d9e10 _$E381
// FUNCTION: STBC 0x006d9e20 _$E386
// FUNCTION: STBC 0x006d9e30 _$E385
// FUNCTION: STBC 0x006d9e40 _$E390
// FUNCTION: STBC 0x006d9e50 _$E389
// FUNCTION: STBC 0x006d9e60 _$E394
// FUNCTION: STBC 0x006d9e70 _$E393
// FUNCTION: STBC 0x006d9e80 _$E398
// FUNCTION: STBC 0x006d9e90 _$E397
// FUNCTION: STBC 0x006d9ea0 _$E402
// FUNCTION: STBC 0x006d9eb0 _$E401
// FUNCTION: STBC 0x006d9ec0 _$E406
// FUNCTION: STBC 0x006d9ed0 _$E405
// FUNCTION: STBC 0x006d9ee0 _$E410
// FUNCTION: STBC 0x006d9ef0 _$E409
// FUNCTION: STBC 0x006d9f00 _$E195
// FUNCTION: STBC 0x006d9f10 _$E194
// FUNCTION: STBC 0x006d9f20 _$E414
// FUNCTION: STBC 0x006d9f30 _$E413
// FUNCTION: STBC 0x006d9f40 _$E200
// FUNCTION: STBC 0x006d9f50 _$E199
// FUNCTION: STBC 0x006d9f60 _$E205
// FUNCTION: STBC 0x006d9f70 _$E204

// The constructors and destructors of classTables and handlerRegistry.
// FUNCTION: STBC 0x006d9f80 _$E420
// FUNCTION: STBC 0x006d9f90 _$E416
// FUNCTION: STBC 0x006d9fd0 _$E419
// FUNCTION: STBC 0x006d9fe0 _$E417
// FUNCTION: STBC 0x006da010 _$E423
// FUNCTION: STBC 0x006da020 _$E422

// DATA: STBC 0x00998728 ?classTables@TGEventHandlerObject@@2V?$NiTArray@PAVTGEventHandlerTable@@@@A
NiTArray<TGEventHandlerTable *> TGEventHandlerObject::classTables;

// DATA: STBC 0x00998638 ?handlerRegistry@@3VTGHandlerRegistry@@A
TGHandlerRegistry handlerRegistry;

// DATA: STBC 0x00998744 ?instance@TGEventManager@@2PAV1@A
TGEventManager *TGEventManager::instance;

// DATA: STBC 0x00998748 ?isAlive@TGEventManager@@2_NA
bool TGEventManager::isAlive;

/*
================
TGEventManager::TGEventManager
================
*/
// FUNCTION: STBC 0x006da040
TGEventManager::TGEventManager( void ) {
	instance = this;
	isAlive = true;
}

/*
================
TGEventManager::~TGEventManager
================
*/
// FUNCTION: STBC 0x006da0b0
TGEventManager::~TGEventManager( void ) {
	handlerRegistry.Clear();
	isAlive = false;
}

/*
================
TGEventManager::RegisterHandlerMethod

Makes a C++ handler method known under the hash of its name, for the
handlers that are set by name.
================
*/
// FUNCTION: STBC 0x006da130 ?RegisterHandlerMethod@TGEventManager@@QAEXP8TGEventHandlerObject@@AEXPAVTGEvent@@@ZPAD@Z
void TGEventManager::RegisterHandlerMethod( TGHandlerMethod method, char *name ) {
	handlerRegistry.Add( NULL, method, TGHashString( name ) );
}

/*
================
TGEventManager::RegisterHandlerFunction

Makes a C++ handler function known under the hash of its name.
================
*/
// FUNCTION: STBC 0x006da160 ?RegisterHandlerFunction@TGEventManager@@QAEXP6AXPAVTGEventHandlerObject@@PAVTGEvent@@@ZPAD@Z
void TGEventManager::RegisterHandlerFunction( TGHandlerFunction function, char *name ) {
	handlerRegistry.Add( function, NULL, TGHashString( name ) );
}

/*
================
TGEventManager::WriteToStream

The queues, the handler table at 0x18 and the broadcast handlers.
================
*/
// FUNCTION: STBC 0x006da190
bool TGEventManager::WriteToStream( TGStream *stream ) {
	if ( !events.WriteToStream( stream ) ) {
		return false;
	}
	if ( !loggedEvents.WriteToStream( stream ) ) {
		return false;
	}
	if ( !unknown18.WriteToStream( stream ) ) {
		return false;
	}
	if ( !broadcastHandlers.WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGEventManager::ReadFromStream

Reads what WriteToStream wrote; the broadcast handlers there are deleted first.
================
*/
// FUNCTION: STBC 0x006da1e0
bool TGEventManager::ReadFromStream( TGStream *stream ) {
	if ( !events.ReadFromStream( stream ) ) {
		return false;
	}
	if ( !loggedEvents.ReadFromStream( stream ) ) {
		return false;
	}
	if ( !unknown18.ReadFromStream( stream ) ) {
		return false;
	}
	broadcastHandlers.RemoveAllHandlers();
	if ( !broadcastHandlers.ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGEventManager::FixUpPointers

Only the first queue and the broadcast handlers hold IDs to fix up.
================
*/
// FUNCTION: STBC 0x006da240
bool TGEventManager::FixUpPointers( void ) {
	if ( !events.FixUpPointers() ) {
		return false;
	}
	if ( !broadcastHandlers.FixUpPointers() ) {
		return false;
	}
	return true;
}

/*
================
TGEventManager::PostFixUp
================
*/
// FUNCTION: STBC 0x006da260
void TGEventManager::PostFixUp( void ) {
	events.PostFixUp();
	broadcastHandlers.PostFixUp();
}

/*
================
TGEventManager::ForgetAll
================
*/
// FUNCTION: STBC 0x006da280
void TGEventManager::ForgetAll( void ) {
	events.Unknown6DE300();
	broadcastHandlers.ForgetHandlers();
}

/*
================
TGEventManager::AddEvent
================
*/
// FUNCTION: STBC 0x006da2a0 ?AddEvent@TGEventManager@@QAE_NPAVTGEvent@@@Z
bool TGEventManager::AddEvent( TGEvent *event ) {
	return events.Unknown6DE330( event );
}

/*
================
TGEventManager::Unknown6DA2B0

Appends the event to the second queue.
================
*/
// FUNCTION: STBC 0x006da2b0
bool TGEventManager::Unknown6DA2B0( TGEvent *event ) {
	return loggedEvents.Unknown6DE330( event );
}

/*
================
TGEventManager::Unknown6DA2C0

Sends the queued events one after another, each taken off the queue first;
false when any of them was refused.
================
*/
// FUNCTION: STBC 0x006da2c0
bool TGEventManager::Unknown6DA2C0( void ) {
	bool result = true;
	while ( events.GetCount() != 0 ) {
		TGEvent *event = events.GetHead();
		event->IncRefCount();
		events.Unknown6DE370();
		result &= ProcessEvent( event );
		event->DecRefCount();
	}
	return result;
}

/*
================
TGEventManager::ProcessEvent

Sends the event at once: to its destination, to the broadcast handlers unless
it is private, and on to Unknown6DA2B0 when it is logged. False when any of
them refuses it.
================
*/
// FUNCTION: STBC 0x006da300 ?ProcessEvent@TGEventManager@@QAE_NPAVTGEvent@@@Z
bool TGEventManager::ProcessEvent( TGEvent *event ) {
	bool result = true;
	TGEventHandlerObject *destination = event->GetDestination();
	if ( destination == INVALID_DESTINATION ) {
		return result;
	}
	if ( destination == NULL || !destination->ProcessEvent( event ) ) {
		result = false;
	}
	if ( !event->IsPrivate() ) {
		if ( !broadcastHandlers.CallHandlers( event ) ) {
			result = false;
		}
	}
	if ( event->IsLogged() ) {
		if ( !Unknown6DA2B0( event ) ) {
			result = false;
		}
	}
	return result;
}

/*
================
TGEventManager::ClearEvents
================
*/
// FUNCTION: STBC 0x006da370
void TGEventManager::ClearEvents( void ) {
	events.Unknown6DE310();
}

/*
================
TGEventManager::ClearLoggedEvents
================
*/
// FUNCTION: STBC 0x006da380
void TGEventManager::ClearLoggedEvents( void ) {
	loggedEvents.Unknown6DE310();
}

/*
================
TGEventManager::RemoveAllBroadcastHandlers
================
*/
// FUNCTION: STBC 0x006da390
void TGEventManager::RemoveAllBroadcastHandlers( void ) {
	broadcastHandlers.RemoveAllHandlers();
}

/*
================
TGEventManager::RemoveAllClassHandlers
================
*/
// FUNCTION: STBC 0x006da3a0
void TGEventManager::RemoveAllClassHandlers( void ) {
	int numTables = TGEventHandlerObject::classTables.GetSize();
	for ( int i = 0; i < numTables; i++ ) {
		TGEventHandlerTable *table = TGEventHandlerObject::classTables.GetAt( i );
		if ( table != NULL ) {
			table->RemoveAllHandlers();
		}
	}
}

/*
================
TGHandlerRegistry::Add
================
*/
// FUNCTION: STBC 0x006da3d0 ?Add@TGHandlerRegistry@@QAE_NP6AXPAVTGEventHandlerObject@@PAVTGEvent@@@ZP82@AEX1@ZH@Z
bool TGHandlerRegistry::Add( TGHandlerFunction function, TGHandlerMethod method, int nameHash ) {
	if ( Find( nameHash ) == NULL ) {
		if ( count * sizeof( TGHandlerRegistryEntry ) == allocatedSize ) {
			allocatedSize += 4 * sizeof( TGHandlerRegistryEntry );
			entries = (TGHandlerRegistryEntry *)realloc( entries, allocatedSize );
		}
		if ( entries != NULL ) {
			if ( function != NULL ) {
				entries[count].function = function;
			} else {
				entries[count].method = method;
			}
			entries[count].nameHash = nameHash;
			count++;
			return true;
		}
	}
	return false;
}

/*
================
TGHandlerRegistry::Find

The function or method registered under the hash; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006da450 ?Find@TGHandlerRegistry@@QAEP6AXPAVTGEventHandlerObject@@PAVTGEvent@@@ZH@Z
TGHandlerFunction TGHandlerRegistry::Find( int nameHash ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		if ( entries[i].nameHash == nameHash ) {
			return entries[i].function;
		}
	}
	return NULL;
}

/*
================
TGHandlerRegistry::FindNameHash

The hash a function or method is registered under; 0 when it is not.
================
*/
// FUNCTION: STBC 0x006da480 ?FindNameHash@TGHandlerRegistry@@QAEHP6AXPAVTGEventHandlerObject@@PAVTGEvent@@@Z@Z
int TGHandlerRegistry::FindNameHash( TGHandlerFunction function ) {
	for ( unsigned int i = 0; i < count; i++ ) {
		if ( entries[i].function == function ) {
			return entries[i].nameHash;
		}
	}
	return 0;
}

/*
================
TGHandlerRegistry::Clear
================
*/
// FUNCTION: STBC 0x006da4b0
void TGHandlerRegistry::Clear( void ) {
	if ( entries != NULL ) {
		free( entries );
	}
	entries = NULL;
	count = 0;
	allocatedSize = 0;
}
