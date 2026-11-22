/*
===============================================================================

	TGDialogWindow.cpp

===============================================================================
*/

#include <iostream>
#include "TGDialogWindow.h"
#include "TGEvent.h"
#include "TGParagraph.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGStream.h"
#include "TGUITheme.h"
#include "TGUIThemeManager.h"
#include "TGRootPane.h"
#include "TGIntEvent.h"
#include "TGTextButton.h"
#include "TGRect.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007380e0 _$E32
// FUNCTION: STBC 0x007380f0 _$E29
// FUNCTION: STBC 0x00738100 _$E31
// FUNCTION: STBC 0x00738110 _$E30
// FUNCTION: STBC 0x00738120 _$E38
// FUNCTION: STBC 0x00738130 _$E35
// FUNCTION: STBC 0x00738140 _$E37
// FUNCTION: STBC 0x00738150 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00738160 _$E57
// FUNCTION: STBC 0x00738170 _$E56
// FUNCTION: STBC 0x00738180 _$E61
// FUNCTION: STBC 0x00738190 _$E60
// FUNCTION: STBC 0x007381a0 _$E72
// FUNCTION: STBC 0x007381b0 _$E71
// FUNCTION: STBC 0x007381c0 _$E76
// FUNCTION: STBC 0x007381d0 _$E75
// FUNCTION: STBC 0x007381e0 _$E80
// FUNCTION: STBC 0x007381f0 _$E79
// FUNCTION: STBC 0x00738200 _$E84
// FUNCTION: STBC 0x00738210 _$E83
// FUNCTION: STBC 0x00738220 _$E208
// FUNCTION: STBC 0x00738230 _$E207
// FUNCTION: STBC 0x00738240 _$E150
// FUNCTION: STBC 0x00738250 _$E149
// FUNCTION: STBC 0x00738260 _$E163
// FUNCTION: STBC 0x00738270 _$E162
// FUNCTION: STBC 0x00738280 _$E213
// FUNCTION: STBC 0x00738290 _$E212
// FUNCTION: STBC 0x007382a0 _$E218
// FUNCTION: STBC 0x007382b0 _$E217
// FUNCTION: STBC 0x007382c0 _$E222
// FUNCTION: STBC 0x007382d0 _$E221
// FUNCTION: STBC 0x007382e0 _$E226
// FUNCTION: STBC 0x007382f0 _$E225
// FUNCTION: STBC 0x00738300 _$E233
// FUNCTION: STBC 0x00738310 _$E232
// FUNCTION: STBC 0x00738320 _$E274
// FUNCTION: STBC 0x00738330 _$E273
// FUNCTION: STBC 0x00738340 _$E278
// FUNCTION: STBC 0x00738350 _$E277
// FUNCTION: STBC 0x00738360 _$E108
// FUNCTION: STBC 0x00738370 _$E107
// FUNCTION: STBC 0x00738380 _$E260
// FUNCTION: STBC 0x00738390 _$E259
// FUNCTION: STBC 0x007383a0 _$E267
// FUNCTION: STBC 0x007383b0 _$E266
// FUNCTION: STBC 0x007383c0 _$E290
// FUNCTION: STBC 0x007383d0 _$E289
// FUNCTION: STBC 0x007383e0 _$E294
// FUNCTION: STBC 0x007383f0 _$E293
// FUNCTION: STBC 0x00738400 _$E334
// FUNCTION: STBC 0x00738410 _$E333
// FUNCTION: STBC 0x00738420 _$E340
// FUNCTION: STBC 0x00738430 _$E339
// FUNCTION: STBC 0x00738440 _$E282
// FUNCTION: STBC 0x00738450 _$E281
// FUNCTION: STBC 0x00738460 _$E286
// FUNCTION: STBC 0x00738470 _$E285
// FUNCTION: STBC 0x00738480 _$E298
// FUNCTION: STBC 0x00738490 _$E297
// FUNCTION: STBC 0x007384a0 _$E302
// FUNCTION: STBC 0x007384b0 _$E301
// FUNCTION: STBC 0x007384c0 _$E306
// FUNCTION: STBC 0x007384d0 _$E305
// FUNCTION: STBC 0x007384e0 _$E310
// FUNCTION: STBC 0x007384f0 _$E309
// FUNCTION: STBC 0x00738500 _$E314
// FUNCTION: STBC 0x00738510 _$E313
// FUNCTION: STBC 0x00738520 _$E318
// FUNCTION: STBC 0x00738530 _$E317
// FUNCTION: STBC 0x00738540 _$E322
// FUNCTION: STBC 0x00738550 _$E321
// FUNCTION: STBC 0x00738560 _$E326
// FUNCTION: STBC 0x00738570 _$E325
// FUNCTION: STBC 0x00738580 _$E113
// FUNCTION: STBC 0x00738590 _$E112
// FUNCTION: STBC 0x007385a0 _$E330
// FUNCTION: STBC 0x007385b0 _$E329
// FUNCTION: STBC 0x007385c0 _$E118
// FUNCTION: STBC 0x007385d0 _$E117
// FUNCTION: STBC 0x007385e0 _$E344
// FUNCTION: STBC 0x007385f0 _$E343
// FUNCTION: STBC 0x00738600 _$E348
// FUNCTION: STBC 0x00738610 _$E347
// FUNCTION: STBC 0x00738620 _$E352
// FUNCTION: STBC 0x00738630 _$E351
// FUNCTION: STBC 0x00738640 _$E356
// FUNCTION: STBC 0x00738650 _$E355
// FUNCTION: STBC 0x00738660 _$E127
// FUNCTION: STBC 0x00738670 _$E126
// FUNCTION: STBC 0x00738680 _$E360
// FUNCTION: STBC 0x00738690 _$E359
// FUNCTION: STBC 0x007386a0 _$E364
// FUNCTION: STBC 0x007386b0 _$E363
// FUNCTION: STBC 0x007386c0 _$E368
// FUNCTION: STBC 0x007386d0 _$E367
// FUNCTION: STBC 0x007386e0 _$E133
// FUNCTION: STBC 0x007386f0 _$E132
// FUNCTION: STBC 0x00738700 _$E138
// FUNCTION: STBC 0x00738710 _$E137
// FUNCTION: STBC 0x00738720 _$E168
// FUNCTION: STBC 0x00738730 _$E167
// FUNCTION: STBC 0x00738740 _$E372
// FUNCTION: STBC 0x00738750 _$E371
// FUNCTION: STBC 0x00738760 _$E173
// FUNCTION: STBC 0x00738770 _$E172
// FUNCTION: STBC 0x00738780 _$E376
// FUNCTION: STBC 0x00738790 _$E375
// FUNCTION: STBC 0x007387a0 _$E380
// FUNCTION: STBC 0x007387b0 _$E379
// FUNCTION: STBC 0x007387c0 _$E178
// FUNCTION: STBC 0x007387d0 _$E177
// FUNCTION: STBC 0x007387e0 _$E384
// FUNCTION: STBC 0x007387f0 _$E383
// FUNCTION: STBC 0x00738800 _$E143
// FUNCTION: STBC 0x00738810 _$E142
// FUNCTION: STBC 0x00738820 _$E388
// FUNCTION: STBC 0x00738830 _$E387
// FUNCTION: STBC 0x00738840 _$E392
// FUNCTION: STBC 0x00738850 _$E391
// FUNCTION: STBC 0x00738860 _$E183
// FUNCTION: STBC 0x00738870 _$E182
// FUNCTION: STBC 0x00738880 _$E188
// FUNCTION: STBC 0x00738890 _$E187
// FUNCTION: STBC 0x007388a0 _$E396
// FUNCTION: STBC 0x007388b0 _$E395
// FUNCTION: STBC 0x007388c0 _$E400
// FUNCTION: STBC 0x007388d0 _$E399
// FUNCTION: STBC 0x007388e0 _$E404
// FUNCTION: STBC 0x007388f0 _$E403
// FUNCTION: STBC 0x00738900 _$E408
// FUNCTION: STBC 0x00738910 _$E407
// FUNCTION: STBC 0x00738920 _$E412
// FUNCTION: STBC 0x00738930 _$E411
// FUNCTION: STBC 0x00738940 _$E416
// FUNCTION: STBC 0x00738950 _$E415
// FUNCTION: STBC 0x00738960 _$E420
// FUNCTION: STBC 0x00738970 _$E419
// FUNCTION: STBC 0x00738980 _$E424
// FUNCTION: STBC 0x00738990 _$E423
// FUNCTION: STBC 0x007389a0 _$E193
// FUNCTION: STBC 0x007389b0 _$E192
// FUNCTION: STBC 0x007389c0 _$E428
// FUNCTION: STBC 0x007389d0 _$E427
// FUNCTION: STBC 0x007389e0 _$E198
// FUNCTION: STBC 0x007389f0 _$E197
// FUNCTION: STBC 0x00738a00 _$E203
// FUNCTION: STBC 0x00738a10 _$E202

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00738a40 _$E434
// FUNCTION: STBC 0x00738a50 _$E430
// FUNCTION: STBC 0x00738a60 _$E433
// FUNCTION: STBC 0x00738a70 _$E431
// DATA: STBC 0x0099dca8
TGEventHandlerTable TGDialogWindow::classHandlers;

