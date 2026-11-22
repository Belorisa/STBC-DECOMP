/*
===============================================================================

	STRepairButton.cpp

===============================================================================
*/

#include "STRepairButton.h"
#include "TGEventManager.h"
#include "GraphicsModeInfo.h"
#include "PyEmbed.h"
#include "NiMath.h"
#include "TGEventHandlerTable.h"
#include "STCheckbox.h"
#include "STColors.h"
#include "DamageDisplay.h"
#include "InterfaceModule.h"
#include "ShipSubsystem.h"
#include "ShipClass.h"
#include "TGIcon.h"
#include "TGParagraph.h"
#include "TGFontManager.h"
#include "TGFontGroup.h"
#include "TGStream.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0052d9e0 _$E508
// FUNCTION: STBC 0x0052da00 _$E507

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0052d0a0 _$E33
// FUNCTION: STBC 0x0052d0b0 _$E32
// FUNCTION: STBC 0x0052d0c0 _$E37
// FUNCTION: STBC 0x0052d0d0 _$E36
// FUNCTION: STBC 0x0052d2a0 _$E48
// FUNCTION: STBC 0x0052d2b0 _$E47
// FUNCTION: STBC 0x0052d2c0 _$E52
// FUNCTION: STBC 0x0052d2d0 _$E51
// FUNCTION: STBC 0x0052d2e0 _$E56
// FUNCTION: STBC 0x0052d2f0 _$E55
// FUNCTION: STBC 0x0052d300 _$E60
// FUNCTION: STBC 0x0052d310 _$E59
// FUNCTION: STBC 0x0052d320 _$E184
// FUNCTION: STBC 0x0052d330 _$E183
// FUNCTION: STBC 0x0052d340 _$E126
// FUNCTION: STBC 0x0052d350 _$E125
// FUNCTION: STBC 0x0052d360 _$E139
// FUNCTION: STBC 0x0052d370 _$E138
// FUNCTION: STBC 0x0052d380 _$E189
// FUNCTION: STBC 0x0052d390 _$E188
// FUNCTION: STBC 0x0052d3a0 _$E194
// FUNCTION: STBC 0x0052d3b0 _$E193
// FUNCTION: STBC 0x0052d3c0 _$E198
// FUNCTION: STBC 0x0052d3d0 _$E197
// FUNCTION: STBC 0x0052d3e0 _$E202
// FUNCTION: STBC 0x0052d3f0 _$E201
// FUNCTION: STBC 0x0052d400 _$E209
// FUNCTION: STBC 0x0052d410 _$E208
// FUNCTION: STBC 0x0052d420 _$E340
// FUNCTION: STBC 0x0052d430 _$E339
// FUNCTION: STBC 0x0052d440 _$E344
// FUNCTION: STBC 0x0052d450 _$E343
// FUNCTION: STBC 0x0052d460 _$E84
// FUNCTION: STBC 0x0052d470 _$E83
// FUNCTION: STBC 0x0052d480 _$E287
// FUNCTION: STBC 0x0052d490 _$E286
// FUNCTION: STBC 0x0052d4a0 _$E294
// FUNCTION: STBC 0x0052d4b0 _$E293
// FUNCTION: STBC 0x0052d4c0 _$E298
// FUNCTION: STBC 0x0052d4d0 _$E297
// FUNCTION: STBC 0x0052d4e0 _$E302
// FUNCTION: STBC 0x0052d4f0 _$E301
// FUNCTION: STBC 0x0052d500 _$E306
// FUNCTION: STBC 0x0052d510 _$E305
// FUNCTION: STBC 0x0052d520 _$E312
// FUNCTION: STBC 0x0052d530 _$E311
// FUNCTION: STBC 0x0052d540 _$E348
// FUNCTION: STBC 0x0052d550 _$E347
// FUNCTION: STBC 0x0052d560 _$E352
// FUNCTION: STBC 0x0052d570 _$E351
// FUNCTION: STBC 0x0052d580 _$E356
// FUNCTION: STBC 0x0052d590 _$E355
// FUNCTION: STBC 0x0052d5a0 _$E360
// FUNCTION: STBC 0x0052d5b0 _$E359
// FUNCTION: STBC 0x0052d5c0 _$E364
// FUNCTION: STBC 0x0052d5d0 _$E363
// FUNCTION: STBC 0x0052d5e0 _$E368
// FUNCTION: STBC 0x0052d5f0 _$E367
// FUNCTION: STBC 0x0052d600 _$E372
// FUNCTION: STBC 0x0052d610 _$E371
// FUNCTION: STBC 0x0052d620 _$E376
// FUNCTION: STBC 0x0052d630 _$E375
// FUNCTION: STBC 0x0052d640 _$E380
// FUNCTION: STBC 0x0052d650 _$E379
// FUNCTION: STBC 0x0052d660 _$E384
// FUNCTION: STBC 0x0052d670 _$E383
// FUNCTION: STBC 0x0052d680 _$E89
// FUNCTION: STBC 0x0052d690 _$E88
// FUNCTION: STBC 0x0052d6a0 _$E388
// FUNCTION: STBC 0x0052d6b0 _$E387
// FUNCTION: STBC 0x0052d6c0 _$E94
// FUNCTION: STBC 0x0052d6d0 _$E93
// FUNCTION: STBC 0x0052d6e0 _$E392
// FUNCTION: STBC 0x0052d6f0 _$E391
// FUNCTION: STBC 0x0052d700 _$E396
// FUNCTION: STBC 0x0052d710 _$E395
// FUNCTION: STBC 0x0052d720 _$E400
// FUNCTION: STBC 0x0052d730 _$E399
// FUNCTION: STBC 0x0052d740 _$E404
// FUNCTION: STBC 0x0052d750 _$E403
// FUNCTION: STBC 0x0052d760 _$E103
// FUNCTION: STBC 0x0052d770 _$E102
// FUNCTION: STBC 0x0052d780 _$E408
// FUNCTION: STBC 0x0052d790 _$E407
// FUNCTION: STBC 0x0052d7a0 _$E412
// FUNCTION: STBC 0x0052d7b0 _$E411
// FUNCTION: STBC 0x0052d7c0 _$E416
// FUNCTION: STBC 0x0052d7d0 _$E415
// FUNCTION: STBC 0x0052d7e0 _$E109
// FUNCTION: STBC 0x0052d7f0 _$E108
// FUNCTION: STBC 0x0052d800 _$E114
// FUNCTION: STBC 0x0052d810 _$E113
// FUNCTION: STBC 0x0052d820 _$E144
// FUNCTION: STBC 0x0052d830 _$E143
// FUNCTION: STBC 0x0052d840 _$E420
// FUNCTION: STBC 0x0052d850 _$E419
// FUNCTION: STBC 0x0052d860 _$E149
// FUNCTION: STBC 0x0052d870 _$E148
// FUNCTION: STBC 0x0052d880 _$E424
// FUNCTION: STBC 0x0052d890 _$E423
// FUNCTION: STBC 0x0052d8a0 _$E428
// FUNCTION: STBC 0x0052d8b0 _$E427
// FUNCTION: STBC 0x0052d8c0 _$E154
// FUNCTION: STBC 0x0052d8d0 _$E153
// FUNCTION: STBC 0x0052d8e0 _$E432
// FUNCTION: STBC 0x0052d8f0 _$E431
// FUNCTION: STBC 0x0052d900 _$E119
// FUNCTION: STBC 0x0052d910 _$E118
// FUNCTION: STBC 0x0052d920 _$E436
// FUNCTION: STBC 0x0052d930 _$E435
// FUNCTION: STBC 0x0052d940 _$E440
// FUNCTION: STBC 0x0052d950 _$E439
// FUNCTION: STBC 0x0052d960 _$E159
// FUNCTION: STBC 0x0052d970 _$E158
// FUNCTION: STBC 0x0052d980 _$E164
// FUNCTION: STBC 0x0052d990 _$E163
// FUNCTION: STBC 0x0052d9a0 _$E444
// FUNCTION: STBC 0x0052d9b0 _$E443
// FUNCTION: STBC 0x0052d9c0 _$E448
// FUNCTION: STBC 0x0052d9d0 _$E447
// FUNCTION: STBC 0x0052da10 _$E452
// FUNCTION: STBC 0x0052da20 _$E451
// FUNCTION: STBC 0x0052da30 _$E456
// FUNCTION: STBC 0x0052da40 _$E455
// FUNCTION: STBC 0x0052da50 _$E460
// FUNCTION: STBC 0x0052da60 _$E459
// FUNCTION: STBC 0x0052da70 _$E464
// FUNCTION: STBC 0x0052da80 _$E463
// FUNCTION: STBC 0x0052da90 _$E468
// FUNCTION: STBC 0x0052daa0 _$E467
// FUNCTION: STBC 0x0052dab0 _$E472
// FUNCTION: STBC 0x0052dac0 _$E471
// FUNCTION: STBC 0x0052dad0 _$E169
// FUNCTION: STBC 0x0052dae0 _$E168
// FUNCTION: STBC 0x0052daf0 _$E476
// FUNCTION: STBC 0x0052db00 _$E475
// FUNCTION: STBC 0x0052db10 _$E174
// FUNCTION: STBC 0x0052db20 _$E173
// FUNCTION: STBC 0x0052db30 _$E179
// FUNCTION: STBC 0x0052db40 _$E178
// FUNCTION: STBC 0x0052db50 _$E480
// FUNCTION: STBC 0x0052db60 _$E479
// FUNCTION: STBC 0x0052db70 _$E485
// FUNCTION: STBC 0x0052db80 _$E484
// FUNCTION: STBC 0x0052db90 _$E489
// FUNCTION: STBC 0x0052dba0 _$E488

