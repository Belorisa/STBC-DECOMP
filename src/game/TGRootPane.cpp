/*
===============================================================================

	TGRootPane.cpp

===============================================================================
*/

#include <windows.h>
#include <stdio.h>

// windows.h renames GetClassName, a TGObject member, to GetClassNameA.
#undef GetClassName

#include "TGRootPane.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGParagraph.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiMain.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00726c50 _$E417
// FUNCTION: STBC 0x00726c60 _$E414
// FUNCTION: STBC 0x00726c70 _$E416
// FUNCTION: STBC 0x00726c80 _$E415
// FUNCTION: STBC 0x00726c90 _$E423
// FUNCTION: STBC 0x00726ca0 _$E420
// FUNCTION: STBC 0x00726cb0 _$E422
// FUNCTION: STBC 0x00726cc0 _$E421

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00726cd0 _$E33
// FUNCTION: STBC 0x00726ce0 _$E32
// FUNCTION: STBC 0x00726cf0 _$E37
// FUNCTION: STBC 0x00726d00 _$E36
// FUNCTION: STBC 0x00726d10 _$E48
// FUNCTION: STBC 0x00726d20 _$E47
// FUNCTION: STBC 0x00726d30 _$E52
// FUNCTION: STBC 0x00726d40 _$E51
// FUNCTION: STBC 0x00726d50 _$E56
// FUNCTION: STBC 0x00726d60 _$E55
// FUNCTION: STBC 0x00726d70 _$E60
// FUNCTION: STBC 0x00726d80 _$E59
// FUNCTION: STBC 0x00726d90 _$E184
// FUNCTION: STBC 0x00726da0 _$E183
// FUNCTION: STBC 0x00726db0 _$E126
// FUNCTION: STBC 0x00726dc0 _$E125
// FUNCTION: STBC 0x00726dd0 _$E139
// FUNCTION: STBC 0x00726de0 _$E138
// FUNCTION: STBC 0x00726e10 _$E194
// FUNCTION: STBC 0x00726e20 _$E193
// FUNCTION: STBC 0x00726e50 _$E202
// FUNCTION: STBC 0x00726e60 _$E201
// FUNCTION: STBC 0x00726e70 _$E209
// FUNCTION: STBC 0x00726e80 _$E208
// FUNCTION: STBC 0x00726ed0 _$E84
// FUNCTION: STBC 0x00726ee0 _$E83
// FUNCTION: STBC 0x007270f0 _$E89
// FUNCTION: STBC 0x00727100 _$E88
// FUNCTION: STBC 0x00727130 _$E94
// FUNCTION: STBC 0x00727140 _$E93
// FUNCTION: STBC 0x007271d0 _$E103
// FUNCTION: STBC 0x007271e0 _$E102
// FUNCTION: STBC 0x00727250 _$E109
// FUNCTION: STBC 0x00727260 _$E108
// FUNCTION: STBC 0x00727270 _$E114
// FUNCTION: STBC 0x00727280 _$E113
// FUNCTION: STBC 0x00727290 _$E144
// FUNCTION: STBC 0x007272a0 _$E143
// FUNCTION: STBC 0x007272d0 _$E149
// FUNCTION: STBC 0x007272e0 _$E148
// FUNCTION: STBC 0x00727330 _$E154
// FUNCTION: STBC 0x00727340 _$E153
// FUNCTION: STBC 0x00727370 _$E119
// FUNCTION: STBC 0x00727380 _$E118
// FUNCTION: STBC 0x007273d0 _$E159
// FUNCTION: STBC 0x007273e0 _$E158
// FUNCTION: STBC 0x007273f0 _$E164
// FUNCTION: STBC 0x00727400 _$E163
// FUNCTION: STBC 0x00727510 _$E169
// FUNCTION: STBC 0x00727520 _$E168
// FUNCTION: STBC 0x00727550 _$E174
// FUNCTION: STBC 0x00727560 _$E173
// FUNCTION: STBC 0x00727570 _$E179
// FUNCTION: STBC 0x00727580 _$E178
// FUNCTION: STBC 0x00726df0 _$E189
// FUNCTION: STBC 0x00726e00 _$E188
// FUNCTION: STBC 0x00726e30 _$E198
// FUNCTION: STBC 0x00726e40 _$E197
// FUNCTION: STBC 0x00726e90 _$E258
// FUNCTION: STBC 0x00726ea0 _$E257
// FUNCTION: STBC 0x00726eb0 _$E262
// FUNCTION: STBC 0x00726ec0 _$E261
// FUNCTION: STBC 0x00726ef0 _$E244
// FUNCTION: STBC 0x00726f00 _$E243
// FUNCTION: STBC 0x00726f10 _$E251
// FUNCTION: STBC 0x00726f20 _$E250
// FUNCTION: STBC 0x00726f30 _$E274
// FUNCTION: STBC 0x00726f40 _$E273
// FUNCTION: STBC 0x00726f50 _$E278
// FUNCTION: STBC 0x00726f60 _$E277
// FUNCTION: STBC 0x00726f70 _$E318
// FUNCTION: STBC 0x00726f80 _$E317
// FUNCTION: STBC 0x00726f90 _$E324
// FUNCTION: STBC 0x00726fa0 _$E323
// FUNCTION: STBC 0x00726fb0 _$E266
// FUNCTION: STBC 0x00726fc0 _$E265
// FUNCTION: STBC 0x00726fd0 _$E270
// FUNCTION: STBC 0x00726fe0 _$E269
// FUNCTION: STBC 0x00726ff0 _$E282
// FUNCTION: STBC 0x00727000 _$E281
// FUNCTION: STBC 0x00727010 _$E286
// FUNCTION: STBC 0x00727020 _$E285
// FUNCTION: STBC 0x00727030 _$E290
// FUNCTION: STBC 0x00727040 _$E289
// FUNCTION: STBC 0x00727050 _$E294
// FUNCTION: STBC 0x00727060 _$E293
// FUNCTION: STBC 0x00727070 _$E298
// FUNCTION: STBC 0x00727080 _$E297
// FUNCTION: STBC 0x00727090 _$E302
// FUNCTION: STBC 0x007270a0 _$E301
// FUNCTION: STBC 0x007270b0 _$E306
// FUNCTION: STBC 0x007270c0 _$E305
// FUNCTION: STBC 0x007270d0 _$E310
// FUNCTION: STBC 0x007270e0 _$E309
// FUNCTION: STBC 0x00727110 _$E314
// FUNCTION: STBC 0x00727120 _$E313
// FUNCTION: STBC 0x00727150 _$E328
// FUNCTION: STBC 0x00727160 _$E327
// FUNCTION: STBC 0x00727170 _$E332
// FUNCTION: STBC 0x00727180 _$E331
// FUNCTION: STBC 0x00727190 _$E336
// FUNCTION: STBC 0x007271a0 _$E335
// FUNCTION: STBC 0x007271b0 _$E340
// FUNCTION: STBC 0x007271c0 _$E339
// FUNCTION: STBC 0x007271f0 _$E344
// FUNCTION: STBC 0x00727200 _$E343
// FUNCTION: STBC 0x00727210 _$E348
// FUNCTION: STBC 0x00727220 _$E347
// FUNCTION: STBC 0x00727230 _$E352
// FUNCTION: STBC 0x00727240 _$E351
// FUNCTION: STBC 0x007272b0 _$E356
// FUNCTION: STBC 0x007272c0 _$E355
// FUNCTION: STBC 0x007272f0 _$E360
// FUNCTION: STBC 0x00727300 _$E359
// FUNCTION: STBC 0x00727310 _$E364
// FUNCTION: STBC 0x00727320 _$E363
// FUNCTION: STBC 0x00727350 _$E368
// FUNCTION: STBC 0x00727360 _$E367
// FUNCTION: STBC 0x00727390 _$E372
// FUNCTION: STBC 0x007273a0 _$E371
// FUNCTION: STBC 0x007273b0 _$E376
// FUNCTION: STBC 0x007273c0 _$E375
// FUNCTION: STBC 0x00727410 _$E380
// FUNCTION: STBC 0x00727420 _$E379
// FUNCTION: STBC 0x00727430 _$E384
// FUNCTION: STBC 0x00727440 _$E383
// FUNCTION: STBC 0x00727450 _$E388
// FUNCTION: STBC 0x00727460 _$E387
// FUNCTION: STBC 0x00727470 _$E392
// FUNCTION: STBC 0x00727480 _$E391
// FUNCTION: STBC 0x00727490 _$E396
// FUNCTION: STBC 0x007274a0 _$E395
// FUNCTION: STBC 0x007274b0 _$E400
// FUNCTION: STBC 0x007274c0 _$E399
// FUNCTION: STBC 0x007274d0 _$E404
// FUNCTION: STBC 0x007274e0 _$E403
// FUNCTION: STBC 0x007274f0 _$E408
// FUNCTION: STBC 0x00727500 _$E407
// FUNCTION: STBC 0x00727530 _$E412
// FUNCTION: STBC 0x00727540 _$E411

