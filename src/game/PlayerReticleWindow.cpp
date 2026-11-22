/*
===============================================================================

	PlayerReticleWindow.cpp

	The player's reticle: the named reticle on the player's target, with a
	marker on the targeted subsystem and the lock bars.

===============================================================================
*/

#include "ReticleWindow.h"
#include "BridgeSet.h"
#include "CameraObjectClass.h"
#include "Game.h"
#include "GraphicsModeInfo.h"
#include "NiMath.h"
#include "PhysicsObjectClass.h"
#include "TGSoundManager.h"
#include "TorpedoAmmoType.h"
#include "TorpedoSystem.h"
#include "NiNode.h"
#include "ShipSubsystem.h"
#include "TGIconManager.h"
#include "ReticleManagerWindow.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGIcon.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "ViewScreenObject.h"
#include "UtopiaModule.h"
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "Planet.h"
#include <stdio.h>
#include "NiDynamicEffect.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00511550 _$E33
// FUNCTION: STBC 0x00511560 _$E32
// FUNCTION: STBC 0x00511570 _$E37
// FUNCTION: STBC 0x00511580 _$E36
// FUNCTION: STBC 0x00511590 _$E48
// FUNCTION: STBC 0x005115a0 _$E47
// FUNCTION: STBC 0x005115b0 _$E52
// FUNCTION: STBC 0x005115c0 _$E51
// FUNCTION: STBC 0x005115d0 _$E56
// FUNCTION: STBC 0x005115e0 _$E55
// FUNCTION: STBC 0x005115f0 _$E60
// FUNCTION: STBC 0x00511600 _$E59
// FUNCTION: STBC 0x00511610 _$E184
// FUNCTION: STBC 0x00511620 _$E183
// FUNCTION: STBC 0x00512fc0 _$E126
// FUNCTION: STBC 0x00512fd0 _$E125
// FUNCTION: STBC 0x00512fe0 _$E139
// FUNCTION: STBC 0x00512ff0 _$E138
// FUNCTION: STBC 0x00513000 _$E189
// FUNCTION: STBC 0x00513010 _$E188
// FUNCTION: STBC 0x005134d0 _$E194
// FUNCTION: STBC 0x005134e0 _$E193
// FUNCTION: STBC 0x005134f0 _$E198
// FUNCTION: STBC 0x00513500 _$E197
// FUNCTION: STBC 0x00513510 _$E202
// FUNCTION: STBC 0x00513520 _$E201
// FUNCTION: STBC 0x00513530 _$E209
// FUNCTION: STBC 0x00513540 _$E208
// FUNCTION: STBC 0x00513550 _$E347
// FUNCTION: STBC 0x00513560 _$E346
// FUNCTION: STBC 0x00513570 _$E351
// FUNCTION: STBC 0x00513580 _$E350
// FUNCTION: STBC 0x00513590 _$E84
// FUNCTION: STBC 0x005135a0 _$E83
// FUNCTION: STBC 0x005135b0 _$E279
// FUNCTION: STBC 0x005135c0 _$E278
// FUNCTION: STBC 0x005135d0 _$E286
// FUNCTION: STBC 0x005135e0 _$E285
// FUNCTION: STBC 0x005135f0 _$E290
// FUNCTION: STBC 0x00513600 _$E289
// FUNCTION: STBC 0x00513610 _$E294
// FUNCTION: STBC 0x00513620 _$E293
// FUNCTION: STBC 0x00513630 _$E298
// FUNCTION: STBC 0x00513640 _$E297
// FUNCTION: STBC 0x00513650 _$E304
// FUNCTION: STBC 0x00513660 _$E303
// FUNCTION: STBC 0x00513670 _$E355
// FUNCTION: STBC 0x00513680 _$E354
// FUNCTION: STBC 0x00513690 _$E359
// FUNCTION: STBC 0x005136a0 _$E358
// FUNCTION: STBC 0x005136b0 _$E363
// FUNCTION: STBC 0x005136c0 _$E362
// FUNCTION: STBC 0x005136d0 _$E367
// FUNCTION: STBC 0x005136e0 _$E366
// FUNCTION: STBC 0x005136f0 _$E371
// FUNCTION: STBC 0x00513700 _$E370
// FUNCTION: STBC 0x00513710 _$E375
// FUNCTION: STBC 0x00513720 _$E374
// FUNCTION: STBC 0x00513730 _$E379
// FUNCTION: STBC 0x00513740 _$E378
// FUNCTION: STBC 0x00513750 _$E383
// FUNCTION: STBC 0x00513760 _$E382
// FUNCTION: STBC 0x00513770 _$E387
// FUNCTION: STBC 0x00513780 _$E386
// FUNCTION: STBC 0x00513790 _$E391
// FUNCTION: STBC 0x005137a0 _$E390
// FUNCTION: STBC 0x005137b0 _$E89
// FUNCTION: STBC 0x005137c0 _$E88
// FUNCTION: STBC 0x005137d0 _$E395
// FUNCTION: STBC 0x005137e0 _$E394
// FUNCTION: STBC 0x005137f0 _$E94
// FUNCTION: STBC 0x00513800 _$E93
// FUNCTION: STBC 0x00513810 _$E399
// FUNCTION: STBC 0x00513820 _$E398
// FUNCTION: STBC 0x00513830 _$E403
// FUNCTION: STBC 0x00513840 _$E402
// FUNCTION: STBC 0x00513850 _$E407
// FUNCTION: STBC 0x00513860 _$E406
// FUNCTION: STBC 0x00513870 _$E411
// FUNCTION: STBC 0x00513880 _$E410
// FUNCTION: STBC 0x00513890 _$E103
// FUNCTION: STBC 0x005138a0 _$E102
// FUNCTION: STBC 0x005138b0 _$E415
// FUNCTION: STBC 0x005138c0 _$E414
// FUNCTION: STBC 0x005138d0 _$E419
// FUNCTION: STBC 0x005138e0 _$E418
// FUNCTION: STBC 0x005138f0 _$E423
// FUNCTION: STBC 0x00513900 _$E422
// FUNCTION: STBC 0x00513910 _$E109
// FUNCTION: STBC 0x00513920 _$E108
// FUNCTION: STBC 0x00513930 _$E114
// FUNCTION: STBC 0x00513940 _$E113
// FUNCTION: STBC 0x00513950 _$E144
// FUNCTION: STBC 0x00513960 _$E143
// FUNCTION: STBC 0x00513970 _$E427
// FUNCTION: STBC 0x00513980 _$E426
// FUNCTION: STBC 0x00513990 _$E149
// FUNCTION: STBC 0x005139a0 _$E148
// FUNCTION: STBC 0x005139b0 _$E431
// FUNCTION: STBC 0x005139c0 _$E430
// FUNCTION: STBC 0x005139d0 _$E435
// FUNCTION: STBC 0x005139e0 _$E434
// FUNCTION: STBC 0x005139f0 _$E154
// FUNCTION: STBC 0x00513a00 _$E153
// FUNCTION: STBC 0x00513a10 _$E439
// FUNCTION: STBC 0x00513a20 _$E438
// FUNCTION: STBC 0x00513a30 _$E119
// FUNCTION: STBC 0x00513a40 _$E118
// FUNCTION: STBC 0x00513a50 _$E443
// FUNCTION: STBC 0x00513a60 _$E442
// FUNCTION: STBC 0x00513a70 _$E447
// FUNCTION: STBC 0x00513a80 _$E446
// FUNCTION: STBC 0x00513a90 _$E159
// FUNCTION: STBC 0x00513aa0 _$E158
// FUNCTION: STBC 0x00513ab0 _$E164
// FUNCTION: STBC 0x00513ac0 _$E163
// FUNCTION: STBC 0x00513ad0 _$E451
// FUNCTION: STBC 0x00513ae0 _$E450
// FUNCTION: STBC 0x00513af0 _$E455
// FUNCTION: STBC 0x00513b00 _$E454
// FUNCTION: STBC 0x00513b10 _$E459
// FUNCTION: STBC 0x00513b20 _$E458
// FUNCTION: STBC 0x00513b30 _$E463
// FUNCTION: STBC 0x00513b40 _$E462
// FUNCTION: STBC 0x00513b50 _$E467
// FUNCTION: STBC 0x00513b60 _$E466
// FUNCTION: STBC 0x00513b70 _$E471
// FUNCTION: STBC 0x00513b80 _$E470
// FUNCTION: STBC 0x00513b90 _$E475
// FUNCTION: STBC 0x00513ba0 _$E474
// FUNCTION: STBC 0x00513bb0 _$E479
// FUNCTION: STBC 0x00513bc0 _$E478
// FUNCTION: STBC 0x00513bd0 _$E169
// FUNCTION: STBC 0x00513be0 _$E168
// FUNCTION: STBC 0x00513bf0 _$E483
// FUNCTION: STBC 0x00513c00 _$E482
// FUNCTION: STBC 0x00513c10 _$E174
// FUNCTION: STBC 0x00513c20 _$E173
// FUNCTION: STBC 0x00513c30 _$E179
// FUNCTION: STBC 0x00513c40 _$E178
// FUNCTION: STBC 0x00513cb0 _$E487
// FUNCTION: STBC 0x00513cc0 _$E486
// FUNCTION: STBC 0x00513cd0 _$E492
// FUNCTION: STBC 0x00513ce0 _$E491
// FUNCTION: STBC 0x00513cf0 _$E496
// FUNCTION: STBC 0x00513d00 _$E495

