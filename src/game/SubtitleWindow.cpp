/*
===============================================================================

	SubtitleWindow.cpp

===============================================================================
*/

#include "SubtitleWindow.h"
#include "GraphicsModeInfo.h"
#include "NiMath.h"
#include "OptionsWindow.h"
#include "PyEmbed.h"
#include "TGEventHandlerTable.h"
#include "TGFontManager.h"
#include "TGIcon.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "TopWindow.h"
#include <stdio.h>
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00508ae0 _$E279
// FUNCTION: STBC 0x00508b00 _$E280
// FUNCTION: STBC 0x00508af0 _$E281
// FUNCTION: STBC 0x00508ad0 _$E282
// FUNCTION: STBC 0x00508b20 _$E285
// FUNCTION: STBC 0x00508b40 _$E286
// FUNCTION: STBC 0x00508b30 _$E287
// FUNCTION: STBC 0x00508b10 _$E288

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00507630 _$E479
// FUNCTION: STBC 0x00507650 _$E480
// FUNCTION: STBC 0x00507640 _$E482
// FUNCTION: STBC 0x00507620 _$E483
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00508b50 _$E33
// FUNCTION: STBC 0x00508b60 _$E32
// FUNCTION: STBC 0x00508b70 _$E37
// FUNCTION: STBC 0x00508b80 _$E36
// FUNCTION: STBC 0x00508b90 _$E48
// FUNCTION: STBC 0x00508ba0 _$E47
// FUNCTION: STBC 0x00508bb0 _$E52
// FUNCTION: STBC 0x00508bc0 _$E51
// FUNCTION: STBC 0x00508bd0 _$E56
// FUNCTION: STBC 0x00508be0 _$E55
// FUNCTION: STBC 0x00508bf0 _$E60
// FUNCTION: STBC 0x00508c00 _$E59
// FUNCTION: STBC 0x00508c10 _$E184
// FUNCTION: STBC 0x00508c20 _$E183
// FUNCTION: STBC 0x00508c30 _$E126
// FUNCTION: STBC 0x00508c40 _$E125
// FUNCTION: STBC 0x00508c50 _$E139
// FUNCTION: STBC 0x00508c60 _$E138
// FUNCTION: STBC 0x00508c70 _$E189
// FUNCTION: STBC 0x00508c80 _$E188
// FUNCTION: STBC 0x00508c90 _$E194
// FUNCTION: STBC 0x00508ca0 _$E193
// FUNCTION: STBC 0x00508cb0 _$E198
// FUNCTION: STBC 0x00508cc0 _$E197
// FUNCTION: STBC 0x00508cd0 _$E202
// FUNCTION: STBC 0x00508ce0 _$E201
// FUNCTION: STBC 0x00508cf0 _$E209
// FUNCTION: STBC 0x00508d00 _$E208
// FUNCTION: STBC 0x00508d10 _$E310
// FUNCTION: STBC 0x00508d20 _$E309
// FUNCTION: STBC 0x00508d30 _$E314
// FUNCTION: STBC 0x00508d40 _$E313
// FUNCTION: STBC 0x00508d50 _$E84
// FUNCTION: STBC 0x00508d60 _$E83
// FUNCTION: STBC 0x00508d70 _$E296
// FUNCTION: STBC 0x00508d80 _$E295
// FUNCTION: STBC 0x00508e30 _$E303
// FUNCTION: STBC 0x00508e40 _$E302
// FUNCTION: STBC 0x00508e50 _$E326
// FUNCTION: STBC 0x00508e60 _$E325
// FUNCTION: STBC 0x00508e70 _$E330
// FUNCTION: STBC 0x00508e80 _$E329
// FUNCTION: STBC 0x00508e90 _$E370
// FUNCTION: STBC 0x00508ea0 _$E369
// FUNCTION: STBC 0x00508eb0 _$E376
// FUNCTION: STBC 0x00508ec0 _$E375
// FUNCTION: STBC 0x00508ed0 _$E318
// FUNCTION: STBC 0x00508ee0 _$E317
// FUNCTION: STBC 0x00508ef0 _$E322
// FUNCTION: STBC 0x00508f00 _$E321
// FUNCTION: STBC 0x00508f10 _$E334
// FUNCTION: STBC 0x00508f20 _$E333
// FUNCTION: STBC 0x00508f60 _$E338
// FUNCTION: STBC 0x00508f70 _$E337
// FUNCTION: STBC 0x00508f80 _$E342
// FUNCTION: STBC 0x00508f90 _$E341
// FUNCTION: STBC 0x00508fa0 _$E346
// FUNCTION: STBC 0x00508fb0 _$E345
// FUNCTION: STBC 0x00508fc0 _$E350
// FUNCTION: STBC 0x00508fd0 _$E349
// FUNCTION: STBC 0x00508fe0 _$E354
// FUNCTION: STBC 0x00508ff0 _$E353
// FUNCTION: STBC 0x00509000 _$E358
// FUNCTION: STBC 0x00509010 _$E357
// FUNCTION: STBC 0x00509020 _$E362
// FUNCTION: STBC 0x00509030 _$E361
// FUNCTION: STBC 0x00509040 _$E89
// FUNCTION: STBC 0x00509050 _$E88
// FUNCTION: STBC 0x00509060 _$E366
// FUNCTION: STBC 0x00509070 _$E365
// FUNCTION: STBC 0x00509080 _$E94
// FUNCTION: STBC 0x00509090 _$E93
// FUNCTION: STBC 0x005090a0 _$E380
// FUNCTION: STBC 0x005090b0 _$E379
// FUNCTION: STBC 0x005090c0 _$E384
// FUNCTION: STBC 0x005090d0 _$E383
// FUNCTION: STBC 0x005090e0 _$E388
// FUNCTION: STBC 0x005090f0 _$E387
// FUNCTION: STBC 0x00509100 _$E392
// FUNCTION: STBC 0x00509110 _$E391
// FUNCTION: STBC 0x00509120 _$E103
// FUNCTION: STBC 0x00509130 _$E102
// FUNCTION: STBC 0x00509140 _$E396
// FUNCTION: STBC 0x00509150 _$E395
// FUNCTION: STBC 0x00509160 _$E400
// FUNCTION: STBC 0x00509170 _$E399
// FUNCTION: STBC 0x00509180 _$E404
// FUNCTION: STBC 0x00509190 _$E403
// FUNCTION: STBC 0x005091a0 _$E109
// FUNCTION: STBC 0x005091b0 _$E108
// FUNCTION: STBC 0x005091f0 _$E114
// FUNCTION: STBC 0x00509200 _$E113
// FUNCTION: STBC 0x00509210 _$E144
// FUNCTION: STBC 0x00509220 _$E143
// FUNCTION: STBC 0x00509230 _$E408
// FUNCTION: STBC 0x00509240 _$E407
// FUNCTION: STBC 0x00509250 _$E149
// FUNCTION: STBC 0x00509260 _$E148
// FUNCTION: STBC 0x00509270 _$E412
// FUNCTION: STBC 0x00509280 _$E411
// FUNCTION: STBC 0x00509290 _$E416
// FUNCTION: STBC 0x005092a0 _$E415
// FUNCTION: STBC 0x005092b0 _$E154
// FUNCTION: STBC 0x005092c0 _$E153
// FUNCTION: STBC 0x005092d0 _$E420
// FUNCTION: STBC 0x005092e0 _$E419
// FUNCTION: STBC 0x005092f0 _$E119
// FUNCTION: STBC 0x00509300 _$E118
// FUNCTION: STBC 0x00509310 _$E424
// FUNCTION: STBC 0x00509320 _$E423
// FUNCTION: STBC 0x00509330 _$E428
// FUNCTION: STBC 0x00509340 _$E427
// FUNCTION: STBC 0x00509350 _$E159
// FUNCTION: STBC 0x00509360 _$E158
// FUNCTION: STBC 0x00509370 _$E164
// FUNCTION: STBC 0x00509380 _$E163
// FUNCTION: STBC 0x00509390 _$E432
// FUNCTION: STBC 0x005093a0 _$E431
// FUNCTION: STBC 0x005093b0 _$E436
// FUNCTION: STBC 0x005093c0 _$E435
// FUNCTION: STBC 0x005093d0 _$E440
// FUNCTION: STBC 0x005093e0 _$E439
// FUNCTION: STBC 0x005093f0 _$E444
// FUNCTION: STBC 0x00509400 _$E443
// FUNCTION: STBC 0x00509410 _$E448
// FUNCTION: STBC 0x00509420 _$E447
// FUNCTION: STBC 0x00509430 _$E452
// FUNCTION: STBC 0x00509440 _$E451
// FUNCTION: STBC 0x00509450 _$E456
// FUNCTION: STBC 0x00509460 _$E455
// FUNCTION: STBC 0x00509470 _$E460
// FUNCTION: STBC 0x00509480 _$E459
// FUNCTION: STBC 0x00509490 _$E169
// FUNCTION: STBC 0x005094a0 _$E168
// FUNCTION: STBC 0x005094b0 _$E464
// FUNCTION: STBC 0x005094c0 _$E463
// FUNCTION: STBC 0x005094d0 _$E174
// FUNCTION: STBC 0x005094e0 _$E173
// FUNCTION: STBC 0x005094f0 _$E179
// FUNCTION: STBC 0x00509500 _$E178
// FUNCTION: STBC 0x00509510 _$E468
// FUNCTION: STBC 0x00509520 _$E467
// FUNCTION: STBC 0x00509530 _$E473
// FUNCTION: STBC 0x00509540 _$E472
// FUNCTION: STBC 0x00509550 _$E477
// FUNCTION: STBC 0x00509560 _$E476

