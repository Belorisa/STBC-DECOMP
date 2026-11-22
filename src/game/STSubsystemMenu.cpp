/*
===============================================================================

	STSubsystemMenu.cpp

	STSubsystemMenu, STComponentMenu and STComponentMenuItem: the target
	list's menus of a ship's subsystems.

===============================================================================
*/

#include <iostream>
#include "STSubsystemMenu.h"
#include "STComponentMenu.h"
#include "STComponentMenuItem.h"
#include "STFillGauge.h"
#include "STSubPane.h"
#include "STColors.h"
#include "ShipClass.h"
#include "ShipProperty.h"
#include "ShipSubsystem.h"
#include "SensorSubsystem.h"
#include "SubsystemProperty.h"
#include "STButton.h"
#include "TGIcon.h"
#include "HullClass.h"
#include "WeaponSystem.h"
#include "PhaserSystem.h"
#include "PulseWeaponSystem.h"
#include "TorpedoSystem.h"
#include "TractorBeamSystem.h"
#include "ObjectClass.h"
#include "Game.h"
#include "InterfaceModule.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00534e90 _$E32
// FUNCTION: STBC 0x00534ea0 _$E29
// FUNCTION: STBC 0x00534eb0 _$E31
// FUNCTION: STBC 0x00534ec0 _$E30
// FUNCTION: STBC 0x00534ed0 _$E38
// FUNCTION: STBC 0x00534ee0 _$E35
// FUNCTION: STBC 0x00534ef0 _$E37
// FUNCTION: STBC 0x00534f00 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00534ff0 _$E496
// FUNCTION: STBC 0x00535000 _$E492
// FUNCTION: STBC 0x00535010 _$E495
// FUNCTION: STBC 0x00535020 _$E493

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00535040 _$E501
// FUNCTION: STBC 0x00535050 _$E498
// FUNCTION: STBC 0x00535060 _$E500
// FUNCTION: STBC 0x00535070 _$E499

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00535090 _$E506
// FUNCTION: STBC 0x005350a0 _$E503
// FUNCTION: STBC 0x005350b0 _$E505
// FUNCTION: STBC 0x005350c0 _$E504

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00534f10 _$E57
// FUNCTION: STBC 0x00534f20 _$E56
// FUNCTION: STBC 0x00534f30 _$E61
// FUNCTION: STBC 0x00534f40 _$E60
// FUNCTION: STBC 0x00534f50 _$E72
// FUNCTION: STBC 0x00534f60 _$E71
// FUNCTION: STBC 0x00534f70 _$E76
// FUNCTION: STBC 0x00534f80 _$E75
// FUNCTION: STBC 0x00534f90 _$E80
// FUNCTION: STBC 0x00534fa0 _$E79
// FUNCTION: STBC 0x00534fb0 _$E84
// FUNCTION: STBC 0x00534fc0 _$E83
// FUNCTION: STBC 0x00534fd0 _$E208
// FUNCTION: STBC 0x00534fe0 _$E207
// FUNCTION: STBC 0x00535740 _$E150
// FUNCTION: STBC 0x00535750 _$E149
// FUNCTION: STBC 0x00535930 _$E163
// FUNCTION: STBC 0x00535940 _$E162
// FUNCTION: STBC 0x00535e50 _$E213
// FUNCTION: STBC 0x00535e60 _$E212
// FUNCTION: STBC 0x00536940 _$E218
// FUNCTION: STBC 0x00536950 _$E217
// FUNCTION: STBC 0x00536b70 _$E222
// FUNCTION: STBC 0x00536b80 _$E221
// FUNCTION: STBC 0x00536fb0 _$E226
// FUNCTION: STBC 0x00536fc0 _$E225
// FUNCTION: STBC 0x00536fd0 _$E233
// FUNCTION: STBC 0x00536fe0 _$E232
// FUNCTION: STBC 0x00536ff0 _$E341
// FUNCTION: STBC 0x00537000 _$E340
// FUNCTION: STBC 0x00537010 _$E345
// FUNCTION: STBC 0x00537020 _$E344
// FUNCTION: STBC 0x00537030 _$E108
// FUNCTION: STBC 0x00537040 _$E107
// FUNCTION: STBC 0x00537050 _$E275
// FUNCTION: STBC 0x00537060 _$E274
// FUNCTION: STBC 0x00537070 _$E282
// FUNCTION: STBC 0x00537080 _$E281
// FUNCTION: STBC 0x00537090 _$E286
// FUNCTION: STBC 0x005370a0 _$E285
// FUNCTION: STBC 0x005370b0 _$E290
// FUNCTION: STBC 0x005370c0 _$E289
// FUNCTION: STBC 0x005370d0 _$E294
// FUNCTION: STBC 0x005370e0 _$E293
// FUNCTION: STBC 0x005370f0 _$E300
// FUNCTION: STBC 0x00537100 _$E299
// FUNCTION: STBC 0x00537110 _$E349
// FUNCTION: STBC 0x00537120 _$E348
// FUNCTION: STBC 0x00537130 _$E353
// FUNCTION: STBC 0x00537140 _$E352
// FUNCTION: STBC 0x00537150 _$E357
// FUNCTION: STBC 0x00537160 _$E356
// FUNCTION: STBC 0x00537170 _$E361
// FUNCTION: STBC 0x00537180 _$E360
// FUNCTION: STBC 0x00537190 _$E365
// FUNCTION: STBC 0x005371a0 _$E364
// FUNCTION: STBC 0x005371b0 _$E369
// FUNCTION: STBC 0x005371c0 _$E368
// FUNCTION: STBC 0x005371d0 _$E373
// FUNCTION: STBC 0x005371e0 _$E372
// FUNCTION: STBC 0x005371f0 _$E377
// FUNCTION: STBC 0x00537200 _$E376
// FUNCTION: STBC 0x00537210 _$E381
// FUNCTION: STBC 0x00537220 _$E380
// FUNCTION: STBC 0x00537230 _$E385
// FUNCTION: STBC 0x00537240 _$E384
// FUNCTION: STBC 0x00537250 _$E113
// FUNCTION: STBC 0x00537260 _$E112
// FUNCTION: STBC 0x00537270 _$E389
// FUNCTION: STBC 0x00537280 _$E388
// FUNCTION: STBC 0x00537290 _$E118
// FUNCTION: STBC 0x005372a0 _$E117
// FUNCTION: STBC 0x005372b0 _$E393
// FUNCTION: STBC 0x005372c0 _$E392
// FUNCTION: STBC 0x005372d0 _$E397
// FUNCTION: STBC 0x005372e0 _$E396
// FUNCTION: STBC 0x005372f0 _$E401
// FUNCTION: STBC 0x00537300 _$E400
// FUNCTION: STBC 0x00537310 _$E405
// FUNCTION: STBC 0x00537320 _$E404
// FUNCTION: STBC 0x00537330 _$E127
// FUNCTION: STBC 0x00537340 _$E126
// FUNCTION: STBC 0x00537350 _$E409
// FUNCTION: STBC 0x00537360 _$E408
// FUNCTION: STBC 0x00537370 _$E413
// FUNCTION: STBC 0x00537380 _$E412
// FUNCTION: STBC 0x00537390 _$E417
// FUNCTION: STBC 0x005373a0 _$E416
// FUNCTION: STBC 0x005373b0 _$E133
// FUNCTION: STBC 0x005373c0 _$E132
// FUNCTION: STBC 0x005373d0 _$E138
// FUNCTION: STBC 0x005373e0 _$E137
// FUNCTION: STBC 0x005373f0 _$E168
// FUNCTION: STBC 0x00537400 _$E167
// FUNCTION: STBC 0x00537410 _$E421
// FUNCTION: STBC 0x00537420 _$E420
// FUNCTION: STBC 0x00537430 _$E173
// FUNCTION: STBC 0x00537440 _$E172
// FUNCTION: STBC 0x00537450 _$E425
// FUNCTION: STBC 0x00537460 _$E424
// FUNCTION: STBC 0x00537470 _$E429
// FUNCTION: STBC 0x00537480 _$E428
// FUNCTION: STBC 0x00537490 _$E178
// FUNCTION: STBC 0x005374a0 _$E177
// FUNCTION: STBC 0x005374b0 _$E433
// FUNCTION: STBC 0x005374c0 _$E432
// FUNCTION: STBC 0x005374d0 _$E143
// FUNCTION: STBC 0x005374e0 _$E142
// FUNCTION: STBC 0x005374f0 _$E437
// FUNCTION: STBC 0x00537500 _$E436
// FUNCTION: STBC 0x00537510 _$E441
// FUNCTION: STBC 0x00537520 _$E440
// FUNCTION: STBC 0x00537530 _$E183
// FUNCTION: STBC 0x00537540 _$E182
// FUNCTION: STBC 0x00537550 _$E188
// FUNCTION: STBC 0x00537560 _$E187
// FUNCTION: STBC 0x00537570 _$E445
// FUNCTION: STBC 0x00537580 _$E444
// FUNCTION: STBC 0x00537590 _$E449
// FUNCTION: STBC 0x005375a0 _$E448
// FUNCTION: STBC 0x005375b0 _$E453
// FUNCTION: STBC 0x005375c0 _$E452
// FUNCTION: STBC 0x005375d0 _$E457
// FUNCTION: STBC 0x005375e0 _$E456
// FUNCTION: STBC 0x005375f0 _$E461
// FUNCTION: STBC 0x00537600 _$E460
// FUNCTION: STBC 0x00537610 _$E465
// FUNCTION: STBC 0x00537620 _$E464
// FUNCTION: STBC 0x00537630 _$E469
// FUNCTION: STBC 0x00537640 _$E468
// FUNCTION: STBC 0x00537650 _$E473
// FUNCTION: STBC 0x00537660 _$E472
// FUNCTION: STBC 0x00537670 _$E193
// FUNCTION: STBC 0x00537680 _$E192
// FUNCTION: STBC 0x00537690 _$E477
// FUNCTION: STBC 0x005376a0 _$E476
// FUNCTION: STBC 0x005376b0 _$E198
// FUNCTION: STBC 0x005376c0 _$E197
// FUNCTION: STBC 0x005376d0 _$E203
// FUNCTION: STBC 0x005376e0 _$E202
// FUNCTION: STBC 0x00537870 _$E481
// FUNCTION: STBC 0x00537880 _$E480
// FUNCTION: STBC 0x00537890 _$E486
// FUNCTION: STBC 0x005378a0 _$E485
// FUNCTION: STBC 0x005378b0 _$E490
// FUNCTION: STBC 0x005378c0 _$E489

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00534b60 _$E515
// FUNCTION: STBC 0x00534b80 _$E514

