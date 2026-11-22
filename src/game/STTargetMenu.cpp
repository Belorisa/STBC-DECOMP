/*
===============================================================================

	STTargetMenu.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "STTargetMenu.h"
#include "CloakingSubsystem.h"
#include "DamageableObject.h"
#include "Game.h"
#include "InterfaceModule.h"
#include "MultiplayerGame.h"
#include "ObjectClass.h"
#include "ObjectGroup.h"
#include "SensorSubsystem.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "ShipSubsystem.h"
#include "STSubPane.h"
#include "STSubsystemMenu.h"
#include "TacticalControlWindow.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGObject.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00537a00 _$E32
// FUNCTION: STBC 0x00537a10 _$E29
// FUNCTION: STBC 0x00537a20 _$E31
// FUNCTION: STBC 0x00537a30 _$E30
// FUNCTION: STBC 0x00537a40 _$E38
// FUNCTION: STBC 0x00537a50 _$E35
// FUNCTION: STBC 0x00537a60 _$E37
// FUNCTION: STBC 0x00537a70 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00537b60 _$E497
// FUNCTION: STBC 0x00537b70 _$E493
// FUNCTION: STBC 0x00537b80 _$E496
// FUNCTION: STBC 0x00537b90 _$E494

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00537a80 _$E57
// FUNCTION: STBC 0x00537a90 _$E56
// FUNCTION: STBC 0x00537aa0 _$E61
// FUNCTION: STBC 0x00537ab0 _$E60
// FUNCTION: STBC 0x00537ac0 _$E72
// FUNCTION: STBC 0x00537ad0 _$E71
// FUNCTION: STBC 0x00537ae0 _$E76
// FUNCTION: STBC 0x00537af0 _$E75
// FUNCTION: STBC 0x00537b00 _$E80
// FUNCTION: STBC 0x00537b10 _$E79
// FUNCTION: STBC 0x00537b20 _$E84
// FUNCTION: STBC 0x00537b30 _$E83
// FUNCTION: STBC 0x00537b40 _$E208
// FUNCTION: STBC 0x00537b50 _$E207
// FUNCTION: STBC 0x00539200 _$E150
// FUNCTION: STBC 0x00539210 _$E149
// FUNCTION: STBC 0x00539220 _$E163
// FUNCTION: STBC 0x00539230 _$E162
// FUNCTION: STBC 0x00539240 _$E213
// FUNCTION: STBC 0x00539250 _$E212
// FUNCTION: STBC 0x00539260 _$E218
// FUNCTION: STBC 0x00539270 _$E217
// FUNCTION: STBC 0x00539280 _$E222
// FUNCTION: STBC 0x00539290 _$E221
// FUNCTION: STBC 0x005392a0 _$E226
// FUNCTION: STBC 0x005392b0 _$E225
// FUNCTION: STBC 0x005392c0 _$E233
// FUNCTION: STBC 0x005392d0 _$E232
// FUNCTION: STBC 0x005392e0 _$E350
// FUNCTION: STBC 0x005392f0 _$E349
// FUNCTION: STBC 0x00539300 _$E354
// FUNCTION: STBC 0x00539310 _$E353
// FUNCTION: STBC 0x00539320 _$E108
// FUNCTION: STBC 0x00539330 _$E107
// FUNCTION: STBC 0x00539340 _$E289
// FUNCTION: STBC 0x00539350 _$E288
// FUNCTION: STBC 0x00539360 _$E296
// FUNCTION: STBC 0x00539370 _$E295
// FUNCTION: STBC 0x00539380 _$E300
// FUNCTION: STBC 0x00539390 _$E299
// FUNCTION: STBC 0x005393a0 _$E304
// FUNCTION: STBC 0x005393b0 _$E303
// FUNCTION: STBC 0x005393c0 _$E308
// FUNCTION: STBC 0x005393d0 _$E307
// FUNCTION: STBC 0x005393e0 _$E314
// FUNCTION: STBC 0x005393f0 _$E313
// FUNCTION: STBC 0x00539400 _$E270
// FUNCTION: STBC 0x00539410 _$E269
// FUNCTION: STBC 0x00539420 _$E274
// FUNCTION: STBC 0x00539430 _$E273
// FUNCTION: STBC 0x00539440 _$E358
// FUNCTION: STBC 0x00539450 _$E357
// FUNCTION: STBC 0x00539460 _$E362
// FUNCTION: STBC 0x00539470 _$E361
// FUNCTION: STBC 0x00539480 _$E366
// FUNCTION: STBC 0x00539490 _$E365
// FUNCTION: STBC 0x005394a0 _$E370
// FUNCTION: STBC 0x005394b0 _$E369
// FUNCTION: STBC 0x005394c0 _$E374
// FUNCTION: STBC 0x005394d0 _$E373
// FUNCTION: STBC 0x005394e0 _$E378
// FUNCTION: STBC 0x005394f0 _$E377
// FUNCTION: STBC 0x00539500 _$E382
// FUNCTION: STBC 0x00539510 _$E381
// FUNCTION: STBC 0x00539520 _$E386
// FUNCTION: STBC 0x00539530 _$E385
// FUNCTION: STBC 0x00539540 _$E113
// FUNCTION: STBC 0x00539550 _$E112
// FUNCTION: STBC 0x00539560 _$E390
// FUNCTION: STBC 0x00539570 _$E389
// FUNCTION: STBC 0x00539580 _$E118
// FUNCTION: STBC 0x00539590 _$E117
// FUNCTION: STBC 0x005395a0 _$E394
// FUNCTION: STBC 0x005395b0 _$E393
// FUNCTION: STBC 0x005395c0 _$E398
// FUNCTION: STBC 0x005395d0 _$E397
// FUNCTION: STBC 0x005395e0 _$E402
// FUNCTION: STBC 0x005395f0 _$E401
// FUNCTION: STBC 0x00539600 _$E406
// FUNCTION: STBC 0x00539610 _$E405
// FUNCTION: STBC 0x00539620 _$E127
// FUNCTION: STBC 0x00539630 _$E126
// FUNCTION: STBC 0x00539640 _$E410
// FUNCTION: STBC 0x00539650 _$E409
// FUNCTION: STBC 0x00539660 _$E414
// FUNCTION: STBC 0x00539670 _$E413
// FUNCTION: STBC 0x00539680 _$E418
// FUNCTION: STBC 0x00539690 _$E417
// FUNCTION: STBC 0x005396a0 _$E133
// FUNCTION: STBC 0x005396b0 _$E132
// FUNCTION: STBC 0x005396c0 _$E138
// FUNCTION: STBC 0x005396d0 _$E137
// FUNCTION: STBC 0x005396e0 _$E168
// FUNCTION: STBC 0x005396f0 _$E167
// FUNCTION: STBC 0x00539700 _$E422
// FUNCTION: STBC 0x00539710 _$E421
// FUNCTION: STBC 0x00539720 _$E173
// FUNCTION: STBC 0x00539730 _$E172
// FUNCTION: STBC 0x00539740 _$E426
// FUNCTION: STBC 0x00539750 _$E425
// FUNCTION: STBC 0x00539760 _$E430
// FUNCTION: STBC 0x00539770 _$E429
// FUNCTION: STBC 0x00539780 _$E178
// FUNCTION: STBC 0x00539790 _$E177
// FUNCTION: STBC 0x005397a0 _$E434
// FUNCTION: STBC 0x005397b0 _$E433
// FUNCTION: STBC 0x005397c0 _$E143
// FUNCTION: STBC 0x005397d0 _$E142
// FUNCTION: STBC 0x005397e0 _$E438
// FUNCTION: STBC 0x005397f0 _$E437
// FUNCTION: STBC 0x00539800 _$E442
// FUNCTION: STBC 0x00539810 _$E441
// FUNCTION: STBC 0x00539820 _$E183
// FUNCTION: STBC 0x00539830 _$E182
// FUNCTION: STBC 0x00539840 _$E188
// FUNCTION: STBC 0x00539850 _$E187
// FUNCTION: STBC 0x00539860 _$E446
// FUNCTION: STBC 0x00539870 _$E445
// FUNCTION: STBC 0x00539880 _$E450
// FUNCTION: STBC 0x00539890 _$E449
// FUNCTION: STBC 0x005398a0 _$E454
// FUNCTION: STBC 0x005398b0 _$E453
// FUNCTION: STBC 0x005398c0 _$E458
// FUNCTION: STBC 0x005398d0 _$E457
// FUNCTION: STBC 0x005398e0 _$E462
// FUNCTION: STBC 0x005398f0 _$E461
// FUNCTION: STBC 0x00539900 _$E466
// FUNCTION: STBC 0x00539910 _$E465
// FUNCTION: STBC 0x00539920 _$E470
// FUNCTION: STBC 0x00539930 _$E469
// FUNCTION: STBC 0x00539940 _$E474
// FUNCTION: STBC 0x00539950 _$E473
// FUNCTION: STBC 0x00539960 _$E193
// FUNCTION: STBC 0x00539970 _$E192
// FUNCTION: STBC 0x00539980 _$E478
// FUNCTION: STBC 0x00539990 _$E477
// FUNCTION: STBC 0x005399a0 _$E198
// FUNCTION: STBC 0x005399b0 _$E197
// FUNCTION: STBC 0x005399c0 _$E203
// FUNCTION: STBC 0x005399d0 _$E202
// FUNCTION: STBC 0x00539a10 _$E482
// FUNCTION: STBC 0x00539a20 _$E481
// FUNCTION: STBC 0x00539a30 _$E487
// FUNCTION: STBC 0x00539a40 _$E486
// FUNCTION: STBC 0x00539a50 _$E491
// FUNCTION: STBC 0x00539a60 _$E490

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00537740 _$E506
// FUNCTION: STBC 0x00537760 _$E505

// The time between the process's refreshes of the list, in seconds (invented name).
// DATA: STBC 0x00891138
static const float UPDATE_DELAY = 1.0f;

// DATA: STBC 0x00989d30
TGEventHandlerTable STTargetMenu::classHandlers;

/*
================
STTargetMenu::Cast

The object as a target menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00537bb0
STTargetMenu *STTargetMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STTARGETMENU_TYPE ) ) {
		return (STTargetMenu *)object;
	}
	return NULL;
}

/*
================
STTargetMenu::STTargetMenu

An empty list on an STTargetMenuSubPane that listens to the objects of the
player's set and refreshes itself every UPDATE_DELAY seconds.
================
*/
// FUNCTION: STBC 0x00537be0 ??0STTargetMenu@@QAE@PAVTGString@@@Z
STTargetMenu::STTargetMenu( TGString *name ) : STTopLevelMenu( name, STMF_DEFAULTS, NULL, new STTargetMenuSubPane( DEFAULT_ST_INDENT_VERT, false ) ), process( ProcessCallback, this ) {
	process.SetDelay( UPDATE_DELAY );
	maxTargets = 8;
	numTargets = 0;
	targets = new STTargetEntry[maxTargets];
	memset( targets, 0, maxTargets * sizeof( STTargetEntry ) );
	persistentTargetID = 0;
	persistentSubsystemID = 0;
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "STTargetMenu::ObjectEnteredSet" );
	g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "STTargetMenu::ObjectExitedSet" );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_EXPLODING, this, "STTargetMenu::HandleObjectExploding" );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_CHANGED, this, "STTargetMenu::ObjectChangedGroup" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "STTargetMenu::TargetChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_CLOAK_COMPLETED, this, "STTargetMenu::ObjectCloaked" );
	g_kEventManager.AddBroadcastMethodHandler( ET_DECLOAK_COMPLETED, this, "STTargetMenu::ObjectDecloaked" );
	g_kEventManager.AddBroadcastMethodHandler( ET_SENSORS_SHIP_IDENTIFIED, this, "STTargetMenu::ObjectIdentified" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGETED_SUBSYSTEM_CHANGED, this, "STTargetMenu::TargetedSubsystemChanged" );
	ClearTargetList();
	UpdateTargets();
	HighlightTarget( NULL );
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	subPane->SetRadioGroup( true );
}

