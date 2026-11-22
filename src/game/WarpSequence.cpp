/*
===============================================================================

	WarpSequence.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "WarpSequence.h"
#include "PyEmbed.h"
#include "SetManager.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGScriptAction.h"
#include "TGStream.h"
#include "WarpEngineSubsystem.h"
#include "WarpSet.h"
#include "WarpEvent.h"
#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "MultiplayerGame.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004d75a0 _$E511
// FUNCTION: STBC 0x004d75c0 _$E510

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d5990 _$E32
// FUNCTION: STBC 0x004d59a0 _$E29
// FUNCTION: STBC 0x004d59b0 _$E31
// FUNCTION: STBC 0x004d59c0 _$E30
// FUNCTION: STBC 0x004d59d0 _$E38
// FUNCTION: STBC 0x004d59e0 _$E35
// FUNCTION: STBC 0x004d59f0 _$E37
// FUNCTION: STBC 0x004d5a00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d5a10 _$E66
// FUNCTION: STBC 0x004d5a20 _$E65
// FUNCTION: STBC 0x004d5a30 _$E307
// FUNCTION: STBC 0x004d5a40 _$E306
// FUNCTION: STBC 0x004d5a50 _$E77
// FUNCTION: STBC 0x004d5a60 _$E76
// FUNCTION: STBC 0x004d5a70 _$E81
// FUNCTION: STBC 0x004d5a80 _$E80
// FUNCTION: STBC 0x004d5a90 _$E85
// FUNCTION: STBC 0x004d5aa0 _$E84
// FUNCTION: STBC 0x004d5ab0 _$E89
// FUNCTION: STBC 0x004d5ac0 _$E88
// FUNCTION: STBC 0x004d6020 _$E206
// FUNCTION: STBC 0x004d6030 _$E205
// FUNCTION: STBC 0x004d6d90 _$E148
// FUNCTION: STBC 0x004d6da0 _$E147
// FUNCTION: STBC 0x004d6db0 _$E161
// FUNCTION: STBC 0x004d6dc0 _$E160
// FUNCTION: STBC 0x004d6dd0 _$E211
// FUNCTION: STBC 0x004d6de0 _$E210
// FUNCTION: STBC 0x004d6df0 _$E312
// FUNCTION: STBC 0x004d6e00 _$E311
// FUNCTION: STBC 0x004d6e10 _$E215
// FUNCTION: STBC 0x004d6e20 _$E214
// FUNCTION: STBC 0x004d6e30 _$E316
// FUNCTION: STBC 0x004d6e40 _$E315
// FUNCTION: STBC 0x004d6e50 _$E323
// FUNCTION: STBC 0x004d6e60 _$E322
// FUNCTION: STBC 0x004d6e70 _$E352
// FUNCTION: STBC 0x004d6e80 _$E351
// FUNCTION: STBC 0x004d6e90 _$E356
// FUNCTION: STBC 0x004d6ea0 _$E355
// FUNCTION: STBC 0x004d6eb0 _$E106
// FUNCTION: STBC 0x004d6ec0 _$E105
// FUNCTION: STBC 0x004d6ed0 _$E234
// FUNCTION: STBC 0x004d6ee0 _$E233
// FUNCTION: STBC 0x004d6ef0 _$E241
// FUNCTION: STBC 0x004d6f00 _$E240
// FUNCTION: STBC 0x004d6f10 _$E245
// FUNCTION: STBC 0x004d6f20 _$E244
// FUNCTION: STBC 0x004d6f30 _$E249
// FUNCTION: STBC 0x004d6f40 _$E248
// FUNCTION: STBC 0x004d6f50 _$E253
// FUNCTION: STBC 0x004d6f60 _$E252
// FUNCTION: STBC 0x004d6f70 _$E259
// FUNCTION: STBC 0x004d6f80 _$E258
// FUNCTION: STBC 0x004d6f90 _$E360
// FUNCTION: STBC 0x004d6fa0 _$E359
// FUNCTION: STBC 0x004d6fb0 _$E364
// FUNCTION: STBC 0x004d6fc0 _$E363
// FUNCTION: STBC 0x004d6fd0 _$E368
// FUNCTION: STBC 0x004d6fe0 _$E367
// FUNCTION: STBC 0x004d6ff0 _$E372
// FUNCTION: STBC 0x004d7000 _$E371
// FUNCTION: STBC 0x004d7010 _$E376
// FUNCTION: STBC 0x004d7020 _$E375
// FUNCTION: STBC 0x004d7030 _$E380
// FUNCTION: STBC 0x004d7040 _$E379
// FUNCTION: STBC 0x004d7050 _$E384
// FUNCTION: STBC 0x004d7060 _$E383
// FUNCTION: STBC 0x004d7070 _$E388
// FUNCTION: STBC 0x004d7080 _$E387
// FUNCTION: STBC 0x004d7090 _$E392
// FUNCTION: STBC 0x004d70a0 _$E391
// FUNCTION: STBC 0x004d70b0 _$E396
// FUNCTION: STBC 0x004d70c0 _$E395
// FUNCTION: STBC 0x004d70d0 _$E111
// FUNCTION: STBC 0x004d70e0 _$E110
// FUNCTION: STBC 0x004d70f0 _$E400
// FUNCTION: STBC 0x004d7100 _$E399
// FUNCTION: STBC 0x004d7110 _$E116
// FUNCTION: STBC 0x004d7120 _$E115
// FUNCTION: STBC 0x004d7130 _$E404
// FUNCTION: STBC 0x004d7140 _$E403
// FUNCTION: STBC 0x004d7150 _$E408
// FUNCTION: STBC 0x004d7160 _$E407
// FUNCTION: STBC 0x004d7170 _$E412
// FUNCTION: STBC 0x004d7180 _$E411
// FUNCTION: STBC 0x004d7190 _$E416
// FUNCTION: STBC 0x004d71a0 _$E415
// FUNCTION: STBC 0x004d71b0 _$E125
// FUNCTION: STBC 0x004d71c0 _$E124
// FUNCTION: STBC 0x004d71d0 _$E420
// FUNCTION: STBC 0x004d71e0 _$E419
// FUNCTION: STBC 0x004d71f0 _$E424
// FUNCTION: STBC 0x004d7200 _$E423
// FUNCTION: STBC 0x004d7210 _$E428
// FUNCTION: STBC 0x004d7220 _$E427
// FUNCTION: STBC 0x004d7230 _$E131
// FUNCTION: STBC 0x004d7240 _$E130
// FUNCTION: STBC 0x004d7250 _$E136
// FUNCTION: STBC 0x004d7260 _$E135
// FUNCTION: STBC 0x004d7270 _$E166
// FUNCTION: STBC 0x004d7280 _$E165
// FUNCTION: STBC 0x004d72c0 _$E432
// FUNCTION: STBC 0x004d72d0 _$E431
// FUNCTION: STBC 0x004d72e0 _$E171
// FUNCTION: STBC 0x004d72f0 _$E170
// FUNCTION: STBC 0x004d7300 _$E436
// FUNCTION: STBC 0x004d7310 _$E435
// FUNCTION: STBC 0x004d7320 _$E440
// FUNCTION: STBC 0x004d7330 _$E439
// FUNCTION: STBC 0x004d7340 _$E176
// FUNCTION: STBC 0x004d7350 _$E175
// FUNCTION: STBC 0x004d7360 _$E444
// FUNCTION: STBC 0x004d7370 _$E443
// FUNCTION: STBC 0x004d7380 _$E141
// FUNCTION: STBC 0x004d7390 _$E140
// FUNCTION: STBC 0x004d73a0 _$E448
// FUNCTION: STBC 0x004d73b0 _$E447
// FUNCTION: STBC 0x004d73c0 _$E452
// FUNCTION: STBC 0x004d73d0 _$E451
// FUNCTION: STBC 0x004d73e0 _$E181
// FUNCTION: STBC 0x004d73f0 _$E180
// FUNCTION: STBC 0x004d7400 _$E186
// FUNCTION: STBC 0x004d7410 _$E185
// FUNCTION: STBC 0x004d7420 _$E456
// FUNCTION: STBC 0x004d7430 _$E455
// FUNCTION: STBC 0x004d7440 _$E460
// FUNCTION: STBC 0x004d7450 _$E459
// FUNCTION: STBC 0x004d7460 _$E464
// FUNCTION: STBC 0x004d7470 _$E463
// FUNCTION: STBC 0x004d7480 _$E468
// FUNCTION: STBC 0x004d7490 _$E467
// FUNCTION: STBC 0x004d74a0 _$E472
// FUNCTION: STBC 0x004d74b0 _$E471
// FUNCTION: STBC 0x004d74c0 _$E476
// FUNCTION: STBC 0x004d74d0 _$E475
// FUNCTION: STBC 0x004d74e0 _$E480
// FUNCTION: STBC 0x004d74f0 _$E479
// FUNCTION: STBC 0x004d7500 _$E484
// FUNCTION: STBC 0x004d7510 _$E483
// FUNCTION: STBC 0x004d7520 _$E191
// FUNCTION: STBC 0x004d7530 _$E190
// FUNCTION: STBC 0x004d7540 _$E488
// FUNCTION: STBC 0x004d7550 _$E487
// FUNCTION: STBC 0x004d7560 _$E196
// FUNCTION: STBC 0x004d7570 _$E195
// FUNCTION: STBC 0x004d7580 _$E201
// FUNCTION: STBC 0x004d7590 _$E200
// FUNCTION: STBC 0x004d75d0 _$E492
// FUNCTION: STBC 0x004d75e0 _$E491
// FUNCTION: STBC 0x004d75f0 _$E497
// FUNCTION: STBC 0x004d7600 _$E496
// FUNCTION: STBC 0x004d7610 _$E501
// FUNCTION: STBC 0x004d7620 _$E500

/*
================
WarpSequence::Cast

The object as a WarpSequence; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004d5ad0
WarpSequence *WarpSequence::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WARPSEQUENCE_TYPE ) ) {
		return (WarpSequence *)object;
	}
	return NULL;
}

/*
================
WarpSequence::WarpSequence

Keeps the ship, where it starts and where it goes, and makes the four
sub-sequences and the script actions of the warp's stages.
================
*/
// FUNCTION: STBC 0x004d5b00 ??0WarpSequence@@QAE@PAVShipClass@@PADM1@Z
WarpSequence::WarpSequence( ShipClass *ship, char *destination_, float timeToWarp_, char *placementName_ ) {
	shipID = ship->GetObjID();
	origin = ship->GetContainingSet();
	if ( destination_ != NULL ) {
		destination = new char[strlen( destination_ ) + 1];
		strcpy( destination, destination_ );
	} else {
		destination = NULL;
	}
	destinationEpisode = NULL;
	destinationMission = NULL;
	timeToWarp = timeToWarp_;
	placementName = NULL;
	if ( placementName_ != NULL ) {
		SetPlacement( placementName_ );
	} else {
		SetPlacement( "Player Start" );
	}

	TGSequence *beforeWarp = new TGSequence;
	beforeWarpID = beforeWarp->GetObjID();
	AddAction( beforeWarp );

	TGScriptAction *action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	warpBeginActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_WARP_INITIATED ) );

	action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	warpEndActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_WARP_ENDING ) );

	action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	warpEnterActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_WARPING ) );

	TGSequence *duringWarp = new TGSequence;
	TGSequence *postDuringWarp = new TGSequence;
	duringWarpID = duringWarp->GetObjID();
	postDuringWarpID = postDuringWarp->GetObjID();

	// The destination set's name: the module name after its last dot.
	char *destinationName = NULL;
	if ( destination != NULL ) {
		destinationName = strrchr( destination, '.' );
		if ( destinationName != NULL ) {
			destinationName++;
		} else {
			destinationName = destination;
		}
	}

	action = new TGScriptAction( "Actions.ShipScriptActions", "MoveBetweenSetsAction" );
	moveAction1ID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "s", GetWarpSet()->GetName() ) );

	action = new TGScriptAction( "Actions.ShipScriptActions", "MoveBetweenSetsAction" );
	moveAction2ID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "s", destinationName ) );

	action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	dewarpBeginActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_DEWARP_INITIATED ) );

	action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	dewarpEndActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_DEWARP_ENDING ) );

	action = new TGScriptAction( "Actions.EffectScriptActions", "SetWarpStage" );
	dewarpFinishActionID = action->GetObjID();
	action->AddArgument( Py_BuildValue( "l", ship->GetObjID() ) );
	action->AddArgument( Py_BuildValue( "i", WarpEngineSubsystem::WES_NOT_WARPING ) );

	TGSequence *afterWarp = new TGSequence;
	afterWarpID = afterWarp->GetObjID();

	eventDestinationID = 0;
}

