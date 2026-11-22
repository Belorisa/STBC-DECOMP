/*
===============================================================================

	STMenu.cpp

===============================================================================
*/

#include "STMenu.h"
#include "STSubPane.h"
#include "STStylizedWindow.h"
#include "STTopLevelMenu.h"
#include "STButton.h"
#include "InterfaceModule.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGParagraph.h"
#include "TGRootPane.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "GraphicsModeInfo.h"
#include "PyEmbed.h"
#include "TGIcon.h"
#include "TGUIModule.h"
#include "NiMath.h"
#include "NiPoint2.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005270a0 _$E33
// FUNCTION: STBC 0x005270b0 _$E32
// FUNCTION: STBC 0x00527150 _$E37
// FUNCTION: STBC 0x00527160 _$E36
// FUNCTION: STBC 0x00527d70 _$E48
// FUNCTION: STBC 0x00527d80 _$E47
// FUNCTION: STBC 0x00527d90 _$E52
// FUNCTION: STBC 0x00527da0 _$E51
// FUNCTION: STBC 0x00527db0 _$E56
// FUNCTION: STBC 0x00527dc0 _$E55
// FUNCTION: STBC 0x00527dd0 _$E60
// FUNCTION: STBC 0x00527de0 _$E59
// FUNCTION: STBC 0x00527df0 _$E184
// FUNCTION: STBC 0x00527e00 _$E183
// FUNCTION: STBC 0x00527e10 _$E126
// FUNCTION: STBC 0x00527e20 _$E125
// FUNCTION: STBC 0x00527e30 _$E139
// FUNCTION: STBC 0x00527e40 _$E138
// FUNCTION: STBC 0x00527e50 _$E189
// FUNCTION: STBC 0x00527e60 _$E188
// FUNCTION: STBC 0x00527e70 _$E194
// FUNCTION: STBC 0x00527e80 _$E193
// FUNCTION: STBC 0x00527e90 _$E198
// FUNCTION: STBC 0x00527ea0 _$E197
// FUNCTION: STBC 0x00527eb0 _$E202
// FUNCTION: STBC 0x00527ec0 _$E201
// FUNCTION: STBC 0x00527ed0 _$E209
// FUNCTION: STBC 0x00527ee0 _$E208
// FUNCTION: STBC 0x00527ef0 _$E312
// FUNCTION: STBC 0x00527f00 _$E311
// FUNCTION: STBC 0x00527f10 _$E316
// FUNCTION: STBC 0x00527f20 _$E315
// FUNCTION: STBC 0x00527f30 _$E84
// FUNCTION: STBC 0x00527f40 _$E83
// FUNCTION: STBC 0x00527f50 _$E298
// FUNCTION: STBC 0x00527f60 _$E297
// FUNCTION: STBC 0x00527f70 _$E305
// FUNCTION: STBC 0x00527f80 _$E304
// FUNCTION: STBC 0x00527f90 _$E328
// FUNCTION: STBC 0x00527fa0 _$E327
// FUNCTION: STBC 0x00527fb0 _$E332
// FUNCTION: STBC 0x00527fc0 _$E331
// FUNCTION: STBC 0x00527fd0 _$E372
// FUNCTION: STBC 0x00527fe0 _$E371
// FUNCTION: STBC 0x00527ff0 _$E378
// FUNCTION: STBC 0x00528000 _$E377
// FUNCTION: STBC 0x00528010 _$E320
// FUNCTION: STBC 0x00528020 _$E319
// FUNCTION: STBC 0x00528030 _$E324
// FUNCTION: STBC 0x00528040 _$E323
// FUNCTION: STBC 0x00528050 _$E336
// FUNCTION: STBC 0x00528060 _$E335
// FUNCTION: STBC 0x00528070 _$E340
// FUNCTION: STBC 0x00528080 _$E339
// FUNCTION: STBC 0x00528090 _$E344
// FUNCTION: STBC 0x005280a0 _$E343
// FUNCTION: STBC 0x005280b0 _$E348
// FUNCTION: STBC 0x005280c0 _$E347
// FUNCTION: STBC 0x005280d0 _$E352
// FUNCTION: STBC 0x005280e0 _$E351
// FUNCTION: STBC 0x005280f0 _$E356
// FUNCTION: STBC 0x00528100 _$E355
// FUNCTION: STBC 0x00528110 _$E360
// FUNCTION: STBC 0x00528120 _$E359
// FUNCTION: STBC 0x00528130 _$E364
// FUNCTION: STBC 0x00528140 _$E363
// FUNCTION: STBC 0x00528150 _$E89
// FUNCTION: STBC 0x00528160 _$E88
// FUNCTION: STBC 0x00528170 _$E368
// FUNCTION: STBC 0x00528180 _$E367
// FUNCTION: STBC 0x00528190 _$E94
// FUNCTION: STBC 0x005281a0 _$E93
// FUNCTION: STBC 0x005281b0 _$E382
// FUNCTION: STBC 0x005281c0 _$E381
// FUNCTION: STBC 0x005281d0 _$E386
// FUNCTION: STBC 0x005281e0 _$E385
// FUNCTION: STBC 0x005281f0 _$E390
// FUNCTION: STBC 0x00528200 _$E389
// FUNCTION: STBC 0x00528210 _$E394
// FUNCTION: STBC 0x00528220 _$E393
// FUNCTION: STBC 0x00528230 _$E103
// FUNCTION: STBC 0x00528240 _$E102
// FUNCTION: STBC 0x00528250 _$E398
// FUNCTION: STBC 0x00528260 _$E397
// FUNCTION: STBC 0x00528270 _$E402
// FUNCTION: STBC 0x00528280 _$E401
// FUNCTION: STBC 0x00528290 _$E406
// FUNCTION: STBC 0x005282a0 _$E405
// FUNCTION: STBC 0x005282b0 _$E109
// FUNCTION: STBC 0x005282c0 _$E108
// FUNCTION: STBC 0x005282d0 _$E114
// FUNCTION: STBC 0x005282e0 _$E113
// FUNCTION: STBC 0x005282f0 _$E144
// FUNCTION: STBC 0x00528300 _$E143
// FUNCTION: STBC 0x00528310 _$E410
// FUNCTION: STBC 0x00528320 _$E409
// FUNCTION: STBC 0x00528330 _$E149
// FUNCTION: STBC 0x00528340 _$E148
// FUNCTION: STBC 0x00528350 _$E414
// FUNCTION: STBC 0x00528360 _$E413
// FUNCTION: STBC 0x00528370 _$E418
// FUNCTION: STBC 0x00528380 _$E417
// FUNCTION: STBC 0x00528390 _$E154
// FUNCTION: STBC 0x005283a0 _$E153
// FUNCTION: STBC 0x005283b0 _$E422
// FUNCTION: STBC 0x005283c0 _$E421
// FUNCTION: STBC 0x005283d0 _$E119
// FUNCTION: STBC 0x005283e0 _$E118
// FUNCTION: STBC 0x005283f0 _$E426
// FUNCTION: STBC 0x00528400 _$E425
// FUNCTION: STBC 0x00528410 _$E430
// FUNCTION: STBC 0x00528420 _$E429
// FUNCTION: STBC 0x00528430 _$E159
// FUNCTION: STBC 0x00528440 _$E158
// FUNCTION: STBC 0x00528450 _$E164
// FUNCTION: STBC 0x00528460 _$E163
// FUNCTION: STBC 0x00528470 _$E434
// FUNCTION: STBC 0x00528480 _$E433
// FUNCTION: STBC 0x00528490 _$E438
// FUNCTION: STBC 0x005284a0 _$E437
// FUNCTION: STBC 0x005284e0 _$E442
// FUNCTION: STBC 0x005284f0 _$E441
// FUNCTION: STBC 0x00528500 _$E446
// FUNCTION: STBC 0x00528510 _$E445
// FUNCTION: STBC 0x00528520 _$E450
// FUNCTION: STBC 0x00528530 _$E449
// FUNCTION: STBC 0x00528540 _$E454
// FUNCTION: STBC 0x00528550 _$E453
// FUNCTION: STBC 0x00528560 _$E458
// FUNCTION: STBC 0x00528570 _$E457
// FUNCTION: STBC 0x00528580 _$E462
// FUNCTION: STBC 0x00528590 _$E461
// FUNCTION: STBC 0x005285a0 _$E169
// FUNCTION: STBC 0x005285b0 _$E168
// FUNCTION: STBC 0x005285c0 _$E466
// FUNCTION: STBC 0x005285d0 _$E465
// FUNCTION: STBC 0x005285e0 _$E174
// FUNCTION: STBC 0x005285f0 _$E173
// FUNCTION: STBC 0x00528600 _$E179
// FUNCTION: STBC 0x00528610 _$E178
// FUNCTION: STBC 0x00528620 _$E470
// FUNCTION: STBC 0x00528630 _$E469
// FUNCTION: STBC 0x00528640 _$E475
// FUNCTION: STBC 0x00528650 _$E474
// FUNCTION: STBC 0x00528660 _$E252
// FUNCTION: STBC 0x00528670 _$E251

