/*
===============================================================================

	SortedRegionMenu.cpp

===============================================================================
*/

#include <string.h>
#include "SortedRegionMenu.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "Game.h"
#include "CharacterClass.h"
#include "SetManager.h"
#include "STWarpButton.h"
#include "STSubPane.h"
#include "STTopLevelMenu.h"
#include "ShipClass.h"
#include "TGIntEvent.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "GraphicsModeInfo.h"
#include "STColors.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIcon.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00518ac0 _$E537
// FUNCTION: STBC 0x00518ae0 _$E536

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005169a0 _$E33
// FUNCTION: STBC 0x005169b0 _$E32
// FUNCTION: STBC 0x005169c0 _$E37
// FUNCTION: STBC 0x005169d0 _$E36
// FUNCTION: STBC 0x005169e0 _$E48
// FUNCTION: STBC 0x005169f0 _$E47
// FUNCTION: STBC 0x00516a00 _$E52
// FUNCTION: STBC 0x00516a10 _$E51
// FUNCTION: STBC 0x00516a20 _$E56
// FUNCTION: STBC 0x00516a30 _$E55
// FUNCTION: STBC 0x00516a40 _$E60
// FUNCTION: STBC 0x00516a50 _$E59
// FUNCTION: STBC 0x00516a60 _$E184
// FUNCTION: STBC 0x00516a70 _$E183
// FUNCTION: STBC 0x005182d0 _$E126
// FUNCTION: STBC 0x005182e0 _$E125
// FUNCTION: STBC 0x005182f0 _$E139
// FUNCTION: STBC 0x00518300 _$E138
// FUNCTION: STBC 0x00518310 _$E189
// FUNCTION: STBC 0x00518320 _$E188
// FUNCTION: STBC 0x00518330 _$E194
// FUNCTION: STBC 0x00518340 _$E193
// FUNCTION: STBC 0x00518350 _$E198
// FUNCTION: STBC 0x00518360 _$E197
// FUNCTION: STBC 0x00518370 _$E202
// FUNCTION: STBC 0x00518380 _$E201
// FUNCTION: STBC 0x00518390 _$E209
// FUNCTION: STBC 0x005183a0 _$E208
// FUNCTION: STBC 0x005183b0 _$E367
// FUNCTION: STBC 0x005183c0 _$E366
// FUNCTION: STBC 0x005183d0 _$E371
// FUNCTION: STBC 0x005183e0 _$E370
// FUNCTION: STBC 0x005183f0 _$E84
// FUNCTION: STBC 0x00518400 _$E83
// FUNCTION: STBC 0x00518410 _$E307
// FUNCTION: STBC 0x00518420 _$E306
// FUNCTION: STBC 0x00518430 _$E314
// FUNCTION: STBC 0x00518440 _$E313
// FUNCTION: STBC 0x00518450 _$E318
// FUNCTION: STBC 0x00518460 _$E317
// FUNCTION: STBC 0x00518470 _$E322
// FUNCTION: STBC 0x00518480 _$E321
// FUNCTION: STBC 0x00518490 _$E326
// FUNCTION: STBC 0x005184a0 _$E325
// FUNCTION: STBC 0x005184b0 _$E332
// FUNCTION: STBC 0x005184c0 _$E331
// FUNCTION: STBC 0x005184d0 _$E375
// FUNCTION: STBC 0x005184e0 _$E374
// FUNCTION: STBC 0x005184f0 _$E379
// FUNCTION: STBC 0x00518500 _$E378
// FUNCTION: STBC 0x00518510 _$E383
// FUNCTION: STBC 0x00518520 _$E382
// FUNCTION: STBC 0x00518530 _$E387
// FUNCTION: STBC 0x00518540 _$E386
// FUNCTION: STBC 0x00518550 _$E391
// FUNCTION: STBC 0x00518560 _$E390
// FUNCTION: STBC 0x00518570 _$E395
// FUNCTION: STBC 0x00518580 _$E394
// FUNCTION: STBC 0x00518590 _$E399
// FUNCTION: STBC 0x005185a0 _$E398
// FUNCTION: STBC 0x005185b0 _$E403
// FUNCTION: STBC 0x005185c0 _$E402
// FUNCTION: STBC 0x005185d0 _$E407
// FUNCTION: STBC 0x005185e0 _$E406
// FUNCTION: STBC 0x005185f0 _$E411
// FUNCTION: STBC 0x00518600 _$E410
// FUNCTION: STBC 0x00518610 _$E89
// FUNCTION: STBC 0x00518620 _$E88
// FUNCTION: STBC 0x00518630 _$E415
// FUNCTION: STBC 0x00518640 _$E414
// FUNCTION: STBC 0x00518650 _$E94
// FUNCTION: STBC 0x00518660 _$E93
// FUNCTION: STBC 0x00518670 _$E419
// FUNCTION: STBC 0x00518680 _$E418
// FUNCTION: STBC 0x00518690 _$E423
// FUNCTION: STBC 0x005186a0 _$E422
// FUNCTION: STBC 0x005186b0 _$E427
// FUNCTION: STBC 0x005186c0 _$E426
// FUNCTION: STBC 0x005186d0 _$E431
// FUNCTION: STBC 0x005186e0 _$E430
// FUNCTION: STBC 0x005186f0 _$E103
// FUNCTION: STBC 0x00518700 _$E102
// FUNCTION: STBC 0x00518710 _$E435
// FUNCTION: STBC 0x00518720 _$E434
// FUNCTION: STBC 0x00518730 _$E439
// FUNCTION: STBC 0x00518740 _$E438
// FUNCTION: STBC 0x00518750 _$E443
// FUNCTION: STBC 0x00518760 _$E442
// FUNCTION: STBC 0x00518770 _$E109
// FUNCTION: STBC 0x00518780 _$E108
// FUNCTION: STBC 0x00518790 _$E114
// FUNCTION: STBC 0x005187a0 _$E113
// FUNCTION: STBC 0x005187b0 _$E144
// FUNCTION: STBC 0x005187c0 _$E143
// FUNCTION: STBC 0x005187d0 _$E447
// FUNCTION: STBC 0x005187e0 _$E446
// FUNCTION: STBC 0x005187f0 _$E149
// FUNCTION: STBC 0x00518800 _$E148
// FUNCTION: STBC 0x00518810 _$E451
// FUNCTION: STBC 0x00518820 _$E450
// FUNCTION: STBC 0x00518830 _$E455
// FUNCTION: STBC 0x00518840 _$E454
// FUNCTION: STBC 0x00518850 _$E154
// FUNCTION: STBC 0x00518860 _$E153
// FUNCTION: STBC 0x00518870 _$E459
// FUNCTION: STBC 0x00518880 _$E458
// FUNCTION: STBC 0x00518890 _$E119
// FUNCTION: STBC 0x005188a0 _$E118
// FUNCTION: STBC 0x005188b0 _$E463
// FUNCTION: STBC 0x005188c0 _$E462
// FUNCTION: STBC 0x005188d0 _$E467
// FUNCTION: STBC 0x005188e0 _$E466
// FUNCTION: STBC 0x005188f0 _$E159
// FUNCTION: STBC 0x00518900 _$E158
// FUNCTION: STBC 0x00518910 _$E164
// FUNCTION: STBC 0x00518920 _$E163
// FUNCTION: STBC 0x00518930 _$E471
// FUNCTION: STBC 0x00518940 _$E470
// FUNCTION: STBC 0x00518950 _$E475
// FUNCTION: STBC 0x00518960 _$E474
// FUNCTION: STBC 0x00518970 _$E479
// FUNCTION: STBC 0x00518980 _$E478
// FUNCTION: STBC 0x00518990 _$E483
// FUNCTION: STBC 0x005189a0 _$E482
// FUNCTION: STBC 0x005189b0 _$E487
// FUNCTION: STBC 0x005189c0 _$E486
// FUNCTION: STBC 0x005189d0 _$E491
// FUNCTION: STBC 0x005189e0 _$E490
// FUNCTION: STBC 0x005189f0 _$E495
// FUNCTION: STBC 0x00518a00 _$E494
// FUNCTION: STBC 0x00518a10 _$E499
// FUNCTION: STBC 0x00518a20 _$E498
// FUNCTION: STBC 0x00518a30 _$E169
// FUNCTION: STBC 0x00518a40 _$E168
// FUNCTION: STBC 0x00518a50 _$E503
// FUNCTION: STBC 0x00518a60 _$E502
// FUNCTION: STBC 0x00518a70 _$E174
// FUNCTION: STBC 0x00518a80 _$E173
// FUNCTION: STBC 0x00518a90 _$E179
// FUNCTION: STBC 0x00518aa0 _$E178
// FUNCTION: STBC 0x00518b00 _$E507
// FUNCTION: STBC 0x00518b10 _$E506
// FUNCTION: STBC 0x00518b20 _$E512
// FUNCTION: STBC 0x00518b30 _$E511
// FUNCTION: STBC 0x00518b40 _$E516
// FUNCTION: STBC 0x00518b50 _$E515