// The unit's static initialisers and exit-time registrations (<iostream>).
// FUNCTION: STBC 0x0052c6a0 _$E324
// FUNCTION: STBC 0x0052c6b0 _$E321
// FUNCTION: STBC 0x0052c6c0 _$E323
// FUNCTION: STBC 0x0052c6d0 _$E322
// FUNCTION: STBC 0x0052c6e0 _$E330
// FUNCTION: STBC 0x0052c6f0 _$E327
// FUNCTION: STBC 0x0052c700 _$E329
// FUNCTION: STBC 0x0052c710 _$E328

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0052c720 _$E495
// FUNCTION: STBC 0x0052c730 _$E491
// FUNCTION: STBC 0x0052c740 _$E494
// FUNCTION: STBC 0x0052c750 _$E492
// DATA: STBC 0x009895b8
TGEventHandlerTable STRepairButton::classHandlers;

// The font size's dynamic initialiser.
// FUNCTION: STBC 0x0052c7a0 _$E498
// FUNCTION: STBC 0x0052c7b0 _$E497

// DATA: STBC 0x00989608
float STRepairButton::fontSize = NI_INFINITY;

/*
================
STRepairButton::Cast

The object as an STRepairButton; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0052c770
STRepairButton *STRepairButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STREPAIRBUTTON_TYPE ) ) {
		return (STRepairButton *)object;
	}
	return NULL;
}

/*
================
STRepairButton::STRepairButton

A button named for the subsystem, with its damage icon.
================
*/
// FUNCTION: STBC 0x0052c7c0 ??0STRepairButton@@QAE@PAVShipSubsystem@@PADPAVTGEvent@@IMM1H@Z
STRepairButton::STRepairButton( ShipSubsystem *subsystem, char *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) : STButton( name, event, buttonFlags, width, height, iconGroupName, bodyIconNum ) {
	SetSubsystem( subsystem );
}

