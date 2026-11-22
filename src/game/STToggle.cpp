/*
===============================================================================

	STToggle.cpp

===============================================================================
*/

#include <iostream>
#include "STToggle.h"
#include "STMenu.h"
#include "InterfaceModule.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "TGEvent.h"
#include "TGStream.h"
#include "TGKeyboardEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "NiPoint2.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0053c2f0 _$E57
// FUNCTION: STBC 0x0053c300 _$E56
// FUNCTION: STBC 0x0053c310 _$E61
// FUNCTION: STBC 0x0053c320 _$E60
// FUNCTION: STBC 0x0053c330 _$E72
// FUNCTION: STBC 0x0053c340 _$E71
// FUNCTION: STBC 0x0053c350 _$E76
// FUNCTION: STBC 0x0053c360 _$E75
// FUNCTION: STBC 0x0053c370 _$E80
// FUNCTION: STBC 0x0053c380 _$E79
// FUNCTION: STBC 0x0053c390 _$E84
// FUNCTION: STBC 0x0053c3a0 _$E83
// FUNCTION: STBC 0x0053c3b0 _$E208
// FUNCTION: STBC 0x0053c3c0 _$E207
// FUNCTION: STBC 0x0053c3d0 _$E150
// FUNCTION: STBC 0x0053c3e0 _$E149
// FUNCTION: STBC 0x0053c3f0 _$E163
// FUNCTION: STBC 0x0053c400 _$E162
// FUNCTION: STBC 0x0053c410 _$E213
// FUNCTION: STBC 0x0053c420 _$E212
// FUNCTION: STBC 0x0053c430 _$E218
// FUNCTION: STBC 0x0053c440 _$E217
// FUNCTION: STBC 0x0053c450 _$E222
// FUNCTION: STBC 0x0053c460 _$E221
// FUNCTION: STBC 0x0053c470 _$E226
// FUNCTION: STBC 0x0053c480 _$E225
// FUNCTION: STBC 0x0053c490 _$E233
// FUNCTION: STBC 0x0053c4a0 _$E232
// FUNCTION: STBC 0x0053c4b0 _$E277
// FUNCTION: STBC 0x0053c4c0 _$E276
// FUNCTION: STBC 0x0053c4d0 _$E281
// FUNCTION: STBC 0x0053c4e0 _$E280
// FUNCTION: STBC 0x0053c4f0 _$E108
// FUNCTION: STBC 0x0053c500 _$E107
// FUNCTION: STBC 0x0053c510 _$E263
// FUNCTION: STBC 0x0053c520 _$E262
// FUNCTION: STBC 0x0053c530 _$E270
// FUNCTION: STBC 0x0053c540 _$E269
// FUNCTION: STBC 0x0053c550 _$E293
// FUNCTION: STBC 0x0053c560 _$E292
// FUNCTION: STBC 0x0053c570 _$E297
// FUNCTION: STBC 0x0053c580 _$E296
// FUNCTION: STBC 0x0053c590 _$E337
// FUNCTION: STBC 0x0053c5a0 _$E336
// FUNCTION: STBC 0x0053c5b0 _$E343
// FUNCTION: STBC 0x0053c5c0 _$E342
// FUNCTION: STBC 0x0053c5d0 _$E285
// FUNCTION: STBC 0x0053c5e0 _$E284
// FUNCTION: STBC 0x0053c5f0 _$E289
// FUNCTION: STBC 0x0053c600 _$E288
// FUNCTION: STBC 0x0053c610 _$E301
// FUNCTION: STBC 0x0053c620 _$E300
// FUNCTION: STBC 0x0053c630 _$E305
// FUNCTION: STBC 0x0053c640 _$E304
// FUNCTION: STBC 0x0053c650 _$E309
// FUNCTION: STBC 0x0053c660 _$E308
// FUNCTION: STBC 0x0053c670 _$E313
// FUNCTION: STBC 0x0053c680 _$E312
// FUNCTION: STBC 0x0053c690 _$E317
// FUNCTION: STBC 0x0053c6a0 _$E316
// FUNCTION: STBC 0x0053c6b0 _$E321
// FUNCTION: STBC 0x0053c6c0 _$E320
// FUNCTION: STBC 0x0053c6d0 _$E325
// FUNCTION: STBC 0x0053c6e0 _$E324
// FUNCTION: STBC 0x0053c6f0 _$E329
// FUNCTION: STBC 0x0053c700 _$E328
// FUNCTION: STBC 0x0053c710 _$E113
// FUNCTION: STBC 0x0053c720 _$E112
// FUNCTION: STBC 0x0053c730 _$E333
// FUNCTION: STBC 0x0053c740 _$E332
// FUNCTION: STBC 0x0053c750 _$E118
// FUNCTION: STBC 0x0053c760 _$E117
// FUNCTION: STBC 0x0053c770 _$E347
// FUNCTION: STBC 0x0053c780 _$E346
// FUNCTION: STBC 0x0053c790 _$E351
// FUNCTION: STBC 0x0053c7a0 _$E350
// FUNCTION: STBC 0x0053c7b0 _$E355
// FUNCTION: STBC 0x0053c7c0 _$E354
// FUNCTION: STBC 0x0053c7d0 _$E359
// FUNCTION: STBC 0x0053c7e0 _$E358
// FUNCTION: STBC 0x0053c7f0 _$E127
// FUNCTION: STBC 0x0053c800 _$E126
// FUNCTION: STBC 0x0053c810 _$E363
// FUNCTION: STBC 0x0053c820 _$E362
// FUNCTION: STBC 0x0053c830 _$E367
// FUNCTION: STBC 0x0053c840 _$E366
// FUNCTION: STBC 0x0053c850 _$E371
// FUNCTION: STBC 0x0053c860 _$E370
// FUNCTION: STBC 0x0053c870 _$E133
// FUNCTION: STBC 0x0053c880 _$E132
// FUNCTION: STBC 0x0053c8c0 _$E138
// FUNCTION: STBC 0x0053c8d0 _$E137
// FUNCTION: STBC 0x0053c8e0 _$E168
// FUNCTION: STBC 0x0053c8f0 _$E167
// FUNCTION: STBC 0x0053c900 _$E375
// FUNCTION: STBC 0x0053c910 _$E374
// FUNCTION: STBC 0x0053c920 _$E173
// FUNCTION: STBC 0x0053c930 _$E172
// FUNCTION: STBC 0x0053c940 _$E379
// FUNCTION: STBC 0x0053c950 _$E378
// FUNCTION: STBC 0x0053c960 _$E383
// FUNCTION: STBC 0x0053c970 _$E382
// FUNCTION: STBC 0x0053c980 _$E178
// FUNCTION: STBC 0x0053c990 _$E177
// FUNCTION: STBC 0x0053c9a0 _$E387
// FUNCTION: STBC 0x0053c9b0 _$E386
// FUNCTION: STBC 0x0053c9c0 _$E143
// FUNCTION: STBC 0x0053c9d0 _$E142
// FUNCTION: STBC 0x0053c9e0 _$E391
// FUNCTION: STBC 0x0053c9f0 _$E390
// FUNCTION: STBC 0x0053ca00 _$E395
// FUNCTION: STBC 0x0053ca10 _$E394
// FUNCTION: STBC 0x0053ca20 _$E183
// FUNCTION: STBC 0x0053ca30 _$E182
// FUNCTION: STBC 0x0053ca40 _$E188
// FUNCTION: STBC 0x0053ca50 _$E187
// FUNCTION: STBC 0x0053ca60 _$E399
// FUNCTION: STBC 0x0053ca70 _$E398
// FUNCTION: STBC 0x0053ca80 _$E403
// FUNCTION: STBC 0x0053ca90 _$E402
// FUNCTION: STBC 0x0053caa0 _$E407
// FUNCTION: STBC 0x0053cab0 _$E406
// FUNCTION: STBC 0x0053cac0 _$E411
// FUNCTION: STBC 0x0053cad0 _$E410
// FUNCTION: STBC 0x0053cae0 _$E415
// FUNCTION: STBC 0x0053caf0 _$E414
// FUNCTION: STBC 0x0053cb00 _$E419
// FUNCTION: STBC 0x0053cb10 _$E418
// FUNCTION: STBC 0x0053cb20 _$E423
// FUNCTION: STBC 0x0053cb30 _$E422
// FUNCTION: STBC 0x0053cb40 _$E427
// FUNCTION: STBC 0x0053cb50 _$E426
// FUNCTION: STBC 0x0053cb60 _$E193
// FUNCTION: STBC 0x0053cb70 _$E192
// FUNCTION: STBC 0x0053cb80 _$E431
// FUNCTION: STBC 0x0053cb90 _$E430
// FUNCTION: STBC 0x0053cba0 _$E198
// FUNCTION: STBC 0x0053cbb0 _$E197
// FUNCTION: STBC 0x0053cbc0 _$E203
// FUNCTION: STBC 0x0053cbd0 _$E202
// FUNCTION: STBC 0x0053cbe0 _$E435
// FUNCTION: STBC 0x0053cbf0 _$E434
// FUNCTION: STBC 0x0053cc00 _$E440
// FUNCTION: STBC 0x0053cc10 _$E439
// FUNCTION: STBC 0x0053cc20 _$E444
// FUNCTION: STBC 0x0053cc30 _$E443

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0053c270 _$E32
// FUNCTION: STBC 0x0053c280 _$E29
// FUNCTION: STBC 0x0053c290 _$E31
// FUNCTION: STBC 0x0053c2a0 _$E30
// FUNCTION: STBC 0x0053c2b0 _$E38
// FUNCTION: STBC 0x0053c2c0 _$E35
// FUNCTION: STBC 0x0053c2d0 _$E37
// FUNCTION: STBC 0x0053c2e0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0053b550 _$E450
// FUNCTION: STBC 0x0053b560 _$E446
// FUNCTION: STBC 0x0053b570 _$E449
// FUNCTION: STBC 0x0053b580 _$E447