// The drag of a dialog by its body: whether one is under way, and where the
// pointer took the dialog (only MouseHandler uses them; invented names).
// DATA: STBC 0x0099dd44
static bool dragging;
// DATA: STBC 0x0099dc98
static NiPoint2 dragStart;

/*
================
TGDialogWindow::TGDialogWindow

A framed window holding the message and a pane of the buttons the
flags name; it sends its events to the destination, or to itself.
================
*/
// FUNCTION: STBC 0x00738a90 ??0TGDialogWindow@@QAE@MMPAD0M0MHPAVTGEventHandlerObject@@@Z
TGDialogWindow::TGDialogWindow( float width, float height, char *message, char *frameName, float frameSize,
		char *buttonFrameName, float buttonFrameSize, int dialogFlags, TGEventHandlerObject *destination )
	: TGFrameWindow( width, height, frameName, 0, frameSize, 0, NULL ) {
	text = NULL;
	buttonPane = NULL;
	draggable = false;
	eventType = ET_DIALOG_RESULT;
	if ( destination == NULL ) {
		this->destination = this;
	} else {
		this->destination = destination;
	}
	if ( message != NULL ) {
		text = new TGParagraph( message, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	} else {
		text = new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	}
	TGPane::InsertChild( 1, text, 0.0f, 0.0f, true );
	buttonPane = new TGPane;
	AddChild( buttonPane );
	AddButtons( dialogFlags & DFT_BUTTONMASK, buttonFrameName, buttonFrameSize );
	if ( dialogFlags & DFT_DRAGGABLE ) {
		draggable = true;
	}
	LayoutButtons();
}

/*
================
TGDialogWindow::Create
================
*/
// FUNCTION: STBC 0x00738c10
TGDialogWindow *TGDialogWindow::Create( float width, float height, char *message, char *frameName, float frameSize,
		char *buttonFrameName, float buttonFrameSize, int dialogFlags, TGEventHandlerObject *destination ) {
	return new TGDialogWindow( width, height, message, frameName, frameSize, buttonFrameName, buttonFrameSize, dialogFlags, destination );
}

/*
================
TGDialogWindow::~TGDialogWindow
================
*/
// FUNCTION: STBC 0x00738c70
TGDialogWindow::~TGDialogWindow( void ) {
}

/*
================
TGDialogWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00738c80
void TGDialogWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGDialogWindow::MouseHandler, "TGDialogWindow::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGDialogWindow::DialogEventHandler, "TGDialogWindow::DialogEventHandler" );
}

/*
================
TGDialogWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00738cb0
void TGDialogWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGDialogWindow::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_DIALOG, "TGDialogWindow::DialogEventHandler" );
}

/*
================
TGDialogWindow::MouseHandler

A dialog whose drag flag is set follows the pointer while the left button
is held down over it.
================
*/
// FUNCTION: STBC 0x00738ce0
// PARKED: complete; retail keeps the window in esi and the event in edi, this build the reverse (accessor, condition and declaration-order spellings tried)
void TGDialogWindow::MouseHandler( TGMouseEvent *event ) {
	CallNextHandler( event );
	if ( event->EventHandled() == true ) {
		return;
	}
	if ( !IsVisible() ) {
		return;
	}

	bool inside;
	NiPoint2 point;
	point.x = event->x;
	point.y = event->y;
	if ( point.x < 0.0f || GetWidth() < point.x || point.y < 0.0f || GetHeight() < point.y ) {
		inside = false;
		if ( !dragging ) {
			return;
		}
	} else {
		inside = true;
	}

	if ( event->IsButtonEvent() ) {
		int buttons = event->GetFlags();
		if ( buttons & MEF_BUTTON_DOWN ) {
			if ( ( buttons & MEF_BUTTON_LEFT ) && inside == true && draggable == true ) {
				g_kRootWindow.GrabMouse( this );
				dragging = true;
				dragStart = point;
				event->SetHandled();
			}
		} else if ( buttons & MEF_BUTTON_UP ) {
			if ( ( buttons & MEF_BUTTON_LEFT ) && dragging == true ) {
				g_kRootWindow.ReleaseMouse( this );
				dragging = false;
				event->SetHandled();
			}
		}
	} else if ( dragging == true ) {
		NiPoint2 delta;
		delta.x = point.x - dragStart.x;
		delta.y = point.y - dragStart.y;
		Move( delta.x, delta.y, true );
		event->SetHandled();
	}
}

/*
================
TGDialogWindow::AddButtons

The buttons of the bits set, left to right in this order.
================
*/
// FUNCTION: STBC 0x00738e40
void TGDialogWindow::AddButtons( int buttons, char *frameName, float frameSize ) {
	NiPoint2 position;
	position.y = 0.0f;
	position.x = 0.0f;
	if ( buttons & DFT_OK ) {
		AddButton( "OK", &position, DFT_OK, frameName, frameSize );
	}
	if ( buttons & DFT_YES ) {
		AddButton( "Yes", &position, DFT_YES, frameName, frameSize );
	}
	if ( buttons & DFT_NO ) {
		AddButton( "No", &position, DFT_NO, frameName, frameSize );
	}
	if ( buttons & DFT_CANCEL ) {
		AddButton( "Cancel", &position, DFT_CANCEL, frameName, frameSize );
	}
	if ( buttons & DFT_ABORT ) {
		AddButton( "Abort", &position, DFT_ABORT, frameName, frameSize );
	}
	if ( buttons & DFT_RETRY ) {
		AddButton( "Retry", &position, DFT_RETRY, frameName, frameSize );
	}
	if ( buttons & DFT_CONTINUE ) {
		AddButton( "Continue", &position, DFT_CONTINUE, frameName, frameSize );
	}
	if ( buttons & DFT_IGNORE ) {
		AddButton( "Ignore", &position, DFT_IGNORE, frameName, frameSize );
	}
	if ( buttons & DFT_HELP ) {
		AddButton( "Help", &position, DFT_HELP, frameName, frameSize );
	}
}

/*
================
TGDialogWindow::AddButton

The button sends the dialog an ET_DIALOG event holding the button's bit.
================
*/
// FUNCTION: STBC 0x00738f60
void TGDialogWindow::AddButton( char *label, NiPoint2 *position, int button, char *frameName, float frameSize ) {
	TGIntEvent *event = new TGIntEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_DIALOG );

	TGTextButton *textButton = new TGTextButton( label, frameName, 0, frameSize );
	buttonPane->AddChild( textButton, position->x, position->y, true );
	position->x += textButton->GetWidth();
	event->SetInt( button );
	textButton->SetActivationEvent( event );
	textButton->AlignTextHorizontal( ALIGN_CENTER, true );
	textButton->AlignTextVertical( ALIGN_MIDDLE, true );
}

