/*
===============================================================================

	WarpEngineSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include "WarpEngineSubsystem.h"
#include "AIManager.h"
#include "ArtificialIntelligence.h"
#include "CloakingSubsystem.h"
#include "PlacementObject.h"
#include "ShipClass.h"
#include "TGAction.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0056f7c0 _$E489
// FUNCTION: STBC 0x0056f7e0 _$E488

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0056ddb0 _$E86
// FUNCTION: STBC 0x0056ddc0 _$E85
// FUNCTION: STBC 0x0056ddd0 _$E299
// FUNCTION: STBC 0x0056dde0 _$E298
// FUNCTION: STBC 0x0056ef40 _$E97
// FUNCTION: STBC 0x0056ef50 _$E96
// FUNCTION: STBC 0x0056ef60 _$E101
// FUNCTION: STBC 0x0056ef70 _$E100
// FUNCTION: STBC 0x0056ef80 _$E105
// FUNCTION: STBC 0x0056ef90 _$E104
// FUNCTION: STBC 0x0056efa0 _$E109
// FUNCTION: STBC 0x0056efb0 _$E108
// FUNCTION: STBC 0x0056efc0 _$E222
// FUNCTION: STBC 0x0056efd0 _$E221
// FUNCTION: STBC 0x0056efe0 _$E164
// FUNCTION: STBC 0x0056eff0 _$E163
// FUNCTION: STBC 0x0056f000 _$E177
// FUNCTION: STBC 0x0056f010 _$E176
// FUNCTION: STBC 0x0056f020 _$E227
// FUNCTION: STBC 0x0056f030 _$E226
// FUNCTION: STBC 0x0056f040 _$E304
// FUNCTION: STBC 0x0056f050 _$E303
// FUNCTION: STBC 0x0056f060 _$E242
// FUNCTION: STBC 0x0056f070 _$E241
// FUNCTION: STBC 0x0056f080 _$E308
// FUNCTION: STBC 0x0056f090 _$E307
// FUNCTION: STBC 0x0056f0a0 _$E315
// FUNCTION: STBC 0x0056f0b0 _$E314
// FUNCTION: STBC 0x0056f0c0 _$E333
// FUNCTION: STBC 0x0056f0d0 _$E332
// FUNCTION: STBC 0x0056f0e0 _$E337
// FUNCTION: STBC 0x0056f0f0 _$E336
// FUNCTION: STBC 0x0056f100 _$E126
// FUNCTION: STBC 0x0056f110 _$E125
// FUNCTION: STBC 0x0056f120 _$E261
// FUNCTION: STBC 0x0056f130 _$E260
// FUNCTION: STBC 0x0056f140 _$E268
// FUNCTION: STBC 0x0056f150 _$E267
// FUNCTION: STBC 0x0056f160 _$E272
// FUNCTION: STBC 0x0056f170 _$E271
// FUNCTION: STBC 0x0056f180 _$E276
// FUNCTION: STBC 0x0056f190 _$E275
// FUNCTION: STBC 0x0056f1a0 _$E280
// FUNCTION: STBC 0x0056f1b0 _$E279
// FUNCTION: STBC 0x0056f1c0 _$E286
// FUNCTION: STBC 0x0056f1d0 _$E285
// FUNCTION: STBC 0x0056f1e0 _$E231
// FUNCTION: STBC 0x0056f1f0 _$E230
// FUNCTION: STBC 0x0056f200 _$E235
// FUNCTION: STBC 0x0056f210 _$E234
// FUNCTION: STBC 0x0056f220 _$E341
// FUNCTION: STBC 0x0056f230 _$E340
// FUNCTION: STBC 0x0056f240 _$E345
// FUNCTION: STBC 0x0056f250 _$E344
// FUNCTION: STBC 0x0056f260 _$E349
// FUNCTION: STBC 0x0056f270 _$E348
// FUNCTION: STBC 0x0056f280 _$E353
// FUNCTION: STBC 0x0056f290 _$E352
// FUNCTION: STBC 0x0056f2a0 _$E357
// FUNCTION: STBC 0x0056f2b0 _$E356
// FUNCTION: STBC 0x0056f2c0 _$E361
// FUNCTION: STBC 0x0056f2d0 _$E360
// FUNCTION: STBC 0x0056f2e0 _$E365
// FUNCTION: STBC 0x0056f2f0 _$E364
// FUNCTION: STBC 0x0056f300 _$E369
// FUNCTION: STBC 0x0056f310 _$E368
// FUNCTION: STBC 0x0056f320 _$E131
// FUNCTION: STBC 0x0056f330 _$E130
// FUNCTION: STBC 0x0056f340 _$E373
// FUNCTION: STBC 0x0056f350 _$E372
// FUNCTION: STBC 0x0056f360 _$E136
// FUNCTION: STBC 0x0056f370 _$E135
// FUNCTION: STBC 0x0056f380 _$E377
// FUNCTION: STBC 0x0056f390 _$E376
// FUNCTION: STBC 0x0056f3a0 _$E381
// FUNCTION: STBC 0x0056f3b0 _$E380
// FUNCTION: STBC 0x0056f3c0 _$E385
// FUNCTION: STBC 0x0056f3d0 _$E384
// FUNCTION: STBC 0x0056f3e0 _$E389
// FUNCTION: STBC 0x0056f3f0 _$E388
// FUNCTION: STBC 0x0056f400 _$E141
// FUNCTION: STBC 0x0056f410 _$E140
// FUNCTION: STBC 0x0056f420 _$E393
// FUNCTION: STBC 0x0056f430 _$E392
// FUNCTION: STBC 0x0056f440 _$E397
// FUNCTION: STBC 0x0056f450 _$E396
// FUNCTION: STBC 0x0056f460 _$E401
// FUNCTION: STBC 0x0056f470 _$E400
// FUNCTION: STBC 0x0056f480 _$E147
// FUNCTION: STBC 0x0056f490 _$E146
// FUNCTION: STBC 0x0056f4a0 _$E152
// FUNCTION: STBC 0x0056f4b0 _$E151
// FUNCTION: STBC 0x0056f4c0 _$E182
// FUNCTION: STBC 0x0056f4d0 _$E181
// FUNCTION: STBC 0x0056f4e0 _$E405
// FUNCTION: STBC 0x0056f4f0 _$E404
// FUNCTION: STBC 0x0056f500 _$E187
// FUNCTION: STBC 0x0056f510 _$E186
// FUNCTION: STBC 0x0056f520 _$E409
// FUNCTION: STBC 0x0056f530 _$E408
// FUNCTION: STBC 0x0056f540 _$E413
// FUNCTION: STBC 0x0056f550 _$E412
// FUNCTION: STBC 0x0056f560 _$E192
// FUNCTION: STBC 0x0056f570 _$E191
// FUNCTION: STBC 0x0056f580 _$E417
// FUNCTION: STBC 0x0056f590 _$E416
// FUNCTION: STBC 0x0056f5a0 _$E157
// FUNCTION: STBC 0x0056f5b0 _$E156
// FUNCTION: STBC 0x0056f5c0 _$E421
// FUNCTION: STBC 0x0056f5d0 _$E420
// FUNCTION: STBC 0x0056f5e0 _$E425
// FUNCTION: STBC 0x0056f5f0 _$E424
// FUNCTION: STBC 0x0056f600 _$E197
// FUNCTION: STBC 0x0056f610 _$E196
// FUNCTION: STBC 0x0056f620 _$E202
// FUNCTION: STBC 0x0056f630 _$E201
// FUNCTION: STBC 0x0056f640 _$E429
// FUNCTION: STBC 0x0056f650 _$E428
// FUNCTION: STBC 0x0056f660 _$E433
// FUNCTION: STBC 0x0056f670 _$E432
// FUNCTION: STBC 0x0056f680 _$E437
// FUNCTION: STBC 0x0056f690 _$E436
// FUNCTION: STBC 0x0056f6a0 _$E441
// FUNCTION: STBC 0x0056f6b0 _$E440
// FUNCTION: STBC 0x0056f6c0 _$E445
// FUNCTION: STBC 0x0056f6d0 _$E444
// FUNCTION: STBC 0x0056f6e0 _$E449
// FUNCTION: STBC 0x0056f6f0 _$E448
// FUNCTION: STBC 0x0056f700 _$E453
// FUNCTION: STBC 0x0056f710 _$E452
// FUNCTION: STBC 0x0056f720 _$E457
// FUNCTION: STBC 0x0056f730 _$E456
// FUNCTION: STBC 0x0056f740 _$E207
// FUNCTION: STBC 0x0056f750 _$E206
// FUNCTION: STBC 0x0056f760 _$E461
// FUNCTION: STBC 0x0056f770 _$E460
// FUNCTION: STBC 0x0056f780 _$E212
// FUNCTION: STBC 0x0056f790 _$E211
// FUNCTION: STBC 0x0056f7a0 _$E217
// FUNCTION: STBC 0x0056f7b0 _$E216
// FUNCTION: STBC 0x0056f7f0 _$E465
// FUNCTION: STBC 0x0056f800 _$E464
// FUNCTION: STBC 0x0056f810 _$E470
// FUNCTION: STBC 0x0056f820 _$E469
// FUNCTION: STBC 0x0056f830 _$E474
// FUNCTION: STBC 0x0056f840 _$E473

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0056dd30 _$E32
// FUNCTION: STBC 0x0056dd40 _$E29
// FUNCTION: STBC 0x0056dd50 _$E31
// FUNCTION: STBC 0x0056dd60 _$E30
// FUNCTION: STBC 0x0056dd70 _$E38
// FUNCTION: STBC 0x0056dd80 _$E35
// FUNCTION: STBC 0x0056dd90 _$E37
// FUNCTION: STBC 0x0056dda0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0056ddf0 _$E480
// FUNCTION: STBC 0x0056de00 _$E476
// FUNCTION: STBC 0x0056de10 _$E479
// FUNCTION: STBC 0x0056de20 _$E477

// DATA: STBC 0x008e5328
float WarpEngineSubsystem::warpEffectTime = 2.0f;

// DATA: STBC 0x008e532c
float WarpEngineSubsystem::warpSpeed = 700.0f;

// DATA: STBC 0x0098c400
TGEventHandlerTable WarpEngineSubsystem::classHandlers;

/*
================
WarpEngineSubsystem::Cast

The object as a WarpEngineSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0056de40
WarpEngineSubsystem *WarpEngineSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WARPENGINESUBSYSTEM_TYPE ) ) {
		return (WarpEngineSubsystem *)object;
	}
	return NULL;
}

/*
================
WarpEngineSubsystem::WarpEngineSubsystem

Engines out of warp, without a warp sequence, a placement or an AI.
================
*/
// FUNCTION: STBC 0x0056de70
WarpEngineSubsystem::WarpEngineSubsystem( long id ) : PoweredSubsystem( id ) {
	warpState = 0;
	unknownB0 = 0.0f;
	shipInvincible = false;
	aiID = 0;
	unknownD0 = false;
	unknownD1 = false;
	warpSequenceID = 0;
	unknownB8 = 0;
	placement = NULL;
	exitPoint = NiPoint3::ZERO;
}