// The icon number of the cursor ReadFromStream read, for PostFixUp (invented name).
static int		loadedCursorIconNum;

/*
================
TGRootPane::TGRootPane
================
*/
// FUNCTION: STBC 0x00727620
TGRootPane::TGRootPane( long id ) : TGPane( 1.0f, 1.0f ) {
	bounds.left = bounds.top = 0.0f;
	bounds.width = 1.0f;
	bounds.height = 1.0f;
	Init();
}

/*
================
TGRootPane::GetObjType
================
*/
// FUNCTION: STBC 0x007276b0
int TGRootPane::GetObjType( void ) {
	return TGROOTPANE_TYPE;
}

/*
================
TGRootPane::IsTypeOf
================
*/
// FUNCTION: STBC 0x007276c0
bool TGRootPane::IsTypeOf( int type ) {
	if ( type == TGROOTPANE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
TGRootPane::GetClassName
================
*/
// FUNCTION: STBC 0x00727720
char *TGRootPane::GetClassName( void ) {
	return "TGRootPane";
}

/*
================
TGRootPane::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00727730
char *TGRootPane::GetSwigTypeName( void ) {
	return "_p_TGRootPane";
}

/*
================
TGRootPane::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00727740
char *TGRootPane::GetPythonClassName( void ) {
	return "TGRootPanePtr";
}

/*
================
TGRootPane::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00727750
TGEventHandlerTable *TGRootPane::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TGRootPane::Init

The root pane has no parent, is always completely visible and starts
without a cursor, focus or frame rate display.
================
*/
// FUNCTION: STBC 0x00727790
void TGRootPane::Init( void ) {
	SetParent( UI_NO_PARENT, true );
	flags |= TGUIO_COMPLETELY_VISIBLE;

	cursor = NULL;
	cursorScale = 1.0f;
	cursorHotSpot = 0;
	cursorVisible = 0;
	hotSpotX = 0.0f;
	hotSpotY = 0.0f;
	trueFocus = NULL;
	mouseGrabOwner = NULL;
	mouseDepth = 0;
	deepestMouseDepth = 0;
	grabDepth = 0;
	numMouseObjects = 0;
	mouseObjects = NULL;
	showFPS = false;
	fpsText = NULL;

	lastFPSTime = GetTickCount();
	unknown94 = 1;
	// The original clears the first period's counters each time round
	// instead of each period's.
	for ( int i = 0; i < FPS_PERIODS; i++ ) {
		fpsValue[0] = 0;
		fpsFrames[0] = 0;
		fpsTime[0] = 0;
	}
}

/*
================
TGRootPane::~TGRootPane
================
*/
// FUNCTION: STBC 0x00727840
TGRootPane::~TGRootPane( void ) {
	Shutdown();
}

/*
================
TGRootPane::Shutdown

Destroys the cursor, the pushed cursors and the frame rate display.
================
*/
// FUNCTION: STBC 0x007278a0
void TGRootPane::Shutdown( void ) {
	DeleteCursor();
	while ( !cursorStack.IsEmpty() ) {
		TGIcon *icon = cursorStack.Pop();
		if ( icon != NULL ) {
			delete icon;
		}
	}
	cursorVisible = 0;
	free( mouseObjects );
	mouseObjects = NULL;
	if ( fpsText != NULL ) {
		delete fpsText;
		fpsText = NULL;
	}
}

/*
================
TGRootPane::DeleteCursor
================
*/
// FUNCTION: STBC 0x00727920
void TGRootPane::DeleteCursor( void ) {
	if ( cursor != NULL ) {
		delete cursor;
		cursor = NULL;
	}
}

/*
================
TGRootPane::ShowFPS

Shows the frame rates in the bottom left corner.
================
*/
// FUNCTION: STBC 0x00727940
void TGRootPane::ShowFPS( void ) {
	if ( showFPS == true ) {
		return;
	}
	showFPS = true;
	fpsText = new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	fpsText->SetParent( this, true );
	fpsText->VirtualSlot59();
	fpsText->SetString( "0:0", true );
	fpsText->SetPosition( 1.0f / g_kIconManager.GetScreenWidth(), 1.0f - fpsText->GetHeight() - 1.0f / g_kIconManager.GetScreenHeight(), true );
}

/*
================
TGRootPane::HideFPS
================
*/
// FUNCTION: STBC 0x00727a10
void TGRootPane::HideFPS( void ) {
	showFPS = false;
	if ( fpsText != NULL ) {
		delete fpsText;
		fpsText = NULL;
	}
}

/*
================
TGRootPane::UpdateFPS

Counts a frame, and shows the rates of the periods that ended.
================
*/
// FUNCTION: STBC 0x00727a40
void TGRootPane::UpdateFPS( void ) {
	unsigned long now = GetTickCount();
	unsigned long elapsed = now - lastFPSTime;
	lastFPSTime = now;

	unsigned long periods[FPS_PERIODS] = { 500, 5000, 60000 };
	bool changed = false;

	for ( int i = 0; i < FPS_PERIODS; i++ ) {
		fpsTime[i] += elapsed;
		fpsFrames[i]++;
		if ( fpsTime[i] > periods[i] ) {
			fpsValue[i] = fpsFrames[i] * 1000 / fpsTime[i];
			changed = true;
			fpsTime[i] = 0;
			fpsFrames[i] = 0;
		}
	}

	if ( changed == true && showFPS == true ) {
		char text[256];
		sprintf( text, "%d : %d : %d", fpsValue[0], fpsValue[1], fpsValue[2] );
		fpsText->SetString( text, true );
	}
}

// The last mouse position, by value (invented, with its named local:
// SetMouseCursor's second NiPoint2 slot needs the copy level; no evidence
// shows it).
static inline NiPoint2 GetLastMousePosition( void ) { NiPoint2 position = lastMousePosition; return position; }

/*
================
TGRootPane::SetMouseCursor

Replaces the cursor with an icon of a group, keeping its place and whether
it was shown. The hot spot is placed in the icon by the MCPP_* flags.
================
*/
// FUNCTION: STBC 0x00727b30
void TGRootPane::SetMouseCursor( char *iconGroupName, int iconNum, int hotSpot, float scale ) {
	// MATCH-PENDING: R17, exact only through the invented by-value accessor GetLastMousePosition (above, with its named local), whose copy level is retail's second NiPoint2 slot; no evidence shows it (user's ruling, 2026-11-18)
	NiPoint2 position = GetLastMousePosition();
	bool wasVisible = false;
	if ( cursor != NULL ) {
		wasVisible = cursor->IsVisible() ? true : false;
	}
	if ( cursor != NULL ) {
		cursor->GetPosition( &position );
	}

	DeleteCursor();
	cursor = new TGIcon( iconGroupName, iconNum );
	if ( cursor == NULL ) {
		return;
	}
	cursor->SetParent( this, true );
	cursor->VirtualSlot59();

	float width = cursor->GetWidth();
	float height = cursor->GetHeight();
	cursorScale = scale;
	cursorHotSpot = hotSpot;
	hotSpotX = width;
	hotSpotY = height;
	switch ( hotSpot & MCPP_VERTICAL_MASK ) {
		case MCPP_VERTICAL_TOP:
			hotSpotY = 0.0f;
			break;
		case MCPP_VERTICAL_CENTER:
			hotSpotY = height * 0.5f;
			break;
	}
	switch ( hotSpot & MCPP_HORIZONTAL_MASK ) {
		case MCPP_HORIZONTAL_LEFT:
			hotSpotX = 0.0f;
			break;
		case MCPP_HORIZONTAL_CENTER:
			hotSpotX = width * 0.5f;
			break;
	}

	if ( scale != 1.0f ) {
		hotSpotX = scale * hotSpotX;
		hotSpotY = scale * hotSpotY;
		cursor->Resize( width * scale, height * scale, true );
	}
	cursor->SetPosition( position.x, position.y, true );

	if ( cursorVisible > 0 || wasVisible == true ) {
		cursor->SetVisible( true );
	} else {
		cursor->SetNotVisible( true );
	}
}

/*
================
TGRootPane::SetCursorVisible

Counts the requests: the cursor shows while there are more to show it than
to hide it.
================
*/
// FUNCTION: STBC 0x00727cd0
void TGRootPane::SetCursorVisible( bool visible, bool layout ) {
	TGIcon *icon = GetMouseCursor();
	if ( visible ) {
		cursorVisible++;
	} else {
		cursorVisible--;
	}
	if ( icon != NULL ) {
		if ( cursorVisible > 0 ) {
			icon->SetVisible( layout );
		} else {
			icon->SetNotVisible( layout );
		}
	}
}

/*
================
TGRootPane::PushCursor

Keeps the cursor for PopCursor and sets a new one. The pushed cursors are
kept first in, first out.
================
*/
// FUNCTION: STBC 0x00727d10
void TGRootPane::PushCursor( char *iconGroupName, int iconNum, int hotSpot, float scale ) {
	cursorStack.Push( cursor );
	cursor = NULL;
	SetMouseCursor( iconGroupName, iconNum, hotSpot, scale );
}

/*
================
TGRootPane::PopCursor

Destroys the cursor and takes back the oldest pushed one.
================
*/
// FUNCTION: STBC 0x00727e30
void TGRootPane::PopCursor( void ) {
	if ( cursorStack.IsEmpty() ) {
		return;
	}
	if ( cursor != NULL ) {
		delete cursor;
	}
	cursor = cursorStack.Pop();
	if ( cursorVisible > 0 ) {
		cursor->SetVisible( true );
	} else {
		cursor->SetNotVisible( true );
	}
}

/*
================
TGRootPane::Layout
================
*/
// FUNCTION: STBC 0x00727ea0
void TGRootPane::Layout( void ) {
	TGPane::Layout();
	if ( fpsText != NULL ) {
		fpsText->Layout();
	}
	if ( cursor != NULL && cursor->IsVisible() ) {
		cursor->Layout();
	}
}

/*
================
TGRootPane::Unknown727EE0

Records the object under the mouse at the current depth, growing the
stack as the tree deepens, and tells the object it replaces that the mouse
left it and the new one that the mouse entered it.
================
*/
// FUNCTION: STBC 0x00727ee0
void TGRootPane::Unknown727EE0( TGUIObject *object ) {
	if ( mouseDepth > deepestMouseDepth ) {
		deepestMouseDepth = mouseDepth;
	}
	if ( mouseDepth >= numMouseObjects ) {
		unsigned short depth = numMouseObjects;
		numMouseObjects = mouseDepth + 1;
		mouseObjects = (TGUIObject **)realloc( mouseObjects, numMouseObjects * sizeof( TGUIObject * ) );
		for ( ; depth < numMouseObjects; depth++ ) {
			mouseObjects[depth] = NULL;
		}
	}

	TGUIObject *oldObject = mouseObjects[mouseDepth];
	if ( oldObject != object ) {
		if ( oldObject != NULL ) {
			oldObject->MouseExited( object );
		}
		mouseObjects[mouseDepth] = object;
		if ( object != NULL ) {
			object->MouseEntered( oldObject );
		}
	}
}

/*
================
TGRootPane::Unknown727FA0
================
*/
// FUNCTION: STBC 0x00727fa0
void TGRootPane::Unknown727FA0( TGUIObject *object ) {
	if ( mouseObjects == NULL ) {
		return;
	}
	for ( int depth = 0; depth < numMouseObjects; depth++ ) {
		if ( mouseObjects[depth] == object ) {
			mouseObjects[depth] = NULL;
			return;
		}
	}
}

/*
================
TGRootPane::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00727fe0
void TGRootPane::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGRootPane::MouseHandler, "TGRootPane::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGRootPane::KeyboardHandler, "TGRootPane::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGRootPane::GamepadHandler, "TGRootPane::GamepadHandler" );
}

/*
================
TGRootPane::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00728020
void TGRootPane::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGRootPane::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGRootPane::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GAMEPAD, "TGRootPane::GamepadHandler" );
}

/*
================
TGRootPane::MouseHandler

Moves the cursor to the mouse. An object that grabbed the mouse gets the
event in its own coordinates, at its depth, and loses the mouse objects
deeper down when the mouse is outside the screen; otherwise the event goes
down the tree.
================
*/
// FUNCTION: STBC 0x00728060
void TGRootPane::MouseHandler( TGMouseEvent *event ) {
	if ( !IsVisible() ) {
		return;
	}

	NiPoint2 position;
	position.x = event->x;
	position.y = event->y;
	if ( cursor != NULL ) {
		cursor->SetPosition( position.x - hotSpotX, position.y - hotSpotY, true );
	}

	if ( mouseGrabOwner != NULL ) {
		NiPoint2 screenPosition;
		screenPosition.x = position.x;
		screenPosition.y = position.y;
		NiPoint2 offset;
		mouseGrabOwner->GetScreenOffset( &offset );
		position -= offset;
		event->x = position.x;
		event->y = position.y;
		event->SetDestination( mouseGrabOwner );
		mouseDepth = grabDepth;
		if ( position.x < 0.0f || bounds.width < position.x || position.y < 0.0f || bounds.height < position.y ) {
			Unknown727EE0( NULL );
		}
		mouseGrabOwner->ProcessEvent( event );
		event->x = screenPosition.x;
		event->y = screenPosition.y;
	} else {
		PassMouseEvent( event );
	}
}

/*
================
TGRootPane::PassMouseEvent

Sends the event down the tree, then forgets the objects under the mouse
deeper than the event went.
================
*/
// FUNCTION: STBC 0x00728180
void TGRootPane::PassMouseEvent( TGMouseEvent *event ) {
	deepestMouseDepth = 0;
	mouseDepth = 0;
	CallNextHandler( event );
	for ( int depth = deepestMouseDepth + 1; depth < numMouseObjects; depth++ ) {
		mouseDepth = depth;
		Unknown727EE0( NULL );
	}
}

/*
================
TGRootPane::KeyboardHandler

Gives the event to each visible child in turn until one handles it.
================
*/
// FUNCTION: STBC 0x007281d0
void TGRootPane::KeyboardHandler( TGKeyboardEvent *event ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos && !event->EventHandled() ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsVisible() ) {
			event->SetDestination( child );
			child->ProcessEvent( event );
		}
	}
}

