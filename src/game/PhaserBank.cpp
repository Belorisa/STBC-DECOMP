/*
===============================================================================

	PhaserBank.cpp

===============================================================================
*/

#include <math.h>

#include "PhaserBank.h"
#include "Game.h"
#include "PhaserSystem.h"
#include "UtopiaModule.h"
#include "PhaserProperty.h"
#include "PoweredSubsystem.h"
#include "PowerSubsystem.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGModelUtils.h"
#include "TGStream.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "DamageableObject.h"
#include "NiMath.h"
#include "Planet.h"
#include "ProximityManager.h"
#include "SetClass.h"
#include "TGEventManager.h"
#include "TGObjPtrEvent.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "UtopiaApp.h"
#include "WeaponSystem.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005709d0 _$E62
// FUNCTION: STBC 0x005709e0 _$E61
// FUNCTION: STBC 0x005709f0 _$E281
// FUNCTION: STBC 0x00570a00 _$E280
// FUNCTION: STBC 0x00570a10 _$E73
// FUNCTION: STBC 0x00570a20 _$E72
// FUNCTION: STBC 0x00570a30 _$E77
// FUNCTION: STBC 0x00570a40 _$E76
// FUNCTION: STBC 0x00570a50 _$E81
// FUNCTION: STBC 0x00570a60 _$E80
// FUNCTION: STBC 0x00570a70 _$E85
// FUNCTION: STBC 0x00570a80 _$E84
// FUNCTION: STBC 0x00570a90 _$E204
// FUNCTION: STBC 0x00570aa0 _$E203
// FUNCTION: STBC 0x00570ab0 _$E146
// FUNCTION: STBC 0x00570ac0 _$E145
// FUNCTION: STBC 0x00570c10 _$E159
// FUNCTION: STBC 0x00570c20 _$E158
// FUNCTION: STBC 0x00570fc0 _$E209
// FUNCTION: STBC 0x00570fd0 _$E208
// FUNCTION: STBC 0x005719e0 _$E286
// FUNCTION: STBC 0x005719f0 _$E285
// FUNCTION: STBC 0x00571e40 _$E213
// FUNCTION: STBC 0x00571e50 _$E212
// FUNCTION: STBC 0x00572420 _$E290
// FUNCTION: STBC 0x00572430 _$E289
// FUNCTION: STBC 0x00572930 _$E297
// FUNCTION: STBC 0x00572940 _$E296
// FUNCTION: STBC 0x005731e0 _$E344
// FUNCTION: STBC 0x005731f0 _$E343
// FUNCTION: STBC 0x005732d0 _$E348
// FUNCTION: STBC 0x005732e0 _$E347
// FUNCTION: STBC 0x005733d0 _$E104
// FUNCTION: STBC 0x005733e0 _$E103
// FUNCTION: STBC 0x005734c0 _$E221
// FUNCTION: STBC 0x005734d0 _$E220
// FUNCTION: STBC 0x005734e0 _$E228
// FUNCTION: STBC 0x005734f0 _$E227
// FUNCTION: STBC 0x00573500 _$E232
// FUNCTION: STBC 0x00573510 _$E231
// FUNCTION: STBC 0x00573520 _$E236
// FUNCTION: STBC 0x00573530 _$E235
// FUNCTION: STBC 0x00573540 _$E240
// FUNCTION: STBC 0x00573550 _$E239
// FUNCTION: STBC 0x00573560 _$E246
// FUNCTION: STBC 0x00573570 _$E245
// FUNCTION: STBC 0x00573580 _$E352
// FUNCTION: STBC 0x00573590 _$E351
// FUNCTION: STBC 0x005735a0 _$E356
// FUNCTION: STBC 0x005735b0 _$E355
// FUNCTION: STBC 0x005735c0 _$E360
// FUNCTION: STBC 0x005735d0 _$E359
// FUNCTION: STBC 0x005735e0 _$E364
// FUNCTION: STBC 0x005735f0 _$E363
// FUNCTION: STBC 0x00573600 _$E368
// FUNCTION: STBC 0x00573610 _$E367
// FUNCTION: STBC 0x00573620 _$E372
// FUNCTION: STBC 0x00573630 _$E371
// FUNCTION: STBC 0x00573640 _$E250
// FUNCTION: STBC 0x00573650 _$E249
// FUNCTION: STBC 0x00573660 _$E376
// FUNCTION: STBC 0x00573670 _$E375
// FUNCTION: STBC 0x00573680 _$E380
// FUNCTION: STBC 0x00573690 _$E379
// FUNCTION: STBC 0x005736a0 _$E384
// FUNCTION: STBC 0x005736b0 _$E383
// FUNCTION: STBC 0x005736c0 _$E109
// FUNCTION: STBC 0x005736d0 _$E108
// FUNCTION: STBC 0x005736e0 _$E388
// FUNCTION: STBC 0x005736f0 _$E387
// FUNCTION: STBC 0x00573700 _$E114
// FUNCTION: STBC 0x00573710 _$E113
// FUNCTION: STBC 0x00573720 _$E392
// FUNCTION: STBC 0x00573730 _$E391
// FUNCTION: STBC 0x00573740 _$E396
// FUNCTION: STBC 0x00573750 _$E395
// FUNCTION: STBC 0x00573760 _$E400
// FUNCTION: STBC 0x00573770 _$E399
// FUNCTION: STBC 0x00573780 _$E404
// FUNCTION: STBC 0x00573790 _$E403
// FUNCTION: STBC 0x005737a0 _$E123
// FUNCTION: STBC 0x005737b0 _$E122
// FUNCTION: STBC 0x005737c0 _$E408
// FUNCTION: STBC 0x005737d0 _$E407
// FUNCTION: STBC 0x005737e0 _$E412
// FUNCTION: STBC 0x005737f0 _$E411
// FUNCTION: STBC 0x00573800 _$E416
// FUNCTION: STBC 0x00573810 _$E415
// FUNCTION: STBC 0x00573820 _$E129
// FUNCTION: STBC 0x00573830 _$E128
// FUNCTION: STBC 0x00573840 _$E134
// FUNCTION: STBC 0x00573850 _$E133
// FUNCTION: STBC 0x00573860 _$E164
// FUNCTION: STBC 0x00573870 _$E163
// FUNCTION: STBC 0x00573880 _$E420
// FUNCTION: STBC 0x00573890 _$E419
// FUNCTION: STBC 0x005738a0 _$E169
// FUNCTION: STBC 0x005738b0 _$E168
// FUNCTION: STBC 0x005738c0 _$E424
// FUNCTION: STBC 0x005738d0 _$E423
// FUNCTION: STBC 0x005738e0 _$E428
// FUNCTION: STBC 0x005738f0 _$E427
// FUNCTION: STBC 0x00573900 _$E174
// FUNCTION: STBC 0x00573910 _$E173
// FUNCTION: STBC 0x00573920 _$E432
// FUNCTION: STBC 0x00573930 _$E431
// FUNCTION: STBC 0x00573940 _$E139
// FUNCTION: STBC 0x00573950 _$E138
// FUNCTION: STBC 0x00573960 _$E436
// FUNCTION: STBC 0x00573970 _$E435
// FUNCTION: STBC 0x00573980 _$E440
// FUNCTION: STBC 0x00573990 _$E439
// FUNCTION: STBC 0x005739a0 _$E179
// FUNCTION: STBC 0x005739b0 _$E178
// FUNCTION: STBC 0x005739c0 _$E184
// FUNCTION: STBC 0x005739d0 _$E183
// FUNCTION: STBC 0x005739e0 _$E444
// FUNCTION: STBC 0x005739f0 _$E443
// FUNCTION: STBC 0x00573a00 _$E448
// FUNCTION: STBC 0x00573a10 _$E447
// FUNCTION: STBC 0x00573a20 _$E452
// FUNCTION: STBC 0x00573a30 _$E451
// FUNCTION: STBC 0x00573a40 _$E456
// FUNCTION: STBC 0x00573a50 _$E455
// FUNCTION: STBC 0x00573a60 _$E460
// FUNCTION: STBC 0x00573a70 _$E459
// FUNCTION: STBC 0x00573a80 _$E464
// FUNCTION: STBC 0x00573a90 _$E463
// FUNCTION: STBC 0x00573aa0 _$E468
// FUNCTION: STBC 0x00573ab0 _$E467
// FUNCTION: STBC 0x00573ac0 _$E472
// FUNCTION: STBC 0x00573ad0 _$E471
// FUNCTION: STBC 0x00573ae0 _$E189
// FUNCTION: STBC 0x00573af0 _$E188
// FUNCTION: STBC 0x00573b00 _$E476
// FUNCTION: STBC 0x00573b10 _$E475
// FUNCTION: STBC 0x00573b20 _$E194
// FUNCTION: STBC 0x00573b30 _$E193
// FUNCTION: STBC 0x00573b40 _$E199
// FUNCTION: STBC 0x00573b50 _$E198
// FUNCTION: STBC 0x00573bb0 _$E480
// FUNCTION: STBC 0x00573bc0 _$E479
// FUNCTION: STBC 0x00573bd0 _$E485
// FUNCTION: STBC 0x00573be0 _$E484
// FUNCTION: STBC 0x00573bf0 _$E489
// FUNCTION: STBC 0x00573c00 _$E488

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00570950 _$E328
// FUNCTION: STBC 0x00570960 _$E325
// FUNCTION: STBC 0x00570970 _$E327
// FUNCTION: STBC 0x00570980 _$E326
// FUNCTION: STBC 0x00570990 _$E334
// FUNCTION: STBC 0x005709a0 _$E331
// FUNCTION: STBC 0x005709b0 _$E333
// FUNCTION: STBC 0x005709c0 _$E332

