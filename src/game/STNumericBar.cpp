/*
===============================================================================

	STNumericBar.cpp

===============================================================================
*/

#include "STNumericBar.h"
#include "STRoundedButton.h"
#include "STSubPane.h"
#include "STMenu.h"
#include "EngPowerCtrl.h"
#include "GraphicsModeInfo.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "STColors.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGBoolEvent.h"
#include "TGFloatEvent.h"
#include "TGFrame.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGIEvent.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGRect.h"
#include "TGRootPane.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGStream.h"
#include "NiPoint2.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0052bd30 _$E33
// FUNCTION: STBC 0x0052bd40 _$E32
// FUNCTION: STBC 0x0052bd50 _$E37
// FUNCTION: STBC 0x0052bd60 _$E36
// FUNCTION: STBC 0x0052bd70 _$E48
// FUNCTION: STBC 0x0052bd80 _$E47
// FUNCTION: STBC 0x0052bd90 _$E52
// FUNCTION: STBC 0x0052bda0 _$E51
// FUNCTION: STBC 0x0052bdb0 _$E56
// FUNCTION: STBC 0x0052bdc0 _$E55
// FUNCTION: STBC 0x0052bdd0 _$E60
// FUNCTION: STBC 0x0052bde0 _$E59
// FUNCTION: STBC 0x0052bdf0 _$E184
// FUNCTION: STBC 0x0052be00 _$E183
// FUNCTION: STBC 0x0052be10 _$E126
// FUNCTION: STBC 0x0052be20 _$E125
// FUNCTION: STBC 0x0052be30 _$E139
// FUNCTION: STBC 0x0052be40 _$E138
// FUNCTION: STBC 0x0052be50 _$E189
// FUNCTION: STBC 0x0052be60 _$E188
// FUNCTION: STBC 0x0052be70 _$E194
// FUNCTION: STBC 0x0052be80 _$E193
// FUNCTION: STBC 0x0052be90 _$E198
// FUNCTION: STBC 0x0052bea0 _$E197
// FUNCTION: STBC 0x0052beb0 _$E202
// FUNCTION: STBC 0x0052bec0 _$E201
// FUNCTION: STBC 0x0052bed0 _$E209
// FUNCTION: STBC 0x0052bee0 _$E208
// FUNCTION: STBC 0x0052bef0 _$E293
// FUNCTION: STBC 0x0052bf00 _$E292
// FUNCTION: STBC 0x0052bf10 _$E297
// FUNCTION: STBC 0x0052bf20 _$E296
// FUNCTION: STBC 0x0052bf30 _$E84
// FUNCTION: STBC 0x0052bf40 _$E83
// FUNCTION: STBC 0x0052bf50 _$E279
// FUNCTION: STBC 0x0052bf60 _$E278
// FUNCTION: STBC 0x0052bf70 _$E286
// FUNCTION: STBC 0x0052bf80 _$E285
// FUNCTION: STBC 0x0052bf90 _$E309
// FUNCTION: STBC 0x0052bfa0 _$E308
// FUNCTION: STBC 0x0052bfb0 _$E313
// FUNCTION: STBC 0x0052bfc0 _$E312
// FUNCTION: STBC 0x0052bfd0 _$E353
// FUNCTION: STBC 0x0052bfe0 _$E352
// FUNCTION: STBC 0x0052bff0 _$E359
// FUNCTION: STBC 0x0052c000 _$E358
// FUNCTION: STBC 0x0052c010 _$E301
// FUNCTION: STBC 0x0052c020 _$E300
// FUNCTION: STBC 0x0052c030 _$E305
// FUNCTION: STBC 0x0052c040 _$E304
// FUNCTION: STBC 0x0052c050 _$E317
// FUNCTION: STBC 0x0052c060 _$E316
// FUNCTION: STBC 0x0052c070 _$E321
// FUNCTION: STBC 0x0052c080 _$E320
// FUNCTION: STBC 0x0052c090 _$E325
// FUNCTION: STBC 0x0052c0a0 _$E324
// FUNCTION: STBC 0x0052c0b0 _$E329
// FUNCTION: STBC 0x0052c0c0 _$E328
// FUNCTION: STBC 0x0052c0d0 _$E333
// FUNCTION: STBC 0x0052c0e0 _$E332
// FUNCTION: STBC 0x0052c0f0 _$E337
// FUNCTION: STBC 0x0052c100 _$E336
// FUNCTION: STBC 0x0052c110 _$E341
// FUNCTION: STBC 0x0052c120 _$E340
// FUNCTION: STBC 0x0052c130 _$E345
// FUNCTION: STBC 0x0052c140 _$E344
// FUNCTION: STBC 0x0052c150 _$E89
// FUNCTION: STBC 0x0052c160 _$E88
// FUNCTION: STBC 0x0052c170 _$E349
// FUNCTION: STBC 0x0052c180 _$E348
// FUNCTION: STBC 0x0052c190 _$E94
// FUNCTION: STBC 0x0052c1a0 _$E93
// FUNCTION: STBC 0x0052c1b0 _$E363
// FUNCTION: STBC 0x0052c1c0 _$E362
// FUNCTION: STBC 0x0052c1d0 _$E367
// FUNCTION: STBC 0x0052c1e0 _$E366
// FUNCTION: STBC 0x0052c1f0 _$E371
// FUNCTION: STBC 0x0052c200 _$E370
// FUNCTION: STBC 0x0052c210 _$E375
// FUNCTION: STBC 0x0052c220 _$E374
// FUNCTION: STBC 0x0052c230 _$E103
// FUNCTION: STBC 0x0052c240 _$E102
// FUNCTION: STBC 0x0052c250 _$E379
// FUNCTION: STBC 0x0052c260 _$E378
// FUNCTION: STBC 0x0052c270 _$E383
// FUNCTION: STBC 0x0052c280 _$E382
// FUNCTION: STBC 0x0052c290 _$E387
// FUNCTION: STBC 0x0052c2a0 _$E386
// FUNCTION: STBC 0x0052c2b0 _$E109
// FUNCTION: STBC 0x0052c2c0 _$E108
// FUNCTION: STBC 0x0052c2d0 _$E114
// FUNCTION: STBC 0x0052c2e0 _$E113
// FUNCTION: STBC 0x0052c2f0 _$E144
// FUNCTION: STBC 0x0052c300 _$E143
// FUNCTION: STBC 0x0052c340 _$E391
// FUNCTION: STBC 0x0052c350 _$E390
// FUNCTION: STBC 0x0052c360 _$E149
// FUNCTION: STBC 0x0052c370 _$E148
// FUNCTION: STBC 0x0052c380 _$E395
// FUNCTION: STBC 0x0052c390 _$E394
// FUNCTION: STBC 0x0052c3a0 _$E399
// FUNCTION: STBC 0x0052c3b0 _$E398
// FUNCTION: STBC 0x0052c3c0 _$E154
// FUNCTION: STBC 0x0052c3d0 _$E153
// FUNCTION: STBC 0x0052c3e0 _$E403
// FUNCTION: STBC 0x0052c3f0 _$E402
// FUNCTION: STBC 0x0052c400 _$E119
// FUNCTION: STBC 0x0052c410 _$E118
// FUNCTION: STBC 0x0052c420 _$E407
// FUNCTION: STBC 0x0052c430 _$E406
// FUNCTION: STBC 0x0052c440 _$E411
// FUNCTION: STBC 0x0052c450 _$E410
// FUNCTION: STBC 0x0052c460 _$E159
// FUNCTION: STBC 0x0052c470 _$E158
// FUNCTION: STBC 0x0052c480 _$E164
// FUNCTION: STBC 0x0052c490 _$E163
// FUNCTION: STBC 0x0052c4a0 _$E415
// FUNCTION: STBC 0x0052c4b0 _$E414
// FUNCTION: STBC 0x0052c4c0 _$E419
// FUNCTION: STBC 0x0052c4d0 _$E418
// FUNCTION: STBC 0x0052c4e0 _$E423
// FUNCTION: STBC 0x0052c4f0 _$E422
// FUNCTION: STBC 0x0052c500 _$E427
// FUNCTION: STBC 0x0052c510 _$E426
// FUNCTION: STBC 0x0052c520 _$E431
// FUNCTION: STBC 0x0052c530 _$E430
// FUNCTION: STBC 0x0052c540 _$E435
// FUNCTION: STBC 0x0052c550 _$E434
// FUNCTION: STBC 0x0052c560 _$E439
// FUNCTION: STBC 0x0052c570 _$E438
// FUNCTION: STBC 0x0052c580 _$E443
// FUNCTION: STBC 0x0052c590 _$E442
// FUNCTION: STBC 0x0052c5a0 _$E169
// FUNCTION: STBC 0x0052c5b0 _$E168
// FUNCTION: STBC 0x0052c5c0 _$E447
// FUNCTION: STBC 0x0052c5d0 _$E446
// FUNCTION: STBC 0x0052c5e0 _$E174
// FUNCTION: STBC 0x0052c5f0 _$E173
// FUNCTION: STBC 0x0052c600 _$E179
// FUNCTION: STBC 0x0052c610 _$E178
// FUNCTION: STBC 0x0052c640 _$E451
// FUNCTION: STBC 0x0052c650 _$E450
// FUNCTION: STBC 0x0052c660 _$E456
// FUNCTION: STBC 0x0052c670 _$E455
// FUNCTION: STBC 0x0052c680 _$E257
// FUNCTION: STBC 0x0052c690 _$E256

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0052bcb0 _$E265
// FUNCTION: STBC 0x0052bcc0 _$E262
// FUNCTION: STBC 0x0052bcd0 _$E264
// FUNCTION: STBC 0x0052bce0 _$E263
// FUNCTION: STBC 0x0052bcf0 _$E271
// FUNCTION: STBC 0x0052bd00 _$E268
// FUNCTION: STBC 0x0052bd10 _$E270
// FUNCTION: STBC 0x0052bd20 _$E269

