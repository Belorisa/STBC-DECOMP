/*
===============================================================================

	STTiledIcon.cpp

===============================================================================
*/

#include <iostream>
#include "STTiledIcon.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGIconPoly.h"
#include "TGRect.h"
#include "TGStream.h"
#include "TGUIBatch.h"
#include "NiPoint2.h"
#include "NiTriShape.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0053abb0 _$E57
// FUNCTION: STBC 0x0053abc0 _$E56
// FUNCTION: STBC 0x0053abd0 _$E61
// FUNCTION: STBC 0x0053abe0 _$E60
// FUNCTION: STBC 0x0053abf0 _$E72
// FUNCTION: STBC 0x0053ac00 _$E71
// FUNCTION: STBC 0x0053ac10 _$E76
// FUNCTION: STBC 0x0053ac20 _$E75
// FUNCTION: STBC 0x0053ac30 _$E80
// FUNCTION: STBC 0x0053ac40 _$E79
// FUNCTION: STBC 0x0053ac50 _$E84
// FUNCTION: STBC 0x0053ac60 _$E83
// FUNCTION: STBC 0x0053ac70 _$E208
// FUNCTION: STBC 0x0053ac80 _$E207
// FUNCTION: STBC 0x0053ac90 _$E150
// FUNCTION: STBC 0x0053aca0 _$E149
// FUNCTION: STBC 0x0053acb0 _$E163
// FUNCTION: STBC 0x0053acc0 _$E162
// FUNCTION: STBC 0x0053acd0 _$E213
// FUNCTION: STBC 0x0053ace0 _$E212
// FUNCTION: STBC 0x0053acf0 _$E218
// FUNCTION: STBC 0x0053ad00 _$E217
// FUNCTION: STBC 0x0053ad10 _$E222
// FUNCTION: STBC 0x0053ad20 _$E221
// FUNCTION: STBC 0x0053ad30 _$E226
// FUNCTION: STBC 0x0053ad40 _$E225
// FUNCTION: STBC 0x0053ad50 _$E233
// FUNCTION: STBC 0x0053ad60 _$E232
// FUNCTION: STBC 0x0053ad70 _$E276
// FUNCTION: STBC 0x0053ad80 _$E275
// FUNCTION: STBC 0x0053ad90 _$E280
// FUNCTION: STBC 0x0053ada0 _$E279
// FUNCTION: STBC 0x0053adb0 _$E108
// FUNCTION: STBC 0x0053adc0 _$E107
// FUNCTION: STBC 0x0053add0 _$E241
// FUNCTION: STBC 0x0053ade0 _$E240
// FUNCTION: STBC 0x0053adf0 _$E248
// FUNCTION: STBC 0x0053ae00 _$E247
// FUNCTION: STBC 0x0053ae10 _$E252
// FUNCTION: STBC 0x0053ae20 _$E251
// FUNCTION: STBC 0x0053ae30 _$E256
// FUNCTION: STBC 0x0053ae40 _$E255
// FUNCTION: STBC 0x0053ae50 _$E260
// FUNCTION: STBC 0x0053ae60 _$E259
// FUNCTION: STBC 0x0053ae70 _$E266
// FUNCTION: STBC 0x0053ae80 _$E265
// FUNCTION: STBC 0x0053ae90 _$E284
// FUNCTION: STBC 0x0053aea0 _$E283
// FUNCTION: STBC 0x0053aeb0 _$E288
// FUNCTION: STBC 0x0053aec0 _$E287
// FUNCTION: STBC 0x0053aed0 _$E292
// FUNCTION: STBC 0x0053aee0 _$E291
// FUNCTION: STBC 0x0053aef0 _$E296
// FUNCTION: STBC 0x0053af00 _$E295
// FUNCTION: STBC 0x0053af10 _$E300
// FUNCTION: STBC 0x0053af20 _$E299
// FUNCTION: STBC 0x0053af60 _$E304
// FUNCTION: STBC 0x0053af70 _$E303
// FUNCTION: STBC 0x0053af80 _$E308
// FUNCTION: STBC 0x0053af90 _$E307
// FUNCTION: STBC 0x0053afa0 _$E312
// FUNCTION: STBC 0x0053afb0 _$E311
// FUNCTION: STBC 0x0053afc0 _$E316
// FUNCTION: STBC 0x0053afd0 _$E315
// FUNCTION: STBC 0x0053afe0 _$E320
// FUNCTION: STBC 0x0053aff0 _$E319
// FUNCTION: STBC 0x0053b000 _$E113
// FUNCTION: STBC 0x0053b010 _$E112
// FUNCTION: STBC 0x0053b020 _$E324
// FUNCTION: STBC 0x0053b030 _$E323
// FUNCTION: STBC 0x0053b040 _$E118
// FUNCTION: STBC 0x0053b050 _$E117
// FUNCTION: STBC 0x0053b060 _$E328
// FUNCTION: STBC 0x0053b070 _$E327
// FUNCTION: STBC 0x0053b080 _$E332
// FUNCTION: STBC 0x0053b090 _$E331
// FUNCTION: STBC 0x0053b0a0 _$E336
// FUNCTION: STBC 0x0053b0b0 _$E335
// FUNCTION: STBC 0x0053b0c0 _$E340
// FUNCTION: STBC 0x0053b0d0 _$E339
// FUNCTION: STBC 0x0053b0e0 _$E127
// FUNCTION: STBC 0x0053b0f0 _$E126
// FUNCTION: STBC 0x0053b100 _$E344
// FUNCTION: STBC 0x0053b110 _$E343
// FUNCTION: STBC 0x0053b120 _$E348
// FUNCTION: STBC 0x0053b130 _$E347
// FUNCTION: STBC 0x0053b140 _$E352
// FUNCTION: STBC 0x0053b150 _$E351
// FUNCTION: STBC 0x0053b160 _$E133
// FUNCTION: STBC 0x0053b170 _$E132
// FUNCTION: STBC 0x0053b180 _$E138
// FUNCTION: STBC 0x0053b190 _$E137
// FUNCTION: STBC 0x0053b1a0 _$E168
// FUNCTION: STBC 0x0053b1b0 _$E167
// FUNCTION: STBC 0x0053b1c0 _$E356
// FUNCTION: STBC 0x0053b1d0 _$E355
// FUNCTION: STBC 0x0053b1e0 _$E173
// FUNCTION: STBC 0x0053b1f0 _$E172
// FUNCTION: STBC 0x0053b200 _$E360
// FUNCTION: STBC 0x0053b210 _$E359
// FUNCTION: STBC 0x0053b220 _$E364
// FUNCTION: STBC 0x0053b230 _$E363
// FUNCTION: STBC 0x0053b240 _$E178
// FUNCTION: STBC 0x0053b250 _$E177
// FUNCTION: STBC 0x0053b260 _$E368
// FUNCTION: STBC 0x0053b270 _$E367
// FUNCTION: STBC 0x0053b280 _$E143
// FUNCTION: STBC 0x0053b290 _$E142
// FUNCTION: STBC 0x0053b2a0 _$E372
// FUNCTION: STBC 0x0053b2b0 _$E371
// FUNCTION: STBC 0x0053b2c0 _$E376
// FUNCTION: STBC 0x0053b2d0 _$E375
// FUNCTION: STBC 0x0053b2e0 _$E183
// FUNCTION: STBC 0x0053b2f0 _$E182
// FUNCTION: STBC 0x0053b300 _$E188
// FUNCTION: STBC 0x0053b310 _$E187
// FUNCTION: STBC 0x0053b320 _$E380
// FUNCTION: STBC 0x0053b330 _$E379
// FUNCTION: STBC 0x0053b340 _$E384
// FUNCTION: STBC 0x0053b350 _$E383
// FUNCTION: STBC 0x0053b360 _$E388
// FUNCTION: STBC 0x0053b370 _$E387
// FUNCTION: STBC 0x0053b380 _$E392
// FUNCTION: STBC 0x0053b390 _$E391
// FUNCTION: STBC 0x0053b3a0 _$E396
// FUNCTION: STBC 0x0053b3b0 _$E395
// FUNCTION: STBC 0x0053b3c0 _$E400
// FUNCTION: STBC 0x0053b3d0 _$E399
// FUNCTION: STBC 0x0053b3e0 _$E404
// FUNCTION: STBC 0x0053b3f0 _$E403
// FUNCTION: STBC 0x0053b400 _$E408
// FUNCTION: STBC 0x0053b410 _$E407
// FUNCTION: STBC 0x0053b420 _$E193
// FUNCTION: STBC 0x0053b430 _$E192
// FUNCTION: STBC 0x0053b440 _$E412
// FUNCTION: STBC 0x0053b450 _$E411
// FUNCTION: STBC 0x0053b460 _$E198
// FUNCTION: STBC 0x0053b470 _$E197
// FUNCTION: STBC 0x0053b480 _$E203
// FUNCTION: STBC 0x0053b490 _$E202
// FUNCTION: STBC 0x0053b4f0 _$E416
// FUNCTION: STBC 0x0053b500 _$E415
// FUNCTION: STBC 0x0053b510 _$E421
// FUNCTION: STBC 0x0053b520 _$E420
// FUNCTION: STBC 0x0053b530 _$E425
// FUNCTION: STBC 0x0053b540 _$E424

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0053ab30 _$E32
// FUNCTION: STBC 0x0053ab40 _$E29
// FUNCTION: STBC 0x0053ab50 _$E31
// FUNCTION: STBC 0x0053ab60 _$E30
// FUNCTION: STBC 0x0053ab70 _$E38
// FUNCTION: STBC 0x0053ab80 _$E35
// FUNCTION: STBC 0x0053ab90 _$E37
// FUNCTION: STBC 0x0053aba0 _$E36

