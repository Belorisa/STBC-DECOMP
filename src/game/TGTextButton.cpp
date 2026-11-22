/*
===============================================================================

	TGTextButton.cpp

===============================================================================
*/

#include <iostream>
#include "TGTextButton.h"
#include "TGParagraph.h"
#include "TGFontGroup.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGIconPoly.h"
#include "TGUIBatch.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiPoint2.h"
#include "NiTriShape.h"

#include <string.h>
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0073a1e0 _$E32
// FUNCTION: STBC 0x0073a1f0 _$E29
// FUNCTION: STBC 0x0073a200 _$E31
// FUNCTION: STBC 0x0073a210 _$E30
// FUNCTION: STBC 0x0073a220 _$E38
// FUNCTION: STBC 0x0073a230 _$E35
// FUNCTION: STBC 0x0073a240 _$E37
// FUNCTION: STBC 0x0073a250 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0073a260 _$E57
// FUNCTION: STBC 0x0073a270 _$E56
// FUNCTION: STBC 0x0073a280 _$E61
// FUNCTION: STBC 0x0073a290 _$E60
// FUNCTION: STBC 0x0073a2a0 _$E72
// FUNCTION: STBC 0x0073a2b0 _$E71
// FUNCTION: STBC 0x0073a2c0 _$E76
// FUNCTION: STBC 0x0073a2d0 _$E75
// FUNCTION: STBC 0x0073a2e0 _$E80
// FUNCTION: STBC 0x0073a2f0 _$E79
// FUNCTION: STBC 0x0073a300 _$E84
// FUNCTION: STBC 0x0073a310 _$E83
// FUNCTION: STBC 0x0073a320 _$E208
// FUNCTION: STBC 0x0073a330 _$E207
// FUNCTION: STBC 0x0073a340 _$E150
// FUNCTION: STBC 0x0073a350 _$E149
// FUNCTION: STBC 0x0073a360 _$E163
// FUNCTION: STBC 0x0073a370 _$E162
// FUNCTION: STBC 0x0073a380 _$E213
// FUNCTION: STBC 0x0073a390 _$E212
// FUNCTION: STBC 0x0073a3a0 _$E218
// FUNCTION: STBC 0x0073a3b0 _$E217
// FUNCTION: STBC 0x0073a3c0 _$E222
// FUNCTION: STBC 0x0073a3d0 _$E221
// FUNCTION: STBC 0x0073a3e0 _$E226
// FUNCTION: STBC 0x0073a3f0 _$E225
// FUNCTION: STBC 0x0073a400 _$E233
// FUNCTION: STBC 0x0073a410 _$E232
// FUNCTION: STBC 0x0073a420 _$E284
// FUNCTION: STBC 0x0073a430 _$E283
// FUNCTION: STBC 0x0073a440 _$E288
// FUNCTION: STBC 0x0073a450 _$E287
// FUNCTION: STBC 0x0073a460 _$E108
// FUNCTION: STBC 0x0073a470 _$E107
// FUNCTION: STBC 0x0073a480 _$E249
// FUNCTION: STBC 0x0073a490 _$E248
// FUNCTION: STBC 0x0073a4a0 _$E256
// FUNCTION: STBC 0x0073a4b0 _$E255
// FUNCTION: STBC 0x0073a4c0 _$E260
// FUNCTION: STBC 0x0073a4d0 _$E259
// FUNCTION: STBC 0x0073a4e0 _$E264
// FUNCTION: STBC 0x0073a4f0 _$E263
// FUNCTION: STBC 0x0073a500 _$E268
// FUNCTION: STBC 0x0073a510 _$E267
// FUNCTION: STBC 0x0073a520 _$E274
// FUNCTION: STBC 0x0073a530 _$E273
// FUNCTION: STBC 0x0073a540 _$E292
// FUNCTION: STBC 0x0073a550 _$E291
// FUNCTION: STBC 0x0073a560 _$E296
// FUNCTION: STBC 0x0073a570 _$E295
// FUNCTION: STBC 0x0073a580 _$E300
// FUNCTION: STBC 0x0073a590 _$E299
// FUNCTION: STBC 0x0073a5a0 _$E304
// FUNCTION: STBC 0x0073a5b0 _$E303
// FUNCTION: STBC 0x0073a5c0 _$E308
// FUNCTION: STBC 0x0073a5d0 _$E307
// FUNCTION: STBC 0x0073a5e0 _$E312
// FUNCTION: STBC 0x0073a5f0 _$E311
// FUNCTION: STBC 0x0073a600 _$E316
// FUNCTION: STBC 0x0073a610 _$E315
// FUNCTION: STBC 0x0073a620 _$E320
// FUNCTION: STBC 0x0073a630 _$E319
// FUNCTION: STBC 0x0073a640 _$E324
// FUNCTION: STBC 0x0073a650 _$E323
// FUNCTION: STBC 0x0073a660 _$E328
// FUNCTION: STBC 0x0073a670 _$E327
// FUNCTION: STBC 0x0073a680 _$E113
// FUNCTION: STBC 0x0073a690 _$E112
// FUNCTION: STBC 0x0073a6a0 _$E332
// FUNCTION: STBC 0x0073a6b0 _$E331
// FUNCTION: STBC 0x0073a6c0 _$E118
// FUNCTION: STBC 0x0073a6d0 _$E117
// FUNCTION: STBC 0x0073a6e0 _$E336
// FUNCTION: STBC 0x0073a6f0 _$E335
// FUNCTION: STBC 0x0073a700 _$E340
// FUNCTION: STBC 0x0073a710 _$E339
// FUNCTION: STBC 0x0073a720 _$E344
// FUNCTION: STBC 0x0073a730 _$E343
// FUNCTION: STBC 0x0073a740 _$E348
// FUNCTION: STBC 0x0073a750 _$E347
// FUNCTION: STBC 0x0073a760 _$E127
// FUNCTION: STBC 0x0073a770 _$E126
// FUNCTION: STBC 0x0073a780 _$E352
// FUNCTION: STBC 0x0073a790 _$E351
// FUNCTION: STBC 0x0073a7a0 _$E356
// FUNCTION: STBC 0x0073a7b0 _$E355
// FUNCTION: STBC 0x0073a7c0 _$E360
// FUNCTION: STBC 0x0073a7d0 _$E359
// FUNCTION: STBC 0x0073a7e0 _$E133
// FUNCTION: STBC 0x0073a7f0 _$E132
// FUNCTION: STBC 0x0073a800 _$E138
// FUNCTION: STBC 0x0073a810 _$E137
// FUNCTION: STBC 0x0073a820 _$E168
// FUNCTION: STBC 0x0073a830 _$E167
// FUNCTION: STBC 0x0073a840 _$E364
// FUNCTION: STBC 0x0073a850 _$E363
// FUNCTION: STBC 0x0073a860 _$E173
// FUNCTION: STBC 0x0073a870 _$E172
// FUNCTION: STBC 0x0073a880 _$E368
// FUNCTION: STBC 0x0073a890 _$E367
// FUNCTION: STBC 0x0073a8a0 _$E372
// FUNCTION: STBC 0x0073a8b0 _$E371
// FUNCTION: STBC 0x0073a8c0 _$E178
// FUNCTION: STBC 0x0073a8d0 _$E177
// FUNCTION: STBC 0x0073a8e0 _$E376
// FUNCTION: STBC 0x0073a8f0 _$E375
// FUNCTION: STBC 0x0073a900 _$E143
// FUNCTION: STBC 0x0073a910 _$E142
// FUNCTION: STBC 0x0073a920 _$E380
// FUNCTION: STBC 0x0073a930 _$E379
// FUNCTION: STBC 0x0073a940 _$E384
// FUNCTION: STBC 0x0073a950 _$E383
// FUNCTION: STBC 0x0073a960 _$E183
// FUNCTION: STBC 0x0073a970 _$E182
// FUNCTION: STBC 0x0073a980 _$E188
// FUNCTION: STBC 0x0073a990 _$E187
// FUNCTION: STBC 0x0073a9a0 _$E388
// FUNCTION: STBC 0x0073a9b0 _$E387
// FUNCTION: STBC 0x0073a9c0 _$E392
// FUNCTION: STBC 0x0073a9d0 _$E391
// FUNCTION: STBC 0x0073a9e0 _$E396
// FUNCTION: STBC 0x0073a9f0 _$E395
// FUNCTION: STBC 0x0073aa00 _$E400
// FUNCTION: STBC 0x0073aa10 _$E399
// FUNCTION: STBC 0x0073aa20 _$E404
// FUNCTION: STBC 0x0073aa30 _$E403
// FUNCTION: STBC 0x0073aa40 _$E408
// FUNCTION: STBC 0x0073aa50 _$E407
// FUNCTION: STBC 0x0073aa60 _$E412
// FUNCTION: STBC 0x0073aa70 _$E411
// FUNCTION: STBC 0x0073aa80 _$E416
// FUNCTION: STBC 0x0073aa90 _$E415
// FUNCTION: STBC 0x0073aaa0 _$E193
// FUNCTION: STBC 0x0073aab0 _$E192
// FUNCTION: STBC 0x0073aac0 _$E420
// FUNCTION: STBC 0x0073aad0 _$E419
// FUNCTION: STBC 0x0073aae0 _$E198
// FUNCTION: STBC 0x0073aaf0 _$E197
// FUNCTION: STBC 0x0073ab00 _$E203
// FUNCTION: STBC 0x0073ab10 _$E202

