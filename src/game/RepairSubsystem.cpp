/*
===============================================================================

	RepairSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "RepairSubsystem.h"
#include "EngRepairPane.h"
#include "Game.h"
#include "RepairSubsystemProperty.h"
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00564f40 _$E32
// FUNCTION: STBC 0x00564f50 _$E29
// FUNCTION: STBC 0x00564f60 _$E31
// FUNCTION: STBC 0x00564f70 _$E30
// FUNCTION: STBC 0x00564f80 _$E38
// FUNCTION: STBC 0x00564f90 _$E35
// FUNCTION: STBC 0x00564fa0 _$E37
// FUNCTION: STBC 0x00564fb0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00565010 _$E480
// FUNCTION: STBC 0x00565020 _$E476
// FUNCTION: STBC 0x00565030 _$E479
// FUNCTION: STBC 0x00565040 _$E477

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x00566aa0 _$E489
// FUNCTION: STBC 0x00566ac0 _$E488

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00564fc0 _$E75
// FUNCTION: STBC 0x00564fd0 _$E74
// FUNCTION: STBC 0x00564ff0 _$E79
// FUNCTION: STBC 0x00565000 _$E78
// FUNCTION: STBC 0x00566140 _$E90
// FUNCTION: STBC 0x00566150 _$E89
// FUNCTION: STBC 0x00566160 _$E94
// FUNCTION: STBC 0x00566170 _$E93
// FUNCTION: STBC 0x00566180 _$E98
// FUNCTION: STBC 0x00566190 _$E97
// FUNCTION: STBC 0x005661a0 _$E102
// FUNCTION: STBC 0x005661b0 _$E101
// FUNCTION: STBC 0x005661c0 _$E221
// FUNCTION: STBC 0x005661d0 _$E220
// FUNCTION: STBC 0x005661e0 _$E163
// FUNCTION: STBC 0x005661f0 _$E162
// FUNCTION: STBC 0x00566200 _$E176
// FUNCTION: STBC 0x00566210 _$E175
// FUNCTION: STBC 0x00566220 _$E226
// FUNCTION: STBC 0x00566230 _$E225
// FUNCTION: STBC 0x00566240 _$E231
// FUNCTION: STBC 0x00566250 _$E230
// FUNCTION: STBC 0x00566260 _$E235
// FUNCTION: STBC 0x00566270 _$E234
// FUNCTION: STBC 0x00566280 _$E239
// FUNCTION: STBC 0x00566290 _$E238
// FUNCTION: STBC 0x005662a0 _$E246
// FUNCTION: STBC 0x005662b0 _$E245
// FUNCTION: STBC 0x005662c0 _$E325
// FUNCTION: STBC 0x005662d0 _$E324
// FUNCTION: STBC 0x005662e0 _$E329
// FUNCTION: STBC 0x005662f0 _$E328
// FUNCTION: STBC 0x00566300 _$E121
// FUNCTION: STBC 0x00566310 _$E120
// FUNCTION: STBC 0x00566320 _$E273
// FUNCTION: STBC 0x00566330 _$E272
// FUNCTION: STBC 0x00566340 _$E280
// FUNCTION: STBC 0x00566350 _$E279
// FUNCTION: STBC 0x00566360 _$E284
// FUNCTION: STBC 0x00566370 _$E283
// FUNCTION: STBC 0x00566380 _$E288
// FUNCTION: STBC 0x00566390 _$E287
// FUNCTION: STBC 0x005663a0 _$E292
// FUNCTION: STBC 0x005663b0 _$E291
// FUNCTION: STBC 0x005663c0 _$E298
// FUNCTION: STBC 0x005663d0 _$E297
// FUNCTION: STBC 0x005663e0 _$E333
// FUNCTION: STBC 0x005663f0 _$E332
// FUNCTION: STBC 0x00566400 _$E337
// FUNCTION: STBC 0x00566410 _$E336
// FUNCTION: STBC 0x00566420 _$E341
// FUNCTION: STBC 0x00566430 _$E340
// FUNCTION: STBC 0x00566440 _$E345
// FUNCTION: STBC 0x00566450 _$E344
// FUNCTION: STBC 0x00566460 _$E349
// FUNCTION: STBC 0x00566470 _$E348
// FUNCTION: STBC 0x00566480 _$E353
// FUNCTION: STBC 0x00566490 _$E352
// FUNCTION: STBC 0x005664a0 _$E357
// FUNCTION: STBC 0x005664b0 _$E356
// FUNCTION: STBC 0x005664c0 _$E361
// FUNCTION: STBC 0x005664d0 _$E360
// FUNCTION: STBC 0x005664e0 _$E365
// FUNCTION: STBC 0x005664f0 _$E364
// FUNCTION: STBC 0x00566500 _$E369
// FUNCTION: STBC 0x00566510 _$E368
// FUNCTION: STBC 0x00566520 _$E126
// FUNCTION: STBC 0x00566530 _$E125
// FUNCTION: STBC 0x00566540 _$E373
// FUNCTION: STBC 0x00566550 _$E372
// FUNCTION: STBC 0x00566560 _$E131
// FUNCTION: STBC 0x00566570 _$E130
// FUNCTION: STBC 0x00566580 _$E377
// FUNCTION: STBC 0x00566590 _$E376
// FUNCTION: STBC 0x005665a0 _$E381
// FUNCTION: STBC 0x005665b0 _$E380
// FUNCTION: STBC 0x005665c0 _$E385
// FUNCTION: STBC 0x005665d0 _$E384
// FUNCTION: STBC 0x005665e0 _$E389
// FUNCTION: STBC 0x005665f0 _$E388
// FUNCTION: STBC 0x00566600 _$E140
// FUNCTION: STBC 0x00566610 _$E139
// FUNCTION: STBC 0x00566620 _$E393
// FUNCTION: STBC 0x00566630 _$E392
// FUNCTION: STBC 0x00566640 _$E397
// FUNCTION: STBC 0x00566650 _$E396
// FUNCTION: STBC 0x00566660 _$E401
// FUNCTION: STBC 0x00566670 _$E400
// FUNCTION: STBC 0x00566680 _$E146
// FUNCTION: STBC 0x00566690 _$E145
// FUNCTION: STBC 0x005666a0 _$E151
// FUNCTION: STBC 0x005666b0 _$E150
// FUNCTION: STBC 0x005666c0 _$E181
// FUNCTION: STBC 0x005666d0 _$E180
// FUNCTION: STBC 0x005667c0 _$E405
// FUNCTION: STBC 0x005667d0 _$E404
// FUNCTION: STBC 0x005667e0 _$E186
// FUNCTION: STBC 0x005667f0 _$E185
// FUNCTION: STBC 0x00566800 _$E409
// FUNCTION: STBC 0x00566810 _$E408
// FUNCTION: STBC 0x00566820 _$E413
// FUNCTION: STBC 0x00566830 _$E412
// FUNCTION: STBC 0x00566840 _$E191
// FUNCTION: STBC 0x00566850 _$E190
// FUNCTION: STBC 0x00566860 _$E417
// FUNCTION: STBC 0x00566870 _$E416
// FUNCTION: STBC 0x00566880 _$E156
// FUNCTION: STBC 0x00566890 _$E155
// FUNCTION: STBC 0x005668a0 _$E421
// FUNCTION: STBC 0x005668b0 _$E420
// FUNCTION: STBC 0x005668c0 _$E425
// FUNCTION: STBC 0x005668d0 _$E424
// FUNCTION: STBC 0x005668e0 _$E196
// FUNCTION: STBC 0x005668f0 _$E195
// FUNCTION: STBC 0x00566900 _$E201
// FUNCTION: STBC 0x00566910 _$E200
// FUNCTION: STBC 0x00566920 _$E429
// FUNCTION: STBC 0x00566930 _$E428
// FUNCTION: STBC 0x00566940 _$E433
// FUNCTION: STBC 0x00566950 _$E432
// FUNCTION: STBC 0x00566960 _$E437
// FUNCTION: STBC 0x00566970 _$E436
// FUNCTION: STBC 0x00566980 _$E441
// FUNCTION: STBC 0x00566990 _$E440
// FUNCTION: STBC 0x005669a0 _$E445
// FUNCTION: STBC 0x005669b0 _$E444
// FUNCTION: STBC 0x005669c0 _$E449
// FUNCTION: STBC 0x005669d0 _$E448
// FUNCTION: STBC 0x005669e0 _$E453
// FUNCTION: STBC 0x005669f0 _$E452
// FUNCTION: STBC 0x00566a00 _$E457
// FUNCTION: STBC 0x00566a10 _$E456
// FUNCTION: STBC 0x00566a20 _$E206
// FUNCTION: STBC 0x00566a30 _$E205
// FUNCTION: STBC 0x00566a40 _$E461
// FUNCTION: STBC 0x00566a50 _$E460
// FUNCTION: STBC 0x00566a60 _$E211
// FUNCTION: STBC 0x00566a70 _$E210
// FUNCTION: STBC 0x00566a80 _$E216
// FUNCTION: STBC 0x00566a90 _$E215
// FUNCTION: STBC 0x00566b20 _$E465
// FUNCTION: STBC 0x00566b30 _$E464
// FUNCTION: STBC 0x00566b40 _$E470
// FUNCTION: STBC 0x00566b50 _$E469
// FUNCTION: STBC 0x00566b60 _$E474
// FUNCTION: STBC 0x00566b70 _$E473

/*
================
RepairSubsystem::GetProperty
================
*/
// FUNCTION: STBC 0x00564fe0
RepairSubsystemProperty *RepairSubsystem::GetProperty( void ) {
	return (RepairSubsystemProperty *)property;
}

