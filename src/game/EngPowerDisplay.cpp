/*
===============================================================================

	EngPowerDisplay.cpp

===============================================================================
*/

#include "EngPowerDisplay.h"
#include "Game.h"
#include "PyEmbed.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include <iostream>
#include "PowerSubsystem.h"
#include "ShipClass.h"
#include "ShipSubsystem.h"
#include "STColors.h"
#include "STFillGauge.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005504a0 _$E500
// FUNCTION: STBC 0x005504c0 _$E499

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0054f8f0 _$E33
// FUNCTION: STBC 0x0054f900 _$E32
// FUNCTION: STBC 0x0054f910 _$E37
// FUNCTION: STBC 0x0054f920 _$E36
// FUNCTION: STBC 0x0054fd00 _$E48
// FUNCTION: STBC 0x0054fd10 _$E47
// FUNCTION: STBC 0x0054fd20 _$E52
// FUNCTION: STBC 0x0054fd30 _$E51
// FUNCTION: STBC 0x0054fd40 _$E56
// FUNCTION: STBC 0x0054fd50 _$E55
// FUNCTION: STBC 0x0054fd60 _$E60
// FUNCTION: STBC 0x0054fd70 _$E59
// FUNCTION: STBC 0x0054fd80 _$E184
// FUNCTION: STBC 0x0054fd90 _$E183
// FUNCTION: STBC 0x0054fda0 _$E126
// FUNCTION: STBC 0x0054fdb0 _$E125
// FUNCTION: STBC 0x0054fdc0 _$E139
// FUNCTION: STBC 0x0054fdd0 _$E138
// FUNCTION: STBC 0x0054fde0 _$E189
// FUNCTION: STBC 0x0054fdf0 _$E188
// FUNCTION: STBC 0x0054fe00 _$E194
// FUNCTION: STBC 0x0054fe10 _$E193
// FUNCTION: STBC 0x0054fe20 _$E198
// FUNCTION: STBC 0x0054fe30 _$E197
// FUNCTION: STBC 0x0054fe40 _$E202
// FUNCTION: STBC 0x0054fe50 _$E201
// FUNCTION: STBC 0x0054fe60 _$E209
// FUNCTION: STBC 0x0054fe70 _$E208
// FUNCTION: STBC 0x0054fe80 _$E336
// FUNCTION: STBC 0x0054fe90 _$E335
// FUNCTION: STBC 0x0054fea0 _$E340
// FUNCTION: STBC 0x0054feb0 _$E339
// FUNCTION: STBC 0x0054fec0 _$E84
// FUNCTION: STBC 0x0054fed0 _$E83
// FUNCTION: STBC 0x0054fee0 _$E293
// FUNCTION: STBC 0x0054fef0 _$E292
// FUNCTION: STBC 0x0054ff00 _$E300
// FUNCTION: STBC 0x0054ff10 _$E299
// FUNCTION: STBC 0x0054ff20 _$E304
// FUNCTION: STBC 0x0054ff30 _$E303
// FUNCTION: STBC 0x0054ff40 _$E308
// FUNCTION: STBC 0x0054ff50 _$E307
// FUNCTION: STBC 0x0054ff60 _$E312
// FUNCTION: STBC 0x0054ff70 _$E311
// FUNCTION: STBC 0x0054ff80 _$E318
// FUNCTION: STBC 0x0054ff90 _$E317
// FUNCTION: STBC 0x0054ffa0 _$E344
// FUNCTION: STBC 0x0054ffb0 _$E343
// FUNCTION: STBC 0x0054ffc0 _$E348
// FUNCTION: STBC 0x0054ffd0 _$E347
// FUNCTION: STBC 0x0054ffe0 _$E352
// FUNCTION: STBC 0x0054fff0 _$E351
// FUNCTION: STBC 0x00550000 _$E356
// FUNCTION: STBC 0x00550010 _$E355
// FUNCTION: STBC 0x00550020 _$E360
// FUNCTION: STBC 0x00550030 _$E359
// FUNCTION: STBC 0x00550040 _$E364
// FUNCTION: STBC 0x00550050 _$E363
// FUNCTION: STBC 0x00550060 _$E368
// FUNCTION: STBC 0x00550070 _$E367
// FUNCTION: STBC 0x00550080 _$E372
// FUNCTION: STBC 0x00550090 _$E371
// FUNCTION: STBC 0x005500a0 _$E376
// FUNCTION: STBC 0x005500b0 _$E375
// FUNCTION: STBC 0x005500c0 _$E380
// FUNCTION: STBC 0x005500d0 _$E379
// FUNCTION: STBC 0x005500e0 _$E89
// FUNCTION: STBC 0x005500f0 _$E88
// FUNCTION: STBC 0x00550100 _$E384
// FUNCTION: STBC 0x00550110 _$E383
// FUNCTION: STBC 0x00550120 _$E94
// FUNCTION: STBC 0x00550130 _$E93
// FUNCTION: STBC 0x00550140 _$E388
// FUNCTION: STBC 0x00550150 _$E387
// FUNCTION: STBC 0x00550160 _$E392
// FUNCTION: STBC 0x00550170 _$E391
// FUNCTION: STBC 0x00550180 _$E396
// FUNCTION: STBC 0x00550190 _$E395
// FUNCTION: STBC 0x005501a0 _$E400
// FUNCTION: STBC 0x005501b0 _$E399
// FUNCTION: STBC 0x005501c0 _$E103
// FUNCTION: STBC 0x005501d0 _$E102
// FUNCTION: STBC 0x005501e0 _$E404
// FUNCTION: STBC 0x005501f0 _$E403
// FUNCTION: STBC 0x00550200 _$E408
// FUNCTION: STBC 0x00550210 _$E407
// FUNCTION: STBC 0x00550220 _$E412
// FUNCTION: STBC 0x00550230 _$E411
// FUNCTION: STBC 0x00550240 _$E109
// FUNCTION: STBC 0x00550250 _$E108
// FUNCTION: STBC 0x00550260 _$E114
// FUNCTION: STBC 0x00550270 _$E113
// FUNCTION: STBC 0x00550280 _$E144
// FUNCTION: STBC 0x00550290 _$E143
// FUNCTION: STBC 0x005502a0 _$E416
// FUNCTION: STBC 0x005502b0 _$E415
// FUNCTION: STBC 0x005502c0 _$E149
// FUNCTION: STBC 0x005502d0 _$E148
// FUNCTION: STBC 0x005502e0 _$E420
// FUNCTION: STBC 0x005502f0 _$E419
// FUNCTION: STBC 0x00550300 _$E424
// FUNCTION: STBC 0x00550310 _$E423
// FUNCTION: STBC 0x00550320 _$E154
// FUNCTION: STBC 0x00550330 _$E153
// FUNCTION: STBC 0x00550340 _$E428
// FUNCTION: STBC 0x00550350 _$E427
// FUNCTION: STBC 0x00550360 _$E119
// FUNCTION: STBC 0x00550370 _$E118
// FUNCTION: STBC 0x00550380 _$E432
// FUNCTION: STBC 0x00550390 _$E431
// FUNCTION: STBC 0x005503a0 _$E436
// FUNCTION: STBC 0x005503b0 _$E435
// FUNCTION: STBC 0x005503c0 _$E159
// FUNCTION: STBC 0x005503d0 _$E158
// FUNCTION: STBC 0x005503e0 _$E164
// FUNCTION: STBC 0x005503f0 _$E163
// FUNCTION: STBC 0x00550400 _$E440
// FUNCTION: STBC 0x00550410 _$E439
// FUNCTION: STBC 0x00550420 _$E444
// FUNCTION: STBC 0x00550430 _$E443
// FUNCTION: STBC 0x00550440 _$E448
// FUNCTION: STBC 0x00550450 _$E447
// FUNCTION: STBC 0x00550460 _$E452
// FUNCTION: STBC 0x00550470 _$E451
// FUNCTION: STBC 0x00550480 _$E456
// FUNCTION: STBC 0x00550490 _$E455
// FUNCTION: STBC 0x005504d0 _$E460
// FUNCTION: STBC 0x005504e0 _$E459
// FUNCTION: STBC 0x005504f0 _$E464
// FUNCTION: STBC 0x00550500 _$E463
// FUNCTION: STBC 0x00550510 _$E468
// FUNCTION: STBC 0x00550520 _$E467
// FUNCTION: STBC 0x00550530 _$E169
// FUNCTION: STBC 0x00550540 _$E168
// FUNCTION: STBC 0x00550550 _$E472
// FUNCTION: STBC 0x00550560 _$E471
// FUNCTION: STBC 0x00550570 _$E174
// FUNCTION: STBC 0x00550580 _$E173
// FUNCTION: STBC 0x00550590 _$E179
// FUNCTION: STBC 0x005505a0 _$E178
// FUNCTION: STBC 0x005505b0 _$E476
// FUNCTION: STBC 0x005505c0 _$E475
// FUNCTION: STBC 0x005505d0 _$E481
// FUNCTION: STBC 0x005505e0 _$E480
// FUNCTION: STBC 0x005505f0 _$E485
// FUNCTION: STBC 0x00550600 _$E484

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0054f870 _$E259
// FUNCTION: STBC 0x0054f880 _$E256
// FUNCTION: STBC 0x0054f890 _$E258
// FUNCTION: STBC 0x0054f8a0 _$E257
// FUNCTION: STBC 0x0054f8b0 _$E265
// FUNCTION: STBC 0x0054f8c0 _$E262
// FUNCTION: STBC 0x0054f8d0 _$E264
// FUNCTION: STBC 0x0054f8e0 _$E263

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0054f930 _$E491
// FUNCTION: STBC 0x0054f940 _$E487
// FUNCTION: STBC 0x0054f950 _$E490
// FUNCTION: STBC 0x0054f960 _$E488
// DATA: STBC 0x0098af30
TGEventHandlerTable EngPowerDisplay::classHandlers;

