/*
===============================================================================

	EngPowerCtrl.cpp

===============================================================================
*/

#include "EngPowerCtrl.h"
#include "ImpulseEngineSubsystem.h"
#include "PhaserSystem.h"
#include "PoweredSubsystem.h"
#include "PulseWeaponSystem.h"
#include "TorpedoSystem.h"
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGObjPtrEvent.h"
#include "KeyboardBinding.h"
#include "STSubPane.h"
#include "TGStream.h"
#include <iostream>
#include <stdio.h>
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "Game.h"
#include "UtopiaModule.h"
#include "TGFloatEvent.h"
#include "ShipSubsystem.h"
#include "STNumericBar.h"
#include "WarpEngineSubsystem.h"
#include "WeaponSystem.h"
#include "NiDynamicEffect.h"
#include "TGIntEvent.h"
#include "InterfaceModule.h"
#include "NiColor.h"
#include "ShieldClass.h"
#include "SensorSubsystem.h"
#include "STColors.h"
#include "TGMouseEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "GraphicsModeInfo.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0054f7e0 _$E512
// FUNCTION: STBC 0x0054f800 _$E511

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0054ceb0 _$E33
// FUNCTION: STBC 0x0054cec0 _$E32
// FUNCTION: STBC 0x0054ced0 _$E37
// FUNCTION: STBC 0x0054cee0 _$E36
// FUNCTION: STBC 0x0054cef0 _$E48
// FUNCTION: STBC 0x0054cf00 _$E47
// FUNCTION: STBC 0x0054cf10 _$E52
// FUNCTION: STBC 0x0054cf20 _$E51
// FUNCTION: STBC 0x0054cf30 _$E56
// FUNCTION: STBC 0x0054cf40 _$E55
// FUNCTION: STBC 0x0054cf50 _$E60
// FUNCTION: STBC 0x0054cf60 _$E59
// FUNCTION: STBC 0x0054cf70 _$E184
// FUNCTION: STBC 0x0054cf80 _$E183
// FUNCTION: STBC 0x0054dc70 _$E126
// FUNCTION: STBC 0x0054dc80 _$E125
// FUNCTION: STBC 0x0054e1d0 _$E139
// FUNCTION: STBC 0x0054e1e0 _$E138
// FUNCTION: STBC 0x0054e400 _$E189
// FUNCTION: STBC 0x0054e410 _$E188
// FUNCTION: STBC 0x0054ecb0 _$E194
// FUNCTION: STBC 0x0054ecc0 _$E193
// FUNCTION: STBC 0x0054ecd0 _$E198
// FUNCTION: STBC 0x0054ece0 _$E197
// FUNCTION: STBC 0x0054ecf0 _$E202
// FUNCTION: STBC 0x0054ed00 _$E201
// FUNCTION: STBC 0x0054ed10 _$E209
// FUNCTION: STBC 0x0054ed20 _$E208
// FUNCTION: STBC 0x0054ed30 _$E348
// FUNCTION: STBC 0x0054ed40 _$E347
// FUNCTION: STBC 0x0054ed50 _$E352
// FUNCTION: STBC 0x0054ed60 _$E351
// FUNCTION: STBC 0x0054ed70 _$E84
// FUNCTION: STBC 0x0054ed80 _$E83
// FUNCTION: STBC 0x0054ed90 _$E260
// FUNCTION: STBC 0x0054eda0 _$E259
// FUNCTION: STBC 0x0054edb0 _$E267
// FUNCTION: STBC 0x0054edc0 _$E266
// FUNCTION: STBC 0x0054edd0 _$E271
// FUNCTION: STBC 0x0054ede0 _$E270
// FUNCTION: STBC 0x0054edf0 _$E275
// FUNCTION: STBC 0x0054ee00 _$E274
// FUNCTION: STBC 0x0054ee10 _$E279
// FUNCTION: STBC 0x0054ee20 _$E278
// FUNCTION: STBC 0x0054ee30 _$E285
// FUNCTION: STBC 0x0054ee40 _$E284
// FUNCTION: STBC 0x0054ee50 _$E356
// FUNCTION: STBC 0x0054ee60 _$E355
// FUNCTION: STBC 0x0054ee70 _$E360
// FUNCTION: STBC 0x0054ee80 _$E359
// FUNCTION: STBC 0x0054ee90 _$E364
// FUNCTION: STBC 0x0054eea0 _$E363
// FUNCTION: STBC 0x0054eeb0 _$E368
// FUNCTION: STBC 0x0054eec0 _$E367
// FUNCTION: STBC 0x0054eed0 _$E372
// FUNCTION: STBC 0x0054eee0 _$E371
// FUNCTION: STBC 0x0054eef0 _$E376
// FUNCTION: STBC 0x0054ef00 _$E375
// FUNCTION: STBC 0x0054ef10 _$E380
// FUNCTION: STBC 0x0054ef20 _$E379
// FUNCTION: STBC 0x0054ef30 _$E384
// FUNCTION: STBC 0x0054ef40 _$E383
// FUNCTION: STBC 0x0054ef50 _$E388
// FUNCTION: STBC 0x0054ef60 _$E387
// FUNCTION: STBC 0x0054ef70 _$E392
// FUNCTION: STBC 0x0054ef80 _$E391
// FUNCTION: STBC 0x0054ef90 _$E89
// FUNCTION: STBC 0x0054efa0 _$E88
// FUNCTION: STBC 0x0054efb0 _$E396
// FUNCTION: STBC 0x0054efc0 _$E395
// FUNCTION: STBC 0x0054efd0 _$E94
// FUNCTION: STBC 0x0054efe0 _$E93
// FUNCTION: STBC 0x0054eff0 _$E400
// FUNCTION: STBC 0x0054f000 _$E399
// FUNCTION: STBC 0x0054f010 _$E404
// FUNCTION: STBC 0x0054f020 _$E403
// FUNCTION: STBC 0x0054f030 _$E408
// FUNCTION: STBC 0x0054f040 _$E407
// FUNCTION: STBC 0x0054f050 _$E412
// FUNCTION: STBC 0x0054f060 _$E411
// FUNCTION: STBC 0x0054f070 _$E103
// FUNCTION: STBC 0x0054f080 _$E102
// FUNCTION: STBC 0x0054f090 _$E416
// FUNCTION: STBC 0x0054f0a0 _$E415
// FUNCTION: STBC 0x0054f0b0 _$E420
// FUNCTION: STBC 0x0054f0c0 _$E419
// FUNCTION: STBC 0x0054f0d0 _$E424
// FUNCTION: STBC 0x0054f0e0 _$E423
// FUNCTION: STBC 0x0054f0f0 _$E109
// FUNCTION: STBC 0x0054f100 _$E108
// FUNCTION: STBC 0x0054f110 _$E114
// FUNCTION: STBC 0x0054f120 _$E113
// FUNCTION: STBC 0x0054f130 _$E144
// FUNCTION: STBC 0x0054f140 _$E143
// FUNCTION: STBC 0x0054f150 _$E428
// FUNCTION: STBC 0x0054f160 _$E427
// FUNCTION: STBC 0x0054f170 _$E149
// FUNCTION: STBC 0x0054f180 _$E148
// FUNCTION: STBC 0x0054f190 _$E432
// FUNCTION: STBC 0x0054f1a0 _$E431
// FUNCTION: STBC 0x0054f1b0 _$E436
// FUNCTION: STBC 0x0054f1c0 _$E435
// FUNCTION: STBC 0x0054f1d0 _$E154
// FUNCTION: STBC 0x0054f1e0 _$E153
// FUNCTION: STBC 0x0054f1f0 _$E440
// FUNCTION: STBC 0x0054f200 _$E439
// FUNCTION: STBC 0x0054f210 _$E119
// FUNCTION: STBC 0x0054f220 _$E118
// FUNCTION: STBC 0x0054f230 _$E444
// FUNCTION: STBC 0x0054f240 _$E443
// FUNCTION: STBC 0x0054f250 _$E448
// FUNCTION: STBC 0x0054f260 _$E447
// FUNCTION: STBC 0x0054f270 _$E159
// FUNCTION: STBC 0x0054f280 _$E158
// FUNCTION: STBC 0x0054f290 _$E164
// FUNCTION: STBC 0x0054f2a0 _$E163
// FUNCTION: STBC 0x0054f2b0 _$E452
// FUNCTION: STBC 0x0054f2c0 _$E451
// FUNCTION: STBC 0x0054f2d0 _$E456
// FUNCTION: STBC 0x0054f2e0 _$E455
// FUNCTION: STBC 0x0054f2f0 _$E460
// FUNCTION: STBC 0x0054f300 _$E459
// FUNCTION: STBC 0x0054f310 _$E464
// FUNCTION: STBC 0x0054f320 _$E463
// FUNCTION: STBC 0x0054f330 _$E468
// FUNCTION: STBC 0x0054f340 _$E467
// FUNCTION: STBC 0x0054f350 _$E472
// FUNCTION: STBC 0x0054f360 _$E471
// FUNCTION: STBC 0x0054f370 _$E476
// FUNCTION: STBC 0x0054f380 _$E475
// FUNCTION: STBC 0x0054f390 _$E480
// FUNCTION: STBC 0x0054f3a0 _$E479
// FUNCTION: STBC 0x0054f3b0 _$E169
// FUNCTION: STBC 0x0054f3c0 _$E168
// FUNCTION: STBC 0x0054f4d0 _$E484
// FUNCTION: STBC 0x0054f4e0 _$E483
// FUNCTION: STBC 0x0054f5c0 _$E174
// FUNCTION: STBC 0x0054f5d0 _$E173
// FUNCTION: STBC 0x0054f640 _$E179
// FUNCTION: STBC 0x0054f650 _$E178
// FUNCTION: STBC 0x0054f810 _$E488
// FUNCTION: STBC 0x0054f820 _$E487
// FUNCTION: STBC 0x0054f830 _$E493
// FUNCTION: STBC 0x0054f840 _$E492
// FUNCTION: STBC 0x0054f850 _$E497
// FUNCTION: STBC 0x0054f860 _$E496

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0054ce30 _$E300
// FUNCTION: STBC 0x0054ce40 _$E297
// FUNCTION: STBC 0x0054ce50 _$E299
// FUNCTION: STBC 0x0054ce60 _$E298
// FUNCTION: STBC 0x0054ce70 _$E306
// FUNCTION: STBC 0x0054ce80 _$E303
// FUNCTION: STBC 0x0054ce90 _$E305
// FUNCTION: STBC 0x0054cea0 _$E304

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0054cf90 _$E503
// FUNCTION: STBC 0x0054cfa0 _$E499
// FUNCTION: STBC 0x0054cfb0 _$E502
// FUNCTION: STBC 0x0054cfc0 _$E500

