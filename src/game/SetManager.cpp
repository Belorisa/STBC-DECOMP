/*
===============================================================================

	SetManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include "SetManager.h"
#include "PyEmbed.h"
#include "TGIconManager.h"
#include "TGSoundRegion.h"
#include "PlacementObject.h"
#include "ProximityCheck.h"
#include "UtopiaModule.h"
#include "SetClass.h"
#include "Game.h"
#include "ShipClass.h"
#include "CameraObjectClass.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004190c0 _$E508
// FUNCTION: STBC 0x004190e0 _$E507

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00418950 _$E32
// FUNCTION: STBC 0x00418960 _$E29
// FUNCTION: STBC 0x00418970 _$E31
// FUNCTION: STBC 0x00418980 _$E30
// FUNCTION: STBC 0x00418990 _$E38
// FUNCTION: STBC 0x004189a0 _$E35
// FUNCTION: STBC 0x004189b0 _$E37
// FUNCTION: STBC 0x004189c0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004189d0 _$E66
// FUNCTION: STBC 0x004189e0 _$E65
// FUNCTION: STBC 0x004189f0 _$E249
// FUNCTION: STBC 0x00418a00 _$E248
// FUNCTION: STBC 0x00418a10 _$E77
// FUNCTION: STBC 0x00418a20 _$E76
// FUNCTION: STBC 0x00418a30 _$E81
// FUNCTION: STBC 0x00418a40 _$E80
// FUNCTION: STBC 0x00418a50 _$E85
// FUNCTION: STBC 0x00418a60 _$E84
// FUNCTION: STBC 0x00418a70 _$E89
// FUNCTION: STBC 0x00418a80 _$E88
// FUNCTION: STBC 0x00418a90 _$E206
// FUNCTION: STBC 0x00418aa0 _$E205
// FUNCTION: STBC 0x00418ab0 _$E148
// FUNCTION: STBC 0x00418ac0 _$E147
// FUNCTION: STBC 0x00418ad0 _$E161
// FUNCTION: STBC 0x00418ae0 _$E160
// FUNCTION: STBC 0x00418af0 _$E211
// FUNCTION: STBC 0x00418b00 _$E210
// FUNCTION: STBC 0x00418b10 _$E254
// FUNCTION: STBC 0x00418b20 _$E253
// FUNCTION: STBC 0x00418b30 _$E215
// FUNCTION: STBC 0x00418b40 _$E214
// FUNCTION: STBC 0x00418b50 _$E258
// FUNCTION: STBC 0x00418b60 _$E257
// FUNCTION: STBC 0x00418b70 _$E265
// FUNCTION: STBC 0x00418b80 _$E264
// FUNCTION: STBC 0x00418b90 _$E345
// FUNCTION: STBC 0x00418ba0 _$E344
// FUNCTION: STBC 0x00418bb0 _$E349
// FUNCTION: STBC 0x00418bc0 _$E348
// FUNCTION: STBC 0x00418bd0 _$E106
// FUNCTION: STBC 0x00418be0 _$E105
// FUNCTION: STBC 0x00418bf0 _$E303
// FUNCTION: STBC 0x00418c00 _$E302
// FUNCTION: STBC 0x00418c10 _$E310
// FUNCTION: STBC 0x00418c20 _$E309
// FUNCTION: STBC 0x00418c30 _$E314
// FUNCTION: STBC 0x00418c40 _$E313
// FUNCTION: STBC 0x00418c50 _$E318
// FUNCTION: STBC 0x00418c60 _$E317
// FUNCTION: STBC 0x00418c70 _$E322
// FUNCTION: STBC 0x00418c80 _$E321
// FUNCTION: STBC 0x00418c90 _$E328
// FUNCTION: STBC 0x00418ca0 _$E327
// FUNCTION: STBC 0x00418cb0 _$E353
// FUNCTION: STBC 0x00418cc0 _$E352
// FUNCTION: STBC 0x00418cd0 _$E357
// FUNCTION: STBC 0x00418ce0 _$E356
// FUNCTION: STBC 0x00418cf0 _$E361
// FUNCTION: STBC 0x00418d00 _$E360
// FUNCTION: STBC 0x00418d10 _$E365
// FUNCTION: STBC 0x00418d20 _$E364
// FUNCTION: STBC 0x00418d30 _$E369
// FUNCTION: STBC 0x00418d40 _$E368
// FUNCTION: STBC 0x00418d50 _$E373
// FUNCTION: STBC 0x00418d60 _$E372
// FUNCTION: STBC 0x00418d70 _$E377
// FUNCTION: STBC 0x00418d80 _$E376
// FUNCTION: STBC 0x00418d90 _$E381
// FUNCTION: STBC 0x00418da0 _$E380
// FUNCTION: STBC 0x00418e00 _$E385
// FUNCTION: STBC 0x00418e10 _$E384
// FUNCTION: STBC 0x00418e20 _$E389
// FUNCTION: STBC 0x00418e30 _$E388
// FUNCTION: STBC 0x00418e40 _$E111
// FUNCTION: STBC 0x00418e50 _$E110
// FUNCTION: STBC 0x00418e60 _$E393
// FUNCTION: STBC 0x00418e70 _$E392
// FUNCTION: STBC 0x00418e80 _$E116
// FUNCTION: STBC 0x00418e90 _$E115
// FUNCTION: STBC 0x00418ea0 _$E397
// FUNCTION: STBC 0x00418eb0 _$E396
// FUNCTION: STBC 0x00418ec0 _$E401
// FUNCTION: STBC 0x00418ed0 _$E400
// FUNCTION: STBC 0x00418ee0 _$E405
// FUNCTION: STBC 0x00418ef0 _$E404
// FUNCTION: STBC 0x00418f00 _$E409
// FUNCTION: STBC 0x00418f10 _$E408
// FUNCTION: STBC 0x00418f20 _$E125
// FUNCTION: STBC 0x00418f30 _$E124
// FUNCTION: STBC 0x00418f40 _$E413
// FUNCTION: STBC 0x00418f50 _$E412
// FUNCTION: STBC 0x00418f60 _$E417
// FUNCTION: STBC 0x00418f70 _$E416
// FUNCTION: STBC 0x00418f80 _$E421
// FUNCTION: STBC 0x00418f90 _$E420
// FUNCTION: STBC 0x00418fa0 _$E131
// FUNCTION: STBC 0x00418fb0 _$E130
// FUNCTION: STBC 0x00418fc0 _$E136
// FUNCTION: STBC 0x00418fd0 _$E135
// FUNCTION: STBC 0x00418fe0 _$E166
// FUNCTION: STBC 0x00418ff0 _$E165
// FUNCTION: STBC 0x00419000 _$E425
// FUNCTION: STBC 0x00419010 _$E424
// FUNCTION: STBC 0x00419020 _$E171
// FUNCTION: STBC 0x00419030 _$E170
// FUNCTION: STBC 0x00419040 _$E429
// FUNCTION: STBC 0x00419050 _$E428
// FUNCTION: STBC 0x00419060 _$E433
// FUNCTION: STBC 0x00419070 _$E432
// FUNCTION: STBC 0x00419080 _$E176
// FUNCTION: STBC 0x00419090 _$E175
// FUNCTION: STBC 0x004190a0 _$E437
// FUNCTION: STBC 0x004190b0 _$E436
// FUNCTION: STBC 0x004190f0 _$E141
// FUNCTION: STBC 0x00419100 _$E140
// FUNCTION: STBC 0x00419110 _$E441
// FUNCTION: STBC 0x00419120 _$E440
// FUNCTION: STBC 0x00419130 _$E445
// FUNCTION: STBC 0x00419140 _$E444
// FUNCTION: STBC 0x00419150 _$E181
// FUNCTION: STBC 0x00419160 _$E180
// FUNCTION: STBC 0x00419170 _$E186
// FUNCTION: STBC 0x00419180 _$E185
// FUNCTION: STBC 0x00419190 _$E449
// FUNCTION: STBC 0x004191a0 _$E448
// FUNCTION: STBC 0x004191b0 _$E453
// FUNCTION: STBC 0x004191c0 _$E452
// FUNCTION: STBC 0x004191d0 _$E457
// FUNCTION: STBC 0x004191e0 _$E456
// FUNCTION: STBC 0x004191f0 _$E461
// FUNCTION: STBC 0x00419200 _$E460
// FUNCTION: STBC 0x00419210 _$E465
// FUNCTION: STBC 0x00419220 _$E464
// FUNCTION: STBC 0x00419230 _$E469
// FUNCTION: STBC 0x00419240 _$E468
// FUNCTION: STBC 0x00419250 _$E473
// FUNCTION: STBC 0x00419260 _$E472
// FUNCTION: STBC 0x00419270 _$E477
// FUNCTION: STBC 0x00419280 _$E476
// FUNCTION: STBC 0x00419290 _$E191
// FUNCTION: STBC 0x004192a0 _$E190
// FUNCTION: STBC 0x004192b0 _$E481
// FUNCTION: STBC 0x004192c0 _$E480
// FUNCTION: STBC 0x004192d0 _$E196
// FUNCTION: STBC 0x004192e0 _$E195
// FUNCTION: STBC 0x004192f0 _$E201
// FUNCTION: STBC 0x00419300 _$E200
// FUNCTION: STBC 0x00419310 _$E485
// FUNCTION: STBC 0x00419320 _$E484
// FUNCTION: STBC 0x00419330 _$E490
// FUNCTION: STBC 0x00419340 _$E489
// FUNCTION: STBC 0x00419350 _$E494
// FUNCTION: STBC 0x00419360 _$E493

// The game's set manager and its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00417e00 _$E496
// FUNCTION: STBC 0x00417e10 _$E498
// FUNCTION: STBC 0x00417e20 _$E497
// DATA: STBC 0x0097e9c0
SetManager g_kSetManager;

/*
================
SetManager::SetManager
================
*/
// FUNCTION: STBC 0x00417e30
SetManager::SetManager( void ) {
	renderedSet = NULL;
	unknown14 = 0;
}