/*
================
STTargetMenu::ProcessCallback
================
*/
// FUNCTION: STBC 0x00537f40
void STTargetMenu::ProcessCallback( double time, void *data ) {
	( (STTargetMenu *)data )->Unknown538C80();
}

/*
================
STTargetMenu::STTargetMenu

An empty menu for the load to fill.
================
*/
// FUNCTION: STBC 0x00537f80 ??0STTargetMenu@@QAE@J@Z
STTargetMenu::STTargetMenu( long id ) : STTopLevelMenu( id ), process( ProcessCallback, this ) {
	process.SetDelay( UPDATE_DELAY );
	maxTargets = 0;
	numTargets = 0;
	targets = NULL;
	persistentTargetID = 0;
	persistentSubsystemID = 0;
}

/*
================
STTargetMenu::~STTargetMenu
================
*/
// FUNCTION: STBC 0x00538100 ??1STTargetMenu@@UAE@XZ
STTargetMenu::~STTargetMenu( void ) {
	if ( targets != NULL ) {
		delete targets;
	}
}

/*
================
STTargetMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00538170
void STTargetMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectEnteredSet, "STTargetMenu::ObjectEnteredSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::HandleObjectExploding, "STTargetMenu::HandleObjectExploding" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectExitedSet, "STTargetMenu::ObjectExitedSet" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectChangedGroup, "STTargetMenu::ObjectChangedGroup" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectCloaked, "STTargetMenu::ObjectCloaked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectDecloaked, "STTargetMenu::ObjectDecloaked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::ObjectIdentified, "STTargetMenu::ObjectIdentified" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::TargetButtonClicked, "STTargetMenu::TargetButtonClicked" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::TargetChanged, "STTargetMenu::TargetChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STTargetMenu::TargetedSubsystemChanged, "STTargetMenu::TargetedSubsystemChanged" );
}

/*
================
STTargetMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00538240
void STTargetMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_TARGET_LIST_CLICKED, "STTargetMenu::TargetButtonClicked" );
}

/*
================
STTargetMenu::GetTargetMenu

The tactical window's target menu; NULL when there is no tactical window.
================
*/
// FUNCTION: STBC 0x00538260
STTargetMenu *STTargetMenu::GetTargetMenu( void ) {
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	if ( window ) {
		return window->GetTargetMenu();
	}
	return NULL;
}

