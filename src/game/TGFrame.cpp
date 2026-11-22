/*
===============================================================================

	TGFrame.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGFrame.h"
#include "TGEventHandlerTable.h"
#include "TGIconPoly.h"
#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "TGPane.h"
#include "TGUIBatch.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00740ca0 _$E32
// FUNCTION: STBC 0x00740cb0 _$E29
// FUNCTION: STBC 0x00740cc0 _$E31
// FUNCTION: STBC 0x00740cd0 _$E30
// FUNCTION: STBC 0x00740ce0 _$E38
// FUNCTION: STBC 0x00740cf0 _$E35
// FUNCTION: STBC 0x00740d00 _$E37
// FUNCTION: STBC 0x00740d10 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00740d20 _$E57
// FUNCTION: STBC 0x00740d30 _$E56
// FUNCTION: STBC 0x00740d40 _$E61
// FUNCTION: STBC 0x00740d50 _$E60
// FUNCTION: STBC 0x00740d60 _$E72
// FUNCTION: STBC 0x00740d70 _$E71
// FUNCTION: STBC 0x00740d80 _$E76
// FUNCTION: STBC 0x00740d90 _$E75
// FUNCTION: STBC 0x00740da0 _$E80
// FUNCTION: STBC 0x00740db0 _$E79
// FUNCTION: STBC 0x00740dc0 _$E84
// FUNCTION: STBC 0x00740dd0 _$E83
// FUNCTION: STBC 0x00740de0 _$E208
// FUNCTION: STBC 0x00740df0 _$E207
// FUNCTION: STBC 0x00740e00 _$E150
// FUNCTION: STBC 0x00740e10 _$E149
// FUNCTION: STBC 0x00740e20 _$E163
// FUNCTION: STBC 0x00740e30 _$E162
// FUNCTION: STBC 0x00740e40 _$E213
// FUNCTION: STBC 0x00740e50 _$E212
// FUNCTION: STBC 0x00740e60 _$E218
// FUNCTION: STBC 0x00740e70 _$E217
// FUNCTION: STBC 0x00740e80 _$E222
// FUNCTION: STBC 0x00740e90 _$E221
// FUNCTION: STBC 0x00740ea0 _$E226
// FUNCTION: STBC 0x00740eb0 _$E225
// FUNCTION: STBC 0x00740ec0 _$E233
// FUNCTION: STBC 0x00740ed0 _$E232
// FUNCTION: STBC 0x00740ee0 _$E277
// FUNCTION: STBC 0x00740ef0 _$E276
// FUNCTION: STBC 0x00740f00 _$E281
// FUNCTION: STBC 0x00740f10 _$E280
// FUNCTION: STBC 0x00740f20 _$E108
// FUNCTION: STBC 0x00740f30 _$E107
// FUNCTION: STBC 0x00740f40 _$E240
// FUNCTION: STBC 0x00740f50 _$E239
// FUNCTION: STBC 0x00740f60 _$E247
// FUNCTION: STBC 0x00740f70 _$E246
// FUNCTION: STBC 0x00740f80 _$E251
// FUNCTION: STBC 0x00740f90 _$E250
// FUNCTION: STBC 0x00740fa0 _$E255
// FUNCTION: STBC 0x00740fb0 _$E254
// FUNCTION: STBC 0x00740fc0 _$E259
// FUNCTION: STBC 0x00740fd0 _$E258
// FUNCTION: STBC 0x00740fe0 _$E265
// FUNCTION: STBC 0x00740ff0 _$E264
// FUNCTION: STBC 0x00741000 _$E285
// FUNCTION: STBC 0x00741010 _$E284
// FUNCTION: STBC 0x00741020 _$E289
// FUNCTION: STBC 0x00741030 _$E288
// FUNCTION: STBC 0x00741040 _$E293
// FUNCTION: STBC 0x00741050 _$E292
// FUNCTION: STBC 0x00741060 _$E297
// FUNCTION: STBC 0x00741070 _$E296
// FUNCTION: STBC 0x00741080 _$E301
// FUNCTION: STBC 0x00741090 _$E300
// FUNCTION: STBC 0x007410a0 _$E305
// FUNCTION: STBC 0x007410b0 _$E304
// FUNCTION: STBC 0x007410c0 _$E309
// FUNCTION: STBC 0x007410d0 _$E308
// FUNCTION: STBC 0x007410e0 _$E313
// FUNCTION: STBC 0x007410f0 _$E312
// FUNCTION: STBC 0x00741100 _$E317
// FUNCTION: STBC 0x00741110 _$E316
// FUNCTION: STBC 0x00741120 _$E321
// FUNCTION: STBC 0x00741130 _$E320
// FUNCTION: STBC 0x00741140 _$E113
// FUNCTION: STBC 0x00741150 _$E112
// FUNCTION: STBC 0x00741160 _$E325
// FUNCTION: STBC 0x00741170 _$E324
// FUNCTION: STBC 0x00741180 _$E118
// FUNCTION: STBC 0x00741190 _$E117
// FUNCTION: STBC 0x007411a0 _$E329
// FUNCTION: STBC 0x007411b0 _$E328
// FUNCTION: STBC 0x007411c0 _$E333
// FUNCTION: STBC 0x007411d0 _$E332
// FUNCTION: STBC 0x007411e0 _$E337
// FUNCTION: STBC 0x007411f0 _$E336
// FUNCTION: STBC 0x00741200 _$E341
// FUNCTION: STBC 0x00741210 _$E340
// FUNCTION: STBC 0x00741220 _$E127
// FUNCTION: STBC 0x00741230 _$E126
// FUNCTION: STBC 0x00741240 _$E345
// FUNCTION: STBC 0x00741250 _$E344
// FUNCTION: STBC 0x00741260 _$E349
// FUNCTION: STBC 0x00741270 _$E348
// FUNCTION: STBC 0x00741280 _$E353
// FUNCTION: STBC 0x00741290 _$E352
// FUNCTION: STBC 0x007412a0 _$E133
// FUNCTION: STBC 0x007412b0 _$E132
// FUNCTION: STBC 0x007412c0 _$E138
// FUNCTION: STBC 0x007412d0 _$E137
// FUNCTION: STBC 0x007412e0 _$E168
// FUNCTION: STBC 0x007412f0 _$E167
// FUNCTION: STBC 0x00741300 _$E357
// FUNCTION: STBC 0x00741310 _$E356
// FUNCTION: STBC 0x00741320 _$E173
// FUNCTION: STBC 0x00741330 _$E172
// FUNCTION: STBC 0x00741340 _$E361
// FUNCTION: STBC 0x00741350 _$E360
// FUNCTION: STBC 0x00741360 _$E365
// FUNCTION: STBC 0x00741370 _$E364
// FUNCTION: STBC 0x00741380 _$E178
// FUNCTION: STBC 0x00741390 _$E177
// FUNCTION: STBC 0x007413a0 _$E369
// FUNCTION: STBC 0x007413b0 _$E368
// FUNCTION: STBC 0x007413c0 _$E143
// FUNCTION: STBC 0x007413d0 _$E142
// FUNCTION: STBC 0x007413e0 _$E373
// FUNCTION: STBC 0x007413f0 _$E372
// FUNCTION: STBC 0x00741400 _$E377
// FUNCTION: STBC 0x00741410 _$E376
// FUNCTION: STBC 0x00741420 _$E183
// FUNCTION: STBC 0x00741430 _$E182
// FUNCTION: STBC 0x00741440 _$E188
// FUNCTION: STBC 0x00741450 _$E187
// FUNCTION: STBC 0x00741460 _$E381
// FUNCTION: STBC 0x00741470 _$E380
// FUNCTION: STBC 0x00741480 _$E385
// FUNCTION: STBC 0x00741490 _$E384
// FUNCTION: STBC 0x007414a0 _$E389
// FUNCTION: STBC 0x007414b0 _$E388
// FUNCTION: STBC 0x007414c0 _$E393
// FUNCTION: STBC 0x007414d0 _$E392
// FUNCTION: STBC 0x007414e0 _$E397
// FUNCTION: STBC 0x007414f0 _$E396
// FUNCTION: STBC 0x00741500 _$E401
// FUNCTION: STBC 0x00741510 _$E400
// FUNCTION: STBC 0x00741520 _$E405
// FUNCTION: STBC 0x00741530 _$E404
// FUNCTION: STBC 0x00741540 _$E409
// FUNCTION: STBC 0x00741550 _$E408
// FUNCTION: STBC 0x00741560 _$E193
// FUNCTION: STBC 0x00741570 _$E192
// FUNCTION: STBC 0x00741580 _$E413
// FUNCTION: STBC 0x00741590 _$E412
// FUNCTION: STBC 0x007415a0 _$E198
// FUNCTION: STBC 0x007415b0 _$E197
// FUNCTION: STBC 0x007415c0 _$E203
// FUNCTION: STBC 0x007415d0 _$E202

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x007415e0 _$E419
// FUNCTION: STBC 0x007415f0 _$E415
// FUNCTION: STBC 0x00741600 _$E418
// FUNCTION: STBC 0x00741610 _$E416
// DATA: STBC 0x0099e4c0
TGEventHandlerTable TGFrame::classHandlers;

/*
================
TGFrame::Cast

The object as a frame; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00741630
TGFrame *TGFrame::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGFRAME_TYPE ) ) {
		return (TGFrame *)object;
	}
	return NULL;
}

/*
================
TGFrame::TGFrame

A frame without a group name keeps none of the other arguments.
================
*/
// FUNCTION: STBC 0x00741660 ??0TGFrame@@QAE@PADHPAVTGRect@@MH@Z
TGFrame::TGFrame( char *iconGroupName, int firstIconNum, TGRect *bounds, float scale, int stretchType ) : TGUIObject( 0 ) {
	if ( iconGroupName == NULL ) {
		this->iconGroupName = NULL;
		return;
	}
	this->iconGroupName = new char[strlen( iconGroupName ) + 1];
	strcpy( this->iconGroupName, iconGroupName );
	this->firstIconNum = firstIconNum;
	SetBounds( bounds, false );
	this->scale = scale;
	this->stretchType = stretchType;
	color = NiColorA( 1.0f, 1.0f, 1.0f, 1.0f );
	flags &= ~TGUIO_BATCH_CHILD_POLYS;
	edgeStretch = 0;
}

