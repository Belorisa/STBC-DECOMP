/*
===============================================================================

	AIManager.cpp

===============================================================================
*/

#include <iostream>
#include "AILog.h"
#include "AIManager.h"
#include "ArtificialIntelligence.h"
#include "BuilderAI.h"
#include "PhysicsObjectClass.h"
#include "ShipClass.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004736a0 _$E501
// FUNCTION: STBC 0x004736c0 _$E500

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004727d0 _$E32
// FUNCTION: STBC 0x004727e0 _$E29
// FUNCTION: STBC 0x004727f0 _$E31
// FUNCTION: STBC 0x00472800 _$E30
// FUNCTION: STBC 0x00472980 _$E38
// FUNCTION: STBC 0x00472990 _$E35
// FUNCTION: STBC 0x004729a0 _$E37
// FUNCTION: STBC 0x004729b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00472dd0 _$E95
// FUNCTION: STBC 0x00472de0 _$E94
// FUNCTION: STBC 0x00472df0 _$E304
// FUNCTION: STBC 0x00472e00 _$E303
// FUNCTION: STBC 0x00472e10 _$E106
// FUNCTION: STBC 0x00472e20 _$E105
// FUNCTION: STBC 0x00472e30 _$E110
// FUNCTION: STBC 0x00472e40 _$E109
// FUNCTION: STBC 0x00472e50 _$E114
// FUNCTION: STBC 0x00472e60 _$E113
// FUNCTION: STBC 0x00472e70 _$E118
// FUNCTION: STBC 0x00472e80 _$E117
// FUNCTION: STBC 0x00472e90 _$E235
// FUNCTION: STBC 0x00472ea0 _$E234
// FUNCTION: STBC 0x00472eb0 _$E177
// FUNCTION: STBC 0x00472ec0 _$E176
// FUNCTION: STBC 0x00472ed0 _$E190
// FUNCTION: STBC 0x00472ee0 _$E189
// FUNCTION: STBC 0x00472ef0 _$E240
// FUNCTION: STBC 0x00472f00 _$E239
// FUNCTION: STBC 0x00472f10 _$E309
// FUNCTION: STBC 0x00472f20 _$E308
// FUNCTION: STBC 0x00472f30 _$E244
// FUNCTION: STBC 0x00472f40 _$E243
// FUNCTION: STBC 0x00472f50 _$E313
// FUNCTION: STBC 0x00472f60 _$E312
// FUNCTION: STBC 0x00472f70 _$E320
// FUNCTION: STBC 0x00472f80 _$E319
// FUNCTION: STBC 0x00472f90 _$E338
// FUNCTION: STBC 0x00472fa0 _$E337
// FUNCTION: STBC 0x00472fb0 _$E342
// FUNCTION: STBC 0x00472fc0 _$E341
// FUNCTION: STBC 0x00472fd0 _$E135
// FUNCTION: STBC 0x00472fe0 _$E134
// FUNCTION: STBC 0x00472ff0 _$E263
// FUNCTION: STBC 0x00473000 _$E262
// FUNCTION: STBC 0x00473010 _$E270
// FUNCTION: STBC 0x00473020 _$E269
// FUNCTION: STBC 0x00473030 _$E274
// FUNCTION: STBC 0x00473040 _$E273
// FUNCTION: STBC 0x00473050 _$E278
// FUNCTION: STBC 0x00473060 _$E277
// FUNCTION: STBC 0x00473070 _$E282
// FUNCTION: STBC 0x00473080 _$E281
// FUNCTION: STBC 0x00473090 _$E288
// FUNCTION: STBC 0x004730a0 _$E287
// FUNCTION: STBC 0x004730b0 _$E346
// FUNCTION: STBC 0x004730c0 _$E345
// FUNCTION: STBC 0x004730d0 _$E350
// FUNCTION: STBC 0x004730e0 _$E349
// FUNCTION: STBC 0x004730f0 _$E354
// FUNCTION: STBC 0x00473100 _$E353
// FUNCTION: STBC 0x00473110 _$E358
// FUNCTION: STBC 0x00473120 _$E357
// FUNCTION: STBC 0x00473130 _$E362
// FUNCTION: STBC 0x00473140 _$E361
// FUNCTION: STBC 0x00473150 _$E366
// FUNCTION: STBC 0x00473160 _$E365
// FUNCTION: STBC 0x00473170 _$E370
// FUNCTION: STBC 0x00473180 _$E369
// FUNCTION: STBC 0x00473190 _$E374
// FUNCTION: STBC 0x004731a0 _$E373
// FUNCTION: STBC 0x004731b0 _$E378
// FUNCTION: STBC 0x004731c0 _$E377
// FUNCTION: STBC 0x004731d0 _$E382
// FUNCTION: STBC 0x004731e0 _$E381
// FUNCTION: STBC 0x004731f0 _$E140
// FUNCTION: STBC 0x00473200 _$E139
// FUNCTION: STBC 0x00473210 _$E386
// FUNCTION: STBC 0x00473220 _$E385
// FUNCTION: STBC 0x00473230 _$E145
// FUNCTION: STBC 0x00473240 _$E144
// FUNCTION: STBC 0x00473250 _$E390
// FUNCTION: STBC 0x00473260 _$E389
// FUNCTION: STBC 0x00473270 _$E394
// FUNCTION: STBC 0x00473280 _$E393
// FUNCTION: STBC 0x00473290 _$E398
// FUNCTION: STBC 0x004732a0 _$E397
// FUNCTION: STBC 0x004732d0 _$E402
// FUNCTION: STBC 0x004732e0 _$E401
// FUNCTION: STBC 0x004732f0 _$E154
// FUNCTION: STBC 0x00473300 _$E153
// FUNCTION: STBC 0x00473310 _$E406
// FUNCTION: STBC 0x00473320 _$E405
// FUNCTION: STBC 0x00473330 _$E410
// FUNCTION: STBC 0x00473340 _$E409
// FUNCTION: STBC 0x00473350 _$E414
// FUNCTION: STBC 0x00473360 _$E413
// FUNCTION: STBC 0x00473370 _$E160
// FUNCTION: STBC 0x00473380 _$E159
// FUNCTION: STBC 0x00473390 _$E165
// FUNCTION: STBC 0x004733a0 _$E164
// FUNCTION: STBC 0x004733b0 _$E195
// FUNCTION: STBC 0x004733c0 _$E194
// FUNCTION: STBC 0x00473400 _$E418
// FUNCTION: STBC 0x00473410 _$E417
// FUNCTION: STBC 0x00473420 _$E200
// FUNCTION: STBC 0x00473430 _$E199
// FUNCTION: STBC 0x00473440 _$E422
// FUNCTION: STBC 0x00473450 _$E421
// FUNCTION: STBC 0x00473460 _$E426
// FUNCTION: STBC 0x00473470 _$E425
// FUNCTION: STBC 0x00473480 _$E205
// FUNCTION: STBC 0x00473490 _$E204
// FUNCTION: STBC 0x004734a0 _$E430
// FUNCTION: STBC 0x004734b0 _$E429
// FUNCTION: STBC 0x004734c0 _$E170
// FUNCTION: STBC 0x004734d0 _$E169
// FUNCTION: STBC 0x004734e0 _$E434
// FUNCTION: STBC 0x004734f0 _$E433
// FUNCTION: STBC 0x00473500 _$E438
// FUNCTION: STBC 0x00473510 _$E437
// FUNCTION: STBC 0x00473520 _$E210
// FUNCTION: STBC 0x00473530 _$E209
// FUNCTION: STBC 0x00473540 _$E215
// FUNCTION: STBC 0x00473550 _$E214
// FUNCTION: STBC 0x00473560 _$E442
// FUNCTION: STBC 0x00473570 _$E441
// FUNCTION: STBC 0x00473580 _$E446
// FUNCTION: STBC 0x00473590 _$E445
// FUNCTION: STBC 0x004735a0 _$E450
// FUNCTION: STBC 0x004735b0 _$E449
// FUNCTION: STBC 0x004735c0 _$E454
// FUNCTION: STBC 0x004735d0 _$E453
// FUNCTION: STBC 0x004735e0 _$E458
// FUNCTION: STBC 0x004735f0 _$E457
// FUNCTION: STBC 0x00473640 _$E462
// FUNCTION: STBC 0x00473650 _$E461
// FUNCTION: STBC 0x00473660 _$E466
// FUNCTION: STBC 0x00473670 _$E465
// FUNCTION: STBC 0x00473680 _$E470
// FUNCTION: STBC 0x00473690 _$E469
// FUNCTION: STBC 0x004736d0 _$E220
// FUNCTION: STBC 0x004736e0 _$E219
// FUNCTION: STBC 0x004736f0 _$E474
// FUNCTION: STBC 0x00473700 _$E473
// FUNCTION: STBC 0x00473710 _$E225
// FUNCTION: STBC 0x00473720 _$E224
// FUNCTION: STBC 0x00473730 _$E230
// FUNCTION: STBC 0x00473740 _$E229
// FUNCTION: STBC 0x004737d0 _$E478
// FUNCTION: STBC 0x004737e0 _$E477
// FUNCTION: STBC 0x004737f0 _$E483
// FUNCTION: STBC 0x00473800 _$E482
// FUNCTION: STBC 0x00473810 _$E487
// FUNCTION: STBC 0x00473820 _$E486

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004720a0 _$E492
// FUNCTION: STBC 0x004720b0 _$E489
// FUNCTION: STBC 0x004720c0 _$E491
// FUNCTION: STBC 0x004720d0 _$E490
// DATA: STBC 0x009817a8
AIManager aiManager;