/*
================
SetManager::~SetManager

Deletes the sets still in the manager.
================
*/
// FUNCTION: STBC 0x00417e70
SetManager::~SetManager( void ) {
	Terminate();
}

/*
================
SetManager::AddSet

Adds the set under the name. A set that has the name already is deleted;
the player's ship and camera, if they were in it, move to the new one.
================
*/
// FUNCTION: STBC 0x00417f00
void SetManager::AddSet( SetClass *set, char *name ) {
	ShipClass *movedShip = NULL;
	CameraObjectClass *movedCamera = NULL;
	bool cameraWasActive = false;

	SetClass *oldSet = sets.Get( name );
	if ( oldSet != NULL ) {
		Game *game = Game::GetCurrentGame();
		ShipClass *player = game->GetPlayer();
		if ( player != NULL && player->GetContainingSet() == oldSet ) {
			movedShip = player;
			oldSet->RemoveObjectFromSet( movedShip->GetObjID() );
		}
		CameraObjectClass *camera = game->GetPlayerCamera();
		if ( camera != NULL && camera->GetContainingSet() == oldSet ) {
			movedCamera = camera;
			oldSet->RemoveCameraFromSet( camera->GetName() );
			cameraWasActive = camera->IsActive();
		}
		DeleteSet( name );
	}

	set->SetName( name );
	sets.Insert( set );
	TGSoundRegion *region = new TGSoundRegion( name );
	region->SetFilter( true );

	if ( movedShip != NULL ) {
		set->AddObjectToSet( movedShip, movedShip->GetName() );
	}
	if ( movedCamera != NULL ) {
		set->AddCameraToSet( movedCamera, movedCamera->GetName() );
		if ( cameraWasActive ) {
			set->SetActiveCamera( movedCamera->GetName() );
		}
	}
}