// Further static initialisers of the unit.
// FUNCTION: STBC 0x0053af30 _$E458
// FUNCTION: STBC 0x0053af50 _$E457

// DATA: STBC 0x0098a160
TGEventHandlerTable STToggle::classHandlers;

/*
================
STToggle::Cast

The object as a toggle; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0053b5a0
STToggle *STToggle::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STTOGGLE_TYPE ) ) {
		return (STToggle *)object;
	}
	return NULL;
}

/*
================
STToggle::STToggle

A toggle as wide as its parent, sending the first state's event until a
state is shown.
================
*/
// FUNCTION: STBC 0x0053b5d0 ??0STToggle@@QAE@PADH0PAVTGEvent@@010101@Z
STToggle::STToggle( char *caption, int state, char *value0, TGEvent *event0, char *value1, TGEvent *event1,
					char *value2, TGEvent *event2, char *value3, TGEvent *event3 )
	: STButton( caption, event0, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		stateEvents[i] = NULL;
	}
	SetStateValues( caption, state, value0, event0, value1, event1, value2, event2, value3, event3 );
}

/*
================
STToggle::STToggle
================
*/
// FUNCTION: STBC 0x0053b840 ??0STToggle@@QAE@PAVTGString@@H0PAVTGEvent@@010101@Z
STToggle::STToggle( TGString *caption, int state, TGString *value0, TGEvent *event0, TGString *value1, TGEvent *event1,
					TGString *value2, TGEvent *event2, TGString *value3, TGEvent *event3 )
	: STButton( caption, event0, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		stateEvents[i] = NULL;
	}
	SetStateValuesW( caption, state, value0, event0, value1, event1, value2, event2, value3, event3 );
}

