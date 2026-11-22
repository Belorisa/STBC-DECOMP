/*
===============================================================================

	TacWeaponsCtrl.cpp

===============================================================================
*/

#include <stdio.h>

#include "TacWeaponsCtrl.h"
#include "STSubPane.h"
#include "TorpedoSystemProperty.h"
#include "WeaponSystem.h"
#include "STToggle.h"
#include "Game.h"
#include "ShipClass.h"
#include "CloakingSubsystem.h"
#include "PhaserSystem.h"
#include "TractorBeamSystem.h"
#include "FiringChain.h"
#include "TGString.h"
#include "TorpedoSystem.h"
#include "TorpedoAmmoType.h"
#include "STColors.h"
#include "TGEvent.h"
#include "GraphicsModeInfo.h"
#include "InterfaceModule.h"
#include "PyEmbed.h"
#include "ShipSubsystem.h"
#include "TGIcon.h"
#include "TGKeyboardEvent.h"
#include "TGFontManager.h"
#include "TGFontGroup.h"
#include "TGParagraph.h"
#include "TGPane.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TractorBeamProjector.h"
#include "UtopiaModule.h"
#include <iostream>
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0054ab50 _$E541
// FUNCTION: STBC 0x0054ab70 _$E540

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005479b0 _$E33
// FUNCTION: STBC 0x005479c0 _$E32
// FUNCTION: STBC 0x005479d0 _$E37
// FUNCTION: STBC 0x005479e0 _$E36
// FUNCTION: STBC 0x005479f0 _$E48
// FUNCTION: STBC 0x00547a00 _$E47
// FUNCTION: STBC 0x00547a10 _$E52
// FUNCTION: STBC 0x00547a20 _$E51
// FUNCTION: STBC 0x00547a30 _$E56
// FUNCTION: STBC 0x00547a40 _$E55
// FUNCTION: STBC 0x00547a50 _$E60
// FUNCTION: STBC 0x00547a60 _$E59
// FUNCTION: STBC 0x00547a70 _$E184
// FUNCTION: STBC 0x00547a80 _$E183
// FUNCTION: STBC 0x005489c0 _$E126
// FUNCTION: STBC 0x005489d0 _$E125
// FUNCTION: STBC 0x005489e0 _$E139
// FUNCTION: STBC 0x005489f0 _$E138
// FUNCTION: STBC 0x00548e40 _$E189
// FUNCTION: STBC 0x00548e50 _$E188
// FUNCTION: STBC 0x00549900 _$E194
// FUNCTION: STBC 0x00549910 _$E193
// FUNCTION: STBC 0x00549aa0 _$E198
// FUNCTION: STBC 0x00549ab0 _$E197
// FUNCTION: STBC 0x0054a100 _$E202
// FUNCTION: STBC 0x0054a110 _$E201
// FUNCTION: STBC 0x0054a3f0 _$E209
// FUNCTION: STBC 0x0054a400 _$E208
// FUNCTION: STBC 0x0054a450 _$E388
// FUNCTION: STBC 0x0054a460 _$E387
// FUNCTION: STBC 0x0054a470 _$E392
// FUNCTION: STBC 0x0054a480 _$E391
// FUNCTION: STBC 0x0054a490 _$E84
// FUNCTION: STBC 0x0054a4a0 _$E83
// FUNCTION: STBC 0x0054a4b0 _$E276
// FUNCTION: STBC 0x0054a4c0 _$E275
// FUNCTION: STBC 0x0054a4d0 _$E283
// FUNCTION: STBC 0x0054a4e0 _$E282
// FUNCTION: STBC 0x0054a4f0 _$E287
// FUNCTION: STBC 0x0054a500 _$E286
// FUNCTION: STBC 0x0054a510 _$E291
// FUNCTION: STBC 0x0054a520 _$E290
// FUNCTION: STBC 0x0054a530 _$E295
// FUNCTION: STBC 0x0054a540 _$E294
// FUNCTION: STBC 0x0054a550 _$E301
// FUNCTION: STBC 0x0054a560 _$E300
// FUNCTION: STBC 0x0054a570 _$E310
// FUNCTION: STBC 0x0054a580 _$E309
// FUNCTION: STBC 0x0054a590 _$E314
// FUNCTION: STBC 0x0054a5a0 _$E313
// FUNCTION: STBC 0x0054a5b0 _$E396
// FUNCTION: STBC 0x0054a5c0 _$E395
// FUNCTION: STBC 0x0054a5d0 _$E400
// FUNCTION: STBC 0x0054a5e0 _$E399
// FUNCTION: STBC 0x0054a5f0 _$E404
// FUNCTION: STBC 0x0054a600 _$E403
// FUNCTION: STBC 0x0054a610 _$E408
// FUNCTION: STBC 0x0054a620 _$E407
// FUNCTION: STBC 0x0054a630 _$E356
// FUNCTION: STBC 0x0054a640 _$E355
// FUNCTION: STBC 0x0054a650 _$E412
// FUNCTION: STBC 0x0054a660 _$E411
// FUNCTION: STBC 0x0054a670 _$E416
// FUNCTION: STBC 0x0054a680 _$E415
// FUNCTION: STBC 0x0054a690 _$E420
// FUNCTION: STBC 0x0054a6a0 _$E419
// FUNCTION: STBC 0x0054a6b0 _$E89
// FUNCTION: STBC 0x0054a6c0 _$E88
// FUNCTION: STBC 0x0054a6d0 _$E424
// FUNCTION: STBC 0x0054a6e0 _$E423
// FUNCTION: STBC 0x0054a6f0 _$E94
// FUNCTION: STBC 0x0054a700 _$E93
// FUNCTION: STBC 0x0054a710 _$E428
// FUNCTION: STBC 0x0054a720 _$E427
// FUNCTION: STBC 0x0054a730 _$E432
// FUNCTION: STBC 0x0054a740 _$E431
// FUNCTION: STBC 0x0054a750 _$E436
// FUNCTION: STBC 0x0054a760 _$E435
// FUNCTION: STBC 0x0054a770 _$E440
// FUNCTION: STBC 0x0054a780 _$E439
// FUNCTION: STBC 0x0054a790 _$E103
// FUNCTION: STBC 0x0054a7a0 _$E102
// FUNCTION: STBC 0x0054a7b0 _$E444
// FUNCTION: STBC 0x0054a7c0 _$E443
// FUNCTION: STBC 0x0054a7d0 _$E448
// FUNCTION: STBC 0x0054a7e0 _$E447
// FUNCTION: STBC 0x0054a7f0 _$E452
// FUNCTION: STBC 0x0054a800 _$E451
// FUNCTION: STBC 0x0054a810 _$E109
// FUNCTION: STBC 0x0054a820 _$E108
// FUNCTION: STBC 0x0054a830 _$E114
// FUNCTION: STBC 0x0054a840 _$E113
// FUNCTION: STBC 0x0054a850 _$E144
// FUNCTION: STBC 0x0054a860 _$E143
// FUNCTION: STBC 0x0054a870 _$E456
// FUNCTION: STBC 0x0054a880 _$E455
// FUNCTION: STBC 0x0054a890 _$E149
// FUNCTION: STBC 0x0054a8a0 _$E148
// FUNCTION: STBC 0x0054a8b0 _$E460
// FUNCTION: STBC 0x0054a8c0 _$E459
// FUNCTION: STBC 0x0054a8d0 _$E464
// FUNCTION: STBC 0x0054a8e0 _$E463
// FUNCTION: STBC 0x0054a8f0 _$E154
// FUNCTION: STBC 0x0054a900 _$E153
// FUNCTION: STBC 0x0054a910 _$E468
// FUNCTION: STBC 0x0054a920 _$E467
// FUNCTION: STBC 0x0054a930 _$E119
// FUNCTION: STBC 0x0054a940 _$E118
// FUNCTION: STBC 0x0054a950 _$E472
// FUNCTION: STBC 0x0054a960 _$E471
// FUNCTION: STBC 0x0054a970 _$E476
// FUNCTION: STBC 0x0054a980 _$E475
// FUNCTION: STBC 0x0054a990 _$E159
// FUNCTION: STBC 0x0054a9a0 _$E158
// FUNCTION: STBC 0x0054a9b0 _$E164
// FUNCTION: STBC 0x0054a9c0 _$E163
// FUNCTION: STBC 0x0054a9d0 _$E480
// FUNCTION: STBC 0x0054a9e0 _$E479
// FUNCTION: STBC 0x0054a9f0 _$E484
// FUNCTION: STBC 0x0054aa00 _$E483
// FUNCTION: STBC 0x0054aa10 _$E488
// FUNCTION: STBC 0x0054aa20 _$E487
// FUNCTION: STBC 0x0054aa30 _$E492
// FUNCTION: STBC 0x0054aa40 _$E491
// FUNCTION: STBC 0x0054aa50 _$E496
// FUNCTION: STBC 0x0054aa60 _$E495
// FUNCTION: STBC 0x0054aa70 _$E500
// FUNCTION: STBC 0x0054aa80 _$E499
// FUNCTION: STBC 0x0054aa90 _$E504
// FUNCTION: STBC 0x0054aaa0 _$E503
// FUNCTION: STBC 0x0054aab0 _$E508
// FUNCTION: STBC 0x0054aac0 _$E507
// FUNCTION: STBC 0x0054aad0 _$E169
// FUNCTION: STBC 0x0054aae0 _$E168
// FUNCTION: STBC 0x0054aaf0 _$E512
// FUNCTION: STBC 0x0054ab00 _$E511
// FUNCTION: STBC 0x0054ab10 _$E174
// FUNCTION: STBC 0x0054ab20 _$E173
// FUNCTION: STBC 0x0054ab30 _$E179
// FUNCTION: STBC 0x0054ab40 _$E178
// FUNCTION: STBC 0x0054ab80 _$E516
// FUNCTION: STBC 0x0054ab90 _$E515
// FUNCTION: STBC 0x0054aba0 _$E521
// FUNCTION: STBC 0x0054abb0 _$E520
// FUNCTION: STBC 0x0054abc0 _$E525
// FUNCTION: STBC 0x0054abd0 _$E524

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00547930 _$E372
// FUNCTION: STBC 0x00547940 _$E369
// FUNCTION: STBC 0x00547950 _$E371
// FUNCTION: STBC 0x00547960 _$E370
// FUNCTION: STBC 0x00547970 _$E378
// FUNCTION: STBC 0x00547980 _$E375
// FUNCTION: STBC 0x00547990 _$E377
// FUNCTION: STBC 0x005479a0 _$E376

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00547a90 _$E531
// FUNCTION: STBC 0x00547aa0 _$E527
// FUNCTION: STBC 0x00547ab0 _$E530
// FUNCTION: STBC 0x00547ac0 _$E528