/*
================
TGTextButton::TGTextButton

A button the size of its text and margins, the text centred both ways.
================
*/
// FUNCTION: STBC 0x0073ab70 ??0TGTextButton@@QAE@PAD0HMHMM@Z
TGTextButton::TGTextButton( char *text, char *iconGroupName, int firstIconNum, float iconScale, int frameMode, float horizontalMargin, float verticalMargin ) {
	this->firstIconNum = firstIconNum;
	this->iconScale = iconScale;
	this->frameMode = frameMode;
	horizontalAlign = ALIGN_CENTER;
	verticalAlign = ALIGN_MIDDLE;
	this->horizontalMargin = horizontalMargin;
	this->verticalMargin = verticalMargin;
	if ( iconGroupName != NULL ) {
		this->iconGroupName = new char[strlen( iconGroupName ) + 1];
		strcpy( this->iconGroupName, iconGroupName );
	} else {
		this->iconGroupName = NULL;
	}

	TGParagraph *paragraph = new TGParagraph( text, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	if ( paragraph == NULL ) {
		return;
	}
	AddChild( paragraph, this->horizontalMargin, this->verticalMargin, false );
	Resize( paragraph->GetWidth() + horizontalMargin * 2.0f, paragraph->GetHeight() + verticalMargin * 2.0f, false );
}

/*
================
TGTextButton::TGTextButton
================
*/
// FUNCTION: STBC 0x0073ad00 ??0TGTextButton@@QAE@PAVTGString@@PADHMHMM@Z
TGTextButton::TGTextButton( TGString *text, char *iconGroupName, int firstIconNum, float iconScale, int frameMode, float horizontalMargin, float verticalMargin ) {
	this->firstIconNum = firstIconNum;
	this->iconScale = iconScale;
	this->frameMode = frameMode;
	horizontalAlign = ALIGN_CENTER;
	verticalAlign = ALIGN_MIDDLE;
	this->horizontalMargin = horizontalMargin;
	this->verticalMargin = verticalMargin;
	if ( iconGroupName != NULL ) {
		this->iconGroupName = new char[strlen( iconGroupName ) + 1];
		strcpy( this->iconGroupName, iconGroupName );
	} else {
		this->iconGroupName = NULL;
	}

	TGParagraph *paragraph = new TGParagraph( text, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	if ( paragraph == NULL ) {
		return;
	}
	AddChild( paragraph, this->horizontalMargin, this->verticalMargin, false );
	Resize( paragraph->GetWidth() + horizontalMargin * 2.0f, paragraph->GetHeight() + verticalMargin * 2.0f, false );
}

/*
================
TGTextButton::~TGTextButton
================
*/
// FUNCTION: STBC 0x0073ae90
TGTextButton::~TGTextButton( void ) {
	if ( iconGroupName != NULL ) {
		delete[] iconGroupName;
		iconGroupName = NULL;
	}
}

/*
================
TGTextButton::Cast

The object as a text button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0073af10
TGTextButton *TGTextButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGTEXTBUTTON_TYPE ) ) {
		return (TGTextButton *)object;
	}
	return NULL;
}

/*
================
TGTextButton::SetText

Replaces the text, realigning it; NULL leaves the text as it is.
================
*/
// FUNCTION: STBC 0x0073af40
void TGTextButton::SetText( char *text ) {
	if ( text == NULL ) {
		return;
	}
	TGRect rect;
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	paragraph->SetBounds( &rect, false );
	paragraph->SetString( text, true );
	AlignTextHorizontal( horizontalAlign, false );
	AlignTextVertical( verticalAlign, false );
	Layout();
}

/*
================
TGTextButton::SetTextW
================
*/
// FUNCTION: STBC 0x0073aff0
void TGTextButton::SetTextW( TGString *text ) {
	TGRect rect;
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	paragraph->SetBounds( &rect, false );
	paragraph->SetString( text, true );
	AlignTextHorizontal( horizontalAlign, false );
	AlignTextVertical( verticalAlign, false );
	Layout();
}

/*
================
TGTextButton::SetIcons

The frame's icon group; NULL draws no frame.
================
*/
// FUNCTION: STBC 0x0073b0a0
void TGTextButton::SetIcons( char *iconGroupName, int firstIconNum ) {
	if ( this->iconGroupName != NULL ) {
		delete[] this->iconGroupName;
		this->iconGroupName = NULL;
	}
	if ( iconGroupName != NULL ) {
		this->iconGroupName = new char[strlen( iconGroupName ) + 1];
		strcpy( this->iconGroupName, iconGroupName );
		this->firstIconNum = firstIconNum;
	}
	Layout();
}

/*
================
TGTextButton::SetTextWrapWidth
================
*/
// FUNCTION: STBC 0x0073b120
void TGTextButton::SetTextWrapWidth( float width ) {
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	if ( paragraph == NULL ) {
		return;
	}
	paragraph->SetWrapWidth( width, false );
	AlignTextHorizontal( horizontalAlign, false );
	AlignTextVertical( verticalAlign, false );
	Layout();
}

/*
================
TGTextButton::SetEnabled

Lays the frame out again for the enabled icons.
================
*/
// FUNCTION: STBC 0x0073b180
void TGTextButton::SetEnabled( bool layout ) {
	TGUIObject::SetEnabled( false );
	Layout();
}

/*
================
TGTextButton::SetDisabled

Lays the frame out again for the disabled icons.
================
*/
// FUNCTION: STBC 0x0073b1a0
void TGTextButton::SetDisabled( bool layout ) {
	// BUG: TGButtonBase::SetDisabled is skipped, so a button disabled in
	// the middle of a click keeps the mouse.
	TGUIObject::SetDisabled( false );
	Layout();
}

/*
================
TGTextButton::AlignTextHorizontal
================
*/
// FUNCTION: STBC 0x0073b1c0
void TGTextButton::AlignTextHorizontal( int align, bool layout ) {
	horizontalAlign = align;
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	if ( paragraph == NULL ) {
		return;
	}
	float x = horizontalMargin;
	// BUG: tests the vertical alignment, so the text is placed as for the
	// vertical one (ALIGN_TOP is ALIGN_LEFT, and so on).
	if ( verticalAlign != ALIGN_LEFT ) {
		x = GetWidth() - paragraph->GetWidth();
		if ( verticalAlign == ALIGN_CENTER ) {
			x *= 0.5f;
		} else {
			x -= horizontalMargin;
		}
	}
	paragraph->SetPosition( x, paragraph->GetTop(), layout );
}

/*
================
TGTextButton::AlignTextVertical
================
*/
// FUNCTION: STBC 0x0073b230
void TGTextButton::AlignTextVertical( int align, bool layout ) {
	verticalAlign = align;
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	if ( paragraph == NULL ) {
		return;
	}
	float y = verticalMargin;
	if ( verticalAlign != ALIGN_TOP ) {
		y = GetHeight() - paragraph->GetHeight();
		if ( verticalAlign == ALIGN_MIDDLE ) {
			y *= 0.5f;
		} else {
			y -= verticalMargin;
		}
	}
	paragraph->SetPosition( paragraph->GetLeft(), y, layout );
}

/*
================
TGTextButton::Resize

Wraps the text to the width inside the margins, with half a space to
spare.
================
*/
// FUNCTION: STBC 0x0073b2a0
void TGTextButton::Resize( float width, float height, bool layout ) {
	TGUIObject::Resize( width, height, false );
	float wrapWidth = GetWidth() - horizontalMargin * 2.0f;
	TGParagraph *paragraph = (TGParagraph *)GetFirstChild();
	if ( paragraph != NULL ) {
		TGFontGroup *fontGroup = paragraph->GetFontGroup();
		if ( fontGroup != NULL ) {
			wrapWidth += fontGroup->GetScreenSpaceWidth() * 0.5f;
		}
	}
	SetTextWrapWidth( wrapWidth );
}

/*
================
TGTextButton::Layout

Rebuilds the frame, when there is one, in the frame mode.
================
*/
// FUNCTION: STBC 0x0073b310
void TGTextButton::Layout( void ) {
	VirtualSlot60();
	if ( iconGroupName != NULL ) {
		switch ( frameMode ) {
			case FRAME_STRETCH_TO_FIT:
				StretchFrame();
				break;
			case FRAME_WRAP_TEXTURES_TO_FIT:
				WrapFrame();
				break;
			case FRAME_TILE_POLYS_TO_FIT:
				TileFrame();
				break;
		}
	}
	TGPane::Layout();
}

/*
================
TGTextButton::VirtualSlot60

Takes the frame's polys out of their batches.
================
*/
// FUNCTION: STBC 0x0073b360
void TGTextButton::VirtualSlot60( void ) {
	int numPolys = framePolys.GetSize();
	for ( int i = 0; i < numPolys; i++ ) {
		TGIconPoly *poly = framePolys.GetAt( i );
		if ( poly != NULL ) {
			poly->batch->RemoveIconPoly( poly );
		}
	}
	framePolys.RemoveAll();
	TGPane::VirtualSlot60();
}

/*
================
TGTextButton::VirtualSlot61

Moves the frame's polys with the button.
================
*/
// FUNCTION: STBC 0x0073b3c0
void TGTextButton::VirtualSlot61( NiPoint2 *offset ) {
	int numPolys = framePolys.GetSize();
	for ( int i = 0; i < numPolys; i++ ) {
		TGIconPoly *poly = framePolys.GetAt( i );
		if ( poly != NULL ) {
			poly->Move( offset );
		}
	}
	TGPane::VirtualSlot61( offset );
}

/*
================
TGTextButton::VirtualSlot62

Clips the frame's polys with the button.
================
*/
// FUNCTION: STBC 0x0073b400
void TGTextButton::VirtualSlot62( TGRect *clipRect ) {
	TGPane::VirtualSlot62( clipRect );
	int numPolys = framePolys.GetSize();
	for ( int i = 0; i < numPolys; i++ ) {
		TGIconPoly *poly = framePolys.GetAt( i );
		if ( poly != NULL ) {
			poly->SetClipRect( clipRect );
		}
	}
}

/*
================
TGTextButton::WrapFrame

The corners at their own size; the edges and the middle stretched over
the rest, their textures repeated to keep the icons' scale.
================
*/
// FUNCTION: STBC 0x0073b440
// PARKED: complete; the bottom edge's x is stored after the argument pushes in retail, before them here
void TGTextButton::WrapFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
	if ( iconGroup == NULL ) {
		return;
	}
	int iconNum = GetFrameIconNum();

	float width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_LEFT ) * iconScale;
	float height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_LEFT ) * iconScale;
	TGRect topLeft( bounds.left, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	TGRect bottomLeft( bounds.left, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_LEFT, &bottomLeft, true );

	height = topLeft.GetBottom() - topLeft.GetTop();
	TGRect top( topLeft.GetRight(), bounds.top, topRight.GetLeft() - topLeft.GetRight(), height );
	AddFramePoly( FRAME_TOP, &top, false );

	height = bottomLeft.GetBottom() - bottomLeft.GetTop();
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - height, bottomRight.GetLeft() - bottomLeft.GetRight(), height );
	AddFramePoly( FRAME_BOTTOM, &bottom, false );

	width = topLeft.GetRight() - topLeft.GetLeft();
	TGRect left( bounds.left, topLeft.GetBottom(), width, bottomLeft.GetTop() - topLeft.GetBottom() );
	AddFramePoly( FRAME_LEFT, &left, false );

	width = topRight.GetRight() - topRight.GetLeft();
	TGRect right( bounds.GetRight() - width, topRight.GetBottom(), width, bottomRight.GetTop() - topRight.GetBottom() );
	AddFramePoly( FRAME_RIGHT, &right, false );

	TGRect middle( topLeft.GetRight(), topLeft.GetBottom(), bottomRight.GetLeft() - bottomLeft.GetRight(), bottomRight.GetTop() - topLeft.GetBottom() );
	AddFramePoly( FRAME_MIDDLE, &middle, false );

	innerRect = middle;
}

