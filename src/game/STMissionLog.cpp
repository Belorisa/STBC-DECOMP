/*
===============================================================================

	STMissionLog.cpp

	The mission log: lines of text in a scrolling window.

===============================================================================
*/

#include "STMissionLog.h"
#include "STStylizedWindow.h"
#include "STSubPane.h"
#include "TGConfigMapping.h"
#include "TGParagraph.h"
#include "STButton.h"
#include "STColors.h"
#include "ModalDialogWindow.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGStream.h"
#include <iostream>
#include "TopWindow.h"
#include "InterfaceModule.h"
#include "TGEventHandlerTable.h"
#include "UtopiaModule.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005284b0 _$E470
// FUNCTION: STBC 0x005284d0 _$E469
// FUNCTION: STBC 0x00529530 _$E33
// FUNCTION: STBC 0x00529540 _$E32
// FUNCTION: STBC 0x00529550 _$E37
// FUNCTION: STBC 0x00529560 _$E36
// FUNCTION: STBC 0x00529570 _$E48
// FUNCTION: STBC 0x00529580 _$E47
// FUNCTION: STBC 0x00529590 _$E52
// FUNCTION: STBC 0x005295a0 _$E51
// FUNCTION: STBC 0x005295b0 _$E56
// FUNCTION: STBC 0x005295c0 _$E55
// FUNCTION: STBC 0x005295d0 _$E60
// FUNCTION: STBC 0x005295e0 _$E59
// FUNCTION: STBC 0x005295f0 _$E184
// FUNCTION: STBC 0x00529600 _$E183
// FUNCTION: STBC 0x00529610 _$E126
// FUNCTION: STBC 0x00529620 _$E125
// FUNCTION: STBC 0x00529630 _$E139
// FUNCTION: STBC 0x00529640 _$E138
// FUNCTION: STBC 0x00529650 _$E189
// FUNCTION: STBC 0x00529660 _$E188
// FUNCTION: STBC 0x00529670 _$E194
// FUNCTION: STBC 0x00529680 _$E193
// FUNCTION: STBC 0x00529690 _$E198
// FUNCTION: STBC 0x005296a0 _$E197
// FUNCTION: STBC 0x005296b0 _$E202
// FUNCTION: STBC 0x005296c0 _$E201
// FUNCTION: STBC 0x005296d0 _$E209
// FUNCTION: STBC 0x005296e0 _$E208
// FUNCTION: STBC 0x005296f0 _$E289
// FUNCTION: STBC 0x00529700 _$E288
// FUNCTION: STBC 0x00529710 _$E293
// FUNCTION: STBC 0x00529720 _$E292
// FUNCTION: STBC 0x00529730 _$E84
// FUNCTION: STBC 0x00529740 _$E83
// FUNCTION: STBC 0x00529750 _$E275
// FUNCTION: STBC 0x00529760 _$E274
// FUNCTION: STBC 0x00529770 _$E282
// FUNCTION: STBC 0x00529780 _$E281
// FUNCTION: STBC 0x00529790 _$E305
// FUNCTION: STBC 0x005297a0 _$E304
// FUNCTION: STBC 0x005297b0 _$E309
// FUNCTION: STBC 0x005297c0 _$E308
// FUNCTION: STBC 0x005297d0 _$E349
// FUNCTION: STBC 0x005297e0 _$E348
// FUNCTION: STBC 0x005297f0 _$E355
// FUNCTION: STBC 0x00529800 _$E354
// FUNCTION: STBC 0x00529810 _$E297
// FUNCTION: STBC 0x00529820 _$E296
// FUNCTION: STBC 0x00529830 _$E301
// FUNCTION: STBC 0x00529840 _$E300
// FUNCTION: STBC 0x00529850 _$E313
// FUNCTION: STBC 0x00529860 _$E312
// FUNCTION: STBC 0x00529870 _$E317
// FUNCTION: STBC 0x00529880 _$E316
// FUNCTION: STBC 0x00529890 _$E321
// FUNCTION: STBC 0x005298a0 _$E320
// FUNCTION: STBC 0x005298b0 _$E325
// FUNCTION: STBC 0x005298c0 _$E324
// FUNCTION: STBC 0x005298d0 _$E329
// FUNCTION: STBC 0x005298e0 _$E328
// FUNCTION: STBC 0x005298f0 _$E333
// FUNCTION: STBC 0x00529900 _$E332
// FUNCTION: STBC 0x00529910 _$E337
// FUNCTION: STBC 0x00529920 _$E336
// FUNCTION: STBC 0x00529930 _$E341
// FUNCTION: STBC 0x00529940 _$E340
// FUNCTION: STBC 0x00529950 _$E89
// FUNCTION: STBC 0x00529960 _$E88
// FUNCTION: STBC 0x00529970 _$E345
// FUNCTION: STBC 0x00529980 _$E344
// FUNCTION: STBC 0x00529990 _$E94
// FUNCTION: STBC 0x005299a0 _$E93
// FUNCTION: STBC 0x005299b0 _$E359
// FUNCTION: STBC 0x005299c0 _$E358
// FUNCTION: STBC 0x005299d0 _$E363
// FUNCTION: STBC 0x005299e0 _$E362
// FUNCTION: STBC 0x005299f0 _$E367
// FUNCTION: STBC 0x00529a00 _$E366
// FUNCTION: STBC 0x00529a10 _$E371
// FUNCTION: STBC 0x00529a20 _$E370
// FUNCTION: STBC 0x00529a30 _$E103
// FUNCTION: STBC 0x00529a40 _$E102
// FUNCTION: STBC 0x00529a50 _$E375
// FUNCTION: STBC 0x00529a60 _$E374
// FUNCTION: STBC 0x00529a70 _$E379
// FUNCTION: STBC 0x00529a80 _$E378
// FUNCTION: STBC 0x00529a90 _$E383
// FUNCTION: STBC 0x00529aa0 _$E382
// FUNCTION: STBC 0x00529ab0 _$E109
// FUNCTION: STBC 0x00529ac0 _$E108
// FUNCTION: STBC 0x00529ad0 _$E114
// FUNCTION: STBC 0x00529ae0 _$E113
// FUNCTION: STBC 0x00529af0 _$E144
// FUNCTION: STBC 0x00529b00 _$E143
// FUNCTION: STBC 0x00529b40 _$E387
// FUNCTION: STBC 0x00529b50 _$E386
// FUNCTION: STBC 0x00529b60 _$E149
// FUNCTION: STBC 0x00529b70 _$E148
// FUNCTION: STBC 0x00529b80 _$E391
// FUNCTION: STBC 0x00529b90 _$E390
// FUNCTION: STBC 0x00529ba0 _$E395
// FUNCTION: STBC 0x00529bb0 _$E394
// FUNCTION: STBC 0x00529bc0 _$E154
// FUNCTION: STBC 0x00529bd0 _$E153
// FUNCTION: STBC 0x00529be0 _$E399
// FUNCTION: STBC 0x00529bf0 _$E398
// FUNCTION: STBC 0x00529c00 _$E119
// FUNCTION: STBC 0x00529c10 _$E118
// FUNCTION: STBC 0x00529c20 _$E403
// FUNCTION: STBC 0x00529c30 _$E402
// FUNCTION: STBC 0x00529c40 _$E407
// FUNCTION: STBC 0x00529c50 _$E406
// FUNCTION: STBC 0x00529c60 _$E159
// FUNCTION: STBC 0x00529c70 _$E158
// FUNCTION: STBC 0x00529c80 _$E164
// FUNCTION: STBC 0x00529c90 _$E163
// FUNCTION: STBC 0x00529ca0 _$E411
// FUNCTION: STBC 0x00529cb0 _$E410
// FUNCTION: STBC 0x00529cc0 _$E415
// FUNCTION: STBC 0x00529cd0 _$E414
// FUNCTION: STBC 0x00529ce0 _$E419
// FUNCTION: STBC 0x00529cf0 _$E418
// FUNCTION: STBC 0x00529d00 _$E423
// FUNCTION: STBC 0x00529d10 _$E422
// FUNCTION: STBC 0x00529d20 _$E427
// FUNCTION: STBC 0x00529d30 _$E426
// FUNCTION: STBC 0x00529d40 _$E431
// FUNCTION: STBC 0x00529d50 _$E430
// FUNCTION: STBC 0x00529d60 _$E435
// FUNCTION: STBC 0x00529d70 _$E434
// FUNCTION: STBC 0x00529d80 _$E439
// FUNCTION: STBC 0x00529d90 _$E438
// FUNCTION: STBC 0x00529da0 _$E169
// FUNCTION: STBC 0x00529db0 _$E168
// FUNCTION: STBC 0x00529dc0 _$E443
// FUNCTION: STBC 0x00529dd0 _$E442
// FUNCTION: STBC 0x00529de0 _$E174
// FUNCTION: STBC 0x00529df0 _$E173
// FUNCTION: STBC 0x00529e00 _$E179
// FUNCTION: STBC 0x00529e10 _$E178
// FUNCTION: STBC 0x00529e20 _$E447
// FUNCTION: STBC 0x00529e30 _$E446
// FUNCTION: STBC 0x00529e40 _$E452
// FUNCTION: STBC 0x00529e50 _$E451
// FUNCTION: STBC 0x00529e60 _$E456
// FUNCTION: STBC 0x00529e70 _$E455

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005294b0 _$E251
// FUNCTION: STBC 0x005294c0 _$E248
// FUNCTION: STBC 0x005294d0 _$E250
// FUNCTION: STBC 0x005294e0 _$E249
// FUNCTION: STBC 0x005294f0 _$E257
// FUNCTION: STBC 0x00529500 _$E254
// FUNCTION: STBC 0x00529510 _$E256
// FUNCTION: STBC 0x00529520 _$E255

