/*
===============================================================================

	OptimizedTorpsReady.cpp

===============================================================================
*/

#include <iostream>
#include "OptimizedTorpsReady.h"
#include "ConditionScript.h"
#include "PyEmbed.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGPoint3.h"
#include "TGStream.h"
#include "TorpedoSystem.h"
#include "TorpedoTube.h"
#include "NiAccumulator.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0048caf0 _$E506
// FUNCTION: STBC 0x0048cb10 _$E505

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0048b4f0 _$E32
// FUNCTION: STBC 0x0048b500 _$E29
// FUNCTION: STBC 0x0048b510 _$E31
// FUNCTION: STBC 0x0048b520 _$E30
// FUNCTION: STBC 0x0048b530 _$E38
// FUNCTION: STBC 0x0048b540 _$E35
// FUNCTION: STBC 0x0048b550 _$E37
// FUNCTION: STBC 0x0048b560 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0048b570 _$E90
// FUNCTION: STBC 0x0048b580 _$E89
// FUNCTION: STBC 0x0048b590 _$E308
// FUNCTION: STBC 0x0048b5a0 _$E307
// FUNCTION: STBC 0x0048c2b0 _$E101
// FUNCTION: STBC 0x0048c2c0 _$E100
// FUNCTION: STBC 0x0048c2d0 _$E105
// FUNCTION: STBC 0x0048c2e0 _$E104
// FUNCTION: STBC 0x0048c2f0 _$E109
// FUNCTION: STBC 0x0048c300 _$E108
// FUNCTION: STBC 0x0048c310 _$E113
// FUNCTION: STBC 0x0048c320 _$E112
// FUNCTION: STBC 0x0048c330 _$E230
// FUNCTION: STBC 0x0048c340 _$E229
// FUNCTION: STBC 0x0048c350 _$E172
// FUNCTION: STBC 0x0048c360 _$E171
// FUNCTION: STBC 0x0048c370 _$E185
// FUNCTION: STBC 0x0048c380 _$E184
// FUNCTION: STBC 0x0048c390 _$E235
// FUNCTION: STBC 0x0048c3a0 _$E234
// FUNCTION: STBC 0x0048c3b0 _$E313
// FUNCTION: STBC 0x0048c3c0 _$E312
// FUNCTION: STBC 0x0048c3d0 _$E239
// FUNCTION: STBC 0x0048c3e0 _$E238
// FUNCTION: STBC 0x0048c3f0 _$E317
// FUNCTION: STBC 0x0048c400 _$E316
// FUNCTION: STBC 0x0048c410 _$E324
// FUNCTION: STBC 0x0048c420 _$E323
// FUNCTION: STBC 0x0048c430 _$E334
// FUNCTION: STBC 0x0048c440 _$E333
// FUNCTION: STBC 0x0048c450 _$E338
// FUNCTION: STBC 0x0048c460 _$E337
// FUNCTION: STBC 0x0048c470 _$E130
// FUNCTION: STBC 0x0048c480 _$E129
// FUNCTION: STBC 0x0048c490 _$E258
// FUNCTION: STBC 0x0048c4a0 _$E257
// FUNCTION: STBC 0x0048c4b0 _$E265
// FUNCTION: STBC 0x0048c4c0 _$E264
// FUNCTION: STBC 0x0048c4d0 _$E269
// FUNCTION: STBC 0x0048c4e0 _$E268
// FUNCTION: STBC 0x0048c4f0 _$E273
// FUNCTION: STBC 0x0048c500 _$E272
// FUNCTION: STBC 0x0048c510 _$E277
// FUNCTION: STBC 0x0048c520 _$E276
// FUNCTION: STBC 0x0048c530 _$E283
// FUNCTION: STBC 0x0048c540 _$E282
// FUNCTION: STBC 0x0048c550 _$E342
// FUNCTION: STBC 0x0048c560 _$E341
// FUNCTION: STBC 0x0048c570 _$E346
// FUNCTION: STBC 0x0048c580 _$E345
// FUNCTION: STBC 0x0048c590 _$E350
// FUNCTION: STBC 0x0048c5a0 _$E349
// FUNCTION: STBC 0x0048c5b0 _$E354
// FUNCTION: STBC 0x0048c5c0 _$E353
// FUNCTION: STBC 0x0048c5d0 _$E358
// FUNCTION: STBC 0x0048c5e0 _$E357
// FUNCTION: STBC 0x0048c5f0 _$E362
// FUNCTION: STBC 0x0048c600 _$E361
// FUNCTION: STBC 0x0048c610 _$E366
// FUNCTION: STBC 0x0048c620 _$E365
// FUNCTION: STBC 0x0048c630 _$E370
// FUNCTION: STBC 0x0048c640 _$E369
// FUNCTION: STBC 0x0048c650 _$E374
// FUNCTION: STBC 0x0048c660 _$E373
// FUNCTION: STBC 0x0048c670 _$E378
// FUNCTION: STBC 0x0048c680 _$E377
// FUNCTION: STBC 0x0048c690 _$E135
// FUNCTION: STBC 0x0048c6a0 _$E134
// FUNCTION: STBC 0x0048c6b0 _$E382
// FUNCTION: STBC 0x0048c6c0 _$E381
// FUNCTION: STBC 0x0048c6d0 _$E140
// FUNCTION: STBC 0x0048c6e0 _$E139
// FUNCTION: STBC 0x0048c6f0 _$E386
// FUNCTION: STBC 0x0048c700 _$E385
// FUNCTION: STBC 0x0048c710 _$E390
// FUNCTION: STBC 0x0048c720 _$E389
// FUNCTION: STBC 0x0048c730 _$E394
// FUNCTION: STBC 0x0048c740 _$E393
// FUNCTION: STBC 0x0048c750 _$E398
// FUNCTION: STBC 0x0048c760 _$E397
// FUNCTION: STBC 0x0048c770 _$E149
// FUNCTION: STBC 0x0048c780 _$E148
// FUNCTION: STBC 0x0048c790 _$E402
// FUNCTION: STBC 0x0048c7a0 _$E401
// FUNCTION: STBC 0x0048c7b0 _$E406
// FUNCTION: STBC 0x0048c7c0 _$E405
// FUNCTION: STBC 0x0048c7d0 _$E410
// FUNCTION: STBC 0x0048c7e0 _$E409
// FUNCTION: STBC 0x0048c7f0 _$E155
// FUNCTION: STBC 0x0048c800 _$E154
// FUNCTION: STBC 0x0048c810 _$E160
// FUNCTION: STBC 0x0048c820 _$E159
// FUNCTION: STBC 0x0048c830 _$E190
// FUNCTION: STBC 0x0048c840 _$E189
// FUNCTION: STBC 0x0048c850 _$E414
// FUNCTION: STBC 0x0048c860 _$E413
// FUNCTION: STBC 0x0048c870 _$E195
// FUNCTION: STBC 0x0048c880 _$E194
// FUNCTION: STBC 0x0048c890 _$E418
// FUNCTION: STBC 0x0048c8a0 _$E417
// FUNCTION: STBC 0x0048c8b0 _$E422
// FUNCTION: STBC 0x0048c8c0 _$E421
// FUNCTION: STBC 0x0048c8d0 _$E200
// FUNCTION: STBC 0x0048c8e0 _$E199
// FUNCTION: STBC 0x0048c8f0 _$E426
// FUNCTION: STBC 0x0048c900 _$E425
// FUNCTION: STBC 0x0048c910 _$E165
// FUNCTION: STBC 0x0048c920 _$E164
// FUNCTION: STBC 0x0048c930 _$E430
// FUNCTION: STBC 0x0048c940 _$E429
// FUNCTION: STBC 0x0048c950 _$E434
// FUNCTION: STBC 0x0048c960 _$E433
// FUNCTION: STBC 0x0048c970 _$E205
// FUNCTION: STBC 0x0048c980 _$E204
// FUNCTION: STBC 0x0048c990 _$E210
// FUNCTION: STBC 0x0048c9a0 _$E209
// FUNCTION: STBC 0x0048c9b0 _$E438
// FUNCTION: STBC 0x0048c9c0 _$E437
// FUNCTION: STBC 0x0048c9d0 _$E442
// FUNCTION: STBC 0x0048c9e0 _$E441
// FUNCTION: STBC 0x0048c9f0 _$E446
// FUNCTION: STBC 0x0048ca00 _$E445
// FUNCTION: STBC 0x0048ca10 _$E450
// FUNCTION: STBC 0x0048ca20 _$E449
// FUNCTION: STBC 0x0048ca30 _$E454
// FUNCTION: STBC 0x0048ca40 _$E453
// FUNCTION: STBC 0x0048ca50 _$E458
// FUNCTION: STBC 0x0048ca60 _$E457
// FUNCTION: STBC 0x0048ca70 _$E462
// FUNCTION: STBC 0x0048ca80 _$E461
// FUNCTION: STBC 0x0048ca90 _$E466
// FUNCTION: STBC 0x0048caa0 _$E465
// FUNCTION: STBC 0x0048cab0 _$E215
// FUNCTION: STBC 0x0048cac0 _$E214
// FUNCTION: STBC 0x0048cad0 _$E470
// FUNCTION: STBC 0x0048cae0 _$E469
// FUNCTION: STBC 0x0048cb20 _$E220
// FUNCTION: STBC 0x0048cb30 _$E219
// FUNCTION: STBC 0x0048cb40 _$E225
// FUNCTION: STBC 0x0048cb50 _$E224
// FUNCTION: STBC 0x0048cb60 _$E474
// FUNCTION: STBC 0x0048cb70 _$E473
// FUNCTION: STBC 0x0048cb80 _$E479
// FUNCTION: STBC 0x0048cb90 _$E478
// FUNCTION: STBC 0x0048cba0 _$E483
// FUNCTION: STBC 0x0048cbb0 _$E482

