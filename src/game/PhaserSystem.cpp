/*
===============================================================================

	PhaserSystem.cpp

===============================================================================
*/

#include "PhaserSystem.h"
#include "PhaserBank.h"
#include "ObjectClass.h"
#include "TGCharEvent.h"
#include "TGEventManager.h"
#include "TGObjPtrEvent.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include <iostream>
#include "StartFiringEvent.h"
#include "UtopiaModule.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005742b0 _$E279
// FUNCTION: STBC 0x005742c0 _$E276
// FUNCTION: STBC 0x005742d0 _$E278
// FUNCTION: STBC 0x005742e0 _$E277
// FUNCTION: STBC 0x005742f0 _$E285
// FUNCTION: STBC 0x00574300 _$E282
// FUNCTION: STBC 0x00574310 _$E284
// FUNCTION: STBC 0x00574320 _$E283

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00574330 _$E66
// FUNCTION: STBC 0x00574340 _$E65
// FUNCTION: STBC 0x00574350 _$E291
// FUNCTION: STBC 0x00574360 _$E290
// FUNCTION: STBC 0x00574370 _$E77
// FUNCTION: STBC 0x00574380 _$E76
// FUNCTION: STBC 0x00574390 _$E81
// FUNCTION: STBC 0x005743a0 _$E80
// FUNCTION: STBC 0x005743b0 _$E85
// FUNCTION: STBC 0x005743c0 _$E84
// FUNCTION: STBC 0x005743d0 _$E89
// FUNCTION: STBC 0x005743e0 _$E88
// FUNCTION: STBC 0x005743f0 _$E208
// FUNCTION: STBC 0x00574400 _$E207
// FUNCTION: STBC 0x00574410 _$E150
// FUNCTION: STBC 0x00574420 _$E149
// FUNCTION: STBC 0x00574430 _$E163
// FUNCTION: STBC 0x00574440 _$E162
// FUNCTION: STBC 0x00574450 _$E213
// FUNCTION: STBC 0x00574460 _$E212
// FUNCTION: STBC 0x00574470 _$E296
// FUNCTION: STBC 0x00574480 _$E295
// FUNCTION: STBC 0x00574490 _$E217
// FUNCTION: STBC 0x005744a0 _$E216
// FUNCTION: STBC 0x005744b0 _$E300
// FUNCTION: STBC 0x005744c0 _$E299
// FUNCTION: STBC 0x005744d0 _$E307
// FUNCTION: STBC 0x005744e0 _$E306
// FUNCTION: STBC 0x005744f0 _$E338
// FUNCTION: STBC 0x00574500 _$E337
// FUNCTION: STBC 0x00574510 _$E342
// FUNCTION: STBC 0x00574520 _$E341
// FUNCTION: STBC 0x00574530 _$E108
// FUNCTION: STBC 0x00574540 _$E107
// FUNCTION: STBC 0x00574550 _$E225
// FUNCTION: STBC 0x00574560 _$E224
// FUNCTION: STBC 0x00574570 _$E232
// FUNCTION: STBC 0x00574580 _$E231
// FUNCTION: STBC 0x00574590 _$E236
// FUNCTION: STBC 0x005745a0 _$E235
// FUNCTION: STBC 0x005745b0 _$E240
// FUNCTION: STBC 0x005745c0 _$E239
// FUNCTION: STBC 0x005745d0 _$E244
// FUNCTION: STBC 0x005745e0 _$E243
// FUNCTION: STBC 0x005745f0 _$E250
// FUNCTION: STBC 0x00574600 _$E249
// FUNCTION: STBC 0x00574610 _$E346
// FUNCTION: STBC 0x00574620 _$E345
// FUNCTION: STBC 0x00574630 _$E350
// FUNCTION: STBC 0x00574640 _$E349
// FUNCTION: STBC 0x00574650 _$E354
// FUNCTION: STBC 0x00574660 _$E353
// FUNCTION: STBC 0x00574670 _$E358
// FUNCTION: STBC 0x00574680 _$E357
// FUNCTION: STBC 0x00574690 _$E362
// FUNCTION: STBC 0x005746a0 _$E361
// FUNCTION: STBC 0x005746b0 _$E366
// FUNCTION: STBC 0x005746c0 _$E365
// FUNCTION: STBC 0x005746d0 _$E254
// FUNCTION: STBC 0x005746e0 _$E253
// FUNCTION: STBC 0x005746f0 _$E370
// FUNCTION: STBC 0x00574700 _$E369
// FUNCTION: STBC 0x00574710 _$E374
// FUNCTION: STBC 0x00574720 _$E373
// FUNCTION: STBC 0x00574730 _$E378
// FUNCTION: STBC 0x00574740 _$E377
// FUNCTION: STBC 0x00574750 _$E113
// FUNCTION: STBC 0x00574760 _$E112
// FUNCTION: STBC 0x00574770 _$E382
// FUNCTION: STBC 0x00574780 _$E381
// FUNCTION: STBC 0x00574790 _$E118
// FUNCTION: STBC 0x005747a0 _$E117
// FUNCTION: STBC 0x005747b0 _$E386
// FUNCTION: STBC 0x005747c0 _$E385
// FUNCTION: STBC 0x005747d0 _$E390
// FUNCTION: STBC 0x005747e0 _$E389
// FUNCTION: STBC 0x005747f0 _$E394
// FUNCTION: STBC 0x00574800 _$E393
// FUNCTION: STBC 0x00574810 _$E398
// FUNCTION: STBC 0x00574820 _$E397
// FUNCTION: STBC 0x00574830 _$E127
// FUNCTION: STBC 0x00574840 _$E126
// FUNCTION: STBC 0x00574850 _$E402
// FUNCTION: STBC 0x00574860 _$E401
// FUNCTION: STBC 0x00574870 _$E406
// FUNCTION: STBC 0x00574880 _$E405
// FUNCTION: STBC 0x00574890 _$E410
// FUNCTION: STBC 0x005748a0 _$E409
// FUNCTION: STBC 0x005748b0 _$E133
// FUNCTION: STBC 0x005748c0 _$E132
// FUNCTION: STBC 0x005748d0 _$E138
// FUNCTION: STBC 0x005748e0 _$E137
// FUNCTION: STBC 0x005748f0 _$E168
// FUNCTION: STBC 0x00574900 _$E167
// FUNCTION: STBC 0x00574940 _$E414
// FUNCTION: STBC 0x00574950 _$E413
// FUNCTION: STBC 0x00574960 _$E173
// FUNCTION: STBC 0x00574970 _$E172
// FUNCTION: STBC 0x00574980 _$E418
// FUNCTION: STBC 0x00574990 _$E417
// FUNCTION: STBC 0x005749a0 _$E422
// FUNCTION: STBC 0x005749b0 _$E421
// FUNCTION: STBC 0x005749c0 _$E178
// FUNCTION: STBC 0x005749d0 _$E177
// FUNCTION: STBC 0x005749e0 _$E426
// FUNCTION: STBC 0x005749f0 _$E425
// FUNCTION: STBC 0x00574a00 _$E143
// FUNCTION: STBC 0x00574a10 _$E142
// FUNCTION: STBC 0x00574a20 _$E430
// FUNCTION: STBC 0x00574a30 _$E429
// FUNCTION: STBC 0x00574a40 _$E434
// FUNCTION: STBC 0x00574a50 _$E433
// FUNCTION: STBC 0x00574a60 _$E183
// FUNCTION: STBC 0x00574a70 _$E182
// FUNCTION: STBC 0x00574a80 _$E188
// FUNCTION: STBC 0x00574a90 _$E187
// FUNCTION: STBC 0x00574aa0 _$E438
// FUNCTION: STBC 0x00574ab0 _$E437
// FUNCTION: STBC 0x00574ac0 _$E442
// FUNCTION: STBC 0x00574ad0 _$E441
// FUNCTION: STBC 0x00574ae0 _$E446
// FUNCTION: STBC 0x00574af0 _$E445
// FUNCTION: STBC 0x00574b00 _$E450
// FUNCTION: STBC 0x00574b10 _$E449
// FUNCTION: STBC 0x00574b20 _$E454
// FUNCTION: STBC 0x00574b30 _$E453
// FUNCTION: STBC 0x00574b40 _$E458
// FUNCTION: STBC 0x00574b50 _$E457
// FUNCTION: STBC 0x00574b60 _$E462
// FUNCTION: STBC 0x00574b70 _$E461
// FUNCTION: STBC 0x00574b80 _$E466
// FUNCTION: STBC 0x00574b90 _$E465
// FUNCTION: STBC 0x00574ba0 _$E193
// FUNCTION: STBC 0x00574bb0 _$E192
// FUNCTION: STBC 0x00574bc0 _$E470
// FUNCTION: STBC 0x00574bd0 _$E469
// FUNCTION: STBC 0x00574be0 _$E198
// FUNCTION: STBC 0x00574bf0 _$E197
// FUNCTION: STBC 0x00574c00 _$E203
// FUNCTION: STBC 0x00574c10 _$E202
// FUNCTION: STBC 0x00574cf0 _$E319
// FUNCTION: STBC 0x00574d00 _$E318
// FUNCTION: STBC 0x00574d10 _$E324
// FUNCTION: STBC 0x00574d20 _$E323
// FUNCTION: STBC 0x00574d30 _$E328
// FUNCTION: STBC 0x00574d40 _$E327

