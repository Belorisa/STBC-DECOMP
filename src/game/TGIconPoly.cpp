/*
===============================================================================

	TGIconPoly.cpp

	One icon drawn as a quad of a UI batch.

===============================================================================
*/

#include <iostream>
#include "TGIconPoly.h"
#include "NiColor.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiTriShape.h"
#include "TGIconManager.h"
#include "TGUIBatch.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00740c70 _$E408
// FUNCTION: STBC 0x00740c90 _$E407

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0073fd50 _$E32
// FUNCTION: STBC 0x0073fd60 _$E29
// FUNCTION: STBC 0x0073fd70 _$E31
// FUNCTION: STBC 0x0073fd80 _$E30
// FUNCTION: STBC 0x0073fd90 _$E38
// FUNCTION: STBC 0x0073fda0 _$E35
// FUNCTION: STBC 0x0073fdb0 _$E37
// FUNCTION: STBC 0x0073fdc0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0073fdd0 _$E48
// FUNCTION: STBC 0x0073fde0 _$E47
// FUNCTION: STBC 0x0073fdf0 _$E229
// FUNCTION: STBC 0x0073fe00 _$E228
// FUNCTION: STBC 0x0073fe10 _$E59
// FUNCTION: STBC 0x0073fe20 _$E58
// FUNCTION: STBC 0x0073fe30 _$E63
// FUNCTION: STBC 0x0073fe40 _$E62
// FUNCTION: STBC 0x0073fe50 _$E67
// FUNCTION: STBC 0x0073fe60 _$E66
// FUNCTION: STBC 0x0073fe70 _$E71
// FUNCTION: STBC 0x0073fe80 _$E70
// FUNCTION: STBC 0x0073fe90 _$E195
// FUNCTION: STBC 0x0073fea0 _$E194
// FUNCTION: STBC 0x0073feb0 _$E137
// FUNCTION: STBC 0x0073fec0 _$E136
// FUNCTION: STBC 0x0073fed0 _$E150
// FUNCTION: STBC 0x0073fee0 _$E149
// FUNCTION: STBC 0x0073fef0 _$E234
// FUNCTION: STBC 0x0073ff00 _$E233
// FUNCTION: STBC 0x0073ff10 _$E239
// FUNCTION: STBC 0x0073ff20 _$E238
// FUNCTION: STBC 0x0073ff30 _$E243
// FUNCTION: STBC 0x0073ff40 _$E242
// FUNCTION: STBC 0x0073ff50 _$E247
// FUNCTION: STBC 0x0073ff60 _$E246
// FUNCTION: STBC 0x0073ff70 _$E254
// FUNCTION: STBC 0x0073ff80 _$E253
// FUNCTION: STBC 0x0073ff90 _$E264
// FUNCTION: STBC 0x0073ffa0 _$E263
// FUNCTION: STBC 0x0073ffb0 _$E268
// FUNCTION: STBC 0x0073ffc0 _$E267
// FUNCTION: STBC 0x0073ffd0 _$E95
// FUNCTION: STBC 0x0073ffe0 _$E94
// FUNCTION: STBC 0x0073fff0 _$E200
// FUNCTION: STBC 0x00740000 _$E199
// FUNCTION: STBC 0x00740010 _$E207
// FUNCTION: STBC 0x00740020 _$E206
// FUNCTION: STBC 0x00740030 _$E211
// FUNCTION: STBC 0x00740040 _$E210
// FUNCTION: STBC 0x00740050 _$E215
// FUNCTION: STBC 0x00740060 _$E214
// FUNCTION: STBC 0x00740070 _$E219
// FUNCTION: STBC 0x00740080 _$E218
// FUNCTION: STBC 0x00740090 _$E225
// FUNCTION: STBC 0x007400a0 _$E224
// FUNCTION: STBC 0x007400b0 _$E272
// FUNCTION: STBC 0x007400c0 _$E271
// FUNCTION: STBC 0x007400d0 _$E276
// FUNCTION: STBC 0x007400e0 _$E275
// FUNCTION: STBC 0x007400f0 _$E280
// FUNCTION: STBC 0x00740100 _$E279
// FUNCTION: STBC 0x00740110 _$E284
// FUNCTION: STBC 0x00740120 _$E283
// FUNCTION: STBC 0x00740130 _$E288
// FUNCTION: STBC 0x00740140 _$E287
// FUNCTION: STBC 0x00740150 _$E292
// FUNCTION: STBC 0x00740160 _$E291
// FUNCTION: STBC 0x00740170 _$E296
// FUNCTION: STBC 0x00740180 _$E295
// FUNCTION: STBC 0x00740190 _$E300
// FUNCTION: STBC 0x007401a0 _$E299
// FUNCTION: STBC 0x007401b0 _$E304
// FUNCTION: STBC 0x007401c0 _$E303
// FUNCTION: STBC 0x007401d0 _$E308
// FUNCTION: STBC 0x007401e0 _$E307
// FUNCTION: STBC 0x007401f0 _$E100
// FUNCTION: STBC 0x00740200 _$E99
// FUNCTION: STBC 0x00740210 _$E312
// FUNCTION: STBC 0x00740220 _$E311
// FUNCTION: STBC 0x00740230 _$E105
// FUNCTION: STBC 0x00740240 _$E104
// FUNCTION: STBC 0x00740250 _$E316
// FUNCTION: STBC 0x00740260 _$E315
// FUNCTION: STBC 0x00740270 _$E320
// FUNCTION: STBC 0x00740280 _$E319
// FUNCTION: STBC 0x00740290 _$E324
// FUNCTION: STBC 0x007402a0 _$E323
// FUNCTION: STBC 0x007402b0 _$E328
// FUNCTION: STBC 0x007402c0 _$E327
// FUNCTION: STBC 0x007402d0 _$E114
// FUNCTION: STBC 0x007402e0 _$E113
// FUNCTION: STBC 0x007402f0 _$E332
// FUNCTION: STBC 0x00740300 _$E331
// FUNCTION: STBC 0x00740310 _$E336
// FUNCTION: STBC 0x00740320 _$E335
// FUNCTION: STBC 0x00740330 _$E340
// FUNCTION: STBC 0x00740340 _$E339
// FUNCTION: STBC 0x00740350 _$E120
// FUNCTION: STBC 0x00740360 _$E119
// FUNCTION: STBC 0x00740370 _$E125
// FUNCTION: STBC 0x00740380 _$E124
// FUNCTION: STBC 0x00740390 _$E155
// FUNCTION: STBC 0x007403a0 _$E154
// FUNCTION: STBC 0x007403b0 _$E344
// FUNCTION: STBC 0x007403c0 _$E343
// FUNCTION: STBC 0x007403d0 _$E160
// FUNCTION: STBC 0x007403e0 _$E159
// FUNCTION: STBC 0x007403f0 _$E348
// FUNCTION: STBC 0x00740400 _$E347
// FUNCTION: STBC 0x00740410 _$E352
// FUNCTION: STBC 0x00740420 _$E351
// FUNCTION: STBC 0x00740430 _$E165
// FUNCTION: STBC 0x00740440 _$E164
// FUNCTION: STBC 0x00740450 _$E356
// FUNCTION: STBC 0x00740460 _$E355
// FUNCTION: STBC 0x00740470 _$E130
// FUNCTION: STBC 0x00740480 _$E129
// FUNCTION: STBC 0x00740490 _$E360
// FUNCTION: STBC 0x007404a0 _$E359
// FUNCTION: STBC 0x007404b0 _$E364
// FUNCTION: STBC 0x007404c0 _$E363
// FUNCTION: STBC 0x007404d0 _$E170
// FUNCTION: STBC 0x007404e0 _$E169
// FUNCTION: STBC 0x007404f0 _$E175
// FUNCTION: STBC 0x00740500 _$E174
// FUNCTION: STBC 0x00740510 _$E368
// FUNCTION: STBC 0x00740520 _$E367
// FUNCTION: STBC 0x00740530 _$E372
// FUNCTION: STBC 0x00740540 _$E371
// FUNCTION: STBC 0x00740550 _$E376
// FUNCTION: STBC 0x00740560 _$E375
// FUNCTION: STBC 0x00740570 _$E380
// FUNCTION: STBC 0x00740580 _$E379
// FUNCTION: STBC 0x00740590 _$E384
// FUNCTION: STBC 0x007405a0 _$E383
// FUNCTION: STBC 0x007405b0 _$E388
// FUNCTION: STBC 0x007405c0 _$E387
// FUNCTION: STBC 0x007405d0 _$E392
// FUNCTION: STBC 0x007405e0 _$E391
// FUNCTION: STBC 0x007405f0 _$E396
// FUNCTION: STBC 0x00740600 _$E395
// FUNCTION: STBC 0x00740610 _$E180
// FUNCTION: STBC 0x00740620 _$E179
// FUNCTION: STBC 0x00740630 _$E400
// FUNCTION: STBC 0x00740640 _$E399
// FUNCTION: STBC 0x00740650 _$E185
// FUNCTION: STBC 0x00740660 _$E184
// FUNCTION: STBC 0x00740670 _$E190
// FUNCTION: STBC 0x00740680 _$E189

