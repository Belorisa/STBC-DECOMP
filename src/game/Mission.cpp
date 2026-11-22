/*
===============================================================================

	Mission.cpp

===============================================================================
*/

#include "Mission.h"
#include "Episode.h"
#include "Game.h"
#include "ShipClass.h"
#include "TGEventManager.h"
#include "TGStringEvent.h"
#include "TGSoundRegion.h"
#include "SetClass.h"
#include "SetManager.h"
#include "ObjectClass.h"
#include "BridgeSet.h"
#include "CinematicWindow.h"
#include "ImageManagerClass.h"
#include "LensFlare.h"
#include "LODModelManager.h"
#include "MainWindow.h"
#include "MultiplayerGame.h"
#include "ObjectGroup.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "SensorSubsystem.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGMovieManager.h"
#include "TGStream.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "ViewScreenObject.h"
#include "WarpSequence.h"
#include <string.h>
#include <iostream>
#include "NiDynamicEffect.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00409c40 _$E528
// FUNCTION: STBC 0x00409c60 _$E527

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00407f40 _$E64
// FUNCTION: STBC 0x00407f50 _$E63
// FUNCTION: STBC 0x00407f60 _$E278
// FUNCTION: STBC 0x00407f70 _$E277
// FUNCTION: STBC 0x00407f80 _$E75
// FUNCTION: STBC 0x00407f90 _$E74
// FUNCTION: STBC 0x00407fa0 _$E79
// FUNCTION: STBC 0x00407fb0 _$E78
// FUNCTION: STBC 0x00407fc0 _$E83
// FUNCTION: STBC 0x00407fd0 _$E82
// FUNCTION: STBC 0x00407fe0 _$E87
// FUNCTION: STBC 0x00407ff0 _$E86
// FUNCTION: STBC 0x00408000 _$E204
// FUNCTION: STBC 0x00408010 _$E203
// FUNCTION: STBC 0x004083f0 _$E146
// FUNCTION: STBC 0x00408400 _$E145
// FUNCTION: STBC 0x00408910 _$E159
// FUNCTION: STBC 0x00408920 _$E158
// FUNCTION: STBC 0x00408be0 _$E209
// FUNCTION: STBC 0x00408bf0 _$E208
// FUNCTION: STBC 0x00409480 _$E283
// FUNCTION: STBC 0x00409490 _$E282
// FUNCTION: STBC 0x004094a0 _$E213
// FUNCTION: STBC 0x004094b0 _$E212
// FUNCTION: STBC 0x004094c0 _$E287
// FUNCTION: STBC 0x004094d0 _$E286
// FUNCTION: STBC 0x004094e0 _$E294
// FUNCTION: STBC 0x004094f0 _$E293
// FUNCTION: STBC 0x00409500 _$E364
// FUNCTION: STBC 0x00409510 _$E363
// FUNCTION: STBC 0x00409520 _$E368
// FUNCTION: STBC 0x00409530 _$E367
// FUNCTION: STBC 0x00409540 _$E104
// FUNCTION: STBC 0x00409550 _$E103
// FUNCTION: STBC 0x00409560 _$E232
// FUNCTION: STBC 0x00409570 _$E231
// FUNCTION: STBC 0x00409580 _$E239
// FUNCTION: STBC 0x00409590 _$E238
// FUNCTION: STBC 0x004095a0 _$E243
// FUNCTION: STBC 0x004095b0 _$E242
// FUNCTION: STBC 0x004095c0 _$E247
// FUNCTION: STBC 0x004095d0 _$E246
// FUNCTION: STBC 0x004095e0 _$E251
// FUNCTION: STBC 0x004095f0 _$E250
// FUNCTION: STBC 0x00409600 _$E257
// FUNCTION: STBC 0x00409610 _$E256
// FUNCTION: STBC 0x00409620 _$E372
// FUNCTION: STBC 0x00409630 _$E371
// FUNCTION: STBC 0x00409640 _$E376
// FUNCTION: STBC 0x00409650 _$E375
// FUNCTION: STBC 0x00409660 _$E380
// FUNCTION: STBC 0x00409670 _$E379
// FUNCTION: STBC 0x00409680 _$E384
// FUNCTION: STBC 0x00409690 _$E383
// FUNCTION: STBC 0x004096a0 _$E388
// FUNCTION: STBC 0x004096b0 _$E387
// FUNCTION: STBC 0x004096c0 _$E392
// FUNCTION: STBC 0x004096d0 _$E391
// FUNCTION: STBC 0x004096e0 _$E396
// FUNCTION: STBC 0x004096f0 _$E395
// FUNCTION: STBC 0x00409700 _$E400
// FUNCTION: STBC 0x00409710 _$E399
// FUNCTION: STBC 0x00409720 _$E404
// FUNCTION: STBC 0x00409730 _$E403
// FUNCTION: STBC 0x00409740 _$E408
// FUNCTION: STBC 0x00409750 _$E407
// FUNCTION: STBC 0x00409760 _$E109
// FUNCTION: STBC 0x00409770 _$E108
// FUNCTION: STBC 0x00409780 _$E412
// FUNCTION: STBC 0x00409790 _$E411
// FUNCTION: STBC 0x004097a0 _$E114
// FUNCTION: STBC 0x004097b0 _$E113
// FUNCTION: STBC 0x004097c0 _$E416
// FUNCTION: STBC 0x004097d0 _$E415
// FUNCTION: STBC 0x004097e0 _$E420
// FUNCTION: STBC 0x004097f0 _$E419
// FUNCTION: STBC 0x00409800 _$E424
// FUNCTION: STBC 0x00409810 _$E423
// FUNCTION: STBC 0x00409820 _$E428
// FUNCTION: STBC 0x00409830 _$E427
// FUNCTION: STBC 0x00409840 _$E123
// FUNCTION: STBC 0x00409850 _$E122
// FUNCTION: STBC 0x00409860 _$E432
// FUNCTION: STBC 0x00409870 _$E431
// FUNCTION: STBC 0x00409880 _$E436
// FUNCTION: STBC 0x00409890 _$E435
// FUNCTION: STBC 0x004098a0 _$E440
// FUNCTION: STBC 0x004098b0 _$E439
// FUNCTION: STBC 0x004098c0 _$E129
// FUNCTION: STBC 0x004098d0 _$E128
// FUNCTION: STBC 0x004098e0 _$E134
// FUNCTION: STBC 0x004098f0 _$E133
// FUNCTION: STBC 0x00409900 _$E164
// FUNCTION: STBC 0x00409910 _$E163
// FUNCTION: STBC 0x00409920 _$E444
// FUNCTION: STBC 0x00409930 _$E443
// FUNCTION: STBC 0x00409940 _$E169
// FUNCTION: STBC 0x00409950 _$E168
// FUNCTION: STBC 0x00409960 _$E448
// FUNCTION: STBC 0x00409970 _$E447
// FUNCTION: STBC 0x00409980 _$E452
// FUNCTION: STBC 0x00409990 _$E451
// FUNCTION: STBC 0x004099a0 _$E174
// FUNCTION: STBC 0x004099b0 _$E173
// FUNCTION: STBC 0x004099c0 _$E456
// FUNCTION: STBC 0x004099d0 _$E455
// FUNCTION: STBC 0x004099e0 _$E139
// FUNCTION: STBC 0x004099f0 _$E138
// FUNCTION: STBC 0x00409a00 _$E460
// FUNCTION: STBC 0x00409a10 _$E459
// FUNCTION: STBC 0x00409a20 _$E464
// FUNCTION: STBC 0x00409a30 _$E463
// FUNCTION: STBC 0x00409a40 _$E179
// FUNCTION: STBC 0x00409a50 _$E178
// FUNCTION: STBC 0x00409a60 _$E184
// FUNCTION: STBC 0x00409a70 _$E183
// FUNCTION: STBC 0x00409a80 _$E468
// FUNCTION: STBC 0x00409a90 _$E467
// FUNCTION: STBC 0x00409aa0 _$E472
// FUNCTION: STBC 0x00409ab0 _$E471
// FUNCTION: STBC 0x00409ac0 _$E476
// FUNCTION: STBC 0x00409ad0 _$E475
// FUNCTION: STBC 0x00409ae0 _$E480
// FUNCTION: STBC 0x00409af0 _$E479
// FUNCTION: STBC 0x00409b00 _$E484
// FUNCTION: STBC 0x00409b10 _$E483
// FUNCTION: STBC 0x00409b20 _$E488
// FUNCTION: STBC 0x00409b30 _$E487
// FUNCTION: STBC 0x00409b40 _$E492
// FUNCTION: STBC 0x00409b50 _$E491
// FUNCTION: STBC 0x00409b60 _$E496
// FUNCTION: STBC 0x00409b70 _$E495
// FUNCTION: STBC 0x00409b80 _$E189
// FUNCTION: STBC 0x00409b90 _$E188
// FUNCTION: STBC 0x00409ba0 _$E500
// FUNCTION: STBC 0x00409bb0 _$E499
// FUNCTION: STBC 0x00409bc0 _$E194
// FUNCTION: STBC 0x00409bd0 _$E193
// FUNCTION: STBC 0x00409c20 _$E199
// FUNCTION: STBC 0x00409c30 _$E198
// FUNCTION: STBC 0x00409c70 _$E504
// FUNCTION: STBC 0x00409c80 _$E503
// FUNCTION: STBC 0x00409c90 _$E509
// FUNCTION: STBC 0x00409ca0 _$E508
// FUNCTION: STBC 0x00409cb0 _$E513
// FUNCTION: STBC 0x00409cc0 _$E512

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00407ec0 _$E348
// FUNCTION: STBC 0x00407ed0 _$E345
// FUNCTION: STBC 0x00407ee0 _$E347
// FUNCTION: STBC 0x00407ef0 _$E346
// FUNCTION: STBC 0x00407f00 _$E354
// FUNCTION: STBC 0x00407f10 _$E351
// FUNCTION: STBC 0x00407f20 _$E353
// FUNCTION: STBC 0x00407f30 _$E352

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00408020 _$E519
// FUNCTION: STBC 0x00408030 _$E515
// FUNCTION: STBC 0x00408040 _$E518
// FUNCTION: STBC 0x00408050 _$E516