/*
================
WarpSequence::~WarpSequence
================
*/
// FUNCTION: STBC 0x004d6040
WarpSequence::~WarpSequence( void ) {
	if ( destination != NULL ) {
		delete destination;
	}
	if ( placementName != NULL ) {
		delete placementName;
	}
	if ( destinationEpisode != NULL ) {
		delete destinationEpisode;
	}
	if ( destinationMission != NULL ) {
		delete destinationMission;
	}
}

/*
================
WarpSequence::SetupSequence
================
*/
// FUNCTION: STBC 0x004d60a0
void WarpSequence::SetupSequence( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "WarpSequence", "SetupSequence", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
WarpSequence::GetWarpSequencePiece

The sequence or action of the warp the piece names; NULL for an unknown piece.
================
*/
// FUNCTION: STBC 0x004d60e0 ?GetWarpSequencePiece@WarpSequence@@QAEPAVTGAction@@H@Z
TGAction *WarpSequence::GetWarpSequencePiece( int piece ) {
	switch ( piece ) {
	case PRE_WARP_SEQUENCE:
		return TGSequence::Cast( GetTGObjectPtr( beforeWarpID ) );
	case DURING_WARP_SEQUENCE:
		return TGSequence::Cast( GetTGObjectPtr( duringWarpID ) );
	case POST_DURING_WARP_SEQUENCE:
		return TGSequence::Cast( GetTGObjectPtr( postDuringWarpID ) );
	case POST_WARP_SEQUENCE:
		return TGSequence::Cast( GetTGObjectPtr( afterWarpID ) );
	case WARP_BEGIN_ACTION:
		return TGAction::Cast( GetTGObjectPtr( warpBeginActionID ) );
	case WARP_END_ACTION:
		return TGAction::Cast( GetTGObjectPtr( warpEndActionID ) );
	case DEWARP_BEGIN_ACTION:
		return TGAction::Cast( GetTGObjectPtr( dewarpBeginActionID ) );
	case DEWARP_END_ACTION:
		return TGAction::Cast( GetTGObjectPtr( dewarpEndActionID ) );
	case WARP_ENTER_ACTION:
		return TGAction::Cast( GetTGObjectPtr( warpEnterActionID ) );
	case DEWARP_FINISH_ACTION:
		return TGAction::Cast( GetTGObjectPtr( dewarpFinishActionID ) );
	case MOVE_ACTION_1:
		return TGAction::Cast( GetTGObjectPtr( moveAction1ID ) );
	case MOVE_ACTION_2:
		return TGAction::Cast( GetTGObjectPtr( moveAction2ID ) );
	}
	return NULL;
}

/*
================
WarpSequence::Completed

Tells the event destination, if it still exists, that the ship has left
warp.
================
*/
// FUNCTION: STBC 0x004d6230
void WarpSequence::Completed( void ) {
	if ( GetTGObjectPtr( eventDestinationID ) != NULL ) {
		TGEvent *event = new TGEvent;
		event->SetSource( GetShip( this ) );
		event->SetDestination( (TGEventHandlerObject *)GetTGObjectPtr( eventDestinationID ) );
		event->SetEventType( ET_EXITED_WARP );
		AddCompletedEvent( event );
	}
	TGSequence::Completed();
}

/*
================
WarpSequence::Skip

Skipped before its first part is done, the warp still moves the ship: out
of its set, or deleted when it has none.
================
*/
// FUNCTION: STBC 0x004d62c0
void WarpSequence::Skip( void ) {
	if ( actions[0].action == NULL ) {
		ShipClass *ship = GetShip( this );
		if ( ship != NULL ) {
			if ( ship->GetContainingSet() != NULL ) {
				ship->GetContainingSet()->DeleteObjectFromSet( shipID );
				TGSequence::Skip();
				return;
			}
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_DELETE_OBJECT_PUBLIC );
			event->SetDestination( ship );
			g_kEventManager.AddEvent( event );
		}
	}
	TGSequence::Skip();
}