/*
================
STTiledIcon::Cast

The object as a tiled icon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0053a6d0
STTiledIcon *STTiledIcon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STTILEDICON_TYPE ) ) {
		return (STTiledIcon *)object;
	}
	return NULL;
}

/*
================
STTiledIcon::STTiledIcon

Stretches on both axes until told to tile.
================
*/
// FUNCTION: STBC 0x0053a700 ??0STTiledIcon@@QAE@PADHPAVNiColorA@@@Z
STTiledIcon::STTiledIcon( char *groupName, int iconNum, NiColorA *color ) : TGIcon( groupName, iconNum, color ) {
	tileNumX = 1.0f;
	tileNumY = 1.0f;
}

/*
================
STTiledIcon::~STTiledIcon
================
*/
// FUNCTION: STBC 0x0053a800
STTiledIcon::~STTiledIcon( void ) {
}

/*
================
STTiledIcon::IsTiling
================
*/
// FUNCTION: STBC 0x0053a810
// PARKED: retail tests bit 31 with test/setne where this build shifts it down (!= 0, == mask, !!, ?: and a signed compare tried); the vertical branch matches
bool STTiledIcon::IsTiling( int axis ) {
	if ( axis == 0 ) {
		return flags & STTIF_TILING_X ? true : false;
	}
	return ( flags >> 30 ) & 1 ? true : false;
}