// The unit's own copies of the angles, beside the class's vtable.
// DATA: STBC 0x0088f358
static const float PI = 3.14159265f;
// DATA: STBC 0x0088f35c
static const float QUARTER_PI = 0.785398163f;

// The lock shows within this many seconds of torpedo flight (invented
// name).
#define TORPEDO_LOCK_SECONDS	60.0f
// Unknown512310's value for no lock (invented name).
#define NO_LOCK					101.0f

// The LCARS group's icons (invented names, as in NamedReticleWindow).
#define RETICLE_BAR_ICON		200
#define RETICLE_FRAME_ICON		0x78

// The locale's ctype id initialiser and its exit-time destructor (<string>).
// FUNCTION: STBC 0x00513c50 _$E511
// FUNCTION: STBC 0x00513c70 _$E510

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005114d0 _$E331
// FUNCTION: STBC 0x005114e0 _$E328
// FUNCTION: STBC 0x005114f0 _$E330
// FUNCTION: STBC 0x00511500 _$E329
// FUNCTION: STBC 0x00511510 _$E337
// FUNCTION: STBC 0x00511520 _$E334
// FUNCTION: STBC 0x00511530 _$E336
// FUNCTION: STBC 0x00511540 _$E335

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00511630 _$E502
// FUNCTION: STBC 0x00511640 _$E498
// FUNCTION: STBC 0x00511650 _$E501
// FUNCTION: STBC 0x00511660 _$E499
// DATA: STBC 0x00987b18
TGEventHandlerTable PlayerReticleWindow::classHandlers;