// A click snaps to the nearest twentieth of a marker step (invented name): counted
// in twentieths, and the count divided by 20 (retail's reciprocal copy of 0.05f, 0x88C288).
#define SNAP_STEP	0.05f

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00529b10 _$E471
// FUNCTION: STBC 0x00529b30 _$E470
// FUNCTION: STBC 0x00529e80 _$E462
// FUNCTION: STBC 0x00529e90 _$E458
// FUNCTION: STBC 0x00529ea0 _$E461
// FUNCTION: STBC 0x00529eb0 _$E459

// The icons of the bar's end while the bar is in the true focus path or not.
#define STNUMERICBAR_ICON_BAR_END_FOCUSED		0x7DB
#define STNUMERICBAR_ICON_BAR_END				0x7DA
#define STNUMERICBAR_ICON_MARKER				0x7E4
#define STNUMERICBAR_ICON_BAR					0xC8
#define STNUMERICBAR_ICON_FRAME					0x7D0

// The icon of the arrow buttons whose size the bar follows.
#define STNUMERICBAR_ICON_ARROW					0xBBA

// DATA: STBC 0x00989490
TGEventHandlerTable STNumericBar::classHandlers;

/*
================
STNumericBar::Cast

The object as a numeric bar; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00529ed0
STNumericBar *STNumericBar::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STNUMERICBAR_TYPE ) ) {
		return (STNumericBar *)object;
	}
	return NULL;
}

/*
================
STNumericBar::STNumericBar

Builds the label and the percentage caption, the two arrow buttons, the
end of the bar, the marker, the bar's frame and the bar pane, which holds the
background, filled and empty icons.
================
*/
// FUNCTION: STBC 0x00529f00 ??0STNumericBar@@QAE@MM@Z
STNumericBar::STNumericBar( float width, float height ) : TGPane( width, height ) {
	updateEvent = NULL;
	minValue = 0.0f;
	maxValue = 1.0f;
	value = 0.0f;
	useMarker = true;
	useAlternateColor = false;
	useButtons = true;
	markerValue = 0.5f;
	keyInterval = 0.1f;
	alternateValue = 0.0f;

	char *iconGroupName = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();

	TGParagraph *label = new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	label->SetColor( (NiColorA *)&NiColorA::BLACK );
	TGParagraph *caption = new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	caption->SetColor( (NiColorA *)&NiColorA::BLACK );

	TGIcon *marker = new TGIcon( iconGroupName, STNUMERICBAR_ICON_MARKER, NULL );
	marker->SetColor( (NiColorA *)&NiColorA::WHITE );
	TGIcon *barEnd = new TGIcon( iconGroupName, STNUMERICBAR_ICON_BAR_END, NULL );

	TGPane *barPane = new TGPane;
	barPane->SetBatchChildPolys( false );
	TGIcon *background = new TGIcon( iconGroupName, STNUMERICBAR_ICON_BAR, NULL );
	NiColorA backgroundColor( 0.1f, 0.1f, 0.1f, 0.15f );
	background->SetColor( &backgroundColor );
	TGIcon *filled = new TGIcon( iconGroupName, STNUMERICBAR_ICON_BAR, NULL );
	filled->SetColor( (NiColorA *)&NiColorA::WHITE );
	normalColor = NiColorA::WHITE;
	TGIcon *empty = new TGIcon( iconGroupName, STNUMERICBAR_ICON_BAR, NULL );
	empty->SetColor( (NiColorA *)&NiColorA::BLACK );
	emptyColor = NiColorA::BLACK;
	barPane->AddChild( background, 0.0f, 0.0f, false );
	barPane->AddChild( filled, 0.0f, 0.0f, false );
	barPane->AddChild( empty, 0.0f, 0.0f, false );

	TGBoolEvent *firstEvent = new TGBoolEvent;
	firstEvent->SetSource( this );
	firstEvent->SetDestination( this );
	firstEvent->SetBool( false );
	firstEvent->SetEventType( ET_ST_NUMERIC_BAR_ARROW );
	STRoundedButton *firstArrow = new STRoundedButton( "<", firstEvent, 0.0f, 0.0f, false );
	firstArrow->normalColor = g_kSTButtonColors[1][0];
	firstArrow->highlightedColor = g_kSTButtonColors[1][2];
	firstArrow->selectedColor = g_kSTButtonColors[1][1];
	firstArrow->disabledColor = g_kSTButtonColors[1][3];
	firstArrow->SetColorBasedOnFlags();

	TGBoolEvent *secondEvent = new TGBoolEvent;
	secondEvent->SetSource( this );
	secondEvent->SetDestination( this );
	secondEvent->SetBool( true );
	secondEvent->SetEventType( ET_ST_NUMERIC_BAR_ARROW );
	STRoundedButton *secondArrow = new STRoundedButton( ">", secondEvent, 0.0f, 0.0f, false );
	secondArrow->normalColor = g_kSTButtonColors[1][0];
	secondArrow->highlightedColor = g_kSTButtonColors[1][2];
	secondArrow->selectedColor = g_kSTButtonColors[1][1];
	secondArrow->disabledColor = g_kSTButtonColors[1][3];
	secondArrow->SetColorBasedOnFlags();

	TGFrame *barFrame = new TGFrame( iconGroupName, STNUMERICBAR_ICON_FRAME, &TGRect(), 1.0f, FRAME_STRETCH_TO_FIT );
	barFrame->SetEdgeStretch( 3, false );
	barFrame->SetColor( NiColorA::WHITE );

	AddChild( label, 0.0f, 0.0f, false );
	AddChild( caption, 0.0f, 0.0f, false );
	AddChild( firstArrow, 0.0f, 0.0f, false );
	AddChild( secondArrow, 0.0f, 0.0f, false );
	AddChild( barEnd, 0.0f, 0.0f, false );
	AddChild( marker, 0.0f, 0.0f, false );
	AddChild( barFrame, 0.0f, 0.0f, false );
	AddChild( barPane, 0.0f, 0.0f, false );

	marker->Resize( marker->GetWidth(), GetHeight(), false );
	barEnd->Resize( barEnd->GetWidth(), GetHeight(), false );
	empty->Resize( width, height, false );
	barPane->Resize( width, height, false );
	barFrame->Resize( width, height, false );
	UpdateGauge( false );
	TGUIObject::SetEnabled( false );
	TGPane::SetNotBatchChildPolys( false );
}

