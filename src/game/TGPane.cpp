/*
===============================================================================

	TGPane.cpp

===============================================================================
*/

#include "TGPane.h"
#include "TGRootPane.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "NiMain.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0072d300 _$E411
// FUNCTION: STBC 0x0072d310 _$E408
// FUNCTION: STBC 0x0072d320 _$E410
// FUNCTION: STBC 0x0072d330 _$E409
// FUNCTION: STBC 0x0072d340 _$E417
// FUNCTION: STBC 0x0072d350 _$E414
// FUNCTION: STBC 0x0072d360 _$E416
// FUNCTION: STBC 0x0072d370 _$E415

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0072d380 _$E33
// FUNCTION: STBC 0x0072d390 _$E32
// FUNCTION: STBC 0x0072d3a0 _$E37
// FUNCTION: STBC 0x0072d3b0 _$E36
// FUNCTION: STBC 0x0072d3c0 _$E48
// FUNCTION: STBC 0x0072d3d0 _$E47
// FUNCTION: STBC 0x0072d3e0 _$E52
// FUNCTION: STBC 0x0072d3f0 _$E51
// FUNCTION: STBC 0x0072d400 _$E56
// FUNCTION: STBC 0x0072d410 _$E55
// FUNCTION: STBC 0x0072d420 _$E60
// FUNCTION: STBC 0x0072d430 _$E59
// FUNCTION: STBC 0x0072d440 _$E184
// FUNCTION: STBC 0x0072d450 _$E183
// FUNCTION: STBC 0x0072d460 _$E126
// FUNCTION: STBC 0x0072d470 _$E125
// FUNCTION: STBC 0x0072d480 _$E139
// FUNCTION: STBC 0x0072d490 _$E138
// FUNCTION: STBC 0x0072d4c0 _$E194
// FUNCTION: STBC 0x0072d4d0 _$E193
// FUNCTION: STBC 0x0072d500 _$E202
// FUNCTION: STBC 0x0072d510 _$E201
// FUNCTION: STBC 0x0072d520 _$E209
// FUNCTION: STBC 0x0072d530 _$E208
// FUNCTION: STBC 0x0072d580 _$E84
// FUNCTION: STBC 0x0072d590 _$E83
// FUNCTION: STBC 0x0072d7a0 _$E89
// FUNCTION: STBC 0x0072d7b0 _$E88
// FUNCTION: STBC 0x0072d7e0 _$E94
// FUNCTION: STBC 0x0072d7f0 _$E93
// FUNCTION: STBC 0x0072d880 _$E103
// FUNCTION: STBC 0x0072d890 _$E102
// FUNCTION: STBC 0x0072d900 _$E109
// FUNCTION: STBC 0x0072d910 _$E108
// FUNCTION: STBC 0x0072d920 _$E114
// FUNCTION: STBC 0x0072d930 _$E113
// FUNCTION: STBC 0x0072d940 _$E144
// FUNCTION: STBC 0x0072d950 _$E143
// FUNCTION: STBC 0x0072d980 _$E149
// FUNCTION: STBC 0x0072d990 _$E148
// FUNCTION: STBC 0x0072d9e0 _$E154
// FUNCTION: STBC 0x0072d9f0 _$E153
// FUNCTION: STBC 0x0072da20 _$E119
// FUNCTION: STBC 0x0072da30 _$E118
// FUNCTION: STBC 0x0072da80 _$E159
// FUNCTION: STBC 0x0072da90 _$E158
// FUNCTION: STBC 0x0072daa0 _$E164
// FUNCTION: STBC 0x0072dab0 _$E163
// FUNCTION: STBC 0x0072dbc0 _$E169
// FUNCTION: STBC 0x0072dbd0 _$E168
// FUNCTION: STBC 0x0072dc00 _$E174
// FUNCTION: STBC 0x0072dc10 _$E173
// FUNCTION: STBC 0x0072dc20 _$E179
// FUNCTION: STBC 0x0072dc30 _$E178
// FUNCTION: STBC 0x0072d4a0 _$E189
// FUNCTION: STBC 0x0072d4b0 _$E188
// FUNCTION: STBC 0x0072d4e0 _$E198
// FUNCTION: STBC 0x0072d4f0 _$E197
// FUNCTION: STBC 0x0072d540 _$E239
// FUNCTION: STBC 0x0072d550 _$E238
// FUNCTION: STBC 0x0072d560 _$E243
// FUNCTION: STBC 0x0072d570 _$E242
// FUNCTION: STBC 0x0072d5a0 _$E225
// FUNCTION: STBC 0x0072d5b0 _$E224
// FUNCTION: STBC 0x0072d5c0 _$E232
// FUNCTION: STBC 0x0072d5d0 _$E231
// FUNCTION: STBC 0x0072d5e0 _$E255
// FUNCTION: STBC 0x0072d5f0 _$E254
// FUNCTION: STBC 0x0072d600 _$E259
// FUNCTION: STBC 0x0072d610 _$E258
// FUNCTION: STBC 0x0072d620 _$E299
// FUNCTION: STBC 0x0072d630 _$E298
// FUNCTION: STBC 0x0072d640 _$E305
// FUNCTION: STBC 0x0072d650 _$E304
// FUNCTION: STBC 0x0072d660 _$E247
// FUNCTION: STBC 0x0072d670 _$E246
// FUNCTION: STBC 0x0072d680 _$E251
// FUNCTION: STBC 0x0072d690 _$E250
// FUNCTION: STBC 0x0072d6a0 _$E263
// FUNCTION: STBC 0x0072d6b0 _$E262
// FUNCTION: STBC 0x0072d6c0 _$E267
// FUNCTION: STBC 0x0072d6d0 _$E266
// FUNCTION: STBC 0x0072d6e0 _$E271
// FUNCTION: STBC 0x0072d6f0 _$E270
// FUNCTION: STBC 0x0072d700 _$E275
// FUNCTION: STBC 0x0072d710 _$E274
// FUNCTION: STBC 0x0072d720 _$E279
// FUNCTION: STBC 0x0072d730 _$E278
// FUNCTION: STBC 0x0072d740 _$E283
// FUNCTION: STBC 0x0072d750 _$E282
// FUNCTION: STBC 0x0072d760 _$E287
// FUNCTION: STBC 0x0072d770 _$E286
// FUNCTION: STBC 0x0072d780 _$E291
// FUNCTION: STBC 0x0072d790 _$E290
// FUNCTION: STBC 0x0072d7c0 _$E295
// FUNCTION: STBC 0x0072d7d0 _$E294
// FUNCTION: STBC 0x0072d800 _$E309
// FUNCTION: STBC 0x0072d810 _$E308
// FUNCTION: STBC 0x0072d820 _$E313
// FUNCTION: STBC 0x0072d830 _$E312
// FUNCTION: STBC 0x0072d840 _$E317
// FUNCTION: STBC 0x0072d850 _$E316
// FUNCTION: STBC 0x0072d860 _$E321
// FUNCTION: STBC 0x0072d870 _$E320
// FUNCTION: STBC 0x0072d8a0 _$E325
// FUNCTION: STBC 0x0072d8b0 _$E324
// FUNCTION: STBC 0x0072d8c0 _$E329
// FUNCTION: STBC 0x0072d8d0 _$E328
// FUNCTION: STBC 0x0072d8e0 _$E333
// FUNCTION: STBC 0x0072d8f0 _$E332
// FUNCTION: STBC 0x0072d960 _$E337
// FUNCTION: STBC 0x0072d970 _$E336
// FUNCTION: STBC 0x0072d9a0 _$E341
// FUNCTION: STBC 0x0072d9b0 _$E340
// FUNCTION: STBC 0x0072d9c0 _$E345
// FUNCTION: STBC 0x0072d9d0 _$E344
// FUNCTION: STBC 0x0072da00 _$E349
// FUNCTION: STBC 0x0072da10 _$E348
// FUNCTION: STBC 0x0072da40 _$E353
// FUNCTION: STBC 0x0072da50 _$E352
// FUNCTION: STBC 0x0072da60 _$E357
// FUNCTION: STBC 0x0072da70 _$E356
// FUNCTION: STBC 0x0072dac0 _$E361
// FUNCTION: STBC 0x0072dad0 _$E360
// FUNCTION: STBC 0x0072dae0 _$E365
// FUNCTION: STBC 0x0072daf0 _$E364
// FUNCTION: STBC 0x0072db00 _$E369
// FUNCTION: STBC 0x0072db10 _$E368
// FUNCTION: STBC 0x0072db20 _$E373
// FUNCTION: STBC 0x0072db30 _$E372
// FUNCTION: STBC 0x0072db40 _$E377
// FUNCTION: STBC 0x0072db50 _$E376
// FUNCTION: STBC 0x0072db60 _$E381
// FUNCTION: STBC 0x0072db70 _$E380
// FUNCTION: STBC 0x0072db80 _$E385
// FUNCTION: STBC 0x0072db90 _$E384
// FUNCTION: STBC 0x0072dba0 _$E389
// FUNCTION: STBC 0x0072dbb0 _$E388
// FUNCTION: STBC 0x0072dbe0 _$E393
// FUNCTION: STBC 0x0072dbf0 _$E392