/*
================
RepairSubsystem::Cast

The object as a RepairSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00565060
RepairSubsystem *RepairSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( REPAIRSUBSYSTEM_TYPE ) ) {
		return (RepairSubsystem *)object;
	}
	return NULL;
}

/*
================
RepairSubsystem::RepairSubsystem
================
*/
// FUNCTION: STBC 0x00565090
RepairSubsystem::RepairSubsystem( long id ) : PoweredSubsystem( id ), repairList( 2 ) {
}

// DATA: STBC 0x0098beb0
TGEventHandlerTable RepairSubsystem::classHandlers;

/*
================
RepairSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x005650e0
int RepairSubsystem::GetObjType( void ) {
	return REPAIRSUBSYSTEM_TYPE;
}

/*
================
RepairSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x00565150
char *RepairSubsystem::GetClassName( void ) {
	return "RepairSubsystem";
}

/*
================
RepairSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00565160
char *RepairSubsystem::GetSwigTypeName( void ) {
	return "_p_RepairSubsystem";
}

/*
================
RepairSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00565170
char *RepairSubsystem::GetPythonClassName( void ) {
	return "RepairSubsystemPtr";
}

/*
================
RepairSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00565180
TGEventHandlerTable *RepairSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}
/*
================
RepairSubsystem::~RepairSubsystem
================
*/
// FUNCTION: STBC 0x005651c0
// FUNCTION: STBC 0x00565190 ??_GRepairSubsystem@@UAEPAXI@Z
RepairSubsystem::~RepairSubsystem( void ) {
}