/*
================
TGFrame::~TGFrame
================
*/
// FUNCTION: STBC 0x00741790
TGFrame::~TGFrame( void ) {
	if ( iconGroupName != NULL ) {
		delete[] iconGroupName;
		iconGroupName = NULL;
	}
	VirtualSlot60();
}

/*
================
TGFrame::VirtualSlot60

Takes the polys out of their batches and empties the array.
================
*/
// FUNCTION: STBC 0x00741820
void TGFrame::VirtualSlot60( void ) {
	int numPolys = polys.GetSize();
	if ( numPolys > 0 ) {
		for ( int i = numPolys - 1; i >= 0; i-- ) {
			TGIconPoly *poly = polys.GetAt( i );
			if ( poly != NULL ) {
				poly->batch->RemoveIconPoly( poly );
			}
			polys.RemoveAt( i );
		}
	}
}

/*
================
TGFrame::VirtualSlot61
================
*/
// FUNCTION: STBC 0x00741880
void TGFrame::VirtualSlot61( NiPoint2 *offset ) {
	int numPolys = polys.GetSize();
	if ( numPolys > 0 ) {
		for ( int i = numPolys - 1; i >= 0; i-- ) {
			TGIconPoly *poly = polys.GetAt( i );
			if ( poly != NULL ) {
				poly->Move( offset );
			}
		}
	}
}