/*
================
SetManager::RemoveSet

Takes the set out of the manager without deleting it, and deletes the
sound region filed under its name.
================
*/
// FUNCTION: STBC 0x004180e0
void SetManager::RemoveSet( char *name ) {
	SetClass *set = sets.Get( name );
	if ( set != NULL && renderedSet == set ) {
		renderedSet = NULL;
	}
	sets.Remove( name );
	delete TGSoundRegion::GetRegion( name );
}

/*
================
SetManager::DeleteSet

Deletes the set and its placements, and the sound region filed under its
name.
================
*/
// FUNCTION: STBC 0x004181e0
void SetManager::DeleteSet( char *name ) {
	SetClass *set = sets.Get( name );
	if ( set != NULL ) {
		PlacementObject::DeleteSetPlacements( set );
		if ( renderedSet == set ) {
			renderedSet = NULL;
		}
	}
	sets.Delete( name );
	delete TGSoundRegion::GetRegion( name );
}

/*
================
SetManager::MakeRenderedSet

Makes the set of the name the rendered one: the old set's sound region
goes back to filtering, the new one's stops, the icon manager draws for the
new set's camera, and the scripts put the player's camera where the set
asks.
================
*/
// FUNCTION: STBC 0x004182f0 ?MakeRenderedSet@SetManager@@QAEXPAD@Z
void SetManager::MakeRenderedSet( char *name ) {
	int count = sets.GetCount();
	SetClass **items = sets.GetItems();
	for ( int i = 0; i < count; i++ ) {
		if ( strcmp( name, items[i]->GetName() ) != 0 ) {
			continue;
		}

		if ( renderedSet != NULL ) {
			TGSoundRegion *region = TGSoundRegion::GetRegion( renderedSet->GetName() );
			if ( Game::GetCurrentGame() != NULL ) {
				SetClass *playerSet = Game::GetCurrentGame()->GetPlayerSet();
				if ( playerSet == renderedSet && region != NULL ) {
					region->SetFilter( 2 );
				} else if ( strcmp( renderedSet->GetName(), "bridge" ) != 0 && region != NULL ) {
					region->SetFilter( 1 );
				}
			}
		}

		renderedSet = items[i];
		NiCamera *camera = renderedSet->GetActiveNiCamera();
		if ( camera != NULL ) {
			g_kIconManager.SetCamera( camera );
		}
		if ( renderedSet != NULL ) {
			TGSoundRegion *region = TGSoundRegion::GetRegion( renderedSet->GetName() );
			if ( region != NULL ) {
				region->SetFilter( 0 );
			}
		}

		if ( strcmp( "bridge", name ) == 0 ) {
			PyEmbed::CallFunction( "Camera", "PlayerCameraAsViewscreen", "", NULL, "()" );
			PyErr_Print();
		} else if ( Game::GetCurrentGame() != NULL ) {
			SetClass *playerSet = Game::GetCurrentGame()->GetPlayerSet();
			if ( playerSet != NULL && strcmp( playerSet->GetName(), name ) == 0 ) {
				PyEmbed::CallFunction( "Camera", "PlayerCameraAsSpace", "", NULL, "()" );
				PyErr_Print();
			}
		}
		return;
	}
}

