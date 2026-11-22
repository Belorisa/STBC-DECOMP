/*
===============================================================================

	TGEventHandlerObject.cpp

===============================================================================
*/

#include <iostream>
#include "TGEventHandlerObject.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006d85a0 _$E428
// FUNCTION: STBC 0x006d85c0 _$E427
// FUNCTION: STBC 0x006d85d0 _$E32
// FUNCTION: STBC 0x006d85e0 _$E29
// FUNCTION: STBC 0x006d85f0 _$E31
// FUNCTION: STBC 0x006d8600 _$E30
// FUNCTION: STBC 0x006d8610 _$E38
// FUNCTION: STBC 0x006d8620 _$E35
// FUNCTION: STBC 0x006d8630 _$E37
// FUNCTION: STBC 0x006d8640 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006d8650 _$E59
// FUNCTION: STBC 0x006d8660 _$E58
// FUNCTION: STBC 0x006d8670 _$E63
// FUNCTION: STBC 0x006d8680 _$E62
// FUNCTION: STBC 0x006d8690 _$E74
// FUNCTION: STBC 0x006d86a0 _$E73
// FUNCTION: STBC 0x006d86b0 _$E78
// FUNCTION: STBC 0x006d86c0 _$E77
// FUNCTION: STBC 0x006d86d0 _$E82
// FUNCTION: STBC 0x006d86e0 _$E81
// FUNCTION: STBC 0x006d86f0 _$E86
// FUNCTION: STBC 0x006d8700 _$E85
// FUNCTION: STBC 0x006d8710 _$E210
// FUNCTION: STBC 0x006d8720 _$E209
// FUNCTION: STBC 0x006d8730 _$E152
// FUNCTION: STBC 0x006d8740 _$E151
// FUNCTION: STBC 0x006d8750 _$E165
// FUNCTION: STBC 0x006d8760 _$E164
// FUNCTION: STBC 0x006d8770 _$E215
// FUNCTION: STBC 0x006d8780 _$E214
// FUNCTION: STBC 0x006d8790 _$E220
// FUNCTION: STBC 0x006d87a0 _$E219
// FUNCTION: STBC 0x006d87b0 _$E224
// FUNCTION: STBC 0x006d87c0 _$E223
// FUNCTION: STBC 0x006d87d0 _$E228
// FUNCTION: STBC 0x006d87e0 _$E227
// FUNCTION: STBC 0x006d87f0 _$E235
// FUNCTION: STBC 0x006d8800 _$E234
// FUNCTION: STBC 0x006d8810 _$E260
// FUNCTION: STBC 0x006d8820 _$E259
// FUNCTION: STBC 0x006d8830 _$E264
// FUNCTION: STBC 0x006d8840 _$E263
// FUNCTION: STBC 0x006d8850 _$E110
// FUNCTION: STBC 0x006d8860 _$E109
// FUNCTION: STBC 0x006d8870 _$E246
// FUNCTION: STBC 0x006d8880 _$E245
// FUNCTION: STBC 0x006d8890 _$E253
// FUNCTION: STBC 0x006d88a0 _$E252
// FUNCTION: STBC 0x006d88b0 _$E276
// FUNCTION: STBC 0x006d88c0 _$E275
// FUNCTION: STBC 0x006d88d0 _$E280
// FUNCTION: STBC 0x006d88e0 _$E279
// FUNCTION: STBC 0x006d88f0 _$E320
// FUNCTION: STBC 0x006d8900 _$E319
// FUNCTION: STBC 0x006d8910 _$E326
// FUNCTION: STBC 0x006d8920 _$E325
// FUNCTION: STBC 0x006d8930 _$E268
// FUNCTION: STBC 0x006d8940 _$E267
// FUNCTION: STBC 0x006d8950 _$E272
// FUNCTION: STBC 0x006d8960 _$E271
// FUNCTION: STBC 0x006d8970 _$E284
// FUNCTION: STBC 0x006d8980 _$E283
// FUNCTION: STBC 0x006d8990 _$E288
// FUNCTION: STBC 0x006d89a0 _$E287
// FUNCTION: STBC 0x006d89b0 _$E292
// FUNCTION: STBC 0x006d89c0 _$E291
// FUNCTION: STBC 0x006d89d0 _$E296
// FUNCTION: STBC 0x006d89e0 _$E295
// FUNCTION: STBC 0x006d89f0 _$E300
// FUNCTION: STBC 0x006d8a00 _$E299
// FUNCTION: STBC 0x006d8a10 _$E304
// FUNCTION: STBC 0x006d8a20 _$E303
// FUNCTION: STBC 0x006d8a30 _$E308
// FUNCTION: STBC 0x006d8a40 _$E307
// FUNCTION: STBC 0x006d8a50 _$E312
// FUNCTION: STBC 0x006d8a60 _$E311
// FUNCTION: STBC 0x006d8a70 _$E115
// FUNCTION: STBC 0x006d8a80 _$E114
// FUNCTION: STBC 0x006d8a90 _$E316
// FUNCTION: STBC 0x006d8aa0 _$E315
// FUNCTION: STBC 0x006d8ab0 _$E120
// FUNCTION: STBC 0x006d8ac0 _$E119
// FUNCTION: STBC 0x006d8ad0 _$E330
// FUNCTION: STBC 0x006d8ae0 _$E329
// FUNCTION: STBC 0x006d8af0 _$E334
// FUNCTION: STBC 0x006d8b00 _$E333
// FUNCTION: STBC 0x006d8b10 _$E338
// FUNCTION: STBC 0x006d8b20 _$E337
// FUNCTION: STBC 0x006d8b30 _$E342
// FUNCTION: STBC 0x006d8b40 _$E341
// FUNCTION: STBC 0x006d8b50 _$E129
// FUNCTION: STBC 0x006d8b60 _$E128
// FUNCTION: STBC 0x006d8b70 _$E346
// FUNCTION: STBC 0x006d8b80 _$E345
// FUNCTION: STBC 0x006d8b90 _$E350
// FUNCTION: STBC 0x006d8ba0 _$E349
// FUNCTION: STBC 0x006d8bb0 _$E354
// FUNCTION: STBC 0x006d8bc0 _$E353
// FUNCTION: STBC 0x006d8bd0 _$E135
// FUNCTION: STBC 0x006d8be0 _$E134
// FUNCTION: STBC 0x006d8bf0 _$E140
// FUNCTION: STBC 0x006d8c00 _$E139
// FUNCTION: STBC 0x006d8c10 _$E170
// FUNCTION: STBC 0x006d8c20 _$E169
// FUNCTION: STBC 0x006d8c30 _$E358
// FUNCTION: STBC 0x006d8c40 _$E357
// FUNCTION: STBC 0x006d8c50 _$E175
// FUNCTION: STBC 0x006d8c60 _$E174
// FUNCTION: STBC 0x006d8c70 _$E362
// FUNCTION: STBC 0x006d8c80 _$E361
// FUNCTION: STBC 0x006d8c90 _$E366
// FUNCTION: STBC 0x006d8ca0 _$E365
// FUNCTION: STBC 0x006d8cb0 _$E180
// FUNCTION: STBC 0x006d8cc0 _$E179
// FUNCTION: STBC 0x006d8cd0 _$E370
// FUNCTION: STBC 0x006d8ce0 _$E369
// FUNCTION: STBC 0x006d8cf0 _$E145
// FUNCTION: STBC 0x006d8d00 _$E144
// FUNCTION: STBC 0x006d8d10 _$E374
// FUNCTION: STBC 0x006d8d20 _$E373
// FUNCTION: STBC 0x006d8d30 _$E378
// FUNCTION: STBC 0x006d8d40 _$E377
// FUNCTION: STBC 0x006d8d50 _$E185
// FUNCTION: STBC 0x006d8d60 _$E184
// FUNCTION: STBC 0x006d8d70 _$E190
// FUNCTION: STBC 0x006d8d80 _$E189
// FUNCTION: STBC 0x006d8d90 _$E382
// FUNCTION: STBC 0x006d8da0 _$E381
// FUNCTION: STBC 0x006d8db0 _$E386
// FUNCTION: STBC 0x006d8dc0 _$E385
// FUNCTION: STBC 0x006d8dd0 _$E390
// FUNCTION: STBC 0x006d8de0 _$E389
// FUNCTION: STBC 0x006d8df0 _$E394
// FUNCTION: STBC 0x006d8e00 _$E393
// FUNCTION: STBC 0x006d8e10 _$E398
// FUNCTION: STBC 0x006d8e20 _$E397
// FUNCTION: STBC 0x006d8e30 _$E402
// FUNCTION: STBC 0x006d8e40 _$E401
// FUNCTION: STBC 0x006d8e50 _$E406
// FUNCTION: STBC 0x006d8e60 _$E405
// FUNCTION: STBC 0x006d8e70 _$E410
// FUNCTION: STBC 0x006d8e80 _$E409
// FUNCTION: STBC 0x006d8e90 _$E195
// FUNCTION: STBC 0x006d8ea0 _$E194
// FUNCTION: STBC 0x006d8eb0 _$E414
// FUNCTION: STBC 0x006d8ec0 _$E413
// FUNCTION: STBC 0x006d8ed0 _$E200
// FUNCTION: STBC 0x006d8ee0 _$E199
// FUNCTION: STBC 0x006d8ef0 _$E205
// FUNCTION: STBC 0x006d8f00 _$E204