/*
================
TGFrame::VirtualSlot62
================
*/
// FUNCTION: STBC 0x007418b0
void TGFrame::VirtualSlot62( TGRect *clipRect ) {
	int numPolys = polys.GetSize();
	if ( numPolys > 0 ) {
		for ( int i = numPolys - 1; i >= 0; i-- ) {
			TGIconPoly *poly = polys.GetAt( i );
			if ( poly != NULL ) {
				poly->SetClipRect( clipRect );
			}
		}
	}
}

/*
================
TGFrame::Layout
================
*/
// FUNCTION: STBC 0x007418e0
void TGFrame::Layout( void ) {
	TGUIObject::Layout();
	if ( iconGroupName == NULL ) {
		return;
	}
	VirtualSlot60();
	switch ( stretchType ) {
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

/*
================
TGFrame::WrapFrame

Lays the frame out from nine icons: the four corners at their own size, the
edges between them wrapping their textures to fit, and the middle the same
way. The middle's rectangle is kept in innerRect.
================
*/
// FUNCTION: STBC 0x00741930
// DRAFT: retail passes bounds.top through the FPU (fld/fstp into the argument) and joins the edge ternaries on the FPU before storing them; frame, calls and branches agree
void TGFrame::WrapFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );

	float width = iconGroup->GetIconScreenWidth( firstIconNum ) * scale;
	float height = iconGroup->GetIconScreenHeight( firstIconNum ) * scale;
	TGRect topLeft( bounds.GetLeft(), bounds.top, width, height );
	AddPoly( FRAME_ICON_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale );
	AddPoly( FRAME_ICON_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	TGRect bottomLeft( bounds.GetLeft(), bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_LEFT, &bottomLeft, true );

	height = ( edgeStretch & 1 ) ? iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP ) * scale : topLeft.height;
	TGRect top( topLeft.GetRight(), bounds.top, topRight.left - topLeft.GetRight(), height );
	AddPoly( FRAME_ICON_TOP, &top, false );

	height = ( edgeStretch & 1 ) ? iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM ) * scale : bottomLeft.height;
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - height, bottomRight.left - bottomLeft.GetRight(), height );
	AddPoly( FRAME_ICON_BOTTOM, &bottom, false );

	width = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_LEFT ) * scale : topLeft.width;
	TGRect leftEdge( bounds.left, topLeft.GetBottom(), width, bottomLeft.top - topLeft.GetBottom() );
	AddPoly( FRAME_ICON_LEFT, &leftEdge, false );

	width = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_RIGHT ) * scale : topRight.width;
	TGRect right( bounds.GetRight() - width, topRight.GetBottom(), width, bottomRight.top - topRight.GetBottom() );
	AddPoly( FRAME_ICON_RIGHT, &right, false );

	TGRect center( leftEdge.GetRight(), top.GetBottom(), right.left - leftEdge.GetRight(), bottom.top - top.GetBottom() );
	AddPoly( FRAME_ICON_CENTER, &center, false );
	innerRect = center;
}