/*
================
STNumericBar::~STNumericBar

Releases the update event.
================
*/
// FUNCTION: STBC 0x0052a6d0
STNumericBar::~STNumericBar( void ) {
	SetUpdateEvent( NULL );
}

/*
================
STNumericBar::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0052a730
void STNumericBar::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STNumericBar::HandleKeyboard, "STNumericBar::HandleKeyboard" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STNumericBar::HandleSelectOption, "STNumericBar::HandleSelectOption" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STNumericBar::HandleMouse, "STNumericBar::HandleMouse" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STNumericBar::HandleArrow, "STNumericBar::HandleArrow" );
}

/*
================
STNumericBar::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0052a790
void STNumericBar::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STNumericBar::HandleKeyboard" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "STNumericBar::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_CLOSE_MENU, "STNumericBar::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "STNumericBar::HandleMouse" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_NUMERIC_BAR_ARROW, "STNumericBar::HandleArrow" );
}

/*
================
STNumericBar::RefreshIconGroups

Has the bar's frame and the end of the bar draw from the icon group of the
current resolution.
================
*/
// FUNCTION: STBC 0x0052a800
void STNumericBar::RefreshIconGroups( void ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	TGFrame *barFrame = TGFrame::Cast( GetNthChild( STNUMERICBAR_BAR_FRAME ) );
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	if ( barFrame != NULL ) {
		barFrame->SetIconGroupName( mode->GetLcarsString(), false );
	}
	if ( barEnd != NULL ) {
		barEnd->SetIconGroupName( mode->GetLcarsString(), false );
	}
}