// The standard streams' initialisers and exit-time destructors (<iostream>).

// DATA: STBC 0x0097e328
TGEventHandlerTable Mission::classHandlers;

/*
================
Mission::Mission

The groups are Python instances the mission owns.
================
*/
// FUNCTION: STBC 0x00408070
Mission::Mission( char *script ) : ScriptObject( script, 0x80C001 ), precreatedShips( 2 ) {
	friendlyGroup = ( new ObjectGroup )->VirtualSlot08();
	PyObject_SetAttrString( friendlyGroup, "thisown", PyInt_FromLong( 1 ) );
	enemyGroup = ( new ObjectGroup )->VirtualSlot08();
	PyObject_SetAttrString( enemyGroup, "thisown", PyInt_FromLong( 1 ) );
	neutralGroup = ( new ObjectGroup )->VirtualSlot08();
	// BUG: marks the enemy group a second time, not the neutral group
	PyObject_SetAttrString( enemyGroup, "thisown", PyInt_FromLong( 1 ) );
	tractorGroup = ( new ObjectGroup )->VirtualSlot08();
	PyObject_SetAttrString( tractorGroup, "thisown", PyInt_FromLong( 1 ) );
	ReticleManagerWindow::GetReticleManagerWindow()->UpdateVisibility();
}

