/*
===============================================================================

	STButton.cpp

===============================================================================
*/

#include "STButton.h"
#include "STCheckbox.h"
#include "STFillGauge.h"
#include "STMenu.h"
#include "STStylizedWindow.h"
#include "STSubPane.h"
#include "InterfaceModule.h"
#include "GraphicsModeInfo.h"
#include "TopWindow.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "TGString.h"
#include "TGStream.h"
#include "TGBoolEvent.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGEventHandlerTable.h"
#include "NiPoint2.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0051b130 _$E469
// FUNCTION: STBC 0x0051b150 _$E468

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0051aaf0 _$E33
// FUNCTION: STBC 0x0051ab00 _$E32
// FUNCTION: STBC 0x0051ab10 _$E37
// FUNCTION: STBC 0x0051ab20 _$E36
// FUNCTION: STBC 0x0051ab30 _$E48
// FUNCTION: STBC 0x0051ab40 _$E47
// FUNCTION: STBC 0x0051ab50 _$E52
// FUNCTION: STBC 0x0051ab60 _$E51
// FUNCTION: STBC 0x0051ab70 _$E56
// FUNCTION: STBC 0x0051ab80 _$E55
// FUNCTION: STBC 0x0051ab90 _$E60
// FUNCTION: STBC 0x0051aba0 _$E59
// FUNCTION: STBC 0x0051abb0 _$E184
// FUNCTION: STBC 0x0051abc0 _$E183
// FUNCTION: STBC 0x0051abd0 _$E126
// FUNCTION: STBC 0x0051abe0 _$E125
// FUNCTION: STBC 0x0051abf0 _$E139
// FUNCTION: STBC 0x0051ac00 _$E138
// FUNCTION: STBC 0x0051ac10 _$E189
// FUNCTION: STBC 0x0051ac20 _$E188
// FUNCTION: STBC 0x0051ac30 _$E194
// FUNCTION: STBC 0x0051ac40 _$E193
// FUNCTION: STBC 0x0051ac50 _$E198
// FUNCTION: STBC 0x0051ac60 _$E197
// FUNCTION: STBC 0x0051ac70 _$E202
// FUNCTION: STBC 0x0051ac80 _$E201
// FUNCTION: STBC 0x0051ac90 _$E209
// FUNCTION: STBC 0x0051aca0 _$E208
// FUNCTION: STBC 0x0051acb0 _$E292
// FUNCTION: STBC 0x0051acc0 _$E291
// FUNCTION: STBC 0x0051acd0 _$E296
// FUNCTION: STBC 0x0051ace0 _$E295
// FUNCTION: STBC 0x0051acf0 _$E84
// FUNCTION: STBC 0x0051ad00 _$E83
// FUNCTION: STBC 0x0051ad10 _$E278
// FUNCTION: STBC 0x0051ad20 _$E277
// FUNCTION: STBC 0x0051ad30 _$E285
// FUNCTION: STBC 0x0051ad40 _$E284
// FUNCTION: STBC 0x0051ad50 _$E308
// FUNCTION: STBC 0x0051ad60 _$E307
// FUNCTION: STBC 0x0051ad70 _$E312
// FUNCTION: STBC 0x0051ad80 _$E311
// FUNCTION: STBC 0x0051ad90 _$E352
// FUNCTION: STBC 0x0051ada0 _$E351
// FUNCTION: STBC 0x0051adb0 _$E358
// FUNCTION: STBC 0x0051adc0 _$E357
// FUNCTION: STBC 0x0051add0 _$E300
// FUNCTION: STBC 0x0051ade0 _$E299
// FUNCTION: STBC 0x0051adf0 _$E304
// FUNCTION: STBC 0x0051ae00 _$E303
// FUNCTION: STBC 0x0051ae10 _$E316
// FUNCTION: STBC 0x0051ae20 _$E315
// FUNCTION: STBC 0x0051ae30 _$E320
// FUNCTION: STBC 0x0051ae40 _$E319
// FUNCTION: STBC 0x0051ae50 _$E324
// FUNCTION: STBC 0x0051ae60 _$E323
// FUNCTION: STBC 0x0051ae70 _$E328
// FUNCTION: STBC 0x0051ae80 _$E327
// FUNCTION: STBC 0x0051ae90 _$E332
// FUNCTION: STBC 0x0051aea0 _$E331
// FUNCTION: STBC 0x0051aeb0 _$E336
// FUNCTION: STBC 0x0051aec0 _$E335
// FUNCTION: STBC 0x0051aed0 _$E340
// FUNCTION: STBC 0x0051aee0 _$E339
// FUNCTION: STBC 0x0051aef0 _$E344
// FUNCTION: STBC 0x0051af00 _$E343
// FUNCTION: STBC 0x0051af10 _$E89
// FUNCTION: STBC 0x0051af20 _$E88
// FUNCTION: STBC 0x0051af30 _$E348
// FUNCTION: STBC 0x0051af40 _$E347
// FUNCTION: STBC 0x0051af50 _$E94
// FUNCTION: STBC 0x0051af60 _$E93
// FUNCTION: STBC 0x0051af70 _$E362
// FUNCTION: STBC 0x0051af80 _$E361
// FUNCTION: STBC 0x0051af90 _$E366
// FUNCTION: STBC 0x0051afa0 _$E365
// FUNCTION: STBC 0x0051afb0 _$E370
// FUNCTION: STBC 0x0051afc0 _$E369
// FUNCTION: STBC 0x0051afd0 _$E374
// FUNCTION: STBC 0x0051afe0 _$E373
// FUNCTION: STBC 0x0051aff0 _$E103
// FUNCTION: STBC 0x0051b000 _$E102
// FUNCTION: STBC 0x0051b010 _$E378
// FUNCTION: STBC 0x0051b020 _$E377
// FUNCTION: STBC 0x0051b030 _$E382
// FUNCTION: STBC 0x0051b040 _$E381
// FUNCTION: STBC 0x0051b050 _$E386
// FUNCTION: STBC 0x0051b060 _$E385
// FUNCTION: STBC 0x0051b070 _$E109
// FUNCTION: STBC 0x0051b080 _$E108
// FUNCTION: STBC 0x0051b090 _$E114
// FUNCTION: STBC 0x0051b0a0 _$E113
// FUNCTION: STBC 0x0051b0b0 _$E144
// FUNCTION: STBC 0x0051b0c0 _$E143
// FUNCTION: STBC 0x0051b0d0 _$E390
// FUNCTION: STBC 0x0051b0e0 _$E389
// FUNCTION: STBC 0x0051b0f0 _$E149
// FUNCTION: STBC 0x0051b100 _$E148
// FUNCTION: STBC 0x0051b110 _$E394
// FUNCTION: STBC 0x0051b120 _$E393
// FUNCTION: STBC 0x0051b160 _$E398
// FUNCTION: STBC 0x0051b170 _$E397
// FUNCTION: STBC 0x0051b180 _$E154
// FUNCTION: STBC 0x0051b190 _$E153
// FUNCTION: STBC 0x0051b1a0 _$E402
// FUNCTION: STBC 0x0051b1b0 _$E401
// FUNCTION: STBC 0x0051b1c0 _$E119
// FUNCTION: STBC 0x0051b1d0 _$E118
// FUNCTION: STBC 0x0051b1e0 _$E406
// FUNCTION: STBC 0x0051b1f0 _$E405
// FUNCTION: STBC 0x0051b200 _$E410
// FUNCTION: STBC 0x0051b210 _$E409
// FUNCTION: STBC 0x0051b220 _$E159
// FUNCTION: STBC 0x0051b230 _$E158
// FUNCTION: STBC 0x0051b240 _$E164
// FUNCTION: STBC 0x0051b250 _$E163
// FUNCTION: STBC 0x0051b260 _$E414
// FUNCTION: STBC 0x0051b270 _$E413
// FUNCTION: STBC 0x0051b280 _$E418
// FUNCTION: STBC 0x0051b290 _$E417
// FUNCTION: STBC 0x0051b2a0 _$E422
// FUNCTION: STBC 0x0051b2b0 _$E421
// FUNCTION: STBC 0x0051b2c0 _$E426
// FUNCTION: STBC 0x0051b2d0 _$E425
// FUNCTION: STBC 0x0051b2e0 _$E430
// FUNCTION: STBC 0x0051b2f0 _$E429
// FUNCTION: STBC 0x0051b300 _$E434
// FUNCTION: STBC 0x0051b310 _$E433
// FUNCTION: STBC 0x0051b320 _$E438
// FUNCTION: STBC 0x0051b330 _$E437
// FUNCTION: STBC 0x0051b340 _$E442
// FUNCTION: STBC 0x0051b350 _$E441
// FUNCTION: STBC 0x0051b360 _$E169
// FUNCTION: STBC 0x0051b370 _$E168
// FUNCTION: STBC 0x0051b380 _$E446
// FUNCTION: STBC 0x0051b390 _$E445
// FUNCTION: STBC 0x0051b3a0 _$E174
// FUNCTION: STBC 0x0051b3b0 _$E173
// FUNCTION: STBC 0x0051b3c0 _$E179
// FUNCTION: STBC 0x0051b3d0 _$E178
// FUNCTION: STBC 0x0051b3f0 _$E450
// FUNCTION: STBC 0x0051b400 _$E449
// FUNCTION: STBC 0x0051b410 _$E455
// FUNCTION: STBC 0x0051b420 _$E454
// FUNCTION: STBC 0x0051b430 _$E256
// FUNCTION: STBC 0x0051b440 _$E255

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0051a400 _$E264
// FUNCTION: STBC 0x0051a410 _$E261
// FUNCTION: STBC 0x0051a420 _$E263
// FUNCTION: STBC 0x0051a430 _$E262
// FUNCTION: STBC 0x0051a480 _$E270
// FUNCTION: STBC 0x0051a490 _$E267
// FUNCTION: STBC 0x0051a4a0 _$E269
// FUNCTION: STBC 0x0051a4b0 _$E268

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00518b60 _$E461
// FUNCTION: STBC 0x00518b70 _$E457
// FUNCTION: STBC 0x00518b80 _$E460
// FUNCTION: STBC 0x00518b90 _$E458
// DATA: STBC 0x00987fa0
TGEventHandlerTable STButton::classHandlers;