/*
================
WarpSequence::Create

A new warp sequence for the ship.
================
*/
// FUNCTION: STBC 0x004d6350 ?Create@WarpSequence@@SAPAV1@PAVShipClass@@PADM1@Z
WarpSequence *WarpSequence::Create( ShipClass *ship, char *destination_, float timeToWarp_, char *placementName_ ) {
	return new WarpSequence( ship, destination_, timeToWarp_, placementName_ );
}

/*
================
WarpSequence::GetWarpSet

The set ships warp through, made the first time.
================
*/
// FUNCTION: STBC 0x004d6390
SetClass *WarpSequence::GetWarpSet( void ) {
	SetClass *set = g_kSetManager.GetSet( "warp" );
	if ( set == NULL ) {
		set = new WarpSet;
		g_kSetManager.AddSet( set, "warp" );
	}
	return set;
}

/*
================
WarpSequence::AddActionBeforeWarp

Appends the action to the part played before the warp.
================
*/
// FUNCTION: STBC 0x004d6430
void WarpSequence::AddActionBeforeWarp( TGAction *action, float delay ) {
	TGSequence *sequence = TGSequence::Cast( GetTGObjectPtr( beforeWarpID ) );
	if ( sequence != NULL ) {
		sequence->AppendAction( action, delay );
	}
}

