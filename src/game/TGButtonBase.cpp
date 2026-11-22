/*
===============================================================================

	TGButtonBase.cpp

===============================================================================
*/

#include <iostream>
#include "TGButtonBase.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "NiPoint2.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0073ed50 _$E32
// FUNCTION: STBC 0x0073ed60 _$E29
// FUNCTION: STBC 0x0073ed70 _$E31
// FUNCTION: STBC 0x0073ed80 _$E30
// FUNCTION: STBC 0x0073ed90 _$E38
// FUNCTION: STBC 0x0073eda0 _$E35
// FUNCTION: STBC 0x0073edb0 _$E37
// FUNCTION: STBC 0x0073edc0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0073edd0 _$E57
// FUNCTION: STBC 0x0073ede0 _$E56
// FUNCTION: STBC 0x0073edf0 _$E61
// FUNCTION: STBC 0x0073ee00 _$E60
// FUNCTION: STBC 0x0073ee10 _$E72
// FUNCTION: STBC 0x0073ee20 _$E71
// FUNCTION: STBC 0x0073ee30 _$E76
// FUNCTION: STBC 0x0073ee40 _$E75
// FUNCTION: STBC 0x0073ee50 _$E80
// FUNCTION: STBC 0x0073ee60 _$E79
// FUNCTION: STBC 0x0073ee70 _$E84
// FUNCTION: STBC 0x0073ee80 _$E83
// FUNCTION: STBC 0x0073ee90 _$E208
// FUNCTION: STBC 0x0073eea0 _$E207
// FUNCTION: STBC 0x0073eeb0 _$E150
// FUNCTION: STBC 0x0073eec0 _$E149
// FUNCTION: STBC 0x0073eed0 _$E163
// FUNCTION: STBC 0x0073eee0 _$E162
// FUNCTION: STBC 0x0073eef0 _$E213
// FUNCTION: STBC 0x0073ef00 _$E212
// FUNCTION: STBC 0x0073ef10 _$E218
// FUNCTION: STBC 0x0073ef20 _$E217
// FUNCTION: STBC 0x0073ef30 _$E222
// FUNCTION: STBC 0x0073ef40 _$E221
// FUNCTION: STBC 0x0073ef50 _$E226
// FUNCTION: STBC 0x0073ef60 _$E225
// FUNCTION: STBC 0x0073ef70 _$E233
// FUNCTION: STBC 0x0073ef80 _$E232
// FUNCTION: STBC 0x0073ef90 _$E264
// FUNCTION: STBC 0x0073efa0 _$E263
// FUNCTION: STBC 0x0073efb0 _$E268
// FUNCTION: STBC 0x0073efc0 _$E267
// FUNCTION: STBC 0x0073efd0 _$E108
// FUNCTION: STBC 0x0073efe0 _$E107
// FUNCTION: STBC 0x0073eff0 _$E250
// FUNCTION: STBC 0x0073f000 _$E249
// FUNCTION: STBC 0x0073f010 _$E257
// FUNCTION: STBC 0x0073f020 _$E256
// FUNCTION: STBC 0x0073f030 _$E280
// FUNCTION: STBC 0x0073f040 _$E279
// FUNCTION: STBC 0x0073f050 _$E284
// FUNCTION: STBC 0x0073f060 _$E283
// FUNCTION: STBC 0x0073f070 _$E324
// FUNCTION: STBC 0x0073f080 _$E323
// FUNCTION: STBC 0x0073f090 _$E330
// FUNCTION: STBC 0x0073f0a0 _$E329
// FUNCTION: STBC 0x0073f0b0 _$E272
// FUNCTION: STBC 0x0073f0c0 _$E271
// FUNCTION: STBC 0x0073f0d0 _$E276
// FUNCTION: STBC 0x0073f0e0 _$E275
// FUNCTION: STBC 0x0073f0f0 _$E288
// FUNCTION: STBC 0x0073f100 _$E287
// FUNCTION: STBC 0x0073f110 _$E292
// FUNCTION: STBC 0x0073f120 _$E291
// FUNCTION: STBC 0x0073f130 _$E296
// FUNCTION: STBC 0x0073f140 _$E295
// FUNCTION: STBC 0x0073f150 _$E300
// FUNCTION: STBC 0x0073f160 _$E299
// FUNCTION: STBC 0x0073f170 _$E304
// FUNCTION: STBC 0x0073f180 _$E303
// FUNCTION: STBC 0x0073f190 _$E308
// FUNCTION: STBC 0x0073f1a0 _$E307
// FUNCTION: STBC 0x0073f1b0 _$E312
// FUNCTION: STBC 0x0073f1c0 _$E311
// FUNCTION: STBC 0x0073f1d0 _$E316
// FUNCTION: STBC 0x0073f1e0 _$E315
// FUNCTION: STBC 0x0073f1f0 _$E113
// FUNCTION: STBC 0x0073f200 _$E112
// FUNCTION: STBC 0x0073f210 _$E320
// FUNCTION: STBC 0x0073f220 _$E319
// FUNCTION: STBC 0x0073f230 _$E118
// FUNCTION: STBC 0x0073f240 _$E117
// FUNCTION: STBC 0x0073f250 _$E334
// FUNCTION: STBC 0x0073f260 _$E333
// FUNCTION: STBC 0x0073f270 _$E338
// FUNCTION: STBC 0x0073f280 _$E337
// FUNCTION: STBC 0x0073f290 _$E342
// FUNCTION: STBC 0x0073f2a0 _$E341
// FUNCTION: STBC 0x0073f2b0 _$E346
// FUNCTION: STBC 0x0073f2c0 _$E345
// FUNCTION: STBC 0x0073f2d0 _$E127
// FUNCTION: STBC 0x0073f2e0 _$E126
// FUNCTION: STBC 0x0073f2f0 _$E350
// FUNCTION: STBC 0x0073f300 _$E349
// FUNCTION: STBC 0x0073f310 _$E354
// FUNCTION: STBC 0x0073f320 _$E353
// FUNCTION: STBC 0x0073f330 _$E358
// FUNCTION: STBC 0x0073f340 _$E357
// FUNCTION: STBC 0x0073f350 _$E133
// FUNCTION: STBC 0x0073f360 _$E132
// FUNCTION: STBC 0x0073f370 _$E138
// FUNCTION: STBC 0x0073f380 _$E137
// FUNCTION: STBC 0x0073f390 _$E168
// FUNCTION: STBC 0x0073f3a0 _$E167
// FUNCTION: STBC 0x0073f3b0 _$E362
// FUNCTION: STBC 0x0073f3c0 _$E361
// FUNCTION: STBC 0x0073f3d0 _$E173
// FUNCTION: STBC 0x0073f3e0 _$E172
// FUNCTION: STBC 0x0073f3f0 _$E366
// FUNCTION: STBC 0x0073f400 _$E365
// FUNCTION: STBC 0x0073f410 _$E370
// FUNCTION: STBC 0x0073f420 _$E369
// FUNCTION: STBC 0x0073f430 _$E178
// FUNCTION: STBC 0x0073f440 _$E177
// FUNCTION: STBC 0x0073f450 _$E374
// FUNCTION: STBC 0x0073f460 _$E373
// FUNCTION: STBC 0x0073f470 _$E143
// FUNCTION: STBC 0x0073f480 _$E142
// FUNCTION: STBC 0x0073f490 _$E378
// FUNCTION: STBC 0x0073f4a0 _$E377
// FUNCTION: STBC 0x0073f4b0 _$E382
// FUNCTION: STBC 0x0073f4c0 _$E381
// FUNCTION: STBC 0x0073f4d0 _$E183
// FUNCTION: STBC 0x0073f4e0 _$E182
// FUNCTION: STBC 0x0073f4f0 _$E188
// FUNCTION: STBC 0x0073f500 _$E187
// FUNCTION: STBC 0x0073f510 _$E386
// FUNCTION: STBC 0x0073f520 _$E385
// FUNCTION: STBC 0x0073f530 _$E390
// FUNCTION: STBC 0x0073f540 _$E389
// FUNCTION: STBC 0x0073f550 _$E394
// FUNCTION: STBC 0x0073f560 _$E393
// FUNCTION: STBC 0x0073f570 _$E398
// FUNCTION: STBC 0x0073f580 _$E397
// FUNCTION: STBC 0x0073f590 _$E402
// FUNCTION: STBC 0x0073f5a0 _$E401
// FUNCTION: STBC 0x0073f5b0 _$E406
// FUNCTION: STBC 0x0073f5c0 _$E405
// FUNCTION: STBC 0x0073f5d0 _$E410
// FUNCTION: STBC 0x0073f5e0 _$E409
// FUNCTION: STBC 0x0073f5f0 _$E414
// FUNCTION: STBC 0x0073f600 _$E413
// FUNCTION: STBC 0x0073f610 _$E193
// FUNCTION: STBC 0x0073f620 _$E192
// FUNCTION: STBC 0x0073f630 _$E418
// FUNCTION: STBC 0x0073f640 _$E417
// FUNCTION: STBC 0x0073f650 _$E198
// FUNCTION: STBC 0x0073f660 _$E197
// FUNCTION: STBC 0x0073f670 _$E203
// FUNCTION: STBC 0x0073f680 _$E202