/*
================
Mission::GetObjType
================
*/
// FUNCTION: STBC 0x00408230
int Mission::GetObjType( void ) {
	return MISSION_TYPE;
}

/*
================
Mission::IsTypeOf
================
*/
// FUNCTION: STBC 0x00408240
bool Mission::IsTypeOf( int type ) {
	if ( type == MISSION_TYPE ) {
		return true;
	}
	return ScriptObject::IsTypeOf( type );
}

/*
================
Mission::GetClassName
================
*/
// FUNCTION: STBC 0x00408290
char *Mission::GetClassName( void ) {
	return "Mission";
}

/*
================
Mission::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004082a0
char *Mission::GetSwigTypeName( void ) {
	return "_p_Mission";
}

/*
================
Mission::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004082b0
char *Mission::GetPythonClassName( void ) {
	return "MissionPtr";
}

/*
================
Mission::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004082c0
TGEventHandlerTable *Mission::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
Mission::~Mission

Gives up the groups; the precreated ships list frees its blocks.
================
*/
Mission::~Mission( void ) {
	Py_DECREF( friendlyGroup );
	Py_DECREF( enemyGroup );
	Py_DECREF( neutralGroup );
	Py_DECREF( tractorGroup );
}

/*
================
Mission::AddPrecreatedShip

Lists the ship's name once for each copy asked for.
================
*/
// FUNCTION: STBC 0x00408410
bool Mission::AddPrecreatedShip( char *name, int count ) {
	if ( name == NULL ) {
		return false;
	}
	while ( --count >= 0 ) {
		precreatedShips.AddTail( new PrecreatedShip( name ) );
	}
	return true;
}