/*
================
SortedRegionMenu::Cast

The object as a SortedRegionMenu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00516AD0
SortedRegionMenu *SortedRegionMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
		return (SortedRegionMenu *)object;
	}
	return NULL;
}

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00516920 _$E351
// FUNCTION: STBC 0x00516930 _$E348
// FUNCTION: STBC 0x00516940 _$E350
// FUNCTION: STBC 0x00516950 _$E349
// FUNCTION: STBC 0x00516960 _$E357
// FUNCTION: STBC 0x00516970 _$E354
// FUNCTION: STBC 0x00516980 _$E356
// FUNCTION: STBC 0x00516990 _$E355

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00516a80 _$E522
// FUNCTION: STBC 0x00516a90 _$E518
// FUNCTION: STBC 0x00516aa0 _$E521
// FUNCTION: STBC 0x00516ab0 _$E519
// DATA: STBC 0x00987ed0
TGEventHandlerTable SortedRegionMenu::classHandlers;

// DATA: STBC 0x00987f60
STWarpButton *SortedRegionMenu::warpButton;

// DATA: STBC 0x00987f64
SortedRegionMenu *SortedRegionMenu::setCourseMenu;

// Two greens nothing in retail reads (their dynamic initialisers are the
// only code that names them).
// FUNCTION: STBC 0x00516b00 _$E525
// FUNCTION: STBC 0x00516b10 _$E524
// DATA: STBC 0x00987f40
static NiColorA unknown987F40( 0.0f, 0.8f, 0.0f, 1.0f );

// FUNCTION: STBC 0x00516b40 _$E528
// FUNCTION: STBC 0x00516b50 _$E527
// DATA: STBC 0x00987e50
static NiColorA unknown987E50( 0.0f, 0.4f, 0.0f, 1.0f );

/*
================
SortedRegionMenu::SortedRegionMenu

A menu that is not openable, with the region and placement it was made for
and the two separator icons of the current resolution. The first one is
the set course menu and follows the player.
================
*/
// FUNCTION: STBC 0x00516b80 ??0SortedRegionMenu@@QAE@PAVTGString@@PAD1@Z
SortedRegionMenu::SortedRegionMenu( TGString *name, char *region, char *placement ) : STCharacterMenu( name, NULL, NULL ) {
	regionName = NULL;
	episodeName = NULL;
	missionName = NULL;
	placementName = NULL;
	useLargeLoadingScreen = false;
	unknown6D = false;
	if ( region != NULL ) {
		SetRegionName( region );
	}
	SetPlacementName( placement );
	SetNotOpenable();

	char *iconGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	unknown70 = new TGIcon( iconGroup, 700, NULL );
	unknown70->SetColor( &g_kLeftSeparatorColor );
	unknown74 = new TGIcon( iconGroup, 700, NULL );
	unknown74->SetColor( &g_kLeftSeparatorColor );

	if ( setCourseMenu == NULL ) {
		setCourseMenu = this;
		g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "SortedRegionMenu::SetPlayerHandler" );
		setCourseMenu->SetOpenable();
	}
}