/*
================
WarpEngineSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x0056def0
int WarpEngineSubsystem::GetObjType( void ) {
	return WARPENGINESUBSYSTEM_TYPE;
}

/*
================
WarpEngineSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x0056df60
char *WarpEngineSubsystem::GetClassName( void ) {
	return "WarpEngineSubsystem";
}

/*
================
WarpEngineSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0056df70
char *WarpEngineSubsystem::GetSwigTypeName( void ) {
	return "_p_WarpEngineSubsystem";
}

/*
================
WarpEngineSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0056df80
char *WarpEngineSubsystem::GetPythonClassName( void ) {
	return "WarpEngineSubsystemPtr";
}

/*
================
WarpEngineSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0056df90
TGEventHandlerTable *WarpEngineSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
WarpEngineSubsystem::~WarpEngineSubsystem
================
*/
// FUNCTION: STBC 0x0056dfd0
// FUNCTION: STBC 0x0056dfa0 ??_GWarpEngineSubsystem@@UAEPAXI@Z
WarpEngineSubsystem::~WarpEngineSubsystem( void ) {
	ClearAI();
}

/*
================
WarpEngineSubsystem::Update

Drives the warp through its states: each initiated state moves on to its
beginning; while the warp begins, the model is stretched along its
forward axis a step per update (six steps) and pushed forward by a fifth
of its radius, then the warp ends; while it ends, the ship flies forward
at the warp speed. While the dewarp begins and ends, the ship is carried
toward the exit (in multiplayer it flies there), and while it ends the
stretch is undone a step per update. Then the children update.
================
*/
// FUNCTION: STBC 0x0056e030
// DRAFT: frame: retail's frame is 0x18 bytes larger (radius and speed, and its point temporaries, keep separate slots), the ship and the model swap ebx/edi, and the multiplayer Length spills z to the stack where this build keeps it on the FPU; calls and branches agree
void WarpEngineSubsystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );

	ShipClass *ship = parentShip;
	NiAVObject *model = ship->GetNiObject();
	switch ( warpState ) {
	case WES_WARP_INITIATED:
		TransitionToState( WES_WARP_BEGINNING );
		break;
	case WES_WARP_BEGINNING:
		if ( unknownB8 < 6 && ship->HasClonedModel() ) {
			NiMatrix3 stretch;
			stretch.MakeDiagonal( 1.0f, 1.25f, 1.0f );
			model->ApplyTransform( stretch, NiPoint3::ZERO, false );
			float radius = model->GetWorldBound().radius;
			NiPoint3 offset = ( radius * 0.2f ) * parentShip->GetWorldForward();
			ship->GetNode()->ApplyTransform( NiMatrix3::IDENTITY, offset, true );
			ship->MarkGeometryChanged( NULL );
			model->GetWorldBound().unknown10 = true;
			unknownB8++;
		} else {
			TransitionToState( WES_WARP_ENDING );
		}
		break;
	case WES_WARP_ENDING: {
		NiPoint3 velocity = ship->GetWorldForward() * warpSpeed;
		ship->SetVelocity( &velocity );
		break;
	}
	case WES_DEWARP_INITIATED:
		TransitionToState( WES_DEWARP_BEGINNING );
		break;
	case WES_DEWARP_BEGINNING:
		if ( g_kUtopiaModule.IsMultiplayer() ) {
			NiPoint3 toExit = exitPoint - ship->GetWorldLocation();
			float speed = toExit.Length() / unknownB0;
			NiPoint3 velocity = speed * ship->GetWorldForward();
			ship->SetVelocity( &velocity );
			unknownB0 -= deltaTime;
		} else if ( placement == NULL ) {
			NiPoint3 velocity = ship->GetWorldForward() * warpSpeed;
			ship->SetVelocity( &velocity );
		} else {
			NiPoint3 offset = placement->GetWorldLocation() - GetWarpExitLocation();
			float fraction = unknownB0 / warpEffectTime;
			if ( fraction < 0.0f ) {
				fraction = 0.0f;
			}
			ship->SetVelocity( &NiPoint3::ZERO );
			offset = offset * ( 1.0f - fraction );
			ship->SetTranslate( &( offset + GetWarpExitLocation() ) );
			unknownB0 -= deltaTime;
		}
		break;
	case WES_DEWARP_ENDING:
		if ( g_kUtopiaModule.IsMultiplayer() ) {
			NiPoint3 toExit = exitPoint - ship->GetWorldLocation();
			float speed = toExit.Length() / unknownB0;
			NiPoint3 velocity = speed * ship->GetWorldForward();
			ship->SetVelocity( &velocity );
			unknownB0 -= deltaTime;
		} else if ( placement == NULL ) {
			NiPoint3 velocity = ship->GetWorldForward() * warpSpeed;
			ship->SetVelocity( &velocity );
		} else {
			NiPoint3 offset = placement->GetWorldLocation() - GetWarpExitLocation();
			float fraction = unknownB0 / warpEffectTime;
			if ( fraction < 0.0f ) {
				fraction = 0.0f;
			}
			ship->SetVelocity( &NiPoint3::ZERO );
			offset = offset * ( 1.0f - fraction );
			ship->SetTranslate( &( offset + GetWarpExitLocation() ) );
			unknownB0 -= deltaTime;
		}
		if ( unknownB8 > 0 && parentShip->HasClonedModel() ) {
			NiMatrix3 squash;
			squash.MakeDiagonal( 1.0f, 0.8f, 1.0f );
			model->ApplyTransform( squash, NiPoint3::ZERO, false );
			float radius = model->GetWorldBound().radius;
			NiPoint3 offset = -( radius * 0.2f ) * parentShip->GetWorldForward();
			ship->GetNode()->ApplyTransform( NiMatrix3::IDENTITY, offset, true );
			parentShip->MarkGeometryChanged( NULL );
			model->GetWorldBound().unknown10 = true;
			unknownB8--;
		}
		break;
	}

	for ( int i = 0; i < numChildSubsystems; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->Update( deltaTime );
		}
	}
}