/*
================
TGTextButton::TileFrame

The corners at their own size; the edges and the middle covered with
polys of the icons at their own size, the last of each row and column
clipped where it runs past the frame.
================
*/
// FUNCTION: STBC 0x0073b8c0
// PARKED: complete; the top edge's first argument is stored before the rectangle's address load in retail, after it here
void TGTextButton::TileFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
	if ( iconGroup == NULL ) {
		return;
	}
	NiPoint2 offset;
	GetScreenOffset( &offset );
	int iconNum = GetFrameIconNum();

	float width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_LEFT ) * iconScale;
	float height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_LEFT ) * iconScale;
	TGRect topLeft( bounds.left, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	TGRect bottomLeft( bounds.left, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_LEFT, &bottomLeft, true );

	// BUG: poly is not initialised, so with no room for a tile on the top
	// edge the clip below works on whatever the variable holds.
	TGIconPoly *poly;

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP ) * iconScale;
	height = topLeft.GetBottom() - topLeft.GetTop();
	TGRect top( topLeft.GetRight(), bounds.top, width, height );
	while ( top.left < topRight.left ) {
		poly = AddFramePoly( FRAME_TOP, &top, true );
		top.left += width;
	}
	if ( poly != NULL ) {
		top.left -= width;
		top.width = topRight.left - top.left;
		top.left += offset.x;
		top.top += offset.y;
		poly->SetClipRect( &top );
	}

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM ) * iconScale;
	height = bottomLeft.GetBottom() - bottomLeft.GetTop();
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - height, width, height );
	while ( bottom.left < bottomRight.left ) {
		poly = AddFramePoly( FRAME_BOTTOM, &bottom, true );
		bottom.left += width;
	}
	if ( poly != NULL ) {
		bottom.left -= width;
		bottom.width = bottomRight.left - bottom.left;
		bottom.left += offset.x;
		bottom.top += offset.y;
		poly->SetClipRect( &bottom );
	}

	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_LEFT ) * iconScale;
	width = topLeft.GetRight() - topLeft.GetLeft();
	TGRect left( bounds.left, topLeft.GetBottom(), width, height );
	while ( left.top < bottomLeft.top ) {
		poly = AddFramePoly( FRAME_LEFT, &left, true );
		left.top += height;
	}
	if ( poly != NULL ) {
		left.top -= height;
		left.height = bottomLeft.top - left.top;
		left.left += offset.x;
		left.top += offset.y;
		poly->SetClipRect( &left );
	}

	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_RIGHT ) * iconScale;
	width = topRight.GetRight() - topRight.GetLeft();
	TGRect right( bounds.GetRight() - width, topRight.GetBottom(), width, height );
	while ( right.top < bottomRight.top ) {
		poly = AddFramePoly( FRAME_RIGHT, &right, true );
		right.top += height;
	}
	if ( poly != NULL ) {
		right.top -= height;
		right.height = bottomRight.top - right.top;
		right.left += offset.x;
		right.top += offset.y;
		poly->SetClipRect( &right );
	}

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_MIDDLE ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_MIDDLE ) * iconScale;
	TGRect middle( topLeft.GetRight(), topLeft.GetBottom(), width, height );
	while ( middle.top < bottomLeft.top ) {
		while ( middle.left < topRight.left ) {
			poly = AddFramePoly( FRAME_MIDDLE, &middle, true );
			if ( middle.GetBottom() > bottomLeft.top ) {
				// The last row: clip the tile to the bottom edge.
				middle.height = bottomLeft.top - middle.top;
				middle.left += offset.x;
				middle.top += offset.y;
				poly->SetClipRect( &middle );
				middle.left -= offset.x;
				middle.top -= offset.y;
				middle.height = height;
			}
			middle.left += width;
		}
		if ( poly != NULL ) {
			// The last column: clip the tile to the right edge.
			// BUG: the clipped width is kept for the rows that follow.
			middle.left -= width;
			middle.width = topRight.left - middle.left;
			middle.left += offset.x;
			middle.top += offset.y;
			poly->SetClipRect( &middle );
			middle.top -= offset.y;
		}
		middle.top += height;
		middle.left = topLeft.GetRight();
	}

	innerRect = middle;
}