/*
================
TGRootPane::GamepadHandler

As KeyboardHandler.
================
*/
// FUNCTION: STBC 0x00728220
void TGRootPane::GamepadHandler( TGIEvent *event ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos && !event->EventHandled() ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsVisible() ) {
			event->SetDestination( child );
			child->ProcessEvent( event );
		}
	}
}

/*
================
TGRootPane::VirtualSlot60

Takes the cursors out of their batches too.
================
*/
// FUNCTION: STBC 0x00728270
void TGRootPane::VirtualSlot60( void ) {
	TGPane::VirtualSlot60();
	if ( cursor != NULL ) {
		cursor->VirtualSlot60();
	}

	TGQueue<TGIcon *> pushed;
	while ( !cursorStack.IsEmpty() ) {
		TGIcon *icon = cursorStack.Pop();
		if ( icon != NULL ) {
			icon->VirtualSlot60();
		}
		pushed.Push( icon );
	}
	while ( !pushed.IsEmpty() ) {
		cursorStack.Push( pushed.Pop() );
	}
}

/*
================
TGRootPane::VirtualSlot63

The frame rate display and the cursor fill the batches too.
================
*/
// FUNCTION: STBC 0x00728420
void TGRootPane::VirtualSlot63( void *argument ) {
	TGPane::VirtualSlot63( argument );
	if ( fpsText != NULL ) {
		fpsText->VirtualSlot63( argument );
	}
	if ( cursor != NULL ) {
		cursor->VirtualSlot63( argument );
	}
}