// DATA: STBC 0x009873e8
TGEventHandlerTable SubtitleWindow::classHandlers;

// The layout's positions and sizes start as NI_INFINITY, to be read from the layout script.
// FUNCTION: STBC 0x005076b0 _$E485
// FUNCTION: STBC 0x005076a0 _$E486
// DATA: STBC 0x009873bc
float SubtitleWindow::bridgeXPos = NI_INFINITY;
// FUNCTION: STBC 0x005076d0 _$E488
// FUNCTION: STBC 0x005076c0 _$E489
// DATA: STBC 0x009873c8
float SubtitleWindow::tacticalXPos = NI_INFINITY;
// FUNCTION: STBC 0x005076f0 _$E491
// FUNCTION: STBC 0x005076e0 _$E492
// DATA: STBC 0x009873b0
float SubtitleWindow::felixXPos = NI_INFINITY;
// FUNCTION: STBC 0x00507710 _$E494
// FUNCTION: STBC 0x00507700 _$E495
// DATA: STBC 0x00987348
float SubtitleWindow::mapXPos = NI_INFINITY;
// FUNCTION: STBC 0x00507730 _$E497
// FUNCTION: STBC 0x00507720 _$E498
// DATA: STBC 0x0098741c
float SubtitleWindow::cinematicXPos = NI_INFINITY;
// FUNCTION: STBC 0x00507750 _$E500
// FUNCTION: STBC 0x00507740 _$E501
// DATA: STBC 0x009873dc
float SubtitleWindow::bridgeCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x00507770 _$E503
// FUNCTION: STBC 0x00507760 _$E504
// DATA: STBC 0x009873a0
float SubtitleWindow::tacticalCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x00507790 _$E506
// FUNCTION: STBC 0x00507780 _$E507
// DATA: STBC 0x00987344
float SubtitleWindow::specialFelixCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x005077b0 _$E509
// FUNCTION: STBC 0x005077a0 _$E510
// DATA: STBC 0x00987424
float SubtitleWindow::felixCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x005077d0 _$E512
// FUNCTION: STBC 0x005077c0 _$E513
// DATA: STBC 0x00987480
float SubtitleWindow::mapCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x005077f0 _$E515
// FUNCTION: STBC 0x005077e0 _$E516
// DATA: STBC 0x00987440
float SubtitleWindow::cinematicCenterYPos = NI_INFINITY;
// FUNCTION: STBC 0x00507810 _$E518
// FUNCTION: STBC 0x00507800 _$E519
// DATA: STBC 0x00987384
float SubtitleWindow::bridgeSubtitleWidth = NI_INFINITY;
// FUNCTION: STBC 0x00507830 _$E521
// FUNCTION: STBC 0x00507820 _$E522
// DATA: STBC 0x0098743c
float SubtitleWindow::tacticalSubtitleWidth = NI_INFINITY;
// FUNCTION: STBC 0x00507850 _$E524
// FUNCTION: STBC 0x00507840 _$E525
// DATA: STBC 0x00987368
float SubtitleWindow::felixSubtitleWidth = NI_INFINITY;
// FUNCTION: STBC 0x00507870 _$E527
// FUNCTION: STBC 0x00507860 _$E528
// DATA: STBC 0x00987370
float SubtitleWindow::mapSubtitleWidth = NI_INFINITY;
// FUNCTION: STBC 0x00507890 _$E530
// FUNCTION: STBC 0x00507880 _$E531
// DATA: STBC 0x00987404
float SubtitleWindow::cinematicSubtitleWidth = NI_INFINITY;
// FUNCTION: STBC 0x005078b0 _$E533
// FUNCTION: STBC 0x005078a0 _$E534
// DATA: STBC 0x00987334
float SubtitleWindow::widthMinusTextWidth = NI_INFINITY;
// FUNCTION: STBC 0x005078d0 _$E536
// FUNCTION: STBC 0x005078c0 _$E537
// DATA: STBC 0x009873c4
float SubtitleWindow::maxSubtitleHeight = NI_INFINITY;
// FUNCTION: STBC 0x005078f0 _$E539
// FUNCTION: STBC 0x005078e0 _$E540
// DATA: STBC 0x0098740c
float SubtitleWindow::felixMaxHeight = NI_INFINITY;