// DATA: STBC 0x00989bb0
TGEventHandlerTable STSubsystemMenu::classHandlers;

// DATA: STBC 0x00989c38
TGEventHandlerTable STComponentMenu::classHandlers;

// DATA: STBC 0x00989bf0
TGEventHandlerTable STComponentMenuItem::classHandlers;

/*
================
STSubsystemMenu::Cast

The object as a subsystem menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005350e0
STSubsystemMenu *STSubsystemMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STSUBSYSTEMMENU_TYPE ) ) {
		return (STSubsystemMenu *)object;
	}
	return NULL;
}

/*
================
STComponentMenu::Cast

The object as a component menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00535110
STComponentMenu *STComponentMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STCOMPONENTMENU_TYPE ) ) {
		return (STComponentMenu *)object;
	}
	return NULL;
}

/*
================
STComponentMenuItem::Cast

The object as a component menu item; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00535140
STComponentMenuItem *STComponentMenuItem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STCOMPONENTMENUITEM_TYPE ) ) {
		return (STComponentMenuItem *)object;
	}
	return NULL;
}

/*
================
STSubsystemMenu::STSubsystemMenu

Names the menu after the ship, shows the hull's condition in the title bar,
adds the items and listens for the ship's subsystems and name.
================
*/
// FUNCTION: STBC 0x00535170 ??0STSubsystemMenu@@QAE@PAVShipClass@@PAVTGEvent@@@Z
STSubsystemMenu::STSubsystemMenu( ShipClass *ship, TGEvent *event )
	: STMenu( ship->GetName(), STMF_TWO_CLICKS | STMF_OPENABLE | STMF_CLOSEABLE, event, NULL ) {
	shipID = ship->GetObjID();

	STFillGauge *gauge = new STFillGauge( STMENU_ICON_TITLE_BAR, NULL, false );
	gauge->SetObject( ship->GetHull() );
	gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage;
	gauge->maxFunction = NULL;
	gauge->SetFillColor( g_kSubsystemFillColor );
	gauge->SetEmptyColor( g_kSubsystemEmptyColor );
	TGPane::InsertChild( STMENU_TITLE_BAR, gauge, 0.0f, 0.0f, false );
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR + 1 );
	TGPane::RemoveChild( titleBar, false );
	delete titleBar;

	SetChosen( false, false );
	VirtualSlot115();

	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_COMPLETELY_DESTROYED, this, "STSubsystemMenu::HandleSubsystemDestroyed", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_REBUILT, this, "STSubsystemMenu::HandleSubsystemRebuilt", ship );
	g_kEventManager.AddBroadcastMethodHandler( ET_NAME_CHANGE, this, "STSubsystemMenu::HandleNameChange", ship );
}