// DATA: STBC 0x008e2ffc
float DEFAULT_ST_INDENT_HORIZ = 0.003f;
// DATA: STBC 0x008e3000
float DEFAULT_ST_INDENT_VERT = 0.003f;
// DATA: STBC 0x008e3004
float MARKER_INDENT_HORIZ = 0.003f;

// DATA: STBC 0x008e3008
int STMenu::colorStates[2][2][2][2] = {
	{
		{ { STMENU_STATE_DISABLED, STMENU_STATE_DISABLED }, { STMENU_STATE_OPENED_DISABLED, STMENU_STATE_OPENED_DISABLED } },
		{ { STMENU_STATE_NORMAL, STMENU_STATE_HIGHLIGHTED }, { STMENU_STATE_OPENED, STMENU_STATE_OPENED_HIGHLIGHTED } }
	},
	{
		{ { STMENU_STATE_DISABLED, STMENU_STATE_DISABLED }, { STMENU_STATE_OPENED_DISABLED, STMENU_STATE_OPENED_DISABLED } },
		{ { STMENU_STATE_NORMAL, STMENU_STATE_HIGHLIGHTED }, { STMENU_STATE_OPENED, STMENU_STATE_OPENED_HIGHLIGHTED } }
	}
};

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00526760 _$E284
// FUNCTION: STBC 0x00526770 _$E281
// FUNCTION: STBC 0x00526780 _$E283
// FUNCTION: STBC 0x00526790 _$E282
// FUNCTION: STBC 0x005267a0 _$E290
// FUNCTION: STBC 0x005267b0 _$E287
// FUNCTION: STBC 0x005267c0 _$E289
// FUNCTION: STBC 0x005267d0 _$E288

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00525510 _$E481
// FUNCTION: STBC 0x00525520 _$E477
// FUNCTION: STBC 0x00525530 _$E480
// FUNCTION: STBC 0x00525540 _$E478
// DATA: STBC 0x009891a0
TGEventHandlerTable STMenu::classHandlers;