/*
================
TGRootPane::UpdateTrueFocus

Moves the true focus to the end of the chain of focused children, telling
the objects that leave and join the focus path.
================
*/
// FUNCTION: STBC 0x00728460
void TGRootPane::UpdateTrueFocus( bool layout ) {
	TGUIObject *newFocus = GetFocusLeaf();
	if ( trueFocus == newFocus ) {
		return;
	}

	TGUIObject *ancestor = NULL;
	if ( trueFocus != NULL ) {
		trueFocus->LostTrueFocus( layout );
		ancestor = trueFocus->GetCommonAncestor( newFocus );
	}
	TGUIObject *oldFocus = trueFocus;
	trueFocus = newFocus;
	if ( oldFocus != NULL ) {
		LoseTrueFocusPath( oldFocus, ancestor, layout );
	}
	if ( trueFocus != NULL ) {
		trueFocus->GotTrueFocus( layout );
	}
	GainTrueFocusPath( ancestor, layout );
}

/*
================
TGRootPane::LoseTrueFocusPath

Tells the old focus and its parents below the common ancestor that they
left the focus path.
================
*/
// FUNCTION: STBC 0x007284c0
void TGRootPane::LoseTrueFocusPath( TGUIObject *oldFocus, TGUIObject *ancestor, bool layout ) {
	if ( ancestor == oldFocus || oldFocus == NULL ) {
		return;
	}
	oldFocus->LostTrueFocusPath( layout );
	for ( TGPane *pane = oldFocus->GetParent(); pane != NULL && pane != ancestor && pane != UI_NO_PARENT; pane = pane->GetParent() ) {
		pane->LostTrueFocusPath( layout );
	}
}