/*
================
STSubsystemMenu::STSubsystemMenu

The menu a stream loads.
================
*/
// FUNCTION: STBC 0x00535420 ??0STSubsystemMenu@@QAE@J@Z
STSubsystemMenu::STSubsystemMenu( long id ) : STMenu( id ) {
	shipID = 0;
	if ( id == 0 ) {
		SetChosen( false, false );
	}
}

/*
================
STSubsystemMenu::GetShip
================
*/
// FUNCTION: STBC 0x00535490
ShipClass *STSubsystemMenu::GetShip( void ) {
	return ShipClass::GetObjectByID( NULL, shipID );
}

/*
================
STSubsystemMenu::SetFocus

A component menu shows itself chosen while it has the focus.
================
*/
// FUNCTION: STBC 0x005354a0
void STSubsystemMenu::SetFocus( TGUIObject *child, bool layout ) {
	if ( STSubPane::Cast( child ) != NULL ) {
		STMenu::SetFocus( child, layout );
		return;
	}
	STComponentMenu *oldMenu = STComponentMenu::Cast( GetFocus() );
	if ( oldMenu != NULL ) {
		oldMenu->SetChosen( false, false );
	}
	STComponentMenu *newMenu = STComponentMenu::Cast( child );
	if ( newMenu != NULL ) {
		newMenu->SetChosen( true, false );
	}
	STMenu::SetFocus( child, layout );
}