// DATA: STBC 0x00989258
float STMenu::arrowOffset = NI_INFINITY;

/*
================
STMenu::Cast

The object as a menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00525560
STMenu *STMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STMENU_TYPE ) ) {
		return (STMenu *)object;
	}
	return NULL;
}

/*
================
STMenu::STMenu

A closed menu with the name, holding its items in the sub pane, or in a
new expanding sub pane when none is given.
================
*/
// FUNCTION: STBC 0x005255b0 ??0STMenu@@QAE@PAVTGString@@IPAVTGEvent@@PAVSTSubPane@@@Z
STMenu::STMenu( TGString *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane )
	: TGWindow( 0.0f, 0.0f, 0.0f, 0.0f, subPane != NULL ? subPane : STSubPane::Create( 0.0f, 0.0f, true ) ) {
	Init( name, menuFlags, event, true );
}

/*
================
STMenu::STMenu
================
*/
// FUNCTION: STBC 0x00525750 ??0STMenu@@QAE@PADIPAVTGEvent@@PAVSTSubPane@@@Z
STMenu::STMenu( char *name, unsigned int menuFlags, TGEvent *event, STSubPane *subPane )
	: TGWindow( 0.0f, 0.0f, 0.0f, 0.0f, subPane != NULL ? subPane : STSubPane::Create( 0.0f, 0.0f, true ) ) {
	Init( &TGString( name ), menuFlags, event, true );
}

