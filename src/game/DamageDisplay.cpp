/*
===============================================================================

	DamageDisplay.cpp

===============================================================================
*/

#include <iostream>
#include "DamageDisplay.h"
#include "DamageIcon.h"
#include "GraphicsModeInfo.h"
#include "ShipProperty.h"
#include "SubsystemProperty.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGUIModule.h"
#include "EngineProperty.h"
#include "HullClass.h"
#include "PhaserBank.h"
#include "PowerSubsystem.h"
#include "PulseWeapon.h"
#include "SensorSubsystem.h"
#include "ShieldClass.h"
#include "TorpedoTube.h"
#include "PyEmbed.h"
#include "ShipClass.h"
#include "ShipDisplay.h"
#include "ShipSubsystem.h"
#include "TGEventManager.h"
#include "TGEvent.h"
#include "TGObjPtrEvent.h"
#include "TGEventHandlerTable.h"
#include "STColors.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00542180 _$E544
// FUNCTION: STBC 0x005421a0 _$E543

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005403d0 _$E32
// FUNCTION: STBC 0x005403e0 _$E29
// FUNCTION: STBC 0x005403f0 _$E31
// FUNCTION: STBC 0x00540400 _$E30
// FUNCTION: STBC 0x00540410 _$E38
// FUNCTION: STBC 0x00540420 _$E35
// FUNCTION: STBC 0x00540430 _$E37
// FUNCTION: STBC 0x00540440 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00540450 _$E57
// FUNCTION: STBC 0x00540460 _$E56
// FUNCTION: STBC 0x00540470 _$E61
// FUNCTION: STBC 0x00540480 _$E60
// FUNCTION: STBC 0x00540490 _$E72
// FUNCTION: STBC 0x005404a0 _$E71
// FUNCTION: STBC 0x005404b0 _$E76
// FUNCTION: STBC 0x005404c0 _$E75
// FUNCTION: STBC 0x005404d0 _$E80
// FUNCTION: STBC 0x005404e0 _$E79
// FUNCTION: STBC 0x005404f0 _$E84
// FUNCTION: STBC 0x00540500 _$E83
// FUNCTION: STBC 0x00540a80 _$E208
// FUNCTION: STBC 0x00540a90 _$E207
// FUNCTION: STBC 0x005418a0 _$E150
// FUNCTION: STBC 0x005418b0 _$E149
// FUNCTION: STBC 0x005418c0 _$E163
// FUNCTION: STBC 0x005418d0 _$E162
// FUNCTION: STBC 0x005418e0 _$E213
// FUNCTION: STBC 0x005418f0 _$E212
// FUNCTION: STBC 0x00541900 _$E218
// FUNCTION: STBC 0x00541910 _$E217
// FUNCTION: STBC 0x00541920 _$E222
// FUNCTION: STBC 0x00541930 _$E221
// FUNCTION: STBC 0x00541940 _$E226
// FUNCTION: STBC 0x00541950 _$E225
// FUNCTION: STBC 0x00541960 _$E233
// FUNCTION: STBC 0x00541970 _$E232
// FUNCTION: STBC 0x00541980 _$E378
// FUNCTION: STBC 0x00541990 _$E377
// FUNCTION: STBC 0x005419a0 _$E382
// FUNCTION: STBC 0x005419b0 _$E381
// FUNCTION: STBC 0x005419c0 _$E108
// FUNCTION: STBC 0x005419d0 _$E107
// FUNCTION: STBC 0x005419e0 _$E264
// FUNCTION: STBC 0x005419f0 _$E263
// FUNCTION: STBC 0x00541a00 _$E271
// FUNCTION: STBC 0x00541a10 _$E270
// FUNCTION: STBC 0x00541a20 _$E275
// FUNCTION: STBC 0x00541a30 _$E274
// FUNCTION: STBC 0x00541a40 _$E279
// FUNCTION: STBC 0x00541a50 _$E278
// FUNCTION: STBC 0x00541a60 _$E283
// FUNCTION: STBC 0x00541a70 _$E282
// FUNCTION: STBC 0x00541a80 _$E289
// FUNCTION: STBC 0x00541a90 _$E288
// FUNCTION: STBC 0x00541aa0 _$E386
// FUNCTION: STBC 0x00541ab0 _$E385
// FUNCTION: STBC 0x00541ac0 _$E390
// FUNCTION: STBC 0x00541ad0 _$E389
// FUNCTION: STBC 0x00541ae0 _$E394
// FUNCTION: STBC 0x00541af0 _$E393
// FUNCTION: STBC 0x00541b00 _$E398
// FUNCTION: STBC 0x00541b10 _$E397
// FUNCTION: STBC 0x00541b20 _$E402
// FUNCTION: STBC 0x00541b30 _$E401
// FUNCTION: STBC 0x00541b40 _$E406
// FUNCTION: STBC 0x00541b50 _$E405
// FUNCTION: STBC 0x00541b60 _$E293
// FUNCTION: STBC 0x00541b70 _$E292
// FUNCTION: STBC 0x00541b80 _$E410
// FUNCTION: STBC 0x00541b90 _$E409
// FUNCTION: STBC 0x00541ba0 _$E414
// FUNCTION: STBC 0x00541bb0 _$E413
// FUNCTION: STBC 0x00541bc0 _$E418
// FUNCTION: STBC 0x00541bd0 _$E417
// FUNCTION: STBC 0x00541be0 _$E113
// FUNCTION: STBC 0x00541bf0 _$E112
// FUNCTION: STBC 0x00541c00 _$E422
// FUNCTION: STBC 0x00541c10 _$E421
// FUNCTION: STBC 0x00541c20 _$E118
// FUNCTION: STBC 0x00541c30 _$E117
// FUNCTION: STBC 0x00541c40 _$E426
// FUNCTION: STBC 0x00541c50 _$E425
// FUNCTION: STBC 0x00541c60 _$E430
// FUNCTION: STBC 0x00541c70 _$E429
// FUNCTION: STBC 0x00541c80 _$E434
// FUNCTION: STBC 0x00541c90 _$E433
// FUNCTION: STBC 0x00541ca0 _$E438
// FUNCTION: STBC 0x00541cb0 _$E437
// FUNCTION: STBC 0x00541cc0 _$E127
// FUNCTION: STBC 0x00541cd0 _$E126
// FUNCTION: STBC 0x00541ce0 _$E442
// FUNCTION: STBC 0x00541cf0 _$E441
// FUNCTION: STBC 0x00541d00 _$E446
// FUNCTION: STBC 0x00541d10 _$E445
// FUNCTION: STBC 0x00541d20 _$E450
// FUNCTION: STBC 0x00541d30 _$E449
// FUNCTION: STBC 0x00541d40 _$E133
// FUNCTION: STBC 0x00541d50 _$E132
// FUNCTION: STBC 0x00541d60 _$E138
// FUNCTION: STBC 0x00541d70 _$E137
// FUNCTION: STBC 0x00541d80 _$E168
// FUNCTION: STBC 0x00541d90 _$E167
// FUNCTION: STBC 0x00541da0 _$E454
// FUNCTION: STBC 0x00541db0 _$E453
// FUNCTION: STBC 0x00541dc0 _$E173
// FUNCTION: STBC 0x00541dd0 _$E172
// FUNCTION: STBC 0x00541de0 _$E458
// FUNCTION: STBC 0x00541df0 _$E457
// FUNCTION: STBC 0x00541e00 _$E462
// FUNCTION: STBC 0x00541e10 _$E461
// FUNCTION: STBC 0x00541e20 _$E178
// FUNCTION: STBC 0x00541e30 _$E177
// FUNCTION: STBC 0x00541e40 _$E466
// FUNCTION: STBC 0x00541e50 _$E465
// FUNCTION: STBC 0x00541e60 _$E143
// FUNCTION: STBC 0x00541e70 _$E142
// FUNCTION: STBC 0x00541ec0 _$E470
// FUNCTION: STBC 0x00541ed0 _$E469
// FUNCTION: STBC 0x00541ee0 _$E474
// FUNCTION: STBC 0x00541ef0 _$E473
// FUNCTION: STBC 0x00541f00 _$E183
// FUNCTION: STBC 0x00541f10 _$E182
// FUNCTION: STBC 0x00541f20 _$E188
// FUNCTION: STBC 0x00541f30 _$E187
// FUNCTION: STBC 0x00541f40 _$E478
// FUNCTION: STBC 0x00541f50 _$E477
// FUNCTION: STBC 0x00541f60 _$E482
// FUNCTION: STBC 0x00541f70 _$E481
// FUNCTION: STBC 0x00541f80 _$E486
// FUNCTION: STBC 0x00541f90 _$E485
// FUNCTION: STBC 0x00541fa0 _$E490
// FUNCTION: STBC 0x00541fb0 _$E489
// FUNCTION: STBC 0x00541fc0 _$E494
// FUNCTION: STBC 0x00541fd0 _$E493
// FUNCTION: STBC 0x00541fe0 _$E498
// FUNCTION: STBC 0x00541ff0 _$E497
// FUNCTION: STBC 0x00542000 _$E502
// FUNCTION: STBC 0x00542010 _$E501
// FUNCTION: STBC 0x00542020 _$E506
// FUNCTION: STBC 0x00542030 _$E505
// FUNCTION: STBC 0x00542040 _$E193
// FUNCTION: STBC 0x00542050 _$E192
// FUNCTION: STBC 0x00542060 _$E510
// FUNCTION: STBC 0x00542070 _$E509
// FUNCTION: STBC 0x00542080 _$E198
// FUNCTION: STBC 0x00542090 _$E197
// FUNCTION: STBC 0x005420a0 _$E203
// FUNCTION: STBC 0x005420b0 _$E202
// FUNCTION: STBC 0x005421b0 _$E514
// FUNCTION: STBC 0x005421c0 _$E513
// FUNCTION: STBC 0x005421d0 _$E519
// FUNCTION: STBC 0x005421e0 _$E518
// FUNCTION: STBC 0x005421f0 _$E523
// FUNCTION: STBC 0x00542200 _$E522

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00540510 _$E529
// FUNCTION: STBC 0x00540520 _$E525
// FUNCTION: STBC 0x00540530 _$E528
// FUNCTION: STBC 0x00540540 _$E526
// DATA: STBC 0x0098a4e8
TGEventHandlerTable DamageDisplay::classHandlers;

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00540560 _$E534
// FUNCTION: STBC 0x00540570 _$E531
// FUNCTION: STBC 0x00540580 _$E533
// FUNCTION: STBC 0x00540590 _$E532
// DATA: STBC 0x0098a580
TGEventHandlerTable DamageIcon::classHandlers;

