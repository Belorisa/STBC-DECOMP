/*
===============================================================================

	TGParagraph.cpp

===============================================================================
*/

// Before <iostream>: iswspace stays the CRT function (wchar.h would make it
// an inline call of iswctype).
#include <ctype.h>
#include <iostream>
#include <windows.h>

// <windows.h>'s ANSI/wide macros would rename the game's own methods.
#undef GetClassName
#undef GetObject
#undef PlaySound

#include "TGParagraph.h"
#include "TGEventHandlerTable.h"
#include "TGFontGroup.h"
#include "TGFontManager.h"
#include "TGIconPoly.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "TGBoolEvent.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include "TGUIBatch.h"
#include "TGUIModule.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007311f0 _$E32
// FUNCTION: STBC 0x00731200 _$E29
// FUNCTION: STBC 0x00731210 _$E31
// FUNCTION: STBC 0x00731220 _$E30
// FUNCTION: STBC 0x00731230 _$E38
// FUNCTION: STBC 0x00731240 _$E35
// FUNCTION: STBC 0x00731250 _$E37
// FUNCTION: STBC 0x00731260 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00731270 _$E57
// FUNCTION: STBC 0x00731280 _$E56
// FUNCTION: STBC 0x00731290 _$E61
// FUNCTION: STBC 0x007312a0 _$E60
// FUNCTION: STBC 0x007312b0 _$E72
// FUNCTION: STBC 0x007312c0 _$E71
// FUNCTION: STBC 0x007312d0 _$E76
// FUNCTION: STBC 0x007312e0 _$E75
// FUNCTION: STBC 0x007312f0 _$E80
// FUNCTION: STBC 0x00731300 _$E79
// FUNCTION: STBC 0x00731310 _$E84
// FUNCTION: STBC 0x00731320 _$E83
// FUNCTION: STBC 0x00731330 _$E208
// FUNCTION: STBC 0x00731340 _$E207
// FUNCTION: STBC 0x00731350 _$E150
// FUNCTION: STBC 0x00731360 _$E149
// FUNCTION: STBC 0x00731370 _$E163
// FUNCTION: STBC 0x00731380 _$E162
// FUNCTION: STBC 0x00731390 _$E213
// FUNCTION: STBC 0x007313a0 _$E212
// FUNCTION: STBC 0x007313b0 _$E218
// FUNCTION: STBC 0x007313c0 _$E217
// FUNCTION: STBC 0x007313d0 _$E222
// FUNCTION: STBC 0x007313e0 _$E221
// FUNCTION: STBC 0x007313f0 _$E226
// FUNCTION: STBC 0x00731400 _$E225
// FUNCTION: STBC 0x00731410 _$E233
// FUNCTION: STBC 0x00731420 _$E232
// FUNCTION: STBC 0x00731430 _$E319
// FUNCTION: STBC 0x00731440 _$E318
// FUNCTION: STBC 0x00731450 _$E323
// FUNCTION: STBC 0x00731460 _$E322
// FUNCTION: STBC 0x00731470 _$E108
// FUNCTION: STBC 0x00731480 _$E107
// FUNCTION: STBC 0x00731490 _$E246
// FUNCTION: STBC 0x007314a0 _$E245
// FUNCTION: STBC 0x007314b0 _$E253
// FUNCTION: STBC 0x007314c0 _$E252
// FUNCTION: STBC 0x007314d0 _$E257
// FUNCTION: STBC 0x007314e0 _$E256
// FUNCTION: STBC 0x007314f0 _$E261
// FUNCTION: STBC 0x00731500 _$E260
// FUNCTION: STBC 0x00731510 _$E265
// FUNCTION: STBC 0x00731520 _$E264
// FUNCTION: STBC 0x00731530 _$E271
// FUNCTION: STBC 0x00731540 _$E270
// FUNCTION: STBC 0x00731550 _$E327
// FUNCTION: STBC 0x00731560 _$E326
// FUNCTION: STBC 0x00731570 _$E331
// FUNCTION: STBC 0x00731580 _$E330
// FUNCTION: STBC 0x00731590 _$E335
// FUNCTION: STBC 0x007315a0 _$E334
// FUNCTION: STBC 0x007315b0 _$E339
// FUNCTION: STBC 0x007315c0 _$E338
// FUNCTION: STBC 0x007315d0 _$E343
// FUNCTION: STBC 0x007315e0 _$E342
// FUNCTION: STBC 0x007315f0 _$E347
// FUNCTION: STBC 0x00731600 _$E346
// FUNCTION: STBC 0x00731610 _$E351
// FUNCTION: STBC 0x00731620 _$E350
// FUNCTION: STBC 0x00731630 _$E355
// FUNCTION: STBC 0x00731640 _$E354
// FUNCTION: STBC 0x00731650 _$E359
// FUNCTION: STBC 0x00731660 _$E358
// FUNCTION: STBC 0x00731670 _$E363
// FUNCTION: STBC 0x00731680 _$E362
// FUNCTION: STBC 0x00731690 _$E113
// FUNCTION: STBC 0x007316a0 _$E112
// FUNCTION: STBC 0x007316b0 _$E367
// FUNCTION: STBC 0x007316c0 _$E366
// FUNCTION: STBC 0x007316d0 _$E118
// FUNCTION: STBC 0x007316e0 _$E117
// FUNCTION: STBC 0x007316f0 _$E371
// FUNCTION: STBC 0x00731700 _$E370
// FUNCTION: STBC 0x00731710 _$E375
// FUNCTION: STBC 0x00731720 _$E374
// FUNCTION: STBC 0x00731730 _$E379
// FUNCTION: STBC 0x00731740 _$E378
// FUNCTION: STBC 0x00731750 _$E383
// FUNCTION: STBC 0x00731760 _$E382
// FUNCTION: STBC 0x00731770 _$E127
// FUNCTION: STBC 0x00731780 _$E126
// FUNCTION: STBC 0x00731790 _$E387
// FUNCTION: STBC 0x007317a0 _$E386
// FUNCTION: STBC 0x007317b0 _$E391
// FUNCTION: STBC 0x007317c0 _$E390
// FUNCTION: STBC 0x007317d0 _$E395
// FUNCTION: STBC 0x007317e0 _$E394
// FUNCTION: STBC 0x007317f0 _$E133
// FUNCTION: STBC 0x00731800 _$E132
// FUNCTION: STBC 0x00731810 _$E138
// FUNCTION: STBC 0x00731820 _$E137
// FUNCTION: STBC 0x00731830 _$E168
// FUNCTION: STBC 0x00731840 _$E167
// FUNCTION: STBC 0x00731850 _$E399
// FUNCTION: STBC 0x00731860 _$E398
// FUNCTION: STBC 0x00731870 _$E173
// FUNCTION: STBC 0x00731880 _$E172
// FUNCTION: STBC 0x00731890 _$E403
// FUNCTION: STBC 0x007318a0 _$E402
// FUNCTION: STBC 0x007318b0 _$E407
// FUNCTION: STBC 0x007318c0 _$E406
// FUNCTION: STBC 0x007318d0 _$E178
// FUNCTION: STBC 0x007318e0 _$E177
// FUNCTION: STBC 0x007318f0 _$E411
// FUNCTION: STBC 0x00731900 _$E410
// FUNCTION: STBC 0x00731910 _$E143
// FUNCTION: STBC 0x00731920 _$E142
// FUNCTION: STBC 0x00731930 _$E415
// FUNCTION: STBC 0x00731940 _$E414
// FUNCTION: STBC 0x00731950 _$E419
// FUNCTION: STBC 0x00731960 _$E418
// FUNCTION: STBC 0x00731970 _$E183
// FUNCTION: STBC 0x00731980 _$E182
// FUNCTION: STBC 0x00731990 _$E188
// FUNCTION: STBC 0x007319a0 _$E187
// FUNCTION: STBC 0x007319b0 _$E423
// FUNCTION: STBC 0x007319c0 _$E422
// FUNCTION: STBC 0x007319d0 _$E427
// FUNCTION: STBC 0x007319e0 _$E426
// FUNCTION: STBC 0x007319f0 _$E431
// FUNCTION: STBC 0x00731a00 _$E430
// FUNCTION: STBC 0x00731a10 _$E435
// FUNCTION: STBC 0x00731a20 _$E434
// FUNCTION: STBC 0x00731a30 _$E439
// FUNCTION: STBC 0x00731a40 _$E438
// FUNCTION: STBC 0x00731a50 _$E443
// FUNCTION: STBC 0x00731a60 _$E442
// FUNCTION: STBC 0x00731a70 _$E447
// FUNCTION: STBC 0x00731a80 _$E446
// FUNCTION: STBC 0x00731a90 _$E451
// FUNCTION: STBC 0x00731aa0 _$E450
// FUNCTION: STBC 0x00731ab0 _$E193
// FUNCTION: STBC 0x00731ac0 _$E192
// FUNCTION: STBC 0x00731ad0 _$E455
// FUNCTION: STBC 0x00731ae0 _$E454
// FUNCTION: STBC 0x00731af0 _$E198
// FUNCTION: STBC 0x00731b00 _$E197
// FUNCTION: STBC 0x00731b10 _$E203
// FUNCTION: STBC 0x00731b20 _$E202