// Standard streams, handler tables and other file-scope statics.
// FUNCTION: STBC 0x00573b80 _$E484
// FUNCTION: STBC 0x00573ba0 _$E483
// FUNCTION: STBC 0x00573c10 _$E476
// FUNCTION: STBC 0x00573c20 _$E472
// FUNCTION: STBC 0x00573c30 _$E475
// FUNCTION: STBC 0x00573c40 _$E473

/*
================
PhaserSystem::Cast

The object as a PhaserSystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00573c60
PhaserSystem *PhaserSystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PHASERSYSTEM_TYPE ) ) {
		return (PhaserSystem *)object;
	}
	return NULL;
}

/*
================
PhaserSystem::PhaserSystem
================
*/
// FUNCTION: STBC 0x00573c90
PhaserSystem::PhaserSystem( long id ) : WeaponSystem( id ) {
	powerLevel = 1;
	unknownA4 = true;
}

// DATA: STBC 0x0098c838
TGEventHandlerTable PhaserSystem::classHandlers;

/*
================
PhaserSystem::GetObjType
================
*/
// FUNCTION: STBC 0x00573cf0
int PhaserSystem::GetObjType( void ) {
	return PHASERSYSTEM_TYPE;
}

/*
================
PhaserSystem::GetClassName
================
*/
// FUNCTION: STBC 0x00573d60
char *PhaserSystem::GetClassName( void ) {
	return "PhaserSystem";
}