/*
================
Mission::GetPrecreatedShip

Takes the first listed entry of the name that has a ship out of the list
and returns the ship.
================
*/
// FUNCTION: STBC 0x004085b0
ShipClass *Mission::GetPrecreatedShip( char *name ) {
	if ( name != NULL ) {
		NiTListIterator pos = precreatedShips.GetHeadPos();
		while ( pos != NULL ) {
			NiTListIterator entryPos = pos;
			PrecreatedShip *entry = precreatedShips.GetNext( pos );
			if ( strcmp( name, entry->name ) == 0 ) {
				TGObject *object = TGObject::GetTGObjectPtr( entry->id );
				if ( object != NULL ) {
					precreatedShips.RemovePos( entryPos );
					if ( entry != NULL ) {
						delete entry->name;
						delete entry;
					}
					if ( object->IsTypeOf( SHIPCLASS_TYPE ) ) {
						return static_cast<ShipClass *>( object );
					}
				}
			}
		}
	}
	return NULL;
}

/*
================
Mission::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00408720
void Mission::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Mission::PlayerDied, "Mission::PlayerDied" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Mission::PlayerChanged, "Mission::PlayerChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Mission::PlayerEnteredSet, "Mission::PlayerEnteredSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Mission::PlayerExitedSet, "Mission::PlayerExitedSet" );
}

/*
================
Mission::RegisterHandlers

The class adds no handlers of its own.
================
*/
// FUNCTION: STBC 0x00408780
void Mission::RegisterHandlers( void ) {
}