// The options file's section and key of the number of stored lines.
#define OPTIONS_SECTION		"General Options"
#define LINES_KEY			"Lines in Mission Log"

// DATA: STBC 0x00989414
STMissionLog *STMissionLog::missionLog;

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00528680 _$E462
// FUNCTION: STBC 0x00528690 _$E458
// FUNCTION: STBC 0x005286a0 _$E461
// FUNCTION: STBC 0x005286b0 _$E459
// DATA: STBC 0x00989338
TGEventHandlerTable STMissionLog::classHandlers;

/*
================
STMissionLog::STMissionLog

Without an ID the log builds its window: the lines pane inside a stylized
window centred over the screen, and the close button below it. The log
starts hidden.
================
*/
// FUNCTION: STBC 0x005286d0 ??0STMissionLog@@QAE@J@Z
// PARKED: register/schedule: the close button's SetPosition loads its vtable and the button's height after pushing the arguments in retail, before them here (tried parentheses, a local for the window's bottom)
STMissionLog::STMissionLog( long id ) : TGPane( id ) {
	numStoredLines = 100;
	numLines = 0;
	if ( g_kConfigMapping.HasValue( OPTIONS_SECTION, LINES_KEY ) ) {
		numStoredLines = g_kConfigMapping.GetIntValue( OPTIONS_SECTION, LINES_KEY );
	} else {
		g_kConfigMapping.SetIntValue( OPTIONS_SECTION, LINES_KEY, 100 );
	}
	focus = NULL;
	if ( id == 0 ) {
		Resize( 1.0f, 1.0f, true );
		STSubPane *linesPane = new STSubPane( 0L, true );
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
		STStylizedWindow *window = new STStylizedWindow( "StylizedWindow", "MissionLog", database->GetString( "Mission Log" ), 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor );
		window->SetFixedSize( 0.7f, 0.7f, false );
		linesPane->Resize( window->GetMaximumInteriorWidth(), window->GetMaximumInteriorHeight(), false );
		window->AddChild( linesPane, 0.0f, 0.0f, false );
		AddChild( window, 0.0f, 0.0f, false );
		TGPane::SetFocus( window, false );
		window->SetPosition( 0.5f - window->GetMaximumWidth() * 0.5f, 0.5f - window->GetMaximumHeight() * 0.5f, false );
		window->SetVisible( true );
		TGEvent *event = new TGEvent;
		event->SetDestination( this );
		event->SetEventType( ET_OKAY );
		STButton *closeButton = new STButton( database->GetString( "Close" ), event, STBF_SIZE_TO_TEXT, 0.0f, 0.0f, NULL, 200 );
		AddChild( closeButton, 0.0f, 0.0f, false );
		closeButton->SetPosition( 0.5f - closeButton->GetWidth() * 0.5f, ( window->GetHeight() + window->GetTop() ) + closeButton->GetHeight() * 0.25f, false );
		g_kLocalizationManager.Unload( database );
		SetNotBatchChildPolys( false );
	}
	TGPane::SetNotVisible( false );
	flags |= TGUIO_ALWAYS_HANDLE_EVENTS;
	unknown54 = false;
	missionLog = this;
}