/*
================
STButton::Cast

The object as a button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00518bb0
STButton *STButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STBUTTON_TYPE ) ) {
		return (STButton *)object;
	}
	return NULL;
}

/*
================
STButton::STButton
================
*/
// FUNCTION: STBC 0x00518be0 ??0STButton@@QAE@PADPAVTGEvent@@IMM0H@Z
STButton::STButton( char *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) : TGButtonBase( width, height ) {
	TGString string( name );
	Init( &string, event, buttonFlags, width, height, iconGroupName, bodyIconNum );
}

/*
================
STButton::STButton
================
*/
// FUNCTION: STBC 0x00518e70 ??0STButton@@QAE@PAVTGString@@PAVTGEvent@@IMMPADH@Z
STButton::STButton( TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) : TGButtonBase( width, height ) {
	Init( name, event, buttonFlags, width, height, iconGroupName, bodyIconNum );
}

/*
================
STButton::Init

Takes the first depth's colours and builds the children: the text pane
with the name, the end caps, the hidden checkbox, the hidden marker a
quarter of the left cap's width, and the body icon.
================
*/
// FUNCTION: STBC 0x00518fd0
// PARKED: only residue is two dead stack copies of the checkbox constructor's float arguments (retail passes them with fld/fstp and one local slot); new STCheckbox, an inline Create and retyped locals tried
void STButton::Init( TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) {
	SetBatchChildPolys( false );
	flags |= buttonFlags;

	normalColor = g_kSTButtonColors[0][0];
	selectedColor = g_kSTButtonColors[0][1];
	highlightedColor = g_kSTButtonColors[0][2];
	disabledColor = g_kSTButtonColors[0][3];
	textColor = g_kSTMenuTextColor;
	disabledTextColor = g_kSTMenuTextColor;
	selectedTextColor = g_kSTMenuTextHighlightColor;
	highlightedTextColor = g_kSTMenuTextHighlightColor;
	markerNormalColor = g_kSTButtonMarkerDefault;
	markerHighlightedColor = g_kSTButtonMarkerHighlighted;
	markerSelectedColor = g_kSTButtonMarkerSelected;
	uiHeight = 0;
	useUIHeight = true;
	usingEndCaps = true;

	// The group given is ignored: the parts always come from the LCARS
	// group of the current resolution.
	iconGroupName = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();

	TGPane *textPane = new TGPane;
	TGParagraph *text = new TGParagraph( name, 1.0f, &g_kSTMenuTextColor, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
	TGIcon *leftEndCap = new TGIcon( iconGroupName, STBUTTON_ICON_LEFT_END_CAP, &normalColor );
	STCheckbox *checkbox = new STCheckbox( leftEndCap->GetWidth(), leftEndCap->GetHeight() );
	TGIcon *marker = new TGIcon( iconGroupName, STBUTTON_ICON_MARKER, &normalColor );
	TGIcon *body = new TGIcon( iconGroupName, bodyIconNum, &normalColor );
	TGIcon *rightEndCap = new TGIcon( iconGroupName, STBUTTON_ICON_RIGHT_END_CAP, &normalColor );

	marker->Resize( leftEndCap->GetWidth() * 0.25f, height, false );
	checkbox->SetNotVisible( false );

	textPane->AddChild( text, DEFAULT_ST_INDENT_HORIZ, DEFAULT_ST_INDENT_VERT, false );
	AddChild( textPane, 0.0f, 0.0f, false );
	AddChild( leftEndCap, 0.0f, 0.0f, false );
	AddChild( checkbox, 0.0f, 0.0f, false );
	AddChild( marker, 0.0f, 0.0f, false );
	AddChild( body, 0.0f, 0.0f, false );
	AddChild( rightEndCap, 0.0f, 0.0f, false );
	marker->SetNotVisible( false );

	SetActivationEvent( event );
	Resize( width, height, false );
	TGUIObject::SetHighlighted( false );
	TGUIObject::SetEnabled( false );
	SetNotHighlighted( false );
}

/*
================
STButton::SetName
================
*/
// FUNCTION: STBC 0x005194e0
void STButton::SetName( TGString *name, bool layout ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	if ( text != NULL ) {
		text->SetString( name, layout );
	}
	Resize( GetWidth(), GetHeight(), layout );
}

/*
================
STButton::GetName
================
*/
// FUNCTION: STBC 0x00519550
void STButton::GetName( TGString *name ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	if ( text != NULL ) {
		text->GetString( name );
	}
}

/*
================
STButton::GetDesiredSize

The width of the parts and the indents around the name, at the marker's
height.
================
*/
// FUNCTION: STBC 0x005195a0
void STButton::GetDesiredSize( NiPoint2 *size ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	TGIcon *leftEndCap = TGIcon::Cast( GetNthChild( STBUTTON_LEFT_END_CAP ) );
	TGIcon *rightEndCap = TGIcon::Cast( GetNthChild( STBUTTON_RIGHT_END_CAP ) );
	if ( textPane != NULL && leftEndCap != NULL && rightEndCap != NULL ) {
		TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
		TGUIObject *marker = GetNthChild( STBUTTON_MARKER );
		size->x = marker->GetWidth() + text->GetWidth() + rightEndCap->GetWidth() + leftEndCap->GetWidth()
				+ MARKER_INDENT_HORIZ * 2.0f + DEFAULT_ST_INDENT_HORIZ * 3.0f;
		size->y = marker->GetHeight();
	}
}

/*
================
STButton::GetUIHeight
================
*/
// FUNCTION: STBC 0x00519650
int STButton::GetUIHeight( void ) {
	int height = 0;
	TGPane *pane = parent;
	while ( pane != NULL && pane->IsTypeOf( STSUBPANE_TYPE ) ) {
		// A sub-pane sits in its menu.
		pane = pane->GetParent();
		height++;
		if ( pane == NULL ) {
			break;
		}
		pane = pane->GetParent();
	}
	if ( IsChosen() == true && !IsChoosable() ) {
		height--;
		if ( height < 0 ) {
			return 0;
		}
	}
	if ( height >= STBUTTON_NUM_UI_HEIGHTS ) {
		return STBUTTON_NUM_UI_HEIGHTS - 1;
	}
	return height;
}

/*
================
STButton::UpdateUIHeightColors
================
*/
// FUNCTION: STBC 0x005196b0
void STButton::UpdateUIHeightColors( void ) {
	int height = GetUIHeight();
	if ( height == uiHeight ) {
		return;
	}
	uiHeight = height;
	normalColor = g_kSTButtonColors[height][0];
	selectedColor = g_kSTButtonColors[height][1];
	highlightedColor = g_kSTButtonColors[height][2];
	disabledColor = g_kSTButtonColors[height][3];
	SetColorBasedOnFlags();
}

/*
================
STButton::Layout

Sizes the button to its parent's width or to its name as the flags ask,
then places the parts left to right: the left end cap (under the
checkbox), the marker, the body with the text pane over it, and the
right end cap.
================
*/
// FUNCTION: STBC 0x00519780
// PARKED: schedule residue: the marker's MARKER_INDENT_HORIZ fadd issues before its vtable load (retail after) and the right end cap's left fadd after its this reload (an input-order tie); operands and calls match
void STButton::Layout( void ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGUIObject *leftEndCap = GetNthChild( STBUTTON_LEFT_END_CAP );
	TGUIObject *marker = GetNthChild( STBUTTON_MARKER );
	TGUIObject *body = GetNthChild( STBUTTON_BODY );
	TGUIObject *rightEndCap = GetNthChild( STBUTTON_RIGHT_END_CAP );
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );

	if ( useUIHeight == true ) {
		UpdateUIHeightColors();
	}

	if ( flags & STBF_FILL_PARENT_WIDTH ) {
		TGPane *parentPane = parent;
		if ( parentPane != NULL && ( flags & STBF_STRETCH_TEXT ) ) {
			float wrapWidth = parentPane->GetWidth() - leftEndCap->GetWidth() - marker->GetWidth() - rightEndCap->GetWidth();
			if ( text->GetWrapWidth() != wrapWidth ) {
				text->SetWrapWidth( wrapWidth, true );
				STSubPane *subPane = STSubPane::Cast( GetParent() );
				if ( subPane != NULL ) {
					subPane->ResizeToContents( true );
				}
			}
		}
		float newHeight = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 4.0f;
		if ( parentPane != NULL && ( GetWidth() != parentPane->GetWidth() || GetHeight() != newHeight ) ) {
			Resize( parentPane->GetWidth(), newHeight, false );
		}
	}
	if ( flags & STBF_SIZE_TO_TEXT ) {
		Resize( GetWidth(), GetHeight(), false );
	}

	leftEndCap->SetPosition( 0.0f, 0.0f, false );
	checkbox->SetPosition( leftEndCap->GetLeft(), leftEndCap->GetTop(), false );
	marker->SetPosition( leftEndCap->GetRight() + MARKER_INDENT_HORIZ, 0.0f, false );
	body->SetPosition( marker->GetRight() + MARKER_INDENT_HORIZ, 0.0f, false );
	rightEndCap->SetPosition( body->GetRight(), 0.0f, false );
	textPane->SetPosition( body->GetLeft(), body->GetTop(), false );
	text->SetPosition( DEFAULT_ST_INDENT_HORIZ, DEFAULT_ST_INDENT_VERT, false );
	TGPane::Layout();
}