/*
================
Mission::Initialize

Creates the precreated ships and runs the script's Initialize. In a
single-player game the bridge's view screen shows the player's camera
unless it already shows one. The handlers follow the player's ship, the
game hears that the mission has loaded, and the player's ship is
adjusted to the difficulty. A movie frame is played between the steps
that take long.
================
*/
// FUNCTION: STBC 0x00408790
void Mission::Initialize( TGEvent *event ) {
	CreatePrecreatedShips();
	g_kMovieManager.PlayForcedMovieFrame();
	ScriptObject::Initialize( event );
	g_kMovieManager.PlayForcedMovieFrame();

	Game *game = Game::GetCurrentGame();
	if ( !g_kUtopiaModule.IsMultiplayer() ) {
		BridgeSet *bridgeSet = BridgeSet::Cast( g_kSetManager.GetSet( "bridge" ) );
		if ( bridgeSet != NULL ) {
			ViewScreenObject *viewScreen = bridgeSet->GetViewScreen();
			if ( viewScreen != NULL && viewScreen->GetRemoteCam() == NULL ) {
				viewScreen->SetRemoteCam( game->GetPlayerCamera() );
				viewScreen->SetIsOn( true );
			}
		}
	}
	if ( game != NULL ) {
		AddPlayerDiedHandler();
		AddPlayerSetHandlers();
		g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "Mission::PlayerChanged", game );
		game->MissionLoaded();
	}
	g_kMovieManager.PlayForcedMovieFrame();
	if ( event != NULL ) {
		CallNextHandler( event );
	}
	g_kLODModelManager.Purge();

	ShipClass *player = Game::GetCurrentPlayer();
	g_kMovieManager.PlayForcedMovieFrame();
	if ( player != NULL && !g_kUtopiaModule.IsMultiplayer() ) {
		PyObject *ship = player->VirtualSlot08();
		PyObject *hardpointFileName = Py_BuildValue( "s", Game::GetPlayerHardpointFileName() );
		// BUG: the format asks for a C string, but the Python string object is passed
		PyEmbed::CallFunction( "loadspacehelper", "AdjustShipForDifficulty", NULL, NULL, "Os", ship, hardpointFileName );
		Py_DECREF( ship );
		Py_DECREF( hardpointFileName );
	}
	g_kMovieManager.PlayForcedMovieFrame();
}

/*
================
Mission::Terminate

Ends the mission: deletes every set but the bridge, the warp set and the
player's set (the bridge stays the rendered set, its view screen off),
then the mission's ships, lens flares, images and timers, what the
sensors identified, and resets the difficulty multipliers.
================
*/
// FUNCTION: STBC 0x00408930
void Mission::Terminate( TGEvent *event ) {
	ScriptObject::Terminate( event );

	BridgeSet *bridge = (BridgeSet *)g_kSetManager.GetSet( "bridge" );
	if ( bridge != NULL ) {
		ViewScreenObject *viewScreen = bridge->GetViewScreen();
		viewScreen->SetRemoteCam( NULL );
		viewScreen->SetIsOn( false );
	}
	g_kSetManager.RemoveSet( "bridge" );

	SetClass *warpSet = g_kSetManager.GetSet( "warp" );
	SetClass *playerSet = NULL;
	g_kSetManager.RemoveSet( "warp" );

	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		playerSet = player->GetContainingSet();
		if ( playerSet != NULL && playerSet != warpSet ) {
			g_kSetManager.RemoveSet( playerSet->GetName() );
		} else {
			playerSet = NULL;
		}
	}

	g_kSetManager.DeleteAllSets();
	g_kSetManager.ClearRenderedSet();

	if ( bridge != NULL ) {
		g_kSetManager.AddSet( bridge, "bridge" );
		g_kSetManager.MakeRenderedSet( bridge );
	}
	if ( warpSet != NULL ) {
		g_kSetManager.AddSet( warpSet, "warp" );
	}
	if ( playerSet != NULL ) {
		g_kSetManager.AddSet( playerSet, playerSet->GetName() );
	}

	DeletePrecreatedShips();
	LensFlare::DeleteAll();
	g_kImageManager.PurgeImages();
	PyEmbed::CallFunction( "MissionLib", "DeleteAllMissionTimers", NULL, NULL, NULL, NULL );
	SensorSubsystem::GetSensorData().ClearIdentified();
	Game::ResetDifficultyMultipliers();
}

/*
================
Mission::CreatePrecreatedShips

Has the loadspacehelper script create each listed ship that has none yet,
and records its ID.
================
*/
// FUNCTION: STBC 0x00408ae0
bool Mission::CreatePrecreatedShips( void ) {
	NiTListIterator pos = precreatedShips.GetHeadPos();
	while ( pos != NULL ) {
		PrecreatedShip *entry = precreatedShips.GetNext( pos );
		if ( entry->id == 0 ) {
			PyObject *ship = NULL;
			Py_INCREF( Py_None );
			int result = PyEmbed::CallFunction( "loadspacehelper", "CreateShip", "O", &ship, "sOssii", entry->name, Py_None, 0, 0, 0, 0 );
			Py_DECREF( Py_None );
			if ( result == -1 ) {
				PyErr_Print();
			} else {
				if ( ship == NULL ) {
					continue;
				}
				if ( ship != Py_None ) {
					ShipClass *object = NULL;
					PyEmbed::Unknown6F98E0( ship, (void **)&object, "ShipClass" );
					if ( object != NULL ) {
						entry->id = object->GetObjID();
					}
				}
			}
			Py_XDECREF( ship );
		}
	}
	return true;
}