/*
================
STNumericBar::GetMarker
================
*/
// FUNCTION: STBC 0x0052a880
TGIcon *STNumericBar::GetMarker( void ) {
	return TGIcon::Cast( GetNthChild( STNUMERICBAR_MARKER ) );
}

/*
================
STNumericBar::SetDisabled

A bar disabled in the middle of a drag lets the mouse go.
================
*/
// FUNCTION: STBC 0x0052a8a0
void STNumericBar::SetDisabled( bool layout ) {
	if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
		g_kRootWindow.ReleaseMouse( this );
	}
	TGUIObject::SetDisabled( layout );
}

/*
================
STNumericBar::SetValue

Clicks when the value crosses into another step of a visible bar and no
click is playing.
================
*/
// FUNCTION: STBC 0x0052a8d0
void STNumericBar::SetValue( float newValue ) {
	if ( keyInterval != 0.0f && (int)( value / keyInterval ) != (int)( newValue / keyInterval ) && IsCompletelyVisible() ) {
		TGSound *sound = g_kSoundManager.GetSound( "UINumericBarClick" );
		if ( sound != NULL && g_kSoundManager.GetPlayingSound( "UINumericBarClick" ) == NULL ) {
			sound->Play( true );
		}
	}
	value = newValue;
}

/*
================
STNumericBar::SetMarkerValue
================
*/
// FUNCTION: STBC 0x0052a950
void STNumericBar::SetMarkerValue( float markerValue ) {
	this->markerValue = markerValue;
	if ( useMarker == true ) {
		UpdateGauge( true );
	}
}

/*
================
STNumericBar::SetUpdateEvent

Keeps a copy of the event.
================
*/
// FUNCTION: STBC 0x0052a970
void STNumericBar::SetUpdateEvent( TGFloatEvent *event ) {
	if ( updateEvent != NULL ) {
		updateEvent->DecRefCount();
		updateEvent = NULL;
	}
	if ( event != NULL ) {
		updateEvent = (TGFloatEvent *)event->Duplicate();
	}
}