// The delay of the control's first refresh (invented name).
// DATA: STBC 0x008e40ec
static const float REFRESH_DELAY = 1.0f;

// DATA: STBC 0x008e40f0
float TAC_WEAPONS_CTRL_INDENT_HORIZ = 0.003f;

// DATA: STBC 0x0098ab70
TGEventHandlerTable TacWeaponsCtrl::classHandlers;

// DATA: STBC 0x0098ac54
TacWeaponsCtrl *TacWeaponsCtrl::instance;

/*
================
TacWeaponsCtrl::Cast

The object as a TacWeaponsCtrl; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00547ae0
TacWeaponsCtrl *TacWeaponsCtrl::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TACWEAPONSCTRL_TYPE ) ) {
		return (TacWeaponsCtrl *)object;
	}
	return NULL;
}

/*
================
TacWeaponsCtrl::TacWeaponsCtrl

The control becomes the one there is. The panes are built by Init; the
control then listens for the player, the target, the torpedo count, the
tractor beam and the subsystems changing, and starts a timer that refreshes
it.
================
*/
// FUNCTION: STBC 0x00547b10 ??0TacWeaponsCtrl@@QAE@MM@Z
TacWeaponsCtrl::TacWeaponsCtrl( float width, float height ) : STStylizedWindow( "StylizedWindow", "LeftSeparator", NULL, 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor ) {
	instance = this;
	Resize( width, height, false );
	Init();

	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "TacWeaponsCtrl::HandlePlayerChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "TacWeaponsCtrl::HandleTargetChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_PLAYER_TORPEDO_COUNT_CHANGED, this, "TacWeaponsCtrl::HandleTorpedoCountChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TRACTOR_BEAM_STARTED_FIRING, this, "TacWeaponsCtrl::HandleTractorBeamFiring" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TRACTOR_BEAM_STOPPED_FIRING, this, "TacWeaponsCtrl::HandleTractorBeamFiring" );

	TGEvent *event = new TGEvent;
	event->SetEventType( ET_TAC_WEAPONS_REFRESH );
	event->SetDestination( this );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + REFRESH_DELAY );
	timer->SetEvent( event );
	timerID = timer->GetObjID();
	g_kTimerManager.AddTimer( timer );

	SetNotHighlighted( false );
	SetUseFocusGlass( true );
}

/*
================
TacWeaponsCtrl::~TacWeaponsCtrl

The control is no more; its timer goes.
================
*/
// FUNCTION: STBC 0x00547de0 ??1TacWeaponsCtrl@@UAE@XZ
TacWeaponsCtrl::~TacWeaponsCtrl() {
	if ( instance == this ) {
		instance = NULL;
	}
	g_kTimerManager.DeleteTimer( timerID );
}

