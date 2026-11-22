/*
===============================================================================

	EngRepairPane.cpp

===============================================================================
*/

#include <iostream>
#include "EngRepairPane.h"
#include "STStylizedWindow.h"
#include "STMenu.h"
#include "STSubPane.h"
#include "STButton.h"
#include "STColors.h"
#include "ShipSubsystem.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGIntEvent.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "ShipClass.h"
#include "RepairSubsystem.h"
#include "Game.h"
#include "GraphicsModeInfo.h"
#include "TGIcon.h"
#include "TGParagraph.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGStream.h"
#include "NiColor.h"
#include "TGObjPtrEvent.h"
#include "STRepairButton.h"
#include "STFillGauge.h"
#include "RepairSubsystemProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005528b0 _$E503
// FUNCTION: STBC 0x005528d0 _$E502

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00550610 _$E32
// FUNCTION: STBC 0x00550620 _$E29
// FUNCTION: STBC 0x00550630 _$E31
// FUNCTION: STBC 0x00550640 _$E30
// FUNCTION: STBC 0x00550650 _$E38
// FUNCTION: STBC 0x00550660 _$E35
// FUNCTION: STBC 0x00550670 _$E37
// FUNCTION: STBC 0x00550680 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00550690 _$E57
// FUNCTION: STBC 0x005506a0 _$E56
// FUNCTION: STBC 0x005506b0 _$E61
// FUNCTION: STBC 0x005506c0 _$E60
// FUNCTION: STBC 0x005506d0 _$E72
// FUNCTION: STBC 0x005506e0 _$E71
// FUNCTION: STBC 0x00550db0 _$E76
// FUNCTION: STBC 0x00550dc0 _$E75
// FUNCTION: STBC 0x00550e40 _$E80
// FUNCTION: STBC 0x00550e50 _$E79
// FUNCTION: STBC 0x00551210 _$E84
// FUNCTION: STBC 0x00551220 _$E83
// FUNCTION: STBC 0x00551650 _$E208
// FUNCTION: STBC 0x00551660 _$E207
// FUNCTION: STBC 0x005520d0 _$E150
// FUNCTION: STBC 0x005520e0 _$E149
// FUNCTION: STBC 0x005520f0 _$E163
// FUNCTION: STBC 0x00552100 _$E162
// FUNCTION: STBC 0x00552110 _$E213
// FUNCTION: STBC 0x00552120 _$E212
// FUNCTION: STBC 0x00552130 _$E218
// FUNCTION: STBC 0x00552140 _$E217
// FUNCTION: STBC 0x00552150 _$E222
// FUNCTION: STBC 0x00552160 _$E221
// FUNCTION: STBC 0x00552170 _$E226
// FUNCTION: STBC 0x00552180 _$E225
// FUNCTION: STBC 0x00552190 _$E233
// FUNCTION: STBC 0x005521a0 _$E232
// FUNCTION: STBC 0x005521b0 _$E339
// FUNCTION: STBC 0x005521c0 _$E338
// FUNCTION: STBC 0x005521d0 _$E343
// FUNCTION: STBC 0x005521e0 _$E342
// FUNCTION: STBC 0x005521f0 _$E108
// FUNCTION: STBC 0x00552200 _$E107
// FUNCTION: STBC 0x00552210 _$E281
// FUNCTION: STBC 0x00552220 _$E280
// FUNCTION: STBC 0x00552230 _$E288
// FUNCTION: STBC 0x00552240 _$E287
// FUNCTION: STBC 0x00552250 _$E292
// FUNCTION: STBC 0x00552260 _$E291
// FUNCTION: STBC 0x00552270 _$E296
// FUNCTION: STBC 0x00552280 _$E295
// FUNCTION: STBC 0x00552290 _$E300
// FUNCTION: STBC 0x005522a0 _$E299
// FUNCTION: STBC 0x005522b0 _$E306
// FUNCTION: STBC 0x005522c0 _$E305
// FUNCTION: STBC 0x005522d0 _$E347
// FUNCTION: STBC 0x005522e0 _$E346
// FUNCTION: STBC 0x005522f0 _$E351
// FUNCTION: STBC 0x00552300 _$E350
// FUNCTION: STBC 0x00552310 _$E355
// FUNCTION: STBC 0x00552320 _$E354
// FUNCTION: STBC 0x00552330 _$E359
// FUNCTION: STBC 0x00552340 _$E358
// FUNCTION: STBC 0x00552350 _$E363
// FUNCTION: STBC 0x00552360 _$E362
// FUNCTION: STBC 0x00552370 _$E367
// FUNCTION: STBC 0x00552380 _$E366
// FUNCTION: STBC 0x00552390 _$E371
// FUNCTION: STBC 0x005523a0 _$E370
// FUNCTION: STBC 0x005523b0 _$E375
// FUNCTION: STBC 0x005523c0 _$E374
// FUNCTION: STBC 0x005523d0 _$E379
// FUNCTION: STBC 0x005523e0 _$E378
// FUNCTION: STBC 0x005523f0 _$E383
// FUNCTION: STBC 0x00552400 _$E382
// FUNCTION: STBC 0x00552410 _$E113
// FUNCTION: STBC 0x00552420 _$E112
// FUNCTION: STBC 0x00552430 _$E387
// FUNCTION: STBC 0x00552440 _$E386
// FUNCTION: STBC 0x00552450 _$E118
// FUNCTION: STBC 0x00552460 _$E117
// FUNCTION: STBC 0x00552470 _$E391
// FUNCTION: STBC 0x00552480 _$E390
// FUNCTION: STBC 0x00552490 _$E395
// FUNCTION: STBC 0x005524a0 _$E394
// FUNCTION: STBC 0x005524b0 _$E399
// FUNCTION: STBC 0x005524c0 _$E398
// FUNCTION: STBC 0x005524d0 _$E403
// FUNCTION: STBC 0x005524e0 _$E402
// FUNCTION: STBC 0x005524f0 _$E127
// FUNCTION: STBC 0x00552500 _$E126
// FUNCTION: STBC 0x00552510 _$E407
// FUNCTION: STBC 0x00552520 _$E406
// FUNCTION: STBC 0x00552530 _$E411
// FUNCTION: STBC 0x00552540 _$E410
// FUNCTION: STBC 0x00552550 _$E415
// FUNCTION: STBC 0x00552560 _$E414
// FUNCTION: STBC 0x00552570 _$E133
// FUNCTION: STBC 0x00552580 _$E132
// FUNCTION: STBC 0x00552590 _$E138
// FUNCTION: STBC 0x005525a0 _$E137
// FUNCTION: STBC 0x005525b0 _$E168
// FUNCTION: STBC 0x005525c0 _$E167
// FUNCTION: STBC 0x005525d0 _$E419
// FUNCTION: STBC 0x005525e0 _$E418
// FUNCTION: STBC 0x005525f0 _$E173
// FUNCTION: STBC 0x00552600 _$E172
// FUNCTION: STBC 0x00552610 _$E423
// FUNCTION: STBC 0x00552620 _$E422
// FUNCTION: STBC 0x00552630 _$E427
// FUNCTION: STBC 0x00552640 _$E426
// FUNCTION: STBC 0x00552650 _$E178
// FUNCTION: STBC 0x00552660 _$E177
// FUNCTION: STBC 0x00552670 _$E431
// FUNCTION: STBC 0x00552680 _$E430
// FUNCTION: STBC 0x00552690 _$E143
// FUNCTION: STBC 0x005526a0 _$E142
// FUNCTION: STBC 0x005526b0 _$E435
// FUNCTION: STBC 0x005526c0 _$E434
// FUNCTION: STBC 0x005526d0 _$E439
// FUNCTION: STBC 0x005526e0 _$E438
// FUNCTION: STBC 0x005526f0 _$E183
// FUNCTION: STBC 0x00552700 _$E182
// FUNCTION: STBC 0x00552710 _$E188
// FUNCTION: STBC 0x00552720 _$E187
// FUNCTION: STBC 0x00552730 _$E443
// FUNCTION: STBC 0x00552740 _$E442
// FUNCTION: STBC 0x00552750 _$E447
// FUNCTION: STBC 0x00552760 _$E446
// FUNCTION: STBC 0x00552770 _$E451
// FUNCTION: STBC 0x00552780 _$E450
// FUNCTION: STBC 0x00552790 _$E455
// FUNCTION: STBC 0x005527a0 _$E454
// FUNCTION: STBC 0x005527b0 _$E459
// FUNCTION: STBC 0x005527c0 _$E458
// FUNCTION: STBC 0x005527d0 _$E463
// FUNCTION: STBC 0x005527e0 _$E462
// FUNCTION: STBC 0x005527f0 _$E467
// FUNCTION: STBC 0x00552800 _$E466
// FUNCTION: STBC 0x00552810 _$E471
// FUNCTION: STBC 0x00552820 _$E470
// FUNCTION: STBC 0x00552830 _$E193
// FUNCTION: STBC 0x00552840 _$E192
// FUNCTION: STBC 0x00552850 _$E475
// FUNCTION: STBC 0x00552860 _$E474
// FUNCTION: STBC 0x00552870 _$E198
// FUNCTION: STBC 0x00552880 _$E197
// FUNCTION: STBC 0x00552890 _$E203
// FUNCTION: STBC 0x005528a0 _$E202
// FUNCTION: STBC 0x005528e0 _$E479
// FUNCTION: STBC 0x005528f0 _$E478
// FUNCTION: STBC 0x00552900 _$E484
// FUNCTION: STBC 0x00552910 _$E483
// FUNCTION: STBC 0x00552920 _$E488
// FUNCTION: STBC 0x00552930 _$E487

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005506f0 _$E494
// FUNCTION: STBC 0x00550700 _$E490
// FUNCTION: STBC 0x00550710 _$E493
// FUNCTION: STBC 0x00550720 _$E491