/*
================
TGParagraph::Cast

The object as a paragraph; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00731b80
TGParagraph *TGParagraph::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGPARAGRAPH_TYPE ) ) {
		return (TGParagraph *)object;
	}
	return NULL;
}

/*
================
TGParagraph::TGParagraph

TGParagraph_CreateW: the paragraph from a wide string.
================
*/
// FUNCTION: STBC 0x00731bb0 ??0TGParagraph@@QAE@PAVTGString@@MPAVNiColorA@@PADMH@Z
TGParagraph::TGParagraph( TGString *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags ) : TGPane( 0 ), string( "" ), ignoreString( "" ) {
	Init( string, width, color, fontGroupName, fontSize, flags );
}

/*
================
TGParagraph::TGParagraph

TGParagraph_Create: the paragraph from a C string.
================
*/
// FUNCTION: STBC 0x00731c50 ??0TGParagraph@@QAE@PADMPAVNiColorA@@0MH@Z
TGParagraph::TGParagraph( char *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags ) : TGPane( 0 ), string( "" ), ignoreString( "" ) {
	TGString text( "" );
	text.AssignC( string );
	Init( &text, width, color, fontGroupName, fontSize, flags );
}

/*
================
TGParagraph::Init

The constructors' common part: takes the string, the wrap width, the
TGPF_* flags, the colour (white without one) and the font group (the
default font when fontGroupName is not found), then sizes the text.
================
*/
// FUNCTION: STBC 0x00731d20
void TGParagraph::Init( TGString *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags ) {
	if ( string == NULL ) {
		return;
	}

	maxChars = 0x7FFFFFFF;
	this->string.Unknown6F4D90( string );
	numPolys = string->GetLength();
	cursorPosition = numPolys;
	polys = (TGIconPoly **)malloc( numPolys * sizeof( TGIconPoly * ) );
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		polys[i] = NULL;
	}

	wrapWidth = width;
	this->flags |= ( flags & TGPF_FLAGS_MASK ) | TGPF_INSERT_MODE;

	if ( color == NULL ) {
		this->color.r = 1.0f;
		this->color.g = 1.0f;
		this->color.b = 1.0f;
		this->color.a = 1.0f;
	} else {
		this->color = *color;
	}

	TGFontGroup *group = g_kFontManager.GetFontGroup( fontGroupName, fontSize );
	if ( group == NULL ) {
		group = g_kFontManager.GetDefaultFont();
	}
	fontGroup = NULL;
	SetFontGroup( group, true );
	RecalcBounds( false );
}

/*
================
TGParagraph::~TGParagraph
================
*/
// FUNCTION: STBC 0x00731e20
TGParagraph::~TGParagraph( void ) {
	VirtualSlot60();
	if ( polys != NULL ) {
		free( polys );
		polys = NULL;
	}
	SetFontGroup( NULL, false );
	if ( g_kRootWindow.GetTrueFocus() == this && parent != NULL ) {
		parent->RemoveChild( this, true );
	}
}

/*
================
TGParagraph::VirtualSlot60

Takes the cursor (when the paragraph has the true focus) and the
characters' polys out of their batches.
================
*/
// FUNCTION: STBC 0x00731ed0
void TGParagraph::VirtualSlot60( void ) {
	if ( cursorPoly != NULL && HasTrueFocus() ) {
		cursorPoly->batch->RemoveIconPoly( cursorPoly );
		cursorPoly = NULL;
	}
	if ( polys != NULL ) {
		for ( unsigned int i = 0; i < numPolys; i++ ) {
			// Entries up to numPolys number embedded children, not polys.
			if ( (unsigned int)polys[i] > numPolys ) {
				polys[i]->batch->RemoveIconPoly( polys[i] );
			}
			polys[i] = NULL;
		}
	}
	TGPane::VirtualSlot60();
}