// The least cosine between a tube's direction and the watched direction.
static const float FACING_COSINE = 0.66f;

// FUNCTION: STBC 0x0048b5b0 _$E486
// FUNCTION: STBC 0x0048b5c0 _$E485
static ConditionScriptCreatorRegistration torpsReadyCreator( "Conditions.ConditionTorpsReady", "ConditionTorpsReady", OptimizedTorpsReady::Create );

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0048b710 _$E492
// FUNCTION: STBC 0x0048b720 _$E488
// FUNCTION: STBC 0x0048b730 _$E491
// FUNCTION: STBC 0x0048b740 _$E489
// DATA: STBC 0x00982828
TGEventHandlerTable OTREventHandler::classHandlers;

/*
================
OTREventHandler::WriteToStream

The condition is written as its ID.
================
*/
// FUNCTION: STBC 0x0048b760
bool OTREventHandler::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( condition->GetObjID() );
	return true;
}

/*
================
OTREventHandler::ReadFromStream

The condition's ID waits in the pointer until PostFixUp.
================
*/
// FUNCTION: STBC 0x0048b790
bool OTREventHandler::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}
	condition = (OptimizedTorpsReady *)stream->ReadID();
	return true;
}

/*
================
OTREventHandler::FixUpPointers
================
*/
// FUNCTION: STBC 0x0048b7c0
bool OTREventHandler::FixUpPointers( void ) {
	return TGObject::GetTGObjectPtr( (long)condition ) != NULL ? true : false;
}