/*
================
WarpSequence::AddActionDuringWarp

Appends the action to the part played during the warp.
================
*/
// FUNCTION: STBC 0x004d6460
void WarpSequence::AddActionDuringWarp( TGAction *action, float delay ) {
	TGSequence *sequence = TGSequence::Cast( GetTGObjectPtr( duringWarpID ) );
	if ( sequence != NULL ) {
		sequence->AppendAction( action, delay );
	}
}

/*
================
WarpSequence::AddActionPostDuringWarp

Appends the action to the part played after the warp effect, still in it.
================
*/
// FUNCTION: STBC 0x004d6490
void WarpSequence::AddActionPostDuringWarp( TGAction *action, float delay ) {
	TGSequence *sequence = TGSequence::Cast( GetTGObjectPtr( postDuringWarpID ) );
	if ( sequence != NULL ) {
		sequence->AppendAction( action, delay );
	}
}

/*
================
WarpSequence::AddActionAfterWarp

Appends the action to the part played after the warp.
================
*/
// FUNCTION: STBC 0x004d64c0
void WarpSequence::AddActionAfterWarp( TGAction *action, float delay ) {
	TGSequence *sequence = TGSequence::Cast( GetTGObjectPtr( afterWarpID ) );
	if ( sequence != NULL ) {
		sequence->AppendAction( action, delay );
	}
}