// The delay of the control's first refresh (invented name).
// DATA: STBC 0x008e44fc
static const float REFRESH_DELAY = 1.0f;

// A bar's scale: power from 0 to 125 percent of normal, in steps of 10
// percent; its height against the label's, and the empty part's grey
// (invented names).
#define MAX_POWER			1.25f
#define EMPTY_GRAY			( 117.0f / 255.0f )
#define BAR_HEIGHT_SCALE	1.75f
// The tick marks across the bars, every eighth of the bar's width
// (invented name).
#define TICK_STEP			0.08f

// DATA: STBC 0x0098ae68
TGEventHandlerTable EngPowerCtrl::classHandlers;

// DATA: STBC 0x0098aef8
EngPowerCtrl *EngPowerCtrl::instance;

/*
================
EngPowerCtrl::EngPowerCtrl

An empty pane of the given width that becomes the one control; it shows the
player's subsystems at once and a timer refreshes it.
================
*/
// FUNCTION: STBC 0x0054cfe0 ??0EngPowerCtrl@@QAE@M@Z
EngPowerCtrl::EngPowerCtrl( float width ) : TGPane( width, 0.0f ) {
	instance = this;
	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "EngPowerCtrl::HandleSetPlayer" );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		Unknown54D410( player );
	}

	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_ENG_POWER_REFRESH );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + REFRESH_DELAY );
	timer->SetEvent( event );
	g_kTimerManager.AddTimer( timer );
	timerID = timer->GetObjID();
}