// DATA: STBC 0x0098b0d8
TGEventHandlerTable EngRepairPane::classHandlers;

// DATA: STBC 0x0098b188
EngRepairPane *EngRepairPane::repairPane;

/*
================
EngRepairPane::Cast

The object as an EngRepairPane; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00550740
EngRepairPane *EngRepairPane::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ENGREPAIRPANE_TYPE ) ) {
		return (EngRepairPane *)object;
	}
	return NULL;
}

/*
================
EngRepairPane::EngRepairPane

Builds the three lists under their labels, with a divider after the
first two, and listens for the change of player ship. The repair team
assignments are tall enough for numRepairTeams buttons and one more.
================
*/
// FUNCTION: STBC 0x00550770 ??0EngRepairPane@@QAE@MMH@Z
// PARKED: x87 schedule (region 15): the fadd [repairLabel+0x24] waits 3 cycles on fadd st(0), st(0) (ready c36) and the pushes take the gap; retail issues it one cycle after push 0, an edge of latency <= 2, and loads the later sums' DEFAULT_ST_INDENT_VERT after the pushes; no spelling of the products moves the x87 chain
EngRepairPane::EngRepairPane( float width, float height, int numRepairTeams ) : TGPane( width, height ) {
	repairPane = this;
	this->numRepairTeams = numRepairTeams;
	repairSubsystemID = 0;

	STSubPane *repairArea = new STSubPane( DEFAULT_ST_INDENT_VERT, true );
	repairArea->Resize( 0.0f, 0.0f, false );
	STSubPane *waitingArea = new STSubPane( DEFAULT_ST_INDENT_VERT, true );
	waitingArea->Resize( 0.0f, 0.0f, false );
	STSubPane *destroyedArea = new STSubPane( DEFAULT_ST_INDENT_VERT, true );
	destroyedArea->Resize( 0.0f, 0.0f, false );
	destroyedArea->SetNoFocus();

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	TGParagraph *repairLabel = new TGParagraph( database->GetString( "Repair team assignments:" ), 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	TGParagraph *waitingLabel = new TGParagraph( database->GetString( "Damaged systems:" ), 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	TGParagraph *destroyedLabel = new TGParagraph( database->GetString( "Destroyed systems:" ), 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	g_kLocalizationManager.Unload( database );

	repairLabel->SetColor( (NiColorA *)&NiColorA::WHITE );
	waitingLabel->SetColor( (NiColorA *)&NiColorA::WHITE );
	destroyedLabel->SetColor( (NiColorA *)&NiColorA::WHITE );
	repairLabel->SetNoFocus();
	waitingLabel->SetNoFocus();
	destroyedLabel->SetNoFocus();
	repairLabel->Layout();
	waitingLabel->Layout();
	destroyedLabel->Layout();

	repairArea->Resize( width, ( DEFAULT_ST_INDENT_VERT * 2.0f + repairLabel->GetHeight() ) * ( numRepairTeams + 1 ), false );

	TGIcon *divider = new TGIcon( GraphicsModeInfo::GetCurrentMode()->GetLcarsString(), 200, (NiColorA *)&NiColorA::WHITE );
	TGIcon *divider2 = new TGIcon( GraphicsModeInfo::GetCurrentMode()->GetLcarsString(), 200, (NiColorA *)&NiColorA::WHITE );
	divider->Resize( width, GraphicsModeInfo::GetCurrentMode()->GetPixelHeight() * 2.0f, false );
	divider2->Resize( width, GraphicsModeInfo::GetCurrentMode()->GetPixelHeight() * 2.0f, false );

	waitingArea->Resize( width, height - repairArea->GetHeight() - repairLabel->GetHeight() - divider->GetHeight() - waitingLabel->GetHeight() - DEFAULT_ST_INDENT_VERT * 8.0f, false );

	AddChild( repairLabel, 0.0f, 0.0f, false );
	AddChild( repairArea, 0.0f, repairLabel->GetBottom() + DEFAULT_ST_INDENT_VERT * 2.0f, false );
	AddChild( waitingLabel, 0.0f, repairArea->GetBottom() + DEFAULT_ST_INDENT_VERT * 4.0f, false );
	AddChild( waitingArea, 0.0f, waitingLabel->GetBottom() + DEFAULT_ST_INDENT_VERT * 2.0f, false );
	AddChild( destroyedLabel, 0.0f, 0.0f, false );
	AddChild( destroyedArea, 0.0f, 0.0f, false );
	AddChild( divider, 0.0f, repairArea->GetBottom() + DEFAULT_ST_INDENT_VERT * 2.0f, false );
	AddChild( divider2, 0.0f, waitingArea->GetBottom() + DEFAULT_ST_INDENT_VERT * 2.0f, false );
	TGPane::SetFocus( repairArea, false );

	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "EngRepairPane::HandleSetPlayer", ANY_TARGET );
}

/*
================
EngRepairPane::IsTypeOf
================
*/
// FUNCTION: STBC 0x00550d10
bool EngRepairPane::IsTypeOf( int type ) {
	if ( type == ENGREPAIRPANE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
EngRepairPane::EngRepairPane
================
*/
// FUNCTION: STBC 0x00550e00 ??0EngRepairPane@@QAE@J@Z
EngRepairPane::EngRepairPane( long id ) : TGPane( id ) {
	repairPane = this;
}

/*
================
EngRepairPane::~EngRepairPane
================
*/
// FUNCTION: STBC 0x00550e20
EngRepairPane::~EngRepairPane( void ) {
	if ( this == repairPane ) {
		repairPane = NULL;
	}
}

/*
================
EngRepairPane::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00550e60
void EngRepairPane::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngRepairPane::HandleKeyboard, "EngRepairPane::HandleKeyboard" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngRepairPane::HandleSelectOption, "EngRepairPane::HandleSelectOption" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&EngRepairPane::HandleSetPlayer, "EngRepairPane::HandleSetPlayer" );
}

/*
================
EngRepairPane::RegisterHandlers

HandleSetPlayer is added for each instance, not to the class's table.
================
*/
// FUNCTION: STBC 0x00550ea0
void EngRepairPane::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "EngRepairPane::HandleKeyboard" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "EngRepairPane::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_CLOSE_MENU, "EngRepairPane::HandleSelectOption" );
}

/*
================
EngRepairPane::SetNumRepairTeams
================
*/
// FUNCTION: STBC 0x00550ee0
void EngRepairPane::SetNumRepairTeams( int numRepairTeams ) {
	this->numRepairTeams = numRepairTeams;
	ArrangeChildren();
}

/*
================
EngRepairPane::SetRepairSubsystem
================
*/
// FUNCTION: STBC 0x00550ef0
void EngRepairPane::SetRepairSubsystem( RepairSubsystem *repair ) {
	if ( repair != NULL ) {
		repairSubsystemID = repair->GetObjID();
	} else {
		repairSubsystemID = 0;
	}
}

/*
================
EngRepairPane::Resize

The lists and dividers take the pane's width.
================
*/
// FUNCTION: STBC 0x00550f10
// PARKED: schedule: the last divider's Resize pushes its slot before loading the vtable in retail; the call order and the four earlier Resizes are retail's, nothing left to spell differently
void EngRepairPane::Resize( float width, float height, bool layout ) {
	TGPane::Resize( width, height, false );
	TGUIObject *repairArea = GetNthChild( REPAIR_AREA );
	TGUIObject *waitingArea = GetNthChild( WAITING_AREA );
	TGUIObject *destroyedArea = GetNthChild( DESTROYED_AREA );
	TGIcon *divider = TGIcon::Cast( GetNthChild( DIVIDER ) );
	TGIcon *divider2 = TGIcon::Cast( GetNthChild( DIVIDER_2 ) );
	repairArea->Resize( width, repairArea->GetHeight(), false );
	waitingArea->Resize( width, waitingArea->GetHeight(), false );
	destroyedArea->Resize( width, destroyedArea->GetHeight(), false );
	divider->Resize( width, divider->GetHeight(), false );
	divider2->Resize( width, divider2->GetHeight(), false );
	if ( layout == true ) {
		Layout();
	}
}

/*
================
EngRepairPane::ArrangeChildren
================
*/
// FUNCTION: STBC 0x00551000
// PARKED: schedule: for the children reloaded from the frame retail loads the vtable before the fadd of the spacing, this build after; the ones kept in registers agree
void EngRepairPane::ArrangeChildren( void ) {
	TGUIObject *repairArea = GetNthChild( REPAIR_AREA );
	TGParagraph *repairLabel = TGParagraph::Cast( GetNthChild( REPAIR_AREA_LABEL ) );
	TGUIObject *waitingArea = GetNthChild( WAITING_AREA );
	TGParagraph *waitingLabel = TGParagraph::Cast( GetNthChild( WAITING_AREA_LABEL ) );
	TGUIObject *destroyedArea = GetNthChild( DESTROYED_AREA );
	TGParagraph *destroyedLabel = TGParagraph::Cast( GetNthChild( DESTROYED_AREA_LABEL ) );
	TGIcon *divider = TGIcon::Cast( GetNthChild( DIVIDER ) );
	TGIcon *divider2 = TGIcon::Cast( GetNthChild( DIVIDER_2 ) );

	repairArea->Resize( repairArea->GetWidth(), ( repairLabel->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f ) * ( numRepairTeams + 1 ), false );
	NiPoint2 size;
	waitingArea->GetDesiredSize( &size );
	waitingArea->Resize( waitingArea->GetWidth(), size.y, false );
	destroyedArea->GetDesiredSize( &size );
	destroyedArea->Resize( destroyedArea->GetWidth(), size.y, false );

	float spacing = DEFAULT_ST_INDENT_VERT * 2.0f;
	repairLabel->SetPosition( 0.0f, 0.0f, false );
	repairArea->SetPosition( 0.0f, repairLabel->GetBottom() + spacing, false );
	divider->SetPosition( 0.0f, repairArea->GetBottom() + spacing, false );
	waitingLabel->SetPosition( 0.0f, divider->GetBottom() + spacing, false );
	waitingArea->SetPosition( 0.0f, waitingLabel->GetBottom() + spacing, false );
	divider2->SetPosition( 0.0f, waitingArea->GetBottom() + spacing, false );
	destroyedLabel->SetPosition( 0.0f, divider2->GetBottom() + spacing, false );
	destroyedArea->SetPosition( 0.0f, destroyedLabel->GetBottom() + spacing, false );
}

/*
================
EngRepairPane::ClearAreas
================
*/
// FUNCTION: STBC 0x00551230
void EngRepairPane::ClearAreas( void ) {
	STSubPane *repairArea = STSubPane::Cast( GetNthChild( REPAIR_AREA ) );
	STSubPane *waitingArea = STSubPane::Cast( GetNthChild( WAITING_AREA ) );
	STSubPane *destroyedArea = STSubPane::Cast( GetNthChild( DESTROYED_AREA ) );
	repairArea->KillChildren();
	waitingArea->KillChildren();
	destroyedArea->KillChildren();
}

/*
================
EngRepairPane::Layout

Takes the parent's width first.
================
*/
// FUNCTION: STBC 0x005512a0
void EngRepairPane::Layout( void ) {
	if ( parent != NULL && parent->GetWidth() != GetWidth() ) {
		Resize( parent->GetWidth(), GetHeight(), false );
	}
	TGPane::Layout();
}

/*
================
EngRepairPane::UpdateAreas

The repair subsystem's list fills the repair team assignments, one
subsystem per team in list order, and the rest the damaged systems;
buttons are moved between the lists or made as needed, and the lists'
extra buttons deleted.
================
*/
// FUNCTION: STBC 0x005512e0
void EngRepairPane::UpdateAreas( void ) {
	RepairSubsystem *repair = RepairSubsystem::Cast( TGObject::GetTGObjectPtr( repairSubsystemID ) );
	if ( repair == NULL ) {
		return;
	}
	NiTList<ShipSubsystem *> *repairList = repair->GetRepairList();
	NiTListIterator pos = repairList->GetHeadPos();
	STSubPane *repairArea = STSubPane::Cast( GetNthChild( REPAIR_AREA ) );
	STSubPane *waitingArea = STSubPane::Cast( GetNthChild( WAITING_AREA ) );
	STSubPane *destroyedArea = STSubPane::Cast( GetNthChild( DESTROYED_AREA ) );
	int numTeams = repair->GetProperty()->GetNumRepairTeams();
	STButton *button = NULL;
	int list = 0;
	bool changed = false;
	bool added = false;
	STButton *current;
	ShipSubsystem *subsystem;

	int i;
	for ( i = 0; i < numTeams; i++ ) {
		current = STButton::Cast( repairArea->GetNthChild( i ) );
		if ( pos == NULL ) {
			break;
		}
		subsystem = repairList->GetNext( pos );
		FindSubsystemButton( subsystem, &button, &list );
		if ( button != NULL ) {
			if ( list == 0 ) {
				if ( current != button ) {
					repairArea->RemoveChild( button, false );
					repairArea->InsertChild( i, button, 0.0f, 0.0f, false );
					changed = true;
				}
			} else if ( list == 1 ) {
				waitingArea->RemoveChild( button, false );
				AddToRepairArea( button, i );
				changed = true;
			} else {
				destroyedArea->RemoveChild( button, false );
				AddToRepairArea( button, i );
				changed = true;
			}
		} else {
			AddToRepairArea( CreateRepairButton( subsystem ), i );
			changed = true;
			added = true;
		}
	}
	for ( ; i < repairArea->GetNumChildren(); i++ ) {
		changed = true;
		TGUIObject *extra = repairArea->GetNthChild( i );
		repairArea->DeleteChild( extra, false );
	}

	for ( i = 0; pos != NULL; i++ ) {
		current = STButton::Cast( waitingArea->GetNthChild( i ) );
		subsystem = repairList->GetNext( pos );
		FindSubsystemButton( subsystem, &button, &list );
		if ( button != NULL ) {
			if ( list == 0 ) {
				repairArea->RemoveChild( button, false );
				waitingArea->InsertChild( i, button, 0.0f, 0.0f, false );
				changed = true;
			} else if ( list == 1 ) {
				if ( current != button ) {
					waitingArea->RemoveChild( button, false );
					waitingArea->InsertChild( i, button, 0.0f, 0.0f, false );
					changed = true;
				}
			} else {
				destroyedArea->RemoveChild( button, false );
				waitingArea->InsertChild( i, button, 0.0f, 0.0f, false );
				changed = true;
			}
		} else {
			waitingArea->InsertChild( i, CreateRepairButton( subsystem ), 0.0f, 0.0f, false );
			added = true;
			changed = true;
		}
	}
	for ( ; i < waitingArea->GetNumChildren(); i++ ) {
		changed = true;
		TGUIObject *extra = waitingArea->GetNthChild( i );
		waitingArea->DeleteChild( extra, false );
	}

	if ( changed == true ) {
		ArrangeChildren();
		if ( added == true ) {
			ResizeToContents();
		}
		repairArea->Layout();
		waitingArea->Layout();
		destroyedArea->Layout();
	}
}

/*
================
EngRepairPane::FindSubsystemButton

Looks for the button named for the subsystem in the repair team
assignments (the second child), the damaged systems (the fourth) and the
destroyed systems (the sixth), in that order; the destroyed systems'
answer stands even when it is NULL.
================
*/
// FUNCTION: STBC 0x00551670
void EngRepairPane::FindSubsystemButton( ShipSubsystem *subsystem, STButton **button, int *list ) {
	STSubPane *repairList = (STSubPane *)GetNthChild( 1 );
	STSubPane *damagedList = (STSubPane *)GetNthChild( 3 );
	STSubPane *destroyedList = (STSubPane *)GetNthChild( 5 );

	*button = repairList->GetButtonW( &subsystem->GetDisplayName() );
	if ( *button != NULL ) {
		*list = 0;
		return;
	}
	*button = damagedList->GetButtonW( &subsystem->GetDisplayName() );
	if ( *button != NULL ) {
		*list = 1;
		return;
	}
	*button = destroyedList->GetButtonW( &subsystem->GetDisplayName() );
	*list = 2;
}

/*
================
EngRepairPane::GetSubsystemButton

The subsystem's button on any of the pane's lists; NULL when it has none.
================
*/
// FUNCTION: STBC 0x00551790 ?GetSubsystemButton@EngRepairPane@@QAEPAVSTButton@@PAVShipSubsystem@@@Z
STButton *EngRepairPane::GetSubsystemButton( ShipSubsystem *subsystem ) {
	STButton *button = NULL;
	int list;
	FindSubsystemButton( subsystem, &button, &list );
	return button;
}

/*
================
EngRepairPane::AddSubsystem

Gives the subsystem a button, at the front of the repair team
assignments when repairing, else at the end of the damaged systems
(invented name; nothing calls it).
================
*/
// FUNCTION: STBC 0x005517c0
void EngRepairPane::AddSubsystem( ShipSubsystem *subsystem, bool repairing ) {
	STButton *button;
	int list;
	FindSubsystemButton( subsystem, &button, &list );
	// BUG: tests the subsystem, not the button found, so a button is made only for a NULL subsystem.
	if ( subsystem != NULL ) {
		return;
	}
	TGPane *repairArea = (TGPane *)GetNthChild( REPAIR_AREA );
	TGPane *waitingArea = (TGPane *)GetNthChild( WAITING_AREA );
	button = CreateRepairButton( subsystem );
	if ( button == NULL ) {
		return;
	}
	if ( repairing == true ) {
		AddToRepairArea( button, 0 );
	} else {
		waitingArea->AddChild( button, 0.0f, 0.0f, true );
	}
	ResizeToContents();
	if ( repairing == true ) {
		repairArea->Layout();
	} else {
		waitingArea->Layout();
	}
}

/*
================
EngRepairPane::AddDestroyedSubsystem

Adds a disabled button with the subsystem's name to the end of the
destroyed systems (the pane's sixth child, a sub pane) and fits the pane
again.
================
*/
// FUNCTION: STBC 0x00551870
void EngRepairPane::AddDestroyedSubsystem( ShipSubsystem *subsystem ) {
	TGPane *destroyedMenu = (TGPane *)GetNthChild( 5 );
	TGString name = subsystem->GetDisplayName();
	STButton *button = new STButton( &name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );
	button->SetDisabled( false );
	button->SetUseUIHeight( false );
	button->SetDisabledColor( &g_kSTMenu2Selected );
	destroyedMenu->InsertChild( 0x7FFFFFFF, button, 0.0f, 0.0f, true );
	ResizeToContents();
}

/*
================
EngRepairPane::RemoveSubsystem

Deletes the subsystem's button from the list holding it, fits the pane
and lays the list out again.
================
*/
// FUNCTION: STBC 0x00551990 ?RemoveSubsystem@EngRepairPane@@QAEXPAVShipSubsystem@@@Z
void EngRepairPane::RemoveSubsystem( ShipSubsystem *subsystem ) {
	STButton *button;
	int list;
	FindSubsystemButton( subsystem, &button, &list );
	if ( button != NULL ) {
		TGPane *buttonList = button->GetParent();
		if ( buttonList != NULL ) {
			buttonList->DeleteChild( button, true );
		}
		ResizeToContents();
		// BUG: the button's parent is laid out without the NULL test above.
		buttonList->Layout();
	}
}

/*
================
EngRepairPane::CreateRepairButton

A repair button for the subsystem, whose activation asks the repair
subsystem to raise its priority, with a condition gauge for its body;
NULL without a repair subsystem.
================
*/
// FUNCTION: STBC 0x005519e0
STButton *EngRepairPane::CreateRepairButton( ShipSubsystem *subsystem ) {
	RepairSubsystem *repair = RepairSubsystem::Cast( TGObject::GetTGObjectPtr( repairSubsystemID ) );
	if ( repair == NULL ) {
		return NULL;
	}
	TGString name = subsystem->GetDisplayName();
	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetSource( this );
	event->SetDestination( repair );
	event->SetObjPtr( subsystem );
	event->SetEventType( ET_REPAIR_INCREASE_PRIORITY );
	STRepairButton *button = new STRepairButton( subsystem, &name, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 );

	STFillGauge *gauge = new STFillGauge( 200, NULL, false );
	gauge->SetObject( subsystem );
	gauge->valueFunction = (GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage;
	gauge->maxFunction = NULL;
	gauge->alternateFunction = (GaugeFlagFunction)&ShipSubsystem::IsDisabled;
	gauge->SetFillColor( g_kSubsystemFillColor );
	gauge->SetEmptyColor( g_kSubsystemEmptyColor );
	gauge->SetAlternateColor( g_kSubsystemDisabledColor );
	button->TGPane::InsertChild( STBUTTON_BODY, gauge, 0.0f, 0.0f, false );
	TGUIObject *body = button->TGPane::GetNthChild( STBUTTON_BODY + 1 );
	button->TGPane::RemoveChild( body, false );
	delete body;
	return button;
}

/*
================
EngRepairPane::AddToRepairArea

Inserts the button among the repair team assignments at index; when the
teams are all taken, the last assignment moves to the end of the damaged
systems first.
================
*/
// FUNCTION: STBC 0x00551c20
void EngRepairPane::AddToRepairArea( STButton *button, int index ) {
	STSubPane *repairArea = STSubPane::Cast( GetNthChild( REPAIR_AREA ) );
	if ( repairArea->GetNumChildren() > numRepairTeams ) {
		STSubPane *waitingArea = STSubPane::Cast( GetNthChild( WAITING_AREA ) );
		STButton *last = STButton::Cast( repairArea->GetLastChild() );
		if ( last != NULL ) {
			repairArea->RemoveChild( last, false );
			waitingArea->AddChild( last, 0.0f, 0.0f, false );
		}
	}
	repairArea->InsertChild( index, button, 0.0f, 0.0f, false );
}

/*
================
EngRepairPane::ResizeToContents

Takes the size that holds every child, as wide as the window's maximum
width when it sits in one, then fits the menus and sub panes holding the
pane again.
================
*/
// FUNCTION: STBC 0x00551cc0
void EngRepairPane::ResizeToContents( void ) {
	float width = GetWidth();
	float height = 0.0f;
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		NiPoint2 size;
		NiPoint2 position;
		child->GetDesiredSize( &size );
		child->GetPosition( &position );
		size.x = size.x + position.x;
		size.y = size.y + position.y;
		if ( size.x > width ) {
			width = size.x;
		}
		if ( size.y > height ) {
			height = size.y;
		}
	}

	STStylizedWindow *window = NULL;
	TGPane *pane;
	for ( pane = parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->GetParent() ) {
		window = STStylizedWindow::Cast( pane );
		if ( window != NULL ) {
			width = window->GetMaximumWidth();
			break;
		}
	}
	Resize( width, height, false );

	pane = parent;
	while ( pane != NULL && pane != window ) {
		if ( STMenu::Cast( pane ) == NULL && STSubPane::Cast( pane ) == NULL ) {
			break;
		}
		STMenu *menu = STMenu::Cast( pane );
		STSubPane *subPane = STSubPane::Cast( pane );
		if ( menu != NULL ) {
			menu->ResizeToContents( false );
		}
		if ( subPane != NULL ) {
			subPane->ResizeToContents( false );
		}
		pane = pane->GetParent();
	}
}

/*
================
EngRepairPane::HandleKeyboard

A release of the key for the focused pane's option may be bound to an action.
================
*/
// FUNCTION: STBC 0x00551e00
void EngRepairPane::HandleKeyboard( TGKeyboardEvent *event ) {
	TGPane *pane = TGPane::Cast( GetFocus() );
	if ( pane == NULL ) {
		CallNextHandler( event );
		return;
	}
	if ( event->GetKeyState() == KS_KEYUP ) {
		if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), KS_KEYUP, this ) ) {
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
EngRepairPane::HandleSelectOption

The event's int moves the focus to the previous (-1) or next (1) element,
going on to the neighbouring pane at either end. A closed menu closes the
menu that holds the pane's parent.
================
*/
// FUNCTION: STBC 0x00551e70
void EngRepairPane::HandleSelectOption( TGEvent *event ) {
	TGPane *pane = TGPane::Cast( GetFocus() );
	TGUIObject *focus = pane->GetFocus();
	TGPane *container = GetParent();
	if ( event->GetEventType() == ET_INPUT_SELECT_OPTION ) {
		if ( ( (TGIntEvent *)event )->GetInt() == -1 ) {
			event->SetDestination( NULL );
			if ( focus != NULL ) {
				TGUIObject *previous = InterfaceModule::GetPrevUIElement( pane, 0 );
				if ( previous != NULL ) {
					pane->SetFocus( previous, true );
				} else {
					TGUIObject *previousPane = InterfaceModule::GetPrevUIElement( this, 0 );
					if ( previousPane != NULL ) {
						SetFocus( previousPane, true );
						pane = TGPane::Cast( previousPane );
						pane->SetFocus( InterfaceModule::GetLastUIElement( pane, 0 ), true );
					}
				}
			} else {
				pane->SetFocus( InterfaceModule::GetFirstUIElement( pane, 0 ), true );
			}
		}
		if ( ( (TGIntEvent *)event )->GetInt() == 1 ) {
			event->SetDestination( NULL );
			if ( focus != NULL ) {
				TGUIObject *next = InterfaceModule::GetNextUIElement( pane, 0 );
				if ( next != NULL ) {
					pane->SetFocus( next, true );
					return;
				}
				TGUIObject *nextPane = InterfaceModule::GetNextUIElement( this, 0 );
				if ( nextPane == NULL ) {
					return;
				}
				SetFocus( nextPane, true );
				pane = TGPane::Cast( nextPane );
			}
			pane->SetFocus( InterfaceModule::GetFirstUIElement( pane, 0 ), true );
		}
	} else if ( event->GetEventType() == ET_INPUT_CLOSE_MENU ) {
		STMenu *menu = STMenu::Cast( container->GetParent() );
		if ( menu != NULL ) {
			menu->Close( true );
		}
	}
}

/*
================
EngRepairPane::HandleSetPlayer

The repair subsystem's ID of the new player ship is kept; none without one.
================
*/
// FUNCTION: STBC 0x00551ff0
// DRAFT: retail keeps a zero in ecx (xor ecx, ecx) for both null tests and the store; this build tests with test and stores an immediate
void EngRepairPane::HandleSetPlayer( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	RepairSubsystem *repair = NULL;
	if ( player != NULL ) {
		repair = player->GetRepairSubsystem();
	}
	if ( repair != NULL ) {
		repairSubsystemID = repair->GetObjID();
	} else {
		repairSubsystemID = 0;
	}
}

/*
================
EngRepairPane::WriteToStream
================
*/
// FUNCTION: STBC 0x00552020
bool EngRepairPane::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( numRepairTeams );
	if ( repairSubsystemID != 0 ) {
		stream->WriteBool( true );
		stream->WriteID( repairSubsystemID );
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
EngRepairPane::ReadFromStream
================
*/
// FUNCTION: STBC 0x00552080
bool EngRepairPane::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	numRepairTeams = stream->ReadInt();
	if ( stream->ReadBool() == true ) {
		repairSubsystemID = stream->ReadID();
	} else {
		repairSubsystemID = 0;
	}
	return true;
}