/*
================
SortedRegionMenu::SortedRegionMenu

The same menu, named by a plain string (the SWIG wrapper
SortedRegionMenu_Create's).
================
*/
// FUNCTION: STBC 0x00516fd0 ??0SortedRegionMenu@@QAE@PAD00@Z
SortedRegionMenu::SortedRegionMenu( char *name, char *region, char *placement ) : STCharacterMenu( name, NULL, NULL ) {
	regionName = NULL;
	episodeName = NULL;
	missionName = NULL;
	placementName = NULL;
	useLargeLoadingScreen = false;
	unknown6D = false;
	if ( region != NULL ) {
		SetRegionName( region );
	}
	SetPlacementName( placement );
	SetNotOpenable();

	char *iconGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	unknown70 = new TGIcon( iconGroup, 700, NULL );
	unknown70->SetColor( &g_kLeftSeparatorColor );
	unknown74 = new TGIcon( iconGroup, 700, NULL );
	unknown74->SetColor( &g_kLeftSeparatorColor );

	if ( setCourseMenu == NULL ) {
		setCourseMenu = this;
		g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "SortedRegionMenu::SetPlayerHandler" );
		setCourseMenu->SetOpenable();
	}
}

/*
================
SortedRegionMenu::~SortedRegionMenu
================
*/
// FUNCTION: STBC 0x00517130 ??1SortedRegionMenu@@UAE@XZ
SortedRegionMenu::~SortedRegionMenu( void ) {
	Unknown517C60();
	delete unknown70;
	delete unknown74;
	if ( regionName != NULL ) {
		delete regionName;
	}
	if ( episodeName != NULL ) {
		delete episodeName;
	}
	if ( missionName != NULL ) {
		delete missionName;
	}
	if ( placementName != NULL ) {
		delete placementName;
	}
	if ( this == setCourseMenu ) {
		setCourseMenu = NULL;
	}
}