/*
================
SubtitleWindow::Cast

The object as a SubtitleWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00507670
SubtitleWindow *SubtitleWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SUBTITLEWINDOW_TYPE ) ) {
		return (SubtitleWindow *)object;
	}
	return NULL;
}

/*
================
SubtitleWindow::SubtitleWindow

The window covers the screen, with the line pane and the background as its
children, placed for the bridge.
================
*/
// FUNCTION: STBC 0x00507900
SubtitleWindow::SubtitleWindow( void ) : MainWindow( MWT_SUBTITLE, 0.0f, 0.0f ) {
	if ( bridgeXPos == NI_INFINITY ) {
		LoadLayout();
	}
	SetBatchChildPolys( false );
	linePane = new TGPane;
	AddChild( linePane, 0.0f, 0.0f, false );
	char *iconGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	background = new TGIcon( iconGroup, BACKGROUND_ICON );
	AddChild( background, 0.0f, 0.0f, false );
	SetPositionForMode( SM_BRIDGE, false );
	SetPositionForMode( SM_NONFELIX, false );
	Resize( 1.0f, 1.0f, false );
	SetPosition( 0.0f, 0.0f, false );
	on = true;
	felix = false;
	cinematic = false;
	SetVisible( true );
	SetNoFocus();
}

/*
================
SubtitleWindow::GetObjType
================
*/
// FUNCTION: STBC 0x00507a80
int SubtitleWindow::GetObjType( void ) {
	return SUBTITLEWINDOW_TYPE;
}