/*
================
EngPowerCtrl::~EngPowerCtrl

The control is no more; its timer goes.
================
*/
// FUNCTION: STBC 0x0054d220 ??1EngPowerCtrl@@UAE@XZ
EngPowerCtrl::~EngPowerCtrl() {
	if ( instance == this ) {
		instance = NULL;
	}
	g_kTimerManager.DeleteTimer( timerID );
	barSubsystemIDs.RemoveAll();
}

/*
================
EngPowerCtrl::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0054d340
void EngPowerCtrl::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngPowerCtrl::HandleSetPlayer, "EngPowerCtrl::HandleSetPlayer" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngPowerCtrl::HandlePowerChange, "EngPowerCtrl::HandlePowerChange" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngPowerCtrl::HandlePeriodicRefresh, "EngPowerCtrl::HandlePeriodicRefresh" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngPowerCtrl::HandleKeyboard, "EngPowerCtrl::HandleKeyboard" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngPowerCtrl::HandleSelectOption, "EngPowerCtrl::HandleSelectOption" );
}

/*
================
EngPowerCtrl::RegisterHandlers

HandleSetPlayer is added for each instance, not to the class's table.
================
*/
// FUNCTION: STBC 0x0054d3b0
void EngPowerCtrl::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SUBSYSTEM_POWER_CHANGED, "EngPowerCtrl::HandlePowerChange" );
	AddMethodHandlerForClass( &classHandlers, ET_ENG_POWER_REFRESH, "EngPowerCtrl::HandlePeriodicRefresh" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "EngPowerCtrl::HandleKeyboard" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "EngPowerCtrl::HandleSelectOption" );
}