/*
================
STButton::Resize

Sizes the parts: the end caps keep their artwork's shape at the button's
height, the marker is a quarter of the left cap, and the body takes the
rest. A button sized to its name or to its parent works out its own size
first.
================
*/
// FUNCTION: STBC 0x005199d0
void STButton::Resize( float width, float height, bool layout ) {
	float newWidth = width;
	float newHeight = height;
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	TGIcon *leftEndCap = (TGIcon *)GetNthChild( STBUTTON_LEFT_END_CAP );
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	TGUIObject *marker = GetNthChild( STBUTTON_MARKER );
	TGUIObject *body = GetNthChild( STBUTTON_BODY );
	TGIcon *rightEndCap = (TGIcon *)GetNthChild( STBUTTON_RIGHT_END_CAP );

	leftEndCap->SizeToArtwork( false );
	rightEndCap->SizeToArtwork( false );
	float leftWidth = leftEndCap->GetWidth();
	float rightWidth = rightEndCap->GetWidth();
	float bodyWidth;

	if ( flags & STBF_SIZE_TO_TEXT ) {
		newHeight = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
		leftWidth *= newHeight / leftEndCap->GetHeight();
		rightWidth *= newHeight / rightEndCap->GetHeight();
		marker->Resize( leftWidth * 0.25f, marker->GetHeight(), false );
		bodyWidth = text->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 2.0f;
		newWidth = MARKER_INDENT_HORIZ * 2.0f + marker->GetWidth() + bodyWidth + rightWidth + leftWidth;
	} else {
		if ( !( flags & STBF_FILL_PARENT_WIDTH ) ) {
			leftWidth *= height / leftEndCap->GetHeight();
			rightWidth *= height / rightEndCap->GetHeight();
		}
		marker->Resize( leftWidth * 0.25f, marker->GetHeight(), false );
		bodyWidth = width - leftWidth - MARKER_INDENT_HORIZ * 2.0f - marker->GetWidth() - rightWidth;
	}
	if ( flags & STBF_FILL_PARENT_WIDTH ) {
		newHeight = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
		leftWidth *= newHeight / leftEndCap->GetHeight();
		rightWidth *= newHeight / rightEndCap->GetHeight();
		marker->Resize( leftWidth * 0.25f, marker->GetHeight(), false );
		if ( parent != NULL ) {
			newWidth = parent->GetWidth();
			bodyWidth = newWidth - leftWidth - MARKER_INDENT_HORIZ * 2.0f - marker->GetWidth() - rightWidth;
		}
	}

	TGButtonBase::Resize( newWidth, newHeight, false );
	leftEndCap->Resize( leftWidth, newHeight, false );
	checkbox->Resize( leftEndCap->GetWidth(), leftEndCap->GetHeight(), false );
	marker->Resize( marker->GetWidth(), newHeight, false );
	body->Resize( bodyWidth, newHeight, false );
	rightEndCap->Resize( rightWidth, newHeight, false );
	if ( IsUseEndCapSpace() == true ) {
		textPane->Resize( rightEndCap->GetWidth() + bodyWidth, newHeight, false );
	} else {
		textPane->Resize( bodyWidth, newHeight, false );
	}
	JustifyText( false );
	if ( layout ) {
		Layout();
	}
}