/*
================
SubtitleWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00507a90
bool SubtitleWindow::IsTypeOf( int type ) {
	if ( type == SUBTITLEWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
SubtitleWindow::GetClassName
================
*/
// FUNCTION: STBC 0x00507af0
char *SubtitleWindow::GetClassName( void ) {
	return "SubtitleWindow";
}

/*
================
SubtitleWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00507b00
char *SubtitleWindow::GetSwigTypeName( void ) {
	return "_p_SubtitleWindow";
}

/*
================
SubtitleWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00507b10
char *SubtitleWindow::GetPythonClassName( void ) {
	return "SubtitleWindowPtr";
}

/*
================
SubtitleWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00507b20
TGEventHandlerTable *SubtitleWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
SubtitleWindow::SetPositionForMode

Places the window for the view. While a cinematic has it, only
SM_CINEMATIC, SM_END_CINEMATIC or force move it; SM_BRIDGE keeps the
Felix place until SM_NONFELIX.
================
*/
// FUNCTION: STBC 0x00507b80
void SubtitleWindow::SetPositionForMode( int newMode, bool force ) {
	unsigned int i;

	switch ( newMode ) {
		case SM_NONFELIX:
			if ( !cinematic || force ) {
				felix = false;
				mode = newMode;
			}
			// fall through
		case SM_BRIDGE:
			if ( ( !cinematic || force ) && !felix ) {
				left = bridgeXPos;
				centerY = bridgeCenterYPos;
				maxHeight = maxSubtitleHeight;
				width = bridgeSubtitleWidth;
				for ( i = 0; i < lines.GetSize(); i++ ) {
					if ( lines.GetAt( i ) != NULL ) {
						lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
					}
				}
				mode = newMode;
				break;
			}
			// fall through
		case SM_FELIX:
			if ( cinematic && !force ) {
				break;
			}
			felix = true;
			left = felixXPos;
			centerY = felixCenterYPos;
			maxHeight = felixMaxHeight;
			width = felixSubtitleWidth;
			for ( i = 0; i < lines.GetSize(); i++ ) {
				if ( lines.GetAt( i ) != NULL ) {
					lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
				}
			}
			mode = newMode;
			break;
		case SM_SPECIAL_FELIX:
			if ( cinematic && !force ) {
				break;
			}
			felix = true;
			left = felixXPos;
			centerY = specialFelixCenterYPos;
			maxHeight = felixMaxHeight;
			width = felixSubtitleWidth;
			for ( i = 0; i < lines.GetSize(); i++ ) {
				if ( lines.GetAt( i ) != NULL ) {
					lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
				}
			}
			mode = newMode;
			break;
		case SM_TACTICAL:
			if ( cinematic && !force ) {
				break;
			}
			left = tacticalXPos;
			centerY = tacticalCenterYPos;
			maxHeight = maxSubtitleHeight;
			width = tacticalSubtitleWidth;
			for ( i = 0; i < lines.GetSize(); i++ ) {
				if ( lines.GetAt( i ) != NULL ) {
					lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
				}
			}
			mode = newMode;
			break;
		case SM_MAP:
			if ( cinematic && !force ) {
				break;
			}
			left = mapXPos;
			centerY = mapCenterYPos;
			maxHeight = maxSubtitleHeight;
			width = mapSubtitleWidth;
			for ( i = 0; i < lines.GetSize(); i++ ) {
				if ( lines.GetAt( i ) != NULL ) {
					lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
				}
			}
			mode = newMode;
			break;
		case SM_END_CINEMATIC:
			cinematic = false;
			mode = newMode;
			break;
		case SM_CINEMATIC:
			cinematic = true;
			left = cinematicXPos;
			centerY = cinematicCenterYPos;
			maxHeight = maxSubtitleHeight;
			width = cinematicSubtitleWidth;
			for ( i = 0; i < lines.GetSize(); i++ ) {
				if ( lines.GetAt( i ) != NULL ) {
					lines.GetAt( i )->SetWrapWidth( width - widthMinusTextWidth, false );
				}
			}
			mode = newMode;
			break;
	}
	Layout();
}