/*
================
STMenu::Init

Takes the menu flags and the first depth's colours, and builds the
children after the sub pane: the title pane with the name, the title
bar, the marker, the two arrows and the end caps, all from the LCARS
group of the current resolution. The menu starts enabled.
================
*/
// FUNCTION: STBC 0x00525820
// PARKED: complete; the flag update keeps menuFlags & 0xF0001000 and flags & ~(OPENED|CLICKED_ONCE) apart before or-ing SIMPLE_RESIZE, where every spelling tried (one expression, separate statements, the bit in the mask) folds into one mask
void STMenu::Init( TGString *name, unsigned int menuFlags, TGEvent *event, bool unused ) {
	SetBatchChildPolys( false );
	activationEvent = NULL;
	SetActivationEvent( event );
	flags = ( flags & ~( STMF_OPENED | STMF_CLICKED_ONCE ) ) | ( menuFlags & STMF_FLAGS_MASK ) | TGWINDOW_SIMPLE_RESIZE;
	uiHeight = -1;
	useUIHeight = true;
	fullTitleBar = false;
	colorScheme = g_kSTMenuColors[0];

	char *iconGroupName = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	TGIcon *marker = new TGIcon( iconGroupName, STMENU_ICON_MARKER );
	TGIcon *leftEndCap = new TGIcon( iconGroupName, STMENU_ICON_LEFT_END_CAP );
	TGIcon *rightEndCap = new TGIcon( iconGroupName, STMENU_ICON_RIGHT_END_CAP );
	TGIcon *openedArrow = new TGIcon( iconGroupName, STMENU_ICON_OPENED_ARROW );
	TGIcon *closedArrow = new TGIcon( iconGroupName, STMENU_ICON_CLOSED_ARROW );
	marker->Resize( leftEndCap->GetWidth() * 0.25f, leftEndCap->GetHeight(), false );

	TGPane *titlePane = new TGPane;
	TGParagraph *text = new TGParagraph( name, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	titlePane->AddChild( text );
	TGPane::InsertChild( 0x7FFFFFFF, titlePane, 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, new TGIcon( iconGroupName, STMENU_ICON_TITLE_BAR ), 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, marker, 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, openedArrow, 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, closedArrow, 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, leftEndCap, 0.0f, 0.0f, false );
	TGPane::InsertChild( 0x7FFFFFFF, rightEndCap, 0.0f, 0.0f, false );
	SetEnabled( false );
}

/*
================
STMenu::~STMenu
================
*/
// FUNCTION: STBC 0x00525b00
STMenu::~STMenu( void ) {
	ClearActivationEvent();
}

/*
================
STMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00525b50
void STMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMenu::MouseHandler, "STMenu::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMenu::KeyboardHandler, "STMenu::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMenu::HandleSelectOption, "STMenu::HandleSelectOption" );
}

/*
================
STMenu::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00525b90
void STMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "STMenu::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STMenu::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_PRE_SELECT_OPTION, "STMenu::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "STMenu::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_CLOSE_MENU, "STMenu::HandleSelectOption" );
}

/*
================
STMenu::GetContainingWindow

The nearest stylized window the menu sits in; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00525c00
STStylizedWindow *STMenu::GetContainingWindow( void ) {
	STStylizedWindow *window = NULL;
	for ( TGPane *pane = parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->GetParent() ) {
		window = STStylizedWindow::Cast( pane );
		if ( window != NULL ) {
			break;
		}
	}
	return window;
}

/*
================
STMenu::GetArrowOffsetX

The offset of the menu's arrows (STMENU_ARROW_OFFSET_X in the LCARS module
of the current resolution), read on the first call and when reload is
set.
================
*/
// FUNCTION: STBC 0x00525c30
float STMenu::GetArrowOffsetX( bool reload ) {
	if ( arrowOffset == NI_INFINITY || reload == true ) {
		if ( PyEmbed::GetValue( GraphicsModeInfo::GetCurrentMode()->GetLcarsModule(), "STMENU_ARROW_OFFSET_X", "f", &arrowOffset ) == -1 ) {
			PyErr_Print();
		}
	}
	return arrowOffset;
}

/*
================
STMenu::Layout

Takes the colour scheme of the menu's depth when it changed, and lays
out a visible menu.
================
*/
// FUNCTION: STBC 0x00525c90
void STMenu::Layout( void ) {
	if ( !IsCompletelyVisible() ) {
		return;
	}
	if ( useUIHeight == true ) {
		int height = GetUIHeight();
		if ( height >= STMENU_NUM_UI_HEIGHTS ) {
			height = STMENU_NUM_UI_HEIGHTS - 1;
		}
		if ( uiHeight != height ) {
			uiHeight = height;
			colorScheme = g_kSTMenuColors[height];
			Unknown5273F0();
		}
	}
	TGPane::Layout();
}

/*
================
STMenu::ForceUpdate
================
*/
// FUNCTION: STBC 0x00525ce0
void STMenu::ForceUpdate( bool layout ) {
	VirtualSlot113();
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::VirtualSlot59

Updates the menu and resizes its sub pane, or itself without one, to
the contents.
================
*/
// FUNCTION: STBC 0x00525d10
void STMenu::VirtualSlot59( void ) {
	TGPane::VirtualSlot59();
	VirtualSlot113();
	VirtualSlot114();
	ResetColorScheme();
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	} else {
		ResizeToContents( true );
	}
}

/*
================
STMenu::InsertChild

Inserts the child into the sub pane and resizes to the contents; an
inserted menu is updated.
================
*/
// FUNCTION: STBC 0x00525d70
void STMenu::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	TGWindow::InsertChild( index, child, x, y, false );
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( layout );
	} else {
		ResizeToContents( layout );
	}
	STMenu *menu = STMenu::Cast( child );
	if ( menu != NULL ) {
		menu->VirtualSlot113();
		menu->VirtualSlot114();
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
STMenu::GetUIHeight
================
*/
// FUNCTION: STBC 0x00525e00
int STMenu::GetUIHeight( void ) {
	STMenu *menu = STMenu::Cast( GetConceptualParent() );
	int height = 0;
	while ( menu != NULL ) {
		height++;
		menu = STMenu::Cast( menu->GetConceptualParent() );
	}
	if ( IsChosen() == true ) {
		height--;
		if ( height < 0 ) {
			return 0;
		}
	}
	return height;
}

/*
================
STMenu::GetDesiredSize

The title row, as wide as its parts and indents and as high as the end
caps; an open menu adds the sub pane below and is at least as wide.
================
*/
// FUNCTION: STBC 0x00525e50
void STMenu::GetDesiredSize( NiPoint2 *size ) {
	// MATCH-PENDING: R17, the end cap's height is named in a single-use local only so its load precedes the size pointer's (the scheduler's tie on input order, tools/c2order.py); no evidence shows the local, and a reference or a pointer copy for size don't give the order
	TGUIObject *text = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	TGUIObject *leftEndCap = TGPane::GetNthChild( STMENU_LEFT_END_CAP );
	// The right end cap is fetched, but the left one is counted twice.
	TGUIObject *rightEndCap = TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
	TGUIObject *marker = TGPane::GetNthChild( STMENU_MARKER );
	float height = leftEndCap->GetHeight();
	size->y = height;
	// the end caps' doubling is a double product: with 2.0f on both doubled terms
	// VC6 factors them into ( width + MARKER_INDENT_HORIZ ) * 2, which retail doesn't
	size->x = leftEndCap->GetWidth() * 2.0 + marker->GetWidth() + text->GetWidth()
			+ MARKER_INDENT_HORIZ * 2.0f + DEFAULT_ST_INDENT_HORIZ * 2.0f + DEFAULT_ST_INDENT_HORIZ;
	if ( IsOpened() == true ) {
		NiPoint2 subPaneSize;
		GetSubPaneDesiredSize( &subPaneSize );
		size->y = DEFAULT_ST_INDENT_VERT + size->y + subPaneSize.y;
		size->x = size->x > subPaneSize.x ? size->x : subPaneSize.x;
	}
}

/*
================
STMenu::GetSubPaneDesiredSize
================
*/
// FUNCTION: STBC 0x00525f20
void STMenu::GetSubPaneDesiredSize( NiPoint2 *size ) {
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	size->x = 0.0f;
	size->y = 0.0f;
	if ( subPane != NULL ) {
		subPane->GetDesiredSize( size );
	}
}

/*
================
STMenu::GetActualHeight

The title bar and the sub pane's desired height, at most the menu's
height.
================
*/
// FUNCTION: STBC 0x00525f60
float STMenu::GetActualHeight( void ) {
	float titleBarHeight = TGPane::GetNthChild( STMENU_TITLE_BAR )->GetHeight();
	NiPoint2 size;
	GetSubPaneDesiredSize( &size );
	float height = size.y + titleBarHeight;
	if ( height > GetHeight() ) {
		height = GetHeight();
	}
	return height;
}

/*
================
STMenu::GetTitleBarHeight
================
*/
// FUNCTION: STBC 0x00525fa0
float STMenu::GetTitleBarHeight( void ) {
	return TGPane::GetNthChild( STMENU_TITLE_BAR )->GetHeight();
}

/*
================
STMenu::GetClientArea

The area below the title bar the sub pane fills: indented by the items'
indent, as wide as the parent (or the menu) less the indent and as high
as the sub pane wants.
================
*/
// FUNCTION: STBC 0x00525fb0
// PARKED: complete; retail copies the left and top inside each branch and loads the parent before the sub pane size call; the parent local and Set in each branch tried
TGRect STMenu::GetClientArea( void ) {
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	float left = Unknown5260B0();
	TGRect area;
	float top = titleBar->GetHeight() + DEFAULT_ST_INDENT_VERT;
	TGPane *parentPane = GetParent();
	NiPoint2 size;
	GetSubPaneDesiredSize( &size );
	if ( parentPane != NULL ) {
		area.Set( left, top, parentPane->GetWidth() - left, size.y );
	} else {
		area.Set( left, top, GetWidth() - left, size.y );
	}
	return area;
}

/*
================
STMenu::GetDesiredHeight
================
*/
// FUNCTION: STBC 0x00526090
float STMenu::GetDesiredHeight( void ) {
	NiPoint2 size;
	GetDesiredSize( &size );
	return size.y;
}

/*
================
STMenu::Unknown5260B0

A third of the left end cap's width, rounded to whole pixels.
================
*/
// FUNCTION: STBC 0x005260b0
float STMenu::Unknown5260B0( void ) {
	return TGUIModule::PixelAlignValue( TGPane::GetNthChild( STMENU_LEFT_END_CAP )->GetWidth() / 3.0f );
}

/*
================
STMenu::VirtualSlot113

Sizes the title row to the name: the end caps and arrows keep their
artwork's proportions at the name's height plus the indents, the title
bar fills the parent's width between the end caps, and the menu is as
wide as the parent. A closed menu is as high as its title bar and shows
the closed arrow when it can open; an open one takes its desired height,
fits the sub pane to the client area and shows the opened arrow when it
can close.
================
*/
// FUNCTION: STBC 0x00526120
// PARKED: complete; residue is FPU stack scheduling of the height and scale temporaries
void STMenu::VirtualSlot113( void ) {
	TGPane *subPane = GetSubPane();
	TGUIObject *text = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	TGIcon *leftEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_LEFT_END_CAP );
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	TGIcon *rightEndCap = (TGIcon *)TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
	TGUIObject *marker = TGPane::GetNthChild( STMENU_MARKER );
	TGPane *titlePane = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) );
	TGIcon *openedArrow = (TGIcon *)TGPane::GetNthChild( STMENU_OPENED_ARROW );
	TGIcon *closedArrow = (TGIcon *)TGPane::GetNthChild( STMENU_CLOSED_ARROW );

	float height = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
	leftEndCap->SizeToArtwork( false );
	float leftScale = height / leftEndCap->GetHeight();
	rightEndCap->SizeToArtwork( false );
	float rightScale = height / rightEndCap->GetHeight();
	leftEndCap->Resize( leftEndCap->GetWidth() * leftScale, height, false );
	rightEndCap->Resize( rightEndCap->GetWidth() * rightScale, height, false );
	marker->Resize( leftEndCap->GetWidth() * 0.25f, height, false );
	openedArrow->SizeToArtwork( false );
	openedArrow->Resize( openedArrow->GetWidth() * leftScale, openedArrow->GetHeight() * leftScale, false );
	closedArrow->SizeToArtwork( false );
	closedArrow->Resize( closedArrow->GetWidth() * leftScale, closedArrow->GetHeight() * leftScale, false );

	if ( parent == NULL ) {
		return;
	}
	float parentWidth = parent->GetWidth();
	float titleBarWidth = parentWidth - leftEndCap->GetWidth() - MARKER_INDENT_HORIZ * 2.0f - marker->GetWidth() - rightEndCap->GetWidth();
	titleBar->Resize( titleBarWidth, height, false );
	titlePane->Resize( ( IsUseEndCapSpace() == true ? rightEndCap->GetWidth() + titleBarWidth : titleBarWidth ) - DEFAULT_ST_INDENT_HORIZ, height, false );
	if ( !IsOpened() ) {
		TGUIObject::Resize( parentWidth, titleBar->GetHeight(), false );
		openedArrow->SetNotVisible( false );
		if ( IsOpenable() == true && GetNumChildren() > 0 ) {
			closedArrow->SetVisible( false );
		}
	} else {
		TGRect clientArea = GetClientArea();
		NiPoint2 size;
		GetDesiredSize( &size );
		TGUIObject::Resize( parentWidth, size.y, false );
		if ( STTopLevelMenu::Cast( this ) == NULL ) {
			subPane->Resize( GetWidth() - Unknown5260B0(), clientArea.GetHeight(), false );
		} else {
			subPane->Resize( GetWidth(), clientArea.GetHeight(), false );
		}
		if ( IsCloseable() == true && GetNumChildren() > 0 ) {
			openedArrow->SetVisible( false );
		}
		closedArrow->SetNotVisible( false );
	}
}

