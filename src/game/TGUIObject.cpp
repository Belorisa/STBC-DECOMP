/*
===============================================================================

	TGUIObject.cpp

===============================================================================
*/

#include "TGUIObject.h"
#include "TGPane.h"
#include "TGRootPane.h"
#include "TGUIBatch.h"
#include "TGUIModule.h"
#include "renderer/NiRender.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "TGTrace.h"
#include "NiMain.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00731190 _$E434
// FUNCTION: STBC 0x007311b0 _$E433

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0072f290 _$E414
// FUNCTION: STBC 0x0072f2a0 _$E411
// FUNCTION: STBC 0x0072f2b0 _$E413
// FUNCTION: STBC 0x0072f2c0 _$E412
// FUNCTION: STBC 0x0072f2d0 _$E420
// FUNCTION: STBC 0x0072f2e0 _$E417
// FUNCTION: STBC 0x0072f2f0 _$E419
// FUNCTION: STBC 0x0072f300 _$E418

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0072f310 _$E33
// FUNCTION: STBC 0x0072f320 _$E32
// FUNCTION: STBC 0x0072f330 _$E37
// FUNCTION: STBC 0x0072f340 _$E36
// FUNCTION: STBC 0x0072f350 _$E48
// FUNCTION: STBC 0x0072f360 _$E47
// FUNCTION: STBC 0x0072f370 _$E52
// FUNCTION: STBC 0x0072f380 _$E51
// FUNCTION: STBC 0x0072f390 _$E56
// FUNCTION: STBC 0x0072f3a0 _$E55
// FUNCTION: STBC 0x0072f3b0 _$E60
// FUNCTION: STBC 0x0072f3c0 _$E59
// FUNCTION: STBC 0x0072f3d0 _$E184
// FUNCTION: STBC 0x0072f3e0 _$E183
// FUNCTION: STBC 0x0072f3f0 _$E126
// FUNCTION: STBC 0x0072f400 _$E125
// FUNCTION: STBC 0x0072f410 _$E139
// FUNCTION: STBC 0x0072f420 _$E138
// FUNCTION: STBC 0x0072f450 _$E194
// FUNCTION: STBC 0x0072f460 _$E193
// FUNCTION: STBC 0x0072f490 _$E202
// FUNCTION: STBC 0x0072f4a0 _$E201
// FUNCTION: STBC 0x0072f4b0 _$E209
// FUNCTION: STBC 0x0072f4c0 _$E208
// FUNCTION: STBC 0x0072f510 _$E84
// FUNCTION: STBC 0x0072f520 _$E83
// FUNCTION: STBC 0x0072f530 _$E217
// FUNCTION: STBC 0x0072f540 _$E216
// FUNCTION: STBC 0x0072f550 _$E224
// FUNCTION: STBC 0x0072f560 _$E223
// FUNCTION: STBC 0x0072f570 _$E228
// FUNCTION: STBC 0x0072f580 _$E227
// FUNCTION: STBC 0x0072f590 _$E232
// FUNCTION: STBC 0x0072f5a0 _$E231
// FUNCTION: STBC 0x0072f5b0 _$E236
// FUNCTION: STBC 0x0072f5c0 _$E235
// FUNCTION: STBC 0x0072f5d0 _$E242
// FUNCTION: STBC 0x0072f5e0 _$E241
// FUNCTION: STBC 0x0072f730 _$E89
// FUNCTION: STBC 0x0072f740 _$E88
// FUNCTION: STBC 0x0072f770 _$E94
// FUNCTION: STBC 0x0072f780 _$E93
// FUNCTION: STBC 0x0072f810 _$E103
// FUNCTION: STBC 0x0072f820 _$E102
// FUNCTION: STBC 0x0072f890 _$E109
// FUNCTION: STBC 0x0072f8a0 _$E108
// FUNCTION: STBC 0x0072f8b0 _$E114
// FUNCTION: STBC 0x0072f8c0 _$E113
// FUNCTION: STBC 0x0072f8d0 _$E144
// FUNCTION: STBC 0x0072f8e0 _$E143
// FUNCTION: STBC 0x0072f910 _$E149
// FUNCTION: STBC 0x0072f920 _$E148
// FUNCTION: STBC 0x0072f970 _$E154
// FUNCTION: STBC 0x0072f980 _$E153
// FUNCTION: STBC 0x0072f9b0 _$E119
// FUNCTION: STBC 0x0072f9c0 _$E118
// FUNCTION: STBC 0x0072fa10 _$E159
// FUNCTION: STBC 0x0072fa20 _$E158
// FUNCTION: STBC 0x0072fa30 _$E164
// FUNCTION: STBC 0x0072fa40 _$E163
// FUNCTION: STBC 0x0072fb50 _$E169
// FUNCTION: STBC 0x0072fb60 _$E168
// FUNCTION: STBC 0x0072fb90 _$E174
// FUNCTION: STBC 0x0072fba0 _$E173
// FUNCTION: STBC 0x0072fbb0 _$E179
// FUNCTION: STBC 0x0072fbc0 _$E178
// FUNCTION: STBC 0x0072f430 _$E189
// FUNCTION: STBC 0x0072f440 _$E188
// FUNCTION: STBC 0x0072f470 _$E198
// FUNCTION: STBC 0x0072f480 _$E197
// FUNCTION: STBC 0x0072f4d0 _$E273
// FUNCTION: STBC 0x0072f4e0 _$E272
// FUNCTION: STBC 0x0072f4f0 _$E277
// FUNCTION: STBC 0x0072f500 _$E276
// FUNCTION: STBC 0x0072f5f0 _$E281
// FUNCTION: STBC 0x0072f600 _$E280
// FUNCTION: STBC 0x0072f610 _$E285
// FUNCTION: STBC 0x0072f620 _$E284
// FUNCTION: STBC 0x0072f630 _$E289
// FUNCTION: STBC 0x0072f640 _$E288
// FUNCTION: STBC 0x0072f650 _$E293
// FUNCTION: STBC 0x0072f660 _$E292
// FUNCTION: STBC 0x0072f670 _$E297
// FUNCTION: STBC 0x0072f680 _$E296
// FUNCTION: STBC 0x0072f690 _$E301
// FUNCTION: STBC 0x0072f6a0 _$E300
// FUNCTION: STBC 0x0072f6b0 _$E305
// FUNCTION: STBC 0x0072f6c0 _$E304
// FUNCTION: STBC 0x0072f6d0 _$E309
// FUNCTION: STBC 0x0072f6e0 _$E308
// FUNCTION: STBC 0x0072f6f0 _$E313
// FUNCTION: STBC 0x0072f700 _$E312
// FUNCTION: STBC 0x0072f710 _$E317
// FUNCTION: STBC 0x0072f720 _$E316
// FUNCTION: STBC 0x0072f750 _$E321
// FUNCTION: STBC 0x0072f760 _$E320
// FUNCTION: STBC 0x0072f790 _$E325
// FUNCTION: STBC 0x0072f7a0 _$E324
// FUNCTION: STBC 0x0072f7b0 _$E329
// FUNCTION: STBC 0x0072f7c0 _$E328
// FUNCTION: STBC 0x0072f7d0 _$E333
// FUNCTION: STBC 0x0072f7e0 _$E332
// FUNCTION: STBC 0x0072f7f0 _$E337
// FUNCTION: STBC 0x0072f800 _$E336
// FUNCTION: STBC 0x0072f830 _$E341
// FUNCTION: STBC 0x0072f840 _$E340
// FUNCTION: STBC 0x0072f850 _$E345
// FUNCTION: STBC 0x0072f860 _$E344
// FUNCTION: STBC 0x0072f870 _$E349
// FUNCTION: STBC 0x0072f880 _$E348
// FUNCTION: STBC 0x0072f8f0 _$E353
// FUNCTION: STBC 0x0072f900 _$E352
// FUNCTION: STBC 0x0072f930 _$E357
// FUNCTION: STBC 0x0072f940 _$E356
// FUNCTION: STBC 0x0072f950 _$E361
// FUNCTION: STBC 0x0072f960 _$E360
// FUNCTION: STBC 0x0072f990 _$E365
// FUNCTION: STBC 0x0072f9a0 _$E364
// FUNCTION: STBC 0x0072f9d0 _$E369
// FUNCTION: STBC 0x0072f9e0 _$E368
// FUNCTION: STBC 0x0072f9f0 _$E373
// FUNCTION: STBC 0x0072fa00 _$E372
// FUNCTION: STBC 0x0072fa50 _$E377
// FUNCTION: STBC 0x0072fa60 _$E376
// FUNCTION: STBC 0x0072fa70 _$E381
// FUNCTION: STBC 0x0072fa80 _$E380
// FUNCTION: STBC 0x0072fa90 _$E385
// FUNCTION: STBC 0x0072faa0 _$E384
// FUNCTION: STBC 0x0072fab0 _$E389
// FUNCTION: STBC 0x0072fac0 _$E388
// FUNCTION: STBC 0x0072fad0 _$E393
// FUNCTION: STBC 0x0072fae0 _$E392
// FUNCTION: STBC 0x0072faf0 _$E397
// FUNCTION: STBC 0x0072fb00 _$E396
// FUNCTION: STBC 0x0072fb10 _$E401
// FUNCTION: STBC 0x0072fb20 _$E400
// FUNCTION: STBC 0x0072fb30 _$E405
// FUNCTION: STBC 0x0072fb40 _$E404
// FUNCTION: STBC 0x0072fb70 _$E409
// FUNCTION: STBC 0x0072fb80 _$E408