// How long a damaged subsystem's icon blinks, and the blink period (invented names).
// DATA: STBC 0x00891a1c
static const float BLINK_TIME = 6.0f;
// DATA: STBC 0x00891a20
static const float BLINK_PERIOD = 1.0f;
// DATA: STBC 0x00891a24
static float ICON_SCALE = 0.85f;	// the damage icons are drawn at this fraction of the ship icon (invented name)

/*
================
DamageIcon::Cast

The object as a DamageIcon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005405b0
DamageIcon *DamageIcon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( DAMAGEICON_TYPE ) ) {
		return (DamageIcon *)object;
	}
	return NULL;
}

/*
================
DamageDisplay::Cast

The object as a DamageDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005405E0
DamageDisplay *DamageDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( DAMAGEDISPLAY_TYPE ) ) {
		return (DamageDisplay *)object;
	}
	return NULL;
}

/*
================
DamageIcon::DamageIcon

An icon of the group for the subsystem, with no timers running.
================
*/
// FUNCTION: STBC 0x00540610 ??0DamageIcon@@QAE@PADHPAVShipSubsystem@@PAVNiColorA@@@Z
DamageIcon::DamageIcon( char *groupName, int iconNum, ShipSubsystem *subsystem_, NiColorA *color ) : TGIcon( groupName, iconNum, color ) {
	subsystem = subsystem_;
	blinkTimerID = 0;
	expireTimerID = 0;
	expireEventID = 0;
	blinking = false;
}