/*
================
SetManager::MakeRenderedSet

Renders the set by its name; nothing for NULL.
================
*/
// FUNCTION: STBC 0x00418510 ?MakeRenderedSet@SetManager@@QAEXPAVSetClass@@@Z
void SetManager::MakeRenderedSet( SetClass *set ) {
	if ( set != NULL ) {
		MakeRenderedSet( set->GetName() );
	}
}

/*
================
SetManager::Update

Steps the sets by the time. A single-player game steps the rendered sets
and the player's every frame and one of the others in turn (unknown14 is
the turn); a multiplayer game steps every set. Then brings up to 12
proximity checks up to date.
================
*/
// FUNCTION: STBC 0x00418530
void SetManager::Update( float time ) {
	Game *game = Game::GetCurrentGame();
	int count = sets.GetCount();
	SetClass **items = sets.GetItems();
	if ( unknown14 >= count ) {
		unknown14 = 0;
	}
	if ( !g_kUtopiaModule.IsMultiplayer() ) {
		int i;
		for ( i = 0; i < count; i++ ) {
			SetClass *set = items[unknown14];
			unknown14 = ( unknown14 + 1 ) % count;
			if ( !set->IsRendered() && ( game == NULL || game->GetPlayerSet() != set ) ) {
				set->Update( time );
				break;
			}
		}
		for ( i = 0; i < count; i++ ) {
			SetClass *set = items[i];
			if ( set->IsRendered() || ( game != NULL && game->GetPlayerSet() == set ) ) {
				set->Update( time );
			}
		}
	} else {
		for ( int i = 0; i < count; i++ ) {
			items[i]->Update( time );
		}
	}
	ProximityCheck::UpdateChecks( 12 );
}