// The time over which every AI gets its turn, in seconds (invented name).
// A const object, not a literal: Update divides by it in memory.
static const float AI_TURN_TIME = 0.5f;

/*
================
AIManager::AIManager
================
*/
// FUNCTION: STBC 0x004720e0
AIManager::AIManager( void ) {
	updatingAI = NULL;
	deleteUpdatingAI = false;
	unknown20 = g_kSystemWrapper.GetTimeInSeconds();
}

/*
================
AIManager::~AIManager

Deletes the AIs still filed.
================
*/
// FUNCTION: STBC 0x00472120
AIManager::~AIManager( void ) {
	while ( entries.GetSize() != 0 ) {
		AIManagerEntry *entry = entries.RemoveEnd();
		if ( entry != NULL ) {
			delete entry->ai;
			delete entry;
		}
	}
}

/*
================
AIManager::Update

Updates the AIs in turns: every AI gets one update before any gets a
second. The share of the AIs updated this frame grows with the time since
the last frame (all of them over AI_TURN_TIME), at least one AI and at
most the AIs not yet updated this frame. The AIs of type 9 are all updated
first (Unknown472660).
================
*/
// FUNCTION: STBC 0x004721b0
void AIManager::Update( void ) {
	float now = g_kSystemWrapper.GetTimeInSeconds();
	float frameTime = now - unknown20;
	unknown20 = now;
	int numUpdated = Unknown472660();
	int numEntries = entries.GetEffectiveSize();
	if ( numEntries == 0 ) {
		return;
	}

	int numToUpdate = 1;
	if ( frameTime > 0.0f ) {
		numToUpdate = (int)( numEntries * frameTime / AI_TURN_TIME );
	}
	if ( numToUpdate < 1 ) {
		numToUpdate = 1;
	} else if ( numToUpdate > numEntries ) {
		numToUpdate = numEntries;
	}
	if ( numUpdated >= numEntries ) {
		return;
	}
	if ( numToUpdate > numEntries - numUpdated ) {
		numToUpdate = numEntries - numUpdated;
	}

	for ( int n = 0; n < numToUpdate; n++ ) {
		bool found = false;
		int i;
		for ( i = 0; i < (int)entries.GetSize(); i++ ) {
			AIManagerEntry *entry = entries.GetAt( i );
			if ( entry != NULL && !entry->unknown06 ) {
				entry->unknown06 = true;
				UpdateAI( g_kSystemWrapper.GetTimeSinceFrameStart() + AI_UPDATE_TIME, entry );
				found = true;
				break;
			}
		}
		if ( !found ) {
			// Every AI has had its turn: a new round starts, and this
			// update does not count.
			for ( i = 0; i < (int)entries.GetSize(); i++ ) {
				AIManagerEntry *entry = entries.GetAt( i );
				if ( entry != NULL && !entry->ai->VirtualSlot20( 9 ) ) {
					entry->unknown06 = false;
				}
			}
			n--;
		}
	}
}