/*
================
SortedRegionMenu::SetWarpButton
================
*/
// FUNCTION: STBC 0x005171F0
void SortedRegionMenu::SetWarpButton( STWarpButton *button ) {
	warpButton = button;
}

/*
================
SortedRegionMenu::InsertChild

A region goes in at the place its name sorts to, and replaces the region of
the same name. Anything else is inserted as a menu does.
================
*/
// FUNCTION: STBC 0x00517200 ?InsertChild@SortedRegionMenu@@UAEXHPAVTGUIObject@@MM_N@Z
void SortedRegionMenu::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	if ( !child->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
		STMenu::InsertChild( index, child, x, y, layout );
		return;
	}
	SortedRegionMenu *region = (SortedRegionMenu *)child;
	TGString name( "" );
	TGString otherName( "" );
	region->GetName( &name );
	int i;
	for ( i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *existing = GetNthChild( i );
		if ( existing->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			( (SortedRegionMenu *)existing )->GetName( &otherName );
			if ( name.Compare( &otherName, false ) == 0 ) {
				RemoveChild( existing, false );
				break;
			}
		}
	}
	for ( i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *existing = GetNthChild( i );
		if ( existing->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			int order = region->Compare( (SortedRegionMenu *)existing );
			if ( order < 0 ) {
				index = i;
				break;
			} else if ( order == 0 ) {
				delete region;
				return;
			}
		}
	}
	STMenu::InsertChild( index, child, x, y, false );
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	} else {
		ResizeToContents( true );
	}
}

/*
================
SortedRegionMenu::Compare

Two regions with the same name are the same; a menu flagged at 0x6D goes
first; the regions without a mission go after the ones with a mission; all
the rest stays as the names compare.
================
*/
// FUNCTION: STBC 0x005173d0
int SortedRegionMenu::Compare( SortedRegionMenu *other ) {
	TGString name( "" );
	TGString otherName( "" );
	GetName( &name );
	other->GetName( &otherName );
	int result = name.Compare( &otherName, false );
	if ( result == 0 ) {
		return 0;
	}
	if ( unknown6D ) {
		return -1;
	}
	if ( other->unknown6D ) {
		return 1;
	}
	if ( missionName != NULL && other->missionName == NULL ) {
		return -1;
	}
	if ( other->missionName != NULL && missionName == NULL ) {
		return 1;
	}
	return result;
}