/*
================
STMenu::VirtualSlot114

Places the title row left to right (end cap, marker, title bar, end
cap), the name in the title bar, the arrows over the left end cap and
the sub pane in the client area, and shows the marker while the menu or
the focus of its window is in the true focus path. An empty menu shows
no arrow; the sub pane shows while the menu is open.
================
*/
// FUNCTION: STBC 0x00526480
// PARKED: complete; residue is FPU scheduling of the title pane and arrow positions (GetRight and GetBottom close the first half)
void STMenu::VirtualSlot114( void ) {
	if ( parent == NULL ) {
		return;
	}
	TGPane *subPane = GetSubPane();
	TGUIObject *text = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	TGUIObject *leftEndCap = TGPane::GetNthChild( STMENU_LEFT_END_CAP );
	TGUIObject *titleBar = TGPane::GetNthChild( STMENU_TITLE_BAR );
	TGUIObject *rightEndCap = TGPane::GetNthChild( STMENU_RIGHT_END_CAP );
	TGIcon *marker = (TGIcon *)TGPane::GetNthChild( STMENU_MARKER );
	TGPane *titlePane = TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) );
	TGUIObject *openedArrow = TGPane::GetNthChild( STMENU_OPENED_ARROW );
	TGUIObject *closedArrow = TGPane::GetNthChild( STMENU_CLOSED_ARROW );

	leftEndCap->SetPosition( 0.0f, 0.0f, false );
	marker->SetPosition( leftEndCap->GetRight() + MARKER_INDENT_HORIZ, 0.0f, false );
	titleBar->SetPosition( marker->GetRight() + MARKER_INDENT_HORIZ, 0.0f, false );
	rightEndCap->SetPosition( titleBar->GetRight(), 0.0f, false );
	titlePane->SetPosition( titleBar->GetLeft() + DEFAULT_ST_INDENT_HORIZ, ( titleBar->GetHeight() - text->GetHeight() ) * 0.5f + titleBar->GetTop(), false );
	float arrowOffset = GetArrowOffsetX( false );
	openedArrow->SetPosition( leftEndCap->GetWidth() * arrowOffset + leftEndCap->GetLeft(),
			( leftEndCap->GetBottom() + leftEndCap->GetTop() - openedArrow->GetHeight() ) * 0.5f, false );
	closedArrow->SetPosition( openedArrow->GetLeft(), openedArrow->GetTop(), false );

	TGRect clientArea = GetClientArea();
	// Retail copies the top through fld/fstp into its argument slot and the
	// left with integer moves: the copy level of this named local (the same in
	// STMenu::VirtualSlot114 and STTopLevelMenu::VirtualSlot114).
	float top = clientArea.top;
	subPane->SetPosition( clientArea.left, top, false );
	leftEndCap->SetVisible( false );
	rightEndCap->SetVisible( false );
	if ( IsInTrueFocusPath() ) {
		marker->SetVisible( true );
		marker->SetColor( &g_kSTButtonMarkerDefault );
	} else {
		STStylizedWindow *window = GetContainingWindow();
		TGUIObject *focus;
		if ( window != NULL && !window->IsInTrueFocusPath() && ( focus = window->GetFocusLeaf() ) != NULL
				&& ( focus->HasAncestor( this ) || focus == this ) ) {
			marker->SetColor( &g_kSTButtonMarkerGray );
			marker->SetVisible( true );
		} else {
			marker->SetNotVisible( false );
		}
	}
	if ( GetNumChildren() == 0 ) {
		openedArrow->SetNotVisible( false );
		closedArrow->SetNotVisible( false );
	}
	if ( IsOpened() == true ) {
		subPane->SetVisible( false );
	} else {
		subPane->SetNotVisible( false );
	}
}