/*
================
TGDialogWindow::LayoutButtons

Sizes the button pane to the buttons' bounds and centres it along the
bottom of the window's sub pane.
================
*/
// FUNCTION: STBC 0x00739060
void TGDialogWindow::LayoutButtons( void ) {
	TGPane *subPane = GetSubPane();
	TGUIObject *child = buttonPane->GetFirstChild();
	TGRect buttonsRect;
	buttonsRect.Clear();
	while ( child ) {
		TGRect childRect;
		child->GetBounds( &childRect );
		if ( childRect.left < buttonsRect.left ) {
			buttonsRect.width += buttonsRect.left - childRect.left;
			buttonsRect.left = childRect.left;
		}
		if ( childRect.top < buttonsRect.top ) {
			buttonsRect.height += buttonsRect.top - childRect.top;
			buttonsRect.top = childRect.top;
		}
		float childRight = childRect.GetRight();
		float right = buttonsRect.GetRight();
		if ( childRight > right ) {
			buttonsRect.width += childRight - right;
		}
		float childBottom = childRect.GetBottom();
		float bottom = buttonsRect.GetBottom();
		if ( childBottom > bottom ) {
			buttonsRect.height += childBottom - bottom;
		}
		child = buttonPane->GetNextChild( child );
	}
	buttonsRect.left = subPane->GetWidth() * 0.5f - buttonsRect.width * 0.5f;
	buttonsRect.top = subPane->GetHeight() - buttonsRect.height;
	buttonPane->Move( buttonsRect.left, buttonsRect.top, false );
	buttonPane->Resize( buttonsRect.width, buttonsRect.height, true );
}