/*
================
TGFrame::TileFrame

Lays the frame out as WrapFrame does, with the edges and the middle made of
as many whole icons as fit, the last one of each row clipped to the
rectangle. The middle's rectangle after tiling is kept in innerRect.
================
*/
// FUNCTION: STBC 0x00741e10
// DRAFT: same corner-rectangle schedule difference as WrapFrame; the tiling loops and clips follow retail's decompilation (first branch polarity differs; 51.1)
void TGFrame::TileFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
	NiPoint2 offset;
	GetScreenOffset( &offset );

	float width = iconGroup->GetIconScreenWidth( firstIconNum ) * scale;
	float height = iconGroup->GetIconScreenHeight( firstIconNum ) * scale;
	TGRect topLeft( bounds.GetLeft(), bounds.top, width, height );
	AddPoly( FRAME_ICON_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, height );
	AddPoly( FRAME_ICON_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	TGRect bottomLeft( bounds.GetLeft(), bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_LEFT, &bottomLeft, true );

	// BUG: poly is not set when an edge has no room for a whole icon, and the
	// clip below then runs on whatever it held.
	TGIconPoly *poly;

	float tileWidth = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_TOP ) * scale;
	float edgeHeight = ( edgeStretch != 1 ) ? topLeft.GetHeight() : iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP ) * scale;
	TGRect top( topLeft.GetRight(), bounds.top, tileWidth, edgeHeight );
	while ( top.left < topRight.left ) {
		poly = AddPoly( FRAME_ICON_TOP, &top, true );
		top.left += tileWidth;
	}
	if ( poly != NULL ) {
		top.width = topRight.left - ( top.left - tileWidth );
		top.left = offset.x + ( top.left - tileWidth );
		top.top = top.top + offset.y;
		poly->SetClipRect( &top );
	}

	tileWidth = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM ) * scale;
	edgeHeight = ( edgeStretch & 1 ) ? iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM ) * scale : bottomLeft.GetHeight();
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - edgeHeight, tileWidth, edgeHeight );
	while ( bottom.left < bottomRight.left ) {
		poly = AddPoly( FRAME_ICON_BOTTOM, &bottom, true );
		bottom.left += tileWidth;
	}
	if ( poly != NULL ) {
		bottom.width = bottomRight.left - ( bottom.left - tileWidth );
		bottom.left = ( bottom.left - tileWidth ) + offset.x;
		bottom.top = bottom.top + offset.y;
		poly->SetClipRect( &bottom );
	}

	float tileHeight = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_LEFT ) * scale;
	float edgeWidth = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_LEFT ) * scale : topLeft.width;
	TGRect leftEdge( bounds.left, topLeft.GetBottom(), edgeWidth, tileHeight );
	while ( leftEdge.top < bottomLeft.top ) {
		poly = AddPoly( FRAME_ICON_LEFT, &leftEdge, true );
		leftEdge.top += tileHeight;
	}
	if ( poly != NULL ) {
		leftEdge.height = bottomLeft.top - ( leftEdge.top - tileHeight );
		leftEdge.left = leftEdge.left + offset.x;
		leftEdge.top = ( leftEdge.top - tileHeight ) + offset.y;
		poly->SetClipRect( &leftEdge );
	}

	tileHeight = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_RIGHT ) * scale;
	edgeWidth = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_RIGHT ) * scale : topRight.width;
	TGRect rightEdge( bounds.GetRight() - edgeWidth, topRight.GetBottom(), edgeWidth, tileHeight );
	while ( rightEdge.top < bottomRight.top ) {
		poly = AddPoly( FRAME_ICON_RIGHT, &rightEdge, true );
		rightEdge.top += tileHeight;
	}
	if ( poly != NULL ) {
		rightEdge.height = bottomRight.top - ( rightEdge.top - tileHeight );
		rightEdge.left = rightEdge.left + offset.x;
		rightEdge.top = ( rightEdge.top - tileHeight ) + offset.y;
		poly->SetClipRect( &rightEdge );
	}

	tileWidth = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_CENTER ) * scale;
	tileHeight = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_CENTER ) * scale;
	TGRect center( leftEdge.GetRight(), top.GetBottom(), tileWidth, tileHeight );
	while ( center.top < bottom.top ) {
		while ( center.left < rightEdge.left ) {
			poly = AddPoly( FRAME_ICON_CENTER, &center, true );
			if ( bottom.top < center.height + center.top ) {
				center.height = bottom.top - center.top;
				center.left = center.left + offset.x;
				center.top = center.top + offset.y;
				poly->SetClipRect( &center );
				center.left = center.left - offset.x;
				center.top = center.top - offset.y;
				center.height = tileHeight;
			}
			center.left += tileWidth;
		}
		if ( poly != NULL ) {
			center.width = rightEdge.left - ( center.left - tileWidth );
			center.left = ( center.left - tileWidth ) + offset.x;
			center.top = center.top + offset.y;
			poly->SetClipRect( &center );
			center.top = center.top - offset.y;
		}
		center.top += tileHeight;
		center.left = leftEdge.GetRight();
	}
	innerRect = center;
}