/*
================
EngPowerCtrl::Unknown54D410

Rebuilds the bars for the ship: one for the weapons (the phasers, else
the torpedoes, else the pulse weapons), one for the engines (impulse,
else warp), the sensors and the shields, each in its colour, stacked
top to bottom; then twelve tick marks across the bars at every tenth of
the power scale, every fifth one twice as wide.
================
*/
// FUNCTION: STBC 0x0054d410 ?Unknown54D410@EngPowerCtrl@@IAEXPAVShipClass@@@Z
// PARKED: x87 order in the tick loop: retail computes the tick's top before the percentage call and keeps it in a frame slot, where this build snapshots firstRow's top and height and computes it after the call (frame 8 bytes larger)
void EngPowerCtrl::Unknown54D410( ShipClass *ship ) {
	KillChildren();
	barSubsystemIDs.RemoveAll();

	TGPane *row = NULL;
	TGPane *firstRow = NULL;
	float height = 0.0f;
	PhaserSystem *phasers = ship->GetPhaserSystem();
	TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
	PulseWeaponSystem *pulseWeapons = ship->GetPulseWeaponSystem();
	if ( phasers != NULL ) {
		row = CreateBar( phasers, g_kEngineeringWeaponsColor );
		AddChild( row, 0.0f, 0.0f, false );
		height = row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		firstRow = row;
	} else if ( torpedoes != NULL ) {
		row = CreateBar( torpedoes, g_kEngineeringWeaponsColor );
		AddChild( row, 0.0f, 0.0f, false );
		height = row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		firstRow = row;
	} else if ( pulseWeapons != NULL ) {
		row = CreateBar( pulseWeapons, g_kEngineeringWeaponsColor );
		AddChild( row, 0.0f, 0.0f, false );
		height = row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		firstRow = row;
	}

	ImpulseEngineSubsystem *impulse = ship->GetImpulseEngineSubsystem();
	WarpEngineSubsystem *warp = ship->GetWarpEngineSubsystem();
	if ( impulse != NULL ) {
		row = CreateBar( impulse, g_kEngineeringEnginesColor );
		AddChild( row, 0.0f, height, false );
		height += row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		if ( firstRow == NULL ) {
			firstRow = row;
		}
	} else if ( warp != NULL ) {
		row = CreateBar( warp, g_kEngineeringEnginesColor );
		AddChild( row, 0.0f, height, false );
		height += row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		if ( firstRow == NULL ) {
			firstRow = row;
		}
	}

	SensorSubsystem *sensors = ship->GetSensorSubsystem();
	if ( sensors != NULL ) {
		row = CreateBar( sensors, g_kEngineeringSensorsColor );
		AddChild( row, 0.0f, height, false );
		height += row->GetHeight() + DEFAULT_ST_INDENT_VERT;
		if ( firstRow == NULL ) {
			firstRow = row;
		}
	}

	ShieldClass *shields = ship->GetShields();
	if ( shields != NULL ) {
		row = CreateBar( shields, g_kEngineeringShieldsColor );
		AddChild( row, 0.0f, height, false );
		height += row->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
		if ( firstRow == NULL ) {
			firstRow = row;
		}
	}
	Resize( GetWidth(), height, false );

	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	if ( pos == NULL || firstRow == NULL || row == NULL || firstRow == row ) {
		return;
	}
	STNumericBar *bar;
	long subsystemID;
	barSubsystemIDs.GetNext( pos, bar, subsystemID );
	NiPoint2 barOffset;
	NiPoint2 offset;
	bar->GetScreenOffset( &barOffset );
	GetScreenOffset( &offset );
	barOffset -= offset;
	float tickHeight = row->GetBottom() - row->GetHeight() * 0.5f - ( firstRow->GetHeight() * 0.5f + firstRow->GetTop() );
	for ( int i = 1; i < 13; i++ ) {
		TGIcon *tick = new TGIcon( mode->GetLcarsString(), 200, &g_kEngineeringCtrlBkgndLineColor );
		if ( i % 5 == 0 ) {
			tick->Resize( mode->GetPixelWidth() * 2.0f, tickHeight, false );
		} else {
			tick->Resize( mode->GetPixelWidth(), tickHeight, false );
		}
		float tickWidth = tick->GetWidth();
		float top = firstRow->GetTop() + firstRow->GetHeight() - firstRow->GetHeight() * 0.5f;
		float x = bar->GetPositionFromAbsolutePercentage( i * TICK_STEP ) + barOffset.x - tickWidth * 0.5f;
		AddChild( tick, x, top, false );
	}
}