// DATA: STBC 0x0095adf8
TGEventHandlerObject *ANY_TARGET = (TGEventHandlerObject *)-1;

// DATA: STBC 0x0095adfc
TGEventHandlerObject *INVALID_DESTINATION = (TGEventHandlerObject *)-1;

/*
================
TGEventHandlerObject::Cast

The object as an event handler; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006d8f10
TGEventHandlerObject *TGEventHandlerObject::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGEVENTHANDLEROBJECT_TYPE ) ) {
		return (TGEventHandlerObject *)object;
	}
	return NULL;
}

// The constructor and destructor of classHandlers.
// FUNCTION: STBC 0x006d8f40 _$E420
// FUNCTION: STBC 0x006d8f50 _$E416
// FUNCTION: STBC 0x006d8f60 _$E419
// FUNCTION: STBC 0x006d8f70 _$E417

// DATA: STBC 0x00998558
TGEventHandlerTable TGEventHandlerObject::classHandlers;

/*
================
TGEventHandlerObject::TGEventHandlerObject
================
*/
// FUNCTION: STBC 0x006d8f90
TGEventHandlerObject::TGEventHandlerObject( long id ) : TGTemplatedAttrObject( id ) {
	handlers = NULL;
}

/*
================
TGEventHandlerObject::GetObjType
================
*/
// FUNCTION: STBC 0x006d8fb0
int TGEventHandlerObject::GetObjType( void ) {
	return TGEVENTHANDLEROBJECT_TYPE;
}