/*
================
STMenu::Resize

Resizes the menu, the sub pane to its width (less the items' indent but
in a top level menu), the buttons in the menu to the sub pane's width,
and updates the menus in it.
================
*/
// FUNCTION: STBC 0x005267e0
// PARKED: complete; retail pushes the sub pane height before calling the indent, the compiler spills it first
void STMenu::Resize( float width, float height, bool layout ) {
	TGWindow::Resize( width, height, false );
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		if ( STTopLevelMenu::Cast( this ) == NULL ) {
			subPane->Resize( width - Unknown5260B0(), subPane->GetHeight(), false );
		} else {
			subPane->Resize( width, subPane->GetHeight(), false );
		}
	}
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *child = GetNthChild( i );
		STMenu *menu = STMenu::Cast( child );
		STButton *button = STButton::Cast( child );
		if ( menu != NULL ) {
			menu->VirtualSlot113();
			menu->VirtualSlot114();
		}
		if ( button != NULL ) {
			button->Resize( subPane->GetWidth(), button->GetHeight(), false );
		}
	}
	if ( layout == true ) {
		STStylizedWindow *window = GetContainingWindow();
		if ( window != NULL ) {
			window->InteriorChangedSize( false );
			window->Layout();
		} else {
			Layout();
		}
	}
}