/*
================
SortedRegionMenu::SendActivationEvent

A region that holds regions is just opened. Choosing the region the warp
button is not set to warps there (SetWarpToHere), tells the Helm's menu, and
closes this menu and its ancestors.
================
*/
// FUNCTION: STBC 0x00517510
void SortedRegionMenu::SendActivationEvent( void ) {
	if ( regionName == NULL ) {
		STMenu::SendActivationEvent();
		return;
	}
	if ( IsOpenable() && GetNumChildren() > 0 ) {
		STMenu::SendActivationEvent();
		return;
	}
	char *warpLocation = warpButton->GetLocation();
	if ( warpLocation != NULL && strcmp( regionName, warpLocation ) == 0 ) {
		if ( GetNumChildren() > 0 ) {
			SetOpenable();
		}
		STMenu::SendActivationEvent();
		return;
	}
	if ( regionName != NULL ) {
		SetWarpToHere();
	}
	if ( !IsOpenable() || GetNumChildren() == 0 ) {
		SetClass *bridge = g_kSetManager.GetSet( "bridge" );
		if ( bridge != NULL ) {
			CharacterClass *helm = CharacterClass::GetObject( bridge, "Helm" );
			if ( helm != NULL && helm->GetMenu() != NULL ) {
				TGIntEvent *event = new TGIntEvent;
				event->SetDestination( helm->GetMenu() );
				event->SetEventType( ET_SORTED_REGION_ACTIVATED );
				event->SetInt( 0 );
				event->SetSource( this );
				g_kEventManager.AddEvent( event );
			}
		}
	}
	Close( true );
	VirtualSlot115();
	STMenu::SendActivationEvent();
}

/*
================
SortedRegionMenu::SetWarpToHere

The warp button now goes to this region, and the set course menu is
arranged around it.
================
*/
// FUNCTION: STBC 0x005176a0
void SortedRegionMenu::SetWarpToHere( void ) {
	warpButton->SetDestination( regionName, missionName, placementName, episodeName, id );
	warpButton->SetUseLargeLoadingScreen( useLargeLoadingScreen );
	if ( setCourseMenu != NULL ) {
		setCourseMenu->Unknown517A00();
	}
}

/*
================
SortedRegionMenu::Open
================
*/
// FUNCTION: STBC 0x005176f0 ?Open@SortedRegionMenu@@UAEX_N@Z
void SortedRegionMenu::Open( bool layout ) {
	Unknown517A00();
	STMenu::Open( layout );
}

/*
================
SortedRegionMenu::VirtualSlot115

Clears the menu's info and closes the ancestors that are region menus.
================
*/
// FUNCTION: STBC 0x00517710 ?VirtualSlot115@SortedRegionMenu@@UAEXXZ
void SortedRegionMenu::VirtualSlot115( void ) {
	Unknown517C60();
	TGPane *pane = parent;
	if ( pane != NULL ) {
		pane->SetNotExclusive( false );
		TGPane *grandparent = pane->GetParent();
		if ( grandparent != NULL && grandparent->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			( (SortedRegionMenu *)grandparent )->Close( false );
			( (SortedRegionMenu *)grandparent )->VirtualSlot115();
		}
	}
}

/*
================
SortedRegionMenu::SetRegionName
================
*/
// FUNCTION: STBC 0x00517760
void SortedRegionMenu::SetRegionName( char *string ) {
	if ( regionName != NULL ) {
		delete regionName;
	}
	regionName = NULL;
	if ( string != NULL ) {
		regionName = new char[strlen( string ) + 1];
		strcpy( regionName, string );
	}
}

/*
================
SortedRegionMenu::SetEpisodeName
================
*/
// FUNCTION: STBC 0x005177D0
void SortedRegionMenu::SetEpisodeName( char *string ) {
	if ( episodeName != NULL ) {
		if ( string != NULL && strcmp( episodeName, string ) == 0 ) {
			return;
		}
		delete episodeName;
	}
	episodeName = NULL;
	if ( string != NULL ) {
		episodeName = new char[strlen( string ) + 1];
		strcpy( episodeName, string );
	}
	if ( parent != NULL ) {
		TGPane *grandparent = parent->GetParent();
		grandparent->RemoveChild( this, false );
		grandparent->AddChild( this, 0.0f, 0.0f, true );
		grandparent->Layout();
	}
}