// DATA: STBC 0x0098b044
EngPowerDisplay *EngPowerDisplay::instance;

/*
================
EngPowerDisplay::Cast

The object as an EngPowerDisplay; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0054f980
EngPowerDisplay *EngPowerDisplay::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ENGPOWERDISPLAY_TYPE ) ) {
		return (EngPowerDisplay *)object;
	}
	return NULL;
}

/*
================
EngPowerDisplay::EngPowerDisplay

An empty pane of the given size; the script fills it, and it becomes the one
display.
================
*/
// FUNCTION: STBC 0x0054f9b0 ??0EngPowerDisplay@@QAE@MM@Z
EngPowerDisplay::EngPowerDisplay( float width, float height ) : TGPane( width, height ) {
	CallInitScript();
	instance = this;
}

/*
================
EngPowerDisplay::~EngPowerDisplay

The display is no more.
================
*/
// FUNCTION: STBC 0x0054faf0 ??1EngPowerDisplay@@UAE@XZ
EngPowerDisplay::~EngPowerDisplay() {
	if ( instance == this ) {
		instance = NULL;
	}
}

/*
================
EngPowerDisplay::RegisterHandlerNames

The display has no handler methods of its own.
================
*/
// FUNCTION: STBC 0x0054fb10
void EngPowerDisplay::RegisterHandlerNames( void ) {
}

