/*
===============================================================================

	PoweredSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "PoweredSubsystem.h"
#include "PoweredSubsystemProperty.h"
#include "PowerSubsystem.h"
#include "ShipClass.h"
#include "TGBoolEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00563230 _$E479
// FUNCTION: STBC 0x00563250 _$E478

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00562360 _$E78
// FUNCTION: STBC 0x00562370 _$E77
// FUNCTION: STBC 0x00562410 _$E281
// FUNCTION: STBC 0x00562420 _$E280
// FUNCTION: STBC 0x00562a70 _$E89
// FUNCTION: STBC 0x00562a80 _$E88
// FUNCTION: STBC 0x00562a90 _$E93
// FUNCTION: STBC 0x00562aa0 _$E92
// FUNCTION: STBC 0x00562ab0 _$E97
// FUNCTION: STBC 0x00562ac0 _$E96
// FUNCTION: STBC 0x00562ad0 _$E101
// FUNCTION: STBC 0x00562ae0 _$E100
// FUNCTION: STBC 0x00562af0 _$E218
// FUNCTION: STBC 0x00562b00 _$E217
// FUNCTION: STBC 0x00562b10 _$E160
// FUNCTION: STBC 0x00562b20 _$E159
// FUNCTION: STBC 0x00562b30 _$E173
// FUNCTION: STBC 0x00562b40 _$E172
// FUNCTION: STBC 0x00562b50 _$E223
// FUNCTION: STBC 0x00562b60 _$E222
// FUNCTION: STBC 0x00562b70 _$E286
// FUNCTION: STBC 0x00562b80 _$E285
// FUNCTION: STBC 0x00562b90 _$E227
// FUNCTION: STBC 0x00562ba0 _$E226
// FUNCTION: STBC 0x00562bb0 _$E290
// FUNCTION: STBC 0x00562bc0 _$E289
// FUNCTION: STBC 0x00562bd0 _$E297
// FUNCTION: STBC 0x00562be0 _$E296
// FUNCTION: STBC 0x00562bf0 _$E315
// FUNCTION: STBC 0x00562c00 _$E314
// FUNCTION: STBC 0x00562c10 _$E319
// FUNCTION: STBC 0x00562c20 _$E318
// FUNCTION: STBC 0x00562c30 _$E118
// FUNCTION: STBC 0x00562c40 _$E117
// FUNCTION: STBC 0x00562c50 _$E243
// FUNCTION: STBC 0x00562c60 _$E242
// FUNCTION: STBC 0x00562c70 _$E250
// FUNCTION: STBC 0x00562c80 _$E249
// FUNCTION: STBC 0x00562c90 _$E254
// FUNCTION: STBC 0x00562ca0 _$E253
// FUNCTION: STBC 0x00562cb0 _$E258
// FUNCTION: STBC 0x00562cc0 _$E257
// FUNCTION: STBC 0x00562cd0 _$E262
// FUNCTION: STBC 0x00562ce0 _$E261
// FUNCTION: STBC 0x00562cf0 _$E268
// FUNCTION: STBC 0x00562d00 _$E267
// FUNCTION: STBC 0x00562d10 _$E323
// FUNCTION: STBC 0x00562d20 _$E322
// FUNCTION: STBC 0x00562d30 _$E327
// FUNCTION: STBC 0x00562d40 _$E326
// FUNCTION: STBC 0x00562d50 _$E331
// FUNCTION: STBC 0x00562d60 _$E330
// FUNCTION: STBC 0x00562d70 _$E335
// FUNCTION: STBC 0x00562d80 _$E334
// FUNCTION: STBC 0x00562d90 _$E339
// FUNCTION: STBC 0x00562da0 _$E338
// FUNCTION: STBC 0x00562db0 _$E343
// FUNCTION: STBC 0x00562dc0 _$E342
// FUNCTION: STBC 0x00562dd0 _$E347
// FUNCTION: STBC 0x00562de0 _$E346
// FUNCTION: STBC 0x00562df0 _$E351
// FUNCTION: STBC 0x00562e00 _$E350
// FUNCTION: STBC 0x00562e10 _$E355
// FUNCTION: STBC 0x00562e20 _$E354
// FUNCTION: STBC 0x00562e30 _$E359
// FUNCTION: STBC 0x00562e40 _$E358
// FUNCTION: STBC 0x00562e50 _$E123
// FUNCTION: STBC 0x00562e60 _$E122
// FUNCTION: STBC 0x00562e70 _$E363
// FUNCTION: STBC 0x00562e80 _$E362
// FUNCTION: STBC 0x00562e90 _$E128
// FUNCTION: STBC 0x00562ea0 _$E127
// FUNCTION: STBC 0x00562eb0 _$E367
// FUNCTION: STBC 0x00562ec0 _$E366
// FUNCTION: STBC 0x00562ed0 _$E371
// FUNCTION: STBC 0x00562ee0 _$E370
// FUNCTION: STBC 0x00562ef0 _$E375
// FUNCTION: STBC 0x00562f00 _$E374
// FUNCTION: STBC 0x00562f10 _$E379
// FUNCTION: STBC 0x00562f20 _$E378
// FUNCTION: STBC 0x00562f30 _$E137
// FUNCTION: STBC 0x00562f40 _$E136
// FUNCTION: STBC 0x00562f50 _$E383
// FUNCTION: STBC 0x00562f60 _$E382
// FUNCTION: STBC 0x00562f70 _$E387
// FUNCTION: STBC 0x00562f80 _$E386
// FUNCTION: STBC 0x00562f90 _$E391
// FUNCTION: STBC 0x00562fa0 _$E390
// FUNCTION: STBC 0x00562fb0 _$E143
// FUNCTION: STBC 0x00562fc0 _$E142
// FUNCTION: STBC 0x00562fd0 _$E148
// FUNCTION: STBC 0x00562fe0 _$E147
// FUNCTION: STBC 0x00562ff0 _$E178
// FUNCTION: STBC 0x00563000 _$E177
// FUNCTION: STBC 0x00563010 _$E395
// FUNCTION: STBC 0x00563020 _$E394
// FUNCTION: STBC 0x00563030 _$E183
// FUNCTION: STBC 0x00563040 _$E182
// FUNCTION: STBC 0x00563050 _$E399
// FUNCTION: STBC 0x00563060 _$E398
// FUNCTION: STBC 0x00563070 _$E403
// FUNCTION: STBC 0x00563080 _$E402
// FUNCTION: STBC 0x00563090 _$E188
// FUNCTION: STBC 0x005630a0 _$E187
// FUNCTION: STBC 0x005630b0 _$E407
// FUNCTION: STBC 0x005630c0 _$E406
// FUNCTION: STBC 0x005630d0 _$E153
// FUNCTION: STBC 0x005630e0 _$E152
// FUNCTION: STBC 0x005630f0 _$E411
// FUNCTION: STBC 0x00563100 _$E410
// FUNCTION: STBC 0x00563110 _$E415
// FUNCTION: STBC 0x00563120 _$E414
// FUNCTION: STBC 0x00563130 _$E193
// FUNCTION: STBC 0x00563140 _$E192
// FUNCTION: STBC 0x00563150 _$E198
// FUNCTION: STBC 0x00563160 _$E197
// FUNCTION: STBC 0x00563170 _$E419
// FUNCTION: STBC 0x00563180 _$E418
// FUNCTION: STBC 0x00563190 _$E423
// FUNCTION: STBC 0x005631a0 _$E422
// FUNCTION: STBC 0x005631b0 _$E427
// FUNCTION: STBC 0x005631c0 _$E426
// FUNCTION: STBC 0x005631d0 _$E431
// FUNCTION: STBC 0x005631e0 _$E430
// FUNCTION: STBC 0x005631f0 _$E435
// FUNCTION: STBC 0x00563200 _$E434
// FUNCTION: STBC 0x00563210 _$E439
// FUNCTION: STBC 0x00563220 _$E438
// FUNCTION: STBC 0x00563260 _$E443
// FUNCTION: STBC 0x00563270 _$E442
// FUNCTION: STBC 0x00563280 _$E447
// FUNCTION: STBC 0x00563290 _$E446
// FUNCTION: STBC 0x005632a0 _$E203
// FUNCTION: STBC 0x005632b0 _$E202
// FUNCTION: STBC 0x005632c0 _$E451
// FUNCTION: STBC 0x005632d0 _$E450
// FUNCTION: STBC 0x005632e0 _$E208
// FUNCTION: STBC 0x005632f0 _$E207
// FUNCTION: STBC 0x00563300 _$E213
// FUNCTION: STBC 0x00563310 _$E212
// FUNCTION: STBC 0x00563320 _$E455
// FUNCTION: STBC 0x00563330 _$E454
// FUNCTION: STBC 0x00563340 _$E460
// FUNCTION: STBC 0x00563350 _$E459
// FUNCTION: STBC 0x00563360 _$E464
// FUNCTION: STBC 0x00563370 _$E463

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00562130 _$E32
// FUNCTION: STBC 0x00562140 _$E29
// FUNCTION: STBC 0x00562150 _$E31
// FUNCTION: STBC 0x00562160 _$E30
// FUNCTION: STBC 0x00562170 _$E38
// FUNCTION: STBC 0x00562180 _$E35
// FUNCTION: STBC 0x00562190 _$E37
// FUNCTION: STBC 0x005621a0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005621c0 _$E470
// FUNCTION: STBC 0x005621d0 _$E466
// FUNCTION: STBC 0x005621e0 _$E469
// FUNCTION: STBC 0x005621f0 _$E467

// DATA: STBC 0x0098bc38
TGEventHandlerTable PoweredSubsystem::classHandlers;

/*
================
PoweredSubsystem::GetProperty
================
*/
// FUNCTION: STBC 0x005621b0
PoweredSubsystemProperty *PoweredSubsystem::GetProperty( void ) {
	return (PoweredSubsystemProperty *)property;
}