/*
================
TGParagraph::VirtualSlot61

Moves the characters' polys with the paragraph.
================
*/
// FUNCTION: STBC 0x00731f40
void TGParagraph::VirtualSlot61( NiPoint2 *offset ) {
	TGPane::VirtualSlot61( offset );
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		if ( (unsigned int)polys[i] > numPolys ) {
			polys[i]->Move( offset );
		}
	}
}

/*
================
TGParagraph::VirtualSlot62

Clips the characters' polys to the paragraph's new clip rectangle.
================
*/
// FUNCTION: STBC 0x00731f80
void TGParagraph::VirtualSlot62( TGRect *clipRect ) {
	TGPane::VirtualSlot62( clipRect );
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		if ( (unsigned int)polys[i] > numPolys ) {
			polys[i]->SetClipRect( clipRect );
		}
	}
}

/*
================
TGParagraph::SetReadOnly
================
*/
// FUNCTION: STBC 0x00731fc0
void TGParagraph::SetReadOnly( bool readOnly ) {
	if ( readOnly ) {
		flags |= TGPF_READ_ONLY;
	} else {
		flags &= ~TGPF_READ_ONLY;
	}
}

/*
================
TGParagraph::SetString

Replaces the text, keeping the cursor inside it, and resizes the poly
block to the new length.
================
*/
// FUNCTION: STBC 0x00731ff0 ?SetString@TGParagraph@@UAEXPAVTGString@@_N@Z
void TGParagraph::SetString( TGString *string, bool layout ) {
	VirtualSlot60();
	this->string = *string;
	numPolys = this->string.GetLength();
	if ( cursorPosition > numPolys ) {
		cursorPosition = numPolys;
	}
	polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		polys[i] = NULL;
	}
	RecalcBounds( layout );
}

/*
================
TGParagraph::SetString

TGParagraph_SetString: the text from a C string.
================
*/
// FUNCTION: STBC 0x00732070 ?SetString@TGParagraph@@QAEXPAD_N@Z
void TGParagraph::SetString( char *string, bool layout ) {
	TGString text( string );
	SetString( &text, layout );
}

/*
================
TGParagraph::SetIgnoreString

The characters the paragraph does not take from the keyboard.
================
*/
// FUNCTION: STBC 0x007320e0 ?SetIgnoreString@TGParagraph@@UAEXPAVTGString@@@Z
void TGParagraph::SetIgnoreString( TGString *string ) {
	ignoreString = *string;
}

/*
================
TGParagraph::SetIgnoreString

TGParagraph_SetIgnoreString: the characters from a C string.
================
*/
// FUNCTION: STBC 0x00732100 ?SetIgnoreString@TGParagraph@@QAEXPAD@Z
void TGParagraph::SetIgnoreString( char *string ) {
	ignoreString.AssignC( string );
}

/*
================
TGParagraph::RecalcBounds

Resizes the paragraph to its text, its embedded paragraphs first.
================
*/
// FUNCTION: STBC 0x00732120
void TGParagraph::RecalcBounds( bool layout ) {
	flags |= TGPF_RECALC_BOUNDS;
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGParagraph *paragraph = Cast( children.GetNext( pos ) );
		if ( paragraph != NULL ) {
			paragraph->RecalcBounds( false );
		}
	}
	Unknown7337F0();
	Layout();
	flags &= ~TGPF_RECALC_BOUNDS;
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGParagraph::Layout

Places the characters a line below a parent paragraph's spacing and, when
the paragraph is shown, the cursor.
================
*/
// FUNCTION: STBC 0x007321a0
void TGParagraph::Layout( void ) {
	bool recalcBounds = ( flags & TGPF_RECALC_BOUNDS ) != 0;
	if ( fontGroup == NULL ) {
		return;
	}
	if ( !IsCompletelyVisible() ) {
		if ( !recalcBounds ) {
			return;
		}
	} else if ( !recalcBounds ) {
		if ( cursorPoly != NULL && HasTrueFocus() ) {
			cursorPoly->batch->RemoveIconPoly( cursorPoly );
			cursorPoly = NULL;
		}
		Unknown732420();
	}

	float spacing = fontGroup->GetScreenVerticalSpacing();
	TGPane *pane = parent;
	if ( pane != NULL && pane->IsTypeOf( TGPARAGRAPH_TYPE ) == true ) {
		TGFontGroup *parentFontGroup = ( (TGParagraph *)pane )->fontGroup;
		if ( parentFontGroup != NULL ) {
			float parentSpacing = parentFontGroup->GetScreenVerticalSpacing();
			if ( spacing > parentSpacing ) {
				spacing -= parentSpacing;
			} else {
				spacing = 0.0f;
			}
		}
	}
	NiPoint2 start;
	start.x = 0.0f;
	start.y = spacing;
	VirtualSlot92( &start );

	if ( !recalcBounds ) {
		Unknown732550();
		TGPane::Layout();
	}
}

/*
================
TGParagraph::VirtualSlot92

Places the characters from start, line by line, and (unless the bounds are
being recalculated) gives each one its poly, or puts the embedded child it
stands for in its place.
================
*/
// FUNCTION: STBC 0x007322b0
void TGParagraph::VirtualSlot92( NiPoint2 *start ) {
	bool recalcBounds = ( flags & TGPF_RECALC_BOUNDS ) != 0;
	if ( !recalcBounds ) {
		VirtualSlot60();
	}
	float lineHeight = fontGroup->GetScreenMaxHeight();

	TGRect clipRect;
	GetClipRect( &clipRect );
	NiPoint2 position;
	GetPosition( &position );
	clipRect -= position;

	int child = 0;
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		unsigned short c = string.Unknown6F4C20( i );
		TGRect rect;
		TGIconPoly *poly = NULL;
		bool lineBreak = VirtualSlot93( i, start, &rect, &lineHeight, child );
		if ( recalcBounds ) {
			continue;
		}
		if ( !lineBreak ) {
			TGUIObject *object;
			if ( c == CURSOR_ICON && ( object = GetNthChild( child++ ) ) != NULL ) {
				poly = (TGIconPoly *)child;
				float top = rect.top;	// (invented)
				object->SetPosition( rect.left, top, true );
			} else {
				poly = Unknown7328A0( i, &rect, &clipRect );
			}
		}
		polys[i] = poly;
	}
}