/*
================
STNumericBar::UpdateGauge

Places the bar's icons for the value: the filled part runs from the left
to the value (the part below the marker's value is dimmed apart when the
marker is shown), the end of the bar follows it, the marker sits at its
own value and the caption shows the percentage at the right. A bar in the
true focus path draws its colours brightened.
================
*/
// FUNCTION: STBC 0x0052a9b0
// PARKED: frame and schedule residue: retail's frame is 4 bytes larger (its scratch triple sits in a 16-byte home, as if an NiColorA), its sums load width before left and the second arrow before the first whatever the source order, and the screen height conversion is scheduled after the useButtons load
void STNumericBar::UpdateGauge( bool layout ) {
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	TGIcon *filled = TGIcon::Cast( barPane->GetNthChild( 1 ) );
	TGIcon *empty = TGIcon::Cast( barPane->GetNthChild( 2 ) );
	TGIcon *background = TGIcon::Cast( barPane->GetNthChild( 0 ) );
	TGIcon *marker = GetMarker();
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	STRoundedButton *firstArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_FIRST_ARROW ) );
	STRoundedButton *secondArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_SECOND_ARROW ) );

	float span = GetWidth() - firstArrow->GetWidth() - secondArrow->GetWidth() - barEnd->GetWidth();
	float height = GetHeight();
	float barHeight = height - 1.0f / (float)(unsigned int)mode->GetHeight() * 2.0f;
	if ( !useButtons ) {
		span += firstArrow->GetWidth() + secondArrow->GetWidth();
	}
	float valueOffset = value - minValue;
	float range = maxValue - minValue;
	float fraction = valueOffset / range;
	float fillEnd = fraction * span;

	if ( useButtons == true ) {
		barPane->SetPosition( firstArrow->GetWidth() + firstArrow->GetLeft(), ( height - barHeight ) * 0.5f, false );
	} else {
		barPane->SetPosition( 0.0f, ( height - barHeight ) * 0.5f, false );
	}

	float fillEdge;
	if ( useAlternateColor == true && value > alternateValue ) {
		background->SetVisible( false );
		float backgroundLeft = ( alternateValue - minValue ) / range * span;
		background->Resize( ( value - minValue ) / range * span - backgroundLeft, barPane->GetHeight(), false );
		background->SetPosition( backgroundLeft, 0.0f, false );
		filled->Resize( fillEnd - background->GetWidth(), barPane->GetHeight(), false );
		filled->SetPosition( 0.0f, 0.0f, false );
		fillEdge = background->GetWidth() + background->GetLeft();
	} else {
		background->SetNotVisible( false );
		filled->Resize( fillEnd, barPane->GetHeight(), false );
		filled->SetPosition( 0.0f, 0.0f, false );
		fillEdge = filled->GetWidth() + filled->GetLeft();
	}
	barEnd->SetPosition( barPane->GetLeft() + fillEdge, 0.0f, false );
	marker->SetPosition( ( markerValue - minValue ) / range * span + barPane->GetLeft() - marker->GetWidth() * 0.5f, barPane->GetTop(), false );

	if ( IsInTrueFocusPath() == true ) {
		NiColorA bright;
		NiColor rgb;
		rgb.r = normalColor.r;
		rgb.g = normalColor.g;
		rgb.b = normalColor.b;
		rgb *= 1.2f;
		bright.r = rgb.r;
		bright.g = rgb.g;
		bright.b = rgb.b;
		if ( rgb.r > 1.0f ) {
			bright.r = 1.0f;
		}
		if ( rgb.g > 1.0f ) {
			bright.g = 1.0f;
		}
		if ( rgb.b > 1.0f ) {
			bright.b = 1.0f;
		}
		bright.a = normalColor.a;
		filled->SetColor( &bright );
		rgb.r = emptyColor.r;
		rgb.g = emptyColor.g;
		rgb.b = emptyColor.b;
		rgb *= 1.2f;
		bright.r = rgb.r;
		bright.g = rgb.g;
		bright.b = rgb.b;
		if ( rgb.r > 1.0f ) {
			bright.r = 1.0f;
		}
		if ( rgb.g > 1.0f ) {
			bright.g = 1.0f;
		}
		if ( rgb.b > 1.0f ) {
			bright.b = 1.0f;
		}
		bright.a = emptyColor.a;
		empty->SetColor( &bright );
	} else {
		filled->SetColor( &normalColor );
		empty->SetColor( &emptyColor );
	}

	TGParagraph *caption = TGParagraph::Cast( GetNthChild( STNUMERICBAR_CAPTION ) );
	char text[16];
	if ( useMarker == true ) {
		int percent = (int)( valueOffset / ( markerValue - minValue ) * 100.0f + 0.5f );
		if ( percent == 0 && value > minValue ) {
			percent = 1;
		}
		sprintf( text, "%3d%%", percent );
	} else {
		int percent = (int)( fraction * 100.0f + 0.5f );
		if ( percent == 0 && value > minValue ) {
			percent = 1;
		}
		sprintf( text, "%3d%%", percent );
	}
	caption->SetString( text, true );
	caption->SetPosition( barPane->GetWidth() + barPane->GetLeft() - caption->GetWidth() - DEFAULT_ST_INDENT_HORIZ * 2.0f,
		barPane->GetHeight() * 0.5f + barPane->GetTop() - caption->GetHeight() * 0.5f, false );
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STNumericBar::GotTrueFocusPath
================
*/
// FUNCTION: STBC 0x0052aec0
void STNumericBar::GotTrueFocusPath( bool layout ) {
	TGUIObject::GotTrueFocusPath( false );
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	barEnd->SetIconNum( STNUMERICBAR_ICON_BAR_END_FOCUSED, true );
	UpdateGauge( layout );
}

/*
================
STNumericBar::LostTrueFocusPath
================
*/
// FUNCTION: STBC 0x0052af00
void STNumericBar::LostTrueFocusPath( bool layout ) {
	TGUIObject::LostTrueFocusPath( false );
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	barEnd->SetIconNum( STNUMERICBAR_ICON_BAR_END, true );
	UpdateGauge( layout );
}

/*
================
STNumericBar::SetUseButtons

Shows or hides the arrow buttons.
================
*/
// FUNCTION: STBC 0x0052af40
void STNumericBar::SetUseButtons( bool use ) {
	useButtons = use;
	if ( use == true ) {
		GetNthChild( STNUMERICBAR_FIRST_ARROW )->SetVisible( false );
		GetNthChild( STNUMERICBAR_SECOND_ARROW )->SetVisible( false );
	} else {
		GetNthChild( STNUMERICBAR_FIRST_ARROW )->SetNotVisible( false );
		GetNthChild( STNUMERICBAR_SECOND_ARROW )->SetNotVisible( false );
	}
	Resize( GetWidth(), GetHeight(), false );
	UpdateGauge( true );
}