/*
================
EngPowerCtrl::CreateBar

A row holding a power bar for the subsystem in the colour, labelled
"Weapons", "Engines" or with the subsystem's name, and filed in
barSubsystemIDs; its changes reach the control as
ET_SUBSYSTEM_POWER_CHANGED.
================
*/
// FUNCTION: STBC 0x0054d940
TGPane *EngPowerCtrl::CreateBar( PoweredSubsystem *subsystem, NiColorA color ) {
	float pixelWidth = GraphicsModeInfo::GetCurrentMode()->GetPixelWidth();
	TGPane *row = new TGPane;
	STNumericBar *bar = new STNumericBar( 0.0f, 0.0f );
	bar->SetRange( 0.0f, MAX_POWER );
	bar->SetMarkerValue( 1.0f );
	bar->SetValue( subsystem->GetPowerPercentageWanted() );
	bar->SetKeyInterval( 0.1f );
	bar->SetUseMarker( true );
	bar->SetUseButtons( false );
	bar->SetNormalColor( &color );
	NiColorA emptyColor( EMPTY_GRAY, EMPTY_GRAY, EMPTY_GRAY, 1.0f );
	bar->SetEmptyColor( &emptyColor );

	TGParagraph *label = TGParagraph::Cast( bar->GetNthChild( 0 ) );
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	TGString *text;
	if ( subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
		text = database->GetString( "Weapons" );
		label->SetString( text, true );
	} else if ( ImpulseEngineSubsystem::Cast( subsystem ) != NULL || WarpEngineSubsystem::Cast( subsystem ) != NULL ) {
		text = database->GetString( "Engines" );
		label->SetString( text, true );
	} else {
		TGLocalizationDatabase *names = g_kLocalizationManager.Load( "data/TGL/Subsystems.tgl", true );
		text = names->GetString( subsystem->GetName() );
		label->SetString( text, true );
		g_kLocalizationManager.Unload( names );
	}
	g_kLocalizationManager.Unload( database );
	label->Layout();
	label->SetColor( (NiColorA *)&NiColorA::BLACK );
	label->AddFuncHandlerForInstance( ET_MOUSE, "InterfaceModule::OverrideHandler" );

	bar->Resize( GetWidth() - pixelWidth * 4.0f, label->GetHeight() * BAR_HEIGHT_SCALE, false );
	TGFloatEvent *event = new TGFloatEvent;
	event->SetDestination( this );
	event->SetEventType( ET_SUBSYSTEM_POWER_CHANGED );
	bar->SetUpdateEvent( event );
	event->DecRefCount();
	barSubsystemIDs.SetAt( bar, subsystem->GetObjID() );

	row->AddChild( bar, pixelWidth * 2.0f, 0.0f, false );
	row->Resize( GetWidth(), bar->GetBottom(), false );
	return row;
}

/*
================
EngPowerCtrl::GetBarForSubsystem

The bar that shows the subsystem: the one filed under its ID, else the
first that shows a subsystem of the same kind (weapon system, or the
impulse and warp engines taken as each other's kind).
================
*/
// FUNCTION: STBC 0x0054dc90 ?GetBarForSubsystem@EngPowerCtrl@@QAEPAVSTNumericBar@@PAVShipSubsystem@@@Z
STNumericBar *EngPowerCtrl::GetBarForSubsystem( ShipSubsystem *subsystem ) {
	if ( subsystem == NULL ) {
		return NULL;
	}
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	while ( pos != NULL ) {
		STNumericBar *bar;
		long id;
		barSubsystemIDs.GetNext( pos, bar, id );
		if ( id != 0 && id == subsystem->GetObjID() ) {
			return bar;
		}
		if ( WeaponSystem::Cast( TGObject::GetTGObjectPtr( id ) ) != NULL && WeaponSystem::Cast( subsystem ) != NULL ) {
			return bar;
		}
		if ( ImpulseEngineSubsystem::Cast( TGObject::GetTGObjectPtr( id ) ) != NULL && WarpEngineSubsystem::Cast( subsystem ) != NULL ) {
			return bar;
		}
		if ( WarpEngineSubsystem::Cast( TGObject::GetTGObjectPtr( id ) ) != NULL && ImpulseEngineSubsystem::Cast( subsystem ) != NULL ) {
			return bar;
		}
	}
	return NULL;
}