/*
================
TGTextButton::StretchFrame

The corners at their own size; the edges and the middle stretched over
the rest.
================
*/
// FUNCTION: STBC 0x0073c050
// PARKED: complete; the bottom edge's x is stored after the argument pushes in retail, before them here
void TGTextButton::StretchFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
	if ( iconGroup == NULL ) {
		return;
	}
	int iconNum = GetFrameIconNum();

	float width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_LEFT ) * iconScale;
	float height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_LEFT ) * iconScale;
	TGRect topLeft( bounds.left, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_TOP_RIGHT ) * iconScale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, height );
	AddFramePoly( FRAME_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_RIGHT ) * iconScale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	height = iconGroup->GetIconScreenHeight( iconNum + FRAME_BOTTOM_LEFT ) * iconScale;
	TGRect bottomLeft( bounds.left, bounds.GetBottom() - height, width, height );
	AddFramePoly( FRAME_BOTTOM_LEFT, &bottomLeft, true );

	height = topLeft.GetBottom() - topLeft.GetTop();
	TGRect top( topLeft.GetRight(), bounds.top, topRight.GetLeft() - topLeft.GetRight(), height );
	AddFramePoly( FRAME_TOP, &top, true );

	height = bottomLeft.GetBottom() - bottomLeft.GetTop();
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - height, bottomRight.GetLeft() - bottomLeft.GetRight(), height );
	AddFramePoly( FRAME_BOTTOM, &bottom, true );

	width = topLeft.GetRight() - topLeft.GetLeft();
	TGRect left( bounds.left, topLeft.GetBottom(), width, bottomLeft.GetTop() - topLeft.GetBottom() );
	AddFramePoly( FRAME_LEFT, &left, true );

	width = topRight.GetRight() - topRight.GetLeft();
	TGRect right( bounds.GetRight() - width, topRight.GetBottom(), width, bottomRight.GetTop() - topRight.GetBottom() );
	AddFramePoly( FRAME_RIGHT, &right, true );

	TGRect middle( topLeft.GetRight(), topLeft.GetBottom(), bottomRight.GetLeft() - bottomLeft.GetRight(), bottomRight.GetTop() - topRight.GetBottom() );
	AddFramePoly( FRAME_MIDDLE, &middle, true );

	innerRect = middle;
}