/*
================
DamageIcon::~DamageIcon

Takes the timers down.
================
*/
// FUNCTION: STBC 0x00540730 ??1DamageIcon@@UAE@XZ
DamageIcon::~DamageIcon() {
	if ( expireTimerID != 0 ) {
		g_kTimerManager.DeleteTimer( expireTimerID );
	}
	if ( blinkTimerID != 0 ) {
		g_kTimerManager.DeleteTimer( blinkTimerID );
	}
}

/*
================
DamageIcon::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005407a0
void DamageIcon::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageIcon::HandleExpire, "DamageIcon::HandleExpire" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageIcon::HandleBlink, "DamageIcon::HandleBlink" );
}

/*
================
DamageIcon::RegisterHandlers
================
*/
// FUNCTION: STBC 0x005407d0
void DamageIcon::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_DAMAGE_ICON_EXPIRE, "DamageIcon::HandleExpire" );
	AddMethodHandlerForClass( &classHandlers, ET_DAMAGE_ICON_BLINK, "DamageIcon::HandleBlink" );
}

/*
================
DamageIcon::HandleExpire

The six seconds are over: the icon stops blinking and shows its colour for
good.
================
*/
// FUNCTION: STBC 0x00540800
void DamageIcon::HandleExpire( TGEvent *event ) {
	expireTimerID = 0;
	expireEventID = 0;
	StopBlinking();
	UpdateColor( false );
	if ( subsystem->GetConditionPercentage() <= 1.0f ) {
		SetVisible( true );
	} else {
		SetNotVisible( false );
	}
	CallNextHandler( event );
}