/*
================
TGFrame::StretchFrame

Lays the frame out as WrapFrame does, with the edges and the middle
stretching their icons' textures over their rectangles.
================
*/
// FUNCTION: STBC 0x00742600
// DRAFT: as WrapFrame: retail passes bounds.top through the FPU and joins the edge ternaries on the FPU before storing them
void TGFrame::StretchFrame( void ) {
	TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );

	float width = iconGroup->GetIconScreenWidth( firstIconNum ) * scale;
	float height = iconGroup->GetIconScreenHeight( firstIconNum ) * scale;
	TGRect topLeft( bounds.GetLeft(), bounds.top, width, height );
	AddPoly( FRAME_ICON_TOP_LEFT, &topLeft, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP_RIGHT ) * scale;
	TGRect topRight( bounds.GetRight() - width, bounds.top, width, height );
	AddPoly( FRAME_ICON_TOP_RIGHT, &topRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_RIGHT ) * scale;
	TGRect bottomRight( bounds.GetRight() - width, bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_RIGHT, &bottomRight, true );

	width = iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	height = iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM_LEFT ) * scale;
	TGRect bottomLeft( bounds.GetLeft(), bounds.GetBottom() - height, width, height );
	AddPoly( FRAME_ICON_BOTTOM_LEFT, &bottomLeft, true );

	height = ( edgeStretch & 1 ) ? iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_TOP ) * scale : topLeft.height;
	TGRect top( topLeft.GetRight(), bounds.top, topRight.left - topLeft.GetRight(), height );
	AddPoly( FRAME_ICON_TOP, &top, true );

	height = ( edgeStretch & 1 ) ? iconGroup->GetIconScreenHeight( firstIconNum + FRAME_ICON_BOTTOM ) * scale : bottomLeft.height;
	TGRect bottom( bottomLeft.GetRight(), bounds.GetBottom() - height, bottomRight.left - bottomLeft.GetRight(), height );
	AddPoly( FRAME_ICON_BOTTOM, &bottom, true );

	width = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_LEFT ) * scale : topLeft.width;
	TGRect leftEdge( bounds.left, topLeft.GetBottom(), width, bottomLeft.top - topLeft.GetBottom() );
	AddPoly( FRAME_ICON_LEFT, &leftEdge, true );

	width = ( edgeStretch & 2 ) ? iconGroup->GetIconScreenWidth( firstIconNum + FRAME_ICON_RIGHT ) * scale : topRight.width;
	TGRect right( bounds.GetRight() - width, topRight.GetBottom(), width, bottomRight.top - topRight.GetBottom() );
	AddPoly( FRAME_ICON_RIGHT, &right, true );

	TGRect center( leftEdge.GetRight(), top.GetBottom(), right.left - leftEdge.GetRight(), bottom.top - top.GetBottom() );
	AddPoly( FRAME_ICON_CENTER, &center, true );
	innerRect = center;
}