/*
================
RepairSubsystem::SetParentShip

Listens for the ship's subsystems being damaged, repaired and rebuilt.
================
*/
// FUNCTION: STBC 0x00565220
void RepairSubsystem::SetParentShip( ShipClass *ship ) {
	PoweredSubsystem::SetParentShip( ship );

	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DAMAGED, this, "RepairSubsystem::HandleHitEvent", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_REPAIR_COMPLETED, this, "RepairSubsystem::HandleRepairCompleted", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_REBUILT, this, "RepairSubsystem::HandleSubsystemRebuilt", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_REPAIR_CANNOT_BE_COMPLETED, this, "RepairSubsystem::HandleRepairCannotBeCompleted", ship );
}

/*
================
RepairSubsystem::Update

While the subsystem is on (and, in multiplayer, on the host), the repair
points of the frame are shared among the first subsystems on the list, as
many as there are repair teams; a repaired subsystem posts that its repair
is complete. A destroyed one on the list, repaired or waiting, posts that
its repair cannot be completed. The player's engineering pane is updated.
================
*/
// FUNCTION: STBC 0x005652a0
void RepairSubsystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );
	if ( !IsOn() ) {
		return;
	}
	if ( g_kUtopiaModule.IsMultiplayer() == false || ( g_kUtopiaModule.IsMultiplayer() == true && g_kUtopiaModule.IsHost() ) ) {
		float repairPoints = GetProperty()->GetMaxRepairPoints() * GetConditionPercentage() * deltaTime;
		NiTListIterator pos = repairList.GetHeadPos();
		int i = 0;
		int numTeams = GetProperty()->GetNumRepairTeams();
		int count = repairList.GetSize();
		while ( pos != NULL && i < numTeams ) {
			ShipSubsystem *subsystem = repairList.GetNext( pos );
			if ( subsystem->GetCondition() <= 0.0f ) {
				TGObjPtrEvent *event = new TGObjPtrEvent;
				event->SetSource( this );
				event->SetDestination( parentShip );
				event->SetObjPtr( subsystem );
				event->SetEventType( ET_REPAIR_CANNOT_BE_COMPLETED );
				g_kEventManager.AddEvent( event );
			} else {
				subsystem->Repair( repairPoints / ( count < numTeams ? count : numTeams ) );
				float condition = subsystem->GetCondition();
				if ( condition / subsystem->GetMaxCondition() >= 1.0f ) {
					TGObjPtrEvent *event = new TGObjPtrEvent;
					event->SetSource( this );
					event->SetDestination( parentShip );
					event->SetObjPtr( subsystem );
					event->SetEventType( ET_REPAIR_COMPLETED );
					g_kEventManager.AddEvent( event );
				}
				i++;
			}
		}
		while ( pos != NULL ) {
			ShipSubsystem *subsystem = repairList.GetNext( pos );
			if ( subsystem->GetCondition() <= 0.0f ) {
				TGObjPtrEvent *event = new TGObjPtrEvent;
				event->SetSource( this );
				event->SetDestination( parentShip );
				event->SetObjPtr( subsystem );
				event->SetEventType( ET_REPAIR_CANNOT_BE_COMPLETED );
				g_kEventManager.AddEvent( event );
			}
		}
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && player == parentShip && EngRepairPane::GetRepairPane() != NULL ) {
		EngRepairPane::GetRepairPane()->UpdateAreas();
	}
}