/*
================
STNumericBar::Resize

Lays the children out for the new size: the arrow buttons at the ends
(sized from the height of the LCARS arrow artwork), the bar frame, the
bar pane and its icons between them, and the label left of the bar.
================
*/
// FUNCTION: STBC 0x0052afd0
// PARKED: register allocation: retail keeps the end icon and the first arrow in registers (ebp, ebx) and spills the second arrow; here both arrows take registers and the end icon is spilled, and retail leaves the icon height divisor on the FPU stack (fld; fdiv st(1); fstp st(0)); declaration order, named locals and the divide's spellings tried
void STNumericBar::Resize( float width, float height, bool layout ) {
	TGPane::Resize( width, height, false );

	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	// BUG: the bar pane's second icon is looked up and never used.
	TGIcon::Cast( barPane->GetNthChild( 1 ) );
	TGIcon *emptyIcon = TGIcon::Cast( barPane->GetNthChild( 2 ) );
	// BUG: the bar pane's first icon is looked up and never used.
	TGIcon::Cast( barPane->GetNthChild( 0 ) );
	TGIcon *marker = GetMarker();
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	STRoundedButton *firstArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_FIRST_ARROW ) );
	STRoundedButton *secondArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_SECOND_ARROW ) );
	TGFrame *barFrame = TGFrame::Cast( GetNthChild( STNUMERICBAR_BAR_FRAME ) );

	float barHeight = height - 1.0f / (float)(unsigned int)mode->GetHeight() * 2.0f;
	emptyIcon->Resize( width, barHeight, false );
	marker->Resize( marker->GetWidth(), barHeight, false );
	barEnd->SizeToArtwork( false );
	barEnd->Resize( barEnd->GetWidth(), height, false );

	// BUG: the first arrow's name paragraph is looked up and never used.
	TGParagraph::Cast( firstArrow->GetNthChild( 0 ) );
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( mode->GetLcarsString() );
	float scale = height / iconGroup->GetIconScreenHeight( STNUMERICBAR_ICON_ARROW );
	float firstWidth = ( iconGroup->GetIconScreenWidth( STNUMERICBAR_ICON_ARROW ) * 1.25f + DEFAULT_ST_INDENT_HORIZ * 2.0f ) * scale;
	float secondWidth = ( iconGroup->GetIconScreenWidth( STNUMERICBAR_ICON_ARROW ) * 1.25f + DEFAULT_ST_INDENT_HORIZ * 2.0f ) * scale;
	float arrowHeight = barHeight * 0.8f;
	firstArrow->Resize( firstWidth, arrowHeight, false );
	secondArrow->Resize( secondWidth, arrowHeight, false );

	float barWidth;
	if ( useButtons == true ) {
		barWidth = width - firstArrow->GetWidth() - secondArrow->GetWidth();
	} else {
		barWidth = width;
	}
	barPane->Resize( barWidth, barHeight, false );
	barFrame->Resize( barWidth, barHeight, false );

	firstArrow->SetPosition( 0.0f, ( height - firstArrow->GetHeight() ) * 0.5f, false );
	secondArrow->SetPosition( width - secondArrow->GetWidth(), ( height - secondArrow->GetHeight() ) * 0.5f, false );
	UpdateGauge( false );
	barFrame->SetPosition( barPane->GetLeft(), barPane->GetTop(), false );

	TGParagraph *label = TGParagraph::Cast( GetNthChild( STNUMERICBAR_LABEL ) );
	label->SetPosition( DEFAULT_ST_INDENT_HORIZ * 4.0f + barPane->GetLeft(), GetHeight() * 0.5f - label->GetHeight() * 0.5f, false );
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STNumericBar::GetPositionFromAbsolutePercentage

Where the bar's marker sits, in the bar's coordinates, for a fraction of
the bar's span.
================
*/
// FUNCTION: STBC 0x0052b310
float STNumericBar::GetPositionFromAbsolutePercentage( float percentage ) {
	STRoundedButton *firstArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_FIRST_ARROW ) );
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	// BUG: the bar pane's third icon is looked up and never used.
	TGIcon::Cast( barPane->GetNthChild( 2 ) );
	float position = ( barPane->GetWidth() - barEnd->GetWidth() ) * percentage;
	if ( useButtons == true ) {
		position += firstArrow->GetWidth() + firstArrow->GetLeft();
	}
	return position;
}

/*
================
STNumericBar::SetNormalColor

Colours the bar's filled part: the first icon of the bar pane.
================
*/
// FUNCTION: STBC 0x0052b390
void STNumericBar::SetNormalColor( NiColorA *color ) {
	normalColor = *color;
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	TGIcon *fill = TGIcon::Cast( barPane->GetNthChild( 1 ) );
	TGIcon *empty = TGIcon::Cast( barPane->GetNthChild( 0 ) );
	fill->SetColor( &normalColor );
	empty->SetColor( color );
}