/*
================
SetManager::Unknown418630

Passes the time to every set's Unknown410460.
================
*/
// FUNCTION: STBC 0x00418630
void SetManager::Unknown418630( float time ) {
	int count = sets.GetCount();
	SetClass **items = sets.GetItems();
	for ( int i = 0; i < count; i++ ) {
		items[i]->Unknown410460( time );
	}
}

/*
================
SetManager::Unknown418660

Hands the frame to the rendered set by calling its VirtualSlot26, when there is one.
================
*/
// FUNCTION: STBC 0x00418660
void SetManager::Unknown418660( void ) {
	if ( renderedSet != NULL ) {
		renderedSet->VirtualSlot26();
	}
}

/*
================
SetManager::Terminate

Deletes every set and forgets the rendered one.
================
*/
// FUNCTION: STBC 0x00418670
void SetManager::Terminate( void ) {
	renderedSet = NULL;
	sets.DeleteAll();
	renderedSet = NULL;
}

/*
================
SetManager::WriteToStream

Writes the rendered set's ID, unknown14 and every set; false when a set
fails to write itself. The name is invented, after the other managers'.
================
*/
// FUNCTION: STBC 0x004186d0
bool SetManager::WriteToStream( TGStream *stream ) {
	stream->WriteID( TGObject::GetSavedID( renderedSet ) );
	stream->WriteInt( unknown14 );
	int count = sets.GetCount();
	SetClass **items = sets.GetItems();
	stream->WriteLong( count );
	for ( int i = 0; i < count; i++ ) {
		if ( !items[i]->WriteToStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
SetManager::ReadFromStream

Reads what WriteToStream wrote. The rendered set's ID waits in renderedSet
until the set with that ID is loaded. The name is invented, after the
other managers'.
================
*/
// FUNCTION: STBC 0x00418740
bool SetManager::ReadFromStream( TGStream *stream ) {
	renderedSet = (SetClass *)stream->ReadID();
	unknown14 = stream->ReadInt();
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		SetClass *set = (SetClass *)TGObject::LoadObject( stream, 0 );
		if ( set == NULL ) {
			return false;
		}
		if ( (long)renderedSet == set->GetObjID() ) {
			renderedSet = set;
		}
		sets.Insert( set );
	}
	return true;
}

/*
================
SetManager::RestoreSets

Has every loaded set set its active camera and re-attach its objects
(SetClass::VirtualSlot30), once the load has fixed up its pointers. The
name is invented.
================
*/
// FUNCTION: STBC 0x00418890
void SetManager::RestoreSets( void ) {
	int count = sets.GetCount();
	SetClass **items = sets.GetItems();
	for ( int i = 0; i < count; i++ ) {
		items[i]->VirtualSlot30();
	}
}

/*
================
SetManager_GetAllSets

The Python method SetManager.GetAllSets: a tuple of new shadow instances
of the manager's sets.
================
*/
// FUNCTION: STBC 0x004188b0
PyObject *SetManager_GetAllSets( PyObject *self, PyObject *args ) {
	PyObject *tuple = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		SetManager *manager = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&manager, "SetManager" );
		if ( manager != NULL ) {
			int count = manager->GetNumSets();
			SetClass **sets = manager->GetSets();
			tuple = PyTuple_New( count );
			for ( int i = 0; i < count; i++ ) {
				PyTuple_SetItem( tuple, i, sets[i]->VirtualSlot08() );
			}
		}
	}
	return tuple;
}

/*
================
SetManager::Clear

Forgets the rendered set and the sets without deleting them. The name is
invented, after AIManager::Clear.
================
*/
// FUNCTION: STBC 0x00418940
void SetManager::Clear( void ) {
	renderedSet = NULL;
	sets.RemoveAll();
}
