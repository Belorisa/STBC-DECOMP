/*
===============================================================================

	STRoundedButton.cpp

===============================================================================
*/

#include "STRoundedButton.h"
#include "STButton.h"
#include "STSubPane.h"
#include "STColors.h"
#include "InterfaceModule.h"
#include "TopWindow.h"
#include "PyEmbed.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "TGString.h"
#include "TGStream.h"
#include "TGBoolEvent.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGEventHandlerTable.h"
#include "NiPoint2.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

#include <iostream>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0052ec70 _$E33
// FUNCTION: STBC 0x0052ec80 _$E32
// FUNCTION: STBC 0x0052ec90 _$E37
// FUNCTION: STBC 0x0052eca0 _$E36
// FUNCTION: STBC 0x0052ecb0 _$E48
// FUNCTION: STBC 0x0052ecc0 _$E47
// FUNCTION: STBC 0x0052ecd0 _$E52
// FUNCTION: STBC 0x0052ece0 _$E51
// FUNCTION: STBC 0x0052ecf0 _$E56
// FUNCTION: STBC 0x0052ed00 _$E55
// FUNCTION: STBC 0x0052ed10 _$E60
// FUNCTION: STBC 0x0052ed20 _$E59
// FUNCTION: STBC 0x0052ed30 _$E184
// FUNCTION: STBC 0x0052ed40 _$E183
// FUNCTION: STBC 0x0052ed50 _$E126
// FUNCTION: STBC 0x0052ed60 _$E125
// FUNCTION: STBC 0x0052ed70 _$E139
// FUNCTION: STBC 0x0052ed80 _$E138
// FUNCTION: STBC 0x0052ed90 _$E189
// FUNCTION: STBC 0x0052eda0 _$E188
// FUNCTION: STBC 0x0052edb0 _$E194
// FUNCTION: STBC 0x0052edc0 _$E193
// FUNCTION: STBC 0x0052edd0 _$E198
// FUNCTION: STBC 0x0052ede0 _$E197
// FUNCTION: STBC 0x0052edf0 _$E202
// FUNCTION: STBC 0x0052ee00 _$E201
// FUNCTION: STBC 0x0052ee10 _$E209
// FUNCTION: STBC 0x0052ee20 _$E208
// FUNCTION: STBC 0x0052ee30 _$E290
// FUNCTION: STBC 0x0052ee40 _$E289
// FUNCTION: STBC 0x0052ee50 _$E294
// FUNCTION: STBC 0x0052ee60 _$E293
// FUNCTION: STBC 0x0052ee70 _$E84
// FUNCTION: STBC 0x0052ee80 _$E83
// FUNCTION: STBC 0x0052ee90 _$E276
// FUNCTION: STBC 0x0052eea0 _$E275
// FUNCTION: STBC 0x0052eeb0 _$E283
// FUNCTION: STBC 0x0052eec0 _$E282
// FUNCTION: STBC 0x0052eed0 _$E306
// FUNCTION: STBC 0x0052eee0 _$E305
// FUNCTION: STBC 0x0052eef0 _$E310
// FUNCTION: STBC 0x0052ef00 _$E309
// FUNCTION: STBC 0x0052ef10 _$E350
// FUNCTION: STBC 0x0052ef20 _$E349
// FUNCTION: STBC 0x0052ef30 _$E356
// FUNCTION: STBC 0x0052ef40 _$E355
// FUNCTION: STBC 0x0052ef50 _$E298
// FUNCTION: STBC 0x0052ef60 _$E297
// FUNCTION: STBC 0x0052ef70 _$E302
// FUNCTION: STBC 0x0052ef80 _$E301
// FUNCTION: STBC 0x0052ef90 _$E314
// FUNCTION: STBC 0x0052efa0 _$E313
// FUNCTION: STBC 0x0052efb0 _$E318
// FUNCTION: STBC 0x0052efc0 _$E317
// FUNCTION: STBC 0x0052efd0 _$E322
// FUNCTION: STBC 0x0052efe0 _$E321
// FUNCTION: STBC 0x0052eff0 _$E326
// FUNCTION: STBC 0x0052f000 _$E325
// FUNCTION: STBC 0x0052f010 _$E330
// FUNCTION: STBC 0x0052f020 _$E329
// FUNCTION: STBC 0x0052f030 _$E334
// FUNCTION: STBC 0x0052f040 _$E333
// FUNCTION: STBC 0x0052f050 _$E338
// FUNCTION: STBC 0x0052f060 _$E337
// FUNCTION: STBC 0x0052f070 _$E342
// FUNCTION: STBC 0x0052f080 _$E341
// FUNCTION: STBC 0x0052f090 _$E89
// FUNCTION: STBC 0x0052f0a0 _$E88
// FUNCTION: STBC 0x0052f0b0 _$E346
// FUNCTION: STBC 0x0052f0c0 _$E345
// FUNCTION: STBC 0x0052f0d0 _$E94
// FUNCTION: STBC 0x0052f0e0 _$E93
// FUNCTION: STBC 0x0052f0f0 _$E360
// FUNCTION: STBC 0x0052f100 _$E359
// FUNCTION: STBC 0x0052f110 _$E364
// FUNCTION: STBC 0x0052f120 _$E363
// FUNCTION: STBC 0x0052f130 _$E368
// FUNCTION: STBC 0x0052f140 _$E367
// FUNCTION: STBC 0x0052f150 _$E372
// FUNCTION: STBC 0x0052f160 _$E371
// FUNCTION: STBC 0x0052f170 _$E103
// FUNCTION: STBC 0x0052f180 _$E102
// FUNCTION: STBC 0x0052f190 _$E376
// FUNCTION: STBC 0x0052f1a0 _$E375
// FUNCTION: STBC 0x0052f1b0 _$E380
// FUNCTION: STBC 0x0052f1c0 _$E379
// FUNCTION: STBC 0x0052f1d0 _$E384
// FUNCTION: STBC 0x0052f1e0 _$E383
// FUNCTION: STBC 0x0052f1f0 _$E109
// FUNCTION: STBC 0x0052f200 _$E108
// FUNCTION: STBC 0x0052f210 _$E114
// FUNCTION: STBC 0x0052f220 _$E113
// FUNCTION: STBC 0x0052f230 _$E144
// FUNCTION: STBC 0x0052f240 _$E143
// FUNCTION: STBC 0x0052f280 _$E388
// FUNCTION: STBC 0x0052f290 _$E387
// FUNCTION: STBC 0x0052f2a0 _$E149
// FUNCTION: STBC 0x0052f2b0 _$E148
// FUNCTION: STBC 0x0052f2c0 _$E392
// FUNCTION: STBC 0x0052f2d0 _$E391
// FUNCTION: STBC 0x0052f2e0 _$E396
// FUNCTION: STBC 0x0052f2f0 _$E395
// FUNCTION: STBC 0x0052f300 _$E154
// FUNCTION: STBC 0x0052f310 _$E153
// FUNCTION: STBC 0x0052f320 _$E400
// FUNCTION: STBC 0x0052f330 _$E399
// FUNCTION: STBC 0x0052f340 _$E119
// FUNCTION: STBC 0x0052f350 _$E118
// FUNCTION: STBC 0x0052f360 _$E404
// FUNCTION: STBC 0x0052f370 _$E403
// FUNCTION: STBC 0x0052f380 _$E408
// FUNCTION: STBC 0x0052f390 _$E407
// FUNCTION: STBC 0x0052f3a0 _$E159
// FUNCTION: STBC 0x0052f3b0 _$E158
// FUNCTION: STBC 0x0052f3c0 _$E164
// FUNCTION: STBC 0x0052f3d0 _$E163
// FUNCTION: STBC 0x0052f3e0 _$E412
// FUNCTION: STBC 0x0052f3f0 _$E411
// FUNCTION: STBC 0x0052f400 _$E416
// FUNCTION: STBC 0x0052f410 _$E415
// FUNCTION: STBC 0x0052f420 _$E420
// FUNCTION: STBC 0x0052f430 _$E419
// FUNCTION: STBC 0x0052f440 _$E424
// FUNCTION: STBC 0x0052f450 _$E423
// FUNCTION: STBC 0x0052f460 _$E428
// FUNCTION: STBC 0x0052f470 _$E427
// FUNCTION: STBC 0x0052f480 _$E432
// FUNCTION: STBC 0x0052f490 _$E431
// FUNCTION: STBC 0x0052f4a0 _$E436
// FUNCTION: STBC 0x0052f4b0 _$E435
// FUNCTION: STBC 0x0052f4c0 _$E440
// FUNCTION: STBC 0x0052f4d0 _$E439
// FUNCTION: STBC 0x0052f4e0 _$E169
// FUNCTION: STBC 0x0052f4f0 _$E168
// FUNCTION: STBC 0x0052f500 _$E444
// FUNCTION: STBC 0x0052f510 _$E443
// FUNCTION: STBC 0x0052f520 _$E174
// FUNCTION: STBC 0x0052f530 _$E173
// FUNCTION: STBC 0x0052f540 _$E179
// FUNCTION: STBC 0x0052f550 _$E178
// FUNCTION: STBC 0x0052f560 _$E448
// FUNCTION: STBC 0x0052f570 _$E447
// FUNCTION: STBC 0x0052f580 _$E453
// FUNCTION: STBC 0x0052f590 _$E452
// FUNCTION: STBC 0x0052f5a0 _$E265
// FUNCTION: STBC 0x0052f5b0 _$E264

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0052ebf0 _$E458
// FUNCTION: STBC 0x0052ec00 _$E455
// FUNCTION: STBC 0x0052ec10 _$E457
// FUNCTION: STBC 0x0052ec20 _$E456
// FUNCTION: STBC 0x0052ec30 _$E464
// FUNCTION: STBC 0x0052ec40 _$E461
// FUNCTION: STBC 0x0052ec50 _$E463
// FUNCTION: STBC 0x0052ec60 _$E462

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x0052f250 _$E478
// FUNCTION: STBC 0x0052f270 _$E477

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0052dbb0 _$E470
// FUNCTION: STBC 0x0052dbc0 _$E466
// FUNCTION: STBC 0x0052dbd0 _$E469
// FUNCTION: STBC 0x0052dbe0 _$E467
// DATA: STBC 0x00989780
TGEventHandlerTable STRoundedButton::classHandlers;