/*
================
TGEventHandlerObject::GetClassName
================
*/
// FUNCTION: STBC 0x006d8ff0
char *TGEventHandlerObject::GetClassName( void ) {
	return "TGEventHandlerObject";
}

/*
================
TGEventHandlerObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006d9000
char *TGEventHandlerObject::GetSwigTypeName( void ) {
	return "_p_TGEventHandlerObject";
}

/*
================
TGEventHandlerObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006d9010
char *TGEventHandlerObject::GetPythonClassName( void ) {
	return "TGEventHandlerObjectPtr";
}

/*
================
TGEventHandlerObject::GetClassHandlers

The handlers of the class; every handler class returns its own static table.
================
*/
// FUNCTION: STBC 0x006d9020
TGEventHandlerTable *TGEventHandlerObject::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TGEventHandlerObject::~TGEventHandlerObject
================
*/
// FUNCTION: STBC 0x006d9060
TGEventHandlerObject::~TGEventHandlerObject( void ) {
	if ( handlers != NULL ) {
		delete handlers;
		handlers = NULL;
	}
	g_kEventManager.RemoveAllBroadcastHandlersForObject( this );
}

/*
================
TGEventHandlerObject::CallNextHandler

Passes the event on to the handler after the one running; false when the
event is not being handled.
================
*/
// FUNCTION: STBC 0x006d90e0
bool TGEventHandlerObject::CallNextHandler( TGEvent *event ) {
	if ( event->currentTable != NULL ) {
		return event->currentTable->CallNextHandler( event, this );
	}
	return false;
}