/*
================
TGParagraph::Unknown732420

Makes the cursor poly, at the origin, while the paragraph has the true
focus, takes text and the cursor is on; the bounds grow to hold it.
================
*/
// FUNCTION: STBC 0x00732420
void TGParagraph::Unknown732420( void ) {
	if ( cursorTimer == NULL || parent == NULL || ( flags & TGPF_READ_ONLY ) || !HasTrueFocus() ) {
		return;
	}
	TGBoolEvent *blink = (TGBoolEvent *)cursorTimer->GetEvent();
	if ( blink == NULL || !blink->GetBool() ) {
		return;
	}

	TGRect rect( 0.0f, 0.0f, fontGroup->GetIconScreenWidth( CURSOR_ICON ), fontGroup->GetScreenMaxHeight() );
	cursorPoly = g_kIconManager.CreateIconPoly( GetBatches(), fontGroup->GetName(), CURSOR_ICON, &rect, &color, 0 );
	float right = rect.GetRight();
	float bottom = rect.GetBottom();
	if ( right > bounds.width ) {
		bounds.width = right;
	}
	if ( bottom > bounds.height ) {
		bounds.height = bottom;
	}
}

/*
================
TGParagraph::Unknown732550

Moves the cursor poly to the cursor while the paragraph has the true focus
and takes text, making it again when it was clipped.
================
*/
// FUNCTION: STBC 0x00732550
// DRAFT: frame packing: retail shares the argument-copy pointer's slot with position and keeps the difference's temporary above offset; it also copies the poly's left to position and oldRect through the FPU, and zeroes position in a block at the end
void TGParagraph::Unknown732550( void ) {
	if ( parent == NULL || !HasTrueFocus() || ( flags & TGPF_READ_ONLY ) ) {
		return;
	}
	TGRect oldRect;
	TGRect cursorRect;
	TGRect clipRect;
	GetClipRect( &clipRect );
	NiPoint2 offset;
	parent->GetScreenOffset( &offset );
	clipRect += offset;

	int within = Unknown732710( &cursorRect, &clipRect );
	if ( within == RECT_OUTSIDE ) {
		return;
	}
	NiPoint2 position;
	if ( cursorPoly != NULL ) {
		position.x = cursorPoly->rect.left;
		position.y = cursorPoly->rect.top;
		oldRect.Set( position.x, position.y, cursorRect.width, cursorRect.height );
		if ( clipRect.Unknown739F00( &oldRect ) != RECT_INSIDE ) {
			if ( cursorPoly != NULL ) {
				cursorPoly->batch->RemoveIconPoly( cursorPoly );
				cursorPoly = NULL;
			}
			Unknown732420();
		}
	} else {
		position.x = 0.0f;
		position.y = 0.0f;
		Unknown732420();
	}
	if ( cursorPoly != NULL ) {
		NiPoint2 corner;
		cursorRect.GetPosition( &corner );
		position = corner - position;
		cursorPoly->Move( &position );
		if ( within == RECT_PARTIAL ) {
			cursorPoly->SetClipRect( &clipRect );
		}
	}
}

/*
================
TGParagraph::Unknown732710

The cursor's rectangle on the screen, between the character before the
cursor and the one after it, and how much of it clipRect holds; the cursor
poly goes when none.
================
*/
// FUNCTION: STBC 0x00732710
int TGParagraph::Unknown732710( TGRect *cursorRect, TGRect *clipRect ) {
	cursorRect->width = fontGroup->GetIconScreenWidth( CURSOR_ICON );
	cursorRect->height = fontGroup->GetScreenMaxHeight();

	NiPoint2 position;
	position.y = fontGroup->GetScreenVerticalSpacing();
	position.x = 0.0f;
	float lineHeight = fontGroup->GetScreenMaxHeight();
	TGRect rect( 0.0f, 0.0f, 0.0f, 0.0f );
	unsigned int i;
	for ( i = 0; i <= cursorPosition && i < numPolys && cursorPosition != 0; i++ ) {
		VirtualSlot93( i, &position, &rect, &lineHeight, i );
	}

	float halfGap = ( fontGroup->GetScreenHorizontalSpacing() - cursorRect->width ) * 0.5f + cursorRect->width;
	cursorRect->top = rect.top;
	cursorRect->left = rect.left;
	if ( i != 0 ) {
		cursorRect->left = rect.left - halfGap;
		if ( cursorPosition == numPolys ) {
			cursorRect->left = rect.width + rect.left - cursorRect->width;
		}
	}

	NiPoint2 offset;
	GetScreenOffset( &offset );
	*cursorRect += offset;
	int within = clipRect->Unknown739F00( cursorRect );
	if ( within == RECT_OUTSIDE && cursorPoly != NULL ) {
		cursorPoly->batch->RemoveIconPoly( cursorPoly );
		cursorPoly = NULL;
	}
	return within;
}

/*
================
TGParagraph::Unknown7328A0

The poly of character index in rect, clipped to clipRect, or NULL when
the rectangle is outside it.
================
*/
// FUNCTION: STBC 0x007328a0
TGIconPoly *TGParagraph::Unknown7328A0( unsigned int index, TGRect *rect, TGRect *clipRect ) {
	int within = clipRect->Unknown739F00( rect );
	if ( within == RECT_OUTSIDE ) {
		return NULL;
	}
	unsigned short icon = fontGroup->MapChar( string.Unknown6F4C20( index ) );
	if ( icon == CURSOR_ICON ) {
		icon = 0;
	}
	TGIconPoly *poly = g_kIconManager.CreateIconPoly( GetBatches(), fontGroup->GetName(), icon, rect, &color, 0 );
	if ( poly == NULL ) {
		return NULL;
	}
	if ( within == RECT_PARTIAL ) {
		poly->SetClipRect( clipRect );
	}
	NiPoint2 offset;
	GetScreenOffset( &offset );
	offset = TGUIModule::PixelAlignPoint( offset );
	poly->Move( &offset );
	return poly;
}