/*
================
SortedRegionMenu::SetMissionName
================
*/
// FUNCTION: STBC 0x005178B0
void SortedRegionMenu::SetMissionName( char *string ) {
	if ( missionName != NULL ) {
		if ( string != NULL && strcmp( missionName, string ) == 0 ) {
			return;
		}
		delete missionName;
	}
	missionName = NULL;
	if ( string != NULL ) {
		missionName = new char[strlen( string ) + 1];
		strcpy( missionName, string );
	}
	if ( parent != NULL ) {
		TGPane *grandparent = parent->GetParent();
		grandparent->RemoveChild( this, false );
		grandparent->AddChild( this, 0.0f, 0.0f, true );
		grandparent->Layout();
	}
}

/*
================
SortedRegionMenu::SetPlacementName
================
*/
// FUNCTION: STBC 0x00517990
void SortedRegionMenu::SetPlacementName( char *string ) {
	if ( placementName != NULL ) {
		delete placementName;
	}
	placementName = NULL;
	if ( string != NULL ) {
		placementName = new char[strlen( string ) + 1];
		strcpy( placementName, string );
	}
}

/*
================
SortedRegionMenu::Unknown517A00

Puts the menu's children in order after a change: the child the warp button
goes to is chosen and focused, the first child follows the menu's openable
state, the separator icons go between the groups (before the regions
and between the ones with a mission and the ones without), and the open
regions below are arranged the same way.
================
*/
// FUNCTION: STBC 0x00517a00
void SortedRegionMenu::Unknown517A00( void ) {
	Unknown517C60();

	int regionCount = 0;
	int i;
	for ( i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			regionCount++;
			( (STMenu *)child )->SetChosen( false, true );
		}
	}

	for ( i = 0; i < GetNumChildren(); i++ ) {
		STMenu *child = (STMenu *)GetNthChild( i );
		if ( warpButton->GetDestinationButtonID() == child->GetObjID() ) {
			child->SetChosen( true, true );
			SetFocus( child, false );
		}
		if ( i == 0 ) {
			SortedRegionMenu *region = Cast( child );
			if ( region != NULL ) {
				if ( !region->IsOpenable() ) {
					region->SetDisabled( true );
					region->Unknown5273F0();
				} else {
					child->SetEnabled( true );
					region->Unknown5273F0();
				}
			}
		} else {
			child->SetEnabled( true );
		}
	}

	if ( regionCount > 1 && IsOpenable() ) {
		unknown70->Resize( GetWidth(), unknown70->GetHeight(), false );
		unknown74->Resize( GetWidth(), unknown74->GetHeight(), false );
		InsertChild( 1, unknown70, 0.0f, 0.0f, false );
		for ( int j = 2; j < GetNumChildren() - 1; j++ ) {
			TGUIObject *child = GetNthChild( j );
			if ( child->IsTypeOf( SORTEDREGIONMENU_TYPE ) && ( (SortedRegionMenu *)child )->missionName == NULL ) {
				if ( j > 2 ) {
					InsertChild( j, unknown74, 0.0f, 0.0f, false );
				}
				break;
			}
		}
	}

	for ( i = 0; i < GetNumChildren(); i++ ) {
		SortedRegionMenu *region = Cast( GetNthChild( i ) );
		if ( region != NULL && region->IsOpened() ) {
			region->Unknown517A00();
		}
	}

	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	} else {
		ResizeToContents( true );
	}
}

/*
================
SortedRegionMenu::Unknown517C60

Takes the two separator icons out of the menu (they stay owned by it).
================
*/
// FUNCTION: STBC 0x00517c60
void SortedRegionMenu::Unknown517C60( void ) {
	RemoveChild( unknown70, false );
	RemoveChild( unknown74, false );
}