/*
================
TGRootPane::GainTrueFocusPath

Tells the new focus and its parents below the common ancestor, from the
focus path, from the ancestor down, that they joined it.
================
*/
// FUNCTION: STBC 0x00728510
void TGRootPane::GainTrueFocusPath( TGUIObject *ancestor, bool layout ) {
	TGUIObject *object = trueFocus;
	if ( ancestor == object || object == NULL ) {
		return;
	}

	NiTList<TGUIObject *> path( 2 );
	path.AddHead( object );
	for ( TGPane *pane = trueFocus->GetParent(); pane != NULL && pane != ancestor && pane != UI_NO_PARENT; pane = pane->GetParent() ) {
		path.AddHead( pane );
	}

	NiTListIterator pos = path.GetHeadPos();
	while ( pos ) {
		path.GetNext( pos )->GotTrueFocusPath( layout );
	}
}

/*
================
TGRootPane::GrabMouse

Sends the mouse events to the object, which must be under the mouse, until
it releases it. False when another object has the mouse.
================
*/
// FUNCTION: STBC 0x007286c0
bool TGRootPane::GrabMouse( TGUIObject *object ) {
	if ( mouseGrabOwner == NULL ) {
		unsigned short depth;
		for ( depth = 0; depth < numMouseObjects; depth++ ) {
			if ( mouseObjects[depth] == object ) {
				break;
			}
		}
		if ( depth < numMouseObjects ) {
			grabDepth = depth;
			mouseGrabOwner = object;
			return true;
		}
	}
	return false;
}