/*
================
PoweredSubsystem::Cast

The object as a PoweredSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00562210
PoweredSubsystem *PoweredSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( POWEREDSUBSYSTEM_TYPE ) ) {
		return (PoweredSubsystem *)object;
	}
	return NULL;
}

/*
================
PoweredSubsystem::PoweredSubsystem

A subsystem that is on and wants all of its normal power.
================
*/
// FUNCTION: STBC 0x00562240
PoweredSubsystem::PoweredSubsystem( long id ) : ShipSubsystem( id ) {
	powerReceived = 0.0f;
	powerWanted = 0.0f;
	powerPercentageWanted = 1.0f;
	normalPowerPercentage = 1.0f;
	powerPercentage = 1.0f;
	on = true;
	powerMode = POWER_MODE_DEDUCT;
}

/*
================
PoweredSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x00562290
int PoweredSubsystem::GetObjType( void ) {
	return POWEREDSUBSYSTEM_TYPE;
}

/*
================
PoweredSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x005622f0
char *PoweredSubsystem::GetClassName( void ) {
	return "PoweredSubsystem";
}

/*
================
PoweredSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00562300
char *PoweredSubsystem::GetSwigTypeName( void ) {
	return "_p_PoweredSubsystem";
}

/*
================
PoweredSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00562310
char *PoweredSubsystem::GetPythonClassName( void ) {
	return "PoweredSubsystemPtr";
}

/*
================
PoweredSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00562320
TGEventHandlerTable *PoweredSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PoweredSubsystem::~PoweredSubsystem
================
*/
// FUNCTION: STBC 0x00562380
// FUNCTION: STBC 0x00562330 ??_GPoweredSubsystem@@UAEPAXI@Z
PoweredSubsystem::~PoweredSubsystem( void ) {
}