/*
================
AIManager::UpdateAI
================
*/
// FUNCTION: STBC 0x004722d0
// PARKED: complete; retail allocates the first TGIntEvent through the out-of-line pool allocation 0x40F030 (30 callers, not modelled in TGPoolManager.h) and the second inline; registers follow
void AIManager::UpdateAI( double time, AIManagerEntry *entry ) {
	if ( entry->ai->GetObject() == NULL ) {
		entry->unknown08 = g_kUtopiaModule.GetGameTime();
		return;
	}
	if ( entry->unknown05 && !entry->building ) {
		entry->unknown08 = g_kUtopiaModule.GetGameTime();
		return;
	}

	ArtificialIntelligence *ai = entry->ai;
	AILog log( "Ship(%s)::UpdateAI", ai->GetObject()->GetName() );
	int status = ArtificialIntelligence::US_ACTIVE;
	if ( entry->building ) {
		AILog buildLog( "Building...", ai->GetObject()->GetName() );
		bool updated = false;
		int numBuilds = 0;
		NiTList<ArtificialIntelligence *> ais( 2 );
		ai->GetAllAIsInTree( ais );
		while ( !ais.IsEmpty() ) {
			ArtificialIntelligence *treeAI = ais.RemoveHead();
			if ( treeAI->VirtualSlot20( AI_TYPE_BUILDER ) ) {
				BuilderAI *builder = (BuilderAI *)treeAI;
				bool inTime = true;
				while ( builder->IsBuilding() && inTime ) {
					builder->Update( time );
					numBuilds++;
					updated = true;
					if ( numBuilds >= 7 && g_kSystemWrapper.GetTimeSinceFrameStart() >= time ) {
						inTime = false;
					}
				}
				if ( !inTime ) {
					break;
				}
			}
		}
		if ( !updated ) {
			entry->building = false;
			TGIntEvent *event = new TGIntEvent;
			event->SetEventType( ET_AI_FINISHED_BUILDING );
			event->SetDestination( entry->ai->GetObject() );
			event->SetInt( entry->ai->GetID() );
			g_kEventManager.AddEvent( event );
		}
	} else {
		aiManager.deleteUpdatingAI = false;
		aiManager.updatingAI = ai;
		if ( !ai->IsActive() ) {
			ai->SetActive();
		}
		if ( !ai->HasFocus() ) {
			ai->GotFocus();
		}
		status = ai->Update( time );
		aiManager.updatingAI = NULL;
	}

	if ( aiManager.deleteUpdatingAI ) {
		if ( ai->HasFocus() ) {
			ai->LostFocus();
		}
		if ( ai->IsActive() ) {
			ai->SetInactive();
		}
		delete ai;
		aiManager.deleteUpdatingAI = false;
	} else if ( status == ArtificialIntelligence::US_DONE ) {
		ai->LostFocus();
		ai->SetInactive();
		aiManager.RemoveAI( ai, true );
	} else {
		if ( status == ArtificialIntelligence::US_DORMANT ) {
			TGIntEvent *event = new TGIntEvent;
			event->SetEventType( ET_AI_DORMANT );
			event->SetDestination( ai->GetObject() );
			event->SetInt( ai->GetID() );
			g_kEventManager.AddEvent( event );
		}
		entry->unknown08 = g_kUtopiaModule.GetGameTime();
	}
}