/*
================
OTREventHandler::PostFixUp
================
*/
// FUNCTION: STBC 0x0048b7e0
void OTREventHandler::PostFixUp( void ) {
	condition = (OptimizedTorpsReady *)TGObject::GetTGObjectPtr( (long)condition );
}

// The module's construction and exit-time destruction.
// FUNCTION: STBC 0x0048b800 _$E497
// FUNCTION: STBC 0x0048b810 _$E494
// FUNCTION: STBC 0x0048bc60 _$E496
// FUNCTION: STBC 0x0048bc70 _$E495
// DATA: STBC 0x00982798
OTREventHandlerModule otrEventHandlerModule;		// invented name

/*
================
OTREventHandlerModule::VirtualSlot00

The startup: the creators, the handler table and the handler names.
================
*/
// FUNCTION: STBC 0x0048b840
void OTREventHandlerModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( CT_OPT_TORPS_READY, CreateOptimizedTorpsReady );
	TGObject::RegisterCreator( CT_OPT_TR_EVENT_HANDLER, CreateOTREventHandler );
	OTREventHandler::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( OTREventHandler::GetClassTable() );

	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OTREventHandler::SystemDisabled, "OTREventHandler::SystemDisabled" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OTREventHandler::SystemEnabled, "OTREventHandler::SystemEnabled" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OTREventHandler::TorpReloaded, "OTREventHandler::TorpReloaded" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&OTREventHandler::TorpFired, "OTREventHandler::TorpFired" );
}

/*
================
OTREventHandlerModule::CreateOptimizedTorpsReady
================
*/
// FUNCTION: STBC 0x0048b9f0
TGObject *OTREventHandlerModule::CreateOptimizedTorpsReady( long id ) {
	return new OptimizedTorpsReady( id );
}