/*
================
STTargetMenu::ObjectEnteredSet

An object entering the player's set refreshes the list; the player's ship
entering one starts it afresh.
================
*/
// FUNCTION: STBC 0x00538280
void STTargetMenu::ObjectEnteredSet( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	if ( player != NULL && object != NULL && player->GetContainingSet() == object->GetContainingSet() ) {
		if ( player == object ) {
			ClearTargetList();
		}
		UpdateTargets();
	}
}

/*
================
STTargetMenu::ObjectExitedSet

An object leaving the player's set refreshes the list.
================
*/
// FUNCTION: STBC 0x005382d0
void STTargetMenu::ObjectExitedSet( TGStringEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( object != NULL && player != NULL ) {
		char *setName = event->GetCString();
		if ( player->GetContainingSet() != NULL && strcmp( player->GetContainingSet()->GetName(), setName ) == 0 ) {
			UpdateTargets();
		}
		delete setName;
	}
}

/*
================
STTargetMenu::HandleObjectExploding

Closes and empties the exploding object's menu.
================
*/
// FUNCTION: STBC 0x00538360
void STTargetMenu::HandleObjectExploding( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( object != NULL && player != NULL && player->GetContainingSet() == object->GetContainingSet() ) {
		STMenu *menu = GetObjectEntry( object );
		if ( menu != NULL ) {
			menu->Close( false );
			menu->KillChildren();
		}
		UpdateTargets();
	}
}