/*
================
SubtitleWindow::SetOff

Takes out every line and turns the window off.
================
*/
// FUNCTION: STBC 0x00507ed0
void SubtitleWindow::SetOff( void ) {
	while ( lines.GetEffectiveSize() > 0 && lines.GetAt( 0 ) != NULL ) {
		RemoveParagraph( lines.GetAt( 0 )->GetObjID() );
		lines.Compact();
	}
	on = false;
}

/*
================
SubtitleWindow::AddLine
================
*/
// FUNCTION: STBC 0x00507f80
long SubtitleWindow::AddLine( TGString *text ) {
	if ( !on ) {
		return 0;
	}
	return AddParagraph( new TGParagraph( text, width - widthMinusTextWidth, NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP ) );
}

/*
================
SubtitleWindow::AddParagraph

Shows the window, or has the options window show it when it closes.
================
*/
// FUNCTION: STBC 0x00508000
// PARKED: register allocation: retail keeps this in ebx and saves ebp/esi/edi after the first Resize; the same calls, inlines and blocks (IsOn, GetTopWindow, SetAtGrow and a shared Layout all move the inline budget)
long SubtitleWindow::AddParagraph( TGParagraph *paragraph ) {
	if ( !on ) {
		return 0;
	}
	if ( lines.GetSize() == 0 ) {
		linePane->Resize( width, 0.0f );
	}
	linePane->InsertChild( 0x7FFFFFFF, paragraph, widthMinusTextWidth * 0.5f, linePane->GetHeight(), false );
	lines.Add( paragraph );
	if ( on ) {
		OptionsWindow *options = (OptionsWindow *)TopWindow::topWindow->FindMainWindow( MWT_OPTIONS );
		if ( options->IsVisible() ) {
			options->AddToPreviouslyVisible( this );
			Layout();
			return paragraph->GetObjID();
		}
		SetVisible();
		Layout();
	}
	return paragraph->GetObjID();
}