/*
================
PlayerReticleWindow::PlayerReticleWindow

A pane in front of the named reticle holds the distance text, the frame
and the bars' pane. The bars' pane holds six green bars, the four lock
brackets (icons 5 to 8 of "ReticleTextures"), fourteen faint white bars
and eight frames; the subsystem marker goes in front of everything.
================
*/
// FUNCTION: STBC 0x00511680
// PARKED: inline budget: registers and calls match except the pool allocations; retail expands the first eight and calls TGPoolManager::AllocateObject (0x40F030) for the rest, assigns the last colour through 0x494C10 and calls PrependChild's copy 0x513C80; needs the two-level TGPOOL_ALLOCATION (docs/levers.md), under which no consistent new or TGIcon::Create spelling expands exactly eight
PlayerReticleWindow::PlayerReticleWindow( void ) {
	unknown80 = new TGPane( 1.0f, 1.0f );
	unknown80->AddChild( new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY ) );

	char *lcarsGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	unknown8C = new TGIcon( lcarsGroup, RETICLE_FRAME_ICON );
	unknown80->AddChild( unknown8C );
	TGPane *bars = new TGPane( 1.0f, 1.0f );
	unknown88 = bars;
	unknown80->AddChild( bars, 0.0f, 0.0f, false );

	NiColorA color( 0.0f, 1.0f, 0.3f, 0.75f );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );

	TGIcon *left = new TGIcon( "ReticleTextures", 5, &color );
	TGIcon *right = new TGIcon( "ReticleTextures", 7, &color );
	bars->AddChild( left, 0.0f, 0.0f, false );
	bars->AddChild( right, 0.0f, 0.0f, false );
	left = new TGIcon( "ReticleTextures", 6, &color );
	right = new TGIcon( "ReticleTextures", 8, &color );
	bars->AddChild( left, 0.0f, 0.0f, false );
	bars->AddChild( right, 0.0f, 0.0f, false );

	color = NiColorA( 1.0f, 1.0f, 1.0f, 0.2f );
	int i;
	for ( i = 0; i < 14; i++ ) {
		bars->AddChild( new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color ), 0.0f, 0.0f, false );
	}
	for ( i = 0; i < 8; i++ ) {
		bars->AddChild( new TGIcon( lcarsGroup, RETICLE_FRAME_ICON ), 0.0f, 0.0f, false );
	}
	PrependChild( unknown80 );

	color = NiColorA( 1.0f, 1.0f, 0.0f, 0.5f );
	subsystemMarker = new TGIcon( "ReticleTextures", 4, &color );
	PrependChild( subsystemMarker );

	unknown90 = false;
	unknown94 = 0.0f;
	unknown98 = 0.5f;
	unknown9C = true;
}

/*
================
PlayerReticleWindow::GetObjType
================
*/
// FUNCTION: STBC 0x00511c40
int PlayerReticleWindow::GetObjType( void ) {
	return PLAYERRETICLEWINDOW_TYPE;
}

/*
================
PlayerReticleWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00511c50
bool PlayerReticleWindow::IsTypeOf( int type ) {
	if ( type == PLAYERRETICLEWINDOW_TYPE ) {
		return true;
	}
	return NamedReticleWindow::IsTypeOf( type );
}

/*
================
PlayerReticleWindow::GetClassName
================
*/
// FUNCTION: STBC 0x00511cc0
char *PlayerReticleWindow::GetClassName( void ) {
	return "PlayerReticleWindow";
}