/*
================
TGEventHandlerObject::WriteToStream

A flag for the instance handlers, then the handlers.
================
*/
// FUNCTION: STBC 0x006d9100
bool TGEventHandlerObject::WriteToStream( TGStream *stream ) {
	bool result = true;
	if ( !TGTemplatedAttrObject::WriteToStream( stream ) ) {
		return false;
	}

	if ( handlers != NULL ) {
		stream->WriteChar( 1 );
		if ( !handlers->WriteToStream( stream ) ) {
			result = false;
		}
	} else {
		stream->WriteChar( 0 );
	}
	return result;
}

/*
================
TGEventHandlerObject::ReadFromStream
================
*/
// FUNCTION: STBC 0x006d9160
bool TGEventHandlerObject::ReadFromStream( TGStream *stream ) {
	bool result = true;
	if ( !TGTemplatedAttrObject::ReadFromStream( stream ) ) {
		return false;
	}

	char hasHandlers = stream->ReadChar();
	if ( hasHandlers ) {
		if ( handlers == NULL ) {
			handlers = new TGEventHandlerTable;
			if ( handlers == NULL ) {
				result = false;
			}
			handlers->SetParent( GetClassHandlers() );
		}
		if ( !handlers->ReadFromStream( stream ) ) {
			result = false;
		}
	} else if ( handlers != NULL ) {
		delete handlers;
		handlers = NULL;
	}
	return result;
}

/*
================
TGEventHandlerObject::ProcessEvent

Calls the first handler of the event's type, the object's own before its
class's. Handlers pass the event on with CallNextHandler; the event's place
in the handler lists is restored afterwards, so that events can be handled
from within handlers.
================
*/
// FUNCTION: STBC 0x006d9240
bool TGEventHandlerObject::ProcessEvent( TGEvent *event ) {
	TGEventHandlerTable *table = event->currentTable;
	TGHandler *handler = event->currentHandler;

	event->currentTable = NULL;
	event->currentHandler = FindHandlers( event->GetEventType(), &event->currentTable );
	if ( event->currentHandler != NULL ) {
		event->currentHandler->Call( event, this );
	}

	event->currentTable = table;
	event->currentHandler = handler;
	return true;
}

/*
================
TGEventHandlerObject::AddFuncHandlerForClass
================
*/
// FUNCTION: STBC 0x006d9290
bool TGEventHandlerObject::AddFuncHandlerForClass( TGEventHandlerTable *table, int eventType, char *function ) {
	return table->AddHandler( eventType, function, true, false );
}

/*
================
TGEventHandlerObject::AddMethodHandlerForClass
================
*/
// FUNCTION: STBC 0x006d92b0
bool TGEventHandlerObject::AddMethodHandlerForClass( TGEventHandlerTable *table, int eventType, char *method ) {
	return table->AddHandler( eventType, method, true, true );
}

/*
================
TGEventHandlerObject::AddPythonFuncHandlerForClass
================
*/
// FUNCTION: STBC 0x006d92d0
bool TGEventHandlerObject::AddPythonFuncHandlerForClass( TGEventHandlerTable *table, int eventType, char *function ) {
	return table->AddHandler( eventType, function, false, false );
}

/*
================
TGEventHandlerObject::AddPythonMethodHandlerForClass
================
*/
// FUNCTION: STBC 0x006d92f0
bool TGEventHandlerObject::AddPythonMethodHandlerForClass( TGEventHandlerTable *table, int eventType, char *method ) {
	return table->AddHandler( eventType, method, false, true );
}

/*
================
TGEventHandlerObject::RemoveHandlerForClass
================
*/
// FUNCTION: STBC 0x006d9310
bool TGEventHandlerObject::RemoveHandlerForClass( TGEventHandlerTable *table, int eventType, char *function ) {
	return table->RemoveHandler( eventType, function );
}

/*
================
TGEventHandlerObject::CreateInstanceHandlers

Makes the object's own handler table on first use.
================
*/
// FUNCTION: STBC 0x006d9330
bool TGEventHandlerObject::CreateInstanceHandlers( void ) {
	if ( handlers == NULL ) {
		handlers = new TGEventHandlerTable;
		if ( handlers == NULL ) {
			return false;
		}
		handlers->SetParent( GetClassHandlers() );
	}
	return true;
}