/*
================
STRoundedButton::Cast

The object as a rounded button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0052dc00
STRoundedButton *STRoundedButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STROUNDEDBUTTON_TYPE ) ) {
		return (STRoundedButton *)object;
	}
	return NULL;
}

/*
================
STRoundedButton::STRoundedButton
================
*/
// FUNCTION: STBC 0x0052dc30 ??0STRoundedButton@@QAE@PADPAVTGEvent@@MM_N@Z
STRoundedButton::STRoundedButton( char *name, TGEvent *event, float width, float height, bool rightRounded ) : TGButtonBase( width, height ) {
	this->rightRounded = rightRounded;
	TGString string( name );
	Init( &string, event, width, height );
}

/*
================
STRoundedButton::STRoundedButton
================
*/
// FUNCTION: STBC 0x0052de80 ??0STRoundedButton@@QAE@PAVTGString@@PAVTGEvent@@MM_N@Z
STRoundedButton::STRoundedButton( TGString *name, TGEvent *event, float width, float height, bool rightRounded ) : TGButtonBase( width, height ) {
	this->rightRounded = rightRounded;
	Init( name, event, width, height );
}

/*
================
STRoundedButton::Init

Takes the first depth's colours of STButton and builds the name paragraph;
the pane is built by the first Resize.
================
*/
// FUNCTION: STBC 0x0052df90
void STRoundedButton::Init( TGString *name, TGEvent *event, float width, float height ) {
	SetBatchChildPolys( false );

	normalColor = g_kSTButtonColors[0][0];
	selectedColor = g_kSTButtonColors[0][1];
	highlightedColor = g_kSTButtonColors[0][2];
	disabledColor = g_kSTButtonColors[0][3];
	textColor = g_kSTMenuTextColor;
	disabledTextColor = g_kSTMenuTextColor;
	selectedTextColor = g_kSTMenuTextHighlightColor;
	highlightedTextColor = g_kTextHighlightColor;
	unknownD8 = 0;

	TGParagraph *text = new TGParagraph( name, 1.0f, &textColor, "", 0.0f, TGPF_READ_ONLY );
	AddChild( text, 0.0f, 0.0f, false );

	SetActivationEvent( event );
	Resize( width, height, false );
	TGUIObject::SetHighlighted( false );
	TGUIObject::SetEnabled( false );
	SetNotHighlighted( false );
	Layout();
}