/*
================
EngPowerCtrl::HandleSetPlayer

The new player ship gets a bar for each of its powered subsystems.
================
*/
// FUNCTION: STBC 0x0054ddb0
void EngPowerCtrl::HandleSetPlayer( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( ( (TGObjPtrEvent *)event )->GetObjPtr() );
	if ( ship != NULL ) {
		Unknown54D410( ship );
	}
}

/*
================
EngPowerCtrl::HandlePowerChange

A bar was set to a new power: the power its subsystem wants follows, and for
the weapon systems and the engines the whole group follows. A group member
is turned on when the power is above zero and off when it is not.
================
*/
// FUNCTION: STBC 0x0054dde0
// PARKED: 99.4, complete; retail copies the event's float through fld/fstp where this build uses an integer move (the FPU copy wall of Refresh)
void EngPowerCtrl::HandlePowerChange( TGEvent *event ) {
	STNumericBar *bar = STNumericBar::Cast( event->GetSource() );
	if ( bar == NULL ) {
		CallNextHandler( event );
		return;
	}
	long id;
	if ( !barSubsystemIDs.GetAt( bar, id ) ) {
		CallNextHandler( event );
		return;
	}
	PoweredSubsystem *subsystem = PoweredSubsystem::Cast( TGObject::GetTGObjectPtr( id ) );
	if ( subsystem == NULL ) {
		CallNextHandler( event );
		return;
	}

	float power = ( (TGFloatEvent *)event )->GetFloat();
	NiTList<PoweredSubsystem *> group( 2 );
	ShipClass *ship = subsystem->GetParentShip();
	if ( subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
		PhaserSystem *phasers = ship->GetPhaserSystem();
		TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
		PulseWeaponSystem *pulseWeapons = ship->GetPulseWeaponSystem();
		if ( phasers != NULL ) {
			group.AddTail( phasers );
		}
		if ( torpedoes != NULL ) {
			group.AddTail( torpedoes );
		}
		if ( pulseWeapons != NULL ) {
			group.AddTail( pulseWeapons );
		}
	} else if ( ImpulseEngineSubsystem::Cast( subsystem ) == NULL && WarpEngineSubsystem::Cast( subsystem ) == NULL ) {
		group.AddTail( subsystem );
	} else {
		ImpulseEngineSubsystem *impulse = ship->GetImpulseEngineSubsystem();
		WarpEngineSubsystem *warp = ship->GetWarpEngineSubsystem();
		if ( impulse != NULL ) {
			group.AddTail( impulse );
		}
		if ( warp != NULL ) {
			group.AddTail( warp );
		}
	}

	NiTListIterator pos = group.GetHeadPos();
	while ( pos ) {
		PoweredSubsystem *member = group.GetNext( pos );
		member->SetPowerPercentageWanted( power );
		if ( power > 0.0f ) {
			if ( !member->IsOn() ) {
				member->TurnOn();
			}
		} else if ( member->IsOn() == true ) {
			member->TurnOff();
		}
	}

	SendPowerChangedEvent( subsystem );
	CallNextHandler( event );
}

/*
================
EngPowerCtrl::HandlePeriodicRefresh

Refreshes the control and sets the timer again.
================
*/
// FUNCTION: STBC 0x0054e1f0
void EngPowerCtrl::HandlePeriodicRefresh( TGEvent *event ) {
	Refresh();
	TGEvent *refresh = new TGEvent;
	refresh->SetSource( this );
	refresh->SetDestination( this );
	refresh->SetEventType( ET_ENG_POWER_REFRESH );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + REFRESH_DELAY );
	timer->SetEvent( refresh );
	g_kTimerManager.AddTimer( timer );
	timerID = timer->GetObjID();
}

/*
================
EngPowerCtrl::HandleKeyboard

A key nobody above handled may be bound to an action.
================
*/
// FUNCTION: STBC 0x0054e2b0
void EngPowerCtrl::HandleKeyboard( TGKeyboardEvent *event ) {
	if ( !event->EventHandled() ) {
		if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), this ) ) {
			event->SetHandled();
		}
		if ( !event->EventHandled() ) {
			CallNextHandler( event );
		}
	}
}

