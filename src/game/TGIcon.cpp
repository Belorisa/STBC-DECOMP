/*
===============================================================================

	TGIcon.cpp

===============================================================================
*/

#include <iostream>
#include "TGIcon.h"
#include "TGIconPoly.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGUIModule.h"
#include "TGPane.h"
#include "TGUIBatch.h"
#include "TGStream.h"
#include "TGEventHandlerTable.h"

#include <string.h>
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0073c930 _$E32
// FUNCTION: STBC 0x0073c940 _$E29
// FUNCTION: STBC 0x0073c950 _$E31
// FUNCTION: STBC 0x0073c960 _$E30
// FUNCTION: STBC 0x0073c970 _$E38
// FUNCTION: STBC 0x0073c980 _$E35
// FUNCTION: STBC 0x0073c990 _$E37
// FUNCTION: STBC 0x0073c9a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0073c9b0 _$E57
// FUNCTION: STBC 0x0073c9c0 _$E56
// FUNCTION: STBC 0x0073c9d0 _$E61
// FUNCTION: STBC 0x0073c9e0 _$E60
// FUNCTION: STBC 0x0073c9f0 _$E72
// FUNCTION: STBC 0x0073ca00 _$E71
// FUNCTION: STBC 0x0073ca10 _$E76
// FUNCTION: STBC 0x0073ca20 _$E75
// FUNCTION: STBC 0x0073ca30 _$E80
// FUNCTION: STBC 0x0073ca40 _$E79
// FUNCTION: STBC 0x0073ca50 _$E84
// FUNCTION: STBC 0x0073ca60 _$E83
// FUNCTION: STBC 0x0073ca70 _$E208
// FUNCTION: STBC 0x0073ca80 _$E207
// FUNCTION: STBC 0x0073ca90 _$E150
// FUNCTION: STBC 0x0073caa0 _$E149
// FUNCTION: STBC 0x0073cab0 _$E163
// FUNCTION: STBC 0x0073cac0 _$E162
// FUNCTION: STBC 0x0073cad0 _$E213
// FUNCTION: STBC 0x0073cae0 _$E212
// FUNCTION: STBC 0x0073caf0 _$E218
// FUNCTION: STBC 0x0073cb00 _$E217
// FUNCTION: STBC 0x0073cb10 _$E222
// FUNCTION: STBC 0x0073cb20 _$E221
// FUNCTION: STBC 0x0073cb30 _$E226
// FUNCTION: STBC 0x0073cb40 _$E225
// FUNCTION: STBC 0x0073cb50 _$E233
// FUNCTION: STBC 0x0073cb60 _$E232
// FUNCTION: STBC 0x0073cb70 _$E279
// FUNCTION: STBC 0x0073cb80 _$E278
// FUNCTION: STBC 0x0073cb90 _$E283
// FUNCTION: STBC 0x0073cba0 _$E282
// FUNCTION: STBC 0x0073cbb0 _$E108
// FUNCTION: STBC 0x0073cbc0 _$E107
// FUNCTION: STBC 0x0073cbd0 _$E240
// FUNCTION: STBC 0x0073cbe0 _$E239
// FUNCTION: STBC 0x0073cbf0 _$E247
// FUNCTION: STBC 0x0073cc00 _$E246
// FUNCTION: STBC 0x0073cc10 _$E251
// FUNCTION: STBC 0x0073cc20 _$E250
// FUNCTION: STBC 0x0073cc30 _$E255
// FUNCTION: STBC 0x0073cc40 _$E254
// FUNCTION: STBC 0x0073cc50 _$E259
// FUNCTION: STBC 0x0073cc60 _$E258
// FUNCTION: STBC 0x0073cc70 _$E265
// FUNCTION: STBC 0x0073cc80 _$E264
// FUNCTION: STBC 0x0073cc90 _$E287
// FUNCTION: STBC 0x0073cca0 _$E286
// FUNCTION: STBC 0x0073ccb0 _$E291
// FUNCTION: STBC 0x0073ccc0 _$E290
// FUNCTION: STBC 0x0073ccd0 _$E295
// FUNCTION: STBC 0x0073cce0 _$E294
// FUNCTION: STBC 0x0073ccf0 _$E299
// FUNCTION: STBC 0x0073cd00 _$E298
// FUNCTION: STBC 0x0073cd10 _$E303
// FUNCTION: STBC 0x0073cd20 _$E302
// FUNCTION: STBC 0x0073cd30 _$E307
// FUNCTION: STBC 0x0073cd40 _$E306
// FUNCTION: STBC 0x0073cd50 _$E311
// FUNCTION: STBC 0x0073cd60 _$E310
// FUNCTION: STBC 0x0073cd70 _$E315
// FUNCTION: STBC 0x0073cd80 _$E314
// FUNCTION: STBC 0x0073cd90 _$E319
// FUNCTION: STBC 0x0073cda0 _$E318
// FUNCTION: STBC 0x0073cdb0 _$E323
// FUNCTION: STBC 0x0073cdc0 _$E322
// FUNCTION: STBC 0x0073cdd0 _$E113
// FUNCTION: STBC 0x0073cde0 _$E112
// FUNCTION: STBC 0x0073cdf0 _$E327
// FUNCTION: STBC 0x0073ce00 _$E326
// FUNCTION: STBC 0x0073ce10 _$E118
// FUNCTION: STBC 0x0073ce20 _$E117
// FUNCTION: STBC 0x0073ce30 _$E331
// FUNCTION: STBC 0x0073ce40 _$E330
// FUNCTION: STBC 0x0073ce50 _$E335
// FUNCTION: STBC 0x0073ce60 _$E334
// FUNCTION: STBC 0x0073ce70 _$E339
// FUNCTION: STBC 0x0073ce80 _$E338
// FUNCTION: STBC 0x0073ce90 _$E343
// FUNCTION: STBC 0x0073cea0 _$E342
// FUNCTION: STBC 0x0073ceb0 _$E127
// FUNCTION: STBC 0x0073cec0 _$E126
// FUNCTION: STBC 0x0073ced0 _$E347
// FUNCTION: STBC 0x0073cee0 _$E346
// FUNCTION: STBC 0x0073cef0 _$E351
// FUNCTION: STBC 0x0073cf00 _$E350
// FUNCTION: STBC 0x0073cf10 _$E355
// FUNCTION: STBC 0x0073cf20 _$E354
// FUNCTION: STBC 0x0073cf30 _$E133
// FUNCTION: STBC 0x0073cf40 _$E132
// FUNCTION: STBC 0x0073cf50 _$E138
// FUNCTION: STBC 0x0073cf60 _$E137
// FUNCTION: STBC 0x0073cf70 _$E168
// FUNCTION: STBC 0x0073cf80 _$E167
// FUNCTION: STBC 0x0073cf90 _$E359
// FUNCTION: STBC 0x0073cfa0 _$E358
// FUNCTION: STBC 0x0073cfb0 _$E173
// FUNCTION: STBC 0x0073cfc0 _$E172
// FUNCTION: STBC 0x0073cfd0 _$E363
// FUNCTION: STBC 0x0073cfe0 _$E362
// FUNCTION: STBC 0x0073cff0 _$E367
// FUNCTION: STBC 0x0073d000 _$E366
// FUNCTION: STBC 0x0073d010 _$E178
// FUNCTION: STBC 0x0073d020 _$E177
// FUNCTION: STBC 0x0073d030 _$E371
// FUNCTION: STBC 0x0073d040 _$E370
// FUNCTION: STBC 0x0073d050 _$E143
// FUNCTION: STBC 0x0073d060 _$E142
// FUNCTION: STBC 0x0073d070 _$E375
// FUNCTION: STBC 0x0073d080 _$E374
// FUNCTION: STBC 0x0073d090 _$E379
// FUNCTION: STBC 0x0073d0a0 _$E378
// FUNCTION: STBC 0x0073d0b0 _$E183
// FUNCTION: STBC 0x0073d0c0 _$E182
// FUNCTION: STBC 0x0073d0d0 _$E188
// FUNCTION: STBC 0x0073d0e0 _$E187
// FUNCTION: STBC 0x0073d0f0 _$E383
// FUNCTION: STBC 0x0073d100 _$E382
// FUNCTION: STBC 0x0073d110 _$E387
// FUNCTION: STBC 0x0073d120 _$E386
// FUNCTION: STBC 0x0073d130 _$E391
// FUNCTION: STBC 0x0073d140 _$E390
// FUNCTION: STBC 0x0073d150 _$E395
// FUNCTION: STBC 0x0073d160 _$E394
// FUNCTION: STBC 0x0073d170 _$E399
// FUNCTION: STBC 0x0073d180 _$E398
// FUNCTION: STBC 0x0073d190 _$E403
// FUNCTION: STBC 0x0073d1a0 _$E402
// FUNCTION: STBC 0x0073d1b0 _$E407
// FUNCTION: STBC 0x0073d1c0 _$E406
// FUNCTION: STBC 0x0073d1d0 _$E411
// FUNCTION: STBC 0x0073d1e0 _$E410
// FUNCTION: STBC 0x0073d1f0 _$E193
// FUNCTION: STBC 0x0073d200 _$E192
// FUNCTION: STBC 0x0073d210 _$E415
// FUNCTION: STBC 0x0073d220 _$E414
// FUNCTION: STBC 0x0073d230 _$E198
// FUNCTION: STBC 0x0073d240 _$E197
// FUNCTION: STBC 0x0073d250 _$E203
// FUNCTION: STBC 0x0073d260 _$E202