/*
================
STMissionLog::~STMissionLog

The game was paused to show the log: it resumes.
================
*/
// FUNCTION: STBC 0x00528b00 ??1STMissionLog@@UAE@XZ
STMissionLog::~STMissionLog( void ) {
	if ( unknown54 ) {
		g_kUtopiaModule.Pause( false );
	}
	missionLog = NULL;
}

/*
================
STMissionLog::GetLinesPane

The sub pane that holds the lines: the first child of the log's window.
================
*/
inline STSubPane *STMissionLog::GetLinesPane( void ) {
	STStylizedWindow *window = STStylizedWindow::Cast( GetNthChild( 0 ) );

	// BUG: the interior pane is fetched and dropped; the lines are the window's own first child.
	window->GetInteriorPane();
	return STSubPane::Cast( window->GetNthChild( 0 ) );
}

/*
================
STMissionLog::SetNumStoredLines

Keeps at most that many lines, dropping the oldest, and saves the number in
the options.
================
*/
// FUNCTION: STBC 0x00528B70
void STMissionLog::SetNumStoredLines( int count ) {
	STSubPane *subPane = GetLinesPane();

	numStoredLines = count;
	while ( numLines > numStoredLines ) {
		TGUIObject *oldest = subPane->GetFirstChild();
		subPane->DeleteChild( oldest, false );
		numLines--;
	}
	subPane->ResizeToContents( true );
	Layout();
	g_kConfigMapping.SetIntValue( OPTIONS_SECTION, LINES_KEY, numStoredLines );
}