/*
================
STTiledIcon::IsTileSize
================
*/
// FUNCTION: STBC 0x0053a830
bool STTiledIcon::IsTileSize( int axis ) {
	if ( axis == 0 ) {
		return ( flags >> 29 ) & 1 ? true : false;
	}
	return ( flags >> 28 ) & 1 ? true : false;
}

/*
================
STTiledIcon::SetTiling
================
*/
// FUNCTION: STBC 0x0053a850
void STTiledIcon::SetTiling( int axis, bool tiling ) {
	if ( axis == 0 ) {
		if ( tiling == true ) {
			flags |= STTIF_TILING_X;
		} else {
			flags &= ~STTIF_TILING_X;
		}
	} else {
		if ( tiling == true ) {
			flags |= STTIF_TILING_Y;
		} else {
			flags &= ~STTIF_TILING_Y;
		}
	}
}

/*
================
STTiledIcon::SetTileSize
================
*/
// FUNCTION: STBC 0x0053a8a0
void STTiledIcon::SetTileSize( int axis, bool tileSize ) {
	if ( axis == 0 ) {
		if ( tileSize == true ) {
			flags |= STTIF_TILE_SIZE_X;
		} else {
			flags &= ~STTIF_TILE_SIZE_X;
		}
	} else {
		if ( tileSize == true ) {
			flags |= STTIF_TILE_SIZE_Y;
		} else {
			flags &= ~STTIF_TILE_SIZE_Y;
		}
	}
}