/*
================
STButton::SetJustification
================
*/
// FUNCTION: STBC 0x00519ce0
void STButton::SetJustification( int justification ) {
	this->justification = justification;
	JustifyText( true );
}

/*
================
STButton::JustifyText
================
*/
// FUNCTION: STBC 0x00519d00
// PARKED: schedule residue: two fsubp issue one instruction later than retail's, after the vtable load; operands and calls match
void STButton::JustifyText( bool layout ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	if ( text == NULL ) {
		return;
	}
	switch ( justification ) {
		case STBUTTON_LEFT:
			text->SetPosition( DEFAULT_ST_INDENT_HORIZ, textPane->GetHeight() * 0.5f - text->GetHeight() * 0.5f, false );
			break;
		case STBUTTON_CENTER:
			text->SetPosition( textPane->GetWidth() * 0.5f - text->GetWidth() * 0.5f, textPane->GetHeight() * 0.5f - text->GetHeight() * 0.5f, false );
			break;
		case STBUTTON_RIGHT:
			text->SetPosition( textPane->GetWidth() - text->GetWidth() - DEFAULT_ST_INDENT_HORIZ, textPane->GetHeight() * 0.5f - text->GetHeight() * 0.5f, false );
			break;
	}
	if ( layout == true ) {
		textPane->Layout();
	}
}