/*
================
STRepairButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x0052c850
bool STRepairButton::IsTypeOf( int type ) {
	if ( type == STREPAIRBUTTON_TYPE ) {
		return true;
	}
	return STButton::IsTypeOf( type );
}

/*
================
STRepairButton::STRepairButton
================
*/
// FUNCTION: STBC 0x0052c940 ??0STRepairButton@@QAE@PAVShipSubsystem@@PAVTGString@@PAVTGEvent@@IMMPADH@Z
STRepairButton::STRepairButton( ShipSubsystem *subsystem, TGString *name, TGEvent *event, unsigned int buttonFlags, float width, float height, char *iconGroupName, int bodyIconNum ) : STButton( name, event, buttonFlags, width, height, iconGroupName, bodyIconNum ) {
	SetSubsystem( subsystem );
}

/*
================
STRepairButton::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0052c9c0
void STRepairButton::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STRepairButton::HandleSubsystemStateChanged, "STRepairButton::HandleSubsystemStateChanged" );
}

/*
================
STRepairButton::RegisterHandlers

The handler is added for each button, not to the class's table.
================
*/
// FUNCTION: STBC 0x0052c9e0
void STRepairButton::RegisterHandlers( void ) {
}

/*
================
STRepairButton::SetSubsystem

Adds the subsystem's icon from "DamageIcons" and, when the subsystem has
a ship, listens for the ship's subsystems being disabled, destroyed or
made operational, and gives the name the repair font size.
================
*/
// FUNCTION: STBC 0x0052c9f0
void STRepairButton::SetSubsystem( ShipSubsystem *subsystem ) {
	if ( fontSize == NI_INFINITY ) {
		LoadFontSize();
	}
	AddChild( new TGIcon( "DamageIcons", DamageDisplay::GetIconNumber( subsystem ) ), 0.0f, 0.0f, false );
	subsystemID = subsystem->GetObjID();
	UpdateIconColor( subsystem );
	if ( subsystem->GetParentShip() != NULL ) {
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DISABLED, this, "STRepairButton::HandleSubsystemStateChanged", subsystem->GetParentShip() );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_DESTROYED, this, "STRepairButton::HandleSubsystemStateChanged", subsystem->GetParentShip() );
		g_kEventManager.AddBroadcastMethodHandler( ET_SUBSYSTEM_OPERATIONAL, this, "STRepairButton::HandleSubsystemStateChanged", subsystem->GetParentShip() );
		TGParagraph *text = TGParagraph::Cast( TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) )->GetNthChild( 0 ) );
		text->SetFontGroup( g_kFontManager.GetFontGroup( g_kFontManager.GetDefaultFont()->GetFontName(), fontSize ), true );
	}
}

