/*
===============================================================================

	TGRect.cpp

===============================================================================
*/

#include <iostream>
#include "TGRect.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00739470 _$E32
// FUNCTION: STBC 0x00739480 _$E29
// FUNCTION: STBC 0x00739490 _$E31
// FUNCTION: STBC 0x007394a0 _$E30
// FUNCTION: STBC 0x007394b0 _$E38
// FUNCTION: STBC 0x007394c0 _$E35
// FUNCTION: STBC 0x007394d0 _$E37
// FUNCTION: STBC 0x007394e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007394f0 _$E48
// FUNCTION: STBC 0x00739500 _$E47
// FUNCTION: STBC 0x00739510 _$E52
// FUNCTION: STBC 0x00739520 _$E51
// FUNCTION: STBC 0x00739530 _$E63
// FUNCTION: STBC 0x00739540 _$E62
// FUNCTION: STBC 0x00739550 _$E67
// FUNCTION: STBC 0x00739560 _$E66
// FUNCTION: STBC 0x00739570 _$E71
// FUNCTION: STBC 0x00739580 _$E70
// FUNCTION: STBC 0x00739590 _$E75
// FUNCTION: STBC 0x007395a0 _$E74
// FUNCTION: STBC 0x007395b0 _$E199
// FUNCTION: STBC 0x007395c0 _$E198
// FUNCTION: STBC 0x007395d0 _$E141
// FUNCTION: STBC 0x007395e0 _$E140
// FUNCTION: STBC 0x007395f0 _$E154
// FUNCTION: STBC 0x00739600 _$E153
// FUNCTION: STBC 0x00739610 _$E204
// FUNCTION: STBC 0x00739620 _$E203
// FUNCTION: STBC 0x00739630 _$E209
// FUNCTION: STBC 0x00739640 _$E208
// FUNCTION: STBC 0x00739650 _$E213
// FUNCTION: STBC 0x00739660 _$E212
// FUNCTION: STBC 0x00739670 _$E217
// FUNCTION: STBC 0x00739680 _$E216
// FUNCTION: STBC 0x00739690 _$E224
// FUNCTION: STBC 0x007396a0 _$E223
// FUNCTION: STBC 0x007396b0 _$E246
// FUNCTION: STBC 0x007396c0 _$E245
// FUNCTION: STBC 0x007396d0 _$E250
// FUNCTION: STBC 0x007396e0 _$E249
// FUNCTION: STBC 0x007396f0 _$E99
// FUNCTION: STBC 0x00739700 _$E98
// FUNCTION: STBC 0x00739710 _$E232
// FUNCTION: STBC 0x00739720 _$E231
// FUNCTION: STBC 0x00739730 _$E239
// FUNCTION: STBC 0x00739740 _$E238
// FUNCTION: STBC 0x00739750 _$E262
// FUNCTION: STBC 0x00739760 _$E261
// FUNCTION: STBC 0x00739770 _$E266
// FUNCTION: STBC 0x00739780 _$E265
// FUNCTION: STBC 0x00739790 _$E306
// FUNCTION: STBC 0x007397a0 _$E305
// FUNCTION: STBC 0x007397b0 _$E312
// FUNCTION: STBC 0x007397c0 _$E311
// FUNCTION: STBC 0x007397d0 _$E254
// FUNCTION: STBC 0x007397e0 _$E253
// FUNCTION: STBC 0x007397f0 _$E258
// FUNCTION: STBC 0x00739800 _$E257
// FUNCTION: STBC 0x00739810 _$E270
// FUNCTION: STBC 0x00739820 _$E269
// FUNCTION: STBC 0x00739830 _$E274
// FUNCTION: STBC 0x00739840 _$E273
// FUNCTION: STBC 0x00739850 _$E278
// FUNCTION: STBC 0x00739860 _$E277
// FUNCTION: STBC 0x00739870 _$E282
// FUNCTION: STBC 0x00739880 _$E281
// FUNCTION: STBC 0x00739890 _$E286
// FUNCTION: STBC 0x007398a0 _$E285
// FUNCTION: STBC 0x007398b0 _$E290
// FUNCTION: STBC 0x007398c0 _$E289
// FUNCTION: STBC 0x007398d0 _$E294
// FUNCTION: STBC 0x007398e0 _$E293
// FUNCTION: STBC 0x007398f0 _$E298
// FUNCTION: STBC 0x00739900 _$E297
// FUNCTION: STBC 0x00739910 _$E104
// FUNCTION: STBC 0x00739920 _$E103
// FUNCTION: STBC 0x00739930 _$E302
// FUNCTION: STBC 0x00739940 _$E301
// FUNCTION: STBC 0x00739950 _$E109
// FUNCTION: STBC 0x00739960 _$E108
// FUNCTION: STBC 0x00739970 _$E316
// FUNCTION: STBC 0x00739980 _$E315
// FUNCTION: STBC 0x00739990 _$E320
// FUNCTION: STBC 0x007399a0 _$E319
// FUNCTION: STBC 0x007399b0 _$E324
// FUNCTION: STBC 0x007399c0 _$E323
// FUNCTION: STBC 0x007399d0 _$E328
// FUNCTION: STBC 0x007399e0 _$E327
// FUNCTION: STBC 0x007399f0 _$E118
// FUNCTION: STBC 0x00739a00 _$E117
// FUNCTION: STBC 0x00739a10 _$E332
// FUNCTION: STBC 0x00739a20 _$E331
// FUNCTION: STBC 0x00739a30 _$E336
// FUNCTION: STBC 0x00739a40 _$E335
// FUNCTION: STBC 0x00739a50 _$E340
// FUNCTION: STBC 0x00739a60 _$E339
// FUNCTION: STBC 0x00739a70 _$E124
// FUNCTION: STBC 0x00739a80 _$E123
// FUNCTION: STBC 0x00739a90 _$E129
// FUNCTION: STBC 0x00739aa0 _$E128
// FUNCTION: STBC 0x00739ab0 _$E159
// FUNCTION: STBC 0x00739ac0 _$E158
// FUNCTION: STBC 0x00739ad0 _$E344
// FUNCTION: STBC 0x00739ae0 _$E343
// FUNCTION: STBC 0x00739af0 _$E164
// FUNCTION: STBC 0x00739b00 _$E163
// FUNCTION: STBC 0x00739b10 _$E348
// FUNCTION: STBC 0x00739b20 _$E347
// FUNCTION: STBC 0x00739b30 _$E352
// FUNCTION: STBC 0x00739b40 _$E351
// FUNCTION: STBC 0x00739b50 _$E169
// FUNCTION: STBC 0x00739b60 _$E168
// FUNCTION: STBC 0x00739b70 _$E356
// FUNCTION: STBC 0x00739b80 _$E355
// FUNCTION: STBC 0x00739b90 _$E134
// FUNCTION: STBC 0x00739ba0 _$E133
// FUNCTION: STBC 0x00739bb0 _$E360
// FUNCTION: STBC 0x00739bc0 _$E359
// FUNCTION: STBC 0x00739bd0 _$E364
// FUNCTION: STBC 0x00739be0 _$E363
// FUNCTION: STBC 0x00739bf0 _$E174
// FUNCTION: STBC 0x00739c00 _$E173
// FUNCTION: STBC 0x00739c10 _$E179
// FUNCTION: STBC 0x00739c20 _$E178
// FUNCTION: STBC 0x00739c30 _$E368
// FUNCTION: STBC 0x00739c40 _$E367
// FUNCTION: STBC 0x00739c50 _$E372
// FUNCTION: STBC 0x00739c60 _$E371
// FUNCTION: STBC 0x00739c70 _$E376
// FUNCTION: STBC 0x00739c80 _$E375
// FUNCTION: STBC 0x00739c90 _$E380
// FUNCTION: STBC 0x00739ca0 _$E379
// FUNCTION: STBC 0x00739cb0 _$E384
// FUNCTION: STBC 0x00739cc0 _$E383
// FUNCTION: STBC 0x00739cd0 _$E388
// FUNCTION: STBC 0x00739ce0 _$E387
// FUNCTION: STBC 0x00739cf0 _$E392
// FUNCTION: STBC 0x00739d00 _$E391
// FUNCTION: STBC 0x00739d10 _$E396
// FUNCTION: STBC 0x00739d20 _$E395
// FUNCTION: STBC 0x00739d30 _$E184
// FUNCTION: STBC 0x00739d40 _$E183
// FUNCTION: STBC 0x00739d50 _$E400
// FUNCTION: STBC 0x00739d60 _$E399
// FUNCTION: STBC 0x00739d70 _$E189
// FUNCTION: STBC 0x00739d80 _$E188
// FUNCTION: STBC 0x00739d90 _$E194
// FUNCTION: STBC 0x00739da0 _$E193