/*
================
WarpSequence::SetDestinationMission
================
*/
// FUNCTION: STBC 0x004d64f0
void WarpSequence::SetDestinationMission( char *mission ) {
	delete destinationMission;
	destinationMission = NULL;
	if ( mission != NULL ) {
		destinationMission = new char[strlen( mission ) + 1];
		strcpy( destinationMission, mission );
	}
}

/*
================
WarpSequence::SetDestinationEpisode
================
*/
// FUNCTION: STBC 0x004d6550
void WarpSequence::SetDestinationEpisode( char *episode ) {
	delete destinationEpisode;
	destinationEpisode = NULL;
	if ( episode != NULL ) {
		destinationEpisode = new char[strlen( episode ) + 1];
		strcpy( destinationEpisode, episode );
	}
}

/*
================
WarpSequence::SetEventDestination
================
*/
// FUNCTION: STBC 0x004d65b0
void WarpSequence::SetEventDestination( TGObject *destination ) {
	if ( destination != NULL ) {
		eventDestinationID = destination->GetObjID();
	} else {
		eventDestinationID = 0;
	}
}

/*
================
WarpSequence::SetPlacement
================
*/
// FUNCTION: STBC 0x004d65e0
void WarpSequence::SetPlacement( char *name ) {
	if ( placementName != NULL ) {
		delete placementName;
		placementName = NULL;
	}
	if ( name != NULL ) {
		placementName = new char[strlen( name ) + 1];
		strcpy( placementName, name );
	}
}