/*
================
STMissionLog::AddLine

Adds the line as a paragraph as wide as the window's interior.
================
*/
// FUNCTION: STBC 0x00528C20
void STMissionLog::AddLine( TGString *line ) {
	STStylizedWindow *window = (STStylizedWindow *)GetNthChild( 0 );
	Unknown528CB0( new TGParagraph( line, window->GetMaximumInteriorWidth() * 0.9f, NULL, "", 0.0f, TGPF_WORD_WRAP | TGPF_READ_ONLY ) );
}

/*
================
STMissionLog::Unknown528CB0
================
*/
// FUNCTION: STBC 0x00528CB0
void STMissionLog::Unknown528CB0( TGParagraph *paragraph ) {
	STSubPane *subPane = GetLinesPane();

	subPane->AddChild( paragraph, 0.0f, 0.0f, false );
	numLines++;
	while ( numLines > numStoredLines ) {
		TGUIObject *oldest = subPane->GetFirstChild();
		subPane->DeleteChild( oldest, false );
		numLines--;
	}
	if ( flags & TGUIO_VISIBLE ) {
		subPane->ResizeToContents( true );
		Layout();
	}
}

/*
================
STMissionLog::ClearLines
================
*/
// FUNCTION: STBC 0x00528D70
void STMissionLog::ClearLines( void ) {
	STSubPane *subPane = GetLinesPane();

	while ( subPane->GetNumChildren() > 0 ) {
		TGUIObject *oldest = subPane->GetFirstChild();
		subPane->DeleteChild( oldest, false );
	}
	numLines = 0;
	if ( flags & TGUIO_VISIBLE ) {
		subPane->ResizeToContents( true );
		Layout();
	}
}