/*
================
TacWeaponsCtrl::Init

Lays out the control's panes one under another with a two pixel gap, the
separator icon between the second and the third, and gives the focus to the
first child with something to focus. The panes are the weapons', the
tractor and cloak toggles' and the third one (names invented).
================
*/
// FUNCTION: STBC 0x00547e50
// DRAFT: retail stores the third pane's y (the separator's height added to y) before the call to Unknown548E60; this build folds it into the AddChild argument
void TacWeaponsCtrl::Init( void ) {
	KillChildren();
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	char *lcars = mode->GetLcarsString();
	float gap = 2 * mode->GetPixelHeight();
	float paneWidth;
	float paneHeight;
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_WIDTH", "f", &paneWidth );
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_HEIGHT", "f", &paneHeight );

	float y = 0.0f;
	TGUIObject *first = Unknown548120( lcars );
	AddChild( first, 0.0f, 0.0f, false );
	if ( first->GetHeight() > 0.0f ) {
		y = first->GetHeight() + gap;
	}
	first->AddFuncHandlerForInstance( ET_KEYBOARD, "TacWeaponsCtrl::KeyboardHandler" );

	TGUIObject *second = Unknown548A00( lcars );
	AddChild( second, 0.0f, y, false );
	if ( second->GetHeight() > 0.0f ) {
		y = second->GetHeight() + y + gap;
	}
	second->AddFuncHandlerForInstance( ET_KEYBOARD, "TacWeaponsCtrl::KeyboardHandler" );

	TGIcon *separator = new TGIcon( lcars, 200, &g_kLeftSeparatorColor );
	AddChild( separator, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
	separator->Resize( paneWidth - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), gap, false );
	y = separator->GetHeight() + y + gap;

	TGUIObject *third = Unknown548E60( lcars );
	AddChild( third, 0.0f, y, false );
	third->AddFuncHandlerForInstance( ET_KEYBOARD, "TacWeaponsCtrl::KeyboardHandler" );
	if ( third->GetHeight() < 0.0f ) {
		separator->SetNotVisible( false );
	}

	for ( TGUIObject *child = GetFirstChild(); child != NULL; child = GetNextChild( child ) ) {
		if ( InterfaceModule::HasUIElement( child, 0 ) == true ) {
			TGPane *childPane = TGPane::Cast( child );
			if ( childPane == NULL ) {
				SetFocus( child, true );
			} else {
				SetFocus( childPane, true );
				childPane->SetFocus( InterfaceModule::GetFirstUIElement( childPane, 0 ), true );
			}
			break;
		}
	}

	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_COMPLETELY_DISABLED, this, "TacWeaponsCtrl::HandleSubsystemDisabled", player );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_PARTIALLY_OPERATIONAL, this, "TacWeaponsCtrl::HandleSubsystemOperational", player );
	}
}