/*
================
DamageIcon::HandleBlink

Shows the icon when it is hidden and hides it when it shows.
================
*/
// FUNCTION: STBC 0x00540870
void DamageIcon::HandleBlink( TGEvent *event ) {
	int wasVisible = IsVisible();
	UpdateColor( false );
	if ( wasVisible ) {
		SetNotVisible( false );
	} else {
		SetVisible( true );
	}
}

/*
================
DamageIcon::ResetPosition

Puts the icon back where its subsystem's property places it on the ship's
icon: the property's position scaled to the icon pane, relative to the
ship icon's centre.
================
*/
// FUNCTION: STBC 0x005408b0 ?ResetPosition@DamageIcon@@QAEXXZ
// DRAFT: retail copies the scale into a frame slot and the screen width into a stack temporary before its fidiv (width/height or scale locals and GetCurrentMode() don't); the position divided and scaled in place gives its frame
void DamageIcon::ResetPosition( void ) {
	if ( subsystem == NULL ) {
		return;
	}
	SubsystemProperty *property = subsystem->GetProperty();
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	NiPoint2 position;
	property->GetPosition2D( &position.x, &position.y );
	position.x /= mode->GetWidth();
	position.y /= (unsigned int)mode->GetHeight();
	position *= ICON_SCALE;

	ShipClass *ship = ( (DamageDisplay *)parent )->GetShip();
	if ( ship == NULL || ship->GetShipProperty() == NULL ) {
		return;
	}
	int species = ship->GetShipProperty()->GetSpecies();
	TGIconGroup *group = g_kIconManager.GetIconGroup( "ShipIcons" );
	float iconWidth = group->GetIconScreenWidth( species ) * ICON_SCALE;
	float iconHeight = group->GetIconScreenHeight( species ) * ICON_SCALE;
	TGPane *pane = parent;
	float x = TGUIModule::PixelAlignValue( ( pane->GetWidth() - iconWidth ) * 0.5f );
	float y = TGUIModule::PixelAlignValue( ( pane->GetHeight() - iconHeight ) * 0.5f, false );
	SetPosition( x - pane->GetLeft() + position.x - bounds.width * 0.5f, y - pane->GetTop() + position.y - bounds.height * 0.5f, false );
}