/*
================
PlayerReticleWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00511cd0
char *PlayerReticleWindow::GetSwigTypeName( void ) {
	return "_p_PlayerReticleWindow";
}

/*
================
PlayerReticleWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00511ce0
char *PlayerReticleWindow::GetPythonClassName( void ) {
	return "PlayerReticleWindowPtr";
}

/*
================
PlayerReticleWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00511cf0
TGEventHandlerTable *PlayerReticleWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PlayerReticleWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00511d80
void PlayerReticleWindow::RegisterHandlerNames( void ) {
}

/*
================
PlayerReticleWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00511d90
void PlayerReticleWindow::RegisterHandlers( void ) {
}

/*
================
PlayerReticleWindow::VirtualSlot82

Follows the player's target. Hidden without a game, a player or a target,
and when the target is outside the rendered set, or, while the map is
zoomed, outside the set the view screen's remote camera shows.
================
*/
// FUNCTION: STBC 0x00511da0
// PARKED: register: the zoomed arm loads the rendered set into eax and the remote camera's set into ecx in retail (ours ecx, then eax/ecx swapped); the operands swapped, a BridgeSet or view screen local don't move it
void PlayerReticleWindow::VirtualSlot82( float time ) {
	if ( Game::GetCurrentGame() == NULL ) {
		SetNotVisible();
		return;
	}
	ShipClass *player = Game::GetCurrentGame()->GetPlayer();
	if ( player == NULL ) {
		SetNotVisible();
		return;
	}
	ObjectClass *target = player->GetTarget();
	if ( target == NULL ) {
		SetNotVisible();
		return;
	}
	if ( target->GetObjID() != targetID ) {
		SetTargetID( target->GetObjID() );
	}
	if ( !ReticleManagerWindow::GetReticleManagerWindow()->IsZoomed() ) {
		if ( target->GetContainingSet() != g_kSetManager.GetRenderedSet() ) {
			SetNotVisible();
			return;
		}
		Unknown511E70( target, time );
		return;
	}
	if ( ( (BridgeSet *)g_kSetManager.GetRenderedSet() )->GetViewScreen()->GetRemoteCam()->GetContainingSet() != target->GetContainingSet() ) {
		SetNotVisible();
		return;
	}
	Unknown511E70( target, time );
}

/*
================
PlayerReticleWindow::Unknown511E70

Places the reticle on the player's target. The point to aim at is the
target subsystem's on the target, led by the target's velocity over the
current torpedo's flight time. Within sixty seconds of torpedo flight, and
while the target arrow is hidden, the subsystem marker shows and the lock
is the angle between the player's heading and the aim, measured from the
nearer end in quarter turns; the lock is 101 (no lock) otherwise. While
blinking is on, the bars flash every interval, with the "UICrosshair"
sound as they light.
================
*/
// FUNCTION: STBC 0x00511e70
// PARKED: register save: retail pushes EDI (the bars' loop counter) at entry, this build only around the blink; a counter declared at the top, a member inline and a file-static helper for the bar loops don't reproduce it (the helper moves every register)
void PlayerReticleWindow::Unknown511E70( ObjectClass *target, float time ) {
	Unknown5103D0( target );
	ShipClass *player = Game::GetCurrentGame()->GetPlayer();
	NiPoint3 aim = target->GetWorldLocation();
	ShipSubsystem *subsystem = player->GetTargetSubsystem();
	if ( subsystem != NULL ) {
		NiPoint3 point = subsystem->GetDamagePoint();
		NiMatrix3 rotation = target->GetRotation();
		point = rotation * point;
		aim += point;
	}
	NiPoint3 velocity = NiPoint3::ZERO;
	PhysicsObjectClass *physics = PhysicsObjectClass::Cast( target );
	if ( physics != NULL ) {
		velocity = physics->GetVelocity();
	}
	aim = aim - player->GetWorldLocation();
	float distance = aim.Length();
	float speed;
	TorpedoSystem *torpedoes = player->GetTorpedoSystem();
	if ( torpedoes != NULL ) {
		speed = torpedoes->GetCurrentAmmoType()->GetLaunchSpeed();
		aim = velocity * ( distance / speed ) + aim;
	} else {
		speed = 0.0f;
	}

	NiColorA color( 0.0f, 0.0f, 0.0f, 0.75f );
	float lock;
	if ( distance < speed * TORPEDO_LOCK_SECONDS && !targetArrow->IsVisible() ) {
		aim.Unitize();
		NiPoint3 forward = player->GetWorldForward();
		lock = NiACos( forward.Dot( aim ) );
		if ( lock < QUARTER_PI || lock > PI - QUARTER_PI ) {
			if ( lock > QUARTER_PI ) {
				lock = PI - lock;
			}
			lock /= QUARTER_PI;
		}
		color.r = 0.86f;
		color.g = 0.37f;
		color.b = 0.28f;
		Unknown513050( player, target );
	} else {
		color.r = 0.75f;
		color.g = 0.75f;
		color.b = 0.75f;
		lock = NO_LOCK;
		subsystemMarker->SetNotVisible();
	}
	SetColor( &color );

	if ( unknown90 && time - unknown94 > unknown98 ) {
		if ( unknown9C ) {
			color.r = 0.0f;
			color.g = 1.0f;
			color.b = 0.3f;
			color.a = 0.0f;
			for ( int i = 4; i < 10; i++ ) {
				TGIcon *bar = (TGIcon *)unknown88->GetNthChild( i );
				bar->SetColor( &color );
			}
			unknown9C = false;
		} else {
			g_kSoundManager.PlaySound( "UICrosshair" );
			color.r = 0.0f;
			color.g = 1.0f;
			color.b = 0.3f;
			color.a = 0.75f;
			for ( int i = 4; i < 10; i++ ) {
				TGIcon *bar = (TGIcon *)unknown88->GetNthChild( i );
				bar->SetColor( &color );
			}
			unknown9C = true;
		}
		unknown94 = time;
	}
	Unknown512310( player, target, lock );
}