/*
================
TGParagraph::VirtualSlot93
================
*/
// FUNCTION: STBC 0x00732a00
// DRAFT: retail calls NiPoint2's copy constructor out of line for PixelAlignPoint's result and keeps the aligned point in memory; the screen height load differs as in Unknown7328A0
bool TGParagraph::VirtualSlot93( unsigned int index, NiPoint2 *position, TGRect *rect, float *lineHeight, int child ) {
	if ( fontGroup == NULL ) {
		return true;
	}
	if ( Unknown732C60( index, position, lineHeight, child ) ) {
		return true;
	}
	unsigned short c = string.Unknown6F4C20( index );
	TGUIObject *object = NULL;
	if ( c == CURSOR_ICON ) {
		object = GetNthChild( child );
	}
	float width;
	float height;
	Unknown732EF0( index, position, &width, &height, object );
	rect->left = position->x;
	rect->top = position->y;
	if ( height > *lineHeight ) {
		*lineHeight = height;
	}
	if ( c == '\r' || c == '\n' || rect->left + width > wrapWidth ) {
		rect->left = 0.0f;
		rect->top += fontGroup->GetScreenVerticalSpacing() + *lineHeight;
		*lineHeight = fontGroup->GetScreenMaxHeight();
	}
	rect->width = width;
	rect->height = height;
	*rect = TGUIModule::PixelAlignRect( *rect );

	position->x = rect->GetRight();
	position->x += fontGroup->GetScreenHorizontalSpacing();
	position->y = rect->top;
	*position = TGUIModule::PixelAlignPoint( *position );

	if ( rect->GetRight() > bounds.width ) {
		bounds.width = rect->GetRight();
	}
	if ( rect->top + *lineHeight > bounds.height ) {
		bounds.height = rect->top + *lineHeight;
	}
	if ( c == '\r' || c == '\n' ) {
		return true;
	}
	return false;
}

/*
================
TGParagraph::Unknown732C60

With word wrap, starts a new line at a space before a word that doesn't
fit the rest of the line; true when it did.
================
*/
// FUNCTION: STBC 0x00732c60
bool TGParagraph::Unknown732C60( unsigned int index, NiPoint2 *position, float *lineHeight, int child ) {
	if ( flags & TGPF_WORD_WRAP ) {
		Unknown732D50( index, position, lineHeight, child );
		if ( iswspace( string.Unknown6F4C20( index ) ) && Unknown732EA0( index, position ) ) {
			position->x = 0.0f;
			float spacing = fontGroup->GetScreenVerticalSpacing();
			position->y += spacing + *lineHeight;
			position->y = TGUIModule::PixelAlignValue( position->y, false );
			*lineHeight = fontGroup->GetScreenMaxHeight();
			return true;
		}
	}
	return false;
}

/*
================
TGParagraph::Unknown732D50

Measures the word that starts at index and, when it runs past the wrap
width, moves position to the start of the next line.
================
*/
// FUNCTION: STBC 0x00732d50
void TGParagraph::Unknown732D50( unsigned int index, NiPoint2 *position, float *lineHeight, int child ) {
	float wordHeight = *lineHeight;
	NiPoint2 end = *position;
	float spacing = fontGroup->GetScreenHorizontalSpacing();
	for ( unsigned int i = index; i < numPolys; i++ ) {
		unsigned short c = string.Unknown6F4C20( i );
		if ( c == '\r' || c == '\n' || iswspace( c ) ) {
			return;
		}
		TGUIObject *object = NULL;
		if ( c == CURSOR_ICON ) {
			object = GetNthChild( child++ );
		}
		float width;
		float height;
		Unknown732EF0( i, &end, &width, &height, object );
		if ( height > wordHeight ) {
			wordHeight = height;
		}
		end.x += width;
		if ( end.x > wrapWidth ) {
			position->x = 0.0f;
			position->y += fontGroup->GetScreenVerticalSpacing() + wordHeight;
			*lineHeight = fontGroup->GetScreenMaxHeight();
			return;
		}
		end.x += spacing;
	}
}

/*
================
TGParagraph::Unknown732EA0

Whether the character at index, placed at position, ends past the wrap
width.
================
*/
// FUNCTION: STBC 0x00732ea0
bool TGParagraph::Unknown732EA0( unsigned int index, NiPoint2 *position ) {
	float x = position->x;
	float width;
	float height;
	Unknown732EF0( index, position, &width, &height, NULL );
	float right = x + width;
	// BUG: the width is added twice
	if ( right + width > wrapWidth ) {
		return true;
	}
	return false;
}

/*
================
TGParagraph::Unknown732EF0

The size of character index at position: an embedded child's size, the
distance to the next tab stop, nothing for a line break, the font's
space or the character's icon.
================
*/
// FUNCTION: STBC 0x00732ef0
// DRAFT: retail copies the character into edi whole before masking it (mov edi, eax), a 16-bit mov here
void TGParagraph::Unknown732EF0( unsigned int index, NiPoint2 *position, float *width, float *height, TGUIObject *child ) {
	if ( child != NULL ) {
		*width = child->GetWidth();
		*height = child->GetHeight();
		return;
	}
	int c = string.Unknown6F4C20( index );
	unsigned short icon = fontGroup->MapChar( c );
	switch ( c ) {
		case '\t': {
			float tabWidth = fontGroup->GetScreenTabWidth();
			float tab = 0.0f;
			while ( tab <= position->x ) {
				tab += tabWidth;
			}
			if ( tab > wrapWidth ) {
				tab = wrapWidth;
			}
			*width = tab - position->x;
			break;
		}
		case ' ':
			*width = fontGroup->GetScreenSpaceWidth();
			break;
		case '\n':
		case '\r':
			*width = 0.0f;
			break;
		default:
			*width = fontGroup->GetIconScreenWidth( icon );
			break;
	}
	if ( c == '\n' || c == '\r' ) {
		*height = fontGroup->GetScreenMaxHeight();
	} else {
		*height = fontGroup->GetIconScreenHeight( icon );
	}
}

/*
================
TGParagraph::SetCursorPosition

Puts the cursor at position, at most the end of the text.
================
*/
// FUNCTION: STBC 0x00733050
void TGParagraph::SetCursorPosition( int position ) {
	if ( position < 0 ) {
		// BUG: a negative position was meant to count back from the end,
		// but it is subtracted, so it lands past the end.
		position = numPolys - position;
		if ( position < 0 ) {
			position = 0;
		}
	}
	if ( position > numPolys ) {
		cursorPosition = numPolys;
	} else {
		cursorPosition = position;
	}
}

// DATA: STBC 0x0095d010
float TGParagraph::cursorBlinkInterval = 0.5f;