/*
================
DamageIcon::UpdateColor

Shows the icon in the colour of its subsystem's state and hides it while the
subsystem is whole or not targetable. A damaged subsystem's icon gets the
timers that make it blink and, six seconds later, stop.
================
*/
// FUNCTION: STBC 0x00540aa0
void DamageIcon::UpdateColor( bool startTimers ) {
	float condition = subsystem->GetConditionPercentage();
	if ( condition < 1.0f && subsystem->IsTargetable() ) {
		if ( !blinking ) {
			SetVisible( true );
		}
		if ( condition <= 0.0f ) {
			SetColor( &g_kDamageDisplayDestroyedColor );
			return;
		}
		if ( subsystem->IsDisabled() == true ) {
			SetColor( &g_kDamageDisplayDisabledColor );
		} else {
			SetColor( &g_kDamageDisplayDamagedColor );
		}
		if ( startTimers == true ) {
			bool hadExpireTimer = expireTimerID != 0;
			if ( hadExpireTimer == true ) {
				StopExpireTimer();
			}

			TGEvent *expireEvent = new TGEvent;
			expireEvent->SetSource( this );
			expireEvent->SetDestination( this );
			expireEvent->SetEventType( ET_DAMAGE_ICON_EXPIRE );
			TGTimer *expireTimer = new TGTimer;
			expireTimer->SetTimerStart( g_kUtopiaModule.GetGameTime() + BLINK_TIME );
			expireTimer->SetEvent( expireEvent );
			g_kTimerManager.AddTimer( expireTimer );
			expireTimerID = expireTimer->GetObjID();
			expireEventID = expireEvent->GetObjID();

			if ( !hadExpireTimer && blinkTimerID == 0 ) {
				TGEvent *blinkEvent = new TGEvent;
				blinkEvent->SetSource( this );
				blinkEvent->SetDestination( this );
				blinkEvent->SetEventType( ET_DAMAGE_ICON_BLINK );
				TGTimer *blinkTimer = new TGTimer;
				blinkTimer->SetTimerStart( g_kUtopiaModule.GetGameTime() + BLINK_PERIOD );
				blinkTimer->SetDuration( TIMER_FOREVER );
				blinkTimer->SetDelay( BLINK_PERIOD );
				blinkTimer->SetEvent( blinkEvent );
				g_kTimerManager.AddTimer( blinkTimer );
				blinkTimerID = blinkTimer->GetObjID();
				blinking = true;
			}
		}
	} else {
		SetNotVisible( false );
	}
}

/*
================
DamageIcon::StopBlinking
================
*/
// FUNCTION: STBC 0x00540ce0
void DamageIcon::StopBlinking( void ) {
	if ( blinkTimerID != 0 ) {
		TGTimer *timer = TGTimer::GetTGTimerPtr( blinkTimerID );
		if ( timer != NULL ) {
			g_kTimerManager.DeleteTimer( timer );
		}
	}
	blinkTimerID = 0;
	blinking = false;
}

/*
================
DamageIcon::StopExpireTimer

Deletes the expire timer, or its event when the timer has gone off already.
================
*/
// FUNCTION: STBC 0x00540d10
void DamageIcon::StopExpireTimer( void ) {
	if ( expireTimerID != 0 ) {
		TGTimer *timer = TGTimer::GetTGTimerPtr( expireTimerID );
		if ( timer != NULL ) {
			g_kTimerManager.DeleteTimer( timer );
		} else {
			g_kEventManager.RemoveQueuedEvent( expireEventID );
		}
	}
	expireTimerID = 0;
	expireEventID = 0;
}