/*
================
STTargetMenu::ObjectChangedGroup
================
*/
// FUNCTION: STBC 0x005383c0
void STTargetMenu::ObjectChangedGroup( TGEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL && game->GetPlayerSet() != NULL ) {
		ResetAffiliationColors();
	}
}

/*
================
STTargetMenu::ResetAffiliationColors
================
*/
// FUNCTION: STBC 0x005383f0
void STTargetMenu::ResetAffiliationColors( void ) {
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( i ) );
		if ( menu ) {
			menu->Unknown5273F0();
		}
	}
}

/*
================
STTargetMenu::ObjectCloaked
================
*/
// FUNCTION: STBC 0x00538440
void STTargetMenu::ObjectCloaked( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( object != NULL && player != NULL && player->GetContainingSet() == object->GetContainingSet() ) {
		UpdateTargets();
	}
}

/*
================
STTargetMenu::ObjectDecloaked
================
*/
// FUNCTION: STBC 0x00538480
void STTargetMenu::ObjectDecloaked( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( object != NULL && player != NULL && player->GetContainingSet() == object->GetContainingSet() ) {
		UpdateTargets();
	}
}

/*
================
STTargetMenu::ObjectIdentified

The identified ship gets a menu if it has none, and shows its name.
================
*/
// FUNCTION: STBC 0x005384c0
void STTargetMenu::ObjectIdentified( TGObjPtrEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetObjPtr() );
	if ( object == NULL ) {
		return;
	}
	int index = FindTarget( object );
	if ( index >= 0 ) {
		targets[index].targetable = true;
		STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( index ) );
		if ( menu != NULL ) {
			menu->ShowRealName();
			menu->ForceUpdate( true );
		}
	} else {
		AddTarget( object );
		index = FindTarget( object );
		if ( index >= 0 ) {
			targets[index].targetable = true;
			STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( index ) );
			if ( menu != NULL ) {
				menu->ShowRealName();
				menu->ForceUpdate( true );
			}
		}
	}
}