/*
================
TGTextButton::AddFramePoly

Makes a poly of one of the frame's icons for the state, over the part of
the rectangle the button shows. Unstretched, the poly's texture repeats
to keep the icon's scale.
================
*/
// FUNCTION: STBC 0x0073c4d0
// PARKED: complete; retail reloads the rectangle's left and top for each subtraction where ours keeps them on the FPU stack, and the repeat and size locals take swapped slots (nested TGRect accessors tried)
TGIconPoly *TGTextButton::AddFramePoly( int iconNum, TGRect *rect, bool stretch ) {
	TGRect clipRect;
	GetClipRect( &clipRect );
	int within = clipRect.Unknown739F00( rect );
	if ( within == RECT_OUTSIDE ) {
		return NULL;
	}

	// The number comes relative to the frame's first icon for the state.
	iconNum += GetFrameIconNum();
	TGIconPoly *poly = g_kIconManager.CreateIconPoly( GetBatches(), iconGroupName, iconNum, rect, NULL, 0 );
	if ( poly == NULL ) {
		return NULL;
	}
	framePolys.Add( poly );

	if ( !stretch ) {
		TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
		float iconWidth = iconGroup->GetIconScreenWidth( iconNum ) * iconScale;
		float iconHeight = iconGroup->GetIconScreenHeight( iconNum ) * iconScale;
		float repeatU = ( rect->GetRight() - rect->GetLeft() ) / iconWidth;
		float repeatV = ( rect->GetBottom() - rect->GetTop() ) / iconHeight;

		NiPoint2 *coords = poly->GetTextureCoords();
		float sizeU = coords[2].x - coords[0].x;
		float sizeV = coords[2].y - coords[0].y;
		coords[1].x = coords[0].x = coords[0].x + sizeU * repeatU;
		coords[3].y = coords[0].y = coords[0].y + sizeV * repeatV;
		poly->batch->shape->GetShapeData()->IncRevisionID();
	}

	if ( within == RECT_PARTIAL ) {
		poly->SetClipRect( &clipRect );
	}
	if ( parent != NULL ) {
		NiPoint2 offset;
		parent->GetScreenOffset( &offset );
		poly->Move( &offset );
	}
	return poly;
}