/*
================
TGRect::TGRect
================
*/
// FUNCTION: STBC 0x00739db0 ??0TGRect@@QAE@XZ
TGRect::TGRect( void ) {
	left = 0.0f;
	top = 0.0f;
	width = 0.0f;
	height = 0.0f;
}

/*
================
TGRect::TGRect
================
*/
// FUNCTION: STBC 0x00739dc0 ??0TGRect@@QAE@ABV0@@Z
TGRect::TGRect( const TGRect &rect ) {
	left = rect.left;
	top = rect.top;
	width = rect.width;
	height = rect.height;
}

/*
================
TGRect::Unknown739DE0

The copy constructor's body as a member function. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00739de0
TGRect &TGRect::Unknown739DE0( const TGRect &rect ) {
	left = rect.left;
	top = rect.top;
	width = rect.width;
	height = rect.height;
	return *this;
}

/*
================
TGRect::TGRect
================
*/
// FUNCTION: STBC 0x00739e00 ??0TGRect@@QAE@MMMM@Z
TGRect::TGRect( float left, float top, float width, float height ) {
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
}

/*
================
TGRect::~TGRect
================
*/
// FUNCTION: STBC 0x00739e20
TGRect::~TGRect( void ) {
}

/*
================
TGRect::DoesIntersect

True when the rectangles overlap; rectangles that only touch do not.
================
*/
// FUNCTION: STBC 0x00739e30
bool TGRect::DoesIntersect( TGRect *rect ) {
	if ( GetRight() <= rect->left ) {
		return false;
	}
	if ( GetBottom() <= rect->top ) {
		return false;
	}
	if ( rect->GetRight() <= left ) {
		return false;
	}
	if ( rect->GetBottom() <= top ) {
		return false;
	}
	return true;
}