/*
================
TGPane::Cast

The object as a pane; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0072dc90
TGPane *TGPane::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGPANE_TYPE ) ) {
		return (TGPane *)object;
	}
	return NULL;
}

/*
================
TGPane::TGPane
================
*/
// FUNCTION: STBC 0x0072dcc0 ??0TGPane@@QAE@MM@Z
TGPane::TGPane( float width, float height ) : children( 2 ) {
	bounds.left = bounds.top = 0.0f;
	Resize( width, height, false );
	focus = NULL;
}

/*
================
TGPane::GetObjType
================
*/
// FUNCTION: STBC 0x0072dd40
int TGPane::GetObjType( void ) {
	return TGPANE_TYPE;
}

/*
================
TGPane::GetClassName
================
*/
// FUNCTION: STBC 0x0072dda0
char *TGPane::GetClassName( void ) {
	return "TGPane";
}

/*
================
TGPane::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0072ddb0
char *TGPane::GetSwigTypeName( void ) {
	return "_p_TGPane";
}

/*
================
TGPane::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0072ddc0
char *TGPane::GetPythonClassName( void ) {
	return "TGPanePtr";
}

/*
================
TGPane::TGPane
================
*/
// FUNCTION: STBC 0x0072de00 ??0TGPane@@QAE@J@Z
TGPane::TGPane( long id ) : TGUIObject( id ), children( 2 ) {
	bounds.Set( 0.0f, 0.0f, 0.0f, 0.0f );
	focus = NULL;
}