/*
================
EngPowerDisplay::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0054fb20
void EngPowerDisplay::RegisterHandlers( void ) {
}

/*
================
EngPowerDisplay::CallInitScript

Has the "Bridge.PowerDisplay" script build the display's gauges.
================
*/
// FUNCTION: STBC 0x0054fb30
void EngPowerDisplay::CallInitScript( void ) {
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Bridge.PowerDisplay", "Init", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
EngPowerDisplay::CreateBatteryGauge

A gauge on the player's power subsystem: 0 shows the main battery, 1 the
backup battery and 2 the warp core's condition.
================
*/
// FUNCTION: STBC 0x0054fb70 ?CreateBatteryGauge@EngPowerDisplay@@QAEPAVSTFillGauge@@H@Z
STFillGauge *EngPowerDisplay::CreateBatteryGauge( int which ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return NULL;
	}
	PowerSubsystem *power = player->GetPowerSubsystem();
	if ( power == NULL ) {
		return NULL;
	}
	STFillGauge *gauge = new STFillGauge( 200, NULL, false );
	switch ( which ) {
	case 0:
		gauge->valueFunction = (GaugeValueFunction)&PowerSubsystem::GetMainBatteryPower;
		gauge->maxFunction = (GaugeValueFunction)&PowerSubsystem::GetMainBatteryLimit;
		gauge->SetFillColor( g_kEngineeringMainPowerColor );
		gauge->SetEmptyColor( NiColorA::BLACK );
		break;
	case 1:
		gauge->valueFunction = (GaugeValueFunction)&PowerSubsystem::GetBackupBatteryPower;
		gauge->maxFunction = (GaugeValueFunction)&PowerSubsystem::GetBackupBatteryLimit;
		gauge->SetFillColor( g_kEngineeringBackupPowerColor );
		gauge->SetEmptyColor( NiColorA::BLACK );
		break;
	case 2:
		gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCondition;
		gauge->maxFunction = (GaugeValueFunction)&ShipSubsystem::GetMaxCondition;
		gauge->SetFillColor( g_kEngineeringWarpCoreColor );
		gauge->SetEmptyColor( NiColorA::BLACK );
		break;
	}
	gauge->SetFillDirection( STFillGauge::BOTTOM_TO_TOP );
	gauge->alternateFunction = NULL;
	gauge->SetObject( power );
	return gauge;
}

/*
================
EngPowerDisplay::ReadFromStream

The pane; the display read is the one there is.
================
*/
// FUNCTION: STBC 0x0054fcd0 ?ReadFromStream@EngPowerDisplay@@UAE_NPAVTGStream@@@Z
bool EngPowerDisplay::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	instance = this;
	return true;
}