/*
================
TGRootPane::ReleaseMouse
================
*/
// FUNCTION: STBC 0x00728720
bool TGRootPane::ReleaseMouse( TGUIObject *object ) {
	if ( mouseGrabOwner == object ) {
		mouseGrabOwner = NULL;
		return true;
	}
	return false;
}

/*
================
TGRootPane::WriteToStream

The cursor is written as its group, scale, hot spot and number; the pushed
cursors as objects, each after a true, then a false.
================
*/
// FUNCTION: STBC 0x00728740
bool TGRootPane::WriteToStream( TGStream *stream ) {
	VirtualSlot60();
	if ( !TGPane::WriteToStream( stream ) ) {
		Layout();
		return false;
	}

	if ( cursor != NULL ) {
		stream->VirtualSlot11( cursor->GetIconGroupName() );
		stream->WriteFloat( cursorScale );
		stream->WriteLong( cursorHotSpot );
		stream->WriteLong( cursor->GetIconNum() );
	} else {
		stream->VirtualSlot11( NULL );
	}
	stream->WriteInt( cursorVisible );

	TGQueue<TGIcon *> pushed;
	while ( !cursorStack.IsEmpty() ) {
		TGIcon *icon = cursorStack.Pop();
		pushed.Push( icon );
		if ( icon != NULL ) {
			stream->WriteBool( true );
			icon->WriteToStream( stream );
		}
	}
	stream->WriteBool( false );
	while ( !pushed.IsEmpty() ) {
		cursorStack.Push( pushed.Pop() );
	}

	stream->WriteID( GetSavedID( trueFocus ) );
	stream->WriteID( GetSavedID( mouseGrabOwner ) );
	stream->WriteShort( mouseDepth );
	stream->WriteShort( deepestMouseDepth );
	stream->WriteShort( grabDepth );
	stream->WriteShort( numMouseObjects );
	for ( unsigned short depth = 0; depth < numMouseObjects; depth++ ) {
		stream->WriteID( GetSavedID( mouseObjects[depth] ) );
	}
	Layout();
	return true;
}