/*
================
AIManager::Unknown472660

Updates every AI of type 9 (a type without a SWIG constant): those have a
turn every frame. Returns how many were updated.
================
*/
// FUNCTION: STBC 0x00472660
int AIManager::Unknown472660( void ) {
	int numUpdated = 0;
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			ArtificialIntelligence *ai = entry->ai;
			if ( ai->VirtualSlot20( 9 ) ) {
				ai->Update( g_kSystemWrapper.GetTimeSinceFrameStart() + AI_UPDATE_TIME );
				numUpdated++;
				entry->unknown06 = true;
			}
		}
	}
	return numUpdated;
}

/*
================
AIManager::AddAI
================
*/
// FUNCTION: STBC 0x004726e0
long AIManager::AddAI( ArtificialIntelligence *ai, bool unknown ) {
	AIManagerEntry *entry = new AIManagerEntry( ai->VirtualSlot13(), unknown );
	entries.AddFirstEmpty( entry );
	// MATCH-PENDING: R17, exact only through the invented AIManagerEntry::GetAI (AIManager.h), the one more inline site after AddFirstEmpty that drops SetAtGrow's share below SetSize's 225 (retail calls SetSize, 0x473750, out of line); no evidence shows it (user's ruling, 2026-11-18)
	return entry->GetAI()->GetID();
}

/*
================
AIManager::RemoveAI

The object hears ET_AI_DONE with the AI's ID. An AI that is not being
updated first loses its focus and is deactivated; the AI being updated
is deleted once its update returns.
================
*/
// FUNCTION: STBC 0x00472810
bool AIManager::RemoveAI( ArtificialIntelligence *ai, bool deleteAI ) {
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL && entry->ai == ai ) {
			TGIntEvent *event = new TGIntEvent;
			event->SetEventType( ET_AI_DONE );
			event->SetDestination( ai->GetObject() );
			event->SetInt( ai->GetID() );
			g_kEventManager.AddEvent( event );

			if ( ai != updatingAI ) {
				AILog log( "Ship(%s)::UpdateAI", ai->GetObject()->GetName() );
				if ( ai->HasFocus() ) {
					ai->LostFocus();
				}
				if ( ai->IsActive() ) {
					ai->SetInactive();
				}
			}

			delete entries.RemoveAt( i );

			if ( deleteAI ) {
				if ( ai == updatingAI ) {
					deleteUpdatingAI = true;
					return true;
				}
				delete ai;
			}
			return true;
		}
	}
	return false;
}