/*
================
STSubsystemMenu::GotFocus

The menu shows itself chosen, and the player targets the ship.
================
*/
// FUNCTION: STBC 0x00535510
void STSubsystemMenu::GotFocus( bool layout ) {
	STMenu::GotFocus( layout );
	SetChosen( true, false );
	ShipClass *player = Game::GetCurrentPlayer();
	ShipClass *ship = GetShip();
	if ( player != NULL && ship != NULL && player->GetTarget() != ship ) {
		player->SetTarget( ship, NULL );
		player->SetTargetSubsystem( NULL );
	}
}

/*
================
STSubsystemMenu::LostFocus
================
*/
// FUNCTION: STBC 0x00535570
void STSubsystemMenu::LostFocus( bool layout ) {
	STMenu::LostFocus( layout );
	SetChosen( false, false );
}

/*
================
STSubsystemMenu::VirtualSlot115

Builds a component menu for each subsystem of the ship that can be
targeted, or has a child that can, and has some condition left (not the
hull, which the menu itself stands for). The menu opens only when it holds
one.
================
*/
// FUNCTION: STBC 0x00535590
void STSubsystemMenu::VirtualSlot115( void ) {
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return;
	}

	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	KillChildren();
	subPane->SetNotExclusive( false );
	SetOpenable();

	bool anyAdded = false;
	NiTListIterator pos = ship->GetSubsystemList().GetHeadPos();
	while ( pos ) {
		ShipSubsystem *subsystem = ship->GetSubsystemList().GetNext( pos );
		if ( ( subsystem->IsTargetable() || subsystem->HasTargetableChildSubsystem() == true ) && subsystem != ship->GetHull() && subsystem->GetCombinedConditionPercentage() > 0.0f ) {
			anyAdded = true;
			STComponentMenu *menu = new STComponentMenu( ship, subsystem, NULL );
			TGEvent *clicked = new TGEvent;
			clicked->SetEventType( ET_SUBSYSTEM_BUTTON_CLICKED );
			clicked->SetSource( menu );
			clicked->SetDestination( this );
			menu->SetActivationEvent( clicked );
			InsertChild( 0x7FFFFFFF, menu, 0.0f, 0.0f, false );
			menu->ForceUpdate( false );
			menu->Close( false );
		}
	}
	if ( !anyAdded ) {
		SetNotOpenable();
	}

	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	} else {
		ResizeToContents( true );
	}
}

/*
================
STSubsystemMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00535760
void STSubsystemMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubsystemMenu::TargetButtonClicked, "STSubsystemMenu::TargetButtonClicked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubsystemMenu::KeyboardHandler, "STSubsystemMenu::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubsystemMenu::HandleSubsystemDestroyed, "STSubsystemMenu::HandleSubsystemDestroyed" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubsystemMenu::HandleSubsystemRebuilt, "STSubsystemMenu::HandleSubsystemRebuilt" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubsystemMenu::HandleNameChange, "STSubsystemMenu::HandleNameChange" );
}

/*
================
STSubsystemMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x005357d0
void STSubsystemMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SUBSYSTEM_BUTTON_CLICKED, "STSubsystemMenu::TargetButtonClicked" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STSubsystemMenu::KeyboardHandler" );
}

/*
================
STSubsystemMenu::ShowUnknownName

Names the menu after what kind of object its ship is, until the sensors
identify it.
================
*/
// FUNCTION: STBC 0x00535800
void STSubsystemMenu::ShowUnknownName( void ) {
	ShipClass *ship = GetShip();
	if ( ship != NULL ) {
		ShipProperty *property = ship->GetShipProperty();
		if ( property != NULL ) {
			TGString name( "" );
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
			switch ( property->GetGenus() ) {
				case GENUS_SHIP:
					name = *database->GetString( "Unknown Ship" );
					break;
				case GENUS_STATION:
					name = *database->GetString( "Unknown Station" );
					break;
				case GENUS_UNKNOWN:
					name = *database->GetString( "Unknown Object" );
					break;
				case GENUS_ASTEROID:
					name = *database->GetString( "Unknown Object" );
					break;
				default:
					name = *database->GetString( "Unknown Object" );
					break;
			}
			g_kLocalizationManager.Unload( database );

			TGString current( "" );
			GetName( &current );
			if ( current.Compare( &name, true ) != 0 ) {
				SetName( &name, true );
			}
		}
	}
}