/*
================
TGRootPane::ReadFromStream

Keeps the cursor's group name in the cursor pointer, and the IDs in the
object pointers, until PostFixUp.
================
*/
// FUNCTION: STBC 0x00728a40
bool TGRootPane::ReadFromStream( TGStream *stream ) {
	Shutdown();
	Init();
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}

	cursor = (TGIcon *)stream->VirtualSlot10( -1 );
	if ( cursor != NULL ) {
		cursorScale = stream->ReadFloat();
		cursorHotSpot = stream->ReadLong();
		loadedCursorIconNum = stream->ReadLong();
	}
	cursorVisible = stream->ReadInt();
	while ( stream->ReadBool() == true ) {
		cursorStack.Push( (TGIcon *)LoadObject( stream, 0 ) );
	}

	trueFocus = (TGUIObject *)stream->ReadID();
	mouseGrabOwner = (TGUIObject *)stream->ReadID();
	mouseDepth = stream->ReadShort();
	deepestMouseDepth = stream->ReadShort();
	grabDepth = stream->ReadShort();
	numMouseObjects = stream->ReadShort();
	mouseObjects = (TGUIObject **)malloc( numMouseObjects * sizeof( TGUIObject * ) );
	for ( unsigned short depth = 0; depth < numMouseObjects; depth++ ) {
		mouseObjects[depth] = (TGUIObject *)stream->ReadID();
	}
	return true;
}