/*
================
TGIconPoly::SetClipRect

Cuts the quad's texture coordinates to the clip rectangle: where the
rectangle lies beyond a clip edge, the coordinates on that side move in by the
fraction of the rectangle cut off, along the texture's horizontal or vertical
extent. Then the rectangle itself is clipped and the vertices rewritten.
================
*/
// FUNCTION: STBC 0x00740690
// PARKED: call set and code order match; the frame keeps size above vertical where retail has it below (framemap: slots swapped), and declaring size elsewhere or as two floats moves code or copies the size with integer moves
void TGIconPoly::SetClipRect( TGRect *clipRect ) {
	NiPoint2 size( rect.GetWidth(), rect.GetHeight() );
	unsigned short vertex = index << 2;
	NiPoint2 *textures = batch->shape->modelData->GetTextures();
	NiPoint2 horizontal = textures[vertex + 2] - textures[vertex + 1];
	NiPoint2 vertical = textures[vertex + 1] - textures[vertex];

	if ( rect.GetLeft() < clipRect->GetLeft() - 0.00001f ) {
		float cut = ( clipRect->GetLeft() - rect.GetLeft() ) / size.x;
		textures[vertex] += horizontal * cut;
		textures[vertex + 1] += horizontal * cut;
	}
	if ( rect.GetRight() > clipRect->GetRight() + 0.00001f ) {
		float cut = ( rect.GetRight() - clipRect->GetRight() ) / size.x;
		textures[vertex + 2] -= horizontal * cut;
		textures[vertex + 3] -= horizontal * cut;
	}
	if ( rect.GetTop() < clipRect->GetTop() - 0.00001f ) {
		float cut = ( clipRect->GetTop() - rect.GetTop() ) / size.y;
		textures[vertex] += vertical * cut;
		textures[vertex + 3] += vertical * cut;
	}
	if ( rect.GetBottom() > clipRect->GetBottom() + 0.00001f ) {
		float cut = ( rect.GetBottom() - clipRect->GetBottom() ) / size.y;
		textures[vertex + 1] -= vertical * cut;
		textures[vertex + 2] -= vertical * cut;
	}

	rect.ClipTo( clipRect );
	UpdateVertices();
}