/*
================
TGButtonBase::Cast

The object as a button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0073f6e0
TGButtonBase *TGButtonBase::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGBUTTONBASE_TYPE ) ) {
		return (TGButtonBase *)object;
	}
	return NULL;
}

/*
================
TGButtonBase::TGButtonBase

A disabled button without events.
================
*/
// FUNCTION: STBC 0x0073f710
TGButtonBase::TGButtonBase( float width, float height ) : TGPane( width, height ) {
	activationEvent = NULL;
	mouseDownEvent = NULL;
	mouseUpEvent = NULL;
	TGUIObject::SetEnabled( false );
}

/*
================
TGButtonBase::~TGButtonBase
================
*/
// FUNCTION: STBC 0x0073f770
TGButtonBase::~TGButtonBase( void ) {
	ClearActivationEvent();
	// BUG: the mouse up event is never released.
	SetMouseDownEvent( NULL );
}

/*
================
TGButtonBase::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0073f7d0
void TGButtonBase::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGButtonBase::MouseHandler, "TGButtonBase::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGButtonBase::KeyboardHandler, "TGButtonBase::KeyboardHandler" );
}

/*
================
TGButtonBase::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0073f800
void TGButtonBase::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGButtonBase::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGButtonBase::KeyboardHandler" );
}

/*
================
TGButtonBase::MouseHandler

A left button press over the button grabs the mouse and selects it; the
release ends the click, sending the activation event when the pointer is
still over the button.
================
*/
// FUNCTION: STBC 0x0073f830
void TGButtonBase::MouseHandler( TGMouseEvent *event ) {
	if ( !IsCompletelyVisible() ) {
		if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
			SetNotSelected( true );
			g_kRootWindow.ReleaseMouse( this );
			event->SetHandled();
		}
		return;
	}
	if ( !IsEnabled() ) {
		return;
	}

	NiPoint2 point;
	point.x = event->x;
	point.y = event->y;
	bool inside = IsWithin( &point );
	if ( inside ) {
		g_kRootWindow.Unknown727EE0( this );
		event->SetHandled();
	}

	if ( !event->IsButtonEvent() ) {
		return;
	}
	int buttons = event->GetFlags();
	if ( !( buttons & MEF_BUTTON_LEFT ) ) {
		return;
	}

	if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
		if ( buttons & MEF_BUTTON_UP ) {
			ClearMouseDown( inside );
			event->SetHandled();
		}
		return;
	}
	if ( buttons & MEF_BUTTON_DOWN ) {
		g_kRootWindow.GrabMouse( this );
		SetSelected( true );
		if ( parent != NULL && parent != UI_NO_PARENT ) {
			parent->SetFocus( this, true );
		}
		SendMouseDownEvent();
	}
	if ( buttons & MEF_BUTTON_UP ) {
		SetNotSelected( true );
	}
	event->SetHandled();
}