/*
================
PhaserSystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00573d70
char *PhaserSystem::GetSwigTypeName( void ) {
	return "_p_PhaserSystem";
}

/*
================
PhaserSystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00573d80
char *PhaserSystem::GetPythonClassName( void ) {
	return "PhaserSystemPtr";
}

/*
================
PhaserSystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00573d90
TGEventHandlerTable *PhaserSystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PhaserSystem::~PhaserSystem
================
*/
// FUNCTION: STBC 0x00573dd0
// FUNCTION: STBC 0x00573da0 ??_GPhaserSystem@@UAEPAXI@Z
PhaserSystem::~PhaserSystem( void ) {
}

/*
================
PhaserSystem::RegisterHandlerNames

The phaser banks' handlers too.
================
*/
// FUNCTION: STBC 0x00573de0
void PhaserSystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhaserSystem::StartFiringHandler, "PhaserSystem::StartFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhaserSystem::StopFiringHandler, "PhaserSystem::StopFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhaserSystem::StopFiringAtTargetHandler, "PhaserSystem::StopFiringAtTargetHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhaserSystem::SetPhaserLevelHandler, "PhaserSystem::SetPhaserLevelHandler" );
	PhaserBank::RegisterHandlerNames();
}

/*
================
PhaserSystem::RegisterHandlers

The phaser banks' handlers too.
================
*/
// FUNCTION: STBC 0x00573e40
void PhaserSystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_START_FIRING, "PhaserSystem::StartFiringHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_STOP_FIRING, "PhaserSystem::StopFiringHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_STOP_FIRING_AT_TARGET, "PhaserSystem::StopFiringAtTargetHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_SET_PHASER_LEVEL, "PhaserSystem::SetPhaserLevelHandler" );
	PhaserBank::RegisterHandlers();
}

/*
================
PhaserSystem::VirtualSlot34

Starts firing at the target and offset; in a multiplayer game, a system that is
trying to fire tells the others with a StartFiringEvent (ET_START_FIRING_NOTIFY).
================
*/
// FUNCTION: STBC 0x00573ea0
void PhaserSystem::VirtualSlot34( ObjectClass *target, NiPoint3 *offset ) {
	WeaponSystem::VirtualSlot34( target, offset );
	if ( g_kUtopiaModule.IsMultiplayer() && tryingToFire && unknownA4 ) {
		StartFiringEvent *event = new StartFiringEvent;
		event->SetEventType( ET_START_FIRING_NOTIFY );
		event->SetDestination( this );
		event->SetTarget( target );
		event->SetOffset( offset );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
PhaserSystem::StopFiring

In a multiplayer game, a firing system that was trying to fire tells the others
with ET_STOP_FIRING_NOTIFY; then WeaponSystem stops the weapons.
================
*/
// FUNCTION: STBC 0x00573f30
void PhaserSystem::StopFiring( void ) {
	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 && tryingToFire ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_STOP_FIRING_NOTIFY );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
	}
	WeaponSystem::StopFiring();
}