/*
================
DamageIcon::WriteToStream

The icon, then the subsystem's ID, the timers and the blink flag.
================
*/
// FUNCTION: STBC 0x00540d60 ?WriteToStream@DamageIcon@@UAE_NPAVTGStream@@@Z
bool DamageIcon::WriteToStream( TGStream *stream ) {
	if ( !TGIcon::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( subsystem != NULL ? subsystem->GetObjID() : 0 );
	stream->WriteID( blinkTimerID );
	stream->WriteID( expireTimerID );
	stream->WriteID( expireEventID );
	stream->WriteBool( blinking );
	return true;
}

/*
================
DamageIcon::ReadFromStream

The icon, then the subsystem's ID, the timers and the blink flag.
================
*/
// FUNCTION: STBC 0x00540dd0 ?ReadFromStream@DamageIcon@@UAE_NPAVTGStream@@@Z
bool DamageIcon::ReadFromStream( TGStream *stream ) {
	if ( !TGIcon::ReadFromStream( stream ) ) {
		return false;
	}
	subsystem = (ShipSubsystem *)stream->ReadID();
	blinkTimerID = stream->ReadID();
	expireTimerID = stream->ReadID();
	expireEventID = stream->ReadID();
	blinking = stream->ReadBool();
	return true;
}

/*
================
DamageIcon::FixUpPointers

The saved subsystem ID must name an object that still exists.
================
*/
// FUNCTION: STBC 0x00540e30 ?FixUpPointers@DamageIcon@@UAE_NXZ
bool DamageIcon::FixUpPointers( void ) {
	if ( !TGIcon::FixUpPointers() ) {
		return false;
	}
	if ( subsystem != NULL && TGObject::GetTGObjectPtr( (long)subsystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
DamageIcon::PostFixUp

The subsystem pointer is the object of the saved ID.
================
*/
// FUNCTION: STBC 0x00540e60 ?PostFixUp@DamageIcon@@UAEXXZ
void DamageIcon::PostFixUp( void ) {
	TGIcon::PostFixUp();
	subsystem = subsystem != NULL ? (ShipSubsystem *)TGObject::GetTGObjectPtr( (long)subsystem ) : NULL;
}

/*
================
DamageDisplay::DamageDisplay

An empty pane of the given size that takes no focus and draws its icons in
one batch.
================
*/
// FUNCTION: STBC 0x00540e90 ??0DamageDisplay@@QAE@MM@Z
DamageDisplay::DamageDisplay( float width, float height ) : TGPane( 0 ), damageIcons( 2 ) {
	timerID = 0;
	Resize( width, height, false );
	flags |= TGUIO_NO_FOCUS;
	SetBatchChildPolys( false );
}

/*
================
DamageDisplay::~DamageDisplay

Takes the icons and the timer down.
================
*/
// FUNCTION: STBC 0x00541000 ??1DamageDisplay@@UAE@XZ
DamageDisplay::~DamageDisplay() {
	RemoveEvents();
}

/*
================
DamageDisplay::GetShip

The ship the ship display above shows.
================
*/
// FUNCTION: STBC 0x00541090
ShipClass *DamageDisplay::GetShip( void ) {
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	if ( shipDisplay == NULL ) {
		return NULL;
	}
	return ShipClass::GetObjectByID( NULL, shipDisplay->GetShipID() );
}

/*
================
DamageDisplay::Unknown5410B0

Listens to the ship's subsystems and starts a timer that sends the display
a refresh event each second.
================
*/
// FUNCTION: STBC 0x005410B0
// PARKED: schedule residue; retail stores the 1.0f delay after the event allocation's argument pushes, every spelling of the timer set-up stores it before them
void DamageDisplay::Unknown5410B0( void ) {
	ShipClass *ship = GetShip();
	if ( ship != NULL ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DAMAGED, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DISABLED, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_OPERATIONAL, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DESTROYED, this, "DamageDisplay::HandleSubsystemEvents", ship );

		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() );
		timer->SetDuration( TIMER_FOREVER );
		timer->SetDelay( 1.0f );
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_DAMAGE_DISPLAY_REFRESH );
		event->SetDestination( this );
		timer->SetEvent( event );
		timerID = timer->GetObjID();
		g_kTimerManager.AddTimer( timer );
	}
}

/*
================
DamageDisplay::RemoveEvents

Deletes the timer and stops listening to the ship's subsystems.
================
*/
// FUNCTION: STBC 0x005411E0
void DamageDisplay::RemoveEvents( void ) {
	if ( timerID != 0 ) {
		g_kTimerManager.DeleteTimer( timerID );
		timerID = 0;
	}

	ShipClass *ship = GetShip();
	if ( ship != NULL ) {
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_DAMAGED, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_DISABLED, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_OPERATIONAL, this, "DamageDisplay::HandleSubsystemEvents", ship );
		g_kEventManager.RemoveBroadcastHandler( ET_SUBSYSTEM_DESTROYED, this, "DamageDisplay::HandleSubsystemEvents", ship );
	}
}