/*
================
STTargetMenu::TargetedSubsystemChanged

Keeps the player's targeted subsystem as the persistent one.
================
*/
// FUNCTION: STBC 0x00538560
void STTargetMenu::TargetedSubsystemChanged( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && event->GetDestination() == player ) {
		ShipSubsystem *subsystem = player->GetTargetSubsystem();
		if ( subsystem != NULL ) {
			persistentSubsystemID = subsystem->GetObjID();
		}
	}
}

/*
================
STTargetMenu::TargetButtonClicked

The player targets the object of the clicked menu.
================
*/
// FUNCTION: STBC 0x00538590
void STTargetMenu::TargetButtonClicked( TGIntEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && player->GetContainingSet() != NULL ) {
		ObjectClass *target = ObjectClass::GetObjectByID( player->GetContainingSet(), event->GetInt() );
		if ( target != NULL && player->CanTargetObject( target ) && player->GetTarget() != target ) {
			player->SetTarget( target, NULL );
			SetPersistentTarget( target );
		}
	}
	CallNextHandler( event );
}

/*
================
STTargetMenu::SetPersistentTarget
================
*/
// FUNCTION: STBC 0x00538600
void STTargetMenu::SetPersistentTarget( ObjectClass *target ) {
	HighlightTarget( target );
	if ( target != NULL ) {
		persistentTargetID = target->GetObjID();
	}
}

/*
================
STTargetMenu::SetFocus

Unchooses the menu that had the focus and chooses the new one.
================
*/
// FUNCTION: STBC 0x00538620 ?SetFocus@STTargetMenu@@UAEXPAVTGUIObject@@_N@Z
void STTargetMenu::SetFocus( TGUIObject *child, bool layout ) {
	STSubsystemMenu *focusMenu = STSubsystemMenu::Cast( GetFocus() );
	if ( child != GetSubPane() && focusMenu != NULL ) {
		focusMenu->SetChosen( false, false );
	}
	STSubsystemMenu *menu = STSubsystemMenu::Cast( child );
	if ( menu != NULL ) {
		menu->SetChosen( true, false );
	}
	STMenu::SetFocus( child, layout );
}

/*
================
STTargetMenu::TargetChanged
================
*/
// FUNCTION: STBC 0x00538690
void STTargetMenu::TargetChanged( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && event->GetDestination() == player ) {
		SetPersistentTarget( player->GetTarget() );
	}
}

/*
================
STTargetMenu::HighlightTarget
================
*/
// FUNCTION: STBC 0x005386c0
void STTargetMenu::HighlightTarget( ObjectClass *target ) {
	int count = GetNumChildren();
	int i;
	for ( i = 0; i < count; i++ ) {
		TGUIObject *child = GetNthChild( i );
		child->SetNotHighlighted( false );
		STSubsystemMenu *menu = STSubsystemMenu::Cast( child );
		if ( menu != NULL ) {
			menu->SetChosen( false, false );
		}
	}
	STMenu *entry = NULL;
	if ( target != NULL ) {
		entry = GetObjectEntry( target );
		if ( entry != NULL ) {
			entry->SetHighlighted( false );
			SetFocus( entry, false );
		}
	}
	for ( i = 0; i < count; i++ ) {
		STMenu *menu = STMenu::Cast( GetNthChild( i ) );
		if ( menu != NULL && menu != entry ) {
			menu->Close( false );
		}
	}
	Layout();
}