/*
================
Mission::DeletePrecreatedShips

Takes every listed ship out of the list, and has the ones that were made
deleted.
================
*/
// FUNCTION: STBC 0x00408c00
void Mission::DeletePrecreatedShips( void ) {
	while ( !precreatedShips.IsEmpty() ) {
		TGEvent *event = NULL;
		PrecreatedShip *entry = precreatedShips.RemoveHead();
		TGObject *object = TGObject::GetTGObjectPtr( entry->id );
		if ( object != NULL && object->IsTypeOf( SHIPCLASS_TYPE ) ) {
			event = new TGEvent;
			event->SetEventType( ET_DELETE_OBJECT_PUBLIC );
			event->SetDestination( static_cast<ShipClass *>( object ) );
			g_kEventManager.AddEvent( event );
		}
		if ( entry != NULL ) {
			delete entry->name;
			delete entry;
		}
	}
}

/*
================
Mission::GetNextEventType

A new event type of the current episode's mission; 0 without one.
================
*/
// FUNCTION: STBC 0x00408cd0
int Mission::GetNextEventType( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		Episode *episode = game->GetCurrentEpisode();
		if ( episode != NULL ) {
			Mission *mission = episode->GetCurrentMission();
			if ( mission != NULL ) {
				return mission->GetInstanceNextEventType();
			}
		}
	}
	return 0;
}