/*
================
TGDialogWindow::DialogEventHandler

Sends a copy of the event, from the window and with the window's event
type, to the destination.
================
*/
// FUNCTION: STBC 0x00739220
void TGDialogWindow::DialogEventHandler( TGEvent *event ) {
	TGEvent *reply = event->Duplicate();
	reply->SetDestination( destination );
	reply->SetSource( this );
	reply->SetEventType( eventType );
	g_kEventManager.AddEvent( reply );
}

/*
================
TGDialogWindow::GotFocus

The window comes to the front of its parent.
================
*/
// FUNCTION: STBC 0x00739260
void TGDialogWindow::GotFocus( bool layout ) {
	parent->MoveToFront( this, layout );
}

/*
================
TGDialogWindow::CreateFromTheme

A dialog in the current theme's dialog frame and button frames; NULL when
no theme is current.
================
*/
// FUNCTION: STBC 0x00739280
TGDialogWindow *TGDialogWindow::CreateFromTheme( float width, float height, char *message, int dialogFlags,
		TGEventHandlerObject *destination ) {
	TGUITheme *theme = g_kUIThemeManager.GetCurrentTheme();
	if ( theme == NULL ) {
		return NULL;
	}
	// BUG: the result is not returned; Create's value is left in EAX
	Create( width, height, message, theme->GetDialogFrameName(), theme->GetDialogFrameScale(),
		theme->GetButtonFrameName(), theme->GetButtonFrameScale(), dialogFlags, destination );
}