/*
================
RepairSubsystem::AddSubsystem

A subsystem that is not on the repair list yet joins it when it has any
condition left; a destroyed one goes to the player's repair pane as a
destroyed system instead. False when it was on the list already.
================
*/
// FUNCTION: STBC 0x00565520
bool RepairSubsystem::AddSubsystem( ShipSubsystem *subsystem ) {
	if ( repairList.FindPos( subsystem ) ) {
		return false;
	}
	if ( subsystem->GetCondition() <= 0.0f ) {
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player == subsystem->GetParentShip() && EngRepairPane::GetRepairPane() != NULL ) {
			EngRepairPane::GetRepairPane()->RemoveSubsystem( subsystem );
			EngRepairPane::GetRepairPane()->AddDestroyedSubsystem( subsystem );
		}
	} else {
		repairList.AddTail( subsystem );
	}
	return true;
}

/*
================
RepairSubsystem::Unknown565600

Moves the subsystem one place towards the front of the repair list and
updates the repair pane; nothing when it is already first. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00565600
void RepairSubsystem::Unknown565600( ShipSubsystem *subsystem ) {
	NiTListIterator pos = repairList.FindPos( subsystem );
	if ( pos != NULL ) {
		if ( pos == repairList.GetHeadPos() ) {
			return;
		}
		NiTListIterator prev = repairList.GetPrevPos( pos );
		repairList.RemovePos( pos );
		repairList.InsertBefore( prev, subsystem );
	}
	Unknown565D30();
}

/*
================
RepairSubsystem::Unknown565750

Moves the subsystem one place towards the end of the repair list and
updates the repair pane; nothing when it is already last. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00565750
void RepairSubsystem::Unknown565750( ShipSubsystem *subsystem ) {
	NiTListIterator pos = repairList.FindPos( subsystem );
	if ( pos != NULL ) {
		if ( pos == repairList.GetTailPos() ) {
			return;
		}
		NiTListIterator next = repairList.GetNextPos( pos );
		repairList.RemovePos( pos );
		repairList.InsertAfter( next, subsystem );
	}
	Unknown565D30();
}

/*
================
RepairSubsystem::IsBeingRepaired

Whether the subsystem is one of the first subsystems on the repair list,
as many as there are repair teams.
================
*/
// FUNCTION: STBC 0x00565890
bool RepairSubsystem::IsBeingRepaired( ShipSubsystem *subsystem ) {
	NiTListIterator pos = repairList.GetHeadPos();
	int numTeams = GetProperty()->GetNumRepairTeams();
	for ( int i = 0; i < numTeams; i++ ) {
		if ( repairList.GetNext( pos ) == subsystem ) {
			return true;
		}
	}
	return false;
}

/*
================
RepairSubsystem::HandleHitEvent

A subsystem that was damaged joins the repair list.
================
*/
// FUNCTION: STBC 0x005658d0
void RepairSubsystem::HandleHitEvent( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( subsystem != NULL ) {
		AddToRepairList( subsystem, true );
	}
	CallNextHandler( event );
}