/*
================
WarpSequence::GetDestinationSet

The destination module's set (its GetSet); NULL without a destination.
================
*/
// FUNCTION: STBC 0x004d6650
SetClass *WarpSequence::GetDestinationSet( void ) {
	if ( destination == NULL ) {
		return NULL;
	}
	PyObject *setObject;
	PyEmbed::CallFunction( destination, "GetSet", "O", &setObject, "" );
	SetClass *set = (SetClass *)PyEmbed::GetObjectPointer( setObject );
	Py_DECREF( setObject );
	return set;
}

/*
================
WarpSequence::Play

Skipped when the ship is gone, its warp engines are disabled, or it is
dead or dying. Otherwise it tells the warp engines their sequence, and
again when it completes, loads the warp flash textures, plays the pieces
SetupSequence put together and announces the warp. In multiplayer only
the player's own ship announces it, and on the host also the ships no
player flies; the announcement then carries the exit point instead of the
placement.
================
*/
// FUNCTION: STBC 0x004d66b0
// PARKED: register allocation: retail keeps the warp engines in ebp and the event type / zero constants in edi, this build the reverse; calls, frame and the exit point's copy agree
void WarpSequence::Play( void ) {
	ShipClass *ship = ShipClass::GetObjectByID( NULL, shipID );
	if ( ship == NULL ) {
		Skip();
		return;
	}
	WarpEngineSubsystem *warpEngines = ship->GetWarpEngineSubsystem();
	if ( warpEngines != NULL && warpEngines->IsDisabled() == true ) {
		Skip();
		return;
	}
	if ( ship->IsDead() || ship->GetLifeTime() < INFINITE_LIFE ) {
		Skip();
		return;
	}

	TGEvent *event = new TGEvent;
	event->SetEventType( ET_SET_WARP_SEQUENCE );
	event->SetSource( this );
	event->SetDestination( warpEngines );
	g_kEventManager.AddEvent( event );

	event = new TGEvent;
	event->SetEventType( ET_SET_WARP_SEQUENCE );
	event->SetSource( NULL );
	event->SetDestination( warpEngines );
	AddCompletedEvent( event );

	NiTArray<NiPointer<NiTextureProperty> > *flashTextures = g_kIconManager.GetIconGroup( "WarpFlashTextures" )->GetTextures();
	for ( unsigned int i = 0; i < flashTextures->GetSize(); i++ ) {
		flashTextures->GetAt( i );
	}

	GetWarpSet();
	SetupSequence();
	TGSequence::Play();

	if ( g_kUtopiaModule.IsMultiplayer() ) {
		Game *game = Game::GetCurrentGame();
		MultiplayerGame::Cast( game );
		if ( g_kUtopiaModule.IsHost() ? ( game->GetPlayer() == ship || !ship->IsPlayerShip() ) : game->GetPlayer() == ship ) {
			WarpEvent *warpEvent = new WarpEvent;
			warpEvent->SetEventType( ET_START_WARP_NOTIFY );
			warpEvent->SetDestination( ship );
			warpEvent->SetDestinationSetName( destination );
			warpEvent->SetPlacementName( NULL );
			warpEvent->SetExitPoint( exitPoint );
			warpEvent->SetDuration( timeToWarp );
			g_kEventManager.AddEvent( warpEvent );
		}
	} else {
		WarpEvent *warpEvent = new WarpEvent;
		warpEvent->SetEventType( ET_START_WARP_NOTIFY );
		warpEvent->SetDestination( GetShip( this ) );
		warpEvent->SetDestinationSetName( destination );
		warpEvent->SetPlacementName( placementName );
		warpEvent->SetDuration( timeToWarp );
		g_kEventManager.AddEvent( warpEvent );
	}
	origin = NULL;
}