/*
================
AIManager::RemoveBuildingAIs
================
*/
// FUNCTION: STBC 0x004729c0
void AIManager::RemoveBuildingAIs( PhysicsObjectClass *object ) {
	long objectID = object->GetObjID();
	for ( int i = entries.GetSize() - 1; i >= 0; i-- ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL && entry->ai->GetObjectID() == objectID && entry->building ) {
			RemoveAI( entry->ai, true );
		}
	}
}

/*
================
AIManager::Unknown472A00
================
*/
// FUNCTION: STBC 0x00472a00
void AIManager::Unknown472A00( ArtificialIntelligence *ai ) {
}

/*
================
AIManager::RemoveAllAIs
================
*/
// FUNCTION: STBC 0x00472a10
bool AIManager::RemoveAllAIs( void ) {
	for ( int i = entries.GetSize() - 1; i >= 0; i-- ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			RemoveAI( entry->ai, true );
		}
	}
	return true;
}

/*
================
AIManager::GetAI
================
*/
// FUNCTION: STBC 0x00472a40
ArtificialIntelligence *AIManager::GetAI( PhysicsObjectClass *object ) {
	long objectID = object->GetObjID();
	ArtificialIntelligence *buildingAI = NULL;
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			ArtificialIntelligence *ai = entry->ai;
			if ( ai->GetObjectID() == objectID ) {
				if ( !entry->building ) {
					return ai;
				}
				buildingAI = ai;
			}
		}
	}
	return buildingAI;
}

/*
================
AIManager::HasBuildingAIs
================
*/
// FUNCTION: STBC 0x00472a80
bool AIManager::HasBuildingAIs( PhysicsObjectClass *object ) {
	if ( object == NULL ) {
		return false;
	}
	long objectID = object->GetObjID();
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL && entry->ai->GetObjectID() == objectID && entry->building ) {
			return true;
		}
	}
	return false;
}

/*
================
AIManager::WriteToStream

Each AI writes itself, its type first, and the entry's state follows.
================
*/
// FUNCTION: STBC 0x00472ad0
bool AIManager::WriteToStream( TGStream *stream ) {
	stream->WriteInt( entries.GetEffectiveSize() );
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			if ( !entry->ai->WriteToStream( stream ) ) {
				return false;
			}
			stream->WriteBool( entry->building );
			stream->WriteBool( entry->unknown05 );
			stream->WriteBool( entry->unknown06 );
			stream->WriteFloat( entry->unknown08 );
		}
	}
	stream->WriteFloat( unknown20 );
	return true;
}

/*
================
AIManager::ReadFromStream

Reads what WriteToStream wrote; false when an AI cannot be made.
================
*/
// FUNCTION: STBC 0x00472b60
bool AIManager::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		ArtificialIntelligence *ai = ArtificialIntelligence::CreateFromStream( stream );
		if ( ai == NULL ) {
			return false;
		}
		AIManagerEntry *entry = new AIManagerEntry( ai->VirtualSlot13(), false );
		entry->building = stream->ReadBool();
		entry->unknown05 = stream->ReadBool();
		entry->unknown06 = stream->ReadBool();
		entry->unknown08 = stream->ReadFloat();
		entries.AddFirstEmpty( entry );
	}
	unknown20 = stream->ReadFloat();
	updatingAI = NULL;
	deleteUpdatingAI = false;
	return true;
}

/*
================
AIManager::FixUpPointers

Whether every AI's pointers are loaded.
================
*/
// FUNCTION: STBC 0x00472d30
bool AIManager::FixUpPointers( void ) {
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL && !entry->ai->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
AIManager::PostFixUp
================
*/
// FUNCTION: STBC 0x00472d70
void AIManager::PostFixUp( void ) {
	for ( int i = 0; i < (int)entries.GetSize(); i++ ) {
		AIManagerEntry *entry = entries.GetAt( i );
		if ( entry != NULL ) {
			entry->ai->PostFixUp();
		}
	}
}

/*
================
AIManager::Clear

Forgets every entry without deleting the entries or their AIs.
================
*/
// FUNCTION: STBC 0x00472da0
void AIManager::Clear( void ) {
	entries.RemoveAll();
	updatingAI = NULL;
	deleteUpdatingAI = false;
}