/*
================
DamageDisplay::ResizeUI

Has the scripts redo the display's sizes.
================
*/
// FUNCTION: STBC 0x00541290
void DamageDisplay::ResizeUI( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.DamageDisplay", "ResizeUI", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
DamageDisplay::RepositionUI

Puts every damage icon back at its place, then lays the pane out.
================
*/
// FUNCTION: STBC 0x005412D0
void DamageDisplay::RepositionUI( void ) {
	unsigned int numChildren = GetNumChildren();
	for ( unsigned int i = 0; i < numChildren; i++ ) {
		( (DamageIcon *)GetNthChild( i ) )->ResetPosition();
	}
	Layout();
}

/*
================
DamageDisplay::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00541310
void DamageDisplay::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageDisplay::HandleSubsystemEvents, "DamageDisplay::HandleSubsystemEvents" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&DamageDisplay::HandleDamageDisplayRefresh, "DamageDisplay::HandleDamageDisplayRefresh" );
}

/*
================
DamageDisplay::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00541340
void DamageDisplay::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_DAMAGE_DISPLAY_REFRESH, "DamageDisplay::HandleDamageDisplayRefresh" );
}

/*
================
DamageDisplay::HandleSubsystemEvents

A subsystem of the ship being damaged, disabled, made operational or
destroyed recolours its icon; damage starts the icon blinking.
================
*/
// FUNCTION: STBC 0x00541360
void DamageDisplay::HandleSubsystemEvents( TGEvent *event ) {
	bool damaged = false;
	ShipSubsystem *subsystem = NULL;
	switch ( event->GetEventType() ) {
	case ET_SUBSYSTEM_DAMAGED:
		subsystem = ShipSubsystem::Cast( ( (TGObjPtrEvent *)event )->GetObjPtr() );
		damaged = true;
		break;
	case ET_SUBSYSTEM_DISABLED:
	case ET_SUBSYSTEM_OPERATIONAL:
	case ET_SUBSYSTEM_DESTROYED:
		subsystem = ShipSubsystem::Cast( event->GetSource() );
		break;
	}
	if ( subsystem != NULL ) {
		NiTListIterator pos = damageIcons.GetHeadPos();
		while ( pos ) {
			DamageIcon *icon = damageIcons.GetNext( pos );
			if ( icon != NULL && icon->GetSubsystem() == subsystem ) {
				icon->UpdateColor( damaged );
				break;
			}
		}
	}
	CallNextHandler( event );
}

/*
================
DamageDisplay::HandleDamageDisplayRefresh

The display's timer recolours every icon.
================
*/
// FUNCTION: STBC 0x005413f0
void DamageDisplay::HandleDamageDisplayRefresh( TGEvent *event ) {
	NiTListIterator pos = damageIcons.GetHeadPos();
	while ( pos ) {
		DamageIcon *icon = damageIcons.GetNext( pos );
		if ( icon != NULL ) {
			icon->UpdateColor( false );
		}
	}
	CallNextHandler( event );
}

/*
================
DamageDisplay::UpdateForNewShip

Throws the icons away; when the ship display above has a ship, shows the new
ship's icons and listens to its subsystems, and when it has none hides the
display.
================
*/
// FUNCTION: STBC 0x00541420
void DamageDisplay::UpdateForNewShip( void ) {
	ShipDisplay *shipDisplay = (ShipDisplay *)GetConceptualParent();
	if ( shipDisplay != NULL ) {
		KillChildren();
		damageIcons.RemoveAll();
		if ( shipDisplay->GetShipID() == 0 ) {
			SetNotVisible( false );
			return;
		}
		Unknown5414B0();
		RemoveEvents();
		Unknown5410B0();
	}
}

/*
================
DamageDisplay::Unknown5414B0

Throws the icons away and makes one for every subsystem of the ship, the
children of each included.
================
*/
// FUNCTION: STBC 0x005414B0
void DamageDisplay::Unknown5414B0( void ) {
	NiTListIterator pos = damageIcons.GetHeadPos();
	while ( pos ) {
		DamageIcon *icon = damageIcons.GetNext( pos );
		if ( icon != NULL && icon->GetParent() != NULL ) {
			icon->GetParent()->DeleteChild( icon, false );
		}
	}
	damageIcons.RemoveAll();

	ShipClass *ship = GetShip();
	if ( ship != NULL ) {
		NiTListIterator subsystemPos = ship->GetSubsystemList().GetHeadPos();
		while ( subsystemPos ) {
			AddSubsystemIcons( ship->GetSubsystemList().GetNext( subsystemPos ) );
		}
	}
}

/*
================
DamageDisplay::AddSubsystemIcons

The subsystem's icon, placed and coloured, then the children's.
================
*/
// FUNCTION: STBC 0x00541550
void DamageDisplay::AddSubsystemIcons( ShipSubsystem *subsystem ) {
	if ( GetConceptualParent() != NULL && GetShip() != NULL ) {
		DamageIcon *icon = new DamageIcon( "DamageIcons", GetIconNumber( subsystem ), subsystem, NULL );
		damageIcons.AddTail( icon );
		AddChild( icon, 0.0f, 0.0f, false );
		icon->ResetPosition();
		icon->UpdateColor( false );
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			ShipSubsystem *child = subsystem->GetChildSubsystem( i );
			if ( child != NULL ) {
				AddSubsystemIcons( child );
			}
		}
	}
}