/*
================
TGUIObject::TGUIObject
================
*/
// FUNCTION: STBC 0x0072fc20
TGUIObject::TGUIObject( long id ) : TGEventHandlerObject( id ) {
	bounds.Clear();
	parent = NULL;
	flags = TGUIO_VISIBLE;
	batches = NULL;
}

/*
================
TGUIObject::GetObjType
================
*/
// FUNCTION: STBC 0x0072fca0
int TGUIObject::GetObjType( void ) {
	return TGUIOBJECT_TYPE;
}

/*
================
TGUIObject::GetClassName
================
*/
// FUNCTION: STBC 0x0072fcf0
char *TGUIObject::GetClassName( void ) {
	return "TGUIObject";
}

/*
================
TGUIObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0072fd00
char *TGUIObject::GetSwigTypeName( void ) {
	return "_p_TGUIObject";
}

/*
================
TGUIObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0072fd10
char *TGUIObject::GetPythonClassName( void ) {
	return "TGUIObjectPtr";
}

/*
================
TGUIObject::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0072fd20
TGEventHandlerTable *TGUIObject::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TGUIObject::VirtualSlot62
================
*/
// FUNCTION: STBC 0x0072fd30
void TGUIObject::VirtualSlot62( TGRect *clipRect ) {
}