/*
================
WarpEngineSubsystem::SetAI
================
*/
// FUNCTION: STBC 0x0056e700
void WarpEngineSubsystem::SetAI( ArtificialIntelligence *ai, bool unknown1, bool unknown2 ) {
	ClearAI( true, NULL );
	if ( warpState != 0 ) {
		aiID = aiManager.AddAI( ai, true );
		unknownD0 = unknown1;
		unknownD1 = unknown2;
	} else if ( parentShip != NULL ) {
		parentShip->SetAI( ai, unknown1, unknown2 );
	}
}

/*
================
WarpEngineSubsystem::ClearAI

Forgets the AI the engines keep; with unknown, removes it from the AI
manager too. Another AI than the one kept is left alone.
================
*/
// FUNCTION: STBC 0x0056e770
void WarpEngineSubsystem::ClearAI( bool unknown, ArtificialIntelligence *ai ) {
	if ( ai != NULL && ai->GetID() != aiID ) {
		return;
	}

	if ( unknown ) {
		ArtificialIntelligence *engineAI = ArtificialIntelligence::GetAIByID( aiID );
		if ( engineAI != NULL ) {
			aiManager.RemoveAI( engineAI, true );
		}
	}
	aiID = 0;
	unknownD0 = false;
	unknownD1 = false;
}