/*
================
PhaserSystem::WriteToStream
================
*/
// FUNCTION: STBC 0x00573fb0
bool PhaserSystem::WriteToStream( TGStream *stream ) {
	if ( !WeaponSystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( powerLevel );
	return true;
}

/*
================
PhaserSystem::ReadFromStream
================
*/
// FUNCTION: STBC 0x00573fe0
bool PhaserSystem::ReadFromStream( TGStream *stream ) {
	if ( !WeaponSystem::ReadFromStream( stream ) ) {
		return false;
	}

	powerLevel = stream->ReadLong();
	return true;
}

/*
================
PhaserSystem::VirtualSlot38

In a multiplayer game, a firing system that was trying to fire tells the
others which target it stopped firing at, with
ET_STOP_FIRING_AT_TARGET_NOTIFY; then WeaponSystem stops the weapons aiming
at it.
================
*/
// FUNCTION: STBC 0x00574010
void PhaserSystem::VirtualSlot38( ObjectClass *target ) {
	WeaponSystem::StopFiringAtTarget( target );
	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 && tryingToFire ) {
		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetEventType( ET_STOP_FIRING_AT_TARGET_NOTIFY );
		event->SetDestination( this );
		event->SetObjPtr( target );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
PhaserSystem::StartFiringHandler

Starts firing at the event's target and point; the change is not sent on
while the event is handled.
================
*/
// FUNCTION: STBC 0x005740b0
void PhaserSystem::StartFiringHandler( TGEvent *event ) {
	StartFiringEvent *startEvent = (StartFiringEvent *)event;
	unknownA4 = false;
	NiPoint3 offset = *startEvent->GetOffset();
	VirtualSlot34( startEvent->GetTarget(), &offset );
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
PhaserSystem::StopFiringHandler

Stops firing; the change is not sent on while the event is handled.
================
*/
// FUNCTION: STBC 0x00574110
void PhaserSystem::StopFiringHandler( TGEvent *event ) {
	unknownA4 = false;
	StopFiring();
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
PhaserSystem::StopFiringAtTargetHandler

Stops firing at the event's object; the change is not sent on while the
event is handled.
================
*/
// FUNCTION: STBC 0x00574140
void PhaserSystem::StopFiringAtTargetHandler( TGEvent *event ) {
	unknownA4 = false;
	VirtualSlot38( (ObjectClass *)( (TGObjPtrEvent *)event )->GetObjPtr() );
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
PhaserSystem::SetPhaserLevelHandler

The event's character is the new power level.
================
*/
// FUNCTION: STBC 0x00574180
void PhaserSystem::SetPhaserLevelHandler( TGEvent *event ) {
	powerLevel = ( (TGCharEvent *)event )->GetChar();
	CallNextHandler( event );
}

/*
================
PhaserSystem::VirtualSlot26

After WeaponSystem's state, the power level.
================
*/
// FUNCTION: STBC 0x005741a0
void PhaserSystem::VirtualSlot26( TGStream *stream ) {
	WeaponSystem::VirtualSlot26( stream );

	stream->WriteChar( (char)powerLevel );
	stream->GetPos();
}

/*
================
PhaserSystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x005741d0
void PhaserSystem::VirtualSlot27( TGStream *stream ) {
	WeaponSystem::VirtualSlot27( stream );

	powerLevel = stream->ReadChar();
	stream->GetPos();
}

/*
================
PhaserSystem::SetPowerLevel

Tells the handlers of the new level, and sets every bank's power setting to
match: level 0 is a low setting, 1 medium and 2 high.
================
*/
// FUNCTION: STBC 0x00574200
void PhaserSystem::SetPowerLevel( int level ) {
	TGCharEvent *event = new TGCharEvent;
	event->SetChar( level );
	event->SetDestination( this );
	event->SetEventType( ET_SET_PHASER_LEVEL );
	g_kEventManager.AddEvent( event );

	for ( int i = 0; i < numChildSubsystems; i++ ) {
		PhaserBank *bank = PhaserBank::Cast( GetChildSubsystem( i ) );
		if ( bank != NULL ) {
			switch ( level ) {
			case 0:
				bank->SetPowerSetting( EnergyWeapon::EW_LOW );
				break;
			case 1:
				bank->SetPowerSetting( EnergyWeapon::EW_MEDIUM );
				break;
			case 2:
				bank->SetPowerSetting( EnergyWeapon::EW_HIGH );
				break;
			}
		}
	}
	powerLevel = level;
}