/*
================
TGEventHandlerObject::AddFuncHandlerForInstance
================
*/
// FUNCTION: STBC 0x006d93c0
bool TGEventHandlerObject::AddFuncHandlerForInstance( int eventType, char *function ) {
	if ( !CreateInstanceHandlers() ) {
		return false;
	}
	return handlers->AddHandler( eventType, function, true, false );
}

/*
================
TGEventHandlerObject::AddMethodHandlerForInstance
================
*/
// FUNCTION: STBC 0x006d93f0
bool TGEventHandlerObject::AddMethodHandlerForInstance( int eventType, char *method ) {
	if ( !CreateInstanceHandlers() ) {
		return false;
	}
	return handlers->AddHandler( eventType, method, true, true );
}

/*
================
TGEventHandlerObject::AddPythonFuncHandlerForInstance
================
*/
// FUNCTION: STBC 0x006d9420
bool TGEventHandlerObject::AddPythonFuncHandlerForInstance( int eventType, char *function ) {
	if ( !CreateInstanceHandlers() ) {
		return false;
	}
	return handlers->AddHandler( eventType, function, false, false );
}

/*
================
TGEventHandlerObject::AddPythonMethodHandlerForInstance
================
*/
// FUNCTION: STBC 0x006d9450
bool TGEventHandlerObject::AddPythonMethodHandlerForInstance( int eventType, char *method ) {
	if ( !CreateInstanceHandlers() ) {
		return false;
	}
	return handlers->AddHandler( eventType, method, false, true );
}

/*
================
TGEventHandlerObject::RemoveHandlerForInstance
================
*/
// FUNCTION: STBC 0x006d9480
bool TGEventHandlerObject::RemoveHandlerForInstance( int eventType, char *function ) {
	if ( handlers != NULL ) {
		return handlers->RemoveHandler( eventType, function );
	}
	return false;
}

/*
================
TGEventHandlerObject::RemoveAllInstanceHandlers
================
*/
// FUNCTION: STBC 0x006d94a0
bool TGEventHandlerObject::RemoveAllInstanceHandlers( void ) {
	if ( handlers != NULL ) {
		handlers->RemoveAllHandlers();
		delete handlers;
		handlers = NULL;
	}
	return true;
}

/*
================
TGEventHandlerObject::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x006d94e0
void TGEventHandlerObject::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( &TGEventHandlerObject::DeleteMeStage1, "TGEventHandlerObject::DeleteMeStage1" );
	g_kEventManager.RegisterHandlerMethod( &TGEventHandlerObject::DeleteMeStage2, "TGEventHandlerObject::DeleteMeStage2" );
}

/*
================
TGEventHandlerObject::RegisterHandlers
================
*/
// FUNCTION: STBC 0x006d9510
void TGEventHandlerObject::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_DELETE_ME_STAGE_1, "TGEventHandlerObject::DeleteMeStage1" );
	AddMethodHandlerForClass( &classHandlers, ET_DELETE_ME_STAGE_2, "TGEventHandlerObject::DeleteMeStage2" );
}

/*
================
TGEventHandlerObject::DeleteMeStage1

Posts the second stage to the object as a private event.
================
*/
// FUNCTION: STBC 0x006d9540
void TGEventHandlerObject::DeleteMeStage1( TGEvent *event ) {
	TGEvent *stage2 = new TGEvent;
	stage2->SetDestination( this );
	stage2->SetEventType( ET_DELETE_ME_STAGE_2 );
	stage2->SetPrivate( true );
	g_kEventManager.AddEvent( stage2 );

	CallNextHandler( event );
}

/*
================
TGEventHandlerObject::DeleteMeStage2

Deletes the object once the other handlers have seen the event.
================
*/
// FUNCTION: STBC 0x006d95b0
void TGEventHandlerObject::DeleteMeStage2( TGEvent *event ) {
	CallNextHandler( event );
	delete this;
}

/*
================
TGEventHandlerObject::FindHandlers

The first handlers of the event type: the object's own, else its class's.
================
*/
// FUNCTION: STBC 0x006d95d0
TGHandler *TGEventHandlerObject::FindHandlers( int eventType, TGEventHandlerTable **owner ) {
	if ( handlers != NULL ) {
		TGHandler *handler = handlers->FindHandlers( eventType, owner );
		if ( handler != NULL ) {
			return handler;
		}
	}
	return GetClassHandlers()->FindHandlers( eventType, owner );
}