/*
================
RepairSubsystem::AddToRepairList

On the host of a multiplayer game, the clients are told about the
subsystem. (The second argument is not used.)
================
*/
// FUNCTION: STBC 0x00565900
void RepairSubsystem::AddToRepairList( ShipSubsystem *subsystem, bool ) {
	if ( AddSubsystem( subsystem ) && g_kUtopiaModule.IsMultiplayer() && g_kUtopiaModule.IsHost() ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_ADD_TO_REPAIR_LIST );
		event->SetDestination( this );
		event->SetSource( subsystem );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
RepairSubsystem::HandleRepairCompleted

The repaired subsystem leaves the repair list, and the player's repair
pane drops it.
================
*/
// FUNCTION: STBC 0x00565980
void RepairSubsystem::HandleRepairCompleted( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)( (TGObjPtrEvent *)event )->GetObjPtr();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( subsystem != NULL ) {
		repairList.Remove( subsystem );
		if ( player == subsystem->GetParentShip() && EngRepairPane::GetRepairPane() != NULL ) {
			EngRepairPane::GetRepairPane()->RemoveSubsystem( subsystem );
		}
	}
	CallNextHandler( event );
}

/*
================
RepairSubsystem::HandleSubsystemRebuilt

A rebuilt subsystem leaves the repair pane, and joins the repair list
again while it is not at its full condition.
================
*/
// FUNCTION: STBC 0x00565a10
void RepairSubsystem::HandleSubsystemRebuilt( TGEvent *event ) {
	ShipSubsystem *subsystem = ShipSubsystem::Cast( event->GetSource() );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( subsystem != NULL && player != NULL ) {
		// BUG: the pane is used without the NULL test the other handlers make.
		EngRepairPane::GetRepairPane()->RemoveSubsystem( subsystem );
		float condition = subsystem->GetCondition();
		if ( condition < subsystem->GetMaxCondition() ) {
			AddToRepairList( subsystem, false );
		}
	}
	CallNextHandler( event );
}

/*
================
RepairSubsystem::HandleRepairCannotBeCompleted

The subsystem leaves the repair list, and the player's repair pane moves
it to the destroyed systems.
================
*/
// FUNCTION: STBC 0x00565a80
// PARKED: retail calls the list's GetNext out of line in the search (inline budget) and keeps the player in the frame; Remove and FindPos/RemovePos spellings both inline it
void RepairSubsystem::HandleRepairCannotBeCompleted( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)( (TGObjPtrEvent *)event )->GetObjPtr();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( subsystem != NULL ) {
		repairList.Remove( subsystem );
		if ( player == subsystem->GetParentShip() && EngRepairPane::GetRepairPane() != NULL ) {
			EngRepairPane::GetRepairPane()->RemoveSubsystem( subsystem );
			EngRepairPane::GetRepairPane()->AddDestroyedSubsystem( subsystem );
		}
	}
	CallNextHandler( event );
}

/*
================
RepairSubsystem::HandleAddToRepairList

On a client, the event's source joins the repair list; the host adds it
itself.
================
*/
// FUNCTION: STBC 0x00565b30
void RepairSubsystem::HandleAddToRepairList( TGEvent *event ) {
	if ( !g_kUtopiaModule.IsHost() ) {
		AddToRepairList( (ShipSubsystem *)event->GetSource(), false );
	}
}