/*
================
TGIcon::Cast

The object as an icon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0073d2c0
TGIcon *TGIcon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGICON_TYPE ) ) {
		return (TGIcon *)object;
	}
	return NULL;
}

/*
================
TGIcon::TGIcon

White without a colour. An icon of a group takes the icon's size and never
takes the focus.
================
*/
// FUNCTION: STBC 0x0073d2f0 ??0TGIcon@@QAE@PADHPAVNiColorA@@@Z
TGIcon::TGIcon( char *groupName, int iconNum, NiColorA *color ) {
	this->iconNum = iconNum;
	clamp = 0;
	iconPoly = NULL;
	if ( color == NULL ) {
		this->color.r = this->color.g = this->color.b = this->color.a = 1.0f;
	} else {
		this->color = *color;
	}

	if ( groupName == NULL ) {
		iconGroupName = NULL;
		return;
	}
	iconGroupName = new char[strlen( groupName ) + 1];
	strcpy( iconGroupName, groupName );
	bounds.Clear();
	SizeToArtwork( false );
	SetNoFocus();
}

/*
================
TGIcon::~TGIcon
================
*/
// FUNCTION: STBC 0x0073d400
TGIcon::~TGIcon( void ) {
	if ( iconGroupName != NULL ) {
		delete iconGroupName;
		iconGroupName = NULL;
	}
	VirtualSlot60();
}