/*
================
STTargetMenu::AddTarget

A menu for a ship the player can target that has none, closed and without
the focus, at the end of the list; the table grows by doubling.
================
*/
// FUNCTION: STBC 0x00538790
void STTargetMenu::AddTarget( ObjectClass *object ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || !player->CanTargetObject( object ) ) {
		return;
	}
	DamageableObject *damageable = DamageableObject::Cast( object );
	if ( damageable != NULL && damageable->IsDead() ) {
		return;
	}
	if ( GetObjectEntry( object ) != NULL ) {
		return;
	}
	ShipClass *ship = ShipClass::Cast( object );
	if ( ship == NULL ) {
		return;
	}

	TGIntEvent *clicked = new TGIntEvent;
	clicked->SetEventType( ET_TARGET_LIST_CLICKED );
	clicked->SetDestination( this );
	clicked->SetInt( object->GetObjID() );
	STSubsystemMenu *menu = new STSubsystemMenu( ship, clicked );
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( sensors != NULL ) {
		if ( sensors->IsObjectKnown( object ) == true ) {
			menu->ShowRealName();
		} else {
			menu->ShowUnknownName();
		}
	} else {
		menu->ShowUnknownName();
	}
	menu->SetNotOpenable();
	menu->SetNoFocus();
	menu->ForceUpdate( false );
	AddChild( menu );
	menu->SetNotNoFocus();
	menu->SetNotHighlighted( false );

	if ( numTargets >= maxTargets ) {
		STTargetEntry *newTargets = new STTargetEntry[maxTargets * 2];
		memcpy( newTargets, targets, maxTargets * sizeof( STTargetEntry ) );
		memset( newTargets + maxTargets, 0, maxTargets * sizeof( STTargetEntry ) );
		delete targets;
		targets = newTargets;
		maxTargets *= 2;
	}
	targets[numTargets].objectID = object->GetObjID();
	targets[numTargets].targetable = false;
	numTargets++;

	TGEvent *added = new TGEvent;
	added->SetSource( menu );
	added->SetDestination( ship );
	added->SetEventType( ET_TARGET_LIST_OBJECT_ADDED );
	g_kEventManager.AddEvent( added );
}

/*
================
STTargetMenu::RemoveTarget
================
*/
// FUNCTION: STBC 0x005389d0
void STTargetMenu::RemoveTarget( ObjectClass *object ) {
	int index = FindTarget( object );
	if ( index != -1 ) {
		RemoveTargetAt( index );
	}
	TGEvent *event = new TGEvent;
	event->SetDestination( object );
	event->SetSource( this );
	event->SetEventType( ET_TARGET_LIST_OBJECT_REMOVED );
	g_kEventManager.AddEvent( event );
}

/*
================
STTargetMenu::RemoveTargetAt

Closes and deletes the entry's menu and closes the table's gap.
================
*/
// FUNCTION: STBC 0x00538a50
void STTargetMenu::RemoveTargetAt( int index ) {
	if ( index < 0 || index >= numTargets ) {
		return;
	}
	TGUIObject *child = GetNthChild( index );
	if ( child != NULL ) {
		STMenu *menu = STMenu::Cast( child );
		if ( menu != NULL ) {
			menu->Close( false );
		}
		if ( GetFocus() == child ) {
			SetFocus( NULL, false );
		}
		DeleteChild( child, true );
	}
	memmove( &targets[index], &targets[index + 1], ( numTargets - ( index + 1 ) ) * sizeof( STTargetEntry ) );
	numTargets--;
	memset( &targets[numTargets], 0, sizeof( STTargetEntry ) );
}

/*
================
STTargetMenu::GetObjectEntry

The menu of the object; NULL when the object is not a target.
================
*/
// FUNCTION: STBC 0x00538b00
STMenu *STTargetMenu::GetObjectEntry( ObjectClass *object ) {
	int index = FindTarget( object );
	if ( index != -1 ) {
		return STMenu::Cast( GetNthChild( index ) );
	}
	return NULL;
}

/*
================
STTargetMenu::ClearPersistentTarget
================
*/
// FUNCTION: STBC 0x00538b30
void STTargetMenu::ClearPersistentTarget( void ) {
	persistentTargetID = 0;
	persistentSubsystemID = 0;
}