/*
================
PoweredSubsystem::SetParentShip

The power wanted follows the new ship's normal power.
================
*/
// FUNCTION: STBC 0x00562390
void PoweredSubsystem::SetParentShip( ShipClass *ship ) {
	ShipSubsystem::SetParentShip( ship );
	// Named as Update names it: retail copies the setting through fld/fstp
	// into the spill before the call, which needs this copy level.
	float percentageWanted = GetPowerPercentageWanted();
	powerWanted = percentageWanted * GetNormalPowerWanted();
}

/*
================
PoweredSubsystem::SetPowerSource

The power subsystem adds this one to those it powers.
================
*/
// FUNCTION: STBC 0x005623c0
void PoweredSubsystem::SetPowerSource( PowerSubsystem *source ) {
	source->AddPoweredSubsystem( this );
}

/*
================
PoweredSubsystem::GetNormalPowerWanted

The property's normal power per second; none while the subsystem is
disabled or off, or without a property.
================
*/
// FUNCTION: STBC 0x005623d0
float PoweredSubsystem::GetNormalPowerWanted( void ) {
	if ( IsDisabled() != true && on && property != NULL ) {
		return GetProperty()->GetNormalPowerPerSecond();
	}
	return 0.0f;
}

/*
================
PoweredSubsystem::SetPowerPercentageWanted

Scales the power wanted to the new percentage.
================
*/
// FUNCTION: STBC 0x00562430
void PoweredSubsystem::SetPowerPercentageWanted( float percentage ) {
	float oldPercentage = powerPercentageWanted;
	powerPercentageWanted = percentage;
	if ( oldPercentage != 0.0f ) {
		SetPowerWanted( GetPowerWanted() * percentage / oldPercentage );
	}
}