/*
================
TacWeaponsCtrl::Unknown548120

Builds the first pane for a ship with torpedoes: the "Torpedoes" heading and
the count of the current type's torpedoes, the type toggle and the spread
toggle (named for the firing chains, hidden for a single chain). The pane is
empty for any other ship (name invented; the parameter, the icon group, is
not read).
================
*/
// FUNCTION: STBC 0x00548120 ?Unknown548120@TacWeaponsCtrl@@IAEPAVTGUIObject@@PAD@Z
// DRAFT: first pass from the disassembly; the y positions of the four children and the colour setters (retail: one out-of-line NiColorA::operator=) are not settled
TGUIObject *TacWeaponsCtrl::Unknown548120( char *lcars ) {
	TGParagraph *label = NULL;
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	float gap = 2 * mode->GetPixelHeight();
	TGFontGroup *defaultFont = g_kFontManager.GetDefaultFont();
	TGFontGroup *headerFont = g_kFontManager.GetFontGroup( defaultFont->GetFontName(), 5.0f );
	float paneWidth;
	float paneHeight;
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_WIDTH", "f", &paneWidth );
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_HEIGHT", "f", &paneHeight );
	ShipClass *player = Game::GetCurrentPlayer();
	TGPane *pane = new TGPane( paneWidth, 0.0f );
	if ( player != NULL && player->GetTorpedoSystem() != NULL ) {
		TorpedoSystem *torpedoes = player->GetTorpedoSystem();
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
		TGString *title = database->GetString( "Torpedoes" );
		label = new TGParagraph( title, 1.0f, &g_kTacWeaponsCtrlHeaderTextColor, "", 0.0f, 0x80000000 );
		label->SetFontGroup( headerFont, true );
		char count[64];
		sprintf( count, "%d", torpedoes->GetNumAvailableTorps( torpedoes->GetCurrentAmmoTypeNumber() ) );
		TGParagraph *countText = new TGParagraph( count, 1.0f, NULL, "", 0.0f, 0x80000000 );
		countText->SetFontGroup( headerFont, true );
		countText->SetColor( &g_kTacWeaponsCtrlHeaderTextColor );

		TGEvent *typeEvent = new TGEvent;
		typeEvent->SetEventType( ET_TAC_TORPEDO_TYPE_STATE );
		typeEvent->SetDestination( this );
		TGEvent *spreadEvent = new TGEvent;
		spreadEvent->SetEventType( ET_TAC_TORPEDO_SPREAD_STATE );
		spreadEvent->SetDestination( this );

		int ammoType = torpedoes->GetCurrentAmmoTypeNumber();
		TGString *typeValue = database->GetString( "Type:" );
		TGString *typeCaption = database->GetString( "Type:" );
		STToggle *typeToggle = new STToggle( typeCaption, ammoType, 0, typeValue, typeEvent, NULL, NULL, NULL, NULL, NULL, NULL );
		typeToggle->GetText()->SetFontGroup( headerFont, true );
		typeToggle->SetUseUIHeight( false );
		// The toggles take the button colours of the third menu depth.
		typeToggle->SetNormalColor( &g_kSTButtonColors[2][0] );
		typeToggle->SetSelectedColor( &g_kSTButtonColors[2][1] );
		typeToggle->SetHighlightedColor( &g_kSTButtonColors[2][2] );
		typeToggle->SetDisabledColor( &g_kSTButtonColors[2][3] );
		typeToggle->SetColorBasedOnFlags();
		typeToggle->SetUseEndCaps( false, false );
		typeToggle->Resize( pane->GetWidth() - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), label->GetHeight() + gap, false );

		TGString chainNames[4];
		TGLocalizationDatabase *subsystems = g_kLocalizationManager.Load( "data/TGL/Subsystems.tgl", true );
		int i;
		for ( i = 0; i < torpedoes->GetNumFiringChains() && i < 4; i++ ) {
			chainNames[i].AssignC( torpedoes->GetFiringChain( i )->GetName() );
			char *key = chainNames[i].Unknown6F56F0();
			if ( subsystems->HasString( key ) ) {
				chainNames[i] = *subsystems->GetString( key );
			}
			delete key;
		}
		g_kLocalizationManager.Unload( subsystems );

		int chainMode = torpedoes->GetFiringChainMode();
		TGString *spreadCaption = database->GetString( "Spread:" );
		STToggle *spread = new STToggle( spreadCaption, chainMode, 0, &chainNames[0], spreadEvent, &chainNames[1], spreadEvent, &chainNames[2], spreadEvent, &chainNames[3], spreadEvent );
		if ( torpedoes->GetNumFiringChains() < 2 ) {
			spread->SetNotVisible( true );
		}
		for ( i = torpedoes->GetNumFiringChains(); i < 4; i++ ) {
			TGString blank( "" );
			spread->SetStateValueW( i, &blank, NULL );
		}
		spread->GetText()->SetFontGroup( headerFont, true );
		spread->SetUseUIHeight( false );
		// The toggles take the button colours of the third menu depth.
		spread->SetNormalColor( &g_kSTButtonColors[2][0] );
		spread->SetSelectedColor( &g_kSTButtonColors[2][1] );
		spread->SetHighlightedColor( &g_kSTButtonColors[2][2] );
		spread->SetDisabledColor( &g_kSTButtonColors[2][3] );
		spread->SetColorBasedOnFlags();
		spread->SetUseEndCaps( false, false );
		spread->Resize( pane->GetWidth() - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), label->GetHeight() + gap, false );

		float y = 0.0f;
		pane->AddChild( label, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
		label->RecalcBounds( false );
		pane->AddChild( countText, 0.0f, 0.0f, false );
		y = label->GetHeight() + gap;
		pane->AddChild( typeToggle, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
		typeToggle->Layout();
		y = typeToggle->GetHeight() + y + gap;
		pane->AddChild( spread, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
		spread->Layout();
		if ( spread->IsVisible() ) {
			y = spread->GetHeight() + y;
		}
		g_kLocalizationManager.Unload( database );
		pane->Resize( pane->GetWidth(), y, false );
		NiPoint2 offset;
		TGPane::Cast( typeToggle->GetNthChild( 0 ) )->GetScreenOffset( &offset );
		label->SetPosition( offset.x + TAC_WEAPONS_CTRL_INDENT_HORIZ, label->GetTop(), false );
		float labelRight = label->GetLeft() + label->GetWidth();
		countText->SetPosition( pane->GetWidth() - ( pane->GetWidth() - labelRight ) * 0.5f - countText->GetWidth() * 0.5f, 0.0f, false );
		pane->SetFocus( typeToggle, true );
	}
	return pane;
}

/*
================
TacWeaponsCtrl::Unknown548A00

Builds the second pane: the "Phasers" heading over the intensity toggle (Low
or Full, by the phasers' power level), for a player ship with phasers. The
pane is empty for any other (name invented; the parameter, the icon group,
is not read).
================
*/
// FUNCTION: STBC 0x00548a00
// PARKED: frame and forward substitution: y packs into gap's slot here (c2trace --frame: slot 2 with gap) where retail gives it state's (slot 0), and the last y = height + y, single-use, is substituted into Resize's argument past the Unload call; retail keeps y in memory and computes the sum before Unload, as if y had a second use or interfered with gap; no placement or naming of y moves either
TGUIObject *TacWeaponsCtrl::Unknown548A00( char *lcars ) {
	TGParagraph *label = NULL;
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	float gap = 2 * mode->GetPixelHeight();
	TGFontGroup *defaultFont = g_kFontManager.GetDefaultFont();
	TGFontGroup *headerFont = g_kFontManager.GetFontGroup( defaultFont->GetFontName(), 5.0f );
	float paneWidth;
	float paneHeight;
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_WIDTH", "f", &paneWidth );
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_HEIGHT", "f", &paneHeight );
	ShipClass *player = Game::GetCurrentPlayer();
	TGPane *pane = new TGPane( paneWidth, 0.0f );
	if ( player != NULL && player->GetPhaserSystem() != NULL ) {
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
		TGString *title = database->GetString( "Phasers" );
		label = new TGParagraph( title, 1.0f, &g_kTacWeaponsCtrlHeaderTextColor, "", 0.0f, 0x80000000 );
		label->SetFontGroup( headerFont, true );

		TGEvent *event = new TGEvent;
		event->SetEventType( ET_TAC_PHASER_INTENSITY );
		event->SetDestination( this );
		int state = 1;
		if ( player->GetPhaserSystem()->GetPowerLevel() == 0 ) {
			state = 0;
		}
		TGString *fullName = database->GetString( "Full" );
		TGString *lowName = database->GetString( "Low" );
		TGString *caption = database->GetString( "Intensity:" );
		STToggle *toggle = new STToggle( caption, state, 0, lowName, event, fullName, event, NULL, NULL, NULL, NULL );
		toggle->GetText()->SetFontGroup( headerFont, true );
		toggle->SetUseUIHeight( false );
		// The toggles take the button colours of the third menu depth.
		toggle->SetNormalColor( &g_kSTButtonColors[2][0] );
		toggle->SetSelectedColor( &g_kSTButtonColors[2][1] );
		toggle->SetHighlightedColor( &g_kSTButtonColors[2][2] );
		toggle->SetDisabledColor( &g_kSTButtonColors[2][3] );
		toggle->SetColorBasedOnFlags();
		toggle->SetUseEndCaps( false, false );
		toggle->Resize( pane->GetWidth() - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), label->GetHeight() + gap, false );
		pane->AddChild( label, TAC_WEAPONS_CTRL_INDENT_HORIZ, 0.0f, false );
		float y = label->GetHeight() + gap;
		pane->AddChild( toggle, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
		toggle->Layout();
		y = toggle->GetHeight() + y;
		g_kLocalizationManager.Unload( database );
		pane->Resize( pane->GetWidth(), y, false );
		TGPane *button = TGPane::Cast( toggle->GetNthChild( 0 ) );
		NiPoint2 offset;
		button->GetScreenOffset( &offset );
		label->SetPosition( TAC_WEAPONS_CTRL_INDENT_HORIZ + offset.x, label->GetTop(), false );
		pane->SetFocus( toggle, true );
	}
	return pane;
}
/*
================
TacWeaponsCtrl::Unknown548E60

Builds the third pane: the tractor beam toggle (hidden for a ship without a
tractor beam) over the cloak toggle (only for a ship with a cloaking
device). The pane is empty without a player ship (name invented; the
parameter, the icon group, is not read).
================
*/
// FUNCTION: STBC 0x00548e60 ?Unknown548E60@TacWeaponsCtrl@@IAEPAVTGUIObject@@PAD@Z
// DRAFT: retail calls NiColorA::operator= (0x494c10) out of line for the normal, highlighted-text and selected colours and copies the other two inline (inline budget); this build copies all five inline
TGUIObject *TacWeaponsCtrl::Unknown548E60( char *lcars ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	float gap = 2 * mode->GetPixelHeight();
	TGFontGroup *defaultFont = g_kFontManager.GetDefaultFont();
	TGFontGroup *headerFont = g_kFontManager.GetFontGroup( defaultFont->GetFontName(), 5.0f );
	float paneWidth;
	float paneHeight;
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_WIDTH", "f", &paneWidth );
	PyEmbed::GetValue( mode->GetLcarsModule(), "WEAPONS_CTRL_PANE_HEIGHT", "f", &paneHeight );
	ShipClass *player = Game::GetCurrentPlayer();
	TGPane *pane = new TGPane( paneWidth, 0.0f );
	if ( player != NULL ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_TAC_TRACTOR_BEAM_STATE );
		event->SetDestination( this );
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
		TGString *offName = database->GetString( "Off" );
		TGString *rangeName = database->GetString( "Out of Range" );
		TGString *caption = database->GetString( "Tractor" );
		STToggle *tractor = new STToggle( caption, 1, 0, rangeName, event, offName, event, NULL, NULL, NULL, NULL );
		tractor->GetText()->SetFontGroup( headerFont, true );
		TGPane *textPane = TGPane::Cast( tractor->GetNthChild( 0 ) );
		TGParagraph *text = NULL;
		if ( textPane != NULL ) {
			text = TGParagraph::Cast( textPane->GetFirstChild() );
		}
		tractor->SetUseUIHeight( false );
		tractor->SetNormalColor( &g_kSTButtonColors[0][0] );
		tractor->SetHighlightedTextColor( &g_kSTButtonColors[0][3] );
		tractor->SetSelectedColor( &g_kMainMenuButtonSelectedColor );
		tractor->SetHighlightedColor( &g_kMainMenuButtonHighlightedColor );
		tractor->SetDisabledColor( &g_kSTButtonColors[1][3] );
		tractor->SetColorBasedOnFlags();
		tractor->SetUseEndCaps( false, false );
		tractor->Resize( pane->GetWidth() - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), text->GetHeight() + gap, false );
		if ( player->GetTractorBeamSystem() == NULL ) {
			tractor->SetNotVisible( true );
		}
		pane->AddChild( tractor, TAC_WEAPONS_CTRL_INDENT_HORIZ, 0.0f, false );
		tractor->Layout();
		float y = tractor->GetHeight() + gap;

		STToggle *cloak = NULL;
		if ( player->GetCloakingSubsystem() != NULL ) {
			TGEvent *cloakEvent = new TGEvent;
			cloakEvent->SetEventType( ET_TAC_CLOAK_STATE );
			cloakEvent->SetDestination( this );
			TGString *cloakOff = database->GetString( "Off" );
			TGString *cloakOn = database->GetString( "On" );
			TGString *cloakCaption = database->GetString( "Cloak" );
			cloak = new STToggle( cloakCaption, 1, 0, cloakOn, cloakEvent, cloakOff, cloakEvent, NULL, NULL, NULL, NULL );
			cloak->GetText()->SetFontGroup( headerFont, true );
			cloak->SetUseUIHeight( false );
			cloak->SetNormalColor( &g_kSTButtonColors[0][0] );
			cloak->SetHighlightedTextColor( &g_kSTButtonColors[0][3] );
			cloak->SetSelectedColor( &g_kMainMenuButtonSelectedColor );
			cloak->SetHighlightedColor( &g_kMainMenuButtonHighlightedColor );
			cloak->SetDisabledColor( &g_kSTButtonColors[1][3] );
			cloak->SetColorBasedOnFlags();
			cloak->SetUseEndCaps( false, false );
			cloak->Resize( pane->GetWidth() - ( TAC_WEAPONS_CTRL_INDENT_HORIZ + TAC_WEAPONS_CTRL_INDENT_HORIZ ), text->GetHeight() + gap, false );
			pane->AddChild( cloak, TAC_WEAPONS_CTRL_INDENT_HORIZ, y, false );
			y = cloak->GetHeight() + y;
		}
		g_kLocalizationManager.Unload( database );
		pane->Resize( pane->GetWidth(), y, false );
		if ( tractor->IsVisible() ) {
			pane->SetFocus( tractor, true );
			return pane;
		}
		if ( cloak != NULL ) {
			pane->SetFocus( cloak, true );
		}
	}
	return pane;
}