/*
================
STTargetMenu::ClearTargetList

Deletes every target's menu and empties the table.
================
*/
// FUNCTION: STBC 0x00538b40
void STTargetMenu::ClearTargetList( void ) {
	for ( int i = GetNumChildren() - 1; i >= 0; i-- ) {
		STMenu *menu = STMenu::Cast( GetNthChild( i ) );
		if ( menu ) {
			RemoveChild( menu, false );
			delete menu;
		}
	}
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane ) {
		subPane->ResizeToContents( true );
	} else {
		ResizeToContents( true );
	}
	memset( targets, 0, numTargets * sizeof( STTargetEntry ) );
	numTargets = 0;
}

/*
================
STTargetMenu::RebuildShipMenu

Rebuilds the items of the object's menu.
================
*/
// FUNCTION: STBC 0x00538be0
void STTargetMenu::RebuildShipMenu( ObjectClass *object ) {
	STSubsystemMenu *menu = STSubsystemMenu::Cast( GetObjectEntry( object ) );
	if ( menu ) {
		menu->VirtualSlot115();
	}
}

/*
================
STTargetMenu::RebuildShipMenus
================
*/
// FUNCTION: STBC 0x00538c10
void STTargetMenu::RebuildShipMenus( void ) {
	for ( int i = 0; i < numTargets; i++ ) {
		STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( i ) );
		if ( menu ) {
			menu->VirtualSlot115();
		}
	}
}

/*
================
STTargetMenu::FindTarget
================
*/
// FUNCTION: STBC 0x00538c50
int STTargetMenu::FindTarget( ObjectClass *object ) {
	if ( object != NULL ) {
		for ( int i = 0; i < numTargets; i++ ) {
			if ( targets[i].objectID == object->GetObjID() ) {
				return i;
			}
		}
	}
	return -1;
}

/*
================
STTargetMenu::Unknown538C80
================
*/
// FUNCTION: STBC 0x00538c80
void STTargetMenu::Unknown538C80( void ) {
	UpdateTargets();
}

/*
================
STTargetMenu::UpdateTargets

Adds a menu for each live ship of the player's set the player can target
(hidden while the sensors do not see it), then walks the table from the
end: an object that is gone loses its entry, one the player can no longer
target is removed, and the others show what the sensors know of them.
Ends by giving back the persistent target.
================
*/
// FUNCTION: STBC 0x00538c90
void STTargetMenu::UpdateTargets( void ) {
	bool changed = false;
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( sensors == NULL ) {
		return;
	}
	SetClass *set = player->GetContainingSet();
	if ( set != NULL ) {
		int count = numTargets;
		bool *targetable = new bool[count];
		memset( targetable, 0, count );
		int numObjects;
		ObjectClass **objects = set->GetTargetableObjects( &numObjects, player, false );
		int i;
		for ( i = 0; i < numObjects; i++ ) {
			ObjectClass *object = objects[i];
			ShipClass *ship = ShipClass::Cast( object );
			int index = FindTarget( object );
			if ( index == -1 ) {
				if ( ship != NULL && !ship->IsDead() && ship->GetLifeTime() >= INFINITE_LIFE ) {
					AddTarget( object );
					changed = true;
					if ( !sensors->IsObjectVisible( object ) ) {
						index = FindTarget( object );
						if ( index >= 0 ) {
							STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( index ) );
							if ( menu != NULL ) {
								menu->SetNotVisible( true );
							}
						}
					}
				}
			} else {
				if ( player->CanTargetObject( object ) ) {
					targetable[index] = true;
				}
				DamageableObject *damageable = DamageableObject::Cast( object );
				if ( damageable != NULL && damageable->IsDead() ) {
					targetable[index] = false;
				}
			}
		}

		for ( i = count - 1; i >= 0; i-- ) {
			ObjectClass *object = ObjectClass::GetObjectByID( NULL, targets[i].objectID );
			if ( object == NULL ) {
				RemoveTargetAt( i );
				continue;
			}
			int index = FindTarget( object );
			STSubsystemMenu *menu = STSubsystemMenu::Cast( GetNthChild( index ) );
			if ( menu == NULL ) {
				continue;
			}
			if ( !targetable[i] ) {
				RemoveTarget( object );
				continue;
			}
			if ( !menu->IsVisible() ) {
				menu->SetVisible( true );
				changed = true;
			}
			if ( sensors->IsObjectKnown( object ) ) {
				if ( !menu->IsShowingRealName() ) {
					menu->ShowRealName();
					changed = true;
				}
				if ( !menu->IsOpenable() && menu->GetNumChildren() > 0 ) {
					menu->SetOpenable();
					changed = true;
				}
			} else {
				if ( menu->IsShowingRealName() == true ) {
					menu->ShowUnknownName();
					changed = true;
				}
				if ( menu->IsOpenable() == true ) {
					if ( menu->IsOpened() == true ) {
						menu->Close( true );
						changed = true;
					}
					menu->SetNotOpenable();
				}
			}
			menu->VirtualSlot113();
			menu->VirtualSlot114();
		}

		delete objects;
		delete targetable;
		if ( changed == true ) {
			STSubPane *subPane = STSubPane::Cast( GetSubPane() );
			if ( subPane != NULL ) {
				subPane->ResizeToContents( true );
			} else {
				ResizeToContents( true );
			}
		}
	}
	RestorePersistentTarget();
}