/*
================
STMenu::ResizeToContents

Takes the desired height, as wide as the interior of the window the
menu sits in at its maximum size, or as the sub pane holding it. With
layout set, the menus and sub panes holding it are fitted again, and
the window is laid out (and told when the menu's size changed).
================
*/
// FUNCTION: STBC 0x00526910
void STMenu::ResizeToContents( bool layout ) {
	float oldWidth = GetWidth();
	float oldHeight = GetHeight();
	float width = GetWidth();
	STStylizedWindow *window = GetContainingWindow();
	STSubPane *parentSubPane = STSubPane::Cast( GetParent() );
	if ( window != NULL ) {
		width = window->GetMaximumInteriorWidth();
	}
	if ( parentSubPane != NULL ) {
		width = parentSubPane->GetWidth();
	}
	NiPoint2 size;
	GetDesiredSize( &size );
	Resize( width, size.y, false );
	if ( layout != true ) {
		return;
	}
	for ( TGPane *pane = parent; pane != NULL && pane != window; pane = pane->GetParent() ) {
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
	}
	if ( window != NULL ) {
		if ( GetWidth() != oldWidth || GetHeight() != oldHeight ) {
			window->Unknown5314F0();
		}
		window->InteriorChangedSize( false );
		window->Layout();
	}
}

/*
================
STMenu::SetName

Names the menu's title.
================
*/
// FUNCTION: STBC 0x00526a50
void STMenu::SetName( TGString *name, bool layout ) {
	TGParagraph *text = (TGParagraph *)TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	if ( text != NULL ) {
		text->SetString( name, layout );
		if ( !layout ) {
			text->RecalcBounds( false );
		}
	}
}

/*
================
STMenu::GetName
================
*/
// FUNCTION: STBC 0x00526aa0
void STMenu::GetName( TGString *name ) {
	TGParagraph *text = (TGParagraph *)TGPane::Cast( TGPane::GetNthChild( STMENU_TITLE_PANE ) )->GetFirstChild();
	if ( text != NULL ) {
		text->GetString( name );
	}
}

/*
================
STMenu::GetMenuDepth
================
*/
// FUNCTION: STBC 0x00526ad0
int STMenu::GetMenuDepth( void ) {
	int depth = 0;
	TGPane *pane = parent;
	while ( pane != NULL && pane != UI_NO_PARENT ) {
		STMenu *menu = STMenu::Cast( pane );
		if ( menu != NULL && STTopLevelMenu::Cast( menu ) == NULL ) {
			depth++;
		}
		pane = pane->GetParent();
	}
	return depth;
}

/*
================
STMenu::SetActivationEvent
================
*/
// FUNCTION: STBC 0x00526b10
void STMenu::SetActivationEvent( TGEvent *event ) {
	if ( event != NULL ) {
		event->IncRefCount();
	}
	ClearActivationEvent();
	activationEvent = event;
}

/*
================
STMenu::ClearActivationEvent
================
*/
// FUNCTION: STBC 0x00526b40
void STMenu::ClearActivationEvent( void ) {
	if ( activationEvent != NULL ) {
		activationEvent->DecRefCount();
		activationEvent = NULL;
	}
}

/*
================
STMenu::SendActivationEvent

Posts a copy of the activation event.
================
*/
// FUNCTION: STBC 0x00526b60
void STMenu::SendActivationEvent( void ) {
	if ( activationEvent != NULL ) {
		g_kEventManager.AddEvent( activationEvent->Duplicate() );
	}
}

/*
================
STMenu::GetSubmenuW
================
*/
// FUNCTION: STBC 0x00526b80
STMenu *STMenu::GetSubmenuW( TGString *name ) {
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		return subPane->GetSubmenuW( name );
	}
	return NULL;
}

/*
================
STMenu::GetButtonW
================
*/
// FUNCTION: STBC 0x00526bb0
STButton *STMenu::GetButtonW( TGString *name ) {
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		return subPane->GetButtonW( name );
	}
	return NULL;
}

/*
================
STMenu::RemoveItemW
================
*/
// FUNCTION: STBC 0x00526be0
void STMenu::RemoveItemW( TGString *name ) {
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->RemoveItemW( name );
	}
}

/*
================
STMenu::Open

Opens an openable menu: plays the sound, focuses and fits the sub pane,
updates the menu and the menus it holds, and lays out the window the
menu sits in (or its parent, or itself).
================
*/
// FUNCTION: STBC 0x00526c10
void STMenu::Open( bool layout ) {
	if ( IsOpened() == true || !IsOpenable() ) {
		return;
	}
	TGSound *sound = g_kSoundManager.GetSound( "UIMenuOpened" );
	if ( sound != NULL ) {
		sound->Play( true );
	}
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	SetOpened();
	TGPane::SetFocus( subPane, false );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	}
	ResizeToContents( true );
	VirtualSlot113();
	VirtualSlot114();
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		STMenu *menu = STMenu::Cast( GetNthChild( i ) );
		if ( menu != NULL ) {
			menu->VirtualSlot113();
			menu->VirtualSlot114();
		}
	}
	STStylizedWindow *window = GetContainingWindow();
	if ( window != NULL ) {
		window->Unknown531730( this );
	}
	if ( layout == true ) {
		if ( window != NULL ) {
			window->InteriorChangedSize( false );
			window->Layout();
		} else if ( parent != NULL ) {
			parent->Layout();
		} else {
			Layout();
		}
	}
}