/*
================
DamageDisplay::GetIconNumber

The number of the subsystem's icon in the "DamageIcons" group, by its kind.
================
*/
// FUNCTION: STBC 0x00541660
int DamageDisplay::GetIconNumber( ShipSubsystem *subsystem ) {
	if ( subsystem->IsTypeOf( HULLCLASS_TYPE ) ) {
		return 0;
	}
	if ( subsystem->IsTypeOf( PHASERBANK_TYPE ) ) {
		return 2;
	}
	if ( subsystem->IsTypeOf( POWERSUBSYSTEM_TYPE ) ) {
		return 3;
	}
	if ( subsystem->IsTypeOf( SENSORSUBSYSTEM_TYPE ) ) {
		return 4;
	}
	if ( subsystem->IsTypeOf( SHIELDCLASS_TYPE ) ) {
		return 5;
	}
	if ( subsystem->IsTypeOf( TORPEDOTUBE_TYPE ) ) {
		return 7;
	}
	if ( subsystem->IsTypeOf( PULSEWEAPON_TYPE ) ) {
		return 9;
	}
	EngineProperty *engine = EngineProperty::Cast( subsystem->GetProperty() );
	if ( engine != NULL ) {
		return engine->GetEngineType() != EngineProperty::EP_IMPULSE ? 8 : 1;
	}
	return 6;
}

/*
================
DamageDisplay::WriteToStream

The pane, then the icons: a flag per entry, and the icon itself when there is one.
================
*/
// FUNCTION: STBC 0x00541730 ?WriteToStream@DamageDisplay@@UAE_NPAVTGStream@@@Z
bool DamageDisplay::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( damageIcons.GetSize() );
	NiTListIterator pos = damageIcons.GetHeadPos();
	while ( pos ) {
		DamageIcon *icon = damageIcons.GetNext( pos );
		if ( icon != NULL ) {
			stream->WriteBool( true );
			icon->WriteToStream( stream );
		} else {
			stream->WriteBool( false );
		}
	}
	return true;
}

/*
================
DamageDisplay::ReadFromStream

The pane, then the icons: a flag per entry, and the icon itself when there is one.
================
*/
// FUNCTION: STBC 0x00541790 ?ReadFromStream@DamageDisplay@@UAE_NPAVTGStream@@@Z
bool DamageDisplay::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		if ( stream->ReadBool() == true ) {
			DamageIcon *icon = (DamageIcon *)LoadObject( stream, 0 );
			damageIcons.AddTail( icon );
		} else {
			damageIcons.AddTail( NULL );
		}
	}
	return true;
}