/*
================
OptimizedTorpsReady::VirtualSlot08
================
*/
// FUNCTION: STBC 0x0048bab0
PyObject *OptimizedTorpsReady::VirtualSlot08( void ) {
	return PyEmbed::Unknown6F9920( this, "TGCondition" );
}

/*
================
OTREventHandlerModule::CreateOTREventHandler
================
*/
// FUNCTION: STBC 0x0048baf0
TGObject *OTREventHandlerModule::CreateOTREventHandler( long id ) {
	return new OTREventHandler( id );
}

/*
================
OTREventHandler::SystemDisabled
================
*/
// FUNCTION: STBC 0x0048bbd0
void OTREventHandler::SystemDisabled( TGEvent *event ) {
	condition->SystemDisabled( event );
}

/*
================
OTREventHandler::SystemEnabled
================
*/
// FUNCTION: STBC 0x0048bbe0
void OTREventHandler::SystemEnabled( TGEvent *event ) {
	condition->SystemEnabled( event );
}

/*
================
OTREventHandler::TorpReloaded
================
*/
// FUNCTION: STBC 0x0048bbf0
void OTREventHandler::TorpReloaded( TGEvent *event ) {
	condition->TorpReloaded( event );
}

/*
================
OTREventHandler::TorpFired
================
*/
// FUNCTION: STBC 0x0048bc00
void OTREventHandler::TorpFired( TGEvent *event ) {
	condition->TorpFired( event );
}

/*
================
OTREventHandlerModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x0048bc50
void OTREventHandlerModule::VirtualSlot05( void ) {
}

/*
================
OptimizedTorpsReady::OptimizedTorpsReady
================
*/
// FUNCTION: STBC 0x0048bc90 ??0OptimizedTorpsReady@@QAE@PADABVNiPoint3@@@Z
OptimizedTorpsReady::OptimizedTorpsReady( char *shipName, const NiPoint3 &direction ) {
	eventHandler = new OTREventHandler( this );
	tubeIDs = NULL;
	numTubes = 0;
	ShipClass *ship = ShipClass::GetObject( NULL, shipName );
	if ( ship != NULL ) {
		WatchTubes( ship, direction );
		UpdateStatus();
	}
}

/*
================
OptimizedTorpsReady::~OptimizedTorpsReady
================
*/
// FUNCTION: STBC 0x0048bd50
// FUNCTION: STBC 0x0048bac0 ??_GOptimizedTorpsReady@@UAEPAXI@Z
OptimizedTorpsReady::~OptimizedTorpsReady( void ) {
	delete eventHandler;
	delete [] tubeIDs;
}

/*
================
OptimizedTorpsReady::Create

NULL when the arguments are wrong.
================
*/
// FUNCTION: STBC 0x0048bdc0
TGObject *OptimizedTorpsReady::Create( PyObject *args ) {
	if ( !PyTuple_Check( args ) || PyTuple_Size( args ) < 3 || !PyString_Check( PyTuple_GetItem( args, 2 ) ) ) {
		return NULL;
	}
	char *shipName = PyString_AsString( PyTuple_GetItem( args, 2 ) );
	NiPoint3 direction = NiPoint3::ZERO;
	if ( PyTuple_Size( args ) >= 4 ) {
		TGPoint3 *point = NULL;
		PyEmbed::Unknown6F98E0( PyTuple_GetItem( args, 3 ), (void **)&point, "TGPoint3" );
		if ( point == NULL ) {
			return NULL;
		}
		direction = *point;
	}
	return new OptimizedTorpsReady( shipName, direction );
}

/*
================
OptimizedTorpsReady::WatchTubes
================
*/
// FUNCTION: STBC 0x0048bec0
void OptimizedTorpsReady::WatchTubes( ShipClass *ship, const NiPoint3 &direction ) {
	TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
	if ( torpedoes == NULL ) {
		return;
	}
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DISABLED, eventHandler, "OTREventHandler::SystemDisabled", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_OPERATIONAL, eventHandler, "OTREventHandler::SystemEnabled", ship );

	numTubes = 0;
	int i;
	for ( i = 0; i < torpedoes->GetNumChildSubsystems(); i++ ) {
		TorpedoTube *tube = (TorpedoTube *)torpedoes->GetChildSubsystem( i );
		if ( tube->GetDirection().Dot( direction ) > FACING_COSINE ) {
			g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_RELOAD, eventHandler, "OTREventHandler::TorpReloaded", tube );
			g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_FIRED, eventHandler, "OTREventHandler::TorpFired", tube );
			numTubes++;
		}
	}
	if ( numTubes == 0 ) {
		return;
	}
	tubeIDs = new long[numTubes];
	int next = 0;
	for ( i = 0; i < torpedoes->GetNumChildSubsystems(); i++ ) {
		TorpedoTube *tube = (TorpedoTube *)torpedoes->GetChildSubsystem( i );
		if ( tube->GetDirection().Dot( direction ) > FACING_COSINE ) {
			tubeIDs[next++] = tube->GetObjID();
		}
	}
}