/*
================
STRepairButton::GetDesiredSize

The button's size with the damage icon beside it.
================
*/
// FUNCTION: STBC 0x0052cb40
void STRepairButton::GetDesiredSize( NiPoint2 *size ) {
	STButton::GetDesiredSize( size );
	TGIcon *icon = TGIcon::Cast( GetNthChild( STREPAIRBUTTON_ICON ) );
	if ( icon != NULL ) {
		size->x += icon->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 2.0f;
		if ( icon->GetHeight() > size->y ) {
			size->y = icon->GetHeight();
		}
	}
}

/*
================
STRepairButton::Layout

STButton's layout with room for the damage icon at the right, the indent
taken from the LCARS module's REPAIR_ST_INDENT_HORIZ.
================
*/
// FUNCTION: STBC 0x0052cba0
// PARKED: x87: each width + left sum carries two op 0x162 tuples (the accessors' substituted return values) that hold the fadd two cycles behind its fld, where retail adds at once; the first sum also loads left first (the return temps' numbering, cards); one-level TGUIObject accessors (a scratch header) keep the tuples
void STRepairButton::Layout( void ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	float indent = 2.0f / (unsigned int)GraphicsModeInfo::GetCurrentMode()->GetWidth();
	PyEmbed::GetValue( GraphicsModeInfo::GetCurrentMode()->GetLcarsString(), "REPAIR_ST_INDENT_HORIZ", "f", &indent );
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	TGUIObject *leftEndCap = GetNthChild( STBUTTON_LEFT_END_CAP );
	TGUIObject *marker = GetNthChild( STBUTTON_MARKER );
	TGUIObject *body = GetNthChild( STBUTTON_BODY );
	TGUIObject *rightEndCap = GetNthChild( STBUTTON_RIGHT_END_CAP );
	STCheckbox *checkbox = STCheckbox::Cast( GetNthChild( STBUTTON_CHECKBOX ) );
	TGUIObject *icon = GetNthChild( STREPAIRBUTTON_ICON );

	marker->Resize( leftEndCap->GetWidth() * 0.25f, marker->GetHeight(), false );
	if ( useUIHeight == true ) {
		UpdateUIHeightColors();
	}
	if ( ( flags & STBF_FILL_PARENT_WIDTH ) && parent != NULL ) {
		Resize( parent->GetWidth() - indent - icon->GetWidth(), text->GetHeight() + DEFAULT_ST_INDENT_VERT * 4.0f, false );
	}
	if ( flags & STBF_SIZE_TO_TEXT ) {
		Resize( GetWidth(), GetHeight(), false );
	}

	leftEndCap->SetPosition( 0.0f, 0.0f, false );
	checkbox->SetPosition( leftEndCap->GetLeft(), leftEndCap->GetTop(), false );
	marker->SetPosition( leftEndCap->GetWidth() + leftEndCap->GetLeft() + indent, 0.0f, false );
	body->SetPosition( marker->GetWidth() + marker->GetLeft() + indent, 0.0f, false );
	rightEndCap->SetPosition( body->GetWidth() + body->GetLeft(), 0.0f, false );
	textPane->SetPosition( body->GetLeft(), ( body->GetHeight() - text->GetHeight() ) * 0.5f + body->GetTop(), false );
	text->SetPosition( indent, 0.0f, false );
	TGPane::Layout();
	icon->SetPosition( rightEndCap->GetWidth() + rightEndCap->GetLeft() + indent, body->GetHeight() * 0.5f - icon->GetHeight() * 0.5f, true );
}