/*
================
TGIconPoly::Move

Moves the rectangle by the offset and rewrites the quad.
================
*/
// FUNCTION: STBC 0x00740A10
void TGIconPoly::Move( NiPoint2 *offset ) {
	rect.left = offset->x + rect.left;
	rect.top = offset->y + rect.top;
	UpdateVertices();
}

/*
================
TGIconPoly::UpdateVertices

Writes the rectangle, pixel centre adjusted, into the quad's four vertices:
top left, bottom left, bottom right and top right.
================
*/
// FUNCTION: STBC 0x00740A30
// PARKED: 94.3; retail copies the rectangle's height through the FPU (fld/fstp to a frame slot): one more copy level gives that pair, but its no-code node then moves the first fadd and the slots (84.6); where the level sits is not found
void TGIconPoly::UpdateVertices( void ) {
	NiGeometryData *data = batch->shape->modelData;
	unsigned short vertex = index << 2;
	float left = rect.GetLeft() + g_kIconManager.screenCenterAdjX;
	float top = rect.GetTop() + g_kIconManager.screenCenterAdjY;
	float width = rect.GetWidth();
	float height = rect.GetHeight();
	NiPoint3 *vertices = data->GetVertices();

	vertices[vertex + 1].x = left;
	vertices[vertex].x = left;
	left += width;
	vertices[vertex + 3].x = left;
	vertices[vertex + 2].x = left;
	vertices[vertex].y = vertices[vertex + 3].y = 1.0f - top;
	top += height;
	vertices[vertex + 1].y = vertices[vertex + 2].y = 1.0f - top;
	vertices[vertex + 3].z = 1.0f;
	vertices[vertex + 2].z = 1.0f;
	vertices[vertex + 1].z = 1.0f;
	vertices[vertex].z = 1.0f;
	data->IncRevisionID();
}

/*
================
TGIconPoly::SetColor

Gives the quad's four vertices the colour.
================
*/
// FUNCTION: STBC 0x00740AE0
void TGIconPoly::SetColor( NiColorA *color ) {
	unsigned short vertex = index << 2;
	NiGeometryData *data = batch->shape->modelData;
	NiColorA *colors = data->GetColors();

	colors[vertex++] = *color;
	colors[vertex++] = *color;
	colors[vertex++] = *color;
	colors[vertex++] = *color;
	data->IncRevisionID();
}

/*
================
TGIconPoly::SetTextureCoords

Copies the four texture coordinates into the quad's.
================
*/
// FUNCTION: STBC 0x00740BB0
// DRAFT: the copy loop is retail's; retail reads the index before the batch and keeps the batch's data in edx with the counter in esi (ours: esi and edx)
void TGIconPoly::SetTextureCoords( NiPoint2 *coords ) {
	unsigned short vertex = index;
	vertex <<= 2;
	NiGeometryData *data = batch->shape->modelData;
	NiPoint2 *textures = data->GetTextures() + vertex;
	for ( int i = 0; i < 4; i++ ) {
		textures->x = coords->x;
		textures->y = coords->y;
		textures++;
		coords++;
	}
	data->IncRevisionID();
}

/*
================
TGIconPoly::GetVertices

The quad's four vertices.
================
*/
// FUNCTION: STBC 0x00740C10
NiPoint3 *TGIconPoly::GetVertices( void ) {
	return batch->shape->modelData->GetVertices() + index * 4;
}

/*
================
TGIconPoly::GetTextureCoords

The quad's four texture coordinates.
================
*/
// FUNCTION: STBC 0x00740C30
NiPoint2 *TGIconPoly::GetTextureCoords( void ) {
	return batch->shape->modelData->GetTextures() + index * 4;
}