/*
================
OptimizedTorpsReady::UpdateStatus

True when a watched tube has a torpedo ready; a disabled torpedo system
makes it false.
================
*/
// FUNCTION: STBC 0x0048c050
void OptimizedTorpsReady::UpdateStatus( void ) {
	bool ready = false;
	for ( int i = 0; i < numTubes; i++ ) {
		TorpedoTube *tube = (TorpedoTube *)TGObject::GetTGObjectPtr( tubeIDs[i] );
		if ( tube != NULL ) {
			if ( tube->GetParentSubsystem()->IsDisabled() ) {
				break;
			}
			if ( !tube->IsDisabled() && tube->GetNumReady() != 0 ) {
				ready = true;
				break;
			}
		}
	}
	SetStatus( ready );
}

/*
================
OptimizedTorpsReady::WriteToStream
================
*/
// FUNCTION: STBC 0x0048c0d0
bool OptimizedTorpsReady::WriteToStream( TGStream *stream ) {
	if ( !TGCondition::WriteToStream( stream ) ) {
		return false;
	}
	eventHandler->WriteToStream( stream );
	stream->WriteInt( numTubes );
	for ( int i = 0; i < numTubes; i++ ) {
		stream->WriteID( tubeIDs[i] );
	}
	return true;
}

/*
================
OptimizedTorpsReady::ReadFromStream
================
*/
// FUNCTION: STBC 0x0048c130
bool OptimizedTorpsReady::ReadFromStream( TGStream *stream ) {
	if ( !TGCondition::ReadFromStream( stream ) ) {
		return false;
	}
	eventHandler = (OTREventHandler *)TGObject::LoadObject( stream, 0 );
	if ( eventHandler == NULL ) {
		return false;
	}
	numTubes = stream->ReadInt();
	if ( numTubes < 0 ) {
		return false;
	}
	tubeIDs = new long[numTubes];
	for ( int i = 0; i < numTubes; i++ ) {
		tubeIDs[i] = stream->ReadID();
	}
	return true;
}

/*
================
OptimizedTorpsReady::SystemDisabled

The torpedo system going down makes the condition false; a tube going
down calls for a new look.
================
*/
// FUNCTION: STBC 0x0048c1c0
void OptimizedTorpsReady::SystemDisabled( TGEvent *event ) {
	ShipClass *ship = (ShipClass *)event->GetDestination();
	if ( ship->GetTorpedoSystem() != NULL && ship->GetTorpedoSystem() == event->GetSource() ) {
		SetStatus( false );
	} else if ( TorpedoTube::Cast( event->GetSource() ) != NULL ) {
		UpdateStatus();
	}
}

/*
================
OptimizedTorpsReady::SystemEnabled
================
*/
// FUNCTION: STBC 0x0048c210
void OptimizedTorpsReady::SystemEnabled( TGEvent *event ) {
	TorpedoSystem *torpedoes = ( (ShipClass *)event->GetDestination() )->GetTorpedoSystem();
	if ( ( torpedoes != NULL && torpedoes == event->GetSource() ) || TorpedoTube::Cast( event->GetSource() ) != NULL ) {
		UpdateStatus();
	}
}

/*
================
OptimizedTorpsReady::TorpReloaded
================
*/
// FUNCTION: STBC 0x0048c250
void OptimizedTorpsReady::TorpReloaded( TGEvent *event ) {
	TorpedoTube *tube = (TorpedoTube *)event->GetDestination();
	if ( !tube->IsDisabled() && !tube->GetParentSubsystem()->IsDisabled() && tube->GetNumReady() != 0 ) {
		SetStatus( true );
	}
}

/*
================
OptimizedTorpsReady::TorpFired
================
*/
// FUNCTION: STBC 0x0048c290
void OptimizedTorpsReady::TorpFired( TGEvent *event ) {
	if ( ( (TorpedoTube *)event->GetDestination() )->GetNumReady() == 0 ) {
		UpdateStatus();
	}
}