/*
================
TacWeaponsCtrl::GetPhaserIntensityToggle

The phaser intensity toggle: child 1 of the control's pane 1.
================
*/
// FUNCTION: STBC 0x00549430
STToggle *TacWeaponsCtrl::GetPhaserIntensityToggle( void ) {
	TGPane *pane = TGPane::Cast( GetNthChild( 1 ) );
	if ( pane == NULL ) {
		return NULL;
	}
	return STToggle::Cast( pane->GetNthChild( 1 ) );
}

/*
================
TacWeaponsCtrl::GetTorpTypeToggle

The torpedo type toggle: child 2 of the control's pane 0.
================
*/
// FUNCTION: STBC 0x00549460
STToggle *TacWeaponsCtrl::GetTorpTypeToggle( void ) {
	TGPane *pane = TGPane::Cast( GetNthChild( 0 ) );
	if ( pane == NULL ) {
		return NULL;
	}
	return STToggle::Cast( pane->GetNthChild( 2 ) );
}

/*
================
TacWeaponsCtrl::GetTorpSpreadToggle

The torpedo spread toggle: child 3 of the control's pane 0.
================
*/
// FUNCTION: STBC 0x00549490
STToggle *TacWeaponsCtrl::GetTorpSpreadToggle( void ) {
	TGPane *pane = TGPane::Cast( GetNthChild( 0 ) );
	if ( pane == NULL ) {
		return NULL;
	}
	return STToggle::Cast( pane->GetNthChild( 3 ) );
}

/*
================
TacWeaponsCtrl::GetBeamToggle

The tractor beam toggle: child 0 of the control's pane 3.
================
*/
// FUNCTION: STBC 0x005494c0
STToggle *TacWeaponsCtrl::GetBeamToggle( void ) {
	TGPane *pane = TGPane::Cast( GetNthChild( 3 ) );
	if ( pane == NULL ) {
		return NULL;
	}
	return STToggle::Cast( pane->GetNthChild( 0 ) );
}

/*
================
TacWeaponsCtrl::GetCloakToggle

The cloak toggle: child 1 of the control's pane 3.
================
*/
// FUNCTION: STBC 0x005494f0
STToggle *TacWeaponsCtrl::GetCloakToggle( void ) {
	TGPane *pane = TGPane::Cast( GetNthChild( 3 ) );
	if ( pane == NULL ) {
		return NULL;
	}
	return STToggle::Cast( pane->GetNthChild( 1 ) );
}

/*
================
TacWeaponsCtrl::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00549520
void TacWeaponsCtrl::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::PhaserSettingChange, "TacWeaponsCtrl::PhaserSettingChange" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::TorpedoTypeToggle, "TacWeaponsCtrl::TorpedoTypeToggle" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::TorpedoSpreadToggle, "TacWeaponsCtrl::TorpedoSpreadToggle" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::OtherBeamToggle, "TacWeaponsCtrl::OtherBeamToggle" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleCloakToggle, "TacWeaponsCtrl::HandleCloakToggle" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleCloakToggleRefresh, "TacWeaponsCtrl::HandleCloakToggleRefresh" );
	g_kEventManager.RegisterHandlerFunction( TacWeaponsCtrl::KeyboardHandler, "TacWeaponsCtrl::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleTargetChanged, "TacWeaponsCtrl::HandleTargetChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandlePlayerChanged, "TacWeaponsCtrl::HandlePlayerChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleTorpedoCountChanged, "TacWeaponsCtrl::HandleTorpedoCountChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleSubsystemDisabled, "TacWeaponsCtrl::HandleSubsystemDisabled" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleSubsystemOperational, "TacWeaponsCtrl::HandleSubsystemOperational" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleSelectOption, "TacWeaponsCtrl::HandleSelectOption" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TacWeaponsCtrl::HandleTractorBeamFiring, "TacWeaponsCtrl::HandleTractorBeamFiring" );
}

/*
================
TacWeaponsCtrl::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00549640
void TacWeaponsCtrl::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_TAC_PHASER_INTENSITY, "TacWeaponsCtrl::PhaserSettingChange" );
	AddMethodHandlerForClass( &classHandlers, ET_TAC_TORPEDO_TYPE_STATE, "TacWeaponsCtrl::TorpedoTypeToggle" );
	AddMethodHandlerForClass( &classHandlers, ET_TAC_TORPEDO_SPREAD_STATE, "TacWeaponsCtrl::TorpedoSpreadToggle" );
	AddMethodHandlerForClass( &classHandlers, ET_TAC_TRACTOR_BEAM_STATE, "TacWeaponsCtrl::OtherBeamToggle" );
	AddMethodHandlerForClass( &classHandlers, ET_TAC_CLOAK_STATE, "TacWeaponsCtrl::HandleCloakToggle" );
	AddMethodHandlerForClass( &classHandlers, ET_TAC_WEAPONS_REFRESH, "TacWeaponsCtrl::HandleCloakToggleRefresh" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "TacWeaponsCtrl::HandleSelectOption" );
}

/*
================
TacWeaponsCtrl::HandleTorpedoCountChanged

Writes the current type's torpedo count into the torpedo pane and centres
it between the type's label and the pane's right edge.
================
*/
// FUNCTION: STBC 0x005496e0
void TacWeaponsCtrl::HandleTorpedoCountChanged( TGEvent *event ) {
	TorpedoSystem *torpedoes = NULL;
	TGParagraph *count = NULL;
	TGParagraph *label = NULL;
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		torpedoes = player->GetTorpedoSystem();
	}
	TGPane *pane = TGPane::Cast( GetNthChild( 0 ) );
	if ( pane != NULL ) {
		count = TGParagraph::Cast( pane->GetNthChild( 1 ) );
		label = TGParagraph::Cast( pane->GetNthChild( 0 ) );
	}
	if ( player != NULL && torpedoes != NULL && pane != NULL && count != NULL && label != NULL ) {
		char text[64];
		sprintf( text, "%d", torpedoes->GetNumAvailableTorps( torpedoes->GetCurrentAmmoTypeNumber() ) );
		count->SetString( text, true );
		count->SetPosition( pane->GetWidth() - ( pane->GetWidth() - ( label->GetLeft() + label->GetWidth() ) ) * 0.5f - count->GetWidth() * 0.5f, 0.0f, true );
	}
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::PhaserSettingChange