/*
================
TGPane::~TGPane

Destroys the children.
================
*/
// FUNCTION: STBC 0x0072de40
TGPane::~TGPane( void ) {
	KillChildren();
}

/*
================
TGPane::GetFocus
================
*/
// FUNCTION: STBC 0x0072ded0
TGUIObject *TGPane::GetFocus( void ) {
	return focus;
}

/*
================
TGPane::SetVisible

A pane that becomes completely visible passes it on to its children.
================
*/
// FUNCTION: STBC 0x0072dee0
void TGPane::SetVisible( bool layout ) {
	bool wasCompletelyVisible = IsCompletelyVisible();
	TGUIObject::SetVisible( false );
	if ( !wasCompletelyVisible && IsCompletelyVisible() ) {
		UpdateChildVisibility( IsCompletelyVisible() );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::SetNotVisible
================
*/
// FUNCTION: STBC 0x0072df40
void TGPane::SetNotVisible( bool layout ) {
	bool wasCompletelyVisible = IsCompletelyVisible();
	TGUIObject::SetNotVisible( false );
	if ( wasCompletelyVisible == true ) {
		UpdateChildVisibility( IsCompletelyVisible() );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::UpdateChildVisibility

Sets TGUIO_COMPLETELY_VISIBLE on every visible child of a completely
visible pane, and clears it on the others, down the pane tree.
================
*/
// FUNCTION: STBC 0x0072df90
void TGPane::UpdateChildVisibility( bool visible ) {
	NiTListIterator pos = children.GetHeadPos();
	bool completelyVisible = IsCompletelyVisible();

	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( ( child->flags & TGUIO_VISIBLE ) && completelyVisible && visible ) {
			child->flags |= TGUIO_COMPLETELY_VISIBLE;
		} else {
			child->flags &= ~TGUIO_COMPLETELY_VISIBLE;
		}
		TGPane *pane = Cast( child );
		if ( pane != NULL ) {
			pane->UpdateChildVisibility( visible );
		}
	}
}

/*
================
TGPane::KillChildren

Drops the focus, then destroys every child.
================
*/
// FUNCTION: STBC 0x0072e000
void TGPane::KillChildren( void ) {
	SetFocus( NULL, true );
	while ( children.GetSize() != 0 ) {
		TGUIObject *child = children.RemoveHead();
		child->SetParent( NULL, false );
		delete child;
	}
}

/*
================
TGPane::Layout

Lays out a completely visible pane, then its visible children, back to
front.
================
*/
// FUNCTION: STBC 0x0072e060
void TGPane::Layout( void ) {
	if ( !IsCompletelyVisible() ) {
		return;
	}

	TGUIObject::Layout();
	NiTListIterator pos = children.GetTailPos();
	while ( pos ) {
		TGUIObject *child = children.GetPrev( pos );
		if ( child->flags & TGUIO_VISIBLE ) {
			child->Layout();
		}
	}
}

/*
================
TGPane::VirtualSlot63

Fills the batches of a completely visible pane that has its own batch,
then of its visible children that have theirs, back to front.
================
*/
// FUNCTION: STBC 0x0072e0a0
void TGPane::VirtualSlot63( void *argument ) {
	if ( !IsCompletelyVisible() || ( flags & TGUIO_USE_PARENT_BATCH ) ) {
		return;
	}

	TGUIObject::VirtualSlot63( argument );
	NiTListIterator pos = children.GetTailPos();
	while ( pos ) {
		TGUIObject *child = children.GetPrev( pos );
		if ( ( child->flags & TGUIO_VISIBLE ) && !( child->flags & TGUIO_USE_PARENT_BATCH ) ) {
			child->VirtualSlot63( argument );
		}
	}
}

/*
================
TGPane::FocusHeight

The depth of the chain of focused children below the pane.
================
*/
// FUNCTION: STBC 0x0072e100
int TGPane::FocusHeight( void ) {
	TGUIObject *child = GetFocus();
	if ( child != NULL ) {
		return child->FocusHeight() + 1;
	}
	return 0;
}

/*
================
TGPane::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0072e120
void TGPane::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGPane::MouseHandler, "TGPane::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGPane::KeyboardHandler, "TGPane::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGPane::GamepadHandler, "TGPane::GamepadHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGPane::ControlHandler, "TGPane::ControlHandler" );
}

/*
================
TGPane::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0072e180
void TGPane::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGPane::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGPane::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_GAMEPAD, "TGPane::GamepadHandler" );
	AddMethodHandlerForClass( &classHandlers, FIRST_TG_CONTROL_MODULE_EVENT_TYPE, "TGPane::ControlHandler" );
}

/*
================
TGPane::MouseHandler

Hands a mouse event over the pane to every visible child under the
pointer, front to back, in the child's coordinates, until one handles it;
the root pane counts the depth of the pane tree as it goes.
================
*/
// FUNCTION: STBC 0x0072e1e0
void TGPane::MouseHandler( TGMouseEvent *event ) {
	if ( !( flags & TGUIO_VISIBLE ) ) {
		return;
	}

	NiPoint2 point;
	point.x = event->x;
	point.y = event->y;
	if ( point.x < 0.0f || bounds.width < point.x || point.y < 0.0f || bounds.height < point.y ) {
		return;
	}

	g_kRootWindow.Unknown727EE0( this );
	g_kRootWindow.mouseDepth++;

	NiPoint2 panePoint = point;
	NiTListIterator pos = children.GetHeadPos();
	while ( pos && !event->EventHandled() ) {
		TGUIObject *child = children.GetNext( pos );
		NiPoint2 childPosition;
		child->GetPosition( &childPosition );
		point.x = panePoint.x - childPosition.x;
		point.y = panePoint.y - childPosition.y;
		if ( ( child->flags & TGUIO_VISIBLE ) && point.x >= 0.0f && child->bounds.width >= point.x
			&& point.y >= 0.0f && child->bounds.height >= point.y ) {
			event->x = point.x;
			event->y = point.y;
			event->SetDestination( child );
			child->ProcessEvent( event );
		}
	}

	g_kRootWindow.mouseDepth--;
	event->x = panePoint.x;
	event->y = panePoint.y;
	CallNextHandler( event );
}

/*
================
TGPane::KeyboardHandler

Passes the event to the visible child with the focus; an unhandled event
goes on to the pane's next handler.
================
*/
// FUNCTION: STBC 0x0072e350
void TGPane::KeyboardHandler( TGKeyboardEvent *event ) {
	TGUIObject *child = TGPane::GetFocus();
	if ( child != NULL && ( child->flags & TGUIO_VISIBLE ) ) {
		event->SetDestination( child );
		child->ProcessEvent( event );
	}
	if ( flags & TGUIO_ALWAYS_HANDLE_EVENTS ) {
		event->SetHandled();
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
TGPane::GamepadHandler

Passes the event to the visible child with the focus.
================
*/
// FUNCTION: STBC 0x0072e3a0
void TGPane::GamepadHandler( TGEvent *event ) {
	TGUIObject *child = GetFocus();
	if ( child != NULL && ( child->flags & TGUIO_VISIBLE ) ) {
		event->SetDestination( child );
		child->ProcessEvent( event );
	}
}

/*
================
TGPane::ControlHandler

Passes the event to the visible child with the focus.
================
*/
// FUNCTION: STBC 0x0072e3d0
void TGPane::ControlHandler( TGEvent *event ) {
	TGUIObject *child = GetFocus();
	if ( child != NULL && ( child->flags & TGUIO_VISIBLE ) ) {
		event->SetDestination( child );
		child->ProcessEvent( event );
	}
}

/*
================
TGPane::InsertChild

Inserts the child before the child at index (a negative index counts back
from the end; past the end appends), adopts it, gives it the focus when
the pane had none, and places it at (x, y).
================
*/
// FUNCTION: STBC 0x0072e400
void TGPane::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	if ( child == NULL ) {
		return;
	}

	int count = children.GetSize();
	if ( index < 0 ) {
		index += count;
	}
	// the count is never negative; the original guards it anyway
	if ( count < 0 ) {
		count = 0;
	}
	if ( index >= count ) {
		children.AddTail( child );
	} else if ( index < 0 ) {
		children.AddHead( child );
	} else {
		NiTListIterator pos = children.GetHeadPos();
		while ( --index >= 0 ) {
			pos = children.GetNextPos( pos );
		}
		children.InsertBefore( pos, child );
	}

	child->SetParent( this, false );
	if ( children.GetSize() == 1 || GetFocus() == NULL ) {
		SetFocus( child, false );
	}
	child->SetPosition( x, y, false );
	child->VirtualSlot59();
	if ( ( flags & TGUIO_BATCH_CHILD_POLYS ) || ( flags & TGUIO_USE_PARENT_BATCH ) ) {
		child->SetUseParentBatch( false );
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
TGPane::RemoveChild

Unlinks the child and orphans it; when it had the focus, the focus moves
to the first child that takes it.
================
*/
// FUNCTION: STBC 0x0072e5b0
void TGPane::RemoveChild( TGUIObject *child, bool layout ) {
	if ( child == NULL || children.GetSize() == 0 ) {
		return;
	}

	children.Remove( child );
	child->SetParent( NULL, true );
	if ( GetFocus() == child ) {
		if ( children.GetSize() == 0 ) {
			SetFocus( NULL, false );
		} else {
			SetFocus( GetFirstFocusableChild(), false );
		}
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
TGPane::DeleteChild

RemoveChild, then destroys the child.
================
*/
// FUNCTION: STBC 0x0072e6c0
void TGPane::DeleteChild( TGUIObject *child, bool layout ) {
	if ( child == NULL || children.GetSize() == 0 || child->parent != this ) {
		return;
	}

	children.Remove( child );
	child->SetParent( NULL, true );
	if ( GetFocus() == child ) {
		if ( children.GetSize() == 0 ) {
			SetFocus( NULL, false );
		} else {
			SetFocus( GetFirstFocusableChild(), false );
		}
	}
	delete child;
	if ( layout ) {
		Layout();
	}
}

/*
================
TGPane::MoveToFront

Makes the child the first (drawn last, given events first) and gives it
the focus.
================
*/
// FUNCTION: STBC 0x0072e7e0
void TGPane::MoveToFront( TGUIObject *child, bool layout ) {
	if ( child == NULL || child->parent != this || children.GetSize() == 0 ) {
		return;
	}

	if ( children.GetHead() != child ) {
		children.Remove( child );
		children.AddHead( child );
	}
	if ( GetFocus() != child ) {
		SetFocus( child, layout );
	}
}

/*
================
TGPane::MoveTowardsFront

Swaps the child with the one before it.
================
*/
// FUNCTION: STBC 0x0072e920
void TGPane::MoveTowardsFront( TGUIObject *child, bool layout ) {
	int index = FindPos( child );
	if ( index > 0 ) {
		RemoveChild( child, false );
		InsertChild( index - 1, child, child->GetLeft(), child->GetTop(), layout );
	}
}

/*
================
TGPane::MoveToBack

Makes the child the last; a child with the focus hands it to the first
child that takes it.
================
*/
// FUNCTION: STBC 0x0072e970
void TGPane::MoveToBack( TGUIObject *child, bool layout ) {
	if ( child == NULL || child->parent != this || children.GetSize() == 0 ) {
		return;
	}

	if ( children.GetTail() != child ) {
		children.Remove( child );
		children.AddTail( child );
	}
	if ( GetFocus() == child ) {
		SetFocus( GetFirstFocusableChild(), layout );
	}
}

/*
================
TGPane::MoveTowardsBack

Swaps the child with the one after it.
================
*/
// FUNCTION: STBC 0x0072eac0
void TGPane::MoveTowardsBack( TGUIObject *child, bool layout ) {
	int index = FindPos( child );
	if ( index != -1 && index != GetNumChildren() - 1 ) {
		RemoveChild( child, false );
		InsertChild( index + 1, child, child->GetLeft(), child->GetTop(), layout );
	}
}

/*
================
TGPane::VirtualSlot59
================
*/
// FUNCTION: STBC 0x0072eb20
void TGPane::VirtualSlot59( void ) {
	TGUIObject::VirtualSlot59();
	UpdateChildVisibility( IsCompletelyVisible() );
}

/*
================
TGPane::SetFocus

Moves the focus to a child of the pane (or to none): the old focus is
told it lost it, the new one that it got it, and the root pane updates the
true focus. A child that already has the focus or refuses it
(TGUIO_NO_FOCUS) changes nothing.
================
*/
// FUNCTION: STBC 0x0072eb40
void TGPane::SetFocus( TGUIObject *child, bool layout ) {
	if ( child != NULL ) {
		if ( child->parent != this || GetFocus() == child || ( child->flags & TGUIO_NO_FOCUS ) ) {
			return;
		}
	}

	TGUIObject *oldFocus = focus;
	focus = NULL;
	if ( oldFocus != NULL ) {
		oldFocus->LostFocus( layout );
	}
	focus = child;
	if ( child != NULL ) {
		child->GotFocus( layout );
	}
	g_kRootWindow.UpdateTrueFocus( layout );
}

/*
================
TGPane::MoveFocus

Moves the focus direction children forward (positive) or back (negative)
from the child that has it, or from the first or last child when none
has it. At the end of the list the walk wraps around when wrap is set and
stops otherwise.
================
*/
// FUNCTION: STBC 0x0072eba0
void TGPane::MoveFocus( int direction, bool wrap, bool layout ) {
	if ( direction == 0 || children.GetSize() == 0 ) {
		return;
	}

	TGUIObject *target = GetFocus();
	if ( target == NULL ) {
		if ( direction > 0 ) {
			direction--;
			target = children.GetHead();
		} else {
			direction++;
			target = children.GetTail();
		}
	}

	NiTListIterator pos = children.FindPos( target );
	while ( direction != 0 ) {
		if ( direction > 0 ) {
			pos = children.GetNextPos( pos );
			direction--;
			if ( pos == NULL ) {
				if ( !wrap ) {
					break;
				}
				pos = children.GetHeadPos();
			}
		} else {
			pos = children.GetPrevPos( pos );
			direction++;
			if ( pos == NULL ) {
				if ( !wrap ) {
					break;
				}
				pos = children.GetTailPos();
			}
		}
		if ( pos == NULL ) {
			break;
		}
		target = children.Get( pos );
	}
	SetFocus( target, layout );
}

/*
================
TGPane::GetFocusLeaf

The end of the chain of focused children.
================
*/
// FUNCTION: STBC 0x0072ec60
TGUIObject *TGPane::GetFocusLeaf( void ) {
	TGUIObject *child = GetFocus();
	if ( child != NULL ) {
		return child->GetFocusLeaf();
	}
	return this;
}

/*
================
TGPane::VirtualSlot60
================
*/
// FUNCTION: STBC 0x0072ec80
void TGPane::VirtualSlot60( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->VirtualSlot60();
	}
}

/*
================
TGPane::VirtualSlot61
================
*/
// FUNCTION: STBC 0x0072ecb0
void TGPane::VirtualSlot61( NiPoint2 *offset ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->VirtualSlot61( offset );
	}
}

/*
================
TGPane::VirtualSlot62

Passes the clip rectangle on to the children in the pane's coordinates.
================
*/
// FUNCTION: STBC 0x0072ece0
void TGPane::VirtualSlot62( TGRect *clipRect ) {
	NiPoint2 position;
	GetPosition( &position );
	*clipRect -= position;

	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->VirtualSlot62( clipRect );
	}

	*clipRect += position;
}

/*
================
TGPane::GetNumChildren
================
*/
// FUNCTION: STBC 0x0072ed50
int TGPane::GetNumChildren( void ) {
	return children.GetSize();
}

/*
================
TGPane::GetFirstChild
================
*/
// FUNCTION: STBC 0x0072ed60
TGUIObject *TGPane::GetFirstChild( void ) {
	if ( children.GetSize() == 0 ) {
		return NULL;
	}
	return children.GetHead();
}

/*
================
TGPane::GetLastChild
================
*/
// FUNCTION: STBC 0x0072ed70
TGUIObject *TGPane::GetLastChild( void ) {
	if ( children.GetSize() == 0 ) {
		return NULL;
	}
	return children.GetTail();
}

/*
================
TGPane::GetNthChild

A negative index counts back from the last child.
================
*/
// FUNCTION: STBC 0x0072ed80
TGUIObject *TGPane::GetNthChild( int index ) {
	int count = children.GetSize();
	if ( count == 0 || index >= count || index <= -count ) {
		return NULL;
	}
	if ( index < 0 ) {
		index += count;
	}

	NiTListIterator pos = children.GetHeadPos();
	while ( index != 0 ) {
		pos = children.GetNextPos( pos );
		index--;
	}
	return children.GetNext( pos );
}

/*
================
TGPane::GetFirstFocusableChild

The first child that does not refuse the focus.
================
*/
// FUNCTION: STBC 0x0072edd0
TGUIObject *TGPane::GetFirstFocusableChild( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( !( child->flags & TGUIO_NO_FOCUS ) ) {
			return child;
		}
	}
	return NULL;
}

/*
================
TGPane::FindPos

The index of the child, -1 when it is not one.
================
*/
// FUNCTION: STBC 0x0072ee00
int TGPane::FindPos( TGUIObject *child ) {
	int index = 0;
	TGUIObject *element = NULL;
	NiTListIterator pos = children.GetHeadPos();

	while ( pos != NULL && ( element = children.GetNext( pos ) ) != child ) {
		index++;
	}
	if ( element == child ) {
		return index;
	}
	return -1;
}

/*
================
TGPane::GetNextChild
================
*/
// FUNCTION: STBC 0x0072ee30
TGUIObject *TGPane::GetNextChild( TGUIObject *child ) {
	NiTListItem<TGUIObject *> *item = (NiTListItem<TGUIObject *> *)children.FindPos( child );
	if ( item != NULL && item->next != NULL ) {
		return item->next->element;
	}
	return NULL;
}

/*
================
TGPane::GetPrevChild
================
*/
// FUNCTION: STBC 0x0072ee70
TGUIObject *TGPane::GetPrevChild( TGUIObject *child ) {
	NiTListItem<TGUIObject *> *item = (NiTListItem<TGUIObject *> *)children.FindPos( child );
	if ( item != NULL && item->prev != NULL ) {
		return item->prev->element;
	}
	return NULL;
}

/*
================
TGPane::SetBatchChildPolys

The children draw into the pane's batch.
================
*/
// FUNCTION: STBC 0x0072eeb0
void TGPane::SetBatchChildPolys( bool layout ) {
	TGUIObject::SetBatchChildPolys( false );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->SetUseParentBatch( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::SetNotBatchChildPolys
================
*/
// FUNCTION: STBC 0x0072ef00
void TGPane::SetNotBatchChildPolys( bool layout ) {
	TGUIObject::SetNotBatchChildPolys( false );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->SetNotUseParentBatch( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::SetUseParentBatch

A pane that draws into its parent's batch takes its children along.
================
*/
// FUNCTION: STBC 0x0072ef50
void TGPane::SetUseParentBatch( bool layout ) {
	TGUIObject::SetUseParentBatch( false );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->SetUseParentBatch( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::SetNotUseParentBatch
================
*/
// FUNCTION: STBC 0x0072efa0
void TGPane::SetNotUseParentBatch( bool layout ) {
	TGUIObject::SetNotUseParentBatch( false );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		children.GetNext( pos )->SetNotUseParentBatch( false );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGPane::Print

The pane, then its children one level deeper, while depth lasts.
================
*/
// FUNCTION: STBC 0x0072eff0
void TGPane::Print( int indent, int depth ) {
	char address[12];

	TGUIObject::Print( indent, depth );
	GetIndentString( indent );
	GetFocus()->FormatAddress( address );
	if ( depth != 0 ) {
		NiTListIterator pos = children.GetHeadPos();
		while ( pos ) {
			TGUIObject *child = children.GetNext( pos );
			if ( child != NULL ) {
				child->Print( indent + 1, depth - 1 );
			}
		}
	}
}

/*
================
TGPane::WriteToStream

The focus's ID, the number of children, and each child behind a flag.
================
*/
// FUNCTION: STBC 0x0072f060
bool TGPane::WriteToStream( TGStream *stream ) {
	if ( !TGUIObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteID( focus != NULL ? focus->GetObjID() : 0 );
	stream->WriteLong( children.GetSize() );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child != NULL ) {
			stream->WriteBool( true );
			child->WriteToStream( stream );
		} else {
			stream->WriteBool( false );
		}
	}
	return true;
}

/*
================
TGPane::ReadFromStream

Reads what WriteToStream wrote: the child with the focus's ID becomes the
focus again, and a missing child stays a NULL entry.
================
*/
// FUNCTION: STBC 0x0072f0e0
bool TGPane::ReadFromStream( TGStream *stream ) {
	if ( !TGUIObject::ReadFromStream( stream ) ) {
		return false;
	}

	focus = NULL;
	long focusID = stream->ReadID();
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		if ( stream->ReadBool() == true ) {
			TGUIObject *child = (TGUIObject *)LoadObject( stream, 0 );
			if ( child->GetObjID() == focusID ) {
				focus = child;
			}
			children.AddTail( child );
		} else {
			children.AddTail( NULL );
		}
	}
	return true;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0072dc40 _$E423
// FUNCTION: STBC 0x0072dc50 _$E419
// FUNCTION: STBC 0x0072dc60 _$E422
// FUNCTION: STBC 0x0072dc70 _$E420
// DATA: STBC 0x0099d4a8
TGEventHandlerTable TGPane::classHandlers;

// The standard library's locale facet id (<iostream>): a test-and-set initialiser and its exit-time destructor.
// FUNCTION: STBC 0x0072f220 _$E431
// FUNCTION: STBC 0x0072f240 _$E430