/*
================
WarpEngineSubsystem::TransitionToState

Enters a warp state: the ship is made invincible for the warp, stopped and
given its warp impulse, stripped of its cloned models while warping, moved
to the exit and squashed for the dewarp, and given its placement's
location, its AI back and its own invincibility when the warp is over.
================
*/
// FUNCTION: STBC 0x0056e7d0
// PARKED: register: retail loads the state argument into ecx after the pushes and the current state into eax; ours swaps them; the body wrapped in a state test (98.2) and the test before the locals (91.5) tried
void WarpEngineSubsystem::TransitionToState( int state ) {
	ShipClass *ship = parentShip;
	CloakingSubsystem *cloak = ship->GetCloakingSubsystem();
	if ( state == warpState ) {
		return;
	}
	switch ( state ) {
	case WES_WARP_INITIATED:
		shipInvincible = ship->IsInvincible();
		ship->SetInvincible( true );
		break;
	case WES_NOT_WARPING:
		while ( ship->GetClonedModelCount() > 0 ) {
			ship->Unknown593550();
		}
		if ( cloak != NULL ) {
			cloak->Unknown55F5F0();
		}
		ship->MarkGeometryChanged( NULL );
		ship->GetNiObject()->GetWorldBound().unknown10 = true;
		ship->GetNiObject()->Update( 0.0f );
		ship->SetVelocity( &NiPoint3::ZERO );
		if ( placement != NULL ) {
			ship->SetTranslate( placement->GetWorldLocation() );
		}
		placement = NULL;
		unknownB0 = 0.0f;
		if ( aiID != 0 ) {
			warpState = WES_NOT_WARPING;
			ArtificialIntelligence *ai = ArtificialIntelligence::GetAIByID( aiID );
			if ( ai != NULL ) {
				aiManager.RemoveAI( ai, false );
				ship->SetAI( ai, unknownD0, unknownD1 );
			}
			ClearAI( false, NULL );
		}
		ship->SetInvincible( shipInvincible );
		shipInvincible = false;
		break;
	case WES_WARP_BEGINNING:
		unknownB0 = warpEffectTime;
		ship->Unknown593270();
		unknownB8 = 0;
		ship->SetImpulse( 0.0f, ship->GetWorldForward(), 1 );
		break;
	case WES_WARPING:
		ship->SetVelocity( &NiPoint3::ZERO );
		while ( ship->GetClonedModelCount() > 0 ) {
			ship->Unknown593550();
		}
		ship->MarkGeometryChanged( NULL );
		ship->GetNiObject()->GetWorldBound().unknown10 = true;
		break;
	case WES_DEWARP_INITIATED:
		ship->SetTranslate( GetWarpExitLocation() );
		ship->SetMatrixRotation( &GetWarpExitRotation() );
		ship->UpdateNodeOnly();
		break;
	case WES_DEWARP_BEGINNING:
		unknownB0 = warpEffectTime;
		ship->Unknown593270();
		if ( parentShip->HasClonedModel() ) {
			if ( cloak != NULL ) {
				cloak->Unknown55F5F0();
			}
			unknownB8 = 6;
			NiMatrix3 squash;
			squash.MakeDiagonal( 1.0f, (float)pow( 1.25, unknownB8 ), 1.0f );
			ship->GetNiObject()->ApplyTransform( squash, NiPoint3::ZERO, false );
			ship->MarkGeometryChanged( NULL );
			ship->GetNiObject()->GetWorldBound().unknown10 = true;
		}
		break;
	case WES_DEWARP_ENDING:
		ship->SetVelocity( &NiPoint3::ZERO );
		break;
	}
	warpState = state;
}