/*
================
STMissionLog::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00528e10
void STMissionLog::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMissionLog::MouseHandler, "STMissionLog::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMissionLog::KeyboardHandler, "STMissionLog::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STMissionLog::Close, "STMissionLog::Close" );
}

/*
================
STMissionLog::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00528e50
void STMissionLog::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "STMissionLog::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STMissionLog::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_OKAY, "STMissionLog::Close" );
}

/*
================
STMissionLog::MouseHandler

A click of the right button, or of the left one outside the window, closes
the log.
================
*/
// FUNCTION: STBC 0x00528e90
void STMissionLog::MouseHandler( TGMouseEvent *event ) {
	if ( !IsCompletelyVisible() ) {
		CallNextHandler( event );
		return;
	}
	if ( event->IsButtonEvent() && ( event->GetFlags() & MEF_BUTTON_UP ) ) {
		NiPoint2 point;
		event->GetPosition( &point );
		STStylizedWindow *window = STStylizedWindow::Cast( GetNthChild( 0 ) );
		NiPoint2 windowPosition;
		window->GetPosition( &windowPosition );
		point -= windowPosition;
		if ( ( event->GetFlags() & MEF_BUTTON_RIGHT ) || ( ( event->GetFlags() & MEF_BUTTON_LEFT ) && !window->IsWithin( &point ) ) ) {
			SetNotVisible( false );
			TGPane *pane = parent;
			pane->SetFocus( pane->GetNextChild( this ), false );
			pane->MoveToBack( this, true );
			g_kUtopiaModule.Pause( false );
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
STMissionLog::KeyboardHandler

Up and down scroll the window while the key is held; left, space and
backspace close the log.
================
*/
// FUNCTION: STBC 0x00528fd0
void STMissionLog::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( !IsVisible() ) {
		CallNextHandler( event );
		return;
	}
	unsigned short key = event->GetUnicode();
	int state = event->GetKeyState();
	STStylizedWindow *window = STStylizedWindow::Cast( GetFirstChild() );
	if ( state == KS_KEYDOWN ) {
		if ( key == WC_UP || key == WC_DOWN ) {
			TGIntEvent *scroll = new TGIntEvent;
			scroll->SetEventType( ET_ST_SCROLL );
			scroll->SetDestination( window );
			if ( key == WC_UP ) {
				scroll->SetInt( -1 );
			} else {
				scroll->SetInt( 1 );
			}
			g_kEventManager.AddEvent( scroll );
			event->SetHandled();
		}
	} else if ( state == KS_KEYUP ) {
		if ( key == WC_LEFT || key == ' ' || key == WC_BACKSPACE ) {
			SetNotVisible( false );
			TGPane *pane = parent;
			pane->SetFocus( pane->GetNextChild( this ), false );
			pane->MoveToBack( this, true );
			g_kUtopiaModule.Pause( false );
			event->SetHandled();
		}
		if ( key == WC_UP || key == WC_DOWN ) {
			TGIntEvent *scroll = new TGIntEvent;
			scroll->SetEventType( ET_ST_SCROLL );
			scroll->SetDestination( window );
			scroll->SetInt( 0 );
			g_kEventManager.AddEvent( scroll );
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
STMissionLog::Close

The close button: the log hides, the focus goes back and the game resumes.
================
*/
// FUNCTION: STBC 0x00529170
void STMissionLog::Close( TGEvent *event ) {
	SetNotVisible( false );
	TGPane *pane = parent;
	pane->SetFocus( pane->GetNextChild( this ), false );
	pane->MoveToBack( this, true );
	unknown54 = false;
	g_kUtopiaModule.Pause( false );
}

/*
================
STMissionLog::Layout

The lines are as wide as the window's interior.
================
*/
// FUNCTION: STBC 0x005291c0
void STMissionLog::Layout( void ) {
	STStylizedWindow *window = (STStylizedWindow *)GetNthChild( 0 );
	if ( window != NULL ) {
		window->GetInteriorPane();
		STSubPane *linesPane = STSubPane::Cast( window->GetNthChild( 0 ) );
		if ( linesPane != NULL ) {
			int numChildren = linesPane->GetNumChildren();
			for ( int i = 0; i < numChildren; i++ ) {
				TGParagraph *paragraph = TGParagraph::Cast( linesPane->GetNthChild( i ) );
				if ( paragraph != NULL ) {
					paragraph->SetWrapWidth( window->GetMaximumInteriorWidth() * 0.9f, true );
				}
			}
		}
	}
	TGPane::Layout();
}

/*
================
STMissionLog::SetVisible

Showing the log hides the subtitles, pauses the game and scrolls the
lines to the end.
================
*/
// FUNCTION: STBC 0x00529280
void STMissionLog::SetVisible( bool layout ) {
	TGPane::GotFocus( true );
	unknown54 = true;
	MainWindow *subtitles = TopWindow::GetTopWindow()->FindMainWindow( MWT_SUBTITLE );
	if ( subtitles != NULL ) {
		subtitles->SetNotVisible( true );
	}
	TGPane::SetVisible( false );
	STStylizedWindow *window = STStylizedWindow::Cast( GetNthChild( 0 ) );
	STSubPane *linesPane = STSubPane::Cast( window->GetNthChild( 0 ) );
	linesPane->Layout();
	if ( window != NULL ) {
		window->GetInteriorPane();
		STSubPane *lines = STSubPane::Cast( window->GetNthChild( 0 ) );
		if ( lines != NULL ) {
			int numChildren = lines->GetNumChildren();
			for ( int i = 0; i < numChildren; i++ ) {
				TGParagraph *paragraph = TGParagraph::Cast( lines->GetNthChild( i ) );
				if ( paragraph != NULL ) {
					paragraph->SetWrapWidth( window->GetMaximumInteriorWidth() * 0.9f, true );
				}
			}
		}
	}
	linesPane->ResizeToContents( true );
	window->InteriorChangedSize( true );
	if ( window != NULL ) {
		window->ScrollToBottom();
	}
	Layout();
	InterfaceModule::ForceFocusOnObject( GetLastChild() );
	InterfaceModule::DoTheRightThing();
}

/*
================
STMissionLog::SetNotVisible
================
*/
// FUNCTION: STBC 0x005293f0
void STMissionLog::SetNotVisible( bool layout ) {
	TGPane::LostFocus( true );
	MainWindow *subtitles = TopWindow::GetTopWindow()->FindMainWindow( MWT_SUBTITLE );
	if ( subtitles != NULL ) {
		subtitles->SetVisible( true );
	}
	TGPane::SetNotVisible( layout );
	InterfaceModule::DoTheRightThing();
}

/*
================
STMissionLog::WriteToStream
================
*/
// FUNCTION: STBC 0x00529430
bool STMissionLog::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( numStoredLines );
	stream->WriteInt( numLines );
	return true;
}

/*
================
STMissionLog::ReadFromStream
================
*/
// FUNCTION: STBC 0x00529470
bool STMissionLog::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	numStoredLines = stream->ReadInt();
	numLines = stream->ReadInt();
	return true;
}