// DATA: STBC 0x008e53dc
float PhaserBank::maxPhaserRange = 700.0f;

// DATA: STBC 0x00893188
const float PhaserBank::DAMAGE_RADIUS_SCALE_LOW = 0.5f;

// DATA: STBC 0x0089318c
const float PhaserBank::DAMAGE_RADIUS_SCALE_MEDIUM = 1.0f;

// DATA: STBC 0x00893190
const float PhaserBank::DAMAGE_RADIUS_SCALE_HIGH = 1.0f;

/*
================
PhaserBank::Cast

The object as a PhaserBank; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00570b20
PhaserBank *PhaserBank::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PHASERBANK_TYPE ) ) {
		return (PhaserBank *)object;
	}
	return NULL;
}

/*
================
PhaserBank::GetProperty
================
*/
// FUNCTION: STBC 0x00570b50
PhaserProperty *PhaserBank::GetProperty( void ) {
	return (PhaserProperty *)property;
}

/*
================
PhaserBank::GetOrientationForward

The direction the bank faces, in the ship's frame.
================
*/
// FUNCTION: STBC 0x00570b60
TGPoint3 PhaserBank::GetOrientationForward( void ) {
	return ( (PhaserProperty *)property )->GetOrientationForward();
}

/*
================
PhaserBank::GetOrientationUp
================
*/
// FUNCTION: STBC 0x00570b80
TGPoint3 PhaserBank::GetOrientationUp( void ) {
	return ( (PhaserProperty *)property )->GetOrientationUp();
}

/*
================
PhaserBank::GetOrientationRight

Forward crossed with up.
================
*/
// FUNCTION: STBC 0x00570ba0
TGPoint3 PhaserBank::GetOrientationRight( void ) {
	return GetOrientationForward().Cross( GetOrientationUp() );
}

/*
================
PhaserBank::Unknown570C30

Copies the forward and up directions out. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00570c30
void PhaserBank::Unknown570C30( NiPoint3 *forward, NiPoint3 *up ) {
	*forward = GetOrientationForward();
	*up = GetOrientationUp();
}

/*
================
PhaserBank::GetWidth
================
*/
// FUNCTION: STBC 0x00570c80
float PhaserBank::GetWidth( void ) {
	return GetProperty()->GetWidth();
}