/*
================
STTiledIcon::SetTileNum
================
*/
// FUNCTION: STBC 0x0053a8f0
void STTiledIcon::SetTileNum( int axis, float tileNum ) {
	if ( axis == 0 ) {
		tileNumX = tileNum;
	} else {
		tileNumY = tileNum;
	}
}

/*
================
STTiledIcon::GetTileNum
================
*/
// FUNCTION: STBC 0x0053a910
float STTiledIcon::GetTileNum( int axis ) {
	if ( axis == 0 ) {
		return tileNumX;
	}
	return tileNumY;
}

/*
================
STTiledIcon::Layout

Lays the icon out as any icon, then widens the poly's texture
coordinates on each tiling axis: by the icon's size over the art's with
tile size, to the tile number without.
================
*/
// FUNCTION: STBC 0x0053a930
// PARKED: register residue: ESI and EDI swapped (this and the texture coordinates), and the bounds copy alternates its registers the other way; calls, CFG and operands match (GetBounds and a direct copy tried)
void STTiledIcon::Layout( void ) {
	TGIcon::Layout();
	TGIconPoly *poly = iconPoly;
	if ( poly != NULL ) {
		NiPoint2 *coords = poly->GetTextureCoords();
		TGRect rect;
		TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( iconGroupName );
		GetBounds( &rect );
		if ( IsTiling( 0 ) == true ) {
			if ( IsTileSize( 0 ) == true ) {
				float iconWidth = iconGroup->GetIconScreenWidth( iconNum );
				float repeatU = ( rect.GetRight() - rect.GetLeft() ) / iconWidth;
				float sizeU = coords[2].x - coords[0].x;
				// The repeat less the span, times the span: what the right edge moves by.
				float extraU = ( repeatU - sizeU ) * sizeU;
				coords[2].x += extraU;
				coords[3].x += extraU;
			} else {
				coords[2].x = tileNumX;
				coords[3].x = tileNumX;
			}
		}
		if ( IsTiling( 1 ) == true ) {
			if ( IsTileSize( 1 ) == true ) {
				float iconHeight = iconGroup->GetIconScreenHeight( iconNum );
				float repeatV = ( rect.GetBottom() - rect.GetTop() ) / iconHeight;
				float sizeV = coords[2].y - coords[0].y;
				float extraV = ( repeatV - sizeV ) * sizeV;
				coords[1].y += extraV;
				coords[2].y += extraV;
			} else {
				coords[1].y = tileNumY;
				coords[2].y = tileNumY;
			}
		}
		poly->batch->shape->GetShapeData()->IncRevisionID();
	}
}

/*
================
STTiledIcon::WriteToStream
================
*/
// FUNCTION: STBC 0x0053aab0
bool STTiledIcon::WriteToStream( TGStream *stream ) {
	if ( !TGIcon::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( tileNumX );
	stream->WriteFloat( tileNumY );
	return true;
}

/*
================
STTiledIcon::ReadFromStream
================
*/
// FUNCTION: STBC 0x0053aaf0
bool STTiledIcon::ReadFromStream( TGStream *stream ) {
	if ( !TGIcon::ReadFromStream( stream ) ) {
		return false;
	}
	tileNumX = stream->ReadFloat();
	tileNumY = stream->ReadFloat();
	return true;
}