/*
================
STSubsystemMenu::ShowRealName

Names the menu after the ship's display name, if it shows another.
================
*/
// FUNCTION: STBC 0x00535950
void STSubsystemMenu::ShowRealName( void ) {
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return;
	}
	TGString name = ship->GetDisplayName();
	TGString current( "" );
	GetName( &current );
	if ( current.Compare( &name, true ) != 0 ) {
		SetName( &name, true );
	}
}

/*
================
STSubsystemMenu::IsShowingRealName
================
*/
// FUNCTION: STBC 0x00535b20
bool STSubsystemMenu::IsShowingRealName( void ) {
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return false;
	}
	TGString name = ship->GetDisplayName();
	TGString current( "" );
	GetName( &current );
	if ( current.Compare( &name, true ) != 0 ) {
		return false;
	}
	return true;
}

/*
================
STSubsystemMenu::Close

Closing the menu of the player's target drops the targeted subsystem.
================
*/
// FUNCTION: STBC 0x00535d10
void STSubsystemMenu::Close( bool layout ) {
	STMenu::Close( layout );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && player->GetTarget() == GetShip() ) {
		ObjectClass *object = ObjectClass::GetObjectByID( NULL, shipID );
		if ( object != NULL && player->CanTargetObject( object ) ) {
			player->SetTargetSubsystem( NULL );
		}
	}
}

/*
================
STSubsystemMenu::VirtualSlot112

Colours the menu for the state, then its end caps with the ship's colour.
================
*/
// FUNCTION: STBC 0x00535d70
void STSubsystemMenu::VirtualSlot112( int state ) {
	STMenu::VirtualSlot112( state );
	if ( GetShip() != NULL ) {
		NiColorA *color = GetShip()->GetAffiliationColor();
		TGIcon *leftEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_LEFT_END_CAP );
		TGIcon *rightEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
		leftEndCap->SetColor( color );
		rightEndCap->SetColor( color );
	}
}

/*
================
STSubsystemMenu::FindItem
================
*/
// FUNCTION: STBC 0x00535dd0
TGUIObject *STSubsystemMenu::FindItem( ShipSubsystem *subsystem ) {
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child != NULL && child->IsTypeOf( STCOMPONENTMENU_TYPE ) ) {
			STComponentMenu *menu = (STComponentMenu *)child;
			if ( menu->GetSubsystem() == subsystem ) {
				return menu;
			}
			TGUIObject *item = menu->FindItem( subsystem );
			if ( item != NULL ) {
				return item;
			}
		}
	}
	return NULL;
}

/*
================
STSubsystemMenu::HandleSubsystemDestroyed

When the player's targeted subsystem is destroyed, moves the target to the
next targetable sibling, else to the ship's hull, re-aims the player's weapon
systems at it and focuses its item; then hides the destroyed subsystem's item.
================
*/
// FUNCTION: STBC 0x00535e70
void STSubsystemMenu::HandleSubsystemDestroyed( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	ShipSubsystem *destroyed = ShipSubsystem::Cast( event->GetSource() );
	if ( player == NULL ) {
		CallNextHandler( event );
		return;
	}
	ShipSubsystem *target = player->GetTargetSubsystem();
	if ( target != NULL && destroyed == target ) {
		ShipSubsystem *next = NULL;
		ShipSubsystem *parent = target->GetParentSubsystem();
		if ( parent != NULL ) {
			next = parent->GetNextTargetableChildSubsystem( target );
		}
		if ( next == NULL ) {
			next = target->GetParentShip()->GetHull();
		}
		if ( next != NULL ) {
			WeaponSystem *weapons = WeaponSystem::Cast( player->GetPhaserSystem() );
			if ( weapons != NULL ) {
				weapons->SetTargetOffset( next->GetParentShip(), next->GetPosition() );
			}
			weapons = WeaponSystem::Cast( player->GetTorpedoSystem() );
			if ( weapons != NULL ) {
				weapons->SetTargetOffset( next->GetParentShip(), next->GetPosition() );
			}
			weapons = WeaponSystem::Cast( player->GetPulseWeaponSystem() );
			if ( weapons != NULL ) {
				weapons->SetTargetOffset( next->GetParentShip(), next->GetPosition() );
			}
			weapons = WeaponSystem::Cast( player->GetTractorBeamSystem() );
			if ( weapons != NULL ) {
				weapons->SetTargetOffset( next->GetParentShip(), next->GetPosition() );
			}
		}
		player->SetTargetSubsystem( next );
		TGUIObject *item = FindItem( player->GetTargetSubsystem() );
		if ( item != NULL ) {
			InterfaceModule::ForceFocusOnObject( item );
			STButton *button = STButton::Cast( item );
			STMenu *menu = STMenu::Cast( item );
			if ( button != NULL ) {
				button->SetChosen( true, false );
			}
			if ( menu != NULL ) {
				menu->SetChosen( true, false );
			}
			item->GetConceptualParent()->SetFocus( item, true );
		}
	}
	TGUIObject *item = FindItem( destroyed );
	STMenu *menu = STMenu::Cast( item );
	if ( menu != NULL ) {
		menu->Close( true );
	}
	if ( item != NULL ) {
		item->SetNotVisible( true );
		STSubPane *subPane = STSubPane::Cast( item->GetParent() );
		STMenu *parentMenu = STMenu::Cast( item->GetParent() );
		if ( subPane != NULL ) {
			subPane->ResizeToContents( true );
		} else if ( parentMenu != NULL ) {
			parentMenu->ResizeToContents( true );
		}
	}
	CallNextHandler( event );
}