/*
================
SubtitleWindow::RemoveParagraph

With no lines left the pane and the background shrink to nothing.
================
*/
// FUNCTION: STBC 0x00508120
void SubtitleWindow::RemoveParagraph( long id ) {
	if ( id != 0 ) {
		for ( unsigned int i = 0; i < lines.GetSize(); i++ ) {
			TGParagraph *paragraph = lines.GetAt( i );
			if ( paragraph->GetObjID() == id ) {
				linePane->RemoveChild( paragraph, false );
				lines.RemoveAt( i );
				lines.Compact();
				float dy = paragraph->GetHeight() * -1.0f;
				delete paragraph;
				if ( i < lines.GetSize() ) {
					MoveLines( dy, i, lines.GetSize() - 1 );
				}
				break;
			}
		}
	}
	if ( lines.GetSize() == 0 ) {
		linePane->Resize( width, 0.0f );
		background->Resize( width, 0.0f );
		return;
	}
	Layout();
}

/*
================
SubtitleWindow::Layout

Stacks the lines, then centers them on centerY with the background behind
them, no taller than maxHeight.
================
*/
// FUNCTION: STBC 0x00508280
void SubtitleWindow::Layout( void ) {
	float height = 0.0f;
	for ( unsigned int i = 0; i < lines.GetSize(); i++ ) {
		TGParagraph *line = lines.GetAt( i );
		height += line->GetHeight();
		line->SetWrapWidth( width - widthMinusTextWidth, false );
	}
	linePane->Resize( width, height, false );
	float shownHeight = height < maxHeight ? height : maxHeight;
	if ( mode == SM_CINEMATIC ) {
		linePane->SetPosition( left, centerY - shownHeight * 0.5f, false );
	} else {
		linePane->SetPosition( left, centerY - shownHeight * 0.5f - linePane->GetHeight(), false );
	}
	background->Resize( width, shownHeight, false );
	background->SetPosition( left, linePane->GetTop(), false );
	MainWindow::Layout();
}