/*
================
SortedRegionMenu::InitializeAllSets

Runs the Initialize function of the script module each region menu below
names, and returns the set of the module this menu names, as its GetSet
function gives it.
================
*/
// FUNCTION: STBC 0x00517c90
SetClass *SortedRegionMenu::InitializeAllSets( void ) {
	SetClass *set = NULL;
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		SortedRegionMenu *child = (SortedRegionMenu *)GetNthChild( i );
		if ( child->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			char *module = child->regionName;
			if ( module != NULL ) {
				PyObject *result;
				if ( PyEmbed::CallFunction( module, "Initialize", "O", &result, "" ) == -1 ) {
					PyErr_Print();
					return NULL;
				}
				Py_DECREF( result );
				if ( strcmp( module, regionName ) == 0 ) {
					if ( PyEmbed::CallFunction( module, "GetSet", "O", &result, "" ) == -1 ) {
						PyErr_Print();
						return NULL;
					}
					SetClass *found = (SetClass *)PyEmbed::GetObjectPointer( result );
					Py_DECREF( result );
					set = found;
				}
			}
			child->InitializeAllSets();
		}
	}
	return set;
}

/*
================
SortedRegionMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00517df0
void SortedRegionMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SortedRegionMenu::PlayerShipMovedHandler, "SortedRegionMenu::PlayerShipMovedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&SortedRegionMenu::SetPlayerHandler, "SortedRegionMenu::SetPlayerHandler" );
}

/*
================
SortedRegionMenu::SetPlayerHandler

The new player's ship entering a set moves the set course menu to the
region of that set.
================
*/
// FUNCTION: STBC 0x00517e20
// DRAFT: register allocation: retail keeps the event in esi and this in edi, ours swaps them (83.7)
void SortedRegionMenu::SetPlayerHandler( TGEvent *event ) {
	TGEventHandlerObject *player = (TGEventHandlerObject *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( player != NULL && setCourseMenu != NULL ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "SortedRegionMenu::PlayerShipMovedHandler", player );
		PlayerShipMovedHandler( NULL );
	}
	event->GetDestination()->CallNextHandler( event );
}

/*
================
SortedRegionMenu::Unknown517E80

Finds the region whose name holds the set name, moving each menu on the way
to the front, and flags the ones on the way (unknown6D).
================
*/
// FUNCTION: STBC 0x00517e80
bool SortedRegionMenu::Unknown517E80( char *setName ) {
	SetNotOpenable();
	unknown6D = false;
	if ( GetNumChildren() <= 0 ) {
		if ( regionName != NULL && strstr( regionName, setName ) != NULL ) {
			unknown6D = true;
			return true;
		}
	} else {
		for ( int i = 0; i < GetNumChildren(); i++ ) {
			TGUIObject *child = GetNthChild( i );
			if ( child->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
				SortedRegionMenu *region = (SortedRegionMenu *)child;
				if ( region->Unknown517E80( setName ) ) {
					unknown6D = true;
					RemoveChild( child, false );
					STMenu::InsertChild( 0, child, 0.0f, 0.0f, false );
					SetOpenable();
					return true;
				}
				if ( i == 0 && ( region->IsOpenable() || !region->IsEnabled() ) ) {
					RemoveChild( child, false );
					InsertChild( 0x7FFFFFFF, child, 0.0f, 0.0f, false );
				}
			}
		}
	}
	return false;
}

/*
================
SortedRegionMenu::PlayerShipMovedHandler

The player's ship moved to another set: the menu of the new set goes to the
front.
================
*/
// FUNCTION: STBC 0x00517f90
void SortedRegionMenu::PlayerShipMovedHandler( TGEvent *event ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL && ( event == NULL || player == (ShipClass *)event->GetDestination() ) ) {
		if ( player->GetContainingSet() != NULL ) {
			setCourseMenu->Unknown517E80( player->GetContainingSet()->GetName() );
		}
		setCourseMenu->SetOpenable();
		setCourseMenu->Unknown517A00();
		if ( event != NULL ) {
			event->GetDestination()->CallNextHandler( event );
		}
	}
}

/*
================
SortedRegionMenu::ClearInfo

Clears the menu and, in turn, every region menu below it.
================
*/
// FUNCTION: STBC 0x00517FF0
void SortedRegionMenu::ClearInfo( void ) {
	SetEpisodeName( NULL );
	SetMissionName( NULL );
	SetPlacementName( NULL );

	int numChildren = GetNumChildren();
	for ( int i = 0; i < numChildren; i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child->IsTypeOf( SORTEDREGIONMENU_TYPE ) ) {
			( (SortedRegionMenu *)child )->ClearInfo();
		}
	}
}