/*
================
STToggle::STToggle

The same with the button's flags.
================
*/
// FUNCTION: STBC 0x0053b9a0 ??0STToggle@@QAE@PAVTGString@@HI0PAVTGEvent@@010101@Z
STToggle::STToggle( TGString *caption, int state, unsigned int buttonFlags, TGString *value0, TGEvent *event0,
					TGString *value1, TGEvent *event1, TGString *value2, TGEvent *event2, TGString *value3,
					TGEvent *event3 )
	: STButton( caption, event0, buttonFlags, 0.0f, 0.0f, NULL, STMENU_ICON_TITLE_BAR ) {
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		stateEvents[i] = NULL;
	}
	SetStateValuesW( caption, state, value0, event0, value1, event1, value2, event2, value3, event3 );
}

/*
================
STToggle::~STToggle

Releases the states' events.
================
*/
// FUNCTION: STBC 0x0053bb00
STToggle::~STToggle( void ) {
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( stateEvents[i] != NULL ) {
			stateEvents[i]->DecRefCount();
		}
	}
}

/*
================
STToggle::GetDesiredSize

Wide enough for the caption with the longest state's text; the button
shows the current state again afterwards.
================
*/
// FUNCTION: STBC 0x0053bba0
// PARKED: x87 residue in the size: retail adds the marker's width from memory and copies its height through fld/fstp, where this build keeps the width on the FPU stack and moves the height as an integer
void STToggle::GetDesiredSize( NiPoint2 *size ) {
	TGPane *textPane = TGPane::Cast( GetNthChild( STBUTTON_TEXT_PANE ) );
	TGParagraph *text = TGParagraph::Cast( textPane->GetNthChild( 0 ) );
	TGUIObject *marker = GetNthChild( STBUTTON_MARKER );
	TGIcon *leftEndCap = TGIcon::Cast( GetNthChild( STBUTTON_LEFT_END_CAP ) );
	int currentState = state;
	float maxWidth = 0.0f;
	TGString name( "" );
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		name = primaryCaption;
		name.Unknown6F5160( name.GetLength(), &stateValues[i] );
		SetName( &name, false );
		maxWidth = __max( text->GetWidth(), maxWidth );
	}
	name = primaryCaption;
	name.Unknown6F5160( name.GetLength(), &stateValues[currentState] );
	SetName( &name, false );
	size->x = leftEndCap->GetWidth() * 2.0f + marker->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 3.0f + maxWidth;
	size->y = marker->GetHeight();
}