/*
================
TGDialogWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x007392d0
bool TGDialogWindow::WriteToStream( TGStream *stream ) {
	if ( !TGFrameWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( eventType );
	stream->WriteChar( draggable );
	if ( destination != NULL ) {
		stream->WriteID( destination->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}
	if ( text != NULL ) {
		stream->WriteChar( true );
		text->WriteToStream( stream );
	} else {
		stream->WriteChar( false );
	}
	if ( buttonPane != NULL ) {
		stream->WriteChar( true );
		buttonPane->WriteToStream( stream );
	} else {
		stream->WriteChar( false );
	}
	return true;
}

/*
================
TGDialogWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x00739370
bool TGDialogWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGFrameWindow::ReadFromStream( stream ) ) {
		return false;
	}
	eventType = stream->ReadLong();
	draggable = false;
	if ( stream->ReadChar() ) {
		draggable = true;
	}
	destination = (TGEventHandlerObject *)stream->ReadID();
	if ( stream->ReadChar() ) {
		text = (TGParagraph *)TGObject::LoadObject( stream, 0 );
	}
	if ( stream->ReadChar() ) {
		buttonPane = (TGPane *)TGObject::LoadObject( stream, 0 );
	}
	return true;
}

/*
================
TGDialogWindow::FixUpPointers
================
*/
// FUNCTION: STBC 0x007393f0
bool TGDialogWindow::FixUpPointers( void ) {
	if ( !TGFrameWindow::FixUpPointers() ) {
		return false;
	}
	if ( destination != NULL && TGObject::GetTGObjectPtr( (long)destination ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGDialogWindow::PostFixUp
================
*/
// FUNCTION: STBC 0x00739420
void TGDialogWindow::PostFixUp( void ) {
	TGFrameWindow::PostFixUp();
	if ( destination != NULL ) {
		destination = (TGEventHandlerObject *)TGObject::GetTGObjectPtr( (long)destination );
	}
}