The phaser intensity toggle sets the phasers' power level.
================
*/
// FUNCTION: STBC 0x005497e0
void TacWeaponsCtrl::PhaserSettingChange( TGEvent *event ) {
	ShipClass *player = ShipClass::Cast( Game::GetCurrentGame()->GetPlayer() );
	PhaserSystem *phasers = player->GetPhaserSystem();
	int state = GetPhaserIntensityToggle()->GetState();
	if ( state == 0 ) {
		phasers->SetPowerLevel( 0 );
	} else if ( state == 1 ) {
		phasers->SetPowerLevel( 2 );
	}
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::TorpedoTypeToggle
================
*/
// FUNCTION: STBC 0x00549830
void TacWeaponsCtrl::TorpedoTypeToggle( TGEvent *event ) {
	SelectTorpedoType();
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::SelectTorpedoType

From the type the toggle shows on, the first type with torpedoes left
becomes the loaded one; with none, the toggle goes back to the type before.
================
*/
// FUNCTION: STBC 0x00549850
void TacWeaponsCtrl::SelectTorpedoType( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	TorpedoSystem *torpedoes = player->GetTorpedoSystem();
	if ( torpedoes == NULL ) {
		return;
	}
	STToggle *toggle = GetTorpTypeToggle();
	int numTypes = torpedoes->GetProperty()->GetNumAmmoTypes();
	if ( numTypes <= 0 ) {
		return;
	}
	int previous = toggle->GetState();
	int type = previous--;
	if ( previous < 0 ) {
		previous = numTypes - 1;
	}
	if ( type == previous ) {
		toggle->SetState( previous, true );
		return;
	}
	while ( torpedoes->GetNumAvailableTorpsToType( type ) <= 0 ) {
		type = ( type + 1 ) % numTypes;
		if ( type == previous ) {
			toggle->SetState( previous, true );
			return;
		}
	}
	torpedoes->SetAmmoType( type, true );
	toggle->SetState( type, true );
}

/*
================
TacWeaponsCtrl::TorpedoSpreadToggle
================
*/
// FUNCTION: STBC 0x00549920
void TacWeaponsCtrl::TorpedoSpreadToggle( TGEvent *event ) {
	SetTorpedoSpread();
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::SetTorpedoSpread
================
*/
// FUNCTION: STBC 0x00549940
void TacWeaponsCtrl::SetTorpedoSpread( void ) {
	ShipClass *player = ShipClass::Cast( Game::GetCurrentGame()->GetPlayer() );
	TorpedoSystem *torpedoes = player->GetTorpedoSystem();
	STToggle *toggle = GetTorpSpreadToggle();
	torpedoes->SetFiringChainMode( toggle->GetState() );
}

/*
================
TacWeaponsCtrl::OtherBeamToggle
================
*/
// FUNCTION: STBC 0x00549980
void TacWeaponsCtrl::OtherBeamToggle( TGEvent *event ) {
	SetTractorBeam();
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::SetTractorBeam

The tractor toggle's first state fires the beam at the target, the other
stops it; without a target the toggle goes back to its second state.
================
*/
// FUNCTION: STBC 0x005499a0
// PARKED: register pick: retail keeps this and the toggle in EDI and the tractor system in EBX; here the tractor system (priority 27) is coloured before the toggle (25) and takes EDI; the control-flow spellings tried are byte-identical
void TacWeaponsCtrl::SetTractorBeam( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game == NULL ) {
		return;
	}
	ShipClass *player = ShipClass::Cast( game->GetPlayer() );
	if ( player == NULL ) {
		return;
	}
	TractorBeamSystem *tractors = player->GetTractorBeamSystem();
	STToggle *toggle = GetBeamToggle();
	if ( tractors != NULL && toggle != NULL ) {
		if ( toggle->GetState() == 0 ) {
			if ( player->GetTarget() != NULL ) {
				tractors->VirtualSlot34( player->GetTarget(), (NiPoint3 *)&player->GetTargetOffset() );
				return;
			}
		} else {
			tractors->StopFiring();
			return;
		}
	}
	if ( toggle != NULL ) {
		toggle->SetState( 1, true );
	}
}

/*
================
TacWeaponsCtrl::HandleCloakToggle
================
*/
// FUNCTION: STBC 0x00549a30
void TacWeaponsCtrl::HandleCloakToggle( TGEvent *event ) {
	SetCloak();
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::SetCloak

The cloak toggle's first state cloaks, the other decloaks.
================
*/
// FUNCTION: STBC 0x00549a50
void TacWeaponsCtrl::SetCloak( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game == NULL ) {
		return;
	}
	ShipClass *player = ShipClass::Cast( game->GetPlayer() );
	if ( player == NULL ) {
		return;
	}
	CloakingSubsystem *cloak = player->GetCloakingSubsystem();
	STToggle *toggle = GetCloakToggle();
	if ( cloak != NULL && toggle != NULL ) {
		if ( toggle->GetState() == 0 ) {
			cloak->StartCloaking();
		} else {
			cloak->StopCloaking();
		}
	}
}

/*
================
TacWeaponsCtrl::KeyboardHandler

Up and down, released, move the focus between the control's buttons and on
into the panes before and after.
================
*/
// FUNCTION: STBC 0x00549ac0
void TacWeaponsCtrl::KeyboardHandler( TGEventHandlerObject *object, TGEvent *event ) {
	TGKeyboardEvent *keyEvent = (TGKeyboardEvent *)event;
	unsigned short key = keyEvent->GetUnicode();
	TGPane *pane = TGPane::Cast( object );
	if ( pane == NULL ) {
		object->CallNextHandler( event );
		return;
	}
	// BUG: redundant: the focus is looked up and never used
	pane->GetFocus();
	TGPane *parent = pane->GetParent();
	if ( keyEvent->GetKeyState() == KS_KEYUP ) {
		if ( key == WC_UP ) {
			keyEvent->SetHandled();
			TGUIObject *previous = InterfaceModule::GetPrevUIElement( pane, TGUIO_ENABLED | TGUIO_VISIBLE );
			if ( previous != NULL ) {
				pane->SetFocus( previous, true );
			} else {
				for ( TGUIObject *sibling = parent->GetPrevChild( pane ); sibling != NULL; sibling = parent->GetPrevChild( sibling ) ) {
					TGPane *siblingPane = TGPane::Cast( sibling );
					if ( siblingPane != NULL && InterfaceModule::HasUIElement( siblingPane, TGUIO_ENABLED | TGUIO_VISIBLE ) == true ) {
						parent->SetFocus( siblingPane, true );
						siblingPane->SetFocus( InterfaceModule::GetLastUIElement( siblingPane, TGUIO_ENABLED | TGUIO_VISIBLE ), true );
						break;
					}
				}
			}
		}
		if ( key == WC_DOWN ) {
			keyEvent->SetHandled();
			TGUIObject *next = InterfaceModule::GetNextUIElement( pane, TGUIO_ENABLED | TGUIO_VISIBLE );
			if ( next != NULL ) {
				pane->SetFocus( next, true );
			} else {
				for ( TGUIObject *sibling = parent->GetNextChild( pane ); sibling != NULL; sibling = parent->GetNextChild( sibling ) ) {
					TGPane *siblingPane = TGPane::Cast( sibling );
					if ( siblingPane != NULL && InterfaceModule::HasUIElement( siblingPane, TGUIO_ENABLED | TGUIO_VISIBLE ) == true ) {
						parent->SetFocus( siblingPane, true );
						siblingPane->SetFocus( InterfaceModule::GetFirstUIElement( siblingPane, TGUIO_ENABLED | TGUIO_VISIBLE ), true );
						return;
					}
				}
			}
		}
	}
	if ( !keyEvent->EventHandled() ) {
		pane->CallNextHandler( event );
	}
}

/*
================
TacWeaponsCtrl::HandleSelectOption

The control takes no select events.
================
*/
// FUNCTION: STBC 0x00549c50
void TacWeaponsCtrl::HandleSelectOption( TGEvent *event ) {
}

/*
================
TacWeaponsCtrl::HandleCloakToggleRefresh

Refreshes the cloak toggle and sets the timer again.
================
*/
// FUNCTION: STBC 0x00549c60
void TacWeaponsCtrl::HandleCloakToggleRefresh( TGEvent *event ) {
	RefreshCloakToggle();
	TGEvent *refresh = new TGEvent;
	refresh->SetEventType( ET_TAC_WEAPONS_REFRESH );
	refresh->SetDestination( this );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + REFRESH_DELAY );
	timer->SetEvent( refresh );
	timerID = timer->GetObjID();
	g_kTimerManager.AddTimer( timer );
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::RefreshCloakToggle

Shows the player ship's cloak intent on the cloak toggle: state 0 while it is
trying to cloak, otherwise state 1.
================
*/
// FUNCTION: STBC 0x00549d20
void TacWeaponsCtrl::RefreshCloakToggle( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		CloakingSubsystem *cloak = player->GetCloakingSubsystem();
		STToggle *toggle = GetCloakToggle();
		if ( cloak != NULL && toggle != NULL ) {
			if ( cloak->IsTryingToCloak() == true ) {
				toggle->SetState( 0, true );
				return;
			}
			toggle->SetState( 1, true );
		}
	}
}

/*
================
TacWeaponsCtrl::RefreshTractorToggle

Shows the tractor beam on the beam toggle: "Firing" while it is firing at
a target in its list, "Out of Range" while it is trying to fire, else the
idle state. The toggle is enabled only for a live target.
================
*/
// FUNCTION: STBC 0x00549d70
void TacWeaponsCtrl::RefreshTractorToggle( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		ShipClass *player = ShipClass::Cast( game->GetPlayer() );
		if ( player != NULL ) {
			TractorBeamSystem *tractor = player->GetTractorBeamSystem();
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
			if ( tractor != NULL ) {
				STToggle *toggle = GetBeamToggle();
				if ( toggle != NULL ) {
					ObjectClass *target = player->GetTarget();
					TGEvent *event = new TGEvent;
					event->SetEventType( ET_TAC_TRACTOR_BEAM_STATE );
					event->SetDestination( this );
					if ( tractor->IsFiring() == true && target != NULL && tractor->IsInTargetList( target ) ) {
						toggle->SetState( 0, true );
						toggle->SetStateValueW( 0, database->GetString( "Firing" ), event );
					} else if ( tractor->IsTryingToFire() == true ) {
						toggle->SetState( 0, true );
						toggle->SetStateValueW( 0, database->GetString( "Out of Range" ), event );
					} else {
						toggle->SetState( 1, true );
						if ( event != NULL ) {
							delete event;
						}
					}
					if ( target != NULL && tractor->IsDisabled() != true ) {
						toggle->SetEnabled( true );
					} else {
						toggle->SetDisabled( true );
					}
				}
			}
			g_kLocalizationManager.Unload( database );
		}
	}
}

/*
================
TacWeaponsCtrl::RefreshPhaserSettings

Shows the player ship's phaser power level on the phaser intensity toggle:
state 0 at level 0, state 1 at levels 1 and 2.
================
*/
// FUNCTION: STBC 0x00549ed0
void TacWeaponsCtrl::RefreshPhaserSettings( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		ShipClass *player = ShipClass::Cast( game->GetPlayer() );
		if ( player != NULL ) {
			PhaserSystem *phasers = player->GetPhaserSystem();
			if ( phasers != NULL ) {
				STToggle *toggle = GetPhaserIntensityToggle();
				if ( toggle != NULL ) {
					switch ( phasers->GetPowerLevel() ) {
					case 0:
						toggle->SetState( 0, true );
						break;
					case 1:
					case 2:
						toggle->SetState( 1, true );
						break;
					}
				}
			}
		}
	}
}