/*
================
EngPowerCtrl::HandleSelectOption

The event's int moves the focus to the previous (-1) or next bar.
================
*/
// FUNCTION: STBC 0x0054e300 ?HandleSelectOption@EngPowerCtrl@@QAEXPAVTGEvent@@@Z
void EngPowerCtrl::HandleSelectOption( TGEvent *event ) {
	int direction = ( (TGIntEvent *)event )->GetInt();
	if ( direction != 0 ) {
		MoveSelection( direction == -1 );
	}
	CallNextHandler( event );
}

/*
================
EngPowerCtrl::MoveSelection

Focuses the nearest child before (previous) or after the focus that can
take the focus; past either end the parent moves its focus back instead.
================
*/
// FUNCTION: STBC 0x0054e330
void EngPowerCtrl::MoveSelection( bool previous ) {
	TGUIObject *focus = GetFocus();
	TGPane *container = parent;
	if ( previous == true ) {
		TGUIObject *child;
		for ( child = GetPrevChild( focus ); child != NULL; child = GetPrevChild( child ) ) {
			if ( InterfaceModule::HasUIElement( child, 0 ) ) {
				break;
			}
		}
		if ( child != NULL ) {
			SetFocus( child, true );
		} else if ( container != NULL ) {
			container->MoveFocus( -1, false, true );
		}
	} else {
		TGUIObject *child;
		for ( child = GetNextChild( focus ); child != NULL; child = GetNextChild( child ) ) {
			if ( InterfaceModule::HasUIElement( child, 0 ) ) {
				break;
			}
		}
		if ( child != NULL ) {
			SetFocus( child, true );
		} else if ( container != NULL ) {
			container->MoveFocus( -1, false, true );
		}
	}
}

/*
================
EngPowerCtrl::Refresh

Sets each bar to its subsystem's wanted power: the weapon systems' bar
shows the mean of the phasers, torpedoes and pulse weapons, the engines'
bar the mean of the impulse and warp engines; a bar whose subsystems are
all off shows none. A bar whose value changes tells the ship first.
================
*/
// FUNCTION: STBC 0x0054e420 ?Refresh@EngPowerCtrl@@QAEXXZ
void EngPowerCtrl::Refresh( void ) {
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	while ( pos ) {
		STNumericBar *bar;
		long id;
		barSubsystemIDs.GetNext( pos, bar, id );
		PoweredSubsystem *subsystem = PoweredSubsystem::Cast( TGObject::GetTGObjectPtr( id ) );
		if ( subsystem == NULL ) {
			continue;
		}

		ShipClass *ship = subsystem->GetParentShip();
		float power = 0.0f;
		if ( subsystem->IsTypeOf( WEAPONSYSTEM_TYPE ) ) {
			PhaserSystem *phasers = ship->GetPhaserSystem();
			TorpedoSystem *torpedoes = ship->GetTorpedoSystem();
			PulseWeaponSystem *pulseWeapons = ship->GetPulseWeaponSystem();
			int count = 0;
			bool allOff = true;
			if ( phasers != NULL ) {
				count = 1;
				power = phasers->GetPowerPercentageWanted();
				if ( phasers->IsOn() == true ) {
					allOff = false;
				}
			}
			if ( torpedoes != NULL ) {
				power += torpedoes->GetPowerPercentageWanted();
				count++;
				if ( torpedoes->IsOn() == true ) {
					allOff = false;
				}
			}
			if ( pulseWeapons != NULL ) {
				power += pulseWeapons->GetPowerPercentageWanted();
				count++;
				if ( pulseWeapons->IsOn() == true ) {
					allOff = false;
				}
			}
			power /= count;
			if ( allOff == true ) {
				power = 0.0f;
			}
		} else if ( subsystem->IsTypeOf( IMPULSEENGINESUBSYSTEM_TYPE ) || subsystem->IsTypeOf( WARPENGINESUBSYSTEM_TYPE ) ) {
			ImpulseEngineSubsystem *impulse = ship->GetImpulseEngineSubsystem();
			WarpEngineSubsystem *warp = ship->GetWarpEngineSubsystem();
			int count = 0;
			bool allOff = true;
			if ( impulse != NULL ) {
				count = 1;
				power = impulse->GetPowerPercentageWanted();
				if ( impulse->IsOn() == true ) {
					allOff = false;
				}
			}
			if ( warp != NULL ) {
				power += warp->GetPowerPercentageWanted();
				count++;
				if ( warp->IsOn() == true ) {
					allOff = false;
				}
			}
			power /= count;
			if ( allOff == true ) {
				power = 0.0f;
			}
		} else {
			power = subsystem->GetPowerPercentageWanted();
			if ( !subsystem->IsOn() ) {
				power = 0.0f;
			}
		}

		if ( bar->GetValue() != power ) {
			SendPowerChangedEvent( subsystem );
		}
		bar->SetValue( power );
		bar->UpdateGauge( true );
	}
}