/*
================
STToggle::GetNumUnusedStates
================
*/
// FUNCTION: STBC 0x0053bd20
int STToggle::GetNumUnusedStates( void ) {
	int count = 0;
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( stateValues[i].CompareC( "" ) == 0 && stateEvents[i] == NULL ) {
			count++;
		}
	}
	return count;
}

/*
================
STToggle::SetStateValueW

An empty text when value is NULL.
================
*/
// FUNCTION: STBC 0x0053bd60
void STToggle::SetStateValueW( int index, TGString *value, TGEvent *event ) {
	stateValues[index] = *( value != NULL ? value : &TGString( "" ) );
	TGEvent *oldEvent = stateEvents[index];
	stateEvents[index] = event;
	if ( event != NULL ) {
		event->IncRefCount();
	}
	if ( oldEvent != NULL ) {
		oldEvent->DecRefCount();
	}
	SetState( state, true );
}

/*
================
STToggle::SetState

Names the button with the caption and the state's text and makes the
state's event the activation event.
================
*/
// FUNCTION: STBC 0x0053be20
void STToggle::SetState( int state, bool layout ) {
	// BUG: never used; retail constructs and destroys it all the same
	TGString unused( "" );
	this->state = state;
	TGString name = primaryCaption;
	name.Unknown6F5160( name.GetLength(), &stateValues[this->state] );
	SetName( &name, layout );
	SetActivationEvent( stateEvents[this->state] );
	if ( layout ) {
		Layout();
	}
}