/*
================
TacWeaponsCtrl::RefreshTorpedoSettings

Names the torpedo type toggle's states after the ship's ammo types, from the
subsystem names file when it has them, and shows the current type and the
firing chain mode.
================
*/
// FUNCTION: STBC 0x00549f30
void TacWeaponsCtrl::RefreshTorpedoSettings( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		ShipClass *player = ShipClass::Cast( game->GetPlayer() );
		if ( player != NULL ) {
			TorpedoSystem *torpedoes = player->GetTorpedoSystem();
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Subsystems.tgl", true );
			if ( torpedoes != NULL ) {
				STToggle *typeToggle = GetTorpTypeToggle();
				if ( typeToggle != NULL ) {
					for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
						TorpedoAmmoType *ammoType = torpedoes->GetAmmoType( i );
						if ( ammoType != NULL ) {
							char *name = ammoType->GetAmmoName();
							if ( name != NULL ) {
								TGEvent *event = new TGEvent;
								event->SetEventType( ET_TAC_TORPEDO_TYPE_STATE );
								event->SetDestination( this );
								if ( database->HasString( name ) == true ) {
									typeToggle->SetStateValueW( i, database->GetString( name ), event );
								} else {
									typeToggle->SetStateValue( i, name, event );
								}
							} else {
								typeToggle->SetStateValue( i, NULL, NULL );
							}
						} else {
							typeToggle->SetStateValue( i, NULL, NULL );
						}
					}
					typeToggle->SetState( torpedoes->GetCurrentAmmoTypeNumber(), true );
					STToggle *spreadToggle = GetTorpSpreadToggle();
					spreadToggle->SetState( torpedoes->GetFiringChainMode(), true );
				}
			}
			g_kLocalizationManager.Unload( database );
		}
	}
}