/*
================
EngPowerCtrl::LostTrueFocusPath

The first bar takes the focus.
================
*/
// FUNCTION: STBC 0x0054e660
void EngPowerCtrl::LostTrueFocusPath( bool layout ) {
	TGUIObject::LostTrueFocusPath( false );
	SetFocus( GetFirstChild(), layout );
}

/*
================
EngPowerCtrl::SendPowerChangedEvent

Queues a float event with the subsystem's wanted power for its ship.
================
*/
// FUNCTION: STBC 0x0054e690
void EngPowerCtrl::SendPowerChangedEvent( PoweredSubsystem *subsystem ) {
	TGFloatEvent *event = new TGFloatEvent;
	event->SetSource( subsystem );
	event->SetDestination( subsystem->GetParentShip() );
	event->SetEventType( ET_SUBSYSTEM_POWER_CHANGED );
	event->SetFloat( subsystem->GetPowerPercentageWanted() );
	g_kEventManager.AddEvent( event );
}

/*
================
EngPowerCtrl::WriteToStream

The pane, then the IDs of each bar and of the subsystem it shows, and the
ID of the refresh timer.
================
*/
// FUNCTION: STBC 0x0054e700 ?WriteToStream@EngPowerCtrl@@UAE_NPAVTGStream@@@Z
bool EngPowerCtrl::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( barSubsystemIDs.GetCount() );
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	while ( pos ) {
		STNumericBar *bar;
		long id;
		barSubsystemIDs.GetNext( pos, bar, id );
		stream->WriteID( bar->GetObjID() );
		stream->WriteID( id );
	}
	stream->WriteID( timerID );
	return true;
}

/*
================
EngPowerCtrl::ReadFromStream

Reads what WriteToStream wrote; the control read is the one there is.
================
*/
// FUNCTION: STBC 0x0054e7d0 ?ReadFromStream@EngPowerCtrl@@UAE_NPAVTGStream@@@Z
bool EngPowerCtrl::ReadFromStream( TGStream *stream ) {
	barSubsystemIDs.RemoveAll();
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		STNumericBar *bar = (STNumericBar *)stream->ReadID();
		long id = stream->ReadID();
		barSubsystemIDs.SetAt( bar, id );
	}
	timerID = stream->ReadID();
	instance = this;
	return true;
}

/*
================
EngPowerCtrl::FixUpPointers

Every bar's ID must name an object that still exists.
================
*/
// FUNCTION: STBC 0x0054e920 ?FixUpPointers@EngPowerCtrl@@UAE_NXZ
bool EngPowerCtrl::FixUpPointers( void ) {
	if ( !TGUIObject::FixUpPointers() ) {
		return false;
	}
	bool ok = true;
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	while ( pos ) {
		STNumericBar *bar;
		long id;
		barSubsystemIDs.GetNext( pos, bar, id );
		if ( TGObject::GetTGObjectPtr( (long)bar ) == NULL ) {
			ok = false;
			break;
		}
	}
	return ok;
}

/*
================
EngPowerCtrl::PostFixUp

The map was loaded with the bars' IDs for keys: each entry is moved to a
map keyed by the bar itself, which then becomes the control's.
================
*/
// FUNCTION: STBC 0x0054e9b0 ?PostFixUp@EngPowerCtrl@@UAEXXZ
void EngPowerCtrl::PostFixUp( void ) {
	TGPane::PostFixUp();
	NiTMap<STNumericBar *, long> bars( 37 );
	NiTMapIterator pos = barSubsystemIDs.GetFirstPos();
	while ( pos ) {
		STNumericBar *bar;
		long subsystemID;
		barSubsystemIDs.GetNext( pos, bar, subsystemID );
		barSubsystemIDs.RemoveAt( bar );
		// A trace whose output was compiled out; only its formatting is left.
		char message[256];
		sprintf( message, "Fixing up EngPowerCtrl for %d bar and %d system\n", bar, subsystemID );
		bar = STNumericBar::Cast( TGObject::GetTGObjectPtr( (long)bar ) );
		bars.SetAt( bar, subsystemID );
	}
	barSubsystemIDs.RemoveAll();

	pos = bars.GetFirstPos();
	while ( pos ) {
		long subsystemID = 0;
		STNumericBar *bar = NULL;
		bars.GetNext( pos, bar, subsystemID );
		barSubsystemIDs.SetAt( bar, subsystemID );
	}
	bars.RemoveAll();
}