// the lock at which the bars close in fully, and how fast they close in
// from there to a half
#define FULL_LOCK				0.1f
#define LOCK_CLOSE_RATE			2.5f
// as NamedReticleWindow's: the gap between the arrow and the text, and how
// far the text drops for a ship, in pixels
#define RETICLE_TEXT_GAP		3.0f
#define RETICLE_NAME_RAISE		7.0f

/*
================
PlayerReticleWindow::Unknown512310

Writes the target's distance (and speed, for a moving target) under the
reticle, in kilometres and kph from "data/TGL/Bridge Menus.TGL". With the
target arrow showing, the text goes beside the arrow and the bars hide.
Otherwise the text goes under the reticle, the side bars (icons 10 to 23)
line its sides, and with a lock under 100 the lock brackets (icons 0 to 3
and 24 to 31) close in on the middle as the lock falls to 0.1; at full
lock the lock bars (icons 4 to 9) show and start blinking.
================
*/
// FUNCTION: STBC 0x00512310
// DRAFT: calls and branches agree; retail copies the text size through fld/fstp, divides 1.0 from memory, loads the offset vectors' components x first, and packs the many float locals into other slots
void PlayerReticleWindow::Unknown512310( ShipClass *player, ObjectClass *target, float lock ) {
	TGParagraph *name = (TGParagraph *)unknown80->GetNthChild( 0 );
	NiPoint3 playerPosition = player->GetWorldLocation();
	NiPoint3 targetPosition = target->GetWorldLocation();
	float distance = NiPoint3::VectorLength( targetPosition - playerPosition );
	if ( target->IsTypeOf( PLANET_TYPE ) ) {
		distance -= target->GetRadius();
	}
	distance = UtopiaModule::ConvertGameUnitsToKilometers( distance );

	TGString text;
	char buffer[256];
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.TGL", true );
	PhysicsObjectClass *physics = PhysicsObjectClass::Cast( target );
	if ( physics != NULL ) {
		int speed = (int)( UtopiaModule::ConvertGameUnitsToKilometers( physics->GetVelocity().Length() ) * 3600.0f );
		sprintf( buffer, "%1.2f ", distance );
		text.AssignC( buffer );
		text.Unknown6F4970( database->GetString( "km" ) );
		sprintf( buffer, " / %d ", speed );
		text.Unknown6F4A40( buffer );
		text.Unknown6F4970( database->GetString( "kph" ) );
	} else {
		sprintf( buffer, "%1.2f ", distance );
		text.AssignC( buffer );
		text.Unknown6F4970( database->GetString( "km" ) );
	}
	name->SetString( &text, false );
	g_kLocalizationManager.Unload( database );

	float textWidth = name->GetWidth();
	float textHeight = name->GetHeight();
	float pixelHeight = 1.0f / g_kIconManager.GetScreenHeight();
	float pixelWidth = 1.0f / g_kIconManager.GetScreenWidth();
	bool isShip = target->IsTypeOf( SHIPCLASS_TYPE );
	if ( targetArrow->IsVisible() ) {
		// the text goes beside the arrow
		float x = pixelWidth * RETICLE_TEXT_GAP;
		float y;
		switch ( targetArrow->GetIconNum() ) {
			case 0:
			case 3:
				x = unknown6C.width + unknown6C.left + x;
				y = textHeight + unknown6C.top;
				if ( isShip ) {
					y += pixelHeight * RETICLE_NAME_RAISE;
				}
				break;
			case 5:
				x = unknown6C.width + unknown6C.left + x;
				y = textHeight + unknown6C.top;
				break;
			case 1:
			case 2:
			case 4:
				x = unknown6C.left - textWidth - x;
				y = textHeight + unknown6C.top;
				if ( isShip ) {
					y += pixelHeight * RETICLE_NAME_RAISE;
				}
				break;
			case 6:
			case 7:
				x = unknown6C.left - textWidth - x;
				y = textHeight + unknown6C.top;
				break;
		}
		name->SetPosition( x, y, true );
		unknown88->SetNotVisible( true );
	} else {
		// the text goes under the reticle, the side bars along it
		unknown88->SetVisible( true );
		name->SetPosition( ( unknown6C.width - textWidth ) * 0.5f + unknown6C.left, pixelHeight * 2.0f + unknown6C.top + unknown6C.height, true );

		float cornerWidth = corners[0]->GetWidth();
		float cornerHeight = corners[0]->GetHeight();
		float left = unknown6C.left;
		float top = unknown6C.top;
		float right = left + unknown6C.width;
		float bottom = top + unknown6C.height;
		float barHeight = cornerHeight * 0.25f;
		float barWidth = cornerWidth * 0.25f;
		float spacing = ( unknown6C.height - cornerHeight * 2.0f ) * 0.125f;
		float barY = spacing + top + cornerHeight;
		float sideBarHeight = barHeight * 0.5f;
		float rightX = right - barWidth;
		int i;
		for ( i = 10; i < 17; i++ ) {
			TGUIObject *leftBar = unknown88->GetNthChild( i );
			TGUIObject *rightBar = unknown88->GetNthChild( i + 7 );
			leftBar->Resize( barWidth, sideBarHeight, false );
			rightBar->Resize( barWidth, sideBarHeight, false );
			leftBar->SetPosition( left, barY, false );
			rightBar->SetPosition( rightX, barY, false );
			barY += spacing;
		}

		if ( lock >= 100.0f ) {
			for ( i = 0; i < 10; i++ ) {
				unknown88->GetNthChild( i )->SetNotVisible( true );
			}
			for ( i = 24; i < 32; i++ ) {
				unknown88->GetNthChild( i )->SetNotVisible( true );
			}
		} else {
			if ( lock > 0.5f ) {
				lock = 0.5f;
			}
			bool locked = false;
			float closing;
			if ( lock < FULL_LOCK ) {
				locked = true;
				closing = 1.0f;
			} else {
				closing = 1.0f - ( lock - FULL_LOCK ) * LOCK_CLOSE_RATE;
			}

			// the brackets on the left
			TGUIObject *bracket24 = unknown88->GetNthChild( 24 );
			TGUIObject *bracket25 = unknown88->GetNthChild( 25 );
			TGUIObject *bracket26 = unknown88->GetNthChild( 26 );
			TGUIObject *bracket27 = unknown88->GetNthChild( 27 );
			TGUIObject *bracket0 = unknown88->GetNthChild( 0 );
			TGUIObject *bracket1 = unknown88->GetNthChild( 1 );
			bracket0->SetVisible( false );
			bracket1->SetVisible( false );
			bracket24->SetVisible( false );
			bracket25->SetVisible( false );
			bracket26->SetVisible( false );
			bracket27->SetVisible( false );
			float travel = unknown6C.height * 0.5f - cornerHeight - barHeight * 1.5f;
			bracket0->Resize( barWidth, barHeight, false );
			bracket1->Resize( barWidth, barHeight, false );
			bracket24->Resize( barWidth, barHeight, false );
			bracket25->Resize( barWidth, barHeight, false );
			bracket26->Resize( barWidth, barHeight, false );
			bracket27->Resize( barWidth, barHeight, false );
			float shift = travel * closing;
			float y0 = shift + barHeight + top + cornerHeight;
			bracket0->SetPosition( left, y0, false );
			float low = bottom - cornerHeight;
			float y1 = low - barHeight * 2.0f - shift;
			bracket1->SetPosition( left, y1, false );
			float y24 = shift + top + cornerHeight;
			bracket24->SetPosition( left, y24, false );
			float y25 = barHeight * 2.0f + shift + top + cornerHeight;
			bracket25->SetPosition( left, y25, false );
			float y26 = low - barHeight * 3.0f - shift;
			bracket26->SetPosition( left, y26, false );
			float y27 = low - barHeight - shift;
			bracket27->SetPosition( left, y27, false );

			// and their mirrors on the right
			TGUIObject *bracket28 = unknown88->GetNthChild( 28 );
			TGUIObject *bracket29 = unknown88->GetNthChild( 29 );
			TGUIObject *bracket30 = unknown88->GetNthChild( 30 );
			TGUIObject *bracket31 = unknown88->GetNthChild( 31 );
			TGUIObject *bracket2 = unknown88->GetNthChild( 2 );
			TGUIObject *bracket3 = unknown88->GetNthChild( 3 );
			bracket2->SetVisible( false );
			bracket3->SetVisible( false );
			bracket28->SetVisible( false );
			bracket29->SetVisible( false );
			bracket30->SetVisible( false );
			bracket31->SetVisible( false );
			bracket2->Resize( barWidth, barHeight, false );
			bracket3->Resize( barWidth, barHeight, false );
			bracket28->Resize( barWidth, barHeight, false );
			bracket29->Resize( barWidth, barHeight, false );
			bracket30->Resize( barWidth, barHeight, false );
			bracket31->Resize( barWidth, barHeight, false );
			bracket2->SetPosition( rightX, y0, false );
			bracket3->SetPosition( rightX, y1, false );
			bracket28->SetPosition( rightX, y24, false );
			bracket29->SetPosition( rightX, y25, false );
			bracket30->SetPosition( rightX, y26, false );
			bracket31->SetPosition( rightX, y27, false );

			if ( locked ) {
				if ( !unknown90 ) {
					unknown90 = true;
					unknown94 = 0.0f;
					unknown9C = false;
				}
				// the lock bars: across the top and bottom, then two pairs at
				// the sides
				TGUIObject *topBar = unknown88->GetNthChild( 4 );
				TGUIObject *bottomBar = unknown88->GetNthChild( 5 );
				topBar->SetVisible( false );
				bottomBar->SetVisible( false );
				float innerWidth = unknown6C.width - cornerWidth * 2.0f;
				float thirdWidth = innerWidth * ( 1.0f / 3.0f );
				topBar->Resize( thirdWidth, barHeight, false );
				bottomBar->Resize( thirdWidth, barHeight, false );
				topBar->SetPosition( unknown6C.width * 0.5f + left - innerWidth * ( 1.0f / 6.0f ), top, false );
				bottomBar->SetPosition( unknown6C.width * 0.5f + left - innerWidth * ( 1.0f / 6.0f ), bottom - barHeight, false );

				TGUIObject *leftBar = unknown88->GetNthChild( 6 );
				TGUIObject *leftBar2 = unknown88->GetNthChild( 7 );
				leftBar->SetVisible( false );
				leftBar2->SetVisible( false );
				float lockBarHeight = cornerHeight * 0.5f;
				float lockBarWidth = cornerWidth * 0.75f;
				leftBar->Resize( lockBarWidth, lockBarHeight, false );
				leftBar2->Resize( lockBarWidth, lockBarHeight, false );
				float inset = barWidth * 1.5f;
				float leftX = inset + left;
				leftBar->SetPosition( leftX, unknown6C.height * 0.5f + top - lockBarHeight, false );
				leftBar2->SetPosition( leftX, unknown6C.height * 0.5f + top - 0.000833333f, false );

				TGUIObject *rightBar = unknown88->GetNthChild( 8 );
				TGUIObject *rightBar2 = unknown88->GetNthChild( 9 );
				rightBar->SetVisible( false );
				rightBar2->SetVisible( false );
				rightBar->Resize( lockBarWidth, lockBarHeight, false );
				rightBar2->Resize( lockBarWidth, lockBarHeight, false );
				float rightBarX = right - inset - lockBarWidth;
				rightBar->SetPosition( rightBarX, unknown6C.height * 0.5f + top - lockBarHeight, false );
				rightBar2->SetPosition( rightBarX, unknown6C.height * 0.5f + top - 0.000833333f, false );
			} else {
				unknown90 = false;
				for ( i = 4; i < 10; i++ ) {
					unknown88->GetNthChild( i )->SetNotVisible( false );
				}
			}
		}
		unknown88->Layout();
	}

	// the frame around the text
	name->RecalcBounds( true );
	unknown8C->SetPosition( name->GetLeft(), name->GetTop(), false );
	unknown8C->Resize( name->GetWidth(), name->GetHeight(), true );
}