/*
================
TGButtonBase::KeyboardHandler

Enter presses an enabled button: down selects it, up sends the activation
event.
================
*/
// FUNCTION: STBC 0x0073f9a0
void TGButtonBase::KeyboardHandler( TGKeyboardEvent *event ) {
	unsigned short unicode = event->GetUnicode();
	int keyState = event->GetKeyState();
	if ( unicode == '\r' && IsEnabled() ) {
		if ( keyState == KS_KEYDOWN ) {
			SetSelected( true );
		} else if ( keyState == KS_KEYUP ) {
			SetNotSelected( true );
			SendActivationEvent();
		}
		event->SetHandled();
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
TGButtonBase::SetActivationEvent
================
*/
// FUNCTION: STBC 0x0073fa10
void TGButtonBase::SetActivationEvent( TGEvent *event ) {
	if ( event != NULL ) {
		event->IncRefCount();
	}
	ClearActivationEvent();
	activationEvent = event;
}

/*
================
TGButtonBase::ClearActivationEvent
================
*/
// FUNCTION: STBC 0x0073fa40
void TGButtonBase::ClearActivationEvent( void ) {
	if ( activationEvent != NULL ) {
		activationEvent->DecRefCount();
		activationEvent = NULL;
	}
}

/*
================
TGButtonBase::SendActivationEvent

Posts a copy of the activation event.
================
*/
// FUNCTION: STBC 0x0073fa60
void TGButtonBase::SendActivationEvent( void ) {
	if ( activationEvent != NULL ) {
		g_kEventManager.AddEvent( activationEvent->Duplicate() );
	}
}

/*
================
TGButtonBase::SetMouseDownEvent
================
*/
// FUNCTION: STBC 0x0073fa80
void TGButtonBase::SetMouseDownEvent( TGEvent *event ) {
	if ( mouseDownEvent != NULL ) {
		mouseDownEvent->DecRefCount();
	}
	mouseDownEvent = event;
	if ( event != NULL ) {
		event->IncRefCount();
	}
}

/*
================
TGButtonBase::SendMouseDownEvent
================
*/
// FUNCTION: STBC 0x0073fab0
void TGButtonBase::SendMouseDownEvent( void ) {
	if ( mouseDownEvent != NULL ) {
		g_kEventManager.AddEvent( mouseDownEvent->Duplicate() );
	}
}

/*
================
TGButtonBase::SetMouseUpEvent
================
*/
// FUNCTION: STBC 0x0073fad0
void TGButtonBase::SetMouseUpEvent( TGEvent *event ) {
	if ( mouseUpEvent != NULL ) {
		mouseUpEvent->DecRefCount();
	}
	mouseUpEvent = event;
	if ( event != NULL ) {
		event->IncRefCount();
	}
}

/*
================
TGButtonBase::SendMouseUpEvent
================
*/
// FUNCTION: STBC 0x0073fb00
void TGButtonBase::SendMouseUpEvent( void ) {
	if ( mouseUpEvent != NULL ) {
		g_kEventManager.AddEvent( mouseUpEvent->Duplicate() );
	}
}

/*
================
TGButtonBase::ClearMouseDown

Ends a click: deselects the button, sends the activation event when asked,
then the mouse up event, and lets the mouse go.
================
*/
// FUNCTION: STBC 0x0073fb20
void TGButtonBase::ClearMouseDown( bool sendActivation ) {
	SetNotSelected( true );
	if ( sendActivation ) {
		SendActivationEvent();
	}
	SendMouseUpEvent();
	g_kRootWindow.ReleaseMouse( this );
}

/*
================
TGButtonBase::SetDisabled

A button disabled in the middle of a click lets the mouse go.
================
*/
// FUNCTION: STBC 0x0073fb60
void TGButtonBase::SetDisabled( bool layout ) {
	if ( g_kRootWindow.GetMouseGrabOwner() == this ) {
		g_kRootWindow.ReleaseMouse( this );
	}
	TGPane::SetDisabled( layout );
}

/*
================
TGButtonBase::WriteToStream

The events are saved as their IDs.
================
*/
// FUNCTION: STBC 0x0073fb90
bool TGButtonBase::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( activationEvent != NULL ? activationEvent->GetObjID() : 0 );
	stream->WriteID( mouseDownEvent != NULL ? mouseDownEvent->GetObjID() : 0 );
	stream->WriteID( mouseUpEvent != NULL ? mouseUpEvent->GetObjID() : 0 );
	return true;
}

/*
================
TGButtonBase::ReadFromStream

Keeps the IDs in the pointers until PostFixUp.
================
*/
// FUNCTION: STBC 0x0073fc10
bool TGButtonBase::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	activationEvent = (TGEvent *)stream->ReadID();
	mouseDownEvent = (TGEvent *)stream->ReadID();
	mouseUpEvent = (TGEvent *)stream->ReadID();
	return true;
}