/*
================
RepairSubsystem::HandleIncreasePriorityEvent

A subsystem on the repair list moves to its end when a team is on it and to
its front otherwise; the repair pane is updated either way.
================
*/
// FUNCTION: STBC 0x00565b50
void RepairSubsystem::HandleIncreasePriorityEvent( TGEvent *event ) {
	ShipSubsystem *subsystem = (ShipSubsystem *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( subsystem != NULL ) {
		NiTListIterator pos = repairList.FindPos( subsystem );
		if ( pos != NULL ) {
			bool beingRepaired = IsBeingRepaired( subsystem );
			repairList.RemovePos( pos );
			if ( beingRepaired == true ) {
				repairList.AddTail( subsystem );
			} else {
				repairList.AddHead( subsystem );
			}
		}
		Unknown565D30();
	}
	CallNextHandler( event );
}

/*
================
RepairSubsystem::HandleSetPlayer

The player changed: the repair pane shows the new player's repair
subsystem.
================
*/
// FUNCTION: STBC 0x00565cd0
void RepairSubsystem::HandleSetPlayer( TGEventHandlerObject *object, TGEvent *event ) {
	EngRepairPane *pane = EngRepairPane::GetRepairPane();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( pane != NULL && player != ShipClass::Cast( event->GetSource() ) ) {
		pane->ClearAreas();
		if ( player != NULL ) {
			RepairSubsystem *repair = player->GetRepairSubsystem();
			pane->SetRepairSubsystem( repair );
			pane->SetNumRepairTeams( repair->GetProperty()->GetNumRepairTeams() );
		}
	}
}

/*
================
RepairSubsystem::Unknown565D30

Updates the repair pane's areas, when there is a pane.
================
*/
// FUNCTION: STBC 0x00565d30
void RepairSubsystem::Unknown565D30( void ) {
	if ( EngRepairPane::GetRepairPane() != NULL ) {
		EngRepairPane::GetRepairPane()->UpdateAreas();
	}
}

/*
================
RepairSubsystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00565d40
void RepairSubsystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleHitEvent, "RepairSubsystem::HandleHitEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleRepairCompleted, "RepairSubsystem::HandleRepairCompleted" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleSubsystemRebuilt, "RepairSubsystem::HandleSubsystemRebuilt" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleRepairCannotBeCompleted, "RepairSubsystem::HandleRepairCannotBeCompleted" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleIncreasePriorityEvent, "RepairSubsystem::HandleIncreasePriorityEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RepairSubsystem::HandleAddToRepairList, "RepairSubsystem::HandleAddToRepairList" );
	g_kEventManager.RegisterHandlerFunction( RepairSubsystem::HandleSetPlayer, "RepairSubsystem::HandleSetPlayer" );
}

/*
================
RepairSubsystem::RegisterHandlers

Besides the class's own handlers, every change of player reaches
HandleSetPlayer.
================
*/
// FUNCTION: STBC 0x00565dd0
void RepairSubsystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_REPAIR_INCREASE_PRIORITY, "RepairSubsystem::HandleIncreasePriorityEvent" );
	AddMethodHandlerForClass( &classHandlers, ET_ADD_TO_REPAIR_LIST, "RepairSubsystem::HandleAddToRepairList" );
	g_kEventManager.AddBroadcastFuncHandler( ET_SET_PLAYER, NULL, "RepairSubsystem::HandleSetPlayer" );
}

/*
================
RepairSubsystem::WriteToStream

The repair list: its length, then each subsystem's ID.
================
*/
// FUNCTION: STBC 0x00565e20
bool RepairSubsystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( repairList.GetSize() );
	NiTListIterator pos = repairList.GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = repairList.GetNext( pos );
		stream->WriteID( subsystem ? subsystem->GetObjID() : 0 );
	}
	return true;
}

/*
================
RepairSubsystem::ReadFromStream

Reads what WriteToStream writes; the subsystems stay IDs until
PostFixUp.
================
*/
// FUNCTION: STBC 0x00565e80
bool RepairSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		repairList.AddTail( (ShipSubsystem *)stream->ReadID() );
	}
	return true;
}

/*
================
RepairSubsystem::FixUpPointers

Fails when a subsystem on the repair list has not been loaded.
================
*/
// FUNCTION: STBC 0x00565f10
bool RepairSubsystem::FixUpPointers( void ) {
	if ( !PoweredSubsystem::FixUpPointers() ) {
		return false;
	}

	NiTListIterator pos = repairList.GetHeadPos();
	while ( pos != NULL ) {
		ShipSubsystem *subsystem = repairList.GetNext( pos );
		if ( subsystem != NULL && GetTGObjectPtr( (long)subsystem ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
RepairSubsystem::PostFixUp

Turns the IDs on the repair list back into subsystems, keeping the order.
================
*/
// FUNCTION: STBC 0x00565f50
void RepairSubsystem::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();
	NiTList<ShipSubsystem *> subsystems( 2 );
	NiTListIterator pos = repairList.GetHeadPos();
	while ( pos != NULL ) {
		long id = (long)repairList.GetNext( pos );
		subsystems.AddTail( id != 0 ? (ShipSubsystem *)GetTGObjectPtr( id ) : NULL );
	}
	repairList.RemoveAll();
	pos = subsystems.GetHeadPos();
	while ( pos != NULL ) {
		repairList.AddTail( subsystems.GetNext( pos ) );
	}
}