/*
================
STSubsystemMenu::HandleSubsystemRebuilt

Shows the subsystem's item again with its gauge up to date.
================
*/
// FUNCTION: STBC 0x00536090
void STSubsystemMenu::HandleSubsystemRebuilt( TGEvent *event ) {
	TGUIObject *item = FindItem( ShipSubsystem::Cast( event->GetSource() ) );
	if ( item != NULL ) {
		item->SetVisible( false );
		TGPane *pane = TGPane::Cast( item );
		if ( pane != NULL ) {
			STFillGauge *gauge = STFillGauge::Cast( pane->TGPane::GetNthChild( STMENU_TITLE_BAR ) );
			if ( gauge == NULL ) {
				gauge = STFillGauge::Cast( pane->TGPane::GetNthChild( STBUTTON_BODY ) );
			}
			if ( gauge != NULL ) {
				gauge->UpdateGauge();
			}
		}
		STSubPane *subPane = STSubPane::Cast( item->GetParent() );
		STMenu *menu = STMenu::Cast( item->GetParent() );
		if ( subPane != NULL ) {
			subPane->ResizeToContents( true );
		} else if ( menu != NULL ) {
			menu->ResizeToContents( true );
		}
	}
	CallNextHandler( event );
}

/*
================
STSubsystemMenu::TargetButtonClicked

Chooses the clicked component menu alone and targets its subsystem. A
subsystem that cannot be targeted itself has one of its working children
chosen at random instead; the target stays the parent, which the player
cannot target, so only the menu shows the choice.
================
*/
// FUNCTION: STBC 0x00536160
// PARKED: register allocation: retail keeps the menu in edi and the event, then the subsystem, in esi; this build swaps them (early returns and the if/else form compile the same)
void STSubsystemMenu::TargetButtonClicked( TGEvent *event ) {
	GetShip();
	STComponentMenu *clicked = (STComponentMenu *)event->GetSource();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( clicked != NULL && player != NULL && player->GetContainingSet() != NULL ) {
		for ( int i = 0; i < GetNumChildren(); i++ ) {
			STMenu *menu = STMenu::Cast( GetNthChild( i ) );
			STButton *button = STButton::Cast( GetNthChild( i ) );
			if ( menu != NULL && menu->IsChosen() == true ) {
				menu->SetChosen( false, false );
			}
			if ( button != NULL && button->IsChosen() == true ) {
				button->SetChosen( false, false );
			}
		}
		SetFocus( clicked, false );
		clicked->SetChosen( true, false );
		ShipSubsystem *subsystem = clicked->GetSubsystem();
		if ( subsystem == NULL ) {
			CallNextHandler( event );
			return;
		}
		if ( !subsystem->IsTargetable() && subsystem->GetNumChildSubsystems() > 0 ) {
			NiTArray<ShipSubsystem *> working;
			for ( int j = 0; j < subsystem->GetNumChildSubsystems(); j++ ) {
				ShipSubsystem *child = subsystem->GetChildSubsystem( j );
				if ( child->GetCondition() > 0.0f ) {
					working.Add( child );
				}
				STComponentMenuItem *item = clicked->FindItem( child );
				if ( item != NULL ) {
					item->SetChosen( false, false );
				}
			}
			if ( working.GetSize() > 0 ) {
				STComponentMenuItem *item = clicked->FindItem( working.GetAt( rand() % working.GetSize() ) );
				if ( item != NULL ) {
					clicked->SetFocus( item, false );
					item->SetChosen( true, false );
				}
			}
		}
		if ( subsystem->IsTargetable() ) {
			// BUG: the subsystem's name is copied to a C string and freed unused.
			char *cName = subsystem->GetProperty()->GetName().Unknown6F56F0();
			delete cName;
			player->SetTargetSubsystem( subsystem );
		}
		CallNextHandler( event );
	} else {
		CallNextHandler( event );
	}
}

/*
================
STSubsystemMenu::HandleNameChange

A ship that has been identified shows its real name.
================
*/
// FUNCTION: STBC 0x00536410
void STSubsystemMenu::HandleNameChange( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship != NULL && SensorSubsystem::GetSensorData().IsIdentified( ship ) ) {
		ShowRealName();
	}
}