/*
================
WarpEngineSubsystem::Warp

Only reports whether the engines are disabled; the warp itself is driven
by TransitionToState.
================
*/
// FUNCTION: STBC 0x0056ead0
bool WarpEngineSubsystem::Warp( void ) {
	if ( IsDisabled() ) {
		return true;
	}
	return false;
}

/*
================
WarpEngineSubsystem::GetWarpExitLocation

Where the ship leaves warp: in multiplayer, the exit point; otherwise the
placement's position; the ship's own when there is no placement. The
exit point and the placement's position are pulled back along the ship's
heading by the distance flown in half the warp effect's time.
================
*/
// FUNCTION: STBC 0x0056eae0 ?GetWarpExitLocation@WarpEngineSubsystem@@QAE?AVTGPoint3@@XZ
TGPoint3 WarpEngineSubsystem::GetWarpExitLocation( void ) {
	ShipClass *ship = GetParentShip();
	PlacementObject *exitPlacement = placement;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		NiPoint3 forward = ship->GetWorldRotation() * MODEL_FORWARD;
		float distance = warpEffectTime * warpSpeed * 0.5f;
		return exitPoint - forward * distance;
	}
	if ( exitPlacement != NULL ) {
		NiPoint3 forward = exitPlacement->GetWorldRotation() * MODEL_FORWARD;
		float distance = warpEffectTime * warpSpeed * 0.5f;
		return exitPlacement->GetTranslate() - forward * distance;
	}
	return ship->GetWorldLocation();
}