/*
================
TacWeaponsCtrl::HandlePlayerChanged

Builds the control again for the new player's ship.
================
*/
// FUNCTION: STBC 0x0054a120
void TacWeaponsCtrl::HandlePlayerChanged( TGEvent *event ) {
	KillChildren();
	Init();
	RefreshPhaserSettings();
	RefreshTorpedoSettings();
	RefreshCloakToggle();
	RefreshTractorToggle();
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::HandleTargetChanged

The player's new target refreshes the tractor toggle.
================
*/
// FUNCTION: STBC 0x0054a160
void TacWeaponsCtrl::HandleTargetChanged( TGEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetDestination() );
	if ( ship != NULL && Game::GetCurrentGame() != NULL ) {
		if ( Game::GetCurrentGame()->GetPlayer() == ship ) {
			RefreshTractorToggle();
		}
		CallNextHandler( event );
	}
}

/*
================
TacWeaponsCtrl::HandleSubsystemDisabled

Disables the toggles of a disabled weapon system or cloak.
================
*/
// FUNCTION: STBC 0x0054a1a0
void TacWeaponsCtrl::HandleSubsystemDisabled( TGEvent *event ) {
	WeaponSystem *weapons = WeaponSystem::Cast( event->GetSource() );
	CloakingSubsystem *cloak = CloakingSubsystem::Cast( event->GetSource() );
	if ( weapons != NULL ) {
		PhaserSystem *phasers = PhaserSystem::Cast( weapons );
		TorpedoSystem *torpedoes = TorpedoSystem::Cast( weapons );
		TractorBeamSystem *tractors = TractorBeamSystem::Cast( weapons );
		if ( phasers != NULL ) {
			STToggle *toggle = GetPhaserIntensityToggle();
			if ( toggle != NULL ) {
				toggle->SetDisabled( true );
			}
		}
		if ( torpedoes != NULL ) {
			STToggle *toggle = GetTorpTypeToggle();
			if ( toggle != NULL ) {
				toggle->SetDisabled( true );
			}
			toggle = GetTorpSpreadToggle();
			if ( toggle != NULL ) {
				toggle->SetDisabled( true );
			}
		}
		if ( tractors != NULL ) {
			RefreshTractorToggle();
		}
	} else if ( cloak != NULL ) {
		STToggle *toggle = GetCloakToggle();
		if ( toggle != NULL ) {
			toggle->SetDisabled( true );
		}
	}
}

/*
================
TacWeaponsCtrl::HandleSubsystemOperational

Enables the toggles of a weapon system or cloak that works again.
================
*/
// FUNCTION: STBC 0x0054a260
void TacWeaponsCtrl::HandleSubsystemOperational( TGEvent *event ) {
	WeaponSystem *weapons = WeaponSystem::Cast( event->GetSource() );
	CloakingSubsystem *cloak = CloakingSubsystem::Cast( event->GetSource() );
	if ( weapons != NULL ) {
		PhaserSystem *phasers = PhaserSystem::Cast( weapons );
		TorpedoSystem *torpedoes = TorpedoSystem::Cast( weapons );
		TractorBeamSystem *tractors = TractorBeamSystem::Cast( weapons );
		if ( phasers != NULL ) {
			STToggle *toggle = GetPhaserIntensityToggle();
			if ( toggle != NULL ) {
				toggle->SetEnabled( true );
			}
		}
		if ( torpedoes != NULL ) {
			STToggle *toggle = GetTorpTypeToggle();
			if ( toggle != NULL ) {
				toggle->SetEnabled( true );
			}
			toggle = GetTorpSpreadToggle();
			if ( toggle != NULL ) {
				toggle->SetEnabled( true );
			}
		}
		if ( tractors != NULL ) {
			RefreshTractorToggle();
		}
	} else if ( cloak != NULL ) {
		STToggle *toggle = GetCloakToggle();
		if ( toggle != NULL ) {
			toggle->SetEnabled( true );
		}
	}
}

/*
================
TacWeaponsCtrl::HandleTractorBeamFiring

The player's tractor beam starting or stopping refreshes the toggle.
================
*/
// FUNCTION: STBC 0x0054a320
void TacWeaponsCtrl::HandleTractorBeamFiring( TGEvent *event ) {
	// BUG: redundant: the toggle is looked up and never used
	GetBeamToggle();
	ShipClass *player = Game::GetCurrentPlayer();
	if ( event->GetDestination() == player ) {
		RefreshTractorToggle();
	}
	CallNextHandler( event );
}

/*
================
TacWeaponsCtrl::SetHighlighted
================
*/
// FUNCTION: STBC 0x0054a350
void TacWeaponsCtrl::SetHighlighted( bool layout ) {
	TGUIObject::SetHighlighted( false );
	SetColorBasedOnFlags();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TacWeaponsCtrl::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x0054a380
void TacWeaponsCtrl::SetNotHighlighted( bool layout ) {
	TGUIObject::SetNotHighlighted( false );
	SetColorBasedOnFlags();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TacWeaponsCtrl::SetColorBasedOnFlags

Empty in retail.
================
*/
// FUNCTION: STBC 0x0054a3b0
void TacWeaponsCtrl::SetColorBasedOnFlags( void ) {
}

/*
================
TacWeaponsCtrl::WriteToStream

The window, then the ID of the refresh timer.
================
*/
// FUNCTION: STBC 0x0054a3c0 ?WriteToStream@TacWeaponsCtrl@@UAE_NPAVTGStream@@@Z
bool TacWeaponsCtrl::WriteToStream( TGStream *stream ) {
	if ( !STStylizedWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( timerID );
	return true;
}

/*
================
TacWeaponsCtrl::ReadFromStream

The window, then the ID of the refresh timer; the control read is the one
there is.
================
*/
// FUNCTION: STBC 0x0054a410 ?ReadFromStream@TacWeaponsCtrl@@UAE_NPAVTGStream@@@Z
bool TacWeaponsCtrl::ReadFromStream( TGStream *stream ) {
	if ( !STStylizedWindow::ReadFromStream( stream ) ) {
		return false;
	}
	timerID = stream->ReadID();
	instance = this;
	return true;
}