/*
================
STTargetMenu::RestorePersistentTarget

When the player has no target, targets the persistent one again with its
subsystem, after an ET_RESTORE_PERSISTENT_TARGET event; a persistent
target that is gone, dying or dead is forgotten.
================
*/
// FUNCTION: STBC 0x00538fc0
// DRAFT: register residue: retail keeps the player in EBX and the target in EDI, this build the other way round; calls and branches agree
void STTargetMenu::RestorePersistentTarget( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->GetTarget() != NULL || persistentTargetID == 0 ) {
		return;
	}
	ObjectClass *target = ObjectClass::Cast( TGObject::GetTGObjectPtr( persistentTargetID ) );
	if ( target != NULL ) {
		DamageableObject *damageable = DamageableObject::Cast( target );
		if ( damageable == NULL || ( damageable->GetLifeTime() >= INFINITE_LIFE && !damageable->IsDead() ) ) {
			if ( !player->CanTargetObject( target ) ) {
				return;
			}
			ShipSubsystem *subsystem = (ShipSubsystem *)TGObject::GetTGObjectPtr( persistentSubsystemID );
			if ( ShipSubsystem::Cast( subsystem ) == NULL ) {
				subsystem = NULL;
			}
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_RESTORE_PERSISTENT_TARGET );
			event->SetSource( this );
			event->SetDestination( player );
			g_kEventManager.AddEvent( event );
			player->SetTarget( target, subsystem );
			return;
		}
	}
	ClearPersistentTarget();
}

/*
================
STTargetMenu::WriteToStream

The menu, then the table's size and count, every allocated entry and the
persistent target's and subsystem's IDs.
================
*/
// FUNCTION: STBC 0x005390e0 ?WriteToStream@STTargetMenu@@UAE_NPAVTGStream@@@Z
bool STTargetMenu::WriteToStream( TGStream *stream ) {
	if ( !STTopLevelMenu::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( maxTargets );
	stream->WriteInt( numTargets );
	for ( int i = 0; i < maxTargets; i++ ) {
		stream->WriteID( targets[i].objectID );
		stream->WriteBool( targets[i].targetable );
	}
	stream->WriteID( persistentTargetID );
	stream->WriteID( persistentSubsystemID );
	return true;
}

/*
================
STTargetMenu::ReadFromStream
================
*/
// FUNCTION: STBC 0x00539170 ?ReadFromStream@STTargetMenu@@UAE_NPAVTGStream@@@Z
bool STTargetMenu::ReadFromStream( TGStream *stream ) {
	if ( !STTopLevelMenu::ReadFromStream( stream ) ) {
		return false;
	}
	maxTargets = stream->ReadInt();
	numTargets = stream->ReadInt();
	targets = new STTargetEntry[maxTargets];
	for ( int i = 0; i < maxTargets; i++ ) {
		targets[i].objectID = stream->ReadID();
		targets[i].targetable = stream->ReadBool();
	}
	persistentTargetID = stream->ReadID();
	persistentSubsystemID = stream->ReadID();
	return true;
}