/*
================
STSubsystemMenu::WriteToStream
================
*/
// FUNCTION: STBC 0x00536450
bool STSubsystemMenu::WriteToStream( TGStream *stream ) {
	if ( !STMenu::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	return true;
}

/*
================
STSubsystemMenu::ReadFromStream
================
*/
// FUNCTION: STBC 0x00536480
bool STSubsystemMenu::ReadFromStream( TGStream *stream ) {
	if ( !STMenu::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	return true;
}

/*
================
STComponentMenu::STComponentMenu

Names the menu after the subsystem, shows the subsystem's condition in the
title bar and adds the items.
================
*/
// FUNCTION: STBC 0x005364b0 ??0STComponentMenu@@QAE@PAVShipClass@@PAVShipSubsystem@@PAVTGEvent@@@Z
STComponentMenu::STComponentMenu( ShipClass *ship, ShipSubsystem *subsystem, TGEvent *event )
	: STMenu( "Default", STMF_TWO_CLICKS | STMF_OPENABLE | STMF_CLOSEABLE, event, NULL ) {
	TGString name = subsystem->GetDisplayName();
	TGParagraph *text = (TGParagraph *)TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	text->SetString( &name, true );
	shipID = ship->GetObjID();
	subsystemID = subsystem->GetObjID();

	STFillGauge *gauge = new STFillGauge( STMENU_ICON_TITLE_BAR, NULL, false );
	gauge->SetObject( subsystem );
	gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage;
	gauge->maxFunction = NULL;
	gauge->alternateFunction = (GaugeFlagFunction)&ShipSubsystem::IsDisabled;
	gauge->SetFillColor( g_kSubsystemFillColor );
	gauge->SetEmptyColor( g_kSubsystemEmptyColor );
	gauge->SetAlternateColor( g_kSubsystemDisabledColor );
	TGPane::InsertChild( STMENU_TITLE_BAR, gauge, 0.0f, 0.0f, false );
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR + 1 );
	TGPane::RemoveChild( titleBar, false );
	delete titleBar;

	BuildItems();
}

/*
================
STComponentMenu::GetShip
================
*/
// FUNCTION: STBC 0x00536790
ShipClass *STComponentMenu::GetShip( void ) {
	return ShipClass::GetObjectByID( NULL, shipID );
}

/*
================
STComponentMenu::GetSubsystem
================
*/
// FUNCTION: STBC 0x005367a0
ShipSubsystem *STComponentMenu::GetSubsystem( void ) {
	return ShipSubsystem::Cast( TGObject::GetTGObjectPtr( subsystemID ) );
}

/*
================
STComponentMenu::BuildItems

Only a subsystem with more than one child opens; the items go in only while
the menu is openable.
================
*/
// FUNCTION: STBC 0x005367c0
void STComponentMenu::BuildItems( void ) {
	ShipSubsystem *subsystem = GetSubsystem();
	KillChildren();
	GetSubPane()->SetNotExclusive( false );
	if ( subsystem->GetNumChildSubsystems() > 1 ) {
		SetOpenable();
	} else {
		SetNotOpenable();
	}
	if ( !IsOpenable() ) {
		Close( false );
	} else {
		for ( int i = 0; i < subsystem->GetNumChildSubsystems(); i++ ) {
			ShipSubsystem *child = subsystem->GetChildSubsystem( i );
			if ( ( child->IsTargetable() || child->HasTargetableChildSubsystem() == true ) && child->GetCombinedConditionPercentage() > 0.0f ) {
				STComponentMenuItem *item = new STComponentMenuItem( child, NULL );
				AddChild( item, 0.0f, 0.0f, false );
				TGEvent *clicked = new TGEvent;
				clicked->SetEventType( ET_COMPONENT_BUTTON_CLICKED );
				clicked->SetSource( item );
				clicked->SetDestination( this );
				item->SetActivationEvent( clicked );
			}
		}
	}
	ForceUpdate( false );
}

/*
================
STComponentMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00536960
void STComponentMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STComponentMenu::TargetButtonClicked, "STComponentMenu::TargetButtonClicked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STComponentMenu::KeyboardHandler, "STComponentMenu::KeyboardHandler" );
}

/*
================
STComponentMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00536990
void STComponentMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_COMPONENT_BUTTON_CLICKED, "STComponentMenu::TargetButtonClicked" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STComponentMenu::KeyboardHandler" );
}

/*
================
STComponentMenu::TargetButtonClicked

Chooses the clicked item alone and targets its subsystem, when it can be
targeted.
================
*/
// FUNCTION: STBC 0x005369c0
void STComponentMenu::TargetButtonClicked( TGEvent *event ) {
	GetSubsystem();
	GetShip();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( Game::GetCurrentGame()->GetPlayerSet() != NULL && player != NULL ) {
		STComponentMenuItem *clicked = (STComponentMenuItem *)event->GetSource();
		ShipSubsystem *subsystem = clicked->GetSubsystem();
		for ( int i = 0; i < GetNumChildren(); i++ ) {
			STMenu *menu = STMenu::Cast( GetNthChild( i ) );
			STButton *button = STButton::Cast( GetNthChild( i ) );
			if ( menu != NULL && menu->IsChosen() == true ) {
				menu->SetChosen( false, false );
			}
			if ( button != NULL && button->IsChosen() == true ) {
				button->SetChosen( false, false );
			}
		}
		SetFocus( clicked, false );
		clicked->SetChosen( true, false );
		if ( subsystem->IsTargetable() == true ) {
			player->SetTargetSubsystem( subsystem );
		}
		CallNextHandler( event );
	} else {
		CallNextHandler( event );
	}
}

/*
================
STComponentMenu::SetFocus

An item shows itself chosen while it has the focus.
================
*/
// FUNCTION: STBC 0x00536b00
void STComponentMenu::SetFocus( TGUIObject *child, bool layout ) {
	if ( STSubPane::Cast( child ) != NULL ) {
		STMenu::SetFocus( child, layout );
		return;
	}
	STComponentMenuItem *oldItem = STComponentMenuItem::Cast( GetFocus() );
	if ( oldItem != NULL ) {
		oldItem->SetChosen( false, false );
	}
	STComponentMenuItem *newItem = STComponentMenuItem::Cast( child );
	if ( newItem != NULL ) {
		newItem->SetChosen( true, false );
	}
	STMenu::SetFocus( child, layout );
}

/*
================
STComponentMenu::WriteToStream
================
*/
// FUNCTION: STBC 0x00536b90
bool STComponentMenu::WriteToStream( TGStream *stream ) {
	if ( !STMenu::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	stream->WriteID( subsystemID );
	return true;
}

/*
================
STComponentMenu::ReadFromStream
================
*/
// FUNCTION: STBC 0x00536bd0
bool STComponentMenu::ReadFromStream( TGStream *stream ) {
	if ( !STMenu::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	subsystemID = stream->ReadID();
	return true;
}

/*
================
STComponentMenu::FindItem
================
*/
// FUNCTION: STBC 0x00536c10
STComponentMenuItem *STComponentMenu::FindItem( ShipSubsystem *subsystem ) {
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child->IsTypeOf( STCOMPONENTMENUITEM_TYPE ) && ( (STComponentMenuItem *)child )->GetSubsystem() == subsystem ) {
			return (STComponentMenuItem *)child;
		}
	}
	return NULL;
}

/*
================
STComponentMenuItem::STComponentMenuItem

Names the button after the subsystem and shows the subsystem's condition in
place of the body.
================
*/
// FUNCTION: STBC 0x00536c80 ??0STComponentMenuItem@@QAE@PAVShipSubsystem@@PAVTGEvent@@@Z
STComponentMenuItem::STComponentMenuItem( ShipSubsystem *subsystem, TGEvent *event )
	: STButton( "", event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	subsystemID = subsystem->GetObjID();
	SetName( &subsystem->GetDisplayName(), false );

	STFillGauge *gauge = new STFillGauge( STMENU_ICON_TITLE_BAR, NULL, false );
	gauge->SetObject( subsystem );
	gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage;
	gauge->maxFunction = NULL;
	gauge->alternateFunction = (GaugeFlagFunction)&ShipSubsystem::IsDisabled;
	gauge->SetFillColor( g_kSubsystemFillColor );
	gauge->SetEmptyColor( g_kSubsystemEmptyColor );
	gauge->SetAlternateColor( g_kSubsystemDisabledColor );
	TGPane::InsertChild( STBUTTON_BODY, gauge, 0.0f, 0.0f, false );
	TGUIObject *body = TGPane::GetNthChild( STBUTTON_BODY + 1 );
	TGPane::RemoveChild( body, false );
	delete body;
}

/*
================
STComponentMenuItem::GetSubsystem
================
*/
// FUNCTION: STBC 0x00536f30
ShipSubsystem *STComponentMenuItem::GetSubsystem( void ) {
	return ShipSubsystem::Cast( TGObject::GetTGObjectPtr( subsystemID ) );
}

/*
================
STComponentMenuItem::WriteToStream
================
*/
// FUNCTION: STBC 0x00536f50
bool STComponentMenuItem::WriteToStream( TGStream *stream ) {
	if ( !STButton::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( subsystemID );
	return true;
}

/*
================
STComponentMenuItem::ReadFromStream
================
*/
// FUNCTION: STBC 0x00536f80
bool STComponentMenuItem::ReadFromStream( TGStream *stream ) {
	if ( !STButton::ReadFromStream( stream ) ) {
		return false;
	}
	subsystemID = stream->ReadID();
	return true;
}