/*
================
TGParagraph::CreateCursorTimer

The timer that blinks the cursor every half second, sending its event to
the paragraph with the true focus.
================
*/
// FUNCTION: STBC 0x00733080
void TGParagraph::CreateCursorTimer( void ) {
	if ( cursorTimer != NULL ) {
		return;
	}
	cursorTimer = new TGTimer;
	if ( cursorTimer == NULL ) {
		return;
	}
	cursorTimer->SetDuration( TIMER_FOREVER );
	cursorTimer->SetDelay( cursorBlinkInterval );
	TGBoolEvent *event = new TGBoolEvent;
	if ( event == NULL ) {
		delete cursorTimer;
		cursorTimer = NULL;
		return;
	}
	event->SetEventType( ET_CURSOR_BLINK );
	cursorTimer->SetEvent( event );
}

/*
================
TGParagraph::DeleteCursorTimer
================
*/
// FUNCTION: STBC 0x00733130
void TGParagraph::DeleteCursorTimer( void ) {
	if ( cursorTimer != NULL ) {
		g_kRealtimeTimerManager.RemoveTimer( cursorTimer );
		delete cursorTimer;
		cursorTimer = NULL;
	}
}

/*
================
TGParagraph::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00733160
void TGParagraph::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGParagraph::MouseHandler, "TGParagraph::MouseHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGParagraph::KeyboardHandler, "TGParagraph::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGParagraph::CursorBlinkHandler, "TGParagraph::CursorBlinkHandler" );
}

/*
================
TGParagraph::RegisterHandlers
================
*/
// FUNCTION: STBC 0x007331a0
void TGParagraph::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGParagraph::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGParagraph::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_CURSOR_BLINK, "TGParagraph::CursorBlinkHandler" );
}

/*
================
TGParagraph::MouseHandler

A left click on a visible paragraph puts the cursor at the character
clicked: past the last character whose left edge is at or before the
point, and past that character too when the point is beyond its middle.
The walk stops at the first character below the point. Embedded children
are tested by their own bounds.
================
*/
// FUNCTION: STBC 0x007331e0
// PARKED: x87 schedule; retail loads left and top onto the FPU stack and compares the point against them there, this build compares against memory
void TGParagraph::MouseHandler( TGMouseEvent *event ) {
	if ( !IsVisible() ) {
		return;
	}
	CallNextHandler( event );

	int buttons = event->GetFlags();
	if ( ( buttons & MEF_BUTTON_LEFT ) && ( buttons & MEF_BUTTON_DOWN ) ) {
		NiPoint2 point;
		point.x = event->x;
		point.y = event->y;
		NiPoint2 offset;
		GetScreenOffset( &offset );
		NiPoint2 screenPoint;
		screenPoint.x = offset.x + point.x;
		screenPoint.y = offset.y + point.y;

		unsigned int position = 0;
		for ( unsigned int i = 0; i < numPolys; i++ ) {
			TGIconPoly *poly = polys[i];
			if ( poly == NULL ) {
				continue;
			}
			if ( (unsigned int)poly > numPolys ) {
				float left = poly->rect.GetLeft();
				float top = poly->rect.GetTop();
				if ( screenPoint.y < top ) {
					break;
				}
				if ( screenPoint.x >= left ) {
					position = i;
				}
				if ( screenPoint.x >= left + poly->rect.GetWidth() * 0.5f ) {
					position = i + 1;
				}
			} else {
				TGUIObject *child = GetNthChild( (int)poly - 1 );
				float left = child->GetLeft();
				float top = child->GetTop();
				if ( point.y < top ) {
					break;
				}
				if ( point.x >= left ) {
					position = i;
				}
				if ( point.x >= left + child->GetWidth() * 0.5f ) {
					position = i + 1;
				}
			}
		}
		SetCursorPosition( position );
		Layout();
	}
	event->SetHandled();
}

/*
================
TGParagraph::KeyboardHandler

Editing keys of a paragraph that is not read only, on key up: the arrows,
home and end (and their Ctrl-A and Ctrl-E bindings) move the cursor,
insert toggles insert mode, delete removes the character at the cursor
and Ctrl-K everything after it. Typed characters are inserted at the
cursor, with Caps Lock swapping the case of letters and the keypad's keys
giving their characters, unless the paragraph is full or ignores the
character; backspace removes the character before the cursor. Every key
the paragraph uses sends ET_PARAGRAPH_KEY to it.
================
*/
// FUNCTION: STBC 0x00733330
void TGParagraph::KeyboardHandler( TGKeyboardEvent *event ) {
	CallNextHandler( event );
	if ( flags & TGPF_READ_ONLY ) {
		return;
	}

	int c = event->GetUnicode();
	bool used = false;
	bool changed = false;
	bool cursorKey = false;
	if ( event->GetKeyState() == KS_KEYUP ) {
		cursorKey = true;
		switch ( c ) {
		case WC_LEFT:
			used = true;
			if ( cursorPosition > 0 ) {
				cursorPosition--;
			}
			break;
		case WC_RIGHT:
			used = true;
			if ( cursorPosition < numPolys ) {
				cursorPosition++;
			}
			break;
		case WC_HOME:
		case WC_CTRL_A:
			used = true;
			cursorPosition = 0;
			break;
		case WC_END:
		case WC_CTRL_E:
			used = true;
			cursorPosition = numPolys;
			break;
		case WC_DELETE:
			used = true;
			if ( cursorPosition < numPolys ) {
				string.Unknown6F55C0( cursorPosition, 1 );
				VirtualSlot60();
				numPolys--;
				polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
				for ( unsigned int i = 0; i < numPolys; i++ ) {
					polys[i] = NULL;
				}
				changed = true;
			}
			break;
		case WC_CTRL_K:
			used = true;
			if ( cursorPosition < numPolys ) {
				VirtualSlot60();
				string.Unknown6F55C0( cursorPosition, numPolys - cursorPosition );
				numPolys = cursorPosition;
				polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
				for ( unsigned int i = 0; i < numPolys; i++ ) {
					polys[i] = NULL;
				}
				changed = true;
			}
			break;
		case WC_INSERT:
			used = true;
			flags ^= TGPF_INSERT_MODE;
			// fall through: insert doesn't move the cursor
		default:
			cursorKey = false;
			break;
		}
	} else if ( event->GetKeyState() == KS_NORMAL ) {
		used = true;
		changed = true;
		if ( c != WC_BACKSPACE ) {
			if ( GetKeyState( VK_CAPITAL ) ) {
				if ( c >= 'a' && c <= 'z' ) {
					c -= 'a' - 'A';
				} else if ( c >= 'A' && c <= 'Z' ) {
					c += 'a' - 'A';
				}
			}
			if ( c >= WC_NUMPAD0 && c <= WC_NUMPAD9 ) {
				c -= WC_NUMPAD0 - '0';
			}
			if ( c == WC_DECIMAL ) {
				c = '.';
			} else if ( c == WC_ADD ) {
				c = '+';
			} else if ( c == WC_SUBTRACT ) {
				c = '-';
			} else if ( c == WC_MULTIPLY ) {
				c = '*';
			} else if ( c == WC_DIVIDE ) {
				c = '/';
			}
			if ( numPolys >= (unsigned int)maxChars ) {
				changed = false;		// full
			} else if ( c >= WC_PRIVATE_USE_START || c == 0 ) {
				changed = false;		// not a character
			} else {
				int length = ignoreString.GetLength();
				for ( int i = 0; i < length; i++ ) {
					if ( c == ignoreString.Unknown6F4C20( i ) ) {
						changed = false;
						break;
					}
				}
				if ( changed ) {
					InsertChar( c, false );
				}
			}
		} else {
			if ( cursorPosition > 0 ) {
				VirtualSlot60();
				string.Unknown6F55C0( cursorPosition - 1, 1 );
				cursorPosition--;
				numPolys--;
				polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
				for ( unsigned int i = 0; i < numPolys; i++ ) {
					polys[i] = NULL;
				}
			}
		}
	}

	event->SetHandled();
	if ( cursorKey == true && !changed ) {
		Unknown732550();
	} else if ( changed == true ) {
		RecalcBounds( true );
	}
	if ( used ) {
		TGEvent *keyEvent = new TGEvent;
		keyEvent->SetEventType( ET_PARAGRAPH_KEY );
		keyEvent->SetDestination( this );
		g_kEventManager.AddEvent( keyEvent );
	}
}