/*
================
STRepairButton::Resize

STButton's sizing of the parts, leaving room for the damage icon when
the button fills its parent's width.
================
*/
// FUNCTION: STBC 0x0052ce10
void STRepairButton::Resize( float width, float height, bool layout ) {
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
	TGIcon *icon = TGIcon::Cast( GetNthChild( STREPAIRBUTTON_ICON ) );

	leftEndCap->SizeToArtwork( false );
	rightEndCap->SizeToArtwork( false );
	float leftWidth = leftEndCap->GetWidth();
	float rightWidth = rightEndCap->GetWidth();
	float bodyWidth;

	if ( flags & STBF_SIZE_TO_TEXT ) {
		newHeight = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
		leftWidth *= newHeight / leftEndCap->GetHeight();
		rightWidth *= newHeight / rightEndCap->GetHeight();
		bodyWidth = text->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 2.0f;
		newWidth = marker->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 2.0f + bodyWidth + rightWidth + leftWidth;
	} else {
		bodyWidth = width - leftWidth - DEFAULT_ST_INDENT_HORIZ * 2.0f - marker->GetWidth() - rightWidth;
	}
	if ( flags & STBF_FILL_PARENT_WIDTH ) {
		newHeight = text->GetHeight() + DEFAULT_ST_INDENT_VERT * 2.0f;
		leftWidth = newHeight / leftEndCap->GetHeight() * leftEndCap->GetWidth();
		rightWidth = newHeight / rightEndCap->GetHeight() * rightEndCap->GetWidth();
		if ( parent != NULL ) {
			float available = parent->GetWidth() - DEFAULT_ST_INDENT_HORIZ - icon->GetWidth();
			bodyWidth = available - leftWidth - DEFAULT_ST_INDENT_HORIZ * 2.0f - marker->GetWidth() - rightWidth;
			newWidth = icon->GetWidth() + DEFAULT_ST_INDENT_HORIZ + available;
		}
	}

	TGButtonBase::Resize( newWidth, newHeight, false );
	textPane->Resize( bodyWidth, newHeight, false );
	leftEndCap->Resize( leftWidth, newHeight, false );
	checkbox->Resize( leftEndCap->GetWidth(), leftEndCap->GetHeight(), false );
	marker->Resize( marker->GetWidth(), newHeight, false );
	body->Resize( bodyWidth, newHeight, false );
	rightEndCap->Resize( rightWidth, newHeight, false );
	JustifyText( false );
	if ( icon != NULL ) {
		icon->SizeToArtwork( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STRepairButton::UpdateIconColor
================
*/
// FUNCTION: STBC 0x0052d0e0
void STRepairButton::UpdateIconColor( ShipSubsystem *subsystem ) {
	TGIcon *icon = TGIcon::Cast( GetNthChild( STREPAIRBUTTON_ICON ) );
	if ( subsystem->GetCondition() <= 0.0f ) {
		icon->SetColor( &g_kDamageDisplayDestroyedColor );
	} else if ( subsystem->IsDisabled() == true ) {
		icon->SetColor( &g_kDamageDisplayDisabledColor );
	} else {
		icon->SetColor( &g_kDamageDisplayDamagedColor );
	}
}

/*
================
STRepairButton::HandleSubsystemStateChanged

Recolours the icon when the event is about the button's subsystem.
================
*/
// FUNCTION: STBC 0x0052d150
void STRepairButton::HandleSubsystemStateChanged( TGEvent *event ) {
	ShipSubsystem *subsystem = ShipSubsystem::Cast( event->GetSource() );
	if ( subsystem != NULL && subsystem->GetObjID() == subsystemID ) {
		UpdateIconColor( subsystem );
	}
	CallNextHandler( event );
}

/*
================
STRepairButton::LoadFontSize
================
*/
// FUNCTION: STBC 0x0052d190
void STRepairButton::LoadFontSize( void ) {
	if ( PyEmbed::GetValue( GraphicsModeInfo::GetCurrentMode()->GetLcarsModule(), "g_fRepairButtonFontSize", "f", &fontSize ) == -1 ) {
		PyErr_Print();
	}
}

/*
================
STRepairButton::UpdateFontSize
================
*/
// FUNCTION: STBC 0x0052d1d0
void STRepairButton::UpdateFontSize( void ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	if ( textPane == NULL ) {
		return;
	}
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	if ( text == NULL ) {
		return;
	}
	text->SetFontGroup( g_kFontManager.GetFontGroup( g_kFontManager.GetDefaultFont()->GetFontName(), fontSize ), true );
}

/*
================
STRepairButton::WriteToStream
================
*/
// FUNCTION: STBC 0x0052d240
bool STRepairButton::WriteToStream( TGStream *stream ) {
	if ( !STButton::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( subsystemID );
	return true;
}

/*
================
STRepairButton::ReadFromStream
================
*/
// FUNCTION: STBC 0x0052d270
bool STRepairButton::ReadFromStream( TGStream *stream ) {
	if ( !STButton::ReadFromStream( stream ) ) {
		return false;
	}
	subsystemID = stream->ReadID();
	return true;
}