/*
================
SubtitleWindow::SetVisible

Gives the lines and their paragraphs the current default font first.
================
*/
// FUNCTION: STBC 0x00508370
void SubtitleWindow::SetVisible( bool layout ) {
	for ( int i = 0; i < linePane->GetNumChildren(); i++ ) {
		TGParagraph *line = TGParagraph::Cast( linePane->GetNthChild( i ) );
		if ( line != NULL && line->GetNumChildren() != 0 ) {
			for ( int j = 0; j < line->GetNumChildren(); j++ ) {
				TGParagraph *paragraph = TGParagraph::Cast( line->GetNthChild( j ) );
				if ( paragraph->GetFontGroup() != g_kFontManager.GetDefaultFont() ) {
					paragraph->SetFontGroup( g_kFontManager.GetDefaultFont(), false );
					paragraph->RecalcBounds( false );
				}
			}
			if ( line->GetFontGroup() != g_kFontManager.GetDefaultFont() ) {
				line->SetFontGroup( g_kFontManager.GetDefaultFont(), false );
				line->RecalcBounds( true );
			}
		}
	}
	MainWindow::SetVisible( layout );
}

/*
================
SubtitleWindow::MoveLines
================
*/
// FUNCTION: STBC 0x005084e0
void SubtitleWindow::MoveLines( float dy, unsigned int first, unsigned int last ) {
	for ( unsigned int i = first; i <= last; i++ ) {
		lines.GetAt( i )->Move( 0.0f, dy, false );
	}
}