/*
================
STRoundedButton::SetName
================
*/
// FUNCTION: STBC 0x0052e1b0 ?SetName@STRoundedButton@@QAEXPAVTGString@@_N@Z
void STRoundedButton::SetName( TGString *name, bool layout ) {
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	if ( text != NULL ) {
		text->SetString( name, true );
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
STRoundedButton::GetName
================
*/
// FUNCTION: STBC 0x0052e200 ?GetName@STRoundedButton@@QAEXPAVTGString@@@Z
void STRoundedButton::GetName( TGString *name ) {
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	if ( text != NULL ) {
		text->GetString( name );
	}
}

/*
================
STRoundedButton::GetDesiredSize
================
*/
// FUNCTION: STBC 0x0052e230 ?GetDesiredSize@STRoundedButton@@UAEXPAVNiPoint2@@@Z
void STRoundedButton::GetDesiredSize( NiPoint2 *size ) {
	size->x = GetWidth();
	size->y = GetHeight();
}

/*
================
STRoundedButton::GetUIHeight

The depth STButton takes its colours by; the rounded button never asks.
================
*/
// FUNCTION: STBC 0x0052e250
int STRoundedButton::GetUIHeight( void ) {
	int height = 0;
	TGPane *pane = parent;
	while ( pane != NULL && pane->IsTypeOf( STSUBPANE_TYPE ) ) {
		pane = pane->GetParent();
		height++;
		if ( pane == NULL ) {
			break;
		}
		pane = pane->GetParent();
	}
	return height;
}

/*
================
STRoundedButton::Layout

Puts the pane at the button's corner, the name in its middle, and colours
the button for its state.
================
*/
// FUNCTION: STBC 0x0052e280
void STRoundedButton::Layout( void ) {
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	TGPane *pane = TGPane::Cast( GetNthChild( STROUNDEDBUTTON_PANE ) );
	if ( pane != NULL && text != NULL ) {
		float textWidth = text->GetWidth();
		float textHeight = text->GetHeight();
		pane->SetPosition( 0.0f, 0.0f, false );
		text->SetPosition( ( pane->GetWidth() - textWidth ) * 0.5f, ( pane->GetHeight() - textHeight ) * 0.5f, false );
	}
	SetColorBasedOnFlags();
	TGPane::Layout();
}

/*
================
STRoundedButton::Resize

Has the scripts build the pane again at the new size, in the button's
colour, and puts the name in its middle.
================
*/
// FUNCTION: STBC 0x0052e320
void STRoundedButton::Resize( float width, float height, bool layout ) {
	// BUG: the name is looked up and never used.
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	TGPane *oldPane = TGPane::Cast( GetNthChild( STROUNDEDBUTTON_PANE ) );
	if ( oldPane != NULL ) {
		DeleteChild( oldPane, false );
	}

	PyObject *color = PyEmbed::Unknown6F9920( &g_kSTMenu1HighlightedBase, "NiColorA" );
	PyObject *shadow;
	if ( rightRounded ) {
		PyEmbed::CallFunction( "UIHelpers", "CreateRightRoundedButton", "O", &shadow, "ffO", width, height, color );
	} else {
		PyEmbed::CallFunction( "UIHelpers", "CreateRoundedButton", "O", &shadow, "ffO", width, height, color );
	}
	TGPane *pane = (TGPane *)PyEmbed::GetObjectPointer( shadow );
	Py_DECREF( shadow );
	Py_DECREF( color );

	InsertChild( 0x7FFFFFFF, pane, 0.0f, 0.0f, false );
	TGButtonBase::Resize( width, height, false );
	CenterText( false );
	if ( layout ) {
		Layout();
	}
}

/*
================
STRoundedButton::CenterText

Puts the name in the middle of the pane.
================
*/
// FUNCTION: STBC 0x0052e460
void STRoundedButton::CenterText( bool layout ) {
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	float textWidth = text->GetWidth();
	float textHeight = text->GetHeight();
	TGPane *pane = TGPane::Cast( GetNthChild( STROUNDEDBUTTON_PANE ) );
	text->SetPosition( ( pane->GetWidth() - textWidth ) * 0.5f, ( pane->GetHeight() - textHeight ) * 0.5f, true );
	if ( layout == true ) {
		text->Layout();
	}
}

/*
================
STRoundedButton::SetColor

Colours the icons of the pane; the name is coloured by the button's state.
================
*/
// FUNCTION: STBC 0x0052e4f0 ?SetColor@STRoundedButton@@QAEXPAVNiColorA@@@Z
void STRoundedButton::SetColor( NiColorA *color ) {
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	if ( text != NULL ) {
		text->SetColor( &textColor );
	}
	TGPane *pane = TGPane::Cast( GetNthChild( STROUNDEDBUTTON_PANE ) );
	if ( pane != NULL ) {
		for ( TGUIObject *child = pane->GetFirstChild(); child != NULL; child = pane->GetNextChild( child ) ) {
			if ( child->IsTypeOf( TGICON_TYPE ) ) {
				( (TGIcon *)child )->SetColor( color );
			}
		}
	}
}

/*
================
STRoundedButton::SetColorBasedOnFlags

Colours the button for its state.
================
*/
// FUNCTION: STBC 0x0052e580 ?SetColorBasedOnFlags@STRoundedButton@@QAEXXZ
void STRoundedButton::SetColorBasedOnFlags( void ) {
	NiColorA *color = &normalColor;
	NiColorA nameColor = textColor;
	if ( !( flags & TGUIO_ENABLED ) ) {
		color = &disabledColor;
		nameColor = disabledTextColor;
	} else if ( flags & TGUIO_SELECTED ) {
		color = &selectedColor;
		nameColor = selectedTextColor;
	} else if ( flags & TGUIO_HIGHLIGHTED ) {
		color = &highlightedColor;
		nameColor = highlightedTextColor;
	}
	SetColor( color );
	TGParagraph *text = TGParagraph::Cast( GetNthChild( STROUNDEDBUTTON_TEXT ) );
	if ( text != NULL ) {
		text->SetColor( &nameColor );
	}
}

/*
================
STRoundedButton::SetSelected
================
*/
// FUNCTION: STBC 0x0052e680
void STRoundedButton::SetSelected( bool layout ) {
	if ( !( flags & TGUIO_SELECTED ) ) {
		TGUIObject::SetSelected( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STRoundedButton::SetNotSelected
================
*/
// FUNCTION: STBC 0x0052e6a0
void STRoundedButton::SetNotSelected( bool layout ) {
	if ( flags & TGUIO_SELECTED ) {
		TGUIObject::SetNotSelected( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STRoundedButton::SetHighlighted
================
*/
// FUNCTION: STBC 0x0052e6c0
void STRoundedButton::SetHighlighted( bool layout ) {
	if ( !( flags & TGUIO_HIGHLIGHTED ) ) {
		TGUIObject::SetHighlighted( layout );
		SetColorBasedOnFlags();
		g_kInterfaceModule.playClickSound = true;
	}
}

/*
================
STRoundedButton::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x0052e6f0
void STRoundedButton::SetNotHighlighted( bool layout ) {
	if ( flags & TGUIO_HIGHLIGHTED ) {
		TGUIObject::SetNotHighlighted( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STRoundedButton::SetEnabled
================
*/
// FUNCTION: STBC 0x0052e710
void STRoundedButton::SetEnabled( bool layout ) {
	if ( !( flags & TGUIO_ENABLED ) ) {
		TGUIObject::SetEnabled( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STRoundedButton::SetDisabled
================
*/
// FUNCTION: STBC 0x0052e730
void STRoundedButton::SetDisabled( bool layout ) {
	if ( flags & TGUIO_ENABLED ) {
		TGButtonBase::SetDisabled( layout );
		SetColorBasedOnFlags();
	}
}

/*
================
STRoundedButton::GotTrueFocus
================
*/
// FUNCTION: STBC 0x0052e750
void STRoundedButton::GotTrueFocus( bool layout ) {
	TGUIObject::GotTrueFocus( false );
	SetHighlighted( false );
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STRoundedButton::LostTrueFocus
================
*/
// FUNCTION: STBC 0x0052e780
void STRoundedButton::LostTrueFocus( bool layout ) {
	TGUIObject::LostTrueFocus( layout );
	SetNotHighlighted( false );
}

/*
================
STRoundedButton::SendActivationEvent

Sends the activation event, plays the click, and offers
ET_ST_BUTTON_CLICKED to each parent below the top window until one handles
it.
================
*/
// FUNCTION: STBC 0x0052e7a0
void STRoundedButton::SendActivationEvent( void ) {
	TGButtonBase::SendActivationEvent();
	TGSound *sound = g_kSoundManager.GetSound( "UIButtonClicked" );
	if ( sound != NULL ) {
		sound->Play( true );
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
STRoundedButton::WriteToStream
================
*/
// FUNCTION: STBC 0x0052e850
bool STRoundedButton::WriteToStream( TGStream *stream ) {
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
	stream->WriteInt( unknownD8 );
	stream->WriteBool( rightRounded );
	return true;
}

/*
================
STRoundedButton::ReadFromStream
================
*/
// FUNCTION: STBC 0x0052ea30
bool STRoundedButton::ReadFromStream( TGStream *stream ) {
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
	unknownD8 = stream->ReadInt();
	rightRounded = stream->ReadBool();
	return true;
}