/*
================
PhaserBank::GetLength
================
*/
// FUNCTION: STBC 0x00570c90
float PhaserBank::GetLength( void ) {
	return GetProperty()->GetLength();
}

/*
================
PhaserBank::GetPhaserTextureStart
================
*/
// FUNCTION: STBC 0x00570ca0
int PhaserBank::GetPhaserTextureStart( void ) {
	return GetProperty()->GetPhaserTextureStart();
}

/*
================
PhaserBank::GetPhaserTextureEnd
================
*/
// FUNCTION: STBC 0x00570cb0
int PhaserBank::GetPhaserTextureEnd( void ) {
	return GetProperty()->GetPhaserTextureEnd();
}

/*
================
PhaserBank::GetPhaserWidth
================
*/
// FUNCTION: STBC 0x00570cc0
float PhaserBank::GetPhaserWidth( void ) {
	return GetProperty()->GetPhaserWidth();
}

/*
================
PhaserBank::GetArcWidthAngles
================
*/
// FUNCTION: STBC 0x00570cd0
void PhaserBank::GetArcWidthAngles( float *min, float *max ) {
	*min = GetProperty()->GetArcWidthAngleMin();
	*max = GetProperty()->GetArcWidthAngleMax();
}

/*
================
PhaserBank::GetArcHeightAngles
================
*/
// FUNCTION: STBC 0x00570d00
void PhaserBank::GetArcHeightAngles( float *min, float *max ) {
	*min = GetProperty()->GetArcHeightAngleMin();
	*max = GetProperty()->GetArcHeightAngleMax();
}

/*
================
PhaserBank::GetArcWidthAngleMin
================
*/
// FUNCTION: STBC 0x00570d30
float PhaserBank::GetArcWidthAngleMin( void ) {
	return GetProperty()->GetArcWidthAngleMin();
}

/*
================
PhaserBank::GetArcWidthAngleMax
================
*/
// FUNCTION: STBC 0x00570d40
float PhaserBank::GetArcWidthAngleMax( void ) {
	return GetProperty()->GetArcWidthAngleMax();
}

/*
================
PhaserBank::GetArcHeightAngleMin
================
*/
// FUNCTION: STBC 0x00570d50
float PhaserBank::GetArcHeightAngleMin( void ) {
	return GetProperty()->GetArcHeightAngleMin();
}

/*
================
PhaserBank::GetArcHeightAngleMax
================
*/
// FUNCTION: STBC 0x00570d60
float PhaserBank::GetArcHeightAngleMax( void ) {
	return GetProperty()->GetArcHeightAngleMax();
}

/*
================
PhaserBank::PhaserBank
================
*/
// FUNCTION: STBC 0x00570d70
PhaserBank::PhaserBank( long id ) : EnergyWeapon( id ) {
}

/*
================
PhaserBank::GetObjType
================
*/
// FUNCTION: STBC 0x00570dd0
int PhaserBank::GetObjType( void ) {
	return PHASERBANK_TYPE;
}

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00570ad0 _$E509
// FUNCTION: STBC 0x00570ae0 _$E505
// FUNCTION: STBC 0x00570af0 _$E508
// FUNCTION: STBC 0x00570b00 _$E506
// DATA: STBC 0x0098c728
TGEventHandlerTable PhaserBank::classHandlers;

/*
================
PhaserBank::GetClassName
================
*/
// FUNCTION: STBC 0x00570e40
char *PhaserBank::GetClassName( void ) {
	return "PhaserBank";
}

/*
================
PhaserBank::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00570e50
char *PhaserBank::GetSwigTypeName( void ) {
	return "_p_PhaserBank";
}

/*
================
PhaserBank::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00570e60
char *PhaserBank::GetPythonClassName( void ) {
	return "PhaserBankPtr";
}

/*
================
PhaserBank::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00570e70
TGEventHandlerTable *PhaserBank::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PhaserBank::GetPosition
================
*/
// FUNCTION: STBC 0x00570e80
NiPoint3 PhaserBank::GetPosition( void ) {
	return position;
}

/*
================
PhaserBank::~PhaserBank
================
*/
// FUNCTION: STBC 0x00570ee0
// FUNCTION: STBC 0x00570eb0 ??_GPhaserBank@@UAEPAXI@Z
PhaserBank::~PhaserBank( void ) {
}

/*
================
PhaserBank::SetProperty

The payload takes the weapon's damage radius factor.
================
*/
// FUNCTION: STBC 0x00570f40
void PhaserBank::SetProperty( SubsystemProperty *property_ ) {
	EnergyWeapon::SetProperty( property_ );
	payload.SetDamageRadiusFactor( GetDamageRadiusFactor() );
}

/*
================
PhaserBank::SetPowerSetting

The payload's damage radius factor follows the power setting.
================
*/
// FUNCTION: STBC 0x00570f60
void PhaserBank::SetPowerSetting( int powerSetting_ ) {
	powerSetting = powerSetting_;

	float scale = 1.0f;
	switch ( powerSetting_ ) {
	case EW_LOW:
		scale = DAMAGE_RADIUS_SCALE_LOW;
		break;
	case EW_MEDIUM:
		scale = DAMAGE_RADIUS_SCALE_MEDIUM;
		break;
	case EW_HIGH:
		scale = DAMAGE_RADIUS_SCALE_HIGH;
		break;
	}
	payload.SetDamageRadiusFactor( GetDamageRadiusFactor() * scale );
}

// A half turn: the width angle on the far side of the bank (the unit's own constant).
// DATA: STBC 0x0089316c
static const float PI = 3.1415927f;

// The fire sounds' priority, and the player's ship's (invented names).
#define FIRE_SOUND_PRIORITY			0.6f
#define PLAYER_FIRE_SOUND_PRIORITY	0.9f
// The start sound plays just below the loop (invented name).
#define START_SOUND_PRIORITY_DROP	0.01f
// Seconds the beam charges, and grows to its full length (invented names).
#define BEAM_CHARGE_TIME			0.35f
#define BEAM_GROW_TIME				0.25f