/*
================
TGButtonBase::FixUpPointers

Fails until the events the stream named are loaded.
================
*/
// FUNCTION: STBC 0x0073fc60
bool TGButtonBase::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	if ( activationEvent != NULL && TGEvent::GetTGEventPtr( (long)activationEvent ) == NULL ) {
		return false;
	}
	if ( mouseDownEvent != NULL && TGEvent::GetTGEventPtr( (long)mouseDownEvent ) == NULL ) {
		return false;
	}
	if ( mouseUpEvent != NULL && TGEvent::GetTGEventPtr( (long)mouseUpEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGButtonBase::PostFixUp

Replaces the IDs with the events.
================
*/
// FUNCTION: STBC 0x0073fcc0
void TGButtonBase::PostFixUp( void ) {
	TGPane::PostFixUp();
	activationEvent = activationEvent != NULL ? TGEvent::GetTGEventPtr( (long)activationEvent ) : NULL;
	mouseDownEvent = mouseDownEvent != NULL ? TGEvent::GetTGEventPtr( (long)mouseDownEvent ) : NULL;
	mouseUpEvent = mouseUpEvent != NULL ? TGEvent::GetTGEventPtr( (long)mouseUpEvent ) : NULL;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0073f690 _$E424
// FUNCTION: STBC 0x0073f6a0 _$E420
// FUNCTION: STBC 0x0073f6b0 _$E423
// FUNCTION: STBC 0x0073f6c0 _$E421
// DATA: STBC 0x0099e318
TGEventHandlerTable TGButtonBase::classHandlers;