/*
================
PoweredSubsystem::Update

Asks the ship's power subsystem for the power the subsystem wants this
step, the normal power scaled by the percentage wanted, and records how
much of it and of its normal power it received.
================
*/
// FUNCTION: STBC 0x00562470
void PoweredSubsystem::Update( float deltaTime ) {
	ShipSubsystem::Update( deltaTime );

	float percentageWanted = GetPowerPercentageWanted();
	float power = percentageWanted * GetNormalPowerWanted() * deltaTime;
	powerWanted = power;

	PowerSubsystem *powerSubsystem = parentShip->GetPowerSubsystem();
	if ( powerMode == POWER_MODE_DEDUCT ) {
		powerReceived = powerSubsystem->DeductPower( power );
	} else if ( powerMode == POWER_MODE_STEAL ) {
		powerReceived = powerSubsystem->StealPower( power );
	} else {
		powerReceived = powerSubsystem->StealPowerFromReserve( power );
	}

	if ( powerWanted > 0.0f ) {
		powerPercentage = powerReceived / powerWanted;
	} else {
		powerPercentage = 0.0f;
	}

	float length = GetLastUpdateLength();
	float normalPower = length * GetNormalPowerWanted();
	if ( normalPower <= 0.0f ) {
		normalPowerPercentage = 1.0f;
	} else {
		normalPowerPercentage = powerReceived / normalPower;
	}
}

/*
================
PoweredSubsystem::TurnOn

Tells the ship that the subsystem's state changed; in multiplayer the
subsystem may also tell its own handlers.
================
*/
// FUNCTION: STBC 0x00562560
void PoweredSubsystem::TurnOn( void ) {
	if ( on ) {
		return;
	}
	on = true;

	TGBoolEvent *event = new TGBoolEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetBool( true );
	event->SetEventType( ET_SUBSYSTEM_STATE_CHANGED );
	g_kEventManager.AddEvent( event );

	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 ) {
		TGBoolEvent *notify = new TGBoolEvent;
		notify->SetBool( true );
		notify->SetEventType( ET_SUBSYSTEM_STATE_CHANGED_NOTIFY );
		notify->SetDestination( this );
		g_kEventManager.AddEvent( notify );
	}
}

/*
================
PoweredSubsystem::TurnOff
================
*/
// FUNCTION: STBC 0x00562630
void PoweredSubsystem::TurnOff( void ) {
	if ( !on ) {
		return;
	}
	on = false;

	TGBoolEvent *event = new TGBoolEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetBool( false );
	event->SetEventType( ET_SUBSYSTEM_STATE_CHANGED );
	g_kEventManager.AddEvent( event );

	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 ) {
		TGBoolEvent *notify = new TGBoolEvent;
		notify->SetBool( false );
		notify->SetEventType( ET_SUBSYSTEM_STATE_CHANGED_NOTIFY );
		notify->SetDestination( this );
		g_kEventManager.AddEvent( notify );
	}
}