/*
================
TGFrame::AddPoly

Makes the icon poly of the frame's icon (firstIconNum + iconOffset) for the
rectangle, within the frame's clip rectangle, and keeps it in the poly
array. Unless noWrap, the poly's texture coordinates are widened so the
texture repeats over the rectangle instead of stretching. NULL when the
rectangle is outside the clip rectangle (the name is invented).
================
*/
// FUNCTION: STBC 0x00742ad0
// DRAFT: retail loads left again for the width numerator (no CSE of rect->left) and allocates a different register for the offset and the revision pointer; the 1.0f compare constant is placed apart
TGIconPoly *TGFrame::AddPoly( int iconOffset, TGRect *rect, bool noWrap ) {
	TGRect clipRect;
	GetClipRect( &clipRect );
	int within = clipRect.Unknown739F00( rect );
	if ( within != RECT_OUTSIDE ) {
		TGIconPoly *poly = g_kIconManager.CreateIconPoly( GetBatches(), iconGroupName, firstIconNum + iconOffset, rect, &color, 0 );
		if ( poly != NULL ) {
			polys.Add( poly );

			if ( !noWrap ) {
				TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
				float iconWidth = iconGroup->GetIconScreenWidth( firstIconNum + iconOffset ) * scale;
				float iconHeight = iconGroup->GetIconScreenHeight( firstIconNum + iconOffset ) * scale;
				float xRatio = ( rect->GetRight() - rect->GetLeft() ) / iconWidth;
				float yRatio = ( rect->GetBottom() - rect->GetTop() ) / iconHeight;
				NiPoint2 *textures = poly->GetTextureCoords();
				if ( xRatio != 1.0f ) {
					float width = textures[2].x - textures[0].x;
					float extra = ( xRatio - width ) * width;
					textures[2].x += extra;
					textures[3].x += extra;
				}
				if ( yRatio != 1.0f ) {
					float height = textures[2].y - textures[0].y;
					float extra = ( yRatio - height ) * height;
					textures[1].y += extra;
					textures[2].y += extra;
				}
				poly->TexturesChanged();
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
	}
	return NULL;
}

/*
================
TGFrame::SetFirstIconNum
================
*/
// FUNCTION: STBC 0x00742ce0
void TGFrame::SetFirstIconNum( int firstIconNum, bool layout ) {
	this->firstIconNum = firstIconNum;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGFrame::SetStretchType
================
*/
// FUNCTION: STBC 0x00742d00
void TGFrame::SetStretchType( int stretchType, bool layout ) {
	this->stretchType = stretchType;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGFrame::SetScale
================
*/
// FUNCTION: STBC 0x00742d20
void TGFrame::SetScale( float scale, bool layout ) {
	this->scale = scale;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGFrame::SetIconGroupName
================
*/
// FUNCTION: STBC 0x00742d40
void TGFrame::SetIconGroupName( char *iconGroupName, bool layout ) {
	if ( this->iconGroupName != NULL ) {
		delete[] this->iconGroupName;
		this->iconGroupName = NULL;
	}
	if ( iconGroupName != NULL ) {
		this->iconGroupName = new char[strlen( iconGroupName ) + 1];
		strcpy( this->iconGroupName, iconGroupName );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGFrame::SetEdgeStretch
================
*/
// FUNCTION: STBC 0x00742dc0
void TGFrame::SetEdgeStretch( int edgeStretch, bool layout ) {
	this->edgeStretch = edgeStretch;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGFrame::WriteToStream
================
*/
// FUNCTION: STBC 0x00742de0
bool TGFrame::WriteToStream( TGStream *stream ) {
	if ( !TGUIObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( iconGroupName );
	stream->WriteLong( firstIconNum );
	stream->WriteFloat( scale );
	stream->WriteLong( stretchType );
	innerRect.WriteToStream( stream );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->WriteFloat( color.a );
	stream->WriteLong( edgeStretch );
	return true;
}

/*
================
TGFrame::ReadFromStream

The polys are made again by the next layout.
================
*/
// FUNCTION: STBC 0x00742e70
bool TGFrame::ReadFromStream( TGStream *stream ) {
	if ( !TGUIObject::ReadFromStream( stream ) ) {
		return false;
	}
	polys.RemoveAll();
	iconGroupName = stream->VirtualSlot10( -1 );
	firstIconNum = stream->ReadLong();
	scale = stream->ReadFloat();
	stretchType = stream->ReadLong();
	innerRect.ReadFromStream( stream );
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	color.a = stream->ReadFloat();
	edgeStretch = stream->ReadLong();
	return true;
}