/*
================
TGTextButton::WriteToStream
================
*/
// FUNCTION: STBC 0x0073c6f0
bool TGTextButton::WriteToStream( TGStream *stream ) {
	if ( !TGButtonBase::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( iconGroupName );
	stream->WriteLong( firstIconNum );
	stream->WriteFloat( iconScale );
	stream->WriteLong( frameMode );
	stream->WriteLong( horizontalAlign );
	stream->WriteLong( verticalAlign );
	innerRect.WriteToStream( stream );
	stream->WriteFloat( horizontalMargin );
	stream->WriteFloat( verticalMargin );
	return true;
}

/*
================
TGTextButton::ReadFromStream

The frame's polys are made again by Layout.
================
*/
// FUNCTION: STBC 0x0073c780
bool TGTextButton::ReadFromStream( TGStream *stream ) {
	if ( !TGButtonBase::ReadFromStream( stream ) ) {
		return false;
	}
	iconGroupName = stream->VirtualSlot10( -1 );
	firstIconNum = stream->ReadLong();
	iconScale = stream->ReadFloat();
	frameMode = stream->ReadLong();
	horizontalAlign = stream->ReadLong();
	verticalAlign = stream->ReadLong();
	innerRect.ReadFromStream( stream );
	horizontalMargin = stream->ReadFloat();
	verticalMargin = stream->ReadFloat();
	return true;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0073ab20 _$E426
// FUNCTION: STBC 0x0073ab30 _$E422
// FUNCTION: STBC 0x0073ab40 _$E425
// FUNCTION: STBC 0x0073ab50 _$E423
// DATA: STBC 0x0099df20
TGEventHandlerTable TGTextButton::classHandlers;