/*
================
Mission::WriteToStream

The groups, then the precreated ships with the ones already made.
================
*/
// FUNCTION: STBC 0x00408d00
bool Mission::WriteToStream( TGStream *stream ) {
	if ( !ScriptObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( friendlyGroup != NULL );
	if ( friendlyGroup != NULL && !PyEmbed::SerializePython( friendlyGroup, stream, 0 ) ) {
		return false;
	}
	stream->WriteBool( enemyGroup != NULL );
	if ( enemyGroup != NULL && !PyEmbed::SerializePython( enemyGroup, stream, 0 ) ) {
		return false;
	}
	stream->WriteBool( neutralGroup != NULL );
	if ( neutralGroup != NULL && !PyEmbed::SerializePython( neutralGroup, stream, 0 ) ) {
		return false;
	}
	stream->WriteBool( tractorGroup != NULL );
	if ( tractorGroup != NULL && !PyEmbed::SerializePython( tractorGroup, stream, 0 ) ) {
		return false;
	}
	stream->WriteInt( precreatedShips.GetSize() );
	NiTListIterator pos = precreatedShips.GetHeadPos();
	while ( pos != NULL ) {
		PrecreatedShip *entry = precreatedShips.GetNext( pos );
		stream->VirtualSlot11( entry->name );
		stream->WriteID( entry->id );
		if ( entry->id != 0 ) {
			TGObject *ship = TGObject::GetTGObjectPtr( entry->id );
			if ( ship != NULL ) {
				stream->WriteBool( true );
				ship->WriteToStream( stream );
			} else {
				stream->WriteBool( false );
			}
		}
	}
	return true;
}

/*
================
Mission::ReadFromStream
================
*/
// FUNCTION: STBC 0x00408e40
bool Mission::ReadFromStream( TGStream *stream ) {
	if ( !ScriptObject::ReadFromStream( stream ) ) {
		return false;
	}
	friendlyGroup = NULL;
	if ( stream->ReadBool() == true ) {
		friendlyGroup = PyEmbed::UnserializePython( stream, 0 );
		if ( friendlyGroup == NULL ) {
			return false;
		}
	}
	enemyGroup = NULL;
	if ( stream->ReadBool() == true ) {
		enemyGroup = PyEmbed::UnserializePython( stream, 0 );
		if ( enemyGroup == NULL ) {
			return false;
		}
	}
	neutralGroup = NULL;
	if ( stream->ReadBool() == true ) {
		neutralGroup = PyEmbed::UnserializePython( stream, 0 );
		if ( neutralGroup == NULL ) {
			return false;
		}
	}
	tractorGroup = NULL;
	if ( stream->ReadBool() == true ) {
		tractorGroup = PyEmbed::UnserializePython( stream, 0 );
		if ( tractorGroup == NULL ) {
			return false;
		}
	}
	for ( int i = stream->ReadInt() - 1; i >= 0; i-- ) {
		PrecreatedShip *entry = new PrecreatedShip;
		entry->name = stream->VirtualSlot10( -1 );
		entry->id = stream->ReadID();
		precreatedShips.AddTail( entry );
		if ( stream->ReadBool() ) {
			TGObject *ship = TGObject::LoadObject( stream, 0 );
			if ( ship == NULL || !ship->IsTypeOf( SHIPCLASS_TYPE ) ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
Mission::FixUpPointers
================
*/
// FUNCTION: STBC 0x00409060
bool Mission::FixUpPointers( void ) {
	return ScriptObject::FixUpPointers() ? true : false;
}

/*
================
Mission::PostFixUp
================
*/
// FUNCTION: STBC 0x00409070
void Mission::PostFixUp( void ) {
	ScriptObject::PostFixUp();
}

/*
================
Mission::AddPlayerSetHandlers

Has the player's ship's set changes reach PlayerEnteredSet and
PlayerExitedSet (invented name).
================
*/
// FUNCTION: STBC 0x00409080
void Mission::AddPlayerSetHandlers( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		ShipClass *player = game->GetPlayer();
		if ( player != NULL ) {
			g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "Mission::PlayerEnteredSet", player );
			g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "Mission::PlayerExitedSet", player );
		}
	}
}

/*
================
Mission::PlayerEnteredSet

The sound region named like the set the event's destination entered is filtered
to 2 while the set being drawn is the bridge and to 0 otherwise.
================
*/
// FUNCTION: STBC 0x004090d0
void Mission::PlayerEnteredSet( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	if ( object != NULL ) {
		char *setName = object->GetContainingSet()->GetName();
		TGSoundRegion *region = TGSoundRegion::GetRegion( setName );
		if ( region != NULL ) {
			SetClass *rendered = g_kSetManager.GetRenderedSet();
			if ( rendered != NULL && strcmp( rendered->GetName(), "bridge" ) == 0 ) {
				region->SetFilter( 2 );
			} else {
				region->SetFilter( 0 );
			}
		}
	}
	CallNextHandler( event );
}

/*
================
Mission::PlayerExitedSet

The sound region of the set named by the event is filtered to 1 unless it
is the set being drawn.
================
*/
// FUNCTION: STBC 0x00409170
void Mission::PlayerExitedSet( TGEvent *event ) {
	char *setName = ( (TGStringEvent *)event )->GetCString();
	SetClass *rendered = g_kSetManager.GetRenderedSet();
	if ( rendered == NULL || strcmp( rendered->GetName(), setName ) != 0 ) {
		TGSoundRegion *region = TGSoundRegion::GetRegion( setName );
		if ( region != NULL ) {
			region->SetFilter( 1 );
		}
	}
	delete [] setName;
	CallNextHandler( event );
}

/*
================
Mission::AddPlayerDiedHandler

Has the player's ship's explosion reach PlayerDied; a single-player
game only (invented name).
================
*/
// FUNCTION: STBC 0x004091f0
void Mission::AddPlayerDiedHandler( void ) {
	if ( !g_kUtopiaModule.IsMultiplayer() && Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() != NULL ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_EXPLODING, this, "Mission::PlayerDied", Game::GetCurrentGame()->GetPlayer() );
	}
}

/*
================
Mission::PlayerDied

When it is the player's ship that exploded, the camera drops to watch
it and the bridge sounds are filtered.
================
*/
// FUNCTION: STBC 0x00409230
void Mission::PlayerDied( TGEvent *event ) {
	if ( Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() == event->GetDestination() ) {
		WatchPlayerDie();
		FilterBridgeSounds();
	}
	CallNextHandler( event );
}

/*
================
Mission::PlayerChanged

The handlers follow the new player's ship.
================
*/
// FUNCTION: STBC 0x00409270
void Mission::PlayerChanged( TGEvent *event ) {
	AddPlayerDiedHandler();
	AddPlayerSetHandlers();
	CallNextHandler( event );
}

/*
================
Mission::WatchPlayerDie

Takes the cinematic window's view of the player's ship, with the camera
script's DropAndWatch, and hides the subtitle window (invented name).
================
*/
// FUNCTION: STBC 0x00409290
void Mission::WatchPlayerDie( void ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	MainWindow *focus = (MainWindow *)topWindow->GetFocus();
	CinematicWindow *cinematicWindow = (CinematicWindow *)topWindow->FindMainWindow( MWT_CINEMATIC );
	cinematicWindow->SetInteractive( false );
	if ( focus != cinematicWindow ) {
		topWindow->ToggleCinematicWindow();
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		PyEmbed::CallFunction( "Camera", "DropAndWatch", "", NULL, "(s)", player->GetName() );
		PyErr_Print();
	}
	MainWindow *subtitleWindow = topWindow->FindMainWindow( MWT_SUBTITLE );
	if ( subtitleWindow != NULL ) {
		subtitleWindow->SetNotVisible( true );
	}
}

/*
================
Mission::FilterBridgeSounds

Filters the bridge's sound region (invented name).
================
*/
// FUNCTION: STBC 0x00409320
void Mission::FilterBridgeSounds( void ) {
	TGSoundRegion *region = TGSoundRegion::GetRegion( "bridge" );
	if ( region != NULL ) {
		region->SetFilter( 1 );
	}
}

/*
================
Mission.GetFriendlyGroup

The Python method: the mission's ObjectGroup of the player's friends, with a new
reference; NULL when the argument is not a Mission.
================
*/
// FUNCTION: STBC 0x00409340
PyObject *Mission_GetFriendlyGroup( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;

	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		Mission *mission = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&mission, "Mission" );
		if ( mission != NULL ) {
			result = mission->GetFriendlyGroup();
			Py_XINCREF( result );
		}
	}
	return result;
}