/*
================
TGRect::IsWithin

True when the point is within the rectangle or on its edge. Nothing is
within an empty rectangle.
================
*/
// FUNCTION: STBC 0x00739e90
bool TGRect::IsWithin( float x, float y ) {
	if ( IsEmpty() ) {
		return false;
	}
	if ( x >= left && x <= GetRight() && y >= top && y <= top + GetHeight() ) {
		return true;
	}
	return false;
}

/*
================
TGRect::Unknown739F00

Counts the corners of the rectangle that are within this one: RECT_INSIDE
when all four are, RECT_PARTIAL for some, RECT_OUTSIDE for none or when
this rectangle is empty.
================
*/
// FUNCTION: STBC 0x00739f00
int TGRect::Unknown739F00( TGRect *rect ) {
	int corners = 0;

	if ( IsEmpty() ) {
		return RECT_OUTSIDE;
	}

	if ( IsWithin( rect->left, rect->top ) ) {
		corners++;
	}
	if ( IsWithin( rect->left, rect->GetBottom() ) ) {
		corners++;
	}
	if ( IsWithin( rect->GetRight(), rect->top ) ) {
		corners++;
	}
	if ( IsWithin( rect->GetRight(), rect->GetBottom() ) ) {
		corners++;
	}

	if ( corners == 0 ) {
		return RECT_OUTSIDE;
	}
	if ( corners < 4 ) {
		return RECT_PARTIAL;
	}
	return RECT_INSIDE;
}

/*
================
TGRect::ClipTo

Shrinks the rectangle to its overlap with the other; clears it when they
do not overlap.
================
*/
// FUNCTION: STBC 0x00739fc0
void TGRect::ClipTo( TGRect *rect ) {
	if ( !DoesIntersect( rect ) ) {
		Clear();
		return;
	}

	float right = GetRight();
	float clipRight = rect->GetRight();
	float bottom = top + GetHeight();
	float clipBottom = rect->GetBottom();

	if ( left < rect->left ) {
		width -= rect->left - left;
		left = rect->left;
	}
	if ( right > clipRight ) {
		width -= right - clipRight;
	}
	if ( top < rect->top ) {
		height -= rect->top - top;
		top = rect->top;
	}
	if ( bottom > clipBottom ) {
		height -= bottom - clipBottom;
	}
}

/*
================
TGRect::Clear
================
*/
// FUNCTION: STBC 0x0073a080
void TGRect::Clear( void ) {
	left = 0.0f;
	top = 0.0f;
	width = 0.0f;
	height = 0.0f;
}

/*
================
TGRect::operator-

Moves the rectangle itself, then returns a copy of it: unlike the usual
binary operator, the left operand changes.
================
*/
// FUNCTION: STBC 0x0073a090 ??GTGRect@@QAE?AV0@VNiPoint2@@@Z
TGRect TGRect::operator-( NiPoint2 offset ) {
	left -= offset.x;
	top -= offset.y;
	return *this;
}

/*
================
TGRect::operator-=
================
*/
// FUNCTION: STBC 0x0073a0c0 ??ZTGRect@@QAEXVNiPoint2@@@Z
void TGRect::operator-=( NiPoint2 offset ) {
	left -= offset.x;
	top -= offset.y;
}

/*
================
TGRect::operator+

Like operator-, moves the rectangle itself and returns a copy of it.
================
*/
// FUNCTION: STBC 0x0073a0e0 ??HTGRect@@QAE?AV0@VNiPoint2@@@Z
TGRect TGRect::operator+( NiPoint2 offset ) {
	left += offset.x;
	top += offset.y;
	return *this;
}

/*
================
TGRect::operator+=
================
*/
// FUNCTION: STBC 0x0073a110 ??YTGRect@@QAEXVNiPoint2@@@Z
void TGRect::operator+=( NiPoint2 offset ) {
	left += offset.x;
	top += offset.y;
}

/*
================
TGRect::WriteToStream
================
*/
// FUNCTION: STBC 0x0073a130
bool TGRect::WriteToStream( TGStream *stream ) {
	stream->WriteFloat( left );
	stream->WriteFloat( top );
	stream->WriteFloat( width );
	stream->WriteFloat( height );
	return true;
}

/*
================
TGRect::ReadFromStream
================
*/
// FUNCTION: STBC 0x0073a170
bool TGRect::ReadFromStream( TGStream *stream ) {
	left = stream->ReadFloat();
	top = stream->ReadFloat();
	width = stream->ReadFloat();
	height = stream->ReadFloat();
	return true;
}