/*
================
TGUIObject::~TGUIObject

Leaves the parent, and makes sure the root pane holds no pointer to the
object.
================
*/
// FUNCTION: STBC 0x0072fd70
TGUIObject::~TGUIObject( void ) {
	if ( parent != NULL && parent != UI_NO_PARENT ) {
		parent->RemoveChild( this, false );
	}
	g_kRootWindow.ReleaseMouse( this );
	g_kRootWindow.Unknown727FA0( this );
	DeleteBatches();
}

/*
================
TGUIObject::DeleteBatches
================
*/
// FUNCTION: STBC 0x0072fe00
void TGUIObject::DeleteBatches( void ) {
	TGUIBatch *batch = batches;
	while ( batch != NULL ) {
		TGUIBatch *current = batch;
		batch = batch->next;
		delete current;
	}
	batches = NULL;
}

/*
================
TGUIObject::GetConceptualParent

The first parent that does not ask to be skipped (TGUIO_SKIP_PARENT); the
last one that does when every parent up to the root does.
================
*/
// FUNCTION: STBC 0x0072fe40
TGPane *TGUIObject::GetConceptualParent( void ) {
	TGPane *pane = parent;
	TGPane *last = parent;

	while ( pane != NULL && pane != UI_NO_PARENT && ( pane->flags & TGUIO_SKIP_PARENT ) ) {
		last = pane;
		pane = pane->parent;
	}
	if ( pane != NULL && pane != UI_NO_PARENT ) {
		return pane;
	}
	if ( last == UI_NO_PARENT ) {
		return NULL;
	}
	return last;
}