/*
================
PoweredSubsystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00562710
void PoweredSubsystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PoweredSubsystem::StateChangedHandler, "PoweredSubsystem::StateChangedHandler" );
}

/*
================
PoweredSubsystem::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00562730
void PoweredSubsystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SUBSYSTEM_STATE_CHANGED, "PoweredSubsystem::StateChangedHandler" );
}

/*
================
PoweredSubsystem::StateChangedHandler

Switches the subsystem on or off as the event says. The change is not
announced again while it is made.
================
*/
// FUNCTION: STBC 0x00562750
void PoweredSubsystem::StateChangedHandler( TGEvent *event ) {
	unknownA4 = false;
	if ( ( (TGBoolEvent *)event )->GetBool() == true ) {
		TurnOn();
	} else {
		TurnOff();
	}
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
PoweredSubsystem::WriteToStream
================
*/
// FUNCTION: STBC 0x005627b0
bool PoweredSubsystem::WriteToStream( TGStream *stream ) {
	if ( !ShipSubsystem::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( powerReceived );
	stream->WriteFloat( powerWanted );
	stream->WriteFloat( powerPercentageWanted );
	stream->WriteFloat( powerPercentage );
	stream->WriteFloat( normalPowerPercentage );
	stream->WriteBool( on );
	stream->WriteInt( powerMode );
	return true;
}

/*
================
PoweredSubsystem::ReadFromStream
================
*/
// FUNCTION: STBC 0x00562830
bool PoweredSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !ShipSubsystem::ReadFromStream( stream ) ) {
		return false;
	}
	powerReceived = stream->ReadFloat();
	powerWanted = stream->ReadFloat();
	powerPercentageWanted = stream->ReadFloat();
	powerPercentage = stream->ReadFloat();
	normalPowerPercentage = stream->ReadFloat();
	on = stream->ReadBool();
	powerMode = stream->ReadInt();
	return true;
}

/*
================
PoweredSubsystem::VirtualSlot26

After ShipSubsystem's state, the power percentage wanted, in percent,
negative while the subsystem is off.
================
*/
// FUNCTION: STBC 0x005628b0
void PoweredSubsystem::VirtualSlot26( TGStream *stream ) {
	ShipSubsystem::VirtualSlot26( stream );

	int power = (int)( powerPercentageWanted * 100.0f );
	if ( !on ) {
		power = -power;
	}
	stream->WriteChar( (char)power );
	stream->GetPos();
}

/*
================
PoweredSubsystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x00562900
void PoweredSubsystem::VirtualSlot27( TGStream *stream ) {
	ShipSubsystem::VirtualSlot27( stream );

	int power = stream->ReadChar();
	if ( power <= 0 ) {
		on = false;
		power = -power;
	} else {
		on = true;
	}
	powerPercentageWanted = power / 100.0f;
	stream->GetPos();
}

/*
================
PoweredSubsystem::VirtualSlot28

After ShipSubsystem's state, whether the power percentage wanted follows,
and unless the flag is set, the percentage itself.
================
*/
// FUNCTION: STBC 0x00562960
void PoweredSubsystem::VirtualSlot28( TGStream *stream, bool flag ) {
	ShipSubsystem::VirtualSlot28( stream, flag );

	if ( !flag ) {
		stream->WriteBool( true );
		stream->WriteChar( (char)(int)( powerPercentageWanted * 100.0f ) );
	} else {
		stream->WriteBool( false );
	}
	stream->GetPos();
}

/*
================
PoweredSubsystem::VirtualSlot29

Reads what VirtualSlot28 writes; the percentage is taken only when the
state is newer than the last one taken.
================
*/
// FUNCTION: STBC 0x005629d0
void PoweredSubsystem::VirtualSlot29( TGStream *stream, float time ) {
	float lastTime = unknown84;
	ShipSubsystem::VirtualSlot29( stream, time );

	if ( stream->ReadBool() ) {
		int power = stream->ReadChar();
		if ( time > lastTime ) {
			SetPowerPercentageWanted( power / 100.0f );
		}
	}
	stream->GetPos();
}

/*
================
PoweredSubsystem::VirtualSlot33

Takes the power percentage wanted, when the time given is newer than the
last state taken.
================
*/
// FUNCTION: STBC 0x00562a40
bool PoweredSubsystem::VirtualSlot33( float percentage, float time ) {
	if ( time > unknown84 ) {
		unknown84 = time;
		SetPowerPercentageWanted( percentage );
	}
	return true;
}