/*
================
STMenu::Close

Closes a closeable open menu and the open menu it has the focus on.
================
*/
// FUNCTION: STBC 0x00526d50
void STMenu::Close( bool layout ) {
	if ( !IsOpened() || !IsCloseable() ) {
		return;
	}
	TGSound *sound = g_kSoundManager.GetSound( "UIMenuClosed" );
	if ( sound != NULL ) {
		sound->Play( true );
	}
	ClearClickedOnce();
	STMenu *menu = STMenu::Cast( GetFocus() );
	if ( menu != NULL ) {
		menu->Close( false );
	}
	ClearOpened();
	STSubPane *subPane = STSubPane::Cast( GetSubPane() );
	if ( subPane != NULL ) {
		subPane->ResizeToContents( true );
	}
	ResizeToContents( true );
	VirtualSlot113();
	VirtualSlot114();
	if ( layout ) {
		STStylizedWindow *window = GetContainingWindow();
		if ( window != NULL ) {
			window->Layout();
		} else if ( parent != NULL ) {
			parent->Layout();
		} else {
			Layout();
		}
	}
}

/*
================
STMenu::SetSelected
================
*/
// FUNCTION: STBC 0x00526e50
void STMenu::SetSelected( bool layout ) {
	if ( IsSelected() ) {
		return;
	}
	TGWindow::SetSelected( false );
	Unknown5273F0();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::SetNotSelected
================
*/
// FUNCTION: STBC 0x00526e80
void STMenu::SetNotSelected( bool layout ) {
	TGWindow::SetNotSelected( false );
	Unknown5273F0();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::SetHighlighted
================
*/
// FUNCTION: STBC 0x00526eb0
void STMenu::SetHighlighted( bool layout ) {
	if ( !IsHighlighted() ) {
		TGWindow::SetHighlighted( false );
		Unknown5273F0();
		g_kInterfaceModule.playClickSound = true;
	}
}

/*
================
STMenu::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x00526ee0
void STMenu::SetNotHighlighted( bool layout ) {
	if ( IsHighlighted() ) {
		TGWindow::SetNotHighlighted( false );
		Unknown5273F0();
	}
}

/*
================
STMenu::SetEnabled
================
*/
// FUNCTION: STBC 0x00526f00
void STMenu::SetEnabled( bool layout ) {
	if ( IsEnabled() ) {
		return;
	}
	TGWindow::SetEnabled( false );
	Unknown5273F0();
	if ( layout ) {
		Layout();
	}
}

/*
================
STMenu::SetDisabled

A disabled menu lets go of the mouse.
================
*/
// FUNCTION: STBC 0x00526f30
void STMenu::SetDisabled( bool layout ) {
	if ( !IsEnabled() ) {
		return;
	}
	if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
		g_kRootWindow.ReleaseMouse( this );
	}
	TGWindow::SetDisabled( false );
	Unknown5273F0();
	if ( layout ) {
		Layout();
	}
}

/*
================
STMenu::SetChosen

Chooses or unchooses the menu. In a radio group, choosing it unchooses
the other buttons and menus of the group.
================
*/
// FUNCTION: STBC 0x00526f80
void STMenu::SetChosen( bool chosen, bool layout ) {
	if ( chosen == true ) {
		flags |= STMF_CHOSEN;
		STSubPane *subPane = STSubPane::Cast( GetParent() );
		if ( subPane != NULL && ( subPane->GetFlags() & STSPF_RADIO_GROUP ) ) {
			for ( TGUIObject *child = subPane->GetFirstChild(); child != NULL; child = subPane->GetNextChild( child ) ) {
				STButton *button = STButton::Cast( child );
				if ( button != NULL ) {
					button->SetChosen( false, false );
				}
				STMenu *menu = STMenu::Cast( child );
				if ( menu != NULL && menu != this ) {
					menu->SetChosen( false, false );
				}
			}
		}
	} else {
		flags &= ~STMF_CHOSEN;
	}
	ResetColorScheme();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STMenu::GetFocus

The sub pane's focus while the menu is open; NULL when closed.
================
*/
// FUNCTION: STBC 0x00527040
TGUIObject *STMenu::GetFocus( void ) {
	if ( IsOpened() ) {
		TGPane *subPane = GetSubPane();
		if ( subPane != NULL ) {
			return subPane->GetFocus();
		}
	}
	return NULL;
}

/*
================
STMenu::SetFocus
================
*/
// FUNCTION: STBC 0x00527070
void STMenu::SetFocus( TGUIObject *child, bool layout ) {
	TGWindow::SetFocus( child, false );
	VirtualSlot114();
	if ( layout == true ) {
		Layout();
	}
}