/*
================
STButton::SetColor

Colours the parts; the name takes the text colour of the button's state.
================
*/
// FUNCTION: STBC 0x00519e00
void STButton::SetColor( NiColorA *color ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	if ( text != NULL ) {
		if ( !IsEnabled() ) {
			text->SetColor( &disabledTextColor );
		} else if ( IsSelected() ) {
			text->SetColor( &selectedTextColor );
		} else if ( IsHighlighted() ) {
			text->SetColor( &highlightedTextColor );
		} else {
			text->SetColor( &textColor );
		}
	}
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	if ( checkbox != NULL ) {
		checkbox->SetBoxColor( color );
	}
	TGIcon *leftEndCap = (TGIcon *)GetNthChild( STBUTTON_LEFT_END_CAP );
	TGUIObject *body = GetNthChild( STBUTTON_BODY );
	TGIcon *rightEndCap = (TGIcon *)GetNthChild( STBUTTON_RIGHT_END_CAP );
	leftEndCap->SetColor( color );
	rightEndCap->SetColor( color );
	if ( body->IsTypeOf( TGICON_TYPE ) ) {
		( (TGIcon *)body )->SetColor( color );
	} else if ( body->IsTypeOf( STFILLGAUGE_TYPE ) ) {
		( (STFillGauge *)body )->SetFillColor( *color );
	}
}