/*
================
PlayerReticleWindow::VirtualSlot84

Passes the call on, and lays the name out again.
================
*/
// FUNCTION: STBC 0x00513020
void PlayerReticleWindow::VirtualSlot84( void ) {
	NamedReticleWindow::VirtualSlot84();
	TGParagraph *name = (TGParagraph *)unknown80->GetNthChild( 0 );
	name->RecalcBounds( false );
}

/*
================
PlayerReticleWindow::Unknown513050

Shows the marker on the player's target subsystem: the subsystem's point
on the target, projected through the icon manager's camera and corrected
for the view screen's zoom; hidden without a target subsystem.
================
*/
// FUNCTION: STBC 0x00513050
// DRAFT: call set agrees; retail loads the frustum bottom before copying the viewport (schedule), and its viewport copy and bottom swap ecx/edx
void PlayerReticleWindow::Unknown513050( ShipClass *player, ObjectClass *target ) {
	ShipSubsystem *subsystem = player->GetTargetSubsystem();
	if ( subsystem == NULL ) {
		subsystemMarker->SetNotVisible();
		return;
	}
	if ( !subsystemMarker->IsVisible() ) {
		subsystemMarker->SetVisible();
	}
	NiPoint3 world = TGModelUtils::LocalToWorldPoint( target->GetNode(), subsystem->GetDamagePoint() );
	NiCamera *camera = g_kIconManager.GetCamera();
	float x;
	float y;
	Unknown515BC0( camera, world, x, y );

	ReticleManagerWindow *manager = ReticleManagerWindow::GetReticleManagerWindow();
	NiRect<float> port = manager->GetViewport();
	float left = manager->GetFrustumLeft();
	float bottom = manager->GetFrustumBottom();
	float portWidth = port.right - port.left;
	float width = manager->GetFrustumRight() + portWidth + left;
	float portHeight = port.top - port.bottom;
	float height = manager->GetFrustumTop() + portHeight + bottom;
	if ( manager->IsZoomed() ) {
		x = ( ( ( x - 0.5f ) * ( height / width ) + 0.5f ) * width - left ) / portWidth;
		y = ( height * y - bottom ) / portHeight;
	}

	subsystemMarker->SizeToArtwork( false );
	float markerWidth = subsystemMarker->GetWidth();
	float markerHeight = subsystemMarker->GetHeight();
	subsystemMarker->Resize( markerWidth + markerWidth, markerHeight + markerHeight );
	subsystemMarker->SetPosition( x - markerWidth, 1.0f - y - markerHeight );
}