// DATA: STBC 0x008e53e0
float PhaserBank::damageInterval = 0.5f;


/*
================
PhaserBank::Fire

Fires at the target the weapon system aims at: the bank stops when it
can't fire, when there is no target position, when the ship's sensors
don't see the target, or when no point of the bank's arc reaches it.
Returns whether the bank is firing.
================
*/
// FUNCTION: STBC 0x00570fe0
bool PhaserBank::Fire( float time, bool playSound ) {
	if ( !CanFire() ) {
		StopFiring();
		return false;
	}

	ShipClass *ship = parentSubsystem->GetParentShip();
	ObjectClass *targetObject = NULL;
	NiPoint3 targetPosition;
	// The named system is the likeliest spelling, not a certain one. The bytes
	// fix only how many locals the function has (the operand order of the world
	// start's x sum follows their count mod 8; one to six more than without it
	// all match), not which one. This one follows the codebase's own
	// '<Type> *system = (<Type> *)parentSubsystem;' idiom
	// (TractorBeamProjector::UpdateTarget); the demo build's copy is identical
	// and the sibling weapons' Fire are neutral to it (user's ruling, 2026-11-18).
	WeaponSystem *system = (WeaponSystem *)parentSubsystem;
	if ( !system->GetTargetPosition( &target, &targetPosition, &targetObject ) ) {
		StopFiring();
		return false;
	}
	if ( !WeaponSystem::IsVisibleTarget( targetObject ) ) {
		StopFiring();
		return false;
	}

	NiNode *node = ship->GetNode();
	NiPoint3 localTarget = TGModelUtils::WorldToLocalPoint( node, targetPosition );
	NiPoint3 position;
	float widthAngle;
	if ( FindFiringPosition( &localTarget, &position, &widthAngle ) ) {
		ShipClass *owner = parentShip;
		NiPoint3 start = owner->GetWorldRotation() * position + owner->GetWorldLocation();
		UpdateBeam( &start, &targetPosition, time, playSound, widthAngle );
	} else {
		StopFiring();
	}
	return firing;
}

/*
================
PhaserBank::StopFiring

Ends the beam: the graphic leaves the scene with its lights, the fire
sounds stop, the ship hears that the bank stopped firing, and a target
still hit takes the last damage and hears that the beam left it.
================
*/
// FUNCTION: STBC 0x00571200
void PhaserBank::StopFiring( void ) {
	if ( firing ) {
		firing = false;
		NiNode *parent = payload.graphic->GetParent();
		if ( parent != NULL ) {
			parent->DetachChild( payload.graphic );
		}

		if ( payload.state == 1 ) {
			payload.graphic->RemoveLights();
		} else if ( payload.state == 2 || payload.state == 3 ) {
			payload.graphic->RemoveStartLight();
		}
		payload.state = 0;

		if ( unknownA4 != unknown896A9C ) {
			g_kSoundManager.StopSound( unknownA4 );
			unknownA4 = unknown896A9C;
		}
		if ( unknownA8 != unknown896A9C ) {
			g_kSoundManager.StopSound( unknownA8 );
			unknownA8 = unknown896A9C;
		}

		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetEventType( ET_PHASER_STOPPED_FIRING );
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( event );

		if ( payload.targetID != 0 ) {
			DamageableObject *hit = DamageableObject::GetObjectByID( parentShip->GetContainingSet(), payload.targetID );
			if ( hit != NULL ) {
				ApplyDamage();
				hit->RemoveLight( payload.graphic->GetEndLight() );
				TGEvent *stopped = new TGEvent;
				stopped->SetSource( this );
				stopped->SetDestination( hit );
				stopped->SetEventType( ET_PHASER_STOPPED_HITTING );
				g_kEventManager.AddEvent( stopped );
			}
		}
		payload.targetID = 0;
	}
	Weapon::StopFiring();
}

/*
================
PhaserBank::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005713f0
void PhaserBank::RegisterHandlerNames( void ) {
}

/*
================
PhaserBank::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00571400
void PhaserBank::RegisterHandlers( void ) {
}

/*
================
PhaserBank::CanHit

Whether the bank can fire at the world point.
================
*/
// FUNCTION: STBC 0x00571410
bool PhaserBank::CanHit( TGPoint3 *point ) {
	NiNode *node = parentShip->GetNode();
	NiPoint3 localPoint = ( ( *point - node->GetWorldTranslate() ) * node->GetWorldRotate() ) * ( 1.0f / node->GetWorldScale() );
	NiPoint3 position;
	float widthAngle;
	return FindFiringPosition( &localPoint, &position, &widthAngle );
}