/*
================
TGParagraph::CursorBlinkHandler

Turns the cursor on or off in the paragraph with the true focus; takes
the cursor away when there is no timer.
================
*/
// FUNCTION: STBC 0x00733780
void TGParagraph::CursorBlinkHandler( TGEvent *event ) {
	CallNextHandler( event );
	if ( cursorTimer == NULL ) {
		if ( cursorPoly != NULL ) {
			cursorPoly->batch->RemoveIconPoly( cursorPoly );
			cursorPoly = NULL;
		}
		return;
	}
	TGBoolEvent *blink = (TGBoolEvent *)cursorTimer->GetEvent();
	if ( blink == NULL ) {
		return;
	}
	blink->ToggleBool();
	TGParagraph *paragraph = Cast( g_kRootWindow.GetTrueFocus() );
	// The read-only flag tested is this paragraph's, not the focused one's.
	if ( paragraph != NULL && !( flags & TGPF_READ_ONLY ) ) {
		paragraph->Layout();
	}
}

/*
================
TGParagraph::Unknown7337F0

Empties the bounds to the height of one line of the font.
================
*/
// FUNCTION: STBC 0x007337f0
void TGParagraph::Unknown7337F0( void ) {
	bounds.width = 0.0f;
	bounds.height = 0.0f;
	if ( fontGroup != NULL ) {
		bounds.height = fontGroup->GetScreenMaxHeight();
	}
}

/*
================
TGParagraph::SetWrapWidth
================
*/
// FUNCTION: STBC 0x00733820
void TGParagraph::SetWrapWidth( float width, bool layout ) {
	wrapWidth = width;
	RecalcBounds( false );
	if ( layout ) {
		Layout();
	}
}

/*
================
TGParagraph::SetColor

The colour of the text, given to the characters' polys at once.
================
*/
// FUNCTION: STBC 0x00733850
void TGParagraph::SetColor( NiColorA *color ) {
	this->color = *color;
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		if ( (unsigned int)polys[i] > numPolys ) {
			polys[i]->SetColor( color );
		}
	}
}

/*
================
TGParagraph::InsertChild

An embedded child takes no focus away from the text.
================
*/
// FUNCTION: STBC 0x007338a0
void TGParagraph::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	TGPane::InsertChild( index, child, x, y, layout );
	SetFocus( NULL, layout );
}

/*
================
TGParagraph::InsertChar

Inserts c at the cursor, or overwrites the character there when the
paragraph is not in insert mode, and moves the cursor past it.
================
*/
// FUNCTION: STBC 0x007338e0
void TGParagraph::InsertChar( short c, bool layout ) {
	unsigned int oldNumPolys = numPolys;
	VirtualSlot60();
	if ( !( flags & TGPF_INSERT_MODE ) && cursorPosition < numPolys ) {
		string.Unknown6F55C0( cursorPosition, 1 );
		numPolys--;
	}
	string.Unknown6F54C0( cursorPosition, c );
	numPolys++;
	cursorPosition++;
	if ( oldNumPolys != numPolys ) {
		polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
		for ( unsigned int i = 0; i < numPolys; i++ ) {
			polys[i] = NULL;
		}
	}
	RecalcBounds( layout );
}

/*
================
TGParagraph::InsertString

Inserts string at the cursor, or overwrites as much of the text after
the cursor when the paragraph is not in insert mode, and moves the cursor
past it.
================
*/
// FUNCTION: STBC 0x00733980
void TGParagraph::InsertString( TGString *string, bool layout ) {
	int length = string->GetLength();
	unsigned int oldNumPolys = numPolys;
	VirtualSlot60();
	if ( !( flags & TGPF_INSERT_MODE ) && cursorPosition < numPolys ) {
		int count = numPolys - cursorPosition;
		if ( count > length ) {
			count = length;
		}
		this->string.Unknown6F55C0( cursorPosition, count );
		numPolys -= count;
	}
	this->string.Unknown6F5160( cursorPosition, string );
	numPolys += length;
	cursorPosition += length;
	if ( oldNumPolys != numPolys ) {
		polys = (TGIconPoly **)realloc( polys, numPolys * sizeof( TGIconPoly * ) );
		for ( unsigned int i = 0; i < numPolys; i++ ) {
			polys[i] = NULL;
		}
	}
	RecalcBounds( layout );
}

/*
================
TGParagraph::GotTrueFocus

Starts the cursor blinking in this paragraph.
================
*/
// FUNCTION: STBC 0x00733a40
void TGParagraph::GotTrueFocus( bool layout ) {
	if ( cursorTimer != NULL && cursorTimer->GetEvent() != NULL ) {
		cursorTimer->SetTimerStart( g_kUtopiaModule.GetRealTime() );
		TGBoolEvent *event = (TGBoolEvent *)cursorTimer->GetEvent();
		event->SetDestination( this );
		event->SetBool( false );
		g_kRealtimeTimerManager.AddTimer( cursorTimer );
		ProcessEvent( event );
	}
}