/*
================
WarpEngineSubsystem::GetWarpExitRotation

The rotation of the placement the ship leaves warp at, or the ship's own
when there is none.
================
*/
// FUNCTION: STBC 0x0056ec70 ?GetWarpExitRotation@WarpEngineSubsystem@@QAE?AVTGMatrix3@@XZ
TGMatrix3 WarpEngineSubsystem::GetWarpExitRotation( void ) {
	ShipClass *ship = GetParentShip();
	if ( placement != NULL ) {
		return placement->GetWorldRotation();
	}
	return ship->GetWorldRotation();
}
/*
================
WarpEngineSubsystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0056ecd0
void WarpEngineSubsystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&WarpEngineSubsystem::SetWarpSequence, "WarpEngineSubsystem::SetWarpSequence" );
}

/*
================
WarpEngineSubsystem::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0056ecf0
void WarpEngineSubsystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SET_WARP_SEQUENCE, "WarpEngineSubsystem::SetWarpSequence" );
}

/*
================
WarpEngineSubsystem::SetWarpSequence

The warp sequence is the event's source; none when the source is no
action.
================
*/
// FUNCTION: STBC 0x0056ed10
void WarpEngineSubsystem::SetWarpSequence( TGEvent *event ) {
	TGAction *sequence = TGAction::Cast( event->GetSource() );
	warpSequenceID = sequence != NULL ? sequence->GetObjID() : 0;
}