/*
================
SortedRegionMenu::ClearSetCourseMenu

Takes the set course menu's two children out and kills the rest.
================
*/
// FUNCTION: STBC 0x00518050
void SortedRegionMenu::ClearSetCourseMenu( void ) {
	if ( setCourseMenu != NULL ) {
		setCourseMenu->RemoveChild( setCourseMenu->unknown70, false );
		setCourseMenu->RemoveChild( setCourseMenu->unknown74, false );
		setCourseMenu->KillChildren();
	}
}

/*
================
SortedRegionMenu::WriteMenus

The warp button and the set course menu are saved as their IDs.
================
*/
// FUNCTION: STBC 0x00518090
bool SortedRegionMenu::WriteMenus( TGStream *stream ) {
	stream->WriteID( GetSavedID( warpButton ) );
	stream->WriteID( GetSavedID( setCourseMenu ) );
	return true;
}

/*
================
SortedRegionMenu::ReadMenus

Keeps the IDs in the pointers until PostFixUpMenus.
================
*/
// FUNCTION: STBC 0x005180e0
bool SortedRegionMenu::ReadMenus( TGStream *stream ) {
	warpButton = (STWarpButton *)stream->ReadID();
	setCourseMenu = (SortedRegionMenu *)stream->ReadID();
	return true;
}

/*
================
SortedRegionMenu::FixUpMenus

Both objects must have been loaded.
================
*/
// FUNCTION: STBC 0x00518110
bool SortedRegionMenu::FixUpMenus( void ) {
	if ( warpButton != NULL && GetTGObjectPtr( (long)warpButton ) == NULL ) {
		return false;
	}
	if ( setCourseMenu != NULL && GetTGObjectPtr( (long)setCourseMenu ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
SortedRegionMenu::PostFixUpMenus
================
*/
// FUNCTION: STBC 0x00518150
void SortedRegionMenu::PostFixUpMenus( void ) {
	if ( warpButton == NULL ) {
		warpButton = NULL;
	} else {
		warpButton = (STWarpButton *)GetTGObjectPtr( (long)warpButton );
	}
	if ( setCourseMenu == NULL ) {
		setCourseMenu = NULL;
	} else {
		setCourseMenu = (SortedRegionMenu *)GetTGObjectPtr( (long)setCourseMenu );
	}
}
/*
================
SortedRegionMenu::WriteToStream

The menu, the two separator icons, the four strings and the two flags.
================
*/
// FUNCTION: STBC 0x00518190 ?WriteToStream@SortedRegionMenu@@UAE_NPAVTGStream@@@Z
bool SortedRegionMenu::WriteToStream( TGStream *stream ) {
	RemoveChild( unknown70, true );
	RemoveChild( unknown74, true );
	if ( !STMenu::WriteToStream( stream ) ) {
		return false;
	}
	if ( !unknown70->WriteToStream( stream ) ) {
		return false;
	}
	if ( !unknown74->WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( regionName );
	stream->VirtualSlot11( episodeName );
	stream->VirtualSlot11( missionName );
	stream->VirtualSlot11( placementName );
	stream->WriteBool( useLargeLoadingScreen );
	stream->WriteBool( unknown6D );
	if ( this == setCourseMenu ) {
		Unknown517A00();
	}
	return true;
}

/*
================
SortedRegionMenu::ReadFromStream

The menu, the two separator icons, the four strings and the two flags.
================
*/
// FUNCTION: STBC 0x00518250
bool SortedRegionMenu::ReadFromStream( TGStream *stream ) {
	if ( !STMenu::ReadFromStream( stream ) ) {
		return false;
	}
	unknown70 = (TGIcon *)TGObject::LoadObject( stream, 0 );
	unknown74 = (TGIcon *)TGObject::LoadObject( stream, 0 );
	regionName = stream->VirtualSlot10( -1 );
	episodeName = stream->VirtualSlot10( -1 );
	missionName = stream->VirtualSlot10( -1 );
	placementName = stream->VirtualSlot10( -1 );
	useLargeLoadingScreen = stream->ReadBool();
	unknown6D = stream->ReadBool();
	return true;
}