/*
================
Mission.GetEnemyGroup

The Python method: the mission's ObjectGroup of the player's enemies, with a new
reference; NULL when the argument is not a Mission.
================
*/
// FUNCTION: STBC 0x00409390
PyObject *Mission_GetEnemyGroup( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;

	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		Mission *mission = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&mission, "Mission" );
		if ( mission != NULL ) {
			result = mission->GetEnemyGroup();
			Py_XINCREF( result );
		}
	}
	return result;
}

/*
================
Mission.GetNeutralGroup

The Python method: the mission's ObjectGroup of the neutral objects, with a new
reference; NULL when the argument is not a Mission.
================
*/
// FUNCTION: STBC 0x004093e0
PyObject *Mission_GetNeutralGroup( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;

	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		Mission *mission = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&mission, "Mission" );
		if ( mission != NULL ) {
			result = mission->GetNeutralGroup();
			Py_XINCREF( result );
		}
	}
	return result;
}

/*
================
Mission.GetTractorGroup

The Python method: the mission's ObjectGroup of the ships the tractor beams hold, with a new
reference; NULL when the argument is not a Mission.
================
*/
// FUNCTION: STBC 0x00409430
PyObject *Mission_GetTractorGroup( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;

	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		Mission *mission = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&mission, "Mission" );
		if ( mission != NULL ) {
			result = mission->GetTractorGroup();
			Py_XINCREF( result );
		}
	}
	return result;
}