/*
================
TGRootPane::FixUpPointers

False while the cursor's group or a saved object is missing.
================
*/
// FUNCTION: STBC 0x00728c60
bool TGRootPane::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	if ( g_kIconManager.GetIconGroup( (char *)cursor ) == NULL ) {
		return false;
	}
	if ( trueFocus != NULL && GetTGObjectPtr( (long)trueFocus ) == NULL ) {
		return false;
	}
	for ( unsigned short depth = 0; depth < numMouseObjects; depth++ ) {
		if ( mouseObjects[depth] != NULL && GetTGObjectPtr( (long)mouseObjects[depth] ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
TGRootPane::PostFixUp

Makes the cursor from the saved group name. The grab owner is not fixed up,
and the cursor comes back at its unscaled size.
================
*/
// FUNCTION: STBC 0x00728ce0
void TGRootPane::PostFixUp( void ) {
	TGPane::PostFixUp();

	char *cursorName = (char *)cursor;
	cursor = NULL;
	SetMouseCursor( cursorName, loadedCursorIconNum, cursorHotSpot, 1.0f );
	delete cursorName;
	if ( cursorVisible > 0 ) {
		cursor->SetVisible( true );
	} else {
		cursor->SetNotVisible( true );
	}

	trueFocus = (TGUIObject *)GetSavedObject( (long)trueFocus );
	for ( unsigned short depth = 0; depth < numMouseObjects; depth++ ) {
		// ReadFromStream left the saved ID in the slot
		long id = (long)mouseObjects[depth];
		if ( id == 0 ) {
			mouseObjects[depth] = NULL;
		} else {
			mouseObjects[depth] = (TGUIObject *)GetTGObjectPtr( id );
		}

	}
}

/*
================
TGRootPane::Unknown728D90

Forgets the children, the cursors and the objects under the mouse without
destroying them.
================
*/
// FUNCTION: STBC 0x00728d90
void TGRootPane::Unknown728D90( void ) {
	children.RemoveAll();
	while ( !cursorStack.IsEmpty() ) {
		cursorStack.Pop();
	}
	cursor = NULL;
	trueFocus = NULL;
	mouseGrabOwner = NULL;
	mouseDepth = 0;
	deepestMouseDepth = 0;
	grabDepth = 0;
	numMouseObjects = 0;
	delete mouseObjects;
	mouseObjects = NULL;
	showFPS = false;
	fpsText = NULL;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00727590 _$E429
// FUNCTION: STBC 0x007275a0 _$E425
// FUNCTION: STBC 0x007275b0 _$E428
// FUNCTION: STBC 0x007275c0 _$E426

// DATA: STBC 0x0099d1a8
TGEventHandlerTable TGRootPane::classHandlers;

// The root window's constructor and its exit-time destructor.
// FUNCTION: STBC 0x007275e0 _$E434
// FUNCTION: STBC 0x007275f0 _$E431
// FUNCTION: STBC 0x00727600 _$E433
// FUNCTION: STBC 0x00727610 _$E432

// DATA: STBC 0x0099d040
TGRootPane g_kRootWindow;

// The standard library's locale facet id (<iostream>): a test-and-set initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00729000 _$E442
// FUNCTION: STBC 0x00729020 _$E441