/*
================
TGIcon::VirtualSlot60

Takes the poly out of its batch.
================
*/
// FUNCTION: STBC 0x0073d470
void TGIcon::VirtualSlot60( void ) {
	if ( iconPoly != NULL ) {
		iconPoly->batch->RemoveIconPoly( iconPoly );
		iconPoly = NULL;
	}
}

/*
================
TGIcon::VirtualSlot61

Moves the poly with the icon.
================
*/
// FUNCTION: STBC 0x0073d490
void TGIcon::VirtualSlot61( NiPoint2 *offset ) {
	if ( iconPoly != NULL ) {
		iconPoly->Move( offset );
	}
}

/*
================
TGIcon::VirtualSlot62

Clips the poly to the icon's new clip rectangle.
================
*/
// FUNCTION: STBC 0x0073d4b0
void TGIcon::VirtualSlot62( TGRect *clipRect ) {
	if ( iconPoly != NULL ) {
		iconPoly->SetClipRect( clipRect );
	}
}

/*
================
TGIcon::SizeToArtwork

Resizes the icon to its artwork's size on the screen.
================
*/
// FUNCTION: STBC 0x0073d4d0
void TGIcon::SizeToArtwork( bool layout ) {
	TGIconGroup *group = g_kIconManager.GetIconGroup( iconGroupName );
	if ( group != NULL ) {
		Resize( group->GetIconScreenWidth( iconNum ), group->GetIconScreenHeight( iconNum ), layout );
	}
}

/*
================
TGIcon::SetIconNum
================
*/
// FUNCTION: STBC 0x0073d520
void TGIcon::SetIconNum( int iconNum, bool layout ) {
	this->iconNum = iconNum;
	if ( layout ) {
		Layout();
	}
}

/*
================
TGIcon::SetColor

White without a colour; the poly takes the new colour at once.
================
*/
// FUNCTION: STBC 0x0073d540
void TGIcon::SetColor( NiColorA *color ) {
	if ( color == NULL ) {
		this->color.r = this->color.g = this->color.b = this->color.a = 1.0f;
	} else {
		this->color = *color;
	}
	if ( iconPoly != NULL ) {
		iconPoly->SetColor( &this->color );
	}
}