/*
================
STButton::SetColorBasedOnFlags

Colours the button for its state, and shows the marker on the button in
the true focus path, or grey on the button that has the focus of a
window that does not.
================
*/
// FUNCTION: STBC 0x00519f30
// PARKED: register residue: the last two virtual calls load the vtable into EAX/EDX swapped; a gray flag with one tail, and its reversed test, tried
void STButton::SetColorBasedOnFlags( void ) {
	NiColorA *color = &normalColor;
	if ( !IsEnabled() ) {
		color = &disabledColor;
	} else if ( IsSelected() ) {
		color = &selectedColor;
	} else if ( IsHighlighted() ) {
		color = &highlightedColor;
	}
	SetColor( color );

	TGIcon *marker = (TGIcon *)GetNthChild( STBUTTON_MARKER );
	if ( IsInTrueFocusPath() ) {
		marker->SetVisible( true );
		marker->SetColor( &markerNormalColor );
		return;
	}
	STStylizedWindow *window = GetContainingWindow();
	if ( window != NULL && !window->IsInTrueFocusPath() ) {
		TGUIObject *focus = window->GetFocusLeaf();
		if ( focus != NULL && ( focus->HasAncestor( this ) || focus == this ) ) {
			marker->SetColor( &g_kSTButtonMarkerGray );
			marker->SetVisible( true );
			return;
		}
	}
	marker->SetNotVisible( false );
}