/*
================
PhaserBank::FindFiringPosition

The width angle of the target seen from the bank, in the plane of the
bank's forward and right axes; when the point of the arc there can't
fire, the angle half a turn round is tried. A firing point needs the
target inside the arcs and in reach, and no planet of the set in the way.
================
*/
// FUNCTION: STBC 0x005714a0
bool PhaserBank::FindFiringPosition( const NiPoint3 *target, NiPoint3 *position, float *widthAngle ) {
	NiPoint3 offset = *target - ShipSubsystem::GetPosition();
	float angle;
	float otherAngle;
	// the bank's width angle to the target in its own plane, and the
	// angle of the far side
	{
		NiPoint3 up = GetOrientationUp();
		NiPoint3 forward = GetOrientationForward();
		NiPoint3 flat = offset - up * offset.Dot( up );
		flat.Unitize();

		angle = NiACos( forward.Dot( flat ) );
		otherAngle = angle - PI;
		if ( flat.Dot( GetOrientationRight() ) < 0.0f ) {
			angle = -angle;
			otherAngle = PI + angle;
		}
	}

	float heightAngle;
	GetArcPosition( angle, &offset, position, &heightAngle );
	if ( IsInArcAngles( angle, heightAngle ) ) {
		if ( CanReach( target, position ) ) {
			*widthAngle = angle;
			ShipClass *ship = parentShip;
			if ( ship != NULL && ship->GetContainingSet() != NULL ) {
				int count = 0;
				Planet **planets = Planet::GetClassObjects( ship->GetContainingSet(), &count );
				if ( count > 0 ) {
					NiPoint3 start = TGModelUtils::LocalToWorldPoint( ship->GetNode(), ShipSubsystem::GetPosition() );
					NiPoint3 end = TGModelUtils::LocalToWorldPoint( ship->GetNode(), *target );
					for ( int i = 0; i < count; i++ ) {
						NiBound &bound = planets[i]->GetWorldBound();
						NiPoint3 hit1;
						NiPoint3 hit2;
						if ( Unknown4570D0( start, end - start, bound.GetCenter(), bound.GetRadius(), &hit1, &hit2 ) > 0 ) {
							return false;
						}
					}
				}
			}
			return true;
		}
	} else {
		GetArcPosition( otherAngle, &offset, position, &heightAngle );
		if ( IsInArcAngles( otherAngle, heightAngle ) && CanReach( target, position ) ) {
			*widthAngle = otherAngle;
			ShipClass *ship = parentShip;
			if ( ship != NULL && ship->GetContainingSet() != NULL ) {
				int count = 0;
				Planet **planets = Planet::GetClassObjects( ship->GetContainingSet(), &count );
				if ( count > 0 ) {
					NiPoint3 end = TGModelUtils::LocalToWorldPoint( ship->GetNode(), *target );
					NiPoint3 start = TGModelUtils::LocalToWorldPoint( ship->GetNode(), ShipSubsystem::GetPosition() );
					for ( int i = 0; i < count; i++ ) {
						NiBound &bound = planets[i]->GetWorldBound();
						NiPoint3 hit1;
						NiPoint3 hit2;
						if ( Unknown4570D0( start, end - start, bound.GetCenter(), bound.GetRadius(), &hit1, &hit2 ) > 0 ) {
							return false;
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

/*
================
PhaserBank::CanReach

A bank that isn't firing reaches only as far as the maximum phaser range.
The ship's power output has to cover what the bank discharges.
================
*/
// FUNCTION: STBC 0x00571a00
bool PhaserBank::CanReach( const NiPoint3 *from, const NiPoint3 *to ) {
	NiPoint3 offset;
	offset = *from - *to;
	if ( !firing ) {
		if ( offset.SqrLength() > maxPhaserRange * maxPhaserRange ) {
			return false;
		}
	}

	if ( parentShip == NULL ) {
		return false;
	}
	PowerSubsystem *power = parentShip->GetPowerSubsystem();
	if ( power == NULL ) {
		return false;
	}
	float output = power->GetPowerOutput();
	if ( GetDischargeFactor() > output ) {
		return false;
	}
	return true;
}

/*
================
PhaserBank::GetArcPosition

The point of the arc at the width angle, from the bank's position; the
height angle is the target's angle above the arc's plane there, negative
below it.
================
*/
// FUNCTION: STBC 0x00571ab0
void PhaserBank::GetArcPosition( float widthAngle, const NiPoint3 *target, NiPoint3 *position, float *heightAngle ) {
	NiPoint3 offset = ( cosf( widthAngle ) * GetLength() ) * GetOrientationForward() + ( sinf( widthAngle ) * GetWidth() ) * GetOrientationRight();
	NiPoint3 direction = offset;
	direction.Unitize();

	NiPoint3 normal = direction.Cross( GetOrientationUp() );
	NiPoint3 toTarget = *target - offset;
	NiPoint3 flat = toTarget - normal * toTarget.Dot( normal );
	flat.Unitize();

	*heightAngle = NiACos( direction.Dot( flat ) );
	if ( flat.Dot( GetOrientationUp() ) < 0.0f ) {
		*heightAngle = -*heightAngle;
	}
	*position = offset + ShipSubsystem::GetPosition();
}

/*
================
PhaserBank::CanFire

Not while the ship is cloaked. A firing bank needs some charge left; one
that is not firing needs the minimum firing charge. The bank must not be
disabled.
================
*/
// FUNCTION: STBC 0x00571e60
bool PhaserBank::CanFire( void ) {
	if ( parentShip != NULL && parentShip->IsCloaked() == true ) {
		return false;
	}

	if ( firing ) {
		if ( chargeLevel <= 0.0f ) {
			return false;
		}
	} else if ( GetProperty()->GetMinFiringCharge() > chargeLevel ) {
		return false;
	}

	if ( parentSubsystem->GetConditionPercentage() * conditionPercentage < GetDisabledPercentage() ) {
		return false;
	}
	return true;
}

/*
================
PhaserBank::IsInArcAngles

Both angles between the arc's limits.
================
*/
// FUNCTION: STBC 0x00571ee0
bool PhaserBank::IsInArcAngles( float width, float height ) {
	if ( GetArcWidthAngleMin() <= width && GetArcWidthAngleMax() > width &&
		GetArcHeightAngleMin() <= height && GetArcHeightAngleMax() > height ) {
		return true;
	}
	return false;
}

/*
================
PhaserBank::UpdateBeam

A bank that isn't firing creates the beam, plays the start and loop
sounds from the ship when asked (louder for the player's ship), and tells
the ship it started firing and fired; a firing bank aims the beam anew.
Then the beam steps: charging until the lights meet at the width angle,
growing until it is shown whole, then holding on what it hits.
================
*/
// FUNCTION: STBC 0x00571f40
void PhaserBank::UpdateBeam( const NiPoint3 *start, const NiPoint3 *target, float time, bool playSound, float widthAngle ) {
	if ( !firing ) {
		CreateBeam( start, target, time, widthAngle );
		if ( playSound ) {
			ShipClass *ship = parentShip;
			SetClass *set = ship->GetContainingSet();
			if ( set != NULL ) {
				TGSound *startSound = g_kSoundManager.GetSound( GetFireStartSound() );
				TGSound *loopSound = g_kSoundManager.GetSound( GetFireLoopSound() );
				if ( startSound != NULL && loopSound != NULL ) {
					startSound->AttachToNode( parentShip->GetNode() );
					loopSound->AttachToNode( parentShip->GetNode() );
					loopSound->SetLooping( true );
					TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
					if ( region != NULL ) {
						region->AddSound( startSound );
						region->AddSound( loopSound );
					}
					float priority = FIRE_SOUND_PRIORITY;
					if ( ship == Game::GetCurrentPlayer() ) {
						priority = PLAYER_FIRE_SOUND_PRIORITY;
					}
					startSound->SetPriority( priority - START_SOUND_PRIORITY_DROP );
					loopSound->SetPriority( priority );
					unknownA4 = startSound->Play( true );
					unknownA8 = loopSound->Play( true );
				}
			}
		}

		TGObjPtrEvent *started = new TGObjPtrEvent;
		started->SetEventType( ET_PHASER_STARTED_FIRING );
		started->SetSource( this );
		started->SetDestination( parentShip );
		started->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( started );

		TGObjPtrEvent *fired = new TGObjPtrEvent;
		fired->SetSource( this );
		fired->SetDestination( parentShip );
		fired->SetEventType( ET_WEAPON_FIRED );
		fired->SetObjPtr( GetTGObjectPtr( GetTargetID() ) );
		g_kEventManager.AddEvent( fired );
	} else {
		AimBeam( start, target, time );
	}

	switch ( payload.state ) {
	case 1:
		if ( payload.graphic->Unknown4E7200( UtopiaApp::GetApp()->GetGameTime(), widthAngle ) ) {
			payload.graphic->RemoveLights();
			payload.graphic->Start( UtopiaApp::GetApp()->GetGameTime(), BEAM_GROW_TIME );
			payload.state = 2;
			parentShip->GetContainingSet()->GetEffectRoot()->AttachChild( payload.graphic, true );
			payload.graphic->Update( payload.start, payload.direction, payload.length, UtopiaApp::GetApp()->GetGameTime() );
		}
		break;
	case 2:
		RetargetBeam( 0.0f );
		payload.graphic->Update( payload.start, payload.direction, payload.length, UtopiaApp::GetApp()->GetGameTime() );
		if ( payload.graphic->IsFinished() == true ) {
			payload.state = 3;
			ApplyDamage();
		}
		break;
	case 3:
		RetargetBeam( time );
		payload.graphic->Update( payload.start, payload.direction, payload.length, UtopiaApp::GetApp()->GetGameTime() );
		break;
	}
}

/*
================
PhaserBank::ApplyDamage

The beam's target takes the damage of the time on target at the beam's
length, along the beam from its start to its end. Damage left over goes on
along the beam a little further, through the shields.
================
*/
// FUNCTION: STBC 0x005722b0
void PhaserBank::ApplyDamage( void ) {
	if ( payload.targetID != 0 ) {
		DamageableObject *object = DamageableObject::GetObjectByID( NULL, payload.targetID );
		float damage = CalculateDamage( payload.timeOnTarget, payload.length );
		NiPoint3 end = payload.start + payload.direction * payload.length;
		object->VirtualSlot81( &payload.start, &end, &payload.hitPoint, &payload.hitNormal, &damage, &payload, this, parentShip );
		if ( damage > 0.0f ) {
			end += payload.direction * 10.0f;
			if ( object->VirtualSlot80( &payload.start, &end, &payload.hitPoint, &payload.hitNormal, true ) ) {
				// BUG: the hit point is passed as the hit normal too
				object->VirtualSlot81( &payload.start, &end, &payload.hitPoint, &payload.hitPoint, &damage, &payload, this, parentShip );
			}
		}
		payload.timeOnTarget = 0.0f;
	}
}

/*
================
PhaserBank::RetargetBeam

The nearest damageable object other than the ship that the beam's line
hits becomes the target, with the hit point and normal. A new target
gets a new payload ID and the end light, and hears that the beam started
hitting it; the old one takes its last damage and hears that the beam
stopped. The beam ends at the hit, and the time on target grows: past
damageInterval the damage is applied.
================
*/
// FUNCTION: STBC 0x00572440
void PhaserBank::RetargetBeam( float time ) {
	if ( parentShip == NULL ) {
		return;
	}
	SetClass *set = parentShip->GetContainingSet();
	if ( set == NULL ) {
		return;
	}

	ProximityManager *proximity = set->GetProximityManager();
	NiPoint3 end = payload.start + payload.direction * payload.length;
	ProximityManager::ObjectIterator *iterator = proximity->GetLineIntersectObjects( payload.start, end, 0.0f, false );
	float nearest = NI_INFINITY;
	DamageableObject *nearestObject = NULL;
	NiPoint3 nearestPoint;
	NiPoint3 nearestNormal;
	ObjectClass *object;
	while ( ( object = proximity->GetNextObject( iterator ) ) != NULL ) {
		if ( object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) && object != parentShip ) {
			DamageableObject *damageable = (DamageableObject *)object;
			NiPoint3 hitPoint;
			NiPoint3 hitNormal;
			if ( damageable->VirtualSlot80( &payload.start, &end, &hitPoint, &hitNormal, false ) ) {
				float distance = ( hitPoint - payload.start ).SqrLength();
				if ( distance < nearest ) {
					nearestPoint = hitPoint;
					nearestObject = damageable;
					nearest = distance;
					nearestNormal = hitNormal;
				}
			}
		}
	}
	proximity->EndObjectIteration( iterator );

	if ( nearestObject != NULL ) {
		if ( nearestObject->GetObjID() != payload.targetID ) {
			DamageableObject *old = DamageableObject::GetObjectByID( set, payload.targetID );
			if ( old != NULL ) {
				old->RemoveLight( payload.graphic->GetEndLight() );
				ApplyDamage();
				TGEvent *stopped = new TGEvent;
				stopped->SetSource( this );
				stopped->SetDestination( old );
				stopped->SetEventType( ET_PHASER_STOPPED_HITTING );
				g_kEventManager.AddEvent( stopped );
			}
			payload.targetID = nearestObject->GetObjID();
			payload.NextID();
			payload.timeOnTarget = 0.0f;
			nearestObject->AddLight( payload.graphic->GetEndLight() );
			TGEvent *started = new TGEvent;
			started->SetSource( this );
			started->SetDestination( nearestObject );
			started->SetEventType( ET_PHASER_STARTED_HITTING );
			g_kEventManager.AddEvent( started );
		}

		payload.hitPoint = nearestPoint;
		payload.hitNormal = nearestNormal;
		payload.timeOnTarget += time;
		float length = sqrtf( nearest );
		payload.length = length;
		if ( payload.timeOnTarget > damageInterval ) {
			ApplyDamage();
			payload.length = length;
		}
	} else {
		if ( payload.targetID != 0 ) {
			ApplyDamage();
			DamageableObject *old = DamageableObject::GetObjectByID( set, payload.targetID );
			if ( old != NULL ) {
				old->RemoveLight( payload.graphic->GetEndLight() );
				TGEvent *stopped = new TGEvent;
				stopped->SetSource( this );
				stopped->SetDestination( old );
				stopped->SetEventType( ET_PHASER_STOPPED_HITTING );
				g_kEventManager.AddEvent( stopped );
			}
		}
		payload.targetID = 0;
	}
}

/*
================
PhaserBank::CreateBeam

Creates the graphic the first time, puts it on the ship and starts the
charge-up; the beam starts with no length and no target.
================
*/
// FUNCTION: STBC 0x00572800
void PhaserBank::CreateBeam( const NiPoint3 *start, const NiPoint3 *target, float time, float ) {
	ShipClass *ship = parentShip;
	if ( payload.graphic == NULL ) {
		payload.graphic = new PhaserGraphic( GetProperty(), ship );
	}
	ship->GetNode()->AttachChild( payload.graphic, true );
	payload.graphic->UpdateProperties();
	payload.graphic->Unknown4E6E90( UtopiaApp::GetApp()->GetGameTime(), BEAM_CHARGE_TIME );
	payload.state = 1;
	payload.length = 0.0f;
	payload.targetID = 0;
	firing = true;
	AimBeam( start, target, time );
}

/*
================
PhaserBank::AimBeam

The beam starts at the first point and points at the second, with its full range.
================
*/
// FUNCTION: STBC 0x00572950
void PhaserBank::AimBeam( const NiPoint3 *from, const NiPoint3 *to, float deltaTime ) {
	payload.start = *from;
	payload.direction = *to - *from;
	payload.direction.Unitize();
	payload.length = maxPhaserRange;
}

/*
================
PhaserBank::CalculateDamage

The most the bank can do, by its condition and its system's, scaled down to
the distance it reaches, up by its power level, and by the time.
================
*/
// FUNCTION: STBC 0x00572a50
// PARKED: x87 operand order: retail loads the bank's own condition percentage before the parent pointer; both operand orders and the accessor spelling load the parent's first
float PhaserBank::CalculateDamage( float time, float distance ) {
	float levelFactor;
	switch ( ( (PhaserSystem *)parentSubsystem )->GetPowerLevel() ) {
	case 0:
		levelFactor = DAMAGE_FACTOR_LEVEL0;
		break;
	case 1:
		levelFactor = DAMAGE_FACTOR_LEVEL1;
		break;
	case 2:
		levelFactor = DAMAGE_FACTOR_LEVEL2;
		break;
	}

	if ( distance <= 0.0f ) {
		distance = GetMaxDamageDistance();
	}
	float reach = GetMaxDamageDistance() / distance;
	if ( reach > 1.0 ) {
		reach = 1.0f;
	}
	float condition = conditionPercentage * parentSubsystem->GetConditionPercentage();
	return GetMaxDamage() * condition * reach * levelFactor * time;
}

/*
================
PhaserBank::GetDischargeFactor

How much of the full rate a firing bank discharges at: low power
discharges slowly, normal power and above at the full rate.
================
*/
// FUNCTION: STBC 0x00572b00
float PhaserBank::GetDischargeFactor( void ) {
	switch ( ( (PhaserSystem *)parentSubsystem )->GetPowerLevel() ) {
	case 0:
		return DISCHARGE_FACTOR_LEVEL0;
	case 1:
		return DISCHARGE_FACTOR_LEVEL1;
	case 2:
		return DISCHARGE_FACTOR_LEVEL2;
	}
	return 0.0f;
}

/*
================
PhaserBank::Update

The bank recharges at its system's normal power while the system is above
its disabled condition.
================
*/
// FUNCTION: STBC 0x00572b30
void PhaserBank::Update( float deltaTime ) {
	PoweredSubsystem *system = (PoweredSubsystem *)parentSubsystem;
	float condition = system->GetConditionPercentage();
	if ( condition > system->GetDisabledPercentage() ) {
		UpdateCharge( deltaTime, system->GetNormalPowerPercentage() );
	}
	EnergyWeapon::Update( deltaTime );
}

/*
================
PhaserBank::UpdateCharge

A firing bank discharges; an idle one recharges, at the rate its
condition and the power it gets allow. A client's bank recharges quicker,
the host corrects it.
================
*/
// FUNCTION: STBC 0x00572b80
void PhaserBank::UpdateCharge( float deltaTime, float powerPercentage ) {
	bool isLocal = true;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		isLocal = Game::GetCurrentGame()->GetPlayer() == parentShip;
	}

	if ( firing == true && ( payload.state == 3 || payload.state == 2 ) ) {
		chargeLevel = chargeLevel - GetDischargeFactor() * deltaTime;
		if ( chargeLevel <= 0.0f ) {
			chargeLevel = 0.0f;
			StopFiring();
		}
	} else if ( !firing ) {
		float condition = GetConditionPercentage();
		float recharge = GetRechargeRate() * condition * deltaTime * powerPercentage;
		if ( !isLocal ) {
			recharge *= 1.25f;
		}
		chargeLevel += recharge;
		if ( GetMaxCharge() < chargeLevel ) {
			chargeLevel = GetMaxCharge();
		}
	}
}

/*
================
PhaserBank::CalculateRoughDirection

The direction the bank points in when it sits in the middle of its arc, in
the world: the forward axis turned right by half the width angles' sum and
up by half the height angles' sum, rotated by the ship's world rotation.
================
*/
// FUNCTION: STBC 0x00572c50
TGPoint3 PhaserBank::CalculateRoughDirection( void ) {
	float width = ( GetArcWidthAngleMin() + GetArcWidthAngleMax() ) * 0.5f;
	NiPoint3 direction = cosf( width ) * GetOrientationForward() + GetOrientationRight() * sinf( width );
	direction.Unitize();

	float height = ( GetArcHeightAngleMin() + GetArcHeightAngleMax() ) * 0.5f;
	direction = cosf( height ) * direction + GetOrientationUp() * sinf( height );
	direction.Unitize();

	return TGPoint3( GetParentShip()->GetWorldRotation() * TGPoint3( direction ) );
}

/*
================
PhaserBank::Unknown572F00

Moves the bank to the middle of its arc: forward by the cosine of the middle
angle times the length, and to the right by its sine times the width,
from the subsystem's position.
================
*/
// FUNCTION: STBC 0x00572f00
// DRAFT: retail keeps TGPoint3 temporaries (EH frame), scales each direction z, y, x and adds the subsystem position through ShipSubsystem::GetPosition out of line; this spelling scales x, y, z with no EH state
void PhaserBank::Unknown572F00( void ) {
	float middle = ( GetArcWidthAngleMax() - GetArcWidthAngleMin() ) * 0.5f + GetArcWidthAngleMin();
	NiPoint3 offset = ( GetWidth() * sinf( middle ) ) * GetOrientationRight();
	offset += ( GetLength() * cosf( middle ) ) * GetOrientationForward();
	position = ShipSubsystem::GetPosition() + offset;
}

/*
================
PhaserBank::WriteToStream
================
*/
// FUNCTION: STBC 0x00573040
bool PhaserBank::WriteToStream( TGStream *stream ) {
	if ( !EnergyWeapon::WriteToStream( stream ) ) {
		return false;
	}
	if ( !payload.WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	return true;
}

/*
================
PhaserBank::ReadFromStream
================
*/
// FUNCTION: STBC 0x005730a0
bool PhaserBank::ReadFromStream( TGStream *stream ) {
	if ( !EnergyWeapon::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !payload.ReadFromStream( stream ) ) {
		return false;
	}

	position.x = stream->ReadFloat();
	position.y = stream->ReadFloat();
	position.z = stream->ReadFloat();
	return true;
}

/*
================
PhaserPayload::PhaserPayload
================
*/
// FUNCTION: STBC 0x00573100
PhaserPayload::PhaserPayload( void ) {
	payloadType = WP_PHASER;
	start = NiPoint3::ZERO;
	direction = NiPoint3::UNIT_Y;
	length = 0.0f;
	graphic = NULL;
	targetID = 0;
	timeOnTarget = 0.0f;
}

/*
================
PhaserPayload::~PhaserPayload

Takes the graphic out of the scene.
================
*/
// FUNCTION: STBC 0x00573220
// FUNCTION: STBC 0x00573200 ??_GPhaserPayload@@UAEPAXI@Z
PhaserPayload::~PhaserPayload( void ) {
	if ( graphic != NULL && graphic->GetParent() != NULL ) {
		graphic->GetParent()->DetachChild( graphic );
	}
}

/*
================
PhaserPayload::WriteToStream
================
*/
// FUNCTION: STBC 0x005732f0
bool PhaserPayload::WriteToStream( TGStream *stream ) {
	if ( !WeaponPayload::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( start.x );
	stream->WriteFloat( start.y );
	stream->WriteFloat( start.z );
	stream->WriteFloat( direction.x );
	stream->WriteFloat( direction.y );
	stream->WriteFloat( direction.z );
	stream->WriteFloat( length );
	stream->WriteLong( state );
	stream->WriteID( targetID );
	stream->WriteFloat( hitPoint.x );
	stream->WriteFloat( hitPoint.y );
	stream->WriteFloat( hitPoint.z );
	stream->WriteFloat( hitNormal.x );
	stream->WriteFloat( hitNormal.y );
	stream->WriteFloat( hitNormal.z );
	stream->WriteFloat( timeOnTarget );
	return true;
}

/*
================
PhaserPayload::ReadFromStream
================
*/
// FUNCTION: STBC 0x005733f0
bool PhaserPayload::ReadFromStream( TGStream *stream ) {
	if ( !WeaponPayload::ReadFromStream( stream ) ) {
		return false;
	}

	start.x = stream->ReadFloat();
	start.y = stream->ReadFloat();
	start.z = stream->ReadFloat();
	direction.x = stream->ReadFloat();
	direction.y = stream->ReadFloat();
	direction.z = stream->ReadFloat();
	length = stream->ReadFloat();
	state = stream->ReadLong();
	targetID = stream->ReadID();
	hitPoint.x = stream->ReadFloat();
	hitPoint.y = stream->ReadFloat();
	hitPoint.z = stream->ReadFloat();
	hitNormal.x = stream->ReadFloat();
	hitNormal.y = stream->ReadFloat();
	hitNormal.z = stream->ReadFloat();
	timeOnTarget = stream->ReadFloat();
	return true;
}

// DATA: STBC 0x0089317c
const float PhaserBank::DISCHARGE_FACTOR_LEVEL0 = 0.35f;

// DATA: STBC 0x00893180
const float PhaserBank::DISCHARGE_FACTOR_LEVEL1 = 1.0f;

// DATA: STBC 0x00893184
const float PhaserBank::DISCHARGE_FACTOR_LEVEL2 = 1.0f;


// DATA: STBC 0x00893170
const float PhaserBank::DAMAGE_FACTOR_LEVEL0 = 0.25f;

// DATA: STBC 0x00893174
const float PhaserBank::DAMAGE_FACTOR_LEVEL1 = 0.5f;

// DATA: STBC 0x00893178
const float PhaserBank::DAMAGE_FACTOR_LEVEL2 = 0.5f;