/*
================
PlayerReticleWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00513250
bool PlayerReticleWindow::WriteToStream( TGStream *stream ) {
	if ( !NamedReticleWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( unknown80 != NULL ? unknown80->GetObjID() : 0 );
	stream->WriteID( subsystemMarker != NULL ? subsystemMarker->GetObjID() : 0 );
	stream->WriteID( unknown88 != NULL ? unknown88->GetObjID() : 0 );
	stream->WriteID( unknown8C != NULL ? unknown8C->GetObjID() : 0 );
	stream->WriteBool( unknown90 );
	stream->WriteFloat( unknown94 );
	stream->WriteFloat( unknown98 );
	stream->WriteBool( unknown9C );
	return true;
}

/*
================
PlayerReticleWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x00513320
bool PlayerReticleWindow::ReadFromStream( TGStream *stream ) {
	if ( !NamedReticleWindow::ReadFromStream( stream ) ) {
		return false;
	}
	unknown80 = (TGPane *)stream->ReadID();
	subsystemMarker = (TGIcon *)stream->ReadID();
	unknown88 = (TGPane *)stream->ReadID();
	unknown8C = (TGIcon *)stream->ReadID();
	unknown90 = stream->ReadBool();
	unknown94 = stream->ReadFloat();
	unknown98 = stream->ReadFloat();
	unknown9C = stream->ReadBool();
	return true;
}

/*
================
PlayerReticleWindow::FixUpPointers
================
*/
// FUNCTION: STBC 0x005133c0
bool PlayerReticleWindow::FixUpPointers( void ) {
	if ( !NamedReticleWindow::FixUpPointers() ) {
		return false;
	}
	if ( unknown80 != NULL && GetTGObjectPtr( (long)unknown80 ) == NULL ) {
		return false;
	}
	if ( subsystemMarker != NULL && GetTGObjectPtr( (long)subsystemMarker ) == NULL ) {
		return false;
	}
	if ( unknown88 != NULL && GetTGObjectPtr( (long)unknown88 ) == NULL ) {
		return false;
	}
	if ( unknown8C != NULL && GetTGObjectPtr( (long)unknown8C ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
PlayerReticleWindow::PostFixUp
================
*/
// FUNCTION: STBC 0x00513440
void PlayerReticleWindow::PostFixUp( void ) {
	NamedReticleWindow::PostFixUp();
	unknown80 = ( unknown80 != NULL ) ? (TGPane *)GetTGObjectPtr( (long)unknown80 ) : NULL;
	subsystemMarker = ( subsystemMarker != NULL ) ? (TGIcon *)GetTGObjectPtr( (long)subsystemMarker ) : NULL;
	unknown88 = ( unknown88 != NULL ) ? (TGPane *)GetTGObjectPtr( (long)unknown88 ) : NULL;
	unknown8C = ( unknown8C != NULL ) ? (TGIcon *)GetTGObjectPtr( (long)unknown8C ) : NULL;
}