/*
================
STButton::SetSelected
================
*/
// FUNCTION: STBC 0x0051a000
void STButton::SetSelected( bool layout ) {
	if ( !IsSelected() ) {
		TGUIObject::SetSelected( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STButton::SetNotSelected
================
*/
// FUNCTION: STBC 0x0051a020
void STButton::SetNotSelected( bool layout ) {
	if ( IsSelected() ) {
		TGUIObject::SetNotSelected( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STButton::SetHighlighted
================
*/
// FUNCTION: STBC 0x0051a040
void STButton::SetHighlighted( bool layout ) {
	if ( !IsHighlighted() ) {
		TGUIObject::SetHighlighted( layout );
		SetColorBasedOnFlags();
		g_kInterfaceModule.playClickSound = true;
	}
}

/*
================
STButton::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x0051a070
void STButton::SetNotHighlighted( bool layout ) {
	if ( IsHighlighted() ) {
		TGUIObject::SetNotHighlighted( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STButton::SetEnabled

Gives the checkmark its own colour again.
================
*/
// FUNCTION: STBC 0x0051a090
void STButton::SetEnabled( bool layout ) {
	if ( IsEnabled() ) {
		return;
	}
	TGUIObject::SetEnabled( layout );
	SetColorBasedOnFlags();
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	if ( checkbox != NULL ) {
		// The original casts the body here and never uses it.
		TGIcon *body = TGIcon::Cast( GetNthChild( STBUTTON_BODY ) );
		checkbox->SetOnColor( &g_kSTButtonCheckmarkOn );
	}
}

/*
================
STButton::SetDisabled

Draws the checkmark in a brighter shade of the body's colour (dark grey
for a body that is no icon).
================
*/
// FUNCTION: STBC 0x0051a0f0
void STButton::SetDisabled( bool layout ) {
	if ( !IsEnabled() ) {
		return;
	}
	TGButtonBase::SetDisabled( layout );
	SetColorBasedOnFlags();
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	if ( checkbox != NULL ) {
		TGIcon *body = TGIcon::Cast( GetNthChild( STBUTTON_BODY ) );
		NiColorA color( 0.3f, 0.3f, 0.3f, 1.0f );
		if ( body != NULL ) {
			body->GetColor( &color );
			color.r *= 1.3f;
			color.g *= 1.3f;
			color.b *= 1.3f;
			if ( color.r > 1.0f ) {
				color.r = 1.0f;
			}
			if ( color.g > 1.0f ) {
				color.g = 1.0f;
			}
			if ( color.b > 1.0f ) {
				color.b = 1.0f;
			}
			color.a = 1.0f;
		}
		checkbox->SetOnColor( &color );
	}
}

/*
================
STButton::SetChoosable

A choosable button shows its checkbox in place of the left end cap.
================
*/
// FUNCTION: STBC 0x0051a210
void STButton::SetChoosable( bool choosable, bool layout ) {
	// The original casts the text pane here and never uses it.
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	TGIcon *leftEndCap = TGIcon::Cast( GetNthChild( STBUTTON_LEFT_END_CAP ) );
	if ( choosable == true ) {
		flags |= STBF_CHOOSABLE;
		checkbox->SetVisible( false );
		leftEndCap->SetNotVisible( false );
	} else {
		flags &= ~STBF_CHOOSABLE;
		checkbox->SetNotVisible( false );
		leftEndCap->SetVisible( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STButton::SetChosen

Checks or clears the checkbox. In a radio group, choosing a button
unchooses the other buttons and menus of the group.
================
*/
// FUNCTION: STBC 0x0051a2c0
void STButton::SetChosen( bool chosen, bool layout ) {
	// The original casts the text pane here and never uses it.
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	if ( chosen == true ) {
		flags |= STBF_CHOSEN;
		checkbox->SetChecked( true );
		STSubPane *subPane = STSubPane::Cast( GetParent() );
		if ( subPane != NULL && ( subPane->GetFlags() & STSPF_RADIO_GROUP ) ) {
			for ( TGUIObject *child = subPane->GetFirstChild(); child != NULL; child = subPane->GetNextChild( child ) ) {
				STButton *button = STButton::Cast( child );
				if ( button != NULL && button != this ) {
					button->SetChosen( false, false );
				}
				STMenu *menu = STMenu::Cast( child );
				if ( menu != NULL ) {
					menu->SetChosen( false, false );
				}
			}
		}
	} else {
		flags &= ~STBF_CHOSEN;
		checkbox->SetChecked( false );
	}
	UpdateUIHeightColors();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STButton::GotTrueFocus
================
*/
// FUNCTION: STBC 0x0051a3c0
void STButton::GotTrueFocus( bool layout ) {
	TGUIObject::GotTrueFocus( false );
	SetHighlighted( false );
	SetColorBasedOnFlags();
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STButton::LostFocus
================
*/
// FUNCTION: STBC 0x0051a440
void STButton::LostFocus( bool layout ) {
	TGUIObject::LostFocus( layout );
	SetColorBasedOnFlags();
}

/*
================
STButton::LostTrueFocus
================
*/
// FUNCTION: STBC 0x0051a460
void STButton::LostTrueFocus( bool layout ) {
	TGUIObject::LostTrueFocus( layout );
	SetNotHighlighted( false );
}

/*
================
STButton::LostTrueFocusPath
================
*/
// FUNCTION: STBC 0x0051a4c0
void STButton::LostTrueFocusPath( bool layout ) {
	TGUIObject::LostTrueFocusPath( layout );
	SetColorBasedOnFlags();
}

/*
================
STButton::SendActivationEvent

Sends the activation event, plays the click, toggles an auto-choosing
button, and offers ET_ST_BUTTON_CLICKED to each parent below the top
window until one handles it.
================
*/
// FUNCTION: STBC 0x0051a4e0
void STButton::SendActivationEvent( void ) {
	TGButtonBase::SendActivationEvent();
	TGSound *sound = g_kSoundManager.GetSound( "UIButtonClicked" );
	if ( sound != NULL ) {
		sound->Play( true );
	}
	if ( IsAutoChoose() == true ) {
		if ( !IsChosen() ) {
			SetChosen( true, true );
		} else {
			SetChosen( false, true );
		}
	}

	TGBoolEvent *event = new TGBoolEvent;
	event->SetBool( false );
	event->SetEventType( ET_ST_BUTTON_CLICKED );
	for ( TGPane *pane = parent; !event->GetBool() && pane != NULL && pane != TopWindow::GetTopWindow(); pane = pane->GetParent() ) {
		event->SetDestination( pane );
		pane->ProcessEvent( event );
	}
	event->DecRefCount();
}

/*
================
STButton::GetContainingWindow

The nearest stylized window above the button; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0051a5b0
STStylizedWindow *STButton::GetContainingWindow( void ) {
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
STButton::WriteToStream
================
*/
// FUNCTION: STBC 0x0051a5e0
bool STButton::WriteToStream( TGStream *stream ) {
	if ( !TGButtonBase::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( normalColor.r );
	stream->WriteFloat( normalColor.g );
	stream->WriteFloat( normalColor.b );
	stream->WriteFloat( normalColor.a );
	stream->WriteFloat( selectedColor.r );
	stream->WriteFloat( selectedColor.g );
	stream->WriteFloat( selectedColor.b );
	stream->WriteFloat( selectedColor.a );
	stream->WriteFloat( highlightedColor.r );
	stream->WriteFloat( highlightedColor.g );
	stream->WriteFloat( highlightedColor.b );
	stream->WriteFloat( highlightedColor.a );
	stream->WriteFloat( disabledColor.r );
	stream->WriteFloat( disabledColor.g );
	stream->WriteFloat( disabledColor.b );
	stream->WriteFloat( disabledColor.a );
	stream->WriteFloat( markerNormalColor.r );
	stream->WriteFloat( markerNormalColor.g );
	stream->WriteFloat( markerNormalColor.b );
	stream->WriteFloat( markerNormalColor.a );
	stream->WriteFloat( markerHighlightedColor.r );
	stream->WriteFloat( markerHighlightedColor.g );
	stream->WriteFloat( markerHighlightedColor.b );
	stream->WriteFloat( markerHighlightedColor.a );
	stream->WriteFloat( markerSelectedColor.r );
	stream->WriteFloat( markerSelectedColor.g );
	stream->WriteFloat( markerSelectedColor.b );
	stream->WriteFloat( markerSelectedColor.a );
	stream->WriteFloat( textColor.r );
	stream->WriteFloat( textColor.g );
	stream->WriteFloat( textColor.b );
	stream->WriteFloat( textColor.a );
	stream->WriteFloat( highlightedTextColor.r );
	stream->WriteFloat( highlightedTextColor.g );
	stream->WriteFloat( highlightedTextColor.b );
	stream->WriteFloat( highlightedTextColor.a );
	stream->WriteFloat( selectedTextColor.r );
	stream->WriteFloat( selectedTextColor.g );
	stream->WriteFloat( selectedTextColor.b );
	stream->WriteFloat( selectedTextColor.a );
	stream->WriteFloat( disabledTextColor.r );
	stream->WriteFloat( disabledTextColor.g );
	stream->WriteFloat( disabledTextColor.b );
	stream->WriteFloat( disabledTextColor.a );
	stream->WriteInt( uiHeight );
	stream->WriteBool( useUIHeight );
	stream->WriteBool( usingEndCaps );
	stream->WriteInt( justification );
	return true;
}

/*
================
STButton::ReadFromStream
================
*/
// FUNCTION: STBC 0x0051a880
bool STButton::ReadFromStream( TGStream *stream ) {
	if ( !TGButtonBase::ReadFromStream( stream ) ) {
		return false;
	}
	normalColor.r = stream->ReadFloat();
	normalColor.g = stream->ReadFloat();
	normalColor.b = stream->ReadFloat();
	normalColor.a = stream->ReadFloat();
	selectedColor.r = stream->ReadFloat();
	selectedColor.g = stream->ReadFloat();
	selectedColor.b = stream->ReadFloat();
	selectedColor.a = stream->ReadFloat();
	highlightedColor.r = stream->ReadFloat();
	highlightedColor.g = stream->ReadFloat();
	highlightedColor.b = stream->ReadFloat();
	highlightedColor.a = stream->ReadFloat();
	disabledColor.r = stream->ReadFloat();
	disabledColor.g = stream->ReadFloat();
	disabledColor.b = stream->ReadFloat();
	disabledColor.a = stream->ReadFloat();
	markerNormalColor.r = stream->ReadFloat();
	markerNormalColor.g = stream->ReadFloat();
	markerNormalColor.b = stream->ReadFloat();
	markerNormalColor.a = stream->ReadFloat();
	markerHighlightedColor.r = stream->ReadFloat();
	markerHighlightedColor.g = stream->ReadFloat();
	markerHighlightedColor.b = stream->ReadFloat();
	markerHighlightedColor.a = stream->ReadFloat();
	markerSelectedColor.r = stream->ReadFloat();
	markerSelectedColor.g = stream->ReadFloat();
	markerSelectedColor.b = stream->ReadFloat();
	markerSelectedColor.a = stream->ReadFloat();
	textColor.r = stream->ReadFloat();
	textColor.g = stream->ReadFloat();
	textColor.b = stream->ReadFloat();
	textColor.a = stream->ReadFloat();
	highlightedTextColor.r = stream->ReadFloat();
	highlightedTextColor.g = stream->ReadFloat();
	highlightedTextColor.b = stream->ReadFloat();
	highlightedTextColor.a = stream->ReadFloat();
	selectedTextColor.r = stream->ReadFloat();
	selectedTextColor.g = stream->ReadFloat();
	selectedTextColor.b = stream->ReadFloat();
	selectedTextColor.a = stream->ReadFloat();
	disabledTextColor.r = stream->ReadFloat();
	disabledTextColor.g = stream->ReadFloat();
	disabledTextColor.b = stream->ReadFloat();
	disabledTextColor.a = stream->ReadFloat();
	uiHeight = stream->ReadInt();
	useUIHeight = stream->ReadBool();
	usingEndCaps = stream->ReadBool();
	justification = stream->ReadInt();
	return true;
}