/*
================
SubtitleWindow::LoadLayout
================
*/
// FUNCTION: STBC 0x00508520
void SubtitleWindow::LoadLayout( void ) {
	char *module = GraphicsModeInfo::GetCurrentMode()->GetLcarsModule();
	PyEmbed::GetValue( module, "SUBTITLE_BRIDGE_X_POS", "f", &bridgeXPos );
	PyEmbed::GetValue( module, "SUBTITLE_TACTICAL_X_POS", "f", &tacticalXPos );
	PyEmbed::GetValue( module, "SUBTITLE_FELIX_X_POS", "f", &felixXPos );
	PyEmbed::GetValue( module, "SUBTITLE_MAP_X_POS", "f", &mapXPos );
	PyEmbed::GetValue( module, "SUBTITLE_CINEMATIC_X_POS", "f", &cinematicXPos );
	PyEmbed::GetValue( module, "SUBTITLE_BRIDGE_CENTER_Y_POS", "f", &bridgeCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_TACTICAL_CENTER_Y_POS", "f", &tacticalCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_FELIX_CENTER_Y_POS", "f", &felixCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_SPECIAL_FELIX_CENTER_Y_POS", "f", &specialFelixCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_MAP_CENTER_Y_POS", "f", &mapCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_CINEMATIC_CENTER_Y_POS", "f", &cinematicCenterYPos );
	PyEmbed::GetValue( module, "SUBTITLE_BRIDGE_SUBTITLE_WIDTH", "f", &bridgeSubtitleWidth );
	PyEmbed::GetValue( module, "SUBTITLE_TACTICAL_SUBTITLE_WIDTH", "f", &tacticalSubtitleWidth );
	PyEmbed::GetValue( module, "SUBTITLE_FELIX_SUBTITLE_WIDTH", "f", &felixSubtitleWidth );
	PyEmbed::GetValue( module, "SUBTITLE_MAP_SUBTITLE_WIDTH", "f", &mapSubtitleWidth );
	PyEmbed::GetValue( module, "SUBTITLE_CINEMATIC_SUBTITLE_WIDTH", "f", &cinematicSubtitleWidth );
	PyEmbed::GetValue( module, "SUBTITLE_WIDTH_MINUS_TEXT_WIDTH", "f", &widthMinusTextWidth );
	PyEmbed::GetValue( module, "SUBTITLE_MAX_HEIGHT", "f", &maxSubtitleHeight );
	PyEmbed::GetValue( module, "SUBTITLE_FELIX_MAX_HEIGHT", "f", &felixMaxHeight );
}

/*
================
SubtitleWindow::ResizeUI
================
*/
// FUNCTION: STBC 0x005086e0
void SubtitleWindow::ResizeUI( void ) {
	LoadLayout();
	Layout();
}

/*
================
SubtitleWindow::RepositionUI
================
*/
// FUNCTION: STBC 0x00508700
void SubtitleWindow::RepositionUI( void ) {
	Layout();
}

/*
================
SubtitleWindow::WriteToStream

The lines and the panes are saved by their IDs.
================
*/
// FUNCTION: STBC 0x00508710
bool SubtitleWindow::WriteToStream( TGStream *stream ) {
	char message[256];

	if ( !MainWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( on );
	stream->WriteBool( felix );
	stream->WriteBool( cinematic );
	stream->WriteFloat( left );
	stream->WriteFloat( centerY );
	stream->WriteFloat( maxHeight );
	stream->WriteFloat( width );
	stream->WriteLong( mode );
	int count = lines.GetSize();
	// The message goes nowhere in this build.
	sprintf( message, "Mode %d, saving %d lines\n", mode, count );
	stream->WriteLong( count );
	for ( int i = 0; i < count; i++ ) {
		TGParagraph *line = lines.GetAt( i );
		if ( line != NULL ) {
			stream->WriteID( line->GetObjID() );
		} else {
			stream->WriteID( 0 );
		}
	}
	stream->WriteID( linePane->GetObjID() );
	stream->WriteID( background->GetObjID() );
	return true;
}

/*
================
SubtitleWindow::ReadFromStream

The lines and the panes hold their IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x00508810
bool SubtitleWindow::ReadFromStream( TGStream *stream ) {
	char message[256];

	if ( !MainWindow::ReadFromStream( stream ) ) {
		return false;
	}
	on = stream->ReadBool();
	felix = stream->ReadBool();
	cinematic = stream->ReadBool();
	left = stream->ReadFloat();
	centerY = stream->ReadFloat();
	maxHeight = stream->ReadFloat();
	width = stream->ReadFloat();
	mode = stream->ReadLong();
	int count = stream->ReadLong();
	// The message goes nowhere in this build.
	sprintf( message, "Mode %d, loading %d lines\n", mode, count );
	if ( count > 0 ) {
		lines.SetSize( count );
		for ( int i = 0; i < count; i++ ) {
			lines.SetAt( i, (TGParagraph *)stream->ReadID() );
		}
	}
	linePane = (TGPane *)stream->ReadID();
	background = (TGIcon *)stream->ReadID();
	return true;
}

/*
================
SubtitleWindow::FixUpPointers

Waits for the objects of the lines and the panes.
================
*/
// FUNCTION: STBC 0x005089b0
bool SubtitleWindow::FixUpPointers( void ) {
	if ( !MainWindow::FixUpPointers() ) {
		return false;
	}
	int count = lines.GetSize();
	for ( int i = 0; i < count; i++ ) {
		if ( lines.GetAt( i ) != NULL && TGObject::GetTGObjectPtr( (long)lines.GetAt( i ) ) == NULL ) {
			return false;
		}
	}
	if ( linePane != NULL && TGObject::GetTGObjectPtr( (long)linePane ) == NULL ) {
		return false;
	}
	if ( background != NULL && TGObject::GetTGObjectPtr( (long)background ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
SubtitleWindow::PostFixUp

Turns the IDs ReadFromStream left back into the objects.
================
*/
// FUNCTION: STBC 0x00508a30
void SubtitleWindow::PostFixUp( void ) {
	MainWindow::PostFixUp();
	int count = lines.GetSize();
	for ( int i = 0; i < count; i++ ) {
		lines.SetAt( i, (TGParagraph *)TGObject::GetTGObjectPtr( (long)lines.GetAt( i ) ) );
	}
	linePane = (TGPane *)TGObject::GetTGObjectPtr( (long)linePane );
	background = (TGIcon *)TGObject::GetTGObjectPtr( (long)background );
}