/*
================
WarpEngineSubsystem::WriteToStream

The warp state and exit, the placement by ID, the AI's flags and the warp
sequence's ID.
================
*/
// FUNCTION: STBC 0x0056ed40
bool WarpEngineSubsystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( shipInvincible );
	stream->WriteFloat( unknownB0 );
	stream->WriteLong( warpState );
	stream->WriteLong( unknownB8 );
	if ( placement != NULL ) {
		stream->WriteID( placement->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}
	stream->WriteFloat( exitPoint.x );
	stream->WriteFloat( exitPoint.y );
	stream->WriteFloat( exitPoint.z );
	stream->WriteLong( aiID );
	stream->WriteBool( unknownD0 );
	stream->WriteBool( unknownD1 );
	stream->WriteID( warpSequenceID );
	return true;
}

/*
================
WarpEngineSubsystem::ReadFromStream

Reads what WriteToStream writes; the placement stays an ID until
PostFixUp.
================
*/
// FUNCTION: STBC 0x0056ee20
bool WarpEngineSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	shipInvincible = stream->ReadBool();
	unknownB0 = stream->ReadFloat();
	warpState = stream->ReadLong();
	unknownB8 = stream->ReadLong();
	placement = (PlacementObject *)stream->ReadID();
	exitPoint.x = stream->ReadFloat();
	exitPoint.y = stream->ReadFloat();
	exitPoint.z = stream->ReadFloat();
	aiID = stream->ReadLong();
	unknownD0 = stream->ReadBool();
	unknownD1 = stream->ReadBool();
	warpSequenceID = stream->ReadID();
	return true;
}

/*
================
WarpEngineSubsystem::FixUpPointers

Fails when the placement has not been loaded.
================
*/
// FUNCTION: STBC 0x0056eee0
bool WarpEngineSubsystem::FixUpPointers( void ) {
	if ( !ShipSubsystem::FixUpPointers() ) {
		return false;
	}
	if ( placement != NULL && GetTGObjectPtr( (long)placement ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
WarpEngineSubsystem::PostFixUp

Turns the placement's ID back into the object.
================
*/
// FUNCTION: STBC 0x0056ef10
void WarpEngineSubsystem::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();
	placement = (PlacementObject *)GetSavedObject( (long)placement );
}