/*
================
TGParagraph::LostTrueFocus

Takes the cursor away and stops its timer.
================
*/
// FUNCTION: STBC 0x00733aa0
void TGParagraph::LostTrueFocus( bool layout ) {
	if ( cursorPoly != NULL ) {
		cursorPoly->batch->RemoveIconPoly( cursorPoly );
		cursorPoly = NULL;
	}
	if ( cursorTimer != NULL ) {
		cursorTimer->GetEvent()->SetDestination( NULL );
		g_kRealtimeTimerManager.RemoveTimer( cursorTimer );
	}
}

/*
================
TGParagraph::SetFontGroup

Draws the text in group from now on, holding a reference to it.
================
*/
// FUNCTION: STBC 0x00733af0
void TGParagraph::SetFontGroup( TGFontGroup *group, bool layout ) {
	if ( fontGroup != NULL ) {
		fontGroup->DecRefCount();
	}
	fontGroup = group;
	if ( group != NULL ) {
		group->IncRefCount();
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
TGParagraph::WriteToStream
================
*/
// FUNCTION: STBC 0x00733b30
bool TGParagraph::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	if ( fontGroup != NULL ) {
		stream->VirtualSlot11( fontGroup->GetFontName() );
		stream->WriteFloat( fontGroup->GetFontSize() );
	} else {
		stream->VirtualSlot11( NULL );
	}
	string.WriteToStream( stream );
	stream->WriteFloat( wrapWidth );
	stream->WriteLong( numPolys );
	stream->WriteLong( cursorPosition );
	stream->WriteLong( maxChars );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->WriteFloat( color.a );
	ignoreString.WriteToStream( stream );
	return true;
}

/*
================
TGParagraph::ReadFromStream

The font group is looked up in PostFixUp: until then fontGroup holds the
font's name and size.
================
*/
// FUNCTION: STBC 0x00733bf0
bool TGParagraph::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	FontRecord *record = (FontRecord *)malloc( sizeof( FontRecord ) );
	record->name = stream->VirtualSlot10( -1 );
	if ( record->name != NULL ) {
		record->size = stream->ReadFloat();
	}
	fontGroup = (TGFontGroup *)record;
	string.ReadFromStream( stream );
	wrapWidth = stream->ReadFloat();
	numPolys = stream->ReadLong();
	cursorPosition = stream->ReadLong();
	maxChars = stream->ReadLong();
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	color.a = stream->ReadFloat();
	ignoreString.ReadFromStream( stream );
	polys = (TGIconPoly **)malloc( numPolys * sizeof( TGIconPoly * ) );
	for ( unsigned int i = 0; i < numPolys; i++ ) {
		polys[i] = NULL;
	}
	return true;
}

/*
================
TGParagraph::WriteCursorTimer

Saves the cursor timer's ID, 0 without one.
================
*/
// FUNCTION: STBC 0x00733ce0
bool TGParagraph::WriteCursorTimer( TGStream *stream ) {
	stream->WriteID( cursorTimer != NULL ? cursorTimer->GetObjID() : 0 );
	return true;
}

/*
================
TGParagraph::ReadCursorTimer

Keeps the ID in the pointer until PostFixUpCursorTimer.
================
*/
// FUNCTION: STBC 0x00733d10
bool TGParagraph::ReadCursorTimer( TGStream *stream ) {
	cursorTimer = (TGTimer *)stream->ReadID();
	return true;
}

/*
================
TGParagraph::FixUpCursorTimer

Fails until the timer the stream named is loaded.
================
*/
// FUNCTION: STBC 0x00733d30
bool TGParagraph::FixUpCursorTimer( void ) {
	if ( cursorTimer != NULL && TGObject::GetTGObjectPtr( (long)cursorTimer ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGParagraph::PostFixUpCursorTimer
================
*/
// FUNCTION: STBC 0x00733d50
void TGParagraph::PostFixUpCursorTimer( void ) {
	cursorTimer = (TGTimer *)TGObject::GetSavedObject( (long)cursorTimer );
}

/*
================
TGParagraph::FixUpPointers

Fails until the font the stream named is registered.
================
*/
// FUNCTION: STBC 0x00733d70
bool TGParagraph::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	FontRecord *record = (FontRecord *)fontGroup;
	return g_kFontManager.GetFontGroup( record->name, record->size ) != NULL ? true : false;
}

/*
================
TGParagraph::PostFixUp

Replaces the font record with the font group it names.
================
*/
// FUNCTION: STBC 0x00733da0
void TGParagraph::PostFixUp( void ) {
	TGPane::PostFixUp();
	FontRecord *record = (FontRecord *)fontGroup;
	// BUG: without a name the record stays in fontGroup after it is freed,
	// and the group found takes no reference.
	if ( record->name != NULL ) {
		fontGroup = g_kFontManager.GetFontGroup( record->name, record->size );
		delete [] record->name;
	}
	free( record );
}

/*
================
TGParagraph::ClearCursor

Forgets the cursor timer and poly without deleting them.
================
*/
// FUNCTION: STBC 0x00733de0
void TGParagraph::ClearCursor( void ) {
	cursorTimer = NULL;
	cursorPoly = NULL;
}

/*
================
TGParagraph_GetCString

The paragraph's string as a Python string; the paragraph is the shadow
instance in the first argument.
================
*/
// FUNCTION: STBC 0x00733df0
PyObject *TGParagraph_GetCString( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;
	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		TGParagraph *paragraph = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&paragraph, "TGParagraph" );
		if ( paragraph != NULL ) {
			char *string = paragraph->GetCString();
			if ( string != NULL ) {
				result = PyString_FromString( string );
			} else {
				result = PyString_FromString( "" );
			}
			// The new string already holds a reference: this one is never released.
			Py_INCREF( result );
			delete[] string;
		}
	}
	return result;
}

// DATA: STBC 0x0099d83c
TGTimer *TGParagraph::cursorTimer;

// DATA: STBC 0x0099d838
TGIconPoly *TGParagraph::cursorPoly;

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00731b30 _$E461
// FUNCTION: STBC 0x00731b40 _$E457
// FUNCTION: STBC 0x00731b50 _$E460
// FUNCTION: STBC 0x00731b60 _$E458
// DATA: STBC 0x0099d730
TGEventHandlerTable TGParagraph::classHandlers;