/*
================
TGUIObject::SetParent

A child of a pane that draws into its parent's batch does the same.
================
*/
// FUNCTION: STBC 0x0072fe80
void TGUIObject::SetParent( TGPane *parent, bool layout ) {
	this->parent = parent;
	if ( parent == NULL ) {
		VirtualSlot60();
	} else if ( parent != UI_NO_PARENT && ( parent->flags & TGUIO_USE_PARENT_BATCH ) ) {
		SetUseParentBatch( false );
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::SetUseParentBatch
================
*/
// FUNCTION: STBC 0x0072fed0
void TGUIObject::SetUseParentBatch( bool layout ) {
	VirtualSlot60();
	DeleteBatches();
	flags &= ~TGUIO_BATCH_CHILD_POLYS;
	flags |= TGUIO_USE_PARENT_BATCH;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGUIObject::SetBatchChildPolys
================
*/
// FUNCTION: STBC 0x0072ff10
void TGUIObject::SetBatchChildPolys( bool layout ) {
	flags &= ~TGUIO_USE_PARENT_BATCH;
	flags |= TGUIO_BATCH_CHILD_POLYS;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGUIObject::SetBounds
================
*/
// FUNCTION: STBC 0x0072ff30
void TGUIObject::SetBounds( TGRect *rect, bool layout ) {
	SetPosition( rect->GetLeft(), rect->GetTop(), false );
	Resize( rect->GetWidth(), rect->GetHeight(), false );
	if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::GetScreenOffset

The object's position on the screen: its own and every parent's.
================
*/
// FUNCTION: STBC 0x0072ff80
void TGUIObject::GetScreenOffset( NiPoint2 *offset ) {
	offset->x = bounds.left;
	offset->y = bounds.top;
	for ( TGPane *pane = parent; pane != UI_NO_PARENT && pane != NULL; pane = pane->parent ) {
		offset->x += pane->bounds.left;
		offset->y += pane->bounds.top;
	}
}

/*
================
TGUIObject::GetClipRect

The part of the object that its parents leave visible, relative to its
parent; empty when the object is empty, has no parent or is hidden.
================
*/
// FUNCTION: STBC 0x0072ffc0
void TGUIObject::GetClipRect( TGRect *rect ) {
	*rect = bounds;
	if ( rect->width == 0.0f || rect->height == 0.0f || parent == NULL || !IsCompletelyVisible() ) {
		rect->Clear();
		return;
	}
	if ( parent == UI_NO_PARENT ) {
		return;
	}

	TGRect parentClip;
	NiPoint2 parentPosition;
	parent->GetPosition( &parentPosition );
	parent->GetClipRect( &parentClip );
	parentClip -= parentPosition;
	rect->ClipTo( &parentClip );
}

/*
================
TGUIObject::Move

Moves the object by whole pixels. An object that was within its parent's
clip rectangle tells its batch it moved (VirtualSlot61) and, if it left the
rectangle, its new clip rectangle (VirtualSlot62), instead of laying out.
================
*/
// FUNCTION: STBC 0x007300e0
// PARKED: complete; schedule: retail loads the parent before the visibility test's branch, so its flags byte is read with mov/test instead of test byte [mem] (the condition's spellings, a parent local and the nested-if shapes tried)
void TGUIObject::Move( float dx, float dy, bool layout ) {
	TGRect parentClip;

	float alignedDx = TGUIModule::PixelAlignValue( dx );
	float alignedDy = TGUIModule::PixelAlignValue( dy, false );

	if ( !IsVisible() || parent == NULL ) {
		bounds.left += alignedDx;
		bounds.top += alignedDy;
		return;
	}

	parent->GetClipRect( &parentClip );
	int before = parentClip.Unknown739F00( &bounds );
	bounds.left += alignedDx;
	bounds.top += alignedDy;
	if ( before == RECT_INSIDE ) {
		int after = parentClip.Unknown739F00( &bounds );
		NiPoint2 offset( alignedDx, alignedDy );
		VirtualSlot61( &offset );
		if ( after != RECT_INSIDE ) {
			TGRect clipRect;
			GetClipRect( &clipRect );
			parent->GetPosition( &offset );
			clipRect -= offset;
			clipRect.ClipTo( &parentClip );
			VirtualSlot62( &clipRect );
		}
	} else if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::SetPosition

Like Move, to a position.
================
*/
// FUNCTION: STBC 0x007302f0
// PARKED: complete; as Move: schedule: retail loads the parent before the visibility test's branch (mov/test of the flags byte instead of test byte [mem])
void TGUIObject::SetPosition( float x, float y, bool layout ) {
	TGRect parentClip;

	float alignedX = TGUIModule::PixelAlignValue( x );
	float alignedY = TGUIModule::PixelAlignValue( y, false );

	if ( !IsVisible() || parent == NULL ) {
		bounds.left = alignedX;
		bounds.top = alignedY;
		return;
	}

	parent->GetClipRect( &parentClip );
	int before = parentClip.Unknown739F00( &bounds );
	float dx = alignedX - bounds.left;
	float dy = alignedY - bounds.top;
	bounds.left = alignedX;
	bounds.top = alignedY;
	if ( before == RECT_INSIDE ) {
		int after = parentClip.Unknown739F00( &bounds );
		NiPoint2 offset( dx, dy );
		VirtualSlot61( &offset );
		if ( after != RECT_INSIDE ) {
			TGRect clipRect;
			GetClipRect( &clipRect );
			parent->GetPosition( &offset );
			clipRect -= offset;
			clipRect.ClipTo( &parentClip );
			VirtualSlot62( &clipRect );
		}
	} else if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::Resize

Sets the size, in whole pixels.
================
*/
// FUNCTION: STBC 0x00730510
void TGUIObject::Resize( float width, float height, bool layout ) {
	width = TGUIModule::PixelAlignValue( width );
	height = TGUIModule::PixelAlignValue( height, false );
	bounds.width = width;
	bounds.height = height;
	if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::AlignTo

Moves the object so that its corner `to` lies on the other object's corner
`from`, both in whole screen pixels. With no other object, the parent's
corner `from` is used and `to` is ignored.
================
*/
// FUNCTION: STBC 0x007305d0
// DRAFT: calls, rects and the two corner switches agree; retail keeps 1 in EBX (EH state and the ALIGN_UR test), has an 8-byte larger frame and lays the `to` switch's cases in source order
void TGUIObject::AlignTo( TGUIObject *object, alignment_t from, alignment_t to, bool layout ) {
	TGRect objectRect;
	TGRect target;
	TGRect rect;

	if ( object == NULL ) {
		if ( parent != NULL ) {
			float x = 0.0f;
			float y = 0.0f;
			if ( from == ALIGN_UR || from == ALIGN_BR ) {
				x = parent->bounds.width - bounds.width;
			}
			if ( from == ALIGN_BL || from == ALIGN_BR ) {
				y = parent->bounds.height - bounds.height;
			}
			SetPosition( x, y, layout );
		}
		return;
	}

	NiPoint2 objectOffset;
	NiPoint2 offset;
	object->GetScreenOffset( &objectOffset );
	GetScreenOffset( &offset );
	rect = bounds;
	rect.left = offset.x;
	rect.top = offset.y;
	rect = TGUIModule::PixelAlignRect( rect );
	float x = rect.left;
	float y = rect.top;
	objectRect.Set( objectOffset.x, objectOffset.y, object->bounds.width, object->bounds.height );
	target = TGUIModule::PixelAlignRect( objectRect );

	float targetX = target.left;
	float targetY = target.top;
	switch ( from ) {
		case ALIGN_UR:
			targetX += target.width;
			break;
		case ALIGN_BL:
			targetY += target.height;
			break;
		case ALIGN_BR:
			targetX += target.width;
			targetY += target.height;
			break;
	}
	switch ( to ) {
		case ALIGN_UR:
			x += rect.width;
			break;
		case ALIGN_BL:
			y += rect.height;
			break;
		case ALIGN_BR:
			x += rect.width;
			y += rect.height;
			break;
	}
	float dx = targetX - x;
	float dy = targetY - y;
	Move( dx, dy, layout );
}

/*
================
TGUIObject::SetEnabled
================
*/
// FUNCTION: STBC 0x00730870
void TGUIObject::SetEnabled( bool layout ) {
	flags |= TGUIO_ENABLED;
}

/*
================
TGUIObject::SetDisabled
================
*/
// FUNCTION: STBC 0x00730880
void TGUIObject::SetDisabled( bool layout ) {
	flags &= ~TGUIO_ENABLED;
}

/*
================
TGUIObject::SetSelected
================
*/
// FUNCTION: STBC 0x00730890
void TGUIObject::SetSelected( bool layout ) {
	flags |= TGUIO_SELECTED;
}

/*
================
TGUIObject::SetNotSelected
================
*/
// FUNCTION: STBC 0x007308a0
void TGUIObject::SetNotSelected( bool layout ) {
	flags &= ~TGUIO_SELECTED;
}

/*
================
TGUIObject::SetHighlighted
================
*/
// FUNCTION: STBC 0x007308b0
void TGUIObject::SetHighlighted( bool layout ) {
	flags |= TGUIO_HIGHLIGHTED;
}

/*
================
TGUIObject::SetNotHighlighted
================
*/
// FUNCTION: STBC 0x007308c0
void TGUIObject::SetNotHighlighted( bool layout ) {
	flags &= ~TGUIO_HIGHLIGHTED;
}

/*
================
TGUIObject::SetExclusive
================
*/
// FUNCTION: STBC 0x007308d0
void TGUIObject::SetExclusive( bool layout ) {
	flags |= TGUIO_EXCLUSIVE;
}

/*
================
TGUIObject::SetNotExclusive
================
*/
// FUNCTION: STBC 0x007308e0
void TGUIObject::SetNotExclusive( bool layout ) {
	flags &= ~TGUIO_EXCLUSIVE;
}

/*
================
TGUIObject::IsCompletelyEnabled

True when the object and every parent are enabled.
================
*/
// FUNCTION: STBC 0x007308f0
bool TGUIObject::IsCompletelyEnabled( void ) {
	for ( TGUIObject *object = this; object != UI_NO_PARENT && object != NULL; object = object->parent ) {
		if ( !( object->flags & TGUIO_ENABLED ) ) {
			return false;
		}
	}
	return true;
}

/*
================
TGUIObject::IsCompletelyVisible
================
*/
// FUNCTION: STBC 0x00730910
bool TGUIObject::IsCompletelyVisible( void ) {
	return ( flags & TGUIO_COMPLETELY_VISIBLE ) ? true : false;
}

/*
================
TGUIObject::SetVisible
================
*/
// FUNCTION: STBC 0x00730920
void TGUIObject::SetVisible( bool layout ) {
	if ( flags & TGUIO_VISIBLE ) {
		return;
	}

	flags |= TGUIO_VISIBLE;
	if ( parent != NULL ) {
		if ( parent == UI_NO_PARENT ) {
			flags |= TGUIO_COMPLETELY_VISIBLE;
		} else if ( parent->IsCompletelyVisible() == true ) {
			flags |= TGUIO_COMPLETELY_VISIBLE;
		}
	}
	if ( layout ) {
		Layout();
	}
}

/*
================
TGUIObject::SetNotVisible

A hidden object gives up its parent's focus.
================
*/
// FUNCTION: STBC 0x00730970
void TGUIObject::SetNotVisible( bool layout ) {
	if ( flags & TGUIO_VISIBLE ) {
		flags &= ~( TGUIO_VISIBLE | TGUIO_COMPLETELY_VISIBLE );
		VirtualSlot60();
		TGPane *pane = parent;
		if ( pane != NULL && pane->GetFocus() == this ) {
			pane->SetFocus( NULL, layout );
		}
	}
}

/*
================
TGUIObject::VirtualSlot60
================
*/
// FUNCTION: STBC 0x007309c0
void TGUIObject::VirtualSlot60( void ) {
}

/*
================
TGUIObject::VirtualSlot61
================
*/
// FUNCTION: STBC 0x007309d0
void TGUIObject::VirtualSlot61( NiPoint2 *offset ) {
}

/*
================
TGUIObject::Layout
================
*/
// FUNCTION: STBC 0x007309e0
void TGUIObject::Layout( void ) {
}

/*
================
TGUIObject::GotFocus
================
*/
// FUNCTION: STBC 0x007309f0
void TGUIObject::GotFocus( bool layout ) {
}

/*
================
TGUIObject::GotTrueFocus
================
*/
// FUNCTION: STBC 0x00730a00
void TGUIObject::GotTrueFocus( bool layout ) {
}

/*
================
TGUIObject::LostFocus
================
*/
// FUNCTION: STBC 0x00730a10
void TGUIObject::LostFocus( bool layout ) {
}

/*
================
TGUIObject::LostTrueFocus
================
*/
// FUNCTION: STBC 0x00730a20
void TGUIObject::LostTrueFocus( bool layout ) {
}

/*
================
TGUIObject::GotTrueFocusPath

An object on the path to the true focus is highlighted.
================
*/
// FUNCTION: STBC 0x00730a30
void TGUIObject::GotTrueFocusPath( bool layout ) {
	SetHighlighted( layout );
}

/*
================
TGUIObject::LostTrueFocusPath
================
*/
// FUNCTION: STBC 0x00730a40
void TGUIObject::LostTrueFocusPath( bool layout ) {
	SetNotHighlighted( layout );
}

/*
================
TGUIObject::VirtualSlot59

TGUIO_COMPLETELY_VISIBLE: the object is visible, and its parent is the root
or completely visible itself.
================
*/
// FUNCTION: STBC 0x00730a50
void TGUIObject::VirtualSlot59( void ) {
	if ( parent != NULL && ( flags & TGUIO_VISIBLE ) && ( parent == UI_NO_PARENT || parent->IsCompletelyVisible() == true ) ) {
		flags |= TGUIO_COMPLETELY_VISIBLE;
	} else {
		flags &= ~TGUIO_COMPLETELY_VISIBLE;
	}
}

/*
================
TGUIObject::VirtualSlot63

Draws the object's batches that hold any polygons with the renderer.
================
*/
// FUNCTION: STBC 0x00730a90
// PARKED: register wall: retail keeps the batch in ebx, the shape in esi and the renderer in ebp, fetches the renderer's vtable pointer and the shape's data before GetWorldBound is called and loads the renderer after the first batch test; this build keeps the batch in ebp and the renderer in a later register
void TGUIObject::VirtualSlot63( void *argument ) {
	NiRender *renderer = (NiRender *)argument;
	TGUIBatch *batch = batches;
	while ( batch != NULL ) {
		if ( batch->unknown10 != 0 ) {
			renderer->SetPropertyState( batch->shape->GetPropertyState() );
			renderer->VirtualSlot45( batch->shape->GetShapeData(), batch->shape->GetWorldBound() );
		}
		batch = batch->next;
	}
}

/*
================
TGUIObject::GetBatches

The batches the object draws into: its own, or those of the first parent
that does not use its parent's; NULL when no parent has batches.
================
*/
// FUNCTION: STBC 0x00730b80
TGUIBatch **TGUIObject::GetBatches( void ) {
	TGUIObject *object = this;
	while ( object->flags & TGUIO_USE_PARENT_BATCH ) {
		object = object->parent;
		if ( object == NULL ) {
			return NULL;
		}
	}
	return &object->batches;
}

/*
================
TGUIObject::MouseEntered
================
*/
// FUNCTION: STBC 0x00730ba0
void TGUIObject::MouseEntered( TGUIObject *from ) {
	SetHighlighted( true );
}

/*
================
TGUIObject::MouseExited

An object on the path to the true focus stays highlighted.
================
*/
// FUNCTION: STBC 0x00730bb0
void TGUIObject::MouseExited( TGUIObject *to ) {
	if ( parent != NULL && parent != UI_NO_PARENT ) {
		if ( !IsInTrueFocusPath() ) {
			SetNotHighlighted( true );
		}
	} else {
		SetNotHighlighted( true );
	}
}

/*
================
TGUIObject::GetFocusLeaf
================
*/
// FUNCTION: STBC 0x00730bf0
TGUIObject *TGUIObject::GetFocusLeaf( void ) {
	return this;
}

/*
================
TGUIObject::GetCommonAncestor

The nearest object that is the object or one of its parents, and also the
other object or one of its parents; NULL when they share none.
================
*/
// FUNCTION: STBC 0x00730c00
// DRAFT: code order and call set match (the object's own membership is tested before its parent chain, as retail's three exits show); retail holds `this` in edi, the zero in ebx and the list tail in ebp, ours ebx, ebp and edi
TGUIObject *TGUIObject::GetCommonAncestor( TGUIObject *object ) {
	NiTList<TGUIObject *> ancestors( 2 );

	ancestors.AddTail( object );
	for ( TGPane *pane = object->parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->parent ) {
		ancestors.AddTail( pane );
	}

	if ( ancestors.FindPos( this ) != NULL ) {
		return this;
	}
	for ( TGPane *current = parent; current != NULL && current != UI_NO_PARENT; current = current->parent ) {
		if ( ancestors.FindPos( current ) != NULL ) {
			return current;
		}
	}
	return NULL;
}

/*
================
TGUIObject::HasFocus

True when the object is its parent's focus.
================
*/
// FUNCTION: STBC 0x00730dd0
bool TGUIObject::HasFocus( void ) {
	if ( parent != NULL && parent->GetFocus() == this ) {
		return true;
	}
	return false;
}

/*
================
TGUIObject::HasTrueFocus
================
*/
// FUNCTION: STBC 0x00730df0
bool TGUIObject::HasTrueFocus( void ) {
	return g_kRootWindow.GetTrueFocus() == this ? true : false;
}

/*
================
TGUIObject::HasAncestor
================
*/
// FUNCTION: STBC 0x00730e00
bool TGUIObject::HasAncestor( TGUIObject *object ) {
	for ( TGPane *pane = parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->parent ) {
		if ( pane == object ) {
			return true;
		}
	}
	return false;
}

/*
================
TGUIObject::IsInTrueFocusPath

True when the object has the true focus or is one of its parents.
================
*/
// FUNCTION: STBC 0x00730e30
bool TGUIObject::IsInTrueFocusPath( void ) {
	TGUIObject *object = g_kRootWindow.GetTrueFocus();
	if ( object == NULL ) {
		return false;
	}
	if ( object == this ) {
		return true;
	}
	for ( TGPane *pane = object->parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->parent ) {
		if ( pane == this ) {
			return true;
		}
	}
	return false;
}

/*
================
TGUIObject::FocusHeight
================
*/
// FUNCTION: STBC 0x00730e60
int TGUIObject::FocusHeight( void ) {
	return 0;
}

/*
================
TGUIObject::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00730e70
void TGUIObject::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGUIObject::MouseHandler, "TGUIObject::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGUIObject::KeyboardHandler, "TGUIObject::KeyboardHandler" );
}

/*
================
TGUIObject::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00730ea0
void TGUIObject::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGUIObject::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGUIObject::KeyboardHandler" );
}

/*
================
TGUIObject::MouseHandler

Tells the root pane the mouse is over the object, and takes the parent's
focus on a button press.
================
*/
// FUNCTION: STBC 0x00730ed0
void TGUIObject::MouseHandler( TGMouseEvent *event ) {
	NiPoint2 point;
	point.x = event->x;
	point.y = event->y;

	if ( point.x >= 0.0f && point.x <= GetWidth() && point.y >= 0.0f && point.y <= GetHeight() ) {
		g_kRootWindow.Unknown727EE0( this );
	}

	if ( event->IsButtonEvent() && ( event->GetFlags() & MEF_BUTTON_DOWN ) && parent != NULL && parent != UI_NO_PARENT ) {
		parent->SetFocus( this, true );
	}
	if ( flags & TGUIO_ALWAYS_HANDLE_EVENTS ) {
		event->SetHandled();
	}
}

/*
================
TGUIObject::KeyboardHandler
================
*/
// FUNCTION: STBC 0x00730f70
void TGUIObject::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( flags & TGUIO_ALWAYS_HANDLE_EVENTS ) {
		event->SetHandled();
	}
}

/*
================
TGUIObject::Print

Traces the bounds (the parent's address is formatted but not printed).
================
*/
// FUNCTION: STBC 0x00730f90
void TGUIObject::Print( int indent, int depth ) {
	char address[12];

	TGObject::Print( indent, depth );
	char *indentString = GetIndentString( indent );
	parent->FormatAddress( address );
	TGTrace( TGTRACE_TYPE_UI, "\n%sBounds(%f, %f, %f, %f) : Size(%f, %f)\n", indentString,
		bounds.left, bounds.top, bounds.width + bounds.left, bounds.height + bounds.top, bounds.width, bounds.height );
}

/*
================
TGUIObject::WriteToStream

The parent is written as its ID: 0 for none, -1 for UI_NO_PARENT.
================
*/
// FUNCTION: STBC 0x00731030
bool TGUIObject::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	if ( parent != NULL && parent != UI_NO_PARENT ) {
		stream->WriteID( parent->GetObjID() );
	} else if ( parent == UI_NO_PARENT ) {
		stream->WriteID( (long)UI_NO_PARENT );
	} else {
		stream->WriteID( 0 );
	}
	bounds.WriteToStream( stream );
	stream->WriteInt( flags );
	return true;
}

/*
================
TGUIObject::ReadFromStream

Keeps the parent's ID in the pointer until PostFixUp.
================
*/
// FUNCTION: STBC 0x007310a0
bool TGUIObject::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	long parentID = stream->ReadID();
	parent = (TGPane *)parentID;
	if ( parentID == (long)UI_NO_PARENT ) {
		parent = UI_NO_PARENT;
	}
	bounds.ReadFromStream( stream );
	flags = stream->ReadInt();
	return true;
}

/*
================
TGUIObject::FixUpPointers

False while the parent's ID names no object yet.
================
*/
// FUNCTION: STBC 0x007310f0
bool TGUIObject::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	if ( parent != NULL && parent != UI_NO_PARENT && GetTGObjectPtr( (long)parent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGUIObject::PostFixUp
================
*/
// FUNCTION: STBC 0x00731120
void TGUIObject::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();
	if ( parent != NULL && parent != UI_NO_PARENT ) {
		parent = (TGPane *)GetTGObjectPtr( (long)parent );
	}
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0072fbd0 _$E426
// FUNCTION: STBC 0x0072fbe0 _$E422
// FUNCTION: STBC 0x0072fbf0 _$E425
// FUNCTION: STBC 0x0072fc00 _$E423
// DATA: STBC 0x0099d638
TGEventHandlerTable TGUIObject::classHandlers;