/*
================
STNumericBar::SetEmptyColor

Colours the bar's empty part: the third icon of the bar pane.
================
*/
// FUNCTION: STBC 0x0052b410
void STNumericBar::SetEmptyColor( NiColorA *color ) {
	emptyColor = *color;
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	TGIcon *empty = TGIcon::Cast( barPane->GetNthChild( 2 ) );
	empty->SetColor( &emptyColor );
}

/*
================
STNumericBar::HandleKeyboard

Runs the key's binding for the bar.
================
*/
// FUNCTION: STBC 0x0052b470
void STNumericBar::HandleKeyboard( TGKeyboardEvent *event ) {
	unsigned short character = event->GetUnicode();	// a 16-bit character: retail masks it at the call
	int keyState = event->GetKeyState();
	if ( !event->EventHandled() ) {
		if ( g_kKeyboardBinding.LaunchEvent( character, keyState, this ) ) {
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
STNumericBar::HandleSelectOption

An enabled bar steps up when its sub pane or menu moves the selection onto
it and down when the menu closes.
================
*/
// FUNCTION: STBC 0x0052b4c0
void STNumericBar::HandleSelectOption( TGIntEvent *event ) {
	if ( event->GetEventType() == ET_INPUT_SELECT_OPTION ) {
		int offset = event->GetInt();
		if ( flags & TGUIO_ENABLED ) {
			if ( offset == 0 ) {
				Step( true );
			} else {
				TGPane *pane = parent;
				if ( pane != NULL && ( pane->IsTypeOf( STSUBPANE_TYPE ) || pane->IsTypeOf( ENGPOWERCTRL_TYPE ) ) ) {
					STSubPane *subPane = STSubPane::Cast( pane );
					if ( subPane != NULL ) {
						subPane->MoveSelection( offset );
					}
				}
			}
		}
	}
	if ( event->GetEventType() == ET_INPUT_CLOSE_MENU ) {
		Step( false );
	}
	CallNextHandler( event );
}

/*
================
STNumericBar::HandleMouse

A click on the bar sets the value from where it lands, snapped to the
nearest twentieth of a marker step when the button goes down; a drag
while the bar holds the mouse follows it. Clicks on the arrows are left
to them.
================
*/
// FUNCTION: STBC 0x0052b550 ?HandleMouse@STNumericBar@@QAEXPAVTGMouseEvent@@@Z
// PARKED: x87: the second arrow offset adds point.y first in retail (the difference's y first here; operator+ of the difference, a named offset, -( second - first ) and component sums don't give it)
void STNumericBar::HandleMouse( TGMouseEvent *event ) {
	STRoundedButton *firstArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_FIRST_ARROW ) );
	STRoundedButton *secondArrow = STRoundedButton::Cast( GetNthChild( STNUMERICBAR_SECOND_ARROW ) );
	TGIcon *barEnd = TGIcon::Cast( GetNthChild( STNUMERICBAR_BAR_END ) );
	TGPane *barPane = TGPane::Cast( GetNthChild( STNUMERICBAR_BAR_PANE ) );
	TGIcon::Cast( barPane->GetNthChild( 2 ) );
	bool grabbed = g_kRootWindow.GetMouseGrabOwner() == this;
	if ( !( flags & TGUIO_VISIBLE ) || !( flags & TGUIO_ENABLED ) ) {
		CallNextHandler( event );
		return;
	}

	NiPoint2 point;
	event->GetPosition( &point );
	if ( useButtons == true ) {
		NiPoint2 firstPosition;
		firstArrow->GetPosition( &firstPosition );
		point -= firstPosition;
		NiPoint2 secondPosition;
		secondArrow->GetPosition( &secondPosition );
		if ( point.x >= 0.0f && point.x <= firstArrow->GetWidth() && point.y >= 0.0f && point.y <= firstArrow->GetHeight() && !grabbed ) {
			CallNextHandler( event );
			event->SetHandled();
			return;
		}
		point += firstPosition - secondPosition;
		if ( point.x >= 0.0f && point.x <= secondArrow->GetWidth() && point.y >= 0.0f && point.y <= secondArrow->GetHeight() && !grabbed ) {
			CallNextHandler( event );
			event->SetHandled();
			return;
		}
		point.x += secondPosition.x;
	}

	NiPoint2 barPosition;
	barPane->GetPosition( &barPosition );
	point -= barPosition;
	float fraction = point.x / ( barPane->GetWidth() - barEnd->GetWidth() );
	float range = maxValue - minValue;
	float newValue = range * fraction + minValue;
	if ( newValue < minValue ) {
		newValue = minValue;
	}
	if ( newValue > maxValue ) {
		newValue = maxValue;
	}

	int inputFlags = event->GetFlags();
	if ( event->IsButtonEvent() && ( inputFlags & MEF_BUTTON_LEFT ) ) {
		int buttonUp = inputFlags & MEF_BUTTON_UP;
		if ( !buttonUp ) {
			if ( inputFlags & MEF_BUTTON_DOWN ) {
				float steps = range / markerValue;
				float position = steps * fraction;
				float above = (float)( (int)( ( position + SNAP_STEP ) * 100.0f ) / 5 ) / 20.0f;
				float below = (float)( (int)( position * 100.0f ) / 5 ) / 20.0f;
				float nearest;
				if ( fabs( above - position ) < fabs( below - position ) ) {
					nearest = above;
				} else {
					nearest = below;
				}
				newValue = nearest / steps * range + minValue;
				if ( newValue < minValue ) {
					newValue = minValue;
				}
				if ( newValue > maxValue ) {
					newValue = maxValue;
				}
			}
			if ( updateEvent != NULL ) {
				TGFloatEvent *update = (TGFloatEvent *)updateEvent->Duplicate();
				update->SetSource( this );
				update->SetFloat( newValue );
				g_kEventManager.AddEvent( update );
			}
			SetValue( newValue );
			UpdateGauge( true );
		}
		if ( inputFlags & MEF_BUTTON_DOWN ) {
			g_kRootWindow.GrabMouse( this );
			if ( parent != NULL && parent->GetFocus() != this ) {
				parent->SetFocus( this, true );
			}
		}
		if ( buttonUp ) {
			g_kRootWindow.ReleaseMouse( this );
		}
		event->SetHandled();
	}

	if ( !event->IsButtonEvent() && grabbed == true ) {
		if ( updateEvent != NULL ) {
			TGFloatEvent *update = (TGFloatEvent *)updateEvent->Duplicate();
			update->SetSource( this );
			update->SetFloat( newValue );
			g_kEventManager.AddEvent( update );
		}
		SetValue( newValue );
		UpdateGauge( true );
		event->SetHandled();
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
STNumericBar::HandleArrow

An arrow button was pressed: the event says which.
================
*/
// FUNCTION: STBC 0x0052b990
void STNumericBar::HandleArrow( TGIEvent *event ) {
	Step( event->EventHandled() );
	CallNextHandler( event );
}

/*
================
STNumericBar::Step

Moves the value one step, within the range, and sends a copy of the update
event with it.
================
*/
// FUNCTION: STBC 0x0052b9c0
// PARKED: x87: the value and the step are float locals (retail loads both before the branch); the down arm is fxch; fsub st(1) here, fsubr st(1) in retail (the result goes to the step's slot)
void STNumericBar::Step( bool up ) {
	float current = value;
	float interval = keyInterval;
	float next;
	if ( !up ) {
		next = current - interval;
	} else {
		next = current + interval;
	}
	if ( next < minValue ) {
		next = minValue;
	}
	if ( next > maxValue ) {
		next = maxValue;
	}
	if ( updateEvent != NULL ) {
		TGFloatEvent *event = (TGFloatEvent *)updateEvent->Duplicate();
		event->SetSource( this );
		event->SetFloat( next );
		g_kEventManager.AddEvent( event );
	}
	SetValue( next );
	UpdateGauge( true );
}

/*
================
STNumericBar::WriteToStream

The update event is saved as its ID.
================
*/
// FUNCTION: STBC 0x0052ba50
bool STNumericBar::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( 7 );
	stream->WriteFloat( minValue );
	stream->WriteFloat( maxValue );
	stream->WriteFloat( value );
	stream->WriteBool( useMarker );
	stream->WriteBool( useAlternateColor );
	stream->WriteBool( useButtons );
	stream->WriteFloat( markerValue );
	stream->WriteFloat( alternateValue );
	stream->WriteFloat( keyInterval );
	stream->WriteID( GetSavedID( updateEvent ) );
	stream->WriteFloat( normalColor.r );
	stream->WriteFloat( normalColor.g );
	stream->WriteFloat( normalColor.b );
	stream->WriteFloat( normalColor.a );
	stream->WriteFloat( emptyColor.r );
	stream->WriteFloat( emptyColor.g );
	stream->WriteFloat( emptyColor.b );
	stream->WriteFloat( emptyColor.a );
	stream->WriteLong( 0x6F7CE );
	return true;
}

/*
================
STNumericBar::ReadFromStream

Keeps the update event's ID until PostFixUp.
================
*/
// FUNCTION: STBC 0x0052bb60
bool STNumericBar::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	stream->ReadLong();
	minValue = stream->ReadFloat();
	maxValue = stream->ReadFloat();
	value = stream->ReadFloat();
	useMarker = stream->ReadBool();
	useAlternateColor = stream->ReadBool();
	useButtons = stream->ReadBool();
	markerValue = stream->ReadFloat();
	alternateValue = stream->ReadFloat();
	keyInterval = stream->ReadFloat();
	updateEvent = (TGFloatEvent *)stream->ReadID();
	normalColor.r = stream->ReadFloat();
	normalColor.g = stream->ReadFloat();
	normalColor.b = stream->ReadFloat();
	normalColor.a = stream->ReadFloat();
	emptyColor.r = stream->ReadFloat();
	emptyColor.g = stream->ReadFloat();
	emptyColor.b = stream->ReadFloat();
	emptyColor.a = stream->ReadFloat();
	stream->ReadLong();
	return true;
}

/*
================
STNumericBar::FixUpPointers

Fails until the update event is loaded.
================
*/
// FUNCTION: STBC 0x0052bc50
bool STNumericBar::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	if ( updateEvent != NULL && TGEvent::GetSavedEvent( (long)updateEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
STNumericBar::PostFixUp

Replaces the ID with the event.
================
*/
// FUNCTION: STBC 0x0052bc80
void STNumericBar::PostFixUp( void ) {
	TGPane::PostFixUp();
	if ( updateEvent == NULL ) {
		updateEvent = NULL;
	} else {
		updateEvent = (TGFloatEvent *)TGEvent::GetSavedEvent( (long)updateEvent );
	}
}