/*
================
TGIcon::SetIconGroupName
================
*/
// FUNCTION: STBC 0x0073d590
void TGIcon::SetIconGroupName( char *groupName, bool layout ) {
	if ( iconGroupName != NULL ) {
		delete iconGroupName;
		iconGroupName = NULL;
	}
	if ( groupName != NULL ) {
		iconGroupName = new char[strlen( groupName ) + 1];
		strcpy( iconGroupName, groupName );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGIcon::Layout

Drops the old poly and, when any of a completely visible icon is within its
clip rectangle, makes a new one, clipped when the icon is partly outside,
and moved to the icon's place on the screen.
================
*/
// FUNCTION: STBC 0x0073d610
void TGIcon::Layout( void ) {
	VirtualSlot60();
	TGUIObject::Layout();

	if ( IsCompletelyVisible() && iconGroupName != NULL ) {
		TGRect clipRect;
		GetClipRect( &clipRect );
		if ( clipRect.width == 0.0f || clipRect.height == 0.0f ) {
			return;
		}

		int iconInClip = clipRect.Unknown739F00( &bounds );
		int clipInIcon = bounds.Unknown739F00( &clipRect );
		if ( iconInClip == RECT_OUTSIDE && clipInIcon == RECT_OUTSIDE ) {
			return;
		}

		iconPoly = g_kIconManager.CreateIconPoly( GetBatches(), iconGroupName, iconNum, &bounds, &color, clamp );
		if ( iconPoly == NULL ) {
			return;
		}
		if ( iconInClip == RECT_PARTIAL || clipInIcon != RECT_OUTSIDE ) {
			iconPoly->SetClipRect( &clipRect );
		}
		if ( parent != NULL ) {
			NiPoint2 offset;
			parent->GetScreenOffset( &offset );
			iconPoly->Move( &offset );
		}
	}
}

/*
================
TGIcon::Move

TGUIObject::Move for an icon: the poly follows the icon, and an icon without
a poly lays out only when it is completely visible.
================
*/
// FUNCTION: STBC 0x0073d750
void TGIcon::Move( float dx, float dy, bool layout ) {
	TGRect parentClip;

	float alignedDx = TGUIModule::PixelAlignValue( dx );
	float alignedDy = TGUIModule::PixelAlignValue( dy, false );

	if ( !IsVisible() ) {
		bounds.left += alignedDx;
		bounds.top += alignedDy;
		return;
	}
	if ( parent == NULL ) {
		bounds.left += alignedDx;
		bounds.top += alignedDy;
		return;
	}

	parent->GetClipRect( &parentClip );
	int before = parentClip.Unknown739F00( &bounds );
	bounds.left += alignedDx;
	bounds.top += alignedDy;
	if ( before == RECT_INSIDE ) {
		if ( iconPoly != NULL ) {
			int after = parentClip.Unknown739F00( &bounds );
			NiPoint2 offset( alignedDx, alignedDy );
			VirtualSlot61( &offset );
			if ( after != RECT_INSIDE ) {
				TGRect clipRect;
				GetClipRect( &clipRect );
				parent->GetScreenOffset( &offset );
				clipRect -= offset;
				clipRect.ClipTo( &parentClip );
				VirtualSlot62( &clipRect );
			}
		} else if ( layout && IsCompletelyVisible() == true ) {
			Layout();
		}
	} else if ( layout ) {
		Layout();
	}
}

/*
================
TGIcon::SetPosition

TGUIObject::SetPosition for an icon: the poly follows the icon.
================
*/
// FUNCTION: STBC 0x0073d9a0
// PARKED: complete; as TGUIObject::SetPosition: schedule: retail loads the parent before the visibility test's branch (mov/test of the flags byte instead of test byte [mem])
void TGIcon::SetPosition( float x, float y, bool layout ) {
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
	if ( before == RECT_INSIDE && iconPoly != NULL ) {
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
TGIcon::WriteToStream
================
*/
// FUNCTION: STBC 0x0073dbd0
bool TGIcon::WriteToStream( TGStream *stream ) {
	if ( !TGUIObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( iconGroupName );
	stream->WriteLong( iconNum );
	stream->WriteLong( clamp );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->WriteFloat( color.a );
	return true;
}

/*
================
TGIcon::ReadFromStream

The poly is made again by the next layout.
================
*/
// FUNCTION: STBC 0x0073dc40
bool TGIcon::ReadFromStream( TGStream *stream ) {
	if ( !TGUIObject::ReadFromStream( stream ) ) {
		return false;
	}
	iconPoly = NULL;
	iconGroupName = stream->VirtualSlot10( -1 );
	iconNum = stream->ReadLong();
	clamp = stream->ReadLong();
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	color.a = stream->ReadFloat();
	return true;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0073d270 _$E421
// FUNCTION: STBC 0x0073d280 _$E417
// FUNCTION: STBC 0x0073d290 _$E420
// FUNCTION: STBC 0x0073d2a0 _$E418
// DATA: STBC 0x0099e050
TGEventHandlerTable TGIcon::classHandlers;