/*
================
WarpSequence::WriteToStream
================
*/
// FUNCTION: STBC 0x004d6a70
bool WarpSequence::WriteToStream( TGStream *stream ) {
	if ( !TGSequence::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	stream->WriteID( GetSavedID( origin ) );
	stream->VirtualSlot11( destination );
	stream->VirtualSlot11( placementName );
	stream->WriteFloat( timeToWarp );
	stream->WriteID( beforeWarpID );
	stream->WriteID( duringWarpID );
	stream->WriteID( postDuringWarpID );
	stream->WriteID( afterWarpID );
	stream->WriteID( warpBeginActionID );
	stream->WriteID( warpEndActionID );
	stream->WriteID( dewarpBeginActionID );
	stream->WriteID( dewarpEndActionID );
	stream->WriteID( warpEnterActionID );
	stream->WriteID( dewarpFinishActionID );
	stream->WriteID( moveAction1ID );
	stream->WriteID( moveAction2ID );
	stream->WriteID( eventDestinationID );
	stream->VirtualSlot11( destinationMission );
	stream->VirtualSlot11( destinationEpisode );
	stream->WriteFloat( exitPoint.x );
	stream->WriteFloat( exitPoint.y );
	stream->WriteFloat( exitPoint.z );
	return true;
}

/*
================
WarpSequence::ReadFromStream
================
*/
// FUNCTION: STBC 0x004d6bd0
bool WarpSequence::ReadFromStream( TGStream *stream ) {
	if ( !TGSequence::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	origin = (SetClass *)stream->ReadID();
	destination = stream->VirtualSlot10( -1 );
	placementName = stream->VirtualSlot10( -1 );
	timeToWarp = stream->ReadFloat();
	beforeWarpID = stream->ReadID();
	duringWarpID = stream->ReadID();
	postDuringWarpID = stream->ReadID();
	afterWarpID = stream->ReadID();
	warpBeginActionID = stream->ReadID();
	warpEndActionID = stream->ReadID();
	dewarpBeginActionID = stream->ReadID();
	dewarpEndActionID = stream->ReadID();
	warpEnterActionID = stream->ReadID();
	dewarpFinishActionID = stream->ReadID();
	moveAction1ID = stream->ReadID();
	moveAction2ID = stream->ReadID();
	eventDestinationID = stream->ReadID();
	destinationMission = stream->VirtualSlot10( -1 );
	destinationEpisode = stream->VirtualSlot10( -1 );
	exitPoint.x = stream->ReadFloat();
	exitPoint.y = stream->ReadFloat();
	exitPoint.z = stream->ReadFloat();
	return true;
}

/*
================
WarpSequence::FixUpPointers

The set the ship started in may be gone; that is fine once the ship is
gone too.
================
*/
// FUNCTION: STBC 0x004d6d20
bool WarpSequence::FixUpPointers( void ) {
	if ( !TGSequence::FixUpPointers() ) {
		return false;
	}
	if ( origin != NULL && GetTGObjectPtr( (long)origin ) == NULL ) {
		if ( GetTGObjectPtr( shipID ) != NULL ) {
			return false;
		}
		origin = NULL;
	}
	return true;
}

/*
================
WarpSequence::PostFixUp
================
*/
// FUNCTION: STBC 0x004d6d70
void WarpSequence::PostFixUp( void ) {
	TGSequence::PostFixUp();
	origin = (SetClass *)GetTGObjectPtr( (long)origin );
}