/*
================
STToggle::NextState
================
*/
// FUNCTION: STBC 0x0053bf20
void STToggle::NextState( void ) {
	unsigned int next = state + 1;
	TGString empty( "" );
	if ( next > STTOGGLE_LAST_STATE || stateValues[next].Compare( &empty ) == 0 ) {
		next = 0;
	}
	SetState( next, true );
}

/*
================
STToggle::PreviousState
================
*/
// FUNCTION: STBC 0x0053bfb0
void STToggle::PreviousState( void ) {
	SetState( state == 0 ? STTOGGLE_LAST_STATE : state - 1, true );
}

/*
================
STToggle::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0053bfe0
void STToggle::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STToggle::KeyboardHandler, "STToggle::KeyboardHandler" );
}

/*
================
STToggle::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0053c000
void STToggle::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STToggle::KeyboardHandler" );
}

/*
================
STToggle::KeyboardHandler

On release, the right arrow activates the toggle (moving to the next
state); End and Home show the last and the first state and send its
event.
================
*/
// FUNCTION: STBC 0x0053c020
void STToggle::KeyboardHandler( TGKeyboardEvent *event ) {
	bool advance = false;
	if ( event->GetKeyState() == KS_KEYUP ) {
		switch ( (unsigned short)event->GetUnicode() ) {
			case WC_RIGHT:
				advance = true;
				break;
			case WC_END:
				SetState( STTOGGLE_LAST_STATE, true );
				break;
			case WC_HOME:
				SetState( 0, true );
				break;
			default:
				CallNextHandler( event );
				return;
		}
		event->SetHandled();
		if ( advance ) {
			SendActivationEvent();
		} else {
			STButton::SendActivationEvent();
		}
		return;
	}
	CallNextHandler( event );
}

/*
================
STToggle::SendActivationEvent
================
*/
// FUNCTION: STBC 0x0053c0a0
void STToggle::SendActivationEvent( void ) {
	NextState();
	STButton::SendActivationEvent();
}

/*
================
STToggle::WriteToStream

The events are saved as their IDs.
================
*/
// FUNCTION: STBC 0x0053c0c0
bool STToggle::WriteToStream( TGStream *stream ) {
	if ( !STButton::WriteToStream( stream ) ) {
		return false;
	}
	if ( !primaryCaption.WriteToStream( stream ) ) {
		return false;
	}
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( !stateValues[i].WriteToStream( stream ) ) {
			return false;
		}
		stream->WriteID( GetSavedID( stateEvents[i] ) );
	}
	stream->WriteInt( state );
	return true;
}

/*
================
STToggle::ReadFromStream

Keeps the IDs in the pointers until PostFixUp.
================
*/
// FUNCTION: STBC 0x0053c160
bool STToggle::ReadFromStream( TGStream *stream ) {
	if ( !STButton::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !primaryCaption.ReadFromStream( stream ) ) {
		return false;
	}
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( !stateValues[i].ReadFromStream( stream ) ) {
			return false;
		}
		stateEvents[i] = (TGEvent *)stream->ReadID();
	}
	state = stream->ReadInt();
	return true;
}

/*
================
STToggle::FixUpPointers

Fails until the events the stream named are loaded.
================
*/
// FUNCTION: STBC 0x0053c1f0
bool STToggle::FixUpPointers( void ) {
	if ( !STButton::FixUpPointers() ) {
		return false;
	}
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( stateEvents[i] != NULL && GetTGObjectPtr( (long)stateEvents[i] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
STToggle::PostFixUp

Replaces the IDs with the events.
================
*/
// FUNCTION: STBC 0x0053c230
void STToggle::PostFixUp( void ) {
	STButton::PostFixUp();
	for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
		if ( stateEvents[i] == NULL ) {
			stateEvents[i] = NULL;
		} else {
			stateEvents[i] = (TGEvent *)GetTGObjectPtr( (long)stateEvents[i] );
		}
	}
}
